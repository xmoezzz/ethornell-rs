//! BGI 1.687 table funcs_1400CB60D, buffer helpers 1400EF1E0..EF9B0.
//! Handles are process-scoped data tokens, never host pointers. Payloads use
//! the VM's checked heap, which follows the active thread during handover.
use crate::*;

const MAX_BYTES: u32 = 0x4000_0000;
#[derive(Clone, Copy, Debug)]
pub(crate) struct Buffer {
    pub pointer: u32,
    pub size: u32,
}
#[derive(Debug)]
pub(crate) struct NativeBuffers {
    slots: [[Option<Buffer>; 256]; 2],
    cursor: [u8; 2],
}
impl Default for NativeBuffers {
    fn default() -> Self {
        Self {
            slots: [[None; 256]; 2],
            cursor: [0; 2],
        }
    }
}
impl NativeBuffers {
    fn key(handle: u32) -> Option<(usize, usize)> {
        if handle >= 0x1000_0000 || handle & 0x0fff_0000 != 0x0fff_0000 {
            return None;
        }
        let kind = ((handle as u16) >> 12) as usize;
        (kind < 2).then_some((kind, handle as u8 as usize))
    }
    pub(crate) fn get(&self, handle: u32) -> Option<Buffer> {
        let (kind, index) = Self::key(handle)?;
        self.slots[kind][index]
    }
    fn get_kind(&self, handle: u32, kind: usize) -> Option<Buffer> {
        (Self::key(handle)?.0 == kind)
            .then(|| self.get(handle))
            .flatten()
    }
    fn put(&mut self, handle: u32, buffer: Option<Buffer>) {
        let (kind, index) = Self::key(handle).expect("validated buffer handle");
        self.slots[kind][index] = buffer;
    }
    fn reserve(&mut self, kind: usize) -> Option<u32> {
        // Native cursors advance by 31, scanning every one of 256 slots.
        for _ in 0..256 {
            self.cursor[kind] = self.cursor[kind].wrapping_add(31);
            if self.slots[kind][self.cursor[kind] as usize].is_none() {
                return Some(0x0fff_0000 | (kind as u32) << 12 | self.cursor[kind] as u32);
            }
        }
        None
    }
}

impl Vm {
    pub(crate) fn native_buffer(&self, handle: u32) -> Option<Buffer> {
        if self.native_abi_profile != NativeAbiProfile::ModernFused {
            return None;
        }
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .native_buffers
            .get(handle)
    }
    fn native_buffer_kind(&self, handle: u32, kind: usize) -> Option<Buffer> {
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .native_buffers
            .get_kind(handle, kind)
    }
    fn publish_buffer(
        &mut self,
        handle: u32,
        bytes: &[u8],
        preserve: &[(u32, u32, usize)],
    ) -> VmResult<bool> {
        // Native accepts up to 1 GiB. Allocation may fail below that bound;
        // retain the old block and report native allocation status 4 then.
        let pointer = if bytes.is_empty() {
            0
        } else {
            self.alloc_heap(bytes.len() as u32)
        };
        if pointer == 0 && !bytes.is_empty() {
            return Ok(false);
        }
        if pointer != 0 {
            let range = self.resolve_write_range(pointer, bytes.len())?;
            self.memory[range].copy_from_slice(bytes);
        }
        // Native copies preserve pointer-valued cells too. Host-side BP
        // strings/programs have shadow values, so transfer those before the
        // old allocation is freed, just like the ordinary VM memcpy path.
        for &(source, offset, length) in preserve {
            if length != 0 {
                self.copy_shadow_values(source, pointer.wrapping_add(offset), length);
            }
        }
        let old = {
            let mut state = self
                .system81_shared
                .lock()
                .expect("system81 state poisoned");
            let old = state.native_buffers.get(handle);
            state.native_buffers.put(
                handle,
                Some(Buffer {
                    pointer,
                    size: bytes.len() as u32,
                }),
            );
            old
        };
        if let Some(old) = old {
            if old.pointer != 0 {
                self.free_heap(old.pointer);
            }
        }
        Ok(true)
    }
    fn buffer_bytes(&self, buffer: Buffer) -> VmResult<Vec<u8>> {
        if buffer.size == 0 {
            return Ok(Vec::new());
        }
        Ok(self.memory[self.resolve_range(buffer.pointer, buffer.size as usize)?].to_vec())
    }
    fn pop_nullable_buffer_string(&mut self) -> VmResult<Vec<u8>> {
        Ok(match self.pop_value()? {
            Value::None | Value::Int(0) | Value::Ptr(0) => Vec::new(),
            Value::Str(text) => self.encode_buffer_text(&text),
            Value::Int(pointer) => self.read_shadowed_c_string_bytes(pointer as u32)?,
            Value::Ptr(pointer) => self.read_shadowed_c_string_bytes(pointer)?,
            Value::Func { offset, .. } => self.read_shadowed_c_string_bytes(offset)?,
            Value::Program(_) => {
                return Err(VmError::Runtime(
                    "program is not a native string buffer".into(),
                ));
            }
        })
    }

