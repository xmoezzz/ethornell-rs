//! Compact BP operations recovered from the Houkago Cinderella 2 FD executable.
//! Dispatch table 0x559710; signed-LEB fetch 0x44E6F0. These are VM operations,
//! independent of launcher-mode queries and game-specific startup policy.
use crate::{BpInstruction, BpOperand, FRAME_SEGMENT, Value, Vm, VmError, VmResult};

pub(super) fn immediate(ins: &BpInstruction, index: usize) -> VmResult<i32> {
    match ins.operands.get(index) {
        Some(BpOperand::U8(n)) => Ok(i32::from(*n)),
        Some(BpOperand::U16(n)) => Ok(i32::from(*n)),
        Some(BpOperand::U32(n)) => Ok(*n as i32),
        Some(BpOperand::I32(n)) => Ok(*n),
        _ => Err(VmError::Runtime(format!(
            "missing BP immediate at {:#x}",
            ins.offset
        ))),
    }
}

pub(super) fn inline_target(ins: &BpInstruction) -> VmResult<u32> {
    match ins.operands.get(1) {
        Some(BpOperand::Offset(target)) => Ok(*target),
        _ => Err(VmError::Runtime(format!(
            "missing BP inline branch target at {:#x}",
            ins.offset
        ))),
    }
}

pub(super) fn width(selector: u32) -> u8 {
    // Native handlers use dword only for 2, word only for 1, byte otherwise.
    match selector {
        2 => 2,
        1 => 1,
        _ => 0,
    }
}

fn shift(value: i32, amount: i8) -> i32 {
    if amount >= 0 {
        value.wrapping_shl(amount as u32)
    } else {
        value.wrapping_shr(-(amount as i32) as u32)
    }
}

fn divide(value: i32, divisor: i32, zero_is_sentinel: bool) -> VmResult<i32> {
    if divisor == 0 && zero_is_sentinel {
        return Ok(-1);
    }
    value
        .checked_div(divisor)
        .ok_or_else(|| VmError::Runtime("compact BP integer division fault".into()))
}

impl Vm {
    fn compact_frame_address(&self, packed: u32) -> VmResult<u32> {
        self.mem_ptr
            .checked_sub(packed & 0x3fff)
            .map(|offset| FRAME_SEGMENT | offset)
            .ok_or_else(|| VmError::Runtime("compact BP frame displacement underflow".into()))
    }

