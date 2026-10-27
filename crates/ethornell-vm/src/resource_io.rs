//! Resource file I/O (Sys80:30..3F), ported from BGI 1.622; the cleaned
//! target code is src/engine/resource_io.c.

use super::*;

/// sub_465AE0: the file is missing (or its drive has no media).
const RESOURCE_MISSING: i32 = 1;
/// sub_465C20 archive status codes.
const ARCHIVE_NOT_FOUND: u32 = 0x8000_0010;
const ARCHIVE_NO_ENTRY: u32 = 0x8000_0020;
const ARCHIVE_RANGE: u32 = 0x8000_0030;
const ARCHIVE_LENGTH: u32 = 0x8000_0040;
const ARCHIVE_READ: u32 = 0x8000_0050;
const ARCHIVE_TOO_BIG: u32 = 0x8000_0060;
const RESOURCE_SIZE_LIMIT: usize = 0x0400_0000;

/// sub_464EE0, extended for absolute paths returned by POSIX hosts.
pub(crate) fn is_absolute_resource_path(path: &str) -> bool {
    let bytes = path.as_bytes();
    path.starts_with(['/', '\\']) || bytes.get(1) == Some(&b':')
}

/// sub_470070(src, dst, n): copies `src` up to and including its n-th
/// backslash (DBCS aware); `None` when `src` has fewer components. For
/// n > 0 with text left over the last copied byte (that backslash) is
/// dropped again.
fn copy_path_components(src: &[u8], n: i32) -> Option<Vec<u8>> {
    let mut remaining = n;
    let mut index = 0;
    let mut out = Vec::new();
    let at = |i: usize| src.get(i).copied().unwrap_or(0);
    while remaining >= 0 && at(index) != 0 {
        let byte = at(index);
        if byte == b'\\' && at(index + 1) != 0 {
            remaining -= 1;
        }
        out.push(byte);
        if byte >= 0x80 && (byte < 0xa0 || byte >= 0xe0) {
            out.push(at(index + 1));
            index += 2;
        } else {
            index += 1;
        }
        if at(index) == 0 {
            remaining -= 1;
        }
    }
    if remaining >= 0 {
        return None;
    }
    if n > 0 && at(index) != 0 {
        out.pop();
    }
    Some(out)
}

/// sub_465280: the longest component count that copies, minus two — the
/// marker's directory part ("A\\b" -> "A\\", "A\\B\\c" -> "A\\B", "c" -> "").
fn marker_directory_prefix(marker: &[u8]) -> Vec<u8> {
    let mut count = 0;
    while copy_path_components(marker, count).is_some() {
        count += 1;
    }
    copy_path_components(marker, count - 2).unwrap_or_default()
}

/// sub_465880: decode a loaded file (DSC, CBG v1/v2 into the 16-byte header
/// plus pixels, otherwise raw) and take `length` bytes at `offset`; both 0
/// select the whole result. Errors are the target statuses 2/3/5/6.
pub(crate) fn target_decode_resource(bytes: &[u8], offset: u32, length: u32) -> Result<Vec<u8>, i32> {
    let decoded: Cow<'_, [u8]> = if bytes.starts_with(b"DSC FORMAT 1.00") {
        let size = bytes
            .get(20..24)
            .map(|v| u32::from_le_bytes(v.try_into().unwrap()) as usize)
            .unwrap_or(0);
        if size > RESOURCE_SIZE_LIMIT {
            return Err(6);
        }
        match ethornell_archive::dsc_decode(bytes) {
            Ok(decoded) if decoded.len() == size => Cow::Owned(decoded),
            _ => return Err(5),
        }
    } else if bytes.starts_with(b"CompressedBG___") {
        match ethornell_image::decode_cbg_native_buffer(bytes) {
            Ok(decoded) => Cow::Owned(decoded),
            Err(_) => return Err(5),
        }
    } else {
        Cow::Borrowed(bytes)
    };
    let total = decoded.len() as u32;
    let count = if offset == 0 && length == 0 { total } else { length };
    if count == 0 || count > total {
        return Err(3);
    }
    match offset.checked_add(count) {
        Some(end) if end <= total => Ok(decoded[offset as usize..end as usize].to_vec()),
        _ => Err(2),
    }
}

