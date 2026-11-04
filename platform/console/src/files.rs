//! Console newlib lacks openat/dirfd; supply hosted files with separate saves.
use ethornell_app::hosted_files::{ResourceFiles, UserStore};
use ethornell_archive::ArchiveSource;
use std::{
    io,
    path::{Component, Path, PathBuf},
    sync::Arc,
};
fn join(root: &Path, name: &str) -> io::Result<PathBuf> {
    if name.contains(['\\', ':', '\0'])
        || Path::new(name)
            .components()
            .any(|c| !matches!(c, Component::Normal(_)))
    {
        return Err(io::ErrorKind::PermissionDenied.into());
    }
    Ok(root.join(name))
}
fn list(root: &Path) -> io::Result<Vec<(String, u64)>> {
    fn walk(root: &Path, dir: &Path, depth: usize, out: &mut Vec<(String, u64)>) -> io::Result<()> {
        if depth > 64 || out.len() > 65536 {
            return Err(io::Error::other("console file-tree budget exceeded"));
        }
        for entry in std::fs::read_dir(dir)? {
            let entry = entry?;
            let kind = entry.file_type()?;
            if kind.is_symlink() {
                continue;
            }
            if kind.is_dir() {
                walk(root, &entry.path(), depth + 1, out)?;
            } else if kind.is_file() {
                let path = entry.path();
                let name = path
                    .strip_prefix(root)
                    .map_err(io::Error::other)?
                    .to_str()
                    .ok_or_else(|| io::Error::other("non-UTF8 filename"))?
                    .to_owned();
                out.push((name, entry.metadata()?.len()));
            }
        }
        Ok(())
    }
    let mut files = Vec::new();
    walk(root, root, 0, &mut files)?;
    files.sort_by(|a, b| a.0.cmp(&b.0));
    Ok(files)
}
struct Resources {
    root: PathBuf,
    files: Vec<(String, u64)>,
}
impl ResourceFiles for Resources {
    fn list(&self) -> Vec<(String, u64)> {
        self.files.clone()
    }
    fn open(&self, path: &str) -> io::Result<Box<dyn ArchiveSource>> {
        Ok(Box::new(std::fs::File::open(join(&self.root, path)?)?))
    }
}
const TEMP_SUFFIX: &str = ".ethornell-tmp";
const BACKUP_SUFFIX: &str = ".ethornell-bak";
fn sidecar(path: &Path, suffix: &str) -> PathBuf {
    let mut name = path.as_os_str().to_owned();
    name.push(suffix);
    PathBuf::from(name)
}
fn remove_if_present(path: &Path) -> io::Result<()> {
    match std::fs::remove_file(path) {
        Err(e) if e.kind() == io::ErrorKind::NotFound => Ok(()),
        result => result,
    }
}
// libnx's rename does not replace an existing destination. Keep the old file
// until the flushed replacement has its final name; recover interrupted moves
// on the next launch. Never depend on POSIX atomic-overwrite semantics.
fn recover(user: &Path) -> io::Result<()> {
    for (name, _) in list(user)? {
        if let Some(original) = name.strip_suffix(BACKUP_SUFFIX) {
            let backup = join(user, &name)?;
            let target = join(user, original)?;
            if target.try_exists()? {
                std::fs::remove_file(backup)?;
            } else {
                std::fs::rename(backup, target)?;
            }
        }
    }
    Ok(())
}
struct Saves(PathBuf);
impl UserStore for Saves {
    fn persist(&self, name: &str, bytes: Option<&[u8]>) {
        let write = || -> io::Result<()> {
            let path = join(&self.0, name)?;
            let Some(bytes) = bytes else {
                remove_if_present(&sidecar(&path, BACKUP_SUFFIX))?;
                return remove_if_present(&path);
            };
            if let Some(parent) = path.parent() {
                std::fs::create_dir_all(parent)?;
            }
            let tmp = sidecar(&path, TEMP_SUFFIX);
            let backup = sidecar(&path, BACKUP_SUFFIX);
            {
                use std::io::Write;
                let mut file = std::fs::File::create(&tmp)?;
                file.write_all(bytes)?;
                file.sync_all()?;
            }
            let had_previous = path.try_exists()?;
            if had_previous {
                remove_if_present(&backup)?;
                std::fs::rename(&path, &backup)?;
            }
            if let Err(error) = std::fs::rename(&tmp, &path) {
                if had_previous {
                    // Keep the backup if rollback also fails; recover() retries.
                    let _ = std::fs::rename(&backup, &path);
                }
                return Err(error);
            }
            remove_if_present(&backup)
        };
        if let Err(e) = write() {
            if let Ok(mut failure) = PERSIST_ERROR.lock() {
                *failure = Some(format!("save persistence failed: {e}"));
            }
        }
    }
}
pub(super) fn install(game: &Path, user: &Path) -> io::Result<()> {
    let resources = Resources {
        root: game.to_owned(),
        files: list(game)?,
    };
    recover(user)?;
    let mut saves = Vec::new();
    let mut total = 0u64;
    for (name, size) in list(user)? {
        if name.ends_with(TEMP_SUFFIX) || name.ends_with(BACKUP_SUFFIX) {
            continue;
        }
        total = total
            .checked_add(size)
            .ok_or_else(|| io::Error::other("save size overflow"))?;
        if total > 64 * 1024 * 1024 {
            return Err(io::Error::other("save budget exceeded"));
        }
        saves.push((name.clone(), std::fs::read(join(user, &name)?)?));
    }
    ethornell_app::hosted_files::install(
        Arc::new(resources),
        Arc::new(Saves(user.to_owned())),
        saves,
    );
    Ok(())
}

static PERSIST_ERROR: std::sync::Mutex<Option<String>> = std::sync::Mutex::new(None);
pub(super) fn take_persist_error() -> Option<String> {
    PERSIST_ERROR.lock().ok()?.take()
}
