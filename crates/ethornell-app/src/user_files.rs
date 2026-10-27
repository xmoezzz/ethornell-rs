use ethornell_archive::ResourceManager;
use std::path::{Path, PathBuf};

use super::find_runtime_resource;

pub(crate) fn authorized_archive_arg(archive: &str) -> bool {
    let normalized = archive.replace('\\', "/");
    is_empty_archive_arg(archive)
        || matches!(normalized.as_str(), "bgi-resource/" | "bgi-user/")
        || (!normalized.contains(['/', ':', '\0']) && !normalized.starts_with('.'))
}
pub(crate) fn scoped_name(archive: &str, file: &str) -> Option<String> {
    let normalized = archive.replace('\\', "/");
    let name = file.replace('\\', "/");
    if matches!(normalized.as_str(), "bgi-resource/" | "bgi-user/") {
        if name.starts_with("bgi-") {
            name.starts_with(&normalized).then_some(name)
        } else {
            Some(format!("{normalized}{name}"))
        }
    } else {
        Some(name)
    }
}
pub(crate) fn named_archive(archive: &str) -> bool {
    authorized_archive_arg(archive)
        && !is_empty_archive_arg(archive)
        && !matches!(
            archive.replace('\\', "/").as_str(),
            "bgi-user/" | "bgi-resource/"
        )
}

pub(crate) fn game_root_path(manager: &ResourceManager) -> PathBuf {
    let root = manager.archives().root.as_path();
    if root.is_absolute() {
        return root.to_path_buf();
    }
    std::env::current_dir()
        .map(|cwd| cwd.join(root))
        .unwrap_or_else(|_| root.to_path_buf())
}

pub(crate) fn script_game_root(native_root: &Path) -> String {
    if cfg!(target_os = "windows") {
        native_root.to_string_lossy().into_owned()
    } else {
        // BP stores paths in Shift-JIS buffers. The host path can contain
        // characters that Shift-JIS cannot represent. Keep it on the host.
        "C:\\Game\\".into()
    }
}

fn normalize_runtime_native_path(native_root: &Path, file: &str) -> PathBuf {
    if !cfg!(target_os = "windows") {
        let normalized = file.replace('\\', "/");
        if normalized.eq_ignore_ascii_case("C:/Game") {
            return native_root.to_path_buf();
        }
        if normalized
            .get(..8)
            .is_some_and(|root| root.eq_ignore_ascii_case("C:/Game/"))
        {
            return native_root.join(normalized[8..].trim_start_matches('/'));
        }
    }
    let drive_absolute = file
        .as_bytes()
        .get(1)
        .is_some_and(|separator| *separator == b':');
    let unc_absolute = file.starts_with("\\\\");
    let root_relative = file.starts_with('\\') && !unc_absolute && !drive_absolute;
    let file = if root_relative {
        file.trim_start_matches('\\')
    } else {
        file
    };
    PathBuf::from(file.replace('\\', std::path::MAIN_SEPARATOR_STR))
}

pub(crate) fn find_runtime_file(
    manager: &ResourceManager,
    archive: &str,
    file: &str,
) -> Option<PathBuf> {
    find_runtime_file_from_root(manager, &game_root_path(manager), archive, file)
}

