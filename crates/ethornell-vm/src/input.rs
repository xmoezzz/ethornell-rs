use crate::{
    NativeCallFrame, NativeOpcode, SysApi, Value, Vm, VmError, VmResult, native_call::opcodes,
};

pub(crate) const TITLE_PENDING_CALLBACK_ADDR: u32 = 1856;
const MOUSE_CLICK_EVENT: i32 = 0x1000_0006;

pub(crate) fn clears_title_pending_callback(event: i32, payload: i32) -> bool {
    let logical_button = payload & 0xffff;
    event == MOUSE_CLICK_EVENT && (0..=8).contains(&logical_button)
}

impl Vm {
    /// Dispatch target-confirmed input opcodes through named call frames.
    ///
    /// These selector IDs come from the executable's real 0x80 function-pointer
    /// table, joined with the IDA/Hex-Rays database. In particular, 0x14/0x15
    /// are the two configuration writes; 0x18/0x19 are scoped registration
    /// operations and must never be treated as those writes.
    pub(super) fn dispatch_input_opcode<A: SysApi>(
        &mut self,
        api: &mut A,
        opcode: NativeOpcode,
    ) -> VmResult<Option<Value>> {
        match opcode {
            opcodes::SYS_RESET_INPUT_CONFIGURATION => {
                let mut call = self.input_call_frame(opcode)?;
                let value = call.pop_i32("value")?;
                call.require_consumed()?;
                api.reset_input_configuration(value);
                Ok(Some(Value::None))
            }
            opcodes::SYS_INPUT_MESSAGE_SERIAL => {
                let call = self.input_call_frame(opcode)?;
                call.require_consumed()?;
                Ok(Some(Value::Int(api.input_message_serial())))
            }
            opcodes::SYS_SET_INPUT_MASTER_GATE => {
                let mut call = self.input_call_frame(opcode)?;
                let value = call.pop_i32("value")?;
                call.require_consumed()?;
                api.set_input_master_gate(value);
                Ok(Some(Value::None))
            }
            opcodes::SYS_SET_INPUT_LATCHED_STATE => {
                let mut call = self.input_call_frame(opcode)?;
                let value = call.pop_i32("value")?;
                call.require_consumed()?;
                api.set_input_latched_state(value);
                Ok(Some(Value::None))
            }
            opcodes::SYS_SAMPLE_CONFIGURED_INPUT => {
                let call = self.input_call_frame(opcode)?;
                call.require_consumed()?;
                api.sample_configured_input();
                Ok(Some(Value::None))
            }
            opcodes::SYS_QUERY_CONFIGURED_INPUT_GATE => {
                let call = self.input_call_frame(opcode)?;
                call.require_consumed()?;
                Ok(Some(Value::Int(api.query_configured_input_gate())))
            }
            opcodes::SYS_REGISTER_INPUT_SCOPE => {
                let mut call = self.input_call_frame(opcode)?;
                let scope = call.pop_i32("scope")?;
                call.require_consumed()?;
                api.register_input_scope(scope);
                Ok(Some(Value::None))
            }
            opcodes::SYS_QUERY_AND_UNREGISTER_INPUT_SCOPE => {
                let mut call = self.input_call_frame(opcode)?;
                let scope = call.pop_i32("scope")?;
                call.require_consumed()?;
                api.query_and_unregister_input_scope(scope);
                Ok(Some(Value::None))
            }
            opcodes::SYS_QUERY_INPUT_EVENT_BITS => {
                let mut call = self.input_call_frame(opcode)?;
                let scope = call.pop_i32("scope")?;
                call.require_consumed()?;
                Ok(Some(Value::Int(api.query_input_event_bits(scope))))
            }
            opcodes::SYS_REGISTER_INPUT_CLASS_DESCRIPTORS => {
                let mut call = self.input_call_frame(opcode)?;
                let descriptor_ptr = call.pop_ptr("descriptors")?;
                let class_mask = call.pop_i32("class_mask")?;
                call.require_consumed()?;
                let descriptors = self.read_zero_terminated_input_descriptors(descriptor_ptr)?;
                // sub_46DFA0 accepts only these mutable class lists; anything
                // else is a script error in sub_4881C0.
                const MUTABLE_CLASSES: [u32; 10] = [
                    0x40, 0x80, 0x100, 0x200, 0x1000, 0x2000, 0x4000, 0x8000, 0x4000_0000,
                    0x8000_0000,
                ];
                if !MUTABLE_CLASSES.contains(&(class_mask as u32)) {
                    return Err(VmError::Runtime(format!(
                        "Sys80_1B input class {class_mask:#x} cannot be redefined"
                    )));
                }
                api.register_input_class_descriptors(class_mask, &descriptors);
                Ok(Some(Value::None))
            }
            opcodes::SYS_QUERY_INPUT_CLASS_LEVEL => {
                let mut call = self.input_call_frame(opcode)?;
                let class_mask = call.pop_i32("class_mask")?;
                call.require_consumed()?;
                Ok(Some(Value::Int(
                    api.query_input_descriptor_state(class_mask),
                )))
            }
            opcodes::SYS_QUERY_SCOPED_INPUT_EVENT => {
                let mut call = self.input_call_frame(opcode)?;
                let input_descriptor = call.pop_i32("input_descriptor")?;
                let scope = call.pop_i32("scope")?;
                call.require_consumed()?;
                Ok(Some(Value::Int(
                    api.query_scoped_input_event(input_descriptor, scope),
                )))
            }
            _ => Ok(None),
        }
    }

    fn read_zero_terminated_input_descriptors(&self, ptr: u32) -> VmResult<Vec<i32>> {
        let base = Self::memory_addr(ptr) as usize;
        let mut descriptors = Vec::new();
        for index in 0..16usize {
            let offset = base.saturating_add(index.saturating_mul(4));
            let bytes = self
                .memory
                .get(offset..offset.saturating_add(4))
                .ok_or_else(|| {
                    VmError::Runtime(format!(
                        "Sys80_1B descriptor array 0x{ptr:08X} is outside VM memory"
                    ))
                })?;
            let value = i32::from_le_bytes(bytes.try_into().expect("four-byte descriptor"));
            if value == 0 {
                return Ok(descriptors);
            }
            descriptors.push(value);
        }
        Err(VmError::Runtime(
            "Sys80_1B descriptor array exceeds the target limit of 15 entries".into(),
        ))
    }

    fn input_call_frame(&mut self, opcode: NativeOpcode) -> VmResult<NativeCallFrame> {
        let args = self.take_dispatch_call_frame(opcode.group, opcode.id)?;
        Ok(NativeCallFrame::new(opcode, args))
    }
}
