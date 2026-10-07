use crate::{
    NativeOpcode, Value,
    native_call::{NativeMessageProcedureClass, NativeMessageProcedureConfig},
};
use std::collections::VecDeque;

/// Exact 32-bit field image of the recovered target `CThread` object.
///
/// The target executable is 32-bit x86, therefore every pointer slot is a
/// `u32`.  The portable runtime embeds this exact layout as `CThread::native`
/// and stores Rust-owned allocations separately in `CThread::host`.  Pointer
/// slots in the portable runtime are audit handles, not host pointers; their
/// zero/non-zero and identity relationships mirror the native field without
/// pretending that a 64-bit Rust address is a target address.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CThreadLayout32 {
    pub vftable: u32,               // +0x00
    pub root_thread: u32,           // +0x04
    pub thread_id: i32,             // +0x08
    pub next_thread: u32,           // +0x0c
    pub operand_capacity: u32,      // +0x10
    pub operand_allocator: u32,     // +0x14, allocator role not fully closed
    pub operand_ring: u32,          // +0x18
    pub code_region_size: u32,      // +0x1c
    pub code_region_base: u32,      // +0x20
    pub code_region_free_top: u32,  // +0x24
    pub code_allocator: u32,        // +0x28
    pub code_memory: u32,           // +0x2c
    pub loaded_modules: u32,        // +0x30
    pub loaded_module_count: u32,   // +0x34
    pub code_used_end: u32,         // +0x38
    pub frame_region_size: u32,     // +0x3c
    pub frame_region_base: u32,     // +0x40
    pub frame_region_free_top: u32, // +0x44
    pub frame_allocator: u32,       // +0x48
    pub frame_memory: u32,          // +0x4c
    pub offset_heap: u32,           // +0x50
    pub return_stack: u32,          // +0x54
    pub current_procedure: u32,     // +0x58
    pub message_value: u32,         // +0x5c, sentinel value
    pub message_next: u32,          // +0x60, sentinel next/head
    pub retain_on_termination: i32, // +0x64
    pub code_allocations: u32,      // +0x68
    pub frame_allocations: u32,     // +0x6c
    pub status_flags: i32,          // +0x70
    pub operand_index: u32,         // +0x74
    pub current_opcode_ip: u32,     // +0x78
    pub instruction_ip: u32,        // +0x7c
    pub frame_base: u32,            // +0x80
    pub deadline_tick: u32,         // +0x84
}

/// Exact 0x20-byte base field image shared by the target `CProc*` hierarchy.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CProcedureLayout32 {
    pub vftable: u32,           // +0x00
    pub owner: u32,             // +0x04, owning CThread
    pub deadline_tick: u32,     // +0x08, procedure-local virtual clock deadline
    pub host_notify_dirty: u32, // +0x0c, pending host notification flag
    pub cancelled: u32,         // +0x10, cancellation latch
    pub callback_head: u32,     // +0x14, CProcedureCallbackNode*
    pub callback_tail: u32,     // +0x18, CProcedureCallbackNode*
    pub procedure_id: u32,      // +0x1c
}

/// Exact target size for `CProcWaitTimingEx`: `CProcedure + 0x0c`.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CProcWaitTimingExLayout32 {
    pub base: CProcedureLayout32,
    pub input_enabled: u32,       // +0x20
    pub input_scope: u32,         // +0x24
    pub callback_completion: u32, // +0x28
}

/// Exact target size for `CProcWaitWndMsg`: `CProcedure + 0x04`.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CProcWaitWndMsgLayout32 {
    pub base: CProcedureLayout32,
    pub waiter: u32, // +0x20, WindowMessageWaiter*
}

/// Native callback queue node used by every recovered `CProcedure` subclass.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CProcedureCallbackNodeLayout32 {
    pub code: u32, // +0x00
    pub arg1: u32, // +0x04
    pub arg2: u32, // +0x08
    pub next: u32, // +0x0c
}

/// Global waiter node used by `CProcWaitWndMsg`.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct WindowMessageWaiterLayout32 {
    pub thread: u32,     // +0x00, CThread*
    pub message_id: u32, // +0x04
    pub lparam: u32,     // +0x08
    pub wparam: u32,     // +0x0c
    pub signaled: u32,   // +0x10
    pub next: u32,       // +0x14
}

/// Target display-object control procedure (`0xa8` bytes).
///
/// This class is central to graph animation.  The subclass body remains
/// opaque until constructor stores and virtual update methods are recovered.
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcCtrlDspObjLayout32 {
    pub base: CProcedureLayout32,     // +0x00..+0x1f
    pub unknown_20_to_a7: [u8; 0x88], // +0x20..+0xa7
}

