//! E0 usage counters and allocation reports, 464C50..4652B0 / AA830..AAF60.
//! These are independent of the optional developer trace collector.
use crate::{NativeAbiProfile, SysApi, Value, Vm, VmResult};
use std::collections::BTreeMap;

#[derive(Debug, Default)]
pub(crate) struct NativeDiagnostics {
    counts: BTreeMap<u16, u32>,
    flags: BTreeMap<u16, u32>,
    allocation_logging: bool,
    allocations: BTreeMap<u32, (u64, String)>,
    allocation_sequence: u64,
}

fn registered(profile: NativeAbiProfile, group: u16) -> bool {
    matches!(
        group,
        0x80 | 0x81 | 0x90 | 0x91 | 0x92 | 0xa0 | 0xb0 | 0xc0 | 0xd0
    ) || profile == NativeAbiProfile::ModernFused && group == 0x7f
}
impl NativeDiagnostics {
    pub(crate) fn count(&mut self, profile: NativeAbiProfile, group: u8, id: u16) {
        if id < 256 && registered(profile, group as u16) {
            let count = self.counts.entry((group as u16) << 8 | id).or_default();
            *count = count.wrapping_add(1);
        }
    }
    fn visible_counts(&self) -> Vec<(u16, u32)> {
        self.counts
            .iter()
            .filter(|(id, n)| **n != 0 && self.flags.get(id).copied().unwrap_or(0) & 1 == 0)
            .map(|(id, n)| (*id, *n))
            .collect()
    }
}
impl Vm {
    pub(super) fn record_native_allocation(&mut self, pointer: u32, size: u32) {
        let mut state = self
            .system81_shared
            .lock()
            .expect("system81 state poisoned");
        if state.diagnostics.allocation_logging {
            let program = self.program_name(self.current_program);
            let line = format!(
                "Address [ ${pointer:08X} ] : Size [ {size} ] : Thread [ {} ] , Program [ {program} ] , IP [ ${:08X} ]\n",
                self.thread.thread_id(),
                self.pc
            );
            state.diagnostics.allocation_sequence =
                state.diagnostics.allocation_sequence.wrapping_add(1);
            let sequence = state.diagnostics.allocation_sequence;
            state
                .diagnostics
                .allocations
                .insert(pointer, (sequence, line));
        }
    }
    pub(super) fn forget_native_allocation(&mut self, pointer: u32) {
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .diagnostics
            .allocations
            .remove(&pointer);
    }
    /// E0:3F (AB400 -> C3610 -> 092CE0) exports a DCIP descriptor;
    /// despite the historical debug-dialog grouping, this does not create UI.
    fn export_native_input_descriptor(&mut self, output: u32, descriptor: u32) -> VmResult<i32> {
        self.resolve_range(descriptor, 40)?;
        let groups = self.read_int(descriptor, 2)?;
        if !(1..=256).contains(&groups) {
            return Ok(-1);
        }
        let pages = self.read_int(descriptor + 4, 2)?;
        self.resolve_range(pages, groups as usize * 64)?;
        let mut text = String::new();
        let mut total = 0usize;
        for group in 0..groups {
            let page = pages + group * 64;
            let count = self.read_int(page, 2)?;
            // The native copier allocates by LOWORD(count), while its dump
            // loops over the whole DWORD. Reject mismatches rather than read
            // beyond that allocation when upper bits are nonzero.
            if !(1..=1024).contains(&count) {
                return Ok(-1);
            }
            let units = self.read_int(page + 8, 2)?;
            self.resolve_range(units, count as usize * 196)?;
            total += count as usize;
            if total > 65536 {
                return Err(crate::VmError::Runtime(
                    "native descriptor export limit".into(),
                ));
            }
            use std::fmt::Write;
            writeln!(&mut text, "- Group [ {group} ] -\n").unwrap();
            for unit in 0..count {
                let p = units + unit * 196;
                let mut fields = [0i32; 13];
                for (i, n) in fields.iter_mut().enumerate() {
                    *n = self.read_int(p + i as u32 * 4, 2)? as i32;
                }
                let truth = |n: i32| if n != 0 { "TRUE " } else { "FALSE" };
                // MS printf's space flag contributes a leading blank to
                // nonnegative numbers, in addition to the requested width.
                let signed = |n: i32, width: usize| {
                    format!(
                        "{:>width$}",
                        if n >= 0 {
                            format!(" {n}")
                        } else {
                            n.to_string()
                        }
                    )
                };
                write!(
                    &mut text,
                    "\tUnit [ {} ] / Validity : {} / Visibility : {} / ",
                    signed(unit as i32, 3),
                    truth(fields[0]),
                    truth(fields[1])
                )
                .unwrap();
                write!(
                    &mut text,
                    "Position( {}, {} ) / Origin( {}, {} ) / ",
                    signed(fields[2], 4),
                    signed(fields[3], 4),
                    signed(fields[4], 4),
                    signed(fields[5], 4)
                )
                .unwrap();
                writeln!(&mut text,"Bitmaps : {} / ChangeInterval : {} / BaseBitmaps( {}, {}, {}, {} ) / BitmapForVPD : {}",fields[6],fields[7],fields[8],fields[9],fields[10],fields[11],fields[12]).unwrap();
            }
            text.push('\n');
        }
        let length = self.encode_buffer_text(&text).len();
        if output != 0 {
            self.write_c_string(output, &text)?;
        }
        Ok(length as i32)
    }
    pub(super) fn execute_version_diagnostic<A: SysApi>(
        &mut self,
        api: &mut A,
        id: u8,
    ) -> VmResult<bool> {
        let modern = self.native_abi_profile == NativeAbiProfile::ModernFused;
        if !modern && self.native_abi_profile != NativeAbiProfile::Anipani {
            return Ok(false);
        }
        let clear = if modern { 0x90 } else { 0x96 };
        let export = if modern { 0x91 } else { 0x90 };
        let flags = if modern { 0x93 } else { 0x92 };
        let report = if modern { 0x92 } else { 0x94 };
        if modern && id == 0x3f {
            let descriptor = self.pop_ptr()?;
            let output = self.pop_ptr()?;
            let result = self.export_native_input_descriptor(output, descriptor)?;
            self.push_value(Value::Int(result));
        } else if id == clear {
            self.system81_shared
                .lock()
                .expect("system81 state poisoned")
                .diagnostics
                .counts
                .clear();
        } else if id == export {
            let output = self.pop_ptr()?;
            let counts = self
                .system81_shared
                .lock()
                .expect("system81 state poisoned")
                .diagnostics
                .visible_counts();
            if output != 0 {
                self.resolve_write_range(output, counts.len() * 8)?;
                for (n, (key, count)) in counts.iter().enumerate() {
                    self.write_int(output + n as u32 * 8, 2, *key as u32)?;
                    self.write_int(output + n as u32 * 8 + 4, 2, *count)?;
                }
            }
            self.push_value(Value::Int(counts.len() as i32));
        } else if id == flags {
            let flags = self.pop_int()? as u32;
            let key = self.pop_int()? as u32;
            let status = if !registered(self.native_abi_profile, (key >> 8) as u16) {
                3
            } else {
                self.system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .diagnostics
                    .flags
                    .insert(key as u16, flags);
                0
            };
            self.push_value(Value::Int(status));
        } else if id == 0xc0 {
            let enabled = self.pop_int()? != 0;
            self.system81_shared
                .lock()
                .expect("system81 state poisoned")
                .diagnostics
                .allocation_logging = enabled;
        } else if id == report || id == 0xc2 {
            let root = self.pop_int()?;
            let path = self.pop_string_lossy()?;
            if !matches!(root, 0 | 1) {
                self.push_value(Value::Int(1));
                return Ok(true);
            }
            let data = {
                let state = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned");
                if id == report {
                    state
                        .diagnostics
                        .visible_counts()
                        .iter()
                        .map(|(id, n)| format!("0x{id:04X} : {}\n", *n as i32))
                        .collect::<String>()
                } else {
                    let mut values = state.diagnostics.allocations.values().collect::<Vec<_>>();
                    values.sort_by_key(|(sequence, _)| std::cmp::Reverse(*sequence));
                    values
                        .into_iter()
                        .map(|(_, line)| line.as_str())
                        .collect::<String>()
                }
            };
            let base = if root == 0 {
                self.resource_primary_root(api)
            } else {
                api.user_data_root(1).unwrap_or_default()
            };
            let path = if crate::resource_io::is_absolute_resource_path(&path) {
                path
            } else {
                format!("{}{}", crate::ensure_trailing_separator(&base), path)
            };
            let bytes = self.encode_buffer_text(&data);
            self.push_value(Value::Int(if api.write_file_bytes(&path, &bytes) {
                0
            } else {
                2
            }));
        } else {
            return Ok(false);
        }
        Ok(true)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[derive(Default)]
    struct Host {
        writes: Vec<(String, Vec<u8>)>,
    }
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut crate::NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn write_file_bytes(&mut self, path: &str, bytes: &[u8]) -> bool {
            self.writes.push((path.into(), bytes.into()));
            true
        }
        fn primary_resource_root(&mut self) -> Option<String> {
            Some("root".into())
        }
    }
    #[test]
    fn version_input_descriptor_export_measures_then_writes_native_text() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        let mut host = Host::default();
        vm.write_int(256, 2, 1).unwrap();
        vm.write_int(260, 2, 512).unwrap();
        vm.write_int(512, 2, 1).unwrap();
        vm.write_int(520, 2, 768).unwrap();
        for (i, n) in [1, 0, -4, 8, 2, 3, 4, 5, 10, 11, 12, 13, 99]
            .into_iter()
            .enumerate()
        {
            vm.write_int(768 + i as u32 * 4, 2, n as u32).unwrap();
        }
        vm.stack = vec![Value::Ptr(0), Value::Ptr(256)];
        vm.execute_version_diagnostic(&mut host, 0x3f).unwrap();
        let size = vm.stack.pop().unwrap().as_i32();
        assert!(size > 0);
        vm.stack = vec![Value::Ptr(1024), Value::Ptr(256)];
        vm.execute_version_diagnostic(&mut host, 0x3f).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(size)));
        let text = vm.read_c_string(1024).unwrap();
        assert_eq!(text.len(), size as usize);
        assert_eq!(
            text,
            "- Group [ 0 ] -\n\n\tUnit [   0 ] / Validity : TRUE  / Visibility : FALSE / Position(   -4,    8 ) / Origin(    2,    3 ) / Bitmaps : 4 / ChangeInterval : 5 / BaseBitmaps( 10, 11, 12, 13 ) / BitmapForVPD : 99\n\n"
        );
        vm.write_int(512, 2, 0x10001).unwrap();
        vm.stack = vec![Value::Ptr(1024), Value::Ptr(256)];
        vm.execute_version_diagnostic(&mut host, 0x3f).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(-1)));
        assert_eq!(vm.read_c_string(1024).unwrap(), text);
    }
    #[test]
    fn version_diagnostic_counter_filter_and_allocation_lifetime() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        let mut host = Host::default();
        vm.note_call("sys", 0x80, 0x20);
        vm.note_call("sys", 0x80, 0x20);
        vm.note_call("sys", 0x81, 0x80);
        vm.stack = vec![Value::Ptr(256)];
        assert!(vm.execute_version_diagnostic(&mut host, 0x91).unwrap());
        assert_eq!(vm.stack.pop(), Some(Value::Int(2)));
        assert_eq!(vm.read_int(256, 2).unwrap(), 0x8020);
        assert_eq!(vm.read_int(260, 2).unwrap(), 2);
        vm.stack = vec![Value::Int(0x8020), Value::Int(1)];
        vm.execute_version_diagnostic(&mut host, 0x93).unwrap();
        vm.stack = vec![Value::Int(0)];
        vm.execute_version_diagnostic(&mut host, 0x91).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(1)));
        vm.stack = vec![Value::Int(1)];
        vm.execute_version_diagnostic(&mut host, 0xc0).unwrap();
        vm.record_native_allocation(0x40000000, 16);
        vm.record_native_allocation(0x40000010, 32);
        vm.forget_native_allocation(0x40000000);
        vm.stack = vec![Value::Str("alloc.txt".into()), Value::Int(0)];
        vm.execute_version_diagnostic(&mut host, 0xc2).unwrap();
        let text = String::from_utf8_lossy(&host.writes[0].1);
        assert!(text.contains("$40000010"));
        assert!(!text.contains("$40000000"));
        vm.execute_version_diagnostic(&mut host, 0x90).unwrap();
        vm.stack = vec![Value::Int(0)];
        vm.execute_version_diagnostic(&mut host, 0x91).unwrap();
        assert_eq!(vm.stack.pop(), Some(Value::Int(0)));
    }
}
