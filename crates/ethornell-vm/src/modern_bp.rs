//! Additional BP operations in the 2024 x86 and later x64 interpreters.
//! Verified tables: Lovepical 0x553660; Dokai 0x1401C4980. Native fetch
//! helpers and assembly are checked as well as Hex-Rays output: inferred
//! char arguments in the latter can hide pointer masks and upper bits.
use crate::compact_bp::{immediate, width};
use crate::extended_opcodes::float_to_fixed;
use crate::{BpInstruction, BpOperand, Value, Vm, VmError, VmResult};

fn offset(ins: &BpInstruction, index: usize) -> VmResult<u32> {
    match ins.operands.get(index) {
        Some(BpOperand::Offset(n)) => Ok(*n),
        _ => Err(VmError::Runtime("missing BP opcode-relative target".into())),
    }
}

fn predicate(mode: i32, left: i32, right: i32) -> i32 {
    match mode {
        0 => i32::from(left == right),
        1 => i32::from(left != right),
        2 => i32::from(left < right),
        3 => i32::from(left <= right),
        4 => i32::from(left >= right),
        5 => i32::from(left > right),
        6 => left & right,
        7 => left | right,
        8 => i32::from(left != 0 && right != 0),
        9 => i32::from(left != 0 || right != 0),
        _ => 0,
    }
}