impl Vm {
    /// ::Buffer: the primary root with its trailing backslash.
    pub(crate) fn resource_primary_root<A: SysApi>(&self, api: &mut A) -> String {
        self.primary_resource_root
            .clone()
            .or_else(|| api.primary_resource_root())
            .map(|root| ensure_trailing_separator(&root))
            .unwrap_or_default()
    }

    /// byte_517C18: the removable-media root, empty when none was found.
    pub(crate) fn resource_media_root(&self) -> String {
        self.secondary_resource_root.clone().unwrap_or_default()
    }

    /// sub_465AE0 + sub_465880 for one resolved path. A relative path (an
    /// empty media root) fails sub_4650E0 and counts as missing.
    fn read_resource_path<A: SysApi>(
        &self,
        api: &mut A,
        path: &str,
        offset: u32,
        length: u32,
    ) -> Result<Vec<u8>, i32> {
        if !is_absolute_resource_path(path) && !std::path::Path::new(path).is_absolute() {
            return Err(RESOURCE_MISSING);
        }
        let bytes = api.read_disk_file(path).ok_or(RESOURCE_MISSING)?;
        if bytes.len() > RESOURCE_SIZE_LIMIT {
            return Err(6);
        }
        target_decode_resource(&bytes, offset, length)
    }

    /// sub_465E30: an absolute name as is, otherwise root + name and then, on
    /// a miss, every search directory (newest first) while Sys80:36 is on.
    pub(crate) fn read_loose_resource<A: SysApi>(
        &self,
        api: &mut A,
        root: &str,
        file: &str,
        offset: u32,
        length: u32,
    ) -> Result<Vec<u8>, i32> {
        if is_absolute_resource_path(file) {
            return self.read_resource_path(api, file, offset, length);
        }
        let mut result = self.read_resource_path(api, &format!("{root}{file}"), offset, length);
        if self.additional_resource_search_enabled {
            for directory in &self.additional_resource_paths {
                if result != Err(RESOURCE_MISSING) {
                    break;
                }
                result = self.read_resource_path(
                    api,
                    &format!("{root}{directory}\\{file}"),
                    offset,
                    length,
                );
            }
        }
        result
    }

    /// sub_4669A0: root + name, then the search directories, exists as a
    /// non-directory.
    fn loose_resource_exists<A: SysApi>(&self, api: &mut A, root: &str, file: &str) -> bool {
        if !is_absolute_resource_path(root) && !std::path::Path::new(root).is_absolute() {
            return false;
        }
        if api.disk_file_exists(&format!("{root}{file}")) {
            return true;
        }
        self.additional_resource_search_enabled
            && self
                .additional_resource_paths
                .iter()
                .any(|directory| api.disk_file_exists(&format!("{root}{directory}\\{file}")))
    }

    /// The archive file named by `archive` below `root` (sub_406AF0 /
    /// sub_406A70 through the archive cache), or the members of a composite
    /// archive registered by Sys80:38.
    fn archive_members(&self, archive: &str) -> Vec<String> {
        self.composite_archives
            .get(archive)
            .cloned()
            .unwrap_or_else(|| vec![archive.to_string()])
    }

    /// sub_465C20: entry names of 0x60 bytes or more are fatal; a missing
    /// archive or entry is ARCHIVE_NOT_FOUND / ARCHIVE_NO_ENTRY; decode
    /// errors map to the archive codes.
    fn read_archive_resource<A: SysApi>(
        &self,
        api: &mut A,
        from_media: bool,
        archive: &str,
        file: &str,
        offset: u32,
        length: u32,
    ) -> VmResult<Result<Vec<u8>, u32>> {
        if self.encode_native_string(file).len() >= 0x60 {
            return Err(VmError::Runtime(format!(
                "指定されたファイル名 [ {file} ] は95文字を超えています"
            )));
        }
        // Only the primary root holds indexed archives on portable hosts; a
        // media root found by Sys80:3F has none. An empty (non-null) name
        // names the root directory itself, which is no archive.
        if archive.is_empty() || from_media {
            return Ok(Err(ARCHIVE_NOT_FOUND));
        }
        let Some(bytes) = self
            .archive_members(archive)
            .iter()
            .find_map(|member| api.read_archive_entry_raw(member, file))
        else {
            return Ok(Err(ARCHIVE_NO_ENTRY));
        };
        if bytes.len() > RESOURCE_SIZE_LIMIT {
            return Ok(Err(ARCHIVE_TOO_BIG));
        }
        Ok(target_decode_resource(&bytes, offset, length).map_err(|status| match status {
            2 => ARCHIVE_RANGE,
            3 => ARCHIVE_LENGTH,
            6 => ARCHIVE_TOO_BIG,
            _ => ARCHIVE_READ,
        }))
    }