/// Target `CProcCtrlDspObjBC` (`0xd8` bytes).
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcCtrlDspObjBCLayout32 {
    pub base: CProcCtrlDspObjLayout32, // +0x00..+0xa7
    pub unknown_a8_to_d7: [u8; 0x30],  // +0xa8..+0xd7
}

/// Target `CProcCtrlDspObjSp` (`0xb8` bytes).
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcCtrlDspObjSpLayout32 {
    pub base: CProcCtrlDspObjLayout32, // +0x00..+0xa7
    pub unknown_a8_to_b7: [u8; 0x10],  // +0xa8..+0xb7
}

/// Exact target `CProcDspMsg` field image (`0x7c` bytes).
///
/// Member names are derived from constructor stores (`sub_432BC0`) and the
/// target virtual methods at `0x433600`, `0x4336B0`, and `0x434150`. Pointer
/// members remain 32-bit audit handles in the portable runtime.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct CProcDspMsgLayout32 {
    pub base: CProcedureLayout32,     // +0x00..+0x1f
    pub display_object: u32,          // +0x20, CDspObj*
    pub input_event_bits: u32,        // +0x24
    pub text_parser: u32,             // +0x28, parser/current text state
    pub configured_value: i32,        // +0x2c, vtable slot 8 setter
    pub completion_latch: u32,        // +0x30, callback 1/258 sets to 1
    pub ordinary_input_latch: u32,    // +0x34
    pub end_wait_policy: i32,         // +0x38
    pub allow_high_bit_input: u32,    // +0x3c, callback 257 setter
    pub allow_auxiliary_input: u32,   // +0x40
    pub initial_delay_enabled: u32,   // +0x44
    pub initial_delay_deadline: u32,  // +0x48
    pub reveal_steps: i32,            // +0x4c
    pub reveal_step_delay_ms: i32,    // +0x50
    pub settle_steps: i32,            // +0x54
    pub settle_step_delay_ms: i32,    // +0x58
    pub auto_deadline_tick: u32,      // +0x5c, armed after reveal completes
    pub reveal_animation_active: u32, // +0x60
    pub animation_step: i32,          // +0x64
    pub parser_state: u32,            // +0x68
    pub parser_initialized: u32,      // +0x6c
    pub remaining_control_state: u32, // +0x70
    pub force_completion: u32,        // +0x74
    pub input_scope: u32,             // +0x78, packed target scope
}

/// Exact target `CProcDspMsgEx` field image (`0xf8` bytes).
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcDspMsgExLayout32 {
    pub base: CProcDspMsgLayout32, // +0x00..+0x7b
    pub effect_state: [u8; 0x48],  // +0x7c..+0xc3
    pub unknown_c4: u32,           // +0xc4
    pub shadow_enabled: u32,       // +0xc8
    pub shadow_x: i32,             // +0xcc
    pub shadow_y: i32,             // +0xd0
    pub shadow_reserved: i32,      // +0xd4
    pub shadow_alpha: i32,         // +0xd8
    pub effect_argument: i32,      // +0xdc
    pub effect_enabled: u32,       // +0xe0
    pub unknown_e4: u32,           // +0xe4
    pub last_clock: [u32; 2],      // +0xe8, target x86 little-endian u64
    pub scaled_clock: [u32; 2],    // +0xf0, target x86 little-endian u64
}

/// Target `CProcDspMsgExVE` does not add fields beyond `CProcDspMsgEx` in the
/// recovered RTTI/allocation evidence. Keep a distinct type alias so selector
/// and vtable recovery can distinguish the class without inventing storage.
impl Default for CProcDspMsgExLayout32 {
    fn default() -> Self {
        Self {
            base: CProcDspMsgLayout32::default(),
            effect_state: [0; 0x48],
            unknown_c4: 0,
            shadow_enabled: 0,
            shadow_x: 0,
            shadow_y: 0,
            shadow_reserved: 0,
            shadow_alpha: 0,
            effect_argument: 0,
            effect_enabled: 0,
            unknown_e4: 0,
            last_clock: [0; 2],
            scaled_clock: [0; 2],
        }
    }
}

pub type CProcDspMsgExVELayout32 = CProcDspMsgExLayout32;

/// Target `CProcUsingThread` has the same recovered field extent as the
/// `CProcedure` base. Its distinct RTTI class is still important for load and
/// encoder procedures.
pub type CProcUsingThreadLayout32 = CProcedureLayout32;

/// Target data/structure encoding procedures add one DWORD to
/// `CProcUsingThread` (`0x24` bytes total).
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcEncodeDataLayout32 {
    pub base: CProcUsingThreadLayout32, // +0x00..+0x1f
    pub unknown_20: u32,                // +0x20
}

