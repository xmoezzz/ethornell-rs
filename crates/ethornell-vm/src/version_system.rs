//! BGI 1.653 / 1.687 system-table extensions. Native buffers remain guest
//! memory; strings are converted without exposing host pointers.
use crate::*;

/// 49E690 / 1400F94B0: a bounded, deliberately non-strict UTF-8 probe.
/// ASCII is ambiguous (80000000); a non-UTF-8 prefix selects CP932 (0).
/// The original stops after three/six multi-byte sequences, even if a later
/// suffix would be invalid. This is not Rust's whole-buffer UTF-8 validation.
pub(super) fn encoding_kind(bytes: &[u8], limit: usize) -> i32 {
    let (mut at, mut count) = (0, 0);
    while let Some(&byte) = bytes.get(at).filter(|&&b| b != 0) {
        if byte < 0x80 {
            at += 1;
            continue;
        }
        if byte < 0xc2 {
            return 0;
        }
        let width = byte.leading_ones() as usize;
        if width < 2
            || bytes
                .get(at + 1..at + width)
                .is_none_or(|tail| tail.iter().any(|b| b & 0xc0 != 0x80))
        {
            return 0;
        }
        count += 1;
        if count >= limit {
            return 1;
        }
        at += width;
    }
    if count == 0 { i32::MIN } else { 1 }
}

pub(super) fn decode_auto(bytes: &[u8], limit: usize) -> String {
    let encoding = if encoding_kind(bytes, limit) == 0 {
        encoding_rs::SHIFT_JIS
    } else {
        encoding_rs::UTF_8
    };
    encoding.decode(bytes).0.into_owned()
}

/// WideCharToMultiByte(CP932) uses the default '?' byte for unmappable
/// characters. encoding_rs::encode alone would emit HTML numeric entities.
pub(super) fn encode_cp932(text: &str) -> Vec<u8> {
    let mut output = Vec::new();
    for ch in text.chars() {
        let mut buffer = [0; 4];
        let (bytes, _, error) = encoding_rs::SHIFT_JIS.encode(ch.encode_utf8(&mut buffer));
        if error {
            output.push(b'?');
        } else {
            output.extend_from_slice(&bytes);
        }
    }
    output
}

/// 4F4640 encodes the original unsigned delimiter, including the older
/// five/six-byte UTF-8 forms. Rust char rejects those native input values.
pub(super) fn encode_utf8_codepoint(value: u32) -> Vec<u8> {
    let width = match value {
        0..=0x7f => return vec![value as u8],
        0x80..=0x7ff => 2,
        0x800..=0xffff => 3,
        0x10000..=0x1fffff => 4,
        0x200000..=0x3ffffff => 5,
        0x4000000..=0x7fffffff => 6,
        _ => return Vec::new(),
    };
    let mut bytes = vec![(0xffu8 << (8 - width)) | (value >> (6 * (width - 1))) as u8];
    for shift in (0..width - 1).rev() {
        bytes.push(0x80 | ((value >> (6 * shift)) & 0x3f) as u8);
    }
    bytes
}

fn utf8_character(bytes: &[u8]) -> VmResult<(u32, usize)> {
    let first = bytes.first().copied().unwrap_or(0);
    if first < 0x80 { return Ok((u32::from(first), 1)); }
    let width = first.leading_ones() as usize;
    if !(2..=6).contains(&width) || bytes.get(1..width)
        .is_none_or(|tail| tail.iter().any(|b| b & 0xc0 != 0x80)) {
        return Err(VmError::Runtime("invalid native UTF-8 character".into()));
    }
    let mut value = u32::from(first & ((1 << (7 - width)) - 1));
    for &byte in &bytes[1..width] { value = (value << 6) | u32::from(byte & 0x3f); }
    Ok((value, width))
}

