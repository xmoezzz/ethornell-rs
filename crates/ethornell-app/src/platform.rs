use std::path::Path;

#[cfg(target_os = "windows")]
#[repr(C)]
struct CursorInfo {
    size: u32,
    flags: u32,
    cursor: *mut core::ffi::c_void,
    screen_x: i32,
    screen_y: i32,
}

#[cfg(target_os = "windows")]
#[link(name = "user32")]
unsafe extern "system" {
    fn GetCursorInfo(info: *mut CursorInfo) -> i32;
}

/// Matches the target's `sub_48EDE0`: failed queries leave the zeroed flags
/// unsuppressed, and bit 1 is the only state observed by the script latch.
#[cfg(target_os = "windows")]
pub(crate) fn cursor_suppressed() -> bool {
    let mut info = CursorInfo {
        size: std::mem::size_of::<CursorInfo>() as u32,
        flags: 0,
        cursor: std::ptr::null_mut(),
        screen_x: 0,
        screen_y: 0,
    };
    // SAFETY: `info` has the Win32 CURSORINFO layout and remains valid for the
    // duration of this synchronous call.
    unsafe {
        GetCursorInfo(&mut info);
    }
    info.flags & 2 != 0
}

#[cfg(not(target_os = "windows"))]
pub(crate) fn cursor_suppressed() -> bool {
    false
}

pub(crate) fn test_path_writable(path: &Path) -> bool {
    let directory = if path.is_dir() {
        path
    } else {
        path.parent().unwrap_or(path)
    };
    if !directory.is_dir() {
        return false;
    }

    let probe = directory.join(format!(
        ".ethornell-write-probe-{}-{}",
        std::process::id(),
        std::thread::current().name().unwrap_or("runtime")
    ));
    match std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(&probe)
    {
        Ok(file) => {
            drop(file);
            std::fs::remove_file(probe).is_ok()
        }
        Err(_) => false,
    }
}

pub(crate) fn shell_open(target: &str) -> bool {
    if super::file_policy::restricted() { return false; }
    if std::env::var_os("ETHORNELL_GUI_HEADLESS").is_some() || super::host_dialog::host_dialogs_suppressed() {
        return false;
    }

    shell_open_platform(target)
}

#[cfg(target_os = "macos")]
fn shell_open_platform(target: &str) -> bool {
    std::process::Command::new("open")
        .arg(target)
        .spawn()
        .is_ok()
}

#[cfg(target_os = "windows")]
fn shell_open_platform(target: &str) -> bool {
    std::process::Command::new("cmd")
        .args(["/C", "start", "", target])
        .spawn()
        .is_ok()
}

#[cfg(all(not(target_os = "macos"), not(target_os = "windows")))]
fn shell_open_platform(target: &str) -> bool {
    std::process::Command::new("xdg-open")
        .arg(target)
        .spawn()
        .is_ok()
}

/// Cross-platform key/value store for Windows-only engine integrations such
/// as registry-backed installer metadata.
///
/// The store is intentionally not bound to a native selector until that
/// selector's argument and output contract is recovered from the target. It
/// provides the portable backend only. Set `ETHORNELL_PLATFORM_STORE` to a
/// UTF-8 text file containing `key=value` pairs; keys are compared
/// case-insensitively with `/` and `\\` treated as the same separator.
#[derive(Debug, Clone, Default)]
pub(crate) struct PortablePlatformStore {
    values: std::collections::BTreeMap<String, String>,
    path: Option<std::path::PathBuf>,
}

impl PortablePlatformStore {
    pub(crate) fn from_environment() -> Self {
        if super::file_policy::restricted() { return Self::default(); }
        let Some(path) = std::env::var_os("ETHORNELL_PLATFORM_STORE") else {
            return Self::default();
        };
        match Self::load(Path::new(&path)) {
            Ok(store) => store,
            Err(error) => {
                tracing::warn!(
                    path = %Path::new(&path).display(),
                    %error,
                    "failed to load portable platform store"
                );
                Self::default()
            }
        }
    }