    /// sub_465F80: without a media root the message is a script error;
    /// otherwise the media prompt loops until the player quits.
    fn resource_media_prompt<A: SysApi>(&self, api: &mut A, message: String) -> VmResult<()> {
        if self.resource_media_root().is_empty() {
            return Err(VmError::Runtime(message));
        }
        if api.media_prompt(&self.media_prompt_title, &self.media_prompt_text) {
            Ok(())
        } else {
            Err(VmError::Runtime("application exit requested at the media prompt".into()))
        }
    }

    /// Sys80:30 (sub_489250 -> sub_466010): pops file, archive, destination.
    pub(crate) fn sys80_30_read_file_bytes<A>(&mut self, api: &mut A) -> VmResult<Value>
    where
        A: SysApi,
    {
        let file = self.pop_string_lossy()?;
        let archive = self.pop_value()?;
        let destination = self.pop_ptr()?;
        let archive = match archive {
            Value::Int(0) | Value::Ptr(0) | Value::None => None,
            other => Some(self.value_as_string_lossy(other)?),
        };
        let header = self.resolve_write_range(destination, 16)?;
        self.memory[header].fill(0);
        self.clear_shadow_values(destination, 16);

        let primary = self.resource_primary_root(api);
        let mut loaded = self.read_loose_resource(api, &primary, &file, 0, 0).ok();
        if let Some(archive) = archive.as_deref() {
            if loaded.is_none() {
                let mut result = self.read_archive_resource(api, false, archive, &file, 0, 0)?;
                while matches!(result, Err(ARCHIVE_NOT_FOUND | ARCHIVE_NO_ENTRY)) {
                    let media = self.resource_media_root();
                    if !media.is_empty() {
                        result = self.read_archive_resource(api, true, archive, &file, 0, 0)?;
                    }
                    if matches!(result, Err(ARCHIVE_NOT_FOUND | ARCHIVE_NO_ENTRY)) {
                        self.resource_media_prompt(
                            api,
                            format!("指定されたファイル [ {archive} : {file} ] は存在しません"),
                        )?;
                    }
                }
                loaded = result.ok();
            }
        } else {
            while loaded.is_none() {
                let media = self.resource_media_root();
                loaded = self.read_loose_resource(api, &media, &file, 0, 0).ok();
                if loaded.is_none() {
                    self.resource_media_prompt(
                        api,
                        format!("指定されたファイル [ {file} ] は存在しません"),
                    )?;
                }
            }
        }
        let Some(bytes) = loaded else {
            return Ok(Value::Int(0));
        };
        let range = self.resolve_write_range(destination, bytes.len())?;
        self.memory[range].copy_from_slice(&bytes);
        self.clear_shadow_values(destination, bytes.len());
        tracing::info!(?archive, file, size = bytes.len(), "Sys80_30_ReadFileBytes");
        Ok(Value::Int(bytes.len() as i32))
    }

