//! Sys81:28..2B. The native worker (raw/text_400000.c, sub_4014D0)
//! transfers in the direction selected by the stream's open mode. The host
//! completes requests synchronously, retaining the separate queue/completion
//! results: close/seek complete with 1, transfer with a byte count, failure -1.
use super::*;

impl Vm {
    pub(crate) fn sys81_open_stream<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let mode = self.pop_int()?;
        let path = self.pop_string_lossy()?;
        let output = self.pop_ptr()?;
        if !(0..=2).contains(&mode) {
            return Ok(Value::Int(1));
        }
        if self
            .system81_shared
            .lock()
            .expect("system81 state poisoned")
            .resource_streams
            .values()
            .any(|s| s.path.eq_ignore_ascii_case(&path))
        {
            return Ok(Value::Int(2));
        }
        let bytes = match mode {
            0 => api
                .read_user_file_bytes(&path)
                .or_else(|| api.load_file_bytes("", &path))
                .or_else(|| {
                    api.allow_direct_filesystem_fallback()
                        .then(|| std::fs::read(&path).ok())
                        .flatten()
                }),
            1 => Some(Vec::new()),
            // OPEN_ALWAYS preserves existing user bytes; it must not import
            // an original save through the resource/archive search path.
            _ => Some(api.read_user_file_bytes(&path).unwrap_or_default()),
        };
        let Some(bytes) = bytes else {
            return Ok(Value::Int(3));
        };
        if mode != 0 && !api.write_file_bytes(&path, &bytes) {
            return Ok(Value::Int(3));
        }
        let handle = self
            .system81_shared
            .lock()
            .expect("system81 state poisoned")
            .open_resource_stream(path, mode, bytes)
            .expect("validated stream open");
        self.write_int(output, 2, handle)?;
        Ok(Value::Int(0))
    }

    pub(crate) fn sys81_close_stream(&mut self) -> VmResult<Value> {
        let handle = self.pop_int()? as u32;
        let output = self.pop_ptr()?;
        let removed = self
            .system81_shared
            .lock()
            .expect("system81 state poisoned")
            .resource_streams
            .remove(&handle)
            .is_some();
        if !removed {
            return Ok(Value::Int(4));
        }
        if output != 0 {
            self.write_int(output, 2, 1)?;
        }
        Ok(Value::Int(0))
    }

    pub(crate) fn sys81_transfer_stream<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let length = self.pop_int()? as u32;
        let buffer = self.pop_ptr()?;
        let handle = self.pop_int()? as u32;
        let output = self.pop_ptr()?;
        if buffer == 0 {
            return self.seek_stream(handle, output, length);
        }
        let stream = self
            .system81_shared
            .lock()
            .expect("system81 state poisoned")
            .resource_streams
            .get(&handle)
            .cloned();
        let Some(mut stream) = stream else {
            return Ok(Value::Int(4));
        };
        let count = if stream.mode == 0 {
            let count = (length as usize).min(stream.bytes.len().saturating_sub(stream.cursor));
            let range = self.resolve_write_range(buffer, count)?;
            self.memory[range].copy_from_slice(&stream.bytes[stream.cursor..stream.cursor + count]);
            self.clear_shadow_values(buffer, count);
            count
        } else {
            let range = self.resolve_range(buffer, length as usize)?;
            let end = stream
                .cursor
                .checked_add(length as usize)
                .ok_or_else(|| VmError::Runtime("stream write range overflow".into()))?;
            stream.bytes.resize(stream.bytes.len().max(end), 0);
            stream.bytes[stream.cursor..end].copy_from_slice(&self.memory[range]);
            if api.write_file_bytes(&stream.path, &stream.bytes) {
                length as usize
            } else {
                0
            }
        };
        if count != 0 {
            stream.cursor += count;
            self.system81_shared
                .lock()
                .expect("system81 state poisoned")
                .resource_streams
                .insert(handle, stream);
        }
        if output != 0 {
            self.write_int(output, 2, if count == 0 { u32::MAX } else { count as u32 })?;
        }
        Ok(Value::Int(0))
    }

    pub(crate) fn sys81_seek_stream(&mut self) -> VmResult<Value> {
        let offset = self.pop_int()? as u32;
        let handle = self.pop_int()? as u32;
        let output = self.pop_ptr()?;
        self.seek_stream(handle, output, offset)
    }

    fn seek_stream(&mut self, handle: u32, output: u32, offset: u32) -> VmResult<Value> {
        let completion = {
            let mut state = self
                .system81_shared
                .lock()
                .expect("system81 state poisoned");
            let Some(stream) = state.resource_streams.get_mut(&handle) else {
                return Ok(Value::Int(4));
            };
            // The native worker treats UINT_MAX as EOF, rejects past EOF,
            // and reports success as 1 (including a seek to offset zero).
            let offset = if offset == u32::MAX {
                stream.bytes.len()
            } else {
                offset as usize
            };
            if offset <= stream.bytes.len() {
                stream.cursor = offset;
                1
            } else {
                u32::MAX
            }
        };
        if output != 0 {
            self.write_int(output, 2, completion)?;
        }
        Ok(Value::Int(0))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[derive(Default)]
    struct Files {
        files: BTreeMap<String, Vec<u8>>,
        deny: bool,
    }
    impl SysApi for Files {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn read_user_file_bytes(&mut self, path: &str) -> Option<Vec<u8>> {
            self.files.get(path).cloned()
        }
        fn write_file_bytes(&mut self, path: &str, bytes: &[u8]) -> bool {
            if self.deny {
                return false;
            }
            self.files.insert(path.into(), bytes.to_vec());
            true
        }
        fn allow_direct_filesystem_fallback(&self) -> bool {
            false
        }
    }
    fn call(vm: &mut Vm, api: &mut Files, id: u16, args: Vec<Value>) -> i32 {
        vm.stack = args;
        let Some(Value::Int(status)) = vm.try_builtin_sys_with_api(api, 0x81, id).unwrap() else {
            panic!()
        };
        assert!(vm.stack.is_empty());
        status
    }
    fn ints(args: &[i32]) -> Vec<Value> {
        args.iter().copied().map(Value::Int).collect()
    }
    fn open(vm: &mut Vm, api: &mut Files, mode: i32) -> i32 {
        let status = call(
            vm,
            api,
            0x28,
            vec![
                Value::Int(0x1200),
                Value::Str("BGI.edb".into()),
                Value::Int(mode),
            ],
        );
        assert_eq!(status, 0);
        vm.read_int(0x1200, 2).unwrap() as i32
    }
    #[test]
    fn stream_create_append_reopen_read_and_completion_statuses() {
        let (mut vm, mut api) = (Vm::new(), Files::default());
        let h = open(&mut vm, &mut api, 1);
        assert_eq!(api.files["BGI.edb"], b"");
        vm.memory[0x1300..0x1303].copy_from_slice(b"one");
        assert_eq!(
            call(&mut vm, &mut api, 0x2a, ints(&[0x1204, h, 0x1300, 3])),
            0
        );
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 3);
        assert_eq!(call(&mut vm, &mut api, 0x29, ints(&[0x1204, h])), 0);
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 1);
        let h = open(&mut vm, &mut api, 2);
        assert_eq!(call(&mut vm, &mut api, 0x2b, ints(&[0x1204, h, -1])), 0);
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 1);
        vm.memory[0x1300..0x1303].copy_from_slice(b"two");
        assert_eq!(
            call(&mut vm, &mut api, 0x2a, ints(&[0x1204, h, 0x1300, 3])),
            0
        );
        assert_eq!(api.files["BGI.edb"], b"onetwo");
        call(&mut vm, &mut api, 0x29, ints(&[0x1204, h]));
        let h = open(&mut vm, &mut api, 0);
        assert_eq!(
            call(&mut vm, &mut api, 0x2a, ints(&[0x1204, h, 0x1400, 10])),
            0
        );
        assert_eq!(&vm.memory[0x1400..0x1406], b"onetwo");
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 6);
        call(&mut vm, &mut api, 0x2a, ints(&[0x1204, h, 0x1400, 1]));
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), u32::MAX);
        call(&mut vm, &mut api, 0x2b, ints(&[0x1204, h, 0]));
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 1);
        call(&mut vm, &mut api, 0x2b, ints(&[0x1204, h, 7]));
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), u32::MAX);
        call(&mut vm, &mut api, 0x29, ints(&[0x1204, h]));
        open(&mut vm, &mut api, 1);
        assert!(api.files["BGI.edb"].is_empty());
    }
    #[test]
    fn stream_rejects_duplicate_or_denied_opens_and_reports_failed_writes() {
        let (mut vm, mut api) = (Vm::new(), Files::default());
        let h = open(&mut vm, &mut api, 1);
        assert_eq!(
            call(
                &mut vm,
                &mut api,
                0x28,
                vec![
                    Value::Int(0x1200),
                    Value::Str("bgi.EDB".into()),
                    Value::Int(1)
                ]
            ),
            2
        );
        api.deny = true;
        vm.memory[0x1300] = 42;
        assert_eq!(
            call(&mut vm, &mut api, 0x2a, ints(&[0x1204, h, 0x1300, 1])),
            0
        );
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), u32::MAX);
        assert!(api.files["BGI.edb"].is_empty());
        call(&mut vm, &mut api, 0x29, ints(&[0x1204, h]));
        assert_eq!(
            call(
                &mut vm,
                &mut api,
                0x28,
                vec![
                    Value::Int(0x1200),
                    Value::Str("new.edb".into()),
                    Value::Int(2)
                ]
            ),
            3
        );
        vm.write_int(0x1204, 2, 123).unwrap();
        assert_eq!(call(&mut vm, &mut api, 0x29, ints(&[0x1204, h])), 4);
        assert_eq!(vm.read_int(0x1204, 2).unwrap(), 123);
    }
}
