use crate::{NativeOpcode, SysApi, Value, Vm, VmError, VmResult, native_call::opcodes};
use std::sync::Arc;

impl Vm {
    /// Dispatch the target CThread module, child-thread, FIFO, and callback
    /// handlers as one shared scheduler subsystem.
    pub(super) fn dispatch_program_thread_opcode<A>(
        &mut self,
        api: &mut A,
        opcode: NativeOpcode,
        trace_events: bool,
    ) -> VmResult<Option<Value>>
    where
        A: SysApi,
    {
        let result = match opcode {
            opcodes::SYS_LOAD_PROGRAM_MODULE => {
                // sub_488C00 pops file then archive and appends the decoded BP
                // image to the current CThread's module chain.
                let file = self.pop_string_lossy()?;
                let archive = self.pop_string_lossy()?;
                // sub_465AB0 failing to read/decode the module is fatal.
                let Some(mut program) = api.load_program(&archive, &file) else {
                    return Err(VmError::Runtime(format!(
                        "LoadProgramModule: cannot load {archive}:{file}"
                    )));
                };
                self.assign_program_instance(&mut program);
                // sub_444CE0 refuses an image that does not fit the code
                // region; sub_488C00 turns that into a fatal error.
                let size = Self::program_target_code_size(&program);
                let used = self.module_chain_code_used();
                if u64::from(size) + u64::from(used) > u64::from(self.thread.code_region_size()) {
                    return Err(VmError::Runtime(format!(
                        "cannot load {archive}:{file}: {size} bytes do not fit the code region ({used} of {} used)",
                        self.thread.code_region_size()
                    )));
                }
                Value::Int(self.append_target_loaded_program(program) as i32)
            }
            opcodes::SYS_FREE_LAST_PROGRAM_MODULE => {
                // sub_488CD0 pops nothing. sub_444D80 returns the module count
                // left (0x80000001 without any record); a count of 0 is a
                // fatal error after the record is gone.
                let (remaining, freed) = self.free_last_target_program(trace_events);
                if let Some(program) = freed {
                    api.free_program(Value::Program(Arc::new(program)));
                }
                if remaining == 0 {
                    return Err(VmError::Runtime(
                        "FreeLastProgramModule removed the last module of the thread".into(),
                    ));
                }
                Value::Int(remaining)
            }
            opcodes::SYS_LOAD_PROGRAM_THREAD => {
                // sub_488D00 native pop order is data bytes, code bytes,
                // operand slots, file, archive.
                let data_bytes = self.pop_value()?;
                let code_bytes = self.pop_value()?;
                let operand_slots = self.pop_value()?;
                let file = self.pop_string_lossy()?;
                let archive = self.pop_string_lossy()?;
                let parameters = [data_bytes, code_bytes, operand_slots];
                // sub_48D080: a file sub_465AB0 cannot read is a script
                // error, as is a module that does not fit the code region
                // (sub_444CE0 returns 0x80000000).
                let Some(mut program) = api.load_program_ex(&archive, &file, &parameters) else {
                    return Err(VmError::Runtime(format!(
                        "LoadProgramThread cannot read {archive}:{file}"
                    )));
                };
                self.assign_program_instance(&mut program);
                let region = |value: &Value| u32::try_from(value.as_i32()).unwrap_or(0);
                let (data_bytes, code_bytes, slots) = (
                    region(&parameters[0]),
                    region(&parameters[1]),
                    region(&parameters[2]),
                );
                let size = Self::program_target_code_size(&program);
                if size > code_bytes {
                    return Err(VmError::Runtime(format!(
                        "cannot load {archive}:{file}: {size} bytes do not fit a {code_bytes}-byte code region"
                    )));
                }
                let thread_id = self
                    .start_async_program_with_args(
                        Value::Program(Arc::new(program)),
                        Vec::new(),
                        trace_events,
                    )
                    .unwrap_or(0);
                self.set_newest_async_thread_regions(code_bytes, data_bytes, slots);
                Value::Int(thread_id)
            }
            opcodes::SYS_CURRENT_THREAD_ID => Value::Int(self.thread.thread_id()),
            opcodes::SYS_THREAD_EXISTS => {
                let thread_or_program = self.pop_value()?;
                Value::Int(self.async_program_is_active(thread_or_program))
            }
            opcodes::SYS_ENQUEUE_MESSAGE => self.sys80_48_enqueue_message(trace_events)?,
            opcodes::SYS_DEQUEUE_MESSAGE => self.sys80_49_dequeue_message()?,
            opcodes::SYS_ENQUEUE_MESSAGE_ARRAY => {
                self.sys80_4a_enqueue_message_array(trace_events)?
            }
            opcodes::SYS_DEQUEUE_MESSAGE_ARRAY => self.sys80_4b_dequeue_message_array()?,
            opcodes::SYS_INVOKE_THREAD_CALLBACK => {
                self.sys80_4c_invoke_thread_callback(trace_events)?
            }
            _ => return Ok(None),
        };
        Ok(Some(result))
    }
}