    /// Sys80:31 (sub_489290 -> sub_466190): pops length, offset, file,
    /// archive, destination; returns a status (0, 1 missing, 2, 3, 5, 6).
    pub(crate) fn sys80_31_read_file_range<A>(&mut self, api: &mut A) -> VmResult<Value>
    where
        A: SysApi,
    {
        let length = self.pop_int()? as u32;
        let offset = self.pop_int()? as u32;
        let file = self.pop_string_lossy()?;
        let archive = self.pop_value()?;
        let destination = self.pop_ptr()?;
        let archive = match archive {
            Value::Int(0) | Value::Ptr(0) | Value::None => None,
            other => Some(self.value_as_string_lossy(other)?),
        };
        let primary = self.resource_primary_root(api);
        let mut result = self.read_loose_resource(api, &primary, &file, offset, length);
        if result == Err(RESOURCE_MISSING) {
            let media = self.resource_media_root();
            result = match archive.as_deref() {
                None => self.read_loose_resource(api, &media, &file, offset, length),
                Some(archive) => {
                    let mut status =
                        self.read_archive_resource(api, false, archive, &file, offset, length)?;
                    if matches!(status, Err(ARCHIVE_NOT_FOUND | ARCHIVE_NO_ENTRY)) && !media.is_empty() {
                        status =
                            self.read_archive_resource(api, true, archive, &file, offset, length)?;
                    }
                    status.map_err(|code| match code {
                        ARCHIVE_NOT_FOUND | ARCHIVE_NO_ENTRY => 1,
                        ARCHIVE_RANGE => 2,
                        ARCHIVE_LENGTH => 3,
                        ARCHIVE_READ => 5,
                        _ => 6,
                    })
                }
            };
        }
        let bytes = match result {
            Ok(bytes) => bytes,
            Err(status) => return Ok(Value::Int(status)),
        };
        let range = self.resolve_write_range(destination, bytes.len())?;
        self.memory[range].copy_from_slice(&bytes);
        self.clear_shadow_values(destination, bytes.len());
        Ok(Value::Int(0))
    }