pub(crate) fn find_runtime_file_from_root(
    manager: &ResourceManager,
    native_root: &Path,
    archive: &str,
    file: &str,
) -> Option<PathBuf> {
    if let Some(policy) = super::file_policy::active() {
        let _ = (policy, native_root, archive, file);
        // Restricted callers must consume an FD/metadata, never reopen a Path.
        return None;
    }
    if file.trim().is_empty() {
        return None;
    }

    let normalized = normalize_runtime_native_path(native_root, file);
    let path = normalized.as_path();
    let windows_absolute = file.starts_with("\\\\")
        || file
            .as_bytes()
            .get(1)
            .is_some_and(|separator| *separator == b':');
    if path.is_absolute() || windows_absolute {
        return Some(path.to_path_buf());
    }

    let game_root = native_root.to_path_buf();
    let archive = archive.trim();
    if !is_empty_archive_arg(archive) {
        // BGI also passes an already-resolved primary/secondary filesystem
        // root through the archive slot. Distinguish that from an ordinary
        // archive name and resolve the loose file below it.
        let normalized_archive = normalize_runtime_native_path(native_root, archive);
        let archive_path = normalized_archive.as_path();
        let rooted = if archive_path.is_absolute() {
            archive_path.to_path_buf()
        } else {
            game_root.join(archive_path)
        };
        let looks_like_root = archive_path.is_absolute()
            || archive.contains('/')
            || archive.contains('\\')
            || rooted.is_dir();
        if looks_like_root {
            return resolve_existing_path_case_insensitive(&rooted, path)
                .or_else(|| Some(rooted.join(path)));
        }
        return None;
    }

    if path.starts_with(manager.archives().root.as_path()) {
        return Some(path.to_path_buf());
    }

    // Loose files are resolved below the target process-global native root.
    // The portable archive scan root is a separate namespace.
    if let Some(loose) = resolve_existing_path_case_insensitive(&game_root, path) {
        return Some(loose);
    }
    // sub_465E30: a loose file missing below the root is retried as
    // `root + dir + "\\" + file` for each Sys80:37 directory, newest first.
    for directory in manager.loose_search_paths() {
        let directory = directory.replace('\\', std::path::MAIN_SEPARATOR_STR);
        if let Some(loose) =
            resolve_existing_path_case_insensitive(&game_root, &Path::new(&directory).join(path))
        {
            return Some(loose);
        }
    }

    manager.archives().archives.iter().find_map(|archive| {
        let relative = archive
            .path
            .strip_prefix(&game_root)
            .unwrap_or(archive.path.as_path())
            .to_string_lossy()
            .replace('\\', "/");
        let requested = file.replace('\\', "/");
        (relative.eq_ignore_ascii_case(&requested)
            || bgi_xxx_pattern_matches(&requested, &relative)
            || archive
                .path
                .file_name()
                .and_then(|name| name.to_str())
                .is_some_and(|name| {
                    name.eq_ignore_ascii_case(file) || bgi_xxx_pattern_matches(file, name)
                }))
        .then(|| archive.path.clone())
    })
}

pub(crate) fn bgi_xxx_pattern_matches(pattern: &str, value: &str) -> bool {
    let pattern = pattern.replace('\\', "/").to_ascii_lowercase();
    let value = value.replace('\\', "/").to_ascii_lowercase();
    let Some(index) = pattern.find("xxx") else {
        return false;
    };
    let (prefix, suffix_with_marker) = pattern.split_at(index);
    let suffix = &suffix_with_marker[3..];
    value.len() >= prefix.len().saturating_add(suffix.len())
        && value.starts_with(prefix)
        && value.ends_with(suffix)
}

pub(crate) fn resolve_existing_path_case_insensitive(
    root: &Path,
    relative: &Path,
) -> Option<PathBuf> {
    let mut current = root.to_path_buf();
    for component in relative.components() {
        match component {
            std::path::Component::CurDir => {}
            std::path::Component::Normal(name) => {
                let direct = current.join(name);
                let requested = name.to_string_lossy();
                if let Some(entry) = std::fs::read_dir(&current).ok()?.flatten().find(|entry| {
                    entry
                        .file_name()
                        .to_string_lossy()
                        .eq_ignore_ascii_case(&requested)
                }) {
                    current = entry.path();
                } else if direct.exists() {
                    current = direct;
                } else {
                    return None;
                }
            }
            std::path::Component::ParentDir
            | std::path::Component::RootDir
            | std::path::Component::Prefix(_) => return None,
        }
    }
    current.exists().then_some(current)
}

pub(crate) fn runtime_file_path(manager: &ResourceManager, file: &str) -> Option<PathBuf> {
    runtime_file_path_from_root(manager, &game_root_path(manager), file)
}

pub(crate) fn runtime_file_path_from_root(
    manager: &ResourceManager,
    native_root: &Path,
    file: &str,
) -> Option<PathBuf> {
    if super::file_policy::restricted() {
        return None;
    }
    if file.trim().is_empty() {
        return None;
    }
    let normalized = normalize_runtime_native_path(native_root, file);
    let path = normalized.as_path();
    if path.is_absolute() {
        Some(path.to_path_buf())
    } else if path.starts_with(manager.archives().root.as_path()) {
        Some(path.to_path_buf())
    } else {
        Some(native_root.join(path))
    }
}

pub(crate) fn runtime_file_exists(manager: &ResourceManager, archive: &str, file: &str) -> bool {
    runtime_file_exists_from_root(manager, &game_root_path(manager), archive, file)
}