pub type CProcEncodeStructLayout32 = CProcEncodeDataLayout32;

/// Target exclusion procedure (`0x128` bytes).
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcExclusionLayout32 {
    pub base: CProcedureLayout32,       // +0x00..+0x1f
    pub unknown_20_to_127: [u8; 0x108], // +0x20..+0x127
}

/// Target asynchronous load procedure (`0x644` bytes). Field semantics remain
/// opaque; the exact extent is retained for future constructor/vtable mapping.
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CProcLoadLayout32 {
    pub base: CProcUsingThreadLayout32, // +0x00..+0x1f
    pub unknown_20_to_643: [u8; 0x624], // +0x20..+0x643
}

/// Recovered target `CThread+0x70` flag bits.
///
/// `sub_444FA0/444FB0/444FC0` show that this field is a bitset, not an
/// interpreter-status enum. Bit 0 means a `CProcedure` is installed at +0x58;
/// the sign bit marks a thread for scheduler removal. Interpreter return
/// statuses 1..6 are transient values returned by opcode handlers and must
/// never be written into this field.
pub(crate) const CTHREAD_FLAG_PROCEDURE_ACTIVE: i32 = 0x0000_0001;
pub(crate) const CTHREAD_FLAG_TERMINATED: i32 = i32::MIN;

/// Rust-owned state corresponding to native pointer fields.
///
/// This type is deliberately separate from [`CThreadLayout32`]. Adding a
/// convenience field here must never silently create a new native field.
#[derive(Debug, Clone, Default)]
pub(crate) struct CThreadHostState {
    /// Portable procedure currently associated with target slot `+0x58`.
    /// Target CThread helpers identify this slot as the currently installed
    /// cooperative procedure. Callback dispatch may invoke that procedure object, but
    /// the recovered CThread field itself is `current_procedure`.
    pub(crate) current_procedure: Option<InstalledCProcedure>,
    /// FIFO represented by native sentinel/head fields `+0x5c/+0x60`.
    pub(crate) message_queue: VecDeque<Value>,
    /// Portable callback delivery queue. The target callback object itself is
    /// referenced through the procedure/thread object; this queue is host
    /// ownership only and is not claimed as a native `CThread` member.
    pub(crate) procedure_callbacks: VecDeque<[Value; 3]>,
}

/// Portable target-shaped `CThread`.
///
/// `native` contains every recovered native field in offset order. `host`
/// contains only Rust ownership that cannot be stored in the 32-bit field
/// image. Runtime code must update both through the methods below instead of
/// creating parallel scheduler/wait fields elsewhere in `Vm`.
#[derive(Debug, Clone)]
pub(crate) struct CThread {
    pub(crate) native: CThreadLayout32,
    pub(crate) host: CThreadHostState,
}

impl Default for CThread {
    fn default() -> Self {
        let mut native = CThreadLayout32::default();
        // The main program thread is created by sub_48C990 with 4096 operand
        // slots, a 0x80000-byte code region and a 0x40000-byte data region.
        native.operand_capacity = 4096;
        native.code_region_size = 0x80000;
        native.frame_region_size = 0x40000;
        native.status_flags = 0;
        Self {
            native,
            host: CThreadHostState::default(),
        }
    }
}

impl CThread {
    pub(crate) fn reset_execution(&mut self) {
        self.clear_current_procedure();
        self.clear_messages();
        self.host.procedure_callbacks.clear();
        self.native.status_flags = 0;
        self.native.operand_index = 0;
        self.native.current_opcode_ip = 0;
        self.native.instruction_ip = 0;
        self.native.frame_base = 0;
        self.native.deadline_tick = 0;
        self.native.loaded_module_count = 0;
        self.native.code_used_end = 0;
        self.native.code_region_free_top = 0;
        self.native.frame_region_free_top = 0;
    }

    /// Region sizes a child thread is created with (`sub_4447C0`).
    pub(crate) fn set_region_sizes(&mut self, code_bytes: u32, data_bytes: u32, operand_slots: u32) {
        self.native.code_region_size = code_bytes;
        self.native.frame_region_size = data_bytes;
        self.native.operand_capacity = operand_slots;
    }

    pub(crate) fn code_region_size(&self) -> u32 {
        self.native.code_region_size
    }

    pub(crate) fn data_region_size(&self) -> u32 {
        self.native.frame_region_size
    }

    pub(crate) fn thread_id(&self) -> i32 {
        self.native.thread_id
    }

    pub(crate) fn set_thread_id(&mut self, thread_id: i32) {
        self.native.thread_id = thread_id;
    }

    pub(crate) fn root_thread_id(&self) -> Option<i32> {
        decode_thread_link(self.native.root_thread)
    }