    pub(crate) fn load(path: &Path) -> std::io::Result<Self> {
        let text = std::fs::read_to_string(path)?;
        let mut values = std::collections::BTreeMap::new();
        for (line_number, raw_line) in text.lines().enumerate() {
            let line = raw_line.trim();
            if line.is_empty() || line.starts_with('#') || line.starts_with(';') {
                continue;
            }
            let Some((key, value)) = line.split_once('=') else {
                tracing::warn!(
                    path = %path.display(),
                    line = line_number + 1,
                    "ignored platform-store line without '='"
                );
                continue;
            };
            values.insert(normalize_platform_key(key), value.trim().to_string());
        }
        Ok(Self {
            values,
            path: Some(path.to_path_buf()),
        })
    }

    pub(crate) fn get(&self, key: &str) -> Option<&str> {
        self.values
            .get(&normalize_platform_key(key))
            .map(String::as_str)
    }

    pub(crate) fn candidate_matches<'a>(
        &'a self,
        candidates: impl IntoIterator<Item = &'a str>,
    ) -> Vec<(&'a str, &'a str)> {
        candidates
            .into_iter()
            .filter_map(|candidate| self.get(candidate).map(|value| (candidate, value)))
            .collect()
    }

    pub(crate) fn ensure_path(&mut self, path: std::path::PathBuf) {
        if super::file_policy::restricted() { return; }
        if self.path.is_none() {
            self.path = Some(path);
        }
    }

    pub(crate) fn set(&mut self, key: &str, value: String) -> bool {
        self.values.insert(normalize_platform_key(key), value);
        self.save()
    }

    pub(crate) fn remove(&mut self, key: &str) -> bool {
        let removed = self.values.remove(&normalize_platform_key(key)).is_some();
        if removed { self.save() } else { false }
    }

    fn save(&self) -> bool {
        let Some(path) = self.path.as_deref() else {
            return true;
        };
        if let Some(parent) = path.parent()
            && std::fs::create_dir_all(parent).is_err()
        {
            return false;
        }
        let mut text = String::new();
        for (key, value) in &self.values {
            text.push_str(key);
            text.push('=');
            text.push_str(value);
            text.push('\n');
        }
        std::fs::write(path, text).is_ok()
    }
}

fn normalize_platform_key(key: &str) -> String {
    key.trim()
        .replace('\\', "/")
        .split('/')
        .filter(|part| !part.is_empty())
        .map(str::trim)
        .collect::<Vec<_>>()
        .join("/")
        .to_ascii_lowercase()
}

#[cfg(test)]
mod tests {
    use super::{PortablePlatformStore, normalize_platform_key};
    use std::collections::BTreeMap;

    #[test]
    fn platform_keys_are_windows_case_and_separator_insensitive() {
        assert_eq!(
            normalize_platform_key(r"HKCU\\Software\\LumpOfSugar\\InstallDir"),
            "hkcu/software/lumpofsugar/installdir"
        );
        assert_eq!(
            normalize_platform_key("hkcu/software/LumpOfSugar/InstallDir"),
            "hkcu/software/lumpofsugar/installdir"
        );
    }

    #[test]
    fn portable_store_supports_registry_style_lookups() {
        let store = PortablePlatformStore {
            values: BTreeMap::from([(
                "hkcu/software/lumpofsugar/installdir".to_string(),
                "/games/tayutama2".to_string(),
            )]),
            path: None,
        };
        assert_eq!(
            store.get(r"HKCU\\SOFTWARE\\LumpOfSugar\\InstallDir"),
            Some("/games/tayutama2")
        );
    }
}

/// Milliseconds/nanoseconds since the host booted, the portable stand-in for
/// GetTickCount/timeGetTime and QueryPerformanceCounter.
#[cfg(unix)]
pub(crate) fn host_uptime_ns() -> u64 {
    let mut now = libc::timespec { tv_sec: 0, tv_nsec: 0 };
    // SAFETY: clock_gettime only writes the timespec.
    unsafe { libc::clock_gettime(libc::CLOCK_MONOTONIC, &mut now) };
    (now.tv_sec as u64).wrapping_mul(1_000_000_000).wrapping_add(now.tv_nsec as u64)
}