    /// Sys80:32 (sub_4892F0 -> sub_466390): pops length, buffer, then file.
    /// An absolute name is used as is, otherwise primary root + name; the
    /// file is created with CREATE_ALWAYS (no parent directories) and the
    /// result is `written == length`, so a zero length always succeeds.
    pub(crate) fn sys80_32_write_file<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let length = self.pop_int()? as u32 as usize;
        let source = self.pop_ptr()?;
        let file = self.pop_string_lossy()?;
        let path = if is_absolute_resource_path(&file) {
            file
        } else {
            format!("{}{file}", self.resource_primary_root(api))
        };
        let range = self.resolve_range(source, length)?;
        let written = api.write_file_bytes(&path, &self.memory[range]);
        Ok(Value::Int(i32::from(written || length == 0)))
    }

    /// Sys80:33 (sub_489340): pops file, then a nullable directory, and
    /// DeleteFileA("dir\\file") or primary root + file. Neither form checks
    /// for an absolute file name.
    pub(crate) fn sys80_33_delete_file<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let file = self.pop_string_lossy()?;
        let path = match self.pop_value()? {
            Value::Int(0) | Value::Ptr(0) | Value::None => {
                format!("{}{file}", self.resource_primary_root(api))
            }
            directory => format!("{}\\{file}", self.value_as_string_lossy(directory)?),
        };
        Ok(Value::Int(i32::from(api.delete_file("", &path))))
    }

    /// Sys80:3C (sub_4895E0 -> sub_466D00) pops text, caption, then file and
    /// checks primary root + file (sprintf "%s%s", no absolute test). The
    /// host owns the prompt loop, so the file operand is replaced by that
    /// path before the call reaches it.
    pub(crate) fn resolve_sys80_3c_file<A: SysApi>(&mut self, api: &mut A) -> VmResult<()> {
        let Some(operand) = self.operand_from_top(2) else {
            return Ok(());
        };
        let file = self.value_as_string_lossy(self.operand_value(operand))?;
        let path = format!("{}{file}", self.resource_primary_root(api));
        self.replace_operand(operand, Value::Str(path));
        Ok(())
    }

    /// Sys80:3F (sub_4896B0 -> sub_4654A0 -> sub_4652D0): pops flags, text,
    /// caption, then the marker file. The drive list is taken once; each
    /// round checks every drive for drive + marker 20 times (Sleep(50) per
    /// try only when there is no drive). On a hit the media root becomes
    /// drive + the marker's directory part (sub_465280). Without a hit, flags
    /// 0 returns 0; otherwise the OK|CANCEL prompt runs and only a confirmed
    /// quit returns 0.
    pub(crate) fn sys80_3f_find_media<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let flags = self.pop_int()?;
        let text = self.pop_string_lossy()?;
        let caption = self.pop_string_lossy()?;
        let marker = self.pop_string_lossy()?;
        self.media_prompt_title = caption.clone();
        self.media_prompt_text = text.clone();
        let suffix = self.decode_native_string_lossy(&marker_directory_prefix(
            &self.encode_native_string(&marker),
        ));
        let drives = api.removable_drive_roots();
        loop {
            for _ in 0..20 {
                if drives.is_empty() {
                    api.sleep_ms(50);
                }
                if let Some(drive) = drives
                    .iter()
                    .find(|drive| api.path_exists(&format!("{drive}{marker}")))
                {
                    self.secondary_resource_root = Some(format!("{drive}{suffix}"));
                    return Ok(Value::Int(1));
                }
            }
            if flags == 0 || !api.retry_or_quit_prompt(&caption, &text) {
                return Ok(Value::Int(0));
            }
        }
    }

    /// Sys80:34 (sub_4893C0 -> sub_466B20): pops file, then archive.
    pub(crate) fn sys80_34_file_exists<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let file = self.pop_string_lossy()?;
        let archive = self.pop_value()?;
        let archive = match archive {
            Value::Int(0) | Value::Ptr(0) | Value::None => None,
            other => Some(self.value_as_string_lossy(other)?),
        };
        let primary = self.resource_primary_root(api);
        let media = self.resource_media_root();
        let found = match archive.as_deref() {
            Some(archive) => {
                self.loose_resource_exists(api, &primary, &file) || {
                    if self.encode_native_string(&file).len() >= 0x60 {
                        return Err(VmError::Runtime(format!(
                            "指定されたファイル名 [ {file} ] は95文字を超えています"
                        )));
                    }
                    !archive.is_empty()
                        && self
                            .archive_members(archive)
                            .iter()
                            .any(|member| api.archive_entry_exists(member, &file))
                }
            }
            None if is_absolute_resource_path(&file) => api.disk_file_exists(&file),
            None => {
                self.loose_resource_exists(api, &primary, &file)
                    || self.loose_resource_exists(api, &media, &file)
            }
        };
        Ok(Value::Int(i32::from(found)))
    }

    /// Sys80:35 (sub_489400 -> sub_466840): pops file, then archive; the
    /// decoded size, 0 on any failure.
    pub(crate) fn sys80_35_file_size<A: SysApi>(&mut self, api: &mut A) -> VmResult<Value> {
        let file = self.pop_string_lossy()?;
        let archive = self.pop_value()?;
        let archive = match archive {
            Value::Int(0) | Value::Ptr(0) | Value::None => None,
            other => Some(self.value_as_string_lossy(other)?),
        };
        let primary = self.resource_primary_root(api);
        let media = self.resource_media_root();
        let size = match self.read_loose_resource(api, &primary, &file, 0, 0) {
            Ok(bytes) => bytes.len(),
            Err(_) => match archive.as_deref() {
                Some(archive) => {
                    let mut result = self.read_archive_resource(api, false, archive, &file, 0, 0)?;
                    if result == Err(ARCHIVE_NOT_FOUND) && !media.is_empty() {
                        result = self.read_archive_resource(api, true, archive, &file, 0, 0)?;
                    }
                    result.map_or(0, |bytes| bytes.len())
                }
                None => self
                    .read_loose_resource(api, &media, &file, 0, 0)
                    .map_or(0, |bytes| bytes.len()),
            },
        };
        Ok(Value::Int(size as i32))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn marker_directory_prefix_keeps_the_target_quirks() {
        assert_eq!(marker_directory_prefix(b"a.arc"), b"");
        assert_eq!(marker_directory_prefix(b"DISC\\a.arc"), b"DISC\\");
        assert_eq!(marker_directory_prefix(b"X\\Y\\a.arc"), b"X\\Y");
        assert_eq!(marker_directory_prefix(b"\\X\\a.arc"), b"\\X");
        // 0x83 0x5C is one DBCS character, not a separator.
        assert_eq!(marker_directory_prefix(b"\x83\x5C\\a.arc"), b"\x83\x5C\\");
    }

    #[derive(Default)]
    struct MediaApi {
        drives: Vec<String>,
        present: Vec<String>,
        prompts: Vec<bool>,
        prompted: usize,
        slept: u32,
    }

    impl SysApi for MediaApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
        fn removable_drive_roots(&mut self) -> Vec<String> {
            self.drives.clone()
        }
        fn path_exists(&mut self, path: &str) -> bool {
            self.present.iter().any(|present| present == path)
        }
        fn retry_or_quit_prompt(&mut self, caption: &str, text: &str) -> bool {
            assert_eq!((caption, text), ("caption", "insert"));
            self.prompted += 1;
            self.prompts.pop().unwrap_or(false)
        }
        fn sleep_ms(&mut self, ms: u32) {
            self.slept += ms;
        }
    }

    fn find_media(vm: &mut Vm, api: &mut MediaApi, flags: i32) -> Value {
        vm.stack.extend([
            Value::Str("DISC\\data.arc".into()),
            Value::Str("caption".into()),
            Value::Str("insert".into()),
            Value::Int(flags),
        ]);
        vm.sys80_3f_find_media(api).unwrap()
    }

    #[test]
    fn sys80_3f_scans_drives_then_prompts() {
        let mut vm = Vm::new();
        let mut api = MediaApi { drives: vec!["D:\\".into(), "E:\\".into()], ..Default::default() };
        api.present.push("E:\\DISC\\data.arc".into());
        assert_eq!(find_media(&mut vm, &mut api, 0), Value::Int(1));
        assert_eq!(vm.secondary_resource_root.as_deref(), Some("E:\\DISC\\"));
        assert_eq!((vm.media_prompt_title.as_str(), vm.media_prompt_text.as_str()), ("caption", "insert"));

        // No drive: 20 tries of Sleep(50), then flags 0 gives up silently.
        let mut vm = Vm::new();
        let mut api = MediaApi::default();
        assert_eq!(find_media(&mut vm, &mut api, 0), Value::Int(0));
        assert_eq!((api.slept, api.prompted), (1000, 0));
        assert_eq!(vm.secondary_resource_root, None);

        // flags != 0: prompt until the player confirms quitting.
        let mut api = MediaApi { prompts: vec![false, true, true], ..Default::default() };
        assert_eq!(find_media(&mut vm, &mut api, 1), Value::Int(0));
        assert_eq!((api.prompted, api.slept), (3, 3000));
    }

    #[derive(Default)]
    struct LaunchApi {
        attempts: Vec<(String, String)>,
        boxes: usize,
        retries: Vec<bool>,
    }

    impl SysApi for LaunchApi {
        fn call_sys(&mut self, _call: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
        fn primary_resource_root(&mut self) -> Option<String> {
            Some("C:\\Game\\".into())
        }
        fn launch_process(&mut self, cwd: &str, command: &str, _: bool, _: bool) -> bool {
            self.attempts.push((cwd.into(), command.into()));
            false
        }
        fn message_box(&mut self, _: &str, _: &str) {
            self.boxes += 1;
        }
        fn retry_or_quit_prompt(&mut self, _: &str, _: &str) -> bool {
            self.retries.pop().unwrap_or(false)
        }
    }

    #[test]
    fn launch_follows_sub_472a10() {
        let mut vm = Vm::new();
        let mut api = LaunchApi::default();
        // No message: one attempt below the primary root, then 0.
        assert_eq!(vm.launch_native_process(&mut api, None, "a.exe x", None, false, true, false), 0);
        assert_eq!(api.attempts, [("C:\\Game\\".into(), "C:\\Game\\a.exe x".into())]);
        // Sys80:E2: one OK box.
        api.attempts.clear();
        let dir = Some("D:\\Tools".to_string());
        assert_eq!(vm.launch_native_process(&mut api, dir.clone(), "u.exe", Some("e".into()), true, false, true), 0);
        assert_eq!((api.attempts.len(), api.boxes), (1, 1));
        assert_eq!(api.attempts[0].1, "D:\\Tools\\u.exe");
        // Sys80:E0: retried until the player confirms quitting.
        api.attempts.clear();
        api.retries = vec![false, true, true];
        assert_eq!(vm.launch_native_process(&mut api, dir, "u.exe", Some("e".into()), false, true, false), 0);
        assert_eq!(api.attempts.len(), 3);
    }
}
