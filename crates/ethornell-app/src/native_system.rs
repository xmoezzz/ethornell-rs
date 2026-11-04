use super::*;
use ethornell_vm::SysApi;

#[derive(Default)]
pub(super) struct NativeSystemState {
    #[cfg(any(target_os="vita",target_os="horizon"))]
    pub(super) mutex_names: BTreeMap<String,u64>,
    pub(super) main_loop_wait_override: i32,
    pub(super) drag_drop_enabled: bool,
    pub(super) fullscreen_hotkeys_enabled: bool,
    pub(super) fullscreen_hotkeys: Vec<i32>,
    pub(super) display_scaling_mode: i32,
    pub(super) cursor_index: i32,
    pub(super) native_close_mode: i32,
    pub(super) raster_wait_enabled: i32,
    /// Sys80:6B: (root, archive, file) of the pending engine restart.
    pub(super) bootstrap_request: Option<(Option<String>, String, String)>,
    cursor_button_mode: i32,
    pub(super) uninstaller_mutex_name: String,
    /// Sys80:E1: (current directory, command line, error message) run by
    /// WinMain after the loop.
    pub(super) restart: Option<(String, String, Option<String>)>,
}

impl NativeSystemState {
    /// Sys80:1E mouse-button mapping mode (dword_56691C, sub_48EE70).
    pub(super) fn mouse_button_mapping_mode(&self) -> i32 {
        self.cursor_button_mode
    }
}

impl RuntimeTraceApi {
    /// Whether the file Sys80:3C waits for can be found (loose or archived).
    /// GetFileAttributesA succeeds without FILE_ATTRIBUTE_DIRECTORY
    /// (sub_466D00); archives are not searched.
    pub(super) fn required_file_present(&mut self, path: &str) -> bool {
        self.disk_file_exists(path)
    }