#[cfg(windows)]
pub(crate) fn host_uptime_ns() -> u64 {
    unsafe extern "system" {
        fn GetTickCount64() -> u64;
    }
    // SAFETY: GetTickCount64 has no preconditions.
    unsafe { GetTickCount64() }.wrapping_mul(1_000_000)
}

#[cfg(not(any(unix, windows)))]
pub(crate) fn host_uptime_ns() -> u64 {
    0
}

/// sub_46FC50: `(GetSystemDefaultLangID() & 0x3FF) == LANG_JAPANESE`.
#[cfg(target_os = "windows")]
pub(crate) fn system_language_is_japanese() -> bool {
    #[link(name = "kernel32")]
    unsafe extern "system" {
        fn GetSystemDefaultLangID() -> u16;
    }
    // SAFETY: no arguments, no preconditions.
    unsafe { GetSystemDefaultLangID() & 0x3FF == 0x11 }
}

/// sub_46FC50 over the language of the current CoreFoundation locale.
#[cfg(target_os = "macos")]
pub(crate) fn system_language_is_japanese() -> bool {
    use core::ffi::c_void;
    #[link(name = "CoreFoundation", kind = "framework")]
    unsafe extern "C" {
        fn CFLocaleCopyCurrent() -> *const c_void;
        fn CFLocaleGetIdentifier(locale: *const c_void) -> *const c_void;
        fn CFStringGetCString(string: *const c_void, buffer: *mut u8, size: isize, encoding: u32) -> u8;
        fn CFRelease(object: *const c_void);
    }
    const UTF8: u32 = 0x0800_0100;
    let mut buffer = [0u8; 64];
    // SAFETY: the locale is released after use; the identifier is borrowed
    // from it (Get rule) and copied into `buffer` before the release.
    let copied = unsafe {
        let locale = CFLocaleCopyCurrent();
        if locale.is_null() {
            return false;
        }
        let ok = CFStringGetCString(CFLocaleGetIdentifier(locale), buffer.as_mut_ptr(), buffer.len() as isize, UTF8);
        CFRelease(locale);
        ok != 0
    };
    copied && locale_name_is_japanese(&String::from_utf8_lossy(&buffer))
}

/// sub_46FC50 over the POSIX message locale.
#[cfg(not(any(target_os = "windows", target_os = "macos")))]
pub(crate) fn system_language_is_japanese() -> bool {
    ["LC_ALL", "LC_MESSAGES", "LANG"]
        .iter()
        .find_map(|name| std::env::var(name).ok().filter(|value| !value.is_empty()))
        .is_some_and(|value| locale_name_is_japanese(&value))
}

#[cfg(not(target_os = "windows"))]
fn locale_name_is_japanese(name: &str) -> bool {
    let language = name.split(['_', '-', '.', '@', '\0']).next().unwrap_or("");
    language.eq_ignore_ascii_case("ja")
}

/// GetLogicalDriveStringsA, keeping DRIVE_REMOVABLE (2) and DRIVE_CDROM (5).
#[cfg(target_os = "windows")]
pub(crate) fn removable_drive_roots() -> Vec<String> {
    #[link(name = "kernel32")]
    unsafe extern "system" {
        fn GetLogicalDriveStringsA(length: u32, buffer: *mut u8) -> u32;
        fn GetDriveTypeA(root: *const u8) -> u32;
    }
    let mut buffer = [0u8; 0x400];
    // SAFETY: the buffer length is passed; the result is NUL-separated.
    let length = unsafe { GetLogicalDriveStringsA(buffer.len() as u32, buffer.as_mut_ptr()) } as usize;
    buffer[..length.min(buffer.len())]
        .split(|&byte| byte == 0)
        .filter(|root| !root.is_empty())
        .filter(|root| {
            let mut name = root.to_vec();
            name.push(0);
            // SAFETY: `name` is NUL-terminated.
            matches!(unsafe { GetDriveTypeA(name.as_ptr()) }, 2 | 5)
        })
        .map(|root| String::from_utf8_lossy(root).into_owned())
        .collect()
}
