//! Sys81:48, 1400B72C0: isolated DCTChildThread invocations.
//! The portable runner uses a serial interleaving of the native logical lanes;
//! it never clones global memory and merges stale copies. All lanes finish
//! before the caller proceeds. This is not a hardware-throughput guarantee.
use crate::*;
impl Vm {
    pub(super) fn execute_parallel_call<A: SysApi + GraphApi + SoundApi>(
        &mut self,
        api: &mut A,
    ) -> VmResult<()> {
        let entry = self.pop_int()? as u32;
        let data = self.pop_int()? as u32;
        let code = self.pop_int()? as u32;
        let slots = self.pop_int()? as u32;
        let lanes = api.native_worker_count();
        if lanes == 0 || lanes > 64 || slots > 1_048_576 {
            return Err(VmError::Runtime(
                "native parallel invocation allocation limit".into(),
            ));
        }
        let (program, offset) = self.resolve_code_target(self.current_program, entry);
        self.jump_target_index(program, offset)?;
        let initial = self.async_tasks.len();
        let mut created = 0;
        let result = (|| {
            for lane in 0..lanes {
                self.start_child_thread(slots, code, data, entry, false)?;
                self.async_tasks
                    .last_mut()
                    .unwrap()
                    .vm
                    .parallel_worker_index = Some(lane as i32);
                created += 1;
            }
            for _ in 0..created {
                let mut task = self.async_tasks.remove(initial);
                crate::async_program::transfer_async_shared_state(self, &mut task.vm);
                crate::child_thread::lend_local_window(self, &mut task.vm);
                task.vm.async_tasks = std::mem::take(&mut self.async_tasks);
                let mut total = 0usize;
                let outcome = loop {
                    task.vm.suppress_async_pump_once = true;
                    let report = task.vm.run_loaded_internal(
                        api,
                        &VmRunOptions {
                            max_steps: 65536,
                            trace: false,
                            fail_on_stub: true,
                            collect_diagnostics: false,
                        },
                        false,
                    );
                    total = total.saturating_add(report.steps);
                    if report.stop_reason == VmStopReason::Completed {
                        break Ok(());
                    }
                    if report.stop_reason == VmStopReason::InterpreterTerminated {
                        self.halted = true;
                        break Ok(());
                    }
                    if matches!(
                        report.stop_reason,
                        VmStopReason::WatchdogExceeded | VmStopReason::QuantumExhausted
                    ) && total < 0x10000000
                    {
                        continue;
                    }
                    break Err(VmError::Runtime(format!(
                        "native parallel lane {} stopped at {} PC {}: {:?}; {}",
                        task.vm.parallel_worker_index.unwrap(),
                        report.program,
                        report.pc,
                        report.stop_reason,
                        report.recent_trace.join("; ")
                    )));
                };
                self.async_tasks = std::mem::take(&mut task.vm.async_tasks);
                crate::child_thread::lend_local_window(self, &mut task.vm);
                crate::async_program::transfer_async_shared_state(&mut task.vm, self);
                task.vm.release_child_thread_regions();
                created -= 1;
                outcome?;
            }
            Ok(())
        })();
        // Construction or execution failure still retires every reserved child.
        for _ in 0..created {
            let mut task = self.async_tasks.remove(initial);
            task.vm.release_child_thread_regions();
        }
        self.refresh_thread_links();
        result
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    struct Host;
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn native_worker_count(&self) -> usize {
            2
        }
    }
    impl GraphApi for Host {
        fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
    }
    impl SoundApi for Host {
        fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
    }
    #[test]
    fn version_parallel_lanes_write_shared_globals_and_release_their_regions() {
        // [256 + lane*4] = lane + 10; each lane obtains its native logical ID.
        let code = [
            0x02, 0, 1, 0, 0, 0x80, 0x46, 0x00, 4, 0x22, 0x20, 0x80, 0x46, 0x00, 10, 0x20, 0x09, 2,
            0x17,
        ];
        let program = ethornell_script::parse_bp_program(Some("parallel._bp".into()), &code);
        let mut vm = Vm::new();
        vm.start(&program);
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        vm.stack = vec![
            Value::Int(16),
            Value::Int(256),
            Value::Int(256),
            Value::Int(0),
        ];
        vm.execute_parallel_call(&mut Host).unwrap();
        assert_eq!(vm.read_int(256, 2).unwrap(), 10);
        assert_eq!(vm.read_int(260, 2).unwrap(), 11);
        assert!(vm.async_tasks.is_empty());
        assert!(vm.parallel_worker_index.is_none());
    }
}
