use crate::{
    ADDRESS_MASK, GraphApi, LOCAL_MEMORY_BASE, SoundApi, SysApi, Value, Vm, VmRunOptions,
    VmStopReason,
};
use ethornell_script::BpProgram;
use std::sync::Arc;

#[derive(Debug)]
pub(crate) struct AsyncProgramTask {
    key: String,
    vm: Box<Vm>,
    pub(crate) runnable: bool,
    completed: bool,
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
            if thread_id == 0 || self.native_thread_exists(thread_id) {
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
        let Some(program) = self.value_program(program) else {
            return None;
        };
        let key = program_key(&program);

        self.flush_shared_heap();
        let mut vm = Vm::new();
        vm.shared_heap = Arc::clone(&self.shared_heap);
        vm.system80_shared = Arc::clone(&self.system80_shared);
        vm.system81_shared = Arc::clone(&self.system81_shared);
        vm.sync_shared_heap();
        let shared_end = shared_memory_end(self);
        vm.memory[..shared_end].copy_from_slice(&self.memory[..shared_end]);
        vm.mem_values = self
            .mem_values
            .range(..LOCAL_MEMORY_BASE)
            .map(|(addr, value)| (*addr, value.clone()))
            .collect();
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
        vm.global_config = self.global_config.clone();
        vm.global_user_data = self.global_user_data.clone();
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
        vm.start(&program);
        let thread_id = self.next_thread_id.max(1);
        self.next_thread_id = self.next_thread_id.saturating_add(1).max(1);
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
            if thread_id == 0 {
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
            if thread_id == 0 {
                if self.thread.thread_id() == 0 {
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
            if thread_id == 0 {
                return (true, Some(0));
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
        watchdog_steps: usize,
    ) where
        A: SysApi + GraphApi + SoundApi,
    {
        self.flush_shared_heap();
        self.sync_shared_heap();
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
                Some(thread_id) if thread_id == 0 || thread_id == self.thread.thread_id() => {
                    // Status 3 selected the root CThread; `run_loaded` executes it
                    // immediately after returning from this child scheduler.
                    self.refresh_thread_links();
                    return;
                }
                Some(thread_id) => {
                    let Some(index) = self.async_tasks.iter().position(|task| {
                        !task.completed && task.vm.thread.thread_id() == thread_id
                    }) else {
                        // Native sub_444B90 returns null for an unknown id and the
                        // outer list walk terminates instead of falling back to the
                        // first thread.
                        self.refresh_thread_links();
                        return;
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
                max_steps: watchdog_steps,
                trace: false,
                fail_on_stub: false,
                collect_diagnostics: false,
            };
            let mut task = self.async_tasks.remove(cursor);
            task.vm.sync_shared_heap();
            transfer_async_shared_state(self, &mut task.vm);
            let mut scheduler_tasks = std::mem::take(&mut self.async_tasks);
            scheduler_tasks.append(&mut task.vm.async_tasks);
            task.vm.async_tasks = scheduler_tasks;
            task.vm.suppress_async_pump_once = true;
            task.vm.root_procedure_active = if self.thread.thread_id() == 0 {
                self.thread.current_procedure().is_some()
            } else {
                self.root_procedure_active
            };
            let report = task.vm.run_loaded(api, &options);
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
            task.vm.flush_shared_heap();
            self.sync_shared_heap();
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
                VmStopReason::Completed
                    | VmStopReason::InterpreterTerminated
                    | VmStopReason::Error
                    | VmStopReason::UnknownOpcode
                    | VmStopReason::UnknownDispatch
            ) {
                task.completed = true;
                if report.stop_reason != VmStopReason::InterpreterTerminated {
                    task.vm.thread.mark_terminated();
                }
            }
            let mut scheduler_tasks = std::mem::take(&mut task.vm.async_tasks);
            transfer_async_shared_state(&mut task.vm, self);
            self.thread.extend_messages(root_messages);
            for callback in root_callbacks {
                self.enqueue_native_callback(callback, trace_events);
            }
            scheduler_tasks.insert(cursor.min(scheduler_tasks.len()), task);
            self.async_tasks = scheduler_tasks;
            scheduler_hops += 1;

            if report.stop_reason == VmStopReason::SwitchedThread {
                let Some(target_thread_id) = requested_switch else {
                    break;
                };
                if target_thread_id == 0 || target_thread_id == self.thread.thread_id() {
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

    pub(crate) fn mark_shared_heap_dirty(&mut self, range: std::ops::Range<usize>) {
        if range.start >= range.end {
            return;
        }
        let mut start = range.start;
        let mut end = range.end;
        self.shared_heap_dirty.retain(|existing| {
            if existing.end < start || existing.start > end {
                true
            } else {
                start = start.min(existing.start);
                end = end.max(existing.end);
                false
            }
        });
        self.shared_heap_dirty.push(start..end);
    }

    fn flush_shared_heap(&mut self) {
        if self.shared_heap_dirty.is_empty() {
            return;
        }
        let ranges = std::mem::take(&mut self.shared_heap_dirty);
        let mut shared = self
            .shared_heap
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        shared.generation = shared.generation.saturating_add(1);
        let generation = shared.generation;
        for range in ranges {
            if shared.bytes.len() < range.end {
                shared.bytes.resize(range.end, 0);
            }
            shared.bytes[range.clone()].copy_from_slice(&self.memory[range.clone()]);
            let value_range = range.start as u32..range.end as u32;
            shared
                .values
                .extract_if(value_range.clone(), |_, _| true)
                .for_each(drop);
            shared.values.extend(
                self.mem_values
                    .range(value_range)
                    .map(|(addr, value)| (*addr, value.clone())),
            );
            shared
                .journal
                .push(super::SharedHeapJournalEntry { generation, range });
        }
        self.shared_heap_generation = generation;
    }

    fn sync_shared_heap(&mut self) {
        let shared = self
            .shared_heap
            .lock()
            .unwrap_or_else(|poisoned| poisoned.into_inner());
        if self.shared_heap_generation == shared.generation {
            return;
        }
        let first = shared
            .journal
            .partition_point(|entry| entry.generation <= self.shared_heap_generation);
        let ranges = shared.journal[first..]
            .iter()
            .map(|entry| entry.range.clone())
            .collect::<Vec<_>>();
        for range in ranges {
            if self.memory.len() < range.end {
                self.memory.resize(range.end, 0);
            }
            self.memory[range.clone()].copy_from_slice(&shared.bytes[range.clone()]);
            let value_range = range.start as u32..range.end as u32;
            self.mem_values
                .extract_if(value_range.clone(), |_, _| true)
                .for_each(drop);
            self.mem_values.extend(
                shared
                    .values
                    .range(value_range)
                    .map(|(addr, value)| (*addr, value.clone())),
            );
        }
        self.shared_heap_generation = shared.generation;
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
                .get(&Self::value_key(ptr))
                .and_then(|value| {
                    if let Value::Program(program) = value {
                        Some((**program).clone())
                    } else {
                        None
                    }
                }),
            Value::Int(ptr) => {
                self.mem_values
                    .get(&Self::value_key(ptr as u32))
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

fn program_key(program: &BpProgram) -> String {
    program
        .script_name
        .as_deref()
        .unwrap_or("<anonymous>")
        .to_string()
}

fn shared_memory_end(vm: &Vm) -> usize {
    (vm.heap_ptr as usize)
        .min(LOCAL_MEMORY_BASE as usize)
        .min(vm.memory.len())
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
    let shared_end = shared_memory_end(source);
    if destination.memory.len() < shared_end {
        destination.memory.resize(shared_end, 0);
    }
    destination.memory[..shared_end].copy_from_slice(&source.memory[..shared_end]);

    // Leave thread-local and heap shadows in place. Only the global segment
    // moves with the active thread; heap shadows use the dirty-range journal.
    destination
        .mem_values
        .extract_if(..LOCAL_MEMORY_BASE, |_, _| true)
        .for_each(drop);
    destination.mem_values.extend(
        source
            .mem_values
            .extract_if(..LOCAL_MEMORY_BASE, |_, _| true),
    );

    std::mem::swap(&mut source.heap_ptr, &mut destination.heap_ptr);
    std::mem::swap(
        &mut source.heap_allocations,
        &mut destination.heap_allocations,
    );
    std::mem::swap(
        &mut source.heap_free_blocks,
        &mut destination.heap_free_blocks,
    );
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
    std::mem::swap(&mut source.resource_names, &mut destination.resource_names);
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
    std::mem::swap(&mut source.global_config, &mut destination.global_config);
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
        for addr in [LOCAL_MEMORY_BASE, crate::HEAP_MEMORY_BASE as u32] {
            assert_eq!(root.mem_values[&addr], Value::Str("root".into()));
            assert_eq!(task.mem_values[&addr], Value::Str("task".into()));
        }
        transfer_async_shared_state(&mut task, &mut root);
        assert_eq!(
            root.mem_values[&(LOCAL_MEMORY_BASE - 1)],
            Value::Str("root".into())
        );
    }

    #[test]
    fn heap_sync_applies_all_ranges_in_each_unseen_generation() {
        let mut root = Vm::new();
        let mut task = Vm::new();
        task.shared_heap = Arc::clone(&root.shared_heap);
        let ptr = root.alloc_heap(32);
        root.write_value(ptr, 2, &Value::Str("first".into()))
            .unwrap();
        root.write_value(ptr + 16, 2, &Value::Str("second".into()))
            .unwrap();
        root.flush_shared_heap();
        task.sync_shared_heap();
        assert_eq!(task.mem_values, root.mem_values);

        root.write_c_string_raw(ptr, "raw").unwrap();
        root.write_value(ptr + 16, 2, &Value::Str("updated".into()))
            .unwrap();
        root.flush_shared_heap();
        task.sync_shared_heap();
        assert_eq!(task.read_c_string(ptr).unwrap(), "raw");
        assert_eq!(task.mem_values, root.mem_values);
        task.sync_shared_heap();
        assert_eq!(task.mem_values, root.mem_values);
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
        assert!(root.heap_allocations.contains_key(&(first & ADDRESS_MASK)));
        assert!(!task.heap_allocations.contains_key(&(first & ADDRESS_MASK)));
    }

    #[test]
    fn async_state_transfer_moves_system80_global_names_and_record_handle_counter() {
        let mut root = Vm::new();
        root.resource_names.push("BG0001".into());
        root.next_indexed_record_handle = 17;

        let mut task = Vm::new();
        transfer_async_shared_state(&mut root, &mut task);
        assert_eq!(task.resource_names, vec!["BG0001".to_string()]);
        assert_eq!(task.next_indexed_record_handle, 17);

        task.resource_names.push("face".into());
        task.next_indexed_record_handle = 18;
        transfer_async_shared_state(&mut task, &mut root);
        assert_eq!(
            root.resource_names,
            vec!["BG0001".to_string(), "face".to_string()]
        );
        assert_eq!(root.next_indexed_record_handle, 18);
    }
}
