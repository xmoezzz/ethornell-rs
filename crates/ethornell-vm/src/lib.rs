use ethornell_script::{
    BpInstruction, BpOpcode, BpOperand, BpProgram,
    calls::{known_call_arg_count, known_call_returns_value, known_call_stack_output_count},
    native_abi,
};
use native_thread::{
    CProcWaitTimingExLayout32, CProcWaitWndMsgLayout32, CProcedure, CProcedureLayout32, CThread,
    CThreadLayout32, InstalledCProcedure,
};
use std::collections::{BTreeMap, HashMap, VecDeque};
use std::sync::{
    Arc, Mutex, OnceLock,
    atomic::{AtomicU64, Ordering},
};

mod async_program;
mod debug;
mod extended_opcodes;
mod input;
pub mod native_call;
pub mod native_input;
pub mod native_motion;
pub mod native_ownership;
pub mod native_thread;
mod procedure_class_map;
mod profile;
mod program_thread;
mod records;
mod resource_io;
mod scenario;
mod scheduler;
mod system80_state;
mod system81_state;
mod time;
mod user_data;

pub use native_call::{
    NativeCallFrame, NativeImplementationLevel, NativeMessageProcedureClass,
    NativeMessageProcedureConfig, NativeOpcode, NativeOpcodeSpec, NativeParameterSpec,
    NativeRecoveryLevel, NativeSchedulingEffect, display_name as native_display_name,
    documented_opcode, implementation_level, is_strictly_supported, recovery_level,
    scheduling_effect,
};

const SYSTEM_PROGRAM_TABLE: u32 = 273_280;
const SYSTEM_PROGRAM_SLOTS: usize = 32;
const SYSTEM_PROGRAM_STRIDE: u32 = 16;
const SYSTEM_PROGRAM_DESCRIPTOR_BASE: u32 = 0x4000_0000;
const ADDRESS_MASK: u32 = 0x01ff_ffff;
const AUX_MEMORY_TAG_BASE: u32 = 0x20;
const AUX_MEMORY_SEGMENT_SIZE: u32 = ADDRESS_MASK + 1;
const LOCAL_MEMORY_BASE: u32 = 0x0080_0000;
const HEAP_OFFSET_BASE: u32 = 0x0020_0000;
const HEAP_MEMORY_BASE: usize = (LOCAL_MEMORY_BASE + HEAP_OFFSET_BASE) as usize;
const INITIAL_MEMORY_SIZE: usize = 16 * 1024 * 1024;
const MAX_MEMORY_SIZE: usize = 256 * 1024 * 1024;
const OPERAND_STACK_CAPACITY: usize = 4096;
/// Target per-CThread interpreter quantum recovered from `sub_48CD70`.
/// The native scheduler executes at most 0x100000 BP instructions for one
/// CThread before continuing with the next scheduler entry.
pub const TARGET_COOPERATIVE_QUANTUM_STEPS: usize = 0x100000;
pub const DEFAULT_COOPERATIVE_WATCHDOG_STEPS: usize = TARGET_COOPERATIVE_QUANTUM_STEPS;
static NEXT_VM_TRACE_ID: AtomicU64 = AtomicU64::new(1);

pub type VmResult<T> = std::result::Result<T, VmError>;

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