    pub(crate) fn set_root_thread_id(&mut self, thread_id: Option<i32>) {
        self.native.root_thread = encode_thread_link(thread_id);
    }

    pub(crate) fn next_thread_id(&self) -> Option<i32> {
        decode_thread_link(self.native.next_thread)
    }

    pub(crate) fn set_next_thread_id(&mut self, thread_id: Option<i32>) {
        self.native.next_thread = encode_thread_link(thread_id);
    }

    pub(crate) fn status(&self) -> i32 {
        self.native.status_flags
    }

    pub(crate) fn has_status_flag(&self, flag: i32) -> bool {
        (self.native.status_flags & flag) != 0
    }

    pub(crate) fn mark_terminated(&mut self) {
        self.native.status_flags |= CTHREAD_FLAG_TERMINATED;
    }

    pub(crate) fn set_deadline_from_now(&mut self, now_ms: i32, duration_ms: i32) {
        self.native.deadline_tick =
            (now_ms.max(0) as u32).saturating_add(duration_ms.max(0) as u32);
    }

    pub(crate) fn deadline_tick(&self) -> u32 {
        self.native.deadline_tick
    }

    pub(crate) fn deadline_reached(&self, now_ms: i32) -> bool {
        (now_ms.max(0) as u32) >= self.native.deadline_tick
    }

    pub(crate) fn remaining_deadline_ms(&self, now_ms: i32) -> i32 {
        self.native
            .deadline_tick
            .saturating_sub(now_ms.max(0) as u32)
            .min(i32::MAX as u32) as i32
    }

    pub(crate) fn sync_operand_index(&mut self, stack_len: usize) {
        let capacity = self.native.operand_capacity.max(1) as usize;
        self.native.operand_index = (stack_len % capacity) as u32;
        // Non-zero audit handle: the actual values remain in Vm's portable
        // operand storage until the full ring allocator is moved here.
        self.native.operand_ring = if capacity != 0 { 1 } else { 0 };
    }

    pub(crate) fn operand_index(&self) -> u32 {
        self.native.operand_index
    }

    pub(crate) fn set_instruction_ips(&mut self, current_opcode_ip: u32, instruction_ip: u32) {
        self.native.current_opcode_ip = current_opcode_ip;
        self.native.instruction_ip = instruction_ip;
    }

    pub(crate) fn current_opcode_ip(&self) -> u32 {
        self.native.current_opcode_ip
    }

    pub(crate) fn instruction_ip(&self) -> u32 {
        self.native.instruction_ip
    }

    pub(crate) fn frame_base(&self) -> u32 {
        self.native.frame_base
    }

    pub(crate) fn sync_program_region(&mut self, module_count: usize, code_used_end: u32) {
        self.native.loaded_module_count = u32::try_from(module_count).unwrap_or(u32::MAX);
        self.native.code_used_end = code_used_end;
        self.native.code_region_free_top = code_used_end;
        self.native.loaded_modules = if module_count != 0 { 1 } else { 0 };
        self.native.code_memory = if code_used_end != 0 { 1 } else { 0 };
    }

    pub(crate) fn current_procedure(&self) -> Option<InstalledCProcedure> {
        self.host.current_procedure
    }

    pub(crate) fn replace_current_procedure(
        &mut self,
        procedure: InstalledCProcedure,
    ) -> Option<InstalledCProcedure> {
        // Target CThread+0x58 stores the currently installed cooperative
        // procedure. Callback dispatch may invoke that object; no separate
        // native callback field is inferred here.
        self.native.current_procedure = procedure.audit_handle();
        self.native.status_flags |= CTHREAD_FLAG_PROCEDURE_ACTIVE;
        self.host.current_procedure.replace(procedure)
    }

    pub(crate) fn clear_current_procedure(&mut self) -> Option<InstalledCProcedure> {
        self.native.current_procedure = 0;
        self.native.status_flags &= !CTHREAD_FLAG_PROCEDURE_ACTIVE;
        self.host.current_procedure.take()
    }

    pub(crate) fn message_queue(&self) -> &VecDeque<Value> {
        &self.host.message_queue
    }

    pub(crate) fn message_queue_mut(&mut self) -> &mut VecDeque<Value> {
        &mut self.host.message_queue
    }

    pub(crate) fn sync_message_sentinel(&mut self) {
        // `+0x5c` is the sentinel node's own value, not the first queued
        // message. Keep it neutral and mirror only whether sentinel.next is
        // null. Message values live exclusively in the host-owned node queue.
        self.native.message_value = 0;
        self.native.message_next = if self.host.message_queue.is_empty() {
            0
        } else {
            1
        };
    }

