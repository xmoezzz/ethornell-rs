use ethornell_script::{
    BpInstruction, BpOpcode, BpOperand, BpProgram,
    calls::{known_call_arg_count, known_call_returns_value, known_call_stack_output_count},
    native_abi,
};
use native_thread::{
    CProcWaitTimingExLayout32, CProcWaitWndMsgLayout32, CProcedure, CProcedureLayout32, CThread,
    CThreadLayout32, InstalledCProcedure,
};
use std::borrow::Cow;
use std::collections::{BTreeMap, HashMap, VecDeque};
use std::sync::{
    Arc, Mutex, OnceLock,
    atomic::{AtomicU64, Ordering},
};

mod async_program;
mod child_thread;
pub mod cpu_record;
mod debug;
mod extended_opcodes;
mod host_request;
mod input;
mod local_window;
pub use local_window::VmMemory;
mod logic;
pub mod native_call;
pub mod native_input;
pub mod native_motion;
pub mod native_ownership;
pub mod native_selection;
mod native_strings;
pub mod native_thread;
mod procedure_class_map;
mod profile;
mod program_thread;
mod records;
mod resource_io;
mod scenario;
mod sentence_table;
mod scheduler;
mod system80_state;
mod system81_state;
pub mod target_thread;
mod target_heap;
mod time;
mod user_data;
mod user_dialog;

pub use host_request::{
    HostMemoryWrite, HostRequest, HostResponse, VmResourceBudget, VmResourceUsage,
};
pub use native_call::{
    NativeCallFrame, NativeImplementationLevel, NativeMessageProcedureClass,
    NativeMessageProcedureConfig, NativeOpcode, NativeOpcodeSpec, NativeParameterSpec,
    NativeRecoveryLevel, NativeSchedulingEffect, display_name as native_display_name,
    documented_opcode, implementation_level, is_strictly_supported, recovery_level,
    scheduling_effect,
};

const ADDRESS_MASK: u32 = 0x01ff_ffff;
const AUX_MEMORY_TAG_BASE: u32 = 0x20;
const AUX_MEMORY_SEGMENT_SIZE: u32 = ADDRESS_MASK + 1;
const LOCAL_MEMORY_BASE: u32 = 0x0080_0000;
/// Segment 2 of sub_48E8C0: the thread's data region (frames). Opcode 0x04
/// pushes `(base - offset) | FRAME_SEGMENT`.
pub(crate) const FRAME_SEGMENT: u32 = 0x0800_0000;
/// Segment 1: the thread's code region (BP modules appended, string
/// literals and function addresses point into it).
pub(crate) const CODE_SEGMENT: u32 = 0x0400_0000;
/// Where the code region sits in the thread's local window (after the data
/// region), and its largest size.
const CODE_WINDOW_OFFSET: u32 = 0x0010_0000;
const CODE_WINDOW_SIZE: u32 = HEAP_OFFSET_BASE - CODE_WINDOW_OFFSET;
const HEAP_OFFSET_BASE: u32 = 0x0020_0000;
const HEAP_MEMORY_BASE: usize = (LOCAL_MEMORY_BASE + HEAP_OFFSET_BASE) as usize;
const INITIAL_MEMORY_SIZE: usize = 16 * 1024 * 1024;
const MAX_MEMORY_SIZE: usize = 256 * 1024 * 1024;
const OPERAND_STACK_CAPACITY: usize = 4096;

/// Where an operand found by `Vm::operand_from_top` is held.
#[derive(Debug, Clone, Copy)]
enum Operand {
    /// `stack[index]`.
    Stack(usize),
    /// `operand_slots[slot]`, pushed before the ring last wrapped.
    Wrapped(usize),
}

/// Target per-CThread interpreter quantum recovered from `sub_48CD70`.
/// The native scheduler executes at most 0x100000 BP instructions for one
/// CThread before continuing with the next scheduler entry.
pub const TARGET_COOPERATIVE_QUANTUM_STEPS: usize = 0x100000;
pub const DEFAULT_COOPERATIVE_WATCHDOG_STEPS: usize = TARGET_COOPERATIVE_QUANTUM_STEPS;
static NEXT_VM_TRACE_ID: AtomicU64 = AtomicU64::new(1);

pub type VmResult<T> = std::result::Result<T, VmError>;
pub use ethornell_script::native_abi::NativeAbiProfile;

#[derive(Debug, thiserror::Error)]
pub enum VmError {
    #[error("stack underflow")]
    StackUnderflow,
    #[error("unsupported instruction: 0x{0:02x}")]
    UnsupportedInstruction(u8),
    #[error("unknown dispatch group=0x{group:02x} id=0x{id:02x}")]
    UnknownDispatch { group: u8, id: u16 },
    #[error("memory access out of bounds addr=0x{addr:08x} size={size}")]
    MemoryOutOfBounds { addr: u32, size: usize },
    #[error("{0}")]
    Runtime(String),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Int(i32),
    Str(String),
    Ptr(u32),
    Func { program_index: usize, offset: u32 },
    Program(Arc<BpProgram>),
    None,
}

impl Value {
    pub fn as_i32(&self) -> i32 {
        match self {
            Value::Int(v) => *v,
            Value::Ptr(v) => *v as i32,
            Value::Func { offset, .. } => *offset as i32,
            Value::Str(_) | Value::Program(_) | Value::None => 0,
        }
    }
}

/// _ismbblead for code page 932.
fn is_sjis_lead_byte(byte: u8) -> bool {
    matches!(byte, 0x81..=0x9F | 0xE0..=0xFC)
}

#[derive(Debug, Clone)]
pub struct VmRunOptions {
    pub max_steps: usize,
    pub trace: bool,
    pub fail_on_stub: bool,
    pub collect_diagnostics: bool,
}

impl Default for VmRunOptions {
    fn default() -> Self {
        Self {
            max_steps: 10_000,
            trace: false,
            fail_on_stub: false,
            collect_diagnostics: true,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VmStopReason {
    /// The current program has no more executable instructions or requested a
    /// global scheduler halt.
    Completed,
    /// The BP coroutine explicitly executed Sys80_5F or the host requested a
    /// frame boundary.
    Yielded,
    /// Sys80_5E activated another coroutine and ended the caller's slice.
    SwitchedThread,
    /// Sys80_6B selected a replacement bootstrap and ended this scheduler pass.
    SchedulerPassEnded,
    /// Native thread status 6 / Sys80_6A terminated the active interpreter.
    InterpreterTerminated,
    /// The coroutine is suspended until input becomes available.
    WaitingForInput,
    /// The coroutine is suspended until either its deadline expires or an
    /// enabled input class becomes active.
    WaitingForInputOrTime,
    /// The coroutine is suspended by a timing procedure.
    WaitingForTime,
    /// The coroutine is suspended by a native CProcedure such as graph or
    /// sound playback.
    WaitingForProcedure,
    /// A host-owned request has not been completed. No placeholder result has
    /// been pushed; the embedding worker remains free to service lifecycle IO.
    WaitingForHost,
    /// The target per-thread 0x100000 instruction quantum was exhausted.
    QuantumExhausted,
    /// A host-configured budget smaller than the target quantum was exhausted.
    WatchdogExceeded,
    UnknownOpcode,
    UnknownDispatch,
    Error,
}

impl VmStopReason {
    pub fn is_fatal(self) -> bool {
        matches!(
            self,
            Self::UnknownOpcode | Self::UnknownDispatch | Self::Error
        )
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum SchedulerSignal {
    Yield,
    SwitchThread,
    EndPass,
    TerminateInterpreter,
}

impl SchedulerSignal {
    fn stop_reason(self) -> VmStopReason {
        match self {
            Self::Yield => VmStopReason::Yielded,
            Self::SwitchThread => VmStopReason::SwitchedThread,
            Self::EndPass => VmStopReason::SchedulerPassEnded,
            Self::TerminateInterpreter => VmStopReason::InterpreterTerminated,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VmProcedureReport {
    WaitTiming {
        source_opcode: NativeOpcode,
        native_layout: CProcedureLayout32,
        duration_ms: i32,
    },
    WaitTimingEx {
        source_opcode: NativeOpcode,
        native_layout: CProcWaitTimingExLayout32,
        duration_ms: i32,
        input_enabled: bool,
        input_scope: i32,
    },
    WaitWndMsg {
        source_opcode: NativeOpcode,
        native_layout: CProcWaitWndMsgLayout32,
        message_id: i32,
        registered_after_serial: i32,
    },
    DspMsg {
        source_opcode: NativeOpcode,
        class: NativeMessageProcedureClass,
        initial_deadline_tick: u32,
        reveal_duration_ms: i32,
        auto_deadline_tick: Option<u32>,
        input_scope: i32,
        completion_control: i32,
        end_wait_policy: i32,
        allow_high_bit_input: bool,
        allow_auxiliary_input: bool,
        input_forces_completion: bool,
    },
    LoadSound {
        source_opcode: NativeOpcode,
        terminal_status: i32,
    },
    HostCompleted {
        source_opcode: NativeOpcode,
        target_class_name: &'static str,
        terminal_status: i32,
        outputs: [i32; 2],
        output_count: u8,
    },
    Graph {
        source_opcode: NativeOpcode,
        target_class_name: &'static str,
        mode: &'static str,
        native_layout: CProcedureLayout32,
    },
    Exclusion {
        source_opcode: NativeOpcode,
        section_id: u32,
        native_layout: CProcedureLayout32,
    },
    Unrecovered {
        source_opcode: NativeOpcode,
        target_class_name: &'static str,
        native_layout: CProcedureLayout32,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct VmThreadReport {
    /// Complete target-offset field image. Pointer fields are portable audit
    /// handles, never host addresses.
    pub native_layout: CThreadLayout32,
    pub root_thread_id: Option<i32>,
    pub thread_id: i32,
    pub next_thread_id: Option<i32>,
    pub status_flags: i32,
    pub operand_index: u32,
    pub current_opcode_ip: u32,
    pub instruction_ip: u32,
    pub frame_base: u32,
    pub deadline_tick: u32,
    pub current_procedure_class: Option<String>,
    pub current_procedure_opcode: Option<NativeOpcode>,
    pub current_procedure: Option<VmProcedureReport>,
}

#[derive(Debug, Clone)]
pub struct VmRunReport {
    pub steps: usize,
    pub pc: usize,
    pub offset: Option<u32>,
    pub program: String,
    pub stop_reason: VmStopReason,
    pub thread: VmThreadReport,
    pub calls: BTreeMap<String, usize>,
    pub stubs: BTreeMap<String, usize>,
    pub recent_trace: Vec<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
/// Sys80:F0 (sub_472510, dialog 108/122): install-folder dialog with the
/// folder field, Browse (which appends `default_subfolder` to the chosen
/// folder) and two check boxes.
pub struct SystemInputDialogRequest {
    pub initial_text: String,
    pub option_value1: i32,
    pub option_value2: i32,
    pub default_subfolder: String,
    pub caption: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SystemInputDialogResponse {
    pub text: String,
    pub option_value1: i32,
    pub option_value2: i32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct InstallerDialogRequest {
    pub text1: String,
    pub text2: String,
    pub text3: String,
    pub text4: String,
    pub mode1: i32,
    pub mode2: i32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct InstallerWorkflowRequest {
    pub root: String,
    pub optional_directories: Vec<String>,
    pub required_directories: Vec<String>,
    pub source_files: Vec<String>,
    pub destination_files: Vec<String>,
    pub vendor: String,
    pub product: String,
    pub uninstall_source: String,
    pub prompt: String,
    pub mode: i32,
    pub flags: i32,
    pub metadata: Vec<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct InstallationProcedureRequest {
    pub root: String,
    pub optional_directories: Vec<String>,
    pub required_directories: Vec<String>,
    pub descriptor_values: Vec<u32>,
    pub source_files: Vec<String>,
    pub destination_files: Vec<String>,
    pub mode: i32,
    pub text1: String,
    pub text2: String,
    pub text3: String,
    pub text4: String,
    pub text5: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ShortcutInstallerWorkflowRequest {
    pub secondary_shortcut_name: String,
    pub program_group: String,
    pub target_root: String,
    pub primary_target: String,
    pub primary_shortcut_name: String,
    pub secondary_target: String,
    pub create_program_group: bool,
    pub create_desktop: bool,
}

pub trait SysApi {
    /// Legacy desktop System81 helpers may use std::fs as a fallback. Sandboxed
    /// hosts MUST return false so denied reads cannot bypass their IO policy.
    /// This does not replace checks in all host filesystem/installer methods.
    fn allow_direct_filesystem_fallback(&self) -> bool {
        true
    }

    /// Policy-aware writeability probe used instead of a direct temporary-file
    /// write when direct filesystem fallbacks are disabled. Never expand roots.
    fn probe_directory_writable(&mut self, _path: &str) -> bool {
        false
    }

    /// Opt in to pre-dispatch interception. Desktop hosts retain synchronous
    /// behavior by default. Android must explicitly reject unsupported modal
    /// calls rather than returning None and falling back to desktop behavior.
    fn host_requests_enabled(&self) -> bool {
        false
    }
    /// Opt-in numeric diagnostic only; never affects execution or guest state.
    fn input_gate_diagnostics_enabled(&self)->bool {false}
    fn record_input_gate(&mut self,_values:[u32;9]) {}

    /// Some(nonzero id) transfers this call to a VM continuation; None leaves
    /// the original desktop implementation in charge. IDs must be unique for
    /// the entire session, including cancelled requests and child threads.
    /// Never block here waiting for UI. The host owns queueing the request.
    fn begin_host_request(&mut self, _request: &HostRequest) -> VmResult<Option<u64>> {
        Ok(None)
    }

    /// Identifier supplied by the game's native executable (Sys80:E8).
    fn game_id(&self) -> &str {
        "Tayutama2TV"
    }

    fn call_sys(&mut self, call: &mut NativeCallFrame) -> VmResult<Value>;

    /// Keep the Microsoft CRT rand() state shared with host-native subsystems
    /// that use the same process-global CRT stream (notably CProcShakeScreen).
    /// The VM keeps its own target-compatible fallback when the host does not
    /// provide shared state.
    fn seed_native_crt_rng(&mut self, _seed: u32) {}

    fn next_native_crt_rand(&mut self) -> Option<i32> {
        None
    }

    fn observe_dispatch(&mut self, _opcode: NativeOpcode) {}

    fn take_runtime_stub(&mut self) -> bool {
        false
    }

    /// Store the raw Sys80:06 flag of the target frame-time accumulator; a
    /// non-zero flag also resets every counter, matching sub_401600.
    fn set_performance_profiling(&mut self, _enabled: i32) {}

    /// Read one target performance metric selected by Sys80:0x07.
    fn read_performance_metric(&mut self, _selector: i32) -> i32 {
        0
    }

    /// Engine clock stored after the last successful Present (sub_460460,
    /// dword_565ED8).
    fn presentation_state(&mut self) -> i32 {
        0
    }

    /// Target 64-byte hardware/capability cache copied by Sys80:0x0A.
    fn graphics_capability_record(&mut self) -> [u32; 16] {
        default_graphics_capability_record()
    }

    /// Object-manager cache budget queried by Sys80:0x0B (sub_46FB30).
    fn graphics_memory_metric(&mut self) -> i32 {
        0
    }

    fn register_graphic_resource(
        &mut self,
        _namespace: i32,
        _resource_id: i32,
        _path: &str,
    ) -> bool {
        false
    }

    fn post_queued_event(&mut self, _code: i32, _parameter: i32) {}

    fn poll_queued_event(&mut self) -> Option<[i32; 3]> {
        None
    }

    fn dispatch_object_event(
        &mut self,
        _object: i32,
        _count: i32,
        _descriptor: &[Value],
    ) -> VmResult<()> {
        Ok(())
    }

    fn load_file_bytes(&mut self, _archive: &str, _file: &str) -> Option<Vec<u8>> {
        None
    }

    /// Bytes of a file on disk only, no archive fallback; `path` is a
    /// resolved BGI path (root + name, or absolute).
    fn read_disk_file(&mut self, path: &str) -> Option<Vec<u8>> {
        self.load_file_bytes("", path)
    }

    /// Whether `path` exists on disk as a non-directory.
    fn disk_file_exists(&mut self, path: &str) -> bool {
        self.file_exists("", path)
    }

    /// Undecoded bytes of `entry` in the archive file `archive` (relative to
    /// the primary root); no other archive is searched.
    fn read_archive_entry_raw(&mut self, archive: &str, entry: &str) -> Option<Vec<u8>> {
        self.load_file_bytes(archive, entry)
    }

    /// Whether the archive file `archive` holds `entry`.
    fn archive_entry_exists(&mut self, archive: &str, entry: &str) -> bool {
        self.file_exists(archive, entry)
    }

    /// sub_465F80 once a media root exists: MessageBox(title, text,
    /// OK|CANCEL); true retries, false means the player confirmed quitting.
    fn media_prompt(&mut self, _title: &str, _text: &str) -> bool {
        false
    }

    fn file_exists(&mut self, archive: &str, file: &str) -> bool {
        self.load_file_bytes(archive, file).is_some()
    }

    fn file_size(&mut self, archive: &str, file: &str) -> i32 {
        self.load_file_bytes(archive, file)
            .map(|bytes| bytes.len() as i32)
            .unwrap_or(-1)
    }

    /// Process-global primary filesystem root initialized by target
    /// sub_465000 before the bootstrap interpreter starts.
    fn primary_resource_root(&mut self) -> Option<String> {
        None
    }

    /// Validate a native filesystem directory used as one of BGI's process
    /// global roots. The portable default rejects it rather than inventing a
    /// writable location.
    fn directory_exists(&mut self, _path: &str) -> bool {
        false
    }

    /// Portable host path for Sys80:0x3A mode 0..=5. The target writes the
    /// selected Win32 special-folder path into a caller buffer.
    fn special_folder_path(&mut self, _mode: i32) -> Option<String> {
        None
    }

    /// Host file chooser for Sys80:0x3B. `Ok(Some(path))` is native success,
    /// `Ok(None)` is user cancellation, and `Err(status)` preserves a target
    /// validation/status code such as 4 or 7.
    fn open_file_dialog(
        &mut self,
        _initial_dir: &str,
        _description: &str,
        _extension: &str,
        _title: &str,
        _mode: i32,
    ) -> Result<Option<String>, i32> {
        Ok(None)
    }

    /// Multi-filter file chooser used by Sys81:0x38. Status follows the
    /// target helper: zero on selection, -1 on cancellation, 4 for invalid
    /// mode and 7 when no filters are supplied.
    fn open_resource_file_dialog(
        &mut self,
        _mode: i32,
        _title: &str,
        _default_name: &str,
        _filters: &[(String, String)],
    ) -> Result<Option<String>, i32> {
        Ok(None)
    }

    /// Modal resource-list acknowledgement used by Sys81:0x3B.
    fn show_resource_list_dialog(&mut self, _title: &str, _pattern: &str) -> bool {
        false
    }

    /// Sys80:3F (sub_4652D0): the roots of the removable and CD-ROM drives
    /// (GetDriveTypeA 2/5), each with its trailing separator, in drive order.
    fn removable_drive_roots(&mut self) -> Vec<String> {
        Vec::new()
    }

    /// GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES: a file or a
    /// directory.
    fn path_exists(&mut self, _path: &str) -> bool {
        false
    }

    /// MessageBox(caption, text, OK|CANCEL), and on Cancel the YES|NO quit
    /// question (sub_466D00, sub_4652D0). True asks the caller to check
    /// again; false means the player confirmed quitting (the caller returns
    /// 0, nothing exits) or no dialog can be shown.
    fn retry_or_quit_prompt(&mut self, _caption: &str, _text: &str) -> bool {
        false
    }

    /// Sleep(ms) on the script thread.
    fn sleep_ms(&mut self, _ms: u32) {}

    /// Final IO boundary: enforce the host's user-write root policy, commit via
    /// same-filesystem temporary file + flush/sync + atomic replacement, and
    /// return false on any failure without destroying an existing save. Read,
    /// enumeration, deletion, rename (both ends), root selection and installer
    /// methods require equivalent policy checks; this hook alone is no sandbox.
    fn write_file_bytes(&mut self, _path: &str, _bytes: &[u8]) -> bool {
        false
    }

    fn read_user_file_bytes(&mut self, _path: &str) -> Option<Vec<u8>> {
        None
    }

    fn enumerate_user_files(
        &mut self,
        _pattern: &str,
        _recursive: bool,
        _max_count: usize,
    ) -> Vec<String> {
        Vec::new()
    }

    fn enumerate_user_directories(&mut self, _pattern: &str, _max_count: usize) -> Vec<String> {
        Vec::new()
    }

    /// Returns the current dropped-file path without consuming it. The target
    /// buffer remains populated until file-drop configuration is changed or a
    /// later WM_DROPFILES replaces it.
    fn take_dropped_file(&mut self) -> Option<String> {
        None
    }

    /// Installs the target zero-terminated fullscreen-toggle descriptor list.
    fn configure_fullscreen_hotkeys(&mut self, _enabled: bool, _descriptors: &[i32]) {}

    /// GetWindowRect(hWndParent) left/top, saved into BGI.gdb by Sys80:81.
    fn window_outer_position(&mut self) -> (i32, i32) {
        (0, 0)
    }

    /// sub_46FE90 / sub_461B40: a stored window position stays when at
    /// least 1/8 of the window would be on some monitor, else the window is
    /// centred on its monitor ((0, 0) in full screen).
    fn restore_window_position(&mut self, x: i32, y: i32) -> (i32, i32) {
        (x, y)
    }

    /// Sys80:6B (sub_465650) with interpreter status 5: restart the engine
    /// from `archive`:`file` with `root` as the primary root (`None`: the
    /// startup default, sub_464F70).
    fn select_bootstrap(&mut self, _root: Option<&str>, _archive: &str, _file: &str) {}

    /// Compares the host desktop aspect with the configured game/base aspect.
    fn display_aspect_mismatch(&mut self) -> bool {
        false
    }

    fn registered_object_value(&mut self, _object: i32) -> Option<i32> {
        None
    }

    fn set_registered_object_value(&mut self, _object: i32, _value: i32) -> bool {
        false
    }

    fn set_system_mode_flag(&mut self, mode: i32) -> bool {
        (0..=1).contains(&mode)
    }

    fn host_user_name(&mut self) -> String {
        String::new()
    }

    fn host_computer_name(&mut self) -> String {
        String::new()
    }

    fn keyboard_state(&mut self) -> [u8; 256] {
        [0; 256]
    }

    /// Sys80:0x0F mirrors the target WM_ACTIVATE/WM_NCACTIVATE latch.
    fn window_active(&mut self) -> bool {
        true
    }

    /// The host's monotonic clock in nanoseconds (Sys80:05), or `None` to
    /// derive it from the engine clock (deterministic replays).
    fn performance_counter_ns(&mut self) -> Option<u64> {
        None
    }

    /// D3DADAPTER_IDENTIFIER9 Description and DriverVersion words (product,
    /// version, subversion, build); empty when the host has none.
    fn display_adapter_identifier(&mut self) -> (String, [u16; 4]) {
        (String::new(), [0; 4])
    }

    /// GetVersionExA: major, minor, build, platform id and szCSDVersion.
    /// Hosts without Windows report the zeroed structure.
    fn host_os_version(&mut self) -> ([u32; 4], String) {
        ([0; 4], String::new())
    }

    /// The desktop display mode (GetAdapterDisplayMode, dword_5177C0/C4).
    fn desktop_display_size(&mut self) -> (u32, u32) {
        (0, 0)
    }

    /// dword_503E40[index]: the logical point of the last press of button
    /// slot `index` (0 left, 1 right, 2 middle, 3 X1, 4 X2), (-1, -1) before
    /// the first one.
    fn pointer_position(&mut self, _index: i32) -> Option<(i32, i32)> {
        None
    }

    fn runtime_command_line(&mut self) -> String {
        String::new()
    }

    fn runtime_screen_dimensions(&mut self) -> (i32, i32) {
        (1280, 720)
    }

    fn configure_screen_size(&mut self, _width: i32, _height: i32) {}

    fn register_display_mode(&mut self, _index: usize, _width: i32, _height: i32) {}

    fn set_window_monitor_adapter_mode(&mut self, mode: i32) -> bool {
        (0..=1).contains(&mode)
    }

    /// sub_45EDE0 (Sys81:61).
    fn fullscreen_scale_mode(&mut self) -> i32 {
        0
    }

    fn set_config_input_mode(&mut self, mode: i32) -> bool {
        (0..=2).contains(&mode)
    }

    fn set_shader_effect_enabled(&mut self, _enabled: bool) -> bool {
        true
    }

    /// Total and available physical memory in bytes for Sys80:0x0D.
    fn host_physical_memory_bytes(&mut self) -> (u64, u64) {
        portable_physical_memory_bytes()
    }

    /// Total and available physical memory in MiB for Sys81:0x0D.
    fn host_physical_memory_mb(&mut self) -> (u32, u32) {
        let (total, available) = self.host_physical_memory_bytes();
        (
            (total >> 20).min(u64::from(u32::MAX)) as u32,
            (available >> 20).min(u64::from(u32::MAX)) as u32,
        )
    }

    /// Win32 IsIconic-compatible minimized-window query for Sys81:0x0F.
    fn window_minimized(&mut self) -> bool {
        false
    }

    /// Portable injection hook for Sys81:0x1E synthetic mouse clicks.
    fn inject_mouse_click(&mut self, _button_code: i32) -> bool {
        false
    }

    /// dword_507690 (Sys81:1F): the extra input bits CProcDspMsg and
    /// CProcedure accept besides 0x80000181.
    fn set_message_auxiliary_input_mask(&mut self, _mask: i32) {}

    /// Host touch-window registration used by Sys81:0x18.
    fn register_touch_input(&mut self, _enabled: bool) -> bool {
        false
    }

    /// Host directory chooser used by Sys81:0x3A.
    fn browse_folder(&mut self, _title: &str, _root: i32) -> Option<String> {
        None
    }

    /// Process launch contract used by Sys81:0xE0. The portable default is an
    /// explicit unavailable result rather than a fabricated success.
    /// Sys81:E0 (sub_472A10 with a8 = 1): CreateProcessA, then wait for
    /// the process while pumping window messages, the main window hidden
    /// meanwhile when `hide_main_window`. `None` when it cannot start,
    /// else the exit code.
    fn launch_process_and_wait(
        &mut self,
        _current_directory: &str,
        _command_line: &str,
        _show_window: bool,
        _hide_main_window: bool,
    ) -> Option<u32> {
        None
    }

    /// Host validation/creation hook for the Win32 special-folder operation
    /// behind Sys81:0xF7.
    fn validate_or_create_user_path(&mut self, _path: &str, _name: &str, _mode: i32) -> bool {
        false
    }

    fn set_window_position_override(&mut self, _value: i32) {}

    fn set_pause_on_deactivate(&mut self, _enabled: bool) {}

    fn set_print_screen_hotkeys_enabled(&mut self, _enabled: bool) {}

    fn pixel_shader_version(&mut self) -> u16 {
        0
    }

    /// CreateMutexA(NULL, TRUE, name): a token for the new mutex, `None`
    /// when it already exists anywhere on the host (ERROR_ALREADY_EXISTS).
    fn create_named_mutex(&mut self, _name: &str) -> Option<u64> {
        Some(0)
    }

    fn release_named_mutex(&mut self, _token: u64) {}

    /// Whether d3dx9_43.dll's D3DXCreateEffect is available (Sys81:6E).
    fn d3dx_effects_available(&mut self) -> bool {
        false
    }

    fn delete_file(&mut self, _root: &str, _file: &str) -> bool {
        false
    }

    /// Sys80:36: whether loose-file reads fall back to the Sys80:37 paths.
    fn set_loose_search_fallback(&mut self, _enabled: bool) {}

    /// Sys80:37: prepend a loose-file fallback directory.
    fn prepend_loose_search_path(&mut self, _directory: &str) {}

    /// Sys80:38: register a DCArchiveComplex name over member archives.
    fn register_composite_archive(&mut self, _name: &str, _members: &[String]) {}

    fn user_data_root(&mut self, _kind: i32) -> Option<String> {
        None
    }

    fn save_global_user_data(&mut self) -> bool {
        false
    }

    fn load_program(&mut self, archive: &str, file: &str) -> Option<BpProgram> {
        let script_name = Some(format!("{archive}:{file}"));
        self.load_file_bytes(archive, file)
            .map(|bytes| ethornell_script::parse_bp_program(script_name, &bytes))
    }

    fn load_program_ex(
        &mut self,
        archive: &str,
        file: &str,
        _params: &[Value],
    ) -> Option<BpProgram> {
        self.load_program(archive, file)
    }

    fn free_program(&mut self, _program: Value) {}

    /// Sys80:0x13 returns the target window/input-message serial.
    fn input_message_serial(&mut self) -> i32 {
        0
    }

    /// The newest message `message_id` delivered after `registered_after_serial`
    /// as `(wParam, lParam)`. The WndProc (sub_499930 -> sub_49AAE0) signals
    /// every waiter for that number and overwrites earlier payloads, so the
    /// latest message wins and polling must not consume it for other waiters.
    fn poll_window_message(
        &mut self,
        _message_id: i32,
        _registered_after_serial: i32,
    ) -> Option<(i32, i32)> {
        None
    }

    /// Sys80:0x16 samples configured descriptors and arms the target poll latch.
    fn sample_configured_input(&mut self) {}

    /// Sys80:0x10 stores dword_506A44 and clears the four transient fields in
    /// every six-DWORD input-state record. The +0x0C accumulator is preserved.
    fn reset_input_configuration(&mut self, _value: i32) {}

    /// Sys80:0x18 registers one packed input scope in both target lists, then
    /// performs one stateful input query and discards only its return value.
    fn register_input_scope(&mut self, _scope: i32) {}

    /// Sys80:0x19 queries and then removes one packed scope from both lists.
    fn query_and_unregister_input_scope(&mut self, _scope: i32) {}

    /// `CProcDspMsg` constructor (`sub_432BC0`) registers its +0x78 member
    /// exactly as stored.  That member is either the special literal `2` or
    /// an already-packed `(scope << 16) | 0xFFFF`; unlike Sys80:18 it must
    /// never be shifted/packed a second time.  The constructor also performs
    /// one immediate stateful input query after both registrations.
    fn register_message_input_scope(&mut self, _input_scope: i32) {}

    /// `CProcDspMsg` destructor (`sub_432D00`) removes the exact +0x78 member
    /// from both native scope lists.  This is deliberately not Sys80:19: the
    /// target destructor does not perform a final input query.
    fn unregister_message_input_scope(&mut self, _input_scope: i32) {}

    /// Stamp of the newest host window message; WaitWndMsg only accepts
    /// messages delivered after the stamp it registered with.
    fn window_message_sequence(&mut self) -> i32 {
        self.input_message_serial()
    }

    /// sub_46D560 (Sys80:11): whether the logical descriptor is physically
    /// held, after the Sys80:1E mapping and the active-window gate. It does
    /// not consume any input event.
    fn query_key_held(&mut self, _descriptor: i32) -> bool {
        false
    }

    /// Sys81:10 (sub_46DA40): replace input record +0x14 of one descriptor
    /// and return the previous value.
    fn swap_input_repeat_flag(&mut self, _descriptor: i32, _value: i32) -> i32 {
        0
    }

    /// Sys81:14 (sub_46D540): dword_5667EC, background key polling.
    fn set_background_key_polling(&mut self, _enabled: i32) {}

    fn read_input_state(&mut self, _descriptor: i32) -> i32 {
        0
    }

    /// Non-destructive snapshot used by Sys80:0x12. The target reads the
    /// descriptor accumulator array directly and does not consume an input
    /// event while summing a zero-terminated descriptor list.
    fn peek_input_state(&mut self, descriptor: i32) -> i32 {
        self.read_input_state(descriptor)
    }

    fn query_input_event_bits(&mut self, _scope: i32) -> i32 {
        0
    }

    /// CProcDspMsg stores the exact target scope: special value `2`, or an
    /// already packed `(scope << 16) | 0xFFFF`. Keep this separate from
    /// Sys80:1A, whose BP argument is still an unpacked scope.
    fn query_message_input_event_bits(&mut self, input_scope: i32) -> i32 {
        if input_scope == 2 {
            self.query_input_event_bits(0)
        } else {
            self.query_input_event_bits(input_scope >> 16)
        }
    }

    fn register_input_class_descriptors(&mut self, _class_mask: i32, _descriptors: &[i32]) {}

    /// Sys80:0x17 configured-input poll. The default implementation delegates
    /// to the edge-triggered event query and never treats configuration state
    /// as an input event.
    fn query_configured_input_gate(&mut self) -> i32 {
        self.query_input_event_bits(0)
    }

    fn query_input_descriptor_state(&mut self, _class_mask: i32) -> i32 {
        0
    }

    /// Sys80:0x1D validates one `(scope << 16) | 0xFFFF` registration and
    /// drains the selected target logical input event counter. The high bit is
    /// set on the first read while the input remains down.
    fn query_scoped_input_event(&mut self, _input_descriptor: i32, _scope: i32) -> i32 {
        0
    }

    fn set_input_master_gate(&mut self, _value: i32) {}

    fn set_input_latched_state(&mut self, _value: i32) {}

    /// One CreateProcessA attempt of sub_472A10 (no application name, so the
    /// command line names the program) with `working_directory` as current
    /// directory; on success it waits for the child, hiding the window
    /// meanwhile when `hide_window` and, with `wait_for_uninstaller`, for the
    /// uninstaller mutex. False when the program could not be started.
    fn launch_process(
        &mut self,
        _working_directory: &str,
        _command_line: &str,
        _hide_window: bool,
        _wait_for_uninstaller: bool,
    ) -> bool {
        false
    }

    /// MessageBox(text, caption, MB_OK | MB_ICONINFORMATION).
    fn message_box(&mut self, _caption: &str, _text: &str) {}

    /// Sys80:E1: the command WinMain runs (once, without waiting) after the
    /// main loop ends; `command_line` is already resolved like sub_472A10.
    fn schedule_restart(
        &mut self,
        _working_directory: &str,
        _command_line: &str,
        _error_message: Option<&str>,
    ) {
    }

    fn shell_open(&mut self, _target: &str) -> bool {
        false
    }

    fn set_uninstaller_product(&mut self, _product: &str) {}
    fn show_system_input_dialog(
        &mut self,
        _request: &SystemInputDialogRequest,
    ) -> Option<SystemInputDialogResponse> {
        None
    }

    fn show_installer_dialog(&mut self, _request: &InstallerDialogRequest) -> bool {
        false
    }

    fn run_installer_workflow(&mut self, _request: &InstallerWorkflowRequest) -> bool {
        false
    }

    fn run_installation_procedure(
        &mut self,
        _request: &InstallationProcedureRequest,
    ) -> std::result::Result<(), i32> {
        Err(-1)
    }

    fn run_shortcut_installer_workflow(
        &mut self,
        _request: &ShortcutInstallerWorkflowRequest,
    ) -> bool {
        false
    }

    fn remove_uninstall_listed_files(&mut self, _root: &str, _exclusions: &[String]) -> bool {
        false
    }

    fn append_uninstall_list_entries(&mut self, _root: &str, _entries: &[String]) -> bool {
        false
    }

    fn remove_installer_shortcuts(
        &mut self,
        _file_name: &str,
        _program_group: &str,
        _secondary_file: &str,
        _remove_group: bool,
    ) {
    }

    fn create_shortcut(
        &mut self,
        _program_group: Option<&str>,
        _shortcut_name: &str,
        _target: &str,
    ) -> bool {
        false
    }

    fn create_special_folder_shortcut(
        &mut self,
        _special_folder_mode: i32,
        _program_group: Option<&str>,
        _shortcut_name: &str,
        _target: &str,
    ) -> bool {
        false
    }

    fn read_installed_folder(&mut self, _vendor: &str, _product: &str) -> Option<String> {
        None
    }

    fn delete_installed_registry_key(&mut self, _vendor: &str, _product: &str) -> bool {
        false
    }

    fn register_file_association(
        &mut self,
        _extension: &str,
        _class_name: &str,
        _description: &str,
        _icon: &str,
        _open_command: &str,
    ) -> bool {
        false
    }

    fn launcher_mode(&mut self) -> i32 {
        0
    }

    fn take_frame_yield(&mut self) -> bool {
        false
    }
}

/// One target System92 text-fragment record produced by the 0x9C renderer
/// and drained by 0x9E. The VM serializes this portable representation to
/// the target's fixed 128-byte Shift-JIS record layout.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct System92TextFragmentRecord {
    pub text: String,
    pub x: i32,
    pub y: i32,
}

/// One target 16-byte icon record consumed by Graph90:B4 and by the common
/// renderer used after Graph90:B5 projects its 64-byte extended records.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct GraphIconRecord {
    pub x: i32,
    pub y: i32,
    pub bitmap: i32,
    pub parameter: i32,
}

pub trait GraphApi {
    /// 90:78 / 424010 copies width*height native little-endian u16 tiles.
    fn upload_map_tile_data(&mut self,_object:i32,_width:u32,_height:u32,_tiles:&[u16])->bool {false}

    fn call_graph(&mut self, call: &mut NativeCallFrame) -> VmResult<Value>;

    /// Graph90:A1 (sub_492090 -> sub_43BA30): clear a Window's text layer
    /// and draw `items` row by row across `columns` equal columns, centred
    /// in each column or at its left edge, in packed RGB `color`. Returns
    /// false when `window` is not a Window object.
    fn draw_item_selection_grid(
        &mut self,
        _window: i32,
        _items: &[String],
        _columns: i32,
        _centered: bool,
        _color: i32,
    ) -> bool {
        false
    }

    /// Returns the two DWORD globals written by target helper sub_434410
    /// when Graph92:9B is called with selector 256.
    fn system92_text_output_pair(&self) -> [i32; 2] {
        [0, 0]
    }

    /// dword_565D4C: the text colour the last layout ended with (92:9B
    /// selector 257).
    fn system92_text_colour(&self) -> i32 {
        0
    }

    /// Drains the process-global fragment table copied by target helper
    /// sub_437EB0. The producer (Graph92:9C) owns record segmentation.
    fn take_system92_text_fragment_records(&mut self) -> Vec<System92TextFragmentRecord> {
        Vec::new()
    }

    /// Whether the currently displayed native message still has unrevealed
    /// glyphs. Message progression belongs to CProcDspMsg, not Sys input
    /// queries.
    fn native_message_is_animating(&self) -> bool {
        false
    }

    /// The text auto-advance setting as it is now (Graph90:97, the target's
    /// MessageConfig_SetAutoCompletion at 0x00433510): `Some(delay_ms)` while
    /// enabled. CProcDspMsg arms and keeps its +0x5c deadline from it, so a
    /// toggle while a line waits applies to that line. `configured` is the
    /// value captured when the procedure was built; hosts that don't track
    /// the setting return it.
    fn native_message_auto_advance_delay_ms(&self, configured: Option<i32>) -> Option<i32> {
        configured
    }

    /// Whether the host still has work associated with an EXE-confirmed graph
    /// procedure installed for `opcode`. This is intentionally selector-aware
    /// even though the current renderer may share one animation registry.
    fn native_graph_procedure_is_active(&self, _opcode: NativeOpcode) -> bool {
        false
    }

    /// Base CProcedure cancellation hook for procedure families without an
    /// object-control identity. CProcShakeScreen uses this to restore the
    /// neutral screen offset before the VM releases the waiting thread.
    fn cancel_native_graph_procedure(&mut self, _opcode: NativeOpcode) {}

    /// Object-bound completion predicate used by CProcCtrlDspObj.  Keep the
    /// selector-only method as a compatibility fallback for graph procedure
    /// families whose target object has not yet been recovered.
    fn native_graph_object_procedure_is_active(
        &self,
        opcode: NativeOpcode,
        _object_id: i32,
    ) -> bool {
        self.native_graph_procedure_is_active(opcode)
    }

    /// Exact CProcedure-owned animation predicate.  Target CProcCtrlDspObj
    /// subclasses carry their own clock/progress state, so object identity is
    /// insufficient when multiple procedures target the same CDspObj.
    fn native_graph_control_procedure_is_active(
        &self,
        opcode: NativeOpcode,
        object_id: i32,
        _control_id: u64,
    ) -> bool {
        self.native_graph_object_procedure_is_active(opcode, object_id)
    }

    /// Mutable CProcCtrlDspObj poll hook. The original cooperative scheduler
    /// invokes the exact control updater from CProcedure::Tick before checking
    /// whether that procedure is still active. Hosts that do not need this
    /// distinction may keep the legacy read-only predicate.
    fn poll_native_graph_control_procedure(
        &mut self,
        opcode: NativeOpcode,
        object_id: i32,
        control_id: u64,
    ) -> bool {
        self.native_graph_control_procedure_is_active(opcode, object_id, control_id)
    }

    /// Deliver a base CProcedure cancellation to one exact graph control.
    /// This is deliberately different from CProcCtrlDspObj's +0x98 local
    /// forced-end latch: base cancellation exits 0x431F00 before the subclass
    /// updater and therefore preserves the current intermediate object state.
    fn cancel_native_graph_control_procedure(
        &mut self,
        _opcode: NativeOpcode,
        _object_id: i32,
        _control_id: u64,
    ) {
    }

    /// CProcCtrlDspObj callback code 1 (sub_4320B0): when the payload or the
    /// control's input flag is non-zero the control is asked to finish at
    /// its end values on the next update (+0x98).
    fn request_native_graph_control_stop(
        &mut self,
        _opcode: NativeOpcode,
        _object_id: i32,
        _control_id: u64,
        _payload: i32,
    ) {
    }

    /// Consume the two deferred values published by CProcCtrlDspObj::Tick
    /// (0x431F00). Value 0 is procedure progress scaled by 1000; value 1 is
    /// 0 for natural completion, 1 for an input/procedure-local forced end,
    /// and -1 for base CProcedure cancellation/abnormal termination.
    fn take_native_graph_control_procedure_completion(
        &mut self,
        _opcode: NativeOpcode,
        _object_id: i32,
        _control_id: u64,
    ) -> [i32; 2] {
        [1000, 0]
    }

    /// Poll the completed item index for the CProcSelectItem family. The
    /// target publishes the selected index twice on ordinary completion; a
    /// cancelled base procedure publishes -1 as its second value.
    fn poll_native_graph_selection(&mut self) -> Option<i32> {
        None
    }

    fn configure_background_frames(&mut self,_frames:&[i32],_alpha:i32)->VmResult<()> {
        Err(VmError::Runtime("BackD backend unavailable".into()))
    }

    fn start_native_selection(&mut self, _request: native_selection::NativeSelectionRequest) -> VmResult<u64> {
        Err(VmError::Runtime("CProcSelect backend unavailable".into()))
    }

    fn poll_native_selection(&mut self, _selection_id: u64, _callbacks: &[[i32; 2]], _cancelled: bool)
        -> Option<native_selection::NativeSelectionCompletion> {
        None
    }

    /// Reveal the current message and return whether any glyph state changed.
    fn reveal_native_message(&mut self) -> bool {
        false
    }

    /// Release host-side message-active state after the procedure completes.
    fn finish_native_message(&mut self) {}

    /// sub_469110: whether a 0xB0 font number is registered.
    fn native_font_registered(&self, _font: i32) -> bool {
        true
    }

    /// dword_565BC0, the character spacing set by 91:98.
    fn text_character_spacing(&self) -> i32 {
        0
    }

    /// The message waits at the end of its text: `sub_433E40` runs
    /// `sub_433F40`, which cycles the Graph90:98 caret frames every Graph90:99
    /// delay as a child sprite of the message window. Called on every tick of
    /// that wait with the engine clock; `finish_native_message` hides the
    /// caret again (`sub_42BAB0(window, 0, 0)`).
    fn tick_native_message_caret(&mut self, _now_tick: u32) {}

    fn register_graph_color_lut(&mut self, _id: i32, _points: [[i32; 2]; 3]) -> bool {
        false
    }

    fn remove_graph_color_lut(&mut self, _id: i32) -> bool {
        false
    }

    fn query_graph_effect_result(&self, _process: i32) -> Option<i32> {
        None
    }

    fn invoke_graph_effect_process(&mut self, _process: i32) -> GraphEffectInvocation {
        GraphEffectInvocation {
            status: 4,
            duration_ms: 0,
        }
    }

    fn query_movie_position(&self, _bitmap: i32) -> Result<i32, i32> {
        Err(4)
    }

    /// sub_407FA0 (91:F7): `None` for an invalid or empty bitmap slot; a
    /// slot without a movie or a stopped movie reports -1.
    fn query_movie_slot_position(&self, _bitmap: i32) -> Option<i32> {
        None
    }

    /// Load and validate one target BF_Movie resource. The VM owns the two
    /// caller output pointers and writes the returned handle/metadata record.
    fn load_buriko_movie_resource(
        &mut self,
        _archive: &str,
        _resource: &str,
    ) -> Result<(i32, [i32; 5]), i32> {
        Err(1)
    }

    fn release_buriko_movie_resource(&mut self, _handle: i32) -> i32 {
        3
    }

    /// Attach one shared child handle to a BF_Movie resource. Status 3 means
    /// invalid source and status 7 means the source already has a child.
    fn attach_buriko_movie_resource(&mut self, _source: i32) -> Result<i32, i32> {
        Err(3)
    }

    /// Validate the public Graph90:F6 destination/source/frame contract, then
    /// decode the frame into the destination bitmap. Status 6 is a frame that
    /// fails to decode.
    fn validate_buriko_movie_decode(
        &mut self,
        _destination: i32,
        _source: i32,
        _frame_index: i32,
    ) -> i32 {
        3
    }

    /// Return the inclusive valid rectangle stored by Graph90:89's target
    /// CDspObjWindow helper. The VM owns the BP pointer write.
    fn query_graph_window_valid_region(&self, _window: i32) -> Option<[i32; 4]> {
        None
    }

    /// Install the Graph90:98 caret bitmap table after the VM has converted
    /// the BP pointer and copied its i32 entries. `Err(bitmap)` identifies the
    /// first target-invalid bitmap handle.
    fn configure_message_caret_frames(
        &mut self,
        _table_pointer: u32,
        _frame_count: i32,
        _frames: &[i32],
    ) -> Result<(), i32> {
        Err(-1)
    }

    /// Query one target CDspObj parameter for Graph91:38 (vtable +0x60).
    /// Returns every DWORD the getter writes through the BP pointer; `input`
    /// is the DWORD already stored there (parameter 0x7FFFFFFF reads its
    /// slot index from it). Error 255 means invalid object, 5 an unsupported
    /// parameter and 254 a rejected value.
    fn query_graph91_object_property(
        &self,
        _object: i32,
        _parameter: i32,
        _input: i32,
    ) -> Result<Vec<i32>, i32> {
        Err(255)
    }

    /// Return the two-DWORD resolved object position written by Graph91:3D.
    /// The VM owns the caller's BP output pointer.
    fn query_graph91_object_composite_position(&self, _object: i32) -> Option<[i32; 2]> {
        None
    }

    /// Hit-test the current pointer against one CDspObjLandscape. The VM
    /// owns the writable two-DWORD line/column output buffer.
    fn graph91_landscape_hit_test(
        &self,
        _landscape: i32,
        _alpha_test_mode: i32,
    ) -> VmResult<Option<[i32; 2]>> {
        Ok(None)
    }

    fn configure_graph91_landscape_parts(
        &mut self,
        _landscape: i32,
        _bitmap: i32,
        _part_count: i32,
        _part_words: &[i32],
        _part_spacing: i32,
        _column_count: i32,
        _column_words: &[i32],
    ) -> VmResult<()> {
        Ok(())
    }

    /// `map` is empty when either dimension is outside 1..=256; the target
    /// reads the table only after its pillar and size checks.
    fn configure_graph91_landscape_map(
        &mut self,
        _landscape: i32,
        _width: i32,
        _height: i32,
        _map: &[i32],
    ) -> VmResult<()> {
        Ok(())
    }

    fn configure_graph91_landscape_guides(
        &mut self,
        _landscape: i32,
        _bitmap: i32,
        _guide_count: i32,
        _guide_words: &[i32],
    ) -> VmResult<()> {
        Ok(())
    }

    fn set_graph91_landscape_cell_guides(
        &mut self,
        _landscape: i32,
        _pairs: &[i32],
        _layer: i32,
        _guide: i32,
        _value: i32,
    ) -> VmResult<()> {
        Ok(())
    }

    fn query_graph91_landscape_cell_value(
        &self,
        _landscape: i32,
        _line: i32,
        _column: i32,
    ) -> VmResult<Option<i32>> {
        Ok(None)
    }

    fn cache_graph_blob(&mut self, _namespace: &str, _name: &str, _bytes: &[u8]) -> bool {
        false
    }

    /// Validate and register one target BG resource in the two-key cache used
    /// by Graph90:C6/C7.
    fn register_bg_resource_data(&mut self, _namespace: &str, _name: &str, _bytes: &[u8]) -> bool {
        false
    }

    /// Portable encoder backing for Graph90:CE. The VM owns destination and
    /// byte-count pointers and copies the returned payload into guest memory.
    fn encode_graph_bitmap(
        &mut self,
        _bitmap: i32,
        _format: i32,
        _parameter: i32,
    ) -> Result<Vec<u8>, i32> {
        Err(-1)
    }

    fn create_bitmap_from_rgb(
        &mut self,
        _bitmap: i32,
        _width: i32,
        _height: i32,
        _format: i32,
        _pixels: &[u8],
    ) -> bool {
        false
    }

    fn read_bitmap_pixels(&mut self, _bitmap: i32, _capacity: usize) -> Option<Vec<u8>> {
        None
    }

    fn set_bitmap_dimensions(&mut self, _bitmap: i32, _width: i32, _height: i32) -> bool {
        false
    }

    /// Set the target bitmap-registry DWORDs at +0x28/+0x2C. They are an
    /// auxiliary reference point, not width/height. `sub_401EF0` seeds them
    /// from CBG +0x1C/+0x1E when the CBG +0x1A presence flag is 1.
    fn set_bitmap_auxiliary_pair(&mut self, _bitmap: i32, _first: i32, _second: i32) -> bool {
        false
    }

    /// Read the target bitmap-registry auxiliary reference point at
    /// +0x28/+0x2C. Newly allocated/released slots default to -1/-1.
    fn query_bitmap_auxiliary_pair(&mut self, _bitmap: i32) -> Option<[i32; 2]> {
        None
    }

    fn query_bitmap_info(&mut self, _bitmap: i32) -> Option<BitmapInfo> {
        None
    }

    fn read_bitmap_pixel(&mut self, _bitmap: i32, _x: i32, _y: i32) -> Option<[u8; 4]> {
        None
    }

    fn call_graph_spline_control(
        &mut self,
        call: &mut NativeCallFrame,
        _points: &[[i32; 4]],
    ) -> VmResult<Value> {
        self.call_graph(call)
    }

    fn configure_graph_input_object(&mut self, _object: i32, _descriptor: GraphInputDescriptor) {}

    fn configure_graph_surface_controls(
        &mut self,
        _surface: i32,
        _descriptor: GraphInputDescriptor,
    ) {
    }

    /// Replace a window's immediate icon backing with the supplied base
    /// records. The VM owns native BP-memory conversion for both B4 and B5.
    fn draw_graph_icon_batch(&mut self, _window: i32, _records: &[GraphIconRecord]) -> bool {
        false
    }

    /// Store or clear the fixed sixteen-DWORD item-selection column layout
    /// after the VM has copied it from BP memory.
    fn set_item_selection_column_layout(
        &mut self,
        _window: i32,
        _layout: Option<[i32; 16]>,
    ) -> bool {
        false
    }

    fn collect_ruby_substitutions(&mut self, _source: &str) -> (String, i32) {
        (String::new(), 0)
    }

    fn poll_object_state(&mut self, _object: i32) -> i32 {
        0
    }

    fn poll_object_event(&mut self, _object: i32) -> i32 {
        0
    }

    fn poll_object_event_payload(&mut self, object: i32) -> (i32, i32) {
        (self.poll_object_event(object), 0)
    }

    fn poll_object_state_record(&mut self, object: i32) -> [i32; 6] {
        [self.poll_object_state(object), 0, 0, 0, 0, 0]
    }

    /// Runtime side of Graph90:BF / GraphPopIconInputEvent.
    ///
    /// `object` is a registered DCIPIcon processor handle. A valid processor
    /// owns a FIFO of three-DWORD records. One call consumes at most one
    /// record; an empty valid queue yields `[0, 0, 0]`. Target evidence:
    /// `sub_47F000 -> sub_46CC70 -> sub_448560`, with `sub_44A220` unlinking
    /// and deleting the queue head after the copy. Producers enqueue through
    /// `sub_44A1D0`; the getter itself never hit-tests or advances input.
    ///
    /// Confirmed base DCIPIcon events produced by `sub_448690` include:
    /// - `0x10000001`: raw hit-object changed; words 1/2 are group/item, or
    ///   `[-1,-1]` when nothing is hit (`sub_4495C0(...,0,0)`).
    /// - `0x10000002`: pointer-current item changed after `sub_449760` /
    ///   `sub_4497A0`; word 1 is packed `HIWORD=group, LOWORD=item` or `-1`
    ///   on leave, and word 2 is 1 iff the compact item +0x14 hover bitmap is
    ///   present.
    /// - `0x10000003`: current group navigation changed; word 1 is the group
    ///   index and word 2 is -1/1 for previous/next navigation.
    /// - `0x10000004` / `0x10000005`: current item navigation/selection
    ///   notifications produced by the target keyboard/pointer navigation
    ///   branches. Their exact word-2 flag values are preserved by the native
    ///   state machine; they are not generic mouse-button events.
    ///
    /// DCIPIconEx additionally emits `0x10000006` state transitions and
    /// `0x10000007` action-position records through `sub_44C170/sub_44C230`
    /// and `sub_44B9E0`. Implementations must never synthesize any of these
    /// records merely because Graph90:BF is polled.
    fn poll_object_event_record(&mut self, object: i32) -> [i32; 3] {
        let (event, payload) = self.poll_object_event_payload(object);
        [event, payload, 0]
    }

    fn graph_window_exists(&self, _window: i32) -> bool {
        false
    }

    fn graph_input_object_exists(&self, _object: i32) -> bool {
        false
    }

    fn graph_input_object_is_extended(&self, _object: i32) -> bool {
        false
    }

    /// Update one extended icon-input item. The target public status contract
    /// is 1=invalid object, 2=invalid group, 3=invalid item, 4=wrong object
    /// variant, and 0=success.
    fn set_graph_input_item_state(
        &mut self,
        _object: i32,
        _group: i32,
        _index: i32,
        _state: i32,
    ) -> i32 {
        1
    }

    /// Store one of the four target key-assignment records (IDs 4..=7). Each
    /// record contains exactly 24 DWORDs copied by Graph91:BF.
    fn set_graph_key_assignment(&mut self, _assignment: i32, _values: [i32; 24]) -> bool {
        false
    }

    fn graph_input_registered_state(&self, _object: i32) -> Option<i32> {
        None
    }

    fn graph_input_region_values(&self, _object: i32) -> Option<Vec<i32>> {
        None
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct GraphEffectInvocation {
    pub status: i32,
    pub duration_ms: i32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct GraphInputDescriptor {
    pub initial_group: i32,
    pub flags: [i32; 7],
    /// Compact DCIPIcon item coordinates are relative to the owning Window's
    /// valid-region left/top (sub_447C10 -> sub_42C2A0). Extended DCIPIconEx
    /// item coordinates are used directly by sub_44A900. This is parser-known
    /// descriptor-layout semantics, not an extra DWORD in guest memory.
    pub compact_uses_valid_region_origin: bool,
    /// DCIPIcon pointer processing is enabled by the base constructor. The
    /// extended 40-byte descriptor can disable it with root+0x20 != 0
    /// (`sub_44A900`: DCIPIcon+0x88 = root[8] == 0).
    pub pointer_processing_enabled: bool,
    /// Per-group behavior copied from the target descriptor. These fields are
    /// not item flags: `sub_448690` consults them before pointer-driven
    /// selection and mouse activation.
    pub groups: Vec<GraphInputGroup>,
    pub regions: Vec<GraphInputRegion>,
}

impl Default for GraphInputDescriptor {
    fn default() -> Self {
        Self {
            initial_group: 0,
            flags: [0; 7],
            compact_uses_valid_region_origin: false,
            pointer_processing_enabled: true,
            groups: Vec::new(),
            regions: Vec::new(),
        }
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct GraphInputGroup {
    /// Grid columns: compact count high word, extended group+4; zero uses item count.
    pub navigation_columns: i32,
    pub index: i32,
    /// Configure-time current item: compact group+0x08, extended group+0x0C.
    /// -1 means no current item.
    pub initial_current_item: i32,
    /// Internal group+0x0C. `sub_449A60` requires this to select the group.
    pub selection_enabled: bool,
    /// Internal group+0x10. When nonzero, pointer motion over an item may call
    /// the group current-item setter (`sub_449BC0`) and emit 0x10000004.
    pub pointer_selection_enabled: bool,
    /// Internal group+0x14. This is a held-button action reinjection flag,
    /// not a fresh-click enable flag. After normal edge sampling, `sub_448690`
    /// may OR action bit 1 from `sub_46E490()` while mouse-left remains held.
    /// The historical field name is retained for source compatibility.
    pub pointer_activation_enabled: bool,
    /// Internal group+0x18. `sub_449D60` uses equal non--1 keys to clear a
    /// current item in peer groups when this group becomes current.
    pub selection_exclusion_key: i32,
    /// DCIPIconEx source-group flags at extended group+0x3C. They are not
    /// projected into the common 52-byte group record. Extended vtable+0x48
    /// (`sub_44C6F0`) consults bit 0x02 together with item+0xC0 bit 0x20 to
    /// choose activation timing: both clear activates on MouseDown; either set
    /// defers the same action until MouseRelease while the pointer remains on
    /// the item. Base/compact DCIPIcon has no corresponding source field and
    /// stores zero here.
    pub extended_flags: i32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct GraphInputRegion {
    pub group: i32,
    pub index: i32,
    pub ordinal: i32,
    pub enabled_depth: i32,
    pub selected: bool,
    pub x: i32,
    pub y: i32,
    /// Legacy host field name. For DCIPIconEx this is source item+0x10,
    /// passed by sub_44A900 to the internal mode-5 Sprite as an origin/transform
    /// parameter; it is NOT the materialized bitmap or Virtual hit width.
    /// Compact descriptors store zero.
    pub width: i32,
    /// Legacy host field name. For DCIPIconEx this is source item+0x14, the
    /// paired mode-5 origin/transform parameter; it is NOT raster/hit height.
    /// Compact descriptors store zero.
    pub height: i32,
    /// Base/idle bitmap resource. Compact DCIPIcon item offset +0x0C;
    /// extended DCIPIconEx item offset +0x20.
    pub normal_resource: i32,
    /// Per-group current/selected bitmap resource. Compact item offset +0x10;
    /// extended item offset +0x28. This is NOT the plain mouse-hover bitmap.
    pub selected_resource: i32,
    /// Plain pointer-hover bitmap resource. Compact item offset +0x14;
    /// extended item offset +0x24. Target sub_4499F0/sub_44BA40 select this
    /// from the global pointer-hit state independently of the group selection.
    pub hover_resource: i32,
    /// DCIPIconEx-only bitmap used when pointer-hover and group selection are
    /// both active (extended item offset +0x2C). Base DCIPIcon has no separate
    /// combined slot and stores -1 here.
    pub hover_selected_resource: i32,
    /// Explicit CDspObjVirtual hit-mask bitmap at compact +0x18 / extended
    /// +0x30. Target sub_447C10/sub_44A900 resolve it and sub_41BC70 converts
    /// native formats 0/1/2/3 into a 1-bit pointer-hit mask. Sentinel -2
    /// explicitly clears/disables the mask; an unresolved resource leaves the
    /// item's intrinsic rectangular Virtual hit area unmasked.
    pub mask_resource: i32,
    /// DCIPIconEx source item +0x34. Target vtable+0x28 (`sub_44C110`)
    /// rejects pointer-hit activation for this item while it is the group's
    /// current selection when this value is nonzero. Compact DCIPIcon has no
    /// corresponding source field and stores false here.
    pub current_selection_hit_excluded: bool,
    pub flags: i32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct BitmapInfo {
    pub row_stride: u32,
    pub width: u32,
    pub height: u32,
    pub format: u32,
    pub bytes_per_pixel: u32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum UserDialogRequest {
    Input {
        title: String,
        initial: String,
        max_bytes: usize,
        numeric: bool,
    },
    TwoField {
        title: String,
        first_label: String,
        first_initial: String,
        first_max_bytes: usize,
        first_numeric: bool,
        second_label: String,
        second_initial: String,
        second_max_bytes: usize,
        second_numeric: bool,
    },
    Segmented {
        title: String,
        prompt: String,
        segment_count: usize,
        max_bytes_per_segment: usize,
        numeric: bool,
    },
    Selection {
        title: String,
        prompt: String,
        options: Vec<String>,
    },
    DateFields {
        fields: [String; 4],
        month_index: i32,
        day_index: i32,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum UserDialogResponse {
    Input(String),
    TwoField([String; 2]),
    Segmented(String),
    Selection(String),
    DateFields {
        fields: [String; 4],
        month_index: i32,
        day_index: i32,
    },
}

fn target_dialog_max_bytes(value: i32, capacity: usize) -> usize {
    let requested = usize::try_from(value.unsigned_abs()).unwrap_or(capacity);
    if requested == 0 {
        capacity
    } else {
        requested.min(capacity)
    }
}

pub trait SoundApi {
    fn call_sound(&mut self, call: &mut NativeCallFrame) -> VmResult<Value>;

    /// A0:28 bridges the target memory-backed sound block out of BP memory.
    /// The block begins with the target 64-byte descriptor and continues with
    /// the encoded/sample payload whose total length is descriptor[0] +
    /// descriptor[2].
    fn register_memory_sound(
        &mut self,
        _channel: i32,
        _block: &[u8],
        _native_start_parameter: i32,
        _decode_gain: f64,
        _playback_rate: f64,
    ) -> bool {
        false
    }

    fn query_bgm_state(&self, _channel: i32) -> Option<(i32, i32)> {
        None
    }

    /// Return the target public MCI mode mapping for A0:86. `None` means the
    /// CD-audio device is not open and the wrapper must return zero without
    /// claiming a valid output mode.
    fn query_cd_audio_mode(&self) -> Option<i32> {
        None
    }

    fn observe_user(&mut self, _call: &NativeCallFrame) {}

    fn call_user(&mut self, _call: &mut NativeCallFrame) -> VmResult<Option<Value>> {
        Ok(None)
    }

    fn current_user_text(&self) -> String {
        String::new()
    }

    /// B0:C4 enumerates these Shift-JIS font families in this order.
    /// Keep the list stable between the size query and the buffer write.
    fn user_font_families(&self) -> Vec<String> {
        Vec::new()
    }

    fn show_user_dialog(&mut self, _request: UserDialogRequest) -> Option<UserDialogResponse> {
        None
    }

    /// sub_4417A0 statuses: 0, 7 (layer count), 8 (relative priority), 255
    /// (handle). The tables are empty when `count` is outside 1..=16.
    fn configure_particle_frame_tables(
        &mut self,
        _handle: i32,
        _count: i32,
        _first: &[i32],
        _second: &[i32],
    ) -> i32 {
        255
    }

    fn user_spline_exists(&self, _handle: i32) -> bool {
        false
    }

    fn configure_user_spline(&mut self, _handle: i32, _duration: i32, _points: &[[i32; 3]]) -> i32 {
        1
    }

    fn sample_user_spline(&self, _handle: i32, _time: i32) -> Result<[i32; 3], i32> {
        Err(1)
    }

    fn create_user_modeless_dialog(&mut self, _initial: [i32; 9]) -> Option<i32> {
        None
    }

    fn close_user_modeless_dialog(&mut self, _handle: i32) -> bool {
        false
    }

    fn set_user_modeless_dialog_visible(&mut self, _handle: i32, _visible: bool) -> bool {
        false
    }

    fn poll_user_modeless_dialog(&mut self, _handle: i32) -> Result<Option<[i32; 2]>, ()> {
        Err(())
    }
}

fn empty_loaded_program(name: String) -> BpProgram {
    placeholder_loaded_program(
        name,
        "ret",
        "generated empty program after runtime load failure",
    )
}

fn portable_physical_memory_bytes() -> (u64, u64) {
    #[cfg(target_os = "linux")]
    {
        if let Ok(text) = std::fs::read_to_string("/proc/meminfo") {
            let mut total_kb = 0u64;
            let mut available_kb = 0u64;
            for line in text.lines() {
                let mut fields = line.split_whitespace();
                match fields.next() {
                    Some("MemTotal:") => {
                        total_kb = fields
                            .next()
                            .and_then(|value| value.parse().ok())
                            .unwrap_or(0);
                    }
                    Some("MemAvailable:") => {
                        available_kb = fields
                            .next()
                            .and_then(|value| value.parse().ok())
                            .unwrap_or(0);
                    }
                    _ => {}
                }
            }
            return (
                total_kb.saturating_mul(1024),
                available_kb.saturating_mul(1024),
            );
        }
    }
    #[cfg(target_os = "macos")]
    {
        fn command_u64(program: &str, args: &[&str]) -> Option<u64> {
            let output = std::process::Command::new(program)
                .args(args)
                .output()
                .ok()?;
            output.status.success().then_some(())?;
            String::from_utf8_lossy(&output.stdout).trim().parse().ok()
        }
        let total = command_u64("/usr/sbin/sysctl", &["-n", "hw.memsize"]).unwrap_or(0);
        let available = std::process::Command::new("/usr/bin/vm_stat")
            .output()
            .ok()
            .filter(|output| output.status.success())
            .and_then(|output| {
                let text = String::from_utf8_lossy(&output.stdout);
                let page_size = text
                    .lines()
                    .next()
                    .and_then(|line| line.split("page size of ").nth(1))
                    .and_then(|tail| tail.split_whitespace().next())
                    .and_then(|value| value.parse::<u64>().ok())
                    .unwrap_or(4096);
                let mut pages = 0u64;
                for line in text.lines() {
                    let (name, value) = line.split_once(':')?;
                    if matches!(
                        name.trim(),
                        "Pages free" | "Pages inactive" | "Pages speculative" | "Pages purgeable"
                    ) {
                        let value = value.trim().trim_end_matches('.').replace('.', "");
                        pages = pages.saturating_add(value.parse::<u64>().unwrap_or(0));
                    }
                }
                Some(pages.saturating_mul(page_size))
            })
            .unwrap_or(0);
        return (total, available.min(total));
    }
    #[cfg(target_os = "windows")]
    {
        #[repr(C)]
        struct MemoryStatusEx {
            length: u32,
            memory_load: u32,
            total_phys: u64,
            avail_phys: u64,
            total_page_file: u64,
            avail_page_file: u64,
            total_virtual: u64,
            avail_virtual: u64,
            avail_extended_virtual: u64,
        }
        #[link(name = "kernel32")]
        unsafe extern "system" {
            fn GlobalMemoryStatusEx(buffer: *mut MemoryStatusEx) -> i32;
        }
        let mut status = MemoryStatusEx {
            length: std::mem::size_of::<MemoryStatusEx>() as u32,
            memory_load: 0,
            total_phys: 0,
            avail_phys: 0,
            total_page_file: 0,
            avail_page_file: 0,
            total_virtual: 0,
            avail_virtual: 0,
            avail_extended_virtual: 0,
        };
        // SAFETY: GlobalMemoryStatusEx receives a correctly sized writable structure.
        if unsafe { GlobalMemoryStatusEx(&mut status) } != 0 {
            return (status.total_phys, status.avail_phys);
        }
    }
    #[cfg(any(target_os = "linux", target_os = "windows"))]
    {
        return (0, 0);
    }
    #[cfg(not(any(target_os = "linux", target_os = "macos", target_os = "windows")))]
    {
        (0, 0)
    }
}

fn portable_physical_memory_mb() -> (u32, u32) {
    let (total, available) = portable_physical_memory_bytes();
    (
        (total >> 20).min(u64::from(u32::MAX)) as u32,
        (available >> 20).min(u64::from(u32::MAX)) as u32,
    )
}

pub(crate) fn portable_local_system_time() -> [u16; 8] {
    // An installed clock (the web build) carries its own zone offset.
    if let Some(time) = ethornell_core::clock::installed() {
        let seconds = time.since_epoch.as_secs() as i64 + i64::from(time.utc_offset_minutes) * 60;
        let days = seconds.div_euclid(86_400);
        let second_of_day = seconds.rem_euclid(86_400);
        let (year, month, day) = user_data::civil_from_days_for_save(days);
        return [
            year.clamp(0, u16::MAX as i32) as u16,
            month as u16,
            (days + 4).rem_euclid(7) as u16,
            day as u16,
            (second_of_day / 3_600) as u16,
            (second_of_day / 60 % 60) as u16,
            (second_of_day % 60) as u16,
            time.since_epoch.subsec_millis() as u16,
        ];
    }
    #[cfg(target_os = "windows")]
    {
        #[repr(C)]
        struct SystemTime {
            year: u16,
            month: u16,
            day_of_week: u16,
            day: u16,
            hour: u16,
            minute: u16,
            second: u16,
            milliseconds: u16,
        }
        #[link(name = "kernel32")]
        unsafe extern "system" {
            fn GetLocalTime(system_time: *mut SystemTime);
        }
        let mut value = SystemTime {
            year: 0,
            month: 0,
            day_of_week: 0,
            day: 0,
            hour: 0,
            minute: 0,
            second: 0,
            milliseconds: 0,
        };
        // SAFETY: GetLocalTime writes one valid SYSTEMTIME value.
        unsafe { GetLocalTime(&mut value) };
        return [
            value.year,
            value.month,
            value.day_of_week,
            value.day,
            value.hour,
            value.minute,
            value.second,
            value.milliseconds,
        ];
    }
    #[cfg(all(unix, target_pointer_width = "64"))]
    {
        use std::os::raw::{c_char, c_int, c_long};
        #[repr(C)]
        struct Tm {
            sec: c_int,
            min: c_int,
            hour: c_int,
            mday: c_int,
            mon: c_int,
            year: c_int,
            wday: c_int,
            yday: c_int,
            isdst: c_int,
            gmtoff: c_long,
            zone: *const c_char,
        }
        unsafe extern "C" {
            fn localtime_r(timer: *const i64, result: *mut Tm) -> *mut Tm;
        }
        // One sample for both the calendar fields and the milliseconds.
        if let Ok(now) = std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH) {
            let seconds = now.as_secs() as i64;
            let milliseconds = now.subsec_millis() as u16;
            let mut local = std::mem::MaybeUninit::<Tm>::uninit();
            // SAFETY: pointers refer to initialized writable storage and localtime_r is reentrant.
            if !unsafe { localtime_r(&seconds, local.as_mut_ptr()) }.is_null() {
                let local = unsafe { local.assume_init() };
                return [
                    local.year.saturating_add(1900).clamp(0, u16::MAX as i32) as u16,
                    local.mon.saturating_add(1).clamp(0, u16::MAX as i32) as u16,
                    local.wday.clamp(0, u16::MAX as i32) as u16,
                    local.mday.clamp(0, u16::MAX as i32) as u16,
                    local.hour.clamp(0, u16::MAX as i32) as u16,
                    local.min.clamp(0, u16::MAX as i32) as u16,
                    local.sec.clamp(0, u16::MAX as i32) as u16,
                    milliseconds,
                ];
            }
        }
    }
    [0; 8]
}

pub fn default_graphics_capability_record() -> [u32; 16] {
    cpu_record::processor_record()
}

fn target_cpu_brand_string() -> Option<String> {
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(target_arch = "x86")]
        use std::arch::x86::{__cpuid, __cpuid_count};
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::{__cpuid, __cpuid_count};

        let maximum = __cpuid(0x8000_0000).eax;
        if maximum < 0x8000_0004 {
            return None;
        }
        let mut bytes = Vec::with_capacity(48);
        for leaf in 0x8000_0002..=0x8000_0004 {
            let result = __cpuid_count(leaf, 0);
            bytes.extend_from_slice(&result.eax.to_le_bytes());
            bytes.extend_from_slice(&result.ebx.to_le_bytes());
            bytes.extend_from_slice(&result.ecx.to_le_bytes());
            bytes.extend_from_slice(&result.edx.to_le_bytes());
        }
        let end = bytes
            .iter()
            .position(|byte| *byte == 0)
            .unwrap_or(bytes.len());
        return Some(String::from_utf8_lossy(&bytes[..end]).into_owned());
    }
    #[allow(unreachable_code)]
    None
}

fn native_file_hash(bytes: &[u8]) -> [u32; 2] {
    native_file_hash_update([0, 0], bytes)
}

fn native_file_hash_update(initial: [u32; 2], bytes: &[u8]) -> [u32; 2] {
    let mut hash = initial[0];
    let mut tail = initial[1].to_le_bytes();
    for &byte in bytes {
        hash = hash.wrapping_mul(233).wrapping_add(u32::from(byte));
        let low = hash as u8;
        tail[0] = low.wrapping_add(tail[0]);
        tail[1] = low ^ tail[1];
        tail[2] = tail[2].wrapping_add(byte);
        tail[3] = byte ^ tail[3];
    }
    [hash, u32::from_le_bytes(tail)]
}

/// sub_496770: the grid walk where every step costs one and a diagonal is
/// allowed only on equal units; the result is len(a1) + len(a2) minus the
/// shortest walk, i.e. the longest common subsequence. An empty `a2`
/// returns 0 before the walk.
fn wide_string_similarity(a1: &[u16], a2: &[u16]) -> i32 {
    if a2.is_empty() {
        return 0;
    }
    let mut previous: Vec<i32> = (0..=a2.len() as i32).collect();
    let mut current = vec![0i32; a2.len() + 1];
    for (row, &unit) in a1.iter().enumerate() {
        current[0] = row as i32 + 1;
        for column in 1..=a2.len() {
            let mut best = current[column - 1].min(previous[column]);
            if a2[column - 1] == unit {
                best = best.min(previous[column - 1]);
            }
            current[column] = best + 1;
        }
        std::mem::swap(&mut previous, &mut current);
    }
    (a1.len() + a2.len()) as i32 - previous[a2.len()]
}

/// sub_45EAE0(.., 1): a desktop of 2.5:1 or wider is two monitors side by
/// side (half the width); one at most 1.25:1 tall is two stacked (half the
/// height). Unsigned arithmetic; the target divides by zero on an empty
/// mode, here an empty mode stays empty.
fn adjusted_desktop_dimensions((width, height): (u32, u32)) -> (u32, u32) {
    let adjusted_width = if height != 0 && 4u32.wrapping_mul(width) / height >= 10 {
        width >> 1
    } else {
        width
    };
    let adjusted_height = if width != 0
        && height / width != 0
        && 100u32.wrapping_mul(height) / width < 125
    {
        height >> 1
    } else {
        height
    };
    (adjusted_width, adjusted_height)
}

/// sub_496650: drops leading spaces, folds each run of spaces into one and
/// removes one trailing space.
fn bgi_normalize_spaces(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for ch in text.trim_start_matches(' ').chars() {
        if ch == ' ' && out.ends_with(' ') {
            continue;
        }
        out.push(ch);
    }
    if out.ends_with(' ') {
        out.pop();
    }
    out
}

fn placeholder_loaded_program(
    name: String,
    opcode_name: &'static str,
    warning: &'static str,
) -> BpProgram {
    let mut labels = HashMap::new();
    labels.insert(0x10, 0);
    BpProgram {
        script_name: Some(name),
        functions: std::sync::Arc::new(Vec::new()),
        strings: std::sync::Arc::new(Vec::new()),
        instructions: std::sync::Arc::new(vec![BpInstruction {
            offset: 0x10,
            opcode: BpOpcode::Known {
                code: 0x17,
                name: opcode_name,
            },
            operands: Vec::new().into(),
            known_call: None,
            raw: vec![0x17].into(),
            warning: Some(warning.into()),
        }]),
        labels: std::sync::Arc::new(labels),
        warnings: vec![warning.into()],
        module_size: 0,
        code: std::sync::Arc::new(Vec::new()),
    }
}

#[derive(Debug, Default)]
pub struct Vm {
    native_abi_profile: NativeAbiProfile,
    trace_id: u64,
    pending_host_request: Option<host_request::PendingHostRequest>,
    ready_host_response: Option<HostResponse>,
    fatal_async_report: Option<Box<VmRunReport>>,
    async_reports: Vec<VmRunReport>,
    /// Threads that ended their slice with Sys80:5F in the most recent root
    /// scheduler pass (the root and its children).
    pass_yields: usize,
    graph_text_encoding: Option<&'static encoding_rs::Encoding>,
    pub stack: Vec<Value>,
    operand_slots: Vec<Value>,
    operand_slots_synced_len: usize,
    pub pc: usize,
    pub call_stack: Vec<(usize, usize, usize)>,
    pub programs: Vec<BpProgram>,
    /// Target code-region base for each parsed BP module. The executable strips
    /// the BP file header before appending code to CThread+0x2C, while the
    /// parser retains file offsets; this table bridges those two address spaces.
    program_code_bases: Vec<u32>,
    /// Whether a program still occupies the target thread's append-only code
    /// region. Freed modules remain in `programs` only for stable Rust indices.
    program_active: Vec<bool>,
    /// LIFO module records created specifically by Sys80:0x40. Sys80:0x41
    /// removes the most recently appended record without consuming a handle.
    target_loaded_programs: Vec<usize>,
    pub current_program: usize,
    pub memory: VmMemory,
    /// An idle thread's local region (`LOCAL_MEMORY_BASE..HEAP_MEMORY_BASE`)
    /// while another thread holds the process memory (`memory` is then empty).
    parked_local: Option<local_window::LocalWindow>,
    /// One reusable restore buffer follows the active process memory so idle
    /// local windows can stay compact without allocating at each handover.
    local_window_scratch: Vec<u8>,
    // Native writes and thread transfers operate on address ranges. Keep the
    // shadow values ordered so a small write does not scan every scene record.
    pub mem_values: BTreeMap<u32, Value>,
    pub mem_ptr: u32,
    pub heap_ptr: u32,
    heap_allocations: BTreeMap<u32, u32>,
    /// Slot table of the target heap (ADR 0005): script-visible heap
    /// addresses are sub_48E760's, mapped onto `heap_allocations` blocks.
    pub(crate) target_heap: target_heap::TargetHeap,
    heap_free_blocks: Vec<(u32, u32)>,
    pub halted: bool,
    pub calls: BTreeMap<String, usize>,
    pub stubs: BTreeMap<String, usize>,
    program_cache: BTreeMap<String, usize>,
    /// sub_445710 / sub_445850 blocks in this thread's regions; a
    /// DCTChildThread holds its ancestor's (Sys81:44, `child_thread.rs`).
    region_reservations: Arc<std::sync::Mutex<child_thread::RegionReservations>>,
    /// A DCTChildThread: its reservation tag (the thread id).
    child_thread_tag: Option<i32>,
    /// The thread whose local window (data and code regions) this one runs in.
    local_window_owner: Option<i32>,
    /// Lowest code offset a module is appended at (a child's code block).
    code_used_floor: u32,
    program_free_stack: Vec<usize>,
    mediation_programs: BTreeMap<u8, BpProgram>,
    record_tables: BTreeMap<u32, records::RecordTableState>,
    next_record_table_handle: u32,
    indexed_record_tables: BTreeMap<u32, records::IndexedRecordState>,
    next_indexed_record_handle: u32,
    system80_shared: Arc<Mutex<system80_state::System80SharedState>>,
    /// Process-global state owned by the target System81 extended dispatcher.
    system81_shared: Arc<Mutex<system81_state::System81SharedState>>,
    script_records: BTreeMap<u32, String>,
    /// Target System80:0x84/0x85 resource-name namespace.
    ///
    /// The native table assigns each distinct name a stable zero-based index.
    /// Keep it separate from the indexed string namespaces used by 0xDA..0xDD;
    /// the target executable stores these in different native tables.
    /// Target dword_506BE0 and dword_566630 resource-search globals.
    additional_resource_search_enabled: bool,
    additional_resource_paths: Vec<String>,
    /// Named DCArchiveComplex component lists registered by Sys80:0x38.
    composite_archives: BTreeMap<String, Vec<String>>,
    /// Target primary/secondary process-global filesystem roots.
    primary_resource_root: Option<String>,
    secondary_resource_root: Option<String>,
    /// dword_566028 / dword_566338: caption and text of the media prompt,
    /// set by Sys80:3F.
    media_prompt_title: String,
    media_prompt_text: String,
    /// Independent validated directory stored by target sub_46B390.
    validated_file_root: Option<String>,
    /// Target process-global dword_507688 written by Sys80:0x50.
    system_wait_state: i32,
    string_hash_tables: BTreeMap<i32, Vec<String>>,
    read_flags: BTreeMap<String, ReadFlagBits>,
    save_data_integrity_enabled: i32,
    /// Target dword_5668A0: one-shot selection of the next binary/BMV
    /// procedure path. The consuming selector always resets it.
    next_binary_or_bmv_async: bool,
    /// Target dword_566898/dword_56689C cooperative scheduling restriction.
    exclusive_thread_id: Option<i32>,
    /// Size of dword_56676C, the global segment (segment 0 of
    /// sub_48E8C0): `memory[..global_size]` (Sys80:70).
    pub(crate) global_size: usize,
    global_user_data: Vec<u8>,
    loaded_bcs_ranges: Vec<scenario::LoadedBcsRange>,
    async_tasks: Vec<async_program::AsyncProgramTask>,
    suppress_async_pump_once: bool,
    timing: time::VmTime,
    rng_seed: u32,
    recent_trace: VecDeque<String>,
    /// The last instructions run, as (program, pc, offset); formatted into
    /// the report only when the slice stops fatally.
    recent_steps: VecDeque<(usize, usize, u64)>,
    scheduler_signal: Option<SchedulerSignal>,
    /// Target thread selected by native status 3 (`Sys80:5E`). The value is
    /// consumed by the flat cooperative scheduler, not persisted in CThread.
    scheduler_switch_target: Option<i32>,
    /// Portable semantic mirror of target `CThread`. Scheduler state, the
    /// operand-ring index, message FIFO, deadline, and the single
    /// `current_procedure` slot live here instead of being scattered across
    /// unrelated VM fields.
    thread: CThread,
    pending_root_program_messages: VecDeque<Value>,
    pending_root_program_callbacks: VecDeque<[Value; 3]>,
    /// Whether the root CThread had a procedure when this child slice began.
    /// The root cannot change it while a child runs, so 0x80:0x4C aimed at
    /// thread 0 can return sub_445230's "procedure present" result at once.
    root_procedure_active: bool,
    next_program_instance_id: u64,
    /// Process-wide monotonically increasing CThread id source. Root uses 0;
    /// child threads start at 1, matching the target constructor counter.
    next_thread_id: i32,
    collect_diagnostics: bool,
    extended_opcodes: extended_opcodes::ExtendedOpcodeState,
    logic: logic::LogicState,
}

#[derive(Debug, Clone, Default)]
struct ReadFlagBits {
    bit_len: u32,
    bytes: Vec<u8>,
}

impl ReadFlagBits {
    fn new(bit_len: usize) -> Self {
        let bit_len = u32::try_from(bit_len).unwrap_or(u32::MAX);
        let byte_len = usize::try_from(bit_len.div_ceil(8)).unwrap_or_default();
        Self {
            bit_len,
            bytes: vec![0; byte_len],
        }
    }

    fn contains(&self, bit: u32) -> Option<bool> {
        if bit >= self.bit_len {
            return None;
        }
        let byte = *self.bytes.get((bit / 8) as usize)?;
        Some(byte & (1 << (bit & 7)) != 0)
    }

    fn resize(&mut self, bit_len: usize) -> bool {
        let Ok(bit_len) = u32::try_from(bit_len) else {
            return false;
        };
        if bit_len == 0 {
            return false;
        }
        let Ok(byte_len) = usize::try_from(bit_len.div_ceil(8)) else {
            return false;
        };
        self.bytes.resize(byte_len, 0);
        self.bit_len = bit_len;
        if bit_len & 7 != 0 {
            let last_mask = (1u16 << (bit_len & 7)) as u8 - 1;
            if let Some(last) = self.bytes.last_mut() {
                *last &= last_mask;
            }
        }
        true
    }

    fn set(&mut self, bit: u32, enabled: bool) -> bool {
        if bit >= self.bit_len {
            return false;
        }
        let byte = &mut self.bytes[(bit / 8) as usize];
        let mask = 1 << (bit & 7);
        if enabled {
            *byte |= mask;
        } else {
            *byte &= !mask;
        }
        true
    }

    fn set_range(&mut self, start: u32, length: u32, enabled: bool) -> bool {
        let Some(end) = start.checked_add(length) else {
            return false;
        };
        if length == 0 || start >= self.bit_len || end > self.bit_len {
            return false;
        }

        let mut cursor = start;
        while cursor < end && cursor & 7 != 0 {
            self.set(cursor, enabled);
            cursor += 1;
        }
        let fill = if enabled { u8::MAX } else { 0 };
        while cursor.saturating_add(8) <= end {
            self.bytes[(cursor / 8) as usize] = fill;
            cursor += 8;
        }
        while cursor < end {
            self.set(cursor, enabled);
            cursor += 1;
        }
        true
    }
}

/// Split Shift-JIS bytes into target characters (`sub_495990`: a lead byte
/// consumes the following byte too).
fn sjis_units(bytes: &[u8]) -> Vec<&[u8]> {
    let mut units = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let lead = matches!(bytes[i], 0x81..=0x9f | 0xe0..=0xfc);
        let width = if lead { 2 } else { 1 }.min(bytes.len() - i);
        units.push(&bytes[i..i + width]);
        i += width;
    }
    units
}

/// `sub_4959B0`: character-wise search that never backtracks. After a
/// mismatch the current character is not retried as a match start, so e.g.
/// `aab` is not found in `aaab`. Returns the byte offset of the match.
fn target_find_mbs(needle: &[u8], haystack: &[u8]) -> Option<usize> {
    let needle = sjis_units(needle);
    if needle.is_empty() {
        return None;
    }
    let mut matched = 0usize;
    let mut start = 0usize;
    let mut position = 0usize;
    for unit in sjis_units(haystack) {
        if unit == needle[matched] {
            if matched == 0 {
                start = position;
            }
            matched += 1;
            if matched >= needle.len() {
                return Some(start);
            }
        } else {
            matched = 0;
        }
        position += unit.len();
    }
    None
}

/// `sub_495A80` as used by BP opcode 0x67: returns the rewritten bytes and
/// the replacement count.
fn target_strreplace(haystack: &[u8], needle: &[u8], replacement: &[u8]) -> (Vec<u8>, i32) {
    let mut out = Vec::with_capacity(haystack.len());
    let mut rest = haystack;
    let mut count = 0i32;
    while let Some(at) = target_find_mbs(needle, rest) {
        out.extend_from_slice(&rest[..at]);
        out.extend_from_slice(replacement);
        rest = &rest[at + needle.len()..];
        count += 1;
    }
    out.extend_from_slice(rest);
    (out, count)
}

/// Id of the script thread `sub_48C990` creates (see `Vm::new`).
pub(crate) const MAIN_THREAD_ID: i32 = 1;

impl Vm {
    pub fn new() -> Self {
        Self::with_buffers(
            vec![0; INITIAL_MEMORY_SIZE],
            vec![0; user_data::GLOBAL_USER_DATA_SIZE],
        )
    }

    fn with_buffers(memory: Vec<u8>, global_user_data: Vec<u8>) -> Self {
        let mut vm = Self {
            trace_id: NEXT_VM_TRACE_ID.fetch_add(1, Ordering::Relaxed),
            operand_slots: vec![Value::Int(0); OPERAND_STACK_CAPACITY],
            native_abi_profile: NativeAbiProfile::default(),
            memory: memory.into(),
            parked_local: None,
            heap_ptr: 0x0020_0000,
            rng_seed: 1,
            system_wait_state: 1,
            next_indexed_record_handle: 1,
            // sub_48C990 first builds an empty root CThread (id 0, never a
            // script thread: sub_444B90 cannot resolve 0) and then the
            // script thread through sub_48D080 (id 1); 0x80:0x44 children
            // continue from 2.
            next_thread_id: MAIN_THREAD_ID + 1,
            global_size: user_data::GLOBAL_CONFIG_SIZE,
            global_user_data,
            ..Self::default()
        };
        vm.thread.set_thread_id(MAIN_THREAD_ID);
        vm
    }

    /// Start the sub_499290 clock at `base_ms` (the host uptime for
    /// interactive runs).
    pub fn set_clock_base_ms(&mut self, base_ms: u32) {
        self.timing.set_base(base_ms);
    }

    /// dword_503DE8 (Sys81:04): the largest clock step sub_499290 counts.
    pub fn clock_jump_threshold_ms(&self) -> u32 {
        self.system81_shared.lock().expect("system81 state poisoned").clock_jump_threshold_ms as u32
    }

    /// How many threads ended their slice with Sys80:5F in the most recent
    /// scheduler pass (`run_loaded`), children included.
    pub fn yields_in_last_pass(&self) -> usize {
        self.pass_yields
    }

    pub fn advance_time_ms(&mut self, milliseconds: u64) {
        self.timing.advance(milliseconds);
    }

    /// The configured display-text code page, including the Shift-JIS default.
    pub fn graph_text_encoding(&self) -> &'static encoding_rs::Encoding {
        self.graph_text_encoding.unwrap_or(encoding_rs::SHIFT_JIS)
    }

    /// Select the code page of text passed to native graph drawing calls.
    /// Resource names and BP system strings retain their original encoding.
    pub fn set_graph_text_encoding(&mut self, encoding: &'static encoding_rs::Encoding) {
        self.graph_text_encoding = Some(encoding);
        for task in &mut self.async_tasks {
            task.vm.set_graph_text_encoding(encoding);
        }
    }

    pub fn run<A>(
        &mut self,
        program: &BpProgram,
        api: &mut A,
        options: &VmRunOptions,
    ) -> VmRunReport
    where
        A: SysApi + GraphApi + SoundApi,
    {
        self.start(program);
        self.run_loaded(api, options)
    }

    pub fn start(&mut self, program: &BpProgram) {
        self.start_execution(program, true);
    }

    fn start_execution(&mut self, program: &BpProgram, install_code: bool) {
        self.pending_host_request = None;
        self.ready_host_response = None;
        self.fatal_async_report = None;
        self.async_reports.clear();
        self.stack.clear();
        self.operand_slots.fill(Value::Int(0));
        self.operand_slots_synced_len = 0;
        self.programs.clear();
        self.programs.push(program.clone());
        self.program_code_bases.clear();
        self.program_code_bases.push(0);
        if install_code {
            self.install_module_code(0, &program.code);
        }
        self.program_active.clear();
        self.program_active.push(true);
        self.target_loaded_programs.clear();
        // The root BP image is installed through the same CThread module
        // record path before execution begins. FreeProgram therefore normally
        // returns at least one after removing a dynamically loaded module.
        self.target_loaded_programs.push(0);
        self.program_cache.clear();
        if let Some(name) = program.script_name.clone() {
            self.program_cache.insert(name, 0);
        }
        self.current_program = 0;
        self.pc = 0;
        self.call_stack.clear();
        self.program_free_stack.clear();
        self.mediation_programs.clear();
        self.halted = false;
        self.scheduler_signal = None;
        self.scheduler_switch_target = None;
        self.thread.reset_execution();
        self.sync_thread_program_region();
        self.pending_root_program_messages.clear();
        self.pending_root_program_callbacks.clear();
        self.timing.reset();
    }
    /// Debug harness entry for replaying a real loaded module with the
    /// initialized framework mediation table; never compiled into release.
    #[cfg(debug_assertions)]
    pub fn diagnostic_start_with_initialized_framework(&mut self,program:&BpProgram) {
        let mediation=self.mediation_programs.clone();
        self.start(program);
        self.mediation_programs=mediation;
    }

    pub fn run_loaded<A>(&mut self, api: &mut A, options: &VmRunOptions) -> VmRunReport
    where
        A: SysApi + GraphApi + SoundApi,
    {
        self.run_loaded_internal(api, options, true)
    }

    fn run_loaded_internal<A>(
        &mut self,
        api: &mut A,
        options: &VmRunOptions,
        reconcile_public_stack: bool,
    ) -> VmRunReport
    where
        A: SysApi + GraphApi + SoundApi,
    {
        if let Some(report) = &self.fatal_async_report {
            return (**report).clone();
        }
        self.collect_diagnostics = options.collect_diagnostics;
        self.async_reports.clear();
        self.pass_yields = 0;
        // Public entry points reconcile direct embedder writes to `stack`.
        // Private asynchronous contexts maintain their ring incrementally and
        // retain the synchronized prefix across scheduler slices.
        if reconcile_public_stack {
            self.operand_slots_synced_len = 0;
        }
        self.sync_operand_slots();
        let mut steps = 0usize;
        let step_limit = options.max_steps.min(TARGET_COOPERATIVE_QUANTUM_STEPS);
        let mut stop_reason = VmStopReason::Completed;
        let trace_stack = std::env::var_os("TRACE_STACK").is_some();
        let trace_stack_vm = std::env::var("TRACE_STACK_VM")
            .ok()
            .and_then(|value| value.parse::<u64>().ok());
        let trace_stack_program = std::env::var("TRACE_STACK_PROGRAM").ok();
        let trace_stack_offset_min = trace_u32_env("TRACE_STACK_OFFSET_MIN");
        let trace_stack_offset_max = trace_u32_env("TRACE_STACK_OFFSET_MAX");
        let mut instruction_profile = debug::InstructionProfile::from_env(self.trace_id);
        let profile_instructions = instruction_profile.is_enabled();
        let trace_motion = debug::debug_enabled();
        let trace_message_loop = debug::trace_msgwnd_enabled();
        let trace_events =
            std::env::var_os("TRACE_VM_EVENTS").is_some() || std::env::var_os("DEBUG").is_some();
        if !self.halted && !std::mem::take(&mut self.suppress_async_pump_once) {
            if let Some(report) = self.pump_async_programs(api, trace_events, options) {
                self.halted = true;
                self.fatal_async_report = Some(Box::new(report.clone()));
                return report;
            }
        }
        let response_error = self.settle_ready_host_response().err();
        // 48D860 skips every non-owner before polling its CProcedure or
        // executing BP instructions. Child pumping above still reaches the
        // owner when the root itself is excluded.
        let procedure_wait = if self.exclusive_thread_id.is_some_and(|owner| owner != self.thread.thread_id()) {
            Some(VmStopReason::WaitingForProcedure)
        } else if let Some(error) = response_error {
            self.push_trace(format!("host response failed: {error}"));
            self.halted = true;
            Some(VmStopReason::Error)
        } else if self.pending_host_request.is_some() {
            Some(VmStopReason::WaitingForHost)
        } else {
            self.poll_current_procedure(api, trace_events)
        };
        if let Some(reason) = procedure_wait {
            stop_reason = reason;
        }
        while steps < step_limit
            && procedure_wait.is_none()
            && !self.halted
            && self
                .programs
                .get(self.current_program)
                .and_then(|program| program.instructions.get(self.pc))
                .is_some()
        {
            let program_index = self.current_program;
            let inst_ptr =
                &self.programs[program_index].instructions[self.pc] as *const BpInstruction;
            // Dispatch may append to `programs`, but it never mutates or removes
            // an existing program's instruction buffer. The boxed Vec storage
            // containing this instruction therefore remains stable for the
            // duration of this iteration.
            let inst = unsafe { &*inst_ptr };
            let next_instruction_ip = self.programs[program_index]
                .instructions
                .get(self.pc + 1)
                .map(|next| next.offset as u32)
                .unwrap_or(inst.offset as u32);
            self.thread
                .set_instruction_ips(inst.offset as u32, next_instruction_ip);
            let stack_before = self.stack.len();
            let pc_before = self.pc;
            if profile_instructions {
                instruction_profile.record(self, program_index, pc_before);
            }
            if options.trace {
                println!(
                    "pc={} off=0x{:08X} {} {:?} stack_top={:?}",
                    self.pc,
                    inst.offset,
                    inst.opcode.name(),
                    inst.operands,
                    self.stack.last()
                );
            }
            if trace_events || options.trace {
                self.push_trace(format!(
                    "program={} pc={} off=0x{:08X} {} {:?}",
                    self.program_name(program_index),
                    self.pc,
                    inst.offset,
                    inst.opcode.name(),
                    inst.operands
                ));
            } else if options.fail_on_stub {
                // Formatting a trace line per instruction was half of a
                // console frame; keep the position and format on failure.
                if self.recent_steps.len() >= 30 {
                    self.recent_steps.pop_front();
                }
                self.recent_steps.push_back((program_index, self.pc, inst.offset));
            }
            if trace_motion {
                self.trace_mtn_body_dispatch(inst.offset);
            }
            match self.dispatch_program(
                program_index,
                inst,
                api,
                options.fail_on_stub,
                trace_events,
            ) {
                Ok(()) => {
                    if trace_stack {
                        let stack_after = self.stack.len();
                        let delta = stack_after as isize - stack_before as isize;
                        let program_name = self
                            .programs
                            .get(program_index)
                            .and_then(|program| program.script_name.as_deref())
                            .unwrap_or("<unknown>");
                        let program_matches = trace_stack_program
                            .as_deref()
                            .is_none_or(|filter| program_name.contains(filter));
                        let offset_matches = trace_stack_offset_min
                            .is_none_or(|minimum| inst.offset >= minimum)
                            && trace_stack_offset_max.is_none_or(|maximum| inst.offset <= maximum);
                        if trace_stack_vm.is_none_or(|trace_id| trace_id == self.trace_id)
                            && program_matches
                            && offset_matches
                            && (delta != 0 || inst.opcode.name().starts_with("sys"))
                        {
                            eprintln!(
                                "TRACE_STACK vm={} program={} pc={} off=0x{:08X} op={} {:?} stack {} -> {} ({:+}) next_pc={} top={:?}",
                                self.trace_id,
                                program_name,
                                pc_before,
                                inst.offset,
                                inst.opcode.name(),
                                inst.operands,
                                stack_before,
                                stack_after,
                                delta,
                                self.pc,
                                self.stack
                                    .iter()
                                    .rev()
                                    .take(4)
                                    .map(value_summary)
                                    .collect::<Vec<_>>()
                            );
                        }
                    }
                    steps += 1;
                    if trace_message_loop {
                        self.trace_msgwnd_loop(inst.offset as u32);
                    }
                    if self.pending_host_request.is_some() {
                        stop_reason = VmStopReason::WaitingForHost;
                        break;
                    }
                    // Every target handler marked as installing CProcedure must
                    // suspend at the syscall boundary. Continuing here would
                    // execute later BP instructions in the same scheduler pass
                    // and is the primary shape of the observed fast-forward bug.
                    if let Some(installed) = self.thread.current_procedure() {
                        stop_reason = match installed.object {
                            CProcedure::WaitTiming(_) => VmStopReason::WaitingForTime,
                            CProcedure::WaitTimingEx(procedure) if procedure.input_enabled() => {
                                VmStopReason::WaitingForInputOrTime
                            }
                            CProcedure::WaitTimingEx(_) => VmStopReason::WaitingForTime,
                            CProcedure::WaitWndMsg(_) => VmStopReason::WaitingForProcedure,
                            CProcedure::DspMsg(_) => VmStopReason::WaitingForInputOrTime,
                            CProcedure::LoadSound(_) => VmStopReason::WaitingForProcedure,
                            CProcedure::HostCompleted(_) => VmStopReason::WaitingForProcedure,
                            CProcedure::Graph(_) => VmStopReason::WaitingForProcedure,
                            CProcedure::Exclusion(_) => VmStopReason::WaitingForProcedure,
                            CProcedure::Unrecovered(_) => VmStopReason::WaitingForProcedure,
                        };
                        break;
                    }
                    if let Some(signal) = self.scheduler_signal.take() {
                        stop_reason = signal.stop_reason();
                        break;
                    }
                    if api.take_frame_yield() {
                        stop_reason = VmStopReason::Yielded;
                        break;
                    }
                }
                Err(VmError::UnsupportedInstruction(_)) => {
                    self.halted = true;
                    stop_reason = VmStopReason::UnknownOpcode;
                    steps += 1;
                    break;
                }
                Err(VmError::UnknownDispatch { .. }) => {
                    self.halted = true;
                    stop_reason = VmStopReason::UnknownDispatch;
                    steps += 1;
                    break;
                }
                Err(err) => {
                    self.report_script_error(api, program_index, inst.offset as u32, inst.raw.first().copied().unwrap_or(0), &err);
                    self.push_trace(format!("error: {err}"));
                    tracing::error!(
                        vm = self.trace_id,
                        program = self.program_name(program_index),
                        pc = pc_before,
                        offset = format_args!("0x{:08X}", inst.offset),
                        %err,
                        "VM instruction failed"
                    );
                    self.halted = true;
                    stop_reason = VmStopReason::Error;
                    steps += 1;
                    break;
                }
            }
        }
        let still_runnable = !self.halted
            && self
                .programs
                .get(self.current_program)
                .and_then(|program| program.instructions.get(self.pc))
                .is_some();
        if steps >= step_limit && still_runnable && matches!(stop_reason, VmStopReason::Completed) {
            if step_limit == TARGET_COOPERATIVE_QUANTUM_STEPS
                && options.max_steps >= TARGET_COOPERATIVE_QUANTUM_STEPS
            {
                stop_reason = VmStopReason::QuantumExhausted;
            } else {
                stop_reason = VmStopReason::WatchdogExceeded;
                tracing::warn!(
                    vm = self.trace_id,
                    program = self.program_name(self.current_program),
                    pc = self.pc,
                    steps,
                    "VM host watchdog reached before the target cooperative quantum"
                );
            }
        }
        instruction_profile.report(self, steps);
        if stop_reason == VmStopReason::Completed && !self.pending_host_request_ids().is_empty() {
            stop_reason = VmStopReason::WaitingForHost;
        }
        if stop_reason == VmStopReason::Yielded {
            self.pass_yields += 1;
        }
        VmRunReport {
            steps,
            pc: self.pc,
            offset: self
                .programs
                .get(self.current_program)
                .and_then(|program| program.instructions.get(self.pc))
                .map(|i| i.offset as u32),
            program: self.program_name(self.current_program).to_string(),
            stop_reason,
            thread: VmThreadReport {
                native_layout: self.thread.native,
                root_thread_id: self.thread.root_thread_id(),
                thread_id: self.thread.thread_id(),
                next_thread_id: self.thread.next_thread_id(),
                status_flags: self.thread.status(),
                operand_index: self.thread.operand_index(),
                current_opcode_ip: self.thread.current_opcode_ip(),
                instruction_ip: self.thread.instruction_ip(),
                frame_base: self.thread.frame_base(),
                deadline_tick: self.thread.deadline_tick(),
                current_procedure_class: self
                    .thread
                    .current_procedure()
                    .map(|installed| installed.object.class_name().to_string()),
                current_procedure_opcode: self
                    .thread
                    .current_procedure()
                    .map(|installed| installed.source_opcode),
                current_procedure: self
                    .thread
                    .current_procedure()
                    .map(|installed| match installed.object {
                        CProcedure::WaitTiming(procedure) => VmProcedureReport::WaitTiming {
                            source_opcode: installed.source_opcode,
                            native_layout: procedure.base.native,
                            duration_ms: procedure.duration_ms,
                        },
                        CProcedure::WaitTimingEx(procedure) => VmProcedureReport::WaitTimingEx {
                            source_opcode: installed.source_opcode,
                            native_layout: procedure.native,
                            duration_ms: procedure.call.duration_ms,
                            input_enabled: procedure.call.input_enabled,
                            input_scope: procedure.call.input_scope,
                        },
                        CProcedure::WaitWndMsg(procedure) => VmProcedureReport::WaitWndMsg {
                            source_opcode: installed.source_opcode,
                            native_layout: procedure.native,
                            message_id: procedure.message_id,
                            registered_after_serial: procedure.registered_after_serial,
                        },
                        CProcedure::DspMsg(procedure) => VmProcedureReport::DspMsg {
                            source_opcode: installed.source_opcode,
                            class: procedure.config.class,
                            initial_deadline_tick: procedure.initial_deadline_tick,
                            reveal_duration_ms: procedure.config.reveal_duration_ms,
                            auto_deadline_tick: procedure.auto_deadline_tick,
                            input_scope: procedure.config.input_scope,
                            completion_control: procedure.config.completion_control,
                            end_wait_policy: procedure.config.end_wait_policy,
                            allow_high_bit_input: procedure.native.allow_high_bit_input != 0,
                            allow_auxiliary_input: procedure.native.allow_auxiliary_input != 0,
                            input_forces_completion: procedure.config.input_forces_completion,
                        },
                        CProcedure::LoadSound(procedure) => VmProcedureReport::LoadSound {
                            source_opcode: installed.source_opcode,
                            terminal_status: procedure.terminal_status,
                        },
                        CProcedure::HostCompleted(procedure) => VmProcedureReport::HostCompleted {
                            source_opcode: installed.source_opcode,
                            target_class_name: procedure.target_class_name,
                            terminal_status: procedure.terminal_status,
                            outputs: procedure.outputs,
                            output_count: procedure.output_count,
                        },
                        CProcedure::Graph(procedure) => VmProcedureReport::Graph {
                            source_opcode: installed.source_opcode,
                            target_class_name: procedure.target_class_name,
                            mode: procedure.mode.name(),
                            native_layout: procedure.base.native,
                        },
                        CProcedure::Exclusion(procedure) => VmProcedureReport::Exclusion {
                            source_opcode: installed.source_opcode,
                            section_id: procedure.section_id,
                            native_layout: procedure.base.native,
                        },
                        CProcedure::Unrecovered(procedure) => VmProcedureReport::Unrecovered {
                            source_opcode: installed.source_opcode,
                            target_class_name: procedure.target_class_name,
                            native_layout: procedure.base.native,
                        },
                    }),
            },
            calls: options
                .collect_diagnostics
                .then(|| self.calls.clone())
                .unwrap_or_default(),
            stubs: options
                .collect_diagnostics
                .then(|| self.stubs.clone())
                .unwrap_or_default(),
            recent_trace: options
                .collect_diagnostics
                .then(|| {
                    let mut trace = Vec::new();
                    if stop_reason.is_fatal() {
                        trace.extend(self.recent_steps.iter().map(|&(program_index, pc, offset)| {
                            let program = &self.programs[program_index.min(self.programs.len().saturating_sub(1))];
                            let (name, operands) = program
                                .instructions
                                .get(pc)
                                .map(|inst| (inst.opcode.name(), format!("{:?}", inst.operands)))
                                .unwrap_or(("?", String::new()));
                            format!(
                                "program={} pc={pc} off=0x{offset:08X} {name} {operands}",
                                self.program_name(program_index)
                            )
                        }));
                    }
                    trace.extend(self.recent_trace.iter().cloned());
                    trace
                })
                .unwrap_or_default(),
        }
    }

    pub fn dispatch<A>(&mut self, instruction: &BpInstruction, api: &mut A) -> VmResult<()>
    where
        A: SysApi + GraphApi + SoundApi,
    {
        self.operand_slots_synced_len = 0;
        self.sync_operand_slots();
        let old_program = self.current_program;
        let old_pc = self.pc;
        self.programs.push(BpProgram {
            script_name: None,
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![instruction.clone()]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        });
        self.current_program = self.programs.len() - 1;
        self.pc = 0;
        let result = self.dispatch_program(self.current_program, instruction, api, false, false);
        self.programs.pop();
        self.current_program = old_program;
        self.pc = old_pc;
        result
    }

    fn dispatch_program<A>(
        &mut self,
        program_index: usize,
        instruction: &BpInstruction,
        api: &mut A,
        fail_on_stub: bool,
        trace_events: bool,
    ) -> VmResult<()>
    where
        A: SysApi + GraphApi + SoundApi,
    {
        let code = instruction.opcode.code();
        let mut next_pc = self.pc + 1;
        // Canonical bytecodes dispatch by their numeric opcode. Names remain
        // diagnostic metadata; legacy synthetic aliases below keep their
        // compatibility handling without putting string matches on core ops.
        match instruction.opcode {
            BpOpcode::Known { code: 0x00, .. } => {
                self.push_value(Value::Int(read_op_i32(instruction)));
            }
            BpOpcode::Known { code: 0x01 | 0x02, .. } => {
                self.push_value(Value::Int(read_op_i32(instruction)));
            }
            BpOpcode::Known { code: 0x04, .. } => {
                self.push_value(Value::Ptr(
                    FRAME_SEGMENT | self.mem_ptr.saturating_sub(read_op_u32(instruction)),
                ));
            }
            BpOpcode::Known { code: 0x05, .. } => {
                if let Some(BpOperand::String(text)) = instruction.operands.first() {
                    // Opcode 0x05 pushes the literal's code address,
                    // `(pc_base + off) | 0x04000000`; the bytes are in the
                    // thread's code region (`install_module_code`).
                    let program = &self.programs[program_index];
                    let relative = instruction
                        .raw
                        .get(1..3)
                        .map(|bytes| i16::from_le_bytes([bytes[0], bytes[1]]) as i32);
                    let start = Self::program_parser_code_start(program);
                    let literal = relative
                        .map(|relative| (instruction.offset as i64 + i64::from(relative)) as u32)
                        .filter(|&target| {
                            target
                                .checked_sub(start)
                                .is_some_and(|offset| (offset as usize) < program.code.len())
                        });
                    if let Some(target) = literal {
                        let base = self.program_code_bases.get(program_index).copied().unwrap_or_default();
                        self.push_value(Value::Ptr(CODE_SEGMENT | base.wrapping_add(target - start)));
                    } else {
                        self.push_value(Value::Str(text.clone()));
                    }
                } else if let Some(BpOperand::Offset(offset)) = instruction.operands.first() {
                    // Opcode 0x05: `(pc_base + off) | 0x04000000`.
                    let program = &self.programs[program_index];
                    let base = self.program_code_bases.get(program_index).copied().unwrap_or_default();
                    let address = base.wrapping_add(offset.wrapping_sub(Self::program_parser_code_start(program)));
                    self.push_value(Value::Ptr(CODE_SEGMENT | address));
                }
            }
            BpOpcode::Known { code: 0x06, .. } => {
                // Opcode 0x06 (sub_473650): the plain code-region offset
                // `insn_start + disp`, without a segment tag.
                let code = self.code_offset_of(program_index, read_op_u32(instruction));
                self.push_value(Value::Int(code as i32));
            }
            BpOpcode::Known { code: 0x10, .. } => {
                // sub_473880 pushes the raw frame-memory offset. Only opcode
                // 0x04 turns base-relative offsets into dereferenceable pointers.
                self.push_value(Value::Int(self.mem_ptr as i32));
            }
            BpOpcode::Known { code: 0x11, .. } => {
                let previous = self.mem_ptr;
                let next = self.pop_int()? as u32;
                // sub_4738A0: the new frame pointer must stay below the data
                // region limit.
                if next >= self.thread.data_region_size() {
                    return Err(VmError::Runtime(format!(
                        "store_base: SP 0x{next:08X} outside the {}-byte data region",
                        self.thread.data_region_size()
                    )));
                }
                if next > previous {
                    // The target leaves the new frame untouched (sub_4738A0).
                    // Clearing it is kept on purpose: some scripts read frame
                    // slots that the original had filled by a step the
                    // portable runtime does not reproduce yet (the save-slot
                    // excerpt line), and stale bytes there decode as garbage
                    // text. See src/README.md "Open fidelity items".
                    let size = (next - previous) as usize;
                    let ptr = FRAME_SEGMENT | previous;
                    let range = self.resolve_write_range(ptr, size)?;
                    self.memory[range].fill(0);
                    self.clear_shadow_values(ptr, size);
                }
                tracing::trace!(
                    target: "vm_frames",
                    program = self.program_name(program_index),
                    pc = self.pc,
                    offset = format_args!("0x{:08X}", instruction.offset),
                    previous = format_args!("0x{previous:08X}"),
                    next = format_args!("0x{next:08X}"),
                    "VM store_base"
                );
                self.mem_ptr = next;
            }
            BpOpcode::Known { code: 0x08, .. } => {
                let width = read_op_u8(instruction);
                let ptr = self.pop_ptr()?;
                let value = self.read_value(ptr, width)?;
                self.push_value(value);
            }
            BpOpcode::Known { code: 0x09, .. } => {
                let width = read_op_u8(instruction);
                let value = self.pop_value()?;
                let ptr = self.pop_ptr()?;
                self.write_value(ptr, width, &value)?;
                // sub_473710 writes through the pointer and calls sub_4450D0
                // to return the assigned value on the operand ring.
                self.push_value(value);
            }
            BpOpcode::Known { code: 0x0a, .. } => {
                let width = read_op_u8(instruction);
                let ptr = self.pop_ptr()?;
                let value = self.pop_value()?;
                tracing::trace!(
                    target: "vm_operands",
                    program = self.program_name(program_index),
                    pc = self.pc,
                    offset = format_args!("0x{:08X}", instruction.offset),
                    width,
                    ptr = format_args!("0x{ptr:08X}"),
                    value = %value_summary(&value),
                    "VM move_arg"
                );
                if std::env::var_os("TRACE_SCRMAIN_DISPATCH").is_some()
                    && instruction.offset == 0x3f4
                    && self.program_name(program_index).contains("scrmain._bp")
                {
                    tracing::warn!(
                        vm = self.trace_id,
                        mem_ptr = format_args!("0x{:08X}", self.mem_ptr),
                        ptr = format_args!("0x{ptr:08X}"),
                        value = %value_summary(&value),
                        stack = self.stack.len(),
                        "TRACE_SCRMAIN_DISPATCH result"
                    );
                }
                self.write_value(ptr, width, &value)?;
            }
            BpOpcode::Known { code: 0x0b, .. } => {
                let ptr = self.pop_ptr()?;
                let bytes = instruction
                    .operands
                    .first()
                    .and_then(|operand| match operand {
                        BpOperand::Raw(bytes) => Some(bytes.as_slice()),
                        _ => None,
                    })
                    .unwrap_or_default();
                for (offset, byte) in bytes.iter().copied().enumerate() {
                    self.write_int(ptr.wrapping_add(offset as u32), 0, u32::from(byte))?;
                }
            }
            BpOpcode::Known { code: 0x0c, .. } => {
                let width = read_op_u8(instruction);
                let count = instruction.raw.get(2).copied().unwrap_or_default() as usize;
                let mut values = Vec::with_capacity(count);
                for _ in 0..count {
                    values.push(self.pop_value()?);
                }
                let dst = self.pop_ptr()?;
                if trace_events && count <= 12 {
                    tracing::info!(
                        pc = self.pc,
                        offset = format_args!("0x{:08X}", instruction.offset),
                        dst = format_args!("0x{dst:08X}"),
                        width,
                        count,
                        values = ?values.iter().rev().map(value_summary).collect::<Vec<_>>(),
                        "VM copy_stack"
                    );
                }
                let stride = 1u32 << width.min(2);
                for (idx, value) in values.into_iter().rev().enumerate() {
                    self.write_value(dst.wrapping_add(stride * idx as u32), width, &value)?;
                }
            }
            BpOpcode::Known { code: 0x14, .. } => {
                let dest = self.pop_int()? as u32;
                if dest == 0 {
                    // sub_473910 rejects a zero target before the range check.
                    return Err(VmError::Runtime(format!(
                        "jmp to null target at 0x{:08X} in {}",
                        instruction.offset,
                        self.program_name(program_index)
                    )));
                }
                let (target_program, parser_offset) = self.resolve_code_target(program_index, dest);
                self.current_program = target_program;
                next_pc = self.jump_target_index(target_program, parser_offset)?;
            }
            BpOpcode::Known { code: 0x15, .. } => {
                let kind = read_op_u8(instruction);
                let dest = self.pop_int()? as u32;
                let value = self.pop_int()?;
                let take = match kind {
                    0 => value != 0,
                    1 => value == 0,
                    2 => value > 0,
                    3 => value >= 0,
                    4 => value <= 0,
                    5 => value < 0,
                    _ => true,
                };
                if trace_vm_branches_enabled() {
                    tracing::debug!(
                        program = self
                            .programs
                            .get(program_index)
                            .and_then(|program| program.script_name.as_deref())
                            .unwrap_or("<anonymous>"),
                        pc = self.pc,
                        offset = format_args!("0x{:08X}", instruction.offset),
                        kind,
                        value,
                        dest = format_args!("0x{dest:08X}"),
                        take,
                        "VM jc"
                    );
                }
                if take {
                    let (target_program, parser_offset) = self.resolve_code_target(program_index, dest);
                    self.current_program = target_program;
                    next_pc = self.jump_target_index(target_program, parser_offset)?;
                }
            }
            BpOpcode::Known { code: 0x16, .. } => match {
                let target = self.pop_value()?;
                if std::env::var_os("TRACE_SCRMAIN_DISPATCH").is_some()
                    && instruction.offset == 0x3f0
                    && self.program_name(program_index).contains("scrmain._bp")
                {
                    let (target_program, target_offset) = match &target {
                        Value::Func {
                            program_index,
                            offset,
                        } => (self.program_name(*program_index), *offset),
                        _ => ("<non-function>", 0),
                    };
                    let command = self
                        .read_int(FRAME_SEGMENT | self.mem_ptr.saturating_sub(4), 2)
                        .unwrap_or_default();
                    let scenario_offset = self.read_int(314_072 + 416, 2).unwrap_or_default();
                    tracing::warn!(
                        vm = self.trace_id,
                        mem_ptr = format_args!("0x{:08X}", self.mem_ptr),
                        target = %value_summary(&target),
                        target_program,
                        target_offset = format_args!("0x{target_offset:08X}"),
                        command = format_args!("0x{command:08X}"),
                        scenario_offset = format_args!("0x{scenario_offset:08X}"),
                        stack = self.stack.len(),
                        "TRACE_SCRMAIN_DISPATCH call"
                    );
                }
                target
            } {
                Value::Func {
                    program_index: dest_program_index,
                    offset,
                } => {
                    if offset == 0 {
                        // sub_473910: a zero jump/call target is a fatal error.
                        return Err(VmError::Runtime(format!(
                            "call to null target at 0x{:08X} in {}",
                            instruction.offset,
                            self.program_name(program_index)
                        )));
                    }
                    if trace_events {
                        tracing::info!(
                            pc = self.pc,
                            offset = format_args!("0x{:08X}", instruction.offset),
                            dest = format_args!("0x{offset:08X}"),
                            dest_program = dest_program_index,
                            stack_top = ?self.stack_summary(8),
                            "VM call"
                        );
                    }
                    tracing::trace!(
                        target: "vm_frames",
                        caller = self.program_name(program_index),
                        caller_pc = self.pc,
                        caller_offset = format_args!("0x{:08X}", instruction.offset),
                        callee = self.program_name(dest_program_index),
                        destination = format_args!("0x{offset:08X}"),
                        mem_ptr = format_args!("0x{:08X}", self.mem_ptr),
                        stack_top = ?self.stack_summary(12),
                        "VM call frame"
                    );
                    self.write_return_addr(program_index, next_pc)?;
                    self.call_stack
                        .push((self.current_program, next_pc, self.stack.len()));
                    self.current_program = dest_program_index;
                    next_pc = self.jump_target_index(dest_program_index, offset)?;
                }
                Value::Program(program) => {
                    let dest_program_index =
                        self.program_index_for_loaded_program((*program).clone());
                    self.program_free_stack.push(dest_program_index);
                    if trace_events {
                        tracing::info!(
                            pc = self.pc,
                            offset = format_args!("0x{:08X}", instruction.offset),
                            dest_program = dest_program_index,
                            stack_top = ?self.stack_summary(8),
                            "VM call program"
                        );
                    }
                    self.write_return_addr(self.current_program, next_pc)?;
                    self.call_stack
                        .push((self.current_program, next_pc, self.stack.len()));
                    self.current_program = dest_program_index;
                    next_pc = self.jump_target_index(self.current_program, 0x10)?;
                }
                value => {
                    let dest = value.as_i32() as u32;
                    if dest == 0 {
                        return Err(VmError::Runtime(format!(
                            "call to null target at 0x{:08X} in {}",
                            instruction.offset,
                            self.program_name(program_index)
                        )));
                    }
                    let (dest_program_index, parser_offset) =
                        self.resolve_indirect_call_target(program_index, dest);
                    if trace_events {
                        tracing::info!(
                            pc = self.pc,
                            offset = format_args!("0x{:08X}", instruction.offset),
                            dest = format_args!("0x{dest:08X}"),
                            dest_program = dest_program_index,
                            parser_offset = format_args!("0x{parser_offset:08X}"),
                            stack_top = ?self.stack_summary(8),
                            "VM call"
                        );
                    }
                    self.write_return_addr(program_index, next_pc)?;
                    self.call_stack
                        .push((self.current_program, next_pc, self.stack.len()));
                    self.current_program = dest_program_index;
                    next_pc = self.jump_target_index(dest_program_index, parser_offset)?;
                }
            },
            BpOpcode::Known { code: 0x17, .. } => {
                // sub_474020: a frame stack at the region base ends the thread.
                if self.mem_ptr <= self.thread.frame_region_base() {
                    self.halted = true;
                } else if let Some((program_id, fallback_ret, stack_base)) = self.call_stack.pop() {
                    let ret_offset = self.read_return_addr()?;
                    if trace_call_frames_enabled(self.trace_id) {
                        eprintln!(
                            "TRACE_CALL_FRAME vm={} callee={} caller={} stack_base={} stack_return={} delta={:+}",
                            self.trace_id,
                            self.program_name(self.current_program),
                            self.program_name(program_id),
                            stack_base,
                            self.stack.len(),
                            self.stack.len() as isize - stack_base as isize
                        );
                    }
                    if trace_events {
                        tracing::info!(
                            pc = self.pc,
                            offset = format_args!("0x{:08X}", instruction.offset),
                            return_program = program_id,
                            return_offset = format_args!("0x{ret_offset:08X}"),
                            fallback_pc = fallback_ret,
                            stack_top = ?self.stack_summary(8),
                            "VM ret"
                        );
                    }
                    let (return_program, parser_offset) = self.resolve_code_target(program_id, ret_offset);
                    self.current_program = return_program;
                    next_pc = self
                        .jump_target_index(return_program, parser_offset)
                        .unwrap_or(fallback_ret);
                } else {
                    self.halted = true;
                }
            }
            BpOpcode::Known { name, .. } if matches!(name, "eq" | "neq") => {
                let right = self.pop_value()?;
                let left = self.pop_value()?;
                let equal = self.values_equal(&left, &right)?;
                let value = match name {
                    "eq" => equal as i32,
                    "neq" => (!equal) as i32,
                    _ => 0,
                };
                self.push_value(Value::Int(value));
            }
            BpOpcode::Known { name, .. }
                if matches!(
                    name,
                    "add"
                        | "sub"
                        | "mul"
                        | "div"
                        | "mod"
                        | "and"
                        | "or"
                        | "xor"
                        | "leq"
                        | "geq"
                        | "lt"
                        | "gt"
                        | "boolean_and"
                        | "boolean_or"
                        | "shl"
                        | "shr"
                        | "sar"
                ) =>
            {
                let right = self.pop_value()?;
                let left = self.pop_value()?;
                let right = self.value_as_numeric_operand(right)?;
                let left = self.value_as_numeric_operand(left)?;
                let value = match name {
                    "add" => left.wrapping_add(right),
                    "sub" => left.wrapping_sub(right),
                    "mul" => left.wrapping_mul(right),
                    "div" => {
                        if right == 0 {
                            -1
                        } else {
                            // x86 idiv traps on MIN / -1; never panic the host.
                            left.wrapping_div(right)
                        }
                    }
                    "mod" => {
                        if right == 0 {
                            -1
                        } else {
                            left.wrapping_rem(right)
                        }
                    }
                    "and" => left & right,
                    "or" => left | right,
                    "xor" => left ^ right,
                    "shl" => left.wrapping_shl((right & 0x1f) as u32),
                    "shr" => ((left as u32) >> (right & 0x1f)) as i32,
                    "sar" => left >> (right & 0x1f),
                    "leq" => (left <= right) as i32,
                    "geq" => (left >= right) as i32,
                    "lt" => (left < right) as i32,
                    "gt" => (left > right) as i32,
                    "boolean_and" => ((left != 0) && (right != 0)) as i32,
                    "boolean_or" => ((left != 0) || (right != 0)) as i32,
                    _ => 0,
                };
                self.push_value(Value::Int(value));
            }
            BpOpcode::Known { code: 0x28, .. } => {
                let value = self.pop_int()?;
                self.push_value(Value::Int(!value));
            }
            BpOpcode::Known { code: 0x3a, .. } => {
                let value = self.pop_int()?;
                self.push_value(Value::Int((value == 0) as i32));
            }
            BpOpcode::Known { code: 0x40, .. } => {
                let false_value = self.pop_value()?;
                let true_value = self.pop_value()?;
                let condition = self.pop_int()?;
                self.push_value(if condition != 0 {
                    true_value
                } else {
                    false_value
                });
            }
            BpOpcode::Known { code: 0x42, .. } => {
                let divisor = self.pop_int()?;
                let multiplier = self.pop_int()?;
                let multiplicand = self.pop_int()?;
                let value = if divisor == 0 {
                    -1
                } else {
                    ((multiplicand as i64 * multiplier as i64) / divisor as i64) as i32
                };
                self.push_value(Value::Int(value));
            }
            BpOpcode::Known { code: 0x43, .. } => {
                let y = self.pop_int()?;
                let x = self.pop_int()?;
                let mut degrees = (y as f64).atan2(x as f64).to_degrees();
                if degrees < 0.0 {
                    degrees += 360.0;
                }
                self.push_value(Value::Int((degrees * 65_536.0) as i32));
            }
            BpOpcode::Known { code: 0x44, .. } => {
                let z = self.pop_int()? as f64;
                let y = self.pop_int()? as f64;
                let x = self.pop_int()? as f64;
                self.push_value(Value::Int(
                    (x.mul_add(x, y.mul_add(y, z * z)).sqrt()) as i32,
                ));
            }
            BpOpcode::Known { code: 0x48 | 0x49, .. } => {
                let angle = self.pop_int()? as f64;
                let radians = angle * ((std::f64::consts::PI / 180.0) / 65_536.0);
                let value = if code == 0x48 {
                    radians.sin()
                } else {
                    radians.cos()
                };
                self.push_value(Value::Int((value * 65_536.0) as i32));
            }
            BpOpcode::Known { code: 0x50 | 0x51 | 0x52 | 0x53 | 0x54, .. } => {
                self.execute_qword_arithmetic(code)?;
            }
            BpOpcode::Known { code: 0x56 | 0x57, .. } => {
                let value = self.execute_fixed_arithmetic(code)?;
                self.push_value(Value::Int(value));
            }
            BpOpcode::Known { code: 0x58 | 0x59 | 0x5a | 0x5b | 0x5d | 0x5e | 0x5f, .. } => {
                self.execute_fixed_vector(code)?;
            }
            BpOpcode::Known { code: 0x60, .. } => {
                let size = self.pop_int()?.max(0) as usize;
                let src = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                let src = self.normalize_scenario_descriptor_src(src, size);
                let src_range = self.resolve_range(src, size)?;
                let dst_range = self.resolve_write_range(dst, size)?;
                let tmp = self.memory[src_range].to_vec();
                self.memory[dst_range].copy_from_slice(&tmp);
                self.trace_watch_write(dst, size, src, "memcpy");
                self.copy_shadow_values(src, dst, size);
            }
            BpOpcode::Known { code: 0x61, .. } => {
                let size = self.pop_int()?.max(0) as usize;
                let ptr = self.pop_ptr()?;
                let range = self.resolve_write_range(ptr, size)?;
                self.memory[range].fill(0);
                self.trace_watch_write(ptr, size, 0, "memclr");
                self.clear_shadow_values(ptr, size);
                self.restore_script_records(ptr, size)?;
            }
            BpOpcode::Known { code: 0x62, .. } => {
                let value = self.pop_int()? as u8;
                let size = self.pop_int()?.max(0) as usize;
                let ptr = self.pop_ptr()?;
                let range = self.resolve_write_range(ptr, size)?;
                self.memory[range].fill(value);
                self.trace_watch_write(ptr, size, value as u32, "memset");
                self.clear_shadow_values(ptr, size);
                self.restore_script_records(ptr, size)?;
            }
            BpOpcode::Known { code: 0x63, .. } => {
                let size = self.pop_int()?.max(0) as usize;
                let right = self.pop_value()?;
                let left = self.pop_value()?;
                let equal = self.value_as_fixed_bytes(left, size)?
                    == self.value_as_fixed_bytes(right, size)?;
                self.push_value(Value::Int(i32::from(equal)));
            }
            BpOpcode::Known { code: 0x64, .. } => {
                let src = self.pop_ptr()?;
                let count = self.pop_int()?.max(0) as usize;
                let size = self.pop_int()?.max(0) as usize;
                let dst = self.pop_ptr()?;
                let src_range = self.resolve_range(src, size)?;
                let block = self.memory[src_range].to_vec();
                let total = size.saturating_mul(count);
                let dst_range = self.resolve_write_range(dst, total)?;
                for chunk in self.memory[dst_range]
                    .chunks_exact_mut(size.max(1))
                    .take(count)
                {
                    if size != 0 {
                        chunk.copy_from_slice(&block);
                    }
                }
                self.clear_shadow_values(dst, total);
            }
            BpOpcode::Known { code: 0x65, .. } => {
                let needle = self.pop_ptr()?;
                let count = self.pop_int()?.max(0) as usize;
                let size = self.pop_int()?.max(0) as usize;
                let haystack = self.pop_ptr()?;
                let needle_range = self.resolve_range(needle, size)?;
                let needle = self.memory[needle_range].to_vec();
                let total = size.saturating_mul(count);
                let haystack_range = self.resolve_range(haystack, total)?;
                let found = if size == 0 || count == 0 {
                    -1
                } else {
                    self.memory[haystack_range]
                        .chunks_exact(size)
                        .position(|block| block == needle)
                        .map(|index| index as i32)
                        .unwrap_or(-1)
                };
                self.push_value(Value::Int(found));
            }
            BpOpcode::Known { code: 0x66, .. } => {
                let needle = self.pop_native_string_bytes()?;
                let haystack = self.pop_native_string_bytes()?;
                let found = if needle.is_empty() {
                    0
                } else {
                    haystack
                        .windows(needle.len())
                        .position(|window| window == needle.as_slice())
                        .map(|offset| offset as i32)
                        .unwrap_or(-1)
                };
                self.push_value(Value::Int(found));
            }
            BpOpcode::Known { code: 0x67, .. } => {
                // sub_474520 pops replacement, needle, haystack, destination,
                // runs sub_495A80 and pushes the number of replacements.
                let replacement = self.pop_native_string_bytes()?;
                let needle = self.pop_native_string_bytes()?;
                let haystack = self.pop_native_string_bytes()?;
                let dst = self.pop_ptr()?;
                let (replaced, count) = target_strreplace(&haystack, &needle, &replacement);
                self.write_c_string_native_bytes(dst, &replaced)?;
                self.push_value(Value::Int(count));
            }
            BpOpcode::Known { code: 0x68, .. } => {
                // The target runs C strlen over the raw buffer, so bytes that
                // do not survive a Shift-JIS round trip (GBK fan patches)
                // must be counted as stored.
                let bytes = self.pop_native_string_bytes()?;
                self.push_value(Value::Int(bytes.len() as i32));
            }
            BpOpcode::Known { code: 0x69, .. } => {
                let right = self.pop_native_string_bytes()?;
                let left = self.pop_native_string_bytes()?;
                self.push_value(Value::Int((left == right) as i32));
            }
            BpOpcode::Known { code: 0x6a, .. } => {
                let right = self.pop_value()?;
                let left = self.pop_ptr()?;
                self.copy_c_string_value(left, right)?;
            }
            BpOpcode::Known { code: 0x6b, .. } => {
                let right = self.pop_native_string_bytes()?;
                let mut joined = self.pop_native_string_bytes()?;
                let dst = self.pop_ptr()?;
                joined.extend_from_slice(&right);
                self.write_c_string_native_bytes(dst, &joined)?;
            }
            BpOpcode::Known { code: 0x6c, .. } => {
                let ptr = self.pop_ptr()?;
                let start = self.memory_addr(ptr) as usize;
                let first = self.memory.get(start).copied().unwrap_or_default();
                let is_two_byte = matches!(first, 0x81..=0x9f | 0xe0..=0xfc);
                let ch = if is_two_byte {
                    u16::from_be_bytes([
                        first,
                        self.memory.get(start + 1).copied().unwrap_or_default(),
                    ]) as i32
                } else {
                    first as i32
                };
                self.push_value(Value::Int(ch));
                self.push_value(Value::Int(is_two_byte as i32));
                self.push_value(Value::Int(is_sjis_delimiter(ch as u16) as i32));
            }
            BpOpcode::Known { code: 0x6d, .. } => {
                let ptr = self.pop_ptr()?;
                let text = self.read_c_string(ptr)?.to_ascii_lowercase();
                self.write_c_string(ptr, &text)?;
            }
            BpOpcode::Known { code: 0x6e, .. } => {
                self.execute_quote_string()?;
            }
            BpOpcode::Known { code: 0x6f, .. } => {
                let fmt = self.pop_string_lossy()?;
                let dst = self.pop_ptr()?;
                let rendered = self.render_sprintf(&fmt);
                self.write_c_string(dst, &rendered)?;
            }
            BpOpcode::Known { code: 0x70, .. } => {
                let size = self.pop_int()?.max(0) as u32;
                let ptr = self.alloc_heap(size);
                self.push_value(Value::Ptr(ptr));
            }
            BpOpcode::Known { code: 0x71, .. } => {
                let ptr = self.pop_ptr()?;
                let freed = self.free_heap(ptr);
                self.push_value(Value::Int(i32::from(freed)));
            }
            BpOpcode::Known { code: 0x74, .. } => {
                self.execute_set_memory_mode()?;
            }
            BpOpcode::Known { code: 0x75, .. } => {
                let _name = self.pop_string_lossy()?;
                let _size = self.pop_int()?;
                let _start = self.pop_int()?;
                self.push_value(Value::Int(1));
            }
            BpOpcode::Known { code: 0x77, .. } => {
                let selector = self.pop_int()?;
                // sub_443180 exposes native renderer/debug fields. They have no
                // portable host equivalent; the observed dbgmngr selector 0 is
                // the idle wheel/state counter and starts at zero.
                let value = if matches!(selector, 0..=7 | 16 | 17) {
                    0
                } else {
                    -1
                };
                self.push_value(Value::Int(value));
            }
            BpOpcode::Known { code: 0x78, .. } => {
                // sub_474FF0: default-button flag first, then the text; the
                // Yes/No box result (Yes == IDYES) is pushed. A windowless
                // runtime has no one to ask, so it answers Yes.
                let default_yes = self.pop_int()? != 0;
                let message = self.pop_string_lossy().unwrap_or_default();
                if self.begin_host_continuation(
                    api,
                    HostRequest::Confirmation {
                        text: message.clone(),
                        default_yes,
                    },
                    1,
                )? {
                    self.pc = next_pc;
                    return Ok(());
                }
                tracing::warn!(%message, "BP confirm answered Yes without a dialog");
                self.push_value(Value::Int(1));
            }
            BpOpcode::Known { code: 0x79, .. } => {
                let message = self
                    .pop_string_lossy()
                    .unwrap_or_else(|_| "<message>".into());
                if self.begin_host_continuation(
                    api,
                    HostRequest::Message {
                        text: message.clone(),
                    },
                    0,
                )? {
                    self.pc = next_pc;
                    return Ok(());
                }
                tracing::warn!(%message, "BP message_box");
            }
            BpOpcode::Known { code: 0x7a, .. } => {
                // sub_4750A0 only displays "Number : %d ( $%.8x )"; it never
                // asserts and never terminates the script.
                let value = self.pop_int()?;
                tracing::warn!(value, "BP show_number: Number : {value} ( ${value:08x} )");
            }
            BpOpcode::Known { code: 0x7b, .. } => {
                // sub_475130 pops byte count, data pointer, title pointer.
                let size = self.pop_int()?;
                let _data = self.pop_value()?;
                let title = self.pop_string_lossy().unwrap_or_default();
                if !(1..=0x400).contains(&size) {
                    return Err(VmError::Runtime(format!(
                        "dumpmem size {size} outside 1..=1024"
                    )));
                }
                tracing::warn!(%title, size, "BP dumpmem");
            }
            BpOpcode::Known { code: 0x7c, .. } => {
                let result = self.execute_modal_list()?;
                self.push_value(Value::Int(result));
            }
            BpOpcode::Known { code: 0x7d, .. } => {
                self.execute_resource_transform()?;
            }
            BpOpcode::Known { code: 0x7e, .. } => {
                let result = self.execute_clipboard_set()?;
                self.push_value(Value::Int(result));
            }
            BpOpcode::Known { code: 0x7f, .. } => {
                self.execute_resource_blend()?;
            }
            BpOpcode::Known { code: 0x80 | 0x81, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("sys", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !self.effective_strict_support(NativeOpcode { group: code, id })
                {
                    self.note_stub("sys", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                let opcode = NativeOpcode { group: code, id };
                if (code, id) == (0x80, 0x3c) {
                    self.resolve_sys80_3c_file(api)?;
                }
                if api.host_requests_enabled() {
                    if self.intercept_native_host_request(api, opcode)? {
                        self.pc = next_pc;
                        return Ok(());
                    }
                }
                let result = if let Some(result) = self.dispatch_scheduler_opcode(
                    api,
                    opcode,
                    program_index,
                    instruction,
                    trace_events,
                )? {
                    result
                } else if let Some(result) = self.dispatch_input_opcode(api, opcode)? {
                    result
                } else if let Some(result) =
                    self.dispatch_program_thread_opcode(api, opcode, trace_events)?
                {
                    result
                } else if (code, id) == (0x80, 0x30) {
                    self.sys80_30_read_file_bytes(api)?
                } else if (code, id) == (0x80, 0x31) {
                    self.sys80_31_read_file_range(api)?
                } else if (code, id) == (0x80, 0x32) {
                    self.sys80_32_write_file(api)?
                } else if (code, id) == (0x80, 0x34) {
                    self.sys80_34_file_exists(api)?
                } else if (code, id) == (0x80, 0x35) {
                    self.sys80_35_file_size(api)?
                } else if (code, id) == (0x80, 0x3e) {
                    // sub_489650: an existing directory becomes ::Buffer as
                    // sprintf("%s\\"), which appends even after a separator.
                    let path = self.pop_string_lossy()?;
                    let valid = api.directory_exists(&path);
                    if valid {
                        self.primary_resource_root = Some(format!("{path}\\"));
                    }
                    Value::Int(i32::from(valid))
                } else if (code, id) == (0x80, 0x3f) {
                    self.sys80_3f_find_media(api)?
                } else if (code, id) == (0x80, 0x45) {
                    // The target table points at __RTC_NumErrors, but the
                    // dispatch ABI exposes no result; this selector is a
                    // BP-visible no-op in the shipped release image.
                    Value::None
                } else if (code, id) == (0x80, 0x50) {
                    // sub_489B30 stores dword_507688 and returns interpreter
                    // status 1: the thread's slice ends like Sys80:5F.
                    self.system_wait_state = self.pop_int()?;
                    self.scheduler_signal = Some(SchedulerSignal::Yield);
                    tracing::debug!(
                        value = self.system_wait_state,
                        "Sys80_50_SetSystemWaitState"
                    );
                    Value::None
                } else if (code, id) == (0x80, 0xa0) {
                    let destination = self.pop_ptr()?;
                    if let Some(event) = api.poll_queued_event() {
                        for (index, value) in event.into_iter().enumerate() {
                            self.write_int(
                                destination.wrapping_add(index as u32 * 4),
                                2,
                                value as u32,
                            )?;
                        }
                        Value::Int(1)
                    } else {
                        Value::Int(0)
                    }
                } else if (code, id) == (0x80, 0xa1) {
                    let parameter = self.pop_int()?;
                    let event_code = self.pop_int()?;
                    api.post_queued_event(event_code, parameter);
                    Value::None
                } else if (code, id) == (0x80, 0xa9) {
                    // sub_48A200: pops the output pointer, then the handle;
                    // writes DCIndProc+0x10 and pushes whether it exists.
                    let output = self.pop_ptr()?;
                    let object = self.pop_int()?;
                    match api.registered_object_value(object) {
                        Some(state) => {
                            self.write_int(output, 2, state as u32)?;
                            self.clear_shadow_values(output, 4);
                            Value::Int(1)
                        }
                        None => Value::Int(0),
                    }
                } else if (code, id) == (0x80, 0xa8) {
                    let state = self.pop_int()?;
                    let object = self.pop_int()?;
                    Value::Int(i32::from(api.set_registered_object_value(object, state)))
                } else if (code, id) == (0x80, 0xac) {
                    let descriptor = self.pop_value()?;
                    let count = self.pop_int()?;
                    let object = self.pop_int()?;
                    let valid_count = (1..=256).contains(&count);
                    let registered = api.registered_object_value(object).is_some();
                    if !registered {
                        Value::Int(0)
                    } else {
                        if valid_count {
                            let descriptor_values =
                                self.read_descriptor_values(descriptor.clone(), count as usize)?;
                            api.dispatch_object_event(object, count, &descriptor_values)?;
                            if trace_events {
                                tracing::info!(
                                    pc = self.pc,
                                    offset = format_args!("0x{:08X}", instruction.offset),
                                    object,
                                    count,
                                    descriptor = ?descriptor,
                                    values = ?descriptor_values.iter().map(value_summary).collect::<Vec<_>>(),
                                    "VM QueueRegisteredObjectMessage"
                                );
                            }
                        }
                        // sub_48A250 returns whether the registry lookup
                        // succeeded; the variable-length queue helper's
                        // count validation result is deliberately ignored.
                        Value::Int(1)
                    }
                } else if (code, id) == (0x80, 0xaf) {
                    let mode = self.pop_int()?;
                    let valid = (0..=1).contains(&mode) && api.set_system_mode_flag(mode);
                    if valid {
                        self.system80_shared
                            .lock()
                            .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                            .system_mode_flag = mode as u32;
                    }
                    Value::Int(i32::from(valid))
                } else if let Some(result) = self.try_builtin_sys_with_api(api, code, id)? {
                    if trace_events
                        && matches!(id, 0x47 | 0x48 | 0x9d | 0xac | 0xd0 | 0xd2 | 0xd4 | 0xdd)
                    {
                        tracing::info!(
                            pc = self.pc,
                            offset = format_args!("0x{:08X}", instruction.offset),
                            group = format_args!("0x{code:02X}"),
                            id = format_args!("0x{id:02X}"),
                            result = ?result,
                            stack_top = ?self.stack_summary(8),
                            "VM builtin sys"
                        );
                    }
                    result
                } else {
                    if trace_events {
                        let audited = !matches!(
                            native_call::recovery_level(NativeOpcode { group: code, id }),
                            native_call::NativeRecoveryLevel::CandidateNameOnly
                                | native_call::NativeRecoveryLevel::Unrecovered
                        );
                        if audited {
                            tracing::debug!(
                                program = self.program_name(program_index),
                                pc = self.pc,
                                offset = format_args!("0x{:08X}", instruction.offset),
                                group = format_args!("0x{code:02X}"),
                                id = format_args!("0x{id:02X}"),
                                stack_len = self.stack.len(),
                                stack_top = ?self.stack_summary(8),
                                "VM runtime sys fallback"
                            );
                        } else {
                            tracing::warn!(
                                program = self.program_name(program_index),
                                pc = self.pc,
                                offset = format_args!("0x{:08X}", instruction.offset),
                                group = format_args!("0x{code:02X}"),
                                id = format_args!("0x{id:02X}"),
                                stack_len = self.stack.len(),
                                stack_top = ?self.stack_summary(8),
                                "VM runtime sys fallback"
                            );
                        }
                    }
                    self.normalize_sys_string_args(code, id)?;
                    let call_stack = self.take_dispatch_call_frame(code, id)?;
                    let mut call =
                        NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                    let result = api.call_sys(&mut call)?;
                    let mut call_stack = call.into_args();
                    let result = self.settle_native_call_outputs(code, id, &mut call_stack, result);
                    self.audit_native_args_consumed(
                        "sys",
                        code,
                        id,
                        call_stack.len(),
                        program_index,
                        fail_on_stub,
                    )?;
                    result
                };
                self.ensure_unrecovered_native_procedure_boundary(
                    NativeOpcode { group: code, id },
                    trace_events,
                );
                if api.take_runtime_stub() {
                    self.note_stub("sys", code, id);
                    if fail_on_stub {
                        return Err(VmError::UnknownDispatch { group: code, id });
                    }
                }
                let result = self.enforce_native_output_contract("sys", code, id, result);
                self.audit_native_return("sys", code, id, &result, program_index, fail_on_stub)?;
                if result != Value::None {
                    self.push_value(result);
                }
            }
            BpOpcode::Known {
                name: "script_load",
                ..
            } => {
                let file = self.pop_string_lossy()?;
                let archive = self.pop_string_lossy()?;
                let slot = self.pop_int()?;
                self.note_call("script", 0xff, 0xf0);
                if !(0..0xf0).contains(&slot) {
                    return Err(VmError::UnknownDispatch {
                        group: 0xff,
                        id: slot as u16,
                    });
                }
                let mut program = api
                    .load_program(&archive, &file)
                    .unwrap_or_else(|| empty_loaded_program(format!("{archive}:{file}")));
                self.assign_program_instance(&mut program);
                self.mediation_programs.insert(slot as u8, program);
                tracing::info!(slot, archive, file, "BP mediation program registered");
            }
            BpOpcode::Known {
                name: "script_free",
                ..
            } => {
                let slot = self.pop_int()?;
                self.note_call("script", 0xff, 0xf1);
                if let Some(program) = self.mediation_programs.remove(&(slot as u8)) {
                    api.free_program(Value::Program(Arc::new(program)));
                }
            }
            BpOpcode::Known {
                name: "script_ret", ..
            } => {
                if self.mem_ptr <= self.thread.frame_region_base() {
                    self.halted = true;
                } else if let Some((program_id, fallback_ret, stack_base)) = self.call_stack.pop() {
                    let ret_offset = self.read_return_addr()?;
                    if trace_call_frames_enabled(self.trace_id) {
                        eprintln!(
                            "TRACE_CALL_FRAME vm={} callee={} caller={} stack_base={} stack_return={} delta={:+}",
                            self.trace_id,
                            self.program_name(self.current_program),
                            self.program_name(program_id),
                            stack_base,
                            self.stack.len(),
                            self.stack.len() as isize - stack_base as isize
                        );
                    }
                    self.current_program = program_id;
                    next_pc = self
                        .jump_target_index(program_id, ret_offset)
                        .unwrap_or(fallback_ret);
                } else {
                    self.halted = true;
                }
            }
            BpOpcode::Known {
                name: "script_call",
                ..
            } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default();
                self.note_call("script", 0xff, id as u16);
                let Some(program) = self.mediation_programs.get(&id).cloned() else {
                    self.note_stub("script", 0xff, id as u16);
                    return Err(VmError::UnknownDispatch {
                        group: 0xff,
                        id: id as u16,
                    });
                };
                let dest_program_index = self.program_index_for_loaded_program(program);
                if std::env::var_os("TRACE_MEDIATION_ARGS").is_some() {
                    tracing::warn!(
                        vm = self.trace_id,
                        caller = self.program_name(self.current_program),
                        caller_pc = self.pc,
                        caller_offset = format_args!("0x{:08X}", instruction.offset),
                        slot = id,
                        callee = self.program_name(dest_program_index),
                        stack = self.stack.len(),
                        stack_top = ?self.stack_summary(12),
                        "TRACE_MEDIATION_ARGS"
                    );
                }
                if trace_events {
                    tracing::info!(
                        slot = id,
                        program = self.program_name(dest_program_index),
                        stack_top = ?self.stack_summary(8),
                        "BP mediation program call"
                    );
                }
                self.write_return_addr(self.current_program, next_pc)?;
                self.call_stack
                    .push((self.current_program, next_pc, self.stack.len()));
                self.current_program = dest_program_index;
                next_pc = self.jump_target_index(dest_program_index, 0x10)?;
            }
            BpOpcode::Known { code: 0x90 | 0x91 | 0x92, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("graph", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !self.effective_strict_support(NativeOpcode { group: code, id })
                {
                    self.note_stub("graph", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                let result = if self.native_abi_profile == NativeAbiProfile::Amachoco
                    && (code, id) == (0x91, 0x09)
                {
                    // amachoco.exe sub_48AB80 -> sub_464380.
                    let mode = self.pop_int()?;
                    let valid = (0..=1).contains(&mode);
                    if valid {
                        self.system81_shared.lock().expect("system81 state poisoned")
                            .graph91_resource_mode = mode;
                    }
                    Value::Int(i32::from(valid))
                } else if self.native_abi_profile == NativeAbiProfile::Amachoco
                    && (code, id) == (0x92, 0x99)
                {
                    self.handle_character_widths_call(api)?
                } else if (code, id) == (0x90, 0xa1) {
                    // sub_47F0F0 pops colour, layout, column count, the item
                    // string table, item count and the Window. sub_491F30
                    // checks the count (1..=16) before the column count (at
                    // most 16); every failure is a script error. A column
                    // count below 1 makes sub_43BA30 divide by zero.
                    let color = self.pop_int()?;
                    let layout = self.pop_int()?;
                    let columns = self.pop_int()?;
                    let table = self.pop_ptr()?;
                    let count = self.pop_int()?;
                    let window = self.pop_int()?;
                    if !(1..=16).contains(&count) {
                        return Err(VmError::Runtime(format!(
                            "無効な項目数 [ {count} ] が指定されました"
                        )));
                    }
                    if !(1..=16).contains(&columns) {
                        return Err(VmError::Runtime(format!(
                            "無効な縦列数 [ {columns} ] が指定されました"
                        )));
                    }
                    let items = self.read_counted_string_pointer_list(table, count as usize)?;
                    if !api.draw_item_selection_grid(window, &items, columns, layout != 0, color) {
                        return Err(VmError::Runtime(
                            "無効なウィンドウハンドルが指定されました".into(),
                        ));
                    }
                    Value::None
                } else if (code,id)==(0x90,0x78) {
                    let pointer=self.pop_ptr()?;
                    let height=self.pop_int()? as u32;
                    let width=self.pop_int()? as u32;
                    let object=self.pop_int()?;
                    let count=width.checked_mul(height).filter(|n|*n!=0).ok_or_else(||VmError::Runtime("90:78: invalid tile table dimensions".into()))?;
                    let bytes=count.checked_mul(2).ok_or_else(||VmError::Runtime("90:78: tile table size overflow".into()))?;
                    // Validate the entire range before allocating a host vector.
                    self.resolve_range(pointer,bytes as usize)?;
                    let mut tiles=Vec::with_capacity(count as usize);
                    for i in 0..count {tiles.push(self.read_int(pointer.wrapping_add(i*2),1)? as u16);}
                    if !api.upload_map_tile_data(object,width,height,&tiles){return Err(VmError::Runtime("90:78: invalid map or tile dimensions".into()));}
                    Value::None
                } else if (code,id)==(0x90,0x44) {
                    let alpha=self.pop_int()?;
                    let table=self.pop_ptr()?;
                    let count=self.pop_int()?;
                    if !(2..=32).contains(&count) {return Err(VmError::Runtime(format!("Graph90:44 invalid frame count {count}")));}
                    let mut frames=Vec::with_capacity(count as usize);
                    for index in 0..count {frames.push(self.read_int(table.wrapping_add(index as u32*4),2)? as i32);}
                    api.configure_background_frames(&frames,alpha)?;
                    Value::None
                } else if code == 0x90 && matches!(id, 0xA0 | 0xA2 | 0xA3 | 0xB0 | 0xB1) {
                    let opcode = NativeOpcode { group: code, id };
                    let args = self.take_dispatch_call_frame(code, id)?;
                    let request = self.decode_native_selection(api, opcode, &args)?;
                    let window = request.window;
                    let selection_id = api.start_native_selection(request)?;
                    let class_name = match id {
                        0xA0 => "CProcSelectItem", 0xA2 => "CProcSelectItemEx",
                        0xA3 => "CProcSelectItemExBlink", 0xB0 => "CProcSelectIcon",
                        _ => "CProcSelectIconEx",
                    };
                    self.install_cprocedure(InstalledCProcedure::graph(
                        self.thread.thread_id(), opcode, class_name,
                        native_thread::NativeGraphProcedureMode::Select,
                        Some(window), Some(selection_id),
                    ), trace_events);
                    Value::None
                } else if (code, id) == (0x90, 0xbc) {
                    let object = self.pop_value()?;
                    let state_buffer = self.pop_ptr()?;
                    let exists = api.graph_input_object_exists(object.as_i32());
                    if exists {
                        let state = api.poll_object_state_record(object.as_i32());
                        for (index, value) in state.into_iter().enumerate() {
                            self.write_int(
                                state_buffer.wrapping_add((index * 4) as u32),
                                2,
                                value as u32,
                            )?;
                        }
                        if self.native_abi_profile==NativeAbiProfile::Amachoco && api.input_gate_diagnostics_enabled() && state_buffer==(FRAME_SEGMENT|self.mem_ptr.wrapping_sub(0x64)) {
                            let base=FRAME_SEGMENT|self.mem_ptr;
                            let read=|offset:u32|self.read_int(base.wrapping_sub(offset),2).unwrap_or(u32::MAX);
                            let event=read(0x70);
                            if event!=0 {api.record_input_gate([self.pc as u32,self.mem_ptr,event,read(0x6c),read(0x68),read(0x44),read(0x40),state[0]as u32,self.read_int(1092,2).unwrap_or(u32::MAX)]);}
                        }
                    }
                    Value::Int(i32::from(exists))
                } else if (code, id) == (0x90, 0xbd) {
                    let object = self.pop_value()?;
                    let destination = self.pop_ptr()?;
                    let value = api.graph_input_registered_state(object.as_i32());
                    if let Some(value) = value {
                        self.write_int(destination, 2, value as u32)?;
                    }
                    Value::Int(i32::from(value.is_some()))
                } else if (code, id) == (0x90, 0xbe) {
                    let object = self.pop_value()?;
                    let destination = self.pop_ptr()?;
                    let values = api.graph_input_region_values(object.as_i32());
                    if let Some(values) = values.as_ref() {
                        for (index, value) in values.iter().copied().enumerate() {
                            self.write_int(
                                destination.wrapping_add((index * 4) as u32),
                                2,
                                value as u32,
                            )?;
                        }
                    }
                    Value::Int(i32::from(values.is_some()))
                } else if (code, id) == (0x90, 0xbf) {
                    let object = self.pop_value()?;
                    let event_buffer = self.pop_ptr()?;
                    let exists = api.graph_input_object_exists(object.as_i32());
                    if exists {
                        // sub_47F000 -> sub_448560: pop one event (zeros when
                        // the queue is empty); nothing else is written.
                        let event = api.poll_object_event_record(object.as_i32());
                        for (index, value) in event.into_iter().enumerate() {
                            self.write_int(
                                event_buffer.wrapping_add((index * 4) as u32),
                                2,
                                value as u32,
                            )?;
                        }
                    }
                    Value::Int(i32::from(exists))
                } else if (code, id) == (0x90, 0x14) {
                    let pixels = self.pop_ptr()?;
                    let format = self.pop_int()?;
                    let height = self.pop_int()?;
                    let width = self.pop_int()?;
                    let bitmap = self.pop_int()?;
                    // sub_47A5C0: sub_4986D0 first.
                    if bitmap as u32 >= 0x4000 {
                        return Err(VmError::Runtime(format!(
                            "無効なビットマップ番号 [ {bitmap} ] が指定されました"
                        )));
                    }
                    // sub_4080B0: format 1 (and 7, which sub_407DA0 maps to
                    // 1) reads packed 3-byte B,G,R pixels into XRGB dwords;
                    // every other format is a tightly packed copy using the
                    // registry pixel size of 0x4E41B0.
                    const FORMAT_BYTES: [usize; 8] = [2, 4, 4, 1, 4, 4, 6, 4];
                    let source_bpp = match format {
                        1 | 7 => 3,
                        0..=6 => FORMAT_BYTES[format as usize],
                        _ => 0,
                    };
                    let byte_count = (width.max(0) as usize)
                        .checked_mul(height.max(0) as usize)
                        .and_then(|pixels| pixels.checked_mul(source_bpp))
                        .ok_or_else(|| VmError::Runtime("bitmap pixel size overflow".into()))?;
                    let range = self.resolve_range(pixels, byte_count)?;
                    let bytes = self.memory[range].to_vec();
                    api.create_bitmap_from_rgb(bitmap, width, height, format, &bytes);
                    Value::None
                } else if (code, id) == (0x90, 0x15) {
                    // sub_47A630 -> sub_4081F0: an unknown bitmap (9) or a
                    // buffer smaller than the packed image (10) is fatal.
                    let bitmap = self.pop_int()?;
                    let capacity = self.pop_int()? as u32;
                    let written = self.pop_ptr()?;
                    let destination = self.pop_ptr()?;
                    if bitmap as u32 >= 0x4000 {
                        return Err(VmError::Runtime(format!(
                            "無効なビットマップ番号 [ {bitmap} ] が指定されました"
                        )));
                    }
                    if api.query_bitmap_info(bitmap).is_none() {
                        return Err(VmError::Runtime(format!(
                            "指定されたビットマップ [ {bitmap} ] は存在しません"
                        )));
                    }
                    let Some(bytes) = api.read_bitmap_pixels(bitmap, capacity as usize) else {
                        return Err(VmError::Runtime(format!(
                            "指定されたイメージの格納先のサイズ [ {} ] ではイメージを取得できません",
                            capacity as i32
                        )));
                    };
                    let range = self.resolve_write_range(destination, bytes.len())?;
                    self.memory[range].copy_from_slice(&bytes);
                    self.clear_shadow_values(destination, bytes.len());
                    self.write_int(written, 2, bytes.len() as u32)?;
                    Value::None
                } else if (code, id) == (0x90, 0x16) {
                    let bitmap = self.pop_value()?.as_i32();
                    let destination = self.pop_ptr()?;
                    let info = api.query_bitmap_info(bitmap);
                    // sub_407F20 writes six DWORDs through hidden ESI. The
                    // public handler then clears field zero and returns found.
                    self.write_int(destination, 2, 0)?;
                    if let Some(info) = info {
                        self.write_int(destination.wrapping_add(4), 2, info.row_stride)?;
                        self.write_int(destination.wrapping_add(8), 2, info.width)?;
                        self.write_int(destination.wrapping_add(12), 2, info.height)?;
                        self.write_int(destination.wrapping_add(16), 2, info.format)?;
                        self.write_int(destination.wrapping_add(20), 2, info.bytes_per_pixel)?;
                    }
                    Value::Int(i32::from(info.is_some()))
                } else if (code, id) == (0x90, 0xC6) {
                    // sub_47F420 pops size, data pointer, name and namespace.
                    // The target validates BG format before inserting a
                    // lower-cased two-key cache entry.
                    let size = self.pop_int()?.max(0) as usize;
                    let source = self.pop_ptr()?;
                    let name = self.pop_string_lossy()?;
                    let namespace = self.pop_string_lossy()?;
                    let range = self.resolve_range(source, size)?;
                    let bytes = self.memory[range].to_vec();
                    Value::Int(i32::from(
                        api.register_bg_resource_data(&namespace, &name, &bytes),
                    ))
                } else if (code, id) == (0x90, 0xCC) {
                    // sub_47F760 pops the optional three-point curve pointer
                    // before the tone-curve identifier.
                    let points = self.pop_ptr()?;
                    let identifier = self.pop_int()?;
                    if points == 0 {
                        // sub_405420 -> sub_40A250: removing an unknown curve
                        // is fatal (0x8000000F); invalid control points
                        // (0x80000016) are ignored by the handler.
                        if !api.remove_graph_color_lut(identifier) {
                            return Err(VmError::Runtime(format!(
                                "登録先に指定されたトーンカーブの識別番号 [ {identifier} ] は無効です"
                            )));
                        }
                    } else {
                        let mut control = [[0i32; 2]; 3];
                        for (index, pair) in control.iter_mut().enumerate() {
                            pair[0] =
                                self.read_int(points.wrapping_add((index * 8) as u32), 2)? as i32;
                            pair[1] = self
                                .read_int(points.wrapping_add((index * 8 + 4) as u32), 2)?
                                as i32;
                        }
                        let _ = api.register_graph_color_lut(identifier, control);
                    }
                    Value::None
                } else if (code, id) == (0x90, 0xCE) {
                    // sub_47F9A0 pops parameter, format, bitmap, size pointer
                    // and destination pointer in this order.
                    let parameter = self.pop_int()?;
                    let format = self.pop_int()?;
                    let bitmap = self.pop_int()?;
                    let size_out = self.pop_ptr()?;
                    let destination = self.pop_ptr()?;
                    match api.encode_graph_bitmap(bitmap, format, parameter) {
                        Ok(bytes) => {
                            self.write_int(size_out, 2, bytes.len() as u32)?;
                            if destination != 0 {
                                let range = self.resolve_write_range(destination, bytes.len())?;
                                self.memory[range].copy_from_slice(&bytes);
                                self.clear_shadow_values(destination, bytes.len());
                            }
                        }
                        Err(code) => {
                            // sub_4804A0: every failure is a script error.
                            let message = match code as u32 {
                                0x8000_0006 => format!(
                                    "指定されたビットマップ [ {bitmap} ] はピクセル数が多過ぎます"
                                ),
                                0x8000_0008 => "必要なメモリが確保できませんでした".to_string(),
                                0x8000_000A => format!("指定されたビットマップ [ {bitmap} ] は無効です"),
                                0x8000_0011 => format!(
                                    "指定されたビットマップ [ {bitmap} ] のピクセルモードはサポートされていません"
                                ),
                                0x8000_0017 => format!("無効な形式 [ {format} ] が指定されました"),
                                0x8000_0018 => format!("無効なパラメータ [ {format} ] が指定されました"),
                                0xFFFF_FFFE => "エンコードに失敗しました".to_string(),
                                _ => "想定外のエラーが発生しました".to_string(),
                            };
                            return Err(VmError::Runtime(message));
                        }
                    }
                    Value::None
                } else if (code, id) == (0x91, 0x38) {
                    // sub_481A90 pops parameter number and object before
                    // converting the first script argument to a writable BP
                    // pointer. The subclass virtual writes one DWORD.
                    let parameter = self.pop_int()?;
                    let object = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let input = self.read_int(destination, 2).unwrap_or(0) as i32;
                    // sub_481A90 turns every non-zero status into a script
                    // error (object missing, unsupported or rejected).
                    let values = api
                        .query_graph91_object_property(object, parameter, input)
                        .map_err(|status| {
                            // sub_482590: 5 unsupported, 254 bad argument,
                            // 255 unknown object.
                            VmError::Runtime(match status {
                                5 => format!(
                                    "サポートされていないパラメータ番号 [ 0x{:08X} ] が設定されました",
                                    parameter as u32
                                ),
                                255 => "無効なオブジェクトハンドルが指定されました".to_string(),
                                _ => format!(
                                    "取得対象パラメータ [ 0x{:08X} ] に渡された引数 [ {input} ( 0x{:08X} ) ] に誤りがあります",
                                    parameter as u32, input as u32
                                ),
                            })
                        })?;
                    for (index, value) in values.iter().enumerate() {
                        let address = destination.wrapping_add((index * 4) as u32);
                        self.write_int(address, 2, *value as u32)?;
                    }
                    self.clear_shadow_values(destination, values.len() * 4);
                    Value::None
                } else if (code, id) == (0x91, 0x3D) {
                    // sub_481B50 pops the object and converts the first BP
                    // argument. sub_41B260 writes exactly two resolved
                    // coordinate DWORDs through that pointer.
                    let object = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let Some(position) = api.query_graph91_object_composite_position(object) else {
                        // sub_482650 -> sub_462650: 255 is fatal.
                        return Err(VmError::Runtime(
                            "無効なオブジェクトハンドルが指定されました".into(),
                        ));
                    };
                    self.write_int(destination, 2, position[0] as u32)?;
                    self.write_int(destination.wrapping_add(4), 2, position[1] as u32)?;
                    self.clear_shadow_values(destination, 8);
                    Value::None
                } else if (code, id) == (0x91, 0x73) {
                    // sub_482DA0 pops alpha-test mode and landscape, then
                    // writes the hit line/column pair through the first BP argument.
                    let alpha_test_mode = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let hit = api.graph91_landscape_hit_test(landscape, alpha_test_mode)?;
                    if let Some([line, column]) = hit {
                        self.write_int(destination, 2, line as u32)?;
                        self.write_int(destination.wrapping_add(4), 2, column as u32)?;
                        self.clear_shadow_values(destination, 8);
                    }
                    Value::Int(i32::from(hit.is_some()))
                } else if (code, id) == (0x91, 0x78) {
                    // Five DWORDs per part and 34 DWORDs per pillar/column.
                    let columns_ptr = self.pop_ptr()?;
                    let column_count = self.pop_int()?.max(0);
                    let part_spacing = self.pop_int()?;
                    let parts_ptr = self.pop_ptr()?;
                    let part_count = self.pop_int()?.max(0);
                    let bitmap = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    self.resolve_range(parts_ptr,part_count as usize*20)?;
                    self.resolve_range(columns_ptr,column_count as usize*136)?;
                    let mut part_words = Vec::with_capacity(part_count as usize * 5);
                    for index in 0..part_count as usize * 5 {
                        part_words.push(
                            self.read_int(parts_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    let mut column_words = Vec::with_capacity(column_count as usize * 34);
                    for index in 0..column_count as usize * 34 {
                        column_words.push(
                            self.read_int(columns_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    api.configure_graph91_landscape_parts(
                        landscape,
                        bitmap,
                        part_count,
                        &part_words,
                        part_spacing,
                        column_count,
                        &column_words,
                    )?;
                    Value::None
                } else if (code, id) == (0x91, 0x79) {
                    let map_ptr = self.pop_ptr()?;
                    let height = self.pop_int()?;
                    let width = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let count = if (1..=256).contains(&width) && (1..=256).contains(&height) {
                        width as usize * height as usize
                    } else {
                        0
                    };
                    self.resolve_range(map_ptr,count*4)?;
                    let mut map = Vec::with_capacity(count);
                    for index in 0..count {
                        map.push(self.read_int(map_ptr.wrapping_add((index * 4) as u32), 2)? as i32);
                    }
                    api.configure_graph91_landscape_map(landscape, width, height, &map)?;
                    Value::None
                } else if (code, id) == (0x91, 0x7A) {
                    let guides_ptr = self.pop_ptr()?;
                    let guide_count = self.pop_int()?.max(0);
                    let bitmap = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    self.resolve_range(guides_ptr,guide_count as usize*20)?;
                    let mut words = Vec::with_capacity(guide_count as usize * 5);
                    for index in 0..guide_count as usize * 5 {
                        words.push(
                            self.read_int(guides_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    api.configure_graph91_landscape_guides(landscape, bitmap, guide_count, &words)?;
                    Value::None
                } else if (code, id) == (0x91, 0x7B) {
                    let value = self.pop_int()?;
                    let guide = self.pop_int()?;
                    let layer = self.pop_int()?;
                    let pairs_ptr = self.pop_ptr()?;
                    let point_count = self.pop_int()?.max(0);
                    let landscape = self.pop_int()?;
                    self.resolve_range(pairs_ptr,point_count as usize*8)?;
                    let mut pairs = Vec::with_capacity(point_count as usize * 2);
                    for index in 0..point_count as usize * 2 {
                        pairs.push(
                            self.read_int(pairs_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    api.set_graph91_landscape_cell_guides(landscape, &pairs, layer, guide, value)?;
                    Value::None
                } else if (code, id) == (0x91, 0x7E) {
                    let column = self.pop_int()?;
                    let line = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let value = api.query_graph91_landscape_cell_value(landscape, line, column)?;
                    if let Some(value) = value {
                        self.write_int(destination, 2, value as u32)?;
                        self.clear_shadow_values(destination, 4);
                    }
                    Value::Int(i32::from(value.is_some()))
                } else if (code, id) == (0x91, 0x03) {
                    // sub_480680 -> sub_401ED0 -> sub_439930 writes a sized
                    // binary payload into the native two-key graph cache.
                    let size = self.pop_int()?.max(0) as usize;
                    let source = self.pop_ptr()?;
                    let name = self.pop_string_lossy()?;
                    let namespace = self.pop_string_lossy()?;
                    let range = self.resolve_range(source, size)?;
                    let bytes = self.memory[range].to_vec();
                    Value::Int(if api.cache_graph_blob(&namespace, &name, &bytes) {
                        0
                    } else {
                        -1
                    })
                } else if (code, id) == (0x91, 0xF1) {
                    // sub_485100 pops the process handle, converts the first
                    // script argument to an output pointer, then starts the
                    // media process. sub_44D110 writes its duration in
                    // milliseconds through that pointer on success.
                    let process = self.pop_int()?;
                    let duration = self.pop_ptr()?;
                    let invocation = api.invoke_graph_effect_process(process);
                    if invocation.status == 0 {
                        self.write_int(duration, 2, invocation.duration_ms as u32)?;
                    }
                    Value::Int(invocation.status)
                } else if (code, id) == (0x91, 0xF7) {
                    // sub_4853F0 -> sub_407FB0 writes the DirectShow media
                    // position through the converted first source argument.
                    // The public return value is a boolean success flag.
                    let bitmap = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    match api.query_movie_slot_position(bitmap) {
                        Some(position) => {
                            self.write_int(destination, 2, position as u32)?;
                            Value::Int(1)
                        }
                        None => Value::Int(0),
                    }
                } else if (code, id) == (0x92, 0x12) {
                    // funcs_486FEE[0x12] -> sub_4857F0 -> sub_402440.
                    // These are descriptor auxiliary DWORDs +0x28/+0x2c,
                    // not bitmap dimensions. Native pop order is second,
                    // first, bitmap.
                    let second = self.pop_int()?;
                    let first = self.pop_int()?;
                    let bitmap = self.pop_int()?;
                    Value::Int(i32::from(
                        api.set_bitmap_auxiliary_pair(bitmap, first, second),
                    ))
                } else if (code, id) == (0x92, 0x16) {
                    // sub_485910 -> sub_402470 writes the same two descriptor
                    // auxiliary DWORDs to the caller-owned BP buffer.
                    let bitmap = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let pair = api.query_bitmap_auxiliary_pair(bitmap);
                    if let Some([first, second]) = pair {
                        self.write_int(destination, 2, first as u32)?;
                        self.write_int(destination.wrapping_add(4), 2, second as u32)?;
                    }
                    Value::Int(i32::from(pair.is_some()))
                } else if (code, id) == (0x92, 0x17) {
                    // funcs_486FEE[0x17] -> sub_485950 -> sub_4025E0.
                    // The destination DWORD is cleared before copying the
                    // native pixel's one-to-four bytes into it.
                    // Only a pixel that passed the checks clears the DWORD.
                    let y = self.pop_int()?;
                    let x = self.pop_int()?;
                    let bitmap = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let status = if let Some(info) = api.query_bitmap_info(bitmap) {
                        if info.format == 6 {
                            2
                        } else if x < 0
                            || y < 0
                            || x as u32 >= info.width
                            || y as u32 >= info.height
                        {
                            3
                        } else if let Some([r, g, b, a]) = api.read_bitmap_pixel(bitmap, x, y) {
                            // App rasters keep format 3 coverage in red and
                            // format 0 as 5-bit channels.
                            let value = match info.format {
                                0 => {
                                    (u32::from(r >> 3) << 10)
                                        | (u32::from(g >> 3) << 5)
                                        | u32::from(b >> 3)
                                }
                                1 => u32::from_le_bytes([b, g, r, 0]),
                                3 => u32::from(r),
                                _ => u32::from_le_bytes([b, g, r, a]),
                            };
                            self.write_int(destination, 2, value)?;
                            0
                        } else {
                            1
                        }
                    } else {
                        1
                    };
                    Value::Int(status)
                } else if (code, id) == (0x90, 0x89) {
                    // sub_47DF50 pops the window then converts the first BP
                    // argument to a writable four-DWORD rectangle. The target
                    // returns one only when sub_440910 copied the inclusive
                    // valid region successfully.
                    let window = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let region = api.query_graph_window_valid_region(window);
                    // sub_47EA50: an unknown window is fatal.
                    if region.is_none() {
                        return Err(VmError::Runtime(
                            "無効なウィンドウハンドルが指定されました".into(),
                        ));
                    }
                    if let Some(region) = region {
                        for (index, value) in region.into_iter().enumerate() {
                            self.write_int(
                                destination.wrapping_add((index * 4) as u32),
                                2,
                                value as u32,
                            )?;
                        }
                        self.clear_shadow_values(destination, 16);
                    }
                    Value::Int(i32::from(region.is_some()))
                } else if (code, id) == (0x90, 0x98) {
                    // sub_47E1D0 converts the bitmap-table BP value before
                    // sub_433300 replaces the process-global 24-byte caret
                    // frame records. Counts <=1 deliberately allocate no
                    // records; -1 entries become empty frames.
                    let table_pointer = self.pop_ptr()?;
                    let frame_count = self.pop_int()?;
                    let mut frames = Vec::new();
                    if frame_count > 1 {
                        let count = usize::try_from(frame_count).map_err(|_| {
                            VmError::Runtime(format!(
                                "Graph90:98 invalid caret frame count {frame_count}"
                            ))
                        })?;
                        let byte_len = count.checked_mul(4).ok_or_else(|| {
                            VmError::Runtime(format!(
                                "Graph90:98 caret frame table is too large: {frame_count}"
                            ))
                        })?;
                        let _ = self.resolve_range(table_pointer, byte_len)?;
                        frames.reserve(count);
                        for index in 0..count {
                            frames.push(
                                self.read_int(table_pointer.wrapping_add((index * 4) as u32), 2)?
                                    as i32,
                            );
                        }
                    }
                    if let Err(bitmap) =
                        api.configure_message_caret_frames(table_pointer, frame_count, &frames)
                    {
                        return Err(VmError::Runtime(format!(
                            "指定されたビットマップ [ {bitmap} ] は存在しないか、スクリーンと互換性がありません"
                        )));
                    }
                    Value::None
                } else if (code, id) == (0x90, 0xf4) {
                    // sub_4802C0 converts all four BP arguments. Source order is
                    // [handle_out, metadata_out, optional archive, resource].
                    let resource = self.pop_string_lossy()?;
                    let archive = self.pop_string_lossy()?;
                    let metadata_out = self.pop_ptr()?;
                    let handle_out = self.pop_ptr()?;
                    let (status, loaded) = match api.load_buriko_movie_resource(&archive, &resource)
                    {
                        Ok((handle, metadata)) => {
                            self.write_int(handle_out, 2, handle as u32)?;
                            for (index, value) in metadata.into_iter().enumerate() {
                                self.write_int(
                                    metadata_out.wrapping_add((index * 4) as u32),
                                    2,
                                    value as u32,
                                )?;
                            }
                            self.clear_shadow_values(handle_out, 4);
                            self.clear_shadow_values(metadata_out, 20);
                            (0, Some((handle, metadata)))
                        }
                        Err(status) => (status, None),
                    };
                    self.install_host_completed_procedure(
                        NativeOpcode { group: code, id },
                        native_call::NativeProcedureCompletion {
                            class: native_call::NativeProcedureClass::LoadBurikoMovie,
                            status,
                            outputs: [0; 2],
                            output_count: 0,
                        },
                        trace_events,
                    );
                    if trace_events {
                        tracing::info!(
                            archive,
                            resource,
                            ?loaded,
                            status,
                            "Graph90:F4 BF_Movie load"
                        );
                    }
                    Value::None
                } else if (code, id) == (0x90, 0xf7) {
                    // sub_4804D0 writes the newly attached shared-resource
                    // handle through source argument zero and returns 0/3/7.
                    let source = self.pop_int()?;
                    let handle_out = self.pop_ptr()?;
                    match api.attach_buriko_movie_resource(source) {
                        Ok(handle) => {
                            self.write_int(handle_out, 2, handle as u32)?;
                            self.clear_shadow_values(handle_out, 4);
                            Value::Int(0)
                        }
                        Err(status) => Value::Int(status),
                    }
                } else if (code, id) == (0x92, 0xf1) {
                    // sub_486C40 source order is
                    // [handle_out, metadata_out, archive, resource, mode].
                    // The target selects DCProcLoadBurikoMV for mode zero and
                    // DCProcLoadBMVHeader otherwise. Both native procedures
                    // own the same handle + five-DWORD output pointers.
                    let mode = self.pop_int()?;
                    let resource = self.pop_string_lossy()?;
                    let archive = self.pop_string_lossy()?;
                    let metadata_out = self.pop_ptr()?;
                    let handle_out = self.pop_ptr()?;
                    let (status, loaded) = match api.load_buriko_movie_resource(&archive, &resource)
                    {
                        Ok((handle, metadata)) => {
                            self.write_int(handle_out, 2, handle as u32)?;
                            for (index, value) in metadata.into_iter().enumerate() {
                                self.write_int(
                                    metadata_out.wrapping_add((index * 4) as u32),
                                    2,
                                    value as u32,
                                )?;
                            }
                            self.clear_shadow_values(handle_out, 4);
                            self.clear_shadow_values(metadata_out, 20);
                            (0, Some((handle, metadata)))
                        }
                        Err(status) => (status, None),
                    };
                    let class = if mode == 0 {
                        native_call::NativeProcedureClass::LoadBurikoMovie
                    } else {
                        native_call::NativeProcedureClass::LoadBurikoMovieHeader
                    };
                    self.install_host_completed_procedure(
                        NativeOpcode { group: code, id },
                        native_call::NativeProcedureCompletion {
                            class,
                            status,
                            outputs: [0; 2],
                            output_count: 0,
                        },
                        trace_events,
                    );
                    if trace_events {
                        tracing::info!(
                            archive,
                            resource,
                            mode,
                            ?loaded,
                            status,
                            "Graph92:F1 BF_Movie load"
                        );
                    }
                    Value::None
                } else if (code, id) == (0x92, 0xf5) {
                    // sub_486EA0 pops the bitmap slot, converts the next
                    // argument to an output pointer, and sub_408600 writes the
                    // current DirectShow position in milliseconds.
                    let bitmap = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    match api.query_movie_position(bitmap) {
                        Ok(position_ms) => {
                            self.write_int(destination, 2, position_ms as u32)?;
                            self.clear_shadow_values(destination, 4);
                            Value::Int(0)
                        }
                        Err(status) => Value::Int(status),
                    }
                } else if (code, id) == (0x90, 0xcc) {
                    // sub_47F760 converts the second script argument to a
                    // native pointer. sub_40A070 consumes three (x, y)
                    // control points and builds one 256-entry curve per RGB
                    // channel.
                    let descriptor = self.pop_ptr()?;
                    let lut_id = self.pop_int()?;
                    if descriptor == 0 {
                        api.remove_graph_color_lut(lut_id);
                    } else {
                        let mut points = [[0_i32; 2]; 3];
                        for (channel, point) in points.iter_mut().enumerate() {
                            let address = descriptor.wrapping_add((channel * 8) as u32);
                            point[0] = self.read_int(address, 2)? as i32;
                            point[1] = self.read_int(address.wrapping_add(4), 2)? as i32;
                        }
                        api.register_graph_color_lut(lut_id, points);
                    }
                    Value::None
                } else if (code, id) == (0x91, 0x9f) {
                    let source = self.pop_native_string_bytes()?;
                    let destination = self.pop_ptr()?;
                    let stripped = strip_native_markup_tags(&source);
                    self.write_c_string_native_bytes(destination, &stripped)?;
                    Value::None
                } else if (code, id) == (0x91, 0x3e) {
                    let call_stack = self.take_dispatch_call_frame(code, id)?;
                    let mut call =
                        NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                    let result = api.call_graph(&mut call)?;
                    self.write_int(1072, 2, result.as_i32() as u32)?;
                    Value::None
                } else if (code, id) == (0x91, 0x9b) {
                    self.handle_text_measure_call(api)?
                } else if (code, id) == (0x91, 0x9e) {
                    // sub_437EE0 extracts every non-empty <l>...</l> payload
                    // into consecutive zero-padded 128-byte records.
                    let source = self.pop_native_string_bytes()?;
                    let destination = self.pop_ptr()?;
                    let labels = extract_native_labels(&source);
                    // A null output only counts.
                    for (index, label) in labels.iter().enumerate().filter(|_| destination != 0) {
                        let address = destination.wrapping_add((index * 128) as u32);
                        let range = self.resolve_write_range(address, 128)?;
                        self.memory[range.clone()].fill(0);
                        self.memory[range.start..range.start + label.len()].copy_from_slice(label);
                        self.clear_shadow_values(address, 128);
                    }
                    Value::Int(labels.len() as i32)
                } else if (code, id) == (0x91, 0x95) {
                    // sub_484740 pops the source, then the output buffer, and
                    // passes the buffer in ecx to sub_434920, which appends
                    // "base\\reading\n" for every dictionary match
                    // (sprintf, so the last one is NUL-terminated) and
                    // returns the match count. No match writes nothing.
                    let source = self.pop_string_lossy()?;
                    let output = self.pop_ptr()?;
                    let (records, count) = api.collect_ruby_substitutions(&source);
                    if count > 0 {
                        self.write_c_string(output, &records)?;
                    }
                    if trace_events && (!source.is_empty() || count != 0) {
                        tracing::debug!(source, count, "GraphCountTextSubstitutionMatches");
                    }
                    Value::Int(count)
                } else if (code, id) == (0x90, 0xa7) {
                    // sub_47E9E0 converts the second BP argument and
                    // sub_42C7A0 copies exactly sixteen DWORD column anchors.
                    let layout_ptr = self.pop_ptr()?;
                    let window = self.pop_int()?;
                    let layout = if layout_ptr == 0 {
                        None
                    } else {
                        let mut values = [0_i32; 16];
                        let _ = self.resolve_range(layout_ptr, values.len() * 4)?;
                        for (index, value) in values.iter_mut().enumerate() {
                            *value = self
                                .read_int(layout_ptr.wrapping_add((index * 4) as u32), 2)?
                                as i32;
                        }
                        Some(values)
                    };
                    if !api.set_item_selection_column_layout(window, layout) {
                        return Err(VmError::Runtime(
                            "無効なウィンドウハンドルが指定されました".into(),
                        ));
                    }
                    Value::None
                } else if matches!((code, id), (0x90, 0xb4) | (0x90, 0xb5)) {
                    // sub_47EC60/sub_47ECF0 pop records, count, then window.
                    // B4 consumes 16-byte records directly. B5 walks 64-byte
                    // extended records and projects DWORDs 0/4/8 plus -1 into
                    // the same base renderer.
                    let records_ptr = self.pop_ptr()?;
                    let count = self.pop_int()?;
                    let window = self.pop_int()?;
                    if !(1..=64).contains(&count) {
                        return Err(VmError::Runtime(format!(
                            "無効なアイコン数 [ {count} ] が指定されました"
                        )));
                    }
                    if !api.graph_window_exists(window) {
                        return Err(VmError::Runtime(
                            "無効なウィンドウハンドルが指定されました".into(),
                        ));
                    }
                    let stride = if id == 0xb4 { 16 } else { 64 };
                    let byte_len = count as usize * stride;
                    let _ = self.resolve_range(records_ptr, byte_len)?;
                    let mut records = Vec::with_capacity(count as usize);
                    for index in 0..count as usize {
                        let base = records_ptr.wrapping_add((index * stride) as u32);
                        records.push(GraphIconRecord {
                            x: self.read_int(base, 2)? as i32,
                            y: self.read_int(base.wrapping_add(4), 2)? as i32,
                            bitmap: self.read_int(base.wrapping_add(8), 2)? as i32,
                            parameter: if id == 0xb4 {
                                self.read_int(base.wrapping_add(12), 2)? as i32
                            } else {
                                -1
                            },
                        });
                    }
                    if !api.draw_graph_icon_batch(window, &records) {
                        return Err(VmError::Runtime(
                            "無効なウィンドウハンドルが指定されました".into(),
                        ));
                    }
                    Value::None
                } else if (code, id) == (0x90, 0xb6) {
                    let descriptor_ptr = self.pop_ptr()?;
                    let window = self.pop_int()?;
                    if !api.graph_window_exists(window) {
                        Value::Int(1)
                    } else if let Some(status) =
                        self.validate_graph_input_descriptor(descriptor_ptr, false)?
                    {
                        Value::Int(status)
                    } else {
                        let descriptor =
                            self.read_compact_graph_input_descriptor(descriptor_ptr)?;
                        api.configure_graph_surface_controls(window, descriptor);
                        Value::Int(0)
                    }
                } else if (code, id) == (0x90, 0xba) {
                    let descriptor_ptr = self.pop_ptr()?;
                    let object = self.pop_int()?;
                    if !api.graph_input_object_exists(object) {
                        Value::Int(1)
                    } else if api.graph_input_object_is_extended(object) {
                        Value::Int(4)
                    } else if let Some(status) =
                        self.validate_graph_input_descriptor(descriptor_ptr, false)?
                    {
                        Value::Int(status)
                    } else {
                        let descriptor =
                            self.read_compact_graph_input_descriptor(descriptor_ptr)?;
                        api.configure_graph_input_object(object, descriptor);
                        Value::Int(0)
                    }
                } else if (code, id) == (0x91, 0xba) {
                    let descriptor_ptr = self.pop_ptr()?;
                    let object = self.pop_int()?;
                    if !api.graph_input_object_exists(object) {
                        Value::Int(1)
                    } else if !api.graph_input_object_is_extended(object) {
                        Value::Int(4)
                    } else if let Some(status) =
                        self.validate_graph_input_descriptor(descriptor_ptr, true)?
                    {
                        Value::Int(status)
                    } else if self.graph_input_group_counts_exceed_range(descriptor_ptr)? {
                        // sub_44A900 empties the processor (vtable+0x34) before
                        // it rechecks every whole group-count dword.
                        api.configure_graph_input_object(object, GraphInputDescriptor::default());
                        Value::Int(3)
                    } else {
                        let descriptor = self.read_graph_input_descriptor(descriptor_ptr)?;
                        api.configure_graph_input_object(object, descriptor);
                        Value::Int(0)
                    }
                } else if (code, id) == (0x91, 0xbf) {
                    // sub_484FB0 converts the second source argument to a BP
                    // pointer, then sub_447BB0 copies exactly 24 DWORDs into
                    // one of four global key-assignment records (IDs 4..=7).
                    let table_ptr = self.pop_ptr()?;
                    let assignment = self.pop_int()?;
                    let _ = self.resolve_range(table_ptr, 24 * 4)?;
                    let mut values = [0_i32; 24];
                    for (index, value) in values.iter_mut().enumerate() {
                        *value =
                            self.read_int(table_ptr.wrapping_add((index * 4) as u32), 2)? as i32;
                    }
                    if !api.set_graph_key_assignment(assignment, values) {
                        return Err(VmError::Runtime(format!(
                            "無効なキーアサインメント [ {assignment} ] が指定されました"
                        )));
                    }
                    Value::None
                } else if (code, id) == (0x92, 0x9b) {
                    // sub_487260 -> sub_4347B0: selector 256 writes
                    // dword_565D44/48, 257 dword_565D4C; anything else is a
                    // script error.
                    let selector = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    match selector {
                        256 => {
                            let [first, second] = api.system92_text_output_pair();
                            let _ = self.resolve_write_range(destination, 8)?;
                            self.write_int(destination, 2, first as u32)?;
                            self.write_int(destination.wrapping_add(4), 2, second as u32)?;
                        }
                        257 => {
                            self.write_int(destination, 2, api.system92_text_colour() as u32)?;
                        }
                        _ => {
                            return Err(VmError::Runtime(format!(
                                "無効なパラメータ番号 [ {selector} ] が指定されました"
                            )));
                        }
                    }
                    Value::None
                } else if (code, id) == (0x92, 0x9e) {
                    // sub_437EB0 copies count * 128 bytes and clears the
                    // process-global table. Each record contains 96 bytes of
                    // zero-padded Shift-JIS text and x/y DWORDs at 120/124.
                    let destination = self.pop_ptr()?;
                    let records = api.take_system92_text_fragment_records();
                    let count = records.len().min(16);
                    if count != 0 {
                        let byte_len = count * 128;
                        let range = self.resolve_write_range(destination, byte_len)?;
                        self.memory[range.clone()].fill(0);
                        for (index, record) in records.into_iter().take(count).enumerate() {
                            let base = range.start + index * 128;
                            let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(&record.text);
                            let text_len = encoded.len().min(95);
                            self.memory[base..base + text_len]
                                .copy_from_slice(&encoded[..text_len]);
                            self.memory[base + 120..base + 124]
                                .copy_from_slice(&record.x.to_le_bytes());
                            self.memory[base + 124..base + 128]
                                .copy_from_slice(&record.y.to_le_bytes());
                        }
                        self.clear_shadow_values(destination, byte_len);
                    }
                    Value::Int(count as i32)
                } else if (code, id) == (0x90, 0xb7) {
                    let descriptor_ptr = self.pop_ptr()?;
                    let window = self.pop_int()?;
                    if !api.graph_window_exists(window) {
                        Value::Int(1)
                    } else if let Some(status) =
                        self.validate_graph_input_descriptor(descriptor_ptr, true)?
                    {
                        Value::Int(status)
                    } else if self.graph_input_group_counts_exceed_range(descriptor_ptr)? {
                        // sub_46C9D0 validated only the low word of each
                        // group count; sub_44B260 has already released the
                        // window's icon sprites when it rechecks the whole
                        // dword and returns 0x80000002 (status 3).
                        api.configure_graph_surface_controls(
                            window,
                            GraphInputDescriptor::default(),
                        );
                        Value::Int(3)
                    } else {
                        let descriptor = self.read_graph_input_descriptor(descriptor_ptr)?;
                        api.configure_graph_surface_controls(window, descriptor);
                        Value::Int(0)
                    }
                } else if (code, id) == (0x90, 0xbe) {
                    let dest = self.pop_ptr()?;
                    let object = self.pop_value()?;
                    let call_stack = vec![object, Value::Ptr(dest)];
                    let mut call =
                        NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                    let value = api.call_graph(&mut call)?.as_i32();
                    self.write_int(dest, 2, value as u32)?;
                    Value::None
                } else {
                    self.normalize_graph_string_args(code, id)?;
                    let call_stack = self.take_dispatch_call_frame(code, id)?;
                    let (mut result, mut call_stack, procedure_start, mut procedure_completion) =
                        if (code, id) == (0x90, 0x29) {
                            let points = self.read_spline_control_points(&call_stack)?;
                            let mut call =
                                NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                            let result = api.call_graph_spline_control(&mut call, &points)?;
                            let procedure_start = call.take_procedure_start();
                            let procedure_completion = call.take_procedure_completion();
                            (
                                result,
                                call.into_args(),
                                procedure_start,
                                procedure_completion,
                            )
                        } else {
                            let mut call =
                                NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                            let result = api.call_graph(&mut call)?;
                            let procedure_start = call.take_procedure_start();
                            let procedure_completion = call.take_procedure_completion();
                            (
                                result,
                                call.into_args(),
                                procedure_start,
                                procedure_completion,
                            )
                        };
                    if (code, id) == (0x90, 0xF6) {
                        let use_procedure = std::mem::take(&mut self.next_binary_or_bmv_async);
                        if use_procedure {
                            if procedure_completion.is_none() {
                                let status = result.as_i32();
                                procedure_completion =
                                    Some(native_call::NativeProcedureCompletion {
                                        class: native_call::NativeProcedureClass::DecodeBurikoMovie,
                                        status,
                                        outputs: [status, 0],
                                        output_count: 1,
                                    });
                            }
                            result = Value::None;
                        } else if let Some(completion) = procedure_completion.take() {
                            result = Value::Int(if completion.output_count != 0 {
                                completion.outputs[0]
                            } else {
                                completion.status
                            });
                        }
                    }
                    let result = self.settle_native_call_outputs(code, id, &mut call_stack, result);
                    self.audit_native_args_consumed(
                        "graph",
                        code,
                        id,
                        call_stack.len(),
                        program_index,
                        fail_on_stub,
                    )?;
                    if let Some(procedure_start) = procedure_start {
                        let opcode = NativeOpcode { group: code, id };
                        match procedure_start {
                            native_call::NativeProcedureStart::Message(mut config) => {
                                config.auxiliary_input_mask = self
                                    .system81_shared
                                    .lock()
                                    .expect("system81 state poisoned")
                                    .message_auxiliary_input_mask;
                                // Target CProcDspMsg constructor sub_432BC0
                                // registers +0x78 exactly as stored (literal 2
                                // or an already-packed scope) in both native
                                // input lists and immediately drains one sample
                                // before the procedure becomes schedulable.
                                api.register_message_input_scope(config.input_scope);
                                let procedure = InstalledCProcedure::dsp_msg(
                                    self.thread.thread_id(),
                                    opcode,
                                    self.timing.tick_count() as u32,
                                    config,
                                );
                                self.install_cprocedure(procedure, trace_events);
                            }
                            native_call::NativeProcedureStart::GraphControl {
                                object_id,
                                control_id,
                            } => {
                                let Some(evidence) =
                                    crate::procedure_class_map::target_procedure_class(opcode)
                                else {
                                    return Err(VmError::Runtime(format!(
                                        "missing target CProcCtrlDspObj class for graph {:02X}:{:02X}",
                                        opcode.group, opcode.id
                                    )));
                                };
                                self.install_cprocedure(
                                    InstalledCProcedure::graph(
                                        self.thread.thread_id(),
                                        opcode,
                                        evidence.class_name,
                                        native_thread::NativeGraphProcedureMode::Control,
                                        Some(object_id),
                                        Some(control_id),
                                    ),
                                    trace_events,
                                );
                            }
                        }
                    }
                    if let Some(completion) = procedure_completion {
                        self.install_host_completed_procedure(
                            NativeOpcode { group: code, id },
                            completion,
                            trace_events,
                        );
                    }
                    // The EXE path map contains seventeen graph entries
                    // whose dispatch descriptor does not set the procedure bit
                    // but whose target handler reaches a concrete CProc*
                    // installation. Preserve that target boundary and class
                    // identity instead of treating the call as synchronous.
                    self.ensure_mapped_internal_graph_procedure_boundary(
                        NativeOpcode { group: code, id },
                        trace_events,
                    );
                    // Direct ABI procedure entries use the same single
                    // CThread+0x58 slot. Unknown completion predicates remain
                    // suspended rather than receiving an invented one-tick wait.
                    self.ensure_unrecovered_native_procedure_boundary(
                        NativeOpcode { group: code, id },
                        trace_events,
                    );
                    result
                };
                if api.take_runtime_stub() {
                    self.note_stub("graph", code, id);
                    if fail_on_stub {
                        return Err(VmError::UnknownDispatch { group: code, id });
                    }
                }
                let result = self.enforce_native_output_contract("graph", code, id, result);
                self.audit_native_return("graph", code, id, &result, program_index, fail_on_stub)?;
                if result != Value::None {
                    self.push_value(result);
                }
            }
            BpOpcode::Known { code: 0xa0, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("snd", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if trace_sound_calls_enabled() {
                    tracing::info!(
                        vm = self.trace_id,
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        group = format_args!("0x{code:02X}"),
                        id = format_args!("0x{id:02X}"),
                        stack_top = ?self.stack_summary(8),
                        "VM sound callsite"
                    );
                }
                if fail_on_stub
                    && !native_call::is_strictly_supported(NativeOpcode { group: code, id })
                {
                    self.note_stub("snd", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                self.normalize_sound_string_args(code, id)?;
                let mut direct_completion = None;
                let direct_result = if (code, id) == (0xa0, 0x15) {
                    let destination = self.pop_ptr()?;
                    let channel = self.pop_int()?;
                    if let Some((status, state)) = api.query_bgm_state(channel) {
                        if destination != 0 {
                            self.write_int(destination, 2, state as u32)?;
                            self.clear_shadow_values(destination, 4);
                        }
                        Some(Value::Int(status))
                    } else {
                        self.push_value(Value::Int(channel));
                        self.push_value(Value::Ptr(destination));
                        None
                    }
                } else if (code, id) == (0xa0, 0x86) {
                    // sub_487C50 -> sub_48D910 writes the translated MCI mode
                    // and returns one when the status query ran; a closed
                    // device or a failed query writes nothing and returns 0.
                    let destination = self.pop_ptr()?;
                    if let Some(mode) = api.query_cd_audio_mode() {
                        if destination != 0 {
                            self.write_int(destination, 2, mode as u32)?;
                            self.clear_shadow_values(destination, 4);
                        }
                        Some(Value::Int(1))
                    } else {
                        Some(Value::Int(0))
                    }
                } else if (code, id) == (0xa0, 0x28) {
                    // sub_487A70 resolves the second source argument to a BP
                    // pointer. DCProcRgstrSound later copies descriptor[0] +
                    // descriptor[2] bytes, with the first 64 bytes retained as
                    // the target sound descriptor. Bridge that exact memory
                    // block before host dispatch so no guest pointer escapes.
                    let playback_rate_fixed = self.pop_int()?;
                    let decode_gain_fixed = self.pop_int()?;
                    let native_start_parameter = self.pop_int()?;
                    let source = self.pop_ptr()?;
                    let channel = self.pop_int()?;
                    let descriptor_range = self.resolve_range(source, 64)?;
                    let descriptor = &self.memory[descriptor_range];
                    let first_size = u32::from_le_bytes(descriptor[0..4].try_into().unwrap());
                    let second_size = u32::from_le_bytes(descriptor[8..12].try_into().unwrap());
                    let total_size = first_size.checked_add(second_size).ok_or_else(|| {
                        VmError::Runtime("SoundA0:28 memory block length overflow".to_string())
                    })?;
                    if !(64..=64 * 1024 * 1024).contains(&total_size) {
                        return Err(VmError::Runtime(format!(
                            "SoundA0:28 invalid memory block length {total_size}"
                        )));
                    }
                    let block = self.resolve_range(source, total_size as usize)?;
                    let registered = api.register_memory_sound(
                        channel,
                        &self.memory[block],
                        native_start_parameter,
                        f64::from(decode_gain_fixed) / 65_536.0,
                        f64::from(playback_rate_fixed) / 65_536.0,
                    );
                    direct_completion = Some(native_call::NativeProcedureCompletion {
                        class: native_call::NativeProcedureClass::RegisterSound,
                        status: i32::from(!registered),
                        outputs: [0; 2],
                        output_count: 0,
                    });
                    Some(Value::None)
                } else {
                    None
                };
                let call_stack = if direct_result.is_some() {
                    Vec::new()
                } else {
                    self.take_dispatch_call_frame(code, id)?
                };
                let (result, call_stack, completed_procedure) = if let Some(result) = direct_result
                {
                    let completed_procedure = direct_completion.is_some();
                    if let Some(completion) = direct_completion {
                        self.install_host_completed_procedure(
                            NativeOpcode { group: code, id },
                            completion,
                            trace_events,
                        );
                        let _ = self.poll_current_procedure(api, trace_events);
                    }
                    (result, call_stack, completed_procedure)
                } else {
                    let mut call =
                        NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                    let result = api.call_sound(&mut call)?;
                    let completion = call.take_procedure_completion();
                    let remaining = call.into_args();
                    let completed_procedure = completion.is_some();
                    if let Some(completion) = completion {
                        self.install_host_completed_procedure(
                            NativeOpcode { group: code, id },
                            completion,
                            trace_events,
                        );
                        // The host backend may have completed both native
                        // asynchronous stages synchronously. Poll through the
                        // target scheduler boundary now so completion destroys
                        // the procedure and execution continues in this same
                        // scheduler pass, matching CThread behavior.
                        let _ = self.poll_current_procedure(api, trace_events);
                    }
                    (result, remaining, completed_procedure)
                };
                self.audit_native_args_consumed(
                    "sound",
                    code,
                    id,
                    call_stack.len(),
                    program_index,
                    fail_on_stub,
                )?;
                if !completed_procedure {
                    self.ensure_unrecovered_native_procedure_boundary(
                        NativeOpcode { group: code, id },
                        trace_events,
                    );
                }
                if api.take_runtime_stub() {
                    self.note_stub("sound", code, id);
                    if fail_on_stub {
                        return Err(VmError::UnknownDispatch { group: code, id });
                    }
                }
                let result = self.enforce_native_output_contract("sound", code, id, result);
                self.audit_native_return("sound", code, id, &result, program_index, fail_on_stub)?;
                if result != Value::None {
                    self.push_value(result);
                }
            }
            BpOpcode::Known { code: 0xb0 | 0xc0, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("user", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !native_call::is_strictly_supported(NativeOpcode { group: code, id })
                {
                    self.note_stub("user", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                if api.host_requests_enabled()
                    && self.intercept_native_host_request(api, NativeOpcode { group: code, id })?
                {
                    self.pc = next_pc;
                    return Ok(());
                }
                self.normalize_user_string_args(code, id)?;
                let direct_result = if (code, id) == (0xb0, 0xc4) {
                    // sub_479D60 -> sub_4694B0 -> sub_42DEB0/sub_42F030:
                    // null queries the byte count, including each terminator.
                    // A buffer receives consecutive C strings and returns
                    // the family count, without an extra list terminator.
                    let destination = self.pop_ptr()?;
                    let families = api.user_font_families();
                    let mut bytes = Vec::new();
                    for family in &families {
                        bytes.extend_from_slice(&encoding_rs::SHIFT_JIS.encode(family).0);
                        bytes.push(0);
                    }
                    let result = if destination == 0 {
                        bytes.len()
                    } else {
                        let range = self.resolve_write_range(destination, bytes.len())?;
                        self.memory[range].copy_from_slice(&bytes);
                        self.clear_shadow_values(destination, bytes.len());
                        families.len()
                    };
                    Some(Value::Int(result as i32))
                } else if (code, id) == (0xb0, 0x27) {
                    let destination = self.pop_ptr()?;
                    let text = api.current_user_text();
                    let bytes_written = self.write_c_string_bounded(destination, &text, 255)?;
                    Some(Value::Int(bytes_written as i32))
                } else if user_dialog::is_user_dialog(code, id) {
                    let (request, sink) = self
                        .parse_user_dialog(id)?
                        .expect("user dialog selector");
                    if self.begin_host_continuation(
                        api,
                        HostRequest::UserDialog(request.clone()),
                        1,
                    )? {
                        if let Some(pending) = self.pending_host_request.as_mut() {
                            pending.user_dialog = Some(sink);
                        }
                        self.pc = next_pc;
                        return Ok(());
                    }
                    let response = api.show_user_dialog(request);
                    let (accepted, writes) = self.user_dialog_writes(&sink, response)?;
                    self.apply_host_writes(writes)?;
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0xa0) {
                    // sub_479040 passes a pointer to nine DWORD initial values,
                    // accepts only mode zero and writes the allocated handle.
                    let initial_pointer = self.pop_ptr()?;
                    let mode = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let handle = if mode == 0 && initial_pointer != 0 {
                        let mut initial = [0i32; 9];
                        for (index, value) in initial.iter_mut().enumerate() {
                            *value = self.read_int(
                                initial_pointer.wrapping_add((index as u32).wrapping_mul(4)),
                                2,
                            )? as i32;
                        }
                        api.create_user_modeless_dialog(initial)
                    } else {
                        None
                    };
                    if let Some(handle) = handle {
                        if destination != 0 {
                            self.write_int(destination, 2, handle as u32)?;
                            self.clear_shadow_values(destination, 4);
                        }
                        Some(Value::Int(1))
                    } else {
                        Some(Value::Int(0))
                    }
                } else if (code, id) == (0xb0, 0xa3) {
                    let handle = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let result = match api.poll_user_modeless_dialog(handle) {
                        Ok(Some([first, second])) => {
                            if destination != 0 {
                                self.write_int(destination, 2, first as u32)?;
                                self.write_int(destination.wrapping_add(4), 2, second as u32)?;
                                self.clear_shadow_values(destination, 8);
                            }
                            0
                        }
                        Ok(None) => 1,
                        Err(()) => -1,
                    };
                    Some(Value::Int(result))
                } else if (code, id) == (0xc0, 0x06) {
                    // sub_475750 consumes two raw DWORD tables rather than
                    // target strings. Native pop order is second table,
                    // first table, count, handle. The target has a dedicated
                    // one-entry default path when the first pointer is null.
                    let second_source = self.pop_ptr()?;
                    let first_source = self.pop_ptr()?;
                    let count = self.pop_int()?;
                    let handle = self.pop_int()?;
                    let layers = count;
                    let count = if (1..=16).contains(&count) { count as usize } else { 0 };
                    let (first, second) = if count == 0 {
                        (Vec::new(), Vec::new())
                    } else if count == 1 && first_source == 0 {
                        (vec![0x7fff], vec![0])
                    } else {
                        let mut first = Vec::with_capacity(count);
                        let mut second = Vec::with_capacity(count);
                        for index in 0..count {
                            let offset = (index as u32).wrapping_mul(4);
                            first.push(if first_source == 0 {
                                0
                            } else {
                                self.read_int(first_source.wrapping_add(offset), 2)? as i32
                            });
                            second.push(if count==1 || second_source == 0 {
                                0
                            } else {
                                self.read_int(second_source.wrapping_add(offset), 2)? as i32
                            });
                        }
                        (first, second)
                    };
                    // sub_4760C0: handle (255), count 1..=16 (7), each
                    // relative priority plus the screen's within 0xFFFF (8).
                    match api.configure_particle_frame_tables(handle, layers, &first, &second) {
                        0 => {}
                        7 => {
                            return Err(VmError::Runtime(format!(
                                "無効なレイヤー数 [ {layers} ] が指定されました"
                            )));
                        }
                        8 => {
                            return Err(VmError::Runtime(
                                "指定された相対プライオリティの中に無効なものが存在します".into(),
                            ));
                        }
                        _ => {
                            return Err(VmError::Runtime(
                                "無効なパーティクルスクリーンハンドルが指定されました".into(),
                            ));
                        }
                    }
                    Some(Value::None)
                } else if code == 0xc0 && matches!(id, 0x25 | 0x2d) {
                    // 476A80 dereferences the final source operand; 476ED0
                    // dereferences source operand 14. Preserve the positional
                    // frame, replacing only this raw DWORD pointer with its value.
                    let count = if id == 0x25 { 11 } else { 18 };
                    let pointer_index = if id == 0x25 { 10 } else { 14 };
                    let mut args = Vec::with_capacity(count);
                    for _ in 0..count { args.push(self.pop_value()?); }
                    args.reverse();
                    let pointer = args[pointer_index].as_i32() as u32;
                    args[pointer_index] = Value::Int(self.read_int(pointer, 2)? as i32);
                    let mut call = NativeCallFrame::new(NativeOpcode {group: code, id}, args);
                    Some(api.call_user(&mut call)?.unwrap_or(Value::None))
                } else if (code, id) == (0xc0, 0xc2) {
                    // sub_4777B0 -> sub_495550: [handle, point_count,
                    // point_record_ptr, duration]; count and duration are
                    // compared unsigned (2, 3) before the handle (1). Every
                    // point occupies four DWORDs; x/y/z are the first three.
                    let duration = self.pop_int()?;
                    let source = self.pop_ptr()?;
                    let count = self.pop_int()?;
                    let handle = self.pop_int()?;
                    let status = if (count as u32) < 2 {
                        2
                    } else if (duration as u32) < 2 {
                        3
                    } else if !api.user_spline_exists(handle) {
                        1
                    } else {
                        let count = usize::try_from(count).unwrap_or(usize::MAX);
                        let mut points = Vec::with_capacity(count.min(100));
                        for index in 0..count.min(100) {
                            let address = source.wrapping_add((index * 16) as u32);
                            points.push([
                                self.read_int(address, 2)? as i32,
                                self.read_int(address.wrapping_add(4), 2)? as i32,
                                self.read_int(address.wrapping_add(8), 2)? as i32,
                            ]);
                        }
                        api.configure_user_spline(handle, duration, &points)
                    };
                    Some(Value::Int(status))
                } else if (code, id) == (0xc0, 0xc3) {
                    // sub_476E90 / sub_494AE0 writes one sampled XYZ triple.
                    let time = self.pop_int()?;
                    let handle = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let status = match api.sample_user_spline(handle, time) {
                        Ok(point) => {
                            for (slot, value) in point.into_iter().enumerate() {
                                self.write_int(
                                    destination.wrapping_add((slot * 4) as u32),
                                    2,
                                    value as u32,
                                )?;
                            }
                            self.clear_shadow_values(destination, 12);
                            0
                        }
                        Err(status) => status,
                    };
                    Some(Value::Int(status))
                } else if (code, id) == (0xc0, 0xf0) {
                    // sub_477840 -> sub_4066C0 loads a BWEF resource and writes
                    // count pairs {base + offset, stride}. Script order is
                    // (table_out, count_out, archive, resource, base).
                    let base = self.pop_int()?;
                    let resource = self.pop_string_lossy()?;
                    let archive = self.pop_string_lossy()?;
                    let count_out = self.pop_ptr()?;
                    let table_out = self.pop_ptr()?;
                    let status = match api.load_file_bytes(&archive, &resource) {
                        None => i32::MIN + 1,
                        Some(bytes) if bytes.len() < 288 || &bytes[..8] != b"bwef    " => {
                            i32::MIN + 2
                        }
                        Some(bytes) => {
                            let count = u32::from_le_bytes(
                                bytes[20..24].try_into().expect("four-byte BWEF count"),
                            ) as usize;
                            let expected =
                                count.checked_mul(4).and_then(|size| size.checked_add(288));
                            if expected != Some(bytes.len()) {
                                i32::MIN + 3
                            } else {
                                let stride = u32::from_le_bytes(
                                    bytes[24..28].try_into().expect("four-byte BWEF stride"),
                                );
                                let table_bytes = count.checked_mul(8).ok_or(VmError::Runtime(
                                    "BWEF table size overflow".to_string(),
                                ))?;
                                self.resolve_write_range(table_out, table_bytes)?;
                                for index in 0..count {
                                    let offset_start = 288 + index * 4;
                                    let offset = u32::from_le_bytes(
                                        bytes[offset_start..offset_start + 4]
                                            .try_into()
                                            .expect("four-byte BWEF offset"),
                                    );
                                    let destination = table_out.wrapping_add((index * 8) as u32);
                                    self.write_int(
                                        destination,
                                        2,
                                        (base as u32).wrapping_add(offset),
                                    )?;
                                    self.write_int(destination.wrapping_add(4), 2, stride)?;
                                }
                                self.write_int(count_out, 2, count as u32)?;
                                self.clear_shadow_values(table_out, table_bytes);
                                self.clear_shadow_values(count_out, 4);
                                0
                            }
                        }
                    };
                    Some(Value::Int(status))
                } else {
                    None
                };
                let call_stack = if direct_result.is_some() {
                    Vec::new()
                } else {
                    self.take_dispatch_call_frame(code, id)?
                };
                let mut call = NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                api.observe_user(&call);
                let host_result = if let Some(result) = direct_result {
                    Some(result)
                } else {
                    api.call_user(&mut call)?
                };
                self.ensure_unrecovered_native_procedure_boundary(
                    NativeOpcode { group: code, id },
                    trace_events,
                );
                let mut call_stack = call.into_args();
                if let Some(result) = host_result {
                    let result = self.settle_native_call_outputs(code, id, &mut call_stack, result);
                    self.audit_native_args_consumed(
                        "user",
                        code,
                        id,
                        call_stack.len(),
                        program_index,
                        fail_on_stub,
                    )?;
                    let result = self.enforce_native_output_contract("user", code, id, result);
                    self.audit_native_return(
                        "user",
                        code,
                        id,
                        &result,
                        program_index,
                        fail_on_stub,
                    )?;
                    if result != Value::None {
                        self.push_value(result);
                    }
                } else {
                    let caller_stack = std::mem::take(&mut self.stack);
                    let caller_synced_len = self.operand_slots_synced_len;
                    let caller_slots = std::mem::replace(
                        &mut self.operand_slots,
                        vec![Value::Int(0); OPERAND_STACK_CAPACITY],
                    );
                    self.stack = call_stack;
                    self.operand_slots_synced_len = 0;
                    self.sync_operand_slots();
                    let builtin_result = self.try_builtin_user(code, id);
                    let mut local_stack = std::mem::replace(&mut self.stack, caller_stack);
                    self.operand_slots = caller_slots;
                    self.operand_slots_synced_len = caller_synced_len;
                    builtin_result?;
                    if known_call_returns_value(code, id) {
                        let result = local_stack
                            .pop()
                            .filter(|value| *value != Value::None)
                            .unwrap_or(Value::None);
                        self.audit_native_return(
                            "user",
                            code,
                            id,
                            &result,
                            program_index,
                            fail_on_stub,
                        )?;
                        if result != Value::None {
                            self.push_value(result);
                        }
                    }
                }
            }
            BpOpcode::Known { code: 0xd0, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default();
                self.note_call("legacy3d", code, u16::from(id));
                if let Some(result) = self.execute_legacy_3d(id)? {
                    self.push_value(result);
                }
            }
            BpOpcode::Known { code: 0xe0, .. } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default();
                self.note_call("debug", code, u16::from(id));
                self.execute_debug_inspect(id)?;
            }
            BpOpcode::Unknown(code) => {
                self.note_stub("opcode", code, 0);
                if fail_on_stub {
                    return Err(VmError::UnsupportedInstruction(code));
                }
            }
            BpOpcode::Known { code, .. } => {
                self.note_stub("opcode", code, 0);
                if fail_on_stub {
                    return Err(VmError::UnsupportedInstruction(code));
                }
            }
        }
        self.trim_operand_stack();
        self.pc = next_pc;
        Ok(())
    }

    fn trim_operand_stack(&mut self) {
        if self.operand_slots.len() != OPERAND_STACK_CAPACITY {
            self.operand_slots = vec![Value::Int(0); OPERAND_STACK_CAPACITY];
            self.operand_slots_synced_len = 0;
        }
        if self.stack.len() >= OPERAND_STACK_CAPACITY {
            let values = std::mem::take(&mut self.stack);
            self.operand_slots_synced_len = 0;
            for value in values {
                self.push_value(value);
            }
        } else {
            self.sync_operand_slots();
        }
    }

    fn install_cprocedure(&mut self, procedure: InstalledCProcedure, trace_events: bool) {
        // sub_4451C0 deletes the previous object, and with it any callbacks
        // still queued on it; the new object starts with an empty queue.
        self.thread.take_procedure_callbacks();
        if let Some(previous) = self.thread.replace_current_procedure(procedure) {
            tracing::warn!(
                previous_class = previous.object.class_name(),
                previous_group = format_args!("0x{:02X}", previous.source_opcode.group),
                previous_id = format_args!("0x{:02X}", previous.source_opcode.id),
                class = procedure.object.class_name(),
                group = format_args!("0x{:02X}", procedure.source_opcode.group),
                id = format_args!("0x{:02X}", procedure.source_opcode.id),
                "CThread::current_procedure replaced before completion"
            );
        }
        if trace_events || self.collect_diagnostics {
            tracing::debug!(
                thread_id = self.thread.thread_id(),
                class = procedure.object.class_name(),
                group = format_args!("0x{:02X}", procedure.source_opcode.group),
                id = format_args!("0x{:02X}", procedure.source_opcode.id),
                deadline_tick = self.thread.deadline_tick(),
                "installed target-shaped CProcedure in CThread+0x58"
            );
        }
    }

    fn install_host_completed_procedure(
        &mut self,
        opcode: NativeOpcode,
        completion: native_call::NativeProcedureCompletion,
        trace_events: bool,
    ) {
        let procedure = if completion.class == native_call::NativeProcedureClass::LoadSound
            && completion.output_count == 0
        {
            InstalledCProcedure::load_sound(self.thread.thread_id(), opcode, completion.status)
        } else {
            InstalledCProcedure::host_completed(
                self.thread.thread_id(),
                opcode,
                completion.class.target_class_name(),
                completion.status,
                completion.outputs,
                completion.output_count,
            )
        };
        self.install_cprocedure(procedure, trace_events);
    }

    fn ensure_mapped_internal_graph_procedure_boundary(
        &mut self,
        opcode: NativeOpcode,
        trace_events: bool,
    ) {
        let Some(evidence) = crate::procedure_class_map::target_procedure_class(opcode) else {
            return;
        };
        if evidence.path_kind != "internal_procedure_path"
            || self.thread.current_procedure().is_some()
        {
            return;
        }
        tracing::debug!(
            group = format_args!("0x{:02X}", opcode.group),
            id = format_args!("0x{:02X}", opcode.id),
            class = evidence.class_name,
            confidence = evidence.confidence,
            "installed selector-specific target procedure class from the EXE path map"
        );
        let mode = match evidence.class_name {
            "CProcSelectItem"
            | "CProcSelectItemEx"
            | "CProcSelectItemExBlink"
            | "CProcSelectIcon"
            | "CProcSelectIconEx" => native_thread::NativeGraphProcedureMode::Select,
            "CProcShakeScreen" => native_thread::NativeGraphProcedureMode::Shake,
            "CProcShakeDspObj" => native_thread::NativeGraphProcedureMode::Control,
            _ => native_thread::NativeGraphProcedureMode::Control,
        };
        self.install_cprocedure(
            InstalledCProcedure::graph(
                self.thread.thread_id(),
                opcode,
                evidence.class_name,
                mode,
                None,
                None,
            ),
            trace_events,
        );
    }

    fn ensure_unrecovered_native_procedure_boundary(
        &mut self,
        opcode: NativeOpcode,
        trace_events: bool,
    ) {
        // These target-confirmed selectors install a CProcedure only when
        // their own gate or initialization path succeeds. The owning handler
        // must decide that boundary; the generic ABI audit must not turn a
        // synchronous or immediate-failure path into a permanent wait.
        if opcode == native_call::opcodes::SYS_WAIT_THREAD_TIMER
            || opcode == native_call::opcodes::SYS81_READ_RESOURCE_BINARY
            || opcode == native_call::opcodes::SYS81_RUN_INSTALLATION_PROCEDURE
            || opcode == native_call::opcodes::GRAPH90_DECODE_BURIKO_MOVIE_FRAME
            // sub_4797E0 loads cached images and, while sub_402080 holds,
            // '/'-free names synchronously without a CProcLoadBitmap.
            || opcode == native_call::opcodes::GRAPH90_LOAD_BITMAP
        {
            // These selectors install a procedure only on a target-confirmed
            // conditional path. Their owning handlers consume the relevant
            // gate and preserve the synchronous/immediate-failure path; do
            // not fabricate an unrecovered procedure when that path is taken.
            return;
        }
        if self.effective_procedure(opcode.group, opcode.id)
            && self.thread.current_procedure().is_none()
        {
            if let Some(evidence) = crate::procedure_class_map::target_procedure_class(opcode) {
                let mode = match evidence.class_name {
                    "CProcSelectItem"
                    | "CProcSelectItemEx"
                    | "CProcSelectItemExBlink"
                    | "CProcSelectIcon"
                    | "CProcSelectIconEx" => Some(native_thread::NativeGraphProcedureMode::Select),
                    "CProcShakeScreen" => Some(native_thread::NativeGraphProcedureMode::Shake),
                    "CProcShakeDspObj" => Some(native_thread::NativeGraphProcedureMode::Control),
                    "CProcCtrlDspObj" | "CProcCtrlDspObjBC" | "CProcCtrlDspObjSp" => {
                        Some(native_thread::NativeGraphProcedureMode::Control)
                    }
                    _ => None,
                };
                if let Some(mode) = mode {
                    self.install_cprocedure(
                        InstalledCProcedure::graph(
                            self.thread.thread_id(),
                            opcode,
                            evidence.class_name,
                            mode,
                            None,
                            None,
                        ),
                        trace_events,
                    );
                    return;
                }
            }
            tracing::error!(
                group = format_args!("0x{:02X}", opcode.group),
                id = format_args!("0x{:02X}", opcode.id),
                call = native_call::display_name(opcode),
                "target installs a CProcedure, but the subclass is unrecovered; keeping the thread suspended instead of inventing one-tick completion"
            );
            self.install_cprocedure(
                InstalledCProcedure::unrecovered(self.thread.thread_id(), opcode),
                trace_events,
            );
        }
    }

    /// `CProcedure::DrainCallbacks` (sub_431AF0): pops queued callbacks in
    /// order. A record whose code is 0 sets the terminal latch `+0x10` and
    /// stops the drain, leaving later records queued; every other record is
    /// handed to the subclass handler (vtable slot 6), returned here in order.
    fn drain_procedure_callbacks(&mut self) -> (bool, Vec<[Value; 3]>) {
        let mut dispatched = Vec::new();
        while let Some(callback) = self.thread.pop_procedure_callback() {
            if callback[0].as_i32() == 0 {
                return (true, dispatched);
            }
            dispatched.push(callback);
        }
        (false, dispatched)
    }

    /// sub_4319C0: a procedure keeps running only while the process-global
    /// wait state `dword_507688` is non-zero and its terminal latch is clear.
    fn procedure_alive(&self, terminal_latch: bool) -> bool {
        self.system_wait_state != 0 && !terminal_latch
    }

    /// Poll the single procedure object stored in target `CThread+0x58`.
    ///
    /// The target does not have independent `wait_blocked`, graph-wait, and
    /// generic-wait slots.  Every cooperative native wait is represented by a
    /// `CProcedure` subclass installed in this one field.  Keeping the portable
    /// runtime identical at this boundary prevents unrelated wait states from
    /// overwriting or auto-completing one another.
    fn poll_current_procedure<A>(&mut self, api: &mut A, trace_events: bool) -> Option<VmStopReason>
    where
        A: SysApi + GraphApi + SoundApi,
    {
        let installed = self.thread.current_procedure()?;
        match installed.object {
            CProcedure::WaitTiming(procedure) => {
                // CProcWaitTiming::Tick (sub_43D2B0): drain (its callback
                // handler is a no-op), then finish when the deadline passed
                // or the procedure is no longer alive. Nothing is pushed.
                let (terminal, _) = self.drain_procedure_callbacks();
                let cancelled = !self.procedure_alive(terminal);
                let now_tick = self.timing.tick_count() as u32;
                let deadline_reached = now_tick >= procedure.base.native.deadline_tick;
                if !cancelled && !deadline_reached {
                    return Some(VmStopReason::WaitingForTime);
                }
                self.thread.clear_current_procedure();
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        cancelled,
                        deadline_reached,
                        duration_ms = procedure.duration_ms,
                        deadline_tick = procedure.base.native.deadline_tick,
                        "CProcWaitTiming completed"
                    );
                }
                None
            }
            CProcedure::WaitTimingEx(procedure) => {
                // CProcWaitTimingEx::Tick (sub_43D430). Its callback handler
                // (sub_43D4B0) sets +0x28 for code 1 and ignores other codes.
                let (terminal, callbacks) = self.drain_procedure_callbacks();
                let callback_completed = procedure.native.callback_completion != 0
                    || callbacks.iter().any(|callback| callback[0].as_i32() == 1);
                let cancelled = !self.procedure_alive(terminal);
                let now_tick = self.timing.tick_count() as u32;
                let deadline_reached = now_tick >= procedure.native.base.deadline_tick;
                // The input query (sub_46DF00((scope << 16) | 0xFFFF) filtered
                // by dword_507690 | 0x80000181) runs only when the deadline,
                // liveness and +0x28 checks did not already finish the wait.
                let input_interrupted = !(deadline_reached || cancelled || callback_completed)
                    && procedure.input_enabled()
                    && {
                        let aux_mask =
                            self.system81_shared
                                .lock()
                                .expect("system81 state poisoned")
                                .message_auxiliary_input_mask as u32;
                        api.query_input_event_bits(procedure.input_scope()) as u32
                            & (aux_mask | 0x8000_0181)
                            != 0
                    };
                if !cancelled && !callback_completed && !input_interrupted && !deadline_reached {
                    return Some(if procedure.input_enabled() {
                        VmStopReason::WaitingForInputOrTime
                    } else {
                        VmStopReason::WaitingForTime
                    });
                }

                self.thread.clear_current_procedure();
                // Target result is 1 only for an input-driven completion.
                // Timeout, cancellation, and callback code 1 all return 0.
                self.push_value(Value::Int(i32::from(input_interrupted)));
                // Destructor sub_43D3B0 removes the scope from both lists
                // without a final query.
                if procedure.input_enabled() {
                    api.unregister_message_input_scope(
                        procedure.input_scope().wrapping_shl(16) | 0xFFFF,
                    );
                }
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        input_interrupted,
                        callback_completed,
                        cancelled,
                        deadline_reached,
                        deadline_tick = procedure.native.base.deadline_tick,
                        "CProcWaitTimingEx completed"
                    );
                }
                None
            }
            CProcedure::WaitWndMsg(procedure) => {
                // CProcWaitWndMsg::Tick (sub_43D990); no-op callback handler.
                // Signaled: push wParam, lParam. Wait state 0 or a {0,..}
                // callback (sub_431D90): push -1, -1.
                let (terminal, _) = self.drain_procedure_callbacks();
                let cancelled = !self.procedure_alive(terminal);
                let result = if cancelled {
                    Some((-1, -1))
                } else {
                    api.poll_window_message(procedure.message_id, procedure.registered_after_serial)
                };
                let Some((wparam, lparam)) = result else {
                    return Some(VmStopReason::WaitingForProcedure);
                };
                self.thread.clear_current_procedure();
                self.push_value(Value::Int(wparam));
                self.push_value(Value::Int(lparam));
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        message_id = format_args!("0x{:04X}", procedure.message_id),
                        lparam,
                        wparam,
                        cancelled,
                        "CProcWaitWndMsg completed"
                    );
                }
                None
            }
            CProcedure::DspMsg(mut procedure) => {
                let now_tick = self.timing.tick_count() as u32;
                let (terminal, callbacks) = self.drain_procedure_callbacks();
                let cancel = !self.procedure_alive(terminal);
                for callback in callbacks {
                    match callback[0].as_i32() {
                        // CProcDspMsg::OnCallback (sub_434150): callbacks 1
                        // and 258 set both force-completion (+0x74) and the
                        // completion latch (+0x30).
                        1 | 258 => {
                            procedure.native.force_completion = 1;
                            procedure.native.completion_latch = 1;
                        }
                        // Callback 256 sets only force-completion. It does not
                        // fabricate an ordinary input edge or completion value.
                        256 => procedure.native.force_completion = 1,
                        // Callback 257 is the setter for +0x3c. Its payload
                        // controls whether bit 31 is accepted; it is not a
                        // reveal-only command.
                        257 => {
                            procedure.native.allow_high_bit_input =
                                u32::from(callback[1].as_i32() != 0);
                        }
                        _ => {}
                    }
                }

                if cancel {
                    api.finish_native_message();
                    api.unregister_message_input_scope(procedure.config.input_scope);
                    self.thread.clear_current_procedure();
                    // sub_433600 pushes +0x34 (set by any filtered input) on
                    // every completion; scrmsg stores it as "ended by input".
                    self.push_value(Value::Int(procedure.native.ordinary_input_latch as i32));
                    return None;
                }

                // CProcDspMsg::Tick (sub_433600) queries the procedure's input
                // scope first, then applies the target's fixed event mask and
                // the two per-message permission members.
                let mut input_bits =
                    api.query_message_input_event_bits(procedure.config.input_scope) as u32;
                input_bits &= (procedure.config.auxiliary_input_mask as u32) | 0x8000_0181;
                if procedure.native.allow_high_bit_input == 0 {
                    input_bits &= 0x7FFF_FFFF;
                }
                if procedure.native.allow_auxiliary_input == 0 {
                    input_bits &= !((procedure.config.auxiliary_input_mask as u32) | 0x80);
                }
                procedure.native.input_event_bits = input_bits;
                if input_bits != 0 {
                    procedure.native.ordinary_input_latch = 1;
                    if procedure.config.input_forces_completion {
                        procedure.native.force_completion = 1;
                    }
                }

                if procedure.native.force_completion != 0 {
                    api.reveal_native_message();
                    api.finish_native_message();
                    api.unregister_message_input_scope(procedure.config.input_scope);
                    self.thread.clear_current_procedure();
                    // sub_433600 pushes +0x34 (set by any filtered input) on
                    // every completion; scrmsg stores it as "ended by input".
                    self.push_value(Value::Int(procedure.native.ordinary_input_latch as i32));
                    return None;
                }

                if input_bits != 0 {
                    if api.native_message_is_animating() {
                        // Ordinary input only reveals while glyph animation is
                        // active. Completion requires a later input edge unless
                        // 0x90:0x9F enabled the force gate.
                        api.reveal_native_message();
                        self.thread.replace_current_procedure(InstalledCProcedure {
                            source_opcode: installed.source_opcode,
                            object: CProcedure::DspMsg(procedure),
                        });
                        return Some(VmStopReason::WaitingForInputOrTime);
                    }
                    api.finish_native_message();
                    api.unregister_message_input_scope(procedure.config.input_scope);
                    self.thread.clear_current_procedure();
                    // sub_433600 pushes +0x34 (set by any filtered input) on
                    // every completion; scrmsg stores it as "ended by input".
                    self.push_value(Value::Int(procedure.native.ordinary_input_latch as i32));
                    return None;
                }

                if !api.native_message_is_animating() && procedure.native.end_wait_policy == 0 {
                    // sub_433E40 finalizes immediately when +0x38 is zero.
                    api.finish_native_message();
                    api.unregister_message_input_scope(procedure.config.input_scope);
                    self.thread.clear_current_procedure();
                    // sub_433600 pushes +0x34 (set by any filtered input) on
                    // every completion; scrmsg stores it as "ended by input".
                    self.push_value(Value::Int(procedure.native.ordinary_input_latch as i32));
                    return None;
                }

                // The start delay (+0x44/+0x48, Graph90:9B) holds only a
                // message that waits at its end: sub_433E40 finalizes a +0x38
                // == 0 message before it looks at +0x44, so a typed message
                // ahead of the waiting one isn't held too.
                if procedure.native.initial_delay_enabled != 0
                    && now_tick < procedure.initial_deadline_tick
                    && input_bits == 0
                {
                    self.thread.replace_current_procedure(InstalledCProcedure {
                        source_opcode: installed.source_opcode,
                        object: CProcedure::DspMsg(procedure),
                    });
                    return Some(VmStopReason::WaitingForInputOrTime);
                }

                // Target +0x5c is armed only after the text processor reaches
                // the end, not at construction time, from the setting as it is
                // then: auto mode switched on while a line waits ends that
                // line, and switched off drops an armed deadline.
                let auto_advance_delay_ms = api
                    .native_message_auto_advance_delay_ms(procedure.config.auto_advance_delay_ms);
                if auto_advance_delay_ms.is_none() {
                    procedure.auto_deadline_tick = None;
                    procedure.native.auto_deadline_tick = 0;
                }
                if let Some(delay) = auto_advance_delay_ms {
                    if !api.native_message_is_animating() {
                        let deadline = *procedure
                            .auto_deadline_tick
                            .get_or_insert_with(|| now_tick.saturating_add(delay.max(0) as u32));
                        procedure.native.auto_deadline_tick = deadline;
                        if now_tick >= deadline {
                            api.finish_native_message();
                            api.unregister_message_input_scope(procedure.config.input_scope);
                            self.thread.clear_current_procedure();
                            // sub_433600 pushes +0x34 (set by any filtered input) on
                            // every completion; scrmsg stores it as "ended by input".
                            self.push_value(Value::Int(
                                procedure.native.ordinary_input_latch as i32,
                            ));
                            return None;
                        }
                    }
                }

                // sub_433E40 reaches sub_433F40 (the caret) only once the
                // start delay is over and the text processor has ended.
                if !api.native_message_is_animating() {
                    api.tick_native_message_caret(now_tick);
                }
                self.thread.replace_current_procedure(InstalledCProcedure {
                    source_opcode: installed.source_opcode,
                    object: CProcedure::DspMsg(procedure),
                });
                Some(VmStopReason::WaitingForInputOrTime)
            }
            CProcedure::LoadSound(procedure) => {
                // The portable backend has already completed the target's two
                // asynchronous stages. CProcLoadSound has no immediate BP
                // output for the recovered A0 loaders; completion only releases
                // CThread+0x58. Preserve the status for diagnostics.
                self.thread.clear_current_procedure();
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        status = procedure.terminal_status,
                        success = procedure.terminal_status == 0,
                        group = format_args!("0x{:02X}", installed.source_opcode.group),
                        id = format_args!("0x{:02X}", installed.source_opcode.id),
                        "CProcLoadSound completed"
                    );
                }
                None
            }
            CProcedure::HostCompleted(procedure) => {
                for output in procedure.outputs[..usize::from(procedure.output_count.min(2))]
                    .iter()
                    .copied()
                {
                    self.push_value(Value::Int(output));
                }
                self.thread.clear_current_procedure();
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        class = procedure.target_class_name,
                        status = procedure.terminal_status,
                        output_count = procedure.output_count,
                        group = format_args!("0x{:02X}", installed.source_opcode.group),
                        id = format_args!("0x{:02X}", installed.source_opcode.id),
                        "host-completed target CProcedure published deferred outputs"
                    );
                }
                None
            }
            CProcedure::Graph(procedure) => {
                // Code 0 / a cleared dword_507688 end every graph procedure.
                // Of the non-zero codes only CProcCtrlDspObj's code 1
                // (sub_4320B0: stop request +0x98) and CProcSelectIcon's
                // code 512 (sub_43A830: forced selection) do anything;
                // CProcSelectItem* and CProcShakeScreen ignore them.
                let (terminal, callbacks) = self.drain_procedure_callbacks();
                if procedure.mode == native_thread::NativeGraphProcedureMode::Control
                    && let (Some(object_id), Some(control_id)) =
                        (procedure.object_id, procedure.control_id)
                {
                    for callback in callbacks
                        .iter()
                        .filter(|callback| callback[0].as_i32() == 1)
                    {
                        api.request_native_graph_control_stop(
                            installed.source_opcode,
                            object_id,
                            control_id,
                            callback[1].as_i32(),
                        );
                    }
                }
                let cancelled = !self.procedure_alive(terminal)
                    || callbacks.iter().any(|callback| {
                        procedure.mode == native_thread::NativeGraphProcedureMode::Select
                            && callback[0].as_i32() == 512
                    });
                match procedure.mode {
                    native_thread::NativeGraphProcedureMode::Select => {
                        if let Some(selection_id) = procedure.control_id {
                            let callbacks = callbacks.iter().map(|v| [v[0].as_i32(), v[1].as_i32()]).collect::<Vec<_>>();
                            // 512 is an Icon-only forced-selection callback,
                            // not a base cancellation. Item procedures ignore it.
                            let completion = api.poll_native_selection(selection_id, &callbacks, !self.procedure_alive(terminal));
                            let Some(completion) = completion else { return Some(VmStopReason::WaitingForProcedure); };
                            for value in &completion.values[..completion.count] { self.push_value(Value::Int(*value)); }
                            self.thread.clear_current_procedure();
                            return None;
                        }
                        let selected = if cancelled {
                            None
                        } else {
                            api.poll_native_graph_selection()
                        };
                        if !cancelled && selected.is_none() {
                            return Some(VmStopReason::WaitingForProcedure);
                        }
                        let selected = selected.unwrap_or(-1);
                        self.push_value(Value::Int(selected));
                        self.push_value(Value::Int(if cancelled { -1 } else { selected }));
                    }
                    native_thread::NativeGraphProcedureMode::Control => {
                        if cancelled {
                            if let (Some(object_id), Some(control_id)) =
                                (procedure.object_id, procedure.control_id)
                            {
                                api.cancel_native_graph_control_procedure(
                                    installed.source_opcode,
                                    object_id,
                                    control_id,
                                );
                            }
                        }
                        let active = match (procedure.object_id, procedure.control_id) {
                            (Some(object_id), Some(control_id)) => api
                                .poll_native_graph_control_procedure(
                                    installed.source_opcode,
                                    object_id,
                                    control_id,
                                ),
                            (Some(object_id), None) => api.native_graph_object_procedure_is_active(
                                installed.source_opcode,
                                object_id,
                            ),
                            (None, _) => {
                                api.native_graph_procedure_is_active(installed.source_opcode)
                            }
                        };
                        if !cancelled && active {
                            return Some(VmStopReason::WaitingForProcedure);
                        }
                        // 0x431F00 writes two deferred values. The first is
                        // not inherently 1000: early input/cancellation reports
                        // the procedure's pre-terminal progress. Base CProcedure
                        // cancellation is also distinct from input: it exits
                        // before the subclass updater and reports -1.
                        let completion = match (procedure.object_id, procedure.control_id) {
                            (Some(object_id), Some(control_id)) => api
                                .take_native_graph_control_procedure_completion(
                                    installed.source_opcode,
                                    object_id,
                                    control_id,
                                ),
                            _ if cancelled => [0, -1],
                            _ => [1000, 0],
                        };
                        self.push_value(Value::Int(completion[0]));
                        self.push_value(Value::Int(completion[1]));
                    }
                    native_thread::NativeGraphProcedureMode::Shake => {
                        if cancelled {
                            api.cancel_native_graph_procedure(installed.source_opcode);
                        } else if api.native_graph_procedure_is_active(installed.source_opcode) {
                            return Some(VmStopReason::WaitingForProcedure);
                        }
                        // CProcShakeScreen::Tick (0x43CDD0) never calls
                        // sub_4450D0, so unlike CProcCtrlDspObj it publishes
                        // no deferred operand-stack values. Wrapper 0x478200's
                        // return value 2 is the native "procedure installed"
                        // dispatch result, not an output count.
                    }
                }
                self.thread.clear_current_procedure();
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        class = procedure.target_class_name,
                        cancelled,
                        group = format_args!("0x{:02X}", installed.source_opcode.group),
                        id = format_args!("0x{:02X}", installed.source_opcode.id),
                        "graph CProcedure completed through CThread+0x58"
                    );
                }
                None
            }
            CProcedure::Exclusion(procedure) => {
                // CProcExclusion::Tick (sub_439620); no-op callback handler.
                let (terminal, _) = self.drain_procedure_callbacks();
                let cancelled = !self.procedure_alive(terminal);
                let status = if cancelled {
                    -1
                } else {
                    self.system80_shared
                        .lock()
                        .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))
                        .ok()?
                        .exclusions
                        .try_acquire(procedure.section_id, procedure.owner_thread_id)
                };
                if status == system80_state::NATIVE_UNAVAILABLE {
                    return Some(VmStopReason::WaitingForProcedure);
                }
                self.push_value(Value::Int(status));
                self.thread.clear_current_procedure();
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        status,
                        section_id = procedure.section_id,
                        thread_id = procedure.owner_thread_id,
                        "CProcExclusion completed"
                    );
                }
                None
            }
            CProcedure::Unrecovered(_) => {
                // Never guess an elapsed-time completion for an unknown native
                // virtual procedure.  A permanent wait here is intentional: it
                // identifies the exact selector that still needs target-side
                // reverse engineering instead of silently fast-forwarding.
                Some(VmStopReason::WaitingForProcedure)
            }
        }
    }

    fn poll_wait_timing_procedure<A>(&mut self, api: &mut A, trace_events: bool) -> bool
    where
        A: SysApi + GraphApi + SoundApi,
    {
        matches!(
            self.poll_current_procedure(api, trace_events),
            Some(VmStopReason::WaitingForTime | VmStopReason::WaitingForInputOrTime)
        )
    }

    fn take_dispatch_call_frame(&mut self, group: u8, id: u16) -> VmResult<Vec<Value>> {
        let Some(argc) = self.effective_arg_count(group, id) else {
            return Ok(Vec::new());
        };
        let mut args = Vec::with_capacity(argc);
        for _ in 0..argc {
            args.push(self.pop_value()?);
        }
        args.reverse();
        // The handlers' `char *` arguments (sub_48EA40) reach the host as
        // text whatever produced the pointer.
        let encoding = if matches!(group, 0x90..=0x92) {
            self.graph_text_encoding.unwrap_or(encoding_rs::SHIFT_JIS)
        } else {
            encoding_rs::SHIFT_JIS
        };
        let strings = native_strings::string_arguments(group, id);
        let pointers = native_strings::pointer_arguments(group, id);
        for index in 0..args.len() {
            // Literals are code pointers (segment 1): any one the handler
            // does not take as a raw pointer is text for the host.
            let literal = matches!(args[index], Value::Ptr(pointer) if pointer >> 26 == 1)
                && !pointers.contains(&index);
            if !strings.contains(&index) && !literal {
                continue;
            }
            let pointer = match &args[index] {
                Value::Ptr(pointer) => *pointer,
                Value::Int(pointer) => *pointer as u32,
                _ => continue,
            };
            if pointer == 0 {
                continue;
            }
            if let Ok(text) = self.value_as_string_with_encoding(Value::Ptr(pointer), encoding) {
                args[index] = Value::Str(text);
            }
        }
        Ok(args)
    }
    pub fn set_native_abi_profile(&mut self,profile:NativeAbiProfile) {
        self.native_abi_profile=profile;
        for task in &mut self.async_tasks {task.vm.set_native_abi_profile(profile);}
    }
    pub fn native_abi_profile(&self)->NativeAbiProfile {self.native_abi_profile}
    fn effective_arg_count(&self,group:u8,id:u16)->Option<usize> {
        native_abi::lookup_profile(self.native_abi_profile,group,id).map(|a|a.argc).or_else(||known_call_arg_count(group,id))
    }
    fn effective_output_count(&self,group:u8,id:u16)->usize {
        native_abi::lookup_profile(self.native_abi_profile,group,id).map(|a|a.stack_outputs).unwrap_or_else(||known_call_stack_output_count(group,id))
    }
    fn effective_procedure(&self,group:u8,id:u16)->bool {
        native_abi::lookup_profile(self.native_abi_profile,group,id).map(|a|a.installs_procedure).unwrap_or_else(||native_abi::installs_procedure(group,id))
    }
    fn effective_strict_support(&self,opcode:NativeOpcode)->bool {
        if self.native_abi_profile == NativeAbiProfile::Amachoco
            && matches!((opcode.group, opcode.id), (0x81, 0x00) | (0x91, 0x09) | (0x92, 0x99))
        { return true; }
        if self.native_abi_profile==NativeAbiProfile::Amachoco && opcode.group==0x81 && matches!(opcode.id,0x21|0xD8|0xDA) {return true;}
        native_call::is_strictly_supported(opcode)
    }

    fn settle_native_call_outputs(
        &mut self,
        group: u8,
        id: u16,
        call_stack: &mut Vec<Value>,
        result: Value,
    ) -> Value {
        let output_count = self.effective_output_count(group, id);
        if output_count <= 1 {
            return result;
        }

        let stack_output_count = output_count - usize::from(result != Value::None);
        if call_stack.len() < stack_output_count {
            return result;
        }
        let mut outputs = call_stack.split_off(call_stack.len() - stack_output_count);
        if result != Value::None {
            for value in outputs {
                self.push_value(value);
            }
            return result;
        }

        let result = outputs.pop().unwrap_or(Value::None);
        for value in outputs {
            self.push_value(value);
        }
        result
    }

    fn read_spline_control_points(&self, args: &[Value]) -> VmResult<Vec<[i32; 4]>> {
        let count = args
            .get(1)
            .map(Value::as_i32)
            .unwrap_or_default()
            .clamp(0, 256) as usize;
        let ptr = args.get(2).map(Value::as_i32).unwrap_or_default() as u32;
        let mut points = Vec::with_capacity(count);
        for point_index in 0..count {
            let base = ptr.saturating_add((point_index * 16) as u32);
            points.push([
                self.read_int(base, 2)? as i32,
                self.read_int(base.saturating_add(4), 2)? as i32,
                self.read_int(base.saturating_add(8), 2)? as i32,
                self.read_int(base.saturating_add(12), 2)? as i32,
            ]);
        }
        Ok(points)
    }

    /// Enforce the target native ABI at the final VM stack boundary.
    ///
    /// Many legacy host stubs returned `Int(0)` or `Int(1)` merely as a Rust
    /// success convention. For a native handler whose recovered contract has
    /// zero immediate outputs, pushing that value corrupts the BP operand ring
    /// and can turn later message-wait conditions permanently true.
    fn enforce_native_output_contract(
        &mut self,
        kind: &str,
        group: u8,
        id: u16,
        result: Value,
    ) -> Value {
        let output_count = self.effective_output_count(group, id);
        if output_count == 0 {
            if result != Value::None {
                tracing::debug!(
                    kind,
                    group = format_args!("0x{group:02X}"),
                    id = format_args!("0x{id:02X}"),
                    discarded = ?result,
                    "discarded host return because native ABI has zero immediate outputs"
                );
            }
            Value::None
        } else {
            result
        }
    }

    fn audit_native_return(
        &mut self,
        kind: &str,
        group: u8,
        id: u16,
        result: &Value,
        program_index: usize,
        fail_on_stub: bool,
    ) -> VmResult<()> {
        if !native_return_audit_enabled()
            || self.effective_output_count(group,id)==0
            || self.effective_procedure(group,id)
            || *result != Value::None
        {
            return Ok(());
        }

        let key = format!("native-return:{kind}:0x{group:02X}:0x{id:02X}");
        let first_hit = !self.stubs.contains_key(&key);
        *self.stubs.entry(key).or_default() += 1;
        if first_hit {
            tracing::warn!(
                program = self.program_name(program_index),
                pc = self.pc,
                group = format_args!("0x{group:02X}"),
                id = format_args!("0x{id:02X}"),
                kind,
                call = native_call::display_name(NativeOpcode { group, id }),
                "native ABI return value omitted"
            );
        }
        if fail_on_stub {
            return Err(VmError::UnknownDispatch { group, id });
        }
        Ok(())
    }

    fn audit_native_args_consumed(
        &mut self,
        kind: &str,
        group: u8,
        id: u16,
        remaining: usize,
        program_index: usize,
        fail_on_stub: bool,
    ) -> VmResult<()> {
        if remaining == 0 {
            return Ok(());
        }
        let key = format!("native-args:{kind}:0x{group:02X}:0x{id:02X}");
        let first_hit = !self.stubs.contains_key(&key);
        *self.stubs.entry(key).or_default() += 1;
        if first_hit {
            tracing::warn!(
                program = self.program_name(program_index),
                pc = self.pc,
                group = format_args!("0x{group:02X}"),
                id = format_args!("0x{id:02X}"),
                kind,
                call = native_call::display_name(NativeOpcode { group, id }),
                remaining,
                "native ABI arguments left unconsumed"
            );
        }
        if fail_on_stub {
            return Err(VmError::UnknownDispatch { group, id });
        }
        Ok(())
    }

    fn program_parser_code_start(program: &BpProgram) -> u32 {
        program
            .instructions
            .first()
            .map(|instruction| instruction.offset as u32)
            .unwrap_or_default()
    }

    fn program_target_code_size(program: &BpProgram) -> u32 {
        if program.module_size != 0 {
            return program.module_size;
        }
        let start = Self::program_parser_code_start(program);
        program
            .instructions
            .iter()
            .map(|instruction| {
                (instruction.offset as u32)
                    .saturating_add(u32::try_from(instruction.raw.len()).unwrap_or(u32::MAX))
            })
            .max()
            .unwrap_or(start)
            .saturating_sub(start)
    }

    fn active_code_used_end(&self) -> u32 {
        self.programs
            .iter()
            .enumerate()
            .filter(|(index, _)| self.program_active.get(*index).copied().unwrap_or(false))
            .map(|(index, program)| {
                self.program_code_bases
                    .get(index)
                    .copied()
                    .unwrap_or_default()
                    .saturating_add(Self::program_target_code_size(program))
            })
            .max()
            .unwrap_or_default()
            .max(self.code_used_floor)
    }

    /// `CThread+0x38` (`code_used`): the initial module plus every module
    /// appended by `0x80:0x40`, in the strict LIFO chain sub_444CE0 keeps.
    /// Programs started by other means (for example `0xFF script_load`) do not
    /// occupy the thread's code region.
    pub(crate) fn module_chain_code_used(&self) -> u32 {
        let initial = if self.target_loaded_programs.contains(&0) {
            0
        } else {
            self.programs
                .first()
                .map(Self::program_target_code_size)
                .unwrap_or_default()
        };
        self.target_loaded_programs
            .iter()
            .filter_map(|index| self.programs.get(*index))
            .map(Self::program_target_code_size)
            .fold(initial, u32::saturating_add)
    }

    fn sync_thread_program_region(&mut self) {
        let code_used_end = self.active_code_used_end();
        self.thread
            .sync_program_region(self.target_loaded_programs.len(), code_used_end);
        self.sync_region_reservations(code_used_end);
    }

    fn resolve_target_code_offset(&self, target_offset: u32) -> Option<(usize, u32)> {
        // The newest module covering the offset owns those code bytes. Freeing
        // a module only rewinds the chain (sub_444D80), so its code stays
        // callable until a later module is loaded over it; senmomo's ipl._bp
        // calls environ._bp's entry again after FreeLastProgramModule.
        for index in (0..self.programs.len()).rev() {
            let base = self.program_code_bases.get(index).copied()?;
            let program = self.programs.get(index)?;
            let size = Self::program_target_code_size(program);
            if target_offset < base || target_offset >= base.saturating_add(size.max(1)) {
                continue;
            }
            let parser_offset = Self::program_parser_code_start(program)
                .saturating_add(target_offset.saturating_sub(base));
            if program.labels.contains_key(&parser_offset) {
                return Some((index, parser_offset));
            }
        }
        None
    }

    /// A parser (file) offset of `program_index` as the thread's code-region
    /// offset scripts see (push_offset, return addresses).
    fn code_offset_of(&self, program_index: usize, parser_offset: u32) -> u32 {
        let Some(program) = self.programs.get(program_index) else {
            return parser_offset;
        };
        let base = self.program_code_bases.get(program_index).copied().unwrap_or_default();
        base.wrapping_add(parser_offset.wrapping_sub(Self::program_parser_code_start(program)))
    }

    /// A code-region offset back to (module, parser offset): the newest
    /// module covering it, else `program_index`.
    fn resolve_code_target(&self, program_index: usize, code_offset: u32) -> (usize, u32) {
        self.resolve_target_code_offset(code_offset).unwrap_or_else(|| {
            let program = &self.programs[program_index];
            let base = self.program_code_bases.get(program_index).copied().unwrap_or_default();
            (program_index, code_offset.wrapping_sub(base).wrapping_add(Self::program_parser_code_start(program)))
        })
    }

    fn resolve_indirect_call_target(
        &self,
        current_program: usize,
        target_offset: u32,
    ) -> (usize, u32) {
        // Integer call targets can be a Sys80:40 global code-region base plus
        // an entry offset. Local compiler-generated calls are Value::Func and
        // never enter this path, so loaded regions must win numeric collisions
        // with parser-relative labels in the current module.
        self.resolve_target_code_offset(target_offset)
            .or_else(|| {
                self.programs
                    .get(current_program)
                    .filter(|program| program.labels.contains_key(&target_offset))
                    .map(|_| (current_program, target_offset))
            })
            .unwrap_or((current_program, target_offset))
    }

    /// sub_4450B0's memcpy: the module bytes at `base` of the thread's code
    /// region (segment 1).
    fn install_module_code(&mut self, base: u32, code: &[u8]) {
        let start = (LOCAL_MEMORY_BASE + CODE_WINDOW_OFFSET) as usize + base as usize;
        let end = (start + code.len()).min((LOCAL_MEMORY_BASE + CODE_WINDOW_OFFSET + CODE_WINDOW_SIZE) as usize);
        if start >= end {
            return;
        }
        if self.memory.len() < end {
            self.memory.resize(end.next_power_of_two().max(INITIAL_MEMORY_SIZE).min(MAX_MEMORY_SIZE), 0);
        }
        self.memory[start..end].copy_from_slice(&code[..end - start]);
    }

    fn append_target_loaded_program(&mut self, program: BpProgram) -> u32 {
        let code_base = self.active_code_used_end();
        self.install_module_code(code_base, &program.code);
        let index = self.programs.len();
        if let Some(name) = program.script_name.clone() {
            self.program_cache.insert(name, index);
        }
        self.programs.push(program);
        self.program_code_bases.push(code_base);
        self.program_active.push(true);
        self.target_loaded_programs.push(index);
        self.sync_thread_program_region();
        code_base
    }

    fn free_last_target_program(&mut self, trace_events: bool) -> (i32, Option<BpProgram>) {
        let Some(index) = self.target_loaded_programs.pop() else {
            // sub_444D80 returns 0x80000001 when no module record exists.
            return (i32::MIN + 1, None);
        };
        let program = self.programs.get(index).cloned();
        if let Some(active) = self.program_active.get_mut(index) {
            *active = false;
        }
        if let Some(name) = program
            .as_ref()
            .and_then(|program| program.script_name.as_ref())
        {
            if self.program_cache.get(name).copied() == Some(index) {
                self.program_cache.remove(name);
            }
        }
        self.program_free_stack.retain(|loaded| *loaded != index);
        self.sync_thread_program_region();
        if trace_events {
            tracing::info!(
                program_index = index,
                remaining = self.target_loaded_programs.len(),
                "VM removed target CThread loaded-module record"
            );
        }
        (
            i32::try_from(self.target_loaded_programs.len()).unwrap_or(i32::MAX),
            program,
        )
    }

    fn assign_program_instance(&mut self, program: &mut BpProgram) {
        self.next_program_instance_id = self.next_program_instance_id.wrapping_add(1).max(1);
        let name = program
            .script_name
            .get_or_insert_with(|| "<anonymous>".to_string());
        name.push_str(&format!("#instance={}", self.next_program_instance_id));
    }

    fn program_index_for_loaded_program(&mut self, program: BpProgram) -> usize {
        if let Some(name) = program.script_name.as_ref() {
            if let Some(index) = self.program_cache.get(name).copied() {
                if self.program_active.get(index).copied().unwrap_or(false) {
                    return index;
                }
            }
        }
        let index = self.programs.len();
        let code_base = self.active_code_used_end();
        if let Some(name) = program.script_name.clone() {
            self.program_cache.insert(name, index);
        }
        self.install_module_code(code_base, &program.code);
        self.programs.push(program);
        self.program_code_bases.push(code_base);
        self.program_active.push(true);
        self.sync_thread_program_region();
        index
    }

    fn free_next_called_program(&mut self, trace_events: bool) -> Value {
        while let Some(index) = self.program_free_stack.pop() {
            let Some(program) = self.programs.get(index) else {
                continue;
            };
            if trace_events {
                tracing::info!(
                    program_index = index,
                    program = self.program_name(index),
                    "VM FreeProgram resolved last called program"
                );
            }
            return Value::Program(Arc::new(program.clone()));
        }
        Value::None
    }

    fn free_loaded_program_value(&mut self, program: Value, trace_events: bool) -> Value {
        let Some(index) = self.program_index_for_program_value(&program) else {
            if trace_events {
                tracing::info!(
                    program = ?value_summary(&program),
                    "VM FreeProgram ignored non-loaded program"
                );
            }
            return program;
        };
        self.free_loaded_program_index(index, trace_events)
    }

    fn free_loaded_program_index(&mut self, index: usize, trace_events: bool) -> Value {
        let Some(program) = self.programs.get(index).cloned() else {
            return Value::None;
        };
        if index == 0 {
            if trace_events {
                tracing::warn!(
                    program = self.program_name(index),
                    "VM FreeProgram ignored root program"
                );
            }
            return Value::Program(Arc::new(program));
        }

        let name = program
            .script_name
            .clone()
            .unwrap_or_else(|| format!("<program#{index}>"));
        if let Some(script_name) = program.script_name.as_ref() {
            if self.program_cache.get(script_name).copied() == Some(index) {
                self.program_cache.remove(script_name);
            }
        }
        if trace_events {
            tracing::info!(
                program_index = index,
                program = name,
                "VM FreeProgram released handle"
            );
        }
        Value::Program(Arc::new(program))
    }

    fn program_index_for_program_value(&self, value: &Value) -> Option<usize> {
        let program = match value {
            Value::Program(program) => Some(program.as_ref()),
            Value::Ptr(ptr) => self
                .mem_values
                .get(&self.value_key(*ptr))
                .and_then(|value| match value {
                    Value::Program(program) => Some(program.as_ref()),
                    _ => None,
                }),
            Value::Int(ptr) => {
                self.mem_values
                    .get(&self.value_key(*ptr as u32))
                    .and_then(|value| match value {
                        Value::Program(program) => Some(program.as_ref()),
                        _ => None,
                    })
            }
            Value::Str(_) | Value::Func { .. } | Value::None => None,
        }?;

        if let Some(name) = program.script_name.as_ref() {
            if let Some(index) = self.program_cache.get(name).copied() {
                if self
                    .programs
                    .get(index)
                    .and_then(|loaded| loaded.script_name.as_ref())
                    == Some(name)
                {
                    return Some(index);
                }
            }
        }
        self.programs
            .iter()
            .enumerate()
            .find_map(|(index, loaded)| (loaded == program).then_some(index))
    }

    fn push_value(&mut self, value: Value) {
        if self.operand_slots.len() != OPERAND_STACK_CAPACITY {
            self.operand_slots = vec![Value::Int(0); OPERAND_STACK_CAPACITY];
            self.operand_slots_synced_len = 0;
        }
        let sp = self.stack.len();
        self.operand_slots[sp] = value.clone();
        if sp + 1 == OPERAND_STACK_CAPACITY {
            self.stack.clear();
            self.operand_slots_synced_len = 0;
        } else {
            self.stack.push(value);
            self.operand_slots_synced_len = self.stack.len();
        }
        self.thread.sync_operand_index(self.stack.len());
    }

    fn pop_value(&mut self) -> VmResult<Value> {
        if let Some(value) = self.stack.pop() {
            self.operand_slots_synced_len = self.operand_slots_synced_len.min(self.stack.len());
            self.thread.sync_operand_index(self.stack.len());
            return Ok(value);
        }
        if self.operand_slots.len() != OPERAND_STACK_CAPACITY {
            self.operand_slots = vec![Value::Int(0); OPERAND_STACK_CAPACITY];
            self.operand_slots_synced_len = 0;
        }
        let value = self.operand_slots[OPERAND_STACK_CAPACITY - 1].clone();
        self.stack
            .extend_from_slice(&self.operand_slots[..OPERAND_STACK_CAPACITY - 1]);
        self.operand_slots_synced_len = self.stack.len();
        self.thread.sync_operand_index(self.stack.len());
        Ok(value)
    }

    fn sync_operand_slots(&mut self) {
        if self.operand_slots.len() != OPERAND_STACK_CAPACITY {
            self.operand_slots = vec![Value::Int(0); OPERAND_STACK_CAPACITY];
            self.operand_slots_synced_len = 0;
        }
        if self.stack.len() >= OPERAND_STACK_CAPACITY {
            self.trim_operand_stack();
            return;
        }
        let start = self.operand_slots_synced_len.min(self.stack.len());
        for index in start..self.stack.len() {
            self.operand_slots[index] = self.stack[index].clone();
        }
        self.operand_slots_synced_len = self.stack.len();
        self.thread.sync_operand_index(self.stack.len());
    }

    fn replace_stack_value(&mut self, index: usize, value: Value) {
        self.stack[index] = value.clone();
        if index < OPERAND_STACK_CAPACITY {
            self.operand_slots[index] = value;
            self.operand_slots_synced_len = self.operand_slots_synced_len.max(index + 1);
        }
    }

    /// The operand `from_top` values below the top of the stack, as the
    /// native's pops will find it. The operand stack is a ring of
    /// OPERAND_STACK_CAPACITY slots, as in the target: a push into the last
    /// slot starts `stack` again from empty while the values pushed before
    /// stay in `operand_slots`, where `pop_value` reads them back. Scripts
    /// leave unused results on the stack, so the ring wraps all the time and
    /// a native call's arguments can straddle the wrap; their conversions
    /// must reach the ones below it too (a text argument left unconverted is
    /// drawn as "0x...").
    fn operand_from_top(&self, from_top: usize) -> Option<Operand> {
        let depth = self.stack.len();
        if let Some(index) = depth.checked_sub(1 + from_top) {
            return Some(Operand::Stack(index));
        }
        let slot = (OPERAND_STACK_CAPACITY + depth).checked_sub(1 + from_top)?;
        (slot >= depth && slot < self.operand_slots.len()).then_some(Operand::Wrapped(slot))
    }

    fn operand_value(&self, operand: Operand) -> Value {
        match operand {
            Operand::Stack(index) => self.stack[index].clone(),
            Operand::Wrapped(slot) => self.operand_slots[slot].clone(),
        }
    }

    fn replace_operand(&mut self, operand: Operand, value: Value) {
        match operand {
            Operand::Stack(index) => self.replace_stack_value(index, value),
            Operand::Wrapped(slot) => self.operand_slots[slot] = value,
        }
    }

    fn pop_int(&mut self) -> VmResult<i32> {
        Ok(self.pop_value()?.as_i32())
    }

    fn pop_ptr(&mut self) -> VmResult<u32> {
        Ok(self.pop_int()? as u32)
    }

    fn value_as_numeric_operand(&mut self, value: Value) -> VmResult<i32> {
        match value {
            Value::Str(text) => {
                if let Some((&addr, _)) = self
                    .script_records
                    .iter()
                    .find(|(_, existing)| existing.as_str() == text.as_str())
                {
                    // The record lives in a heap block: hand back its
                    // script address.
                    if let Some(ptr) = self.target_heap.address_of(addr - LOCAL_MEMORY_BASE, &self.heap_allocations) {
                        return Ok(ptr as i32);
                    }
                }
                let byte_len = encoding_rs::SHIFT_JIS
                    .encode(&text)
                    .0
                    .len()
                    .saturating_add(1);
                let ptr = self.alloc_heap(byte_len.min(u32::MAX as usize) as u32);
                if ptr == 0 {
                    return Err(VmError::Runtime(
                        "failed to materialize script string".into(),
                    ));
                }
                self.remember_script_record(ptr, &text)?;
                Ok(ptr as i32)
            }
            other => Ok(other.as_i32()),
        }
    }

    /// sub_48E8C0's fatal error for an address no segment maps.
    fn invalid_pointer(ptr: u32) -> VmError {
        VmError::Runtime(format!("無効なポインタ [ ${ptr:08X} ] にアクセスしようとしました"))
    }

    fn resolve_range(&self, ptr: u32, size: usize) -> VmResult<std::ops::Range<usize>> {
        let addr = self.memory_addr(ptr);
        if addr == u32::MAX {
            return Err(Self::invalid_pointer(ptr));
        }
        let start = addr as usize;
        let end = start
            .checked_add(size)
            .ok_or(VmError::MemoryOutOfBounds { addr, size })?;
        if end > self.memory.len() {
            return Err(VmError::MemoryOutOfBounds { addr, size });
        }
        Ok(start..end)
    }

    pub(crate) fn resolve_write_range(
        &mut self,
        ptr: u32,
        size: usize,
    ) -> VmResult<std::ops::Range<usize>> {
        let addr = self.memory_addr(ptr);
        if addr == u32::MAX {
            return Err(Self::invalid_pointer(ptr));
        }
        let start = addr as usize;
        let end = start
            .checked_add(size)
            .ok_or(VmError::MemoryOutOfBounds { addr, size })?;
        if end > MAX_MEMORY_SIZE {
            return Err(VmError::MemoryOutOfBounds { addr, size });
        }
        if end > self.memory.len() {
            let grown = end
                .next_power_of_two()
                .max(INITIAL_MEMORY_SIZE)
                .min(MAX_MEMORY_SIZE);
            self.memory.resize(grown, 0);
        }
        Ok(start..end)
    }

    fn read_int(&self, ptr: u32, width: u8) -> VmResult<u32> {
        if let Some(value) = self.scenario_guarded_read_int(ptr, width) {
            let size = 1usize << width.min(2);
            self.trace_watch_read(
                ptr,
                size,
                &Value::Int(value as i32),
                "scenario_guarded_read_int",
            );
            return Ok(value);
        }
        let size = 1usize << width.min(2);
        let range = self.resolve_range(ptr, size)?;
        let bytes = &self.memory[range];
        let value = match width {
            0 => bytes[0] as u32,
            1 => u16::from_le_bytes([bytes[0], bytes[1]]) as u32,
            _ => u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]),
        };
        self.trace_watch_read(ptr, size, &Value::Int(value as i32), "read_int");
        Ok(value)
    }

    fn write_int(&mut self, ptr: u32, width: u8, value: u32) -> VmResult<()> {
        let size = 1usize << width.min(2);
        let range = self.resolve_write_range(ptr, size)?;
        let bytes = value.to_le_bytes();
        self.memory[range].copy_from_slice(&bytes[..size]);
        // Raw native writes replace the complete value at this address. A
        // stale Ptr/Str/Func shadow must never survive and override the bytes
        // on a later load2. write_value re-adds a typed shadow after this call
        // when the assigned value itself is tagged.
        self.clear_shadow_values(ptr, size);
        self.trace_watch_write(ptr, size, value, "write_int");
        Ok(())
    }

    /// sub_464C50 -> sub_464AD0 / sub_464A80: the error text with the
    /// thread, module, IP, instruction and SP header and the call chain,
    /// written to BGIError.txt; the first error is shown (MB_ICONHAND) and,
    /// while Sys81:6A capture is on, kept for Sys81:6B.
    fn report_script_error<A: SysApi>(
        &mut self,
        api: &mut A,
        program_index: usize,
        parser_offset: u32,
        opcode: u8,
        error: &VmError,
    ) {
        let mut message = format!(
            "Thread [ {} ] , Program [ {} ] , IP [ ${:08X} ] , Instruction [ ${:X} ] , SP [ ${:08X} ]\n\n",
            self.thread.thread_id(),
            self.program_name(program_index),
            self.code_offset_of(program_index, parser_offset),
            opcode,
            FRAME_SEGMENT | self.mem_ptr,
        );
        let frames = self.call_stack.clone();
        for &(program, pc, _) in frames.iter().rev() {
            let offset = self
                .programs
                .get(program)
                .and_then(|module| module.instructions.get(pc))
                .map_or(0, |instruction| self.code_offset_of(program, instruction.offset as u32));
            let base = self.program_code_bases.get(program).copied().unwrap_or_default();
            message.push_str(&format!("( {} - ${:08X} )\n", self.program_name(program), offset.wrapping_sub(base)));
        }
        if !frames.is_empty() {
            message.push('\n');
        }
        message.push_str(&error.to_string());
        let root = self
            .validated_file_root
            .clone()
            .unwrap_or_else(|| self.resource_primary_root(api));
        let (bytes, _, _) = encoding_rs::SHIFT_JIS.encode(&message);
        api.write_file_bytes(&format!("{root}BGIError.txt"), &bytes);
        let mut shared = self.system81_shared.lock().expect("system81 state poisoned");
        if shared.captured_error.is_none() {
            let capture = shared.error_capture_mode != 0;
            drop(shared);
            api.message_box("", &message);
            if capture {
                self.system81_shared.lock().expect("system81 state poisoned").captured_error = Some(message);
            }
        }
    }

    fn write_return_addr(&mut self, program_index: usize, next_pc: usize) -> VmResult<()> {
        let return_offset = self
            .programs
            .get(program_index)
            .and_then(|program| program.instructions.get(next_pc))
            .map(|instruction| self.code_offset_of(program_index, instruction.offset as u32))
            .unwrap_or_default();
        // sub_473FD0: the return address must fit below the data limit
        // (sub_445010: base + free limit, lowered by child reservations).
        if self.mem_ptr.wrapping_add(4) >= self.data_region_limit() {
            return Err(VmError::Runtime("スタック領域が不足しています".into()));
        }
        self.write_int(FRAME_SEGMENT | self.mem_ptr, 2, return_offset)?;
        self.mem_ptr = self.mem_ptr.saturating_add(4);
        Ok(())
    }

    fn read_return_addr(&mut self) -> VmResult<u32> {
        self.mem_ptr = self.mem_ptr.saturating_sub(4);
        self.read_int(FRAME_SEGMENT | self.mem_ptr, 2)
    }

    fn read_value(&self, ptr: u32, width: u8) -> VmResult<Value> {
        let addr = self.value_key(ptr);
        if width >= 2 {
            if let Some(value) = self.mem_values.get(&addr) {
                let size = 1usize << width.min(2);
                self.trace_watch_read(ptr, size, value, "read_value_tagged");
                return Ok(value.clone());
            }
        }
        // Target BP opcode 0x08 is sub_473680. Its width selectors load
        // `char`, `__int16`, and `int` respectively before passing the value
        // to sub_4450D0. The narrow loads are therefore sign-extended. Keep
        // read_int() itself raw/unsigned because native structure readers use
        // it for byte/word fields whose signedness is selector-specific.
        let raw = self.read_int(ptr, width)?;
        let value = match width {
            0 => raw as u8 as i8 as i32,
            1 => raw as u16 as i16 as i32,
            _ => raw as i32,
        };
        Ok(Value::Int(value))
    }

    fn read_descriptor_values(&self, descriptor: Value, count: usize) -> VmResult<Vec<Value>> {
        let ptr = match descriptor {
            Value::Ptr(ptr) => ptr,
            Value::Int(value) if value != 0 => value as u32,
            _ => return Ok(Vec::new()),
        };
        let mut values = Vec::with_capacity(count);
        for index in 0..count {
            values.push(self.read_value(ptr.saturating_add((index * 4) as u32), 2)?);
        }
        Ok(values)
    }

    /// Validate the two target DCIP descriptor families without turning
    /// script-data errors into fatal VM memory errors. Returns target status
    /// 2 for an invalid root and 3 for an invalid nested group.
    fn validate_graph_input_descriptor(&self, ptr: u32, extended: bool) -> VmResult<Option<i32>> {
        let root_size = if extended { 40_usize } else { 32_usize };
        if ptr == 0 || self.resolve_range(ptr, root_size).is_err() {
            return Ok(Some(2));
        }
        let group_count = self.read_int(ptr, 2)? as i32;
        let groups_ptr = self.read_pointer_field(ptr.wrapping_add(4))?;
        if !(1..=256).contains(&group_count) || groups_ptr == 0 {
            return Ok(Some(2));
        }
        let group_stride = if extended { 64_usize } else { 52_usize };
        let region_stride = if extended { 196_usize } else { 60_usize };
        let regions_offset = if extended { 8_u32 } else { 4_u32 };
        let groups_size = usize::try_from(group_count)
            .ok()
            .and_then(|count| count.checked_mul(group_stride));
        if groups_size
            .and_then(|size| self.resolve_range(groups_ptr, size).ok())
            .is_none()
        {
            return Ok(Some(2));
        }
        for group in 0..group_count as u32 {
            let group_ptr = groups_ptr.wrapping_add(group.wrapping_mul(group_stride as u32));
            let region_count = (self.read_int(group_ptr, 2)? & 0xffff) as i32;
            let regions_ptr = self.read_pointer_field(group_ptr.wrapping_add(regions_offset))?;
            if !(1..=256).contains(&region_count) || regions_ptr == 0 {
                return Ok(Some(3));
            }
            let regions_size = usize::try_from(region_count)
                .ok()
                .and_then(|count| count.checked_mul(region_stride));
            if regions_size
                .and_then(|size| self.resolve_range(regions_ptr, size).ok())
                .is_none()
            {
                return Ok(Some(3));
            }
        }
        Ok(None)
    }

    /// Extended-descriptor recheck in sub_44B260: every 64-byte group's
    /// count dword (not just its low word) must be in 1..=256.
    fn graph_input_group_counts_exceed_range(&self, ptr: u32) -> VmResult<bool> {
        let group_count = self.read_int(ptr, 2)? as i32;
        let groups_ptr = self.read_pointer_field(ptr.wrapping_add(4))?;
        for group in 0..group_count.clamp(0, 256) as u32 {
            let count = self.read_int(groups_ptr.wrapping_add(group * 64), 2)? as i32;
            if !(1..=256).contains(&count) {
                return Ok(true);
            }
        }
        Ok(false)
    }

    fn read_graph_input_descriptor(&self, ptr: u32) -> VmResult<GraphInputDescriptor> {
        const GROUP_STRIDE: u32 =
            std::mem::size_of::<native_input::InputGroupDescriptorLayout32>() as u32;
        const REGION_STRIDE: u32 =
            std::mem::size_of::<native_input::InputRegionDescriptorLayout32>() as u32;
        const MAX_GROUPS: usize = 256;
        const MAX_REGIONS_PER_GROUP: usize = 256;

        let group_count = (self.read_int(ptr, 2)? as i32).clamp(0, MAX_GROUPS as i32) as usize;
        let groups_ptr = self.read_pointer_field(ptr.wrapping_add(4))?;
        tracing::debug!(
            descriptor = format_args!("0x{ptr:08X}"),
            group_count,
            groups = format_args!("0x{groups_ptr:08X}"),
            "read graph input descriptor"
        );
        let mut flags = [0; 7];
        for (index, flag) in flags.iter_mut().enumerate() {
            *flag = self.read_int(ptr.wrapping_add(12 + (index * 4) as u32), 2)? as i32;
        }
        let mut descriptor = GraphInputDescriptor {
            initial_group: self.read_int(ptr.wrapping_add(8), 2)? as i32,
            flags,
            compact_uses_valid_region_origin: false,
            // sub_44A900 copies the 40-byte root and stores
            // DCIPIcon+0x88 = (root+0x20 == 0).
            pointer_processing_enabled: self.read_int(ptr.wrapping_add(32), 2)? == 0,
            groups: Vec::with_capacity(group_count),
            regions: Vec::new(),
        };
        if groups_ptr == 0 {
            return Ok(descriptor);
        }

        let mut ordinal = 0i32;
        for group in 0..group_count {
            let group_ptr = groups_ptr.wrapping_add(group as u32 * GROUP_STRIDE);
            let region_count =
                (self.read_int(group_ptr, 2)? & 0xffff).min(MAX_REGIONS_PER_GROUP as u32) as usize;
            let regions_ptr = self.read_pointer_field(group_ptr.wrapping_add(8))?;
            let selected_index = self.read_int(group_ptr.wrapping_add(12), 2)? as i32;
            // sub_44A900 projects the 64-byte extended source group into the
            // common 52-byte runtime group: source +0x14/+0x18/+0x1C/+0x20
            // become internal +0x0C/+0x10/+0x14/+0x18 respectively.
            descriptor.groups.push(GraphInputGroup { navigation_columns: self.read_int(group_ptr.wrapping_add(4),2)? as i32,
                index: group as i32,
                initial_current_item: selected_index,
                selection_enabled: self.read_int(group_ptr.wrapping_add(20), 2)? != 0,
                pointer_selection_enabled: self.read_int(group_ptr.wrapping_add(24), 2)? != 0,
                pointer_activation_enabled: self.read_int(group_ptr.wrapping_add(28), 2)? != 0,
                selection_exclusion_key: self.read_int(group_ptr.wrapping_add(32), 2)? as i32,
                // DCIPIconEx vtable+0x48 (`sub_44C6F0`) reads source group
                // byte +0x3C bit 0x02 together with item+0xC0 bit 0x20. A
                // set bit defers activation to MouseRelease; it does not
                // disable the item.
                extended_flags: self.read_int(group_ptr.wrapping_add(60), 2)? as i32,
            });
            tracing::debug!(
                group,
                group_ptr = format_args!("0x{group_ptr:08X}"),
                region_count,
                regions = format_args!("0x{regions_ptr:08X}"),
                "read graph input group"
            );
            if regions_ptr == 0 {
                continue;
            }
            for index in 0..region_count {
                let region_ptr = regions_ptr.wrapping_add(index as u32 * REGION_STRIDE);
                let enabled_depth = self.read_int(region_ptr.wrapping_add(4), 2)? as i32;
                let x = self.read_int(region_ptr.wrapping_add(8), 2)? as i32;
                let y = self.read_int(region_ptr.wrapping_add(12), 2)? as i32;
                let width = self.read_int(region_ptr.wrapping_add(16), 2)? as i32;
                let height = self.read_int(region_ptr.wrapping_add(20), 2)? as i32;
                // DCIPIconEx::SelectItemBitmap (target sub_44BA40) reads four
                // distinct visual-state slots at +0x20/+0x24/+0x28/+0x2C.
                // The old port skipped +0x24/+0x2C and therefore rendered a
                // keyboard/current resource for plain pointer hover.
                let normal_resource = self.read_int(region_ptr.wrapping_add(32), 2)? as i32;
                let hover_resource = self.read_int(region_ptr.wrapping_add(36), 2)? as i32;
                let selected_resource = self.read_int(region_ptr.wrapping_add(40), 2)? as i32;
                let hover_selected_resource = self.read_int(region_ptr.wrapping_add(44), 2)? as i32;
                // sub_44A900 clears the item Virtual's hit test (sub_41BDD0)
                // for mask -2 and also when item+0x00 is zero: such an item
                // is drawn but never hovered or clicked. The log's entries
                // without a voice are such items.
                let mask_resource = match self.read_int(region_ptr, 2)? {
                    0 => -2,
                    _ => self.read_int(region_ptr.wrapping_add(48), 2)? as i32,
                };
                // sub_44C110 reads extended source item+0x34 before the
                // CDspObjVirtual hit query. A nonzero value excludes the
                // group's already-current item from fresh pointer hits.
                let current_selection_hit_excluded =
                    self.read_int(region_ptr.wrapping_add(52), 2)? != 0;
                let region_flags = self.read_int(region_ptr.wrapping_add(192), 2)? as i32;
                tracing::debug!(
                    group,
                    index,
                    region_ptr = format_args!("0x{region_ptr:08X}"),
                    x,
                    y,
                    width,
                    height,
                    normal_resource,
                    selected_resource,
                    hover_resource,
                    hover_selected_resource,
                    mask_resource,
                    current_selection_hit_excluded,
                    flags = format_args!("0x{region_flags:08X}"),
                    "read graph input region"
                );
                // sub_44A900 copies every 196-byte source item into its
                // internal descriptor before attempting bitmap resolution. A
                // missing normal bitmap does not erase or renumber the logical
                // item: the configure-time selected bitmap may be used instead,
                // and a failed sub_407F20 merely leaves that item's live child
                // pointer null. Preserve the complete item table here and let
                // RuntimeTraceApi materialize only the children it can resolve.
                descriptor.regions.push(GraphInputRegion {
                    group: group as i32,
                    index: index as i32,
                    ordinal,
                    enabled_depth,
                    selected: index as i32 == selected_index,
                    x,
                    y,
                    width,
                    height,
                    normal_resource,
                    selected_resource,
                    hover_resource,
                    hover_selected_resource,
                    mask_resource,
                    current_selection_hit_excluded,
                    flags: region_flags,
                });
                ordinal = ordinal.saturating_add(1);
            }
            // sub_44A900 invalidates the group's configure-time current item
            // when that item is not enabled (the extended source tests
            // item+0x04 before keeping source group+0x0C).  Do this in the
            // parsed descriptor as well so BE/current-item visuals cannot
            // point at a region the target removed from selection.
            let current_is_enabled = selected_index >= 0
                && descriptor.regions.iter().any(|region| {
                    region.group == group as i32
                        && region.index == selected_index
                        && region.enabled_depth != 0
                });
            if selected_index >= 0 && !current_is_enabled {
                if let Some(group_record) = descriptor.groups.get_mut(group) {
                    group_record.initial_current_item = -1;
                }
                for region in descriptor
                    .regions
                    .iter_mut()
                    .filter(|region| region.group == group as i32)
                {
                    region.selected = false;
                }
            }
        }
        Ok(descriptor)
    }

    fn read_compact_graph_input_descriptor(&self, ptr: u32) -> VmResult<GraphInputDescriptor> {
        // funcs_48065E[0xBA] -> sub_47EF00 -> sub_46C8E0 -> sub_46C750
        // copies a 32-byte root, 52-byte groups, and 60-byte regions.
        const GROUP_STRIDE: u32 = 52;
        const REGION_STRIDE: u32 = 60;
        const MAX_GROUPS: usize = 256;
        const MAX_REGIONS_PER_GROUP: usize = 256;

        let group_count = (self.read_int(ptr, 2)? as i32).clamp(0, MAX_GROUPS as i32) as usize;
        let groups_ptr = self.read_pointer_field(ptr.wrapping_add(4))?;
        let mut flags = [0; 7];
        for (index, flag) in flags.iter_mut().take(5).enumerate() {
            *flag = self.read_int(ptr.wrapping_add(12 + (index * 4) as u32), 2)? as i32;
        }
        let mut descriptor = GraphInputDescriptor {
            initial_group: self.read_int(ptr.wrapping_add(8), 2)? as i32,
            flags,
            compact_uses_valid_region_origin: true,
            // Base DCIPIcon constructor sub_447990 initializes this[34]=1;
            // the 32-byte compact descriptor has no extended disable field.
            pointer_processing_enabled: true,
            groups: Vec::with_capacity(group_count),
            regions: Vec::new(),
        };
        if groups_ptr == 0 {
            return Ok(descriptor);
        }

        let mut ordinal = 0i32;
        for group in 0..group_count {
            let group_ptr = groups_ptr.wrapping_add(group as u32 * GROUP_STRIDE);
            let region_count =
                (self.read_int(group_ptr, 2)? & 0xffff).min(MAX_REGIONS_PER_GROUP as u32) as usize;
            let regions_ptr = self.read_pointer_field(group_ptr.wrapping_add(4))?;
            let selected_index = self.read_int(group_ptr.wrapping_add(8), 2)? as i32;
            descriptor.groups.push(GraphInputGroup { navigation_columns: (self.read_int(group_ptr,2)?>>16) as i32,
                index: group as i32,
                initial_current_item: selected_index,
                selection_enabled: self.read_int(group_ptr.wrapping_add(12), 2)? != 0,
                pointer_selection_enabled: self.read_int(group_ptr.wrapping_add(16), 2)? != 0,
                pointer_activation_enabled: self.read_int(group_ptr.wrapping_add(20), 2)? != 0,
                selection_exclusion_key: self.read_int(group_ptr.wrapping_add(24), 2)? as i32,
                extended_flags: 0,
            });
            if regions_ptr == 0 {
                continue;
            }
            for index in 0..region_count {
                let region_ptr = regions_ptr.wrapping_add(index as u32 * REGION_STRIDE);
                let enabled_depth = self.read_int(region_ptr, 2)? as i32;
                let x = self.read_int(region_ptr.wrapping_add(4), 2)? as i32;
                let y = self.read_int(region_ptr.wrapping_add(8), 2)? as i32;
                // DCIPIcon::SelectItemBitmap (target sub_4499F0) reads
                // normal/current/pointer-hover from +0x0C/+0x10/+0x14.  The
                // prior parser skipped +0x14, which made hover use the wrong
                // bitmap state.
                let normal_resource = self.read_int(region_ptr.wrapping_add(12), 2)? as i32;
                let selected_resource = self.read_int(region_ptr.wrapping_add(16), 2)? as i32;
                let hover_resource = self.read_int(region_ptr.wrapping_add(20), 2)? as i32;
                let mask_resource = self.read_int(region_ptr.wrapping_add(24), 2)? as i32;
                let region_flags = self.read_int(region_ptr.wrapping_add(56), 2)? as i32;
                descriptor.regions.push(GraphInputRegion {
                    group: group as i32,
                    index: index as i32,
                    ordinal,
                    enabled_depth,
                    selected: index as i32 == selected_index,
                    x,
                    y,
                    width: 0,
                    height: 0,
                    normal_resource,
                    selected_resource,
                    hover_resource,
                    hover_selected_resource: -1,
                    mask_resource,
                    current_selection_hit_excluded: false,
                    flags: region_flags,
                });
                ordinal = ordinal.saturating_add(1);
            }
            // Compact sub_447C10 performs the same guard directly:
            // `if (item == group.current && !item+0x00) group.current = -1`.
            // Preserve that configure-time rule instead of carrying a stale
            // current item into BD/BE and visual selection state.
            let current_is_enabled = selected_index >= 0
                && descriptor.regions.iter().any(|region| {
                    region.group == group as i32
                        && region.index == selected_index
                        && region.enabled_depth != 0
                });
            if selected_index >= 0 && !current_is_enabled {
                if let Some(group_record) = descriptor.groups.get_mut(group) {
                    group_record.initial_current_item = -1;
                }
                for region in descriptor
                    .regions
                    .iter_mut()
                    .filter(|region| region.group == group as i32)
                {
                    region.selected = false;
                }
            }
        }
        Ok(descriptor)
    }

    fn read_pointer_field(&self, ptr: u32) -> VmResult<u32> {
        Ok(match self.read_value(ptr, 2)? {
            Value::Ptr(value) => value,
            Value::Int(value) if value != 0 => value as u32,
            _ => 0,
        })
    }

    /// sub_4854B0 -> sub_403BA0: font number (p2), size (p3), width
    /// percent (p4) and style (p5) go through sub_4035A0, whose failures
    /// are returned, not raised: no face 0x80000003, size outside 4..=200
    /// 0x80000001, width outside 25..=200 0x80000002. sub_4352B0 without the
    /// glyph flag (p6) advances size*width/100 per double-byte character,
    /// half that per single byte, plus the 91:98 character spacing after
    /// every character but the last. The glyph-box path (p6 != 0) uses GDI
    /// metrics; it shares this cell arithmetic here.
    fn handle_text_measure_call<A: GraphApi + ?Sized>(&mut self, api: &mut A) -> VmResult<Value> {
        let _glyph_metrics = self.pop_int()?;
        let _style = self.pop_int()?;
        let width_percent = self.pop_int()?;
        let font_size = self.pop_int()?;
        let font = self.pop_int()?;
        let text = self.pop_native_string_bytes()?;
        let dest = self.pop_ptr()?;
        if !api.native_font_registered(font) {
            return Ok(Value::Int(0x8000_0003_u32 as i32));
        }
        if !(4..=200).contains(&font_size) {
            return Ok(Value::Int(0x8000_0001_u32 as i32));
        }
        if !(25..=200).contains(&width_percent) {
            return Ok(Value::Int(0x8000_0002_u32 as i32));
        }
        let cell = font_size * width_percent / 100;
        let spacing = api.text_character_spacing();
        let mut width = 0i32;
        if self.native_utf8_enabled() {
            // BP code literals still contain CP932 bytes in the Japanese release.
            let text = std::str::from_utf8(&text)
                .map(Cow::Borrowed)
                .unwrap_or_else(|_| encoding_rs::SHIFT_JIS.decode(&text).0);
            for unit in text.encode_utf16() {
                let narrow = unit < 0x80 || (0xff61..=0xff9f).contains(&unit);
                width = width
                    .wrapping_add(if narrow { cell / 2 } else { cell })
                    .wrapping_add(spacing);
            }
            self.write_int(dest, 2, width.wrapping_sub(spacing) as u32)?;
            return Ok(Value::Int(0));
        }
        let mut index = 0;
        while index < text.len() {
            let double = is_sjis_lead_byte(text[index]) && text.get(index + 1).is_some_and(|&b| b != 0);
            width = width
                .wrapping_add(if double { cell } else { cell / 2 })
                .wrapping_add(spacing);
            index += if double { 2 } else { 1 };
        }
        self.write_int(dest, 2, width.wrapping_sub(spacing) as u32)?;
        Ok(Value::Int(0))
    }

    fn handle_character_widths_call<A: GraphApi + ?Sized>(&mut self, api: &mut A) -> VmResult<Value> {
        // amachoco.exe sub_490540 -> sub_4041C0: optional ABC-width array,
        // optional UTF-16 character count, text, font, size, width%, style.
        let _style = self.pop_int()?;
        let width_percent = self.pop_int()?;
        let font_size = self.pop_int()?;
        let font = self.pop_int()?;
        let text = self.pop_string_lossy()?;
        let count_destination = self.pop_ptr()?;
        let destination = self.pop_ptr()?;
        if !api.native_font_registered(font) {
            return Ok(Value::Int(0x8000_0003_u32 as i32));
        }
        if !(4..=200).contains(&font_size) {
            return Ok(Value::Int(0x8000_0001_u32 as i32));
        }
        if !(25..=200).contains(&width_percent) {
            return Ok(Value::Int(0x8000_0002_u32 as i32));
        }
        let units: Vec<u16> = text.encode_utf16().collect();
        if destination != 0 {
            self.resolve_write_range(destination, units.len() * 12)?;
        }
        if count_destination != 0 {
            self.resolve_write_range(count_destination, 4)?;
        }
        // Use the same cell approximation as Graph91:9B. Native GDI returns
        // the left bearing, black-box width, and right bearing for each unit.
        let cell = font_size * width_percent / 100;
        if destination != 0 {
            for (index, unit) in units.iter().enumerate() {
                let narrow = *unit < 0x80 || (0xff61..=0xff9f).contains(unit);
                let width = if narrow { cell / 2 } else { cell };
                let address = destination.wrapping_add((index * 12) as u32);
                for (offset, value) in [0, width, 0].into_iter().enumerate() {
                    self.write_int(address.wrapping_add((offset * 4) as u32), 2, value as u32)?;
                }
            }
            self.clear_shadow_values(destination, units.len() * 12);
        }
        if count_destination != 0 {
            self.write_int(count_destination, 2, units.len() as u32)?;
            self.clear_shadow_values(count_destination, 4);
        }
        Ok(Value::Int(0))
    }

    fn native_utf8_enabled(&self) -> bool {
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .native_utf8_override
            .unwrap_or(self.native_abi_profile == NativeAbiProfile::Amachoco)
    }

    fn write_value(&mut self, ptr: u32, width: u8, value: &Value) -> VmResult<()> {
        let addr = self.value_key(ptr);
        self.write_int(ptr, width, value.as_i32() as u32)?;
        if width >= 2 {
            match value {
                // Pointers and code offsets are raw cells like the target's;
                // only host-side strings and program handles have no bytes.
                Value::Int(_) | Value::None | Value::Ptr(_) | Value::Func { .. } => {
                    self.mem_values.remove(&addr);
                }
                Value::Str(_) | Value::Program(_) => {
                    self.mem_values.insert(addr, value.clone());
                }
            }
        } else {
            self.mem_values.remove(&addr);
        }
        Ok(())
    }

    fn trace_watch_write(&self, ptr: u32, size: usize, value: u32, source: &'static str) {
        self.trace_watch_access(
            ptr,
            size,
            Some(value),
            None,
            source,
            "VM watched memory write",
        );
    }

    fn trace_watch_read(&self, ptr: u32, size: usize, value: &Value, source: &'static str) {
        self.trace_watch_access(
            ptr,
            size,
            None,
            Some(value),
            source,
            "VM watched memory read",
        );
    }

    fn trace_watch_access(
        &self,
        ptr: u32,
        size: usize,
        write_value: Option<u32>,
        read_value: Option<&Value>,
        source: &'static str,
        message: &'static str,
    ) {
        let watches = trace_watch_addresses();
        if watches.is_empty() {
            return;
        }
        let start = self.memory_addr(ptr);
        let end = start.saturating_add(size as u32);
        let hit = watches
            .iter()
            .copied()
            .find(|watch| *watch >= start && *watch < end);
        if let Some(watch) = hit {
            let watch_value = (watch as usize)
                .checked_add(4)
                .filter(|end| *end <= self.memory.len())
                .map(|end| {
                    let bytes = &self.memory[end - 4..end];
                    u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]])
                });
            let program = self
                .programs
                .get(self.current_program)
                .and_then(|program| program.script_name.as_deref())
                .unwrap_or("<anonymous>");
            let offset = self
                .programs
                .get(self.current_program)
                .and_then(|program| program.instructions.get(self.pc))
                .map(|instruction| instruction.offset);
            tracing::warn!(
                pc = self.pc,
                offset = offset.map(|offset| format!("0x{offset:08X}")).as_deref(),
                program,
                ptr = format_args!("0x{ptr:08X}"),
                addr = format_args!("0x{start:08X}"),
                size,
                write_value = write_value.map(|value| format!("0x{value:08X}")).as_deref(),
                read_value = read_value.map(value_summary).as_deref(),
                watch = format_args!("0x{watch:08X}"),
                watch_value = watch_value.map(|value| format!("0x{value:08X}")).as_deref(),
                source,
                "{message}"
            );
        }
    }

    fn alloc_heap(&mut self, size: u32) -> u32 {
        let requested = size;
        if !self.target_heap.has_room(requested) {
            return 0;
        }
        let size = size.max(4).saturating_add(3) & !3;
        let previous_heap_ptr = self.heap_ptr;
        let offset = if let Some(index) = self
            .heap_free_blocks
            .iter()
            .position(|&(offset, available)| Self::heap_free_block_takes(offset, available, size))
        {
            let (offset, available) = self.heap_free_blocks[index];
            if available == size {
                self.heap_free_blocks.remove(index);
            } else {
                self.heap_free_blocks[index] = (offset + size, available - size);
            }
            offset
        } else {
            let mut offset = self.heap_ptr;
            if !Self::heap_block_fits_segment(offset, size) {
                offset = offset
                    .checked_div(AUX_MEMORY_SEGMENT_SIZE)
                    .and_then(|segment| segment.checked_add(1))
                    .and_then(|segment| segment.checked_mul(AUX_MEMORY_SEGMENT_SIZE))
                    .unwrap_or(u32::MAX);
            }
            let Some(next) = offset.checked_add(size).filter(|next| {
                LOCAL_MEMORY_BASE
                    .checked_add(*next)
                    .is_some_and(|physical_end| physical_end as usize <= MAX_MEMORY_SIZE)
            }) else {
                // Scripts written for the target's process heap don't check
                // for a null block, so this is worth seeing in any log.
                tracing::warn!(
                    thread = self.thread.thread_id(),
                    requested = size,
                    heap_ptr = format_args!("0x{:08X}", self.heap_ptr),
                    allocation_count = self.heap_allocations.len(),
                    free_block_count = self.heap_free_blocks.len(),
                    "VM heap allocation exhausted the tagged address space"
                );
                return 0;
            };
            self.heap_ptr = next;
            offset
        };
        let ptr = self
            .target_heap
            .alloc(requested, offset)
            .expect("checked by TargetHeap::has_room");
        let Ok(range) = self.resolve_write_range(ptr, size as usize) else {
            return 0;
        };
        self.memory[range].fill(0);
        self.clear_shadow_values(ptr, size as usize);
        self.heap_allocations.insert(offset, size);
        if std::env::var_os("TRACE_HEAP").is_some() {
            tracing::info!(
                thread = self.thread.thread_id(),
                requested = size,
                ptr = format_args!("0x{ptr:08X}"),
                previous_heap_ptr = format_args!("0x{previous_heap_ptr:08X}"),
                heap_ptr = format_args!("0x{:08X}", self.heap_ptr),
                allocation_count = self.heap_allocations.len(),
                free_block_count = self.heap_free_blocks.len(),
                "VM heap allocation"
            );
        }
        ptr
    }

    fn free_heap(&mut self, ptr: u32) -> bool {
        if ptr == 0 {
            return true;
        }
        let Some((offset, 0)) = self.target_heap.translate(ptr) else {
            return false;
        };
        let Some(size) = self.heap_allocations.remove(&offset) else {
            return false;
        };
        self.clear_shadow_values(ptr, size as usize);
        self.target_heap.free(ptr);
        self.heap_free_blocks.push((offset, size));
        self.heap_free_blocks.sort_unstable_by_key(|block| block.0);
        let mut merged: Vec<(u32, u32)> = Vec::with_capacity(self.heap_free_blocks.len());
        for (offset, size) in self.heap_free_blocks.drain(..) {
            if let Some((previous_offset, previous_size)) = merged.last_mut() {
                if previous_offset.saturating_add(*previous_size) == offset
                    && Self::heap_block_fits_segment(
                        *previous_offset,
                        previous_size.saturating_add(size),
                    )
                {
                    *previous_size = previous_size.saturating_add(size);
                    continue;
                }
            }
            merged.push((offset, size));
        }
        self.heap_free_blocks = merged;
        if std::env::var_os("TRACE_HEAP").is_some() {
            tracing::info!(
                thread = self.thread.thread_id(),
                ptr = format_args!("0x{ptr:08X}"),
                size,
                heap_ptr = format_args!("0x{:08X}", self.heap_ptr),
                allocation_count = self.heap_allocations.len(),
                free_block_count = self.heap_free_blocks.len(),
                "VM heap free"
            );
        }
        true
    }

    /// Whether the free block at `offset` can take a request of `size`.
    /// A request larger than a segment starts on a segment boundary, as a
    /// new one does (`heap_block_fits_segment` never holds for it), so it
    /// can take a free block of that kind. Such blocks are kept for such
    /// requests: a small block carved from their start would leave no
    /// aligned space, and each large request would then take new address
    /// space. Wonderful Everyday's save routine allocates (400 + 1) * 88124
    /// bytes, a little over a segment, and frees it every time; without the
    /// reuse each save took two more segments and the fourth one got a null
    /// block, which the script then filled from address 0 over the globals.
    fn heap_free_block_takes(offset: u32, available: u32, size: u32) -> bool {
        if available < size {
            return false;
        }
        let large_aligned =
            available > AUX_MEMORY_SEGMENT_SIZE && offset % AUX_MEMORY_SEGMENT_SIZE == 0;
        if size > AUX_MEMORY_SEGMENT_SIZE {
            large_aligned
        } else {
            !large_aligned && Self::heap_block_fits_segment(offset, size)
        }
    }

    fn heap_block_fits_segment(offset: u32, size: u32) -> bool {
        size <= AUX_MEMORY_SEGMENT_SIZE
            && (offset & ADDRESS_MASK)
                .checked_add(size)
                .is_some_and(|end| end <= AUX_MEMORY_SEGMENT_SIZE)
    }

    fn rand_msvc(&mut self) -> i32 {
        self.rng_seed = self.rng_seed.wrapping_mul(214013).wrapping_add(2531011);
        ((self.rng_seed >> 16) & 0x7fff) as i32
    }

    fn rand_msvc_with_api<A>(&mut self, api: &mut A) -> i32
    where
        A: SysApi + ?Sized,
    {
        api.next_native_crt_rand()
            .unwrap_or_else(|| self.rand_msvc())
    }

    fn rand_msvc_wide(&mut self) -> i32 {
        let high = self.rand_msvc() << 8;
        let mid = self.rand_msvc();
        let value = (high ^ mid) << 8;
        value ^ self.rand_msvc()
    }

    fn read_c_string(&self, ptr: u32) -> VmResult<String> {
        self.read_c_string_with_encoding(ptr, encoding_rs::SHIFT_JIS)
    }

    fn read_c_string_with_encoding(
        &self,
        ptr: u32,
        encoding: &'static encoding_rs::Encoding,
    ) -> VmResult<String> {
        let bytes = self.read_c_string_bytes(ptr)?;
        // This confirmed native revision's81:21 and sentence-table lookup
        // produce UTF-8 buffers. Raw memcpy/strcpy may intentionally drop the
        // synthetic String shadow; preserve the actual native bytes rather
        // than reinterpret valid UTF-8 as legacy CP932/graph GBK mojibake.
        if self.native_utf8_enabled() {
            if let Ok(text)=std::str::from_utf8(&bytes) {return Ok(text.to_owned());}
        }
        let bytes = if encoding == encoding_rs::SHIFT_JIS {
            engine_shift_jis_bytes(&bytes)
        } else {
            Cow::Borrowed(bytes.as_slice())
        };
        let (text, _, _) = encoding.decode(&bytes);
        Ok(text.into_owned())
    }

    fn read_c_string_bytes(&self, ptr: u32) -> VmResult<Vec<u8>> {
        let start = self.memory_addr(ptr) as usize;
        if start >= self.memory.len() {
            return Err(VmError::MemoryOutOfBounds { addr: ptr, size: 1 });
        }
        let end = self.memory[start..]
            .iter()
            .position(|byte| *byte == 0)
            .map(|offset| start + offset)
            .unwrap_or(self.memory.len());
        Ok(self.memory[start..end].to_vec())
    }

    fn read_wide_c_string(&self, ptr: u32) -> VmResult<Vec<u16>> {
        let start = self.memory_addr(ptr) as usize;
        if start >= self.memory.len() {
            return Err(VmError::MemoryOutOfBounds { addr: ptr, size: 2 });
        }
        let mut values = Vec::new();
        let mut cursor = start;
        while cursor.saturating_add(2) <= self.memory.len() {
            let value = u16::from_le_bytes([self.memory[cursor], self.memory[cursor + 1]]);
            if value == 0 {
                return Ok(values);
            }
            values.push(value);
            cursor += 2;
        }
        Err(VmError::MemoryOutOfBounds {
            addr: ptr.wrapping_add((cursor.saturating_sub(start)) as u32),
            size: 2,
        })
    }

    fn c_string_byte_len(&self, ptr: u32) -> VmResult<usize> {
        let start = self.memory_addr(ptr) as usize;
        if start >= self.memory.len() {
            return Err(VmError::MemoryOutOfBounds { addr: ptr, size: 1 });
        }
        Ok(self.memory[start..]
            .iter()
            .position(|byte| *byte == 0)
            .unwrap_or(self.memory.len() - start))
    }

    fn write_c_string(&mut self, ptr: u32, text: &str) -> VmResult<()> {
        self.write_c_string_raw(ptr, text)?;
        let len = self.encode_native_string(text).len().saturating_add(1);
        self.restore_script_records(ptr, len)?;
        Ok(())
    }

    fn copy_c_string_value(&mut self, dst: u32, source: Value) -> VmResult<()> {
        match source {
            Value::Str(text) => self.write_c_string(dst, &text),
            Value::Int(ptr) if ptr != 0 => {
                let src = ptr as u32;
                self.copy_c_string_pointer(dst, src)
            }
            Value::Ptr(ptr) if ptr != 0 => {
                let src = ptr;
                self.copy_c_string_pointer(dst, src)
            }
            Value::Func { offset, .. } if offset != 0 => self.copy_c_string_pointer(dst, offset),
            Value::Int(_) | Value::Ptr(_) | Value::Func { .. } | Value::None => {
                self.write_c_string(dst, "")
            }
            Value::Program(_) => Err(VmError::Runtime(
                "program value cannot be copied as a C string".into(),
            )),
        }
    }

    fn copy_c_string_pointer(&mut self, dst: u32, src: u32) -> VmResult<()> {
        if let Some(Value::Str(text)) = self.mem_values.get(&self.value_key(src)).cloned() {
            return self.write_c_string(dst, &text);
        }
        let size = self.c_string_byte_len(src)?.saturating_add(1);
        self.copy_buffer(dst, src, size)
    }

    /// Write native bytes, keeping the String path (and its script-record
    /// restore) for anything that round-trips through the native encoding.
    fn write_c_string_native_bytes(&mut self, ptr: u32, bytes: &[u8]) -> VmResult<()> {
        let text = if self.native_utf8_enabled() {
            String::from_utf8_lossy(bytes)
        } else {
            encoding_rs::SHIFT_JIS.decode(bytes).0
        };
        if self.encode_native_string(&text).as_ref() == bytes {
            return self.write_c_string(ptr, &text);
        }
        let range = self.resolve_write_range(ptr, bytes.len().saturating_add(1))?;
        let start = range.start;
        self.memory[start..start + bytes.len()].copy_from_slice(bytes);
        self.memory[start + bytes.len()] = 0;
        self.clear_shadow_values(ptr, bytes.len().saturating_add(1));
        self.trace_watch_write(ptr, bytes.len().saturating_add(1), 0, "write_c_string");
        Ok(())
    }

    fn write_c_string_raw(&mut self, ptr: u32, text: &str) -> VmResult<()> {
        let encoded=if self.native_utf8_enabled() {std::borrow::Cow::Borrowed(text.as_bytes())} else {encoding_rs::SHIFT_JIS.encode(text).0};
        let bytes = encoded.as_ref();
        let range = self.resolve_write_range(ptr, bytes.len().saturating_add(1))?;
        let start = range.start;
        self.memory[start..start + bytes.len()].copy_from_slice(bytes);
        self.memory[start + bytes.len()] = 0;
        self.trace_watch_write(ptr, bytes.len().saturating_add(1), 0, "write_c_string");
        Ok(())
    }
    fn encode_native_string<'a>(&self,text:&'a str)->std::borrow::Cow<'a,[u8]> {
        if self.native_utf8_enabled() {std::borrow::Cow::Borrowed(text.as_bytes())} else {encoding_rs::SHIFT_JIS.encode(text).0}
    }
    fn decode_native_string_lossy(&self, bytes: &[u8]) -> String {
        if self.native_utf8_enabled() {
            String::from_utf8_lossy(bytes).into_owned()
        } else {
            encoding_rs::SHIFT_JIS.decode(bytes).0.into_owned()
        }
    }

    /// Write one target Shift-JIS C string without splitting a multibyte
    /// character. The caller supplies the maximum payload length excluding
    /// the terminating NUL, matching the native dialog buffers.
    fn write_c_string_bounded(
        &mut self,
        ptr: u32,
        text: &str,
        max_bytes: usize,
    ) -> VmResult<usize> {
        if ptr == 0 {
            return Err(VmError::Runtime(
                "null writable Shift-JIS string pointer".into(),
            ));
        }
        let mut bounded = String::new();
        for ch in text.chars() {
            let mut candidate = bounded.clone();
            candidate.push(ch);
            if encoding_rs::SHIFT_JIS.encode(&candidate).0.len() > max_bytes {
                break;
            }
            bounded.push(ch);
        }
        let byte_len = encoding_rs::SHIFT_JIS.encode(&bounded).0.len();
        self.write_c_string_raw(ptr, &bounded)?;
        self.clear_shadow_values(ptr, byte_len.saturating_add(1));
        Ok(byte_len)
    }

    fn copy_buffer(&mut self, dst: u32, src: u32, size: usize) -> VmResult<()> {
        let src_range = self.resolve_range(src, size)?;
        let dst_range = self.resolve_write_range(dst, size)?;
        let tmp = self.memory[src_range].to_vec();
        self.memory[dst_range].copy_from_slice(&tmp);
        self.copy_shadow_values(src, dst, size);
        Ok(())
    }

    fn read_counted_string_pointer_list(
        &self,
        pointer: u32,
        count: usize,
    ) -> VmResult<Vec<String>> {
        if count == 0 {
            return Ok(Vec::new());
        }
        if pointer == 0 {
            return Err(VmError::Runtime(
                "null counted native string-pointer list".into(),
            ));
        }
        let mut values = Vec::with_capacity(count);
        for index in 0..count {
            let nested = self.read_int(pointer.wrapping_add(index as u32 * 4), 2)?;
            values.push(if nested == 0 {
                String::new()
            } else {
                self.read_c_string(nested)?
            });
        }
        Ok(values)
    }

    fn read_zero_terminated_string_pointer_list(
        &self,
        pointer: u32,
        maximum: usize,
    ) -> VmResult<Vec<String>> {
        let mut values = Vec::new();
        let mut cursor = pointer;
        for _ in 0..maximum {
            let nested = self.read_int(cursor, 2)?;
            if nested == 0 {
                return Ok(values);
            }
            values.push(self.read_c_string(nested)?);
            cursor = cursor.wrapping_add(4);
        }
        Err(VmError::Runtime(format!(
            "unterminated native string-pointer list at 0x{pointer:08X}"
        )))
    }

    fn load_resource_bytes_with_search<A>(
        &mut self,
        api: &mut A,
        archive: &str,
        file: &str,
    ) -> Option<(Vec<u8>, ResourceLoadOrigin)>
    where
        A: SysApi,
    {
        let mut archives = vec![archive.to_string()];
        if let Some(components) = self.composite_archives.get(archive) {
            archives.extend(components.iter().cloned());
        }
        for candidate_archive in &archives {
            if let Some(bytes) = api.load_file_bytes(candidate_archive, file) {
                let origin = if is_named_archive_argument(candidate_archive) {
                    ResourceLoadOrigin::NamedArchive
                } else {
                    ResourceLoadOrigin::LooseFile
                };
                return Some((bytes, origin));
            }
        }

        // Sys80:3E/3F configure process-global primary and secondary search
        // roots. These roots were previously recorded but never consulted,
        // causing valid installed-game data to be reported as missing.
        for root in [
            self.primary_resource_root.clone(),
            self.secondary_resource_root.clone(),
        ]
        .into_iter()
        .flatten()
        {
            let candidate = join_native_path(&root, file);
            if let Some(bytes) = api.load_file_bytes("", &candidate) {
                return Some((bytes, ResourceLoadOrigin::LooseFile));
            }
        }

        if self.additional_resource_search_enabled {
            let paths = self.additional_resource_paths.clone();
            for path in paths {
                let candidate = join_native_path(&path, file);
                for candidate_archive in &archives {
                    if let Some(bytes) = api.load_file_bytes(candidate_archive, &candidate) {
                        let origin = if is_named_archive_argument(candidate_archive) {
                            ResourceLoadOrigin::NamedArchive
                        } else {
                            ResourceLoadOrigin::LooseFile
                        };
                        return Some((bytes, origin));
                    }
                }
            }
        }
        None
    }

    /// Sys80:25 (sub_488F90 -> sub_467110): pop max count, recursive flag,
    /// pattern, buffer size and destination.
    fn sys80_25_enumerate_files<A>(&mut self, api: &mut A) -> VmResult<Value>
    where
        A: SysApi,
    {
        let max_count = self.pop_int()? as u32 as usize;
        let recursive = self.pop_int()? != 0;
        let pattern = self.pop_string_lossy()?;
        let capacity = self.pop_int()? as u32 as usize;
        let destination = self.pop_ptr()?;
        // sub_467110 builds each subdirectory pattern from strrchr(pattern,
        // '\\'); without a backslash it formats "(null)" and finds nothing.
        let recursive = recursive && pattern.contains('\\');
        let found = api.enumerate_user_files(&pattern, recursive, 0);
        let names = target_file_enumeration_order(&found, recursive, max_count);
        self.write_enumerated_names(destination, capacity, &names)
    }

    /// Sys80:26 (sub_489010 -> sub_467460): pop max count, pattern, buffer
    /// size and destination. Only directories other than `.`/`..`, no recursion.
    fn sys80_26_enumerate_directories<A>(&mut self, api: &mut A) -> VmResult<Value>
    where
        A: SysApi,
    {
        let max_count = self.pop_int()? as u32 as usize;
        let pattern = self.pop_string_lossy()?;
        let capacity = self.pop_int()? as u32 as usize;
        let destination = self.pop_ptr()?;
        let mut names = api
            .enumerate_user_directories(&pattern, 0)
            .into_iter()
            .filter(|name| name != "." && name != "..")
            .collect::<Vec<_>>();
        names.sort_by_key(|name| ntfs_order_key(name));
        // FindNextFileA stops once the count reaches the limit; the check
        // follows each accepted entry.
        if max_count != 0 {
            names.truncate(max_count);
        }
        self.write_enumerated_names(destination, capacity, &names)
    }

    /// Pack names as consecutive C strings. A null destination only measures
    /// and returns the byte total; a name that does not fit fails with -1
    /// after the earlier names were copied, as in sub_467110/sub_467460.
    fn write_enumerated_names(
        &mut self,
        destination: u32,
        capacity: usize,
        names: &[String],
    ) -> VmResult<Value> {
        let mut written = 0usize;
        for name in names {
            let mut bytes = self.encode_native_string(name).into_owned();
            bytes.push(0);
            if destination != 0 {
                if bytes.len() > capacity - written {
                    return Ok(Value::Int(-1));
                }
                let start = destination.wrapping_add(written as u32);
                let range = self.resolve_write_range(start, bytes.len())?;
                self.memory[range].copy_from_slice(&bytes);
                self.clear_shadow_values(start, bytes.len());
            }
            written += bytes.len();
        }
        let result = if destination == 0 { written } else { names.len() };
        Ok(Value::Int(result.min(i32::MAX as usize) as i32))
    }

    fn write_loaded_file<A>(
        &mut self,
        api: &mut A,
        buffer: u32,
        archive: &str,
        file: &str,
        offset: usize,
        length: Option<usize>,
    ) -> VmResult<(i32, i32)>
    where
        A: SysApi,
    {
        let requested = length.unwrap_or_default();
        if requested > 0x0400_0000 {
            tracing::debug!(
                archive,
                file,
                offset,
                length,
                written = 0,
                available = 0,
                status = 3,
                "VM ReadResourceToBuffer request exceeds target limit"
            );
            return Ok((0, 3));
        }
        let Some((bytes, origin)) = self.load_resource_bytes_with_search(api, archive, file) else {
            tracing::debug!(
                archive,
                file,
                offset,
                length,
                written = 0,
                available = 0,
                status = 1,
                "VM ReadResourceToBuffer missing"
            );
            return Ok((0, 1));
        };
        if offset >= bytes.len() && requested != 0 {
            tracing::debug!(
                archive,
                file,
                offset,
                length,
                written = 0,
                available = bytes.len(),
                status = 2,
                "VM ReadResourceToBuffer out of range"
            );
            return Ok((0, 2));
        }
        let available = bytes.len().saturating_sub(offset);
        let requested = length.unwrap_or(available);
        let count = requested.min(available);

        if buffer != 0 && count != 0 {
            let range = self.resolve_write_range(buffer, count)?;
            self.memory[range].copy_from_slice(&bytes[offset..offset + count]);
            self.clear_shadow_values(buffer, count);
            if offset == 0 {
                self.scenario_loaded_file_preprocess(file, buffer, &bytes[..count])?;
            }
        }

        // Target sub_467F50 has two distinct short-read paths. Loose files are
        // read through sub_467CC0, which returns status 3 unless the requested
        // byte count is satisfied exactly. Named archives are read through
        // sub_467EC0/sub_406A70; a non-error return is the actual byte count and
        // sub_467F50 converts it to status 0 even when it is smaller than the
        // caller-provided capacity. data10000.arc demonstrates both explicit
        // `evdb` and default-first-entry calls with a 453-byte container
        // capacity and a 309-byte SDC payload.
        let status = if requested > available && origin != ResourceLoadOrigin::NamedArchive {
            3
        } else {
            0
        };
        tracing::debug!(
            archive,
            file,
            offset,
            length,
            written = count,
            available = bytes.len(),
            status,
            ?origin,
            "VM ReadResourceToBuffer"
        );
        Ok((count.min(i32::MAX as usize) as i32, status))
    }

    #[cfg(test)]
    fn try_builtin_sys(&mut self, group: u8, id: u16) -> VmResult<Option<Value>> {
        self.try_builtin_sys_with_api(&mut TraceApi, group, id)
    }

    /// System80:0x48 — append one DWORD/value to a target thread FIFO.
    ///
    /// Target evidence: `BPThread_EnqueueDword` at 0x004452C0 and the
    /// System80 slot at 0x00504420. `CThread+0x5C/+0x60` are the FIFO
    /// sentinel/head fields.
    ///
    /// BP push order: `[thread_or_program, message]`; native pop order:
    /// `message`, then `thread_or_program`. No immediate BP output.
    fn sys80_48_enqueue_message(&mut self, trace_events: bool) -> VmResult<Value> {
        let message = self.pop_value()?;
        let thread_or_program = self.pop_value()?;
        // sub_488DE0: a thread sub_444B90 cannot find is a script error.
        if !self.post_async_program_message(thread_or_program.clone(), message, trace_events) {
            return Err(VmError::Runtime(format!(
                "無効なスレッドハンドルが指定されました ({})",
                value_summary(&thread_or_program)
            )));
        }
        Ok(Value::None)
    }

    /// System80:0x49 — dequeue one DWORD/value from the current thread FIFO.
    ///
    /// Target evidence: `BPThread_DequeueDword` at 0x00445300 and slot
    /// 0x00504424. The handler writes through `output_ptr` and pushes boolean
    /// success.
    ///
    /// BP push order: `[output_ptr]`; native pop order: `output_ptr`.
    fn sys80_49_dequeue_message(&mut self) -> VmResult<Value> {
        let output_ptr = self.pop_ptr()?;
        if let Some(message) = self.thread.pop_message() {
            self.write_value(output_ptr, 2, &message)?;
            Ok(Value::Int(1))
        } else {
            Ok(Value::Int(0))
        }
    }

    /// System80:0x4A — enqueue an array of DWORD/value messages.
    ///
    /// Target evidence: System80 slot 0x00504428 and the target handler's
    /// repeated calls to `BPThread_EnqueueDword`.
    ///
    /// BP push order: `[thread_or_program, message_count, messages_ptr]`;
    /// native pop order: `messages_ptr`, `message_count`, `thread_or_program`.
    /// No immediate BP output.
    fn sys80_4a_enqueue_message_array(&mut self, trace_events: bool) -> VmResult<Value> {
        // sub_488E60: an unknown thread and a count below 1 are script
        // errors; every value is posted, without an upper bound.
        let messages_ptr = self.pop_ptr()?;
        let message_count = self.pop_int()?;
        let thread_or_program = self.pop_value()?;
        if !self.async_program_is_active_value(&thread_or_program) {
            return Err(VmError::Runtime(format!(
                "無効なスレッドハンドルが指定されました ({})",
                value_summary(&thread_or_program)
            )));
        }
        if message_count < 1 {
            return Err(VmError::Runtime(format!(
                "無効なスレッドメッセージ数 [ {message_count} ] が指定されました"
            )));
        }
        let message_count = message_count as usize;
        if trace_events {
            tracing::debug!(
                program = ?value_summary(&thread_or_program),
                message_count,
                messages_ptr = format_args!("0x{messages_ptr:08X}"),
                "VM program message batch"
            );
        }
        for index in 0..message_count {
            let message = self.read_value(messages_ptr.saturating_add((index * 4) as u32), 2)?;
            self.post_async_program_message(thread_or_program.clone(), message, trace_events);
        }
        Ok(Value::None)
    }

    /// System80:0x4B (sub_489A10): pops the output array, then the count;
    /// a count below 1 is fatal, there is no upper bound. Dequeues
    /// (sub_4456D0) until the count or the queue runs out and pushes how
    /// many were written.
    fn sys80_4b_dequeue_message_array(&mut self) -> VmResult<Value> {
        let output_ptr = self.pop_ptr()?;
        let max_count = self.pop_int()?;
        if max_count < 1 {
            return Err(VmError::Runtime(format!(
                "無効なスレッドメッセージ数 [ {max_count} ] が指定されました"
            )));
        }
        let max_count = max_count as usize;
        let mut received = 0usize;
        while received < max_count {
            let Some(value) = self.thread.pop_message() else {
                break;
            };
            self.write_value(output_ptr.saturating_add(received as u32 * 4), 2, &value)?;
            received += 1;
        }
        Ok(Value::Int(received as i32))
    }

    /// System80:0x4C — invoke the target thread callback with three values.
    ///
    /// Target evidence: `BPThread_InvokeCallback` at 0x00445230 reads the
    /// callback object at `CThread+0x58`. The immediate output reports whether
    /// a callback target was present/invoked.
    ///
    /// BP push order: `[thread_or_program, arg1, arg2, arg3]`; native pop
    /// order: `arg3`, `arg2`, `arg1`, `thread_or_program`.
    fn sys80_4c_invoke_thread_callback(&mut self, trace_events: bool) -> VmResult<Value> {
        let arg3 = self.pop_value()?;
        let arg2 = self.pop_value()?;
        let arg1 = self.pop_value()?;
        let thread_or_program = self.pop_value()?;
        let active = self.post_async_program_callback(
            thread_or_program.clone(),
            [arg1.clone(), arg2.clone(), arg3.clone()],
            trace_events,
        );
        if trace_events {
            tracing::debug!(
                program = ?value_summary(&thread_or_program),
                args = ?[value_summary(&arg1), value_summary(&arg2), value_summary(&arg3)],
                active,
                "VM program callback invocation"
            );
        }
        Ok(Value::Int(i32::from(active)))
    }

    /// sub_46C310's length checks (31, 31, 31, 255, 511 bytes) with the
    /// caller's fatal texts: Sys80:94 (sub_48A730) or Sys80:96 (sub_48A960).
    fn validate_structured_history_record(
        record: &system80_state::StructuredHistoryRecord,
        from_buffer: bool,
    ) -> VmResult<()> {
        let messages: [&str; 5] = if from_buffer {
            [
                "アーカイブ名が{}文字を超えています",
                "ファイル名が{}文字を超えています",
                "名前が{}文字を超えています",
                "メッセージが{}文字を超えています",
                "読み仮名リストが{}文字を超えています",
            ]
        } else {
            [
                "指定されたアーカイブ名は{}文字を超えています",
                "指定されたファイル名は{}文字を超えています",
                "指定された名前は{}文字を超えています",
                "指定されたメッセージは{}文字を超えています",
                "",
            ]
        };
        let fields = [
            (&record.short_text[0], 32),
            (&record.short_text[1], 32),
            (&record.short_text[2], 32),
            (&record.text, 256),
            (&record.extended_text, 512),
        ];
        for ((text, limit), message) in fields.into_iter().zip(messages) {
            if encoding_rs::SHIFT_JIS.encode(text).0.len() >= limit {
                return Err(VmError::Runtime(message.replace("{}", &(limit - 1).to_string())));
            }
        }
        Ok(())
    }

    fn read_structured_history_record(
        &self,
        ptr: u32,
    ) -> VmResult<system80_state::StructuredHistoryRecord> {
        let mut values = [0i32; 9];
        values[0] = self.read_int(ptr, 2)? as i32;
        for index in 1..9 {
            values[index] = self.read_int(ptr + 64 + (index as u32 - 1) * 4, 2)? as i32;
        }
        let record = system80_state::StructuredHistoryRecord {
            values,
            short_text: [
                self.read_c_string(ptr + 160)?,
                self.read_c_string(ptr + 192)?,
                self.read_c_string(ptr + 224)?,
            ],
            text: self.read_c_string(ptr + 256)?,
            extended_text: self.read_c_string(ptr + 512)?,
        };
        Self::validate_structured_history_record(&record, true)?;
        Ok(record)
    }

    fn write_structured_history_record(
        &mut self,
        ptr: u32,
        record: &system80_state::StructuredHistoryRecord,
        include_extended: bool,
    ) -> VmResult<()> {
        let range = self.resolve_write_range(ptr, 512)?;
        self.memory[range].fill(0);
        self.clear_shadow_values(ptr, 512);
        self.write_int(ptr, 2, record.values[0] as u32)?;
        for index in 1..9 {
            self.write_int(
                ptr + 64 + (index as u32 - 1) * 4,
                2,
                record.values[index] as u32,
            )?;
        }
        self.write_c_string(ptr + 160, &record.short_text[0])?;
        self.write_c_string(ptr + 192, &record.short_text[1])?;
        self.write_c_string(ptr + 224, &record.short_text[2])?;
        self.write_c_string(ptr + 256, &record.text)?;
        if include_extended && !record.extended_text.is_empty() {
            self.write_c_string(ptr + 512, &record.extended_text)?;
        }
        Ok(())
    }

    fn sys80_90_reset_structured_history(&mut self) -> VmResult<Value> {
        let capacity = self.pop_int()?;
        self.system80_shared
            .lock()
            .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
            .history
            .reset(capacity);
        Ok(Value::None)
    }

    fn sys80_91_structured_history_count(&mut self) -> VmResult<Value> {
        let count = self
            .system80_shared
            .lock()
            .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
            .history
            .records
            .len();
        Ok(Value::Int(count.min(i32::MAX as usize) as i32))
    }

    fn sys80_94_append_structured_history_fields(&mut self) -> VmResult<Value> {
        let text = self.pop_string_lossy()?;
        let short3 = self.pop_string_lossy()?;
        let short2 = self.pop_string_lossy()?;
        let short1 = self.pop_string_lossy()?;
        let mut popped = [0i32; 9];
        for value in &mut popped {
            *value = self.pop_int()?;
        }
        popped.reverse();
        let record = system80_state::StructuredHistoryRecord {
            values: popped,
            short_text: [short1, short2, short3],
            text,
            extended_text: String::new(),
        };
        Self::validate_structured_history_record(&record, false)?;
        self.system80_shared
            .lock()
            .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
            .history
            .push(record);
        Ok(Value::None)
    }

    fn sys80_95_97_read_structured_history(&mut self, include_extended: bool) -> VmResult<Value> {
        let index = self.pop_int()?;
        let dst = self.pop_ptr()?;
        let record = u32::try_from(index).ok().and_then(|index| {
            self.system80_shared
                .lock()
                .ok()
                .and_then(|shared| shared.history.newest(index))
        });
        let Some(record) = record else {
            return Err(VmError::Runtime(format!(
                "無効なヒストリインデックス [ {index} ] が指定されました"
            )));
        };
        self.write_structured_history_record(dst, &record, include_extended)?;
        Ok(Value::Int(1))
    }

    fn sys80_96_append_structured_history_record(&mut self) -> VmResult<Value> {
        let src = self.pop_ptr()?;
        let record = self.read_structured_history_record(src)?;
        self.system80_shared
            .lock()
            .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
            .history
            .push(record);
        Ok(Value::None)
    }

    /// System80:0x84 — intern a resource name.
    ///
    /// Target evidence: `BP_Sys80_84_InternResourceName` at 0x004899B0
    /// calls the native resource-name table intern helper and then pushes
    /// the constant success value one. The stable table index remains native-
    /// internal and is not exposed to BP code.
    ///
    /// BP push order: `[name]`; native pop order: `name`.
    /// A string operand that may be a null pointer.
    fn pop_nullable_string(&mut self) -> VmResult<Option<String>> {
        match self.pop_value()? {
            Value::Int(0) | Value::Ptr(0) | Value::None => Ok(None),
            value => self.value_as_native_string_lossy(value).map(Some),
        }
    }

    /// sub_472A10: the program is "dir\\cmd", or primary root + cmd without
    /// a directory (which is then also the current directory). On failure:
    /// no message -> 0; without retry MessageBox(OK) -> 0; with retry
    /// OK|CANCEL and the quit question, where only a confirmed quit -> 0.
    #[allow(clippy::too_many_arguments)]
    fn launch_native_process<A: SysApi>(
        &mut self,
        api: &mut A,
        directory: Option<String>,
        command: &str,
        error_message: Option<String>,
        hide_window: bool,
        retry: bool,
        wait_for_uninstaller: bool,
    ) -> i32 {
        let (command_line, current_directory) = match directory {
            Some(directory) => (format!("{directory}\\{command}"), directory),
            None => {
                let root = self.resource_primary_root(api);
                (format!("{root}{command}"), root)
            }
        };
        loop {
            if api.launch_process(&current_directory, &command_line, hide_window, wait_for_uninstaller) {
                return 1;
            }
            let Some(message) = error_message.as_deref() else {
                return 0;
            };
            if !retry {
                api.message_box("", message);
                return 0;
            }
            if !api.retry_or_quit_prompt("", message) {
                return 0;
            }
        }
    }

    /// Namespace 0x80000000 of the string tables (dword_503E00): the
    /// resource names of Sys80:84/85, saved in BGI.gdb, and spared by 80:D8.
    pub(crate) fn resource_names(&self) -> &[String] {
        self.string_hash_tables.get(&RESOURCE_NAMESPACE).map_or(&[], Vec::as_slice)
    }

    /// sub_496070(0x80000000, ..): a zero count removes the namespace.
    pub(crate) fn set_resource_names(&mut self, names: Vec<String>) {
        if names.is_empty() {
            self.string_hash_tables.remove(&RESOURCE_NAMESPACE);
        } else {
            self.string_hash_tables.insert(RESOURCE_NAMESPACE, names);
        }
    }

    fn sys80_84_intern_resource_name(&mut self) -> VmResult<Value> {
        let name = self.pop_string_lossy()?;
        // sub_496200 on namespace 0x80000000 (created on first use).
        let names = self.string_hash_tables.entry(RESOURCE_NAMESPACE).or_default();
        if !names.iter().any(|entry| entry == &name) {
            names.push(name);
        }
        // sub_46B430 always returns one after interning; the stable index is
        // internal to the native table and is not exposed to BP code.
        Ok(Value::Int(1))
    }

    /// System80:0x85 — test whether a resource name is already interned.
    ///
    /// Target evidence: `BP_Sys80_85_ResourceNameExists` at 0x004899E0
    /// performs lookup only and pushes a script boolean. It must not insert a
    /// missing name.
    ///
    /// BP push order: `[name]`; native pop order: `name`.
    fn sys80_85_resource_name_exists(&mut self) -> VmResult<Value> {
        let name = self.pop_string_lossy()?;
        Ok(Value::Int(i32::from(
            self.resource_names().iter().any(|entry| entry == &name),
        )))
    }

    /// System80:0x88 — create or resize a named read-flag bitset.
    ///
    /// Target evidence: `BP_Sys80_88_CreateReadFlagTable` at 0x00489A10
    /// forwards to `ReadFlagTable_CreateOrResize`, which preserves the
    /// overlapping prefix and clears newly allocated bytes. This handler is
    /// unrelated to BCS/scenario preprocessing; the old coupling was a port
    /// guess and has been removed.
    ///
    /// BP push order: `[name, bit_count]`; native pop order: `bit_count`,
    /// then `name`. The exact distinction among non-success status values is
    /// not yet proven, so the existing boolean success contract is retained.
    fn sys80_88_create_or_resize_read_flag_table(&mut self) -> VmResult<Value> {
        let bit_count = self.pop_int()?;
        let name = self.pop_string_lossy()?;
        // sub_446FD0 fails only for a zero byte count ((bits + 7) >> 3,
        // unsigned); an empty name is a valid table name.
        let Ok(bit_count) = usize::try_from(bit_count) else {
            return Ok(Value::Int(0));
        };
        if bit_count == 0 {
            return Ok(Value::Int(0));
        }
        let success = if let Some(flags) = self.read_flags.get_mut(&name) {
            flags.resize(bit_count)
        } else {
            self.read_flags.insert(name, ReadFlagBits::new(bit_count));
            true
        };
        Ok(Value::Int(i32::from(success)))
    }

    /// System80:0x89 — set or clear one read flag.
    ///
    /// Target evidence: `BP_Sys80_89_SetReadFlagBit` at 0x00489A40.
    /// BP push order: `[name, bit_offset, enabled]`; native pop order:
    /// `enabled`, `bit_offset`, `name`.
    fn sys80_89_set_read_flag_bit(&mut self) -> VmResult<Value> {
        let enabled = self.pop_int()? != 0;
        let bit_offset = self.pop_int()?;
        let name = self.pop_string_lossy()?;
        let status = match (self.read_flags.get_mut(&name), u32::try_from(bit_offset)) {
            (None, _) => 1,
            (Some(_), Err(_)) => 2,
            (Some(flags), Ok(bit_offset)) => {
                if flags.set(bit_offset, enabled) {
                    0
                } else {
                    2
                }
            }
        };
        Ok(Value::Int(status))
    }

    /// System80:0x8A — set or clear a contiguous read-flag range.
    ///
    /// Target evidence: `BP_Sys80_8A_SetReadFlagRange` at 0x00489AC0 and
    /// `ReadFlagTable_SetRange` at 0x00446D40.
    /// BP push order: `[name, start_bit, enabled, bit_count]`; native pop
    /// order: `bit_count`, `enabled`, `start_bit`, `name`.
    fn sys80_8a_set_read_flag_range(&mut self) -> VmResult<Value> {
        let bit_count = self.pop_int()?;
        let enabled = self.pop_int()? != 0;
        let start_bit = self.pop_int()?;
        let name = self.pop_string_lossy()?;
        // sub_447190, all unsigned: unknown table 1; start >= length 2;
        // count - 1 > 0xFFFF or start + count > length 3.
        let (start_bit, bit_count) = (start_bit as u32, bit_count as u32);
        let status = match self.read_flags.get_mut(&name) {
            None => 1,
            Some(flags) if start_bit >= flags.bit_len => 2,
            Some(_) if bit_count.wrapping_sub(1) > 0xFFFF => 3,
            Some(flags) if start_bit + bit_count > flags.bit_len => 3,
            Some(flags) => {
                flags.set_range(start_bit, bit_count, enabled);
                0
            }
        };
        Ok(Value::Int(status))
    }

    /// System80:0x8B — query one read flag through an output pointer.
    ///
    /// Target evidence: `BP_Sys80_8B_QueryReadFlagBit` at 0x00489B80.
    /// BP push order: `[name, output_ptr, bit_offset]`; native pop order:
    /// `bit_offset`, `output_ptr`, `name`. The immediate result is a status;
    /// the queried boolean is written to `output_ptr`.
    ///
    /// Some system scripts push the first two the other way round:
    /// Wonderful Everyday's `scrmain._bp` pushes `[output_ptr, name,
    /// bit_offset]` (the flag variable, then the current script's name in
    /// its script table). Taking that name as the output pointer overwrote
    /// it on every scenario instruction, so saves held empty script names.
    /// The name is therefore whichever argument names a read-flag table;
    /// when neither does, the order above applies.
    fn sys80_8b_query_read_flag_bit(&mut self) -> VmResult<Value> {
        // sub_48A670 pops the bit index, the table name, then the output
        // DWORD; sub_447230 writes the output only on success (status 0;
        // unknown table 1, index >= length 2, unsigned).
        let bit_offset = self.pop_int()? as u32;
        let name = self.pop_string_lossy()?;
        let output = self.pop_value()?;
        let output_ptr = output.as_i32() as u32;
        let (status, enabled) = match self.read_flags.get(&name) {
            None => (1, None),
            Some(flags) => match flags.contains(bit_offset) {
                Some(enabled) => (0, Some(enabled)),
                None => (2, None),
            },
        };
        if let Some(enabled) = enabled {
            self.write_int(output_ptr, 2, u32::from(enabled))?;
        }
        Ok(Value::Int(status))
    }

    fn try_builtin_sys_with_api<A: SysApi>(
        &mut self,
        api: &mut A,
        group: u8,
        id: u16,
    ) -> VmResult<Option<Value>> {
        let result = match (group, id) {
            (0x81, 0x00) if self.native_abi_profile == NativeAbiProfile::Amachoco => {
                // sub_494EF0 -> sub_49EA60: 0 selects CP932, 1 selects UTF-8.
                let mode = self.pop_int()?;
                let valid = (0..=1).contains(&mode);
                if valid {
                    self.system81_shared.lock().expect("system81 state poisoned")
                        .native_utf8_override = Some(mode == 1);
                }
                Value::Int(i32::from(valid))
            }
            (0x80, 0x53) => {
                // sub_489070 -> sub_48D190 arms the one-shot global gate.
                // Sys81:30 and Graph90:F6 consume and clear it when choosing
                // their DCProcReadBinary / DCProcDecodeBMV paths.
                self.next_binary_or_bmv_async = true;
                Value::None
            }
            (0x81, 0x04) => {
                let threshold = self.pop_int()?;
                let accepted = (50..=60_000).contains(&threshold);
                if accepted {
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .clock_jump_threshold_ms = threshold;
                }
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x07) => {
                // sub_48F090: index < 5 (unsigned) copies the press slot.
                let index = self.pop_int()?;
                let destination = self.pop_ptr()?;
                if (index as u32) < 5 {
                    let (x, y) = api.pointer_position(index).unwrap_or((-1, -1));
                    self.write_int(destination, 2, x as u32)?;
                    self.write_int(destination.wrapping_add(4), 2, y as u32)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x81, 0x08) => {
                // GetUserNameA with a 257-byte buffer: a longer name fails
                // and leaves the buffer as it was.
                let destination = self.pop_ptr()?;
                let name = api.host_user_name();
                if self.encode_native_string(&name).len() < 257 {
                    self.write_c_string(destination, &name)?;
                }
                Value::None
            }
            (0x81, 0x09) => {
                // GetComputerNameA with nSize 16 (the NetBIOS name, at most
                // 15 characters; a longer one fails, nothing written).
                let destination = self.pop_ptr()?;
                let name = api.host_computer_name();
                if self.encode_native_string(&name).len() < 16 {
                    self.write_c_string(destination, &name)?;
                }
                Value::None
            }
            (0x81, 0x0a) => {
                // sub_46FC80: CPUID 0x80000002..4 through sub_496650; 0 and
                // nothing written when leaf 0x80000004 is not supported.
                let destination = self.pop_ptr()?;
                match target_cpu_brand_string() {
                    Some(brand) => {
                        self.write_c_string(destination, &bgi_normalize_spaces(&brand))?;
                        Value::Int(1)
                    }
                    None => Value::Int(0),
                }
            }
            (0x81, 0x0b) => {
                // sub_45E9F0: the D3DADAPTER_IDENTIFIER9 that sub_4605A0
                // read at start-up (zeroed when GetAdapterIdentifier
                // failed): Description through sub_496650, then the
                // DriverVersion words product, version, subversion, build.
                let _ignored = self.pop_ptr()?;
                let version_destination = self.pop_ptr()?;
                let description_destination = self.pop_ptr()?;
                let (description, version) = api.display_adapter_identifier();
                self.write_c_string(description_destination, &bgi_normalize_spaces(&description))?;
                for (index, value) in version.into_iter().enumerate() {
                    self.write_int(
                        version_destination.wrapping_add(index as u32 * 4),
                        2,
                        u32::from(value),
                    )?;
                }
                Value::None
            }
            (0x81, 0x0c) => {
                // sub_46FD60: GetVersionExA once (OSVERSIONINFOA zeroed
                // first, so a host without Windows reports 0.0.0 platform 0
                // and an empty service pack).
                let service_pack = self.pop_ptr()?;
                let version = self.pop_ptr()?;
                let (values, csd) = api.host_os_version();
                for (index, value) in values.into_iter().enumerate() {
                    self.write_int(version.wrapping_add(index as u32 * 4), 2, value)?;
                }
                self.write_c_string(service_pack, &csd)?;
                Value::None
            }
            (0x81, 0x0d) => {
                // GlobalMemoryStatusEx: ullTotalPhys / ullAvailPhys >> 20.
                let available = self.pop_ptr()?;
                let total = self.pop_ptr()?;
                let (total_mb, available_mb) = api.host_physical_memory_mb();
                self.write_int(total, 2, total_mb)?;
                self.write_int(available, 2, available_mb)?;
                Value::None
            }
            (0x81, 0x0e) => {
                // sub_45EAE0(.., 1) over the start-up GetAdapterDisplayMode.
                let destination = self.pop_ptr()?;
                let (width, height) = adjusted_desktop_dimensions(api.desktop_display_size());
                self.write_int(destination, 2, width)?;
                self.write_int(destination.wrapping_add(4), 2, height)?;
                Value::None
            }
            (0x81, 0x0f) => Value::Int(i32::from(api.window_minimized())),
            (0x81, 0x10) => {
                let replacement = self.pop_int()?;
                let index = self.pop_int()?;
                // sub_497B40 rejects a descriptor >= 0x100 with a fatal error.
                if !(0..256).contains(&index) {
                    return Err(VmError::Runtime(
                        "不正な仮想キーコードが指定されています".into(),
                    ));
                } else {
                    api.swap_input_repeat_flag(index, replacement);
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    let old = std::mem::replace(
                        &mut state.input_binding_values[index as usize],
                        replacement,
                    );
                    Value::Int(old)
                }
            }
            (0x81, 0x11) => {
                let destination = self.pop_ptr()?;
                let state = api.keyboard_state();
                let range = self.resolve_range(destination, state.len())?;
                self.memory[range].copy_from_slice(&state);
                self.clear_shadow_values(destination, state.len());
                Value::None
            }
            (0x81, 0x14) => {
                let enabled = self.pop_int()?;
                api.set_background_key_polling(enabled);
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .keyboard_polling_override = enabled;
                Value::None
            }
            (0x81, 0x16) => {
                // sub_46B720: capacity <= 0x200 (unsigned) empties the touch
                // history and sets its capacity and minimum step.
                let distance = self.pop_int()?;
                let capacity = self.pop_int()?;
                let accepted = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .configure_touch_history(capacity as u32, distance as u32);
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x17) => {
                // sub_46B860 walks the WM_TOUCH history (dword_5666BC). The
                // host delivers no WM_TOUCH, so the history stays empty and
                // `start < count` never holds.
                let _count = self.pop_int()?;
                let _start = self.pop_int()?;
                let _angles = self.pop_ptr()?;
                let _points = self.pop_ptr()?;
                Value::Int(0)
            }
            (0x81, 0x18) => {
                // sub_46B660: RegisterTouchWindow / UnregisterTouchWindow
                // when user32 has them (dword_566690), else 0.
                let enabled = self.pop_int()? != 0;
                Value::Int(i32::from(api.register_touch_input(enabled)))
            }
            (0x81, 0x19) => {
                // sub_46B6A0 counts (and copies, 24 bytes each) the active
                // contacts of dword_5666B0; none without WM_TOUCH.
                let _destination = self.pop_ptr()?;
                Value::Int(0)
            }
            (0x81, 0x1b) => {
                let value = self.pop_int()?;
                let index = self.pop_int()?;
                let accepted = if (0..36).contains(&index) {
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .controller_wake_entries[index as usize] = value;
                    true
                } else {
                    false
                };
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x1d) => {
                // sub_4613C0 sums the DirectInput joysticks of dword_565F18
                // (all, or the one whose id matches). The host enumerates no
                // DirectInput device, so the result is 0x80000001 and the
                // handler pushes 0 without writing.
                let _device = self.pop_int()?;
                let _destination = self.pop_ptr()?;
                Value::Int(0)
            }
            (0x81, 0x1e) => {
                let button = self.pop_int()?;
                Value::Int(i32::from(api.inject_mouse_click(button)))
            }
            (0x81, 0x1f) => {
                let mask = self.pop_int()?;
                api.set_message_auxiliary_input_mask(mask);
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .message_auxiliary_input_mask = mask;
                Value::None
            }
            (0x81, 0x21) => {
                if self.native_abi_profile!=NativeAbiProfile::Amachoco {return Ok(None);}
                // amachoco.exe's sub_4953A0 pops source then destination and
                // converts the source from CP932 to UTF-8. Retain the decoded
                // string as a shadow value so later BP copies and graph calls
                // do not lose it by treating the pointer as an integer.
                let source = self.pop_value()?;
                let destination = self.pop_ptr()?;
                let text = self.value_as_native_string_lossy(source)?;
                let bytes=text.as_bytes();
                let range=self.resolve_write_range(destination,bytes.len()+1)?;
                self.memory[range.start..range.start+bytes.len()].copy_from_slice(bytes);
                self.memory[range.start+bytes.len()]=0;
                self.clear_shadow_values(destination,bytes.len()+1);
                self.mem_values
                    .insert(self.value_key(destination), Value::Str(text));
                Value::None
            }
            (0x81, 0x28) => {
                let mode = self.pop_int()?;
                let path = self.pop_string_lossy()?;
                let handle_destination = self.pop_ptr()?;
                let bytes = api.load_file_bytes("", &path).or_else(|| {
                    api.allow_direct_filesystem_fallback()
                        .then(|| std::fs::read(&path).ok())
                        .flatten()
                });
                let status = if !(0..=2).contains(&mode) {
                    1
                } else if let Some(bytes) = bytes {
                    let result = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .open_resource_stream(path, mode, bytes);
                    match result {
                        Ok(handle) => {
                            self.write_int(handle_destination, 2, handle)?;
                            0
                        }
                        Err(system81_state::NATIVE_NOT_FOUND) => 2,
                        Err(_) => 3,
                    }
                } else {
                    3
                };
                Value::Int(status)
            }
            (0x81, 0x29) => {
                let handle = self.pop_int()?;
                let status_destination = self.pop_ptr()?;
                let removed = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .resource_streams
                    .remove(&(handle as u32))
                    .is_some();
                if status_destination != 0 {
                    self.write_int(status_destination, 2, if removed { 0 } else { 4 })?;
                }
                Value::Int(if removed { 0 } else { 4 })
            }
            (0x81, 0x2a) => {
                let length = self.pop_int()?.max(0) as usize;
                let destination = self.pop_ptr()?;
                let handle = self.pop_int()? as u32;
                let status_destination = self.pop_ptr()?;
                let result = {
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    if let Some(stream) = state.resource_streams.get_mut(&handle) {
                        let available = stream.bytes.len().saturating_sub(stream.cursor);
                        let count = length.min(available);
                        let bytes = stream.bytes[stream.cursor..stream.cursor + count].to_vec();
                        stream.cursor += count;
                        Some(bytes)
                    } else {
                        None
                    }
                };
                if let Some(bytes) = result {
                    let range = self.resolve_range(destination, bytes.len())?;
                    self.memory[range].copy_from_slice(&bytes);
                    self.clear_shadow_values(destination, bytes.len());
                    if status_destination != 0 {
                        self.write_int(status_destination, 2, bytes.len() as u32)?;
                    }
                    Value::Int(0)
                } else {
                    Value::Int(4)
                }
            }
            (0x81, 0x2b) => {
                let offset = self.pop_int()?.max(0) as usize;
                let handle = self.pop_int()? as u32;
                let status_destination = self.pop_ptr()?;
                let status = {
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    if let Some(stream) = state.resource_streams.get_mut(&handle) {
                        stream.cursor = offset.min(stream.bytes.len());
                        stream.cursor as i32
                    } else {
                        -1
                    }
                };
                if status_destination != 0 {
                    self.write_int(status_destination, 2, status as u32)?;
                }
                Value::Int(if status >= 0 { 0 } else { 4 })
            }
            (0x81, 0x2c) => {
                let path = self.pop_string_lossy()?;
                let write_time = self.pop_ptr()?;
                let access_time = self.pop_ptr()?;
                let creation_time = self.pop_ptr()?;
                let exists = (api.allow_direct_filesystem_fallback()
                    && std::fs::metadata(&path).is_ok())
                    || api.file_exists("", &path);
                for destination in [creation_time, access_time, write_time] {
                    if destination != 0 {
                        for index in 0..8 {
                            self.write_int(destination.wrapping_add(index * 2), 1, 0)?;
                        }
                    }
                }
                Value::Int(i32::from(exists))
            }
            (0x81, 0x2d) => {
                let _write_time = self.pop_ptr()?;
                let _access_time = self.pop_ptr()?;
                let _creation_time = self.pop_ptr()?;
                let path = self.pop_string_lossy()?;
                Value::Int(i32::from(if api.allow_direct_filesystem_fallback() {
                    std::fs::metadata(path).is_ok()
                } else {
                    api.file_exists("", &path)
                }))
            }
            (0x81, 0x2f) => {
                let path = self.pop_string_lossy()?;
                let directory = std::path::Path::new(&path);
                let test_path = directory.join(format!("BGI{:08x}.tmp", self.trace_id));
                let direct = api.allow_direct_filesystem_fallback();
                let writable = if direct {
                    std::fs::write(&test_path, []).is_ok()
                } else {
                    api.probe_directory_writable(&path)
                };
                if writable && direct {
                    let _ = std::fs::remove_file(test_path);
                }
                Value::Int(i32::from(writable))
            }
            (0x81, 0x30) => {
                // Target native pop order from sub_48BAB0 is:
                // length, offset, file, archive/root, destination.
                // The previous port popped destination before the two strings,
                // so a call shaped as [destination, archive, file, offset, length]
                // wrote the archive bytes at address zero and left the real
                // destination buffer untouched.
                let length = self.pop_int()?.max(0) as usize;
                let offset = self.pop_int()?.max(0) as usize;
                let file = self.pop_string_lossy()?;
                let archive = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let (written, status) = self.write_loaded_file(
                    api,
                    destination,
                    &archive,
                    &file,
                    offset,
                    Some(length),
                )?;
                let header = if written >= 32 {
                    self.resolve_range(destination, 32)
                        .ok()
                        .map(|range| self.memory[range].to_vec())
                } else {
                    None
                };
                tracing::info!(
                    archive,
                    file,
                    destination = format_args!("0x{destination:08X}"),
                    offset,
                    requested = length,
                    written,
                    status,
                    header = header.as_deref().map(|bytes| bytes
                        .iter()
                        .map(|byte| format!("{byte:02X}"))
                        .collect::<String>()),
                    "Sys81_30_ReadResourceBinary completed"
                );
                if std::mem::take(&mut self.next_binary_or_bmv_async) {
                    self.install_host_completed_procedure(
                        NativeOpcode { group, id },
                        native_call::NativeProcedureCompletion {
                            class: native_call::NativeProcedureClass::ReadBinary,
                            status,
                            outputs: [status, 0],
                            output_count: 1,
                        },
                        false,
                    );
                } else {
                    self.push_value(Value::Int(status));
                }
                Value::None
            }
            (0x81, 0x31) => {
                let requested = self.pop_int()?;
                let offset = self.pop_int()?;
                let url = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let _ = (requested, offset, url);
                if destination != 0 {
                    self.write_int(destination, 2, 0)?;
                }
                Value::Int(-1)
            }
            (0x81, 0x32) => {
                let length_destination = self.pop_ptr()?;
                let path = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let requested = self.pop_int()?.max(0) as usize;
                let bytes = if api.allow_direct_filesystem_fallback() {
                    std::fs::read(&path)
                } else {
                    Err(std::io::Error::from(std::io::ErrorKind::PermissionDenied))
                }
                .or_else(|_| {
                    api.load_file_bytes("", &path)
                        .ok_or(std::io::Error::from(std::io::ErrorKind::NotFound))
                });
                match bytes {
                    Ok(bytes) => {
                        let count = requested.min(bytes.len());
                        let range = self.resolve_range(destination, count)?;
                        self.memory[range].copy_from_slice(&bytes[..count]);
                        self.clear_shadow_values(destination, count);
                        self.write_int(length_destination, 2, count as u32)?;
                        Value::Int(0)
                    }
                    Err(_) => Value::Int(1),
                }
            }
            (0x81, 0x35) => {
                let path = self.pop_string_lossy()?;
                let _context = self.pop_ptr()?;
                let size = api
                    .load_file_bytes("", &path)
                    .or_else(|| {
                        api.allow_direct_filesystem_fallback()
                            .then(|| std::fs::read(&path).ok())
                            .flatten()
                    })
                    .map_or(0, |bytes| bytes.len().min(i32::MAX as usize) as i32);
                Value::Int(size)
            }
            (0x81, 0x36) => {
                let destination = self.pop_ptr()?;
                let mut count = 0;
                for index in 0..26u32 {
                    let drive_type = 0u32;
                    self.write_int(destination.wrapping_add(index * 4), 2, drive_type)?;
                    count += usize::from(drive_type != 0);
                }
                Value::Int(count as i32)
            }
            (0x81, 0x37) => {
                let path = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let exists = if api.allow_direct_filesystem_fallback() {
                    std::fs::metadata(path).is_ok()
                } else {
                    api.file_exists("", &path)
                };
                self.write_int(destination, 2, 0)?;
                Value::Int(i32::from(exists))
            }
            (0x81, 0x38) => {
                let mode = self.pop_int()?;
                let title = self.pop_string_lossy()?;
                let default_name = self.pop_string_lossy()?;
                let extension_table = self.pop_ptr()?;
                let label_table = self.pop_ptr()?;
                let filter_count = self.pop_int()?;
                let destination = self.pop_ptr()?;
                if filter_count <= 0 {
                    Value::Int(7)
                } else {
                    let count = filter_count as usize;
                    let mut labels = Vec::with_capacity(count);
                    let mut extensions = Vec::with_capacity(count);
                    for index in 0..count {
                        let pointer =
                            self.read_int(label_table.wrapping_add(index as u32 * 4), 2)?;
                        labels.push(self.read_c_string(pointer)?);
                    }
                    for index in 0..count {
                        let pointer =
                            self.read_int(extension_table.wrapping_add(index as u32 * 4), 2)?;
                        extensions.push(self.read_c_string(pointer)?);
                    }
                    let filters = labels.into_iter().zip(extensions).collect::<Vec<_>>();
                    match api.open_resource_file_dialog(mode, &title, &default_name, &filters) {
                        Ok(Some(path)) => {
                            self.write_c_string(destination, &path)?;
                            Value::Int(0)
                        }
                        Ok(None) => Value::Int(-1),
                        Err(status) => Value::Int(status),
                    }
                }
            }
            (0x81, 0x39) => {
                let destination = self.pop_ptr()?;
                let required_destination = self.pop_ptr()?;
                let pattern = self.pop_string_lossy()?;
                let entries = api.enumerate_user_files(&pattern, true, usize::MAX);
                let mut packed = Vec::new();
                for entry in &entries {
                    let (bytes, _, _) = encoding_rs::SHIFT_JIS.encode(entry);
                    packed.extend_from_slice(&bytes);
                    packed.push(0);
                }
                if required_destination != 0 {
                    self.write_int(required_destination, 2, packed.len() as u32)?;
                }
                if destination != 0 && !packed.is_empty() {
                    let range = self.resolve_range(destination, packed.len())?;
                    self.memory[range].copy_from_slice(&packed);
                    self.clear_shadow_values(destination, packed.len());
                }
                Value::Int(entries.len().min(i32::MAX as usize) as i32)
            }
            (0x81, 0x3a) => {
                let root = self.pop_int()?;
                let title = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                if let Some(path) = api.browse_folder(&title, root) {
                    self.write_c_string(destination, &path)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x81, 0x3b) => {
                let _reserved = self.pop_ptr()?;
                let title = self.pop_string_lossy()?;
                let pattern = self.pop_string_lossy()?;
                let _context = self.pop_ptr()?;
                Value::Int(if api.show_resource_list_dialog(&title, &pattern) {
                    0
                } else {
                    -1
                })
            }
            (0x81, 0x3c) => {
                let path = self.pop_string_lossy()?;
                Value::Int(i32::from(
                    api.file_exists("", &path)
                        || api.directory_exists(&path)
                        || (api.allow_direct_filesystem_fallback()
                            && std::fs::metadata(path).is_ok()),
                ))
            }
            (0x81, 0x3d) => {
                let path = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let label = std::path::Path::new(&path)
                    .components()
                    .next()
                    .map(|component| component.as_os_str().to_string_lossy().into_owned())
                    .unwrap_or_default();
                self.write_c_string(destination, &label)?;
                Value::Int(i32::from(!label.is_empty()))
            }
            (0x81, 0x3e) => {
                let path = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let available = if api.allow_direct_filesystem_fallback() {
                    std::fs::metadata(path).is_ok()
                } else {
                    api.file_exists("", &path)
                };
                if destination != 0 {
                    self.write_int(destination, 2, u32::from(available))?;
                }
                Value::Int(i32::from(available))
            }
            (0x81, 0x44) => {
                // sub_48CA50 pops the initial IP, the data (stack) bytes,
                // the code bytes and the operand slots (`child_thread.rs`).
                let initial_ip = self.pop_int()? as u32;
                let data_bytes = self.pop_int()? as u32;
                let code_bytes = self.pop_int()? as u32;
                let operand_slots = self.pop_int()? as u32;
                Value::Int(self.start_child_thread(operand_slots, code_bytes, data_bytes, initial_ip, false)?)
            }
            (0x81, 0x60) => {
                // sub_4615A0: index >= 8 (unsigned) -> 1, a zero dimension
                // -> 2, else dword_506B8C/506BAC[index] = width/height.
                let height = self.pop_int()?;
                let width = self.pop_int()?;
                let index = self.pop_int()?;
                let status = if index as u32 >= 8 {
                    1
                } else if width == 0 || height == 0 {
                    2
                } else {
                    api.register_display_mode(index as usize, width, height);
                    0
                };
                Value::Int(status)
            }
            // sub_45EDE0: the effective full-screen scaling mode.
            (0x81, 0x61) => Value::Int(api.fullscreen_scale_mode()),
            (0x81, 0x62) => {
                // sub_45ED90: dword_50721C = mode for mode <= 1 (unsigned).
                let mode = self.pop_int()?;
                Value::Int(i32::from(mode as u32 <= 1 && api.set_window_monitor_adapter_mode(mode)))
            }
            (0x81, 0x63) => {
                // sub_45EDC0: dword_565EAC = mode for mode <= 2 (unsigned).
                let mode = self.pop_int()?;
                Value::Int(i32::from(mode as u32 <= 2 && api.set_config_input_mode(mode)))
            }
            (0x81, 0x64) => {
                let height = self.pop_int()?;
                let width = self.pop_int()?;
                api.configure_screen_size(width, height);
                Value::None
            }
            (0x81, 0x65) => {
                let value = self.pop_int()?;
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .window_position_override = value;
                api.set_window_position_override(value);
                Value::None
            }
            (0x81, 0x68) => {
                let value = self.pop_int()?;
                let old = {
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    std::mem::replace(&mut state.pause_on_deactivate, value)
                };
                api.set_pause_on_deactivate(value != 0);
                Value::Int(old)
            }
            (0x81, 0x69) => {
                let enabled = self.pop_int()? != 0;
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .print_screen_hotkeys_enabled = enabled;
                api.set_print_screen_hotkeys_enabled(enabled);
                Value::None
            }
            (0x81, 0x6a) => {
                let mode = self.pop_int()?;
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .error_capture_mode = mode;
                Value::None
            }
            (0x81, 0x6b) => {
                // sub_4649E0 returns 0 without touching the destination while
                // no error has been captured; errdtctn._bp polls this and
                // starts its error report on any non-zero result.
                let destination = self.pop_ptr()?;
                let message = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .captured_error
                    .clone();
                match message {
                    None => Value::Int(0),
                    Some(message) => {
                        if destination != 0 {
                            self.write_c_string(destination, &message)?;
                        }
                        let (bytes, _, _) = encoding_rs::SHIFT_JIS.encode(&message);
                        Value::Int(bytes.len().saturating_add(1).min(i32::MAX as usize) as i32)
                    }
                }
            }
            // LOWORD(D3DCAPS9::PixelShaderVersion) (dword_5172E8).
            (0x81, 0x6d) => Value::Int(i32::from(api.pixel_shader_version())),
            // The D3DXCreateEffect import of d3dx9_43.dll (sub_460B00).
            (0x81, 0x6e) => Value::Int(i32::from(api.d3dx_effects_available())),
            (0x81, 0x6f) => {
                // sub_460AB0: 0 drops the bicubic effect and returns 1; 1
                // needs ps_3_0 and D3DXCreateEffect (sub_460980), else 0.
                let mode = self.pop_int()?;
                let accepted = match mode {
                    0 => api.set_shader_effect_enabled(false),
                    1 => {
                        api.pixel_shader_version() >= 0x0300
                            && api.d3dx_effects_available()
                            && api.set_shader_effect_enabled(true)
                    }
                    _ => false,
                };
                Value::Int(i32::from(accepted))
            }
            (0x81, 0xb0) => {
                // sub_496770(a1 = first pop, a2 = second pop): -1 for a null
                // pointer, else the LCS length of the two UTF-16 strings.
                let a1 = self.pop_ptr()?;
                let a2 = self.pop_ptr()?;
                if a1 == 0 || a2 == 0 {
                    Value::Int(-1)
                } else {
                    let a1 = self.read_wide_c_string(a1)?;
                    let a2 = self.read_wide_c_string(a2)?;
                    Value::Int(wide_string_similarity(&a1, &a2))
                }
            }
            (0x81, 0xb7) => {
                // sub_496700: each Shift-JIS character becomes one 16-bit
                // unit, a double-byte one as lead << 8 | trail (no UTF-16
                // conversion); a null destination only counts.
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                if source == 0 {
                    Value::Int(-1)
                } else {
                    let mut at = source;
                    let mut out = destination;
                    let mut count = 0i32;
                    loop {
                        let lead = self.read_int(at, 0)? as u8;
                        if lead == 0 {
                            break;
                        }
                        let unit = if (0x81..=0x9f).contains(&lead) || lead >= 0xe0 && lead <= 0xfc {
                            let trail = self.read_int(at.wrapping_add(1), 0)?;
                            at = at.wrapping_add(2);
                            u32::from(lead) << 8 | trail
                        } else {
                            at = at.wrapping_add(1);
                            u32::from(lead)
                        };
                        if destination != 0 {
                            self.write_int(out, 1, unit)?;
                            out = out.wrapping_add(2);
                        }
                        count = count.wrapping_add(1);
                    }
                    if destination != 0 {
                        self.write_int(out, 1, 0)?;
                    }
                    Value::Int(count)
                }
            }
            (0x81, 0xd0) => {
                // sub_46AE10: capacity <= 1 (unsigned) -> 0x80000001; else a
                // new table, handle ++dword_566684, written to the output.
                let capacity = self.pop_int()? as u32;
                let destination = self.pop_ptr()?;
                let created = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .create_blob_table(capacity);
                match created {
                    Some(handle) => {
                        self.write_int(destination, 2, handle)?;
                        Value::Int(0)
                    }
                    None => Value::Int(system81_state::NATIVE_INVALID_ARGUMENT),
                }
            }
            (0x81, 0xd1) => {
                // sub_46AEC0: 0, or 0x80000002 for an unknown handle.
                let handle = self.pop_int()? as u32;
                let removed = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .blob_tables
                    .remove(&handle)
                    .is_some();
                Value::Int(if removed { 0 } else { system81_state::NATIVE_NOT_FOUND })
            }
            (0x81, 0xd2) => {
                // sub_46AF30(index output, index, source; size in edx).
                // With an output pointer the first empty slot (or the
                // capacity) is used and reported. Size 0 -> 0x80000003.
                let size = self.pop_int()? as u32;
                let source = self.pop_ptr()?;
                let index = self.pop_int()? as u32;
                let handle = self.pop_int()? as u32;
                let index_destination = self.pop_ptr()?;
                let exists = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .blob_tables
                    .contains_key(&handle);
                if !exists {
                    Value::Int(system81_state::NATIVE_NOT_FOUND)
                } else if size == 0 {
                    Value::Int(system81_state::NATIVE_OPERATION_FAILED)
                } else {
                    let range = self.resolve_range(source, size as usize)?;
                    let bytes = self.memory[range].to_vec();
                    let mut state = self.system81_shared.lock().expect("system81 state poisoned");
                    let table = state.blob_tables.get_mut(&handle).expect("checked above");
                    let slot = if index_destination != 0 { table.first_free() } else { index };
                    table.store(slot, bytes);
                    drop(state);
                    if index_destination != 0 {
                        self.write_int(index_destination, 2, slot)?;
                    }
                    Value::Int(0)
                }
            }
            (0x81, 0xd3) => {
                // sub_46AF90 -> sub_447A20: free one stored slot.
                let index = self.pop_int()? as u32;
                let handle = self.pop_int()? as u32;
                let mut state = self.system81_shared.lock().expect("system81 state poisoned");
                Value::Int(match state.blob_tables.get_mut(&handle).map(|table| table.remove(index)) {
                    None => system81_state::NATIVE_NOT_FOUND,
                    Some(true) => 0,
                    Some(false) => system81_state::NATIVE_INVALID_INDEX,
                })
            }
            (0x81, 0xd4) => {
                // sub_46AFC0 -> sub_447A60: copy a stored slot (when the
                // buffer is not null) and write its size.
                let index = self.pop_int()? as u32;
                let handle = self.pop_int()? as u32;
                let size_destination = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let fetched = {
                    let state = self.system81_shared.lock().expect("system81 state poisoned");
                    state.blob_tables.get(&handle).map(|table| table.get(index).map(<[u8]>::to_vec))
                };
                match fetched {
                    None => Value::Int(system81_state::NATIVE_NOT_FOUND),
                    Some(None) => Value::Int(system81_state::NATIVE_INVALID_INDEX),
                    Some(Some(bytes)) => {
                        if destination != 0 {
                            let range = self.resolve_write_range(destination, bytes.len())?;
                            self.memory[range].copy_from_slice(&bytes);
                            self.clear_shadow_values(destination, bytes.len());
                        }
                        self.write_int(size_destination, 2, bytes.len() as u32)?;
                        Value::Int(0)
                    }
                }
            }
            (0x81, 0xd5) => {
                // sub_46B000 -> sub_447AA0: (index, size) of every stored
                // slot into the list (when not null), count to the output.
                let handle = self.pop_int()? as u32;
                let count_destination = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let entries = {
                    let state = self.system81_shared.lock().expect("system81 state poisoned");
                    state.blob_tables.get(&handle).map(|table| table.entries())
                };
                match entries {
                    None => Value::Int(system81_state::NATIVE_NOT_FOUND),
                    Some(entries) => {
                        if destination != 0 {
                            for (position, (index, size)) in entries.iter().enumerate() {
                                let at = destination.wrapping_add(position as u32 * 8);
                                self.write_int(at, 2, *index)?;
                                self.write_int(at.wrapping_add(4), 2, *size)?;
                            }
                        }
                        self.write_int(count_destination, 2, entries.len() as u32)?;
                        Value::Int(0)
                    }
                }
            }
            (0x81, 0xda) => {
                if self.native_abi_profile!=NativeAbiProfile::Amachoco {return Ok(None);}
                // Confirmed ac_chinese.exe497340 ->4A47B0: resolve group/key,
                // optionally copy UTF-8 including NUL, return payload bytes
                // excluding NUL. A null destination is a length query.
                let text = self.pop_value()?;
                let keyword = self.pop_value()?;
                let destination = self.pop_ptr()?;
                let key=self.value_as_native_string_lossy(text)?;
                let group=self.value_as_native_string_lossy(keyword)?;
                let replacement=self.system81_shared.lock().map_err(|_|VmError::Runtime("sentence table state poisoned".into()))?.sentence_table.get(&group,&key).map(str::to_owned);
                if let Some(replacement)=replacement {
                    let bytes=replacement.as_bytes();let length=bytes.len();
                    if destination!=0 {
                        let range=self.resolve_write_range(destination,length+1)?;
                        self.memory[range.start..range.start+length].copy_from_slice(bytes);self.memory[range.start+length]=0;
                        self.clear_shadow_values(destination,length+1);self.mem_values.insert(self.value_key(destination),Value::Str(replacement));
                    }
                    Value::Int(length as i32)
                } else {Value::Int(0)}
            }
            (0x81,0xd8) if self.native_abi_profile==NativeAbiProfile::Amachoco => {
                // The confirmed later script consumes (bytes,length) and one
                // result, unlike the default executable's unregistered slot.
                // Only install a fully validated table; malformed data returns
                // failure and leaves prior shared registrations unchanged.
                let length=self.pop_int()?;
                let pointer=self.pop_ptr()?;
                if length==0 && pointer==0 {
                    self.system81_shared.lock().map_err(|_|VmError::Runtime("sentence table state poisoned".into()))?.sentence_table=sentence_table::SentenceTable::default();
                    Value::Int(1)
                } else if length<0 || !sentence_table::SentenceTable::valid_size(length as usize) {Value::Int(0)} else {
                    let bytes=self.resolve_range(pointer,length as usize)?;
                    if let Some(table)=sentence_table::SentenceTable::parse(&self.memory[bytes]) {
                        self.system81_shared.lock().map_err(|_|VmError::Runtime("sentence table state poisoned".into()))?.sentence_table=table;
                        Value::Int(1)
                    } else {Value::Int(0)}
                }
            }
            (0x81, 0xe0) => {
                // sub_48CFE0 -> sub_472A10(current dir, message, exit code,
                // dir, command, show, hide main window, wait, retry). The
                // command line is "dir\\command" or primary root + command;
                // the current directory defaults to dir, or to the primary
                // root without its trailing backslash.
                let hide_main_window = self.pop_int()? != 0;
                let error_message = self.pop_nullable_string()?;
                let show_window = self.pop_int()? != 0;
                let current_directory = self.pop_nullable_string()?;
                let command = self.pop_string_lossy()?;
                let directory = self.pop_nullable_string()?;
                let exit_code_destination = self.pop_ptr()?;
                let (command_line, default_directory) = match directory {
                    Some(directory) => (format!("{directory}\\{command}"), directory),
                    None => {
                        let root = self.resource_primary_root(api);
                        let mut trimmed = root.clone();
                        trimmed.pop();
                        (format!("{root}{command}"), trimmed)
                    }
                };
                let current_directory = current_directory.unwrap_or(default_directory);
                loop {
                    if let Some(code) = api.launch_process_and_wait(
                        &current_directory,
                        &command_line,
                        show_window,
                        hide_main_window,
                    ) {
                        if exit_code_destination != 0 {
                            self.write_int(exit_code_destination, 2, code)?;
                        }
                        break Value::Int(1);
                    }
                    let Some(message) = error_message.as_deref() else {
                        break Value::Int(0);
                    };
                    if !api.retry_or_quit_prompt("", message) {
                        break Value::Int(0);
                    }
                }
            }
            (0x81, 0xe9) => {
                let length = self.pop_int()?.max(0) as usize;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let range = self.resolve_range(source, length)?;
                let bytes = self.memory[range].to_vec();
                let initial = [
                    self.read_int(destination, 2)?,
                    self.read_int(destination.wrapping_add(4), 2)?,
                ];
                let hash = native_file_hash_update(initial, &bytes);
                self.write_int(destination, 2, hash[0])?;
                self.write_int(destination.wrapping_add(4), 2, hash[1])?;
                Value::None
            }
            (0x81, 0xea) => {
                let length = self.pop_int()?.max(0) as usize;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let range = self.resolve_range(source, length)?;
                let digest = system81_state::md5_digest(&self.memory[range]);
                let output = self.resolve_range(destination, digest.len())?;
                self.memory[output].copy_from_slice(&digest);
                self.clear_shadow_values(destination, digest.len());
                Value::None
            }
            (0x81, 0xec) => {
                // sub_490470: CreateMutexA(NULL, TRUE, name); 0 when it fails
                // or already existed (any process), else ++dword_56697C.
                let name = self.pop_string_lossy()?;
                match api.create_named_mutex(&name) {
                    Some(token) => {
                        let mut state = self.system81_shared.lock().expect("system81 state poisoned");
                        state.last_mutex_handle = state.last_mutex_handle.wrapping_add(1);
                        let handle = state.last_mutex_handle;
                        state.named_mutexes.insert(handle, token);
                        Value::Int(handle as i32)
                    }
                    None => Value::Int(0),
                }
            }
            (0x81, 0xed) => {
                // sub_490500: release and close; 1 when the handle is known.
                let handle = self.pop_int()? as u32;
                let token = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .named_mutexes
                    .remove(&handle);
                if let Some(token) = token {
                    api.release_named_mutex(token);
                }
                Value::Int(i32::from(token.is_some()))
            }
            (0x81, 0xf2) => {
                let text5 = self.pop_string_lossy()?;
                let text4 = self.pop_string_lossy()?;
                let text3 = self.pop_string_lossy()?;
                let text2 = self.pop_string_lossy()?;
                let text1 = self.pop_string_lossy()?;
                let mode = self.pop_int()?;
                let destination_pointer = self.pop_ptr()?;
                let source_pointer = self.pop_ptr()?;
                let descriptor_pointer = self.pop_ptr()?;
                let count = self.pop_int()?;
                let required_pointer = self.pop_ptr()?;
                let optional_pointer = self.pop_ptr()?;
                let root = self.pop_string_lossy()?;

                if !(0..=65_536).contains(&count) {
                    self.push_value(Value::Int(-1));
                    return Ok(Some(Value::None));
                }
                let count = count as usize;
                let optional_directories = if optional_pointer == 0 {
                    Vec::new()
                } else {
                    self.read_zero_terminated_string_pointer_list(optional_pointer, 4096)?
                };
                if required_pointer == 0 {
                    self.push_value(Value::Int(2));
                    return Ok(Some(Value::None));
                }
                let required_directories =
                    self.read_zero_terminated_string_pointer_list(required_pointer, 4096)?;
                let descriptor_values = if count == 0 {
                    Vec::new()
                } else if descriptor_pointer == 0 {
                    self.push_value(Value::Int(-1));
                    return Ok(Some(Value::None));
                } else {
                    let mut values = Vec::with_capacity(count);
                    for index in 0..count {
                        values.push(
                            self.read_int(descriptor_pointer.wrapping_add(index as u32 * 4), 2)?,
                        );
                    }
                    values
                };
                let source_files = self.read_counted_string_pointer_list(source_pointer, count)?;
                let destination_files =
                    self.read_counted_string_pointer_list(destination_pointer, count)?;
                let request = InstallationProcedureRequest {
                    root,
                    optional_directories,
                    required_directories,
                    descriptor_values,
                    source_files,
                    destination_files,
                    mode,
                    text1,
                    text2,
                    text3,
                    text4,
                    text5,
                };
                match api.run_installation_procedure(&request) {
                    Ok(()) => {
                        self.install_host_completed_procedure(
                            NativeOpcode { group, id },
                            native_call::NativeProcedureCompletion {
                                class: native_call::NativeProcedureClass::Installation,
                                status: 0,
                                outputs: [0; 2],
                                output_count: 0,
                            },
                            false,
                        );
                    }
                    Err(status @ (1..=3)) => self.push_value(Value::Int(status)),
                    Err(_) => self.push_value(Value::Int(-1)),
                }
                Value::None
            }
            (0x81, 0xf7) => {
                let special_folder_mode = self.pop_int()?;
                let target = self.pop_string_lossy()?;
                let shortcut_name = self.pop_string_lossy()?;
                let program_group = self.pop_string_lossy()?;
                let group = (!program_group.is_empty()).then_some(program_group.as_str());
                Value::Int(i32::from(api.create_special_folder_shortcut(
                    special_folder_mode,
                    group,
                    &shortcut_name,
                    &target,
                )))
            }
            (0x80, 0x06) => {
                let enabled = self.pop_int()?;
                api.set_performance_profiling(enabled);
                Value::None
            }
            (0x80, 0x07) => {
                let metric_id = self.pop_int()?;
                let output_ptr = self.pop_ptr()?;
                self.write_int(output_ptr, 2, api.read_performance_metric(metric_id) as u32)?;
                self.clear_shadow_values(output_ptr, 4);
                Value::None
            }
            (0x80, 0x09) => Value::Int(api.presentation_state()),
            (0x80, 0x0a) => {
                let destination = self.pop_ptr()?;
                let record = api.graphics_capability_record();
                for (index, value) in record.into_iter().enumerate() {
                    self.write_int(destination.wrapping_add(index as u32 * 4), 2, value)?;
                }
                self.clear_shadow_values(destination, 0x40);
                Value::None
            }
            (0x80, 0x0b) => Value::Int(api.graphics_memory_metric()),
            (0x80, 0x2b) => {
                // sub_489130 pops the path, then the extension, file name,
                // directory and drive outputs (script order drive, dir,
                // name, ext, path). sub_42D7F0 splits the path as wide text
                // (_wsplitpath) and writes each non-null output; a null
                // path writes nothing and returns 0.
                let path = self.pop_value()?;
                let ext = self.pop_ptr()?;
                let name = self.pop_ptr()?;
                let dir = self.pop_ptr()?;
                let drive = self.pop_ptr()?;
                if matches!(path, Value::Int(0) | Value::Ptr(0) | Value::None) {
                    Value::Int(0)
                } else {
                    let text = self.value_as_string_lossy(path)?;
                    let parts = msvc_wsplitpath(&text);
                    for (destination, part) in [(drive, &parts[0]), (dir, &parts[1]), (name, &parts[2]), (ext, &parts[3])] {
                        if destination != 0 {
                            self.write_c_string(destination, part)?;
                        }
                    }
                    Value::Int(1)
                }
            }
            (0x80, 0x0c) => {
                let destination = self.pop_ptr()?;
                let fields = portable_local_system_time();
                for (index, value) in fields.into_iter().enumerate() {
                    self.write_int(
                        destination.wrapping_add(index as u32 * 2),
                        1,
                        u32::from(value),
                    )?;
                }
                self.clear_shadow_values(destination, 16);
                Value::None
            }
            (0x80, 0x0d) => {
                let (total, available) = api.host_physical_memory_bytes();
                let total = total.min(i32::MAX as u64) as i32;
                let available = available.min(i32::MAX as u64) as i32;
                self.push_value(Value::Int(total));
                Value::Int(available)
            }
            (0x80, 0x00) => {
                let seed = self.pop_int()? as u32;
                self.rng_seed = seed;
                api.seed_native_crt_rng(seed);
                Value::None
            }
            (0x80, 0x01) => Value::Int(self.rand_msvc_with_api(api)),
            (0x80, 0x02) => {
                let max = self.pop_int()?;
                if max > 0 {
                    let high = self.rand_msvc_with_api(api) << 8;
                    let mid = self.rand_msvc_with_api(api);
                    let value = (high ^ mid) << 8;
                    let wide = value ^ self.rand_msvc_with_api(api);
                    Value::Int(wide.rem_euclid(max))
                } else {
                    Value::Int(0)
                }
            }
            (0x80, 0x0f) => Value::Int(i32::from(api.window_active())),
            (0x80, 0x11) => {
                let descriptor = self.pop_int()?;
                Value::Int(i32::from(api.query_key_held(descriptor)))
            }
            (0x80, 0x12) => {
                // Target 0x00488040 treats the argument as a contiguous,
                // zero-terminated DWORD descriptor array. It sums
                // dword_518CA4[6 * descriptor] for every entry without
                // consuming the input state.
                let descriptors = self.pop_ptr()?;
                let mut total = 0i32;
                if descriptors != 0 {
                    let start = self.memory_addr(descriptors) as usize;
                    if start >= self.memory.len() {
                        return Err(VmError::MemoryOutOfBounds {
                            addr: self.memory_addr(descriptors),
                            size: 4,
                        });
                    }
                    let max_entries = (self.memory.len() - start) / 4;
                    let mut terminated = false;
                    for index in 0..max_entries {
                        let descriptor = self
                            .read_int(descriptors.wrapping_add((index as u32).wrapping_mul(4)), 2)?
                            as i32;
                        if descriptor == 0 {
                            terminated = true;
                            break;
                        }
                        total = total.wrapping_add(api.peek_input_state(descriptor));
                    }
                    if !terminated {
                        return Err(VmError::Runtime(
                            "Sys80:12 input descriptor array is not zero terminated".into(),
                        ));
                    }
                }
                Value::Int(total)
            }
            (0x80, 0x10) => {
                let value = self.pop_int()?;
                api.reset_input_configuration(value);
                Value::None
            }
            (0x80, 0x13) => Value::Int(api.input_message_serial()),
            (0x80, 0x14) => {
                let value = self.pop_int()?;
                api.set_input_master_gate(value);
                Value::None
            }
            (0x80, 0x15) => {
                let value = self.pop_int()?;
                api.set_input_latched_state(value);
                Value::None
            }
            (0x80, 0x16) => {
                api.sample_configured_input();
                Value::None
            }
            (0x80, 0x17) => Value::Int(api.query_configured_input_gate()),
            (0x80, 0x18) => {
                let scope = self.pop_int()?;
                api.register_input_scope(scope);
                Value::None
            }
            (0x80, 0x19) => {
                let scope = self.pop_int()?;
                api.query_and_unregister_input_scope(scope);
                Value::None
            }
            (0x80, 0x1a) => {
                let scope = self.pop_int()?;
                Value::Int(api.query_input_event_bits(scope))
            }
            (0x80, 0x1b) => {
                // sub_4881C0 pops the descriptor pointer before the class
                // mask. sub_46DFA0 accepts at most fifteen nonzero entries.
                let descriptor_ptr = self.pop_ptr()?;
                let class_mask = self.pop_int()?;
                let mut descriptors = Vec::new();
                if descriptor_ptr != 0 {
                    let mut terminated = false;
                    for index in 0..16u32 {
                        let descriptor =
                            self.read_int(descriptor_ptr.wrapping_add(index * 4), 2)? as i32;
                        if descriptor == 0 {
                            terminated = true;
                            break;
                        }
                        descriptors.push(descriptor);
                    }
                    if !terminated {
                        // sub_46E500 counts the list first (-2).
                        return Err(VmError::Runtime(format!(
                            "指定されたキー配列は長過ぎます\n\nキー配列の長さは [ {} ] エレメント以内でなければなりません",
                            15
                        )));
                    }
                }
                // Then the class must be one of sub_46E500's ten tables (-1).
                const CLASSES: [u32; 10] =
                    [0x40, 0x80, 0x100, 0x200, 0x1000, 0x2000, 0x4000, 0x8000, 0x4000_0000, 0x8000_0000];
                if !CLASSES.contains(&(class_mask as u32)) {
                    return Err(VmError::Runtime(format!(
                        "無効なスタンダードキー [ ${:08X} ] が指定されました",
                        class_mask as u32
                    )));
                }
                api.register_input_class_descriptors(class_mask, &descriptors);
                Value::None
            }
            (0x80, 0x1c) => {
                let class_mask = self.pop_int()?;
                Value::Int(api.query_input_descriptor_state(class_mask))
            }
            (0x80, 0x62) => {
                // sub_489360 pops the BP descriptor pointer first and the
                // enable flag second. sub_461740 preserves the previous list
                // while disabled, clears it for an enabled null pointer, and
                // rejects lists with sixteen or more nonzero entries.
                let descriptor_ptr = self.pop_ptr()?;
                let enabled = self.pop_int()? != 0;
                if !enabled {
                    api.configure_fullscreen_hotkeys(false, &[]);
                    Value::None
                } else {
                    let mut descriptors = Vec::new();
                    if descriptor_ptr != 0 {
                        let mut terminated = false;
                        for index in 0..16u32 {
                            let descriptor =
                                self.read_int(descriptor_ptr.wrapping_add(index * 4), 2)? as i32;
                            if descriptor == 0 {
                                terminated = true;
                                break;
                            }
                            descriptors.push(descriptor);
                        }
                        if !terminated {
                            return Err(VmError::Runtime(
                                "ディスプレイスタイルチェンジの対象として指定されたキーの配列が長過ぎます"
                                    .into(),
                            ));
                        }
                    }
                    api.configure_fullscreen_hotkeys(true, &descriptors);
                    Value::None
                }
            }
            (0x80, 0x4b) => self.sys80_4b_dequeue_message_array()?,
            (0x80, 0x5a) => Value::None,
            (0x80, 0xcf) => {
                let requested = self.pop_int()?.max(0) as usize;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                // sub_494430: strcmp, so the terminating NUL is part of it.
                let sdc_header = b"SDC FORMAT 1.00\0";
                let has_sdc_header = self
                    .resolve_range(source, sdc_header.len())
                    .ok()
                    .is_some_and(|range| &self.memory[range] == sdc_header);
                let output = if has_sdc_header {
                    self.decode_sdc_records(source, destination)?
                } else {
                    // 499120 -> 465880 dispatches DSC, both CBG decoders,
                    // or an unwrapped payload. Length describes the input,
                    // not the capacity of the decoded destination.
                    let range = self.resolve_range(source, requested)?;
                    let input = &self.memory[range];
                    let decoded = if input.starts_with(b"DSC FORMAT 1.00") {
                        if input.get(20..24).is_some_and(|v| u32::from_le_bytes(v.try_into().unwrap()) <= 0x400_0000) {
                            ethornell_archive::dsc_decode(input).unwrap_or_default()
                        } else { Vec::new() }
                    } else if input.starts_with(b"CompressedBG___") {
                        ethornell_image::decode_cbg_native_buffer(input).unwrap_or_default()
                    } else { input.to_vec() };
                    let count = decoded.len();
                    let range = self.resolve_write_range(destination, count)?;
                    self.memory[range].copy_from_slice(&decoded);
                    self.clear_shadow_values(destination, count);
                    count as i32
                };
                self.install_host_completed_procedure(
                    NativeOpcode { group, id },
                    native_call::NativeProcedureCompletion {
                        class: native_call::NativeProcedureClass::DecodeData,
                        status: 0,
                        outputs: [output, 0],
                        output_count: 1,
                    },
                    false,
                );
                Value::None
            }
            (0x80, 0xc0) => {
                let length = self.pop_int()?;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let output = self.encode_user_data_buffer(destination, source, length)?;
                self.install_host_completed_procedure(
                    NativeOpcode { group, id },
                    native_call::NativeProcedureCompletion {
                        class: native_call::NativeProcedureClass::EncodeData,
                        status: 0,
                        outputs: [output, 0],
                        output_count: 1,
                    },
                    false,
                );
                Value::None
            }
            (0x80, 0xc1) => {
                // Exact handler 0x48A4D0 saves the first pop in ESI and passes
                // it in EAX as sub_4938F0's source. The second pop is pushed
                // as that decoder's destination argument.
                let src = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                let written = self.decode_sdc_records(src, dst)?;
                tracing::debug!(
                    src = format_args!("0x{src:08X}"),
                    dst = format_args!("0x{dst:08X}"),
                    written,
                    "SdcDecodeRecords"
                );
                Value::Int(written)
            }
            (0x80, 0xc4) => {
                let record_count = self.pop_int()?;
                let record_size = self.pop_int()?;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let output =
                    self.encode_user_data_structs(destination, source, record_size, record_count)?;
                self.install_host_completed_procedure(
                    NativeOpcode { group, id },
                    native_call::NativeProcedureCompletion {
                        class: native_call::NativeProcedureClass::EncodeStruct,
                        status: 0,
                        outputs: [output, 0],
                        output_count: 1,
                    },
                    false,
                );
                Value::None
            }
            (0x80, 0xc5) => {
                let src = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                Value::Int(self.decode_sdc_struct_array(src, dst)?)
            }
            (0x80, 0xd2) => {
                let src = self.pop_ptr()?;
                let key = self.pop_string_lossy()?;
                let handle = self.pop_int()? as u32;
                Value::Int(self.sys_record_table_copy(handle, Value::Str(key), src)?)
            }
            (0x80, 0xd3) => {
                let key = self.pop_string_lossy()?;
                let handle = self.pop_int()? as u32;
                Value::Int(self.sys_record_table_remove(handle, Value::Str(key)))
            }
            (0x80, 0x84) => self.sys80_84_intern_resource_name()?,
            (0x80, 0x85) => self.sys80_85_resource_name_exists()?,
            (0x80, 0x8a) => self.sys80_8a_set_read_flag_range()?,
            (0x80, 0x8b) => self.sys80_8b_query_read_flag_bit()?,
            (0x80, 0xd8) => {
                // sub_4954A0 is called with EDI=1: clear every namespace
                // except the protected table whose key is 0x80000000.
                self.string_hash_tables
                    .retain(|table_id, _| *table_id == i32::MIN);
                Value::None
            }
            (0x80, 0xd9) => {
                let table_id = self.pop_int()?;
                Value::Int(
                    self.string_hash_tables
                        .get(&table_id)
                        .map_or(0, |values| values.len().min(i32::MAX as usize) as i32),
                )
            }
            (0x80, 0xda) => {
                // Handler pop order: packed source, count, table id.
                let src = self.pop_ptr()?;
                let count = self.pop_int()?;
                let table_id = self.pop_int()?;
                if count == 0 {
                    self.string_hash_tables.remove(&table_id);
                    Value::Int(1)
                } else if count < 0 || src == 0 {
                    Value::Int(0)
                } else {
                    let mut cursor = src;
                    let mut values = Vec::with_capacity(count as usize);
                    for _ in 0..count as usize {
                        let text = self.read_c_string(cursor)?;
                        let byte_len = self.c_string_byte_len(cursor)?;
                        values.push(text);
                        cursor = cursor.saturating_add(byte_len as u32 + 1);
                    }
                    self.string_hash_tables.insert(table_id, values);
                    Value::Int(1)
                }
            }
            (0x80, 0xdb) => {
                let table_id = self.pop_int()?;
                let destination = self.pop_ptr()?;
                let values = self
                    .string_hash_tables
                    .get(&table_id)
                    .cloned()
                    .unwrap_or_default();
                let mut cursor = destination;
                let mut total = 0usize;
                for value in values {
                    let byte_len = self.encode_native_string(&value).len() + 1;
                    if destination != 0 {
                        self.write_c_string_raw(cursor, &value)?;
                        cursor = cursor.saturating_add(byte_len as u32);
                    }
                    total = total.saturating_add(byte_len);
                }
                Value::Int(total.min(i32::MAX as usize) as i32)
            }
            (0x80, 0xdc) => {
                // sub_48A850 pops the string first and then the namespace id.
                // It interns the Shift-JIS string only; no graph/resource side
                // effect is performed by the target helper.
                let value = self.pop_string_lossy()?;
                let table_id = self.pop_int()?;
                let values = self.string_hash_tables.entry(table_id).or_default();
                let index = values
                    .iter()
                    .position(|saved| saved == &value)
                    .unwrap_or_else(|| {
                        let index = values.len();
                        values.push(value);
                        index
                    });
                Value::Int(index.min(i32::MAX as usize) as i32)
            }
            (0x80, 0xde) => {
                let index = self.pop_int()?;
                let table_id = self.pop_int()?;
                let output_length = self.pop_ptr()?;
                let Some(values) = self.string_hash_tables.get(&table_id) else {
                    return Ok(Some(Value::Int(i32::MIN + 1)));
                };
                let Some(text) = usize::try_from(index)
                    .ok()
                    .and_then(|index| values.get(index))
                else {
                    return Ok(Some(Value::Int(i32::MIN + 2)));
                };
                let byte_len = self.encode_native_string(text).len();
                self.write_int(output_length, 2, byte_len.min(u32::MAX as usize) as u32)?;
                Value::Int(0)
            }
            (0x80, 0x04) => Value::Int(self.timing.tick_count()),
            (0x80, 0x05) => {
                // sub_4888D0 -> sub_499430: QueryPerformanceCounter as
                // nanoseconds; it does not stop inside clock pause sections.
                let ptr = self.pop_ptr()?;
                let counter = api
                    .performance_counter_ns()
                    .unwrap_or_else(|| self.timing.performance_counter() as u64);
                self.write_int(ptr, 2, counter as u32)?;
                self.write_int(ptr.wrapping_add(4), 2, (counter >> 32) as u32)?;
                Value::Int(1)
            }
            (0x80, 0x20) => {
                // sub_488E80: sub_498460 (1..=64 MiB, unsigned), then
                // sub_48E760; no free slot is fatal.
                let size = self.pop_int()? as u32;
                if size == 0 || size > 0x400_0000 {
                    return Err(VmError::Runtime(format!(
                        "無効なサイズ [ {} ] byteのメモリ領域を確保しようとしました\nメモリサイズは最大で [ {} ] byteまでしか確保できません",
                        size as i32, 0x400_0000
                    )));
                }
                let ptr = self.alloc_heap(size);
                if ptr == 0 {
                    return Err(VmError::Runtime("グローバル領域に空きがありません".into()));
                }
                Value::Int(ptr as i32)
            }
            (0x80, 0x21) => {
                // sub_488ED0: 0 is accepted; anything sub_48E800 does not
                // free (not a block start) is fatal.
                let ptr = self.pop_int()? as u32;
                if ptr != 0 && !self.free_heap(ptr) {
                    return Err(VmError::Runtime(format!(
                        "グローバルメモリの解放の対象に不正なアドレス [ ${ptr:08X} ] が指定されました"
                    )));
                }
                Value::Int(1)
            }
            (0x80, 0x33) => self.sys80_33_delete_file(api)?,
            (0x80, 0x25) => self.sys80_25_enumerate_files(api)?,
            (0x80, 0x26) => self.sys80_26_enumerate_directories(api)?,
            (0x80, 0x6d) => {
                // sub_464040 copies szFile while file drop is enabled and a
                // path is pending; reading leaves the buffer in place.
                let destination = self.pop_ptr()?;
                match api.take_dropped_file().filter(|path| !path.is_empty()) {
                    Some(path) => {
                        self.write_c_string(destination, &path)?;
                        Value::Int(1)
                    }
                    None => Value::Int(0),
                }
            }
            (0x80, 0x36) => {
                // sub_4657A0 stores the raw value in dword_506BE0.
                let enabled = self.pop_int()? != 0;
                self.additional_resource_search_enabled = enabled;
                api.set_loose_search_fallback(enabled);
                Value::None
            }
            (0x80, 0x37) => {
                // sub_4657F0 links the copy in front of dword_566648.
                let path = self.pop_string_lossy()?;
                self.additional_resource_paths.insert(0, path.clone());
                api.prepend_loose_search_path(&path);
                Value::None
            }
            (0x80, 0x38) => {
                // sub_489480 pops the NUL-terminated archive-name pointer list,
                // then the composite name; sub_406BC0 refuses a name already
                // in the archive chain, sub_445C50 an empty list and
                // sub_445C00 a null name. Names compare after the DBCS-aware
                // ASCII lowering of sub_42EE60; the chain's plain archives are
                // keyed by full path, which a composite name never matches.
                let list = self.pop_ptr()?;
                let name = match self.pop_value()? {
                    Value::Int(0) | Value::Ptr(0) | Value::None => None,
                    value => Some(self.value_as_string_lossy(value)?),
                };
                let members = self.read_zero_terminated_string_pointer_list(list, 0x10000)?;
                let registered = name.as_ref().is_some_and(|name| {
                    !members.is_empty()
                        && !self
                            .composite_archives
                            .keys()
                            .any(|existing| existing.eq_ignore_ascii_case(name))
                });
                if let Some(name) = name.filter(|_| registered) {
                    api.register_composite_archive(&name, &members);
                    self.composite_archives.insert(name, members);
                }
                Value::Int(i32::from(registered))
            }
            (0x80, 0x39) => {
                // sub_46B8F0 keeps an existing directory with a trailing
                // backslash in byte_518988.
                let path = self.pop_string_lossy()?;
                let valid = api.directory_exists(&path);
                if valid {
                    self.validated_file_root = Some(ensure_trailing_separator(&path));
                }
                Value::Int(i32::from(valid))
            }
            (0x80, 0x3a) => {
                // sub_467AD0: 0 GetWindowsDirectoryA and 4 SHGetFolderPathA
                // (CSIDL_PROGRAM_FILES) report 1 even when they fail and leave
                // the buffer alone; 1..3 (CSIDL 16/2/5) and 5 (the registry's
                // ProgramFilesDir) report whether a path was written; other
                // modes report 0.
                let mode = self.pop_int()?;
                let destination = self.pop_ptr()?;
                let path = api.special_folder_path(mode);
                if let Some(path) = &path {
                    self.write_c_string(destination, path)?;
                }
                Value::Int(i32::from(path.is_some() || matches!(mode, 0 | 4)))
            }
            (0x80, 0x3b) => {
                // sub_489570 -> sub_467070 -> sub_466E80: one filter
                // "description(*.ext)"; the 780-byte path buffer is cleared
                // before the mode check (0 open, 1 save, otherwise 4); the
                // result is GetOpen/SaveFileNameA - 1.
                let mode = self.pop_int()?;
                let initial_directory = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let extension = self.pop_string_lossy()?;
                let description = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let range = self.resolve_write_range(destination, 0x30c)?;
                self.memory[range].fill(0);
                self.clear_shadow_values(destination, 0x30c);
                match api.open_file_dialog(
                    &initial_directory,
                    &description,
                    &extension,
                    &title,
                    mode,
                ) {
                    Ok(Some(path)) => {
                        self.write_c_string(destination, &path)?;
                        Value::Int(0)
                    }
                    Ok(None) => Value::Int(-1),
                    Err(status) => Value::Int(status),
                }
            }
            (0x80, 0x3d) => {
                // sub_464F00: kind 0 copies ::Buffer, kind 1 the media root
                // only when one was found; anything else returns 0.
                let kind = self.pop_int()?;
                let ptr = self.pop_ptr()?;
                let root = match kind {
                    0 => Some(self.resource_primary_root(api)),
                    1 => Some(self.resource_media_root()).filter(|root| !root.is_empty()),
                    _ => None,
                };
                if let Some(root) = root {
                    self.write_c_string(ptr, &root)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x80, 0x45) => Value::None,
            (0x80, 0x80) => {
                let result = self.load_global_user_data(api)?;
                self.push_value(Value::Int(result.window_x));
                self.push_value(Value::Int(result.window_y));
                Value::Int(result.status)
            }
            (0x80, 0x81) => Value::Int(i32::from(self.save_global_user_data(api))),
            (0x80, 0x88) => self.sys80_88_create_or_resize_read_flag_table()?,
            (0x80, 0x89) => self.sys80_89_set_read_flag_bit()?,
            (0x80, 0xd0) => {
                let record_size = self.pop_int()?;
                let slot = self.pop_ptr()?;
                Value::Int(self.sys_record_table_open(slot, record_size.max(0) as u32)?)
            }
            (0x80, 0xd1) => {
                let handle = self.pop_int()? as u32;
                Value::Int(self.sys_record_table_close(handle))
            }
            (0x80, 0xd4) => {
                let index = self.pop_int()?;
                let key_ptr = self.pop_ptr()?;
                let selector = if key_ptr == 0 {
                    Value::Ptr(0)
                } else {
                    Value::Str(self.read_c_string(key_ptr)?)
                };
                let handle = self.pop_int()? as u32;
                let dst = self.pop_ptr()?;
                self.sys_record_table_fetch(dst, handle, selector, index)?
            }
            (0x80, 0x82) => {
                let length = self.pop_int()?;
                let src = self.pop_ptr()?;
                let offset = self.pop_int()?;
                self.copy_to_global_data(offset, src, length)?;
                Value::None
            }
            (0x80, 0x83) => {
                let length = self.pop_int()?;
                let offset = self.pop_int()?;
                let dst = self.pop_ptr()?;
                self.copy_from_global_data(dst, offset, length)?;
                Value::None
            }
            (0x80, 0x90) => self.sys80_90_reset_structured_history()?,
            (0x80, 0x91) => self.sys80_91_structured_history_count()?,
            (0x80, 0x94) => self.sys80_94_append_structured_history_fields()?,
            (0x80, 0x95) => self.sys80_95_97_read_structured_history(false)?,
            (0x80, 0x96) => self.sys80_96_append_structured_history_record()?,
            (0x80, 0x97) => self.sys80_95_97_read_structured_history(true)?,
            (0x80, 0x98) => {
                // sub_46C730: either value 0 -> 2; capacity is unsigned.
                let record_size = self.pop_int()?.max(0) as u32;
                let capacity = self.pop_int()? as u32;
                let slot = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_open(slot, capacity, record_size)?)
            }
            (0x80, 0x9a) => {
                let handle = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_count(dst, handle)?)
            }
            (0x80, 0x9d) => {
                let index = self.pop_int()?;
                let handle = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_load(dst, handle, index)?)
            }
            (0x80, 0x9e) => {
                let count = self.pop_int()? as u32;
                let start = self.pop_int()?;
                let handle = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_remove(handle, start, count))
            }
            (0x80, 0x99) => {
                let handle = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_close(handle))
            }
            (0x80, 0x9c) => {
                let src = self.pop_ptr()?;
                let handle = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_push(handle, src)?)
            }
            (0x80, 0xb0) => {
                let capacity = self.pop_int()?;
                let name = self.pop_string_lossy()?;
                Value::Int(
                    self.system80_shared
                        .lock()
                        .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                        .exclusions
                        .create(name, capacity),
                )
            }
            (0x80, 0xb1) => {
                let name = self.pop_string_lossy()?;
                Value::Int(
                    self.system80_shared
                        .lock()
                        .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                        .exclusions
                        .delete(&name),
                )
            }
            (0x80, 0xb4) => {
                let priority = self.pop_int()? as u32;
                let name = self.pop_string_lossy()?;
                let thread_id = self.thread.thread_id();
                let section_id = self
                    .system80_shared
                    .lock()
                    .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                    .exclusions
                    .enqueue(&name, thread_id, priority);
                if let Some(section_id) = section_id {
                    self.install_cprocedure(
                        InstalledCProcedure::exclusion(
                            thread_id,
                            NativeOpcode { group, id },
                            section_id,
                        ),
                        false,
                    );
                } else {
                    self.install_host_completed_procedure(
                        NativeOpcode { group, id },
                        native_call::NativeProcedureCompletion {
                            class: native_call::NativeProcedureClass::Exclusion,
                            status: system80_state::NATIVE_NOT_FOUND,
                            outputs: [system80_state::NATIVE_NOT_FOUND, 0],
                            output_count: 1,
                        },
                        false,
                    );
                }
                Value::None
            }
            (0x80, 0xb5) => {
                let name = self.pop_string_lossy()?;
                Value::Int(
                    self.system80_shared
                        .lock()
                        .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                        .exclusions
                        .release(&name, self.thread.thread_id()),
                )
            }
            (0x80, 0xb6) => {
                let priority = self.pop_int()? as u32;
                let name = self.pop_string_lossy()?;
                Value::Int(
                    self.system80_shared
                        .lock()
                        .map_err(|_| VmError::Runtime("system80 shared state poisoned".into()))?
                        .exclusions
                        .query_available(&name, priority),
                )
            }
            (0x80, 0xdd) => {
                // sub_48A900 pops index, table_id, then destination.
                // sub_495850 uses the 12-byte metadata slot only to obtain the
                // payload pointer and stored length, then copies those bytes.
                // sub_495550 stores Shift-JIS entry bytes including NUL, which
                // is exactly the representation retained by this namespace.
                let index = self.pop_int()?;
                let table_id = self.pop_int()?;
                let dst = self.pop_ptr()?;
                let Some(values) = self.string_hash_tables.get(&table_id) else {
                    return Ok(Some(Value::Int(i32::MIN + 1)));
                };
                let Some(text) = usize::try_from(index)
                    .ok()
                    .and_then(|index| values.get(index))
                    .cloned()
                else {
                    return Ok(Some(Value::Int(i32::MIN + 2)));
                };
                self.write_c_string(dst, &text)?;
                tracing::debug!(table_id, index, text, "StringHashTableGet");
                Value::Int(0)
            }
            (0x80, 0x58) => {
                let _duration = self.pop_value()?;
                Value::None
            }
            (0x80, 0x70) => {
                // sub_48A090 -> sub_46C0F0: an index above 12 is fatal; the
                // handler only ever pushes 1.
                let shift = self.pop_int()?;
                if !self.allocate_global_config(shift) {
                    return Err(VmError::Runtime(format!(
                        "無効なサイズ番号 [ {shift} ] が指定されました"
                    )));
                }
                Value::Int(1)
            }
            (0x80, 0x71) => {
                // sub_48A100: memset(dword_56676C, 0, Size).
                let size = self.global_size;
                self.global_bytes_mut().fill(0);
                self.clear_shadow_values(0, size);
                Value::None
            }
            (0x80, 0x74) => {
                // sub_489630 -> sub_46B420 stores the source value verbatim
                // in dword_506BDC. Save-file writers and readers use zero as
                // the sole disabled value when adding/checking their 64-byte
                // integrity header.
                self.save_data_integrity_enabled = self.pop_int()?;
                Value::None
            }
            (0x80, 0x78) => {
                // sub_489650 pops the label pointer first and the slot second.
                // sub_48A140 ignores whether sub_4664F0 wrote the file.
                let label = self.pop_string_lossy()?;
                let slot = self.pop_int()?;
                self.save_config_slot(api, slot, &label)?;
                Value::None
            }
            (0x80, 0x79) => {
                let slot = self.pop_int()?;
                Value::Int(self.load_config_slot(api, slot))
            }
            (0x80, 0x7a) => {
                // sub_489730 pops the slot before the destination pointer.
                let slot = self.pop_int()?;
                let destination = self.pop_ptr()?;
                match self.read_config_slot_header(api, slot) {
                    Ok(header) => {
                        let range = self.resolve_write_range(destination, header.len())?;
                        self.memory[range].copy_from_slice(&header);
                        self.clear_shadow_values(destination, header.len());
                        Value::Int(0)
                    }
                    Err(status) => Value::Int(status),
                }
            }
            (0x80, 0x7b) => {
                let slot = self.pop_int()?;
                Value::Int(self.validate_config_slot(api, slot))
            }
            (0x80, 0xe0) => {
                // sub_48B470: pops hide-window flag, error message, command
                // line, directory; retries through OK|CANCEL.
                let hide_window = self.pop_int()? != 0;
                let error_message = self.pop_nullable_string()?;
                let command_line = self.pop_string_lossy()?;
                let working_directory = self.pop_nullable_string()?;
                Value::Int(self.launch_native_process(
                    api,
                    working_directory,
                    &command_line,
                    error_message,
                    hide_window,
                    true,
                    false,
                ))
            }
            (0x80, 0xe1) => {
                // The target distinguishes a null command-line pointer from a
                // valid pointer to an empty string. Only the null pointer is a
                // fatal script contract violation; the working directory and
                // error-message pointers are nullable.
                let error_message_ptr = self.pop_ptr()?;
                let command_line_ptr = self.pop_ptr()?;
                let working_directory_ptr = self.pop_ptr()?;
                if command_line_ptr == 0 {
                    return Err(VmError::Runtime(
                        "外部プログラムのファイル名にNULLポインタが指定されました".into(),
                    ));
                }
                let error_message = if error_message_ptr == 0 {
                    None
                } else {
                    Some(self.read_c_string(error_message_ptr)?)
                };
                let command = self.read_c_string(command_line_ptr)?;
                // sub_48B4D0 keeps the strings; WinMain resolves them with
                // sub_472A10 like Sys80:E0 (the roots do not change after
                // status 6).
                let (command_line, working_directory) = if working_directory_ptr == 0 {
                    let root = self.resource_primary_root(api);
                    (format!("{root}{command}"), root)
                } else {
                    let directory = self.read_c_string(working_directory_ptr)?;
                    (format!("{directory}\\{command}"), directory)
                };
                api.schedule_restart(&working_directory, &command_line, error_message.as_deref());
                self.scheduler_signal = Some(SchedulerSignal::TerminateInterpreter);
                Value::None
            }
            (0x80, 0xe2) => {
                // sub_48B5D0: pops error message, command line, directory;
                // hides the window, no retry, then waits for the
                // uninstaller mutex.
                let error_message = self.pop_nullable_string()?;
                let command_line = self.pop_string_lossy()?;
                let working_directory = self.pop_nullable_string()?;
                Value::Int(self.launch_native_process(
                    api,
                    working_directory,
                    &command_line,
                    error_message,
                    true,
                    false,
                    true,
                ))
            }
            (0x80, 0xe3) => {
                let target = self.pop_string_lossy()?;
                Value::Int(i32::from(api.shell_open(&target)))
            }
            (0x80, 0xe8) => {
                let ptr = self.pop_ptr()?;
                self.write_c_string(ptr, api.game_id())?;
                Value::None
            }
            (0x80, 0xe9) => {
                // sub_48B680: an absolute name as is, else primary root +
                // name; the raw file (CFileDX) is hashed by sub_401900.
                let file = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                let path = if resource_io::is_absolute_resource_path(&file) {
                    file
                } else {
                    format!("{}{file}", self.resource_primary_root(api))
                };
                let Some(bytes) = api.read_user_file_bytes(&path) else {
                    return Ok(Some(Value::Int(0)));
                };
                let mut hash = 0u32;
                let mut tail = [0u8; 4];
                for byte in bytes {
                    hash = hash.wrapping_mul(233).wrapping_add(u32::from(byte));
                    let low = hash as u8;
                    tail[0] = tail[0].wrapping_add(low);
                    tail[1] ^= low;
                    tail[2] = tail[2].wrapping_add(byte);
                    tail[3] ^= byte;
                }
                let range = self.resolve_write_range(output, 8)?;
                self.memory[range.start..range.start + 4].copy_from_slice(&hash.to_le_bytes());
                self.memory[range.start + 4..range.end].copy_from_slice(&tail);
                self.clear_shadow_values(output, 8);
                Value::Int(1)
            }
            (0x80, 0xea) => {
                let product = self.pop_string_lossy()?;
                api.set_uninstaller_product(&product);
                Value::None
            }
            (0x80, 0xf0) => {
                // sub_48B740 pops: description (nullable, control 1004),
                // default subfolder, check box 1003 and 1002 states, the
                // initial folder, the 1003 and 1002 outputs, and the output
                // folder buffer; outputs are written only on OK.
                let caption = self.pop_nullable_string()?.unwrap_or_default();
                let default_subfolder = self.pop_string_lossy()?;
                let option_value2 = self.pop_int()?;
                let option_value1 = self.pop_int()?;
                let initial_text = self.pop_string_lossy()?;
                let output_value2 = self.pop_ptr()?;
                let output_value1 = self.pop_ptr()?;
                let output_text = self.pop_ptr()?;
                let request = SystemInputDialogRequest {
                    initial_text,
                    option_value1,
                    option_value2,
                    default_subfolder,
                    caption,
                };
                if let Some(response) = api.show_system_input_dialog(&request) {
                    self.write_int(output_value1, 2, response.option_value1 as u32)?;
                    self.write_int(output_value2, 2, response.option_value2 as u32)?;
                    self.write_c_string_bounded(output_text, &response.text, 779)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x80, 0xf1) => {
                let mode2 = self.pop_int()?;
                let mode1 = self.pop_int()?;
                let text4 = self.pop_string_lossy()?;
                let text3 = self.pop_string_lossy()?;
                let text2 = self.pop_string_lossy()?;
                let text1 = self.pop_string_lossy()?;
                Value::Int(i32::from(api.show_installer_dialog(
                    &InstallerDialogRequest {
                        text1,
                        text2,
                        text3,
                        text4,
                        mode1,
                        mode2,
                    },
                )))
            }
            (0x80, 0xf2) => {
                let mode = self.pop_int()?;
                let prompt = self.pop_string_lossy()?;
                let uninstall_source = self.pop_string_lossy()?;
                let product = self.pop_string_lossy()?;
                let vendor = self.pop_string_lossy()?;
                let format_string = self.pop_string_lossy()?;
                let flags = self.pop_int()?;
                let destination_pointer = self.pop_ptr()?;
                let source_pointer = self.pop_ptr()?;
                let metadata_value = self.pop_string_lossy()?;
                let count = self.pop_int()?.max(0) as usize;
                let required_pointer = self.pop_ptr()?;
                let optional_pointer = self.pop_ptr()?;
                let root = self.pop_string_lossy()?;
                let optional_directories = if optional_pointer == 0 {
                    Vec::new()
                } else {
                    self.read_zero_terminated_string_pointer_list(optional_pointer, 4096)?
                };
                let required_directories = if required_pointer == 0 {
                    Vec::new()
                } else {
                    self.read_zero_terminated_string_pointer_list(required_pointer, 4096)?
                };
                let source_files = self.read_counted_string_pointer_list(source_pointer, count)?;
                let destination_files =
                    self.read_counted_string_pointer_list(destination_pointer, count)?;
                Value::Int(i32::from(api.run_installer_workflow(
                    &InstallerWorkflowRequest {
                        root,
                        optional_directories,
                        required_directories,
                        source_files,
                        destination_files,
                        vendor,
                        product,
                        uninstall_source,
                        prompt,
                        mode,
                        flags,
                        metadata: vec![format_string, metadata_value],
                    },
                )))
            }
            (0x80, 0xf3) => {
                let create_desktop = self.pop_int()? != 0;
                let create_program_group = self.pop_int()? != 0;
                let program_group = self.pop_string_lossy()?;
                let secondary_shortcut_name = self.pop_string_lossy()?;
                let secondary_target = self.pop_string_lossy()?;
                let primary_shortcut_name = self.pop_string_lossy()?;
                let primary_target = self.pop_string_lossy()?;
                let target_root = self.pop_string_lossy()?;
                Value::Int(i32::from(api.run_shortcut_installer_workflow(
                    &ShortcutInstallerWorkflowRequest {
                        secondary_shortcut_name,
                        program_group,
                        target_root,
                        primary_target,
                        primary_shortcut_name,
                        secondary_target,
                        create_program_group,
                        create_desktop,
                    },
                )))
            }
            (0x80, 0xf4) => {
                let array = self.pop_ptr()?;
                let root = self.pop_string_lossy()?;
                let exclusions = self.read_zero_terminated_string_pointer_list(array, 4096)?;
                Value::Int(i32::from(
                    api.remove_uninstall_listed_files(&root, &exclusions),
                ))
            }
            (0x80, 0xf5) => {
                let array = self.pop_ptr()?;
                let root = self.pop_string_lossy()?;
                let entries = self.read_zero_terminated_string_pointer_list(array, 4096)?;
                Value::Int(i32::from(
                    api.append_uninstall_list_entries(&root, &entries),
                ))
            }
            (0x80, 0xf6) => {
                let remove_group = self.pop_int()? != 0;
                let program_group = self.pop_string_lossy()?;
                let secondary_file = self.pop_string_lossy()?;
                let file_name = self.pop_string_lossy()?;
                api.remove_installer_shortcuts(
                    &file_name,
                    &program_group,
                    &secondary_file,
                    remove_group,
                );
                Value::None
            }
            (0x80, 0xf7) => {
                let target = self.pop_string_lossy()?;
                let shortcut_name = self.pop_string_lossy()?;
                let program_group = self.pop_string_lossy()?;
                let group = (!program_group.is_empty()).then_some(program_group.as_str());
                Value::Int(i32::from(api.create_shortcut(
                    group,
                    &shortcut_name,
                    &target,
                )))
            }
            (0x80, 0xf8) => {
                let product = self.pop_string_lossy()?;
                let vendor = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                if let Some(path) = api.read_installed_folder(&vendor, &product) {
                    self.write_c_string(output, &path)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x80, 0xf9) => {
                let product = self.pop_string_lossy()?;
                let vendor = self.pop_string_lossy()?;
                Value::Int(i32::from(
                    api.delete_installed_registry_key(&vendor, &product),
                ))
            }
            (0x80, 0xfa) => {
                let file_name = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                let Some(windows_directory) = api.special_folder_path(0) else {
                    return Ok(Some(Value::Int(0)));
                };
                // sub_472000: "%s\\%s" of the Windows directory and the name;
                // the whole file is read into the output, and only a file
                // ending in "\\" NUL has that backslash cleared and gives 1.
                let path = format!("{windows_directory}\\{file_name}");
                let Some(mut bytes) = api.read_user_file_bytes(&path) else {
                    return Ok(Some(Value::Int(0)));
                };
                let terminated = bytes.len() >= 2
                    && bytes[bytes.len() - 2] == b'\\'
                    && bytes[bytes.len() - 1] == 0;
                if terminated {
                    let trailing_slash = bytes.len() - 2;
                    bytes[trailing_slash] = 0;
                }
                let range = self.resolve_write_range(output, bytes.len())?;
                self.memory[range].copy_from_slice(&bytes);
                self.clear_shadow_values(output, bytes.len());
                Value::Int(i32::from(terminated))
            }
            (0x80, 0xfb) => {
                let output = self.pop_ptr()?;
                if let Some(path) = api.special_folder_path(0) {
                    self.write_c_string(output, &path)?;
                }
                Value::None
            }
            (0x80, 0xfc) => {
                let open_command = self.pop_string_lossy()?;
                let icon = self.pop_string_lossy()?;
                let description = self.pop_string_lossy()?;
                let class_name = self.pop_string_lossy()?;
                let extension = self.pop_string_lossy()?;
                Value::Int(i32::from(api.register_file_association(
                    &extension,
                    &class_name,
                    &description,
                    &icon,
                    &open_command,
                )))
            }
            (0x80, 0xfd) => Value::Int(api.launcher_mode()),
            (0x80, 0xfe) => Value::Int(1),
            _ => return Ok(None),
        };
        Ok(Some(result))
    }

    fn try_builtin_user(&mut self, group: u8, id: u16) -> VmResult<()> {
        match (group, id) {
            (0xb0, 0x02) => {}
            (0xb0, 0x06) => {}
            (0xb0, 0x03) => {
                let _arg2 = self.pop_value()?;
                let _arg1 = self.pop_value()?;
            }
            (0xb0, 0x05) => {
                let _arg = self.pop_value()?;
            }
            (0xb0, 0x80) => {
                let _message = self.pop_string_lossy()?;
            }
            (0xb0, 0xc7) => {
                let _font_name = self.pop_string_lossy()?;
                let _slot = self.pop_int()?;
            }
            (0xb0, 0xc1) => {
                let _font = self.pop_int()?;
                let _text = self.pop_string_lossy()?;
                self.push_value(Value::Int(1));
            }
            (0xc0, 0x00) => {
                let _height = self.pop_value()?;
                let _width = self.pop_value()?;
            }
            (0xc0, 0x01) => {
                let _target = self.pop_value()?;
            }
            (0xc0, 0x04) => {
                let _enabled = self.pop_value()?;
                let _target = self.pop_value()?;
            }
            (0xc0, 0x05) => {
                for _ in 0..6 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x09) => {
                let _interval = self.pop_value()?;
                let _target = self.pop_value()?;
            }
            (0xc0, 0x0a) => {
                let _capacity = self.pop_value()?;
                let _target = self.pop_value()?;
            }
            (0xc0, 0x0b) => {
                for _ in 0..10 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x0c) => {
                let _interval = self.pop_value()?;
                let _target = self.pop_value()?;
            }
            (0xc0, 0x0d) => {
                let _duration = self.pop_value()?;
                let _target = self.pop_value()?;
            }
            (0xc0, 0x0f) => {
                let _target = self.pop_value()?;
            }
            (0xc0, 0x18) => {
                for _ in 0..7 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x1f) => {
                let _arg = self.pop_value()?;
            }
            (0xc0, 0x28) => {
                for _ in 0..4 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x29) => {
                for _ in 0..16 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x2d) => {
                for _ in 0..18 {
                    let _arg = self.pop_value()?;
                }
            }
            (0xc0, 0x41) => {
                let _target = self.pop_value()?;
            }
            _ => {}
        }
        Ok(())
    }

    fn normalize_sys_string_args(&mut self, group: u8, id: u16) -> VmResult<()> {
        let positions_from_top: &[usize] = match (group, id) {
            // Host continuations intercept before the VM-owned dialog paths
            // that would otherwise pop/decode these arguments themselves.
            (0x80, 0x3b) => &[1, 2, 3, 4],
            (0x80, 0x3c) => &[0, 1, 2],
            (0x80, 0x3f) => &[1, 2, 3],
            (0x80, 0xf0) => &[0, 7],
            (0x80, 0xf1) => &[2, 3, 4, 5],
            (0x81, 0x38) => &[1, 2],
            (0x81, 0x3a) => &[1],
            (0x81, 0x3b) => &[1, 2],
            (0x80, 0x34) | (0x80, 0x35) | (0x80, 0x40) => &[0, 1],
            (0x80, 0x28)
            | (0x80, 0x29)
            | (0x80, 0x2a)
            | (0x80, 0x2c)
            | (0x80, 0x66)
            | (0x80, 0xe3) => &[0],
            (0x80, 0x2d) => &[1],
            (0x80, 0x2f) => &[0, 1],
            (0x80, 0x44) => &[3, 4],
            (0x80, 0xdc) => &[0],
            _ => return Ok(()),
        };
        for &from_top in positions_from_top {
            let Some(operand) = self.operand_from_top(from_top) else {
                continue;
            };
            let text = self.value_as_string_lossy(self.operand_value(operand))?;
            self.replace_operand(operand, Value::Str(text));
        }
        Ok(())
    }

    fn normalize_sound_string_args(&mut self, group: u8, id: u16) -> VmResult<()> {
        let positions_from_top: &[usize] = match (group, id) {
            (0xa0, 0x11) => &[2, 3],
            // funcs_487CFE[0x12] -> sub_487280 converts source arguments
            // 2/3/4 through sub_48DF50. They are archive, diagnostic name,
            // and the actual resource name in script order.
            (0xa0, 0x12) => &[3, 4, 5],
            (0xa0, 0x10) => &[1],
            (0xa0, 0x23) => &[2, 3],
            (0xa0, 0x27) => &[3, 4],
            (0xa0, 0xC0) => &[0],
            (0xa0, 0x20) => &[0, 1],
            (0xa0, 0x21) => &[2, 3],
            _ => return Ok(()),
        };
        for &from_top in positions_from_top {
            let Some(operand) = self.operand_from_top(from_top) else {
                continue;
            };
            if std::env::var_os("TRACE_SOUND_ARGS").is_some() {
                self.trace_sound_arg(group, id, from_top, self.operand_value(operand));
            }
            let text = if matches!(
                (group, id, from_top),
                (0xa0, 0x11, 2) | (0xa0, 0x12, 3 | 4) | (0xa0, 0x21, 2)
            ) {
                self.value_as_sound_file_string(self.operand_value(operand))?
            } else {
                self.value_as_string_lossy(self.operand_value(operand))?
            };
            self.replace_operand(operand, Value::Str(text));
        }
        Ok(())
    }

    fn normalize_user_string_args(&mut self, group: u8, id: u16) -> VmResult<()> {
        // sub_476110 and sub_476560 convert one native argument through
        // sub_48DF50 and immediately dereference the first DWORD. Preserve
        // that memory value rather than replacing it with a host pointer or
        // target string.
        let dword_pointer_from_top = match (group, id) {
            (0xc0, 0x25) => Some(0usize),
            (0xc0, 0x2d) => Some(3usize),
            _ => None,
        };
        if let Some(from_top) = dword_pointer_from_top {
            if let Some(operand) = self.operand_from_top(from_top) {
                let pointer = match self.operand_value(operand) {
                    Value::Int(value) => value as u32,
                    Value::Ptr(pointer) => pointer,
                    _ => 0,
                };
                if pointer != 0 {
                    let value = self.read_int(pointer, 2)? as i32;
                    self.replace_operand(operand, Value::Int(value));
                }
            }
        }
        let positions_from_top: &[usize] = match (group, id) {
            // sub_4783D0 pops height/width/y/x before resolving the title.
            (0xb0, 0x10) => &[4],
            (0xb0, 0x15) | (0xb0, 0x1C) | (0xb0, 0x26) => &[0],
            (0xb0, 0x1A) => &[6],
            (0xb0, 0x80) | (0xb0, 0x83) => &[0],
            (0xb0, 0x81) => &[1],
            (0xb0, 0x82) => &[2],
            // The output buffers remain raw BP pointers because the VM owns
            // the target writes after the host dialog returns.
            (0xb0, 0x84) => &[1, 2],
            (0xb0, 0x85) => &[1, 2, 4, 5, 6],
            (0xb0, 0x86) => &[2, 3],
            (0xb0, 0x87) => &[2, 3, 6, 7, 8],
            (0xb0, 0x8C) => &[0, 1, 2],
            (0xb0, 0xC0) | (0xb0, 0xC2) => &[0],
            (0xb0, 0xC1) => &[1],
            (0xb0, 0xC3) | (0xb0, 0xC6) | (0xb0, 0xC7) => &[0, 1],
            (0xb0, 0xF0) => &[2],
            (0xc0, 0xF0) => &[1, 4],
            _ => return Ok(()),
        };
        for &from_top in positions_from_top {
            let Some(operand) = self.operand_from_top(from_top) else {
                continue;
            };
            let text = self.value_as_string_lossy(self.operand_value(operand))?;
            self.replace_operand(operand, Value::Str(text));
        }
        Ok(())
    }

    fn normalize_graph_string_args(&mut self, group: u8, id: u16) -> VmResult<()> {
        let positions_from_top: &[usize] = match (group, id) {
            // GraphLoadResource has the same (target, archive, resource)
            // string contract as preload; both strings may live in frame slots.
            (0x90, 0x10) => &[0, 1],
            // System90:C0 consumes target, namespace/archive and file in BP
            // order; C4/C5 use one filesystem string; C6/C7 use two cache keys.
            (0x90, 0xC0) => &[0, 1],
            (0x90, 0xC4) => &[1],
            (0x90, 0xC5) => &[3],
            (0x90, 0xC6) => &[2, 3],
            (0x90, 0xC7) => &[1, 2],
            // sub_480160 pops height, width, x/y, then converts the movie
            // resource pointer at the bottom of the five-argument frame.
            (0x90, 0xF0) => &[4],
            // Native sub_47DF50 converts the second pop before looking it up
            // in the target graph object.
            (0x90, 0x89) => &[1],
            (0x90, 0x90) => &[4],
            // sub_4844F0/sub_484650 consume the text pointer at these exact
            // stack positions. sub_484C40 has two independent text inputs.
            (0x91, 0x91) => &[3],
            (0x91, 0x93) => &[4],
            // funcs_48547E[0x9c] -> sub_4848A0 pops 14 arguments.
            // The fourth and sixth script arguments are converted through
            // sub_48DF50; in native top-to-bottom pop order they are 10/8.
            (0x91, 0x9C) => &[8, 10],
            (0x91, 0x9D) => &[9, 11],
            (0x91, 0x9E) => &[0, 1],
            (0x91, 0xF0) => &[2],
            (0x92, 0x1C) => &[6],
            (0x92, 0x1D) => &[7],
            (0x92, 0x1E) => &[5],
            (0x92, 0x1F) => &[0],
            // sub_486B80 pops height/width/two display operands/resource/archive.
            (0x92, 0xF0) => &[4, 5],
            // sub_486C40 pops mode/resource/archive/two output pointers.
            (0x92, 0xF1) => &[1, 2],
            // sub_486D30 pops volume/loop/resource/archive/bitmap. Its third and
            // fourth pops are converted through sub_48DF50 before the graph
            // effect resource is created.
            (0x92, 0xF2) => &[2, 3],
            // Native GraphPreloadResource receives (archive, resource). Both
            // values are script pointers and are popped in reverse order.
            (0x92, 0x14) => &[0, 1],
            // Native sub_4863E0 pops 15 arguments. Its fourteenth pop is
            // converted by sub_48DF50 before constructing CProcDspMsgEx.
            (0x92, 0x90) => &[13],
            // Native sub_486500 pops eleven arguments. The tenth native
            // pop is the formatted text pointer converted by sub_48DF50.
            (0x92, 0x91) => &[9],
            // Native sub_4867D0 converts two arguments through sub_48DF50.
            // In top-to-bottom native pop order they are slots 14 and 17.
            (0x92, 0x9c) => &[14, 17],
            _ => return Ok(()),
        };
        for &from_top in positions_from_top {
            let Some(operand) = self.operand_from_top(from_top) else {
                continue;
            };
            let original = self.operand_value(operand);
            let text_slot = matches!(
                (group, id),
                (0x90, 0x90)
                    | (0x91, 0x91 | 0x93 | 0x9C | 0x9D)
                    | (0x92, 0x1C | 0x1D | 0x1E | 0x1F | 0x90 | 0x91 | 0x9C)
            );
            let encoding = if text_slot {
                self.graph_text_encoding.unwrap_or(encoding_rs::SHIFT_JIS)
            } else {
                encoding_rs::SHIFT_JIS
            };
            // A text slot holds a `char *`, not a descriptor: an integer
            // addressing an empty string is empty text. Searching around it
            // picks up unrelated strings (Wonderful Everyday showed the tail
            // of a sprite name for lines whose text part was empty).
            if text_slot
                && let Value::Int(value) = original
                && value != 0
                && self
                    .read_c_string_bytes(value as u32)
                    .is_ok_and(|bytes| bytes.is_empty())
                && self.shadow_string_at(value as u32).is_none()
            {
                self.replace_operand(operand, Value::Str(String::new()));
                continue;
            }
            // sub_48DF50 turns a script pointer into a native `char *`; it does
            // not look for a string elsewhere. Only untyped integers keep the
            // descriptor fallback.
            // A script may cut a double-byte character in half to make a
            // string fit (usdtwnd's save-slot excerpt loop); the lone lead
            // byte is not damage and must not send a valid pointer into the
            // descriptor fallback.
            let text = match original {
                Value::Ptr(ptr) if ptr != 0 && self.shadow_string_at(ptr).is_some() => {
                    self.shadow_string_at(ptr).unwrap_or_default()
                }
                Value::Ptr(ptr) if ptr != 0 => {
                    let direct = self.value_as_string_with_encoding(original.clone(), encoding)?;
                    let trimmed = direct.trim_matches('\0').trim_end_matches('\u{fffd}');
                    if trimmed.is_empty() || is_plausible_text_payload(trimmed) {
                        trimmed.to_string()
                    } else {
                        self.value_as_text_descriptor_string(original.clone(), encoding)?
                    }
                }
                _ => self.value_as_text_descriptor_string(original.clone(), encoding)?,
            };
            if std::env::var_os("TRACE_RESOURCE_ARGS").is_some() {
                let ptr = match original {
                    Value::Int(value) if value != 0 => Some(value as u32),
                    Value::Ptr(ptr) if ptr != 0 => Some(ptr),
                    _ => None,
                };
                tracing::warn!(
                    program = self.program_name(self.current_program),
                    pc = self.pc,
                    group = format_args!("0x{group:02X}"),
                    id = format_args!("0x{id:02X}"),
                    from_top,
                    value = %value_summary(&original),
                    text,
                    dump = ptr.map(|ptr| self.memory_preview(ptr, 96)).unwrap_or_default(),
                    "TRACE_RESOURCE_ARG"
                );
            }
            // sub_48DF50 converts every pointer argument, so a non-null pointer
            // to an empty string must become an empty string rather than stay a
            // pointer that a renderer would draw as "0x...". Integers keep the
            // plausibility test because they may be plain numbers.
            let non_null_pointer = matches!(original, Value::Ptr(ptr) if ptr != 0);
            if non_null_pointer && !text.starts_with("0x") {
                let cleaned = text.trim_matches('\0').trim_end_matches('\u{fffd}');
                self.replace_operand(operand, Value::Str(cleaned.to_string()));
            } else if is_plausible_text_payload(&text) {
                self.replace_operand(operand, Value::Str(text));
            } else if let Value::Int(value) = original
                && value != 0
                && self
                    .read_c_string_bytes(value as u32)
                    .is_ok_and(|bytes| bytes.is_empty())
            {
                // An integer addressing VM memory that holds an empty string,
                // with no descriptor string around it, is a cleared buffer
                // (scrmsg clears the message window's speaker name between
                // speakers and passes its address, computed with `add`, as
                // an integer). sub_48DF50 converts it to an empty `char *`;
                // left as an integer, the renderer draws it as "0x...".
                self.replace_operand(operand, Value::Str(String::new()));
            }
        }
        Ok(())
    }

    fn trace_sound_arg(&self, group: u8, id: u16, from_top: usize, value: Value) {
        let ptr = match value {
            Value::Int(value) => Some(value as u32),
            Value::Ptr(ptr) => Some(ptr),
            _ => None,
        };
        let direct = self
            .value_as_string_lossy(value.clone())
            .unwrap_or_default();
        let dump = ptr
            .map(|ptr| self.memory_preview(ptr, 96))
            .unwrap_or_default();
        tracing::warn!(
            group = format_args!("0x{group:02X}"),
            id = format_args!("0x{id:02X}"),
            from_top,
            value = ?value,
            direct,
            dump,
            "TRACE_SOUND_ARG"
        );
    }

    fn memory_preview(&self, ptr: u32, size: usize) -> String {
        let addr = self.memory_addr(ptr) as usize;
        let Some(bytes) = self
            .memory
            .get(addr..addr.saturating_add(size).min(self.memory.len()))
        else {
            return String::new();
        };
        bytes
            .chunks(16)
            .enumerate()
            .map(|(row, chunk)| {
                let hex = chunk
                    .iter()
                    .map(|byte| format!("{byte:02X}"))
                    .collect::<Vec<_>>()
                    .join(" ");
                let ascii: String = chunk
                    .iter()
                    .map(|byte| match *byte {
                        0x20..=0x7e => *byte as char,
                        _ => '.',
                    })
                    .collect();
                format!("+{:02X}: {hex:<47} {ascii}", row * 16)
            })
            .collect::<Vec<_>>()
            .join(" | ")
    }

    fn values_equal(&self, left: &Value, right: &Value) -> VmResult<bool> {
        match (left, right) {
            (Value::Str(left), Value::Str(right)) => Ok(left == right),
            (Value::Str(_), Value::Int(0) | Value::Ptr(0) | Value::None)
            | (Value::Int(0) | Value::Ptr(0) | Value::None, Value::Str(_)) => Ok(false),
            (Value::Str(left), Value::Ptr(ptr)) | (Value::Ptr(ptr), Value::Str(left)) => {
                Ok(self.read_c_string(*ptr).ok().as_deref() == Some(left.as_str()))
            }
            (Value::Str(left), Value::Int(ptr)) | (Value::Int(ptr), Value::Str(left)) => {
                Ok(self.read_c_string(*ptr as u32).ok().as_deref() == Some(left.as_str()))
            }
            _ => Ok(left.as_i32() == right.as_i32()),
        }
    }

    fn value_as_string_lossy(&self, value: Value) -> VmResult<String> {
        self.value_as_string_with_encoding(value, encoding_rs::SHIFT_JIS)
    }

    fn value_as_string_with_encoding(
        &self,
        value: Value,
        encoding: &'static encoding_rs::Encoding,
    ) -> VmResult<String> {
        match value {
            Value::Str(text) => Ok(text),
            Value::Ptr(ptr) => self.read_c_string_with_encoding(ptr, encoding),
            Value::Int(value) => match self.read_c_string_with_encoding(value as u32, encoding) {
                _ if self
                    .mem_values
                    .get(&self.value_key(value as u32))
                    .and_then(|stored| match stored {
                        Value::Str(text) => Some(text),
                        _ => None,
                    })
                    .is_some() =>
                {
                    Ok(
                        match self
                            .mem_values
                            .get(&self.value_key(value as u32))
                            .expect("checked above")
                        {
                            Value::Str(text) => text.clone(),
                            _ => String::new(),
                        },
                    )
                }
                Ok(text) if !text.is_empty() => Ok(text),
                _ => Ok(format!("0x{value:08X}")),
            },
            Value::Func { offset, .. } => Ok(format!("0x{offset:08X}")),
            Value::Program(_) | Value::None => Ok(String::new()),
        }
    }

    fn value_as_fixed_bytes(&self, value: Value, size: usize) -> VmResult<Vec<u8>> {
        match value {
            Value::Str(text) => {
                let encoded=if self.native_utf8_enabled() {std::borrow::Cow::Borrowed(text.as_bytes())} else {encoding_rs::SHIFT_JIS.encode(&text).0};
                let mut bytes = Vec::with_capacity(size);
                bytes.extend_from_slice(&encoded);
                bytes.push(0);
                bytes.resize(size, 0);
                bytes.truncate(size);
                Ok(bytes)
            }
            Value::Int(ptr) => {
                let range =
                    self.resolve_range(ptr as u32, size)?;
                Ok(self.memory[range].to_vec())
            }
            Value::Ptr(ptr) => {
                let range = self.resolve_range(ptr, size)?;
                Ok(self.memory[range].to_vec())
            }
            Value::None => Ok(vec![0; size]),
            Value::Func { offset, .. } => {
                let range = self.resolve_range(offset, size)?;
                Ok(self.memory[range].to_vec())
            }
            Value::Program(_) => Err(VmError::Runtime(
                "program value cannot be used as a memory block".into(),
            )),
        }
    }

    fn value_as_text_descriptor_string(
        &self,
        value: Value,
        encoding: &'static encoding_rs::Encoding,
    ) -> VmResult<String> {
        let direct = self.value_as_string_with_encoding(value.clone(), encoding)?;
        if is_plausible_text_payload(&direct) {
            return Ok(direct);
        }
        let ptr = match value {
            Value::Int(value) if value != 0 => value as u32,
            Value::Ptr(ptr) if ptr != 0 => ptr,
            _ => return Ok(direct),
        };
        const DESCRIPTOR_OFFSETS: [i32; 21] = [
            0_i32, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, -4, -8, -12, -16,
        ];
        for offset in DESCRIPTOR_OFFSETS {
            let Some(addr) = ptr.checked_add_signed(offset) else {
                continue;
            };
            if let Some(text) = self.shadow_string_at(addr) {
                self.trace_text_descriptor_hit(ptr, offset, addr, &text, "shadow");
                return Ok(text);
            }
            if let Ok(Value::Ptr(nested)) = self.read_value(addr, 2) {
                if nested != 0 {
                    if let Some(text) = self.shadow_string_at(nested) {
                        self.trace_text_descriptor_hit(ptr, offset, nested, &text, "nested_shadow");
                        return Ok(text);
                    }
                    if let Ok(text) = self.read_c_string_with_encoding(nested, encoding) {
                        if is_plausible_text_payload(&text) {
                            self.trace_text_descriptor_hit(
                                ptr,
                                offset,
                                nested,
                                &text,
                                "nested_cstr",
                            );
                            return Ok(text);
                        }
                    }
                }
            }
            if let Ok(nested) = self.read_int(addr, 2) {
                if nested != 0 {
                    if let Some(text) = self.shadow_string_at(nested) {
                        self.trace_text_descriptor_hit(
                            ptr,
                            offset,
                            nested,
                            &text,
                            "nested_int_shadow",
                        );
                        return Ok(text);
                    }
                    if let Ok(text) = self.read_c_string_with_encoding(nested, encoding) {
                        if is_plausible_text_payload(&text) {
                            self.trace_text_descriptor_hit(
                                ptr,
                                offset,
                                nested,
                                &text,
                                "nested_int_cstr",
                            );
                            return Ok(text);
                        }
                    }
                }
            }
        }
        if is_damaged_text_payload(&direct) {
            return Ok(direct);
        }
        // Descriptor fields take precedence over raw byte scanning. Otherwise
        // an invalid leading conversion such as "&#65533;" can expose a
        // plausible-looking suffix at +4 before a valid nested pointer at +8.
        for offset in DESCRIPTOR_OFFSETS {
            let Some(addr) = ptr.checked_add_signed(offset) else {
                continue;
            };
            if let Ok(text) = self.read_c_string_with_encoding(addr, encoding) {
                if is_plausible_text_payload(&text) {
                    self.trace_text_descriptor_hit(ptr, offset, addr, &text, "cstr");
                    return Ok(text);
                }
            }
        }
        Ok(direct)
    }

    fn shadow_string_at(&self, addr: u32) -> Option<String> {
        self.mem_values
            .get(&self.value_key(addr))
            .and_then(|value| match value {
                Value::Str(text) if is_plausible_text_payload(text) => Some(text.clone()),
                _ => None,
            })
    }

    fn trace_text_descriptor_hit(
        &self,
        ptr: u32,
        offset: i32,
        addr: u32,
        text: &str,
        source: &'static str,
    ) {
        if std::env::var_os("TRACE_TEXT_ARGS").is_some() {
            tracing::debug!(
                ptr = format_args!("0x{ptr:08X}"),
                offset,
                addr = format_args!("0x{addr:08X}"),
                source,
                text,
                "TextDescriptorString"
            );
        }
    }

    fn value_as_sound_file_string(&self, value: Value) -> VmResult<String> {
        let direct = self.value_as_string_lossy(value.clone())?;
        if !direct.starts_with("0x") {
            return Ok(direct);
        }
        let ptr = match value {
            Value::Int(value) => value as u32,
            Value::Ptr(ptr) => ptr,
            _ => return Ok(direct),
        };
        for offset in [0x5c_u32, 0x60, 0x58] {
            let addr = ptr.saturating_add(offset);
            if let Some(Value::Str(text)) = self.mem_values.get(&self.value_key(addr)) {
                if !text.is_empty() {
                    tracing::debug!(
                        ptr = format_args!("0x{ptr:08X}"),
                        offset = format_args!("0x{offset:X}"),
                        text,
                        "SoundDescriptorFileString"
                    );
                    return Ok(text.clone());
                }
            }
            if let Ok(text) = self.read_c_string(addr) {
                if !text.is_empty()
                    && text.len() <= 64
                    && text
                        .chars()
                        .all(|ch| ch.is_ascii_alphanumeric() || ch == '_' || ch == '-')
                {
                    tracing::debug!(
                        ptr = format_args!("0x{ptr:08X}"),
                        offset = format_args!("0x{offset:X}"),
                        text,
                        "SoundDescriptorFileString"
                    );
                    return Ok(text);
                }
            }
        }
        Ok(direct)
    }

    fn value_as_native_string_lossy(&self, value: Value) -> VmResult<String> {
        match value {
            Value::Str(text) => Ok(text),
            Value::Ptr(ptr) => self.read_shadowed_c_string(ptr),
            Value::Int(value) => self.read_shadowed_c_string(value as u32),
            Value::Func { offset, .. } => self.read_shadowed_c_string(offset),
            Value::Program(_) | Value::None => Ok(String::new()),
        }
    }

    fn read_shadowed_c_string(&self, ptr: u32) -> VmResult<String> {
        if let Some(Value::Str(text)) = self.mem_values.get(&self.value_key(ptr)) {
            Ok(text.clone())
        } else {
            self.read_c_string(ptr)
        }
    }

    fn pop_string_lossy(&mut self) -> VmResult<String> {
        let value = self.pop_value()?;
        self.value_as_native_string_lossy(value)
    }

    /// Pop a string operand as the bytes the target would see in its buffer.
    fn pop_native_string_bytes(&mut self) -> VmResult<Vec<u8>> {
        match self.pop_value()? {
            Value::Str(text) => Ok(self.encode_native_string(&text).into_owned()),
            Value::Ptr(ptr) => self.read_shadowed_c_string_bytes(ptr),
            Value::Int(value) => self.read_shadowed_c_string_bytes(value as u32),
            Value::Func { offset, .. } => self.read_shadowed_c_string_bytes(offset),
            Value::Program(_) | Value::None => Ok(Vec::new()),
        }
    }

    /// Shadows of loaded scenario text are decoded as Shift-JIS, which is
    /// lossy for other encodings. Memory stays authoritative whenever the
    /// shadow is just that decoding of it.
    fn read_shadowed_c_string_bytes(&self, ptr: u32) -> VmResult<Vec<u8>> {
        let Some(Value::Str(text)) = self.mem_values.get(&self.value_key(ptr)) else {
            return self.read_c_string_bytes(ptr);
        };
        if let Ok(raw) = self.read_c_string_bytes(ptr) {
            if self
                .read_c_string(ptr)
                .is_ok_and(|decoded| decoded == *text)
            {
                return Ok(raw);
            }
        }
        Ok(self.encode_native_string(text).into_owned())
    }

    fn render_sprintf(&mut self, fmt: &str) -> String {
        let mut output = String::new();
        let mut chars = fmt.chars().peekable();
        while let Some(ch) = chars.next() {
            if ch != '%' {
                output.push(ch);
                continue;
            }
            if chars.peek() == Some(&'%') {
                chars.next();
                output.push('%');
                continue;
            }
            let mut zero_pad = false;
            while matches!(chars.peek(), Some('-' | '+' | ' ' | '#')) {
                chars.next();
            }
            if chars.peek() == Some(&'0') {
                zero_pad = true;
                chars.next();
            }
            let mut width = 0usize;
            if chars.peek() == Some(&'*') {
                width = self.pop_int().unwrap_or_default().max(0) as usize;
                chars.next();
            } else {
                while let Some(digit) = chars.peek().and_then(|ch| ch.to_digit(10)) {
                    width = width.saturating_mul(10).saturating_add(digit as usize);
                    chars.next();
                }
            }
            if chars.peek() == Some(&'.') {
                chars.next();
                if chars.peek() == Some(&'*') {
                    let _precision = self.pop_int().unwrap_or_default();
                    chars.next();
                } else {
                    while chars.peek().and_then(|ch| ch.to_digit(10)).is_some() {
                        chars.next();
                    }
                }
            }
            while matches!(chars.peek(), Some('h' | 'l' | 'j' | 'z' | 't' | 'L')) {
                chars.next();
            }
            let spec = chars.next().unwrap_or('%');
            match spec {
                'd' | 'i' | 'u' | 'x' | 'X' => {
                    let value = self.pop_int().unwrap_or_default();
                    let rendered = if spec == 'x' {
                        format!("{value:x}")
                    } else if spec == 'X' {
                        format!("{value:X}")
                    } else {
                        value.to_string()
                    };
                    if width > rendered.len() {
                        let pad = if zero_pad { '0' } else { ' ' };
                        output.extend(std::iter::repeat_n(pad, width - rendered.len()));
                    }
                    output.push_str(&rendered);
                }
                'c' => {
                    // sub_474720 hands the values to the C runtime's sprintf:
                    // the low byte as one character. A byte that is no
                    // character on its own (a Shift-JIS lead byte) is left out.
                    let byte = [self.pop_int().unwrap_or_default() as u8];
                    let (text, _, malformed) = encoding_rs::SHIFT_JIS.decode(&byte);
                    if !malformed {
                        if width > 1 {
                            output.extend(std::iter::repeat_n(' ', width - 1));
                        }
                        output.push_str(&text);
                    }
                }
                's' => {
                    let value = self.pop_string_lossy().unwrap_or_default();
                    output.push_str(&value);
                }
                other => {
                    output.push('%');
                    output.push(other);
                }
            }
        }
        output
    }

    fn jump_target(&self, program: &BpProgram, target: u32) -> VmResult<usize> {
        program.labels.get(&target).copied().ok_or_else(|| {
            VmError::Runtime(format!("jump target 0x{target:08X} is not an instruction"))
        })
    }

    fn jump_target_index(&self, program_index: usize, target: u32) -> VmResult<usize> {
        let program = self
            .programs
            .get(program_index)
            .ok_or_else(|| VmError::Runtime(format!("program #{program_index} is not loaded")))?;
        self.jump_target(program, target)
    }

    fn note_call(&mut self, kind: &str, group: u8, id: u16) {
        if !self.collect_diagnostics {
            return;
        }
        let label = native_call::display_name(NativeOpcode { group, id });
        *self
            .calls
            .entry(format!("{kind}:0x{group:02X}:0x{id:02X}:{label}"))
            .or_default() += 1;
    }

    fn note_stub(&mut self, kind: &str, group: u8, id: u16) {
        *self
            .stubs
            .entry(format!("{kind}:0x{group:02X}:0x{id:02X}"))
            .or_default() += 1;
    }

    fn push_trace(&mut self, line: String) {
        if self.recent_trace.len() >= 30 {
            self.recent_trace.pop_front();
        }
        self.recent_trace.push_back(line);
    }

    fn program_name(&self, program_index: usize) -> &str {
        self.programs
            .get(program_index)
            .and_then(|program| program.script_name.as_deref())
            .unwrap_or("<anonymous>")
    }

    fn stack_summary(&self, count: usize) -> Vec<String> {
        self.stack
            .iter()
            .rev()
            .take(count)
            .map(value_summary)
            .collect()
    }

    /// sub_48E8C0: a script address to its byte in the backing buffer.
    /// Segment 0 is the global window, 2 the thread's frames, 0x10..0x3F
    /// heap blocks; an unmapped address returns `u32::MAX` (out of range).
    pub(crate) fn memory_addr(&self, ptr: u32) -> u32 {
        if ptr >> 26 == 2 {
            // The data region shares the local window with the code region.
            let offset = ptr & 0x03FF_FFFF;
            return if offset < CODE_WINDOW_OFFSET { LOCAL_MEMORY_BASE + offset } else { u32::MAX };
        }
        if ptr >> 26 == 1 {
            let offset = ptr & 0x03FF_FFFF;
            return if offset < CODE_WINDOW_SIZE {
                LOCAL_MEMORY_BASE + CODE_WINDOW_OFFSET + offset
            } else {
                u32::MAX
            };
        }
        if target_heap::TargetHeap::is_heap_address(ptr) {
            return match self.target_heap.translate(ptr) {
                Some((backing, inside)) => LOCAL_MEMORY_BASE.saturating_add(backing).saturating_add(inside),
                None => u32::MAX,
            };
        }
        // Segment 0: the globals, below the local window.
        if ptr < LOCAL_MEMORY_BASE { ptr } else { u32::MAX }
    }

    /// `memory_addr` for addresses that need no heap lookup (debug watch
    /// lists); heap addresses map to `u32::MAX`.
    fn static_memory_addr(ptr: u32) -> u32 {
        if ptr >> 26 == 2 && ptr & 0x03FF_FFFF < CODE_WINDOW_OFFSET {
            LOCAL_MEMORY_BASE + (ptr & 0x03FF_FFFF)
        } else if ptr >> 26 == 1 && ptr & 0x03FF_FFFF < CODE_WINDOW_SIZE {
            LOCAL_MEMORY_BASE + CODE_WINDOW_OFFSET + (ptr & 0x03FF_FFFF)
        } else if ptr < LOCAL_MEMORY_BASE {
            ptr
        } else {
            u32::MAX
        }
    }

    pub(crate) fn value_key(&self, ptr: u32) -> u32 {
        self.memory_addr(ptr)
    }
}

/// sub_438430 on the native bytes: after a '<' followed by a letter or '/',
/// the closing '>' is searched from two bytes past the current position
/// (not past the '<'), and everything before the '<' is copied. Any other
/// '<' moves the scan one byte. An unclosed tag ends the scan.
fn strip_native_markup_tags(source: &[u8]) -> Vec<u8> {
    let mut output = Vec::with_capacity(source.len());
    let mut position = 0;
    while let Some(relative) = scenario::find_bytes(&source[position..], b"<") {
        let tag = position + relative;
        let next = source.get(tag + 1).copied().unwrap_or(0);
        if next.is_ascii_alphabetic() || next == b'/' {
            let from = position + 2;
            let Some(end) = scenario::find_bytes(&source[from..], b">") else {
                break;
            };
            output.extend_from_slice(&source[position..tag]);
            position = from + end + 1;
        } else {
            output.push(source[position]);
            position += 1;
        }
    }
    output.extend_from_slice(&source[position..]);
    output
}

/// sub_4382A0: "<l>" is searched before "<L>" and "</l>" before "</L>";
/// the scan resumes three bytes past the closing tag. Empty labels are
/// skipped; each payload keeps at most 95 bytes.
fn extract_native_labels(source: &[u8]) -> Vec<Vec<u8>> {
    let find = |from: usize, lower: &[u8], upper: &[u8]| {
        scenario::find_bytes(&source[from..], lower)
            .or_else(|| scenario::find_bytes(&source[from..], upper))
            .map(|relative| from + relative)
    };
    let mut labels = Vec::new();
    let mut position = 0;
    while let Some(open) = find(position, b"<l>", b"<L>") {
        let start = open + 3;
        let Some(end) = find(start, b"</l>", b"</L>") else {
            break;
        };
        if end != start {
            labels.push(source[start..end.min(start + 95)].to_vec());
        }
        position = end + 3;
    }
    labels
}

fn ensure_trailing_separator(path: &str) -> String {
    if path.ends_with(['/', '\\']) {
        path.to_string()
    } else {
        format!("{path}{}", std::path::MAIN_SEPARATOR)
    }
}

fn join_native_path(root: &str, file: &str) -> String {
    let root = root.trim_end_matches(['/', '\\']);
    let file = file.trim_start_matches(['/', '\\']);
    if root.is_empty() {
        file.to_string()
    } else if file.is_empty() {
        root.to_string()
    } else {
        format!("{root}\\{file}")
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum ResourceLoadOrigin {
    LooseFile,
    NamedArchive,
}

fn is_named_archive_argument(archive: &str) -> bool {
    let archive = archive.trim();
    !archive.is_empty()
        && !archive.contains('/')
        && !archive.contains('\\')
        && archive.to_ascii_lowercase().ends_with(".arc")
}

fn trace_u32_env(key: &str) -> Option<u64> {
    let value = std::env::var(key).ok()?;
    value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
        .map(|hex| u64::from_str_radix(hex, 16).ok())
        .unwrap_or_else(|| value.parse().ok())
}

fn native_return_audit_enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("ETHORNELL_NATIVE_RETURN_AUDIT").is_some())
}

fn trace_sound_calls_enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("TRACE_SOUND_CALLS").is_some())
}

fn trace_call_frames_enabled(trace_id: u64) -> bool {
    static CONFIG: OnceLock<(bool, Option<u64>)> = OnceLock::new();
    let (enabled, filter) = CONFIG.get_or_init(|| {
        (
            std::env::var_os("TRACE_CALL_FRAMES").is_some(),
            std::env::var("TRACE_CALL_FRAMES_VM")
                .ok()
                .and_then(|value| value.parse::<u64>().ok()),
        )
    });
    *enabled && filter.is_none_or(|filter| filter == trace_id)
}

fn trace_vm_branches_enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("TRACE_VM_BRANCHES").is_some())
}

fn trace_watch_addresses() -> &'static [u32] {
    static ADDRESSES: OnceLock<Vec<u32>> = OnceLock::new();
    ADDRESSES.get_or_init(|| {
        std::env::var("TRACE_WATCH_ADDR")
            .ok()
            .into_iter()
            .flat_map(|spec| {
                spec.split(',')
                    .filter_map(|part| {
                        let part = part.trim();
                        if part.is_empty() {
                            None
                        } else if let Some(hex) =
                            part.strip_prefix("0x").or_else(|| part.strip_prefix("0X"))
                        {
                            u32::from_str_radix(hex, 16).ok()
                        } else {
                            part.parse::<u32>().ok()
                        }
                    })
                    .map(Vm::static_memory_addr)
                    .collect::<Vec<_>>()
            })
            .collect()
    })
}

fn value_summary(value: &Value) -> String {
    match value {
        Value::Int(value) => format!("Int({value})"),
        Value::Ptr(ptr) => format!("Ptr(0x{ptr:08X})"),
        Value::Str(text) => format!("Str({text:?})"),
        Value::Func {
            program_index,
            offset,
        } => format!("Func(program={program_index}, offset=0x{offset:08X})"),
        Value::Program(program) => {
            let name = program.script_name.as_deref().unwrap_or("<anonymous>");
            format!("Program({name})")
        }
        Value::None => "None".into(),
    }
}

fn is_plausible_text_payload(text: &str) -> bool {
    let trimmed = text.trim_matches('\0');
    if trimmed.is_empty() || trimmed.starts_with("0x") || is_damaged_text_payload(trimmed) {
        return false;
    }
    trimmed
        .chars()
        .any(|ch| !ch.is_control() || ch == '\n' || ch == '\r' || ch == '\t')
}

/// Shift-JIS as the target's glyph routine reads it. sub_42F790 draws 0x7F
/// as U+2014, 0xEF40 as two U+2014 (a dash filling its cell) and nothing for
/// the other codes from 0xEF40 up, which code page 932 leaves unassigned or
/// to user-defined characters. Those are rewritten to the Shift-JIS code
/// that looks the same (U+2015; U+2500, a dash without side bearings) or
/// dropped: the decoder would turn them into replacement characters, which
/// make the whole string implausible text (a message line was drawn as its
/// address).
/// Order and limit enumerated files like sub_467110: a directory's own files
/// first, then each subdirectory in turn with its relative prefix. The limit
/// is checked after each file; a subdirectory gets the remainder, and a
/// remainder of zero means unlimited there, as in the target.
/// MSVC _wsplitpath: drive ("X:"), directory up to the last '\\' or '/',
/// file name, and extension from the last '.' of the remainder.
fn msvc_wsplitpath(path: &str) -> [String; 4] {
    let chars = path.chars().collect::<Vec<_>>();
    let (drive, rest) = if chars.len() >= 2 && chars[1] == ':' {
        (chars[..2].iter().collect::<String>(), &chars[2..])
    } else {
        (String::new(), &chars[..])
    };
    let split = rest.iter().rposition(|&c| c == '\\' || c == '/').map_or(0, |index| index + 1);
    let directory = rest[..split].iter().collect::<String>();
    let leaf = &rest[split..];
    let dot = leaf.iter().rposition(|&c| c == '.').unwrap_or(leaf.len());
    [
        drive,
        directory,
        leaf[..dot].iter().collect(),
        leaf[dot..].iter().collect(),
    ]
}

/// NTFS directory order (what FindFirstFileA/FindNextFileA return): UTF-16
/// code units compared after the volume's simple uppercase mapping.
fn ntfs_order_key(name: &str) -> Vec<u16> {
    name.encode_utf16()
        .map(|unit| {
            char::from_u32(u32::from(unit))
                .and_then(|c| {
                    let mut upper = c.to_uppercase();
                    match (upper.next(), upper.next()) {
                        (Some(u), None) if (u as u32) <= 0xFFFF => Some(u as u32 as u16),
                        _ => None,
                    }
                })
                .unwrap_or(unit)
        })
        .collect()
}

fn target_file_enumeration_order(found: &[String], recursive: bool, max_count: usize) -> Vec<String> {
    fn walk(paths: &[Vec<String>], depth: usize, recursive: bool, max: usize, out: &mut Vec<String>) {
        let mut files = paths
            .iter()
            .filter(|path| path.len() == depth + 1)
            .collect::<Vec<_>>();
        files.sort_by_key(|path| ntfs_order_key(&path[depth]));
        let mut count = 0usize;
        for path in files {
            out.push(path.join("\\"));
            count += 1;
            if max != 0 && count >= max {
                break;
            }
        }
        if !recursive {
            return;
        }
        let mut directories = paths
            .iter()
            .filter(|path| path.len() > depth + 1)
            .map(|path| path[depth].clone())
            .collect::<Vec<_>>();
        directories.sort_by_key(|name| ntfs_order_key(name));
        directories.dedup_by_key(|name| ntfs_order_key(name));
        for directory in directories {
            let nested = paths
                .iter()
                .filter(|path| path.len() > depth + 1 && path[depth].eq_ignore_ascii_case(&directory))
                .cloned()
                .collect::<Vec<_>>();
            let before = out.len();
            // a7 - v23 is unsigned: it wraps once the quirk has passed the limit.
            walk(&nested, depth + 1, true, if max != 0 { max.wrapping_sub(count) } else { 0 }, out);
            count += out.len() - before;
        }
    }
    let paths = found
        .iter()
        .map(|path| path.split(['\\', '/']).map(str::to_owned).collect::<Vec<_>>())
        .collect::<Vec<_>>();
    let mut out = Vec::new();
    walk(&paths, 0, recursive, max_count, &mut out);
    out
}

fn engine_shift_jis_bytes(bytes: &[u8]) -> Cow<'_, [u8]> {
    let mut output: Option<Vec<u8>> = None;
    let mut index = 0;
    while index < bytes.len() {
        let lead = bytes[index];
        let width = if matches!(lead, 0x81..=0x9F | 0xE0..=0xFC) && index + 1 < bytes.len() {
            2
        } else {
            1
        };
        let code = if width == 2 {
            u16::from_be_bytes([lead, bytes[index + 1]])
        } else {
            u16::from(lead)
        };
        let replacement: Option<&[u8]> = match code {
            0x7F => Some(&[0x81, 0x5C]),
            0xEF40 => Some(&[0x84, 0x9F]),
            0xEF41.. => Some(&[]),
            _ => None,
        };
        match (replacement, output.as_mut()) {
            (Some(replacement), Some(output)) => output.extend_from_slice(replacement),
            (Some(replacement), None) => {
                let mut rewritten = bytes[..index].to_vec();
                rewritten.extend_from_slice(replacement);
                output = Some(rewritten);
            }
            (None, Some(output)) => output.extend_from_slice(&bytes[index..index + width]),
            (None, None) => {}
        }
        index += width;
    }
    output.map_or(Cow::Borrowed(bytes), Cow::Owned)
}

fn is_damaged_text_payload(text: &str) -> bool {
    text.contains('\u{fffd}') || text.contains("&#65533;") || text.contains("&#xFFFD;")
}

fn is_sjis_delimiter(c: u16) -> bool {
    matches!(
        c,
        0x002c
            | 0x002e
            | 0x00a4
            | 0x00a1
            | 0x003a
            | 0x003b
            | 0x003f
            | 0x0021
            | 0x00de
            | 0x00df
            | 0x00a5
            | 0x8141
            | 0x8142
            | 0x8143
            | 0x8144
            | 0x8146
            | 0x8147
            | 0x8148
            | 0x8149
            | 0x814a
            | 0x814b
            | 0x815d
            | 0x005d
            | 0x007d
            | 0x0029
            | 0x816a
            | 0x816c
            | 0x816e
            | 0x8170
            | 0x8172
            | 0x8174
            | 0x8176
            | 0x8178
            | 0x817a
            | 0x8165
            | 0x8167
    )
}

fn read_op_u8(instruction: &BpInstruction) -> u8 {
    instruction.raw.get(1).copied().unwrap_or_default()
}

fn read_op_u32(instruction: &BpInstruction) -> u32 {
    match instruction.operands.first() {
        Some(BpOperand::U8(v)) => *v as u32,
        Some(BpOperand::U16(v)) => *v as u32,
        Some(BpOperand::U32(v)) => *v,
        Some(BpOperand::I32(v)) => *v as u32,
        Some(BpOperand::Offset(v)) => *v,
        _ => 0,
    }
}

fn read_op_i32(instruction: &BpInstruction) -> i32 {
    match instruction.operands.first() {
        Some(BpOperand::U8(v)) => *v as i8 as i32,
        Some(BpOperand::U16(v)) => *v as i16 as i32,
        Some(BpOperand::U32(v)) => *v as i32,
        Some(BpOperand::I32(v)) => *v,
        Some(BpOperand::Offset(v)) => *v as i32,
        _ => 0,
    }
}

#[derive(Debug, Default)]
pub struct TraceApi;

impl SysApi for TraceApi {
    fn call_sys(&mut self, call: &mut NativeCallFrame) -> VmResult<Value> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        match (group, id) {
            (0x80, 0x40) => {
                let _file = stack.pop();
                let _archive = stack.pop();
                return Ok(Value::Int(0x2f));
            }
            (0x80, 0x44) => {
                for _ in 0..3 {
                    let _arg = stack.pop();
                }
                let _file = stack.pop();
                let _archive = stack.pop();
                return Ok(Value::Int(0x30));
            }
            (0x80, 0x41) => {
                return Ok(Value::Int(1));
            }
            (0x80, 0x34) => {
                let _file = stack.pop();
                let _archive = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x80, 0x35) => {
                let _file = stack.pop();
                let _archive = stack.pop();
                return Ok(Value::Int(-1));
            }
            (0x80, 0x1b) => {
                let _descriptor = stack.pop();
                let _size = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x08) => {
                stack.push(Value::Int(0));
                stack.push(Value::Int(0));
                return Ok(Value::None);
            }
            (0x80, 0x1f) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
                return Ok(Value::None);
            }
            (0x80, 0x60) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x62) => {
                let _descriptor = stack.pop();
                let _mode = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x64) => {
                let _enabled = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x46) => {
                return Ok(Value::None);
            }
            (0x80, 0x28) | (0x80, 0x2a) => {
                let _path = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x80, 0x31) => {
                for _ in 0..5 {
                    let _ = stack.pop();
                }
                return Ok(Value::Int(0));
            }
            (0x80, 0x13) => {
                return Ok(Value::Int(0));
            }
            (0x80, 0x33) => {
                let _mode = stack.pop();
                let _key = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x36) | (0x80, 0x37) => {
                let _arg = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x3d) => {
                let _kind = stack.pop();
                let _ptr = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x80) => {
                let _arg = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x80, 0x81) => {
                return Ok(Value::None);
            }
            (0x80, 0x82) | (0x80, 0x83) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x84) => {
                let _value = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x80, 0x98) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x9d) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x99) => {
                let _handle = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xa8) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xa1) => {
                let _arg = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xac) => {
                let _descriptor = stack.pop();
                let _count = stack.pop();
                let _object = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xd0) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x80, 0xd2) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xda) => {
                let _arg3 = stack.pop();
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xc5) => {
                let _dst = stack.pop();
                let _src = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x80, 0x5a) => {
                return Ok(Value::None);
            }
            (0x80, 0x61) => {
                return Ok(Value::Int(0));
            }
            (0x80, 0x50) => {
                let _enabled = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0x58)
            | (0x80, 0x67)
            | (0x80, 0x68)
            | (0x80, 0x70)
            | (0x80, 0x74)
            | (0x80, 0xc1)
            | (0x80, 0xaf) => {
                let _arg = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xe8) => {
                let _ptr = stack.pop();
                return Ok(Value::None);
            }
            (0x80, 0xfd) => {
                return Ok(Value::Int(0));
            }
            _ => {}
        }
        Ok(Value::None)
    }
}

impl GraphApi for TraceApi {
    fn call_graph(&mut self, call: &mut NativeCallFrame) -> VmResult<Value> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        match (group, id) {
            (0x90, 0x06) => {
                let _y = stack.pop();
                let _x = stack.pop();
            }
            (0x90, 0x0c) | (0x90, 0x4c) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0x05) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0x13) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0x16) => {
                let _source = stack.pop();
                let _target = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x90, 0x17) => {
                let _mode = stack.pop();
                let value = stack.pop().unwrap_or(Value::Int(0));
                return Ok(Value::Int(i32::from(value.as_i32() != 0)));
            }
            (0x90, 0x11) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x10) => {
                for _ in 0..3 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x1f) => {
                // funcs_48065E[0x1f] (sub_47A590) pops exactly six values.
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x18) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x1e) => {
                for _ in 0..8 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x20) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x21) => {
                for _ in 0..9 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x22) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x23) => {
                for _ in 0..10 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x32) => {
                let _value = stack.pop();
                let _object = stack.pop();
            }
            (0x90, 0x3c) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0x3d) => {
                let value = stack.pop().unwrap_or(Value::Int(0));
                return Ok(Value::Int(i32::from(value.as_i32() != 0)));
            }
            (0x90, 0x30) => {
                let _enabled = stack.pop();
                let _timeline = stack.pop();
            }
            (0x90, 0x31) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0x50) => {
                return Ok(Value::Int(1));
            }
            (0x90, 0x53) => {
                for _ in 0..8 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x54) => {
                let _enabled = stack.pop();
                let _node = stack.pop();
            }
            (0x90, 0x55) => {
                let _value = stack.pop();
                let _target = stack.pop();
            }
            (0x90, 0x56) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x58) => {
                for _ in 0..9 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x5a) => {
                for _ in 0..10 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x5c) => {
                for _ in 0..17 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x5d) => {
                for _ in 0..12 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x60) => {
                return Ok(Value::Int(2));
            }
            (0x90, 0x61) => {
                let _object = stack.pop();
            }
            (0x90, 0x64) => {
                let _enabled = stack.pop();
                let _object = stack.pop();
            }
            (0x90, 0x65) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x66) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x80) => {
                let _height = stack.pop();
                let _width = stack.pop();
                return Ok(Value::Int(3));
            }
            (0x90, 0x83) => {
                let _surface = stack.pop();
                let _buffer = stack.pop();
            }
            (0x90, 0x84) => {
                let _enabled = stack.pop();
                let _surface = stack.pop();
            }
            (0x90, 0x85) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x86) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x87) => {
                let _value = stack.pop();
                let _surface = stack.pop();
            }
            (0x90, 0x88) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x89) => {
                for _ in 0..12 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x90) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0x00)
            | (0x90, 0x01)
            | (0x90, 0x02)
            | (0x90, 0x03)
            | (0x90, 0x07)
            | (0x90, 0x08)
            | (0x90, 0x0d)
            | (0x90, 0x12)
            | (0x91, 0x0d) => {
                let _arg = stack.pop();
            }
            (0x90, 0x94) | (0x90, 0x9c) | (0x90, 0x9f) | (0x90, 0xaf) => {
                let _arg = stack.pop();
            }
            (0x90, 0x96) | (0x90, 0x97) | (0x91, 0x06) => {
                let _y = stack.pop();
                let _x = stack.pop();
            }
            (0x91, 0x89) => {
                let _value = stack.pop();
                let _target = stack.pop();
            }
            (0x91, 0x94) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x91, 0x9b) => {
                for _ in 0..7 {
                    stack.pop();
                }
                return Ok(Value::Int(0));
            }
            (0x90, 0x95) => {
                let _b = stack.pop();
                let _a = stack.pop();
            }
            (0x90, 0xf6) => {
                // Target sub_4803C0 performs exactly three BP pops before
                // selecting synchronous decode or DCProcDecodeBMV.
                for _ in 0..3 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0xb7) => {
                let _state = stack.pop();
                let _surface = stack.pop();
            }
            (0x90, 0xb6) => {
                let _descriptor = stack.pop();
                let _object = stack.pop();
            }
            (0x90, 0xb8) => {
                let _object = stack.pop();
            }
            (0x90, 0xb9) => {
                let _object = stack.pop();
            }
            (0x90, 0xba) => {
                let _descriptor = stack.pop();
                let _object = stack.pop();
            }
            (0x90, 0xbe) => {
                let _dest = stack.pop();
                let _object = stack.pop();
                return Ok(Value::Int(-1));
            }
            (0x90, 0xd0) => {
                let _target = stack.pop();
                return Ok(Value::Int(0xF000_0000u32 as i32));
            }
            (0x90, 0xd1) => {
                let _knob = stack.pop();
            }
            (0x90, 0xd4) => {
                let _enabled = stack.pop();
                let _knob = stack.pop();
            }
            (0x90, 0xd5) | (0x90, 0xd6) | (0x90, 0xd8) | (0x90, 0xd9) => {
                let _y = stack.pop();
                let _x = stack.pop();
                let _knob = stack.pop();
            }
            (0x90, 0xd7) => {
                let _knob = stack.pop();
                stack.push(Value::Int(0));
                stack.push(Value::Int(0));
            }
            (0x90, 0xda) => {
                let _knob = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x90, 0xdb) => {
                return Ok(Value::Int(0));
            }
            (0x90, 0xdc) => {
                let _relative_mode = stack.pop();
                let _knob = stack.pop();
            }
            (0x90, 0xdd) => {
                let _mode = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x90, 0xde) | (0x90, 0xdf) => {
                let _knob = stack.pop();
            }
            (0x90, 0xbc) => {
                let _object = stack.pop();
                let _state_buffer = stack.pop();
            }
            (0x90, 0xbf) => {
                let _object = stack.pop();
                let _event_buffer = stack.pop();
            }
            (0x90, 0xcc) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x90, 0xcd) => {
                for _ in 0..8 {
                    let _arg = stack.pop();
                }
            }
            (0x90, 0xe0) => {
                return Ok(Value::Int(0xF100_0000u32 as i32));
            }
            (0x90, 0xe1) => {
                let _group = stack.pop();
            }
            (0x90, 0xe4) => {
                let _enabled = stack.pop();
                let _group = stack.pop();
            }
            (0x90, 0xe5) => {
                let _priority = stack.pop();
                let _y = stack.pop();
                let _x = stack.pop();
                let _group = stack.pop();
            }
            (0x90, 0xe8) => {
                let _local_y = stack.pop();
                let _local_x = stack.pop();
                let _object = stack.pop();
                let _group = stack.pop();
            }
            (0x90, 0xe9) => {
                let _object = stack.pop();
                let _group = stack.pop();
            }
            (0x91, 0x0e) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x10) | (0x91, 0x13) | (0x91, 0x15) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x11) => {
                let _value = stack.pop();
                let _target = stack.pop();
            }
            (0x91, 0x12) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x16) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x3e) => {
                let _output = stack.pop();
                let _mode = stack.pop();
                let _x = stack.pop();
                let _layer = stack.pop();
                return Ok(Value::None);
            }
            (0x91, 0x40) => {
                for _ in 0..9 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x38) => {
                let _mode = stack.pop();
                let _layer = stack.pop();
                let _buffer = stack.pop();
            }
            (0x91, 0x55) => {
                let _linked = stack.pop();
                let _sprite = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x91, 0x60) => {
                return Ok(Value::Int(0x9100_0000u32 as i32));
            }
            (0x91, 0x61) => {
                let _handle = stack.pop();
            }
            (0x91, 0x64) => {
                let _enabled = stack.pop();
                let _handle = stack.pop();
            }
            (0x91, 0x65) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x66) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x67) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x68) => {
                for _ in 0..9 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x69) => {
                for _ in 0..3 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x70) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(0xA100_0000u32 as i32));
            }
            (0x91, 0x71) => {
                let _handle = stack.pop();
            }
            (0x91, 0x73) => {
                let _mode = stack.pop();
                let _handle = stack.pop();
                let _out = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x91, 0x74) => {
                let _enabled = stack.pop();
                let _handle = stack.pop();
            }
            (0x91, 0x75) | (0x91, 0x76) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x78) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x79) | (0x91, 0x7A) | (0x91, 0x7D) | (0x91, 0x7E) | (0x91, 0x7F) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
                if matches!(id, 0x7E | 0x7F) {
                    return Ok(Value::Int(0));
                }
            }
            (0x91, 0x7B) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x7C) => {
                for _ in 0..3 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x1f) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0x91, 0x1d) => {
                for _ in 0..8 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x33) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x19) => {
                for _ in 0..11 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x98) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x9c) => {
                for _ in 0..14 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x9f) => while stack.pop().is_some() {},
            (0x92, 0x97) => {
                for _ in 0..7 {
                    let _arg = stack.pop();
                }
            }
            (0x92, 0x90) => {
                for _ in 0..15 {
                    let _arg = stack.pop();
                }
            }
            (0x92, 0x88) => {
                let _value = stack.pop();
                let _surface = stack.pop();
            }
            (0x92, 0x14) => {
                for _ in 0..2 {
                    let _arg = stack.pop();
                }
            }
            (0x92, 0x15) => {}
            (0x92, 0x16) => {
                let _resource = stack.pop();
                let _target = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x92, 0x19) => {
                let _target = stack.pop();
            }
            (0x92, 0x91) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0x92, 0x8e) => {
                let _target = stack.pop();
            }
            (0x90, 0x0e) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0x91, 0x9a) => {
                let _value = stack.pop();
                let _property = stack.pop();
            }
            (0x91, 0x8d) => {
                let _surface = stack.pop();
                stack.push(Value::Int(0));
                stack.push(Value::Int(0));
                return Ok(Value::Int(0));
            }
            (0x91, 0x8e) => {
                let _target = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x91, 0xb8) => {
                let _window = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x91, 0xba) => {
                let _descriptor = stack.pop();
                let _object = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x91, 0xbb) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(1));
            }
            (0x91, 0xdb) => return Ok(Value::Int(0)),
            (0x91, 0xf0) => {
                for _ in 0..4 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(2));
            }
            (0x91, 0xf1) => {
                let _bitmap = stack.pop();
                let _duration_out = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x91, 0xf2) => {
                let _bitmap = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x91, 0xf3) => {
                let _paused = stack.pop();
                let _bitmap = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x91, 0xf4) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(2));
            }
            (0x91, 0xf5) => {
                let _bitmap = stack.pop();
                return Ok(Value::Int(2));
            }
            (0x91, 0xf6) => {
                let _bitmap = stack.pop();
                return Ok(Value::Int(1));
            }
            (0x91, 0xf7) => {
                let _bitmap = stack.pop();
                let _position_out = stack.pop();
                return Ok(Value::Int(0));
            }
            (0x92, 0xf1) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(0));
            }
            (0x92, 0xf2) => {
                for _ in 0..13 {
                    let _arg = stack.pop();
                }
                return Ok(Value::Int(0));
            }
            (0x92, 0xf4) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
                return Ok(Value::Int(1));
            }
            _ => {}
        }
        Ok(Value::None)
    }
}

impl SoundApi for TraceApi {
    fn call_sound(&mut self, call: &mut NativeCallFrame) -> VmResult<Value> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        match (group, id) {
            (0xa0, 0x11) => {
                for _ in 0..5 {
                    let _arg = stack.pop();
                }
            }
            (0xa0, 0x14) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0xa0, 0x16) => {
                let _duration = stack.pop();
                let _volume = stack.pop();
                let _channel = stack.pop();
            }
            (0xa0, 0x20) => {
                let _file = stack.pop();
                let _archive = stack.pop();
                let _slot = stack.pop();
            }
            (0xa0, 0x21) => {
                for _ in 0..6 {
                    let _arg = stack.pop();
                }
            }
            (0xa0, 0x22) => {
                let _channel = stack.pop();
            }
            (0xa0, 0x24) => {
                let _fade = stack.pop();
                let _volume = stack.pop();
                let _slot = stack.pop();
                return Ok(Value::Int(0));
            }
            (0xa0, 0x25) => {
                let _channel = stack.pop();
            }
            (0xa0, 0x26) => {
                let _arg2 = stack.pop();
                let _arg1 = stack.pop();
            }
            (0xa0, 0x08) | (0xa0, 0x09) => {
                let _value = stack.pop();
                let _channel = stack.pop();
            }
            _ => {}
        }
        Ok(Value::None)
    }
}

#[cfg(test)]
mod tests {
    use super::{target_find_mbs, target_strreplace};

    #[test]
    fn strreplace_search_is_character_wise_and_never_backtracks() {
        // sub_4959B0 does not retry the mismatching character as a start.
        assert_eq!(target_find_mbs(b"aab", b"aaab"), None);
        assert_eq!(target_find_mbs(b"ab", b"xxab"), Some(2));
        assert_eq!(target_find_mbs(b"", b"abc"), None);
        // A trail byte equal to an ASCII needle byte is not a match start.
        let sjis = encoding_rs::SHIFT_JIS.encode("ア\u{3042}").0.into_owned();
        assert_eq!(target_find_mbs(&sjis[2..], &sjis), Some(2));
    }

    #[test]
    fn strreplace_counts_replacements_and_keeps_the_tail() {
        assert_eq!(
            target_strreplace(b"a-b-c", b"-", b"++"),
            (b"a++b++c".to_vec(), 2)
        );
        assert_eq!(target_strreplace(b"abc", b"x", b"y"), (b"abc".to_vec(), 0));
    }

    use super::{
        ADDRESS_MASK, AUX_MEMORY_SEGMENT_SIZE, GraphApi, GraphIconRecord, LOCAL_MEMORY_BASE,
        MAX_MEMORY_SIZE, NativeCallFrame, NativeOpcode, ResourceLoadOrigin, SoundApi, SysApi,
        System92TextFragmentRecord, TraceApi, UserDialogRequest, UserDialogResponse, Value, Vm,
        VmRunOptions, VmStopReason, empty_loaded_program, ensure_trailing_separator,
        join_native_path, native_call, strip_native_markup_tags, system80_state,
    };
    use ethornell_script::{BpInstruction, BpOpcode, BpOperand, BpProgram};
    use std::sync::Arc;

    #[derive(Default)]
    struct SchedulingApi {
        game_id: Option<String>,
        font_families: Vec<String>,
        host_sys_calls: usize,
        system_events: std::collections::VecDeque<[i32; 3]>,
        bitmap_dimensions: std::collections::BTreeMap<i32, (u32, u32)>,
        bitmap_auxiliary_pairs: std::collections::BTreeMap<i32, [i32; 2]>,
        bitmap_pixels: std::collections::BTreeMap<i32, Vec<u8>>,
        bitmap_pixel: Option<[u8; 4]>,
        color_lut: Option<(i32, [[i32; 2]; 3])>,
        graph_input_object: Option<i32>,
        graph_input_extended: bool,
        graph_window: Option<i32>,
        graph_icon_batch: Option<(i32, Vec<GraphIconRecord>)>,
        graph_surface_controls: Option<(i32, super::GraphInputDescriptor)>,
        graph_input_registered_state: i32,
        graph_input_region_values: Vec<i32>,
        input_class_state: i32,
        input_descriptor_states: std::collections::BTreeMap<i32, i32>,
        input_configuration_reset: Option<i32>,
        input_master_gate: Option<i32>,
        input_latched_state: Option<i32>,
        configured_input_samples: usize,
        registered_input_scopes: Vec<i32>,
        unregistered_input_scopes: Vec<i32>,
        registered_input_descriptors: std::collections::BTreeMap<i32, Vec<i32>>,
        last_input_scope: Option<i32>,
        bgm_state: Option<(i32, i32)>,
        memory_sound: Option<(i32, Vec<u8>, i32, f64, f64)>,
        movie_position: Option<i32>,
        buriko_movie_load: Option<Result<(i32, [i32; 5]), i32>>,
        graph_call_result: Option<i32>,
        graph_object_property: Option<(i32, i32, Result<i32, i32>)>,
        window_valid_region: Option<[i32; 4]>,
        caret_frame_call: Option<(u32, i32, Vec<i32>)>,
        message_animating: bool,
        message_auto_advance: Option<Option<i32>>,
        message_reveals: usize,
        message_finishes: usize,
        message_caret_ticks: Vec<u32>,
        window_message_serial: i32,
        window_messages: std::collections::VecDeque<(i32, i32, i32, i32)>,
        system92_text_output_pair: [i32; 2],
        system92_text_fragment_records: Vec<System92TextFragmentRecord>,
        performance_profiling_enabled: Option<i32>,
        performance_metric: i32,
        presentation_state: i32,
        graphics_capability_record: [u32; 16],
        graphics_memory_metric: i32,
        physical_memory_bytes: (u64, u64),
        fullscreen_hotkeys_enabled: bool,
        fullscreen_hotkeys: Vec<i32>,
        special_folder: Option<String>,
        file_dialog_result: Option<Result<Option<String>, i32>>,
        file_dialog_args: Option<String>,
        resource_dialog_result: Option<Result<Option<String>, i32>>,
        resource_dialog_filters: Vec<(String, String)>,
        browse_folder_result: Option<String>,
        resource_list_result: bool,
        resource_list_request: Option<(String, String)>,
        user_dialog_request: Option<UserDialogRequest>,
        user_dialog_response: Option<UserDialogResponse>,
        modeless_initial: Option<[i32; 9]>,
    }

    impl SysApi for SchedulingApi {
        fn game_id(&self) -> &str {
            self.game_id.as_deref().unwrap_or("Tayutama2TV")
        }

        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            self.host_sys_calls += 1;
            Ok(Value::None)
        }

        fn set_performance_profiling(&mut self, enabled: i32) {
            self.performance_profiling_enabled = Some(enabled);
        }

        fn read_performance_metric(&mut self, _selector: i32) -> i32 {
            self.performance_metric
        }

        fn presentation_state(&mut self) -> i32 {
            self.presentation_state
        }

        fn graphics_capability_record(&mut self) -> [u32; 16] {
            self.graphics_capability_record
        }

        fn graphics_memory_metric(&mut self) -> i32 {
            self.graphics_memory_metric
        }

        fn host_physical_memory_bytes(&mut self) -> (u64, u64) {
            self.physical_memory_bytes
        }

        fn configure_fullscreen_hotkeys(&mut self, enabled: bool, descriptors: &[i32]) {
            self.fullscreen_hotkeys_enabled = enabled;
            if enabled {
                self.fullscreen_hotkeys.clear();
                self.fullscreen_hotkeys.extend_from_slice(descriptors);
            }
        }

        fn special_folder_path(&mut self, _mode: i32) -> Option<String> {
            self.special_folder.clone()
        }

        fn open_file_dialog(
            &mut self,
            initial_dir: &str,
            description: &str,
            extension: &str,
            title: &str,
            mode: i32,
        ) -> Result<Option<String>, i32> {
            self.file_dialog_args = Some(
                [initial_dir, description, extension, title]
                    .map(str::to_string)
                    .join("|")
                    + &format!("|{mode}"),
            );
            self.file_dialog_result.take().unwrap_or(Ok(None))
        }

        fn open_resource_file_dialog(
            &mut self,
            _mode: i32,
            _title: &str,
            _default_name: &str,
            filters: &[(String, String)],
        ) -> Result<Option<String>, i32> {
            self.resource_dialog_filters = filters.to_vec();
            self.resource_dialog_result.take().unwrap_or(Ok(None))
        }

        fn browse_folder(&mut self, _title: &str, _root: i32) -> Option<String> {
            self.browse_folder_result.take()
        }

        fn show_resource_list_dialog(&mut self, title: &str, pattern: &str) -> bool {
            self.resource_list_request = Some((title.to_string(), pattern.to_string()));
            self.resource_list_result
        }

        fn post_queued_event(&mut self, code: i32, parameter: i32) {
            self.system_events.push_back([0, code, parameter]);
        }

        fn poll_queued_event(&mut self) -> Option<[i32; 3]> {
            self.system_events.pop_front()
        }

        fn peek_input_state(&mut self, descriptor: i32) -> i32 {
            self.input_descriptor_states
                .get(&descriptor)
                .copied()
                .unwrap_or_default()
        }

        fn query_input_event_bits(&mut self, scope: i32) -> i32 {
            self.last_input_scope = Some(scope);
            self.input_class_state
        }

        fn reset_input_configuration(&mut self, value: i32) {
            self.input_configuration_reset = Some(value);
        }

        fn sample_configured_input(&mut self) {
            self.configured_input_samples += 1;
        }

        fn query_configured_input_gate(&mut self) -> i32 {
            self.input_class_state
        }

        fn register_input_scope(&mut self, scope: i32) {
            self.registered_input_scopes.push(scope);
        }

        fn query_and_unregister_input_scope(&mut self, scope: i32) {
            self.unregistered_input_scopes.push(scope);
        }

        fn register_input_class_descriptors(&mut self, class_mask: i32, descriptors: &[i32]) {
            self.registered_input_descriptors
                .insert(class_mask, descriptors.to_vec());
        }

        fn query_input_descriptor_state(&mut self, class_mask: i32) -> i32 {
            self.input_descriptor_states
                .get(&class_mask)
                .copied()
                .unwrap_or_default()
        }

        fn set_input_master_gate(&mut self, value: i32) {
            self.input_master_gate = Some(value);
        }

        fn set_input_latched_state(&mut self, value: i32) {
            self.input_latched_state = Some(value);
        }

        fn input_message_serial(&mut self) -> i32 {
            self.window_message_serial
        }

        fn poll_window_message(
            &mut self,
            message_id: i32,
            registered_after_serial: i32,
        ) -> Option<(i32, i32)> {
            self.window_messages
                .iter()
                .rev()
                .find(|&&(serial, id, _, _)| serial > registered_after_serial && id == message_id)
                .map(|&(_, _, wparam, lparam)| (wparam, lparam))
        }
    }

    impl GraphApi for SchedulingApi {
        fn native_message_is_animating(&self) -> bool {
            self.message_animating
        }

        fn native_message_auto_advance_delay_ms(&self, configured: Option<i32>) -> Option<i32> {
            self.message_auto_advance.unwrap_or(configured)
        }

        fn reveal_native_message(&mut self) -> bool {
            let changed = self.message_animating;
            self.message_animating = false;
            self.message_reveals += usize::from(changed);
            changed
        }

        fn finish_native_message(&mut self) {
            self.message_finishes += 1;
        }

        fn tick_native_message_caret(&mut self, now_tick: u32) {
            self.message_caret_ticks.push(now_tick);
        }

        fn register_graph_color_lut(&mut self, id: i32, points: [[i32; 2]; 3]) -> bool {
            self.color_lut = Some((id, points));
            true
        }

        fn graph_input_object_exists(&self, object: i32) -> bool {
            self.graph_input_object == Some(object)
        }

        fn graph_input_object_is_extended(&self, object: i32) -> bool {
            self.graph_input_object_exists(object) && self.graph_input_extended
        }

        fn graph_window_exists(&self, window: i32) -> bool {
            self.graph_window == Some(window)
        }

        fn draw_graph_icon_batch(&mut self, window: i32, records: &[GraphIconRecord]) -> bool {
            if !self.graph_window_exists(window) {
                return false;
            }
            self.graph_icon_batch = Some((window, records.to_vec()));
            true
        }

        fn configure_graph_surface_controls(
            &mut self,
            surface: i32,
            descriptor: super::GraphInputDescriptor,
        ) {
            self.graph_surface_controls = Some((surface, descriptor));
        }

        fn graph_input_registered_state(&self, object: i32) -> Option<i32> {
            self.graph_input_object_exists(object)
                .then_some(self.graph_input_registered_state)
        }

        fn graph_input_region_values(&self, object: i32) -> Option<Vec<i32>> {
            self.graph_input_object_exists(object)
                .then(|| self.graph_input_region_values.clone())
        }

        fn create_bitmap_from_rgb(
            &mut self,
            bitmap: i32,
            width: i32,
            height: i32,
            _format: i32,
            pixels: &[u8],
        ) -> bool {
            self.bitmap_pixels.insert(bitmap, pixels.to_vec());
            self.bitmap_dimensions.insert(bitmap, (width as u32, height as u32));
            true
        }

        fn read_bitmap_pixels(&mut self, bitmap: i32, capacity: usize) -> Option<Vec<u8>> {
            self.bitmap_pixels
                .get(&bitmap)
                .filter(|pixels| pixels.len() <= capacity)
                .cloned()
        }

        fn system92_text_output_pair(&self) -> [i32; 2] {
            self.system92_text_output_pair
        }

        fn take_system92_text_fragment_records(&mut self) -> Vec<System92TextFragmentRecord> {
            std::mem::take(&mut self.system92_text_fragment_records)
        }

        fn call_graph(&mut self, call: &mut NativeCallFrame) -> super::VmResult<Value> {
            call.args_mut().clear();
            Ok(self.graph_call_result.map_or(Value::None, Value::Int))
        }

        fn query_bitmap_info(&mut self, bitmap: i32) -> Option<super::BitmapInfo> {
            if let Some(&(width, height)) = self.bitmap_dimensions.get(&bitmap) {
                return Some(super::BitmapInfo {
                    row_stride: width.saturating_mul(4),
                    width,
                    height,
                    format: 2,
                    bytes_per_pixel: 4,
                });
            }
            (bitmap == 3792).then_some(super::BitmapInfo {
                row_stride: 512,
                width: 128,
                height: 32,
                format: 2,
                bytes_per_pixel: 4,
            })
        }

        fn read_bitmap_pixel(&mut self, _bitmap: i32, _x: i32, _y: i32) -> Option<[u8; 4]> {
            self.bitmap_pixel
        }

        fn set_bitmap_dimensions(&mut self, bitmap: i32, width: i32, height: i32) -> bool {
            if !(0..0x4000).contains(&bitmap) {
                return false;
            }
            self.bitmap_dimensions
                .insert(bitmap, (width as u32, height as u32));
            true
        }

        fn set_bitmap_auxiliary_pair(&mut self, bitmap: i32, first: i32, second: i32) -> bool {
            if !(0..0x4000).contains(&bitmap) {
                return false;
            }
            self.bitmap_auxiliary_pairs.insert(bitmap, [first, second]);
            true
        }

        fn query_bitmap_auxiliary_pair(&mut self, bitmap: i32) -> Option<[i32; 2]> {
            self.bitmap_auxiliary_pairs.get(&bitmap).copied()
        }

        fn query_graph_window_valid_region(&self, _window: i32) -> Option<[i32; 4]> {
            self.window_valid_region
        }

        fn query_graph91_object_property(
            &self,
            object: i32,
            parameter: i32,
            _input: i32,
        ) -> Result<Vec<i32>, i32> {
            self.graph_object_property
                .as_ref()
                .filter(|(expected_object, expected_parameter, _)| {
                    *expected_object == object && *expected_parameter == parameter
                })
                .map(|(_, _, result)| result.clone().map(|value| vec![value]))
                .unwrap_or(Err(255))
        }

        fn configure_message_caret_frames(
            &mut self,
            table_pointer: u32,
            frame_count: i32,
            frames: &[i32],
        ) -> Result<(), i32> {
            self.caret_frame_call = Some((table_pointer, frame_count, frames.to_vec()));
            Ok(())
        }

        fn query_movie_position(&self, _bitmap: i32) -> Result<i32, i32> {
            self.movie_position.ok_or(4)
        }

        fn load_buriko_movie_resource(
            &mut self,
            _archive: &str,
            _resource: &str,
        ) -> Result<(i32, [i32; 5]), i32> {
            self.buriko_movie_load.take().unwrap_or(Err(4))
        }
    }

    impl SoundApi for SchedulingApi {
        fn user_font_families(&self) -> Vec<String> {
            self.font_families.clone()
        }

        fn register_memory_sound(
            &mut self,
            channel: i32,
            block: &[u8],
            native_start_parameter: i32,
            decode_gain: f64,
            playback_rate: f64,
        ) -> bool {
            self.memory_sound = Some((
                channel,
                block.to_vec(),
                native_start_parameter,
                decode_gain,
                playback_rate,
            ));
            true
        }

        fn query_bgm_state(&self, _channel: i32) -> Option<(i32, i32)> {
            self.bgm_state
        }

        fn show_user_dialog(&mut self, request: UserDialogRequest) -> Option<UserDialogResponse> {
            self.user_dialog_request = Some(request);
            self.user_dialog_response.take()
        }

        fn create_user_modeless_dialog(&mut self, initial: [i32; 9]) -> Option<i32> {
            self.modeless_initial = Some(initial);
            Some(77)
        }

        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    struct MediationApi {
        program: BpProgram,
    }

    impl SysApi for MediationApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn load_program(&mut self, _archive: &str, _file: &str) -> Option<BpProgram> {
            Some(self.program.clone())
        }
    }

    impl GraphApi for MediationApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    impl SoundApi for MediationApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    #[derive(Default)]
    struct ResourceSearchApi {
        deleted: Vec<(String, String)>,
        fallback: Option<bool>,
        loose: Vec<String>,
        composites: Vec<(String, Vec<String>)>,
    }

    impl SysApi for ResourceSearchApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn delete_file(&mut self, root: &str, file: &str) -> bool {
            self.deleted.push((root.into(), file.into()));
            true
        }

        fn directory_exists(&mut self, path: &str) -> bool {
            path == "C:\\save"
        }

        fn set_loose_search_fallback(&mut self, enabled: bool) {
            self.fallback = Some(enabled);
        }

        fn prepend_loose_search_path(&mut self, directory: &str) {
            self.loose.push(directory.into());
        }

        fn register_composite_archive(&mut self, name: &str, members: &[String]) {
            self.composites.push((name.into(), members.to_vec()));
        }

        fn take_dropped_file(&mut self) -> Option<String> {
            Some("C:\\drop\\a.png".into())
        }
    }

    impl GraphApi for ResourceSearchApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    impl SoundApi for ResourceSearchApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    #[test]
    fn file_enumeration_follows_the_find_first_file_walk() {
        let found = ["b.txt", "Sub\\z.txt", "a.txt", "Sub\\Deep\\y.txt", "Sub\\c.txt"]
            .map(String::from);
        assert_eq!(
            super::target_file_enumeration_order(&found, true, 0),
            ["a.txt", "b.txt", "Sub\\c.txt", "Sub\\z.txt", "Sub\\Deep\\y.txt"]
        );
        assert_eq!(super::target_file_enumeration_order(&found, false, 1), ["a.txt"]);
        // a7 - v23 reaches zero for the subdirectory, which then has no limit.
        assert_eq!(
            super::target_file_enumeration_order(&found, true, 2),
            ["a.txt", "b.txt", "Sub\\c.txt", "Sub\\z.txt", "Sub\\Deep\\y.txt"]
        );
        // Inside Sub the limit is 1; Deep then gets 1 - 1 = 0, unlimited again.
        assert_eq!(
            super::target_file_enumeration_order(&found, true, 3),
            ["a.txt", "b.txt", "Sub\\c.txt", "Sub\\Deep\\y.txt"]
        );
    }

    #[test]
    fn enumerated_names_measure_pack_and_fail_on_overflow() {
        let names = ["ab".to_string(), "c".to_string()];
        let mut vm = Vm::new();
        assert_eq!(vm.write_enumerated_names(0, 0, &names).unwrap(), Value::Int(5));
        assert_eq!(vm.write_enumerated_names(0x100, 5, &names).unwrap(), Value::Int(2));
        assert_eq!(&vm.memory[0x100..0x105], b"ab\0c\0");
        assert_eq!(vm.write_enumerated_names(0x200, 4, &names).unwrap(), Value::Int(-1));
        assert_eq!(&vm.memory[0x200..0x203], b"ab\0");
    }

    #[derive(Default)]
    struct ItemGridApi {
        drawn: Vec<(i32, Vec<String>, i32, bool, i32)>,
    }

    impl SysApi for ItemGridApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    impl GraphApi for ItemGridApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn draw_item_selection_grid(
            &mut self,
            window: i32,
            items: &[String],
            columns: i32,
            centered: bool,
            color: i32,
        ) -> bool {
            self.drawn.push((window, items.to_vec(), columns, centered, color));
            window == 0xB000_0001_u32 as i32
        }
    }

    impl SoundApi for ItemGridApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    #[test]
    fn item_grid_reads_the_string_table_and_checks_its_operands() {
        let grid = test_instruction(0, 0x90, "grp1", vec![0x90, 0xa1], Vec::new());
        let window = 0xB000_0001_u32 as i32;
        let mut vm = Vm::new();
        vm.write_c_string(0x200, "Yes").unwrap();
        vm.write_c_string(0x210, "No").unwrap();
        vm.write_int(0x100, 2, 0x200).unwrap();
        vm.write_int(0x104, 2, 0x210).unwrap();
        let operands = |window: i32, count: i32, columns: i32| {
            [
                Value::Int(window),
                Value::Int(count),
                Value::Ptr(0x100),
                Value::Int(columns),
                Value::Int(1),
                Value::Int(0x00ff_00ff),
            ]
        };
        let mut api = ItemGridApi::default();

        vm.stack.extend(operands(window, 2, 2));
        vm.dispatch(&grid, &mut api).unwrap();
        assert!(vm.stack.is_empty());
        assert_eq!(
            api.drawn,
            [(window, vec!["Yes".to_string(), "No".to_string()], 2, true, 0x00ff_00ff)]
        );

        for (window, count, columns) in [(window, 17, 2), (window, 2, 17), (window, 2, 0), (5, 2, 2)] {
            vm.stack.clear();
            vm.stack.extend(operands(window, count, columns));
            assert!(vm.dispatch(&grid, &mut api).is_err(), "{window} {count} {columns}");
        }
    }

    #[test]
    fn sys80_resource_search_selectors_follow_the_target_contracts() {
        let sys = |id: u8| test_instruction(0, 0x80, "sys1", vec![0x80, id], Vec::new());
        let mut vm = Vm::new();
        let mut api = ResourceSearchApi::default();

        vm.stack.extend([Value::Int(0), Value::Str("a.sav".into())]);
        vm.dispatch(&sys(0x33), &mut api).unwrap();
        vm.stack
            .extend([Value::Str("C:\\save".into()), Value::Str("b.sav".into())]);
        vm.dispatch(&sys(0x33), &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(1), Value::Int(1)]);
        assert_eq!(
            api.deleted,
            [
                (String::new(), "a.sav".into()),
                (String::new(), "C:\\save\\b.sav".into())
            ]
        );
        vm.stack.clear();

        vm.stack.push(Value::Str("Graphic\\CVTD".into()));
        vm.dispatch(&sys(0x37), &mut api).unwrap();
        vm.stack.push(Value::Str("Script\\CVTD".into()));
        vm.dispatch(&sys(0x37), &mut api).unwrap();
        vm.stack.push(Value::Int(1));
        vm.dispatch(&sys(0x36), &mut api).unwrap();
        assert!(vm.stack.is_empty());
        assert!(vm.additional_resource_search_enabled);
        assert_eq!(vm.additional_resource_paths, ["Script\\CVTD", "Graphic\\CVTD"]);
        assert_eq!(api.fallback, Some(true));

        let names = [0x200u32, 0x210];
        vm.write_c_string(0x200, "data05001.arc").unwrap();
        vm.write_c_string(0x210, "data03001.arc").unwrap();
        for (index, name) in names.into_iter().enumerate() {
            vm.write_int(0x100 + index as u32 * 4, 2, name).unwrap();
        }
        vm.write_int(0x108, 2, 0).unwrap();
        for expected in [1, 0] {
            vm.stack
                .extend([Value::Str("data05xxx.arc".into()), Value::Ptr(0x100)]);
            vm.dispatch(&sys(0x38), &mut api).unwrap();
            assert_eq!(vm.stack.pop(), Some(Value::Int(expected)));
        }
        let members = vec!["data05001.arc".to_string(), "data03001.arc".to_string()];
        assert_eq!(api.composites, [("data05xxx.arc".to_string(), members.clone())]);
        assert_eq!(vm.composite_archives.get("data05xxx.arc"), Some(&members));

        vm.stack.push(Value::Ptr(0x300));
        vm.dispatch(&sys(0x6d), &mut api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(vm.read_c_string(0x300).unwrap(), "C:\\drop\\a.png");

        vm.stack.push(Value::Str("C:\\missing".into()));
        vm.dispatch(&sys(0x39), &mut api).unwrap();
        vm.stack.push(Value::Str("C:\\save".into()));
        vm.dispatch(&sys(0x39), &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(0), Value::Int(1)]);
        assert_eq!(
            vm.validated_file_root,
            Some(super::ensure_trailing_separator("C:\\save"))
        );
    }

    struct RootFileBytesApi {
        expected_file: String,
        bytes: Vec<u8>,
        primary_root: Option<String>,
    }

    impl SysApi for RootFileBytesApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn load_file_bytes(&mut self, archive: &str, file: &str) -> Option<Vec<u8>> {
            (archive.is_empty() && file == self.expected_file).then(|| self.bytes.clone())
        }

        fn primary_resource_root(&mut self) -> Option<String> {
            self.primary_root.clone()
        }
    }

    impl GraphApi for RootFileBytesApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    impl SoundApi for RootFileBytesApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    struct ArchiveBytesApi {
        expected_archive: String,
        expected_file: String,
        bytes: Vec<u8>,
        calls: Vec<(String, String)>,
    }

    impl SysApi for ArchiveBytesApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn load_file_bytes(&mut self, archive: &str, file: &str) -> Option<Vec<u8>> {
            self.calls.push((archive.to_string(), file.to_string()));
            (archive == self.expected_archive && file == self.expected_file)
                .then(|| self.bytes.clone())
        }
    }

    impl GraphApi for ArchiveBytesApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    impl SoundApi for ArchiveBytesApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }
    }

    struct FileBytesApi {
        bytes: Vec<u8>,
    }

    impl SysApi for FileBytesApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            Ok(Value::None)
        }

        fn load_file_bytes(&mut self, _archive: &str, file: &str) -> Option<Vec<u8>> {
            (file == "sample.bin").then(|| self.bytes.clone())
        }
    }

    fn test_instruction(
        offset: u64,
        code: u8,
        name: &'static str,
        raw: Vec<u8>,
        operands: Vec<BpOperand>,
    ) -> BpInstruction {
        BpInstruction {
            offset,
            opcode: BpOpcode::Known { code, name },
            operands: operands.into(),
            known_call: None,
            raw: raw.into(),
            warning: None,
        }
    }

    fn run_stack_program(instructions: Vec<BpInstruction>) -> (crate::VmRunReport, Vm) {
        let program = BpProgram {
            script_name: Some("debug-opcode-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(instructions),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        let report = vm.run(&program, &mut api, &VmRunOptions::default());
        (report, vm)
    }

    fn push_dword(offset: u64, value: u32) -> BpInstruction {
        test_instruction(
            offset,
            0x02,
            "push_dword",
            vec![0x02],
            vec![BpOperand::U32(value)],
        )
    }

    #[test]
    fn show_number_never_fails_and_pops_one_value() {
        // 0x7A (sub_4750A0) only displays the number.
        let (report, vm) = run_stack_program(vec![
            push_dword(0, 0),
            test_instruction(5, 0x7a, "show_number", vec![0x7a], Vec::new()),
            test_instruction(6, 0x17, "ret", vec![0x17], Vec::new()),
        ]);
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn confirm_pops_flag_and_text_and_pushes_the_answer() {
        let (report, vm) = run_stack_program(vec![
            test_instruction(
                0,
                0x05,
                "push_string",
                vec![0x05],
                vec![BpOperand::String("ok?".into())],
            ),
            push_dword(4, 0),
            test_instruction(9, 0x78, "confirm", vec![0x78], Vec::new()),
            test_instruction(10, 0x17, "ret", vec![0x17], Vec::new()),
        ]);
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn call_and_jmp_reject_a_null_target_like_the_target() {
        for opcode in [(0x16u8, "call"), (0x14, "jmp")] {
            let (report, _vm) = run_stack_program(vec![
                push_dword(0, 0),
                test_instruction(5, opcode.0, opcode.1, vec![opcode.0], Vec::new()),
                test_instruction(6, 0x17, "ret", vec![0x17], Vec::new()),
            ]);
            assert!(
                !matches!(report.stop_reason, VmStopReason::Completed),
                "{} to 0 must not complete",
                opcode.1
            );
        }
    }

    #[test]
    fn ret_with_an_empty_data_stack_ends_the_program() {
        let (report, vm) = run_stack_program(vec![
            test_instruction(0, 0x17, "ret", vec![0x17], Vec::new()),
            push_dword(1, 7),
        ]);
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(vm.stack.is_empty());
    }

    fn install_wait_timing_ex(vm: &mut Vm, duration_ms: i32, input_enabled: i32, input_scope: i32) {
        vm.stack.extend([
            Value::Int(duration_ms),
            Value::Int(input_enabled),
            Value::Int(input_scope),
        ]);
        let instruction = test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x5C], Vec::new());
        let mut api = SchedulingApi::default();
        assert_eq!(
            vm.dispatch_scheduler_opcode(
                &mut api,
                native_call::opcodes::SYS_WAIT_TIMING_EX,
                0,
                &instruction,
                false,
            )
            .unwrap(),
            Some(Value::None)
        );
    }

    fn install_message_procedure(vm: &mut Vm) {
        let opcode = NativeOpcode {
            group: 0x90,
            id: 0x90,
        };
        vm.install_cprocedure(
            super::native_thread::InstalledCProcedure::dsp_msg(
                vm.thread.thread_id(),
                opcode,
                vm.timing.tick_count() as u32,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: false,
                    initial_delay_ms: 0,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: None,
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy: 1,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0,
                    input_forces_completion: false,
                },
            ),
            false,
        );
    }

    #[test]
    fn explicit_yield_ends_exactly_one_scheduler_pass() {
        let program = BpProgram {
            script_name: Some("yield-boundary-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x5F], Vec::new()),
                test_instruction(
                    0x12,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        let options = VmRunOptions {
            max_steps: 100,
            ..Default::default()
        };

        let first = vm.run(&program, &mut api, &options);
        assert_eq!(first.stop_reason, VmStopReason::Yielded);
        assert_eq!(first.steps, 1);
        assert_eq!(first.pc, 1);
        assert!(vm.stack.is_empty());
        assert_eq!(api.host_sys_calls, 0);
        assert_eq!(vm.yields_in_last_pass(), 1);

        let second = vm.run_loaded(&mut api, &options);
        assert_eq!(vm.yields_in_last_pass(), 0);
        assert_eq!(second.stop_reason, VmStopReason::Completed);
        assert_eq!(second.steps, 1);
        assert_eq!(vm.stack, [Value::Int(99)]);
        assert_eq!(api.host_sys_calls, 0);
    }

    #[test]
    fn unknown_dispatch_halts_vm_instead_of_retrying_mutated_instruction() {
        let program = BpProgram {
            script_name: Some("unknown-dispatch-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![test_instruction(
                0x10,
                0x80,
                "sys1",
                vec![0x82, 0xff],
                Vec::new(),
            )]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let options = VmRunOptions {
            max_steps: 100,
            fail_on_stub: true,
            ..Default::default()
        };
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();

        let first = vm.run(&program, &mut api, &options);
        assert_eq!(first.stop_reason, VmStopReason::UnknownDispatch);
        assert!(first.stop_reason.is_fatal());
        assert!(vm.halted);

        let second = vm.run_loaded(&mut api, &options);
        assert_eq!(second.steps, 0);
        assert_eq!(second.pc, first.pc);
    }

    #[test]
    fn wait_thread_timer_without_a_deadline_returns_false_synchronously() {
        let program = BpProgram {
            script_name: Some("sys80-5a-unrecovered-procedure-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x5A], Vec::new()),
                test_instruction(
                    0x12,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let options = VmRunOptions {
            max_steps: 100,
            ..Default::default()
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let first = vm.run(&program, &mut api, &options);
        assert_eq!(first.stop_reason, VmStopReason::Completed);
        assert_eq!(first.steps, 2);
        assert_eq!(first.pc, 2);
        assert_eq!(first.thread.current_procedure_opcode, None);
        assert_eq!(vm.thread.native.current_procedure, 0);
        assert_eq!(vm.stack, [Value::Int(0), Value::Int(99)]);
    }

    #[test]
    fn unrecovered_one_arg_procedure_consumes_arg_without_inventing_deadline() {
        let program = BpProgram {
            script_name: Some("sys80-54-unrecovered-procedure-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_byte",
                    vec![0x01, 7],
                    vec![BpOperand::U8(7)],
                ),
                test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x54], Vec::new()),
                test_instruction(
                    0x14,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let options = VmRunOptions {
            max_steps: 100,
            ..Default::default()
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let first = vm.run(&program, &mut api, &options);
        assert_eq!(first.stop_reason, VmStopReason::WaitingForProcedure);
        assert_eq!(first.steps, 2);
        assert_eq!(first.pc, 2);
        assert!(vm.stack.is_empty());
        assert_eq!(vm.thread.deadline_tick(), 0);
        assert_eq!(
            first.thread.current_procedure_opcode,
            Some(native_call::opcodes::SYS_PROCEDURE_54)
        );

        vm.advance_time_ms(60_000);
        let still_waiting = vm.run_loaded(&mut api, &options);
        assert_eq!(still_waiting.stop_reason, VmStopReason::WaitingForProcedure);
        assert_eq!(still_waiting.steps, 0);
        assert_eq!(still_waiting.pc, 2);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn watchdog_does_not_masquerade_as_a_cooperative_yield() {
        let program = BpProgram {
            script_name: Some("watchdog-boundary-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            // Jump targets are code-region offsets: the loop starts at code
            // offset 2 (file offset 0x12).
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x00, "push_byte", vec![0x00, 0x00], vec![BpOperand::U8(0)]),
                test_instruction(
                    0x12,
                    0x02,
                    "push_dword",
                    vec![0x02, 0x02, 0x00, 0x00, 0x00],
                    vec![BpOperand::U32(2)],
                ),
                test_instruction(0x17, 0x14, "jmp", vec![0x14], Vec::new()),
            ]),
            labels: std::sync::Arc::new([(0x10, 0), (0x12, 1)].into_iter().collect()),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        let options = VmRunOptions {
            max_steps: 4,
            ..Default::default()
        };

        let report = vm.run(&program, &mut api, &options);
        assert_eq!(
            report.stop_reason,
            VmStopReason::WatchdogExceeded,
            "{report:#?}"
        );
        assert_eq!(report.steps, 4);
        assert_eq!(report.pc, 2);
    }

    #[test]
    fn native_move_returns_the_assigned_value_and_inline_copy_writes_payload() {
        let destination = 0x2400u32;
        let program = BpProgram {
            script_name: Some("native-memory-opcodes".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x01,
                    "push_word",
                    vec![0x01, 0x34, 0x12],
                    vec![BpOperand::U16(0x1234)],
                ),
                test_instruction(0x18, 0x09, "move", vec![0x09, 2], vec![BpOperand::U8(2)]),
                test_instruction(
                    0x1a,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination + 4)],
                ),
                test_instruction(
                    0x1f,
                    0x0b,
                    "copy_inline",
                    vec![0x0b, 3, 0xaa, 0xbb, 0xcc],
                    vec![BpOperand::Raw(vec![0xaa, 0xbb, 0xcc])],
                ),
                test_instruction(0x24, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 0x1234);
        assert_eq!(vm.read_int(destination + 4, 0).unwrap(), 0xaa);
        assert_eq!(vm.read_int(destination + 5, 0).unwrap(), 0xbb);
        assert_eq!(vm.read_int(destination + 6, 0).unwrap(), 0xcc);
        assert_eq!(vm.stack, [Value::Int(0x1234)]);
    }

    #[test]
    fn load_sign_extends_target_byte_and_word_widths() {
        let byte_ptr = 0x2200u32;
        let word_ptr = 0x2210u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        vm.write_int(byte_ptr, 0, 0xff).unwrap();
        vm.write_int(word_ptr, 1, 0xffff).unwrap();

        vm.stack.push(Value::Ptr(byte_ptr));
        vm.dispatch(
            &test_instruction(0, 0x08, "load", vec![0x08, 0], vec![BpOperand::U8(0)]),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(-1)));

        vm.stack.push(Value::Ptr(word_ptr));
        vm.dispatch(
            &test_instruction(2, 0x08, "load", vec![0x08, 1], vec![BpOperand::U8(1)]),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(-1)));

        // cnfgwndsub._bp around decoded file offset 0x17F2 uses load(1)
        // on a signed index. For -1 the target bitmap expression is
        // 0x1000 + 0x2C6 + 0x2D - 1 == 0x12F2, not 0x112F2.
        assert_eq!(0x1000_i32 + 0x2c6 + 0x2d - 1, 0x12f2);
    }

    #[test]
    fn string_pointer_arithmetic_exposes_shift_jis_bytes_to_load0() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        vm.stack.extend([Value::Str("A\nB".into()), Value::Int(1)]);
        vm.dispatch(
            &test_instruction(0, 0x20, "add", vec![0x20], Vec::new()),
            &mut api,
        )
        .unwrap();
        vm.dispatch(
            &test_instruction(1, 0x08, "load", vec![0x08, 0], vec![BpOperand::U8(0)]),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.stack.last(), Some(&Value::Int(10)));
        assert_eq!(vm.script_records.len(), 1);

        vm.stack.extend([Value::Str("A\nB".into()), Value::Int(2)]);
        vm.dispatch(
            &test_instruction(2, 0x20, "add", vec![0x20], Vec::new()),
            &mut api,
        )
        .unwrap();
        vm.dispatch(
            &test_instruction(3, 0x08, "load", vec![0x08, 0], vec![BpOperand::U8(0)]),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.stack.last(), Some(&Value::Int(b'B' as i32)));
        assert_eq!(vm.script_records.len(), 1);
    }

    #[test]
    fn sprintf_c_writes_the_low_byte_of_its_value() {
        // Wonderful Everyday's scrmsg._bp prefixes a line that opens with a
        // bracket with an indent code: sprintf(buffer, "%c%s", code, text).
        // Without %c the code was read as the %s pointer and the text was
        // left on the stack.
        let destination = 0x2300u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        let sprintf = test_instruction(0, 0x6f, "sprintf", vec![0x6f], Vec::new());
        vm.stack.extend([
            Value::Str("「Hello」".into()),
            Value::Int(0x107),
            Value::Ptr(destination),
            Value::Str("%c%s".into()),
        ]);

        vm.dispatch(&sprintf, &mut api).unwrap();

        assert_eq!(vm.read_c_string(destination).unwrap(), "\u{7}「Hello」");
        assert!(vm.stack.is_empty());

        // A single-byte Shift-JIS character (half-width katakana), padded.
        vm.stack.extend([
            Value::Int(0xB1),
            Value::Ptr(destination),
            Value::Str("[%2c]".into()),
        ]);

        vm.dispatch(&sprintf, &mut api).unwrap();

        assert_eq!(vm.read_c_string(destination).unwrap(), "[ ｱ]");
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn user_font_enumeration_queries_bytes_and_writes_names_before_allocating_records() {
        // amachoco ipl._bp: B0:C4(0), Alloc(bytes), B0:C4(buffer),
        // then Alloc(40 * count). A boolean result makes count zero.
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            font_families: vec!["ＭＳ ゴシック".into(), "@ＭＳ 明朝".into(), "System".into()],
            ..SchedulingApi::default()
        };
        let enumerate = test_instruction(0x10, 0xb0, "usr1", vec![0xb0, 0xc4], Vec::new());
        let allocate = test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x20], Vec::new());
        let mut expected = Vec::new();
        for name in &api.font_families {
            expected.extend_from_slice(&encoding_rs::SHIFT_JIS.encode(name).0);
            expected.push(0);
        }

        vm.stack.extend([Value::Int(12345), Value::Int(0)]);
        vm.dispatch(&enumerate, &mut api).unwrap();
        assert_eq!(
            vm.stack,
            [Value::Int(12345), Value::Int(expected.len() as i32)]
        );
        // Japanese byte counts must use Shift-JIS, not UTF-8 or characters.
        assert_eq!(expected.len(), 32);
        vm.dispatch(&allocate, &mut api).unwrap();
        let destination = vm.stack.pop().unwrap();
        let pointer = destination.as_i32() as u32;
        vm.write_int(pointer + expected.len() as u32, 0, 0x7f)
            .unwrap();
        vm.stack.push(destination);
        vm.dispatch(&enumerate, &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(12345), Value::Int(3)]);
        let range = vm.resolve_range(pointer, expected.len()).unwrap();
        assert_eq!(&vm.memory[range], expected.as_slice());
        assert_eq!(
            vm.read_int(pointer + expected.len() as u32, 0).unwrap(),
            0x7f
        );
        assert_eq!(vm.read_c_string(pointer).unwrap(), api.font_families[0]);

        let count = vm.stack.pop().unwrap().as_i32();
        vm.stack.push(Value::Int(40 * count));
        vm.dispatch(&allocate, &mut api).unwrap();
        assert_ne!(vm.stack.pop().unwrap().as_i32(), 0);
        assert_eq!(vm.stack, [Value::Int(12345)]);
    }

    #[test]
    fn user_font_enumeration_preserves_empty_results_and_rejects_invalid_output() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        let enumerate = test_instruction(0x10, 0xb0, "usr1", vec![0xb0, 0xc4], Vec::new());
        vm.stack.push(Value::Int(0));
        vm.dispatch(&enumerate, &mut api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));

        let destination = vm.alloc_heap(4);
        vm.write_int(destination, 2, 0x12345678).unwrap();
        vm.stack.push(Value::Ptr(destination));
        vm.dispatch(&enumerate, &mut api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(destination, 2).unwrap(), 0x12345678);

        api.font_families.push("ＭＳ ゴシック".into());
        vm.stack.push(Value::Ptr(0xfc00_0000));
        assert!(vm.dispatch(&enumerate, &mut api).is_err());
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn user_modal_input_dialog_preserves_pointer_and_shift_jis_boundary() {
        let destination = 0x2300u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            user_dialog_response: Some(UserDialogResponse::Input("ABあZ".into())),
            ..SchedulingApi::default()
        };
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("Input title".into()),
            Value::Str("initial".into()),
            Value::Int(4),
        ]);

        vm.dispatch(
            &test_instruction(0x10, 0xb0, "usr1", vec![0xb0, 0x84], Vec::new()),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(vm.read_c_string(destination).unwrap(), "ABあ");
        assert_eq!(
            api.user_dialog_request,
            Some(UserDialogRequest::Input {
                title: "Input title".into(),
                initial: "initial".into(),
                max_bytes: 4,
                numeric: false,
            })
        );
    }

    #[test]
    fn user_modeless_dialog_copies_nine_initial_dwords() {
        let handle_out = 0x2340u32;
        let initial_ptr = 0x2380u32;
        let initial = [10, 20, 30, 40, 50, 0, 1, 1, 0];
        let mut vm = Vm::new();
        for (index, value) in initial.into_iter().enumerate() {
            vm.write_int(initial_ptr + index as u32 * 4, 2, value as u32)
                .unwrap();
        }
        let mut api = SchedulingApi::default();
        vm.stack.extend([
            Value::Ptr(handle_out),
            Value::Int(0),
            Value::Ptr(initial_ptr),
        ]);

        vm.dispatch(
            &test_instruction(0x10, 0xb0, "usr1", vec![0xb0, 0xa0], Vec::new()),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(vm.read_int(handle_out, 2).unwrap(), 77);
        assert_eq!(api.modeless_initial, Some(initial));
    }

    #[test]
    fn sys80_special_folder_and_single_filter_dialog_write_native_buffers() {
        let folder_out = 0x2380u32;
        let file_out = 0x23c0u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            special_folder: Some("/portable/Desktop".into()),
            file_dialog_result: Some(Ok(Some("/portable/save.dat".into()))),
            ..SchedulingApi::default()
        };

        vm.stack.extend([Value::Ptr(folder_out), Value::Int(1)]);
        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x3a], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(vm.read_c_string(folder_out).unwrap(), "/portable/Desktop");

        vm.stack.extend([
            Value::Ptr(file_out),
            Value::Str("Save data".into()),
            Value::Str("dat".into()),
            Value::Str("Choose file".into()),
            Value::Str("/portable".into()),
            Value::Int(1),
        ]);
        vm.dispatch(
            &test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x3b], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
        assert_eq!(vm.read_c_string(file_out).unwrap(), "/portable/save.dat");
        assert_eq!(
            api.file_dialog_args.as_deref(),
            Some("/portable|Save data|dat|Choose file|1")
        );
    }

    #[test]
    fn sys81_multi_filter_folder_and_resource_list_dialog_bridges() {
        let destination = 0x2400u32;
        let extension_table = 0x2500u32;
        let label_table = 0x2520u32;
        let extension0 = 0x2600u32;
        let extension1 = 0x2620u32;
        let label0 = 0x2640u32;
        let label1 = 0x2660u32;
        let mut vm = Vm::new();
        vm.write_c_string(extension0, "sav").unwrap();
        vm.write_c_string(extension1, "dat").unwrap();
        vm.write_c_string(label0, "Save files").unwrap();
        vm.write_c_string(label1, "Data files").unwrap();
        vm.write_int(extension_table, 2, extension0).unwrap();
        vm.write_int(extension_table + 4, 2, extension1).unwrap();
        vm.write_int(label_table, 2, label0).unwrap();
        vm.write_int(label_table + 4, 2, label1).unwrap();
        let mut api = SchedulingApi {
            resource_dialog_result: Some(Ok(Some("/portable/chosen.sav".into()))),
            browse_folder_result: Some("/portable/folder".into()),
            resource_list_result: true,
            ..SchedulingApi::default()
        };

        vm.stack.extend([
            Value::Ptr(destination),
            Value::Int(2),
            Value::Ptr(label_table),
            Value::Ptr(extension_table),
            Value::Str("default.sav".into()),
            Value::Str("Choose resource".into()),
            Value::Int(0),
        ]);
        vm.dispatch(
            &test_instruction(0x10, 0x81, "sys2", vec![0x81, 0x38], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
        assert_eq!(
            vm.read_c_string(destination).unwrap(),
            "/portable/chosen.sav"
        );
        assert_eq!(
            api.resource_dialog_filters,
            vec![
                ("Save files".into(), "sav".into()),
                ("Data files".into(), "dat".into()),
            ]
        );

        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("Choose folder".into()),
            Value::Int(1),
        ]);
        vm.dispatch(
            &test_instruction(0x12, 0x81, "sys2", vec![0x81, 0x3a], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(vm.read_c_string(destination).unwrap(), "/portable/folder");

        vm.stack.extend([
            Value::Ptr(0),
            Value::Str("*.sav".into()),
            Value::Str("Resources".into()),
            Value::Ptr(0),
        ]);
        vm.dispatch(
            &test_instruction(0x14, 0x81, "sys2", vec![0x81, 0x3b], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
        assert_eq!(
            api.resource_list_request,
            Some(("Resources".into(), "*.sav".into()))
        );
    }

    #[test]
    fn sys80_performance_controls_and_metric_pointer_bridge() {
        let destination = 0x2440u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            performance_metric: 12_345,
            ..SchedulingApi::default()
        };

        vm.stack.push(Value::Int(7));
        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x06], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(api.performance_profiling_enabled, Some(7));

        vm.write_value(destination, 2, &Value::Ptr(0x0800_4321))
            .unwrap();
        vm.stack.extend([Value::Ptr(destination), Value::Int(2)]);
        vm.dispatch(
            &test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x07], Vec::new()),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.read_value(destination, 2).unwrap(), Value::Int(12_345));
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn sys80_input_reset_and_accumulator_array_use_vm_owned_abi() {
        let descriptors = 0x2460u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            input_descriptor_states: [(13, 7), (38, 5)].into_iter().collect(),
            ..SchedulingApi::default()
        };

        vm.stack.push(Value::Int(0));
        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x10], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(api.input_configuration_reset, Some(0));
        assert!(vm.stack.is_empty());

        vm.write_int(descriptors, 2, 13).unwrap();
        vm.write_int(descriptors + 4, 2, 38).unwrap();
        vm.write_int(descriptors + 8, 2, 0).unwrap();
        vm.stack.push(Value::Ptr(descriptors));
        vm.dispatch(
            &test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x12], Vec::new()),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.stack, [Value::Int(12)]);
        assert_eq!(api.input_descriptor_states, [(13, 7), (38, 5)].into());
    }

    #[test]
    fn sys80_input_control_family_reaches_the_shared_vm_path() {
        let descriptors = 0x24c0u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            window_message_serial: 44,
            input_class_state: 0x1234,
            input_descriptor_states: [(0x40, 9)].into_iter().collect(),
            ..SchedulingApi::default()
        };

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x13).unwrap(),
            Some(Value::Int(44))
        );
        vm.stack.push(Value::Int(3));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x14).unwrap(),
            Some(Value::None)
        );
        vm.stack.push(Value::Int(5));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x15).unwrap(),
            Some(Value::None)
        );
        assert_eq!(api.input_master_gate, Some(3));
        assert_eq!(api.input_latched_state, Some(5));

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x16).unwrap(),
            Some(Value::None)
        );
        assert_eq!(api.configured_input_samples, 1);
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x17).unwrap(),
            Some(Value::Int(0x1234))
        );

        vm.stack.push(Value::Int(7));
        vm.try_builtin_sys_with_api(&mut api, 0x80, 0x18).unwrap();
        vm.stack.push(Value::Int(8));
        vm.try_builtin_sys_with_api(&mut api, 0x80, 0x19).unwrap();
        vm.stack.push(Value::Int(9));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x1a).unwrap(),
            Some(Value::Int(0x1234))
        );
        assert_eq!(api.registered_input_scopes, [7]);
        assert_eq!(api.unregistered_input_scopes, [8]);
        assert_eq!(api.last_input_scope, Some(9));

        for (index, descriptor) in [11u32, 13, 0].into_iter().enumerate() {
            vm.write_int(descriptors + index as u32 * 4, 2, descriptor)
                .unwrap();
        }
        vm.stack.extend([Value::Int(0x40), Value::Ptr(descriptors)]);
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x1b).unwrap(),
            Some(Value::None)
        );
        assert_eq!(
            api.registered_input_descriptors.get(&0x40),
            Some(&vec![11, 13])
        );
        vm.stack.push(Value::Int(0x40));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x1c).unwrap(),
            Some(Value::Int(9))
        );
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn sys80_presentation_capability_memory_and_physical_memory_contracts() {
        let destination = 0x2480u32;
        let capabilities = std::array::from_fn(|index| 0x1000_0000u32 + index as u32);
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            presentation_state: 777,
            graphics_capability_record: capabilities,
            graphics_memory_metric: 16_777_216,
            physical_memory_bytes: (u64::MAX, 1_234_567_890),
            ..SchedulingApi::default()
        };

        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x09], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(777)));

        vm.stack.push(Value::Ptr(destination));
        vm.dispatch(
            &test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x0a], Vec::new()),
            &mut api,
        )
        .unwrap();
        for (index, expected) in capabilities.into_iter().enumerate() {
            assert_eq!(
                vm.read_int(destination + (index as u32 * 4), 2).unwrap(),
                expected
            );
        }

        vm.dispatch(
            &test_instruction(0x14, 0x80, "sys1", vec![0x80, 0x0b], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(16_777_216)));

        vm.dispatch(
            &test_instruction(0x16, 0x80, "sys1", vec![0x80, 0x0d], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack, [Value::Int(i32::MAX), Value::Int(1_234_567_890)]);
    }

    #[cfg(any(target_os = "windows", all(unix, target_pointer_width = "64")))]
    #[test]
    fn sys80_local_time_writes_systemtime_layout() {
        let destination = 0x24c0u32;
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        vm.stack.push(Value::Ptr(destination));

        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x0c], Vec::new()),
            &mut api,
        )
        .unwrap();

        let fields = std::array::from_fn::<u16, 8, _>(|index| {
            vm.read_int(destination + index as u32 * 2, 1).unwrap() as u16
        });
        assert!(fields[0] >= 1970);
        assert!((1..=12).contains(&fields[1]));
        assert!(fields[2] <= 6);
        assert!((1..=31).contains(&fields[3]));
        assert!(fields[4] <= 23);
        assert!(fields[5] <= 59);
        assert!(fields[6] <= 60);
        assert!(fields[7] <= 999);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn raw_native_write_replaces_a_stale_pointer_shadow() {
        let mut vm = Vm::new();
        let destination = 0x2480;
        vm.write_value(destination, 2, &Value::Ptr(0x0800_4321))
            .unwrap();
        // A stored pointer is the raw cell, as in the target.
        assert_eq!(
            vm.read_value(destination, 2).unwrap(),
            Value::Int(0x0800_4321)
        );

        vm.write_int(destination, 2, 10_466).unwrap();

        assert_eq!(vm.read_value(destination, 2).unwrap(), Value::Int(10_466));
    }

    #[test]
    fn window_valid_region_writes_four_native_dwords() {
        let destination = 0x2580u32;
        let window = 0xB000_0003u32;
        let program = BpProgram {
            script_name: Some("window-valid-region".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(window)],
                ),
                test_instruction(0x1a, 0x90, "grp1", vec![0x90, 0x89], Vec::new()),
                test_instruction(0x1c, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            window_valid_region: Some([11, 22, 333, 444]),
            ..Default::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap() as i32, 11);
        assert_eq!(vm.read_int(destination + 4, 2).unwrap() as i32, 22);
        assert_eq!(vm.read_int(destination + 8, 2).unwrap() as i32, 333);
        assert_eq!(vm.read_int(destination + 12, 2).unwrap() as i32, 444);
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn caret_frame_table_is_copied_from_bp_memory() {
        let table = 0x25c0u32;
        let program = BpProgram {
            script_name: Some("caret-frame-table".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(3)]),
                test_instruction(
                    0x13,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(table)],
                ),
                test_instruction(0x18, 0x90, "grp1", vec![0x90, 0x98], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        vm.write_int(table, 2, 10).unwrap();
        vm.write_int(table + 4, 2, u32::MAX).unwrap();
        vm.write_int(table + 8, 2, 20).unwrap();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(api.caret_frame_call, Some((table, 3, vec![10, -1, 20])));
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn bitmap_query_writes_the_native_six_dword_record() {
        let destination = 0x2600u32;
        let program = BpProgram {
            script_name: Some("bitmap-info-record".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(3792)],
                ),
                test_instruction(0x18, 0x90, "grp1", vec![0x90, 0x16], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 0);
        assert_eq!(vm.read_int(destination + 4, 2).unwrap(), 512);
        assert_eq!(vm.read_int(destination + 8, 2).unwrap(), 128);
        assert_eq!(vm.read_int(destination + 12, 2).unwrap(), 32);
        assert_eq!(vm.read_int(destination + 16, 2).unwrap(), 2);
        assert_eq!(vm.read_int(destination + 20, 2).unwrap(), 4);
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn bitmap_auxiliary_reference_point_round_trips_through_group_92() {
        let destination = 0x2680u32;
        let bitmap = 43u16;
        let program = BpProgram {
            script_name: Some("bitmap-auxiliary-reference-point".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(bitmap)],
                ),
                test_instruction(
                    0x13,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(740)],
                ),
                test_instruction(
                    0x16,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(205)],
                ),
                test_instruction(0x19, 0x92, "grp3", vec![0x92, 0x12], Vec::new()),
                test_instruction(
                    0x1b,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x20,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(bitmap)],
                ),
                test_instruction(0x23, 0x92, "grp3", vec![0x92, 0x16], Vec::new()),
                test_instruction(0x25, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 740);
        assert_eq!(vm.read_int(destination + 4, 2).unwrap(), 205);
        assert_eq!(vm.stack, [Value::Int(1), Value::Int(1)]);
    }

    #[test]
    fn bitmap_pixel_read_writes_native_argb_and_status() {
        let destination = 0x26c0u32;
        let program = BpProgram {
            script_name: Some("bitmap-pixel-read".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(3792)],
                ),
                test_instruction(0x18, 0x00, "push_byte", vec![0x00], vec![BpOperand::U8(2)]),
                test_instruction(0x1a, 0x00, "push_byte", vec![0x00], vec![BpOperand::U8(3)]),
                test_instruction(0x1c, 0x92, "grp3", vec![0x92, 0x17], Vec::new()),
                test_instruction(0x1e, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            bitmap_pixel: Some([0x11, 0x22, 0x33, 0x44]),
            ..SchedulingApi::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 0x4411_2233);
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn graph90_f6_returns_its_status_or_completes_dcprocdecodebmv_after_sys80_53() {
        let instruction = test_instruction(0x10, 0x90, "grp1", vec![0x90, 0xf6], Vec::new());
        let mut api = SchedulingApi {
            graph_call_result: Some(0),
            ..SchedulingApi::default()
        };

        let mut vm = Vm::new();
        vm.stack
            .extend([Value::Int(128), Value::Int(73), Value::Int(1)]);
        vm.dispatch(&instruction, &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert!(vm.thread.current_procedure().is_none());

        let mut vm = Vm::new();
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x53).unwrap(),
            Some(Value::None)
        );
        vm.stack
            .extend([Value::Int(128), Value::Int(73), Value::Int(1)]);
        vm.dispatch(&instruction, &mut api).unwrap();
        assert!(vm.stack.is_empty(), "the status is the procedure's output");
        assert!(!vm.next_binary_or_bmv_async);
        let procedure = vm.thread.current_procedure().expect("DCProcDecodeBMV");
        let super::CProcedure::HostCompleted(completed) = procedure.object else {
            panic!("Graph90:F6 must complete its procedure on the host");
        };
        assert_eq!(completed.target_class_name, "DCProcDecodeBMV");
        assert_eq!(
            (
                completed.terminal_status,
                completed.outputs,
                completed.output_count
            ),
            (0, [0, 0], 1)
        );
    }

    #[test]
    fn graph92_buriko_loader_writes_both_native_output_pointers() {
        let handle_out = 0x26f0u32;
        let metadata_out = 0x2710u32;
        let mut vm = Vm::new();
        vm.stack.extend([
            Value::Ptr(handle_out),
            Value::Ptr(metadata_out),
            Value::Str("movie.arc".into()),
            Value::Str("opening.bmv".into()),
            Value::Int(0),
        ]);
        let instruction = test_instruction(0x10, 0x92, "grp3", vec![0x92, 0xf1], Vec::new());
        let metadata = [640, 360, 30, 1_200, 7];
        let mut api = SchedulingApi {
            buriko_movie_load: Some(Ok((73, metadata))),
            ..SchedulingApi::default()
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.read_int(handle_out, 2).unwrap(), 73);
        for (index, value) in metadata.into_iter().enumerate() {
            assert_eq!(
                vm.read_int(metadata_out + (index * 4) as u32, 2).unwrap() as i32,
                value
            );
        }
        assert!(vm.stack.is_empty());
        let procedure = vm.thread.current_procedure().expect("Graph92:F1 procedure");
        assert_eq!(procedure.object.class_name(), "DCProcLoadBurikoMV");
    }

    #[test]
    fn graph91_object_property_writes_the_vm_owned_output_pointer() {
        let output = 0x2728u32;
        let mut vm = Vm::new();
        vm.stack.extend([
            Value::Ptr(output),
            Value::Int(0x8000_0012u32 as i32),
            Value::Int(7),
        ]);
        let instruction = test_instruction(0x10, 0x91, "grp2", vec![0x91, 0x38], Vec::new());
        let mut api = SchedulingApi {
            graph_object_property: Some((0x8000_0012u32 as i32, 7, Ok(0x1234_5678))),
            ..SchedulingApi::default()
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.read_int(output, 2).unwrap(), 0x1234_5678);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn graph92_buriko_header_mode_installs_the_header_procedure() {
        let mut vm = Vm::new();
        vm.stack.extend([
            Value::Ptr(0x2730),
            Value::Ptr(0x2740),
            Value::Str("movie.arc".into()),
            Value::Str("opening.bmv".into()),
            Value::Int(1),
        ]);
        let instruction = test_instruction(0x10, 0x92, "grp3", vec![0x92, 0xf1], Vec::new());
        let mut api = SchedulingApi {
            buriko_movie_load: Some(Err(2)),
            ..SchedulingApi::default()
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        let procedure = vm
            .thread
            .current_procedure()
            .expect("Graph92:F1 header procedure");
        assert_eq!(procedure.object.class_name(), "DCProcLoadBMVHeader");
    }

    #[test]
    fn movie_position_writes_the_native_output_pointer() {
        let destination = 0x2700u32;
        let program = BpProgram {
            script_name: Some("movie-position".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(3792)],
                ),
                test_instruction(0x18, 0x92, "grp3", vec![0x92, 0xf5], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            movie_position: Some(12_345),
            ..SchedulingApi::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 12_345);
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn color_lut_registration_reads_the_native_descriptor() {
        let descriptor = 0x2740u32;
        let program = BpProgram {
            script_name: Some("color-lut-registration".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(31)],
                ),
                test_instruction(
                    0x13,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(descriptor)],
                ),
                test_instruction(0x18, 0x90, "grp1", vec![0x90, 0xcc], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        let points = [[64_i32, 32_i32], [128, 160], [192, 224]];
        for (channel, point) in points.iter().enumerate() {
            let address = descriptor + (channel * 8) as u32;
            vm.write_int(address, 2, point[0] as u32).unwrap();
            vm.write_int(address + 4, 2, point[1] as u32).unwrap();
        }

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(api.color_lut, Some((31, points)));
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn graph_input_region_query_writes_every_native_value() {
        let destination = 0x27c0u32;
        let object = 77u16;
        let program = BpProgram {
            script_name: Some("graph-input-region-values".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x15,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(object)],
                ),
                test_instruction(0x18, 0x90, "grp1", vec![0x90, 0xbe], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            graph_input_object: Some(i32::from(object)),
            graph_input_region_values: vec![4, 8, 15, 16, 23, 42],
            ..SchedulingApi::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        for (index, value) in [4_u32, 8, 15, 16, 23, 42].into_iter().enumerate() {
            assert_eq!(
                vm.read_int(destination + (index * 4) as u32, 2).unwrap(),
                value
            );
        }
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn graph90_compact_icon_configuration_rejects_the_extended_variant_before_parsing() {
        let object = 77u16;
        let invalid_descriptor = 0x00ff_ff00u32;
        let program = BpProgram {
            script_name: Some("graph90-compact-icon-variant-check".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_word",
                    vec![0x01],
                    vec![BpOperand::U16(object)],
                ),
                test_instruction(
                    0x13,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(invalid_descriptor)],
                ),
                test_instruction(0x18, 0x90, "grp1", vec![0x90, 0xba], Vec::new()),
                test_instruction(0x1a, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            graph_input_object: Some(i32::from(object)),
            graph_input_extended: true,
            ..SchedulingApi::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(4)]);
    }

    #[test]
    fn bitmap_rgb_syscalls_round_trip_through_vm_memory() {
        let source = 0x2800u32;
        let destination = 0x2900u32;
        let written = 0x2a00u32;
        let pixels = [10, 20, 30, 40, 50, 60];
        let program = BpProgram {
            script_name: Some("bitmap-rgb-roundtrip".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(7)]),
                test_instruction(0x13, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(2)]),
                test_instruction(0x16, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(1)]),
                test_instruction(0x19, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(1)]),
                test_instruction(
                    0x1c,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(source)],
                ),
                test_instruction(0x21, 0x90, "grp1", vec![0x90, 0x14], Vec::new()),
                test_instruction(
                    0x23,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(
                    0x28,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(written)],
                ),
                test_instruction(0x2d, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(6)]),
                test_instruction(0x30, 0x01, "push_word", vec![0x01], vec![BpOperand::U16(7)]),
                test_instruction(0x33, 0x90, "grp1", vec![0x90, 0x15], Vec::new()),
                test_instruction(0x35, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        vm.resolve_write_range(source, pixels.len())
            .map(|range| vm.memory[range].copy_from_slice(&pixels))
            .unwrap();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(written, 2).unwrap(), pixels.len() as u32);
        let range = vm.resolve_range(destination, pixels.len()).unwrap();
        assert_eq!(&vm.memory[range], &pixels);
    }

    #[test]
    fn expanding_the_local_base_zeroes_a_reused_frame() {
        let program = BpProgram {
            script_name: Some("zero-reused-frame".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x120)],
                ),
                test_instruction(0x15, 0x11, "store_base", vec![0x11], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        vm.start(&program);
        vm.mem_ptr = 0x100;
        vm.write_value(0x0800_0118, 2, &Value::Ptr(0x1000_1234))
            .unwrap();

        let report = vm.run_loaded(&mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(0x0800_0118, 2).unwrap(), 0);
        assert!(!vm.mem_values.contains_key(&vm.value_key(0x0800_0118)));
    }

    #[test]
    fn native_heap_reuses_and_coalesces_freed_blocks() {
        let mut vm = Vm::new();
        let first = vm.alloc_heap(64);
        let second = vm.alloc_heap(96);
        assert!(vm.free_heap(first));
        assert!(vm.free_heap(second));

        let combined = vm.alloc_heap(160);

        assert_eq!(combined, first);
        assert!(!vm.free_heap(second));
        assert!(vm.free_heap(combined));
    }

    #[test]
    fn native_heap_reuses_a_freed_block_larger_than_a_segment() {
        // Wonderful Everyday's userdata._bp allocates this block for every
        // save and frees it afterwards.
        const SAVE_HISTORY: u32 = (400 + 1) * 88124;
        let mut vm = Vm::new();
        let first = vm.alloc_heap(SAVE_HISTORY);
        assert_ne!(first, 0);
        assert_eq!(first & ADDRESS_MASK, 0);
        let heap_ptr = vm.heap_ptr;
        assert!(vm.free_heap(first));
        // Small blocks don't take the freed large one apart.
        let small = vm.alloc_heap(64);
        assert_ne!(small, first);

        for _ in 0..16 {
            let block = vm.alloc_heap(SAVE_HISTORY);
            assert_eq!(block, first);
            assert!(vm.free_heap(block));
        }
        assert_eq!(vm.heap_ptr, heap_ptr + 64);
        assert!(vm.free_heap(small));
    }

    #[test]
    fn native_heap_moves_large_allocations_to_the_next_auxiliary_segment() {
        let mut vm = Vm::new();
        vm.heap_ptr = AUX_MEMORY_SEGMENT_SIZE - 1024;

        let ptr = vm.alloc_heap(2048);

        // The script sees sub_48E760's address (class 0, slot 0); the
        // backing block still moves to the next auxiliary segment.
        assert_eq!(ptr, 0x4000_0000);
        assert_eq!(
            vm.memory_addr(ptr),
            LOCAL_MEMORY_BASE + AUX_MEMORY_SEGMENT_SIZE
        );
        assert!(vm.free_heap(ptr));
    }

    #[test]
    fn native_markup_strip_matches_sub_438430() {
        let strip = |text: &str| {
            let bytes = encoding_rs::SHIFT_JIS.encode(text).0;
            encoding_rs::SHIFT_JIS.decode(&strip_native_markup_tags(&bytes)).0.into_owned()
        };
        assert_eq!(strip("序章<Ruby>本文</Ruby><1>保持"), "序章本文<1>保持");
        assert_eq!(strip("未閉合<Tag"), "未閉合<Tag");
        // The '>' search starts two bytes past the scan position, so an
        // earlier '>' closes the tag and the bytes after it repeat.
        assert_eq!(strip("ab>c<d>"), "ab>cc");
        // Lowercase "<l>" is found first even after an earlier "<L>".
        assert_eq!(
            super::extract_native_labels(b"<L>a</l><l></l><l>b</L>"),
            vec![b"b".to_vec()]
        );
    }

    #[test]
    fn mediation_program_registration_and_call_preserve_the_native_stack() {
        let mediation = BpProgram {
            script_name: Some("loadbmpdx._bp".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x10, "load_base", vec![0x10], Vec::new()),
                test_instruction(
                    0x11,
                    0x01,
                    "push_word",
                    vec![0x01, 12, 0],
                    vec![BpOperand::U16(12)],
                ),
                test_instruction(0x14, 0x20, "add", vec![0x20], Vec::new()),
                test_instruction(0x15, 0x11, "store_base", vec![0x11], Vec::new()),
                test_instruction(
                    0x16,
                    0x04,
                    "push_base_offset",
                    vec![0x04, 4, 0],
                    vec![BpOperand::U16(4)],
                ),
                test_instruction(
                    0x19,
                    0x0a,
                    "move_arg",
                    vec![0x0a, 2],
                    vec![BpOperand::U8(2)],
                ),
                test_instruction(
                    0x1b,
                    0x04,
                    "push_base_offset",
                    vec![0x04, 8, 0],
                    vec![BpOperand::U16(8)],
                ),
                test_instruction(
                    0x1e,
                    0x0a,
                    "move_arg",
                    vec![0x0a, 2],
                    vec![BpOperand::U8(2)],
                ),
                test_instruction(
                    0x20,
                    0x04,
                    "push_base_offset",
                    vec![0x04, 12, 0],
                    vec![BpOperand::U16(12)],
                ),
                test_instruction(
                    0x23,
                    0x0a,
                    "move_arg",
                    vec![0x0a, 2],
                    vec![BpOperand::U8(2)],
                ),
                test_instruction(
                    0x25,
                    0x04,
                    "push_base_offset",
                    vec![0x04, 12, 0],
                    vec![BpOperand::U16(12)],
                ),
                test_instruction(0x28, 0x08, "load", vec![0x08, 2], vec![BpOperand::U8(2)]),
                test_instruction(0x2a, 0x10, "load_base", vec![0x10], Vec::new()),
                test_instruction(
                    0x2b,
                    0x01,
                    "push_word",
                    vec![0x01, 12, 0],
                    vec![BpOperand::U16(12)],
                ),
                test_instruction(0x2e, 0x21, "sub", vec![0x21], Vec::new()),
                test_instruction(0x2f, 0x11, "store_base", vec![0x11], Vec::new()),
                test_instruction(0x30, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: std::sync::Arc::new(std::iter::once((0x10, 0)).collect()),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let main = BpProgram {
            script_name: Some("mediation-main".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x10, "load_base", vec![0x10], Vec::new()),
                test_instruction(
                    0x11,
                    0x00,
                    "push_byte",
                    vec![0x00, 4],
                    vec![BpOperand::U8(4)],
                ),
                test_instruction(0x13, 0x20, "add", vec![0x20], Vec::new()),
                test_instruction(0x14, 0x11, "store_base", vec![0x11], Vec::new()),
                test_instruction(
                    0x15,
                    0x00,
                    "push_byte",
                    vec![0x00, 0x40],
                    vec![BpOperand::U8(0x40)],
                ),
                test_instruction(
                    0x17,
                    0x05,
                    "push_string",
                    vec![0x05],
                    vec![BpOperand::String("sysprg.arc".into())],
                ),
                test_instruction(
                    0x1a,
                    0x05,
                    "push_string",
                    vec![0x05],
                    vec![BpOperand::String("loadbmpdx._bp".into())],
                ),
                test_instruction(
                    0x1d,
                    0xff,
                    "script_load",
                    vec![0xff, 0xf0],
                    vec![BpOperand::U8(0xf0)],
                ),
                test_instruction(
                    0x1f,
                    0x01,
                    "push_word",
                    vec![0x01, 9, 3],
                    vec![BpOperand::U16(777)],
                ),
                test_instruction(
                    0x22,
                    0x05,
                    "push_string",
                    vec![0x05],
                    vec![BpOperand::String("data02xxx.arc".into())],
                ),
                test_instruction(
                    0x25,
                    0x05,
                    "push_string",
                    vec![0x05],
                    vec![BpOperand::String("bg50d_a".into())],
                ),
                test_instruction(
                    0x28,
                    0xff,
                    "script_call",
                    vec![0xff, 0x40],
                    vec![BpOperand::U8(0x40)],
                ),
                test_instruction(0x2a, 0x10, "load_base", vec![0x10], Vec::new()),
                test_instruction(
                    0x2b,
                    0x00,
                    "push_byte",
                    vec![0x00, 4],
                    vec![BpOperand::U8(4)],
                ),
                test_instruction(0x2d, 0x21, "sub", vec![0x21], Vec::new()),
                test_instruction(0x2e, 0x11, "store_base", vec![0x11], Vec::new()),
                test_instruction(0x2f, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = MediationApi { program: mediation };
        let mut vm = Vm::new();

        let report = vm.run(
            &main,
            &mut api,
            &VmRunOptions {
                collect_diagnostics: true,
                ..Default::default()
            },
        );

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(777)]);
        assert!(
            vm.calls
                .keys()
                .any(|key| key.starts_with("script:0xFF:0x40:"))
        );
    }

    #[test]
    fn graph_text_decodes_gbk_memory_before_validating_the_payload() {
        let mut vm = Vm::new();
        vm.set_graph_text_encoding(encoding_rs::GBK);
        let pointer = vm.alloc_heap(64) + 7;
        // GBK bytes can be invalid Shift-JIS or valid but unrelated halfwidth
        // characters. Both require the selected code page, not error fallback.
        for (group, id, count, from_top, bytes, expected) in [
            (
                0x92,
                0x90,
                15,
                13,
                &b"\xa1\xa1\xcf\xf2\xc8\xd5\xbf\xfb\xa1\xa3"[..],
                "　向日葵。",
            ),
            (0x92, 0x1c, 10, 6, &b"\xd3\xbd"[..], "咏"),
            (0x90, 0x90, 6, 4, &b"\xeb\xca\xb0\xd7"[..], "胧白"),
        ] {
            let start = vm.memory_addr(pointer) as usize;
            vm.memory
                .resize(vm.memory.len().max(start + bytes.len() + 1), 0);
            vm.memory[start..start + bytes.len()].copy_from_slice(bytes);
            vm.memory[start + bytes.len()] = 0;
            vm.stack = vec![Value::Int(0); count];
            let index = count - 1 - from_top;
            vm.stack[index] = Value::Int(pointer as i32);

            vm.normalize_graph_string_args(group, id).unwrap();

            assert_eq!(vm.stack[index], Value::Str(expected.into()));
            assert_eq!(&vm.memory[start..start + bytes.len()], bytes);
        }
    }

    #[test]
    fn graph_text_code_page_preserves_system_resource_names_and_unicode_values() {
        let mut vm = Vm::new();
        vm.set_graph_text_encoding(encoding_rs::GBK);
        vm.write_c_string(0x3000, "日本語").unwrap();
        vm.stack = vec![
            Value::Int(1),
            Value::Str("data.arc".into()),
            Value::Ptr(0x3000),
        ];
        vm.normalize_graph_string_args(0x90, 0x10).unwrap();
        assert_eq!(vm.stack[2], Value::Str("日本語".into()));

        vm.stack = vec![Value::Str("已解码的文字".into())];
        vm.normalize_graph_string_args(0x92, 0x1f).unwrap();
        assert_eq!(vm.stack[0], Value::Str("已解码的文字".into()));
    }

    #[test]
    fn graph_preload_normalizes_both_native_string_arguments() {
        let mut vm = Vm::new();
        vm.write_c_string(0x3000, "data02xxx.arc").unwrap();
        vm.write_c_string(0x3100, "bg50d_a").unwrap();
        vm.stack.extend([Value::Ptr(0x3000), Value::Ptr(0x3100)]);

        vm.normalize_graph_string_args(0x92, 0x14).unwrap();

        assert_eq!(
            vm.stack,
            [
                Value::Str("data02xxx.arc".into()),
                Value::Str("bg50d_a".into())
            ]
        );
    }

    #[test]
    fn graph_load_resolves_dynamic_strings_stored_in_frame_slots() {
        let mut vm = Vm::new();
        let local_name = 0x0800_3000;
        vm.write_value(local_name, 2, &Value::Str("bg50d_a".into()))
            .unwrap();
        vm.stack.extend([
            Value::Int(43),
            Value::Str("data02xxx.arc".into()),
            Value::Ptr(local_name),
        ]);

        vm.normalize_graph_string_args(0x90, 0x10).unwrap();

        assert_eq!(
            vm.stack,
            [
                Value::Int(43),
                Value::Str("data02xxx.arc".into()),
                Value::Str("bg50d_a".into())
            ]
        );
    }

    #[test]
    fn graph_load_skips_a_damaged_direct_string_for_nested_descriptor_text() {
        let mut vm = Vm::new();
        let descriptor = 0x0800_3000;
        let resource_name = 0x0800_3100;
        vm.write_c_string(descriptor, "&#65533;&#65533;").unwrap();
        vm.write_c_string(resource_name, "bg_WHITE").unwrap();
        vm.write_int(descriptor + 8, 2, resource_name).unwrap();
        vm.stack.extend([
            Value::Int(43),
            Value::Str("data02xxx.arc".into()),
            Value::Ptr(descriptor),
        ]);

        vm.normalize_graph_string_args(0x90, 0x10).unwrap();

        assert_eq!(vm.stack.last(), Some(&Value::Str("bg_WHITE".into())));
    }

    #[test]
    fn graph_draw_text_ex_converts_both_native_text_arguments() {
        let mut vm = Vm::new();
        let primary = 0x0800_3000;
        let annotation = 0x0800_3100;
        vm.write_c_string(primary, "history text").unwrap();
        vm.write_c_string(annotation, "ruby data").unwrap();
        vm.stack.extend([
            Value::Int(1833),
            Value::Int(0),
            Value::Int(12),
            Value::Ptr(primary),
            Value::Int(1),
            Value::Ptr(annotation),
            Value::Int(0),
            Value::Int(28),
            Value::Int(100),
            Value::Int(0),
            Value::Int(0),
            Value::Int(1),
            Value::Int(58),
            Value::Int(0x00ff_ffff),
        ]);

        vm.normalize_graph_string_args(0x91, 0x9c).unwrap();

        assert_eq!(vm.stack[3], Value::Str("history text".into()));
        assert_eq!(vm.stack[5], Value::Str("ruby data".into()));
    }

    #[test]
    fn graph92_9b_writes_the_last_text_output_pair() {
        let destination = 0x3000;
        let mut vm = Vm::new();
        vm.stack.extend([Value::Ptr(destination), Value::Int(256)]);
        let instruction = test_instruction(0x10, 0x92, "grp3", vec![0x92, 0x9B], Vec::new());
        let mut api = SchedulingApi {
            system92_text_output_pair: [123, -45],
            ..Default::default()
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.read_int(destination, 2).unwrap() as i32, 123);
        assert_eq!(vm.read_int(destination + 4, 2).unwrap() as i32, -45);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn graph92_9e_serializes_and_drains_fixed_fragment_records() {
        let destination = 0x3000;
        let mut vm = Vm::new();
        vm.stack.push(Value::Ptr(destination));
        let instruction = test_instruction(0x10, 0x92, "grp3", vec![0x92, 0x9E], Vec::new());
        let mut api = SchedulingApi {
            system92_text_fragment_records: vec![System92TextFragmentRecord {
                text: "日本語".into(),
                x: 17,
                y: -9,
            }],
            ..Default::default()
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.stack, [Value::Int(1)]);
        let encoded = encoding_rs::SHIFT_JIS.encode("日本語").0;
        for (index, byte) in encoded.iter().enumerate() {
            assert_eq!(
                vm.read_int(destination + index as u32, 0).unwrap(),
                *byte as u32
            );
        }
        assert_eq!(vm.read_int(destination + 120, 2).unwrap() as i32, 17);
        assert_eq!(vm.read_int(destination + 124, 2).unwrap() as i32, -9);
        assert!(api.system92_text_fragment_records.is_empty());
    }

    #[test]
    fn graph90_b7_rechecks_full_group_counts_after_clearing_the_window() {
        let window = 0xB000_0001_u32 as i32;
        let (root, groups, items) = (0x3000_u32, 0x4000_u32, 0x5000_u32);
        let mut vm = Vm::new();
        vm.write_int(root, 2, 1).unwrap();
        vm.write_int(root + 4, 2, groups).unwrap();
        // Low word 1 passes sub_46C9D0; sub_44B260 sees 0x10001.
        vm.write_int(groups, 2, 0x0001_0001).unwrap();
        vm.write_int(groups + 8, 2, items).unwrap();
        vm.write_int(items + 4, 2, 1).unwrap();
        let mut api = SchedulingApi {
            graph_window: Some(window),
            ..Default::default()
        };
        let b7 = test_instruction(0x10, 0x90, "grp1", vec![0x90, 0xB7], Vec::new());
        vm.stack.extend([Value::Int(window), Value::Ptr(root)]);
        vm.dispatch(&b7, &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(3)]);
        let (surface, descriptor) = api.graph_surface_controls.take().unwrap();
        assert_eq!(surface, window);
        assert!(descriptor.regions.is_empty());

        vm.stack.clear();
        vm.write_int(groups, 2, 1).unwrap();
        vm.stack.extend([Value::Int(window), Value::Ptr(root)]);
        vm.dispatch(&b7, &mut api).unwrap();
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(api.graph_surface_controls.unwrap().1.regions.len(), 1);
    }

    #[test]
    fn graph90_b4_b5_decode_base_and_extended_icon_record_arrays() {
        let window = 0xB000_0000_u32 as i32;
        let base_pointer = 0x3000;
        let extended_pointer = 0x4000;
        let mut vm = Vm::new();
        for (index, values) in [[11_i32, 12, 101, 7], [-3, 25, 102, 9]]
            .into_iter()
            .enumerate()
        {
            let address = base_pointer + (index * 16) as u32;
            for (field, value) in values.into_iter().enumerate() {
                vm.write_int(address + (field * 4) as u32, 2, value as u32)
                    .unwrap();
            }
        }
        let mut api = SchedulingApi {
            graph_window: Some(window),
            ..Default::default()
        };
        vm.stack
            .extend([Value::Int(window), Value::Int(2), Value::Ptr(base_pointer)]);
        let b4 = test_instruction(0x10, 0x90, "grp1", vec![0x90, 0xB4], Vec::new());

        vm.dispatch(&b4, &mut api).unwrap();

        assert_eq!(vm.stack, []);
        assert_eq!(
            api.graph_icon_batch,
            Some((
                window,
                vec![
                    GraphIconRecord {
                        x: 11,
                        y: 12,
                        bitmap: 101,
                        parameter: 7,
                    },
                    GraphIconRecord {
                        x: -3,
                        y: 25,
                        bitmap: 102,
                        parameter: 9,
                    },
                ],
            ))
        );

        for (index, values) in [[31_i32, 32, 201], [41, -42, 202]].into_iter().enumerate() {
            let address = extended_pointer + (index * 64) as u32;
            for (field, value) in values.into_iter().enumerate() {
                vm.write_int(address + (field * 4) as u32, 2, value as u32)
                    .unwrap();
            }
            vm.write_int(address + 12, 2, 0x1234_0000 + index as u32)
                .unwrap();
        }
        vm.stack.extend([
            Value::Int(window),
            Value::Int(2),
            Value::Ptr(extended_pointer),
        ]);
        let b5 = test_instruction(0x20, 0x90, "grp1", vec![0x90, 0xB5], Vec::new());

        vm.dispatch(&b5, &mut api).unwrap();

        assert_eq!(vm.stack, []);
        assert_eq!(
            api.graph_icon_batch,
            Some((
                window,
                vec![
                    GraphIconRecord {
                        x: 31,
                        y: 32,
                        bitmap: 201,
                        parameter: -1,
                    },
                    GraphIconRecord {
                        x: 41,
                        y: -42,
                        bitmap: 202,
                        parameter: -1,
                    },
                ],
            ))
        );
    }

    #[test]
    fn graph92_draw_formatted_text_normalizes_native_text_argument() {
        let mut vm = Vm::new();
        let text_pointer = 0x0800_3000;
        vm.write_c_string(text_pointer, "formatted text").unwrap();
        vm.stack.extend((0..11).map(Value::Int));
        let text_index = vm.stack.len() - 1 - 9;
        vm.stack[text_index] = Value::Ptr(text_pointer);

        vm.normalize_graph_string_args(0x92, 0x91).unwrap();

        assert_eq!(vm.stack[text_index], Value::Str("formatted text".into()));
    }

    #[test]
    fn message_text_with_the_engine_glyph_codes_is_text() {
        let mut vm = Vm::new();
        let text_pointer = 0x0800_3000;
        // "A", the dash code 0xEF40 twice, "B", the codes 0xEF41 and 0xF040,
        // "!" and 0x7F.
        let bytes = [
            b'A', 0xEF, 0x40, 0xEF, 0x40, b'B', 0xEF, 0x41, 0xF0, 0x40, b'!', 0x7F, 0,
        ];
        let at = vm.memory_addr(text_pointer) as usize;
        vm.memory[at..at + bytes.len()].copy_from_slice(&bytes);
        vm.stack.extend((0..11).map(Value::Int));
        let text_index = vm.stack.len() - 1 - 9;
        vm.stack[text_index] = Value::Int(text_pointer as i32);

        vm.normalize_graph_string_args(0x92, 0x91).unwrap();

        let expected = "A\u{2500}\u{2500}B!\u{2015}";
        assert_eq!(vm.stack[text_index], Value::Str(expected.into()));

        // A line formatted with sprintf keeps them.
        let formatted = 0x0800_3100;
        vm.stack.clear();
        vm.stack.push(Value::Ptr(text_pointer));
        let rendered = vm.render_sprintf("%s");
        vm.write_c_string(formatted, &rendered).unwrap();
        assert_eq!(vm.read_c_string(formatted).unwrap(), expected);
    }

    #[test]
    fn extended_icon_item_with_a_zero_first_word_is_never_hit() {
        let mut vm = Vm::new();
        let (root, group, items) = (0x0800_4000_u32, 0x0800_4100_u32, 0x0800_4200_u32);
        vm.write_int(root, 2, 1).unwrap();
        vm.write_int(root + 4, 2, group).unwrap();
        vm.write_int(group, 2, 2).unwrap();
        vm.write_int(group + 8, 2, items).unwrap();
        vm.write_int(group + 12, 2, u32::MAX).unwrap();
        for (index, first_word) in [1_u32, 0].into_iter().enumerate() {
            let item = items + index as u32 * 196;
            vm.write_int(item, 2, first_word).unwrap();
            vm.write_int(item + 4, 2, 1).unwrap();
            vm.write_int(item + 32, 2, 7).unwrap();
            vm.write_int(item + 48, 2, u32::MAX).unwrap();
        }

        let descriptor = vm.read_graph_input_descriptor(root).unwrap();

        let masks = descriptor
            .regions
            .iter()
            .map(|region| region.mask_resource)
            .collect::<Vec<_>>();
        assert_eq!(masks, [-1, -2]);
    }

    #[test]
    fn graph92_render_text_normalizes_both_native_text_arguments() {
        let mut vm = Vm::new();
        let primary = 0x0800_3000;
        let auxiliary = 0x0800_3100;
        vm.write_c_string(primary, "main text").unwrap();
        vm.write_c_string(auxiliary, "auxiliary text").unwrap();
        vm.stack.extend((0..21).map(Value::Int));
        let primary_index = vm.stack.len() - 1 - 17;
        let auxiliary_index = vm.stack.len() - 1 - 14;
        vm.stack[primary_index] = Value::Ptr(primary);
        vm.stack[auxiliary_index] = Value::Ptr(auxiliary);

        vm.normalize_graph_string_args(0x92, 0x9c).unwrap();

        assert_eq!(vm.stack[primary_index], Value::Str("main text".into()));
        assert_eq!(
            vm.stack[auxiliary_index],
            Value::Str("auxiliary text".into())
        );
    }

    #[test]
    fn native_text_arguments_across_an_operand_stack_wrap_are_converted() {
        let mut vm = Vm::new();
        let primary = 0x0800_3000;
        let auxiliary = 0x0800_3100;
        vm.write_c_string(primary, "main text").unwrap();
        vm.write_c_string(auxiliary, "auxiliary text").unwrap();
        // Results nobody popped fill the ring up to ten slots before its end,
        // so the push of the call's tenth argument wraps it.
        for _ in 0..crate::OPERAND_STACK_CAPACITY - 10 {
            vm.push_value(Value::Int(-1));
        }
        for index in 0..21 {
            vm.push_value(match index {
                3 => Value::Ptr(primary),
                6 => Value::Ptr(auxiliary),
                _ => Value::Int(index),
            });
        }
        assert_eq!(vm.stack.len(), 11);

        vm.normalize_graph_string_args(0x92, 0x9c).unwrap();

        let mut args = (0..21).map(|_| vm.pop_value().unwrap()).collect::<Vec<_>>();
        args.reverse();
        assert_eq!(args[3], Value::Str("main text".into()));
        assert_eq!(args[6], Value::Str("auxiliary text".into()));
        assert_eq!(args[20], Value::Int(20));
        assert_eq!(args[0], Value::Int(0));
    }

    #[test]
    fn graph92_render_text_integer_address_of_an_empty_buffer_is_empty_text() {
        let mut vm = Vm::new();
        // The message window's speaker-name buffer, as the script computes it.
        let name_buffer = 86024 + 79240;
        // Out of reach of the descriptor search (it looks up to 64 bytes away).
        let text_buffer = name_buffer + 0x1000;
        vm.write_c_string(name_buffer as u32, "").unwrap();
        vm.write_c_string(text_buffer as u32, "a line").unwrap();
        vm.stack.extend((0..21).map(Value::Int));
        let primary_index = vm.stack.len() - 1 - 17;
        let auxiliary_index = vm.stack.len() - 1 - 14;
        vm.stack[primary_index] = Value::Int(name_buffer);
        vm.stack[auxiliary_index] = Value::Int(text_buffer);

        vm.normalize_graph_string_args(0x92, 0x9c).unwrap();

        assert_eq!(vm.stack[primary_index], Value::Str(String::new()));
        assert_eq!(vm.stack[auxiliary_index], Value::Str("a line".into()));
    }

    #[test]
    fn message_text_integer_address_of_an_empty_string_is_not_searched_around() {
        let mut vm = Vm::new();
        // scrmsg passes a line's text as `pointer + 1`, an integer; some
        // lines have an empty text part. A sprite name 16 bytes on is in
        // reach of the descriptor search, which used to return it.
        let text = 0x0800_3000_u32;
        let neighbour = text + 16;
        vm.write_c_string(text, "").unwrap();
        vm.write_c_string(neighbour, "ev0005a_l").unwrap();
        vm.write_int(text + 4, 2, neighbour + 3).unwrap();
        vm.stack.extend((0..15).map(Value::Int));
        let text_index = vm.stack.len() - 1 - 13;
        vm.stack[text_index] = Value::Int(text as i32);

        vm.normalize_graph_string_args(0x92, 0x90).unwrap();

        assert_eq!(vm.stack[text_index], Value::Str(String::new()));
    }

    #[test]
    fn graph92_render_text_keeps_integers_outside_memory() {
        let mut vm = Vm::new();
        let outside = super::INITIAL_MEMORY_SIZE as i32 + 0x10;
        assert!(outside as usize >= vm.memory.len());
        vm.stack.extend((0..21).map(Value::Int));
        let primary_index = vm.stack.len() - 1 - 17;
        vm.stack[primary_index] = Value::Int(outside);

        vm.normalize_graph_string_args(0x92, 0x9c).unwrap();

        assert_eq!(vm.stack[primary_index], Value::Int(outside));
    }

    #[test]
    fn strcpy_preserves_native_bytes_for_pointer_sources() {
        let source = 0x3000;
        let destination = 0x3100;
        let mut vm = Vm::new();
        vm.write_int(source, 0, 0x81).unwrap();
        vm.write_int(source + 1, 0, 0).unwrap();
        vm.stack
            .extend([Value::Ptr(destination), Value::Ptr(source)]);

        vm.dispatch(
            &test_instruction(0x10, 0x6a, "strcpy", vec![0x6a], Vec::new()),
            &mut TraceApi,
        )
        .unwrap();

        assert_eq!(vm.read_int(destination, 0).unwrap(), 0x81);
        assert_eq!(vm.read_int(destination + 1, 0).unwrap(), 0);
        assert!(!vm.memory_preview(destination, 16).contains("&#65533;"));
    }

    #[test]
    fn strcpy_materializes_shadowed_scenario_strings() {
        let source = 0x3000;
        let destination = 0x3100;
        let mut vm = Vm::new();
        vm.mem_values
            .insert(vm.value_key(source), Value::Str("シナリオの台詞".into()));
        vm.stack
            .extend([Value::Ptr(destination), Value::Ptr(source)]);

        vm.dispatch(
            &test_instruction(0x10, 0x6a, "strcpy", vec![0x6a], Vec::new()),
            &mut TraceApi,
        )
        .unwrap();

        assert_eq!(vm.read_c_string(destination).unwrap(), "シナリオの台詞");
    }

    #[test]
    fn amachoco_modes_preserve_arguments_and_reject_invalid_values() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(super::NativeAbiProfile::Amachoco);
        let sys = test_instruction(0, 0x81, "sys2", vec![0x81, 0x00], Vec::new());
        let graph = test_instruction(0, 0x91, "grp2", vec![0x91, 0x09], Vec::new());
        for mode in [0, 1, 2, -1] {
            vm.stack = vec![Value::Int(123), Value::Int(mode)];
            vm.dispatch(&sys, &mut TraceApi).unwrap();
            assert_eq!(
                vm.stack,
                [
                    Value::Int(123),
                    Value::Int(i32::from((0..=1).contains(&mode)))
                ]
            );
            assert_eq!(vm.native_utf8_enabled(), mode != 0);
            vm.stack = vec![Value::Int(123), Value::Int(mode)];
            vm.dispatch(&graph, &mut TraceApi).unwrap();
            assert_eq!(
                vm.stack,
                [
                    Value::Int(123),
                    Value::Int(i32::from((0..=1).contains(&mode)))
                ]
            );
            assert_eq!(
                vm.system81_shared.lock().unwrap().graph91_resource_mode,
                i32::from(mode != 0)
            );
        }
        for mode in [0, 1] {
            vm.stack = vec![Value::Int(mode)];
            vm.dispatch(&sys, &mut TraceApi).unwrap();
            vm.write_c_string(0x3000, "きゃべつそふと").unwrap();
            let expected = if mode == 0 {
                encoding_rs::SHIFT_JIS
                    .encode("きゃべつそふと")
                    .0
                    .into_owned()
            } else {
                "きゃべつそふと".as_bytes().to_vec()
            };
            assert_eq!(vm.read_c_string_bytes(0x3000).unwrap(), expected);
            assert_eq!(vm.read_c_string(0x3000).unwrap(), "きゃべつそふと");
        }
        let mut old = Vm::new();
        assert_eq!(
            old.try_builtin_sys_with_api(&mut TraceApi, 0x81, 0x00)
                .unwrap(),
            None
        );
        assert_eq!(old.effective_arg_count(0x91, 0x09), None);
        assert_eq!(old.effective_arg_count(0x92, 0x99), None);
    }

    #[test]
    fn amachoco_character_widths_write_records_and_count() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(super::NativeAbiProfile::Amachoco);
        let instruction = test_instruction(0, 0x92, "grp3", vec![0x92, 0x99], Vec::new());
        for (destination, count) in [(0x3000, 0x3100), (0, 0x3100), (0x3000, 0), (0, 0)] {
            vm.stack = vec![
                Value::Int(123),
                Value::Ptr(destination),
                Value::Ptr(count),
                Value::Str("Aあｱ".into()),
                Value::Int(0),
                Value::Int(28),
                Value::Int(100),
                Value::Int(0),
            ];
            vm.dispatch(&instruction, &mut TraceApi).unwrap();
            assert_eq!(vm.stack, [Value::Int(123), Value::Int(0)]);
        }
        assert_eq!(vm.read_int(0x3100, 2).unwrap(), 3);
        let actual: Vec<_> = (0..9)
            .map(|i| vm.read_int(0x3000 + i * 4, 2).unwrap())
            .collect();
        assert_eq!(actual, [0, 14, 0, 0, 28, 0, 0, 14, 0]);
        let measure = test_instruction(0, 0x91, "grp2", vec![0x91, 0x9b], Vec::new());
        vm.memory[0x3300..0x3305].copy_from_slice(&[b'A', 0x82, 0xa0, 0xb1, 0]);
        for text in [Value::Str("Aあｱ".into()), Value::Ptr(0x3300)] {
            vm.stack = vec![
                Value::Ptr(0x3200), text, Value::Int(0), Value::Int(28),
                Value::Int(100), Value::Int(0), Value::Int(0),
            ];
            vm.dispatch(&measure, &mut TraceApi).unwrap();
            assert_eq!(vm.stack, [Value::Int(0)]);
            assert_eq!(vm.read_int(0x3200, 2).unwrap(), 56);
        }
        vm.write_int(0x3100, 2, 123).unwrap();
        vm.stack = vec![
            Value::Ptr(0),
            Value::Ptr(0x3100),
            Value::Str("あ".into()),
            Value::Int(0),
            Value::Int(3),
            Value::Int(100),
            Value::Int(0),
        ];
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(0x8000_0001_u32 as i32)]);
        assert_eq!(vm.read_int(0x3100, 2).unwrap(), 123);
    }

    #[test]
    fn system81_utf8_copy_materializes_shadowed_scenario_strings() {
        let source = 0x3000;
        let destination = 0x3100;
        let mut vm = Vm::new();
        vm.set_native_abi_profile(super::NativeAbiProfile::Amachoco);
        vm.mem_values
            .insert(vm.value_key(source), Value::Str("シナリオの台詞".into()));
        vm.stack
            .extend([Value::Ptr(destination), Value::Ptr(source)]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x81, 0x21)
                .unwrap(),
            Some(Value::None)
        );
        assert!(vm.stack.is_empty());
        assert_eq!(vm.read_c_string_bytes(destination).unwrap(), "シナリオの台詞".as_bytes());
        assert_eq!(
            vm.mem_values.get(&vm.value_key(destination)),
            Some(&Value::Str("シナリオの台詞".into()))
        );
    }

    #[test]
    fn native_system_event_queue_preserves_two_argument_abi() {
        let destination = 0x2000u32;
        let program = BpProgram {
            script_name: Some("system-event-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x4001)],
                ),
                test_instruction(
                    0x15,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(100)],
                ),
                test_instruction(0x1a, 0x80, "sys1", vec![0x80, 0xa1], Vec::new()),
                test_instruction(
                    0x1c,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(0x21, 0x80, "sys1", vec![0x80, 0xa0], Vec::new()),
                test_instruction(0x23, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(1)]);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 0);
        assert_eq!(vm.read_int(destination + 4, 2).unwrap(), 0x4001);
        assert_eq!(vm.read_int(destination + 8, 2).unwrap(), 100);
    }

    #[test]
    fn native_bgm_query_separates_completion_status_from_output_state() {
        let destination = 0x2200u32;
        let program = BpProgram {
            script_name: Some("bgm-query-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(15)],
                ),
                test_instruction(
                    0x15,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(destination)],
                ),
                test_instruction(0x1a, 0xa0, "snd1", vec![0xa0, 0x15], Vec::new()),
                test_instruction(0x1c, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi {
            bgm_state: Some((0, 9)),
            ..SchedulingApi::default()
        };
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(vm.read_int(destination, 2).unwrap(), 9);
    }

    #[test]
    fn sound_memory_registration_copies_the_target_descriptor_and_payload() {
        let source = 0x2600u32;
        let program = BpProgram {
            script_name: Some("sound-memory-registration-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(7)],
                ),
                test_instruction(
                    0x15,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(source)],
                ),
                test_instruction(
                    0x1a,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(3)],
                ),
                test_instruction(
                    0x1f,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x0001_0000)],
                ),
                test_instruction(
                    0x24,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x0002_0000)],
                ),
                test_instruction(0x29, 0xa0, "snd1", vec![0xa0, 0x28], Vec::new()),
                test_instruction(0x2b, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        vm.write_int(source, 2, 64).unwrap();
        vm.write_int(source + 8, 2, 8).unwrap();
        for (index, byte) in (0u8..8).enumerate() {
            vm.memory[source as usize + 64 + index] = byte;
        }

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(vm.stack.is_empty());
        let (channel, block, native_start, decode_gain, playback_rate) =
            api.memory_sound.expect("memory sound bridge");
        assert_eq!(channel, 7);
        assert_eq!(block.len(), 72);
        assert_eq!(&block[64..], &[0, 1, 2, 3, 4, 5, 6, 7]);
        assert_eq!(native_start, 3);
        assert_eq!(decode_gain, 1.0);
        assert_eq!(playback_rate, 2.0);
    }

    #[test]
    fn wait_poll_opcodes_do_not_leave_synthetic_stack_values() {
        let program = BpProgram {
            script_name: Some("wait-poll-stack-contract-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0)],
                ),
                test_instruction(0x15, 0x80, "sys1", vec![0x80, 0x14], Vec::new()),
                test_instruction(0x17, 0x80, "sys1", vec![0x80, 0x16], Vec::new()),
                test_instruction(0x19, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(
            vm.stack.is_empty(),
            "zero-output native handlers must not alter BP stack depth"
        );
    }

    #[test]
    fn native_game_id_writes_host_identifier_without_a_stack_result() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            game_id: Some("HimawariNoKyoukaiToNagaiNatsuyasumi".into()),
            ..Default::default()
        };
        let destination = 0x3000;
        vm.stack.extend([Value::Int(42), Value::Ptr(destination)]);

        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0xe8], Vec::new()),
            &mut api,
        )
        .unwrap();

        assert_eq!(vm.read_c_string(destination).unwrap(), api.game_id());
        assert_eq!(vm.stack, vec![Value::Int(42)]);
    }

    #[test]
    fn native_terminate_interpreter_stops_following_bytecode() {
        let program = BpProgram {
            script_name: Some("terminate-interpreter-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x6a], Vec::new()),
                test_instruction(
                    0x12,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x1234)],
                ),
                test_instruction(0x17, 0x17, "ret", vec![0x17], Vec::new()),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::InterpreterTerminated);
        assert!(vm.halted);
        assert!(vm.stack.is_empty());
        assert_eq!(report.steps, 1);
    }

    #[test]
    fn recovered_movie_loader_completes_on_the_next_scheduler_pass() {
        let program = BpProgram {
            script_name: Some("procedure-boundary-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_byte",
                    vec![0x01, 1],
                    vec![BpOperand::U8(1)],
                ),
                test_instruction(
                    0x12,
                    0x01,
                    "push_byte",
                    vec![0x01, 2],
                    vec![BpOperand::U8(2)],
                ),
                test_instruction(
                    0x14,
                    0x01,
                    "push_byte",
                    vec![0x01, 3],
                    vec![BpOperand::U8(3)],
                ),
                test_instruction(
                    0x16,
                    0x01,
                    "push_byte",
                    vec![0x01, 4],
                    vec![BpOperand::U8(4)],
                ),
                test_instruction(
                    0x18,
                    0x01,
                    "push_byte",
                    vec![0x01, 5],
                    vec![BpOperand::U8(5)],
                ),
                // Graph92:F1 is procedure-installing in the recovered native ABI.
                test_instruction(0x1a, 0x92, "grp3", vec![0x92, 0xf1], Vec::new()),
                test_instruction(
                    0x1c,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let options = VmRunOptions {
            max_steps: 100,
            ..Default::default()
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &options);
        assert_eq!(report.stop_reason, VmStopReason::WaitingForProcedure);
        assert_eq!(report.steps, 6);
        assert_eq!(report.pc, 6);
        assert!(vm.stack.is_empty());

        let report = vm.run_loaded(&mut api, &options);
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(report.steps, 1);
        assert_eq!(report.pc, 7);
        assert_eq!(vm.stack, [Value::Int(99)]);
    }

    #[test]
    fn host_wait_is_ignored_for_an_abi_nonprocedure_handler() {
        let program = BpProgram {
            script_name: Some("nonprocedure-boundary-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(
                    0x10,
                    0x01,
                    "push_byte",
                    vec![0x01, 7],
                    vec![BpOperand::U8(7)],
                ),
                // Graph90:B9 returns synchronously and does not install CProcedure.
                test_instruction(0x12, 0x90, "grp1", vec![0x90, 0xb9], Vec::new()),
                test_instruction(
                    0x14,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();

        let report = vm.run(&program, &mut api, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(report.steps, 3);
        assert_eq!(vm.stack, [Value::Int(99)]);
        assert!(vm.thread.current_procedure().is_none());
    }

    #[test]
    fn internal_graph_procedure_paths_install_their_target_class() {
        let mut vm = Vm::new();
        vm.ensure_mapped_internal_graph_procedure_boundary(
            NativeOpcode {
                group: 0x90,
                id: 0x20,
            },
            false,
        );
        assert_eq!(
            vm.thread
                .current_procedure()
                .map(|procedure| procedure.object.class_name()),
            Some("CProcCtrlDspObj")
        );

        vm.thread.clear_current_procedure();
        vm.ensure_mapped_internal_graph_procedure_boundary(
            NativeOpcode {
                group: 0x90,
                id: 0xF6,
            },
            false,
        );
        assert!(
            vm.thread.current_procedure().is_none(),
            "conditional Graph90:F6 must not install a procedure unconditionally"
        );
    }

    #[test]
    fn message_first_input_reveals_and_second_input_completes() {
        let mut vm = Vm::new();
        vm.stack.push(Value::Int(42));
        install_message_procedure(&mut vm);
        let mut api = SchedulingApi {
            input_class_state: 1,
            message_animating: true,
            ..Default::default()
        };

        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert!(vm.thread.current_procedure().is_some());
        assert_eq!(api.message_reveals, 1);
        assert_eq!(api.message_finishes, 0);
        assert_eq!(vm.stack, [Value::Int(42)]);

        api.input_class_state = 1;
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert!(vm.thread.current_procedure().is_none());
        assert_eq!(api.message_finishes, 1);
        // sub_433600 pushes +0x34, which input latched to 1.
        assert_eq!(vm.stack, [Value::Int(42), Value::Int(1)]);
    }

    #[test]
    fn message_auto_deadline_arms_only_after_reveal_finishes() {
        let mut vm = Vm::new();
        vm.install_cprocedure(
            super::native_thread::InstalledCProcedure::dsp_msg(
                vm.thread.thread_id(),
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: true,
                    initial_delay_ms: 10,
                    reveal_duration_ms: 20,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: Some(30),
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy: 1,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0,
                    input_forces_completion: false,
                },
            ),
            false,
        );
        let mut api = SchedulingApi {
            message_animating: true,
            ..Default::default()
        };

        vm.advance_time_ms(59);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        api.message_animating = false;
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(29);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(1);
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(api.message_reveals, 0);
        assert_eq!(api.message_finishes, 1);
    }

    fn install_message_procedure_with_auto_advance(
        vm: &mut Vm,
        auto_advance_delay_ms: Option<i32>,
    ) {
        vm.install_cprocedure(
            super::native_thread::InstalledCProcedure::dsp_msg(
                vm.thread.thread_id(),
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: false,
                    initial_delay_ms: 0,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms,
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy: 1,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0,
                    input_forces_completion: false,
                },
            ),
            false,
        );
    }

    #[test]
    fn message_auto_mode_switched_on_while_a_line_waits_ends_that_line() {
        // Wonderful Everyday's auto button calls Graph90:97(1, 0) while the
        // line it was clicked on waits for input; the line started with auto
        // mode off.
        let mut vm = Vm::new();
        install_message_procedure_with_auto_advance(&mut vm, None);
        let mut api = SchedulingApi::default();

        vm.advance_time_ms(500);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        api.message_auto_advance = Some(Some(0));
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(api.message_finishes, 1);
    }

    #[test]
    fn message_auto_mode_switched_off_drops_an_armed_deadline() {
        let mut vm = Vm::new();
        install_message_procedure_with_auto_advance(&mut vm, Some(30));
        let mut api = SchedulingApi::default();

        // Armed at the first poll after the text ends: due at 30 ms.
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        api.message_auto_advance = Some(None);
        vm.advance_time_ms(40);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        // Switched on again: armed anew from now, not from the old deadline.
        api.message_auto_advance = Some(Some(30));
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(29);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(1);
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(api.message_finishes, 1);
    }

    #[test]
    fn the_start_delay_holds_only_a_message_that_waits_at_its_end() {
        // Wonderful Everyday's scrmsg sets the voice's remaining time as the
        // start delay, then runs a typed message (+0x38 = 0) and a waiting
        // one in auto mode. Only the waiting one is held, as in sub_433E40.
        let message = |end_wait_policy: i32| {
            super::native_thread::InstalledCProcedure::dsp_msg(
                super::MAIN_THREAD_ID,
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: true,
                    initial_delay_ms: 1000,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: Some(30),
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0,
                    input_forces_completion: false,
                },
            )
        };

        let mut vm = Vm::new();
        vm.install_cprocedure(message(0), false);
        let mut api = SchedulingApi::default();
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(api.message_finishes, 1);

        let mut vm = Vm::new();
        vm.install_cprocedure(message(1), false);
        let mut api = SchedulingApi::default();
        vm.advance_time_ms(999);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(1);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(30);
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(api.message_finishes, 1);
    }

    #[test]
    fn the_caret_runs_only_while_a_message_waits_at_its_end() {
        // sub_433E40 calls sub_433F40 (the caret) after the start delay
        // and once the text processor has ended; a typed message (+0x38 ==
        // 0) is finalized before that.
        let message = |end_wait_policy: i32| {
            super::native_thread::InstalledCProcedure::dsp_msg(
                super::MAIN_THREAD_ID,
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: true,
                    initial_delay_ms: 100,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: None,
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0,
                    input_forces_completion: false,
                },
            )
        };

        let mut vm = Vm::new();
        vm.install_cprocedure(message(0), false);
        let mut api = SchedulingApi::default();
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert!(api.message_caret_ticks.is_empty());

        let mut vm = Vm::new();
        vm.install_cprocedure(message(1), false);
        let mut api = SchedulingApi {
            message_animating: true,
            ..Default::default()
        };
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        api.message_animating = false;
        vm.advance_time_ms(50);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert!(api.message_caret_ticks.is_empty());
        vm.advance_time_ms(50);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        vm.advance_time_ms(500);
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert_eq!(api.message_caret_ticks, [100, 600]);
        assert_eq!(api.message_finishes, 0);
    }

    #[test]
    fn message_callback_256_force_completes_without_fabricating_input_value() {
        let mut vm = Vm::new();
        vm.stack.push(Value::Int(77));
        install_message_procedure(&mut vm);
        let mut api = SchedulingApi {
            message_animating: true,
            ..Default::default()
        };
        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(256), Value::Int(0), Value::Int(0)],
            false,
        ));

        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert!(vm.thread.current_procedure().is_none());
        assert_eq!(api.message_reveals, 1);
        assert_eq!(api.message_finishes, 1);
        // Completion pushes +0x34; callback 256 does not set it.
        assert_eq!(vm.stack, [Value::Int(77), Value::Int(0)]);
    }

    #[test]
    fn message_callbacks_1_and_258_set_force_and_completion_latch() {
        for code in [1, 258] {
            let mut vm = Vm::new();
            install_message_procedure(&mut vm);
            let mut api = SchedulingApi {
                message_animating: true,
                ..Default::default()
            };
            assert!(vm.post_async_program_callback(
                Value::Int(super::MAIN_THREAD_ID),
                [Value::Int(code), Value::Int(0), Value::Int(0)],
                false,
            ));

            assert_eq!(vm.poll_current_procedure(&mut api, false), None);
            assert!(vm.thread.current_procedure().is_none());
            assert_eq!(api.message_reveals, 1);
            assert_eq!(api.message_finishes, 1);
        }
    }

    #[test]
    fn message_callback_257_controls_high_bit_permission_only() {
        let mut vm = Vm::new();
        install_message_procedure(&mut vm);
        let mut api = SchedulingApi {
            input_class_state: i32::MIN,
            message_animating: true,
            ..Default::default()
        };
        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(257), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert_eq!(api.message_reveals, 0);
        assert_eq!(api.message_finishes, 0);

        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(257), Value::Int(1), Value::Int(0)],
            false,
        ));
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert_eq!(api.message_reveals, 1);
        assert_eq!(api.message_finishes, 0);
    }

    #[test]
    fn message_auxiliary_input_mask_accepts_registered_bits_when_enabled() {
        let mut vm = Vm::new();
        vm.install_cprocedure(
            super::native_thread::InstalledCProcedure::dsp_msg(
                vm.thread.thread_id(),
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: false,
                    initial_delay_ms: 0,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: None,
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy: 1,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: true,
                    auxiliary_input_mask: 0x4000,
                    input_forces_completion: false,
                },
            ),
            false,
        );
        let mut api = SchedulingApi {
            input_class_state: 0x4000,
            message_animating: true,
            ..Default::default()
        };

        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert_eq!(api.message_reveals, 1);
        assert_eq!(api.message_finishes, 0);
    }

    #[test]
    fn message_auxiliary_input_mask_is_removed_when_permission_is_disabled() {
        let mut vm = Vm::new();
        vm.install_cprocedure(
            super::native_thread::InstalledCProcedure::dsp_msg(
                vm.thread.thread_id(),
                NativeOpcode {
                    group: 0x90,
                    id: 0x90,
                },
                0,
                super::NativeMessageProcedureConfig {
                    class: super::NativeMessageProcedureClass::DspMsg,
                    initial_delay_enabled: false,
                    initial_delay_ms: 0,
                    reveal_duration_ms: 0,
                    reveal_steps: 0,
                    reveal_step_delay_ms: 0,
                    settle_steps: 0,
                    settle_step_delay_ms: 0,
                    auto_advance_delay_ms: None,
                    input_scope: 2,
                    completion_control: 0,
                    end_wait_policy: 1,
                    allow_high_bit_input: true,
                    allow_auxiliary_input: false,
                    auxiliary_input_mask: 0x4000,
                    input_forces_completion: false,
                },
            ),
            false,
        );
        let mut api = SchedulingApi {
            input_class_state: 0x4000,
            message_animating: true,
            ..Default::default()
        };

        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForInputOrTime)
        );
        assert_eq!(api.message_reveals, 0);
        assert_eq!(api.message_finishes, 0);
    }

    #[test]
    fn thread_timer_wait_is_conditional_and_completes_at_deadline() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi::default();
        let set_instruction = test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x58], Vec::new());
        let wait_instruction = test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x5A], Vec::new());

        vm.stack.push(Value::Int(50));
        assert_eq!(
            vm.dispatch_scheduler_opcode(
                &mut api,
                native_call::opcodes::SYS_SET_THREAD_TIMER,
                0,
                &set_instruction,
                false,
            )
            .unwrap(),
            Some(Value::None)
        );
        assert_eq!(
            vm.dispatch_scheduler_opcode(
                &mut api,
                native_call::opcodes::SYS_WAIT_THREAD_TIMER,
                0,
                &wait_instruction,
                false,
            )
            .unwrap(),
            Some(Value::Int(1))
        );
        assert!(matches!(
            vm.thread
                .current_procedure()
                .map(|installed| installed.object),
            Some(super::native_thread::CProcedure::WaitTiming(_))
        ));
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForTime)
        );

        vm.advance_time_ms(50);
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert!(vm.thread.current_procedure().is_none());

        assert_eq!(
            vm.dispatch_scheduler_opcode(
                &mut api,
                native_call::opcodes::SYS_WAIT_THREAD_TIMER,
                0,
                &wait_instruction,
                false,
            )
            .unwrap(),
            Some(Value::Int(0))
        );
        assert!(vm.thread.current_procedure().is_none());
    }

    #[test]
    fn wait_window_message_ignores_stale_and_unrelated_messages() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            window_message_serial: 10,
            ..SchedulingApi::default()
        };
        let instruction = test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x54], Vec::new());
        vm.stack.push(Value::Int(0x0201));

        assert_eq!(
            vm.dispatch_scheduler_opcode(
                &mut api,
                native_call::opcodes::SYS_WAIT_WINDOW_MESSAGE,
                0,
                &instruction,
                false,
            )
            .unwrap(),
            Some(Value::None)
        );
        api.window_messages.push_back((9, 0x0201, 1, 2));
        api.window_messages.push_back((11, 0x0200, 3, 4));
        assert_eq!(
            vm.poll_current_procedure(&mut api, false),
            Some(VmStopReason::WaitingForProcedure)
        );

        // (serial, id, wParam, lParam): the newest one wins and stays for
        // other waiters.
        api.window_messages.push_back((12, 0x0201, 0x1234, 1));
        api.window_messages.push_back((13, 0x0201, 0x5678, 2));
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(vm.stack, [Value::Int(0x5678), Value::Int(2)]);
        assert_eq!(api.window_messages.len(), 4);
        assert!(vm.thread.current_procedure().is_none());
    }

    #[test]
    fn wait_timing_ex_times_out_and_pushes_zero() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 100, 1, 1808);
        let mut api = SchedulingApi::default();

        assert!(vm.poll_wait_timing_procedure(&mut api, false));
        vm.advance_time_ms(100);
        assert!(!vm.poll_wait_timing_procedure(&mut api, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(api.last_input_scope, Some(1808));
    }

    #[test]
    fn wait_timing_ex_reports_input_or_time_without_spinning() {
        let program = BpProgram {
            script_name: Some("wait-boundary-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![test_instruction(
                0x10,
                0x80,
                "sys1",
                vec![0x80, 0x5C],
                Vec::new(),
            )]),
            labels: Default::default(),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut vm = Vm::new();
        vm.start(&program);
        vm.stack
            .extend([Value::Int(100), Value::Int(1), Value::Int(1808)]);
        let mut api = SchedulingApi::default();
        let report = vm.run_loaded(&mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::WaitingForInputOrTime);
        assert_eq!(report.steps, 1);
    }

    #[test]
    fn wait_timing_ex_is_interrupted_by_its_input_class() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 1, 1808);
        let mut api = SchedulingApi {
            input_class_state: 1,
            ..Default::default()
        };

        assert!(!vm.poll_wait_timing_procedure(&mut api, false));
        assert_eq!(vm.stack, [Value::Int(1)]);
        assert_eq!(api.last_input_scope, Some(1808));
    }

    #[test]
    fn native_program_callback_code_zero_cancels_wait_timing_with_zero_result() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);

        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(0), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert!(!vm.poll_wait_timing_procedure(&mut TraceApi, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn native_program_callback_code_one_completes_wait_timing_with_zero_result() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);

        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(1), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert!(!vm.poll_wait_timing_procedure(&mut TraceApi, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn wait_timing_ex_ignores_callback_codes_other_than_zero_and_one() {
        // sub_43D4B0 reacts to code 1 only; sub_431AF0 latches only code 0.
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);
        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(2), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert!(vm.poll_wait_timing_procedure(&mut TraceApi, false));
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn wait_timing_ex_stops_when_the_system_wait_state_is_cleared() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);
        vm.system_wait_state = 0;
        assert!(!vm.poll_wait_timing_procedure(&mut TraceApi, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn wait_timing_ex_timeout_wins_over_pending_input() {
        // sub_43D430 tests the deadline before it queries input.
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 10, 1, 1808);
        vm.advance_time_ms(10);
        let mut api = SchedulingApi {
            input_class_state: 1,
            ..Default::default()
        };
        assert!(!vm.poll_wait_timing_procedure(&mut api, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn wait_timing_ex_keeps_the_thread_timer_of_0x5a() {
        let mut vm = Vm::new();
        vm.thread.set_deadline_from_now(vm.timing.tick_count(), 500);
        let before = vm.thread.deadline_tick();
        install_wait_timing_ex(&mut vm, 20, 0, 1808);
        assert_eq!(vm.thread.deadline_tick(), before);
    }

    #[test]
    fn replacing_or_finishing_a_procedure_discards_its_queued_callbacks() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);
        assert!(vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(7), Value::Int(0), Value::Int(0)],
            false,
        ));
        install_wait_timing_ex(&mut vm, 60_001, 0, 1808);
        assert!(vm.thread.procedure_callbacks_is_empty());
    }

    #[test]
    fn native_program_callback_fails_without_an_installed_procedure() {
        let mut vm = Vm::new();
        assert!(!vm.post_async_program_callback(
            Value::Int(super::MAIN_THREAD_ID),
            [Value::Int(1), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert!(vm.thread.procedure_callbacks_is_empty());
    }

    #[test]
    fn wait_timing_ex_without_input_ignores_input_state() {
        let mut vm = Vm::new();
        install_wait_timing_ex(&mut vm, 20, 0, 1808);
        let mut api = SchedulingApi {
            input_class_state: 1,
            ..Default::default()
        };

        assert!(vm.poll_wait_timing_procedure(&mut api, false));
        assert_eq!(api.last_input_scope, None);
        vm.advance_time_ms(20);
        assert!(!vm.poll_wait_timing_procedure(&mut api, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn sys80_12_sums_zero_terminated_descriptor_array_without_consuming_state() {
        let mut vm = Vm::new();
        let descriptors = 0x2a00u32;
        for (index, value) in [11_u32, 13, 17, 0].into_iter().enumerate() {
            vm.write_int(descriptors + index as u32 * 4, 2, value)
                .unwrap();
        }
        vm.stack.push(Value::Ptr(descriptors));
        let mut api = SchedulingApi::default();
        api.input_descriptor_states.insert(11, 2);
        api.input_descriptor_states.insert(13, 6);
        api.input_descriptor_states.insert(17, -1);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x12).unwrap(),
            Some(Value::Int(7))
        );
        assert!(vm.stack.is_empty());
        assert_eq!(api.input_descriptor_states.get(&11), Some(&2));
        assert_eq!(api.input_descriptor_states.get(&13), Some(&6));
    }

    #[test]
    fn sys80_12_rejects_unterminated_descriptor_array() {
        let mut vm = Vm::new();
        // The last two words of the global window: the terminator would lie
        // past it.
        let descriptors = LOCAL_MEMORY_BASE - 8;
        vm.write_int(descriptors, 2, 11).unwrap();
        vm.write_int(descriptors + 4, 2, 13).unwrap();
        vm.stack.push(Value::Ptr(descriptors));

        let error = vm
            .try_builtin_sys_with_api(&mut SchedulingApi::default(), 0x80, 0x12)
            .unwrap_err();
        // Reading past the window is an invalid pointer before the
        // terminator check can run.
        assert!(error.to_string().contains("無効なポインタ"), "{error}");
    }

    #[test]
    fn fullscreen_hotkey_descriptors_are_decoded_before_host_dispatch() {
        let mut vm = Vm::new();
        let descriptor_ptr = 0x2b00u32;
        for (index, value) in [37_u32, 13, 0].into_iter().enumerate() {
            vm.write_int(descriptor_ptr + index as u32 * 4, 2, value)
                .unwrap();
        }
        vm.stack.extend([Value::Int(1), Value::Ptr(descriptor_ptr)]);
        let mut api = SchedulingApi::default();

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x62).unwrap(),
            Some(Value::None)
        );
        assert!(api.fullscreen_hotkeys_enabled);
        assert_eq!(api.fullscreen_hotkeys, [37, 13]);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn disabling_fullscreen_hotkeys_preserves_the_target_descriptor_table() {
        let mut vm = Vm::new();
        let mut api = SchedulingApi {
            fullscreen_hotkeys_enabled: true,
            fullscreen_hotkeys: vec![37, 13],
            ..SchedulingApi::default()
        };
        // The disabled target path stores the flag but never dereferences or
        // replaces the descriptor list. Use an intentionally invalid pointer
        // to lock that behavior down.
        vm.stack.extend([Value::Int(0), Value::Ptr(0xffff_fffc)]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x62).unwrap(),
            Some(Value::None)
        );
        assert!(!api.fullscreen_hotkeys_enabled);
        assert_eq!(api.fullscreen_hotkeys, [37, 13]);
    }

    #[test]
    fn fullscreen_hotkey_descriptor_list_rejects_sixteen_nonzero_entries() {
        let mut vm = Vm::new();
        let descriptor_ptr = 0x2c00u32;
        for index in 0..16u32 {
            vm.write_int(descriptor_ptr + index * 4, 2, index + 1)
                .unwrap();
        }
        vm.stack.extend([Value::Int(1), Value::Ptr(descriptor_ptr)]);
        let mut api = SchedulingApi::default();

        let error = vm
            .try_builtin_sys_with_api(&mut api, 0x80, 0x62)
            .unwrap_err();
        assert!(
            error
                .to_string()
                .contains("キーの配列が長過ぎます")
        );
        assert!(!api.fullscreen_hotkeys_enabled);
        assert!(api.fullscreen_hotkeys.is_empty());
    }

    #[test]
    fn resource_name_intern_returns_constant_success_and_lookup_results() {
        let mut vm = Vm::new();

        vm.stack.push(Value::Str("mm05_100001".into()));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x84)
                .unwrap(),
            Some(Value::Int(1))
        );
        vm.stack.push(Value::Str("mm05_100001".into()));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x84)
                .unwrap(),
            Some(Value::Int(1))
        );
        vm.stack.push(Value::Str("mm05_100002".into()));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x84)
                .unwrap(),
            Some(Value::Int(1))
        );

        vm.stack.push(Value::Str("mm05_100001".into()));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x85)
                .unwrap(),
            Some(Value::Int(1))
        );
        vm.stack.push(Value::Str("missing".into()));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x85)
                .unwrap(),
            Some(Value::Int(0))
        );
    }

    #[test]
    fn resource_name_registry_is_separate_from_indexed_string_namespaces() {
        let mut vm = Vm::new();
        let source = 0x3000;
        vm.write_c_string(source, "shared-name").unwrap();

        vm.stack
            .extend([Value::Int(77), Value::Int(1), Value::Ptr(source)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xda).unwrap(), Some(Value::Int(1)));

        vm.stack.push(Value::Str("shared-name".into()));
        assert_eq!(vm.try_builtin_sys(0x80, 0x85).unwrap(), Some(Value::Int(0)));
        vm.stack.push(Value::Str("shared-name".into()));
        assert_eq!(vm.try_builtin_sys(0x80, 0x84).unwrap(), Some(Value::Int(1)));
        vm.stack.push(Value::Str("shared-name".into()));
        assert_eq!(vm.try_builtin_sys(0x80, 0x85).unwrap(), Some(Value::Int(1)));
    }

    #[test]
    fn structured_history_is_bounded_and_read_newest_first() {
        let mut vm = Vm::new();
        vm.stack.push(Value::Int(2));
        assert_eq!(vm.try_builtin_sys(0x80, 0x90).unwrap(), Some(Value::None));

        for value in [1, 2, 3] {
            vm.stack
                .extend((0..9).map(|offset| Value::Int(value * 10 + offset)));
            vm.stack.extend([
                Value::Str(format!("short1-{value}")),
                Value::Str(format!("short2-{value}")),
                Value::Str(format!("short3-{value}")),
                Value::Str(format!("text-{value}")),
            ]);
            assert_eq!(vm.try_builtin_sys(0x80, 0x94).unwrap(), Some(Value::None));
        }

        assert_eq!(vm.try_builtin_sys(0x80, 0x91).unwrap(), Some(Value::Int(2)));
        let destination = 0x5000;
        vm.stack.extend([Value::Ptr(destination), Value::Int(0)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x95).unwrap(), Some(Value::Int(1)));
        assert_eq!(vm.read_int(destination, 2).unwrap(), 30);
        assert_eq!(vm.read_c_string(destination + 256).unwrap(), "text-3");

        vm.stack.extend([Value::Ptr(destination), Value::Int(1)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x95).unwrap(), Some(Value::Int(1)));
        assert_eq!(vm.read_int(destination, 2).unwrap(), 20);
        assert_eq!(vm.read_c_string(destination + 160).unwrap(), "short1-2");
    }

    #[test]
    fn exclusion_procedure_waits_for_capacity_and_preserves_priority_queue() {
        let mut first = Vm::new();
        first.thread.set_thread_id(11);
        first
            .stack
            .extend([Value::Str("save".into()), Value::Int(1)]);
        assert_eq!(
            first.try_builtin_sys(0x80, 0xB0).unwrap(),
            Some(Value::Int(1))
        );

        first
            .stack
            .extend([Value::Str("save".into()), Value::Int(10)]);
        assert_eq!(
            first.try_builtin_sys(0x80, 0xB4).unwrap(),
            Some(Value::None)
        );
        assert_eq!(first.poll_current_procedure(&mut TraceApi, false), None);
        assert_eq!(first.stack.pop(), Some(Value::Int(0)));

        let mut second = Vm::new();
        second.thread.set_thread_id(12);
        second.system80_shared = Arc::clone(&first.system80_shared);
        second
            .stack
            .extend([Value::Str("save".into()), Value::Int(20)]);
        assert_eq!(
            second.try_builtin_sys(0x80, 0xB4).unwrap(),
            Some(Value::None)
        );
        assert_eq!(
            second.poll_current_procedure(&mut TraceApi, false),
            Some(VmStopReason::WaitingForProcedure)
        );

        first.stack.push(Value::Str("save".into()));
        assert_eq!(
            first.try_builtin_sys(0x80, 0xB5).unwrap(),
            Some(Value::Int(0))
        );
        assert_eq!(second.poll_current_procedure(&mut TraceApi, false), None);
        assert_eq!(second.stack.pop(), Some(Value::Int(0)));

        first.stack.push(Value::Str("save".into()));
        assert_eq!(
            first.try_builtin_sys(0x80, 0xB1).unwrap(),
            Some(Value::Int(system80_state::NATIVE_BUSY))
        );
        second.stack.push(Value::Str("save".into()));
        assert_eq!(
            second.try_builtin_sys(0x80, 0xB5).unwrap(),
            Some(Value::Int(0))
        );
        first.stack.push(Value::Str("save".into()));
        assert_eq!(
            first.try_builtin_sys(0x80, 0xB1).unwrap(),
            Some(Value::Int(0))
        );
    }

    #[test]
    fn native_string_hash_table_round_trips_script_names() {
        let mut vm = Vm::new();
        let source = 0x3000;
        let destination = 0x3100;
        vm.write_c_string(source, "SetupForOmake").unwrap();

        vm.stack
            .extend([Value::Int(77), Value::Int(1), Value::Ptr(source)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xda).unwrap(), Some(Value::Int(1)));

        vm.stack
            .extend([Value::Ptr(destination), Value::Int(77), Value::Int(0)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xdd).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_c_string(destination).unwrap(), "SetupForOmake");
    }

    #[test]
    fn indexed_string_namespace_uses_target_error_codes_and_nul_payloads() {
        let mut vm = Vm::new();
        let source = 0x3000;
        let destination = 0x3100;
        vm.write_c_string(source, "日本語").unwrap();

        vm.stack
            .extend([Value::Int(77), Value::Int(1), Value::Ptr(source)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xda).unwrap(), Some(Value::Int(1)));

        vm.stack
            .extend([Value::Ptr(destination), Value::Int(77), Value::Int(0)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xdd).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_c_string(destination).unwrap(), "日本語");
        let encoded = encoding_rs::SHIFT_JIS.encode("日本語").0;
        let range = vm.resolve_range(destination, encoded.len() + 1).unwrap();
        assert_eq!(
            &vm.memory[range.start..range.start + encoded.len()],
            encoded.as_ref()
        );
        assert_eq!(vm.memory[range.end - 1], 0);

        vm.stack
            .extend([Value::Ptr(destination), Value::Int(999), Value::Int(0)]);
        assert_eq!(
            vm.try_builtin_sys(0x80, 0xdd).unwrap(),
            Some(Value::Int(i32::MIN + 1))
        );

        vm.stack
            .extend([Value::Ptr(destination), Value::Int(77), Value::Int(1)]);
        assert_eq!(
            vm.try_builtin_sys(0x80, 0xdd).unwrap(),
            Some(Value::Int(i32::MIN + 2))
        );
    }

    #[test]
    fn save_data_integrity_switch_preserves_the_native_integer_value() {
        let mut vm = Vm::new();

        vm.stack.push(Value::Int(7));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x74)
                .unwrap(),
            Some(Value::None)
        );
        assert_eq!(vm.save_data_integrity_enabled, 7);

        vm.stack.push(Value::Int(0));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x80, 0x74)
                .unwrap(),
            Some(Value::None)
        );
        assert_eq!(vm.save_data_integrity_enabled, 0);
    }

    #[test]
    fn native_string_table_serialization_matches_namespace_contents() {
        let mut vm = Vm::new();
        let source = 0x3000;
        let destination = 0x3100;
        vm.write_c_string(source, "first").unwrap();
        vm.write_c_string(source + 6, "second").unwrap();

        vm.stack
            .extend([Value::Int(0), Value::Int(2), Value::Ptr(source)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0xda).unwrap(), Some(Value::Int(1)));

        vm.stack.extend([Value::Ptr(0), Value::Int(0)]);
        assert_eq!(
            vm.try_builtin_sys(0x80, 0xdb).unwrap(),
            Some(Value::Int(13))
        );

        vm.stack.extend([Value::Ptr(destination), Value::Int(0)]);
        assert_eq!(
            vm.try_builtin_sys(0x80, 0xdb).unwrap(),
            Some(Value::Int(13))
        );
        assert_eq!(vm.read_c_string(destination).unwrap(), "first");
        assert_eq!(vm.read_c_string(destination + 6).unwrap(), "second");
    }

    #[test]
    fn native_operand_stack_push_wraps_the_pointer_to_slot_zero() {
        let mut vm = Vm::new();
        vm.stack
            .extend((0..(super::OPERAND_STACK_CAPACITY + 3)).map(|value| Value::Int(value as i32)));
        vm.trim_operand_stack();

        assert_eq!(vm.stack.len(), 3);
        assert_eq!(vm.stack.first(), Some(&Value::Int(4096)));
        assert_eq!(vm.stack.last(), Some(&Value::Int(4098)));
    }

    #[test]
    fn native_operand_stack_empty_pop_wraps_to_the_last_stale_slot() {
        let mut vm = Vm::new();
        vm.operand_slots[super::OPERAND_STACK_CAPACITY - 1] = Value::Int(777);

        assert_eq!(vm.pop_value().unwrap(), Value::Int(777));
        assert_eq!(vm.stack.len(), super::OPERAND_STACK_CAPACITY - 1);
        assert_eq!(vm.pop_value().unwrap(), Value::Int(0));
        assert_eq!(vm.stack.len(), super::OPERAND_STACK_CAPACITY - 2);
    }

    #[test]
    fn native_double_not_opcodes_match_testcase_pe_dispatch_table() {
        // testcase's 0x506300 opcode table maps 0x38 to sub_473E70 (AND)
        // and 0x39 to sub_473EB0 (OR). The older openbgi reference differs.
        let and_instruction = test_instruction(0x10, 0x38, "boolean_and", vec![0x38], Vec::new());
        let or_instruction = test_instruction(0x11, 0x39, "boolean_or", vec![0x39], Vec::new());
        let mut vm = Vm::new();

        vm.stack.extend([Value::Int(0), Value::Int(-1)]);
        vm.dispatch(&and_instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(0)]);

        vm.stack.extend([Value::Int(0), Value::Int(-1)]);
        vm.dispatch(&or_instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(0), Value::Int(1)]);
    }

    #[test]
    fn memcmp_opcode_returns_boolean_equality() {
        let instruction = test_instruction(0x10, 0x63, "memory_equal", vec![0x63], Vec::new());
        let mut vm = Vm::new();
        vm.memory[0x100..0x103].copy_from_slice(b"mf2");
        vm.memory[0x200..0x203].copy_from_slice(b"mf2");

        vm.stack
            .extend([Value::Ptr(0x100), Value::Ptr(0x200), Value::Int(3)]);
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(1)]);

        vm.memory[0x202] = b'3';
        vm.stack
            .extend([Value::Ptr(0x100), Value::Ptr(0x200), Value::Int(3)]);
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(1), Value::Int(0)]);
    }

    #[test]
    fn memcmp_opcode_compares_string_literals_as_shift_jis_c_strings() {
        let instruction = test_instruction(0x10, 0x63, "memory_equal", vec![0x63], Vec::new());
        let mut vm = Vm::new();
        let magic = "BurikoCompiledScriptVer1.00";
        let size = magic.len() + 1;
        vm.memory[0x100..0x100 + magic.len()].copy_from_slice(magic.as_bytes());
        vm.memory[0x100 + magic.len()] = 0;

        vm.stack.extend([
            Value::Ptr(0x100),
            Value::Str(magic.into()),
            Value::Int(size as i32),
        ]);
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn native_string_find_returns_shift_jis_byte_offset() {
        let instruction = test_instruction(0x10, 0x66, "strfind", vec![0x66], Vec::new());
        let mut vm = Vm::new();

        vm.stack
            .extend([Value::Str("前:後".into()), Value::Str(":".into())]);
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(2)]);

        vm.stack
            .extend([Value::Str("Main".into()), Value::Str(":".into())]);
        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(2), Value::Int(-1)]);
    }

    #[test]
    fn script_errors_are_captured_with_the_target_header() {
        let program = BpProgram {
            script_name: Some("err-test".into()),
            functions: std::sync::Arc::new(Vec::new()),
            strings: std::sync::Arc::new(Vec::new()),
            instructions: std::sync::Arc::new(vec![
                test_instruction(0x10, 0x00, "push_byte", vec![0x00, 0x00], vec![BpOperand::U8(0)]),
                test_instruction(0x12, 0x14, "jmp", vec![0x14], Vec::new()),
            ]),
            labels: std::sync::Arc::new([(0x10, 0), (0x12, 1)].into_iter().collect()),
            warnings: Vec::new(),
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        };
        let mut vm = Vm::new();
        vm.system81_shared.lock().unwrap().error_capture_mode = 1;
        let report = vm.run(&program, &mut SchedulingApi::default(), &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Error);
        let captured = vm.system81_shared.lock().unwrap().captured_error.clone().unwrap();
        assert!(captured.starts_with("Thread [ "), "{captured}");
        assert!(captured.contains("Program [ err-test ] , IP [ $00000002 ] , Instruction [ $14 ]"), "{captured}");
        assert!(captured.contains("jmp to null target"), "{captured}");
    }

    #[test]
    fn captured_error_query_returns_zero_until_an_error_is_captured() {
        let query = test_instruction(0x10, 0x81, "sys2", vec![0x81, 0x6b], Vec::new());
        let mut vm = Vm::new();
        vm.memory[0x100] = b'x';
        vm.stack.push(Value::Ptr(0x100));
        vm.dispatch(&query, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(vm.memory[0x100], b'x');
        vm.stack.clear();

        vm.system81_shared.lock().unwrap().captured_error = Some("err".into());
        vm.stack.push(Value::Ptr(0x100));
        vm.dispatch(&query, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(4)]);
        assert_eq!(vm.read_c_string(0x100).unwrap(), "err");
    }

    #[test]
    fn string_opcodes_measure_raw_bytes_behind_lossy_shadows() {
        // GBK "恭介" is not valid Shift-JIS; scenario preprocessing shadows it
        // with a lossy decoding while memory keeps the stored bytes.
        let gbk = [0xb9, 0xa7, 0xbd, 0xe9];
        let mut vm = Vm::new();
        vm.memory[0x100..0x104].copy_from_slice(&gbk);
        vm.memory[0x104] = 0;
        let lossy = encoding_rs::SHIFT_JIS.decode(&gbk).0.into_owned();
        vm.mem_values.insert(vm.value_key(0x100), Value::Str(lossy));

        let strlen = test_instruction(0x10, 0x68, "strlen", vec![0x68], Vec::new());
        vm.stack.push(Value::Ptr(0x100));
        vm.dispatch(&strlen, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(4)]);
        vm.stack.clear();

        let strconcat = test_instruction(0x10, 0x6b, "strconcat", vec![0x6b], Vec::new());
        vm.stack
            .extend([Value::Ptr(0x200), Value::Ptr(0x100), Value::Str("!".into())]);
        vm.dispatch(&strconcat, &mut TraceApi).unwrap();
        assert_eq!(&vm.memory[0x200..0x206], &[0xb9, 0xa7, 0xbd, 0xe9, b'!', 0]);

        let streq = test_instruction(0x10, 0x69, "streq", vec![0x69], Vec::new());
        vm.stack.extend([Value::Ptr(0x100), Value::Ptr(0x200)]);
        vm.dispatch(&streq, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn native_fixed_block_find_returns_block_index() {
        let instruction = test_instruction(0x10, 0x65, "memfind", vec![0x65], Vec::new());
        let mut vm = Vm::new();
        vm.memory[0x100..0x10c].copy_from_slice(b"aaaabbbbcccc");
        vm.memory[0x200..0x204].copy_from_slice(b"bbbb");
        vm.stack.extend([
            Value::Ptr(0x100),
            Value::Int(4),
            Value::Int(3),
            Value::Ptr(0x200),
        ]);

        vm.dispatch(&instruction, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn native_vector_math_opcodes_match_fixed_degree_contract() {
        let atan2 = test_instruction(0x10, 0x43, "atan2", vec![0x43], Vec::new());
        let length = test_instruction(0x11, 0x44, "vec3_length", vec![0x44], Vec::new());
        let mut vm = Vm::new();

        vm.stack.extend([Value::Int(0), Value::Int(-1)]);
        vm.dispatch(&atan2, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(270 * 65_536)]);

        vm.stack
            .extend([Value::Int(3), Value::Int(4), Value::Int(12)]);
        vm.dispatch(&length, &mut TraceApi).unwrap();
        assert_eq!(vm.stack, [Value::Int(270 * 65_536), Value::Int(13)]);
    }

    #[test]
    fn load_program_adds_a_module_without_creating_a_native_thread() {
        let mut vm = Vm::new();
        let root = empty_loaded_program("root._bp".into());
        let expected_base = Vm::program_target_code_size(&root);
        vm.start(&root);
        vm.stack.extend([
            Value::Str("sysprg.arc".into()),
            Value::Str("worker._bp".into()),
        ]);
        let instruction = test_instruction(0, 0x80, "sys1", vec![0x80, 0x40], Vec::new());
        let program = empty_loaded_program("worker._bp".into());

        vm.dispatch(&instruction, &mut MediationApi { program })
            .unwrap();

        assert!(vm.async_tasks.is_empty());
        assert_eq!(vm.stack.last(), Some(&Value::Int(expected_base as i32)));
        assert_eq!(vm.target_loaded_programs.len(), 2);
    }

    #[test]
    fn integer_call_target_prefers_loaded_code_region_over_local_label_collision() {
        let mut vm = Vm::new();
        let mut root = empty_loaded_program("root._bp".into());
        let expected_base = Vm::program_target_code_size(&root);
        std::sync::Arc::make_mut(&mut root.labels).insert(expected_base, 0);
        vm.start(&root);
        let worker = empty_loaded_program("worker._bp".into());

        assert_eq!(vm.append_target_loaded_program(worker), expected_base);
        assert_eq!(
            vm.resolve_indirect_call_target(0, expected_base),
            (1, Vm::program_parser_code_start(&vm.programs[1]))
        );
    }

    #[test]
    fn freed_module_code_stays_callable_until_a_later_module_reuses_it() {
        let mut vm = Vm::new();
        vm.start(&empty_loaded_program("root._bp".into()));
        let environ = vm.append_target_loaded_program(empty_loaded_program("environ._bp".into()));
        let entry = (1, Vm::program_parser_code_start(&vm.programs[1]));
        vm.free_last_target_program(false);

        assert_eq!(vm.resolve_indirect_call_target(0, environ), entry);

        assert_eq!(
            vm.append_target_loaded_program(empty_loaded_program("bitmap._bp".into())),
            environ
        );
        assert_eq!(
            vm.resolve_indirect_call_target(0, environ),
            (2, Vm::program_parser_code_start(&vm.programs[2]))
        );
    }

    #[test]
    fn load_program_ex_creates_an_immediately_scheduled_native_thread() {
        let mut vm = Vm::new();
        let root = empty_loaded_program("root._bp".into());
        vm.start(&root);
        vm.stack.extend([
            Value::Str("sysprg.arc".into()),
            Value::Str("worker._bp".into()),
            Value::Int(1024),
            Value::Int(8192),
            Value::Int(8192),
        ]);
        let instruction = test_instruction(0, 0x80, "sys1", vec![0x80, 0x44], Vec::new());
        let program = empty_loaded_program("worker._bp".into());

        vm.dispatch(&instruction, &mut MediationApi { program })
            .unwrap();

        assert_eq!(vm.async_tasks.len(), 1);
        let Some(Value::Int(thread_id)) = vm.stack.last().cloned() else {
            panic!("LoadProgramThread did not return an integer CThread id");
        };
        assert_ne!(thread_id, 0);
        assert!(vm.native_thread_exists(thread_id));
        assert!(vm.async_tasks[0].runnable);
        let handle = Value::Int(thread_id);
        assert!(vm.post_async_program_message(handle.clone(), Value::Int(0x40ff_ffff), false));
        assert!(vm.async_tasks[0].runnable);
        assert!(vm.switch_to_async_program(handle, false).0);
        assert!(vm.async_tasks[0].runnable);
    }

    #[test]
    fn terminate_interpreter_in_a_child_thread_ends_the_root() {
        // endsys._bp quits with Sys80:6A from a child thread; status 6 ends
        // the whole interpreter, not only that thread.
        #[rustfmt::skip]
        let code = vec![
            0x02, 4, 0, 0, 0,          // operand slots
            0x02, 0, 1, 0, 0,          // code bytes 0x100
            0x02, 0, 1, 0, 0,          // data bytes 0x100
            0x06, 0x0B, 0x00,          // 0x0F push_offset 0x1A (child entry)
            0x81, 0x44,                // 0x12 Sys81:44
            0x80, 0x5F,                // 0x14 Sys80:5F yield
            0x06, 0xFE, 0xFF,          // 0x16 push_offset 0x14
            0x14,                      // 0x19 jmp
            0x80, 0x6A,                // 0x1A child: Sys80:6A
            0x17,                      // ret
        ];
        let program = ethornell_script::parse_bp_program(Some("quit-root._bp".into()), &code);
        let mut vm = Vm::new();
        vm.start(&program);
        let mut report = vm.run_loaded(&mut TraceApi, &VmRunOptions::default());
        for _ in 0..4 {
            if vm.halted {
                break;
            }
            report = vm.run_loaded(&mut TraceApi, &VmRunOptions::default());
        }
        assert!(vm.halted, "{report:?}");
        assert_eq!(report.stop_reason, VmStopReason::InterpreterTerminated, "{report:?}");
    }

    #[test]
    fn child_thread_runs_parent_code_in_a_block_of_the_parent_regions() {
        #[rustfmt::skip]
        let code = vec![
            0x02, 4, 0, 0, 0,          // operand slots
            0x02, 0, 1, 0, 0,          // code bytes 0x100
            0x02, 0, 1, 0, 0,          // data bytes 0x100
            0x06, 0x06, 0x00,          // push_offset 0x15 (child entry)
            0x81, 0x44,                // Sys81:44
            0x17,                      // ret
            // child entry 0x15
            0x02, 0x10, 0, 0, 0, 0x00, 42, 0x09, 2,   // [0x10] = 42
            0x04, 0, 0, 0x00, 7, 0x09, 2,             // [frame + 0] = 7
            0x17,                                     // ret at the block base ends it
        ];
        let program = ethornell_script::parse_bp_program(Some("child-root._bp".into()), &code);
        let mut vm = Vm::new();
        vm.start(&program);
        let report = vm.run_loaded(&mut TraceApi, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Completed, "{report:?}");
        assert_eq!(vm.async_tasks.len(), 1);
        let child = &vm.async_tasks[0].vm;
        // The blocks sit at the top of the 0x80000 / 0x40000 regions.
        assert_eq!(child.mem_ptr, 0x3FF00);
        assert_eq!(child.thread.frame_region_base(), 0x3FF00);
        assert_eq!(vm.data_region_limit(), 0x3FF00);
        vm.pump_async_programs(&mut TraceApi, false, &VmRunOptions::default());
        assert!(vm.async_tasks.is_empty());
        assert_eq!(vm.read_int(0x10, 2).unwrap(), 42);
        assert_eq!(vm.read_int(crate::FRAME_SEGMENT | 0x3FF00, 2).unwrap(), 7);
        // sub_445850 drops the blocks but not the lowered limit.
        let regions = vm.region_reservations.lock().unwrap();
        assert_eq!(regions.data_free_limit(), 0x3FF00);
        drop(regions);
        // A block that does not fit, and an IP past the child's code block.
        let instruction = test_instruction(0, 0x81, "sys2", vec![0x81, 0x44], Vec::new());
        vm.stack.extend([Value::Int(4), Value::Int(0x100), Value::Int(0x40001), Value::Int(0x15)]);
        let error = vm.dispatch(&instruction, &mut TraceApi).unwrap_err();
        assert!(error.to_string().contains("[ 262145 ] のスタック領域"), "{error}");
        vm.stack.extend([Value::Int(4), Value::Int(0x100), Value::Int(0x10), Value::Int(0x80000)]);
        let error = vm.dispatch(&instruction, &mut TraceApi).unwrap_err();
        assert!(error.to_string().contains("$80000"), "{error}");
    }

    #[test]
    fn identical_program_images_create_distinct_native_threads() {
        let mut vm = Vm::new();
        vm.start(&empty_loaded_program("root._bp".into()));
        let program = Value::Program(Arc::new(empty_loaded_program("worker._bp".into())));

        let first = vm
            .start_async_program_with_args(program.clone(), Vec::new(), false)
            .unwrap();
        let second = vm
            .start_async_program_with_args(program, Vec::new(), false)
            .unwrap();

        assert_ne!(first, second);
        assert_eq!(vm.async_tasks.len(), 2);
        assert!(vm.native_thread_exists(first));
        assert!(vm.native_thread_exists(second));
    }

    #[test]
    fn free_program_module_returns_remaining_target_module_count() {
        let mut vm = Vm::new();
        let root = empty_loaded_program("root._bp".into());
        vm.start(&root);
        let _ = vm.append_target_loaded_program(empty_loaded_program("worker._bp".into()));
        assert_eq!(vm.target_loaded_programs.len(), 2);

        let (remaining, freed) = vm.free_last_target_program(false);
        assert_eq!(remaining, 1);
        assert!(freed.is_some());
        assert_eq!(vm.target_loaded_programs, [0]);
    }

    #[test]
    fn freeing_the_last_module_or_loading_a_missing_one_is_fatal() {
        let mut vm = Vm::new();
        vm.start(&empty_loaded_program("root._bp".into()));
        // Only the main module is left: sub_488CD0 reports a script error.
        assert!(
            vm.dispatch_program_thread_opcode(
                &mut TraceApi,
                native_call::opcodes::SYS_FREE_LAST_PROGRAM_MODULE,
                false,
            )
            .is_err()
        );
        // With no record at all the handler pushes 0x80000001.
        assert_eq!(
            vm.dispatch_program_thread_opcode(
                &mut TraceApi,
                native_call::opcodes::SYS_FREE_LAST_PROGRAM_MODULE,
                false,
            )
            .unwrap(),
            Some(Value::Int(i32::MIN + 1))
        );
        vm.stack
            .extend([Value::Str("none.arc".into()), Value::Str("none._bp".into())]);
        assert!(
            vm.dispatch_program_thread_opcode(
                &mut TraceApi,
                native_call::opcodes::SYS_LOAD_PROGRAM_MODULE,
                false,
            )
            .is_err()
        );
        // sub_48D080 reports an unreadable thread file as well.
        vm.stack.extend([
            Value::Str("none.arc".into()),
            Value::Str("none._bp".into()),
            Value::Int(256),
            Value::Int(0x10000),
            Value::Int(0x10000),
        ]);
        assert!(
            vm.dispatch_program_thread_opcode(
                &mut TraceApi,
                native_call::opcodes::SYS_LOAD_PROGRAM_THREAD,
                false,
            )
            .is_err()
        );
    }

    #[test]
    fn configured_secondary_root_participates_in_resource_search() {
        let mut vm = Vm::new();
        vm.secondary_resource_root = Some("/Volumes/TayutamaData/".into());
        let mut api = RootFileBytesApi {
            expected_file: join_native_path("/Volumes/TayutamaData/", "marker.dat"),
            bytes: b"installed".to_vec(),
            primary_root: None,
        };

        assert_eq!(
            vm.load_resource_bytes_with_search(&mut api, "", "marker.dat"),
            Some((b"installed".to_vec(), ResourceLoadOrigin::LooseFile))
        );
    }

    #[test]
    fn sys80_3d_returns_the_target_primary_root_and_status() {
        let mut vm = Vm::new();
        let destination = 0x2000;
        vm.stack.extend([Value::Ptr(destination), Value::Int(0)]);
        let instruction = test_instruction(0, 0x80, "sys1", vec![0x80, 0x3D], Vec::new());
        let mut api = RootFileBytesApi {
            expected_file: String::new(),
            bytes: Vec::new(),
            primary_root: Some("/portable/native-root".into()),
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.stack.last(), Some(&Value::Int(1)));
        assert_eq!(
            vm.read_c_string(destination).unwrap(),
            ensure_trailing_separator("/portable/native-root")
        );
    }

    #[test]
    fn sys80_34_and_35_preserve_archive_and_file_pop_order() {
        let mut vm = Vm::new();
        let exists_instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x34], vec![BpOperand::U8(0x34)]);
        let size_instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x35], vec![BpOperand::U8(0x35)]);
        let mut exists_api = ArchiveBytesApi {
            expected_archive: "data01xxx.arc".into(),
            expected_file: "main".into(),
            bytes: b"scenario".to_vec(),
            calls: Vec::new(),
        };
        vm.stack.extend([
            Value::Str("data01xxx.arc".into()),
            Value::Str("main".into()),
        ]);
        vm.dispatch(&exists_instruction, &mut exists_api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(
            exists_api.calls.first(),
            Some(&("data01xxx.arc".to_string(), "main".to_string()))
        );

        let mut size_api = ArchiveBytesApi {
            expected_archive: "data01xxx.arc".into(),
            expected_file: "main".into(),
            bytes: b"scenario".to_vec(),
            calls: Vec::new(),
        };
        vm.stack.extend([
            Value::Str("data01xxx.arc".into()),
            Value::Str("main".into()),
        ]);
        vm.dispatch(&size_instruction, &mut size_api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(8)));
        assert_eq!(
            size_api.calls.first(),
            Some(&("data01xxx.arc".to_string(), "main".to_string()))
        );

        let mut missing_api = ArchiveBytesApi {
            expected_archive: "other.arc".into(),
            expected_file: "missing".into(),
            bytes: Vec::new(),
            calls: Vec::new(),
        };
        vm.stack.extend([
            Value::Str("data01xxx.arc".into()),
            Value::Str("missing".into()),
        ]);
        vm.dispatch(&size_instruction, &mut missing_api).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));

        // A null archive looks for the loose file below the primary root.
        let root = ensure_trailing_separator("/game");
        let mut null_archive_api = RootFileBytesApi {
            expected_file: format!("{root}marker.dat"),
            bytes: b"marker".to_vec(),
            primary_root: Some("/game".into()),
        };
        vm.stack
            .extend([Value::Int(0), Value::Str("marker.dat".into())]);
        vm.dispatch(&exists_instruction, &mut null_archive_api)
            .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
    }

    #[test]
    fn sys80_30_decodes_resource_into_caller_buffer_and_returns_size() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x30], vec![BpOperand::U8(0x30)]);
        let destination = 0x1800;
        let decoded = [
            0x1c, 0x00, 0x00, 0x00, 0x99, 0x3a, 0x00, 0x00, b't', b'a', b'y', b'u', b't', b'a',
            b'm', b'a', b'2', 0, 0x7f, 0x55,
        ];
        let mut vm = Vm::new();
        vm.memory[destination as usize..destination as usize + 32].fill(0xcc);
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("data01xxx.arc".into()),
            Value::Str("StringsDB".into()),
        ]);
        let mut api = ArchiveBytesApi {
            expected_archive: "data01xxx.arc".into(),
            expected_file: "StringsDB".into(),
            bytes: decoded.to_vec(),
            calls: Vec::new(),
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.stack, [Value::Int(decoded.len() as i32)]);
        assert_eq!(
            &vm.memory[destination as usize..destination as usize + decoded.len()],
            decoded.as_slice()
        );
        assert_eq!(
            api.calls,
            [("data01xxx.arc".to_string(), "StringsDB".to_string())]
        );
    }

    #[test]
    fn sys80_30_clears_target_header_then_fails_when_resource_is_missing() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x30], vec![BpOperand::U8(0x30)]);
        let destination = 0x1900;
        let mut vm = Vm::new();
        vm.memory[destination as usize..destination as usize + 16].fill(0xcc);
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("missing.arc".into()),
            Value::Str("missing.bin".into()),
        ]);
        let mut api = ArchiveBytesApi {
            expected_archive: "other.arc".into(),
            expected_file: "other.bin".into(),
            bytes: Vec::new(),
            calls: Vec::new(),
        };

        // sub_466010 -> sub_465F80: without a media root a missing file is
        // a script error.
        let error = vm.dispatch(&instruction, &mut api).unwrap_err();
        assert!(error.to_string().contains("[ missing.arc : missing.bin ]"));
        assert_eq!(
            &vm.memory[destination as usize..destination as usize + 16],
            &[0; 16]
        );
    }

    #[test]
    fn exclusive_child_owner_suspends_the_root_interpreter() {
        let mut vm=Vm::new();
        vm.start(&empty_loaded_program("exclusive-root._bp".into()));
        vm.exclusive_thread_id=Some(vm.thread.thread_id()+1);
        let report=vm.run_loaded(&mut TraceApi,&VmRunOptions::default());
        assert_eq!(report.stop_reason,VmStopReason::WaitingForProcedure);
        assert!(!vm.halted);
        vm.exclusive_thread_id=None;
        let report=vm.run_loaded(&mut TraceApi,&VmRunOptions::default());
        assert_eq!(report.stop_reason,VmStopReason::Completed);
    }

    #[test]
    fn sys80_cf_resolves_tagged_raw_input_and_returns_decoded_length() {
        let mut vm=Vm::new();
        let source=crate::CODE_SEGMENT|0x1000;
        let destination=0x2000u32;
        let at = vm.memory_addr(source) as usize; vm.memory[at..at + 4].copy_from_slice(b"data");
        vm.stack.extend([Value::Ptr(destination),Value::Ptr(source),Value::Int(4)]);
        let instruction=test_instruction(0,0x80,"sys1",vec![0x80,0xcf],Vec::new());
        vm.dispatch(&instruction,&mut TraceApi).unwrap();
        assert!(vm.stack.is_empty());
        assert_eq!(&vm.memory[destination as usize..destination as usize+4],b"data");
        assert_eq!(vm.poll_current_procedure(&mut TraceApi,false),None);
        assert_eq!(vm.stack,[Value::Int(4)]);
    }

    #[test]
    fn sys80_c1_uses_target_source_then_destination_pop_order() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0xc1], vec![BpOperand::U8(0xc1)]);
        let raw_source = 0x1a00;
        let encoded_source = 0x1b00;
        let decoded_destination = 0x1d00;
        let raw = b"target-confirmed-sdc-pop-order";
        let mut vm = Vm::new();
        vm.memory[raw_source as usize..raw_source as usize + raw.len()].copy_from_slice(raw);
        let encoded_len = vm
            .encode_user_data_buffer(encoded_source, raw_source, raw.len() as i32)
            .unwrap();
        assert!(encoded_len > 32);
        vm.memory[decoded_destination as usize..decoded_destination as usize + raw.len()]
            .fill(0xcc);
        vm.stack
            .extend([Value::Ptr(decoded_destination), Value::Ptr(encoded_source)]);

        vm.dispatch(&instruction, &mut TraceApi).unwrap();

        assert_eq!(vm.stack, [Value::Int(raw.len() as i32)]);
        assert_eq!(
            &vm.memory[decoded_destination as usize..decoded_destination as usize + raw.len()],
            raw
        );
    }

    #[test]
    fn read_file_range_copies_requested_slice_and_reports_target_status() {
        let mut vm = Vm::new();
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str(String::new()),
            Value::Str("sample.bin".into()),
            Value::Int(2),
            Value::Int(3),
        ]);
        let mut api = RootFileBytesApi {
            expected_file: format!("{}sample.bin", ensure_trailing_separator("/game")),
            bytes: b"abcdefgh".to_vec(),
            primary_root: Some("/game".into()),
        };

        assert_eq!(
            vm.sys80_31_read_file_range(&mut api).unwrap(),
            Value::Int(0)
        );
        assert_eq!(&vm.memory[0x1000..0x1003], b"cde");

        vm.stack.extend([
            Value::Ptr(0x1100),
            Value::Str(String::new()),
            Value::Str("sample.bin".into()),
            Value::Int(7),
            Value::Int(2),
        ]);
        assert_eq!(
            vm.sys80_31_read_file_range(&mut api).unwrap(),
            Value::Int(2)
        );
    }

    #[test]
    fn sys80_31_dispatch_reports_missing_file_instead_of_fallback_success() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x31], vec![BpOperand::U8(0x31)]);
        let mut vm = Vm::new();
        vm.stack.extend([
            Value::Ptr(0x1200),
            Value::Int(0),
            Value::Str("ScriptToExecute.bsx".into()),
            Value::Int(0),
            Value::Int(256),
        ]);
        let mut api = ArchiveBytesApi {
            expected_archive: "other.arc".into(),
            expected_file: "other.bin".into(),
            bytes: Vec::new(),
            calls: Vec::new(),
        };

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.stack, [Value::Int(1)]);
    }

    #[test]
    fn sys80_save_paths_preserve_absolute_names() {
        let mut vm = Vm::new();
        let source = vm.alloc_heap(4);
        let bytes = vm.resolve_write_range(source, 4).unwrap();
        vm.memory[bytes].copy_from_slice(b"save");
        let mut api = WriteFileApi {
            success: true,
            primary_root: Some("/game/きゃべつそふと/".into()),
            ..WriteFileApi::default()
        };
        for (file, expected) in [
            (
                "/game/きゃべつそふと/UserData/slot.sud",
                "/game/きゃべつそふと/UserData/slot.sud",
            ),
            (
                "UserData/slot.sud",
                "/game/きゃべつそふと/UserData/slot.sud",
            ),
            (
                "C:\\game\\UserData\\slot.sud",
                "C:\\game\\UserData\\slot.sud",
            ),
            ("\\\\server\\game\\slot.sud", "\\\\server\\game\\slot.sud"),
        ] {
            vm.stack = vec![Value::Str(file.into()), Value::Ptr(source), Value::Int(4)];
            assert_eq!(vm.sys80_32_write_file(&mut api).unwrap(), Value::Int(1));
            assert_eq!(
                api.writes.last(),
                Some(&(expected.into(), b"save".to_vec()))
            );
            let mut reader = RootFileBytesApi {
                expected_file: expected.into(),
                bytes: b"save".to_vec(),
                primary_root: api.primary_root.clone(),
            };
            vm.stack = vec![Value::Int(0), Value::Str(file.into())];
            assert_eq!(vm.sys80_35_file_size(&mut reader).unwrap(), Value::Int(4));
        }
    }

    #[test]
    fn sys80_32_writes_caller_bytes_and_returns_host_status() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x32], vec![BpOperand::U8(0x32)]);
        let mut vm = Vm::new();
        let mut api = WriteFileApi::default();
        let path = 0x0800_1200;
        vm.write_c_string(path, "UserData/slot.sud").unwrap();
        let source = vm.alloc_heap(8);
        let range = vm.resolve_write_range(source, 8).unwrap();
        vm.memory[range].copy_from_slice(b"\0save\xff!!");
        for success in [true, false] {
            api.success = success;
            vm.stack.extend([
                Value::Int(123),
                Value::Ptr(path),
                Value::Ptr(source),
                Value::Int(6),
            ]);
            vm.dispatch(&instruction, &mut api).unwrap();
            assert_eq!(vm.stack, [Value::Int(123), Value::Int(i32::from(success))]);
            assert_eq!(
                api.writes.last(),
                Some(&("UserData/slot.sud".into(), b"\0save\xff".to_vec()))
            );
            vm.stack.clear();
        }
        assert_eq!(api.writes.len(), 2);
    }

    #[test]
    fn sys80_32_rejects_out_of_bounds_before_writing() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x32], vec![BpOperand::U8(0x32)]);
        let mut vm = Vm::new();
        let mut api = WriteFileApi::default();
        for (source, length) in [(vm.memory.len() as u32 - 1, 2), (0x1800, -1)] {
            vm.stack.extend([
                Value::Str("UserData/slot.sud".into()),
                Value::Ptr(source),
                Value::Int(length),
            ]);
            // Past the global window (invalid pointer) or a huge length.
            assert!(matches!(
                vm.dispatch(&instruction, &mut api),
                Err(super::VmError::MemoryOutOfBounds { .. } | super::VmError::Runtime(_))
            ));
            assert!(vm.stack.is_empty());
        }
        assert!(api.writes.is_empty());
    }

    #[derive(Default)]
    struct WriteFileApi {
        success: bool,
        writes: Vec<(String, Vec<u8>)>,
        primary_root: Option<String>,
    }

    impl SysApi for WriteFileApi {
        fn primary_resource_root(&mut self) -> Option<String> {
            self.primary_root.clone()
        }

        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            panic!("WriteFileBytes must not fall through to host dispatch");
        }

        fn write_file_bytes(&mut self, path: &str, bytes: &[u8]) -> bool {
            self.writes.push((path.into(), bytes.to_vec()));
            self.success
        }
    }

    impl GraphApi for WriteFileApi {
        fn call_graph(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            unreachable!()
        }
    }

    impl SoundApi for WriteFileApi {
        fn call_sound(&mut self, _call: &mut NativeCallFrame) -> super::VmResult<Value> {
            unreachable!()
        }
    }

    #[test]
    fn sys80_53_arms_and_sys81_30_consumes_the_async_read_gate() {
        let destination = 0x1200u32;
        let mut vm = Vm::new();
        let mut api = FileBytesApi {
            bytes: b"abcdefgh".to_vec(),
        };

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x53).unwrap(),
            Some(Value::None)
        );
        assert!(vm.next_binary_or_bmv_async);

        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str(String::new()),
            Value::Str("sample.bin".into()),
            Value::Int(2),
            Value::Int(3),
        ]);
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x81, 0x30).unwrap(),
            Some(Value::None)
        );

        assert_eq!(
            &vm.memory[destination as usize..destination as usize + 3],
            b"cde"
        );
        assert!(!vm.next_binary_or_bmv_async);
        assert!(
            vm.stack.is_empty(),
            "async path must defer its status output"
        );
        let procedure = vm
            .thread
            .current_procedure()
            .expect("DCProcReadBinary was not installed");
        assert_eq!(procedure.object.class_name(), "DCProcReadBinary");
    }

    #[test]
    fn sys81_30_sync_path_returns_status_without_fabricating_a_procedure() {
        let destination = 0x1240u32;
        let mut vm = Vm::new();
        let mut api = FileBytesApi {
            bytes: b"abcdefgh".to_vec(),
        };
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str(String::new()),
            Value::Str("sample.bin".into()),
            Value::Int(1),
            Value::Int(4),
        ]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x81, 0x30).unwrap(),
            Some(Value::None)
        );
        vm.ensure_unrecovered_native_procedure_boundary(
            native_call::opcodes::SYS81_READ_RESOURCE_BINARY,
            false,
        );

        assert_eq!(
            &vm.memory[destination as usize..destination as usize + 4],
            b"bcde"
        );
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert!(vm.thread.current_procedure().is_none());
        assert_eq!(vm.thread.status(), 0);
    }

    #[test]
    fn sys81_35_uses_target_context_then_path_argument_order() {
        let mut vm = Vm::new();
        let mut api = FileBytesApi {
            bytes: b"save data".to_vec(),
        };
        vm.stack
            .extend([Value::Int(0), Value::Str("sample.bin".into())]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x81, 0x35).unwrap(),
            Some(Value::Int(9))
        );
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn sys81_30_uses_target_pointer_and_string_pop_order_for_default_archive_entry() {
        let payload = b"SDC FORMAT 1.00\0payload".to_vec();
        let mut vm = Vm::new();
        let destination = vm.alloc_heap(256);
        let mut api = ArchiveBytesApi {
            expected_archive: "data10000.arc".into(),
            expected_file: String::new(),
            bytes: payload.clone(),
            calls: Vec::new(),
        };
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("data10000.arc".into()),
            Value::Str(String::new()),
            Value::Int(0),
            Value::Int(453),
        ]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x81, 0x30).unwrap(),
            Some(Value::None)
        );
        let range = vm.resolve_range(destination, payload.len()).unwrap();
        assert_eq!(&vm.memory[range], payload.as_slice());
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(
            api.calls.first(),
            Some(&("data10000.arc".to_string(), String::new()))
        );
    }

    #[test]
    fn sys81_30_named_archive_entry_treats_requested_length_as_capacity() {
        let payload = b"SDC FORMAT 1.00\0named-entry-payload".to_vec();
        let mut vm = Vm::new();
        let destination = vm.alloc_heap(256) + 0x58;
        let mut api = ArchiveBytesApi {
            expected_archive: "data10000.arc".into(),
            expected_file: "evdb".into(),
            bytes: payload.clone(),
            calls: Vec::new(),
        };
        vm.stack.extend([
            Value::Ptr(destination),
            Value::Str("data10000.arc".into()),
            Value::Str("evdb".into()),
            Value::Int(0),
            Value::Int(453),
        ]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x81, 0x30).unwrap(),
            Some(Value::None)
        );
        let range = vm.resolve_range(destination, payload.len()).unwrap();
        assert_eq!(&vm.memory[range], payload.as_slice());
        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(
            api.calls.first(),
            Some(&("data10000.arc".to_string(), "evdb".to_string()))
        );
    }

    #[test]
    fn operand_slot_sync_keeps_program_values_as_shared_handles() {
        let mut vm = Vm::new();
        let program = Arc::new(empty_loaded_program("shared".into()));
        vm.push_value(Value::Program(program.clone()));
        vm.sync_operand_slots();

        let Value::Program(stack_program) = &vm.stack[0] else {
            panic!("program value missing from operand stack");
        };
        let Value::Program(slot_program) = &vm.operand_slots[0] else {
            panic!("program value missing from operand slot ring");
        };
        assert!(Arc::ptr_eq(&program, stack_program));
        assert!(Arc::ptr_eq(&program, slot_program));
    }

    #[test]
    fn native_program_messages_are_fifo_and_report_the_received_count() {
        let mut vm = Vm::new();
        vm.thread
            .extend_messages([Value::Int(0x40ff_ffff), Value::Int(0x4100_0001)]);
        vm.stack.extend([Value::Int(2), Value::Ptr(0x1000)]);

        assert_eq!(vm.try_builtin_sys(0x80, 0x4b).unwrap(), Some(Value::Int(2)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 0x40ff_ffff);
        assert_eq!(vm.read_int(0x1004, 2).unwrap(), 0x4100_0001);

        vm.stack.extend([Value::Int(2), Value::Ptr(0x1000)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x4b).unwrap(), Some(Value::Int(0)));
    }

    #[test]
    fn native_program_lookup_does_not_report_a_missing_task_as_active() {
        let vm = Vm::new();
        assert_eq!(
            vm.async_program_is_active(Value::Program(Arc::new(empty_loaded_program(
                "missing".into()
            )))),
            0
        );
    }

    #[test]
    fn read_flag_range_round_trips_and_clears_bits() {
        let mut vm = Vm::new();
        vm.stack.extend([Value::Str("main".into()), Value::Int(16)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack.extend([
            Value::Str("main".into()),
            Value::Int(10),
            Value::Int(1),
            Value::Int(3),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8a).unwrap(), Some(Value::Int(0)));

        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("main".into()),
            Value::Int(11),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 1);

        vm.stack.extend([
            Value::Str("main".into()),
            Value::Int(11),
            Value::Int(0),
            Value::Int(1),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8a).unwrap(), Some(Value::Int(0)));
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("main".into()),
            Value::Int(11),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 0);

        vm.stack
            .extend([Value::Str("main".into()), Value::Int(15), Value::Int(1)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x89).unwrap(), Some(Value::Int(0)));
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("main".into()),
            Value::Int(15),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 1);

        vm.stack.extend([
            Value::Str("main".into()),
            Value::Int(15),
            Value::Int(1),
            Value::Int(2),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8a).unwrap(), Some(Value::Int(3)));
    }

    #[test]
    fn read_flag_resize_preserves_existing_bits_and_enforces_new_length() {
        let mut vm = Vm::new();
        vm.stack.extend([Value::Str("main".into()), Value::Int(16)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack
            .extend([Value::Str("main".into()), Value::Int(15), Value::Int(1)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x89).unwrap(), Some(Value::Int(0)));

        vm.stack.extend([Value::Str("main".into()), Value::Int(24)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("main".into()),
            Value::Int(15),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 1);

        vm.stack.extend([Value::Str("main".into()), Value::Int(12)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("main".into()),
            Value::Int(15),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(2)));
    }

    #[test]
    fn read_flag_query_accepts_the_output_pointer_before_the_name() {
        // Wonderful Everyday's scrmain._bp pushes the flag variable, then a
        // pointer to the current script's name in its script table.
        let flag = 0x1000;
        let name = 0x2000;
        let mut vm = Vm::new();
        vm.write_c_string(name, "main").unwrap();
        vm.stack.extend([Value::Str("main".into()), Value::Int(16)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack
            .extend([Value::Str("main".into()), Value::Int(15), Value::Int(1)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x89).unwrap(), Some(Value::Int(0)));

        for (bit, expected) in [(15, 1), (14, 0)] {
            vm.write_int(flag, 2, 7).unwrap();
            vm.stack.extend([
                Value::Int(flag as i32),
                Value::Int(name as i32),
                Value::Int(bit),
            ]);
            assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
            assert_eq!(vm.read_int(flag, 2).unwrap(), expected);
            assert_eq!(vm.read_c_string(name).unwrap(), "main");
        }
    }

    #[test]
    fn read_flag_query_without_a_table_keeps_the_documented_order() {
        let mut vm = Vm::new();
        vm.write_int(0x1000, 2, 7).unwrap();
        vm.stack.extend([
            Value::Ptr(0x1000),
            Value::Str("missing".into()),
            Value::Int(3),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(1)));
        // sub_447230 leaves the output untouched on failure.
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 7);
    }

    #[test]
    fn system_ext_display_calls_preserve_native_outputs_and_status_codes() {
        let mut vm = Vm::new();
        // No host desktop mode: sub_45EAE0 reports it unchanged.
        vm.stack.push(Value::Ptr(0x1000));
        assert_eq!(vm.try_builtin_sys(0x81, 0x0e).unwrap(), Some(Value::None));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 0);

        vm.stack.extend([Value::Int(8), Value::Int(1280), Value::Int(720)]);
        assert_eq!(vm.try_builtin_sys(0x81, 0x60).unwrap(), Some(Value::Int(1)));
        vm.stack.extend([Value::Int(3), Value::Int(0), Value::Int(720)]);
        assert_eq!(vm.try_builtin_sys(0x81, 0x60).unwrap(), Some(Value::Int(2)));
        vm.stack.extend([Value::Int(3), Value::Int(1280), Value::Int(720)]);
        assert_eq!(vm.try_builtin_sys(0x81, 0x60).unwrap(), Some(Value::Int(0)));

        vm.stack.push(Value::Int(2));
        assert_eq!(vm.try_builtin_sys(0x81, 0x62).unwrap(), Some(Value::Int(0)));
        vm.stack.push(Value::Int(1));
        assert_eq!(vm.try_builtin_sys(0x81, 0x62).unwrap(), Some(Value::Int(1)));
        vm.stack.push(Value::Int(3));
        assert_eq!(vm.try_builtin_sys(0x81, 0x63).unwrap(), Some(Value::Int(0)));
        vm.stack.push(Value::Int(2));
        assert_eq!(vm.try_builtin_sys(0x81, 0x63).unwrap(), Some(Value::Int(1)));

        // No D3DX effect framework on the host: the bicubic effect fails.
        vm.stack.push(Value::Int(1));
        assert_eq!(vm.try_builtin_sys(0x81, 0x6f).unwrap(), Some(Value::Int(0)));
        vm.stack.push(Value::Int(0));
        assert_eq!(vm.try_builtin_sys(0x81, 0x6f).unwrap(), Some(Value::Int(1)));
    }
    #[test]
    fn system81_wide_distance_preserves_target_empty_operand_behavior() {
        assert_eq!(super::wide_string_similarity(&[1, 2], &[]), 0);
        assert_eq!(super::wide_string_similarity(&[], &[1, 2]), 0);
        assert_eq!(super::wide_string_similarity(&[1, 2], &[1, 3]), 1);
        assert_eq!(super::wide_string_similarity(&[1, 2, 3, 4], &[2, 4, 3]), 2);
    }

    #[test]
    fn system81_auxiliary_message_mask_is_process_global() {
        let mut vm = Vm::new();
        vm.stack.push(Value::Int(0x4000));
        assert_eq!(vm.try_builtin_sys(0x81, 0x1f).unwrap(), Some(Value::None));
        assert_eq!(
            vm.system81_shared
                .lock()
                .expect("system81 state poisoned")
                .message_auxiliary_input_mask,
            0x4000
        );
    }
}

/// The protected string namespace (Sys80:D8 keeps it).
pub(crate) const RESOURCE_NAMESPACE: i32 = i32::MIN;