pub(crate) fn runtime_file_exists_from_root(
    manager: &ResourceManager,
    native_root: &Path,
    archive: &str,
    file: &str,
) -> bool {
    if let Some(policy) = super::file_policy::active() {
        if !authorized_archive_arg(archive) {
            return false;
        }
        if named_archive(archive) {
            return find_runtime_resource(manager, archive, file).is_some();
        }
        let Some(name) = scoped_name(archive, file) else {
            return false;
        };
        if policy.metadata(&name).is_some_and(|m| m.is_file()) {
            return true;
        }
        if name.replace('\\', "/").starts_with("bgi-") {
            return false;
        }
        return (!is_empty_archive_arg(archive)
            && find_runtime_resource(manager, archive, file).is_some())
            || authorized_container_name(manager, file).is_some();
    }
    if file.trim().is_empty() {
        return false;
    }

    // Target sub_4665C0 distinguishes a null archive/root argument from a
    // named archive. With no archive/root, it checks only exact loose files
    // under the configured roots (or an absolute path); it does not search
    // archive entries by resource name. This distinction is critical for
    // marker probes such as `Tayutama2TV`.
    if let Some(path) = find_runtime_file_from_root(manager, native_root, archive, file)
        && path.is_file()
    {
        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                backend = "loose_file",
                path = %path.display(),
                archive,
                file,
                "RuntimeFileExistsHit"
            );
        }
        return true;
    }

    if !is_empty_archive_arg(archive)
        && let Some(entry) = find_runtime_resource(manager, archive, file)
    {
        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                backend = "archive_entry",
                archive_path = %entry.archive_path.display(),
                entry_name = entry.entry_name,
                archive,
                file,
                "RuntimeFileExistsHit"
            );
        }
        return true;
    }

    if std::env::var_os("DEBUG").is_some() {
        tracing::info!(backend = "none", archive, file, "RuntimeFileExistsMiss");
    }
    false
}

pub(crate) fn runtime_file_cache_key(archive: &str, file: &str) -> String {
    format!(
        "{}\0{}",
        archive.trim().to_ascii_lowercase(),
        file.replace('\\', "/").to_ascii_lowercase()
    )
}

pub(crate) fn read_runtime_bytes(
    manager: &ResourceManager,
    archive: &str,
    file: &str,
) -> Option<Vec<u8>> {
    if let Some(policy) = super::file_policy::active() {
        use std::io::Read;
        if !authorized_archive_arg(archive) {
            return None;
        }
        if named_archive(archive) {
            let entry = find_runtime_resource(manager, archive, file)?;
            return manager.read_by_entry_decoded(&entry).ok();
        }
        let file = scoped_name(archive, file)?;
        let file = file.as_str();
        match policy.read_loose(file) {
            Ok(bytes) => return Some(bytes),
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
            Err(_) => return None,
        }
        if file.replace('\\', "/").starts_with("bgi-user/") {
            return None;
        }
        if file.replace('\\', "/").starts_with("bgi-resource/") {
            return None;
        }
        if let Some(container) = authorized_container_name(manager, file) {
            return policy.read_resource(&container, 256 * 1024 * 1024).ok();
        }
        let entry = find_runtime_resource(manager, archive, file)?;
        if entry.packed_size > 256 * 1024 * 1024
            || entry.unpacked_size.unwrap_or(0) > 256 * 1024 * 1024
        {
            return None;
        }
        return manager
            .read_by_entry_decoded(&entry)
            .ok()
            .filter(|b| b.len() <= 256 * 1024 * 1024);
    }
    if let Some(path) = find_runtime_file(manager, archive, file)
        && let Ok(bytes) = std::fs::read(path)
    {
        return Some(bytes);
    }
    let entry = find_runtime_resource(manager, archive, file)?;
    manager.read_by_entry_decoded(&entry).ok()
}

pub(crate) fn authorized_container_name(manager: &ResourceManager, file: &str) -> Option<String> {
    // Only a container already indexed from an authorized FD can satisfy the
    // BGI data01xxx.arc family alias. Never interpret it as a host glob.
    let requested = file.replace('\\', "/");
    if requested.contains('/')
        || requested.contains(':')
        || !requested.to_ascii_lowercase().contains("xxx")
    {
        return None;
    }
    manager.archives().archives.iter().find_map(|a| {
        let name = a.path.file_name()?.to_str()?;
        bgi_xxx_pattern_matches(&requested, name)
            .then(|| {
                a.path
                    .strip_prefix(&manager.archives().root)
                    .ok()
                    .map(|p| p.to_string_lossy().into_owned())
            })
            .flatten()
    })
}