impl Vm {
    pub(super) fn execute_modern_bp(&mut self, ins: &BpInstruction) -> VmResult<Option<u32>> {
        match ins.opcode.code() {
            0x03 => {
                let control = immediate(ins, 0)? as u8;
                let count = if control < 0x80 {
                    usize::from(control) + 1
                } else if control <= 0x83 {
                    1
                } else {
                    0
                };
                for i in 1..=count {
                    self.push_value(Value::Int(immediate(ins, i)?));
                }
            }
            0x0d | 0x0e => {
                let frame = ins.opcode.code() == 0x0e;
                let base = usize::from(frame);
                let selector = immediate(ins, base)? as u32;
                let value = Value::Int(immediate(ins, base + 1)?);
                let ptr = if frame {
                    self.modern_frame_offset(immediate(ins, 0)? as u32)?
                } else {
                    self.pop_ptr()?
                };
                self.write_value(ptr, width(selector), &value)?;
            }
            0x13 => return Ok(Some(offset(ins, 0)?)),
            0x36 | 0x37 => {
                let branch = ins.opcode.code() == 0x37;
                let mode = immediate(ins, 0)?;
                let right = immediate(ins, if branch { 2 } else { 1 })?;
                let left = self.pop_int()?;
                let result = predicate(mode & 15, left, right);
                if branch {
                    if (result != 0) != (mode & 0x80 == 0) {
                        return Ok(Some(offset(ins, 1)?));
                    }
                } else {
                    self.push_value(Value::Int(result));
                }
            }
            0x3e | 0x3f => {
                let mode = immediate(ins, 0)? as u32;
                let right = if ins.opcode.code() == 0x3f {
                    immediate(ins, 1)?
                } else {
                    self.pop_int()?
                };
                let left = self.pop_int()?;
                let ptr = self.pop_ptr()?;
                let value = if mode & 0x20 != 0 {
                    predicate((mode & 31) as i32, left, right)
                } else {
                    match mode & 31 {
                        0 => left.wrapping_add(right),
                        1 => left.wrapping_sub(right),
                        2 => left.wrapping_mul(right),
                        3 if right != 0 => left.checked_div(right).ok_or_else(|| {
                            VmError::Runtime("BP integer division overflow".into())
                        })?,
                        4 if right != 0 => left.checked_rem(right).ok_or_else(|| {
                            VmError::Runtime("BP integer division overflow".into())
                        })?,
                        5 => left & right,
                        6 => left | right,
                        7 => left ^ right,
                        8 => left.wrapping_shl(right as u32),
                        9 => (left as u32).wrapping_shr(right as u32) as i32,
                        10 => left.wrapping_shr(right as u32),
                        _ => i32::MIN,
                    }
                };
                self.write_value(ptr, width(mode >> 6), &Value::Int(value))?;
            }
            0x45 => {
                let rz = f64::from(self.pop_int()?) * std::f64::consts::PI / 11_796_480.0;
                let ry = f64::from(self.pop_int()?) * std::f64::consts::PI / 11_796_480.0;
                let rx = f64::from(self.pop_int()?) * std::f64::consts::PI / 11_796_480.0;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let x = f64::from(self.read_int(source, 2)? as i32);
                let y = f64::from(self.read_int(source.wrapping_add(4), 2)? as i32);
                let z = f64::from(self.read_int(source.wrapping_add(8), 2)? as i32);
                let z1 = rx.cos() * z + rx.sin() * y;
                let y1 = rx.cos() * y - rx.sin() * z;
                let z2 = ry.cos() * z1 - ry.sin() * x;
                let x2 = ry.sin() * z1 + ry.cos() * x;
                for (i, value) in [
                    rz.cos() * x2 - rz.sin() * y1,
                    rz.cos() * y1 + rz.sin() * x2,
                    z2,
                ]
                .into_iter()
                .enumerate()
                {
                    let rounded = (value + 0.5).floor();
                    let native = if rounded >= i64::MIN as f64 && rounded < -(i64::MIN as f64) {
                        rounded as i64 as i32
                    } else {
                        0
                    };
                    self.write_int(destination.wrapping_add(i as u32 * 4), 2, native as u32)?;
                }
            }
            0x46 | 0x47 => {
                let progress = if ins.opcode.code() == 0x46 {
                    let total = self.pop_int()?;
                    let position = self.pop_int()?;
                    if total == 0 {
                        0.0
                    } else {
                        position as f32 / total as f32
                    }
                } else {
                    self.pop_int()? as f32 / 65_536.0
                };
                let scale = if ins.opcode.code() == 0x46 {
                    1.0
                } else {
                    1.0 / 65_536.0
                };
                let to = self.pop_int()? as f32 * scale;
                let from = self.pop_int()? as f32 * scale;
                self.push_value(Value::Int(float_to_fixed(
                    (to - from) * progress.clamp(0.0, 1.0) + from,
                )));
            }
            0x55 => {
                let power = f64::from(self.pop_int()? as f32 / 65_536.0);
                let base = f64::from(self.pop_int()? as f32 / 65_536.0);
                self.push_value(Value::Int(float_to_fixed(base.powf(power) as f32)));
            }
            0xe3 => {
                let count = immediate(ins, 0)? as usize + 1;
                for index in 1..=count {
                    let packed = immediate(ins, index)? as u32;
                    let ptr = self.modern_frame_offset(packed & 0x3fff)?;
                    let value = self.read_value(ptr, width(packed >> 14))?;
                    self.push_value(value);
                }
            }
            0xe4..=0xe7 => {
                let packed = immediate(ins, 0)? as u32;
                let selector = width(immediate(ins, 1)? as u32);
                let factor = immediate(ins, 2)?;
                let code = ins.opcode.code();
                let addend = if matches!(code, 0xe5 | 0xe7) {
                    self.pop_int()?
                } else {
                    0
                };
                let destination = if code >= 0xe6 {
                    Some(self.pop_ptr()?)
                } else {
                    None
                };
                let ptr = self.modern_frame_offset(packed & 0x3fff)?;
                let field = self.read_value(ptr, width(packed >> 14))?.as_i32();
                let value = if matches!(code, 0xe5 | 0xe7) {
                    addend.wrapping_add(factor.wrapping_mul(field))
                } else {
                    field.wrapping_add(factor)
                };
                if let Some(destination) = destination {
                    self.write_value(destination, selector, &Value::Int(value))?;
                } else {
                    let loaded = self.read_value(value as u32, selector)?;
                    self.push_value(loaded);
                }
            }
            0xec | 0xed => {
                let frame = ins.opcode.code() == 0xed;
                let size = immediate(ins, usize::from(frame))?;
                let size = usize::try_from(size)
                    .map_err(|_| VmError::Runtime("negative BP inline copy size".into()))?;
                let (source, destination) = if frame {
                    (
                        self.modern_frame_offset(immediate(ins, 0)? as u32)?,
                        self.pop_ptr()?,
                    )
                } else {
                    (self.pop_ptr()?, self.pop_ptr()?)
                };
                let source_range = self.resolve_range(source, size)?;
                let destination_range = self.resolve_write_range(destination, size)?;
                self.memory
                    .copy_within(source_range, destination_range.start);
                self.copy_shadow_values(source, destination, size);
            }
            code => return Err(VmError::UnsupportedInstruction(code)),
        }
        Ok(None)
    }