    pub(super) fn execute_compact_bp(&mut self, ins: &BpInstruction) -> VmResult<Option<u32>> {
        let code = ins.opcode.code();
        match code {
            0x12 => {
                let delta = immediate(ins, 0)?;
                let next = self
                    .mem_ptr
                    .checked_add_signed(delta)
                    .filter(|next| {
                        *next >= self.thread.frame_region_base()
                            && *next <= self.data_region_limit()
                    })
                    .ok_or_else(|| {
                        VmError::Runtime(
                            "compact BP frame adjustment outside owned data region".into(),
                        )
                    })?;
                // Native 0x488370 directly changes SP without initializing memory.
                // The portable runtime deliberately initializes newly exposed
                // frame bytes, consistently with store_base; malformed changes
                // fail before touching SP or another thread's reservation.
                if next > self.mem_ptr {
                    let ptr = FRAME_SEGMENT | self.mem_ptr;
                    let size = (next - self.mem_ptr) as usize;
                    let range = self.resolve_write_range(ptr, size)?;
                    self.memory[range].fill(0);
                    self.clear_shadow_values(ptr, size);
                }
                self.mem_ptr = next;
            }
            0x0f => {
                let packed = immediate(ins, 0)? as u32;
                let ptr = self.compact_frame_address(packed)?;
                let value = self.pop_value()?;
                self.write_value(ptr, width(packed >> 14), &value)?;
            }
            0x18 => {
                let packed = immediate(ins, 0)? as u32;
                let value = self.read_value(packed & 0x3fff_ffff, width(packed >> 30))?;
                self.push_value(value);
            }
            0x19..=0x1e => {
                let packed = immediate(ins, 0)? as u32;
                let ptr = self.compact_frame_address(packed)?;
                let value = self.read_value(ptr, width(packed >> 14))?;
                let result = match code {
                    0x19 => value,
                    0x1a => Value::Int(value.as_i32().wrapping_add(immediate(ins, 1)?)),
                    0x1b => {
                        let addend = self.pop_int()?;
                        Value::Int(
                            addend.wrapping_add(immediate(ins, 1)?.wrapping_mul(value.as_i32())),
                        )
                    }
                    0x1c => Value::Int(shift(value.as_i32(), immediate(ins, 1)? as i8)),
                    0x1d => Value::Int(self.pop_int()?.wrapping_mul(value.as_i32())),
                    0x1e => Value::Int(divide(self.pop_int()?, value.as_i32(), false)?),
                    _ => unreachable!(),
                };
                self.push_value(result);
            }
            0x1f => {
                let packed = immediate(ins, 0)?;
                let ptr = self.pop_ptr()?.wrapping_add((packed >> 2) as u32);
                let value = self.read_value(ptr, width(packed as u32 & 3))?;
                self.push_value(value);
            }
            0x2c..=0x2f => {
                let factor = immediate(ins, 0)?;
                let right = self.pop_int()?;
                let value = match code {
                    0x2c => right.wrapping_add(factor),
                    0x2d => right.wrapping_mul(factor),
                    0x2e => self.pop_int()?.wrapping_add(factor.wrapping_mul(right)),
                    0x2f => divide(right, factor, true)?,
                    _ => unreachable!(),
                };
                self.push_value(Value::Int(value));
            }
            0x3b => {
                let mode = immediate(ins, 0)?;
                let target = inline_target(ins)?;
                let right = self.pop_int()?;
                let left = self.pop_int()?;
                let predicate = match mode & 15 {
                    0 => left == right,
                    1 => left != right,
                    2 => left < right,
                    3 => left <= right,
                    4 => left >= right,
                    5 => left > right,
                    6 => left & right != 0,
                    7 => left | right != 0,
                    8 => left != 0 && right != 0,
                    9 => left != 0 || right != 0,
                    _ => false,
                };
                if predicate != (mode & 0x80 == 0) {
                    return Ok(Some(target));
                }
            }
            0x3c => {
                let value = self.pop_int()?;
                self.push_value(Value::Int(shift(value, immediate(ins, 0)? as i8)));
            }
            _ => return Err(VmError::UnsupportedInstruction(code)),
        }
        Ok(None)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{TraceApi, VmRunOptions};
    use ethornell_script::parse_bp_program;

    fn exec(vm: &mut Vm, bytes: &[u8]) -> VmResult<Option<u32>> {
        let program = parse_bp_program(None, bytes);
        vm.execute_compact_bp(&program.instructions[0])
    }

    #[test]
    fn compact_bp_frame_and_tagged_memory() {
        let mut vm = Vm::new();
        exec(&mut vm, &[0x12, 0xf0, 0x21]).unwrap();
        assert_eq!(vm.mem_ptr, 0x10f0);
        vm.push_value(Value::Ptr(crate::CODE_SEGMENT | 0x1234));
        exec(&mut vm, &[0x0f, 0xec, 0x90]).unwrap();
        assert!(vm.stack.is_empty());
        exec(&mut vm, &[0x19, 0xec, 0x90]).unwrap();
        assert_eq!(vm.pop_int().unwrap() as u32, crate::CODE_SEGMENT | 0x1234);
        vm.push_value(Value::Str("host string".into()));
        exec(&mut vm, &[0x0f, 0xec, 0x90]).unwrap();
        exec(&mut vm, &[0x19, 0xec, 0x90]).unwrap();
        assert_eq!(vm.pop_value().unwrap(), Value::Str("host string".into()));
        vm.write_int(FRAME_SEGMENT | 4, 0, 0x81).unwrap();
        exec(&mut vm, &[0x19, 0xec, 0x10]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), -127);
        // -0x10f0, restoring the initial frame instead of leaking it.
        exec(&mut vm, &[0x12, 0x90, 0x5e]).unwrap();
        assert_eq!(vm.mem_ptr, 0);
        assert!(exec(&mut vm, &[0x12, 0x7f]).is_err());
        assert_eq!(vm.mem_ptr, 0);
    }

