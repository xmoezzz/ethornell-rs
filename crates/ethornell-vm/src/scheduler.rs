use crate::{
    NativeCallFrame, NativeOpcode, SchedulerSignal, SysApi, Value, Vm, VmResult,
    native_call::opcodes, native_thread::InstalledCProcedure,
};
use ethornell_script::BpInstruction;

impl Vm {
    /// Dispatch native opcodes that directly control the cooperative BP
    /// scheduler. Keeping these handlers outside the generic system-call chain
    /// makes their parameters and stop semantics explicit and prevents a
    /// synchronous fallback from accidentally swallowing a yield.
    pub(super) fn dispatch_scheduler_opcode<A>(
        &mut self,
        api: &mut A,
        opcode: NativeOpcode,
        program_index: usize,
        instruction: &BpInstruction,
        trace_events: bool,
    ) -> VmResult<Option<Value>>
    where
        A: SysApi,
    {
        match opcode {
            opcodes::SYS_WAIT_WINDOW_MESSAGE => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let message_id = call.pop_i32("message_id")?;
                call.require_consumed()?;
                let registered_after_serial = api.window_message_sequence();
                self.install_cprocedure(
                    InstalledCProcedure::wait_window_message(
                        self.thread.thread_id(),
                        opcode,
                        message_id,
                        registered_after_serial,
                    ),
                    trace_events,
                );
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        message_id = format_args!("0x{message_id:04X}"),
                        registered_after_serial,
                        "Sys80_54_WaitWndMsg registered a target-shaped window-message wait"
                    );
                }
                Ok(Some(Value::None))
            }
            opcodes::SYS_SET_THREAD_TIMER => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let duration_ms = call.pop_i32("duration_ms")?.max(0);
                call.require_consumed()?;
                self.thread
                    .set_deadline_from_now(self.timing.tick_count(), duration_ms);
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        duration_ms,
                        deadline_tick = self.thread.deadline_tick(),
                        "Sys80_58_SetThreadTimer updated CThread+0x84"
                    );
                }
                Ok(Some(Value::None))
            }
            opcodes::SYS_SET_AND_QUERY_THREAD_TIMER => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let duration_ms = call.pop_i32("duration_ms")?.max(0);
                call.require_consumed()?;
                self.thread
                    .set_deadline_from_now(self.timing.tick_count(), duration_ms);
                let active = self.thread.remaining_deadline_ms(self.timing.tick_count()) != 0;
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        duration_ms,
                        deadline_tick = self.thread.deadline_tick(),
                        active,
                        "Sys80_59_SetAndQueryThreadTimer"
                    );
                }
                Ok(Some(Value::Int(i32::from(active))))
            }
            opcodes::SYS_WAIT_THREAD_TIMER => {
                let call = self.scheduler_call_frame(opcode)?;
                call.require_consumed()?;
                let remaining_ms = self.thread.remaining_deadline_ms(self.timing.tick_count());
                if remaining_ms > 0 {
                    self.install_cprocedure(
                        InstalledCProcedure::wait_timing(
                            self.thread.thread_id(),
                            opcode,
                            self.thread.deadline_tick(),
                            remaining_ms,
                        ),
                        trace_events,
                    );
                }
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        remaining_ms,
                        installed = remaining_ms > 0,
                        "Sys80_5A_WaitTiming evaluated the thread timer"
                    );
                }
                Ok(Some(Value::Int(i32::from(remaining_ms > 0))))
            }
            opcodes::SYS_WAIT_TIMING_EX => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let input_scope = call.pop_i32("input_scope")?;
                let input_enabled = call.pop_i32("input_enabled")? != 0;
                let duration_ms = call.pop_i32("duration_ms")?;
                call.require_consumed()?;

                // sub_43D2E0: the deadline lives in the procedure (+0x08,
                // sub_431A10); the thread timer of 0x80:0x5A is untouched.
                // With input enabled the scope is registered exactly like
                // 0x80:0x18 and primed by one discarded sub_46DF00 query.
                let deadline_tick =
                    (self.timing.tick_count().max(0) as u32).wrapping_add(duration_ms as u32);
                if input_enabled {
                    api.register_input_scope(input_scope);
                }
                self.install_cprocedure(
                    InstalledCProcedure::wait_timing_ex(
                        self.thread.thread_id(),
                        opcode,
                        deadline_tick,
                        duration_ms,
                        i32::from(input_enabled),
                        input_scope,
                    ),
                    trace_events,
                );
                if trace_events || self.collect_diagnostics {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        duration_ms,
                        input_enabled,
                        input_scope,
                        "Sys80_5C_WaitTimingEx installed a cooperative wait procedure"
                    );
                }
                Ok(Some(Value::None))
            }
            opcodes::SYS_SET_EXCLUSIVE_THREAD => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let enabled = call.pop_i32("enabled")? != 0;
                call.require_consumed()?;
                self.exclusive_thread_id = enabled.then(|| self.thread.thread_id());
                Ok(Some(Value::None))
            }
            opcodes::SYS_SELECT_BOOTSTRAP => {
                // Native pop order is archive namespace, then root/archive path.
                let archive_namespace = self.pop_string_lossy()?;
                let root_or_archive_path = self.pop_string_lossy()?;
                api.select_bootstrap(&root_or_archive_path, &archive_namespace);
                self.scheduler_signal = Some(SchedulerSignal::EndPass);
                Ok(Some(Value::None))
            }
            opcodes::SYS_SWITCH_PROGRAM => {
                let mut call = self.scheduler_call_frame(opcode)?;
                let program = call.pop_value("program")?;
                call.require_consumed()?;
                let (activated, target_thread_id) =
                    self.switch_to_async_program(program, trace_events);
                self.scheduler_switch_target = target_thread_id;
                self.scheduler_signal = Some(SchedulerSignal::SwitchThread);
                if trace_events {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        activated,
                        ?target_thread_id,
                        "Sys80_5E_SwitchThread selected the next cooperative CThread"
                    );
                }
                Ok(Some(Value::None))
            }
            opcodes::SYS_YIELD => {
                let call = self.scheduler_call_frame(opcode)?;
                call.require_consumed()?;
                self.scheduler_signal = Some(SchedulerSignal::Yield);
                tracing::debug!(
                    target: "vm_yield",
                    tick_ms = self.timing.tick_count(),
                    program = self.program_name(program_index),
                    pc = self.pc,
                    offset = format_args!("0x{:08X}", instruction.offset),
                    stack_top = ?self.stack_summary(8),
                    "Sys80_5F_Yield ended the current cooperative slice"
                );
                Ok(Some(Value::None))
            }
            opcodes::SYS_TERMINATE_INTERPRETER => {
                let call = self.scheduler_call_frame(opcode)?;
                call.require_consumed()?;
                self.halted = true;
                self.scheduler_signal = Some(SchedulerSignal::TerminateInterpreter);
                if trace_events {
                    tracing::debug!(
                        program = self.program_name(program_index),
                        offset = format_args!("0x{:08X}", instruction.offset),
                        "Sys80_6A_TerminateInterpreter halted the active interpreter"
                    );
                }
                Ok(Some(Value::None))
            }
            _ => Ok(None),
        }
    }

    fn scheduler_call_frame(&mut self, opcode: NativeOpcode) -> VmResult<NativeCallFrame> {
        let args = self.take_dispatch_call_frame(opcode.group, opcode.id)?;
        Ok(NativeCallFrame::new(opcode, args))
    }
}
