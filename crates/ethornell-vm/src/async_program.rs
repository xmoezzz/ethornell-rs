use crate::MAIN_THREAD_ID;
use crate::local_window::LocalWindow;
use crate::{
    ADDRESS_MASK, GraphApi, HEAP_MEMORY_BASE, LOCAL_MEMORY_BASE, SoundApi, SysApi, Value, Vm,
    VmRunOptions, VmStopReason,
};
use ethornell_script::BpProgram;
use std::sync::Arc;

#[derive(Debug)]
pub(crate) struct AsyncProgramTask {
    key: String,
    pub(crate) vm: Box<Vm>,
    pub(crate) runnable: bool,
    pub(crate) completed: bool,
}

impl AsyncProgramTask {
    pub(crate) fn key(&self) -> &str {
        &self.key
    }
}

impl Vm {
    /// Apply the region sizes `0x80:0x44` passed to the child thread that
    /// `start_async_program_with_args` just created.
    pub(crate) fn set_newest_async_thread_regions(&mut self, code: u32, data: u32, slots: u32) {
        if let Some(task) = self.async_tasks.last_mut() {
            task.vm.thread.set_region_sizes(code, data, slots);
        }
    }

    pub(crate) fn async_program_is_active(&self, program: Value) -> i32 {
        if matches!(program, Value::Int(_) | Value::Ptr(_)) {
            let thread_id = program.as_i32();
            if thread_id == MAIN_THREAD_ID || self.native_thread_exists(thread_id) {
                return 1;
            }
            return 0;
        }
        let Some(key) = self.async_program_key(&program) else {
            return 0;
        };
        self.async_tasks
            .iter()
            .find(|task| task.key == key)
            .map_or(0, |task| i32::from(!task.completed))
    }

    pub(crate) fn start_async_program_with_args(
        &mut self,
        program: Value,
        args: Vec<Value>,
        trace_events: bool,
    ) -> Option<i32> {
        self.spawn_async_program(program, args, None, trace_events)
    }

    /// A thread whose id was taken already (Sys81:44 numbers the child
    /// before it reserves its blocks).
    pub(crate) fn start_async_program_with_thread_id(
        &mut self,
        program: Value,
        thread_id: i32,
        trace_events: bool,
    ) -> bool {
        self.spawn_async_program(program, Vec::new(), Some(thread_id), trace_events)
            .is_some()
    }

    fn spawn_async_program(
        &mut self,
        program: Value,
        args: Vec<Value>,
        thread_id: Option<i32>,
        trace_events: bool,
    ) -> Option<i32> {
        let Some(program) = self.value_program(program) else {
            return None;
        };
        let key = program_key(&program);

        // The new thread gets the globals and heap when it first runs (see
        // `hand_over_memory`); until then it owns only its local region.
        // Global user data follows the active process buffer during handover;
        // an idle thread must not keep another 1 MiB snapshot of it.
        let mut vm = Vm::with_buffers(Vec::new(), Vec::new());
        vm.native_abi_profile = self.native_abi_profile;
        vm.graph_text_encoding = self.graph_text_encoding;
        vm.system80_shared = Arc::clone(&self.system80_shared);
        vm.system81_shared = Arc::clone(&self.system81_shared);
        vm.heap_ptr = self.heap_ptr;
        vm.record_tables = self.record_tables.clone();
        vm.next_record_table_handle = self.next_record_table_handle;
        vm.indexed_record_tables = self.indexed_record_tables.clone();
        vm.next_indexed_record_handle = self.next_indexed_record_handle;
        vm.script_records = self.script_records.clone();
        vm.string_hash_tables = self.string_hash_tables.clone();
        vm.read_flags = self.read_flags.clone();
        vm.save_data_integrity_enabled = self.save_data_integrity_enabled;
        vm.next_binary_or_bmv_async = self.next_binary_or_bmv_async;
        vm.exclusive_thread_id = self.exclusive_thread_id;
        vm.global_size = self.global_size;
        vm.loaded_bcs_ranges = self.loaded_bcs_ranges.clone();
        vm.timing = self.timing.clone();
        vm.rng_seed = self.rng_seed;
        vm.additional_resource_search_enabled = self.additional_resource_search_enabled;
        vm.additional_resource_paths = self.additional_resource_paths.clone();
        vm.composite_archives = self.composite_archives.clone();
        vm.primary_resource_root = self.primary_resource_root.clone();
        vm.secondary_resource_root = self.secondary_resource_root.clone();
        vm.validated_file_root = self.validated_file_root.clone();
        vm.system_wait_state = self.system_wait_state;
        vm.start_execution(&program, false);
        vm.parked_local = Some(LocalWindow::from_code(&program.code));
        let thread_id = thread_id.unwrap_or_else(|| {
            let thread_id = self.next_thread_id.max(1);
            self.next_thread_id = self.next_thread_id.saturating_add(1).max(1);
            thread_id
        });
        vm.next_thread_id = self.next_thread_id;
        vm.thread.set_thread_id(thread_id);
        vm.thread.set_root_thread_id(Some(self.thread.thread_id()));
        vm.mediation_programs = self.mediation_programs.clone();
        if !args.is_empty() {
            vm.thread.extend_messages(args);
        }
        self.async_tasks.push(AsyncProgramTask {
            key: key.clone(),
            vm: Box::new(vm),
            runnable: true,
            completed: false,
        });
        self.refresh_thread_links();
        if trace_events {
            tracing::info!(program = key, thread_id, "VM async program created");
        }
        Some(thread_id)
    }