    #[test]
    fn compact_bp_immediate_arithmetic_and_faults() {
        let mut vm = Vm::new();
        vm.push_value(Value::Int(i32::MAX));
        exec(&mut vm, &[0x2c, 1]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), i32::MIN);
        vm.push_value(Value::Int(5));
        vm.push_value(Value::Int(7));
        exec(&mut vm, &[0x2e, 0x7d]).unwrap(); // 5 + (-3)*7
        assert_eq!(vm.pop_int().unwrap(), -16);
        vm.push_value(Value::Int(-8));
        exec(&mut vm, &[0x3c, 0xfe]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), -2);
        vm.push_value(Value::Int(8));
        exec(&mut vm, &[0x2f, 0]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), -1);
        vm.push_value(Value::Int(i32::MIN));
        assert!(exec(&mut vm, &[0x2f, 0x7f]).is_err());
    }

    #[test]
    fn compact_bp_inline_branch_does_not_pop_a_destination() {
        // jc starts at offset 2 and targets 8. It only consumes the condition.
        let program = parse_bp_program(None, &[0, 1, 0x15, 8, 6, 0, 0, 99, 0, 42, 0x17]);
        let mut vm = Vm::new();
        vm.run(&program, &mut TraceApi, &VmRunOptions::default());
        assert_eq!(vm.stack, vec![Value::Int(42)]);
        let branch = parse_bp_program(None, &[0x3b, 0x82, 6, 0]);
        vm.stack = vec![Value::Int(2), Value::Int(3)];
        assert_eq!(
            vm.execute_compact_bp(&branch.instructions[0]).unwrap(),
            Some(6)
        );
        assert!(vm.stack.is_empty());
        vm.stack = vec![Value::Int(3), Value::Int(2)];
        assert_eq!(
            vm.execute_compact_bp(&branch.instructions[0]).unwrap(),
            None
        );
    }

    #[test]
    fn compact_bp_unknown_instruction_stops_without_skipping_bytes() {
        let mut vm = Vm::new();
        let program = parse_bp_program(None, &[0x07, 0x00, 42, 0x17]);
        let report = vm.run(&program, &mut TraceApi, &VmRunOptions::default());
        assert_eq!(report.stop_reason, crate::VmStopReason::UnknownOpcode);
        assert_eq!(vm.pc, 0);
        assert!(vm.stack.is_empty());
        // The legacy conditional form still consumes target + condition.
        let program = parse_bp_program(None, &[0, 1, 0, 10, 0x15, 0, 0, 99, 0, 88, 0, 42, 0x17]);
        let mut legacy = Vm::new();
        legacy.run(&program, &mut TraceApi, &VmRunOptions::default());
        assert_eq!(legacy.stack, vec![Value::Int(42)]);
    }

    #[test]
    fn compact_bp_pointer_load_and_owned_frame_bounds() {
        let mut vm = Vm::new();
        vm.write_int(64, 1, 0xff80).unwrap();
        vm.push_value(Value::Ptr(68));
        exec(&mut vm, &[0x1f, 0x71]).unwrap(); // (-4 << 2) | word
        assert_eq!(vm.pop_int().unwrap(), -128);
        vm.start(&parse_bp_program(None, &[0x17]));
        vm.start_child_thread(16, 0x100, 0x100, 0, false).unwrap();
        let limit = vm.data_region_limit();
        vm.mem_ptr = limit - 1;
        vm.write_int(FRAME_SEGMENT | limit, 0, 0xab).unwrap();
        assert!(exec(&mut vm, &[0x12, 2]).is_err());
        assert_eq!(vm.mem_ptr, limit - 1);
        assert_eq!(vm.read_int(FRAME_SEGMENT | limit, 0).unwrap(), 0xab);
    }
}