    pub(super) fn dispatch_native_system(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group != 0x80 {
            return None;
        }

        let value = match id {
            // sub_49A240 is the WM_ACTIVATE/minimize latch.
            0x0E => ethornell_vm::Value::Int(i32::from(self.window_minimize_latch)),
            0x1D => {
                let input_descriptor = pop_int_value(stack).unwrap_or_default();
                let scope = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(ethornell_vm::SysApi::query_scoped_input_event(
                    self,
                    input_descriptor,
                    scope,
                ))
            }
            0x1E => {
                let mode = pop_int_value(stack).unwrap_or_default();
                let accepted = (0..=1).contains(&mode);
                if accepted {
                    self.native_system.cursor_button_mode = mode;
                }
                ethornell_vm::Value::Int(i32::from(accepted))
            }
            0x1F => {
                let cancel_on_large_move = pop_int_value(stack).unwrap_or_default();
                let updates_per_second = pop_int_value(stack).unwrap_or_default();
                let duration_ms = pop_int_value(stack).unwrap_or_default();
                let interpolation_mode = pop_int_value(stack).unwrap_or_default();
                let target_y = pop_int_value(stack).unwrap_or_default();
                let target_x = pop_int_value(stack).unwrap_or_default();
                self.configure_cursor_motion(
                    cancel_on_large_move,
                    updates_per_second,
                    duration_ms,
                    interpolation_mode,
                    target_y,
                    target_x,
                );
                ethornell_vm::Value::None
            }
            0x24 => {
                let recursive = pop_int_value(stack).unwrap_or_default() != 0;
                let pattern = pop_string_value(stack).unwrap_or_default();
                // sub_467110 recurses through strrchr(pattern, '\\'); a
                // pattern without a backslash finds nothing below.
                let recursive = recursive && pattern.contains('\\');
                let count = self
                    .enumerate_native_paths(&pattern, recursive, 0, false)
                    .len()
                    .min(i32::MAX as usize) as i32;
                ethornell_vm::Value::Int(count)
            }
            0x27 => {
                let from = pop_string_value(stack).unwrap_or_default();
                let to = pop_string_value(stack).unwrap_or_default();
                if let Some(policy)=file_policy::active() {
                    return Some(Ok(ethornell_vm::Value::Int(i32::from(policy.rename(&from,&to).is_ok()))));
                }
                let ok = match (
                    runtime_file_path_from_root(&self.manager, &self.native_root, &from),
                    runtime_file_path_from_root(&self.manager, &self.native_root, &to),
                ) {
                    (Some(from), Some(to)) => win32_move_file(&from, &to),
                    _ => false,
                };
                tracing::info!(from, to, ok, "MoveFile");
                ethornell_vm::Value::Int(i32::from(ok))
            }
            // Native sub_4667A0 repeatedly resolves the required file and
            // presents Retry/Quit until media becomes available or the user
            // quits. The middle string is popped but unused by the target.
            0x3C => {
                // sub_466D00: the VM has resolved the file to primary root +
                // name. MessageBox(caption, text, OK|CANCEL); Cancel asks to
                // quit (YES|NO) and YES returns 0 without exiting; every other
                // answer checks again.
                let text = pop_string_value(stack).unwrap_or_default();
                let caption = pop_string_value(stack).unwrap_or_default();
                let path = pop_string_value(stack).unwrap_or_default();
                let found = loop {
                    if self.required_file_present(&path) {
                        break true;
                    }
                    tracing::warn!(path, "Sys80_3C_RequiredResourceMissing");
                    if !retry_or_quit_prompt(&caption, &text) {
                        break false;
                    }
                };
                ethornell_vm::Value::Int(i32::from(found))
            }
            0x53 => ethornell_vm::Value::None,
            // VM-owned scheduler boundary. Target ABI proves that 0x54
            // installs CProcedure, but the single argument and wake predicate
            // remain unrecovered. This host fallback only consumes the value;
            // it must not invent the legacy guessed WaitWndMsg implementation.
            0x54 => {
                let _arg0_unrecovered = stack.pop();
                ethornell_vm::Value::None
            }
            0x5D => unreachable!("exclusive-thread ownership is handled by the VM scheduler"),
            0x63 => {
                let argument = pop_int_value(stack).unwrap_or_default();
                self.native_system.display_scaling_mode = i32::from(argument == 0);
                ethornell_vm::Value::None
            }
            // sub_4650F0 selects a new bootstrap pair and returns interpreter
            // status 5. The VM intercepts actual program replacement.
            0x6B => {
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                tracing::info!(archive, file, "SelectBootstrapProgram");
                ethornell_vm::Value::None
            }
            0x6C => {
                self.native_system.drag_drop_enabled =
                    pop_int_value(stack).unwrap_or_default() != 0;
                self.dropped_files.clear();
                ethornell_vm::Value::None
            }
            0x6E => {
                self.native_system.raster_wait_enabled = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::None
            }
            0x6F => ethornell_vm::Value::Int(i32::from(self.display_aspect_mismatch())),
            // Sys80:78..7B are VM-owned because their payload and output pointers
            // address BP memory and the global-config buffer.
            // Sys80:90..B6 are VM-owned process-global record, object-message,
            // and exclusion primitives. They must not be reinterpreted as app-local events.
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn enumerate_native_paths(
        &self,
        pattern: &str,
        recursive: bool,
        max_count: usize,
        directories: bool,
    ) -> Vec<String> {
        if let Some(policy) = file_policy::active() { return policy.enumerate_user(pattern, recursive, max_count, directories).unwrap_or_default(); }
        let normalized = pattern.replace('\\', std::path::MAIN_SEPARATOR_STR);
        let pattern_path = Path::new(&normalized);
        let wildcard = pattern_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or("*");
        let parent = pattern_path.parent().unwrap_or_else(|| Path::new(""));
        // FindFirstFileA resolves a bare pattern such as "data0????.arc"
        // against the working directory, the game root.
        let root = if parent.as_os_str().is_empty() {
            Some(self.native_root.clone())
        } else {
            runtime_file_path_from_root(&self.manager, &self.native_root, &parent.to_string_lossy())
        };
        let Some(root) = root else {
            return Vec::new();
        };
        let mut output = Vec::new();
        enumerate_native_directory(
            &root,
            &root,
            wildcard,
            recursive,
            max_count,
            directories,
            &mut output,
        );
        output
    }
}

const WIN32_SETTABLE_ATTRIBUTES: u32 = 0x1 | 0x2 | 0x4 | 0x20 | 0x100 | 0x1000 | 0x2000;
#[cfg(unix)]
const WIN32_ATTRIBUTE_XATTR: &[u8] = b"user.ethornell.win32-attributes\0";

/// GetFileAttributesA: READONLY follows the host permission, DIRECTORY the
/// file type; the other settable bits are what SetFileAttributesA stored
/// (kept in an extended attribute), by default ARCHIVE for files as Windows
/// sets it on every write. A file with no bit left reports NORMAL (0x80).
pub(super) fn win32_file_attributes(path: &Path) -> Option<u32> {
    let metadata = std::fs::metadata(path).ok()?;
    let stored = read_win32_attribute_xattr(path)
        .unwrap_or(if metadata.is_dir() { 0 } else { 0x20 });
    let mut attrs = stored & WIN32_SETTABLE_ATTRIBUTES & !1;
    if metadata.permissions().readonly() {
        attrs |= 1;
    }
    if metadata.is_dir() {
        attrs |= 0x10;
    } else if attrs == 0 {
        attrs = 0x80;
    }
    Some(attrs)
}

/// SetFileAttributesA: keeps only the settable bits (NORMAL alone clears
/// them), maps READONLY to the host permission and stores the rest.
pub(super) fn set_win32_file_attributes(path: &Path, attrs: u32) -> bool {
    let Ok(metadata) = std::fs::metadata(path) else {
        return false;
    };
    let attrs = attrs & WIN32_SETTABLE_ATTRIBUTES;
    let mut permissions = metadata.permissions();
    #[allow(clippy::permissions_set_readonly_false)]
    permissions.set_readonly(attrs & 1 != 0);
    if std::fs::set_permissions(path, permissions).is_err() {
        return false;
    }
    write_win32_attribute_xattr(path, attrs & !1);
    true
}

#[cfg(any(target_vendor = "apple", target_os = "linux", target_os = "android"))]
fn xattr_path(path: &Path) -> Option<std::ffi::CString> {
    use std::os::unix::ffi::OsStrExt;
    std::ffi::CString::new(path.as_os_str().as_bytes()).ok()
}

#[cfg(target_vendor = "apple")]
fn read_win32_attribute_xattr(path: &Path) -> Option<u32> {
    let path = xattr_path(path)?;
    let mut value = [0u8; 4];
    // SAFETY: valid NUL-terminated strings and a 4-byte buffer.
    let read = unsafe {
        libc::getxattr(path.as_ptr(), WIN32_ATTRIBUTE_XATTR.as_ptr().cast(), value.as_mut_ptr().cast(), 4, 0, 0)
    };
    (read == 4).then(|| u32::from_le_bytes(value))
}

#[cfg(any(target_os = "linux", target_os = "android"))]
fn read_win32_attribute_xattr(path: &Path) -> Option<u32> {
    let path = xattr_path(path)?;
    let mut value = [0u8; 4];
    // SAFETY: valid NUL-terminated strings and a 4-byte buffer.
    let read = unsafe {
        libc::getxattr(path.as_ptr(), WIN32_ATTRIBUTE_XATTR.as_ptr().cast(), value.as_mut_ptr().cast(), 4)
    };
    (read == 4).then(|| u32::from_le_bytes(value))
}

#[cfg(target_vendor = "apple")]
fn write_win32_attribute_xattr(path: &Path, attrs: u32) {
    if let Some(path) = xattr_path(path) {
        let value = attrs.to_le_bytes();
        // SAFETY: valid NUL-terminated strings and a 4-byte value.
        unsafe {
            libc::setxattr(path.as_ptr(), WIN32_ATTRIBUTE_XATTR.as_ptr().cast(), value.as_ptr().cast(), 4, 0, 0);
        }
    }
}

#[cfg(any(target_os = "linux", target_os = "android"))]
fn write_win32_attribute_xattr(path: &Path, attrs: u32) {
    if let Some(path) = xattr_path(path) {
        let value = attrs.to_le_bytes();
        // SAFETY: valid NUL-terminated strings and a 4-byte value.
        unsafe {
            libc::setxattr(path.as_ptr(), WIN32_ATTRIBUTE_XATTR.as_ptr().cast(), value.as_ptr().cast(), 4, 0);
        }
    }
}

// FreeBSD has extattr_* instead of xattrs; Windows and the web keep no
// attribute store.
#[cfg(not(any(target_vendor = "apple", target_os = "linux", target_os = "android")))]
fn read_win32_attribute_xattr(_path: &Path) -> Option<u32> {
    None
}

#[cfg(not(any(target_vendor = "apple", target_os = "linux", target_os = "android")))]
fn write_win32_attribute_xattr(_path: &Path, _attrs: u32) {}

/// CopyFileA(..., FALSE): overwrites the destination (unless it is read-only),
/// copies the data, the attributes (plus ARCHIVE) and the last-write time;
/// a directory source fails.
pub(super) fn win32_copy_file(from: &Path, to: &Path) -> bool {
    let Ok(source) = std::fs::metadata(from) else {
        return false;
    };
    if !source.is_file() {
        return false;
    }
    if std::fs::metadata(to).is_ok_and(|meta| meta.permissions().readonly()) {
        return false;
    }
    if std::fs::copy(from, to).is_err() {
        return false;
    }
    if let (Ok(modified), Ok(file)) = (source.modified(), std::fs::File::options().write(true).open(to)) {
        let _ = file.set_modified(modified);
    }
    let stored = read_win32_attribute_xattr(from).unwrap_or(0x20) & !1;
    write_win32_attribute_xattr(to, stored | 0x20);
    true
}

/// MoveFileA without flags: the destination must not exist (a case-only
/// rename of the same file is allowed), its directory is not created, and a
/// file may move across volumes by copy and delete; a directory may not.
pub(super) fn win32_move_file(from: &Path, to: &Path) -> bool {
    let Ok(source) = std::fs::symlink_metadata(from) else {
        return false;
    };
    if std::fs::symlink_metadata(to).is_ok() && !same_file(from, to) {
        return false;
    }
    match std::fs::rename(from, to) {
        Ok(()) => true,
        Err(_) if source.is_file() && to.parent().is_some_and(Path::is_dir) => {
            std::fs::copy(from, to).is_ok() && std::fs::remove_file(from).is_ok()
        }
        Err(_) => false,
    }
}

fn same_file(a: &Path, b: &Path) -> bool {
    match (std::fs::canonicalize(a), std::fs::canonicalize(b)) {
        (Ok(a), Ok(b)) => a == b,
        _ => false,
    }
}

pub(super) fn split_native_command_line(command_line: &str) -> Vec<String> {
    let mut args = Vec::new();
    let mut current = String::new();
    let mut quoted = false;
    let mut backslashes = 0usize;
    for ch in command_line.chars() {
        match ch {
            '\\' => backslashes += 1,
            '"' => {
                current.extend(std::iter::repeat_n('\\', backslashes / 2));
                if backslashes.is_multiple_of(2) {
                    quoted = !quoted;
                } else {
                    current.push('"');
                }
                backslashes = 0;
            }
            ch if ch.is_whitespace() && !quoted => {
                current.extend(std::iter::repeat_n('\\', backslashes));
                backslashes = 0;
                if !current.is_empty() {
                    args.push(std::mem::take(&mut current));
                }
            }
            ch => {
                current.extend(std::iter::repeat_n('\\', backslashes));
                backslashes = 0;
                current.push(ch);
            }
        }
    }
    current.extend(std::iter::repeat_n('\\', backslashes));
    if !current.is_empty() {
        args.push(current);
    }
    args
}

pub(super) fn decode_native_text(bytes: &[u8]) -> String {
    let (decoded, _, _) = encoding_rs::SHIFT_JIS.decode(bytes);
    decoded.into_owned()
}

pub(super) fn encode_native_text(text: &str) -> Vec<u8> {
    encoding_rs::SHIFT_JIS.encode(text).0.into_owned()
}

fn enumerate_native_directory(
    root: &Path,
    directory: &Path,
    pattern: &str,
    recursive: bool,
    max_count: usize,
    directories: bool,
    output: &mut Vec<String>,
) {
    if file_policy::restricted() && output.len()>=10_000 {return;}
    let Ok(entries) = std::fs::read_dir(directory) else {
        return;
    };
    let mut entries = entries.filter_map(|entry| entry.ok()).collect::<Vec<_>>();
    entries.sort_by_key(|entry| entry.file_name().to_string_lossy().to_ascii_lowercase());
    for entry in entries {
        if max_count != 0 && output.len() >= max_count {
            return;
        }
        let Ok(file_type) = entry.file_type() else {
            continue;
        };
        let name = entry.file_name();
        let name = name.to_string_lossy();
        if file_type.is_dir() {
            if directories && windows_wildcard_matches(pattern, &name) {
                let relative = entry.path();
                let relative = relative.strip_prefix(root).unwrap_or(&relative);
                output.push(
                    relative
                        .to_string_lossy()
                        .replace(std::path::MAIN_SEPARATOR, "\\"),
                );
            }
            if recursive && name != "." && name != ".." {
                enumerate_native_directory(
                    root,
                    &entry.path(),
                    pattern,
                    true,
                    max_count,
                    directories,
                    output,
                );
            }
        } else if !directories && windows_wildcard_matches(pattern, &name) {
            let relative = entry.path();
            let relative = relative.strip_prefix(root).unwrap_or(&relative);
            output.push(
                relative
                    .to_string_lossy()
                    .replace(std::path::MAIN_SEPARATOR, "\\"),
            );
        }
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn vm_dispatched_group_80_handlers_are_explicitly_owned() {
        for id in [
            0x26u16, 0x6D, 0x7A, 0xA9, 0xCF, 0xDE, 0xE9, 0xEA, 0xF1, 0xF3, 0xF4, 0xF5, 0xF6, 0xF9,
            0xFA, 0xFB, 0xFC, 0xFE,
        ] {
            assert!(
                ethornell_vm::native_ownership::owns_before_host_dispatch(0x80, id),
                "System80:{id:02X} has a VM dispatch but no VM ownership declaration"
            );
        }
    }
}
