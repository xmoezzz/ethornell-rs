//! Policy roots handed over by the host, for targets without directory
//! descriptors: the web page passes the picked game folder as
//! [`ResourceFiles`] (read in place, never copied), the consoles their game
//! directory. User data lives in memory and every change goes to
//! [`UserStore`], which the host persists. Names resolve as
//! in the Unix policy: `bgi-resource/` and `bgi-user/` select a root, a bare
//! name tries the user root and then the resource root, and components match
//! ignoring ASCII case.
use super::{FilePolicy, PolicyMetadata, WriterLock, relative, user_data_alias};
use ethornell_archive::ArchiveSource;
use std::collections::BTreeMap;
use std::io::{self, Read};
use std::path::Path;
use std::sync::{Arc, Mutex, OnceLock};

/// The picked game folder.
pub trait ResourceFiles: Send + Sync {
    /// Every file below the root: `/`-separated path and byte length.
    fn list(&self) -> Vec<(String, u64)>;
    fn open(&self, path: &str) -> io::Result<Box<dyn ArchiveSource>>;
}

/// Receives every change to user data, so the page can persist it.
pub trait UserStore: Send + Sync {
    /// `None` deletes the file.
    fn persist(&self, path: &str, bytes: Option<&[u8]>);
}

struct Backend {
    resources: Arc<dyn ResourceFiles>,
    store: Arc<dyn UserStore>,
    initial_user_files: Mutex<Vec<(String, Vec<u8>)>>,
}

static BACKEND: OnceLock<Backend> = OnceLock::new();

/// Installs the roots of this page's session (first call wins);
/// `user_files` are the saves the page restored from its storage.
pub fn install(
    resources: Arc<dyn ResourceFiles>,
    store: Arc<dyn UserStore>,
    user_files: Vec<(String, Vec<u8>)>,
) {
    let _ = BACKEND.set(Backend {
        resources,
        store,
        initial_user_files: Mutex::new(user_files),
    });
}

const USER_FILE_LIMIT: u64 = 64 * 1024 * 1024;

fn fold(path: &str) -> String {
    path.to_ascii_lowercase()
}

/// A tree of files keyed by folded path; directories are implied by files
/// and may also exist on their own (user roots create empty ones).
#[derive(Default)]
struct Tree<F> {
    files: BTreeMap<String, (String, F)>,
    dirs: BTreeMap<String, String>,
}

impl<F> Tree<F> {
    fn insert(&mut self, path: &str, file: F) {
        let mut parent = String::new();
        let parts = path.split('/').collect::<Vec<_>>();
        for part in &parts[..parts.len() - 1] {
            if !parent.is_empty() {
                parent.push('/');
            }
            parent.push_str(part);
            self.dirs
                .entry(fold(&parent))
                .or_insert_with(|| parent.clone());
        }
        self.files.insert(fold(path), (path.to_string(), file));
    }

    fn is_dir(&self, key: &str) -> bool {
        key.is_empty() || self.dirs.contains_key(key)
    }

    /// `(relative name, is_dir)` below `dir`, sorted by folded name as the
    /// Unix walk does; `recursive` includes every depth.
    fn children(&self, dir: &str, recursive: bool) -> Vec<(String, bool)> {
        let prefix = if dir.is_empty() {
            String::new()
        } else {
            format!("{dir}/")
        };
        let strip = |actual: &str| actual.get(prefix.len()..).map(str::to_string);
        let mut out = Vec::new();
        let entries = self
            .dirs
            .iter()
            .map(|(key, actual)| (key, actual, true))
            .chain(
                self.files
                    .iter()
                    .map(|(key, (actual, _))| (key, actual, false)),
            );
        for (key, actual, is_dir) in entries {
            if !key.starts_with(&prefix) || key.len() == prefix.len() {
                continue;
            }
            let rest = &key[prefix.len()..];
            if !recursive && rest.contains('/') {
                continue;
            }
            if let Some(name) = strip(actual) {
                out.push((name, is_dir));
            }
        }
        out.sort_by_key(|(name, _)| fold(name));
        out
    }
}

pub(crate) struct HostedRoots {
    resources: Arc<dyn ResourceFiles>,
    store: Arc<dyn UserStore>,
    resource_tree: Tree<u64>,
    user_tree: Mutex<Tree<Vec<u8>>>,
}

impl std::fmt::Debug for HostedRoots {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str("HostedRoots")
    }
}

#[derive(Clone, Copy)]
enum Root {
    Resource,
    User,
}

