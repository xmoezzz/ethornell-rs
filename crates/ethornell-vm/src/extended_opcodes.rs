use super::{Value, Vm, VmError, VmResult};

#[derive(Debug, Default)]
pub(super) struct ExtendedOpcodeState {
    memory_handle_mode: bool,
    clipboard_text: String,
    last_modal_list: Option<(i32, String)>,
    last_resource_transform: [i32; 7],
    last_resource_blend: [i32; 5],
    last_legacy_3d_call: Option<(u8, Vec<i32>)>,
    last_debug_inspection: Option<u8>,
}

impl Vm {
    pub(super) fn execute_qword_arithmetic(&mut self, opcode: u8) -> VmResult<()> {
        let right = self.pop_ptr()?;
        let left = self.pop_ptr()?;
        let destination = self.pop_ptr()?;
        let right = self.read_i64(right)?;
        let left = self.read_i64(left)?;
        let result = match opcode {
            0x50 => left.wrapping_add(right),
            0x51 => left.wrapping_sub(right),
            0x52 => left.wrapping_mul(right),
            0x53 if right != 0 => left.wrapping_div(right),
            0x54 if right != 0 => left.wrapping_rem(right),
            0x53 | 0x54 => -1,
            _ => return Err(VmError::UnsupportedInstruction(opcode)),
        };
        self.write_i64(destination, result)
    }

    /// 0x56/0x57 (sub_4747C0, sub_4747F0): the first value popped is the
    /// right operand.
    pub(super) fn execute_fixed_arithmetic(&mut self, opcode: u8) -> VmResult<i32> {
        let right = self.pop_int()?;
        let left = self.pop_int()?;
        Ok(match opcode {
            0x56 => fixed_mul(left, right),
            0x57 if right == 0 => 0,
            0x57 => float_to_fixed(fixed_to_float(left) / fixed_to_float(right)),
            _ => return Err(VmError::UnsupportedInstruction(opcode)),
        })
    }

    /// 0x58..=0x5F (sub_474890..sub_474B40): SSE helpers over four 16.16
    /// lanes. Pointers pop right operand first and the destination last.
    pub(super) fn execute_fixed_vector(&mut self, opcode: u8) -> VmResult<()> {
        let result = match opcode {
            0x58..=0x5b => {
                let right = self.pop_ptr()?;
                let left = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let (left, right) = (self.read_fixed4(left)?, self.read_fixed4(right)?);
                let lanes = std::array::from_fn(|lane| {
                    let (left, right) = (left[lane], right[lane]);
                    match opcode {
                        0x58 => left.wrapping_add(right),
                        0x59 => left.wrapping_sub(right),
                        0x5a => fixed_mul(left, right),
                        _ if right == 0 => 0,
                        _ => float_to_fixed(fixed_to_float(left) / fixed_to_float(right)),
                    }
                });
                (destination, lanes)
            }
            0x5d => {
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let [x, y, z, _] = self.read_fixed4(source)?.map(fixed_to_float);
                // sub_4558C0 sums z² + y² + x² and returns zero for a zero
                // vector; w becomes the length.
                let squared = z * z + y * y + x * x;
                let lanes = if squared > 0.0 {
                    let inverse = 1.0 / squared.sqrt();
                    [x * inverse, y * inverse, z * inverse, squared * inverse].map(float_to_fixed)
                } else {
                    [0; 4]
                };
                (destination, lanes)
            }
            0x5e | 0x5f => {
                let scalar = self.pop_int()?;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let source = self.read_fixed4(source)?;
                let lanes = match opcode {
                    0x5e => source.map(|lane| fixed_mul(lane, scalar)),
                    _ if scalar == 0 => [0; 4],
                    _ => source
                        .map(|lane| float_to_fixed(fixed_to_float(lane) / fixed_to_float(scalar))),
                };
                (destination, lanes)
            }
            _ => return Err(VmError::UnsupportedInstruction(opcode)),
        };
        let (destination, lanes) = result;
        for (index, lane) in lanes.into_iter().enumerate() {
            self.write_int(destination.wrapping_add(index as u32 * 4), 2, lane as u32)?;
        }
        Ok(())
    }

    fn read_fixed4(&self, pointer: u32) -> VmResult<[i32; 4]> {
        let mut lanes = [0; 4];
        for (index, lane) in lanes.iter_mut().enumerate() {
            *lane = self.read_int(pointer.wrapping_add(index as u32 * 4), 2)? as i32;
        }
        Ok(lanes)
    }

