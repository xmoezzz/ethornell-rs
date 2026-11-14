//! Fused BP operations in Triple Pairing BGI 1.687 (x64 table 1401CBB70).
//! D4..DF use full-width frame slots, unlike E3..E7's packed loads. For
//! F0..F5/F7/FA/FB the packed width describes the final store/load, while
//! the frame index itself is always read as a dword. All address arithmetic
//! wraps as native u32; dereferences still go through the VM's owned regions.
use crate::compact_bp::{immediate, width};
use crate::modern_bp::predicate;
use crate::{BpInstruction, BpOperand, Value, Vm, VmError, VmResult};

fn target(ins: &BpInstruction, index: usize) -> VmResult<u32> {
    match ins.operands.get(index) {
        Some(BpOperand::Offset(value)) => Ok(*value),
        _ => Err(VmError::Runtime("missing fused BP branch target".into())),
    }
}

impl Vm {
    fn fused_frame_value(&mut self, displacement: i32) -> VmResult<i32> {
        let value = self.read_value(self.modern_frame_offset(displacement as u32)?, 2)?;
        self.value_as_numeric_operand(value)
    }

    fn fused_indirect(&mut self, frame: i32, offset: i32) -> VmResult<u32> {
        Ok((self.fused_frame_value(frame)? as u32).wrapping_add(offset as u32))
    }