    pub(crate) fn async_program_is_active_value(&self, program: &Value) -> bool {
        self.async_program_is_active(program.clone()) != 0
    }

    pub(crate) fn native_thread_exists(&self, thread_id: i32) -> bool {
        if thread_id == 0 {
            return false;
        }
        self.thread.thread_id() == thread_id
            || self
                .async_tasks
                .iter()
                .any(|task| !task.completed && task.vm.native_thread_exists(thread_id))
    }

    fn post_native_thread_message(&mut self, thread_id: i32, message: Value) -> bool {
        if self.thread.thread_id() == thread_id {
            self.thread.push_message(message);
            return true;
        }
        for task in &mut self.async_tasks {
            if task.completed {
                continue;
            }
            if task
                .vm
                .post_native_thread_message(thread_id, message.clone())
            {
                task.runnable = true;
                return true;
            }
        }
        false
    }

    fn post_native_thread_callback(
        &mut self,
        thread_id: i32,
        callback: [Value; 3],
        trace_events: bool,
    ) -> bool {
        if self.thread.thread_id() == thread_id {
            return self.enqueue_native_callback(callback, trace_events);
        }
        for task in &mut self.async_tasks {
            if task.completed {
                continue;
            }
            if task
                .vm
                .post_native_thread_callback(thread_id, callback.clone(), trace_events)
            {
                task.runnable = true;
                return true;
            }
        }
        false
    }

    fn activate_native_thread(&mut self, thread_id: i32) -> bool {
        if self.thread.thread_id() == thread_id {
            return true;
        }
        for task in &mut self.async_tasks {
            if task.completed {
                continue;
            }
            if task.vm.activate_native_thread(thread_id) {
                task.runnable = true;
                return true;
            }
        }
        false
    }

    pub(crate) fn post_async_program_message(
        &mut self,
        program: Value,
        message: Value,
        trace_events: bool,
    ) -> bool {
        if matches!(program, Value::Int(_) | Value::Ptr(_)) {
            let thread_id = program.as_i32();
            if thread_id == MAIN_THREAD_ID && self.thread.thread_id() != MAIN_THREAD_ID {
                self.pending_root_program_messages.push_back(message);
                return true;
            }
            return self.post_native_thread_message(thread_id, message);
        }
        let Some(key) = self.async_program_key(&program) else {
            if trace_events {
                tracing::warn!(
                    ?program,
                    ?message,
                    "VM program message target is not a program handle"
                );
            }
            return false;
        };
        let Some(task) = self
            .async_tasks
            .iter_mut()
            .find(|task| task.key == key && !task.completed)
        else {
            if trace_events {
                tracing::warn!(program = key, ?message, "VM program message target missing");
            }
            return false;
        };
        if trace_events {
            tracing::debug!(program = key, ?message, "VM program message posted");
        }
        task.vm.thread.push_message(message);
        true
    }

    pub(crate) fn post_async_program_callback(
        &mut self,
        program: Value,
        callback: [Value; 3],
        trace_events: bool,
    ) -> bool {
        if matches!(program, Value::Int(_) | Value::Ptr(_)) {
            let thread_id = program.as_i32();
            if thread_id == MAIN_THREAD_ID {
                if self.thread.thread_id() == MAIN_THREAD_ID {
                    return self.enqueue_native_callback(callback, trace_events);
                }
                if !self.root_procedure_active {
                    return false;
                }
                self.pending_root_program_callbacks.push_back(callback);
                return true;
            }
            return self.post_native_thread_callback(thread_id, callback, trace_events);
        }
        let Some(key) = self.async_program_key(&program) else {
            if trace_events {
                tracing::warn!(
                    ?program,
                    ?callback,
                    "VM callback target is not a thread handle"
                );
            }
            return false;
        };
        let Some(task) = self
            .async_tasks
            .iter_mut()
            .find(|task| task.key == key && !task.completed)
        else {
            if trace_events {
                tracing::warn!(program = key, ?callback, "VM callback target missing");
            }
            return false;
        };
        task.vm.enqueue_native_callback(callback, trace_events)
    }

    fn enqueue_native_callback(&mut self, callback: [Value; 3], trace_events: bool) -> bool {
        let active = self.thread.current_procedure().is_some();
        if active {
            if trace_events {
                tracing::debug!(?callback, "VM native procedure callback queued");
            }
            self.thread.push_procedure_callback(callback);
        }
        active
    }