    pub(crate) fn clear_messages(&mut self) {
        self.host.message_queue.clear();
        self.sync_message_sentinel();
    }

    pub(crate) fn push_message(&mut self, value: Value) {
        self.host.message_queue.push_back(value);
        self.sync_message_sentinel();
    }

    pub(crate) fn extend_messages<I>(&mut self, values: I)
    where
        I: IntoIterator<Item = Value>,
    {
        self.host.message_queue.extend(values);
        self.sync_message_sentinel();
    }

    pub(crate) fn pop_message(&mut self) -> Option<Value> {
        let value = self.host.message_queue.pop_front();
        self.sync_message_sentinel();
        value
    }

    pub(crate) fn push_procedure_callback(&mut self, callback: [Value; 3]) {
        self.host.procedure_callbacks.push_back(callback);
    }

    pub(crate) fn pop_procedure_callback(&mut self) -> Option<[Value; 3]> {
        self.host.procedure_callbacks.pop_front()
    }

    pub(crate) fn take_procedure_callbacks(&mut self) -> VecDeque<[Value; 3]> {
        std::mem::take(&mut self.host.procedure_callbacks)
    }

    pub(crate) fn procedure_callbacks_is_empty(&self) -> bool {
        self.host.procedure_callbacks.is_empty()
    }
}

fn encode_thread_link(thread_id: Option<i32>) -> u32 {
    thread_id.map(|id| (id as u32).wrapping_add(1)).unwrap_or(0)
}

fn decode_thread_link(raw: u32) -> Option<i32> {
    (raw != 0).then_some(raw.wrapping_sub(1) as i32)
}

/// Semantic wrapper around the exact recovered 0x20-byte base image.
///
/// Recovered base fields retain their exact native offsets. The portable
/// source selector is deliberately stored in [`InstalledCProcedure`]
/// instead of being smuggled into one of those unknown native fields.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcedureBase {
    pub(crate) native: CProcedureLayout32,
}

impl CProcedureBase {
    fn for_owner(owner_thread_id: i32) -> Self {
        let mut native = CProcedureLayout32::default();
        native.owner = encode_thread_link(Some(owner_thread_id));
        Self { native }
    }
}

/// Target `CProcWaitTiming` has no fields beyond `CProcedure`.
/// Sys80:0x5A constructs this class only when the owning thread timer still
/// has positive remaining time.
pub type CProcWaitTimingLayout32 = CProcedureLayout32;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcWaitTiming {
    pub(crate) base: CProcedureBase,
    /// Portable copy of the constructor argument for diagnostics. The native
    /// object stores only the resulting absolute deadline at `+0x08`.
    pub(crate) duration_ms: i32,
}

/// Target `CProcWaitTimingEx` is `CProcedure + 0x0c`.
///
/// The syscall pop order is target-confirmed. Constructor-to-member lvar
/// identity is not, so members keep exact offset names.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcWaitTimingEx {
    /// Exact native object image, including the recovered input and callback latches.
    pub(crate) native: CProcWaitTimingExLayout32,
    /// Portable syscall contract, kept outside the native object until target
    /// constructor stores are directly recovered.
    pub(crate) call: WaitTimingExCall,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct WaitTimingExCall {
    pub(crate) duration_ms: i32,
    pub(crate) input_enabled: bool,
    pub(crate) input_scope: i32,
}

impl CProcWaitTimingEx {
    pub(crate) fn input_enabled(&self) -> bool {
        self.call.input_enabled
    }

    pub(crate) fn input_scope(&self) -> i32 {
        self.call.input_scope
    }
}

/// Portable state for target `CProcWaitWndMsg` (`0x24` bytes).
///
/// The target's `+0x20` member is the waiter-list handle. The portable runtime
/// keeps the requested Win32 message and registration serial outside the exact
/// native image so stale host messages cannot satisfy a newly installed wait.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcWaitWndMsg {
    pub(crate) native: CProcWaitWndMsgLayout32,
    pub(crate) message_id: i32,
    pub(crate) registered_after_serial: i32,
}

/// A target-confirmed procedure installation whose concrete RTTI subclass or
/// virtual completion predicate has not been recovered.
///
/// This object never completes from elapsed host time. The unresolved selector
/// remains visible instead of being converted into a one-frame delay.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcUnrecovered {
    pub(crate) base: CProcedureBase,
    pub(crate) target_class_name: &'static str,
}

/// Portable execution state for the target `CProcLoadSound` class. The native
/// loader is two-stage (resource read, then decoder/registration). The current
/// host sound backend performs both stages synchronously, but still crosses
/// the target procedure boundary and resumes only through the scheduler.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcLoadSound {
    pub(crate) base: CProcedureBase,
    pub(crate) terminal_status: i32,
}