    pub(super) fn execute_quote_string(&mut self) -> VmResult<()> {
        let delimiter = char::from(self.pop_int()? as u8);
        let source = self.pop_string_lossy()?;
        let destination = self.pop_ptr()?;
        self.write_c_string(destination, &format!("{delimiter}{source}{delimiter}"))
    }

    pub(super) fn execute_set_memory_mode(&mut self) -> VmResult<()> {
        self.extended_opcodes.memory_handle_mode = self.pop_int()? != 0;
        Ok(())
    }

    pub(super) fn execute_modal_list(&mut self) -> VmResult<i32> {
        let text = self.pop_string_lossy()?;
        let owner = self.pop_int()?;
        self.extended_opcodes.last_modal_list = Some((owner, text));
        // This opcode is an engine diagnostics dialog. Windowless validation
        // models the native cancel result instead of opening a second window.
        Ok(0)
    }

    pub(super) fn execute_resource_transform(&mut self) -> VmResult<()> {
        let mut values = [0; 7];
        for value in &mut values {
            *value = self.pop_int()?;
        }
        self.extended_opcodes.last_resource_transform = values;
        Ok(())
    }

    pub(super) fn execute_clipboard_set(&mut self) -> VmResult<i32> {
        self.extended_opcodes.clipboard_text = self.pop_string_lossy()?;
        Ok(1)
    }

    pub(super) fn execute_resource_blend(&mut self) -> VmResult<()> {
        let mut values = [0; 5];
        for value in &mut values {
            *value = self.pop_int()?;
        }
        self.extended_opcodes.last_resource_blend = values;
        Ok(())
    }

    pub(super) fn execute_legacy_3d(&mut self, id: u8) -> VmResult<Option<Value>> {
        let Some((argc, returns_value)) = legacy_3d_abi(id) else {
            return Err(VmError::UnknownDispatch {
                group: 0xd0,
                id: u16::from(id),
            });
        };
        let mut args = Vec::with_capacity(argc);
        for _ in 0..argc {
            args.push(self.pop_value()?);
        }
        let values = args.iter().map(Value::as_i32).collect::<Vec<_>>();
        self.extended_opcodes.last_legacy_3d_call = Some((id, values.clone()));
        args.reverse();
        if let Some(result) = self.execute_logic(id, &args)? {
            return Ok(returns_value.then_some(Value::Int(result)));
        }

        // Every registered selector is ported; keep the stack balanced anyway.
        let result = 0;
        Ok(returns_value.then_some(Value::Int(result)))
    }

    pub(super) fn execute_debug_inspect(&mut self, id: u8) -> VmResult<()> {
        if !matches!(id, 0x00 | 0x40 | 0x80) {
            return Err(VmError::UnknownDispatch {
                group: 0xe0,
                id: u16::from(id),
            });
        }
        self.extended_opcodes.last_debug_inspection = Some(id);
        Ok(())
    }

    fn read_i64(&self, pointer: u32) -> VmResult<i64> {
        let range = self.resolve_range(pointer, 8)?;
        let mut bytes = [0; 8];
        bytes.copy_from_slice(&self.memory[range]);
        Ok(i64::from_le_bytes(bytes))
    }

    fn write_i64(&mut self, pointer: u32, value: i64) -> VmResult<()> {
        let range = self.resolve_write_range(pointer, 8)?;
        self.memory[range].copy_from_slice(&value.to_le_bytes());
        self.clear_shadow_values(pointer, 8);
        Ok(())
    }
}

fn fixed_mul(left: i32, right: i32) -> i32 {
    ((i64::from(left) * i64::from(right)) >> 16) as i32
}

fn fixed_to_float(value: i32) -> f32 {
    value as f32 * (1.0 / 65_536.0)
}

/// sub_455940: round to the nearest 16.16 value (cvtps2dq in the default
/// ties-to-even mode; the handlers' `and 0F3FFh` touches exception masks, not
/// the rounding bits) and snap a fraction within 0x40 of an integer to it.
/// Out-of-range and NaN lanes become the cvtps2dq indefinite value.
pub(crate) fn float_to_fixed(value: f32) -> i32 {
    let scaled = (value * 65_536.0).round_ties_even();
    let raw = if scaled >= -2_147_483_648.0 && scaled < 2_147_483_648.0 {
        scaled as i32
    } else {
        i32::MIN
    };
    let whole = raw & !0xffff;
    let fraction = raw.wrapping_sub(whole);
    if fraction > 0xffc0 {
        whole.wrapping_add(0x10000)
    } else if fraction < 0x40 {
        whole
    } else {
        raw
    }
}