pub(crate) fn is_empty_archive_arg(archive: &str) -> bool {
    archive.is_empty() || archive == "0" || archive == "0x00000000"
}

#[cfg(test)]
mod tests {
    use super::*;

    fn temporary_root(name: &str) -> PathBuf {
        let unique = format!(
            "ethornell-{name}-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        );
        let root = std::env::temp_dir().join(unique);
        std::fs::create_dir_all(&root).unwrap();
        root
    }

    #[test]
    #[cfg(not(target_os = "windows"))]
    fn unicode_game_root_survives_bp_path_buffers_and_save_io() {
        use ethornell_vm::{SysApi, Value, Vm, VmRunOptions, VmStopReason};

        let root = temporary_root("unicode-game-root");
        let native_root = root.join("秽翼的尤斯蒂娅");
        std::fs::create_dir(&native_root).unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();
        let mut api =
            super::super::RuntimeTraceApi::new_with_native_root(manager, native_root.clone());
        let mut vm = Vm::new();
        let run = |vm: &mut Vm, api: &mut super::super::RuntimeTraceApi, id, args| {
            let program = ethornell_script::parse_bp_program(None, &[0x80, id]);
            vm.start(&program);
            vm.stack = args;
            let report = vm.run_loaded(api, &VmRunOptions::default());
            assert_eq!(report.stop_reason, VmStopReason::Completed, "{report:?}");
            assert_eq!(vm.stack, vec![Value::Int(1)]);
        };

        // Sys80:3D writes the root into the same Shift-JIS buffer that the
        // bootstrap uses to construct UserData. This failed for this name.
        let buffer = 0x2000;
        run(
            &mut vm,
            &mut api,
            0x3d,
            vec![Value::Ptr(buffer), Value::Int(0)],
        );
        let buffer = buffer as usize;
        let length = vm.memory[buffer..]
            .iter()
            .position(|byte| *byte == 0)
            .unwrap();
        assert_eq!(&vm.memory[buffer..buffer + length], b"C:\\Game\\");
        let suffix = b"UserData\0";
        vm.memory[buffer + length..buffer + length + suffix.len()].copy_from_slice(suffix);

        run(&mut vm, &mut api, 0x28, vec![Value::Ptr(buffer as u32)]);
        assert!(native_root.join("UserData").is_dir());
        run(&mut vm, &mut api, 0x2a, vec![Value::Ptr(buffer as u32)]);
        assert!(api.directory_exists("C:\\Game\\UserData"));

        let save = "C:\\Game\\UserData\\slot.sud";
        assert!(api.write_file_bytes(save, b"saved data"));
        assert_eq!(
            std::fs::read(native_root.join("UserData/slot.sud")).unwrap(),
            b"saved data"
        );
        assert_eq!(api.read_user_file_bytes(save), Some(b"saved data".to_vec()));
        assert_eq!(api.read_disk_file(save), Some(b"saved data".to_vec()));
        assert!(api.file_exists("C:\\Game\\", "UserData/slot.sud"));
        assert_eq!(api.file_size("", save), 10);
        assert_eq!(
            api.enumerate_user_files("C:\\Game\\UserData\\*.sud", false, 0),
            vec!["slot.sud"]
        );

        // Existing saves can contain absolute host paths from older builds.
        let host_save = native_root.join("UserData/slot.sud");
        assert_eq!(
            api.read_disk_file(host_save.to_str().unwrap()),
            Some(b"saved data".to_vec())
        );
        assert!(api.delete_file("", save));
        assert!(!host_save.exists());
        std::fs::remove_dir_all(root).unwrap();
    }

    #[test]
    #[cfg(not(target_os = "windows"))]
    fn game_mount_accepts_windows_separators_and_exact_root_components() {
        let root = Path::new("/games/秽翼的尤斯蒂娅");
        for path in ["C:\\Game", "C:\\Game\\", "c:/gAmE/"] {
            assert_eq!(normalize_runtime_native_path(root, path), root);
        }
        for path in [
            "C:\\Game\\UserData",
            "c:/game/UserData",
            "C:\\Game\\\\UserData",
        ] {
            assert_eq!(
                normalize_runtime_native_path(root, path),
                root.join("UserData")
            );
        }
        assert_eq!(
            normalize_runtime_native_path(root, "C:\\GameOther\\UserData"),
            Path::new("C:/GameOther/UserData")
        );
        assert_eq!(
            normalize_runtime_native_path(root, "秽翼的尤斯蒂娅/UserData"),
            Path::new("秽翼的尤斯蒂娅/UserData")
        );
    }

    #[test]
    fn writing_a_save_refreshes_all_cached_path_aliases() {
        use ethornell_vm::SysApi;

        let root = temporary_root("save-cache");
        let native_root = root.join("native");
        std::fs::create_dir(&native_root).unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();
        let mut api =
            super::super::RuntimeTraceApi::new_with_native_root(manager, native_root.clone());
        // CreateFileA does not create directories (sub_42D940).
        std::fs::create_dir_all(native_root.join("UserData")).unwrap();
        let absolute = native_root
            .join("UserData/slot.sud")
            .to_string_lossy()
            .into_owned();
        let aliases = [
            (String::new(), "UserData\\slot.sud".to_owned()),
            (String::new(), absolute.clone()),
            (
                native_root.to_string_lossy().into_owned(),
                "UserData/slot.sud".to_owned(),
            ),
        ];
        for (archive, file) in &aliases {
            assert!(!api.file_exists(archive, file));
            assert_eq!(api.file_size(archive, file), -1);
        }
        for bytes in [
            b"first save".as_slice(),
            b"overwritten save with a new size".as_slice(),
        ] {
            assert!(api.write_file_bytes(&absolute, bytes));
            for (archive, file) in &aliases {
                assert!(api.file_exists(archive, file));
                assert_eq!(api.file_size(archive, file), bytes.len() as i32);
            }
            assert_eq!(api.load_file_bytes("", &absolute), Some(bytes.to_vec()));
        }
        std::fs::remove_dir_all(root).unwrap();
    }

    #[test]
    fn empty_entry_name_reads_the_first_entry_from_a_named_arc20_archive() {
        let root = temporary_root("first-archive-entry");
        let archive = root.join("data10000.arc");
        let payload = b"SDC FORMAT 1.00\0payload";
        let mut bytes = Vec::new();
        bytes.extend_from_slice(b"BURIKO ARC20");
        bytes.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 0x60];
        name[..4].copy_from_slice(b"evdb");
        bytes.extend_from_slice(&name);
        bytes.extend_from_slice(&0u32.to_le_bytes());
        bytes.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        bytes.extend_from_slice(&[0u8; 24]);
        bytes.extend_from_slice(payload);
        std::fs::write(&archive, bytes).unwrap();

        let manager = ResourceManager::open_game(&root).unwrap();
        assert_eq!(
            read_runtime_bytes(&manager, "data10000.arc", ""),
            Some(payload.to_vec())
        );
        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn empty_root_lookup_does_not_alias_the_first_archive_entry() {
        let root = temporary_root("empty-root-lookup");
        let archive = root.join("data10000.arc");
        let payload = b"payload";
        let mut bytes = Vec::new();
        bytes.extend_from_slice(b"BURIKO ARC20");
        bytes.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 0x60];
        name[..4].copy_from_slice(b"evdb");
        bytes.extend_from_slice(&name);
        bytes.extend_from_slice(&0u32.to_le_bytes());
        bytes.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        bytes.extend_from_slice(&[0u8; 24]);
        bytes.extend_from_slice(payload);
        std::fs::write(&archive, bytes).unwrap();

        let manager = ResourceManager::open_game(&root).unwrap();
        assert_eq!(read_runtime_bytes(&manager, "", ""), None);
        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn empty_archive_file_exists_does_not_search_archive_entries() {
        let root = temporary_root("file-exists-loose-only");
        let archive = root.join("data01000.arc");
        let payload = b"not-a-loose-marker";
        let mut bytes = Vec::new();
        bytes.extend_from_slice(b"BURIKO ARC20");
        bytes.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 0x60];
        let entry_name = b"Tayutama2TV";
        name[..entry_name.len()].copy_from_slice(entry_name);
        bytes.extend_from_slice(&name);
        bytes.extend_from_slice(&0u32.to_le_bytes());
        bytes.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        bytes.extend_from_slice(&[0u8; 24]);
        bytes.extend_from_slice(payload);
        std::fs::write(&archive, bytes).unwrap();

        let manager = ResourceManager::open_game(&root).unwrap();
        assert!(!runtime_file_exists(&manager, "", "Tayutama2TV"));
        assert!(runtime_file_exists(
            &manager,
            "data01000.arc",
            "Tayutama2TV"
        ));

        std::fs::create_dir(root.join("directory-marker")).unwrap();
        assert!(!runtime_file_exists(&manager, "", "directory-marker"));
        std::fs::write(root.join("loose-marker"), b"marker").unwrap();
        assert!(runtime_file_exists(&manager, "", "loose-marker"));

        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn native_loose_root_is_independent_from_the_archive_scan_root() {
        let resource_root = temporary_root("resource-root");
        let native_root = temporary_root("native-root");
        std::fs::write(resource_root.join("installation-media-marker"), b"marker").unwrap();
        let archive = resource_root.join("data01000.arc");
        let mut bytes = b"PackFile    ".to_vec();
        bytes.extend_from_slice(&0u32.to_le_bytes());
        std::fs::write(&archive, bytes).unwrap();
        let manager = ResourceManager::open_game(&resource_root).unwrap();

        assert!(!runtime_file_exists_from_root(
            &manager,
            &native_root,
            "",
            "installation-media-marker"
        ));
        assert!(runtime_file_exists_from_root(
            &manager,
            &native_root,
            "",
            "data01000.arc"
        ));
        std::fs::write(native_root.join("installation-media-marker"), b"marker").unwrap();
        assert!(runtime_file_exists_from_root(
            &manager,
            &native_root,
            "",
            "installation-media-marker"
        ));

        let _ = std::fs::remove_dir_all(resource_root);
        let _ = std::fs::remove_dir_all(native_root);
    }

    #[test]
    fn xxx_archive_family_name_resolves_an_installed_archive() {
        let root = temporary_root("xxx-archive-family");
        let archive = root.join("data01000.arc");
        let payload = b"scenario";
        let mut bytes = Vec::new();
        bytes.extend_from_slice(b"BURIKO ARC20");
        bytes.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 0x60];
        name[..4].copy_from_slice(b"main");
        bytes.extend_from_slice(&name);
        bytes.extend_from_slice(&0u32.to_le_bytes());
        bytes.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        bytes.extend_from_slice(&[0u8; 24]);
        bytes.extend_from_slice(payload);
        std::fs::write(&archive, bytes).unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();

        assert_eq!(
            find_runtime_file(&manager, "", "data01xxx.arc"),
            Some(archive)
        );
        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn relative_loose_files_are_resolved_below_the_game_root_case_insensitively() {
        let root = temporary_root("loose-root");
        std::fs::create_dir_all(root.join("GameData")).unwrap();
        std::fs::write(root.join("GameData").join("Install.dat"), b"marker").unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();

        assert_eq!(
            find_runtime_file(&manager, "", "gamedata\\install.DAT"),
            Some(root.join("GameData").join("Install.dat"))
        );
        assert_eq!(
            find_runtime_file(&manager, "", "\\GameData\\Install.dat"),
            Some(root.join("GameData").join("Install.dat"))
        );
        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn configured_filesystem_roots_resolve_loose_files() {
        let root = temporary_root("secondary-root");
        let secondary = root.join("DiscData");
        std::fs::create_dir_all(&secondary).unwrap();
        std::fs::write(secondary.join("movie.arc"), b"marker").unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();

        assert_eq!(
            find_runtime_file(&manager, &secondary.to_string_lossy(), "MOVIE.ARC"),
            Some(secondary.join("movie.arc"))
        );
        let _ = std::fs::remove_dir_all(root);
    }

    #[test]
    fn loaded_archive_container_names_are_runtime_files() {
        let root = temporary_root("archive-container");
        let archive = root.join("GameData.arc");
        let mut bytes = b"PackFile    ".to_vec();
        bytes.extend_from_slice(&0u32.to_le_bytes());
        std::fs::write(&archive, bytes).unwrap();
        let manager = ResourceManager::open_game(&root).unwrap();

        assert_eq!(
            find_runtime_file(&manager, "", "gamedata.ARC"),
            Some(archive)
        );
        let _ = std::fs::remove_dir_all(root);
    }
}