/// A target-confirmed procedure class whose expensive host operation has
/// already completed synchronously. The VM still resumes through the target
/// CThread+0x58 procedure boundary and only then publishes deferred outputs.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcHostCompleted {
    pub(crate) base: CProcedureBase,
    pub(crate) target_class_name: &'static str,
    pub(crate) terminal_status: i32,
    pub(crate) outputs: [i32; 2],
    pub(crate) output_count: u8,
}

/// Recovered behavior families for graph procedures installed by handlers
/// whose ABI descriptor does not carry the procedure bit. The EXE still
/// stores these objects in CThread+0x58 and publishes their values only from
/// the virtual completion method.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum NativeGraphProcedureMode {
    Control,
    Select,
    Shake,
}

impl NativeGraphProcedureMode {
    pub(crate) const fn name(self) -> &'static str {
        match self {
            Self::Control => "control",
            Self::Select => "select",
            Self::Shake => "shake",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcGraph {
    pub(crate) base: CProcedureBase,
    pub(crate) target_class_name: &'static str,
    pub(crate) mode: NativeGraphProcedureMode,
    /// Explicit CDspObj owned by CProcCtrlDspObj. Other graph CProcedure
    /// families do not necessarily have an object-bound completion predicate.
    pub(crate) object_id: Option<i32>,
    /// Portable identity of the exact host animation instance owned by this
    /// CProcedure.  The target stores timing/state in the procedure object
    /// itself, so a later animation on the same CDspObj must not extend this
    /// wait.
    pub(crate) control_id: Option<u64>,
}

/// Portable state for target `CProcExclusion` (0x128 bytes). The section
/// object itself is process-global; the procedure retains only its stable
/// audit identifier and owner thread, matching the target's deferred acquire.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcExclusion {
    pub(crate) base: CProcedureBase,
    pub(crate) section_id: u32,
    pub(crate) owner_thread_id: i32,
}

/// Portable execution state for the target CProcDspMsg hierarchy. The native
/// members recovered from constructor and virtual-method evidence remain in
/// the exact 32-bit field image; only host timing conveniences stay outside.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CProcDspMsg {
    pub(crate) native: CProcDspMsgLayout32,
    pub(crate) config: NativeMessageProcedureConfig,
    pub(crate) initial_deadline_tick: u32,
    /// Target `+0x5c` is armed only after the text animation reaches its end.
    pub(crate) auto_deadline_tick: Option<u32>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum CProcedure {
    WaitTiming(CProcWaitTiming),
    WaitTimingEx(CProcWaitTimingEx),
    WaitWndMsg(CProcWaitWndMsg),
    DspMsg(CProcDspMsg),
    LoadSound(CProcLoadSound),
    HostCompleted(CProcHostCompleted),
    Graph(CProcGraph),
    Exclusion(CProcExclusion),
    Unrecovered(CProcUnrecovered),
}

impl CProcedure {
    pub(crate) fn class_name(self) -> &'static str {
        match self {
            Self::WaitTiming(_) => "CProcWaitTiming",
            Self::WaitTimingEx(_) => "CProcWaitTimingEx",
            Self::WaitWndMsg(_) => "CProcWaitWndMsg",
            Self::DspMsg(procedure) => match procedure.config.class {
                NativeMessageProcedureClass::DspMsg => "CProcDspMsg",
                NativeMessageProcedureClass::DspMsgEx => "CProcDspMsgEx",
                NativeMessageProcedureClass::DspMsgExVE => "CProcDspMsgExVE",
            },
            Self::LoadSound(_) => "CProcLoadSound",
            Self::HostCompleted(procedure) => procedure.target_class_name,
            Self::Graph(procedure) => procedure.target_class_name,
            Self::Exclusion(_) => "CProcExclusion",
            Self::Unrecovered(procedure) => procedure.target_class_name,
        }
    }
}

/// Audit wrapper around the object stored in target `CThread+0x58`.
///
/// `source_opcode` is portable metadata and is deliberately outside the native
/// object fields so it cannot be mistaken for a recovered target member.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct InstalledCProcedure {
    pub(crate) source_opcode: NativeOpcode,
    pub(crate) object: CProcedure,
}

impl InstalledCProcedure {
    pub(crate) fn wait_timing(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        deadline_tick: u32,
        duration_ms: i32,
    ) -> Self {
        let mut base = CProcedureBase::for_owner(owner_thread_id);
        base.native.deadline_tick = deadline_tick;
        Self {
            source_opcode,
            object: CProcedure::WaitTiming(CProcWaitTiming { base, duration_ms }),
        }
    }