/// 49EE40 / 1400F61C0: consumed byte count, native character value and
/// full-cell flag. UTF-8 also accepts the target's CESU-8 surrogate pairs.
pub(super) fn character_info(bytes: &[u8], kind: i32) -> VmResult<(usize, u32, bool)> {
    let first = bytes.first().copied().unwrap_or(0);
    match kind {
        1 => {
            let (mut value, mut width) = utf8_character(bytes)?;
            if (0xd800..=0xdbff).contains(&value) && width < bytes.len() {
                let (low, tail) = utf8_character(&bytes[width..])?;
                if (0xdc00..=0xdfff).contains(&low) {
                    value = 0x10000 + ((value - 0xd800) << 10) + low - 0xdc00;
                    width += tail;
                }
            }
            Ok((width, value, value.wrapping_sub(128) <= 0xfee0 || value >= 0xffa0))
        }
        i32::MIN => Ok((1, u32::from(first), false)),
        _ => {
            // The native CP932 lead test includes FF, but excludes FD/FE.
            let double = first.wrapping_add(96) > 0x3f && first > 0x80 && !matches!(first, 0xfd | 0xfe);
            let value = if double { (u32::from(first) << 8) | u32::from(bytes.get(1).copied().unwrap_or(0)) }
                else { u32::from(first) };
            Ok((if double { 2 } else { 1 }, value, double))
        }
    }
}

pub(super) fn unicode_delimiter(value: u32) -> bool {
    // The UTF-16 delimiter table referenced by 140068D70.
    ",.､｡:;?!ﾞﾟ･、。，．：；？！゛゜‐]})）〕］｝〉》」』】‘“".chars().any(|ch| ch as u32 == value)
}

/// 49EF90 / 1400F5E20 compares native characters and returns a BYTE
/// offset. It resets after a mismatch without retrying an overlapping start.
pub(super) fn find_native_string(haystack: &[u8], needle: &[u8], kind: i32) -> VmResult<Option<usize>> {
    // The original multibyte helper reads an uninitialized item for an empty
    // needle. Define the portable result consistently with its ASCII strstr.
    if needle.is_empty() { return Ok(Some(0)); }
    if kind == i32::MIN {
        return Ok(haystack.windows(needle.len()).position(|part| part == needle));
    }
    let mut characters = Vec::new();
    let mut at = 0;
    while at < needle.len() {
        let (width, value, _) = character_info(&needle[at..], kind)?;
        characters.push(value);
        at += width;
    }
    let (mut at, mut matched, mut start) = (0, 0, 0);
    while at < haystack.len() {
        let (width, value, _) = character_info(&haystack[at..], kind)?;
        if value == characters[matched] {
            if matched == 0 { start = at; }
            matched += 1;
            if matched == characters.len() { return Ok(Some(start)); }
        } else { matched = 0; }
        at += width;
    }
    Ok(None)
}

pub(super) fn replace_native_string(haystack: &[u8], needle: &[u8], replacement: &[u8], limit: usize, utf8: bool) -> VmResult<(Vec<u8>, i32)> {
    // The native empty-needle replacement loop never advances. Keep the
    // portable runtime's defined no-op instead of reproducing that fault.
    if needle.is_empty() { return Ok((haystack.to_vec(), 0)); }
    let kind = encoding_kind(haystack, limit);
    let target_utf8 = if kind == i32::MIN { utf8 } else { kind == 1 };
    let replacement_kind = encoding_kind(replacement, limit);
    let converted = if replacement_kind == i32::MIN || (replacement_kind == 1) == target_utf8 {
        replacement.to_vec()
    } else {
        let text = decode_auto(replacement, limit);
        if target_utf8 { text.into_bytes() } else { encode_cp932(&text) }
    };
    let (mut output, mut rest, mut count) = (Vec::new(), haystack, 0);
    while let Some(at) = find_native_string(rest, needle, kind)? {
        output.extend_from_slice(&rest[..at]);
        output.extend_from_slice(&converted);
        rest = rest.get(at + needle.len()..).ok_or_else(|| VmError::Runtime("native replacement span exceeds source".into()))?;
        count += 1;
    }
    output.extend_from_slice(rest);
    Ok((output, count))
}