    pub(super) fn execute_fused_bp(&mut self, ins: &BpInstruction) -> VmResult<Option<u32>> {
        let op = ins.opcode.code();
        let arg = |i| immediate(ins, i);
        match op {
            0xd4 => {
                let value = self.read_value(self.modern_frame_offset(arg(0)? as u32)?, 2)?;
                self.push_value(value);
            }
            0xd5 | 0xd8 => {
                let field = self.fused_frame_value(arg(0)?)?;
                let inline = arg(1)?;
                let value = if op == 0xd8 {
                    self.pop_int()?.wrapping_add(field.wrapping_mul(inline))
                } else {
                    field.wrapping_add(inline)
                };
                self.push_value(Value::Int(value));
            }
            0xd6 => {
                let pointer = self.fused_indirect(arg(0)?, arg(2)?)?;
                let value = self.read_value(pointer, width(arg(1)? as u32))?;
                self.push_value(value);
            }
            0xd7 => {
                let pointer = self.fused_indirect(arg(0)?, arg(1)?)?;
                self.write_int(pointer, width(arg(2)? as u32), arg(3)? as u32)?;
            }
            0xda..=0xdd => {
                let indirect = matches!(op, 0xdb | 0xdd);
                let branch = op >= 0xdc;
                let mode = arg(0)?;
                let base = if branch { 2 } else { 1 };
                let left = if indirect {
                    let pointer = self.fused_indirect(arg(base)?, arg(base + 2)?)?;
                    let value = self.read_value(pointer, width(arg(base + 1)? as u32))?;
                    self.value_as_numeric_operand(value)?
                } else {
                    self.fused_frame_value(arg(base)?)?
                };
                let right = arg(base + if indirect { 3 } else { 1 })?;
                let value = predicate(if branch { mode & 15 } else { mode }, left, right);
                if branch {
                    if (value != 0) == (mode & 0x80 != 0) {
                        return Ok(Some(target(ins, 1)?));
                    }
                } else {
                    self.push_value(Value::Int(value));
                }
            }
            0xde | 0xdf => {
                let packed = arg(1)? as u32;
                let left = if op == 0xde {
                    self.fused_frame_value((packed & 0x7fff) as i32)?
                } else {
                    let pointer = self.fused_indirect((packed & 0x7fff) as i32, arg(3)?)?;
                    let value = self.read_value(pointer, width(arg(2)? as u32))?;
                    self.value_as_numeric_operand(value)?
                };
                if (left != 0) == (packed & 0x8000 != 0) {
                    return Ok(Some(target(ins, 0)?));
                }
            }
            0xe2 => {
                for index in 1..=arg(0)? as usize + 1 {
                    let packed = arg(index)? as u32;
                    let pointer = self.modern_frame_offset(packed & 0x3fff)?;
                    let value = self.pop_value()?;
                    self.write_value(pointer, width(packed >> 14), &value)?;
                }
            }
            0xe8 => {
                let packed = arg(0)? as u32;
                let no = self.pop_value()?;
                let yes = self.pop_value()?;
                let value = if self.pop_int()? != 0 { yes } else { no };
                self.write_value(
                    self.modern_frame_offset(packed & 0x3fff)?,
                    width(packed >> 14),
                    &value,
                )?;
            }
            0xe9 | 0xea => {
                let packed = arg(0)? as u32;
                let pointer = self.fused_indirect((packed & 0x3fff) as i32, arg(1)?)?;
                let value = if op == 0xe9 {
                    self.pop_value()?
                } else {
                    Value::Int(arg(2)?)
                };
                self.write_value(pointer, width(packed >> 14), &value)?;
            }
            0xf0..=0xf2 | 0xf4..=0xf5 => {
                let (destination, packed, value) = match op {
                    0xf0 => (
                        arg(0)? as u32,
                        arg(1)? as u32,
                        self.fused_frame_value(arg(1)? & 0x3fff)?
                            .wrapping_add(arg(2)?),
                    ),
                    0xf1 => (
                        self.modern_frame_offset(arg(0)? as u32)?,
                        arg(1)? as u32,
                        self.fused_frame_value(arg(1)? & 0x3fff)?
                            .wrapping_add(arg(2)?),
                    ),
                    0xf2 => (
                        self.fused_indirect(arg(0)?, arg(1)?)?,
                        arg(2)? as u32,
                        self.fused_frame_value(arg(2)? & 0x3fff)?
                            .wrapping_add(arg(3)?),
                    ),
                    0xf4 => (
                        self.modern_frame_offset(arg(0)? as u32)?,
                        arg(2)? as u32,
                        arg(1)?.wrapping_add(
                            self.fused_frame_value(arg(2)? & 0x3fff)?
                                .wrapping_mul(arg(3)? as i16 as i32),
                        ),
                    ),
                    0xf5 => (
                        self.modern_frame_offset(arg(0)? as u32)?,
                        arg(3)? as u32,
                        self.fused_frame_value(arg(1)?)?
                            .wrapping_add(arg(2)? as i16 as i32)
                            .wrapping_add(
                                self.fused_frame_value(arg(3)? & 0x3fff)?
                                    .wrapping_mul(arg(4)? as i16 as i32),
                            ),
                    ),
                    _ => unreachable!(),
                };
                self.write_int(destination, width(packed >> 14), value as u32)?;
            }
            0xfa | 0xfb => {
                let base = if op == 0xfa {
                    arg(0)? as u32
                } else {
                    self.modern_frame_offset(arg(0)? as u32)?
                };
                let packed = arg(1)? as u32;
                let index = self.fused_frame_value((packed & 0x3fff) as i32)?;
                let pointer = base.wrapping_add(index.wrapping_mul(arg(2)? as i16 as i32) as u32);
                // This is a load, not arithmetic on the loaded word. Frame
                // arrays can contain shadow strings/program handles (e.g.
                // syswnd's module-name table); coercing them to i32 loses
                // their value and turns the following LoadProgram into "".
                let value = self.read_value(pointer, width(packed >> 14))?;
                self.push_value(value);
            }
            0xf7..=0xf9 => {
                let value = match op {
                    0xf7 => {
                        let packed = arg(0)? as u32;
                        let pointer = self.fused_indirect((packed & 0x3fff) as i32, arg(1)?)?;
                        let field = self.read_value(pointer, width(packed >> 14))?;
                        let field = self.value_as_numeric_operand(field)?;
                        self.pop_int()?
                            .wrapping_add(field.wrapping_mul(arg(2)? as i16 as i32))
                    }
                    0xf8 => arg(0)?.wrapping_add(
                        self.fused_frame_value(arg(1)?)?
                            .wrapping_mul(arg(2)? as i16 as i32),
                    ),
                    0xf9 => self
                        .fused_frame_value(arg(0)?)?
                        .wrapping_add(arg(1)? as i16 as i32)
                        .wrapping_add(
                            self.fused_frame_value(arg(2)?)?
                                .wrapping_mul(arg(3)? as i16 as i32),
                        ),
                    _ => unreachable!(),
                };
                self.push_value(Value::Int(value));
            }
            _ => return Err(VmError::UnsupportedInstruction(op)),
        }
        Ok(None)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_script::parse_bp_program;

    #[test]
    fn fused_bp_indexed_loads_preserve_module_names_and_program_handles() {
        let mut vm = Vm::new();
        vm.mem_ptr = 128;
        vm.write_int(crate::FRAME_SEGMENT | 124, 2, 1).unwrap();
        let module = Value::Str("settings._bp".into());
        vm.write_value(260, 2, &module).unwrap();
        vm.write_value(crate::FRAME_SEGMENT | 68, 2, &module).unwrap();
        for bytes in [
            &[0xfa, 0, 1, 0, 0, 4, 0x80, 4, 0][..],
            &[0xfb, 64, 0, 4, 0x80, 4, 0][..],
        ] {
            let program = parse_bp_program(None, bytes);
            vm.execute_fused_bp(&program.instructions[0]).unwrap();
            assert_eq!(vm.pop_string_lossy().unwrap(), "settings._bp");
        }
        let handle = Value::Program(std::sync::Arc::new(parse_bp_program(None, &[0x17])));
        vm.write_value(260, 2, &handle).unwrap();
        let program = parse_bp_program(None, &[0xfa, 0, 1, 0, 0, 4, 0x80, 4, 0]);
        vm.execute_fused_bp(&program.instructions[0]).unwrap();
        assert_eq!(vm.pop_value().unwrap(), handle);
        // Arithmetic needs the guest address of a shadow string, rather than
        // Value::as_i32's zero. Exercise the corresponding frame+0 store.
        vm.write_value(crate::FRAME_SEGMENT | 124, 2, &module).unwrap();
        let program = parse_bp_program(None, &[0xf0, 0, 1, 0, 0, 4, 0x80, 0]);
        vm.execute_fused_bp(&program.instructions[0]).unwrap();
        let pointer = vm.read_int(256, 2).unwrap();
        assert_ne!(pointer, 0);
        assert_eq!(vm.read_shadowed_c_string(pointer).unwrap(), "settings._bp");
    }

    #[test]
    fn fused_bp_covers_native_table_operations_and_operand_boundaries() {
        // Each fixture is one original encoding, followed by RET. This also
        // catches swallowed opcodes and wrong signed-LEB/word boundaries.
        let cases: &[(u8, &[u8], &[i32], Option<i32>, Option<(u32, i32)>, bool)] = &[
            (0xd4, &[4, 0], &[], Some(3), None, false),
            (0xd5, &[4, 0, 1], &[], Some(4), None, false),
            (0xd6, &[8, 0, 2], &[], Some(-2), None, false),
            (0xd7, &[8, 0, 0, 0, 22], &[], None, Some((256, 5)), false),
            (0xd8, &[4, 0, 2], &[5], Some(11), None, false),
            (0xda, &[0, 4, 0, 3], &[], Some(1), None, false),
            (0xdb, &[0, 8, 0, 2, 0x7e], &[], Some(1), None, false),
            (0xdc, &[0x80, 100, 0, 4, 0, 3], &[], None, None, true),
            (0xdd, &[0x80, 100, 0, 8, 0, 2, 0x7e], &[], None, None, true),
            (0xde, &[100, 0, 4, 0x80], &[], None, None, true),
            (0xdf, &[100, 0, 8, 0x80, 2], &[], None, None, true),
            (
                0xe2,
                &[0, 16, 0x80],
                &[5],
                None,
                Some((crate::FRAME_SEGMENT | 112, 5)),
                false,
            ),
            (
                0xe8,
                &[16, 0x80],
                &[1, 11, 22],
                None,
                Some((crate::FRAME_SEGMENT | 112, 11)),
                false,
            ),
            (0xe9, &[8, 0x80, 0, 0], &[7], None, Some((256, 7)), false),
            (0xea, &[8, 0x80, 0, 0, 7], &[], None, Some((256, 7)), false),
            (
                0xf0,
                &[0, 1, 0, 0, 4, 0x80, 1],
                &[],
                None,
                Some((256, 4)),
                false,
            ),
            (
                0xf1,
                &[16, 0, 4, 0x80, 1],
                &[],
                None,
                Some((crate::FRAME_SEGMENT | 112, 4)),
                false,
            ),
            (
                0xf2,
                &[8, 0, 0, 0, 4, 0x80, 1],
                &[],
                None,
                Some((256, 4)),
                false,
            ),
            (
                0xf4,
                &[16, 0, 5, 0, 0, 0, 4, 0x80, 2, 0],
                &[],
                None,
                Some((crate::FRAME_SEGMENT | 112, 11)),
                false,
            ),
            (
                0xf5,
                &[16, 0, 12, 0, 255, 255, 4, 0x80, 2, 0],
                &[],
                None,
                Some((crate::FRAME_SEGMENT | 112, 12)),
                false,
            ),
            (0xf7, &[8, 0x80, 0, 0, 2, 0], &[5], Some(1), None, false),
            (0xf8, &[5, 0, 0, 0, 4, 0, 2, 0], &[], Some(11), None, false),
            (
                0xf9,
                &[12, 0, 255, 255, 4, 0, 2, 0],
                &[],
                Some(12),
                None,
                false,
            ),
            (
                0xfa,
                &[0, 1, 0, 0, 4, 0x80, 4, 0],
                &[],
                Some(99),
                None,
                false,
            ),
            (0xfb, &[28, 0, 4, 0x80, 4, 0], &[], Some(12), None, false),
        ];
        for &(op, payload, stack, result, store, branch) in cases {
            let mut bytes = vec![op];
            bytes.extend_from_slice(payload);
            bytes.push(0x17);
            let p = parse_bp_program(None, &bytes);
            assert_eq!(p.instructions[0].raw.len(), payload.len() + 1, "{op:02x}");
            assert_eq!(p.instructions[1].opcode.code(), 0x17, "{op:02x}");
            let mut vm = Vm::new();
            vm.mem_ptr = 128;
            for (p, v) in [(124, 3), (120, 256), (116, 7), (112, 12)] {
                vm.write_int(crate::FRAME_SEGMENT | p, 2, v).unwrap();
            }
            vm.write_int(256, 2, (-2i32) as u32).unwrap();
            vm.write_int(268, 2, 99).unwrap();
            vm.stack = stack.iter().copied().map(Value::Int).collect();
            assert_eq!(
                vm.execute_fused_bp(&p.instructions[0]).unwrap(),
                branch.then_some(100),
                "{op:02x}"
            );
            assert_eq!(
                vm.stack,
                result.map(Value::Int).into_iter().collect::<Vec<_>>(),
                "{op:02x}"
            );
            if let Some((at, value)) = store {
                assert_eq!(vm.read_int(at, 2).unwrap() as i32, value, "{op:02x}");
            }
        }
    }

    #[test]
    fn fused_bp_preserves_signed_indirection_and_branch_inversion() {
        let mut vm = Vm::new();
        vm.mem_ptr = 32;
        vm.write_int(crate::FRAME_SEGMENT | 28, 2, 256).unwrap();
        vm.write_int(254, 1, 0xff80).unwrap();
        let p = parse_bp_program(None, &[0xdb, 2, 4, 0, 0x79, 0]); // width 1, offset -2; -128 < 0
        vm.execute_fused_bp(&p.instructions[0]).unwrap();
        assert_eq!(vm.pop_int().unwrap(), 1);
        for mode in [0u8, 0x80] {
            let p = parse_bp_program(None, &[0xdc, mode, 7, 0, 4, 0, 0]);
            assert_eq!(
                vm.execute_fused_bp(&p.instructions[0]).unwrap(),
                (mode == 0).then_some(7)
            );
        }
        let p = parse_bp_program(None, &[0xd6, 255, 255, 2]);
        assert!(vm.execute_fused_bp(&p.instructions[0]).is_err());
    }
}