fn measure_text_width_value(value: &Value, font_size: i32, max_width: i32) -> i32 {
    let text = match value {
        Value::Str(text) => text.as_str(),
        Value::Int(0) | Value::Ptr(0) | Value::None => "",
        Value::Int(_) | Value::Ptr(_) | Value::Func { .. } | Value::Program(_) => {
            return max_width.max(font_size);
        }
    };
    let mut width = 0i32;
    for ch in text.chars() {
        width += if ch.is_ascii() {
            (font_size + 1) / 2
        } else {
            font_size
        };
    }
    if max_width > 0 {
        width.min(max_width)
    } else {
        width
    }
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
pub struct SystemInputDialogRequest {
    pub initial_text: String,
    pub option_value1: i32,
    pub option_value2: i32,
    pub mode: i32,
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

    /// Enable or disable the target render-performance accumulator. Enabling
    /// also resets every counter, matching sub_401600.
    fn set_performance_profiling(&mut self, _enabled: bool) {}

    /// Read one target performance metric selected by Sys80:0x07.
    fn read_performance_metric(&mut self, _selector: i32) -> i32 {
        0
    }

    /// Last successful native presentation timestamp/state (dword_565EC4).
    fn presentation_state(&mut self) -> i32 {
        0
    }

    /// Target 64-byte hardware/capability cache copied by Sys80:0x0A.
    fn graphics_capability_record(&mut self) -> [u32; 16] {
        default_graphics_capability_record()
    }

    /// Renderer-owned capacity field queried by Sys80:0x0B.
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

    /// Portable removable-media discovery used by Sys80:0x3F. On success the
    /// returned directory becomes the target secondary resource root.
    fn locate_removable_archive_root(
        &mut self,
        _archive_name: &str,
        _prompt: &str,
        _subdirectory: &str,
        _retry: bool,
    ) -> Option<String> {
        None
    }

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

    /// Status-5 bootstrap selection used by Sys80:0x6B.
    fn select_bootstrap(&mut self, _root_or_archive_path: &str, _archive_namespace: &str) {}

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

    fn set_window_monitor_adapter_mode(&mut self, mode: i32) -> bool {
        (0..=1).contains(&mode)
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
    fn launch_process_wait(
        &mut self,
        _working_directory: &str,
        _executable: &str,
        _arguments: &str,
        _error_message: &str,
        _show_window: bool,
        _exit_code: Option<&mut i32>,
    ) -> bool {
        false
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

    fn delete_file(&mut self, _root: &str, _file: &str) -> bool {
        false
    }

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

    /// Poll a Win32-style message posted after a `CProcWaitWndMsg` waiter was
    /// registered. The returned pair is `(lParam, wParam)`, matching the target
    /// procedure's two BP pushes.
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

    /// Launches one target command line. Sys80:E0/E2 always wait for the child;
    /// `wait_for_uninstaller` additionally waits for the target uninstaller mutex.
    fn launch_process(
        &mut self,
        _working_directory: &str,
        _command_line: &str,
        _error_message: &str,
        _restore_parent_window: bool,
        _wait_for_uninstaller: bool,
    ) -> bool {
        false
    }

    /// Stores the command executed by WinMain after the current engine window exits.
    fn schedule_restart(
        &mut self,
        _working_directory: &str,
        _command_line: &str,
        _error_message: &str,
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
    fn call_graph(&mut self, call: &mut NativeCallFrame) -> VmResult<Value>;

    /// Returns the two DWORD globals written by target helper sub_434410
    /// when Graph92:9B is called with selector 256.
    fn system92_text_output_pair(&self) -> [i32; 2] {
        [0, 0]
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

    /// Reveal the current message and return whether any glyph state changed.
    fn reveal_native_message(&mut self) -> bool {
        false
    }

    /// Release host-side message-active state after the procedure completes.
    fn finish_native_message(&mut self) {}

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

    /// Validate the public Graph90:F6 destination/source/frame contract. The
    /// portable backend returns status 6 after successful validation while the
    /// proprietary frame codec remains unavailable.
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

    /// Query one target CDspObj parameter for Graph91:38. The VM owns the
    /// writable BP destination pointer. Error 255 means invalid object and 5
    /// means the concrete subclass does not support the parameter number.
    fn query_graph91_object_property(&self, _object: i32, _parameter: i32) -> Result<i32, i32> {
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
    ) -> Option<[i32; 2]> {
        None
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
    ) -> bool {
        false
    }

    fn configure_graph91_landscape_map(
        &mut self,
        _landscape: i32,
        _width: i32,
        _height: i32,
        _map: &[i32],
    ) -> bool {
        false
    }

    fn configure_graph91_landscape_guides(
        &mut self,
        _landscape: i32,
        _bitmap: i32,
        _guide_count: i32,
        _guide_words: &[i32],
    ) -> bool {
        false
    }

    fn set_graph91_landscape_cell_guides(
        &mut self,
        _landscape: i32,
        _pairs: &[i32],
        _layer: i32,
        _guide: i32,
        _value: i32,
    ) -> bool {
        false
    }

    fn query_graph91_landscape_cell_value(
        &self,
        _landscape: i32,
        _line: i32,
        _column: i32,
    ) -> Option<i32> {
        None
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct GraphInputGroup {
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

    fn show_user_dialog(&mut self, _request: UserDialogRequest) -> Option<UserDialogResponse> {
        None
    }

    fn configure_particle_frame_tables(
        &mut self,
        _handle: i32,
        _first: &[i32],
        _second: &[i32],
    ) -> bool {
        false
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

fn portable_local_system_time() -> [u16; 8] {
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
            fn time(timer: *mut i64) -> i64;
            fn localtime_r(timer: *const i64, result: *mut Tm) -> *mut Tm;
        }
        let mut seconds = 0i64;
        // SAFETY: pointers refer to initialized writable storage and localtime_r is reentrant.
        if unsafe { time(&mut seconds) } != -1 {
            let mut local = std::mem::MaybeUninit::<Tm>::uninit();
            if !unsafe { localtime_r(&seconds, local.as_mut_ptr()) }.is_null() {
                let local = unsafe { local.assume_init() };
                let milliseconds = std::time::SystemTime::now()
                    .duration_since(std::time::UNIX_EPOCH)
                    .map(|duration| duration.subsec_millis() as u16)
                    .unwrap_or(0);
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
    let mut record = [0u32; 16];
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(target_arch = "x86")]
        use std::arch::x86::{__cpuid, __cpuid_count};
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::{__cpuid, __cpuid_count};
        let vendor = __cpuid(0);
        let mut vendor_bytes = Vec::with_capacity(12);
        vendor_bytes.extend_from_slice(&vendor.ebx.to_le_bytes());
        vendor_bytes.extend_from_slice(&vendor.edx.to_le_bytes());
        vendor_bytes.extend_from_slice(&vendor.ecx.to_le_bytes());
        let vendor_name = String::from_utf8_lossy(&vendor_bytes);
        record[3] = match vendor_name.as_ref() {
            "GenuineIntel" => 0,
            "AuthenticAMD" => 1,
            "CentaurHauls" => 2,
            "GenuineTMx86" => 3,
            _ => 4,
        };
        let signature = __cpuid(1);
        let base_family = (signature.eax >> 8) & 0x0f;
        let ext_family = (signature.eax >> 20) & 0xff;
        let base_model = (signature.eax >> 4) & 0x0f;
        let ext_model = (signature.eax >> 16) & 0x0f;
        record[1] = signature.eax & 0x0f;
        record[4] = if base_family == 0x0f {
            base_family + ext_family
        } else {
            base_family
        };
        record[0] = if matches!(base_family, 0x06 | 0x0f) {
            base_model | (ext_model << 4)
        } else {
            base_model
        };
        record[2] = if record[3] == 0 {
            signature.ebx & 0xff
        } else if __cpuid(0x8000_0000).eax >= 0x8000_0001 {
            __cpuid_count(0x8000_0001, 0).ebx & 0xffff
        } else {
            0
        };
    }
    record[9] = std::thread::available_parallelism()
        .map(|count| count.get().min(u32::MAX as usize) as u32)
        .unwrap_or(1);
    record
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
        let text = String::from_utf8_lossy(&bytes[..end]);
        return Some(text.split_whitespace().collect::<Vec<_>>().join(" "));
    }
    #[allow(unreachable_code)]
    None
}

fn target_cpu_signature_words() -> [u32; 4] {
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(target_arch = "x86")]
        use std::arch::x86::__cpuid;
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::__cpuid;
        let result = __cpuid(1);
        return [
            result.eax & 0xffff,
            (result.eax >> 16) & 0xffff,
            result.edx & 0xffff,
            (result.edx >> 16) & 0xffff,
        ];
    }
    #[allow(unreachable_code)]
    [0; 4]
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

fn wide_string_similarity(left: &[u16], right: &[u16]) -> i32 {
    // sub_495C50 has an asymmetric empty-string fast path: an empty second
    // operand returns zero, while an empty first operand returns the second
    // length through the normal len(left)+len(right)-LCS result.
    if right.is_empty() {
        return 0;
    }
    let mut previous = vec![0usize; right.len() + 1];
    let mut current = vec![0usize; right.len() + 1];
    for &left_value in left {
        for (index, &right_value) in right.iter().enumerate() {
            current[index + 1] = if left_value == right_value {
                previous[index] + 1
            } else {
                current[index].max(previous[index + 1])
            };
        }
        std::mem::swap(&mut previous, &mut current);
        current.fill(0);
    }
    left.len()
        .saturating_add(right.len())
        .saturating_sub(previous[right.len()])
        .min(i32::MAX as usize) as i32
}

fn adjusted_desktop_dimensions((width, height): (i32, i32)) -> (i32, i32) {
    let width = width.max(1);
    let height = height.max(1);
    let adjusted_width = if 4_i64.saturating_mul(i64::from(width)) / i64::from(height) >= 10 {
        width / 2
    } else {
        width
    };
    let adjusted_height = if height / width != 0
        && 100_i64.saturating_mul(i64::from(height)) / i64::from(width) < 125
    {
        height / 2
    } else {
        height
    };
    (adjusted_width.max(1), adjusted_height.max(1))
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
        functions: Vec::new(),
        strings: Vec::new(),
        instructions: vec![BpInstruction {
            offset: 0x10,
            opcode: BpOpcode::Known {
                code: 0x17,
                name: opcode_name,
            },
            opcode_hex: "0x17".into(),
            opcode_name: opcode_name.into(),
            operands: Vec::new(),
            known_call: None,
            raw: vec![0x17],
            warning: Some(warning.into()),
        }],
        labels,
        warnings: vec![warning.into()],
    }
}

#[derive(Debug, Default)]
pub struct Vm {
    trace_id: u64,
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
    pub memory: Vec<u8>,
    // Native writes and thread transfers operate on address ranges. Keep the
    // shadow values ordered so a small write does not scan every scene record.
    pub mem_values: BTreeMap<u32, Value>,
    pub mem_ptr: u32,
    pub heap_ptr: u32,
    heap_allocations: BTreeMap<u32, u32>,
    heap_free_blocks: Vec<(u32, u32)>,
    shared_heap: Arc<Mutex<SharedHeapState>>,
    shared_heap_generation: u64,
    shared_heap_dirty: Vec<std::ops::Range<usize>>,
    pub halted: bool,
    pub calls: BTreeMap<String, usize>,
    pub stubs: BTreeMap<String, usize>,
    program_cache: BTreeMap<String, usize>,
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
    resource_names: Vec<String>,
    /// Target dword_506BE0 and dword_566630 resource-search globals.
    additional_resource_search_enabled: bool,
    additional_resource_paths: Vec<String>,
    /// Named DCArchiveComplex component lists registered by Sys80:0x38.
    composite_archives: BTreeMap<String, Vec<String>>,
    /// Target primary/secondary process-global filesystem roots.
    primary_resource_root: Option<String>,
    secondary_resource_root: Option<String>,
    /// Independent validated directory stored by target sub_46B390.
    validated_file_root: Option<String>,
    /// Target process-global dword_507688 written by Sys80:0x50.
    system_wait_state: i32,
    string_hash_tables: BTreeMap<i32, Vec<String>>,
    read_flags: BTreeMap<String, ReadFlagBits>,
    display_mode_slots: [Option<(i32, i32)>; 8],
    window_monitor_adapter_mode: i32,
    config_input_mode: i32,
    shader_effect_enabled: bool,
    save_data_integrity_enabled: i32,
    /// Target dword_5668A0: one-shot selection of the next binary/BMV
    /// procedure path. The consuming selector always resets it.
    next_binary_or_bmv_async: bool,
    /// Target dword_566898/dword_56689C cooperative scheduling restriction.
    exclusive_thread_id: Option<i32>,
    global_config: Vec<u8>,
    global_user_data: Vec<u8>,
    loaded_bcs_ranges: Vec<scenario::LoadedBcsRange>,
    async_tasks: Vec<async_program::AsyncProgramTask>,
    suppress_async_pump_once: bool,
    timing: time::VmTime,
    rng_seed: u32,
    recent_trace: VecDeque<String>,
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
    next_program_instance_id: u64,
    /// Process-wide monotonically increasing CThread id source. Root uses 0;
    /// child threads start at 1, matching the target constructor counter.
    next_thread_id: i32,
    collect_diagnostics: bool,
    extended_opcodes: extended_opcodes::ExtendedOpcodeState,
}

#[derive(Debug, Default)]
struct SharedHeapState {
    bytes: Vec<u8>,
    values: BTreeMap<u32, Value>,
    generation: u64,
    journal: Vec<SharedHeapJournalEntry>,
}

#[derive(Debug, Clone)]
struct SharedHeapJournalEntry {
    generation: u64,
    range: std::ops::Range<usize>,
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

impl Vm {
    pub fn new() -> Self {
        Self {
            trace_id: NEXT_VM_TRACE_ID.fetch_add(1, Ordering::Relaxed),
            operand_slots: vec![Value::Int(0); OPERAND_STACK_CAPACITY],
            memory: vec![0; INITIAL_MEMORY_SIZE],
            heap_ptr: 0x0020_0000,
            rng_seed: 1,
            system_wait_state: 1,
            next_indexed_record_handle: 1,
            next_thread_id: 1,
            global_config: vec![0; user_data::GLOBAL_CONFIG_SIZE],
            global_user_data: vec![0; user_data::GLOBAL_USER_DATA_SIZE],
            ..Self::default()
        }
    }

    pub fn advance_time_ms(&mut self, milliseconds: u64) {
        self.timing.advance(milliseconds);
    }

    /// Select the code page of text passed to native graph drawing calls.
    /// Resource names and BP system strings retain their original encoding.
    pub fn set_graph_text_encoding(&mut self, encoding: &'static encoding_rs::Encoding) {
        self.graph_text_encoding = Some(encoding);
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
        self.stack.clear();
        self.operand_slots.fill(Value::Int(0));
        self.operand_slots_synced_len = 0;
        self.programs.clear();
        self.programs.push(program.clone());
        self.program_code_bases.clear();
        self.program_code_bases.push(0);
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

    pub fn run_loaded<A>(&mut self, api: &mut A, options: &VmRunOptions) -> VmRunReport
    where
        A: SysApi + GraphApi + SoundApi,
    {
        self.collect_diagnostics = options.collect_diagnostics;
        // `stack` is public for embedders and tests. Reconcile it once at the
        // host slice boundary; instruction dispatch maintains the ring
        // incrementally after this point.
        self.operand_slots_synced_len = 0;
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
        let trace_events =
            std::env::var_os("TRACE_VM_EVENTS").is_some() || std::env::var_os("DEBUG").is_some();
        if !self.halted && !std::mem::take(&mut self.suppress_async_pump_once) {
            self.pump_async_programs(api, trace_events, options.max_steps);
        }
        let procedure_wait = self.poll_current_procedure(api, trace_events);
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
            instruction_profile.record(self, program_index, pc_before);
            if options.trace {
                println!(
                    "pc={} off=0x{:08X} {} {:?} stack_top={:?}",
                    self.pc,
                    inst.offset,
                    inst.opcode_name,
                    inst.operands,
                    self.stack.last()
                );
            }
            if trace_events || options.trace || options.fail_on_stub {
                self.push_trace(format!(
                    "program={} pc={} off=0x{:08X} {} {:?}",
                    self.program_name(program_index),
                    self.pc,
                    inst.offset,
                    inst.opcode_name,
                    inst.operands
                ));
            }
            self.trace_mtn_body_dispatch(inst.offset);
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
                            && (delta != 0 || inst.opcode_name.starts_with("sys"))
                        {
                            eprintln!(
                                "TRACE_STACK vm={} program={} pc={} off=0x{:08X} op={} {:?} stack {} -> {} ({:+}) next_pc={} top={:?}",
                                self.trace_id,
                                program_name,
                                pc_before,
                                inst.offset,
                                inst.opcode_name,
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
                    self.trace_msgwnd_loop(inst.offset as u32);
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
                .then(|| self.recent_trace.iter().cloned().collect())
                .unwrap_or_default(),
        }
    }

    pub fn dispatch<A>(&mut self, instruction: &BpInstruction, api: &mut A) -> VmResult<()>
    where
        A: SysApi + GraphApi + SoundApi,
    {
        self.operand_slots_synced_len = 0;
        let old_program = self.current_program;
        let old_pc = self.pc;
        self.programs.push(BpProgram {
            script_name: None,
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![instruction.clone()],
            labels: Default::default(),
            warnings: Vec::new(),
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
        self.sync_operand_slots();
        let code = instruction.opcode.code();
        let mut next_pc = self.pc + 1;
        match instruction.opcode {
            BpOpcode::Known {
                name: "push_byte", ..
            } => {
                self.push_value(Value::Int(read_op_i32(instruction)));
            }
            BpOpcode::Known {
                name: "push_word" | "push_dword",
                ..
            } => {
                self.push_value(Value::Int(read_op_i32(instruction)));
            }
            BpOpcode::Known {
                name: "push_base_offset",
                ..
            } => {
                self.push_value(Value::Ptr(
                    0x1200_0000u32 | self.mem_ptr.saturating_sub(read_op_u32(instruction)),
                ));
            }
            BpOpcode::Known {
                name: "push_string",
                ..
            } => {
                if let Some(BpOperand::String(text)) = instruction.operands.first() {
                    self.push_value(Value::Str(text.clone()));
                } else if let Some(BpOperand::Offset(offset)) = instruction.operands.first() {
                    self.push_value(Value::Ptr(0x1000_0000 | *offset));
                }
            }
            BpOpcode::Known {
                name: "push_offset",
                ..
            } => {
                self.push_value(Value::Func {
                    program_index,
                    offset: read_op_u32(instruction),
                });
            }
            BpOpcode::Known {
                name: "load_base", ..
            } => {
                // sub_473880 pushes the raw frame-memory offset. Only opcode
                // 0x04 turns base-relative offsets into dereferenceable pointers.
                self.push_value(Value::Int(self.mem_ptr as i32));
            }
            BpOpcode::Known {
                name: "store_base", ..
            } => {
                let previous = self.mem_ptr;
                let next = self.pop_int()? as u32;
                if next > previous {
                    let size = (next - previous) as usize;
                    let ptr = 0x1200_0000u32 | previous;
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
            BpOpcode::Known { name: "load", .. } => {
                let width = read_op_u8(instruction);
                let ptr = self.pop_ptr()?;
                let value = self.read_value(ptr, width)?;
                self.push_value(value);
            }
            BpOpcode::Known { name: "move", .. } => {
                let width = read_op_u8(instruction);
                let value = self.pop_value()?;
                let ptr = self.pop_ptr()?;
                self.write_value(ptr, width, &value)?;
                // sub_473710 writes through the pointer and calls sub_4450D0
                // to return the assigned value on the operand ring.
                self.push_value(value);
            }
            BpOpcode::Known {
                name: "move_arg", ..
            } => {
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
            BpOpcode::Known {
                name: "copy_inline",
                ..
            } => {
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
            BpOpcode::Known {
                name: "copy_stack", ..
            } => {
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
            BpOpcode::Known { name: "jmp", .. } => {
                let dest = self.pop_int()? as u32;
                next_pc = self.jump_target_index(program_index, dest)?;
            }
            BpOpcode::Known { name: "jc", .. } => {
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
                    next_pc = self.jump_target_index(program_index, dest)?;
                }
            }
            BpOpcode::Known { name: "call", .. } => match {
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
                        .read_int(0x1200_0000 | self.mem_ptr.saturating_sub(4), 2)
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
                        if trace_events {
                            tracing::info!(
                                pc = self.pc,
                                offset = format_args!("0x{:08X}", instruction.offset),
                                dest_program = dest_program_index,
                                "VM null function call ignored"
                            );
                        }
                        self.pc = next_pc;
                        return Ok(());
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
                        if trace_events {
                            tracing::info!(
                                pc = self.pc,
                                offset = format_args!("0x{:08X}", instruction.offset),
                                "VM null call ignored"
                            );
                        }
                        self.pc = next_pc;
                        return Ok(());
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
            BpOpcode::Known { name: "ret", .. } => {
                if self.mem_ptr == 0 {
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
                    self.current_program = program_id;
                    next_pc = self
                        .jump_target_index(program_id, ret_offset)
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
                            left / right
                        }
                    }
                    "mod" => {
                        if right == 0 {
                            -1
                        } else {
                            left % right
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
            BpOpcode::Known { name: "not", .. } => {
                let value = self.pop_int()?;
                self.push_value(Value::Int(!value));
            }
            BpOpcode::Known {
                name: "bool_zero", ..
            } => {
                let value = self.pop_int()?;
                self.push_value(Value::Int((value == 0) as i32));
            }
            BpOpcode::Known {
                name: "ternary", ..
            } => {
                let false_value = self.pop_value()?;
                let true_value = self.pop_value()?;
                let condition = self.pop_int()?;
                self.push_value(if condition != 0 {
                    true_value
                } else {
                    false_value
                });
            }
            BpOpcode::Known { name: "muldiv", .. } => {
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
            BpOpcode::Known { name: "atan2", .. } => {
                let y = self.pop_int()?;
                let x = self.pop_int()?;
                let mut degrees = (y as f64).atan2(x as f64).to_degrees();
                if degrees < 0.0 {
                    degrees += 360.0;
                }
                self.push_value(Value::Int((degrees * 65_536.0) as i32));
            }
            BpOpcode::Known {
                name: "vec3_length",
                ..
            } => {
                let z = self.pop_int()? as f64;
                let y = self.pop_int()? as f64;
                let x = self.pop_int()? as f64;
                self.push_value(Value::Int(
                    (x.mul_add(x, y.mul_add(y, z * z)).sqrt()) as i32,
                ));
            }
            BpOpcode::Known {
                name: "sin" | "cos",
                ..
            } => {
                let angle = self.pop_int()? as f64;
                let radians = angle * ((std::f64::consts::PI / 180.0) / 65_536.0);
                let value = if instruction.opcode.name() == "sin" {
                    radians.sin()
                } else {
                    radians.cos()
                };
                self.push_value(Value::Int((value * 65_536.0) as i32));
            }
            BpOpcode::Known {
                name: "qword_add" | "qword_sub" | "qword_mul" | "qword_div" | "qword_mod",
                ..
            } => {
                self.execute_qword_arithmetic(code)?;
            }
            BpOpcode::Known { name: "memcpy", .. } => {
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
            BpOpcode::Known { name: "memclr", .. } => {
                let size = self.pop_int()?.max(0) as usize;
                let ptr = self.pop_ptr()?;
                let range = self.resolve_write_range(ptr, size)?;
                self.memory[range].fill(0);
                self.trace_watch_write(ptr, size, 0, "memclr");
                self.clear_shadow_values(ptr, size);
                self.restore_script_records(ptr, size)?;
            }
            BpOpcode::Known { name: "memset", .. } => {
                let value = self.pop_int()? as u8;
                let size = self.pop_int()?.max(0) as usize;
                let ptr = self.pop_ptr()?;
                let range = self.resolve_write_range(ptr, size)?;
                self.memory[range].fill(value);
                self.trace_watch_write(ptr, size, value as u32, "memset");
                self.clear_shadow_values(ptr, size);
                self.restore_script_records(ptr, size)?;
            }
            BpOpcode::Known {
                name: "memory_equal",
                ..
            } => {
                let size = self.pop_int()?.max(0) as usize;
                let right = self.pop_value()?;
                let left = self.pop_value()?;
                let equal = self.value_as_fixed_bytes(left, size)?
                    == self.value_as_fixed_bytes(right, size)?;
                self.push_value(Value::Int(i32::from(equal)));
            }
            BpOpcode::Known {
                name: "memrepeat", ..
            } => {
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
            BpOpcode::Known {
                name: "memfind", ..
            } => {
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
            BpOpcode::Known {
                name: "strfind", ..
            } => {
                let needle = self.pop_string_lossy()?;
                let haystack = self.pop_string_lossy()?;
                let (needle, _, _) = encoding_rs::SHIFT_JIS.encode(&needle);
                let (haystack, _, _) = encoding_rs::SHIFT_JIS.encode(&haystack);
                let found = if needle.is_empty() {
                    0
                } else {
                    haystack
                        .windows(needle.len())
                        .position(|window| window == needle.as_ref())
                        .map(|offset| offset as i32)
                        .unwrap_or(-1)
                };
                self.push_value(Value::Int(found));
            }
            BpOpcode::Known {
                name: "strreplace", ..
            } => {
                let replacement = self.pop_string_lossy()?;
                let needle = self.pop_string_lossy()?;
                let haystack = self.pop_string_lossy()?;
                let dst = self.pop_ptr()?;
                self.write_c_string(dst, &haystack.replace(&needle, &replacement))?;
            }
            BpOpcode::Known { name: "strlen", .. } => {
                let text = self.pop_string_lossy()?;
                let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(&text);
                self.push_value(Value::Int(encoded.len() as i32));
            }
            BpOpcode::Known { name: "streq", .. } => {
                let right = self.pop_string_lossy()?;
                let left = self.pop_string_lossy()?;
                self.push_value(Value::Int((left == right) as i32));
            }
            BpOpcode::Known { name: "strcpy", .. } => {
                let right = self.pop_value()?;
                let left = self.pop_ptr()?;
                self.copy_c_string_value(left, right)?;
            }
            BpOpcode::Known {
                name: "strconcat", ..
            } => {
                let right = self.pop_string_lossy()?;
                let left = self.pop_string_lossy()?;
                let dst = self.pop_ptr()?;
                self.write_c_string(dst, &format!("{left}{right}"))?;
            }
            BpOpcode::Known {
                name: "getchar", ..
            } => {
                let ptr = self.pop_ptr()?;
                let start = Self::memory_addr(ptr) as usize;
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
            BpOpcode::Known {
                name: "tolower", ..
            } => {
                let ptr = self.pop_ptr()?;
                let text = self.read_c_string(ptr)?.to_ascii_lowercase();
                self.write_c_string(ptr, &text)?;
            }
            BpOpcode::Known {
                name: "quote_string",
                ..
            } => {
                self.execute_quote_string()?;
            }
            BpOpcode::Known {
                name: "sprintf", ..
            } => {
                let fmt = self.pop_string_lossy()?;
                let dst = self.pop_ptr()?;
                let rendered = self.render_sprintf(&fmt);
                self.write_c_string(dst, &rendered)?;
            }
            BpOpcode::Known { name: "malloc", .. } => {
                let size = self.pop_int()?.max(0) as u32;
                let ptr = self.alloc_heap(size);
                self.push_value(Value::Ptr(ptr));
            }
            BpOpcode::Known { name: "free", .. } => {
                let ptr = self.pop_ptr()?;
                let freed = self.free_heap(ptr);
                self.push_value(Value::Int(i32::from(freed)));
            }
            BpOpcode::Known {
                name: "set_memory_mode",
                ..
            } => {
                self.execute_set_memory_mode()?;
            }
            BpOpcode::Known {
                name: "addmemboundary",
                ..
            } => {
                let _name = self.pop_string_lossy()?;
                let _size = self.pop_int()?;
                let _start = self.pop_int()?;
                self.push_value(Value::Int(1));
            }
            BpOpcode::Known {
                name: "engine_state",
                ..
            } => {
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
            BpOpcode::Known {
                name: "confirm", ..
            } => {
                let _message = self.pop_string_lossy().unwrap_or_default();
                self.push_value(Value::Int(1));
            }
            BpOpcode::Known {
                name: "message_box",
                ..
            } => {
                let message = self
                    .pop_string_lossy()
                    .unwrap_or_else(|_| "<message>".into());
                tracing::warn!(%message, "BP message_box");
            }
            BpOpcode::Known { name: "assert", .. } => {
                let value = self.pop_int()?;
                if value == 0 {
                    return Err(VmError::Runtime("BP assert failed".into()));
                }
            }
            BpOpcode::Known {
                name: "dumpmem", ..
            } => {
                let _size = self.pop_int().unwrap_or_default();
                let _ptr = self.pop_ptr().unwrap_or_default();
            }
            BpOpcode::Known {
                name: "modal_list", ..
            } => {
                let result = self.execute_modal_list()?;
                self.push_value(Value::Int(result));
            }
            BpOpcode::Known {
                name: "resource_transform",
                ..
            } => {
                self.execute_resource_transform()?;
            }
            BpOpcode::Known {
                name: "clipboard_set",
                ..
            } => {
                let result = self.execute_clipboard_set()?;
                self.push_value(Value::Int(result));
            }
            BpOpcode::Known {
                name: "resource_blend",
                ..
            } => {
                self.execute_resource_blend()?;
            }
            BpOpcode::Known {
                name: "sys1" | "sys2",
                ..
            } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("sys", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !native_call::is_strictly_supported(NativeOpcode { group: code, id })
                {
                    self.note_stub("sys", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                let opcode = NativeOpcode { group: code, id };
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
                    // Arguments are pushed as path, source, then byte count.
                    let length = self.pop_int()? as u32 as usize;
                    let source = self.pop_ptr()?;
                    let path = self.pop_string_lossy()?;
                    let range = self.resolve_range(source, length)?;
                    Value::Int(i32::from(api.write_file_bytes(&path, &self.memory[range])))
                } else if (code, id) == (0x80, 0x34) {
                    // Target 0x4888C0 pops file first and archive/root second,
                    // then passes both to sub_4665C0 (ECX=file, stack=archive).
                    let raw_file = self.pop_value()?;
                    let raw_archive = self.pop_value()?;
                    let file = self.value_as_native_string_lossy(raw_file.clone())?;
                    let archive = self.value_as_native_string_lossy(raw_archive.clone())?;
                    let found = self.resource_file_exists_with_search(api, &archive, &file);
                    tracing::info!(
                        archive,
                        file,
                        ?raw_archive,
                        ?raw_file,
                        found,
                        "Sys80_34_FileExistsWithConfiguredRoots"
                    );
                    Value::Int(i32::from(found))
                } else if (code, id) == (0x80, 0x35) {
                    // Target 0x488900 pops file first and archive/root second,
                    // then calls sub_4662E0 with ECX=archive and stack=file.
                    let file = self.pop_string_lossy()?;
                    let archive = self.pop_string_lossy()?;
                    let size = self
                        .resource_file_size_with_search(api, &archive, &file)
                        .max(0);
                    tracing::info!(archive, file, size, "Sys80_35_FileSizeWithConfiguredRoots");
                    Value::Int(size)
                } else if (code, id) == (0x80, 0x3e) {
                    let path = self.pop_string_lossy()?;
                    let valid = api.directory_exists(&path);
                    if valid {
                        self.primary_resource_root = Some(ensure_trailing_separator(&path));
                    }
                    Value::Int(i32::from(valid))
                } else if (code, id) == (0x80, 0x3f) {
                    let retry = self.pop_int()? != 0;
                    let prompt = self.pop_string_lossy()?;
                    let subdirectory = self.pop_string_lossy()?;
                    let archive_name = self.pop_string_lossy()?;
                    let root = api.locate_removable_archive_root(
                        &archive_name,
                        &prompt,
                        &subdirectory,
                        retry,
                    );
                    if let Some(root) = root {
                        let configured_root = ensure_trailing_separator(&root);
                        tracing::info!(
                            archive_name,
                            subdirectory,
                            root = %configured_root,
                            "Sys80_3F_ConfiguredSecondaryResourceRoot"
                        );
                        self.secondary_resource_root = Some(configured_root);
                        Value::Int(1)
                    } else {
                        tracing::warn!(
                            archive_name,
                            subdirectory,
                            "Sys80_3F_SecondaryResourceRootNotFound"
                        );
                        Value::Int(0)
                    }
                } else if (code, id) == (0x80, 0x45) {
                    // The target table points at __RTC_NumErrors, but the
                    // dispatch ABI exposes no result; this selector is a
                    // BP-visible no-op in the shipped release image.
                    Value::None
                } else if (code, id) == (0x80, 0x50) {
                    self.system_wait_state = self.pop_int()?;
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
                if self.mem_ptr == 0 {
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
            BpOpcode::Known {
                name: "grp1" | "grp2" | "grp3",
                ..
            } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("graph", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !native_call::is_strictly_supported(NativeOpcode { group: code, id })
                {
                    self.note_stub("graph", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                let result = if (code, id) == (0x90, 0xbc) {
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
                        let event = api.poll_object_event_record(object.as_i32());
                        if input::clears_title_pending_callback(event[0], event[1]) {
                            self.write_value(
                                input::TITLE_PENDING_CALLBACK_ADDR,
                                2,
                                &Value::Int(0),
                            )?;
                        }
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
                    let byte_count = (width.max(0) as usize)
                        .checked_mul(height.max(0) as usize)
                        .and_then(|pixels| pixels.checked_mul(3))
                        .ok_or_else(|| VmError::Runtime("bitmap RGB size overflow".into()))?;
                    let range = self.resolve_range(pixels, byte_count)?;
                    let bytes = self.memory[range].to_vec();
                    api.create_bitmap_from_rgb(bitmap, width, height, format, &bytes);
                    Value::None
                } else if (code, id) == (0x90, 0x15) {
                    let bitmap = self.pop_int()?;
                    let capacity = self.pop_int()?.max(0) as usize;
                    let written = self.pop_ptr()?;
                    let destination = self.pop_ptr()?;
                    let bytes = api
                        .read_bitmap_pixels(bitmap, capacity)
                        .filter(|bytes| bytes.len() <= capacity);
                    let count = bytes.as_ref().map(Vec::len).unwrap_or_default();
                    if let Some(bytes) = bytes {
                        let range = self.resolve_write_range(destination, bytes.len())?;
                        self.memory[range].copy_from_slice(&bytes);
                        self.clear_shadow_values(destination, bytes.len());
                    }
                    self.write_int(written, 2, count as u32)?;
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
                        let _ = api.remove_graph_color_lut(identifier);
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
                        Err(_) => {
                            self.write_int(size_out, 2, 0)?;
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
                    if destination != 0 {
                        if let Ok(value) = api.query_graph91_object_property(object, parameter) {
                            self.write_int(destination, 2, value as u32)?;
                            self.clear_shadow_values(destination, 4);
                        }
                    }
                    Value::None
                } else if (code, id) == (0x91, 0x3D) {
                    // sub_481B50 pops the object and converts the first BP
                    // argument. sub_41B260 writes exactly two resolved
                    // coordinate DWORDs through that pointer.
                    let object = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    if let Some(position) = api.query_graph91_object_composite_position(object) {
                        self.write_int(destination, 2, position[0] as u32)?;
                        self.write_int(destination.wrapping_add(4), 2, position[1] as u32)?;
                        self.clear_shadow_values(destination, 8);
                    }
                    Value::None
                } else if (code, id) == (0x91, 0x73) {
                    // sub_482DA0 pops alpha-test mode and landscape, then
                    // writes the hit line/column pair through the first BP argument.
                    let alpha_test_mode = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let hit = api.graph91_landscape_hit_test(landscape, alpha_test_mode);
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
                    let _ = api.configure_graph91_landscape_parts(
                        landscape,
                        bitmap,
                        part_count,
                        &part_words,
                        part_spacing,
                        column_count,
                        &column_words,
                    );
                    Value::None
                } else if (code, id) == (0x91, 0x79) {
                    let map_ptr = self.pop_ptr()?;
                    let height = self.pop_int()?;
                    let width = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let count = width.max(0) as usize * height.max(0) as usize;
                    let mut map = Vec::with_capacity(count);
                    for index in 0..count {
                        map.push(self.read_int(map_ptr.wrapping_add((index * 4) as u32), 2)? as i32);
                    }
                    let _ = api.configure_graph91_landscape_map(landscape, width, height, &map);
                    Value::None
                } else if (code, id) == (0x91, 0x7A) {
                    let guides_ptr = self.pop_ptr()?;
                    let guide_count = self.pop_int()?.max(0);
                    let bitmap = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let mut words = Vec::with_capacity(guide_count as usize * 5);
                    for index in 0..guide_count as usize * 5 {
                        words.push(
                            self.read_int(guides_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    let _ = api.configure_graph91_landscape_guides(
                        landscape,
                        bitmap,
                        guide_count,
                        &words,
                    );
                    Value::None
                } else if (code, id) == (0x91, 0x7B) {
                    let value = self.pop_int()?;
                    let guide = self.pop_int()?;
                    let layer = self.pop_int()?;
                    let pairs_ptr = self.pop_ptr()?;
                    let point_count = self.pop_int()?.max(0);
                    let landscape = self.pop_int()?;
                    let mut pairs = Vec::with_capacity(point_count as usize * 2);
                    for index in 0..point_count as usize * 2 {
                        pairs.push(
                            self.read_int(pairs_ptr.wrapping_add((index * 4) as u32), 2)? as i32,
                        );
                    }
                    let _ = api
                        .set_graph91_landscape_cell_guides(landscape, &pairs, layer, guide, value);
                    Value::None
                } else if (code, id) == (0x91, 0x7E) {
                    let column = self.pop_int()?;
                    let line = self.pop_int()?;
                    let landscape = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    let value = api.query_graph91_landscape_cell_value(landscape, line, column);
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
                    match api.query_movie_position(bitmap) {
                        Ok(position) => {
                            self.write_int(destination, 2, position as u32)?;
                            Value::Int(1)
                        }
                        Err(_) => Value::Int(0),
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
                    let y = self.pop_int()?;
                    let x = self.pop_int()?;
                    let bitmap = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    self.write_int(destination, 2, 0)?;
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
                            let value = match info.format {
                                1 => u32::from_le_bytes([b, g, r, 0]),
                                3 => u32::from(a),
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
                            "Graph90:98 invalid caret bitmap #{bitmap}"
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
                    let source = self.pop_string_lossy()?;
                    let destination = self.pop_ptr()?;
                    self.write_c_string(destination, &strip_native_markup_tags(&source))?;
                    Value::None
                } else if (code, id) == (0x91, 0x3e) {
                    let call_stack = self.take_dispatch_call_frame(code, id)?;
                    let mut call =
                        NativeCallFrame::new(NativeOpcode { group: code, id }, call_stack);
                    let result = api.call_graph(&mut call)?;
                    self.write_int(1072, 2, result.as_i32() as u32)?;
                    Value::None
                } else if (code, id) == (0x91, 0x9b) {
                    self.handle_text_measure_call()?;
                    Value::Int(0)
                } else if (code, id) == (0x91, 0x9e) {
                    // sub_437EE0 extracts every non-empty <l>...</l> payload
                    // into consecutive zero-padded 128-byte records.
                    let source = self.pop_string_lossy()?;
                    let destination = self.pop_ptr()?;
                    let labels = extract_native_labels(&source);
                    for (index, label) in labels.iter().enumerate() {
                        let address = destination.wrapping_add((index * 128) as u32);
                        let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(label);
                        let size = encoded.len().min(95);
                        let range = self.resolve_write_range(address, 128)?;
                        self.memory[range.clone()].fill(0);
                        self.memory[range.start..range.start + size]
                            .copy_from_slice(&encoded[..size]);
                        self.clear_shadow_values(address, 128);
                    }
                    Value::Int(labels.len() as i32)
                } else if (code, id) == (0x91, 0x95) {
                    // sub_484740 accepts a second converted argument but the
                    // target core ignores it. The public result is the match
                    // count; no BP output buffer is written.
                    let source = self.pop_string_lossy()?;
                    let _ignored = self.pop_value()?;
                    let (_, count) = api.collect_ruby_substitutions(&source);
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
                        return Err(VmError::Runtime(format!(
                            "Graph90:A7 invalid window handle #{window}"
                        )));
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
                            "Graph90:{id:02X} invalid icon count {count}"
                        )));
                    }
                    if !api.graph_window_exists(window) {
                        return Err(VmError::Runtime(format!(
                            "Graph90:{id:02X} invalid window handle #{window}"
                        )));
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
                        return Err(VmError::Runtime(format!(
                            "Graph90:{id:02X} failed to draw icon batch for window #{window}"
                        )));
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
                            "Graph91:BF invalid key assignment [{assignment}]"
                        )));
                    }
                    Value::None
                } else if (code, id) == (0x92, 0x9b) {
                    // sub_486760 pops selector first and a BP destination
                    // pointer second. sub_434410 accepts only selector 256
                    // and writes dword_565D34/dword_565D38.
                    let selector = self.pop_int()?;
                    let destination = self.pop_ptr()?;
                    if selector != 256 {
                        return Err(VmError::Runtime(format!(
                            "Graph92:9B invalid output selector {selector}"
                        )));
                    }
                    let [first, second] = api.system92_text_output_pair();
                    let _ = self.resolve_write_range(destination, 8)?;
                    self.write_int(destination, 2, first as u32)?;
                    self.write_int(destination.wrapping_add(4), 2, second as u32)?;
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
                } else if (code, id) == (0xa0, 0x86) {
                    // sub_48D910 writes the translated MCI mode through the
                    // converted destination pointer and returns one when the
                    // status query ran. A closed/not-ready portable device
                    // preserves the target failure path: return zero and do
                    // not manufacture a successful mode value.
                    let destination = self.pop_ptr()?;
                    if let Some(mode) = api.query_cd_audio_mode() {
                        self.write_int(destination, 2, mode as u32)?;
                        self.clear_shadow_values(destination, 4);
                        Value::Int(1)
                    } else {
                        self.write_int(destination, 2, u32::MAX)?;
                        self.clear_shadow_values(destination, 4);
                        Value::Int(0)
                    }
                } else if (code, id) == (0x90, 0xb7) {
                    let descriptor_ptr = self.pop_ptr()?;
                    let window = self.pop_int()?;
                    if !api.graph_window_exists(window) {
                        Value::Int(1)
                    } else if let Some(status) =
                        self.validate_graph_input_descriptor(descriptor_ptr, true)?
                    {
                        Value::Int(status)
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
                                    self.timing.tick_count().max(0) as u32,
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
                    if (code, id) == (0x92, 0x14) {
                        let pending = self.read_int(1644, 2)?.saturating_sub(1);
                        self.write_int(1644, 2, pending)?;
                        if trace_events {
                            tracing::info!(pending, "VM graph preload completed");
                        }
                    }
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
            BpOpcode::Known { name: "snd1", .. } => {
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
            BpOpcode::Known {
                name: "usr1" | "usr2",
                ..
            } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default() as u16;
                self.note_call("user", code, id);
                api.observe_dispatch(NativeOpcode { group: code, id });
                if fail_on_stub
                    && !native_call::is_strictly_supported(NativeOpcode { group: code, id })
                {
                    self.note_stub("user", code, id);
                    return Err(VmError::UnknownDispatch { group: code, id });
                }
                self.normalize_user_string_args(code, id)?;
                let direct_result = if (code, id) == (0xb0, 0x27) {
                    let destination = self.pop_ptr()?;
                    let text = api.current_user_text();
                    let bytes_written = self.write_c_string_bounded(destination, &text, 255)?;
                    Some(Value::Int(bytes_written as i32))
                } else if (code, id) == (0xb0, 0x84) {
                    // sub_478D70: mode, initial, title, output buffer. A negative
                    // mode enables the target signed-decimal edit filter.
                    let mode = self.pop_int()?;
                    let initial = self.pop_string_lossy()?;
                    let title = self.pop_string_lossy()?;
                    let destination = self.pop_ptr()?;
                    let max_bytes = target_dialog_max_bytes(mode, 255);
                    let accepted = match api.show_user_dialog(UserDialogRequest::Input {
                        title,
                        initial,
                        max_bytes,
                        numeric: mode < 0,
                    }) {
                        Some(UserDialogResponse::Input(value)) => {
                            self.write_c_string_bounded(destination, &value, max_bytes)?;
                            true
                        }
                        _ => false,
                    };
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0x85) {
                    // sub_478DC0: two independent 256-byte output buffers.
                    let second_max = self.pop_int()?;
                    let second_initial = self.pop_string_lossy()?;
                    let second_label = self.pop_string_lossy()?;
                    let first_max = self.pop_int()?;
                    let first_initial = self.pop_string_lossy()?;
                    let first_label = self.pop_string_lossy()?;
                    let title = self.pop_string_lossy()?;
                    let second_destination = self.pop_ptr()?;
                    let first_destination = self.pop_ptr()?;
                    let first_max_bytes = target_dialog_max_bytes(first_max, 255);
                    let second_max_bytes = target_dialog_max_bytes(second_max, 255);
                    let accepted = match api.show_user_dialog(UserDialogRequest::TwoField {
                        title,
                        first_label,
                        first_initial,
                        first_max_bytes,
                        first_numeric: false,
                        second_label,
                        second_initial,
                        second_max_bytes,
                        second_numeric: false,
                    }) {
                        Some(UserDialogResponse::TwoField([first, second])) => {
                            self.write_c_string_bounded(
                                first_destination,
                                &first,
                                first_max_bytes,
                            )?;
                            self.write_c_string_bounded(
                                second_destination,
                                &second,
                                second_max_bytes,
                            )?;
                            true
                        }
                        _ => false,
                    };
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0x86) {
                    // sub_478E60: four fixed segments joined with '-'.
                    let _extra = self.pop_int()?;
                    let signed_max = self.pop_int()?;
                    let prompt = self.pop_string_lossy()?;
                    let title = self.pop_string_lossy()?;
                    let destination = self.pop_ptr()?;
                    let max_bytes_per_segment = target_dialog_max_bytes(signed_max, 32);
                    let accepted = match api.show_user_dialog(UserDialogRequest::Segmented {
                        title,
                        prompt,
                        segment_count: 4,
                        max_bytes_per_segment,
                        numeric: signed_max < 0,
                    }) {
                        Some(UserDialogResponse::Segmented(value)) => {
                            let capacity =
                                max_bytes_per_segment.saturating_mul(4).saturating_add(3);
                            self.write_c_string_bounded(destination, &value, capacity)?;
                            true
                        }
                        _ => false,
                    };
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0x87) {
                    // sub_478EC0: extended two-field template with independent
                    // numeric filters and output buffers.
                    let second_numeric = self.pop_int()? != 0;
                    let second_max = self.pop_int()?;
                    let second_initial = self.pop_string_lossy()?;
                    let second_label = self.pop_string_lossy()?;
                    let first_numeric = self.pop_int()? != 0;
                    let first_max = self.pop_int()?;
                    let first_initial = self.pop_string_lossy()?;
                    let first_label = self.pop_string_lossy()?;
                    let title = self.pop_string_lossy()?;
                    let second_destination = self.pop_ptr()?;
                    let first_destination = self.pop_ptr()?;
                    let _template = self.pop_int()?;
                    let first_max_bytes = target_dialog_max_bytes(first_max, 255);
                    let second_max_bytes = target_dialog_max_bytes(second_max, 255);
                    let accepted = match api.show_user_dialog(UserDialogRequest::TwoField {
                        title,
                        first_label,
                        first_initial,
                        first_max_bytes,
                        first_numeric,
                        second_label,
                        second_initial,
                        second_max_bytes,
                        second_numeric,
                    }) {
                        Some(UserDialogResponse::TwoField([first, second])) => {
                            self.write_c_string_bounded(
                                first_destination,
                                &first,
                                first_max_bytes,
                            )?;
                            self.write_c_string_bounded(
                                second_destination,
                                &second,
                                second_max_bytes,
                            )?;
                            true
                        }
                        _ => false,
                    };
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0x8c) {
                    // sub_478F80 builds one list from newline-delimited text and
                    // writes the selected row through the caller buffer.
                    let option_text = self.pop_string_lossy()?;
                    let prompt = self.pop_string_lossy()?;
                    let title = self.pop_string_lossy()?;
                    let destination = self.pop_ptr()?;
                    let options = option_text
                        .split('\n')
                        .map(|line| line.trim_end_matches('\r').to_string())
                        .filter(|line| !line.is_empty())
                        .collect::<Vec<_>>();
                    let accepted = match api.show_user_dialog(UserDialogRequest::Selection {
                        title,
                        prompt,
                        options,
                    }) {
                        Some(UserDialogResponse::Selection(value)) => {
                            self.write_c_string_bounded(destination, &value, 779)?;
                            true
                        }
                        _ => false,
                    };
                    Some(Value::Int(i32::from(accepted)))
                } else if (code, id) == (0xb0, 0x8f) {
                    // sub_478FD0 uses four mutable 10-byte text fields and two
                    // caller-owned zero-based month/day indices.
                    let day_destination = self.pop_ptr()?;
                    let month_destination = self.pop_ptr()?;
                    let fourth_destination = self.pop_ptr()?;
                    let third_destination = self.pop_ptr()?;
                    let second_destination = self.pop_ptr()?;
                    let first_destination = self.pop_ptr()?;
                    let read_initial = |vm: &Vm, pointer: u32| -> VmResult<String> {
                        if pointer == 0 {
                            Ok(String::new())
                        } else {
                            vm.read_c_string(pointer)
                        }
                    };
                    let fields = [
                        read_initial(self, first_destination)?,
                        read_initial(self, second_destination)?,
                        read_initial(self, third_destination)?,
                        read_initial(self, fourth_destination)?,
                    ];
                    let month_index = if month_destination == 0 {
                        0
                    } else {
                        self.read_int(month_destination, 2)? as i32
                    };
                    let day_index = if day_destination == 0 {
                        0
                    } else {
                        self.read_int(day_destination, 2)? as i32
                    };
                    let accepted = match api.show_user_dialog(UserDialogRequest::DateFields {
                        fields,
                        month_index,
                        day_index,
                    }) {
                        Some(UserDialogResponse::DateFields {
                            fields,
                            month_index,
                            day_index,
                        }) => {
                            for (destination, value) in [
                                first_destination,
                                second_destination,
                                third_destination,
                                fourth_destination,
                            ]
                            .into_iter()
                            .zip(fields.iter())
                            {
                                self.write_c_string_bounded(destination, value, 10)?;
                            }
                            if month_destination != 0 {
                                self.write_int(month_destination, 2, month_index as u32)?;
                                self.clear_shadow_values(month_destination, 4);
                            }
                            if day_destination != 0 {
                                self.write_int(day_destination, 2, day_index as u32)?;
                                self.clear_shadow_values(day_destination, 4);
                            }
                            true
                        }
                        _ => false,
                    };
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
                    let count = usize::try_from(count.max(0))
                        .unwrap_or(usize::MAX)
                        .min(4096);
                    let (first, second) = if count == 1 && first_source == 0 {
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
                            second.push(if second_source == 0 {
                                0
                            } else {
                                self.read_int(second_source.wrapping_add(offset), 2)? as i32
                            });
                        }
                        (first, second)
                    };
                    let _ = api.configure_particle_frame_tables(handle, &first, &second);
                    Some(Value::None)
                } else if (code, id) == (0xc0, 0xc2) {
                    // sub_476E40 / sub_494A60: [handle, duration,
                    // point_record_ptr, point_count]. Every point occupies
                    // four DWORDs; x/y/z are the first three and the fourth is
                    // target padding.
                    let count = self.pop_int()?;
                    let source = self.pop_ptr()?;
                    let duration = self.pop_int()?;
                    let handle = self.pop_int()?;
                    let status = if count < 2 {
                        2
                    } else if duration < 2 {
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
                    // sub_476ED0 / sub_4066C0 loads a BWEF resource and writes
                    // count pairs {base + offset, stride}. Native pop order is
                    // base, resource, count_out, table_out, archive.
                    let base = self.pop_int()?;
                    let resource = self.pop_string_lossy()?;
                    let count_out = self.pop_ptr()?;
                    let table_out = self.pop_ptr()?;
                    let archive = self.pop_string_lossy()?;
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
            BpOpcode::Known {
                name: "legacy_3d", ..
            } => {
                let id = instruction.raw.get(1).copied().unwrap_or_default();
                self.note_call("legacy3d", code, u16::from(id));
                if let Some(result) = self.execute_legacy_3d(id)? {
                    self.push_value(result);
                }
            }
            BpOpcode::Known {
                name: "debug_inspect",
                ..
            } => {
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
        {
            // These selectors install a procedure only on a target-confirmed
            // conditional path. Their owning handlers consume the relevant
            // gate and preserve the synchronous/immediate-failure path; do
            // not fabricate an unrecovered procedure when that path is taken.
            return;
        }
        if native_abi::installs_procedure(opcode.group, opcode.id)
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
                // CProcedure::DrainCallbacks sets the base terminal latch
                // after draining any non-empty callback queue, regardless of
                // the subclass callback code.
                let cancelled = !self.thread.take_procedure_callbacks().is_empty();
                let now_tick = self.timing.tick_count().max(0) as u32;
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
                let callbacks = self.thread.take_procedure_callbacks();
                let callback_completed = callbacks.iter().any(|callback| callback[0].as_i32() == 1);
                // sub_431AF0 sets CProcedure+0x10 after every non-empty drain;
                // CProcWaitTimingEx's code-1 handler additionally sets +0x28.
                let cancelled = !callbacks.is_empty();
                let input_interrupted = procedure.input_enabled()
                    && api.query_input_event_bits(procedure.input_scope()) != 0;
                let now_tick = self.timing.tick_count().max(0) as u32;
                let deadline_reached = now_tick >= procedure.native.base.deadline_tick;
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
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        input_interrupted,
                        callback_completed,
                        cancelled,
                        deadline_reached,
                        deadline_tick = self.thread.deadline_tick(),
                        "CProcWaitTimingEx completed"
                    );
                }
                None
            }
            CProcedure::WaitWndMsg(procedure) => {
                let cancelled = !self.thread.take_procedure_callbacks().is_empty();
                let result = if cancelled {
                    // Target's engine-disabled path pushes -1 then 0.
                    Some((-1, 0))
                } else {
                    api.poll_window_message(procedure.message_id, procedure.registered_after_serial)
                };
                let Some((lparam, wparam)) = result else {
                    return Some(VmStopReason::WaitingForProcedure);
                };
                self.thread.clear_current_procedure();
                self.push_value(Value::Int(lparam));
                self.push_value(Value::Int(wparam));
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
                let now_tick = self.timing.tick_count().max(0) as u32;
                let mut cancel = false;
                for callback in self.thread.take_procedure_callbacks() {
                    match callback[0].as_i32() {
                        0 => cancel = true,
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
                    return None;
                }

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
                    return None;
                }

                if !api.native_message_is_animating() && procedure.native.end_wait_policy == 0 {
                    // sub_433E40 finalizes immediately when +0x38 is zero.
                    api.finish_native_message();
                    api.unregister_message_input_scope(procedure.config.input_scope);
                    self.thread.clear_current_procedure();
                    return None;
                }

                // Target +0x5c is armed only after the text processor reaches
                // the end, not at construction time.
                if let Some(delay) = procedure.config.auto_advance_delay_ms {
                    if !api.native_message_is_animating() {
                        let deadline = *procedure
                            .auto_deadline_tick
                            .get_or_insert_with(|| now_tick.saturating_add(delay.max(0) as u32));
                        procedure.native.auto_deadline_tick = deadline;
                        if now_tick >= deadline {
                            api.finish_native_message();
                            api.unregister_message_input_scope(procedure.config.input_scope);
                            self.thread.clear_current_procedure();
                            return None;
                        }
                    }
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
                let cancelled = !self.thread.take_procedure_callbacks().is_empty();
                match procedure.mode {
                    native_thread::NativeGraphProcedureMode::Select => {
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
                let cancelled = !self.thread.take_procedure_callbacks().is_empty()
                    || self.system_wait_state == 0;
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
        let Some(argc) = known_call_arg_count(group, id) else {
            return Ok(Vec::new());
        };
        let mut args = Vec::with_capacity(argc);
        for _ in 0..argc {
            args.push(self.pop_value()?);
        }
        args.reverse();
        Ok(args)
    }

    fn settle_native_call_outputs(
        &mut self,
        group: u8,
        id: u16,
        call_stack: &mut Vec<Value>,
        result: Value,
    ) -> Value {
        let output_count = known_call_stack_output_count(group, id);
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
        let output_count = known_call_stack_output_count(group, id);
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
            || !known_call_returns_value(group, id)
            || native_abi::installs_procedure(group, id)
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
    }

    fn sync_thread_program_region(&mut self) {
        let code_used_end = self.active_code_used_end();
        self.thread
            .sync_program_region(self.target_loaded_programs.len(), code_used_end);
    }

    fn resolve_target_code_offset(&self, target_offset: u32) -> Option<(usize, u32)> {
        // Prefer the newest module when a freed region has been reused.
        for index in (1..self.programs.len()).rev() {
            if !self.program_active.get(index).copied().unwrap_or(false) {
                continue;
            }
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

    fn append_target_loaded_program(&mut self, program: BpProgram) -> u32 {
        let code_base = self.active_code_used_end();
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
                .get(&Self::value_key(*ptr))
                .and_then(|value| match value {
                    Value::Program(program) => Some(program.as_ref()),
                    _ => None,
                }),
            Value::Int(ptr) => {
                self.mem_values
                    .get(&Self::value_key(*ptr as u32))
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

    fn pop_int(&mut self) -> VmResult<i32> {
        Ok(self.pop_value()?.as_i32())
    }

    fn pop_ptr(&mut self) -> VmResult<u32> {
        Ok(Self::translate_system_descriptor(self.pop_int()? as u32))
    }

    fn value_as_numeric_operand(&mut self, value: Value) -> VmResult<i32> {
        match value {
            Value::Str(text) => {
                if let Some((&addr, _)) = self
                    .script_records
                    .iter()
                    .find(|(_, existing)| existing.as_str() == text.as_str())
                {
                    let offset = addr.saturating_sub(LOCAL_MEMORY_BASE);
                    return Ok((0x1200_0000 | offset) as i32);
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

    fn resolve_range(&self, ptr: u32, size: usize) -> VmResult<std::ops::Range<usize>> {
        let addr = Self::memory_addr(ptr);
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
        let addr = Self::memory_addr(ptr);
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
        if start >= HEAP_MEMORY_BASE && size != 0 {
            self.mark_shared_heap_dirty(start..end);
        }
        Ok(start..end)
    }

    fn translate_system_descriptor(ptr: u32) -> u32 {
        let slot = ptr.wrapping_sub(SYSTEM_PROGRAM_DESCRIPTOR_BASE);
        if slot < SYSTEM_PROGRAM_SLOTS as u32 {
            SYSTEM_PROGRAM_TABLE + slot * SYSTEM_PROGRAM_STRIDE
        } else {
            ptr
        }
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

    fn write_return_addr(&mut self, program_index: usize, next_pc: usize) -> VmResult<()> {
        let return_offset = self
            .programs
            .get(program_index)
            .and_then(|program| program.instructions.get(next_pc))
            .map(|instruction| instruction.offset as u32)
            .unwrap_or_default();
        self.write_int(0x1200_0000 | self.mem_ptr, 2, return_offset)?;
        self.mem_ptr = self.mem_ptr.saturating_add(4);
        Ok(())
    }

    fn read_return_addr(&mut self) -> VmResult<u32> {
        self.mem_ptr = self.mem_ptr.saturating_sub(4);
        self.read_int(0x1200_0000 | self.mem_ptr, 2)
    }

    fn read_value(&self, ptr: u32, width: u8) -> VmResult<Value> {
        let addr = Self::value_key(ptr);
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
            descriptor.groups.push(GraphInputGroup {
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
                let mask_resource = self.read_int(region_ptr.wrapping_add(48), 2)? as i32;
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
            descriptor.groups.push(GraphInputGroup {
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

    fn handle_text_measure_call(&mut self) -> VmResult<()> {
        let _flags = self.pop_value()?;
        let _style = self.pop_value()?;
        let _scale = self.pop_value()?;
        let font_size = self.pop_value()?.as_i32().max(1);
        let max_width = self.pop_value()?.as_i32().max(0);
        let text = self.pop_value()?;
        let dest = self.pop_ptr()?;
        let measured = measure_text_width_value(&text, font_size, max_width);
        self.write_int(dest, 2, measured as u32)?;
        Ok(())
    }

    fn write_value(&mut self, ptr: u32, width: u8, value: &Value) -> VmResult<()> {
        let addr = Self::value_key(ptr);
        self.write_int(ptr, width, value.as_i32() as u32)?;
        if width >= 2 {
            match value {
                Value::Int(_) | Value::None => {
                    self.mem_values.remove(&addr);
                }
                Value::Ptr(_) | Value::Str(_) | Value::Func { .. } | Value::Program(_) => {
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
        let start = Self::memory_addr(ptr);
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
        let size = size.max(4).saturating_add(3) & !3;
        let previous_heap_ptr = self.heap_ptr;
        let offset = if let Some(index) =
            self.heap_free_blocks
                .iter()
                .position(|(offset, available)| {
                    *available >= size && Self::heap_block_fits_segment(*offset, size)
                }) {
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
                if std::env::var_os("TRACE_HEAP").is_some() {
                    tracing::warn!(
                        thread = self.thread.thread_id(),
                        requested = size,
                        heap_ptr = format_args!("0x{:08X}", self.heap_ptr),
                        allocation_count = self.heap_allocations.len(),
                        free_block_count = self.heap_free_blocks.len(),
                        "VM heap allocation exhausted the tagged address space"
                    );
                }
                return 0;
            };
            self.heap_ptr = next;
            offset
        };
        let ptr = Self::heap_tagged_ptr(offset);
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
        let Some(offset) = Self::heap_logical_offset(ptr) else {
            return false;
        };
        let Some(size) = self.heap_allocations.remove(&offset) else {
            return false;
        };
        self.clear_shadow_values(ptr, size as usize);
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

    fn heap_block_fits_segment(offset: u32, size: u32) -> bool {
        size <= AUX_MEMORY_SEGMENT_SIZE
            && (offset & ADDRESS_MASK)
                .checked_add(size)
                .is_some_and(|end| end <= AUX_MEMORY_SEGMENT_SIZE)
    }

    fn heap_tagged_ptr(offset: u32) -> u32 {
        let segment = offset / AUX_MEMORY_SEGMENT_SIZE;
        let tag = AUX_MEMORY_TAG_BASE.saturating_add(segment.saturating_mul(2));
        (tag << 24) | (offset & ADDRESS_MASK)
    }

    fn heap_logical_offset(ptr: u32) -> Option<u32> {
        let tag = ptr >> 24;
        if tag < AUX_MEMORY_TAG_BASE {
            return None;
        }
        let segment = (tag >> 1).checked_sub(AUX_MEMORY_TAG_BASE >> 1)?;
        segment
            .checked_mul(AUX_MEMORY_SEGMENT_SIZE)?
            .checked_add(ptr & ADDRESS_MASK)
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
        let (text, _, _) = encoding.decode(&bytes);
        Ok(text.into_owned())
    }

    fn read_c_string_bytes(&self, ptr: u32) -> VmResult<Vec<u8>> {
        let start = Self::memory_addr(ptr) as usize;
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
        let start = Self::memory_addr(ptr) as usize;
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
        let start = Self::memory_addr(ptr) as usize;
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
        let len = encoding_rs::SHIFT_JIS
            .encode(text)
            .0
            .len()
            .saturating_add(1);
        self.restore_script_records(ptr, len)?;
        Ok(())
    }

    fn copy_c_string_value(&mut self, dst: u32, source: Value) -> VmResult<()> {
        match source {
            Value::Str(text) => self.write_c_string(dst, &text),
            Value::Int(ptr) if ptr != 0 => {
                let src = Self::translate_system_descriptor(ptr as u32);
                self.copy_c_string_pointer(dst, src)
            }
            Value::Ptr(ptr) if ptr != 0 => {
                let src = Self::translate_system_descriptor(ptr);
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
        if let Some(Value::Str(text)) = self.mem_values.get(&Self::value_key(src)).cloned() {
            return self.write_c_string(dst, &text);
        }
        let size = self.c_string_byte_len(src)?.saturating_add(1);
        self.copy_buffer(dst, src, size)
    }

    fn write_c_string_raw(&mut self, ptr: u32, text: &str) -> VmResult<()> {
        let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(text);
        let bytes = encoded.as_ref();
        let range = self.resolve_write_range(ptr, bytes.len().saturating_add(1))?;
        let start = range.start;
        self.memory[start..start + bytes.len()].copy_from_slice(bytes);
        self.memory[start + bytes.len()] = 0;
        self.trace_watch_write(ptr, bytes.len().saturating_add(1), 0, "write_c_string");
        Ok(())
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

    fn resource_file_exists_with_search<A>(&self, api: &mut A, archive: &str, file: &str) -> bool
    where
        A: SysApi,
    {
        let mut archives = vec![archive.to_string()];
        if let Some(components) = self.composite_archives.get(archive) {
            archives.extend(components.iter().cloned());
        }
        if archives
            .iter()
            .any(|candidate_archive| api.file_exists(candidate_archive, file))
        {
            return true;
        }
        for root in [
            self.primary_resource_root.as_deref(),
            self.secondary_resource_root.as_deref(),
        ]
        .into_iter()
        .flatten()
        {
            if api.file_exists("", &join_native_path(root, file)) {
                return true;
            }
        }
        if self.additional_resource_search_enabled {
            for path in &self.additional_resource_paths {
                let candidate = join_native_path(path, file);
                if archives
                    .iter()
                    .any(|candidate_archive| api.file_exists(candidate_archive, &candidate))
                {
                    return true;
                }
            }
        }
        false
    }

    fn resource_file_size_with_search<A>(&self, api: &mut A, archive: &str, file: &str) -> i32
    where
        A: SysApi,
    {
        let mut archives = vec![archive.to_string()];
        if let Some(components) = self.composite_archives.get(archive) {
            archives.extend(components.iter().cloned());
        }
        for candidate_archive in &archives {
            let size = api.file_size(candidate_archive, file);
            if size >= 0 {
                return size;
            }
        }
        for root in [
            self.primary_resource_root.as_deref(),
            self.secondary_resource_root.as_deref(),
        ]
        .into_iter()
        .flatten()
        {
            let size = api.file_size("", &join_native_path(root, file));
            if size >= 0 {
                return size;
            }
        }
        if self.additional_resource_search_enabled {
            for path in &self.additional_resource_paths {
                let candidate = join_native_path(path, file);
                for candidate_archive in &archives {
                    let size = api.file_size(candidate_archive, &candidate);
                    if size >= 0 {
                        return size;
                    }
                }
            }
        }
        -1
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

    fn sys80_31_read_file_range<A>(&mut self, api: &mut A) -> VmResult<Value>
    where
        A: SysApi,
    {
        // Target pop order: length, offset, file, optional secondary root,
        // destination. The packed u64 passed to sub_465C30 stores offset in
        // the low dword and requested length in the high dword.
        let requested_length = self.pop_int()? as u32 as usize;
        let offset = self.pop_int()? as u32 as usize;
        let file = self.pop_string_lossy()?;
        let secondary_root = self.pop_string_lossy()?;
        let destination = self.pop_ptr()?;

        let bytes = self
            .load_resource_bytes_with_search(api, "", &file)
            .map(|(bytes, _)| bytes)
            .or_else(|| {
                (!secondary_root.is_empty())
                    .then(|| api.load_file_bytes(&secondary_root, &file))
                    .flatten()
            });
        let Some(bytes) = bytes else {
            return Ok(Value::Int(1));
        };
        if bytes.len() > 0x0400_0000 {
            return Ok(Value::Int(6));
        }
        let length = if offset == 0 && requested_length == 0 {
            bytes.len()
        } else {
            requested_length
        };
        if length == 0 || length > bytes.len() {
            return Ok(Value::Int(3));
        }
        let Some(end) = offset.checked_add(length) else {
            return Ok(Value::Int(2));
        };
        if end > bytes.len() {
            return Ok(Value::Int(2));
        }
        let range = self.resolve_write_range(destination, length)?;
        self.memory[range].copy_from_slice(&bytes[offset..end]);
        self.clear_shadow_values(destination, length);
        Ok(Value::Int(0))
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
        self.post_async_program_message(thread_or_program, message, trace_events);
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
        let messages_ptr = self.pop_ptr()?;
        let message_count = self.pop_int()?.max(0).min(64) as usize;
        let thread_or_program = self.pop_value()?;
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

    /// System80:0x4B — bounded dequeue into a caller-provided array.
    ///
    /// Target evidence: System80 slot 0x0050442C and repeated target calls to
    /// `BPThread_DequeueDword`. Returns the actual number of dequeued values.
    ///
    /// BP push order: `[max_count, output_ptr]`; native pop order:
    /// `output_ptr`, then `max_count`.
    fn sys80_4b_dequeue_message_array(&mut self) -> VmResult<Value> {
        let output_ptr = self.pop_ptr()?;
        let max_count = self.pop_int()?.max(0).min(256) as usize;
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

    fn validate_structured_history_record(
        record: &system80_state::StructuredHistoryRecord,
    ) -> VmResult<()> {
        for (index, text) in record.short_text.iter().enumerate() {
            if encoding_rs::SHIFT_JIS.encode(text).0.len() >= 32 {
                return Err(VmError::Runtime(format!(
                    "Sys80 structured-history short string {} exceeds 31 bytes",
                    index + 1
                )));
            }
        }
        if encoding_rs::SHIFT_JIS.encode(&record.text).0.len() >= 256 {
            return Err(VmError::Runtime(
                "Sys80 structured-history text exceeds 255 bytes".into(),
            ));
        }
        if encoding_rs::SHIFT_JIS.encode(&record.extended_text).0.len() >= 512 {
            return Err(VmError::Runtime(
                "Sys80 structured-history extended text exceeds 511 bytes".into(),
            ));
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
        Self::validate_structured_history_record(&record)?;
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
        Self::validate_structured_history_record(&record)?;
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
                "Sys80 structured-history index out of range: {index}"
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
    fn sys80_84_intern_resource_name(&mut self) -> VmResult<Value> {
        let name = self.pop_string_lossy()?;
        if !self.resource_names.iter().any(|entry| entry == &name) {
            self.resource_names.push(name);
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
            self.resource_names.iter().any(|entry| entry == &name),
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
        let Ok(bit_count) = usize::try_from(bit_count) else {
            return Ok(Value::Int(0));
        };
        if bit_count == 0 || name.is_empty() {
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
        let status = match (
            self.read_flags.get_mut(&name),
            u32::try_from(start_bit),
            u32::try_from(bit_count),
        ) {
            (None, _, _) => 1,
            (Some(_), Err(_), _) => 2,
            (Some(_), _, Err(_) | Ok(0)) => 3,
            (Some(_), _, Ok(bit_count)) if bit_count > 65_536 => 3,
            (Some(flags), Ok(start_bit), Ok(bit_count)) => {
                if flags.set_range(start_bit, bit_count, enabled) {
                    0
                } else {
                    3
                }
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
    fn sys80_8b_query_read_flag_bit(&mut self) -> VmResult<Value> {
        let bit_offset = self.pop_int()?;
        let output_ptr = self.pop_ptr()?;
        let name = self.pop_string_lossy()?;
        let (status, enabled) = match (self.read_flags.get(&name), u32::try_from(bit_offset)) {
            (None, _) => (1, false),
            (Some(_), Err(_)) => (2, false),
            (Some(flags), Ok(bit_offset)) => match flags.contains(bit_offset) {
                Some(enabled) => (0, enabled),
                None => (2, false),
            },
        };
        self.write_int(output_ptr, 2, u32::from(enabled))?;
        Ok(Value::Int(status))
    }

    fn try_builtin_sys_with_api<A: SysApi>(
        &mut self,
        api: &mut A,
        group: u8,
        id: u16,
    ) -> VmResult<Option<Value>> {
        let result = match (group, id) {
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
                let index = self.pop_int()?;
                let destination = self.pop_ptr()?;
                let value = if (0..5).contains(&index) {
                    let cached = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .coordinate_slots[index as usize];
                    cached.or_else(|| api.pointer_position(index))
                } else {
                    None
                };
                if let Some((x, y)) = value {
                    self.write_int(destination, 2, x as u32)?;
                    self.write_int(destination.wrapping_add(4), 2, y as u32)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x81, 0x08) => {
                let destination = self.pop_ptr()?;
                self.write_c_string(destination, &api.host_user_name())?;
                Value::None
            }
            (0x81, 0x09) => {
                let destination = self.pop_ptr()?;
                self.write_c_string(destination, &api.host_computer_name())?;
                Value::None
            }
            (0x81, 0x0a) => {
                let destination = self.pop_ptr()?;
                if let Some(brand) = target_cpu_brand_string() {
                    self.write_c_string(destination, &brand)?;
                    Value::Int(1)
                } else {
                    if destination != 0 {
                        self.write_c_string(destination, "")?;
                    }
                    Value::Int(0)
                }
            }
            (0x81, 0x0b) => {
                let _ignored = self.pop_ptr()?;
                let signature_destination = self.pop_ptr()?;
                let text = self.pop_ptr()?;
                if text != 0 {
                    let normalized = self
                        .read_c_string(text)?
                        .split_whitespace()
                        .collect::<Vec<_>>()
                        .join(" ");
                    self.write_c_string(text, &normalized)?;
                }
                for (index, value) in target_cpu_signature_words().into_iter().enumerate() {
                    self.write_int(
                        signature_destination.wrapping_add(index as u32 * 4),
                        2,
                        value,
                    )?;
                }
                Value::None
            }
            (0x81, 0x0c) => {
                let service_pack = self.pop_ptr()?;
                let version = self.pop_ptr()?;
                let values = if cfg!(target_os = "windows") {
                    [10u32, 0, 0, 2]
                } else {
                    [0u32, 0, 0, 0]
                };
                for (index, value) in values.into_iter().enumerate() {
                    self.write_int(version.wrapping_add(index as u32 * 4), 2, value)?;
                }
                self.write_c_string(service_pack, "")?;
                Value::None
            }
            (0x81, 0x0d) => {
                let available = self.pop_ptr()?;
                let total = self.pop_ptr()?;
                let (total_mb, available_mb) = api.host_physical_memory_mb();
                self.write_int(total, 2, total_mb)?;
                self.write_int(available, 2, available_mb)?;
                Value::None
            }
            (0x81, 0x0e) => {
                let destination = self.pop_ptr()?;
                let (width, height) = adjusted_desktop_dimensions(api.runtime_screen_dimensions());
                self.write_int(destination, 2, width as u32)?;
                self.write_int(destination.wrapping_add(4), 2, height as u32)?;
                Value::None
            }
            (0x81, 0x0f) => Value::Int(i32::from(api.window_minimized())),
            (0x81, 0x10) => {
                let replacement = self.pop_int()?;
                let index = self.pop_int()?;
                if !(0..256).contains(&index) {
                    Value::Int(0)
                } else {
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
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .keyboard_polling_override = enabled;
                Value::None
            }
            (0x81, 0x16) => {
                let distance = self.pop_int()?;
                let capacity = self.pop_int()?;
                let accepted = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .configure_pointer_history(capacity, distance);
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x17) => {
                let count = self.pop_int()?.max(0) as usize;
                let start = self.pop_int()?.max(0) as usize;
                let distances = self.pop_ptr()?;
                let points = self.pop_ptr()?;
                if let Some((x, y)) = api.pointer_position(0) {
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .record_pointer(x, y);
                }
                let samples = {
                    let state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    state
                        .pointer_history
                        .iter()
                        .skip(start)
                        .take(count)
                        .copied()
                        .collect::<Vec<_>>()
                };
                for (index, sample) in samples.iter().enumerate() {
                    self.write_int(points.wrapping_add(index as u32 * 8), 2, sample.x as u32)?;
                    self.write_int(
                        points.wrapping_add(index as u32 * 8 + 4),
                        2,
                        sample.y as u32,
                    )?;
                    let distance = samples.get(index + 1).map_or(-1, |next| {
                        let dx = sample.x.saturating_sub(next.x);
                        let dy = sample.y.saturating_sub(next.y);
                        (((i64::from(dx) * i64::from(dx) + i64::from(dy) * i64::from(dy)) as f64)
                            .sqrt()) as i32
                    });
                    self.write_int(distances.wrapping_add(index as u32 * 4), 2, distance as u32)?;
                }
                Value::Int(samples.len().min(i32::MAX as usize) as i32)
            }
            (0x81, 0x18) => {
                let enabled = self.pop_int()? != 0;
                let accepted = api.register_touch_input(enabled);
                if accepted {
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .touch_registered = enabled;
                }
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x19) => {
                let destination = self.pop_ptr()?;
                let records = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .touch_records
                    .clone();
                for (record_index, record) in records.iter().enumerate() {
                    for (field_index, value) in record.values.into_iter().enumerate() {
                        self.write_int(
                            destination.wrapping_add((record_index * 24 + field_index * 4) as u32),
                            2,
                            value as u32,
                        )?;
                    }
                }
                Value::Int(records.len().min(i32::MAX as usize) as i32)
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
                let destination = self.pop_ptr()?;
                let _device = self.pop_ptr()?;
                let (x, y) = api.pointer_position(0).unwrap_or_default();
                for (index, value) in [x, y, 0, 0, -1, 0].into_iter().enumerate() {
                    self.write_int(destination.wrapping_add(index as u32 * 4), 2, value as u32)?;
                }
                Value::Int(1)
            }
            (0x81, 0x1e) => {
                let button = self.pop_int()?;
                let valid = matches!(button, 1 | 2 | 4 | 5 | 6);
                Value::Int(i32::from(valid && api.inject_mouse_click(button)))
            }
            (0x81, 0x1f) => {
                let mask = self.pop_int()?;
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .message_auxiliary_input_mask = mask;
                Value::None
            }
            (0x81, 0x21) => {
                // amachoco.exe's sub_4953A0 pops source then destination and
                // converts the source from CP932 to UTF-8. Retain the decoded
                // string as a shadow value so later BP copies and graph calls
                // do not lose it by treating the pointer as an integer.
                let source = self.pop_value()?;
                let destination = self.pop_ptr()?;
                let text = self.value_as_native_string_lossy(source)?;
                self.write_c_string(destination, &text)?;
                self.mem_values
                    .insert(Self::value_key(destination), Value::Str(text));
                Value::None
            }
            (0x81, 0x28) => {
                let mode = self.pop_int()?;
                let path = self.pop_string_lossy()?;
                let handle_destination = self.pop_ptr()?;
                let bytes = api
                    .load_file_bytes("", &path)
                    .or_else(|| std::fs::read(&path).ok());
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
                let exists = std::fs::metadata(&path).is_ok() || api.file_exists("", &path);
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
                Value::Int(i32::from(std::fs::metadata(path).is_ok()))
            }
            (0x81, 0x2f) => {
                let path = self.pop_string_lossy()?;
                let directory = std::path::Path::new(&path);
                let test_path = directory.join(format!("BGI{:08x}.tmp", self.trace_id));
                let writable = std::fs::write(&test_path, []).is_ok();
                if writable {
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
                let bytes = std::fs::read(&path).or_else(|_| {
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
                    .or_else(|| std::fs::read(&path).ok())
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
                let exists = std::fs::metadata(path).is_ok();
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
                    api.file_exists("", &path) || std::fs::metadata(path).is_ok(),
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
                let available = std::fs::metadata(path).is_ok();
                if destination != 0 {
                    self.write_int(destination, 2, u32::from(available))?;
                }
                Value::Int(i32::from(available))
            }
            (0x81, 0x44) => {
                let data_size = self.pop_int()?;
                let code_size = self.pop_int()?;
                let operand_slots = self.pop_int()?;
                let entry_index = self.pop_int()?;
                let program = empty_loaded_program(format!(
                    "DCTChildThread:{entry_index}:{operand_slots}:{code_size}:{data_size}"
                ));
                let thread_id = self
                    .start_async_program_with_args(
                        Value::Program(Arc::new(program)),
                        Vec::new(),
                        false,
                    )
                    .unwrap_or(0);
                Value::Int(thread_id)
            }
            (0x81, 0x60) => {
                let second = self.pop_int()?;
                let first = self.pop_int()?;
                let index = self.pop_int()?;
                let status = if !(0..8).contains(&index) {
                    1
                } else if first == 0 || second == 0 {
                    2
                } else {
                    self.display_mode_slots[index as usize] = Some((first, second));
                    0
                };
                Value::Int(status)
            }
            (0x81, 0x61) => {
                let mut mode = self.config_input_mode;
                if mode == 2 {
                    let (width, height) =
                        adjusted_desktop_dimensions(api.runtime_screen_dimensions());
                    let required = self
                        .display_mode_slots
                        .iter()
                        .flatten()
                        .next()
                        .copied()
                        .unwrap_or((0, 0));
                    if width < required.0 || height < required.1 {
                        mode = 0;
                    }
                }
                Value::Int(mode)
            }
            (0x81, 0x62) => {
                let mode = self.pop_int()?;
                let accepted = (0..=1).contains(&mode) && api.set_window_monitor_adapter_mode(mode);
                if accepted {
                    self.window_monitor_adapter_mode = mode;
                }
                Value::Int(i32::from(accepted))
            }
            (0x81, 0x63) => {
                let mode = self.pop_int()?;
                let accepted = (0..=2).contains(&mode) && api.set_config_input_mode(mode);
                if accepted {
                    self.config_input_mode = mode;
                }
                Value::Int(i32::from(accepted))
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
                let destination = self.pop_ptr()?;
                let message = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .captured_error
                    .clone();
                if destination != 0 {
                    self.write_c_string(destination, &message)?;
                }
                let (bytes, _, _) = encoding_rs::SHIFT_JIS.encode(&message);
                Value::Int(bytes.len().saturating_add(1).min(i32::MAX as usize) as i32)
            }
            (0x81, 0x6d) => {
                let version = api.pixel_shader_version();
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .pixel_shader_version = version;
                Value::Int(i32::from(version))
            }
            (0x81, 0x6e) => Value::Int(i32::from(std::thread::panicking())),
            (0x81, 0x6f) => {
                let mode = self.pop_int()?;
                let accepted = match mode {
                    0 => api.set_shader_effect_enabled(false),
                    1 => api.set_shader_effect_enabled(true),
                    _ => false,
                };
                if accepted {
                    self.shader_effect_enabled = mode != 0;
                }
                Value::Int(i32::from(accepted))
            }
            (0x81, 0xb0) => {
                // Handler sub_48C2F0 pops helper a2 first and a1 second. The
                // distance is symmetric for non-empty strings, but target's
                // empty-a2 fast path is not, so preserve the native order.
                let right = self.pop_ptr()?;
                let left = self.pop_ptr()?;
                if left == 0 || right == 0 {
                    Value::Int(-1)
                } else {
                    let left = self.read_wide_c_string(left)?;
                    let right = self.read_wide_c_string(right)?;
                    Value::Int(wide_string_similarity(&left, &right))
                }
            }
            (0x81, 0xb7) => {
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                if source == 0 {
                    Value::Int(-1)
                } else {
                    let source_bytes = self.read_c_string_bytes(source)?;
                    let (decoded, _, _) = encoding_rs::SHIFT_JIS.decode(&source_bytes);
                    let encoded = decoded.encode_utf16().collect::<Vec<_>>();
                    if destination != 0 {
                        for (index, value) in encoded.iter().copied().enumerate() {
                            self.write_int(
                                destination.wrapping_add(index as u32 * 2),
                                1,
                                value as u32,
                            )?;
                        }
                        self.write_int(destination.wrapping_add(encoded.len() as u32 * 2), 1, 0)?;
                    }
                    Value::Int(encoded.len().min(i32::MAX as usize) as i32)
                }
            }
            (0x81, 0xd0) => {
                let capacity = self.pop_int()?;
                let destination = self.pop_ptr()?;
                let result = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .create_blob_table(capacity);
                match result {
                    Ok(handle) => {
                        self.write_int(destination, 2, handle)?;
                        Value::Int(0)
                    }
                    Err(status) => Value::Int(status),
                }
            }
            (0x81, 0xd1) => {
                let handle = self.pop_int()? as u32;
                let removed = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .blob_tables
                    .remove(&handle)
                    .is_some();
                Value::Int(if removed {
                    0
                } else {
                    system81_state::NATIVE_NOT_FOUND
                })
            }
            (0x81, 0xd2) => {
                let handle = self.pop_int()? as u32;
                let source = self.pop_ptr()?;
                let size = self.pop_int()?.max(0) as usize;
                let requested_index = self.pop_int()?.max(0) as usize;
                let index_destination = self.pop_ptr()?;
                if size == 0 {
                    Value::Int(system81_state::NATIVE_OPERATION_FAILED)
                } else {
                    let range = self.resolve_range(source, size)?;
                    let bytes = self.memory[range].to_vec();
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    if let Some(table) = state.blob_tables.get_mut(&handle) {
                        let index = if index_destination != 0 {
                            table.insert_first_free(bytes)
                        } else {
                            table.insert_at(requested_index, bytes);
                            requested_index
                        };
                        drop(state);
                        if index_destination != 0 {
                            self.write_int(index_destination, 2, index as u32)?;
                        }
                        Value::Int(0)
                    } else {
                        Value::Int(system81_state::NATIVE_NOT_FOUND)
                    }
                }
            }
            (0x81, 0xd3) => {
                let index = self.pop_int()?.max(0) as usize;
                let handle = self.pop_int()? as u32;
                let mut state = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned");
                let status = if let Some(table) = state.blob_tables.get_mut(&handle) {
                    if index < table.slots.len() && table.slots[index].take().is_some() {
                        0
                    } else {
                        system81_state::NATIVE_INVALID_INDEX
                    }
                } else {
                    system81_state::NATIVE_NOT_FOUND
                };
                Value::Int(status)
            }
            (0x81, 0xd4) => {
                let index = self.pop_int()?.max(0) as usize;
                let handle = self.pop_int()? as u32;
                let size_destination = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let result = {
                    let state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    match state.blob_tables.get(&handle) {
                        None => Err(system81_state::NATIVE_NOT_FOUND),
                        Some(table) => table
                            .slots
                            .get(index)
                            .and_then(|slot| slot.clone())
                            .ok_or(system81_state::NATIVE_INVALID_INDEX),
                    }
                };
                match result {
                    Ok(bytes) => {
                        if destination != 0 {
                            let range = self.resolve_range(destination, bytes.len())?;
                            self.memory[range].copy_from_slice(&bytes);
                            self.clear_shadow_values(destination, bytes.len());
                        }
                        if size_destination != 0 {
                            self.write_int(size_destination, 2, bytes.len() as u32)?;
                        }
                        Value::Int(0)
                    }
                    Err(status) => Value::Int(status),
                }
            }
            (0x81, 0xd5) => {
                let handle = self.pop_int()? as u32;
                let count_destination = self.pop_ptr()?;
                let pairs_destination = self.pop_ptr()?;
                let entries = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .blob_tables
                    .get(&handle)
                    .map(|table| {
                        table
                            .slots
                            .iter()
                            .enumerate()
                            .filter_map(|(index, slot)| {
                                slot.as_ref().map(|bytes| (index, bytes.len()))
                            })
                            .collect::<Vec<_>>()
                    });
                if let Some(entries) = entries {
                    if pairs_destination != 0 {
                        for (output_index, (index, size)) in entries.iter().copied().enumerate() {
                            self.write_int(
                                pairs_destination.wrapping_add((output_index * 8) as u32),
                                2,
                                index as u32,
                            )?;
                            self.write_int(
                                pairs_destination.wrapping_add((output_index * 8 + 4) as u32),
                                2,
                                size as u32,
                            )?;
                        }
                    }
                    if count_destination != 0 {
                        self.write_int(count_destination, 2, entries.len() as u32)?;
                    }
                    Value::Int(0)
                } else {
                    Value::Int(system81_state::NATIVE_NOT_FOUND)
                }
            }
            (0x81, 0xda) => {
                // amachoco.exe's sub_4961E0 queries an optional sentence
                // substitution table. Returning zero selects the script's
                // built-in fallback, which copies the original sentence via
                // Sys81:21.
                let _text = self.pop_value()?;
                let _keyword = self.pop_value()?;
                let _destination = self.pop_ptr()?;
                Value::Int(0)
            }
            (0x81, 0xe0) => {
                let flags = self.pop_int()?;
                let error_message = self.pop_string_lossy()?;
                let show_window = self.pop_int()? != 0;
                let working_directory = self.pop_string_lossy()?;
                let executable = self.pop_string_lossy()?;
                let arguments = self.pop_string_lossy()?;
                let exit_code_destination = self.pop_ptr()?;
                let mut exit_code = 0;
                let success = api.launch_process_wait(
                    &working_directory,
                    &executable,
                    &arguments,
                    &error_message,
                    show_window || flags != 0,
                    (exit_code_destination != 0).then_some(&mut exit_code),
                );
                if exit_code_destination != 0 {
                    self.write_int(exit_code_destination, 2, exit_code as u32)?;
                }
                Value::Int(i32::from(success))
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
                let name = self.pop_string_lossy()?;
                Value::Int(
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .create_named_mutex(name),
                )
            }
            (0x81, 0xed) => {
                let handle = self.pop_int()?;
                Value::Int(i32::from(
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .release_named_mutex(handle),
                ))
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
                let enabled = self.pop_int()? != 0;
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
                Value::Int(i32::from(api.read_input_state(descriptor) != 0))
            }
            (0x80, 0x12) => {
                // Target 0x00488040 treats the argument as a contiguous,
                // zero-terminated DWORD descriptor array. It sums
                // dword_518CA4[6 * descriptor] for every entry without
                // consuming the input state.
                let descriptors = self.pop_ptr()?;
                let mut total = 0i32;
                if descriptors != 0 {
                    let start = Self::memory_addr(descriptors) as usize;
                    if start >= self.memory.len() {
                        return Err(VmError::MemoryOutOfBounds {
                            addr: Self::memory_addr(descriptors),
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
                        return Err(VmError::Runtime(
                            "Sys80:1B input descriptor list exceeds 15 entries".into(),
                        ));
                    }
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
                                "Sys80:62 fullscreen hotkey descriptor list exceeds 15 entries"
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
                let sdc_header = b"SDC FORMAT 1.00";
                let has_sdc_header = self
                    .resolve_range(source, sdc_header.len())
                    .ok()
                    .is_some_and(|range| &self.memory[range] == sdc_header);
                let output = if has_sdc_header {
                    self.decode_sdc_records(source, destination)?
                } else {
                    // Target sub_465320 is the engine-wide resource/data
                    // decoder. Until every wrapped resource format is ported,
                    // preserve the destination/length contract with a bounded
                    // raw copy rather than inventing a successful decode size.
                    let start = source as usize;
                    if start >= self.memory.len() {
                        0
                    } else {
                        let count = requested.min(self.memory.len() - start);
                        let bytes = self.memory[start..start + count].to_vec();
                        let range = self.resolve_write_range(destination, count)?;
                        self.memory[range].copy_from_slice(&bytes);
                        self.clear_shadow_values(destination, count);
                        count.min(i32::MAX as usize) as i32
                    }
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
                    let byte_len = encoding_rs::SHIFT_JIS.encode(&value).0.len() + 1;
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
                let byte_len = encoding_rs::SHIFT_JIS.encode(text).0.len();
                self.write_int(output_length, 2, byte_len.min(u32::MAX as usize) as u32)?;
                Value::Int(0)
            }
            (0x80, 0x04) => Value::Int(self.timing.tick_count()),
            (0x80, 0x05) => {
                let ptr = self.pop_ptr()?;
                let counter = self.timing.performance_counter();
                self.write_int(ptr, 2, counter as u32)?;
                self.write_int(ptr.wrapping_add(4), 2, (counter >> 32) as u32)?;
                Value::Int(1)
            }
            (0x80, 0x20) => {
                let size = self.pop_int()?.max(0) as u32;
                Value::Ptr(self.alloc_heap(size))
            }
            (0x80, 0x21) => {
                let ptr = self.pop_ptr()?;
                Value::Int(i32::from(self.free_heap(ptr)))
            }
            (0x80, 0x33) => {
                let _mode = self.pop_value()?;
                let _key = self.pop_value()?;
                Value::None
            }
            (0x80, 0x36) => {
                let _enabled = self.pop_value()?;
                Value::None
            }
            (0x80, 0x37) => {
                let _path = self.pop_string_lossy()?;
                Value::None
            }
            (0x80, 0x38) => {
                let _records = self.pop_value()?;
                let _name = self.pop_value()?;
                Value::Int(0)
            }
            (0x80, 0x39) => {
                let _path = self.pop_string_lossy()?;
                Value::None
            }
            (0x80, 0x3a) => {
                let mode = self.pop_int()?;
                let destination = self.pop_ptr()?;
                if let Some(path) = api.special_folder_path(mode) {
                    self.write_c_string(destination, &path)?;
                    Value::Int(1)
                } else {
                    Value::Int(0)
                }
            }
            (0x80, 0x3b) => {
                let mode = self.pop_int()?;
                let title = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let extension = self.pop_string_lossy()?;
                let description = self.pop_string_lossy()?;
                let initial_directory = self.pop_string_lossy()?;
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
                let kind = self.pop_int()?;
                let ptr = self.pop_ptr()?;
                let root = match kind {
                    0 => self
                        .primary_resource_root
                        .clone()
                        .or_else(|| api.primary_resource_root()),
                    1 => self.secondary_resource_root.clone(),
                    _ => None,
                };
                if let Some(root) = root {
                    self.write_c_string(ptr, &ensure_trailing_separator(&root))?;
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
                let record_size = self.pop_int()?.max(0) as u32;
                let capacity = self.pop_int()?.max(0) as u32;
                let slot = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_open(slot, capacity, record_size)?)
            }
            (0x80, 0x9a) => {
                let handle = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_count(dst, handle)?)
            }
            (0x80, 0x9d) => {
                let index = self.pop_int()?.max(0) as u32;
                let handle = self.pop_ptr()?;
                let dst = self.pop_ptr()?;
                Value::Int(self.sys_indexed_record_load(dst, handle, index)?)
            }
            (0x80, 0x9e) => {
                let count = self.pop_int()?.max(0) as u32;
                let start = self.pop_int()?.max(0) as u32;
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
                let shift = self.pop_int()?;
                Value::Int(i32::from(self.allocate_global_config(shift)))
            }
            (0x80, 0x71) => {
                self.global_config.fill(0);
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
                let label = self.pop_string_lossy()?;
                let slot = self.pop_int()?;
                let _written = self.save_config_slot(api, slot, &label)?;
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
                let restore_parent_window = self.pop_int()? != 0;
                let error_message = self.pop_string_lossy()?;
                let command_line = self.pop_string_lossy()?;
                let working_directory = self.pop_string_lossy()?;
                Value::Int(i32::from(api.launch_process(
                    &working_directory,
                    &command_line,
                    &error_message,
                    restore_parent_window,
                    false,
                )))
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
                        "Sys80:E1 restart command-line pointer is null".into(),
                    ));
                }
                let error_message = if error_message_ptr == 0 {
                    String::new()
                } else {
                    self.read_c_string(error_message_ptr)?
                };
                let command_line = self.read_c_string(command_line_ptr)?;
                let working_directory = if working_directory_ptr == 0 {
                    String::new()
                } else {
                    self.read_c_string(working_directory_ptr)?
                };
                api.schedule_restart(&working_directory, &command_line, &error_message);
                self.scheduler_signal = Some(SchedulerSignal::TerminateInterpreter);
                Value::None
            }
            (0x80, 0xe2) => {
                let error_message = self.pop_string_lossy()?;
                let command_line = self.pop_string_lossy()?;
                let working_directory = self.pop_string_lossy()?;
                Value::Int(i32::from(api.launch_process(
                    &working_directory,
                    &command_line,
                    &error_message,
                    true,
                    true,
                )))
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
                let path = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
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
                let caption = self.pop_string_lossy()?;
                let mode = self.pop_int()?;
                let option_value2 = self.pop_int()?;
                let option_value1 = self.pop_int()?;
                let output_text = self.pop_ptr()?;
                let output_value2 = self.pop_ptr()?;
                let output_value1 = self.pop_ptr()?;
                let initial_text = self.pop_string_lossy()?;
                let request = SystemInputDialogRequest {
                    initial_text,
                    option_value1,
                    option_value2,
                    mode,
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
                let root = windows_directory.trim_end_matches(|ch| ch == '\\' || ch == '/');
                let path = format!("{root}\\{file_name}");
                let Some(mut bytes) = api.read_user_file_bytes(&path) else {
                    return Ok(Some(Value::Int(0)));
                };
                if bytes.len() < 2 || bytes[bytes.len() - 2] != b'\\' || bytes[bytes.len() - 1] != 0
                {
                    Value::Int(0)
                } else {
                    let trailing_slash = bytes.len() - 2;
                    bytes[trailing_slash] = 0;
                    let range = self.resolve_write_range(output, bytes.len())?;
                    self.memory[range].copy_from_slice(&bytes);
                    self.clear_shadow_values(output, bytes.len());
                    Value::Int(1)
                }
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
            (0xb0, 0xc4) => {
                let _value = self.pop_int()?;
                self.push_value(Value::Int(0));
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
            let Some(index) = self.stack.len().checked_sub(1 + from_top) else {
                continue;
            };
            let text = self.value_as_string_lossy(self.stack[index].clone())?;
            self.replace_stack_value(index, Value::Str(text));
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
            let Some(index) = self.stack.len().checked_sub(1 + from_top) else {
                continue;
            };
            if std::env::var_os("TRACE_SOUND_ARGS").is_some() {
                self.trace_sound_arg(group, id, from_top, index);
            }
            let text = if matches!(
                (group, id, from_top),
                (0xa0, 0x11, 2) | (0xa0, 0x12, 3 | 4) | (0xa0, 0x21, 2)
            ) {
                self.value_as_sound_file_string(self.stack[index].clone())?
            } else {
                self.value_as_string_lossy(self.stack[index].clone())?
            };
            self.replace_stack_value(index, Value::Str(text));
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
            if let Some(index) = self.stack.len().checked_sub(1 + from_top) {
                let pointer = match self.stack[index] {
                    Value::Int(value) => value as u32,
                    Value::Ptr(pointer) => pointer,
                    _ => 0,
                };
                if pointer != 0 {
                    let value = self.read_int(pointer, 2)? as i32;
                    self.replace_stack_value(index, Value::Int(value));
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
            (0xb0, 0xC0) | (0xb0, 0xC2) | (0xb0, 0xC4) => &[0],
            (0xb0, 0xC1) => &[1],
            (0xb0, 0xC3) | (0xb0, 0xC6) | (0xb0, 0xC7) => &[0, 1],
            (0xb0, 0xF0) => &[2],
            (0xc0, 0xF0) => &[1, 4],
            _ => return Ok(()),
        };
        for &from_top in positions_from_top {
            let Some(index) = self.stack.len().checked_sub(1 + from_top) else {
                continue;
            };
            let text = self.value_as_string_lossy(self.stack[index].clone())?;
            self.replace_stack_value(index, Value::Str(text));
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
            (0x90, 0x56) => &[0, 3],
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
            let Some(index) = self.stack.len().checked_sub(1 + from_top) else {
                continue;
            };
            let original = self.stack[index].clone();
            let encoding = if matches!(
                (group, id),
                (0x90, 0x90)
                    | (0x91, 0x91 | 0x93 | 0x9C | 0x9D)
                    | (0x92, 0x1C | 0x1D | 0x1E | 0x1F | 0x90 | 0x91 | 0x9C)
            ) {
                self.graph_text_encoding.unwrap_or(encoding_rs::SHIFT_JIS)
            } else {
                encoding_rs::SHIFT_JIS
            };
            let text = self.value_as_text_descriptor_string(original.clone(), encoding)?;
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
            if is_plausible_text_payload(&text) {
                self.replace_stack_value(index, Value::Str(text));
            }
        }
        Ok(())
    }

    fn trace_sound_arg(&self, group: u8, id: u16, from_top: usize, index: usize) {
        let value = self.stack.get(index).cloned().unwrap_or(Value::None);
        let ptr = match value {
            Value::Int(value) => Some(value as u32),
            Value::Ptr(ptr) => Some(ptr),
            _ => None,
        };
        let direct = self
            .stack
            .get(index)
            .cloned()
            .and_then(|value| self.value_as_string_lossy(value).ok())
            .unwrap_or_default();
        let dump = ptr
            .map(|ptr| self.memory_preview(ptr, 96))
            .unwrap_or_default();
        tracing::warn!(
            group = format_args!("0x{group:02X}"),
            id = format_args!("0x{id:02X}"),
            from_top,
            index,
            value = ?self.stack.get(index),
            direct,
            dump,
            "TRACE_SOUND_ARG"
        );
    }

    fn memory_preview(&self, ptr: u32, size: usize) -> String {
        let addr = Self::memory_addr(ptr) as usize;
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
                    .get(&Self::value_key(value as u32))
                    .and_then(|stored| match stored {
                        Value::Str(text) => Some(text),
                        _ => None,
                    })
                    .is_some() =>
                {
                    Ok(
                        match self
                            .mem_values
                            .get(&Self::value_key(value as u32))
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
                let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(&text);
                let mut bytes = Vec::with_capacity(size);
                bytes.extend_from_slice(&encoded);
                bytes.push(0);
                bytes.resize(size, 0);
                bytes.truncate(size);
                Ok(bytes)
            }
            Value::Int(ptr) => {
                let range =
                    self.resolve_range(Self::translate_system_descriptor(ptr as u32), size)?;
                Ok(self.memory[range].to_vec())
            }
            Value::Ptr(ptr) => {
                let range = self.resolve_range(Self::translate_system_descriptor(ptr), size)?;
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
            .get(&Self::value_key(addr))
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
            if let Some(Value::Str(text)) = self.mem_values.get(&Self::value_key(addr)) {
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
        if let Some(Value::Str(text)) = self.mem_values.get(&Self::value_key(ptr)) {
            Ok(text.clone())
        } else {
            self.read_c_string(ptr)
        }
    }

    fn pop_string_lossy(&mut self) -> VmResult<String> {
        let value = self.pop_value()?;
        self.value_as_native_string_lossy(value)
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

    pub(crate) fn memory_addr(ptr: u32) -> u32 {
        let tag = ptr >> 24;
        match tag {
            0x12 | 0x13 => LOCAL_MEMORY_BASE.saturating_add(ptr & ADDRESS_MASK),
            AUX_MEMORY_TAG_BASE..=u32::MAX => {
                let segment = (tag >> 1).saturating_sub(AUX_MEMORY_TAG_BASE >> 1);
                LOCAL_MEMORY_BASE
                    .saturating_add(segment.saturating_mul(AUX_MEMORY_SEGMENT_SIZE))
                    .saturating_add(ptr & ADDRESS_MASK)
            }
            _ => ptr & ADDRESS_MASK,
        }
    }

    pub(crate) fn value_key(ptr: u32) -> u32 {
        Self::memory_addr(ptr)
    }
}

fn strip_native_markup_tags(source: &str) -> String {
    let mut output = String::with_capacity(source.len());
    let mut rest = source;
    while let Some(start) = rest.find('<') {
        output.push_str(&rest[..start]);
        let tag = &rest[start..];
        let valid_tag = tag
            .as_bytes()
            .get(1)
            .is_some_and(|byte| byte.is_ascii_alphabetic() || *byte == b'/');
        if valid_tag {
            if let Some(end) = tag.find('>') {
                rest = &tag[end + 1..];
                continue;
            }
        }
        output.push('<');
        rest = &tag[1..];
    }
    output.push_str(rest);
    output
}

fn extract_native_labels(source: &str) -> Vec<String> {
    let lower = source.to_ascii_lowercase();
    let mut labels = Vec::new();
    let mut offset = 0;
    while let Some(relative_start) = lower[offset..].find("<l>") {
        let start = offset + relative_start + 3;
        let Some(relative_end) = lower[start..].find("</l>") else {
            break;
        };
        let end = start + relative_end;
        if end != start {
            labels.push(source[start..end].to_string());
        }
        offset = end + 4;
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
                    .map(Vm::memory_addr)
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
                let _flags = stack.pop();
                let _style = stack.pop();
                let _scale = stack.pop();
                let font_size = stack.pop().map(|value| value.as_i32()).unwrap_or(24);
                let max_width = stack.pop().map(|value| value.as_i32()).unwrap_or(0);
                let text = stack.pop().unwrap_or(Value::None);
                let _dest = stack.pop();
                let measured = measure_text_width_value(&text, font_size.max(1), max_width);
                return Ok(Value::Int(measured));
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
        graph_object_property: Option<(i32, i32, Result<i32, i32>)>,
        window_valid_region: Option<[i32; 4]>,
        caret_frame_call: Option<(u32, i32, Vec<i32>)>,
        message_animating: bool,
        message_reveals: usize,
        message_finishes: usize,
        window_message_serial: i32,
        window_messages: std::collections::VecDeque<(i32, i32, i32, i32)>,
        system92_text_output_pair: [i32; 2],
        system92_text_fragment_records: Vec<System92TextFragmentRecord>,
        performance_profiling_enabled: Option<bool>,
        performance_metric: i32,
        presentation_state: i32,
        graphics_capability_record: [u32; 16],
        graphics_memory_metric: i32,
        physical_memory_bytes: (u64, u64),
        fullscreen_hotkeys_enabled: bool,
        fullscreen_hotkeys: Vec<i32>,
        special_folder: Option<String>,
        file_dialog_result: Option<Result<Option<String>, i32>>,
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

        fn set_performance_profiling(&mut self, enabled: bool) {
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
            _initial_dir: &str,
            _description: &str,
            _extension: &str,
            _title: &str,
            _mode: i32,
        ) -> Result<Option<String>, i32> {
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
            let index = self
                .window_messages
                .iter()
                .position(|&(serial, id, _, _)| {
                    serial > registered_after_serial && id == message_id
                })?;
            let (_, _, lparam, wparam) = self.window_messages.remove(index)?;
            Some((lparam, wparam))
        }
    }

    impl GraphApi for SchedulingApi {
        fn native_message_is_animating(&self) -> bool {
            self.message_animating
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
            _width: i32,
            _height: i32,
            _format: i32,
            pixels: &[u8],
        ) -> bool {
            self.bitmap_pixels.insert(bitmap, pixels.to_vec());
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
            Ok(Value::None)
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

        fn query_graph91_object_property(&self, object: i32, parameter: i32) -> Result<i32, i32> {
            self.graph_object_property
                .as_ref()
                .filter(|(expected_object, expected_parameter, _)| {
                    *expected_object == object && *expected_parameter == parameter
                })
                .map(|(_, _, result)| result.clone())
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
            opcode_hex: format!("0x{code:02X}"),
            opcode_name: name.into(),
            operands,
            known_call: None,
            raw,
            warning: None,
        }
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
                vm.timing.tick_count().max(0) as u32,
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x5F], Vec::new()),
                test_instruction(
                    0x12,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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

        let second = vm.run_loaded(&mut api, &options);
        assert_eq!(second.stop_reason, VmStopReason::Completed);
        assert_eq!(second.steps, 1);
        assert_eq!(vm.stack, [Value::Int(99)]);
        assert_eq!(api.host_sys_calls, 0);
    }

    #[test]
    fn unknown_dispatch_halts_vm_instead_of_retrying_mutated_instruction() {
        let program = BpProgram {
            script_name: Some("unknown-dispatch-test".into()),
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![test_instruction(
                0x10,
                0x80,
                "sys1",
                vec![0x82, 0xff],
                Vec::new(),
            )],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x5A], Vec::new()),
                test_instruction(
                    0x12,
                    0x01,
                    "push_byte",
                    vec![0x01, 99],
                    vec![BpOperand::U8(99)],
                ),
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02, 0x10, 0x00, 0x00, 0x00],
                    vec![BpOperand::U32(0x10)],
                ),
                test_instruction(0x15, 0x14, "jmp", vec![0x14], Vec::new()),
            ],
            labels: [(0x10, 0)].into_iter().collect(),
            warnings: Vec::new(),
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
        assert_eq!(report.pc, 0);
    }

    #[test]
    fn native_move_returns_the_assigned_value_and_inline_copy_writes_payload() {
        let destination = 0x2400u32;
        let program = BpProgram {
            script_name: Some("native-memory-opcodes".into()),
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            Value::Str("/portable".into()),
            Value::Str("Save data".into()),
            Value::Str("dat".into()),
            Value::Ptr(file_out),
            Value::Str("Choose file".into()),
            Value::Int(1),
        ]);
        vm.dispatch(
            &test_instruction(0x12, 0x80, "sys1", vec![0x80, 0x3b], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
        assert_eq!(vm.read_c_string(file_out).unwrap(), "/portable/save.dat");
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

        vm.stack.push(Value::Int(1));
        vm.dispatch(
            &test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x06], Vec::new()),
            &mut api,
        )
        .unwrap();
        assert_eq!(api.performance_profiling_enabled, Some(true));

        vm.write_value(destination, 2, &Value::Ptr(0x1200_4321))
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
        vm.write_value(destination, 2, &Value::Ptr(0x1200_4321))
            .unwrap();
        assert_eq!(
            vm.read_value(destination, 2).unwrap(),
            Value::Ptr(0x1200_4321)
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
                test_instruction(
                    0x10,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x120)],
                ),
                test_instruction(0x15, 0x11, "store_base", vec![0x11], Vec::new()),
            ],
            labels: Default::default(),
            warnings: Vec::new(),
        };
        let mut api = SchedulingApi::default();
        let mut vm = Vm::new();
        vm.start(&program);
        vm.mem_ptr = 0x100;
        vm.write_value(0x1200_0118, 2, &Value::Ptr(0x1000_1234))
            .unwrap();

        let report = vm.run_loaded(&mut api, &VmRunOptions::default());

        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.read_int(0x1200_0118, 2).unwrap(), 0);
        assert!(!vm.mem_values.contains_key(&Vm::value_key(0x1200_0118)));
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
    fn native_heap_moves_large_allocations_to_the_next_auxiliary_segment() {
        let mut vm = Vm::new();
        vm.heap_ptr = AUX_MEMORY_SEGMENT_SIZE - 1024;

        let ptr = vm.alloc_heap(2048);

        assert_eq!(ptr >> 24, 0x22);
        assert_eq!(ptr & ADDRESS_MASK, 0);
        assert_eq!(
            Vm::memory_addr(ptr),
            LOCAL_MEMORY_BASE + AUX_MEMORY_SEGMENT_SIZE
        );
        assert!(vm.free_heap(ptr));
    }

    #[test]
    fn native_markup_strip_matches_sub_438070() {
        assert_eq!(
            strip_native_markup_tags("序章<Ruby>本文</Ruby><1>保持"),
            "序章本文<1>保持"
        );
        assert_eq!(strip_native_markup_tags("未閉合<Tag"), "未閉合<Tag");
    }

    #[test]
    fn mediation_program_registration_and_call_preserve_the_native_stack() {
        let mediation = BpProgram {
            script_name: Some("loadbmpdx._bp".into()),
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: std::iter::once((0x10, 0)).collect(),
            warnings: Vec::new(),
        };
        let main = BpProgram {
            script_name: Some("mediation-main".into()),
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
        let pointer = 0x2021_8ac7;
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
            let start = Vm::memory_addr(pointer) as usize;
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
        let local_name = 0x1200_3000;
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
        let descriptor = 0x1200_3000;
        let resource_name = 0x1200_3100;
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
        let primary = 0x1200_3000;
        let annotation = 0x1200_3100;
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
        let text_pointer = 0x1200_3000;
        vm.write_c_string(text_pointer, "formatted text").unwrap();
        vm.stack.extend((0..11).map(Value::Int));
        let text_index = vm.stack.len() - 1 - 9;
        vm.stack[text_index] = Value::Ptr(text_pointer);

        vm.normalize_graph_string_args(0x92, 0x91).unwrap();

        assert_eq!(vm.stack[text_index], Value::Str("formatted text".into()));
    }

    #[test]
    fn graph92_render_text_normalizes_both_native_text_arguments() {
        let mut vm = Vm::new();
        let primary = 0x1200_3000;
        let auxiliary = 0x1200_3100;
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
            .insert(Vm::value_key(source), Value::Str("シナリオの台詞".into()));
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
    fn system81_utf8_copy_materializes_shadowed_scenario_strings() {
        let source = 0x3000;
        let destination = 0x3100;
        let mut vm = Vm::new();
        vm.mem_values
            .insert(Vm::value_key(source), Value::Str("シナリオの台詞".into()));
        vm.stack
            .extend([Value::Ptr(destination), Value::Ptr(source)]);

        assert_eq!(
            vm.try_builtin_sys_with_api(&mut TraceApi, 0x81, 0x21)
                .unwrap(),
            Some(Value::None)
        );
        assert!(vm.stack.is_empty());
        assert_eq!(vm.read_c_string(destination).unwrap(), "シナリオの台詞");
        assert_eq!(
            vm.mem_values.get(&Vm::value_key(destination)),
            Some(&Value::Str("シナリオの台詞".into()))
        );
    }

    #[test]
    fn native_system_event_queue_preserves_two_argument_abi() {
        let destination = 0x2000u32;
        let program = BpProgram {
            script_name: Some("system-event-test".into()),
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
                test_instruction(0x10, 0x80, "sys1", vec![0x80, 0x6a], Vec::new()),
                test_instruction(
                    0x12,
                    0x02,
                    "push_dword",
                    vec![0x02],
                    vec![BpOperand::U32(0x1234)],
                ),
                test_instruction(0x17, 0x17, "ret", vec![0x17], Vec::new()),
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![
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
            ],
            labels: Default::default(),
            warnings: Vec::new(),
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
        assert_eq!(vm.stack, [Value::Int(42)]);
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
            Value::Int(0),
            [Value::Int(256), Value::Int(0), Value::Int(0)],
            false,
        ));

        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert!(vm.thread.current_procedure().is_none());
        assert_eq!(api.message_reveals, 1);
        assert_eq!(api.message_finishes, 1);
        assert_eq!(vm.stack, [Value::Int(77)]);
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
                Value::Int(0),
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
            Value::Int(0),
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
            Value::Int(0),
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

        api.window_messages.push_back((12, 0x0201, 0x1234, 1));
        assert_eq!(vm.poll_current_procedure(&mut api, false), None);
        assert_eq!(vm.stack, [Value::Int(0x1234), Value::Int(1)]);
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
            functions: Vec::new(),
            strings: Vec::new(),
            instructions: vec![test_instruction(
                0x10,
                0x80,
                "sys1",
                vec![0x80, 0x5C],
                Vec::new(),
            )],
            labels: Default::default(),
            warnings: Vec::new(),
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
            Value::Int(0),
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
            Value::Int(0),
            [Value::Int(1), Value::Int(0), Value::Int(0)],
            false,
        ));
        assert!(!vm.poll_wait_timing_procedure(&mut TraceApi, false));
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn native_program_callback_fails_without_an_installed_procedure() {
        let mut vm = Vm::new();
        assert!(!vm.post_async_program_callback(
            Value::Int(0),
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
        let descriptors = (MAX_MEMORY_SIZE - 8) as u32;
        vm.write_int(descriptors, 2, 11).unwrap();
        vm.write_int(descriptors + 4, 2, 13).unwrap();
        vm.stack.push(Value::Ptr(descriptors));

        let error = vm
            .try_builtin_sys_with_api(&mut SchedulingApi::default(), 0x80, 0x12)
            .unwrap_err();
        assert!(
            error
                .to_string()
                .contains("input descriptor array is not zero terminated")
        );
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
                .contains("fullscreen hotkey descriptor list exceeds 15 entries")
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
        root.labels.insert(expected_base, 0);
        vm.start(&root);
        let worker = empty_loaded_program("worker._bp".into());

        assert_eq!(vm.append_target_loaded_program(worker), expected_base);
        assert_eq!(
            vm.resolve_indirect_call_target(0, expected_base),
            (1, Vm::program_parser_code_start(&vm.programs[1]))
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

        let mut null_archive_api = ArchiveBytesApi {
            expected_archive: String::new(),
            expected_file: "marker.dat".into(),
            bytes: b"marker".to_vec(),
            calls: Vec::new(),
        };
        vm.stack
            .extend([Value::Int(0), Value::Str("marker.dat".into())]);
        vm.dispatch(&exists_instruction, &mut null_archive_api)
            .unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        assert_eq!(
            null_archive_api.calls.first(),
            Some(&(String::new(), "marker.dat".to_string()))
        );
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
    fn sys80_30_clears_target_header_when_resource_is_missing() {
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

        vm.dispatch(&instruction, &mut api).unwrap();

        assert_eq!(vm.stack, [Value::Int(0)]);
        assert_eq!(
            &vm.memory[destination as usize..destination as usize + 16],
            &[0; 16]
        );
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
        let mut api = FileBytesApi {
            bytes: b"abcdefgh".to_vec(),
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
    fn sys80_32_writes_caller_bytes_and_returns_host_status() {
        let instruction =
            test_instruction(0, 0x80, "sys1", vec![0x80, 0x32], vec![BpOperand::U8(0x32)]);
        let mut vm = Vm::new();
        let mut api = WriteFileApi::default();
        let path = 0x1200_1200;
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
            assert!(matches!(
                vm.dispatch(&instruction, &mut api),
                Err(super::VmError::MemoryOutOfBounds { .. })
            ));
            assert!(vm.stack.is_empty());
        }
        assert!(api.writes.is_empty());
    }

    #[derive(Default)]
    struct WriteFileApi {
        success: bool,
        writes: Vec<(String, Vec<u8>)>,
    }

    impl SysApi for WriteFileApi {
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
        let destination = 0x2020_0000u32;
        let payload = b"SDC FORMAT 1.00\0payload".to_vec();
        let mut vm = Vm::new();
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
        let destination = 0x2020_0058u32;
        let payload = b"SDC FORMAT 1.00\0named-entry-payload".to_vec();
        let mut vm = Vm::new();
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
            Value::Str("main".into()),
            Value::Ptr(0x1000),
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
            Value::Str("main".into()),
            Value::Ptr(0x1000),
            Value::Int(11),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 0);

        vm.stack
            .extend([Value::Str("main".into()), Value::Int(15), Value::Int(1)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x89).unwrap(), Some(Value::Int(0)));
        vm.stack.extend([
            Value::Str("main".into()),
            Value::Ptr(0x1000),
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
            Value::Str("main".into()),
            Value::Ptr(0x1000),
            Value::Int(15),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 1);

        vm.stack.extend([Value::Str("main".into()), Value::Int(12)]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x88).unwrap(), Some(Value::Int(1)));
        vm.stack.extend([
            Value::Str("main".into()),
            Value::Ptr(0x1000),
            Value::Int(15),
        ]);
        assert_eq!(vm.try_builtin_sys(0x80, 0x8b).unwrap(), Some(Value::Int(2)));
    }

    #[test]
    fn system_ext_display_calls_preserve_native_outputs_and_status_codes() {
        let mut vm = Vm::new();
        vm.stack.push(Value::Ptr(0x1000));
        assert_eq!(vm.try_builtin_sys(0x81, 0x0e).unwrap(), Some(Value::None));
        assert_eq!(vm.read_int(0x1000, 2).unwrap(), 1280);
        assert_eq!(vm.read_int(0x1004, 2).unwrap(), 720);

        vm.stack
            .extend([Value::Int(3), Value::Int(1280), Value::Int(720)]);
        assert_eq!(vm.try_builtin_sys(0x81, 0x60).unwrap(), Some(Value::Int(0)));
        assert_eq!(vm.display_mode_slots[3], Some((1280, 720)));

        vm.stack.push(Value::Int(2));
        assert_eq!(vm.try_builtin_sys(0x81, 0x62).unwrap(), Some(Value::Int(0)));
        vm.stack.push(Value::Int(1));
        assert_eq!(vm.try_builtin_sys(0x81, 0x62).unwrap(), Some(Value::Int(1)));

        vm.stack.push(Value::Int(2));
        assert_eq!(vm.try_builtin_sys(0x81, 0x63).unwrap(), Some(Value::Int(1)));
        assert_eq!(vm.config_input_mode, 2);

        vm.stack.push(Value::Int(1));
        assert_eq!(vm.try_builtin_sys(0x81, 0x6f).unwrap(), Some(Value::Int(1)));
        assert!(vm.shader_effect_enabled);
    }
    #[test]
    fn system81_wide_distance_preserves_target_empty_operand_behavior() {
        assert_eq!(super::wide_string_similarity(&[1, 2], &[]), 0);
        assert_eq!(super::wide_string_similarity(&[], &[1, 2]), 2);
        assert_eq!(super::wide_string_similarity(&[1, 2], &[1, 3]), 3);
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