    pub(crate) fn wait_window_message(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        message_id: i32,
        registered_after_serial: i32,
    ) -> Self {
        let mut native = CProcWaitWndMsgLayout32::default();
        native.base = CProcedureBase::for_owner(owner_thread_id).native;
        // Non-zero target-shaped audit handle for the registered waiter node.
        native.waiter = 1;
        Self {
            source_opcode,
            object: CProcedure::WaitWndMsg(CProcWaitWndMsg {
                native,
                message_id,
                registered_after_serial,
            }),
        }
    }

    pub(crate) fn wait_timing_ex(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        deadline_tick: u32,
        duration_ms: i32,
        input_enabled: i32,
        input_scope: i32,
    ) -> Self {
        Self {
            source_opcode,
            object: CProcedure::WaitTimingEx(CProcWaitTimingEx {
                native: CProcWaitTimingExLayout32 {
                    base: {
                        let mut base = CProcedureBase::for_owner(owner_thread_id).native;
                        base.deadline_tick = deadline_tick;
                        base
                    },
                    input_enabled: u32::from(input_enabled != 0),
                    input_scope: input_scope as u32,
                    callback_completion: 0,
                },
                call: WaitTimingExCall {
                    duration_ms,
                    input_enabled: input_enabled != 0,
                    input_scope,
                },
            }),
        }
    }

    pub(crate) fn dsp_msg(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        now_tick: u32,
        config: NativeMessageProcedureConfig,
    ) -> Self {
        let initial_deadline_tick = now_tick.saturating_add(config.initial_delay_ms.max(0) as u32);
        let mut native = CProcDspMsgLayout32::default();
        native.base = CProcedureBase::for_owner(owner_thread_id).native;
        native.display_object = 1;
        native.completion_latch = config.completion_control as u32;
        native.end_wait_policy = config.end_wait_policy;
        native.allow_high_bit_input = u32::from(config.allow_high_bit_input);
        native.allow_auxiliary_input = u32::from(config.allow_auxiliary_input);
        native.initial_delay_enabled = u32::from(config.initial_delay_enabled);
        native.initial_delay_deadline = initial_deadline_tick;
        native.reveal_steps = config.reveal_steps;
        native.reveal_step_delay_ms = config.reveal_step_delay_ms;
        native.settle_steps = config.settle_steps;
        native.settle_step_delay_ms = config.settle_step_delay_ms;
        native.input_scope = config.input_scope as u32;
        Self {
            source_opcode,
            object: CProcedure::DspMsg(CProcDspMsg {
                native,
                config,
                initial_deadline_tick,
                auto_deadline_tick: None,
            }),
        }
    }

    pub(crate) fn load_sound(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        terminal_status: i32,
    ) -> Self {
        Self {
            source_opcode,
            object: CProcedure::LoadSound(CProcLoadSound {
                base: CProcedureBase::for_owner(owner_thread_id),
                terminal_status,
            }),
        }
    }

    pub(crate) fn host_completed(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        target_class_name: &'static str,
        terminal_status: i32,
        outputs: [i32; 2],
        output_count: u8,
    ) -> Self {
        debug_assert!(output_count <= 2);
        Self {
            source_opcode,
            object: CProcedure::HostCompleted(CProcHostCompleted {
                base: CProcedureBase::for_owner(owner_thread_id),
                target_class_name,
                terminal_status,
                outputs,
                output_count: output_count.min(2),
            }),
        }
    }

    pub(crate) fn graph(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        target_class_name: &'static str,
        mode: NativeGraphProcedureMode,
        object_id: Option<i32>,
        control_id: Option<u64>,
    ) -> Self {
        Self {
            source_opcode,
            object: CProcedure::Graph(CProcGraph {
                base: CProcedureBase::for_owner(owner_thread_id),
                target_class_name,
                mode,
                object_id,
                control_id,
            }),
        }
    }

    pub(crate) fn exclusion(
        owner_thread_id: i32,
        source_opcode: NativeOpcode,
        section_id: u32,
    ) -> Self {
        Self {
            source_opcode,
            object: CProcedure::Exclusion(CProcExclusion {
                base: CProcedureBase::for_owner(owner_thread_id),
                section_id,
                owner_thread_id,
            }),
        }
    }

    pub(crate) fn unrecovered(owner_thread_id: i32, source_opcode: NativeOpcode) -> Self {
        let target_class_name = crate::procedure_class_map::target_procedure_class(source_opcode)
            .map(|evidence| evidence.class_name)
            .unwrap_or("CProcedure(unrecovered target subclass)");
        Self {
            source_opcode,
            object: CProcedure::Unrecovered(CProcUnrecovered {
                base: CProcedureBase::for_owner(owner_thread_id),
                target_class_name,
            }),
        }
    }