impl Vm {
    pub(super) fn try_version_system<A: SysApi>(
        &mut self,
        api: &mut A,
        group: u8,
        id: u16,
    ) -> VmResult<Option<Value>> {
        let legacy_conversion = group == 0x81 && id == 0x21
            && matches!(self.native_abi_profile, NativeAbiProfile::HoukagoFd | NativeAbiProfile::Modern);
        if !legacy_conversion && !matches!(
            self.native_abi_profile,
            NativeAbiProfile::Anipani | NativeAbiProfile::ModernFused
        ) {
            return Ok(None);
        }
        let modern = self.native_abi_profile == NativeAbiProfile::ModernFused;
        let probe_limit = if modern { 6 } else { 3 };
        Ok(Some(match (group, id) {
            (0x80, 0xec..=0xee) => self.execute_native_extension(api,id)?,
            (0x81, 0xb8|0xb9) => Value::Int(self.search_native_records(id == 0xb9)?),
            (0x81, 0x34) if modern => {
                // EC050 -> DCProcExamineHealthOfFile. The null archive is 9;
                // an absent file is 1, before installing the native procedure.
                let file=self.pop_string_lossy()?;
                let archive=self.pop_value()?;
                if matches!(archive,Value::None|Value::Int(0)|Value::Ptr(0)) {Value::Int(9)} else {
                    let archive=self.value_as_string_lossy(archive)?;
                    if !api.file_exists(&archive,&file) {Value::Int(1)} else {
                        let bytes=api.read_archive_entry_raw(&archive,&file)
                            .or_else(||api.read_disk_file(&crate::join_native_path(&archive,&file)))
                            .or_else(||api.load_file_bytes(&archive,&file));
                        let expected=self.system81_shared.lock().expect("system81 state poisoned").resource_checksums
                            .get(&(native_checksum_key(&archive),native_checksum_key(&file))).copied();
                        let status=match (bytes,expected) {
                            (None,_)=>3,(_,None)=>10,
                            (Some(bytes),Some(expected))=>if crate::native_file_hash(&bytes)==expected {0} else {11},
                        };
                        self.install_host_completed_procedure(crate::NativeOpcode {group,id},crate::native_call::NativeProcedureCompletion {
                            class:crate::native_call::NativeProcedureClass::ExamineHealthOfFile,status,outputs:[status,0],output_count:1,
                        },false);
                        Value::None
                    }
                }
            }
            (0x81, 0x66) if modern => {
                // B6880 / B68F0: WS_SIZEBOX is the only differing style bit.
                let enabled=self.pop_int()?!=0;api.configure_window_resizable(enabled);Value::None
            }
            (0x81, 0x67) if modern => {
                let output=self.pop_ptr()?;
                let token=api.portable_window_identity();
                self.resolve_write_range(output,8)?;
                self.write_int(output,2,token as u32)?;
                self.write_int(output.wrapping_add(4),2,(token>>32) as u32)?;Value::None
            }
            (0x81, 0x6c) if modern => {
                // FFF60 validates the ratio, then updates the output client size.
                let height=self.pop_int()?;let width=self.pop_int()?;
                let (native_width,native_height)=api.runtime_screen_dimensions();
                let (width,height)=if width<=0||height<=0 {(native_width,native_height)} else {(width,height)};
                let valid=width>0&&height>0&&native_width>0&&native_height>0
                    && ((native_height as f32/native_width as f32)-(height as f32/width as f32)).abs()<=0.001;
                Value::Int(i32::from(valid&&api.configure_presentation_size(width,height)))
            }
            (0x81, 0x80) => {
                let path = self.pop_string_lossy()?;
                let user_size = self.pop_ptr()?;
                let user = self.pop_ptr()?;
                let config_size = self.pop_ptr()?;
                let config = self.pop_ptr()?;
                Value::Int(i32::from(self.restore_gdb_buffers(api, config, config_size, user, user_size, &path)?))
            }
            (0x81, 0x8c..=0x8f) if modern => {
                // 1400F7080..F70B0 are original unconditional 80000004
                // failure routines. EB480..EB600 map that status to 1.
                if matches!(id,0x8c|0x8e) { self.pop_int()?; } else { self.pop_ptr()?; }
                self.pop_ptr()?;
                Value::Int(1)
            }
            (0x80, 0x9f) => {
                let mode = self.pop_int()? as u32;
                let handle = self.pop_int()? as u32;
                Value::Int(self.sys_indexed_record_mode(handle, mode))
            }
            (0x80, 0x03) => {
                // 496B40 / 1400ED920, latest frame first, up to 600 DWORDs.
                let count = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                if !(1..=600).contains(&count) {
                    Value::Int(0)
                } else {
                    let values = api.native_frame_history(count as usize).ok_or_else(|| {
                        VmError::Runtime("host frame history is unavailable".into())
                    })?;
                    if values.len() != count as usize {
                        return Err(VmError::Runtime("invalid host frame history length".into()));
                    }
                    self.resolve_write_range(output, values.len() * 4)?;
                    for (i, value) in values.into_iter().enumerate() {
                        self.write_int(output.wrapping_add(i as u32 * 4), 2, value as u32)?;
                    }
                    Value::Int(1)
                }
            }
            (0x81, 0x06) if modern => {
                // 1400ECD10: rand_s, independent of the CRT rand stream.
                let bound = self.pop_int()?;
                if bound == 0 {
                    Value::Int(0)
                } else {
                    let random = api.secure_random_u32()?;
                    let result = (random % bound.unsigned_abs()) as i32;
                    Value::Int(if bound < 0 {
                        result.wrapping_neg()
                    } else {
                        result
                    })
                }
            }
            (0x81, 0x20) => {
                // 494DD0 returns success; 1400EC6F0 returns the payload
                // length and additionally accepts UTF-16 (mode 2).
                let mode = self.pop_int()?;
                let bytes = self.pop_native_string_bytes()?;
                let output = self.pop_ptr()?;
                if !matches!(mode, -1..=1) && !(modern && mode == 2) {
                    Value::Int(0)
                } else {
                    let text = decode_auto(&bytes, probe_limit);
                    let (bytes, length) = if mode == 2 {
                        let words: Vec<u16> = text.encode_utf16().collect();
                        let length = words.len();
                        let bytes = words
                            .into_iter()
                            .chain([0])
                            .flat_map(u16::to_le_bytes)
                            .collect();
                        (bytes, length)
                    } else {
                        let utf8 = mode == 1 || (mode == -1 && self.native_utf8_enabled());
                        let kind = encoding_kind(&bytes, probe_limit);
                        let mut encoded = if kind == i32::MIN || (kind == 1) == utf8 {
                            bytes.clone()
                        } else if utf8 {
                            text.as_bytes().to_vec()
                        } else {
                            encode_cp932(&text)
                        };
                        let length = encoded.len();
                        encoded.push(0);
                        (encoded, length)
                    };
                    if output != 0 {
                        let range = self.resolve_write_range(output, bytes.len())?;
                        self.memory[range].copy_from_slice(&bytes);
                        self.clear_shadow_values(output, bytes.len());
                    }
                    Value::Int(if modern { length as i32 } else { 1 })
                }
            }
            (0x81, 0x21) => {
                // 494E20 ->49ECC0 / 1400EC6B0 ->F7D30 always CP932->UTF8.
                let bytes = self.pop_native_string_bytes()?;
                let output = self.pop_ptr()?;
                let text = encoding_rs::SHIFT_JIS.decode(&bytes).0.into_owned();
                self.write_version_utf8(output, &text)?;
                Value::None
            }
            (0x81, 0x27) => {
                Value::Int(encoding_kind(&self.pop_native_string_bytes()?, probe_limit))
            }
            (0x81, 0xd8) => {
                // 4A2EF0 / 1400FDAD0: registration replaces named groups,
                // retaining other groups. Null clears, regardless of length.
                let length = self.pop_int()?;
                let input = self.pop_ptr()?;
                if input == 0 {
                    self.system81_shared
                        .lock()
                        .expect("system81 state poisoned")
                        .sentence_table = Default::default();
                    Value::Int(1)
                } else if length < 0 || !sentence_table::SentenceTable::valid_size(length as usize)
                {
                    Value::Int(0)
                } else {
                    let range = self.resolve_range(input, length as usize)?;
                    // Unlike the original unchecked strlen walks, a malformed
                    // buffer cannot publish partially parsed guest memory.
                    if let Some(table) =
                        sentence_table::SentenceTable::parse_native(&self.memory[range])
                    {
                        self.system81_shared
                            .lock()
                            .expect("system81 state poisoned")
                            .sentence_table
                            .merge(table);
                        Value::Int(1)
                    } else {
                        Value::Int(0)
                    }
                }
            }
            (0x81, 0xda) => {
                // 4A3100 / 1400FDA40 normalize both lookup keys to UTF-8.
                let key = decode_auto(&self.pop_native_string_bytes()?, probe_limit);
                let group = decode_auto(&self.pop_native_string_bytes()?, probe_limit);
                let output = self.pop_ptr()?;
                let text = self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .sentence_table
                    .get(&group, &key)
                    .map(str::to_owned);
                if let Some(text) = text {
                    if output != 0 {
                        self.write_version_utf8(output, &text)?;
                    }
                    Value::Int(text.len() as i32)
                } else {
                    Value::Int(0)
                }
            }
            _ => return Ok(None),
        }))
    }