    pub(super) fn execute_buffer_extension(&mut self, id: u8) -> VmResult<i32> {
        Ok(match id {
            0x00 => {
                // 1400F5530: count<2 wins over invalid type/record size.
                let kind = self.pop_int()?;
                let key = self.pop_int()? as u32;
                let stride = self.pop_int()? as u32;
                let count = self.pop_int()? as u32;
                let base = self.pop_ptr()?;
                if count < 2 {
                    1
                } else if !(0..=5).contains(&kind) {
                    3
                } else {
                    if count > 1 << 20 {
                        return Err(VmError::Runtime(
                            "native sort record budget exceeded".into(),
                        ));
                    }
                    let width = 1usize << (kind / 2);
                    if key.checked_add(width as u32).is_none_or(|end| end > stride) {
                        2
                    } else {
                        let length = (count as usize)
                            .checked_mul(stride as usize)
                            .filter(|n| *n <= 64 * 1024 * 1024)
                            .ok_or_else(|| {
                                VmError::Runtime("native sort byte budget exceeded".into())
                            })?;
                        let range = self.resolve_write_range(base, length)?;
                        let bytes = self.memory[range.clone()].to_vec();
                        let mut order: Vec<usize> = (0..count as usize).collect();
                        crate::logic::msvc_qsort::qsort(&mut order, |&a, &b| {
                            let read = |row: usize| {
                                let at = row * stride as usize + key as usize;
                                match width {
                                    1 => bytes[at] as i8 as i32,
                                    2 => i16::from_le_bytes(bytes[at..at + 2].try_into().unwrap())
                                        as i32,
                                    _ => i32::from_le_bytes(bytes[at..at + 4].try_into().unwrap()),
                                }
                            };
                            if kind & 1 == 0 {
                                read(a).wrapping_sub(read(b))
                            } else {
                                read(b).wrapping_sub(read(a))
                            }
                        });
                        let start = self.value_key(base);
                        let shadows: Vec<_> = self
                            .mem_values
                            .range(start..start + length as u32)
                            .map(|(address, value)| (*address - start, value.clone()))
                            .collect();
                        let mut moved = Vec::new();
                        if !shadows.is_empty() {
                            let mut inverse = vec![0; count as usize];
                            for (destination, &source) in order.iter().enumerate() {
                                inverse[source] = destination;
                            }
                            for (offset, value) in shadows {
                                let row = offset as usize / stride as usize;
                                let inside = offset % stride;
                                moved.push((start + inverse[row] as u32 * stride + inside, value));
                            }
                        }
                        for (i, row) in order.into_iter().enumerate() {
                            let source = row * stride as usize;
                            let dest = range.start + i * stride as usize;
                            self.memory[dest..dest + stride as usize]
                                .copy_from_slice(&bytes[source..source + stride as usize]);
                        }
                        self.clear_shadow_values(base, length);
                        self.mem_values.extend(moved);
                        0
                    }
                }
            }
            0x80 | 0x88 => {
                let bytes = if id == 0x80 {
                    let size = self.pop_int()? as u32;
                    let output = self.pop_ptr()?;
                    if size > MAX_BYTES {
                        return Ok(3);
                    }
                    if size > 64 * 1024 * 1024 {
                        return Ok(4);
                    }
                    // Native operator new leaves this uninitialized. Zero
                    // new bytes deliberately, including resize tails, so guest
                    // reads cannot expose prior host allocations.
                    (vec![0; size as usize], output, 0)
                } else {
                    let mut text = self.pop_nullable_buffer_string()?;
                    let output = self.pop_ptr()?;
                    text.push(0);
                    (text, output, 1)
                };
                self.resolve_write_range(bytes.1, 4)?;
                let handle = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .native_buffers
                    .reserve(bytes.2);
                if let Some(handle) = handle {
                    if self.publish_buffer(handle, &bytes.0, &[])? {
                        self.write_int(bytes.1, 2, handle)?;
                        0
                    } else {
                        4
                    }
                } else {
                    4
                }
            }
            0x81 | 0x89 => {
                let handle = self.pop_int()? as u32;
                let Some(buffer) = self.native_buffer_kind(handle, usize::from(id == 0x89)) else {
                    return Ok(1);
                };
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .native_buffers
                    .put(handle, None);
                if buffer.pointer != 0 {
                    self.free_heap(buffer.pointer);
                }
                0
            }
            0x82 => {
                let size = self.pop_int()? as u32;
                let handle = self.pop_int()? as u32;
                if size > MAX_BYTES {
                    return Ok(3);
                }
                let Some(buffer) = self.native_buffer_kind(handle, 0) else {
                    return Ok(1);
                };
                if size > 64 * 1024 * 1024 {
                    return Ok(4);
                }
                let mut bytes = self.buffer_bytes(buffer)?;
                bytes.resize(size as usize, 0);
                if self.publish_buffer(
                    handle,
                    &bytes,
                    &[(buffer.pointer, 0, buffer.size.min(size) as usize)],
                )? {
                    0
                } else {
                    4
                }
            }
            0x83 => {
                let handle = self.pop_int()? as u32;
                self.native_buffer_kind(handle, 0)
                    .map_or(-1, |buffer| buffer.size as i32)
            }
            0x84 | 0x85 | 0x86 => {
                let count = self.pop_int()? as u32;
                let pointer = if id == 0x85 {
                    None
                } else {
                    Some(self.pop_ptr()?)
                };
                let offset = self.pop_int()?;
                let handle = self.pop_int()? as u32;
                let output = if id == 0x85 {
                    Some(self.pop_ptr()?)
                } else {
                    None
                };
                let Some(buffer) = self.native_buffer_kind(handle, 0) else {
                    return Ok(1);
                };
                let offset = if id == 0x86 && offset < 0 {
                    buffer.size
                } else {
                    offset as u32
                };
                if if id == 0x86 {
                    offset > buffer.size
                } else {
                    offset >= buffer.size
                } {
                    return Ok(2);
                }
                if id != 0x86 && count > buffer.size - offset {
                    return Ok(3);
                }
                if id == 0x86 && count > MAX_BYTES - buffer.size {
                    return Ok(3);
                }
                if count == 0 {
                    return Ok(0);
                }
                if let Some(output) = output {
                    let source = buffer.pointer.wrapping_add(offset);
                    let bytes = self.memory[self.resolve_range(source, count as usize)?].to_vec();
                    let dest = self.resolve_write_range(output, count as usize)?;
                    self.memory[dest].copy_from_slice(&bytes);
                    self.copy_shadow_values(source, output, count as usize);
                    0
                } else {
                    let source = self.resolve_range(pointer.unwrap(), count as usize)?;
                    let inserted = self.memory[source].to_vec();
                    if id == 0x84 {
                        let dest = buffer.pointer.wrapping_add(offset);
                        let range = self.resolve_write_range(dest, count as usize)?;
                        self.memory[range].copy_from_slice(&inserted);
                        self.copy_shadow_values(pointer.unwrap(), dest, count as usize);
                        0
                    } else {
                        if buffer.size.saturating_add(count) > 64 * 1024 * 1024 {
                            return Ok(4);
                        }
                        let mut bytes = self.buffer_bytes(buffer)?;
                        bytes.splice(offset as usize..offset as usize, inserted);
                        if self.publish_buffer(
                            handle,
                            &bytes,
                            &[
                                (buffer.pointer, 0, offset as usize),
                                (pointer.unwrap(), offset, count as usize),
                                (
                                    buffer.pointer.wrapping_add(offset),
                                    offset + count,
                                    (buffer.size - offset) as usize,
                                ),
                            ],
                        )? {
                            0
                        } else {
                            4
                        }
                    }
                }
            }
            0x8a | 0x8b => {
                let format =
                    crate::version_system::decode_auto(&self.pop_nullable_buffer_string()?, 6);
                let offset = if id == 0x8a { self.pop_int()? } else { 0 };
                let handle = self.pop_int()? as u32;
                let Some(buffer) = self.native_buffer_kind(handle, 1) else {
                    return Ok(1);
                };
                let mut bytes = if id == 0x8b {
                    vec![0]
                } else {
                    self.buffer_bytes(buffer)?
                };
                let offset = if offset < 0 {
                    bytes.len() - 1
                } else {
                    offset as usize
                };
                if offset >= bytes.len() {
                    return Ok(2);
                }
                let text = self.render_buffer_format(&format)?;
                if text.len() > MAX_BYTES as usize - bytes.len()
                    || text.len() + bytes.len() > 64 * 1024 * 1024
                {
                    return Ok(4);
                }
                bytes.splice(offset..offset, text);
                if self.publish_buffer(handle, &bytes, &[])? {
                    0
                } else {
                    4
                }
            }
            _ => {
                return Err(VmError::UnknownDispatch {
                    group: 0x7f,
                    id: id as u16,
                });
            }
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    fn call(vm: &mut Vm, id: u8, args: Vec<Value>) -> i32 {
        vm.stack = args;
        let result = vm.execute_buffer_extension(id).unwrap();
        assert!(vm.stack.is_empty());
        result
    }
    #[test]
    fn native_buffers_binary_lifetime_bounds_insertion_and_aliases() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        assert_eq!(call(&mut vm, 0x80, vec![Value::Ptr(128), Value::Int(4)]), 0);
        let handle = vm.read_int(128, 2).unwrap();
        assert_eq!(handle, 0x0fff001f);
        for (i, b) in b"abcd".iter().enumerate() {
            vm.write_int(256 + i as u32, 0, *b as u32).unwrap();
        }
        assert_eq!(
            call(
                &mut vm,
                0x84,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(0),
                    Value::Ptr(256),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(vm.read_int(handle, 0).unwrap(), b'a' as u32);
        assert!(vm.resolve_range(handle, 5).is_err());
        assert_eq!(
            call(
                &mut vm,
                0x85,
                vec![
                    Value::Ptr(300),
                    Value::Int(handle as i32),
                    Value::Int(1),
                    Value::Int(2)
                ]
            ),
            0
        );
        assert_eq!(&vm.memory[vm.resolve_range(300, 2).unwrap()], b"bc");
        assert_eq!(
            call(
                &mut vm,
                0x84,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(4),
                    Value::Ptr(256),
                    Value::Int(0)
                ]
            ),
            2
        );
        assert_eq!(
            call(
                &mut vm,
                0x86,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(-1),
                    Value::Ptr(256),
                    Value::Int(2)
                ]
            ),
            0
        );
        assert_eq!(call(&mut vm, 0x83, vec![Value::Int(handle as i32)]), 6);
        assert_eq!(
            call(
                &mut vm,
                0x82,
                vec![Value::Int(handle as i32), Value::Int(3)]
            ),
            0
        );
        assert_eq!(&vm.memory[vm.resolve_range(handle, 3).unwrap()], b"abc");
        assert_eq!(call(&mut vm, 0x81, vec![Value::Int(handle as i32)]), 0);
        assert!(vm.resolve_range(handle, 1).is_err());
        assert_eq!(call(&mut vm, 0x83, vec![Value::Int(handle as i32)]), -1);
        assert_eq!(call(&mut vm, 0x81, vec![Value::Int(handle as i32)]), 1);
        assert_eq!(call(&mut vm, 0x80, vec![Value::Ptr(128), Value::Int(4)]), 0);
        let handle = vm.read_int(128, 2).unwrap();
        let value = Value::Str("owned BP string".into());
        vm.write_value(400, 2, &value).unwrap();
        assert_eq!(
            call(
                &mut vm,
                0x84,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(0),
                    Value::Ptr(400),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(vm.read_value(handle, 2).unwrap(), value);
        assert_eq!(
            call(
                &mut vm,
                0x84,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(0),
                    Value::Ptr(handle),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(vm.read_value(handle, 2).unwrap(), value);

        assert_eq!(
            call(
                &mut vm,
                0x82,
                vec![Value::Int(handle as i32), Value::Int(8)]
            ),
            0
        );
        assert_eq!(vm.read_value(handle, 2).unwrap(), value);
        assert_eq!(
            call(
                &mut vm,
                0x86,
                vec![
                    Value::Int(handle as i32),
                    Value::Int(0),
                    Value::Ptr(256),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(
            call(
                &mut vm,
                0x85,
                vec![
                    Value::Ptr(420),
                    Value::Int(handle as i32),
                    Value::Int(4),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(vm.read_value(420, 2).unwrap(), value);
    }
    #[test]
    fn native_buffers_string_format_varargs_and_signed_sort_keys() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        assert_eq!(
            call(
                &mut vm,
                0x88,
                vec![Value::Ptr(128), Value::Str("prefix".into())]
            ),
            0
        );
        let handle = vm.read_int(128, 2).unwrap();
        assert_eq!(
            call(
                &mut vm,
                0x8a,
                vec![
                    Value::Int(-98304),
                    Value::Int(7),
                    Value::Int(handle as i32),
                    Value::Int(-1),
                    Value::Str(" %03d %.2f".into())
                ]
            ),
            0
        );
        assert_eq!(
            vm.read_shadowed_c_string(handle).unwrap(),
            "prefix 007 -1.50"
        );
        assert_eq!(
            call(
                &mut vm,
                0x8b,
                vec![
                    Value::Int(0xff),
                    Value::Int(handle as i32),
                    Value::Str("%c".into())
                ]
            ),
            0
        );
        assert_eq!(vm.read_int(handle, 0).unwrap(), 0xff);
        assert!(vm.render_buffer_format("%+d").is_err());
        assert!(vm.render_buffer_format("%#x").is_err());
        vm.stack = vec![Value::Str("日本".into())];
        let rendered = vm.render_buffer_format("%5s").unwrap();
        assert_eq!(&rendered[..3], b"   ");
        assert_eq!(
            &rendered[3..],
            encoding_rs::SHIFT_JIS.encode("日本").0.as_ref()
        );

        assert_eq!(
            call(
                &mut vm,
                0x8b,
                vec![
                    Value::Int(0),
                    Value::Int(handle as i32),
                    Value::Str("%cignored".into())
                ]
            ),
            0
        );
        assert_eq!(vm.native_buffer(handle).unwrap().size, 1);
        assert_eq!(
            call(
                &mut vm,
                0x8b,
                vec![Value::Int(handle as i32), Value::Int(0)]
            ),
            0
        );
        assert_eq!(call(&mut vm, 0x89, vec![Value::Int(handle as i32)]), 0);
        for (i, n) in [3i16, -1, 0].into_iter().enumerate() {
            vm.write_int(256 + i as u32 * 2, 1, n as u16 as u32)
                .unwrap();
        }
        assert_eq!(
            call(
                &mut vm,
                0,
                vec![
                    Value::Ptr(256),
                    Value::Int(3),
                    Value::Int(2),
                    Value::Int(0),
                    Value::Int(2)
                ]
            ),
            0
        );
        assert_eq!(
            (0..3)
                .map(|i| vm.read_int(256 + i * 2, 1).unwrap() as u16 as i16)
                .collect::<Vec<_>>(),
            [-1, 0, 3]
        );
        vm.write_int(320, 2, 2).unwrap();
        vm.write_value(324, 2, &Value::Str("two".into())).unwrap();
        vm.write_int(328, 2, 1).unwrap();
        vm.write_value(332, 2, &Value::Str("one".into())).unwrap();
        assert_eq!(
            call(
                &mut vm,
                0,
                vec![
                    Value::Ptr(320),
                    Value::Int(2),
                    Value::Int(8),
                    Value::Int(0),
                    Value::Int(4)
                ]
            ),
            0
        );
        assert_eq!(vm.read_value(324, 2).unwrap(), Value::Str("one".into()));
        assert_eq!(vm.read_value(332, 2).unwrap(), Value::Str("two".into()));
        assert_eq!(
            call(
                &mut vm,
                0,
                vec![
                    Value::Ptr(256),
                    Value::Int(1),
                    Value::Int(0),
                    Value::Int(99),
                    Value::Int(99)
                ]
            ),
            1
        );
    }
    #[test]
    fn native_buffers_profile_grammar_preserves_following_instruction() {
        let bytes = [0x7f, 0x83, 0x17];
        let program =
            ethornell_script::parse_bp_program_profile(None, &bytes, NativeAbiProfile::ModernFused);
        assert_eq!(&*program.instructions[0].raw, &[0x7f, 0x83]);
        assert_eq!(program.instructions[1].opcode.code(), 0x17);
        let legacy = ethornell_script::parse_bp_program(None, &bytes);
        assert_eq!(&*legacy.instructions[0].raw, &[0x7f]);
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        vm.start(&program);
        vm.stack.push(Value::Int(0));
        let report = vm.run_loaded(
            &mut TraceApi,
            &VmRunOptions {
                fail_on_stub: true,
                ..Default::default()
            },
        );
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert_eq!(vm.stack, [Value::Int(-1)]);
    }
}