    fn modern_frame_offset(&self, displacement: u32) -> VmResult<u32> {
        self.mem_ptr
            .checked_sub(displacement)
            .map(|p| crate::FRAME_SEGMENT | p)
            .ok_or_else(|| VmError::Runtime("BP frame displacement underflow".into()))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{TraceApi, VmRunOptions, VmStopReason, FRAME_SEGMENT};
    use ethornell_script::parse_bp_program;

    fn exec(vm: &mut Vm, bytes: &[u8]) -> VmResult<Option<u32>> {
        let program = parse_bp_program(None, bytes);
        vm.execute_modern_bp(&program.instructions[0])
    }
    fn run(bytes: &[u8]) -> Vm {
        let mut vm = Vm::new();
        let report = vm.run(&parse_bp_program(None, bytes), &mut TraceApi, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Completed, "{report:?}");
        vm
    }

    #[test]
    fn modern_bp_large_local_regions_do_not_alias_and_survive_parking() {
        let mut vm = Vm::new();
        vm.set_modern_pointer_tags(true);
        let data = 0x2000_0000 | (crate::CODE_WINDOW_OFFSET - 4);
        let code = 0x1000_0000 | (crate::CODE_WINDOW_SIZE - 4);
        vm.write_int(data, 2, 0x12345678).unwrap();
        vm.write_int(code, 2, 0xaabbccdd).unwrap();
        assert_eq!(vm.read_int(data, 2).unwrap(), 0x12345678);
        assert_eq!(vm.read_int(code, 2).unwrap(), 0xaabbccdd);
        assert!(vm.write_int(data + 1, 2, 0).is_err());
        assert!(vm.read_int(code + 1, 2).is_err());
        let mut window = crate::async_program::take_local_window(&mut vm);
        let mut other = Vm::new();
        other.set_modern_pointer_tags(true);
        crate::async_program::exchange_saved_local(&mut other, &mut window);
        assert_eq!(other.read_int(data, 2).unwrap(), 0x12345678);
        assert_eq!(other.read_int(code, 2).unwrap(), 0xaabbccdd);
    }

    #[test]
    fn modern_bp_batch_literals_and_inline_frame_stores() {
        let vm = run(&[3, 2, 1, 0x7f, 0x80, 0x80, 0x80, 0x80, 8, 0x17]);
        assert_eq!(vm.stack, vec![Value::Int(1), Value::Int(-1), Value::Int(i32::MIN)]);
        let vm = run(&[3, 0x83, 0x44, 0x33, 0x22, 0x11, 0xff, 0xff, 0xff, 0xff, 0x17]);
        assert_eq!(vm.stack, vec![Value::Int(0x11223344)]);
        let mut vm = Vm::new();
        vm.mem_ptr = 16;
        exec(&mut vm, &[0x0e, 4, 0, 0x76]).unwrap(); // width 2, immediate -3
        assert_eq!(vm.read_int(FRAME_SEGMENT | 12, 2).unwrap() as i32, -3);
        exec(&mut vm, &[0xe3, 1, 4, 0x80, 8, 0x80]).unwrap();
        assert_eq!(vm.stack, vec![Value::Int(-3), Value::Int(0)]);
    }

    #[test]
    fn modern_bp_relative_and_immediate_predicate_branches() {
        let vm = run(&[0x13, 7, 0, 7, 7, 7, 7, 0, 42, 0x17]);
        assert_eq!(vm.stack, vec![Value::Int(42)]);
        let vm = run(&[0, 2, 0x37, 0x82, 7, 0, 3, 0, 99, 0, 42, 0x17]);
        assert_eq!(vm.stack, vec![Value::Int(42)]);
        let vm = run(&[0, 10, 0x36, 6, 6, 0x17]);
        assert_eq!(vm.stack, vec![Value::Int(2)]);
    }

    #[test]
    fn modern_bp_indexed_loads_and_stores_preserve_selected_width() {
        let mut vm = Vm::new(); vm.mem_ptr = 16;
        vm.write_int(FRAME_SEGMENT | 12, 2, 64).unwrap();
        vm.write_int(68, 1, 0xff80).unwrap();
        exec(&mut vm, &[0xe4, 4, 0x80, 0x11]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), -128);
        vm.write_int(FRAME_SEGMENT | 12, 2, 2).unwrap();
        vm.write_int(80, 2, 1234).unwrap();
        vm.push_value(Value::Ptr(64));
        exec(&mut vm, &[0xe5, 4, 0x80, 0x22]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), 1234);
        vm.push_value(Value::Ptr(96));
        vm.push_value(Value::Int(5));
        exec(&mut vm, &[0xe7, 4, 0x80, 0x0e]).unwrap(); // scale 3, dword
        assert_eq!(vm.read_int(96, 2).unwrap(), 11);
        vm.push_value(Value::Ptr(100));
        vm.push_value(Value::Int(5));
        exec(&mut vm, &[0x3f, 0x80, 3]).unwrap(); // add immediate, dword
        assert_eq!(vm.read_int(100, 2).unwrap(), 8);
        vm.push_value(Value::Ptr(104));
        vm.push_value(Value::Int(2));
        vm.push_value(Value::Int(3));
        exec(&mut vm, &[0x3e, 0xa2]).unwrap(); // less-than, dword
        assert_eq!(vm.read_int(104, 2).unwrap(), 1);
    }

    #[test]
    fn modern_bp_direct_calls_and_immediate_copies() {
        assert_eq!(run(&[0xee, 6, 0, 0x17, 7, 7, 0, 42, 0x17]).stack, vec![Value::Int(42)]);
        let mut vm = Vm::new(); vm.write_int(64, 2, 6).unwrap();
        let program = parse_bp_program(None, &[0xef, 64, 0, 0, 0x80, 0x17, 0, 42, 0x17]);
        let report = vm.run(&program, &mut TraceApi, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Completed, "{report:?}");
        assert_eq!(vm.stack, vec![Value::Int(42)]);
        vm.write_int(64, 2, 0x12345678).unwrap();
        vm.stack = vec![Value::Ptr(96), Value::Ptr(64)];
        exec(&mut vm, &[0xec, 4]).unwrap();
        assert_eq!(vm.read_int(96, 2).unwrap(), 0x12345678);
    }

    #[test]
    fn modern_bp_rotation_lerp_and_power_follow_native_scaling() {
        let mut vm = Vm::new();
        vm.write_int(64, 2, 0).unwrap(); vm.write_int(68, 2, 1).unwrap(); vm.write_int(72, 2, 0).unwrap();
        vm.stack = vec![Value::Ptr(96), Value::Ptr(64), Value::Int(90<<16), Value::Int(0), Value::Int(0)];
        exec(&mut vm, &[0x45]).unwrap();
        assert_eq!([96,100,104].map(|p| vm.read_int(p,2).unwrap()), [0,0,1]);
        vm.stack = [2,6,1,4].map(Value::Int).into(); exec(&mut vm, &[0x46]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), 3<<16);
        vm.stack = [2<<16,6<<16,1<<14].map(Value::Int).into(); exec(&mut vm, &[0x47]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), 3<<16);
        vm.stack = [2<<16,3<<16].map(Value::Int).into(); exec(&mut vm, &[0x55]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), 8<<16);
    }

    #[test]
    fn modern_bp_pointer_tags_resolve_to_owned_code_and_frame_windows() {
        let mut vm = Vm::new(); vm.set_modern_pointer_tags(true);
        let program = parse_bp_program(None, &[0x04,0,0,0x05,4,0,0x17,b'x',0]);
        vm.run(&program, &mut TraceApi, &VmRunOptions::default());
        assert_eq!(vm.stack[0], Value::Ptr(0x20000000));
        assert_eq!(vm.stack[1], Value::Ptr(0x10000007));
        vm.write_int(0x20000010, 2, 99).unwrap();
        assert_eq!(vm.read_int(FRAME_SEGMENT | 16, 2).unwrap(), 99);
        assert_eq!(vm.read_c_string(0x10000007).unwrap(), "x");
    }
}