    pub(crate) fn switch_to_async_program(
        &mut self,
        program: Value,
        trace_events: bool,
    ) -> (bool, Option<i32>) {
        if matches!(program, Value::Int(_) | Value::Ptr(_)) {
            let thread_id = program.as_i32();
            if thread_id == MAIN_THREAD_ID && self.thread.thread_id() != MAIN_THREAD_ID {
                return (true, Some(MAIN_THREAD_ID));
            }
            let activated = self.activate_native_thread(thread_id);
            return (activated, Some(thread_id));
        }
        let Some(key) = self.async_program_key(&program) else {
            if trace_events {
                tracing::warn!(?program, "VM program switch target is not a program handle");
            }
            return (false, None);
        };
        let Some(task) = self
            .async_tasks
            .iter_mut()
            .find(|task| task.key == key && !task.completed)
        else {
            if trace_events {
                tracing::warn!(program = key, "VM program switch target missing");
            }
            return (false, None);
        };
        task.runnable = true;
        let thread_id = task.vm.thread.thread_id();
        if trace_events || std::env::var_os("TRACE_ASYNC_PROGRAMS").is_some() {
            tracing::info!(program = key, thread_id, "VM async program activated");
        }
        (true, Some(thread_id))
    }

    pub(crate) fn pump_async_programs<A>(
        &mut self,
        api: &mut A,
        trace_events: bool,
        run_options: &VmRunOptions,
    ) -> Option<crate::VmRunReport>
    where
        A: SysApi + GraphApi + SoundApi,
    {
        let trace_async = trace_events || std::env::var_os("TRACE_ASYNC_PROGRAMS").is_some();

        // Native `sub_48CD70` walks one flat CThread list. Status 3 replaces
        // the current iterator with the requested thread immediately; status
        // 0/1/procedure waits continue with the linked-list successor. The
        // portable VMs remain separate execution contexts, but scheduling here
        // follows that same flat-list order instead of pumping every child in a
        // fixed for-loop.
        let requested_start = self.scheduler_switch_target.take();
        let mut cursor =
            match requested_start {
                Some(thread_id)
                    if thread_id == MAIN_THREAD_ID || thread_id == self.thread.thread_id() =>
                {
                    // Status 3 selected the root CThread; `run_loaded` executes it
                    // immediately after returning from this child scheduler.
                    self.refresh_thread_links();
                    return None;
                }
                Some(thread_id) => {
                    let Some(index) = self.async_tasks.iter().position(|task| {
                        !task.completed && task.vm.thread.thread_id() == thread_id
                    }) else {
                        // Native sub_444B90 returns null for an unknown id and the
                        // outer list walk terminates instead of falling back to the
                        // first thread.
                        self.refresh_thread_links();
                        return None;
                    };
                    index
                }
                None => 0,
            };
        let mut scheduler_hops = 0usize;
        const MAX_SCHEDULER_HOPS_PER_PASS: usize = 4096;

        while cursor < self.async_tasks.len() && scheduler_hops < MAX_SCHEDULER_HOPS_PER_PASS {
            if self.async_tasks[cursor].completed || !self.async_tasks[cursor].runnable {
                cursor += 1;
                continue;
            }
            if let Some(exclusive_thread_id) = self.exclusive_thread_id {
                if self.async_tasks[cursor].vm.thread.thread_id() != exclusive_thread_id {
                    cursor += 1;
                    continue;
                }
            }

            let options = VmRunOptions {
                ..run_options.clone()
            };
            let mut task = self.async_tasks.remove(cursor);
            transfer_async_shared_state(self, &mut task.vm);
            let mut others = std::mem::take(&mut self.async_tasks);
            lend_owner_window(self, &mut others, &mut task.vm);
            self.async_tasks = others;
            let mut scheduler_tasks = std::mem::take(&mut self.async_tasks);
            scheduler_tasks.append(&mut task.vm.async_tasks);
            task.vm.async_tasks = scheduler_tasks;
            task.vm.suppress_async_pump_once = true;
            task.vm.root_procedure_active = if self.thread.thread_id() == MAIN_THREAD_ID {
                self.thread.current_procedure().is_some()
            } else {
                self.root_procedure_active
            };
            let report = task.vm.run_loaded_internal(api, &options, false);
            self.pass_yields += task.vm.pass_yields;
            if options.collect_diagnostics || report.stop_reason.is_fatal() {
                self.async_reports.push(report.clone());
            }
            let requested_switch = task.vm.scheduler_switch_target.take();
            if report.stop_reason == VmStopReason::WatchdogExceeded {
                tracing::warn!(
                    program = %task.key,
                    pc = report.pc,
                    offset = ?report.offset,
                    steps = report.steps,
                    "async VM host watchdog ended this scheduler slice"
                );
            }
            let root_messages = std::mem::take(&mut task.vm.pending_root_program_messages);
            let root_callbacks = std::mem::take(&mut task.vm.pending_root_program_callbacks);
            if trace_async {
                let (
                    scenario_pending,
                    motion_active,
                    motion_pending,
                    body_active,
                    body_pending,
                    body_records,
                ) = async_queue_summary(&task.vm);
                let local_base = (task.vm.mem_ptr & ADDRESS_MASK) as usize;
                let body_slot = local_base
                    .checked_sub(0x90)
                    .map(|addr| read_u32(&task.vm.memory, LOCAL_MEMORY_BASE as usize + addr));
                let body_record_ptr = local_base
                    .checked_sub(0x50)
                    .map(|addr| read_u32(&task.vm.memory, LOCAL_MEMORY_BASE as usize + addr));
                tracing::info!(
                    program = task.key,
                    steps = report.steps,
                    pc = report.pc,
                    offset = ?report.offset,
                    reason = ?report.stop_reason,
                    mem_ptr = format_args!("{:#010x}", task.vm.mem_ptr),
                    body_slot,
                    body_record_ptr = ?body_record_ptr.map(|ptr| format!("{ptr:#010x}")),
                    scenario_pending,
                    motion_active,
                    motion_pending,
                    body_active,
                    body_pending,
                    body_records = ?body_records,
                    "VM async program pump"
                );
            }
            if matches!(
                report.stop_reason,
                VmStopReason::Completed | VmStopReason::InterpreterTerminated
            ) {
                task.completed = true;
                task.vm.release_child_thread_regions();
                if report.stop_reason != VmStopReason::InterpreterTerminated {
                    task.vm.thread.mark_terminated();
                }
            }
            let mut scheduler_tasks = std::mem::take(&mut task.vm.async_tasks);
            lend_owner_window(self, &mut scheduler_tasks, &mut task.vm);
            transfer_async_shared_state(&mut task.vm, self);
            self.thread.extend_messages(root_messages);
            for callback in root_callbacks {
                self.enqueue_native_callback(callback, trace_events);
            }
            scheduler_tasks.insert(cursor.min(scheduler_tasks.len()), task);
            self.async_tasks = scheduler_tasks;
            scheduler_hops += 1;
            if report.stop_reason.is_fatal()
                || report.stop_reason == VmStopReason::InterpreterTerminated
            {
                // Keep the failing context and report its actual thread/PC to
                // the root host. It is not a successfully completed coroutine.
                // Sys80:6A (status 6) from any thread ends the whole
                // interpreter: sub_48D860 returns it to WinMain, which exits
                // (endsys._bp quits from a child thread).
                self.refresh_thread_links();
                return Some(report);
            }

            if report.stop_reason == VmStopReason::SwitchedThread {
                let Some(target_thread_id) = requested_switch else {
                    break;
                };
                if target_thread_id == MAIN_THREAD_ID || target_thread_id == self.thread.thread_id()
                {
                    // The requested root CThread is executed by `run_loaded`
                    // immediately after this flat child pass.
                    break;
                }
                if let Some(target_index) = self.async_tasks.iter().position(|candidate| {
                    !candidate.completed && candidate.vm.thread.thread_id() == target_thread_id
                }) {
                    cursor = target_index;
                    continue;
                }
                // Native sub_444B90 returns null for an unknown target, which
                // ends the current list walk.
                break;
            }

            cursor += 1;
        }

        if scheduler_hops == MAX_SCHEDULER_HOPS_PER_PASS {
            tracing::warn!(
                scheduler_hops,
                "portable scheduler safety cap reached while following native thread switches"
            );
        }
        self.async_tasks.retain(|task| !task.completed);
        self.refresh_thread_links();
        None
    }