/// A checked relative name as a folded key ("" is the root).
fn key_of(name: &str) -> io::Result<String> {
    let path = relative(name).ok_or_else(|| io::Error::from(io::ErrorKind::PermissionDenied))?;
    let parts = path
        .components()
        .filter_map(|c| match c {
            std::path::Component::Normal(n) => Some(n.to_string_lossy().into_owned()),
            _ => None,
        })
        .collect::<Vec<_>>();
    Ok(fold(&parts.join("/")))
}

fn not_found() -> io::Error {
    io::Error::from(io::ErrorKind::NotFound)
}

impl HostedRoots {
    fn metadata_in(&self, root: Root, key: &str) -> Option<PolicyMetadata> {
        match root {
            Root::Resource => {
                if let Some((_, len)) = self.resource_tree.files.get(key) {
                    return Some(PolicyMetadata {
                        is_dir: false,
                        len: *len,
                        readonly: true,
                    });
                }
                self.resource_tree.is_dir(key).then_some(PolicyMetadata {
                    is_dir: true,
                    len: 0,
                    readonly: true,
                })
            }
            Root::User => {
                let tree = self.user_tree.lock().ok()?;
                if let Some((_, bytes)) = tree.files.get(key) {
                    return Some(PolicyMetadata {
                        is_dir: false,
                        len: bytes.len() as u64,
                        readonly: false,
                    });
                }
                tree.is_dir(key).then_some(PolicyMetadata {
                    is_dir: true,
                    len: 0,
                    readonly: false,
                })
            }
        }
    }

    fn read_in(&self, root: Root, key: &str, limit: u64) -> io::Result<Vec<u8>> {
        let budget = || io::Error::new(io::ErrorKind::InvalidData, "file budget exceeded");
        match root {
            Root::Resource => {
                let (actual, len) = self.resource_tree.files.get(key).ok_or_else(not_found)?;
                if *len > limit {
                    return Err(budget());
                }
                let mut bytes = Vec::new();
                self.resources
                    .open(actual)?
                    .take(limit + 1)
                    .read_to_end(&mut bytes)?;
                if bytes.len() as u64 > limit {
                    return Err(budget());
                }
                Ok(bytes)
            }
            Root::User => {
                let tree = self
                    .user_tree
                    .lock()
                    .map_err(|_| io::Error::other("user tree"))?;
                let (_, bytes) = tree.files.get(key).ok_or_else(not_found)?;
                if bytes.len() as u64 > limit {
                    return Err(budget());
                }
                Ok(bytes.clone())
            }
        }
    }