    fn audit_handle(self) -> u32 {
        let encoded = ((self.source_opcode.group as u32) << 16) | self.source_opcode.id as u32;
        encoded.wrapping_add(1).max(1)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::mem::size_of;

    #[test]
    fn recovered_target_layout_sizes_are_stable() {
        assert_eq!(size_of::<CThreadLayout32>(), 0x88);
        assert_eq!(size_of::<CProcedureLayout32>(), 0x20);
        assert_eq!(size_of::<CProcWaitTimingExLayout32>(), 0x2c);
        assert_eq!(size_of::<CProcWaitWndMsgLayout32>(), 0x24);
        assert_eq!(size_of::<CProcedureCallbackNodeLayout32>(), 0x10);
        assert_eq!(size_of::<WindowMessageWaiterLayout32>(), 0x18);
        assert_eq!(size_of::<CProcCtrlDspObjLayout32>(), 0xa8);
        assert_eq!(size_of::<CProcCtrlDspObjBCLayout32>(), 0xd8);
        assert_eq!(size_of::<CProcCtrlDspObjSpLayout32>(), 0xb8);
        assert_eq!(size_of::<CProcDspMsgLayout32>(), 0x7c);
        assert_eq!(size_of::<CProcDspMsgExLayout32>(), 0xf8);
        assert_eq!(size_of::<CProcDspMsgExVELayout32>(), 0xf8);
        assert_eq!(size_of::<CProcUsingThreadLayout32>(), 0x20);
        assert_eq!(size_of::<CProcEncodeDataLayout32>(), 0x24);
        assert_eq!(size_of::<CProcEncodeStructLayout32>(), 0x24);
        assert_eq!(size_of::<CProcExclusionLayout32>(), 0x128);
        assert_eq!(size_of::<CProcLoadLayout32>(), 0x644);
    }

    #[test]
    fn target_offsets_match_recovered_layout() {
        assert_eq!(
            std::mem::offset_of!(CThreadLayout32, current_procedure),
            0x58
        );
        assert_eq!(std::mem::offset_of!(CThreadLayout32, message_value), 0x5c);
        assert_eq!(std::mem::offset_of!(CThreadLayout32, status_flags), 0x70);
        assert_eq!(std::mem::offset_of!(CThreadLayout32, operand_index), 0x74);
        assert_eq!(
            std::mem::offset_of!(CThreadLayout32, current_opcode_ip),
            0x78
        );
        assert_eq!(std::mem::offset_of!(CThreadLayout32, instruction_ip), 0x7c);
        assert_eq!(std::mem::offset_of!(CThreadLayout32, frame_base), 0x80);
        assert_eq!(std::mem::offset_of!(CThreadLayout32, deadline_tick), 0x84);
    }

    #[test]
    fn portable_host_state_keeps_native_slots_synchronized() {
        let mut thread = CThread::default();
        thread.set_thread_id(7);
        let opcode = NativeOpcode {
            group: 0x90,
            id: 0x10,
        };
        thread.replace_current_procedure(InstalledCProcedure::unrecovered(7, opcode));
        assert_ne!(thread.native.current_procedure, 0);
        assert!(thread.has_status_flag(CTHREAD_FLAG_PROCEDURE_ACTIVE));
        assert_eq!(thread.current_procedure().unwrap().source_opcode, opcode);

        thread.push_message(Value::Int(42));
        assert_eq!(thread.native.message_value, 0);
        assert_ne!(thread.native.message_next, 0);
        assert_eq!(thread.pop_message(), Some(Value::Int(42)));
        assert_eq!(thread.native.message_next, 0);

        thread.clear_current_procedure();
        assert_eq!(thread.native.current_procedure, 0);
        assert!(!thread.has_status_flag(CTHREAD_FLAG_PROCEDURE_ACTIVE));
        thread.mark_terminated();
        assert!(thread.has_status_flag(CTHREAD_FLAG_TERMINATED));
    }

    #[test]
    fn every_abi_procedure_has_an_explicit_target_class() {
        for group in [0x80, 0x81, 0x90, 0x91, 0x92, 0xA0, 0xB0, 0xC0] {
            for id in 0..=0xFF {
                if ethornell_script::native_abi::installs_procedure(group, id) {
                    let opcode = NativeOpcode { group, id };
                    let evidence = crate::procedure_class_map::target_procedure_class(opcode)
                        .unwrap_or_else(|| panic!("missing class for 0x{group:02X}:0x{id:02X}"));
                    assert_ne!(
                        evidence.class_name,
                        "CProcedure(unrecovered target subclass)"
                    );
                    assert_eq!(evidence.path_kind, "abi_procedure");
                    assert!(!evidence.confidence.is_empty());
                }
            }
        }
    }
}