    fn refresh_thread_links(&mut self) {
        let thread_ids = self
            .async_tasks
            .iter()
            .filter(|task| !task.completed)
            .map(|task| task.vm.thread.thread_id())
            .collect::<Vec<_>>();
        self.thread.set_next_thread_id(thread_ids.first().copied());
        let root_thread_id = self.thread.thread_id();
        for (index, task) in self.async_tasks.iter_mut().enumerate() {
            task.vm.thread.set_root_thread_id(Some(root_thread_id));
            task.vm
                .thread
                .set_next_thread_id(thread_ids.get(index + 1).copied());
        }
    }

    fn async_program_key(&self, value: &Value) -> Option<String> {
        if matches!(value, Value::Int(_) | Value::Ptr(_)) {
            let thread_id = value.as_i32();
            if let Some(task) = self
                .async_tasks
                .iter()
                .find(|task| task.vm.thread.thread_id() == thread_id)
            {
                return Some(task.key.clone());
            }
        }
        self.value_program(value.clone())
            .map(|program| program_key(&program))
    }

    pub(crate) fn value_program(&self, value: Value) -> Option<BpProgram> {
        match value {
            Value::Program(program) => Some((*program).clone()),
            Value::Ptr(ptr) => self
                .mem_values
                .get(&self.value_key(ptr))
                .and_then(|value| {
                    if let Value::Program(program) = value {
                        Some((**program).clone())
                    } else {
                        None
                    }
                }),
            Value::Int(ptr) => {
                self.mem_values
                    .get(&self.value_key(ptr as u32))
                    .and_then(|value| {
                        if let Value::Program(program) = value {
                            Some((**program).clone())
                        } else {
                            None
                        }
                    })
            }
            Value::Str(_) | Value::Func { .. } | Value::None => None,
        }
    }
}