fn legacy_3d_abi(id: u8) -> Option<(usize, bool)> {
    Some(match id {
        0x00 => (3, true),
        0x01 => (1, true),
        0x04 => (4, true),
        0x05 => (3, true),
        0x10 | 0x11 | 0x12 => (2, true),
        0x14 | 0x15 => (4, true),
        0x16 | 0x17 | 0x18 => (3, true),
        0x20 | 0x21 => (6, true),
        0x22 => (3, true),
        0x23 => (4, true),
        0x28 => (11, true),
        0x2c => (5, true),
        0x2d => (6, true),
        0x40 | 0x41 => (1, true),
        0x60 => (13, true),
        0x61 => (2, true),
        0x64 => (5, true),
        0x65 => (3, true),
        0x66 => (5, true),
        0x67 => (3, true),
        0x68 | 0x69 => (4, true),
        0x6a => (5, true),
        0x70 => (7, true),
        0x72 => (11, true),
        0x74 => (5, true),
        0x75 => (9, true),
        0x78 => (5, true),
        0x79 => (4, true),
        // sub_4787C0 pops five integers and one pointer.
        0x7a => (6, true),
        0x80 => (1, false),
        0x81 => (1, true),
        0x84 | 0x87 | 0x8c => (2, true),
        0x88 | 0x8a | 0x8d => (5, true),
        0x8e => (3, true),
        _ => return None,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn write_fixed4(vm: &mut Vm, pointer: u32, lanes: [i32; 4]) {
        for (index, lane) in lanes.into_iter().enumerate() {
            vm.write_int(pointer + index as u32 * 4, 2, lane as u32)
                .unwrap();
        }
    }

    #[test]
    fn fixed_arithmetic_pops_the_right_operand_first() {
        let mut vm = Vm::new();
        vm.stack.extend([Value::Int(3 << 16), Value::Int(0x8000)]);
        assert_eq!(vm.execute_fixed_arithmetic(0x56).unwrap(), 0x18000);
        vm.stack.extend([Value::Int(3 << 16), Value::Int(2 << 16)]);
        assert_eq!(vm.execute_fixed_arithmetic(0x57).unwrap(), 0x18000);
        vm.stack.extend([Value::Int(3 << 16), Value::Int(0)]);
        assert_eq!(vm.execute_fixed_arithmetic(0x57).unwrap(), 0);
    }

    #[test]
    fn fixed_vectors_follow_the_target_lane_rules() {
        let mut vm = Vm::new();
        let (dst, left, right) = (0x0800_0100, 0x0800_0110, 0x0800_0120);
        write_fixed4(&mut vm, left, [6 << 16, 1 << 16, -(4 << 16), 5]);
        write_fixed4(&mut vm, right, [2 << 16, 0, 2 << 16, 7]);

        vm.stack
            .extend([Value::Ptr(dst), Value::Ptr(left), Value::Ptr(right)]);
        vm.execute_fixed_vector(0x59).unwrap();
        assert_eq!(
            vm.read_fixed4(dst).unwrap(),
            [4 << 16, 1 << 16, -(6 << 16), -2]
        );

        vm.stack
            .extend([Value::Ptr(dst), Value::Ptr(left), Value::Ptr(right)]);
        vm.execute_fixed_vector(0x5b).unwrap();
        assert_eq!(
            vm.read_fixed4(dst).unwrap(),
            [3 << 16, 0, -(2 << 16), float_to_fixed(5.0 / 7.0)]
        );

        vm.stack
            .extend([Value::Ptr(dst), Value::Ptr(left), Value::Int(0x8000)]);
        vm.execute_fixed_vector(0x5e).unwrap();
        assert_eq!(
            vm.read_fixed4(dst).unwrap(),
            [3 << 16, 0x8000, -(2 << 16), 2]
        );

        vm.stack
            .extend([Value::Ptr(dst), Value::Ptr(left), Value::Int(0)]);
        vm.execute_fixed_vector(0x5f).unwrap();
        assert_eq!(vm.read_fixed4(dst).unwrap(), [0; 4]);
    }

    #[test]
    fn vec3_normalize_stores_the_length_in_w() {
        let mut vm = Vm::new();
        let (dst, source) = (0x0800_0100, 0x0800_0110);
        write_fixed4(&mut vm, source, [3 << 16, 0, 4 << 16, 9 << 16]);
        vm.stack.extend([Value::Ptr(dst), Value::Ptr(source)]);
        vm.execute_fixed_vector(0x5d).unwrap();
        assert_eq!(
            vm.read_fixed4(dst).unwrap(),
            [float_to_fixed(0.6), 0, float_to_fixed(0.8), 5 << 16]
        );

        write_fixed4(&mut vm, source, [0, 0, 0, 1 << 16]);
        vm.stack.extend([Value::Ptr(dst), Value::Ptr(source)]);
        vm.execute_fixed_vector(0x5d).unwrap();
        assert_eq!(vm.read_fixed4(dst).unwrap(), [0; 4]);
    }

    #[test]
    fn float_to_fixed_snaps_near_integers() {
        assert_eq!(float_to_fixed(1.0 + 0x3f as f32 / 65_536.0), 1 << 16);
        assert_eq!(float_to_fixed(2.0 - 0x3f as f32 / 65_536.0), 2 << 16);
        assert_eq!(float_to_fixed(1.5), 0x18000);
        assert_eq!(float_to_fixed(f32::NAN), i32::MIN);
    }

    #[test]
    fn qword_arithmetic_uses_native_pointer_operands() {
        let mut vm = Vm::new();
        vm.write_i64(0x0800_0100, 40).unwrap();
        vm.write_i64(0x0800_0108, 2).unwrap();
        vm.stack.extend([
            Value::Ptr(0x0800_0110),
            Value::Ptr(0x0800_0100),
            Value::Ptr(0x0800_0108),
        ]);
        vm.execute_qword_arithmetic(0x50).unwrap();
        assert_eq!(vm.read_i64(0x0800_0110).unwrap(), 42);
    }

    /// Pushes call-order arguments and runs one 0xD0 selector.
    fn d0(vm: &mut Vm, id: u8, args: &[Value]) -> Option<Value> {
        vm.stack.extend(args.iter().cloned());
        vm.execute_legacy_3d(id).unwrap()
    }

    #[test]
    fn grid_field_runs_through_the_script_stack() {
        let mut vm = Vm::new();
        let (out, tiles, pair, dirs, count) = (
            0x0800_0100,
            0x0800_0200,
            0x0800_0400,
            0x0800_0500,
            0x0800_0540,
        );
        // The first handle is 0 (dword_5669E4 starts at 0); kind != 0 fails.
        assert_eq!(
            d0(
                &mut vm,
                0x00,
                &[Value::Ptr(out), Value::Int(1), Value::Int(1)]
            ),
            Some(Value::Int(0))
        );
        assert_eq!(
            d0(
                &mut vm,
                0x00,
                &[Value::Ptr(out), Value::Int(0), Value::Int(1)]
            ),
            Some(Value::Int(1))
        );
        let grid = vm.read_int(out, 2).unwrap() as i32;
        assert_eq!(grid, 0);
        // A 3x2 map with a wall at (1,0).
        for index in 0..6u32 {
            for word in 0..4u32 {
                let value = if index == 1 && word == 2 { 2 } else { 0 };
                vm.write_int(tiles + index * 16 + word * 4, 2, value)
                    .unwrap();
            }
        }
        let int = Value::Int;
        assert_eq!(
            d0(
                &mut vm,
                0x04,
                &[int(grid), int(3), int(2), Value::Ptr(tiles)]
            ),
            Some(int(0))
        );
        assert_eq!(
            d0(&mut vm, 0x10, &[Value::Ptr(out), int(grid)]),
            Some(int(0))
        );
        let unit = vm.read_int(out, 2).unwrap() as i32;
        assert_eq!(unit, 0x2000_0000);
        assert_eq!(
            d0(&mut vm, 0x14, &[int(grid), int(unit), int(0), int(0)]),
            Some(int(0))
        );
        vm.write_int(pair, 2, 0).unwrap();
        vm.write_int(pair + 4, 2, 1).unwrap();
        assert_eq!(
            d0(&mut vm, 0x16, &[int(grid), int(unit), Value::Ptr(pair)]),
            Some(int(0))
        );
        assert_eq!(
            d0(
                &mut vm,
                0x20,
                &[int(grid), int(unit), int(1), int(0), int(-1), int(-1)]
            ),
            Some(int(0))
        );
        // Around the wall: down, right, right, up.
        assert_eq!(
            d0(
                &mut vm,
                0x21,
                &[
                    Value::Ptr(dirs),
                    Value::Ptr(count),
                    int(grid),
                    int(unit),
                    int(2),
                    int(0)
                ]
            ),
            Some(int(0))
        );
        assert_eq!(vm.read_int(count, 2).unwrap(), 4);
        let path: Vec<u32> = (0..4)
            .map(|i| vm.read_int(dirs + i * 4, 2).unwrap())
            .collect();
        assert_eq!(path, [3, 5, 5, 2]);
        // Unknown handle -> status 1, unknown unit -> 4.
        assert_eq!(d0(&mut vm, 0x12, &[int(7), int(unit)]), Some(int(1)));
        assert_eq!(d0(&mut vm, 0x12, &[int(grid), int(5)]), Some(int(4)));
        assert_eq!(d0(&mut vm, 0x01, &[int(grid)]), Some(int(1)));
        assert_eq!(d0(&mut vm, 0x01, &[int(grid)]), Some(int(0)));
    }

    #[test]
    fn evaluator_runs_through_the_script_stack() {
        let mut vm = Vm::new();
        let int = Value::Int;
        let (out, tiles, units, results, count) = (
            0x0800_0100,
            0x0800_0200,
            0x0800_1000,
            0x0800_4000,
            0x0800_4400,
        );
        d0(&mut vm, 0x00, &[Value::Ptr(out), int(0), int(1)]);
        let grid = vm.read_int(out, 2).unwrap() as i32;
        for i in 0..4 * 6 {
            vm.write_int(tiles + i * 4, 2, 0).unwrap();
        }
        d0(
            &mut vm,
            0x04,
            &[int(grid), int(6), int(1), Value::Ptr(tiles)],
        );
        let mut ids = Vec::new();
        for x in [0, 5] {
            d0(&mut vm, 0x10, &[Value::Ptr(out), int(grid)]);
            let id = vm.read_int(out, 2).unwrap() as i32;
            d0(&mut vm, 0x14, &[int(grid), int(id), int(x), int(0)]);
            ids.push(id);
        }
        // Two records of 525 words: teams 1 and 2, melee, 100 HP.
        for (n, &id) in ids.iter().enumerate() {
            let base = units + n as u32 * 2100;
            for word in 0..525u32 {
                vm.write_int(base + word * 4, 2, 0).unwrap();
            }
            for (word, value) in [
                (0, id),
                (1, 1 << n),
                (2, n as i32),
                (3, -1),
                (7, 100),
                (8, 100),
                (141, 1),
                (142, 2),
                (149, 1),
                (11, 1),
                (12, 10),
                (13, 1),
                (159, 1),
                (160, 1),
            ] {
                vm.write_int(base + word * 4, 2, value as u32).unwrap();
            }
        }
        // D0:80 pushes nothing; the first evaluator handle is 1.
        assert_eq!(d0(&mut vm, 0x80, &[Value::Ptr(out)]), None);
        let ai = vm.read_int(out, 2).unwrap() as i32;
        assert_eq!(ai, 1);
        assert_eq!(d0(&mut vm, 0x84, &[int(ai), int(0)]), Some(int(16)));
        assert_eq!(d0(&mut vm, 0x84, &[int(ai), int(2)]), Some(int(0)));
        assert_eq!(
            d0(
                &mut vm,
                0x88,
                &[int(ai), int(grid), int(2), Value::Ptr(units), int(1)]
            ),
            Some(int(0))
        );
        assert_eq!(d0(&mut vm, 0x87, &[Value::Ptr(out), int(ai)]), Some(int(0)));
        assert_eq!(vm.read_int(out, 2).unwrap(), 0);
        assert_eq!(
            d0(
                &mut vm,
                0x8A,
                &[
                    Value::Ptr(results),
                    Value::Ptr(count),
                    int(ai),
                    int(0),
                    int(3)
                ]
            ),
            Some(int(0))
        );
        assert_eq!(vm.read_int(count, 2).unwrap(), 3);
        // Best: walk 2 tiles toward the enemy at x=5; from there it could
        // strike next turn (1 + 15 at 1.5x, weighted 67%) and faces the way
        // it walked (+1).
        let best: Vec<i32> = (0..7)
            .map(|i| vm.read_int(results + i * 4, 2).unwrap() as i32)
            .collect();
        assert_eq!(best, [2, 0, 3, -1, -1, -1, 10 + 1]);
        assert_eq!(d0(&mut vm, 0x87, &[Value::Ptr(out), int(ai)]), Some(int(0)));
        assert_eq!(vm.read_int(out, 2).unwrap(), 0);
        assert_eq!(d0(&mut vm, 0x87, &[Value::Ptr(out), int(ai)]), Some(int(0)));
        assert_eq!(vm.read_int(out, 2).unwrap(), 0xFFFF_0000);
        // D0:8E copies the script part back, with the chosen target.
        assert_eq!(
            d0(&mut vm, 0x8E, &[Value::Ptr(out), int(ai), int(0)]),
            Some(int(0))
        );
        assert_eq!(vm.read_int(out + 3 * 4, 2).unwrap(), 1);
        assert_eq!(
            d0(&mut vm, 0x8E, &[Value::Ptr(out), int(ai), int(5)]),
            Some(int(17))
        );
        assert_eq!(d0(&mut vm, 0x81, &[int(ai)]), Some(int(0)));
        assert_eq!(d0(&mut vm, 0x81, &[int(ai)]), Some(int(1)));
    }

    #[test]
    fn space_manager_runs_through_the_script_stack() {
        let mut vm = Vm::new();
        let int = Value::Int;
        let fixed = |v: i32| int(v << 16);
        let (out, list, count) = (0x0800_0100, 0x0800_0200, 0x0800_0300);
        assert_eq!(d0(&mut vm, 0x40, &[Value::Ptr(out)]), Some(int(0)));
        let space = vm.read_int(out, 2).unwrap() as i32;
        assert_eq!(space, 1);
        // (h, i, x,y,z, dx,dy,dz, radius, speed, probe, mask, priority)
        for (i, x) in [(0, 0), (1, 5)] {
            let args = [
                int(space),
                int(i),
                fixed(x),
                fixed(0),
                fixed(0),
                fixed(1),
                fixed(0),
                fixed(0),
                fixed(1),
                fixed(3),
                fixed(1),
                int(1),
                int(0),
            ];
            assert_eq!(d0(&mut vm, 0x60, &args), Some(int(0)));
        }
        assert_eq!(
            d0(&mut vm, 0x65, &[Value::Ptr(out), int(space), int(1)]),
            Some(int(0))
        );
        assert_eq!(vm.read_int(out, 2).unwrap(), 5 << 16);
        // Body 0's next step (x = 3) with probe 1 reaches body 1 (radius 1)? 3+1+1 = 5: no.
        assert_eq!(
            d0(
                &mut vm,
                0x74,
                &[
                    Value::Ptr(list),
                    Value::Ptr(count),
                    int(space),
                    int(0),
                    int(0)
                ]
            ),
            Some(int(0))
        );
        assert_eq!(vm.read_int(count, 2).unwrap(), 0);
        // Sweep to x = 10 through body 1.
        let sweep = [
            int(space),
            int(0),
            fixed(10),
            fixed(0),
            fixed(0),
            int(0),
            int(1),
        ];
        assert_eq!(d0(&mut vm, 0x70, &sweep), Some(int(8)));
        // Link and query.
        assert_eq!(
            d0(&mut vm, 0x68, &[int(space), int(1), int(0), int(0)]),
            Some(int(0))
        );
        assert_eq!(
            d0(
                &mut vm,
                0x69,
                &[Value::Ptr(out), int(space), int(1), int(0)]
            ),
            Some(int(0))
        );
        assert_eq!(vm.read_int(out, 2).unwrap(), 0);
        assert_eq!(
            d0(
                &mut vm,
                0x79,
                &[Value::Ptr(out), int(space), int(0), int(1)]
            ),
            Some(int(0))
        );
        assert_eq!(vm.read_int(out, 2).unwrap(), 1);
        assert_eq!(vm.read_int(out + 28, 2).unwrap(), 5 << 16);
        assert_eq!(d0(&mut vm, 0x61, &[int(space), int(9)]), Some(int(18)));
        assert_eq!(d0(&mut vm, 0x41, &[int(space)]), Some(int(0)));
        assert_eq!(d0(&mut vm, 0x61, &[int(space), int(0)]), Some(int(1)));
    }
}