    fn write_version_utf8(&mut self, output: u32, text: &str) -> VmResult<()> {
        let range = self.resolve_write_range(output, text.len() + 1)?;
        self.memory[range.start..range.end - 1].copy_from_slice(text.as_bytes());
        self.memory[range.end - 1] = 0;
        self.clear_shadow_values(output, text.len() + 1);
        self.mem_values
            .insert(self.value_key(output), Value::Str(text.into()));
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    struct Host;
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn native_frame_history(&self, n: usize) -> Option<Vec<i32>> {
            Some(
                [17, 16, 15]
                    .into_iter()
                    .chain(std::iter::repeat(0))
                    .take(n)
                    .collect(),
            )
        }
        fn secure_random_u32(&mut self) -> VmResult<u32> {
            Ok(0x8000_0001)
        }
    }
    fn call(vm: &mut Vm, id: u16, args: Vec<Value>) -> Value {
        vm.stack = args;
        vm.try_builtin_sys_with_api(&mut Host, 0x81, id)
            .unwrap()
            .unwrap()
    }
    #[test]
    fn pre_fused_conversion_has_a_two_argument_zero_result_contract() {
        for profile in [NativeAbiProfile::HoukagoFd, NativeAbiProfile::Modern] {
            let mut vm = Vm::new();
            vm.set_native_abi_profile(profile);
            let opcode = NativeOpcode { group: 0x81, id: 0x21 };
            assert!(vm.effective_strict_support(opcode));
            assert_eq!(vm.effective_arg_count(0x81, 0x21), Some(2));
            assert_eq!(vm.effective_output_count(0x81, 0x21), 0);
            let bytes = encoding_rs::SHIFT_JIS.encode("ＭＳ ゴシック").0.into_owned();
            for (offset, byte) in bytes.into_iter().chain([0]).enumerate() {
                vm.write_int(256 + offset as u32, 0, u32::from(byte)).unwrap();
            }
            vm.stack = vec![Value::Int(77), Value::Ptr(512), Value::Ptr(256)];
            assert_eq!(vm.try_builtin_sys_with_api(&mut Host, 0x81, 0x21).unwrap(), Some(Value::None));
            assert_eq!(vm.stack, [Value::Int(77)]);
            assert_eq!(vm.read_c_string_bytes(512).unwrap(), "ＭＳ ゴシック".as_bytes());
        }
    }
    #[test]
    fn version_system_encoding_probe_conversion_and_versioned_results() {
        assert_eq!(encoding_kind(b"ASCII", 3), i32::MIN);
        assert_eq!(encoding_kind(&[0x82, 0xa0], 3), 0);
        let mut prefix = "日本語".as_bytes().to_vec();
        prefix.push(0xff);
        assert_eq!(encoding_kind(&prefix, 3), 1);
        assert_eq!(encoding_kind(&prefix, 6), 0);
        assert_eq!(encode_cp932("😀"), b"?");
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        let bytes = encoding_rs::SHIFT_JIS.encode("日本").0.into_owned();
        for (i, b) in bytes.iter().chain([0].iter()).enumerate() {
            vm.write_int(256 + i as u32, 0, *b as u32).unwrap();
        }
        assert_eq!(
            call(
                &mut vm,
                0x20,
                vec![Value::Ptr(300), Value::Ptr(256), Value::Int(1)]
            ),
            Value::Int(6)
        );
        assert_eq!(
            vm.memory[vm.resolve_range(300, 7).unwrap()],
            *"日本\0".as_bytes()
        );
        assert_eq!(
            call(
                &mut vm,
                0x20,
                vec![Value::Int(0), Value::Ptr(256), Value::Int(2)]
            ),
            Value::Int(2)
        );
        assert_eq!(
            call(
                &mut vm,
                0x20,
                vec![Value::Ptr(320), Value::Ptr(256), Value::Int(2)]
            ),
            Value::Int(2)
        );
        assert_eq!(vm.read_int(320, 1).unwrap(), 0x65e5);
        assert_eq!(vm.read_int(322, 1).unwrap(), 0x672c);
        assert_eq!(vm.read_int(324, 1).unwrap(), 0);
        vm.set_native_abi_profile(NativeAbiProfile::Anipani);
        assert_eq!(
            call(
                &mut vm,
                0x20,
                vec![Value::Int(0), Value::Ptr(256), Value::Int(1)]
            ),
            Value::Int(1)
        );
        assert_eq!(
            call(&mut vm, 0x21, vec![Value::Ptr(340), Value::Ptr(256)]),
            Value::None
        );
        assert_eq!(vm.read_shadowed_c_string(340).unwrap(), "日本");
        assert_eq!(call(&mut vm, 0x00, vec![Value::Int(1)]), Value::Int(1));
    }
    #[test]
    fn version_system_history_entropy_and_incremental_sentence_groups() {
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        vm.stack = vec![Value::Ptr(256), Value::Int(4)];
        assert_eq!(
            vm.try_version_system(&mut Host, 0x80, 3).unwrap(),
            Some(Value::Int(1))
        );
        assert_eq!(
            (0..4)
                .map(|i| vm.read_int(256 + i * 4, 2).unwrap())
                .collect::<Vec<_>>(),
            [17, 16, 15, 0]
        );
        assert_eq!(call(&mut vm, 6, vec![Value::Int(-10)]), Value::Int(-9));
        assert_eq!(call(&mut vm, 6, vec![Value::Int(i32::MIN)]), Value::Int(-1));
        for (group, text) in [("one", "first"), ("two", "second"), ("one", "replacement")] {
            let mut bytes = 1u32.to_le_bytes().to_vec();
            bytes.extend(group.as_bytes());
            bytes.push(0);
            bytes.extend(1u32.to_le_bytes());
            bytes.extend(b"key\0");
            bytes.extend(text.as_bytes());
            bytes.push(0);
            let range = vm.resolve_write_range(512, bytes.len()).unwrap();
            vm.memory[range].copy_from_slice(&bytes);
            assert_eq!(
                call(
                    &mut vm,
                    0xd8,
                    vec![Value::Ptr(512), Value::Int(bytes.len() as i32)]
                ),
                Value::Int(1)
            );
        }
        assert_eq!(
            call(
                &mut vm,
                0xda,
                vec![
                    Value::Ptr(1024),
                    Value::Str("one".into()),
                    Value::Str("key".into())
                ]
            ),
            Value::Int(11)
        );
        assert_eq!(vm.read_shadowed_c_string(1024).unwrap(), "replacement");
        assert_eq!(
            call(
                &mut vm,
                0xda,
                vec![
                    Value::Int(0),
                    Value::Str("two".into()),
                    Value::Str("key".into())
                ]
            ),
            Value::Int(6)
        );
        assert_eq!(
            call(&mut vm, 0xd8, vec![Value::Int(0), Value::Int(42)]),
            Value::Int(1)
        );
        assert_eq!(
            call(
                &mut vm,
                0xda,
                vec![
                    Value::Int(0),
                    Value::Str("two".into()),
                    Value::Str("key".into())
                ]
            ),
            Value::Int(0)
        );
    }
}

fn native_checksum_key(path:&str)->String {path.replace('\\',"/").to_lowercase()}
impl Vm {
    /// Installs the owned counterpart of CArchive entry+104. Native archives
    /// may have no integrity record (81:34 returns 10), which is distinct from
    /// a matching checksum. No on-disk value is treated as a process pointer.
    pub fn register_native_resource_checksum(&mut self,archive:&str,file:&str,checksum:Option<[u32;2]>) {
        let key=(native_checksum_key(archive),native_checksum_key(file));
        let mut state=self.system81_shared.lock().expect("system81 state poisoned");
        if let Some(checksum)=checksum {state.resource_checksums.insert(key,checksum);} else {state.resource_checksums.remove(&key);}
    }
}