/// A DCTChildThread runs in its owner's local window: swapped in after the
/// shared state reaches it and back out before it leaves (the swap is its
/// own inverse). The owner is the scheduling VM or another idle task.
fn lend_owner_window(scheduler: &mut Vm, tasks: &mut [AsyncProgramTask], child: &mut Vm) {
    let Some(owner) = child.local_window_owner else {
        return;
    };
    if scheduler.thread.thread_id() == owner {
        crate::child_thread::lend_local_window(scheduler, child);
    } else if let Some(task) = tasks
        .iter_mut()
        .find(|task| task.vm.thread.thread_id() == owner)
    {
        crate::child_thread::lend_local_window(&mut task.vm, child);
    }
}

fn program_key(program: &BpProgram) -> String {
    program
        .script_name
        .as_deref()
        .unwrap_or("<anonymous>")
        .to_string()
}

/// Hands the process memory -- globals and the shared heap, as one buffer --
/// to the thread about to run, without copying it. Only threads' local
/// regions (`LOCAL_MEMORY_BASE..HEAP_MEMORY_BASE`, and their shadow values)
/// stay with each thread. The idle side keeps a buffer whose globals and heap
/// are stale; it gets the live one back before it runs again. Sharing the
/// heap by copying it instead cost every idle thread a full heap.
///
/// The idle side keeps only its local region (`parked_local`), not a buffer
/// of stale globals: one process buffer exists, plus one local region per
/// thread.
fn hand_over_memory(source: &mut Vm, destination: &mut Vm) {
    let local = LOCAL_MEMORY_BASE as usize..HEAP_MEMORY_BASE;
    let mut live = std::mem::take(&mut source.memory);
    if live.len() < HEAP_MEMORY_BASE {
        live.resize(HEAP_MEMORY_BASE, 0);
    }
    let mut scratch = std::mem::take(&mut source.local_window_scratch);
    let mut window = take_local_window(destination);
    let bounds = live.local_bounds();
    let incoming_bounds = window.prefix_lengths();
    window.exchange_bounded(&mut live[local], &mut scratch, bounds);
    live.set_local_bounds(incoming_bounds);
    source.parked_local = Some(window);
    destination.memory = live;
    destination.local_window_scratch = scratch;

    let local_keys = LOCAL_MEMORY_BASE..HEAP_MEMORY_BASE as u32;
    let source_locals = source
        .mem_values
        .extract_if(local_keys.clone(), |_, _| true)
        .collect::<Vec<_>>();
    let destination_locals = destination
        .mem_values
        .extract_if(local_keys, |_, _| true)
        .collect::<Vec<_>>();
    std::mem::swap(&mut source.mem_values, &mut destination.mem_values);
    destination.mem_values.extend(destination_locals);
    source.mem_values.clear();
    source.mem_values.extend(source_locals);
}

/// A thread's own local region, out of its parked copy or its buffer (a new
/// thread starts with a buffer of its own). The buffer is released.
pub(crate) fn take_local_window(vm: &mut Vm) -> LocalWindow {
    let local = LOCAL_MEMORY_BASE as usize..HEAP_MEMORY_BASE;
    if let Some(window) = vm.parked_local.take() {
        vm.memory = Vec::new().into();
        return window;
    }
    let mut buffer = std::mem::take(&mut vm.memory).into_vec();
    if buffer.is_empty() {
        return LocalWindow::default();
    }
    if buffer.len() < HEAP_MEMORY_BASE {
        buffer.resize(HEAP_MEMORY_BASE, 0);
    }
    buffer.truncate(HEAP_MEMORY_BASE);
    buffer.drain(..LOCAL_MEMORY_BASE as usize);
    debug_assert_eq!(buffer.len(), local.len());
    LocalWindow::from_dense(buffer)
}

/// A byte of a thread's local region, parked or not (tests).
#[cfg(test)]
pub(crate) fn local_byte(vm: &Vm, addr: usize) -> u8 {
    match vm.parked_local.as_ref() {
        Some(window) => window.byte(addr - LOCAL_MEMORY_BASE as usize),
        None => vm.memory[addr],
    }
}

/// Exchange a parked local window with the active process buffer, including
/// the write bounds. Child threads borrow their owner's window this way so a
/// compact snapshot need not expand to 2 MiB merely to lend it for one pass.
pub(crate) fn exchange_saved_local(vm: &mut Vm, window: &mut LocalWindow) {
    if vm.memory.len() < HEAP_MEMORY_BASE {
        vm.memory.resize(HEAP_MEMORY_BASE, 0);
    }
    let bounds = vm.memory.local_bounds();
    let incoming_bounds = window.prefix_lengths();
    window.exchange_bounded(
        &mut vm.memory[LOCAL_MEMORY_BASE as usize..HEAP_MEMORY_BASE],
        &mut vm.local_window_scratch, bounds,
    );
    vm.memory.set_local_bounds(incoming_bounds);
}