    /// The user-root name for a write: never the resource root, and with
    /// its parent directory present (created when `create`).
    fn user_target(&self, name: &str, create: bool) -> io::Result<(String, String)> {
        let normalized = user_data_alias(name);
        if normalized.starts_with("bgi-resource/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        let rest = normalized.strip_prefix("bgi-user/").unwrap_or(&normalized);
        let key = key_of(rest)?;
        if key.is_empty() {
            return Err(io::Error::from(io::ErrorKind::InvalidInput));
        }
        let mut tree = self
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        // Keep the spelling of existing components; new ones as given.
        let given = rest.replace('\\', "/");
        let parts = given
            .split('/')
            .filter(|p| !p.is_empty() && *p != ".")
            .collect::<Vec<_>>();
        let leaf_of = |path: &str| path.rsplit('/').next().unwrap_or(path).to_string();
        let mut actual = Vec::new();
        let mut prefix = String::new();
        for (index, part) in parts.iter().enumerate() {
            if !prefix.is_empty() {
                prefix.push('/');
            }
            prefix.push_str(&fold(part));
            if index + 1 == parts.len() {
                let existing = tree
                    .files
                    .get(&prefix)
                    .map(|(path, _)| path)
                    .or(tree.dirs.get(&prefix));
                actual.push(existing.map_or_else(|| part.to_string(), |path| leaf_of(path)));
            } else if let Some(dir) = tree.dirs.get(&prefix) {
                actual.push(leaf_of(dir));
            } else if create {
                actual.push(part.to_string());
                tree.dirs.insert(prefix.clone(), actual.join("/"));
            } else {
                return Err(not_found());
            }
        }
        Ok((key, actual.join("/")))
    }

    fn walk(&self, root: Root, dir: &str, recursive: bool) -> io::Result<Vec<(String, bool)>> {
        match root {
            Root::Resource => {
                if !self.resource_tree.is_dir(dir) {
                    return Err(not_found());
                }
                Ok(self.resource_tree.children(dir, recursive))
            }
            Root::User => {
                let tree = self
                    .user_tree
                    .lock()
                    .map_err(|_| io::Error::other("user tree"))?;
                if !tree.is_dir(dir) {
                    return Err(not_found());
                }
                Ok(tree.children(dir, recursive))
            }
        }
    }
}

impl FilePolicy {
    pub fn new(resource: &Path, user: &Path) -> io::Result<Self> {
        let backend = BACKEND.get().ok_or_else(|| {
            io::Error::new(
                io::ErrorKind::NotFound,
                "host_policy:101:0 no hosted file roots",
            )
        })?;
        let mut resource_tree = Tree::default();
        for (path, len) in backend.resources.list() {
            if key_of(&path).is_ok() {
                resource_tree.insert(&path, len);
            }
        }
        let mut user_tree = Tree::default();
        let initial = std::mem::take(&mut *backend.initial_user_files.lock().unwrap());
        for (path, bytes) in initial {
            if key_of(&path).is_ok() {
                user_tree.insert(&path, bytes);
            }
        }
        Ok(Self {
            resource: resource.to_path_buf(),
            user: user.to_path_buf(),
            hosted: Arc::new(HostedRoots {
                resources: backend.resources.clone(),
                store: backend.store.clone(),
                resource_tree,
                user_tree: Mutex::new(user_tree),
            }),
        })
    }
    pub fn lock_writer(&self) -> io::Result<WriterLock> {
        Ok(())
    }
    pub fn validate_resource_tree(&self) -> io::Result<()> {
        Ok(())
    }
    pub fn open_manager(&self) -> ethornell_core::Result<ethornell_archive::ResourceManager> {
        use std::io::{Seek, SeekFrom};
        let mut files = Vec::new();
        for (actual, len) in self.hosted.resource_tree.files.values() {
            if *len < 12 {
                continue;
            }
            let mut source = self.hosted.resources.open(actual)?;
            let mut header = [0u8; 12];
            source.read_exact(&mut header)?;
            if ethornell_archive::detect_archive_format_from_bytes(&header)
                == ethornell_archive::ArchiveFormat::Unknown
            {
                continue;
            }
            source.seek(SeekFrom::Start(0))?;
            files.push((self.resource.join(actual), source));
        }
        files.sort_by(|a, b| a.0.cmp(&b.0));
        ethornell_archive::ResourceManager::from_authorized_sources_with_limits(
            self.resource.clone(),
            files,
            ethornell_archive::ResourceBudget::android(),
        )
    }
    pub fn read_resource(&self, name: &str, limit: u64) -> io::Result<Vec<u8>> {
        let normalized = name.replace('\\', "/");
        if normalized.starts_with("bgi-user/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        let key = key_of(
            normalized
                .strip_prefix("bgi-resource/")
                .unwrap_or(&normalized),
        )?;
        self.hosted.read_in(Root::Resource, &key, limit)
    }
    pub fn metadata(&self, name: &str) -> Option<PolicyMetadata> {
        let normalized = name.replace('\\', "/");
        if let Some(rest) = normalized.strip_prefix("bgi-user/") {
            return self.hosted.metadata_in(Root::User, &key_of(rest).ok()?);
        }
        if let Some(rest) = normalized.strip_prefix("bgi-resource/") {
            return self.hosted.metadata_in(Root::Resource, &key_of(rest).ok()?);
        }
        let key = key_of(&normalized).ok()?;
        self.hosted
            .metadata_in(Root::User, &key)
            .or_else(|| self.hosted.metadata_in(Root::Resource, &key))
    }
    pub fn user_metadata(&self, name: &str) -> Option<PolicyMetadata> {
        self.metadata(&user_data_alias(name))
    }
    pub fn enumerate_user(
        &self,
        pattern: &str,
        recursive: bool,
        max_count: usize,
        directories: bool,
    ) -> io::Result<Vec<String>> {
        self.enumerate(&user_data_alias(pattern), recursive, max_count, directories)
    }
    pub fn read_loose(&self, name: &str) -> io::Result<Vec<u8>> {
        let normalized = name.replace('\\', "/");
        if let Some(rest) = normalized.strip_prefix("bgi-resource/") {
            return self
                .hosted
                .read_in(Root::Resource, &key_of(rest)?, 256 * 1024 * 1024);
        }
        if let Some(rest) = normalized.strip_prefix("bgi-user/") {
            return self
                .hosted
                .read_in(Root::User, &key_of(rest)?, USER_FILE_LIMIT);
        }
        let key = key_of(&normalized)?;
        match self.hosted.read_in(Root::User, &key, USER_FILE_LIMIT) {
            Err(error) if error.kind() == io::ErrorKind::NotFound => {
                self.hosted.read_in(Root::Resource, &key, 256 * 1024 * 1024)
            }
            other => other,
        }
    }
    pub fn enumerate(
        &self,
        pattern: &str,
        recursive: bool,
        max_count: usize,
        directories: bool,
    ) -> io::Result<Vec<String>> {
        let normalized = pattern.replace('\\', "/");
        let (root, name) = if let Some(name) = normalized.strip_prefix("bgi-resource/") {
            (Root::Resource, name)
        } else {
            (
                Root::User,
                normalized.strip_prefix("bgi-user/").unwrap_or(&normalized),
            )
        };
        let path =
            relative(name).ok_or_else(|| io::Error::from(io::ErrorKind::PermissionDenied))?;
        let parent = path.parent().unwrap_or(Path::new(""));
        let wildcard = path.file_name().and_then(|p| p.to_str()).unwrap_or("*");
        let dir = key_of(&parent.to_string_lossy())?;
        let limit = if max_count == 0 {
            10_000
        } else {
            max_count.clamp(1, 10_000)
        };
        Ok(self
            .hosted
            .walk(root, &dir, recursive)?
            .into_iter()
            .filter(|(name, is_dir)| {
                let leaf = name.rsplit('/').next().unwrap_or(name);
                *is_dir == directories
                    && !fold(leaf).starts_with(".ethornell-")
                    && crate::windows_wildcard_matches(wildcard, leaf)
            })
            .take(limit)
            .map(|(name, _)| name.replace('/', "\\"))
            .collect())
    }
    pub fn create_directory(&self, name: &str) -> io::Result<()> {
        let (key, actual) = self.hosted.user_target(name, false)?;
        let mut tree = self
            .hosted
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        if tree.files.contains_key(&key) || tree.dirs.contains_key(&key) {
            return Err(io::Error::from(io::ErrorKind::AlreadyExists));
        }
        tree.dirs.insert(key, actual);
        Ok(())
    }
    pub fn probe_writable(&self, name: &str) -> io::Result<()> {
        let normalized = user_data_alias(name);
        if name.replace('\\', "/").starts_with("bgi-resource/")
            && normalized.starts_with("bgi-user/")
        {
            // As on Unix: materialize the user-side UserData directory.
            let marker = format!("{}/.probe-parent", normalized.trim_end_matches('/'));
            self.hosted.user_target(&marker, true)?;
        }
        if normalized.starts_with("bgi-resource/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        let key = key_of(normalized.strip_prefix("bgi-user/").unwrap_or(&normalized))?;
        let tree = self
            .hosted
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        if tree.is_dir(&key) {
            Ok(())
        } else {
            Err(not_found())
        }
    }
    pub fn rename(&self, from: &str, to: &str) -> io::Result<()> {
        let (from_key, from_actual) = self.hosted.user_target(from, false)?;
        let (to_key, to_actual) = self.hosted.user_target(to, true)?;
        let mut tree = self
            .hosted
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        let (_, bytes) = tree.files.remove(&from_key).ok_or_else(not_found)?;
        self.hosted.store.persist(&from_actual, None);
        self.hosted.store.persist(&to_actual, Some(&bytes));
        tree.files.insert(to_key, (to_actual, bytes));
        Ok(())
    }
    pub fn delete(&self, name: &str) -> io::Result<()> {
        let (key, actual) = self.hosted.user_target(name, false)?;
        let mut tree = self
            .hosted
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        tree.files.remove(&key).ok_or_else(not_found)?;
        self.hosted.store.persist(&actual, None);
        Ok(())
    }
    pub fn read_user(&self, name: &str) -> io::Result<Vec<u8>> {
        let (key, _) = self.hosted.user_target(name, false)?;
        self.hosted.read_in(Root::User, &key, USER_FILE_LIMIT)
    }
    pub fn atomic_write(&self, name: &str, bytes: &[u8]) -> io::Result<()> {
        if bytes.len() as u64 > USER_FILE_LIMIT {
            return Err(io::Error::new(io::ErrorKind::InvalidData, "save too large"));
        }
        let (key, actual) = self.hosted.user_target(name, true)?;
        let mut tree = self
            .hosted
            .user_tree
            .lock()
            .map_err(|_| io::Error::other("user tree"))?;
        if tree.dirs.contains_key(&key) {
            return Err(io::Error::from(io::ErrorKind::IsADirectory));
        }
        self.hosted.store.persist(&actual, Some(bytes));
        tree.files.insert(key, (actual, bytes.to_vec()));
        Ok(())
    }
}
