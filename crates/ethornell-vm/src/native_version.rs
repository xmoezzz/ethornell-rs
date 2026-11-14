//! Native pointer bridges added by the 1.653 / 1.687 profiles.
//! Their buffers belong to checked BP memory; no host pointer escapes.
use crate::*;

impl Vm {
    /// FE1B0/BCA40 recognize self-describing compressed inputs before
    /// consulting the caller's byte count. In particular count=0 is valid
    /// for DSC/CBG. Bound parsing to the owning guest allocation rather than
    /// reproducing the native worker's unchecked pointer reads.
    fn native_bitmap_source_range(&self,source:u32,size:i32)->VmResult<std::ops::Range<usize>> {
        let prefix=self.resolve_range(source,16)?;
        let compressed=self.memory[prefix.clone()].starts_with(b"DSC FORMAT 1.00")
            || self.memory[prefix.clone()].starts_with(b"CompressedBG___");
        if !compressed && size>0 {return self.resolve_range(source,size as usize)}
        let available=if let Some(buffer)=self.native_buffer(source) {Some(buffer.size as usize)}
            else if let Some((base,offset))=self.target_heap.translate(source) {
                self.heap_allocations.get(&base).and_then(|size|size.checked_sub(offset)).map(|n|n as usize)
            } else {None};
        if let Some(available)=available {
            if available>0x04000000 {return Err(VmError::Runtime("native bitmap source allocation limit".into()))}
            return self.resolve_range(source,available);
        }
        if size>0 {return self.resolve_range(source,size as usize)}
        // A static/unallocated input still has a checked code/data region.
        // Locate its readable extent without crossing a segment or exposing
        // the next heap allocation. Failed decoders retain native failure.
        let mut low=16usize;let mut high=(self.memory.len()-prefix.start).min(0x04000000);
        while low<high {
            let mid=low+(high-low).div_ceil(2);
            if self.resolve_range(source,mid).is_ok() {low=mid} else {high=mid-1}
        }
        self.resolve_range(source,low)
    }
    pub(super) fn try_version_graph<A: SysApi + GraphApi>(
        &mut self,
        api: &mut A,
        code: u8,
        id: u16,
        trace_events: bool,
    ) -> VmResult<Option<Value>> {
        Ok(match (code, id) {
            (0x91, 0x04) if self.native_abi_profile == NativeAbiProfile::ModernFused => {
                // 1400E30B0 -> DCProcRegisterBmp -> DCProcDecodeData.
                // PopPtr returns a guest buffer, not a filename. Argument 2
                // is its source byte count; it is not a decode-mode selector.
                let bg_store = self.pop_int()? != 0;
                let name = self.pop_value()?;
                let name = if matches!(name, Value::Int(0) | Value::Ptr(0) | Value::None) {
                    None
                } else {
                    Some(self.value_as_string_lossy(name)?)
                };
                let namespace = self.pop_value()?;
                let namespace = if matches!(namespace, Value::Int(0) | Value::Ptr(0) | Value::None)
                {
                    None
                } else {
                    Some(self.value_as_string_lossy(namespace)?)
                };
                let size = self.pop_int()?;
                let source = self.pop_ptr()?;
                let bitmap = self.pop_int()?;
                let status = if source == 0 {
                    -1
                } else {
                    let range = self.native_bitmap_source_range(source,size)?;
                    match crate::resource_io::target_decode_resource(&self.memory[range], 0, 0) {
                        Ok(bytes) => api.register_decoded_graph_bitmap(
                            bitmap,
                            &bytes,
                            namespace.as_deref(),
                            name.as_deref(),
                            bg_store,
                        ),
                        Err(_) => -1,
                    }
                };
                let mut call = NativeCallFrame::new(NativeOpcode { group: code, id }, Vec::new());
                call.complete_procedure_with_output(
                    native_call::NativeProcedureClass::RegisterBitmap,
                    status,
                    status,
                );
                self.install_host_completed_procedure(
                    NativeOpcode { group: code, id },
                    call.take_procedure_completion().unwrap(),
                    trace_events,
                );
                Some(Value::None)
            }

            (0x90, 0xc1) => {
                // 1.653 489110 -> 4023B0 / 1.687 1400D7F30:
                // validate every synthesis member; report the first
                // absent resource, or #NNN for an empty component.
                let spec = self.pop_string_lossy()?;
                let archive = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                let mut failure = None;
                for (index, part) in spec.split('/').enumerate() {
                    let name = part.split(',').next().unwrap_or_default().trim_matches(' ');
                    if name.is_empty() {
                        failure = Some(format!("#{index:03}"));
                        break;
                    }
                    let exists = if archive.is_empty() {
                        api.disk_file_exists(name)
                    } else {
                        api.archive_entry_exists(&archive, name)
                    };
                    if !exists {
                        failure = Some(name.to_owned());
                        break;
                    }
                }
                if output != 0
                    && let Some(name) = failure.as_deref()
                {
                    self.write_c_string(output, name)?;
                }
                Some(Value::Int(i32::from(failure.is_none())))
            }
            (0x90, 0xcf) => {
                // 4899F0 -> 4063B0 / 1400D7350 -> 140032140.
                let depth_y = self.pop_int()? as u32;
                let depth_x = self.pop_int()? as u32;
                let source = self.pop_ptr()?;
                let destination = self.pop_ptr()?;
                let xyz = [0, 4, 8].map(|offset| {
                    self.read_int(source.wrapping_add(offset), 2)
                        .map(|n| n as i32)
                });
                let [x, y, z] = xyz;
                let (x, y, z) = (x?, y?, z?);
                let factor = |depth: u32| -> u32 {
                    if depth == 0 || z == 0 {
                        0x10000
                    } else if z < 0 {
                        (((u64::from(depth) << 16) + (-i64::from(z)) as u64) / u64::from(depth))
                            as u32
                    } else {
                        ((u64::from(depth) << 32) / ((u64::from(depth) << 16) + z as u64)) as u32
                    }
                };
                let x = (i64::from(x) * i64::from(factor(depth_x))) >> 16;
                let y = (i64::from(y) * i64::from(factor(depth_y))) >> 16;
                self.write_int(destination, 2, x as u32)?;
                self.write_int(destination.wrapping_add(4), 2, y as u32)?;
                Some(Value::None)
            }
            (0x91, 0xbd) if self.native_abi_profile == NativeAbiProfile::ModernFused => {
                let initial = self.pop_int()?;
                let duration = self.pop_int()?;
                let curve_in = self.pop_int()?;
                let curve_out = self.pop_int()?;
                let alpha_end = self.pop_int()?;
                let alpha_start = self.pop_int()?;
                let source = self.pop_ptr()?;
                let count = self.pop_int()?;
                let item = self.pop_int()?;
                let group = self.pop_int()?;
                let input = self.pop_int()?;
                let mut points = Vec::new();
                if count >= 2 {
                    if count > 65536 {
                        return Err(VmError::Runtime(
                            "icon path control-point budget exceeded".into(),
                        ));
                    }
                    self.resolve_range(source, count as usize * 16)?;
                    for i in 0..count as u32 {
                        let p = source.wrapping_add(i * 16);
                        let mut row = [0; 4];
                        for j in 0..4 {
                            row[j] = self.read_int(p.wrapping_add(j as u32 * 4), 2)? as i32;
                        }
                        points.push(row);
                    }
                }
                Some(Value::Int(api.configure_icon_path(
                    [
                        input,
                        group,
                        item,
                        count,
                        alpha_start,
                        alpha_end,
                        curve_out,
                        curve_in,
                        duration,
                        initial,
                    ],
                    &points,
                )?))
            }
            (0x92, 0x94) if self.native_abi_profile == NativeAbiProfile::ModernFused => {
                let token = self.pop_int()?;
                let destination = self.pop_ptr()?;
                let values = api.text_line_height_record(token, false);
                if destination != 0 && !values.is_empty() {
                    self.resolve_write_range(destination, values.len() * 4)?;
                    for (i, &v) in values.iter().enumerate() {
                        self.write_int(destination.wrapping_add(i as u32 * 4), 2, v as u32)?;
                    }
                    let _ = api.text_line_height_record(token, true);
                }
                Some(Value::Int(values.len() as i32))
            }
            (0x92, 0x3d) => {
                let destination = self.pop_ptr()?;
                Some(if let Some((handle, kind)) = api.pointer_graph_object() {
                    self.write_int(destination, 2, handle as u32)?;
                    Value::Int(kind)
                } else {
                    Value::Int(-1)
                })
            }
            _ => None,
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    struct Pending {
        polls: usize,
    }
    impl SysApi for Pending {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl SoundApi for Pending {
        fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl GraphApi for Pending {
        fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
        fn poll_native_graph_result(
            &mut self,
            _: NativeOpcode,
            cancel: bool,
        ) -> VmResult<Option<i32>> {
            assert!(!cancel);
            self.polls += 1;
            Ok((self.polls > 1).then_some(7))
        }
    }
    #[test]
    fn version_native_procedure_waits_for_host_and_pushes_one_deferred_result() {
        let mut vm = Vm::new();
        vm.start(&ethornell_script::parse_bp_program(None, &[0x17]));
        vm.install_cprocedure(
            InstalledCProcedure::graph(
                vm.thread.thread_id(),
                NativeOpcode {
                    group: 0x91,
                    id: 0xf8,
                },
                "DCProcPlayMovieByMF",
                native_thread::NativeGraphProcedureMode::HostResult,
                None,
                None,
            ),
            false,
        );
        let mut host = Pending { polls: 0 };
        assert_eq!(
            vm.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::WaitingForProcedure
        );
        assert!(vm.stack.is_empty());
        assert_eq!(
            vm.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::Completed
        );
        assert_eq!(vm.stack, [Value::Int(7)]);
    }

    #[derive(Default)]
    struct BitmapHost {
        observed: Option<(i32, Vec<u8>, Option<String>, Option<String>, bool)>,
    }
    impl SysApi for BitmapHost {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl SoundApi for BitmapHost {
        fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl GraphApi for BitmapHost {
        fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            panic!("guest buffer must use the pointer bridge")
        }
        fn register_decoded_graph_bitmap(
            &mut self,
            bitmap: i32,
            bytes: &[u8],
            namespace: Option<&str>,
            name: Option<&str>,
            bg: bool,
        ) -> i32 {
            self.observed = Some((
                bitmap,
                bytes.to_vec(),
                namespace.map(str::to_owned),
                name.map(str::to_owned),
                bg,
            ));
            0
        }
    }
    #[test]
    fn version_native_bitmap_registration_decodes_guest_bytes_and_defers_status() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        vm.start(&ethornell_script::parse_bp_program(None, &[0x91, 4, 0x17]));
        let raw = [
            1, 0, 1, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x11, 0x22, 0x33,
        ];
        for (i, &byte) in raw.iter().enumerate() {
            vm.write_int(64 + i as u32, 0, byte as u32).unwrap();
        }
        vm.stack = vec![
            Value::Int(3),
            Value::Ptr(64),
            Value::Int(raw.len() as i32),
            Value::Str("namespace".into()),
            Value::Str("bitmap".into()),
            Value::Int(1),
        ];
        let mut host = BitmapHost::default();
        assert_eq!(
            vm.run_loaded(
                &mut host,
                &VmRunOptions {
                    fail_on_stub: true,
                    ..Default::default()
                }
            )
            .stop_reason,
            VmStopReason::WaitingForProcedure
        );
        assert!(vm.stack.is_empty());
        assert_eq!(
            host.observed,
            Some((
                3,
                raw.to_vec(),
                Some("namespace".into()),
                Some("bitmap".into()),
                true
            ))
        );
        assert_eq!(
            vm.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::Completed
        );
        assert_eq!(vm.stack, [Value::Int(0)]);
    }

    #[test]
    fn version_native_projection_uses_signed_depth_and_checked_output_memory() {
        let mut vm = Vm::new();
        for (at, v) in [(64, 100), (68, -50), (72, -65536)] {
            vm.write_int(at, 2, v as u32).unwrap();
        }
        vm.stack = vec![Value::Ptr(96), Value::Ptr(64), Value::Int(1), Value::Int(1)];
        assert_eq!(
            vm.try_version_graph(&mut TraceApi, 0x90, 0xcf, false)
                .unwrap(),
            Some(Value::None)
        );
        assert_eq!(vm.read_int(96, 2).unwrap() as i32, 200);
        assert_eq!(vm.read_int(100, 2).unwrap() as i32, -100);
        assert!(vm.stack.is_empty());
    }
}