/// A thread's local region wherever it is: parked, or in its buffer.
pub(crate) fn local_window_mut(vm: &mut Vm) -> &mut [u8] {
    if vm.parked_local.is_none() && vm.memory.len() < HEAP_MEMORY_BASE {
        vm.memory.resize(HEAP_MEMORY_BASE, 0);
    }
    match vm.parked_local.as_mut() {
        Some(window) => window.as_mut_slice(),
        None => &mut vm.memory[LOCAL_MEMORY_BASE as usize..HEAP_MEMORY_BASE],
    }
}

fn transfer_async_shared_state(source: &mut Vm, destination: &mut Vm) {
    let trace_records = std::env::var_os("TRACE_ASYNC_RECORDS").is_some();
    if trace_records {
        tracing::info!(
            source_thread = source.thread.thread_id(),
            destination_thread = destination.thread.thread_id(),
            source_tables = source.indexed_record_tables.len(),
            source_records = indexed_record_count(source),
            destination_tables = destination.indexed_record_tables.len(),
            destination_records = indexed_record_count(destination),
            "VM indexed-record state transfer begin"
        );
    }
    hand_over_memory(source, destination);

    std::mem::swap(&mut source.heap_ptr, &mut destination.heap_ptr);
    std::mem::swap(
        &mut source.heap_allocations,
        &mut destination.heap_allocations,
    );
    std::mem::swap(
        &mut source.heap_free_blocks,
        &mut destination.heap_free_blocks,
    );
    std::mem::swap(&mut source.target_heap, &mut destination.target_heap);
    std::mem::swap(&mut source.record_tables, &mut destination.record_tables);
    std::mem::swap(
        &mut source.next_record_table_handle,
        &mut destination.next_record_table_handle,
    );
    std::mem::swap(
        &mut source.indexed_record_tables,
        &mut destination.indexed_record_tables,
    );
    std::mem::swap(
        &mut source.next_indexed_record_handle,
        &mut destination.next_indexed_record_handle,
    );
    std::mem::swap(&mut source.script_records, &mut destination.script_records);
    std::mem::swap(
        &mut source.string_hash_tables,
        &mut destination.string_hash_tables,
    );
    std::mem::swap(
        &mut source.mediation_programs,
        &mut destination.mediation_programs,
    );
    std::mem::swap(&mut source.read_flags, &mut destination.read_flags);
    std::mem::swap(
        &mut source.save_data_integrity_enabled,
        &mut destination.save_data_integrity_enabled,
    );
    std::mem::swap(
        &mut source.next_binary_or_bmv_async,
        &mut destination.next_binary_or_bmv_async,
    );
    std::mem::swap(
        &mut source.exclusive_thread_id,
        &mut destination.exclusive_thread_id,
    );
    std::mem::swap(&mut source.global_size, &mut destination.global_size);
    std::mem::swap(
        &mut source.global_user_data,
        &mut destination.global_user_data,
    );
    std::mem::swap(
        &mut source.loaded_bcs_ranges,
        &mut destination.loaded_bcs_ranges,
    );
    std::mem::swap(&mut source.timing, &mut destination.timing);
    std::mem::swap(&mut source.rng_seed, &mut destination.rng_seed);
    std::mem::swap(
        &mut source.additional_resource_search_enabled,
        &mut destination.additional_resource_search_enabled,
    );
    std::mem::swap(
        &mut source.additional_resource_paths,
        &mut destination.additional_resource_paths,
    );
    std::mem::swap(
        &mut source.composite_archives,
        &mut destination.composite_archives,
    );
    std::mem::swap(
        &mut source.primary_resource_root,
        &mut destination.primary_resource_root,
    );
    std::mem::swap(
        &mut source.secondary_resource_root,
        &mut destination.secondary_resource_root,
    );
    std::mem::swap(
        &mut source.validated_file_root,
        &mut destination.validated_file_root,
    );
    std::mem::swap(
        &mut source.system_wait_state,
        &mut destination.system_wait_state,
    );
    std::mem::swap(
        &mut source.next_program_instance_id,
        &mut destination.next_program_instance_id,
    );
    std::mem::swap(&mut source.next_thread_id, &mut destination.next_thread_id);
    if trace_records {
        tracing::info!(
            source_thread = source.thread.thread_id(),
            destination_thread = destination.thread.thread_id(),
            source_tables = source.indexed_record_tables.len(),
            source_records = indexed_record_count(source),
            destination_tables = destination.indexed_record_tables.len(),
            destination_records = indexed_record_count(destination),
            "VM indexed-record state transfer end"
        );
    }
}

fn indexed_record_count(vm: &Vm) -> usize {
    vm.indexed_record_tables
        .values()
        .map(|table| table.records.len())
        .sum()
}

fn async_queue_summary(vm: &Vm) -> (u32, usize, usize, usize, usize, Vec<[u32; 7]>) {
    let scenario_pending = read_u32(&vm.memory, 1_644);
    let mut motion_active = 0usize;
    let mut motion_pending = 0usize;
    for slot in 0..80usize {
        let base = 11_280 + slot * 432;
        motion_active += usize::from(read_u32(&vm.memory, base) != 0);
        motion_pending += usize::from(read_u32(&vm.memory, base + 8) != 0);
    }
    let mut body_active = 0usize;
    let mut body_pending = 0usize;
    let mut body_records = Vec::new();
    for slot in 0..64usize {
        let base = 45_840 + slot * 3_208;
        let active = read_u32(&vm.memory, base);
        body_active += usize::from(active != 0);
        body_pending += usize::from(read_u32(&vm.memory, base + 4) != 0);
        if active != 0 {
            body_records.push([
                slot as u32,
                active,
                read_u32(&vm.memory, base + 4),
                read_u32(&vm.memory, base + 8),
                read_u32(&vm.memory, base + 12),
                read_u32(&vm.memory, base + 16),
                read_u32(&vm.memory, base + 20),
            ]);
        }
    }
    (
        scenario_pending,
        motion_active,
        motion_pending,
        body_active,
        body_pending,
        body_records,
    )
}

fn read_u32(memory: &[u8], addr: usize) -> u32 {
    memory
        .get(addr..addr.saturating_add(4))
        .and_then(|bytes| bytes.try_into().ok())
        .map(u32::from_le_bytes)
        .unwrap_or_default()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn shadow_transfer_replaces_globals_but_preserves_local_and_heap_values() {
        let mut root = Vm::new();
        let mut task = Vm::new();
        for (vm, label) in [(&mut root, "root"), (&mut task, "task")] {
            for addr in [
                LOCAL_MEMORY_BASE - 1,
                LOCAL_MEMORY_BASE,
                crate::HEAP_MEMORY_BASE as u32,
            ] {
                vm.mem_values.insert(addr, Value::Str(label.into()));
            }
        }
        task.mem_values.insert(4, Value::Str("stale".into()));

        transfer_async_shared_state(&mut root, &mut task);
        assert!(!root.mem_values.contains_key(&(LOCAL_MEMORY_BASE - 1)));
        assert!(!task.mem_values.contains_key(&4));
        assert_eq!(
            task.mem_values[&(LOCAL_MEMORY_BASE - 1)],
            Value::Str("root".into())
        );
        // Locals stay with each thread; heap values travel with the memory.
        assert_eq!(root.mem_values[&LOCAL_MEMORY_BASE], Value::Str("root".into()));
        assert_eq!(task.mem_values[&LOCAL_MEMORY_BASE], Value::Str("task".into()));
        let heap = HEAP_MEMORY_BASE as u32;
        assert_eq!(task.mem_values[&heap], Value::Str("root".into()));
        assert!(!root.mem_values.contains_key(&heap));
        transfer_async_shared_state(&mut task, &mut root);
        assert_eq!(
            root.mem_values[&(LOCAL_MEMORY_BASE - 1)],
            Value::Str("root".into())
        );
        assert_eq!(root.mem_values[&heap], Value::Str("root".into()));
        assert_eq!(root.mem_values[&LOCAL_MEMORY_BASE], Value::Str("root".into()));
    }

    #[test]
    fn running_thread_holds_the_one_heap_and_idle_threads_keep_only_locals() {
        let mut root = Vm::new();
        let mut task = Vm::new();
        task.memory.truncate(HEAP_MEMORY_BASE);
        let local = LOCAL_MEMORY_BASE as usize;
        root.memory[local] = 1;
        task.memory[local] = 2;
        let ptr = root.alloc_heap(32);
        root.write_c_string_raw(ptr, "root").unwrap();
        root.write_value(ptr + 16, 2, &Value::Str("value".into()))
            .unwrap();

        transfer_async_shared_state(&mut root, &mut task);
        assert_eq!(task.read_c_string(ptr).unwrap(), "root");
        assert_eq!(task.read_value(ptr + 16, 2).unwrap(), Value::Str("value".into()));
        assert_eq!((local_byte(&task, local), local_byte(&root, local)), (2, 1));
        // The idle root keeps only its local region, not a heap copy.
        assert!(root.memory.is_empty());
        task.write_c_string_raw(ptr, "task").unwrap();

        transfer_async_shared_state(&mut task, &mut root);
        assert_eq!(root.read_c_string(ptr).unwrap(), "task");
        assert_eq!((local_byte(&root, local), local_byte(&task, local)), (1, 2));
        assert!(task.memory.is_empty());
    }

    #[test]
    fn async_state_transfer_keeps_heap_ownership_with_the_active_thread() {
        let mut root = Vm::new();
        let first = root.alloc_heap(64);
        let second = root.alloc_heap(96);
        assert!(root.free_heap(first));

        let mut task = Vm::new();
        transfer_async_shared_state(&mut root, &mut task);

        assert!(task.free_heap(second));
        assert_eq!(task.alloc_heap(160), first);

        transfer_async_shared_state(&mut task, &mut root);
        assert!(root.target_heap.translate(first).is_some());
        assert!(task.target_heap.translate(first).is_none());
    }

    #[test]
    fn async_state_transfer_moves_system80_global_names_and_record_handle_counter() {
        let mut root = Vm::new();
        root.set_resource_names(vec!["BG0001".into()]);
        root.next_indexed_record_handle = 17;

        let mut task = Vm::new();
        transfer_async_shared_state(&mut root, &mut task);
        assert_eq!(task.resource_names(), ["BG0001".to_string()]);
        assert_eq!(task.next_indexed_record_handle, 17);

        task.set_resource_names(vec!["BG0001".into(), "face".into()]);
        task.next_indexed_record_handle = 18;
        transfer_async_shared_state(&mut task, &mut root);
        assert_eq!(
            root.resource_names(),
            ["BG0001".to_string(), "face".to_string()]
        );
        assert_eq!(root.next_indexed_record_handle, 18);
    }

    #[test]
    fn child_threads_borrow_locals_without_keeping_idle_windows() {
        let program = ethornell_script::parse_bp_program(None, &[0x17]);
        let mut root = Vm::new();
        root.start(&program);
        let local = LOCAL_MEMORY_BASE as usize;
        root.memory[local + 32] = 0x12;
        let code_byte = local + crate::CODE_WINDOW_OFFSET as usize + 17;
        root.memory[code_byte] = 0x34;
        let heap = root.alloc_heap(16);
        root.write_c_string_raw(heap, "shared").unwrap();
        let owner = root.thread.thread_id();
        root.start_child_thread(64, 256, 256, 0, false).unwrap();
        root.start_child_thread(64, 256, 256, 0, false).unwrap();
        let mut children = std::mem::take(&mut root.async_tasks);
        for (index, child) in children.iter_mut().enumerate() {
            assert_eq!(child.vm.local_window_owner, Some(owner));
            assert!(child.vm.memory.is_empty());
            assert!(child.vm.parked_local.is_none());
            transfer_async_shared_state(&mut root, &mut child.vm);
            crate::child_thread::lend_local_window(&mut root, &mut child.vm);
            assert_eq!(child.vm.memory[local + 32], 0x12 + index as u8);
            assert_eq!(child.vm.memory[code_byte], 0x34 + index as u8);
            assert_eq!(child.vm.read_c_string(heap).unwrap(), "shared");
            child.vm.memory[local + 32] += 1;
            child.vm.memory[code_byte] += 1;
            crate::child_thread::lend_local_window(&mut root, &mut child.vm);
            transfer_async_shared_state(&mut child.vm, &mut root);
            assert!(child.vm.memory.is_empty());
            assert!(child.vm.parked_local.as_ref().unwrap().is_empty());
            assert!(child.vm.local_window_scratch.is_empty());
            // The process scratch only stages compact prefixes; it never
            // needs more storage than the former dense 2 MiB window.
            assert!(root.local_window_scratch.capacity() <= crate::HEAP_OFFSET_BASE as usize);
        }
        assert_eq!(root.memory[local + 32], 0x14);
        assert_eq!(root.memory[code_byte], 0x36);
        assert_eq!(root.read_c_string(heap).unwrap(), "shared");
    }

    #[test]
    fn new_async_thread_installs_code_without_a_full_process_buffer() {
        let code = [0x02, 1, 2, 3, 4, 0x17];
        let program = ethornell_script::parse_bp_program(None, &code);
        let mut root = Vm::new();
        root.start_async_program_with_args(Value::Program(Arc::new(program)), Vec::new(), false)
            .unwrap();
        let mut child = root.async_tasks.pop().unwrap().vm;
        assert!(child.memory.is_empty());
        transfer_async_shared_state(&mut root, &mut child);
        assert_eq!(
            &child.memory[(LOCAL_MEMORY_BASE + crate::CODE_WINDOW_OFFSET) as usize..][..code.len()],
            &code,
        );
    }

    #[test]
    fn global_user_data_moves_between_threads_without_idle_snapshots() {
        let mut root = Vm::new();
        let last = root.global_user_data.len() - 1;
        root.global_user_data[0] = 0x12;
        root.global_user_data[last] = 0x34;
        let program = ethornell_script::parse_bp_program(None, &[0x17]);
        root.start_async_program_with_args(Value::Program(Arc::new(program)), Vec::new(), false)
            .unwrap();
        let mut child = root.async_tasks.pop().unwrap().vm;
        assert!(child.global_user_data.is_empty());
        let allocation = root.global_user_data.as_ptr();
        transfer_async_shared_state(&mut root, &mut child);
        assert!(root.global_user_data.is_empty());
        assert_eq!(child.global_user_data.as_ptr(), allocation);
        assert_eq!(child.global_user_data[0], 0x12);
        assert_eq!(child.global_user_data[last], 0x34);
        child.global_user_data[last] = 0x56;
        transfer_async_shared_state(&mut child, &mut root);
        assert!(child.global_user_data.is_empty());
        assert_eq!(root.global_user_data.as_ptr(), allocation);
        assert_eq!(root.global_user_data[last], 0x56);
    }
}
