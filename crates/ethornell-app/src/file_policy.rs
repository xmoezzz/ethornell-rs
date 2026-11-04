//! A scoped per-session policy. Desktop calls have no policy. Host calls install
//! it with an RAII guard, including during bootstrap and all native dispatch.
use std::{
    cell::RefCell,
    io::{self, Write},
    path::{Component, Path, PathBuf},
};

// Bounded, non-secret diagnostics. Do not embed any user path or upstream IO
// message: the ABI may safely expose this numeric stage and errno only.
fn policy_error(code: u32, error: io::Error) -> io::Error {
    io::Error::new(error.kind(), format!("host_policy:{code}:{}", error.raw_os_error().unwrap_or(0)))
}

/// What the policy reports about a name (std::fs::Metadata has no portable
/// constructor, and the web build has no host files).
#[derive(Clone, Copy, Debug)]
pub(crate) struct PolicyMetadata {
    is_dir: bool,
    len: u64,
    readonly: bool,
}
impl PolicyMetadata {
    pub fn is_file(&self) -> bool { !self.is_dir }
    pub fn is_dir(&self) -> bool { self.is_dir }
    pub fn len(&self) -> u64 { self.len }
    pub fn readonly(&self) -> bool { self.readonly }
}
impl From<std::fs::Metadata> for PolicyMetadata {
    fn from(m: std::fs::Metadata) -> Self {
        Self { is_dir: m.is_dir(), len: m.len(), readonly: m.permissions().readonly() }
    }
}
/// Held for the session: the host's exclusive claim on the user root.
#[cfg(not(hosted_files))]
pub(crate) type WriterLock = std::fs::File;
#[cfg(hosted_files)]
pub(crate) type WriterLock = ();

#[derive(Clone, Debug)]
pub(crate) struct FilePolicy {
    pub resource: PathBuf,
    pub user: PathBuf,
    #[cfg(fd_policy)]
    resource_dir: std::sync::Arc<std::fs::File>,
    #[cfg(fd_policy)]
    user_dir: std::sync::Arc<std::fs::File>,
    #[cfg(hosted_files)]
    hosted: std::sync::Arc<hosted::HostedRoots>,
}
#[cfg(hosted_files)]
pub mod hosted;
thread_local! { static ACTIVE: RefCell<Option<FilePolicy>> = const { RefCell::new(None) }; }
pub(crate) fn active() -> Option<FilePolicy> {
    ACTIVE.with(|p| p.borrow().clone())
}
pub(crate) fn restricted() -> bool {
    ACTIVE.with(|p| p.borrow().is_some())
}
pub(crate) fn scope<T>(policy: &FilePolicy, f: impl FnOnce() -> T) -> T {
    struct Restore(Option<FilePolicy>);
    impl Drop for Restore {
        fn drop(&mut self) {
            ACTIVE.with(|p| *p.borrow_mut() = self.0.take());
        }
    }
    let _restore = Restore(ACTIVE.with(|p| p.replace(Some(policy.clone()))));
    f()
}
fn relative(name: &str) -> Option<PathBuf> {
    if name.contains('\0') || name.contains(':') || name.starts_with(['/', '\\']) {
        return None;
    }
    let normalized = name.replace('\\', "/");
    let path = Path::new(&normalized);
    if path
        .components()
        .any(|c| !matches!(c, Component::Normal(_) | Component::CurDir))
    {
        return None;
    }
    if path.components().any(|c| matches!(c, Component::Normal(n) if n.to_string_lossy().to_ascii_lowercase().starts_with(".ethornell-"))) { return None; }
    Some(path.to_owned())
}

// Some BGI scripts build their user-data directory from the primary install
// root. Only this conventional user-data subtree is a writable virtual alias;
// arbitrary resource-root names remain read-only. It never denotes host IO in
// the resource directory and never imports original save bytes.
pub(crate) fn user_data_alias(name: &str) -> String {
    let normalized=name.replace('\\',"/");
    if let Some(rest)=normalized.strip_prefix("bgi-resource/") {
        let first=rest.split('/').next().unwrap_or("");
        if first.eq_ignore_ascii_case("UserData") {return format!("bgi-user/{rest}");}
    }
    normalized
}

#[cfg(fd_policy)]
unsafe fn errno_ptr() -> *mut libc::c_int {
    #[cfg(any(target_vendor = "apple", target_os = "freebsd"))]
    {
        unsafe { libc::__error() }
    }
    #[cfg(target_os = "android")]
    {
        unsafe { libc::__errno() }
    }
    #[cfg(all(
        not(target_vendor = "apple"),
        not(target_os = "android"),
        not(target_os = "freebsd")
    ))]
    {
        unsafe { libc::__errno_location() }
    }
}

#[cfg(fd_policy)]
fn next_dir_entry(stream: *mut libc::DIR) -> io::Result<*mut libc::dirent> {
    unsafe {
        *errno_ptr() = 0;
        let entry = libc::readdir(stream);
        if entry.is_null() && *errno_ptr() != 0 {
            return Err(io::Error::last_os_error());
        }
        Ok(entry)
    }
}

// Enumerate the opened directory, not a checked path that can be swapped before
// read_dir. Every recursive descent is opened relative to that same descriptor.
#[cfg(fd_policy)]
fn walk_directory(
    dir: &std::fs::File,
    prefix: &Path,
    depth: usize,
    count: &mut usize,
    recurse: bool,
    visit: &mut impl FnMut(&Path, &std::fs::File) -> io::Result<()>,
) -> io::Result<()> {
    use std::{
        ffi::{CStr, CString},
        os::fd::{AsRawFd, FromRawFd, IntoRawFd},
        os::unix::ffi::OsStrExt,
    };
    if depth > 64 {
        return Err(io::Error::new(
            io::ErrorKind::InvalidData,
            "directory depth budget exceeded",
        ));
    }
    // openat(".") gives a fresh directory stream offset; dup/try_clone does not.
    let dot = CString::new(".").unwrap();
    let fd = unsafe {
        libc::openat(
            dir.as_raw_fd(),
            dot.as_ptr(),
            libc::O_RDONLY | libc::O_DIRECTORY | libc::O_CLOEXEC,
        )
    };
    if fd < 0 {
        return Err(io::Error::last_os_error());
    }
    let file = unsafe { std::fs::File::from_raw_fd(fd) };
    let stream = unsafe { libc::fdopendir(file.as_raw_fd()) };
    if stream.is_null() {
        return Err(io::Error::last_os_error());
    }
    let _ = file.into_raw_fd(); // fdopendir owns it after success.
    struct Stream(*mut libc::DIR);
    impl Drop for Stream {
        fn drop(&mut self) {
            unsafe {
                libc::closedir(self.0);
            }
        }
    }
    let stream = Stream(stream);
    let mut names = Vec::new();
    loop {
        let entry = next_dir_entry(stream.0)?;
        if entry.is_null() {
            break;
        }
        let name = unsafe { CStr::from_ptr((*entry).d_name.as_ptr()) };
        if name.to_bytes() == b"." || name.to_bytes() == b".." {
            continue;
        }
        *count += 1;
        if *count > 100_000 {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "directory entry budget exceeded",
            ));
        }
        names.push(name.to_owned());
    }
    names.sort_by_key(|n| n.to_bytes().to_ascii_lowercase());
    for name in names {
        if name
            .to_bytes()
            .to_ascii_lowercase()
            .starts_with(b".ethornell-")
        {
            continue;
        }
        let fd = unsafe {
            libc::openat(
                dir.as_raw_fd(),
                name.as_ptr(),
                libc::O_RDONLY | libc::O_NOFOLLOW | libc::O_CLOEXEC | libc::O_NONBLOCK,
            )
        };
        if fd < 0 {
            return Err(io::Error::last_os_error());
        }
        let file = unsafe { std::fs::File::from_raw_fd(fd) };
        let metadata = file.metadata()?;
        if !metadata.is_file() && !metadata.is_dir() {
            return Err(io::Error::new(
                io::ErrorKind::PermissionDenied,
                "special file denied",
            ));
        }
        let path = prefix.join(std::ffi::OsStr::from_bytes(name.as_bytes()));
        visit(&path, &file)?;
        if metadata.is_dir() && recurse {
            walk_directory(&file, &path, depth + 1, count, recurse, visit)?;
        }
    }
    Ok(())
}

#[cfg(fd_policy)]
fn case_component(
    dir: &std::fs::File,
    requested: &std::ffi::OsStr,
) -> io::Result<std::ffi::CString> {
    use std::{
        ffi::{CStr, CString},
        os::{
            fd::{AsRawFd, FromRawFd, IntoRawFd},
            unix::ffi::OsStrExt,
        },
    };
    let mut matched = None;
    let mut ambiguous = false;
    let dot = CString::new(".").unwrap();
    let fd = unsafe {
        libc::openat(
            dir.as_raw_fd(),
            dot.as_ptr(),
            libc::O_RDONLY | libc::O_DIRECTORY | libc::O_CLOEXEC,
        )
    };
    if fd < 0 {
        return Err(io::Error::last_os_error());
    }
    let file = unsafe { std::fs::File::from_raw_fd(fd) };
    let stream = unsafe { libc::fdopendir(file.as_raw_fd()) };
    if stream.is_null() {
        return Err(io::Error::last_os_error());
    }
    let _ = file.into_raw_fd();
    struct Close(*mut libc::DIR);
    impl Drop for Close {
        fn drop(&mut self) {
            unsafe {
                libc::closedir(self.0);
            }
        }
    }
    let _close = Close(stream);
    for _ in 0..100_001 {
        let entry = next_dir_entry(stream)?;
        if entry.is_null() {
            if ambiguous {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    "ambiguous case-insensitive filename",
                ));
            }
            return Ok(CString::new(
                matched.as_deref().unwrap_or(requested).as_bytes(),
            )?);
        }
        let name = unsafe { CStr::from_ptr((*entry).d_name.as_ptr()) }.to_bytes();
        if name == requested.as_bytes() {
            return Ok(CString::new(name)?);
        }
        if name.eq_ignore_ascii_case(requested.as_bytes()) {
            ambiguous |= matched.is_some();
            matched = Some(std::ffi::OsStr::from_bytes(name).to_owned());
        }
    }
    return Err(io::Error::new(
        io::ErrorKind::InvalidData,
        "directory entry budget exceeded",
    ));
}

#[cfg(fd_policy)]
impl FilePolicy {
    #[cfg(fd_policy)]
    pub fn lock_writer(&self) -> io::Result<WriterLock> {
        use std::os::{fd::AsRawFd, unix::fs::OpenOptionsExt};
        use std::{ffi::CString, os::fd::FromRawFd};
        let name = CString::new(".ethornell-session.lock").unwrap();
        let fd = unsafe {
            libc::openat(
                self.user_dir.as_raw_fd(),
                name.as_ptr(),
                libc::O_RDWR | libc::O_CREAT | libc::O_NOFOLLOW | libc::O_CLOEXEC,
                0o600,
            )
        };
        if fd < 0 {
            return Err(policy_error(201, io::Error::last_os_error()));
        }
        let file = unsafe { std::fs::File::from_raw_fd(fd) };
        if unsafe { libc::flock(file.as_raw_fd(), libc::LOCK_EX | libc::LOCK_NB) } < 0 {
            return Err(policy_error(202, io::Error::last_os_error()));
        }
        Ok(file)
    }
    #[cfg(fd_policy)]
    pub fn open_resource(&self, name: &str) -> io::Result<std::fs::File> {
        let normalized = name.replace('\\', "/");
        if normalized.starts_with("bgi-user/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        self.open_relative(
            &self.resource_dir,
            normalized
                .strip_prefix("bgi-resource/")
                .unwrap_or(&normalized),
            false,
        )
    }
    /// A resource-root file of at most `limit` bytes.
    pub fn read_resource(&self, name: &str, limit: u64) -> io::Result<Vec<u8>> {
        use std::io::Read;
        let file = self.open_resource(name)?;
        let budget = || io::Error::new(io::ErrorKind::InvalidData, "resource file budget exceeded");
        if file.metadata()?.len() > limit {
            return Err(budget());
        }
        let mut bytes = Vec::new();
        file.take(limit + 1).read_to_end(&mut bytes)?;
        if bytes.len() as u64 > limit {
            return Err(budget());
        }
        Ok(bytes)
    }
    fn open_relative(
        &self,
        root: &std::fs::File,
        name: &str,
        directory: bool,
    ) -> io::Result<std::fs::File> {
        use std::{
            ffi::CString,
            os::fd::{AsRawFd, FromRawFd},
            os::unix::ffi::OsStrExt,
        };
        let normalized = name.replace('\\', "/");
        let rel = relative(&normalized)
            .ok_or_else(|| io::Error::from(io::ErrorKind::PermissionDenied))?;
        let mut dir = root.try_clone()?;
        let parts = rel
            .components()
            .filter_map(|c| {
                if let Component::Normal(n) = c {
                    Some(n)
                } else {
                    None
                }
            })
            .collect::<Vec<_>>();
        if parts.is_empty() && directory {
            return Ok(dir);
        }
        if parts.is_empty() {
            return Err(io::Error::from(io::ErrorKind::InvalidInput));
        }
        for (i, part) in parts.iter().enumerate() {
            let c = case_component(&dir, part)?;
            let flags = libc::O_RDONLY
                | libc::O_NOFOLLOW
                | libc::O_CLOEXEC
                | libc::O_NONBLOCK
                | if i + 1 < parts.len() || directory {
                    libc::O_DIRECTORY
                } else {
                    0
                };
            let fd = unsafe { libc::openat(dir.as_raw_fd(), c.as_ptr(), flags) };
            if fd < 0 {
                return Err(io::Error::last_os_error());
            }
            dir = unsafe { std::fs::File::from_raw_fd(fd) };
        }
        if !directory && !dir.metadata()?.is_file() {
            return Err(io::Error::from(io::ErrorKind::InvalidInput));
        }
        Ok(dir)
    }
    pub fn open_manager(&self) -> ethornell_core::Result<ethornell_archive::ResourceManager> {
        use std::io::{Read, Seek, SeekFrom};
        let mut files = Vec::new();
        walk_directory(
            &self.resource_dir,
            Path::new(""),
            0,
            &mut 0,
            true,
            &mut |rel, file| {
                if !file.metadata()?.is_file() {
                    return Ok(());
                }
                let mut file = file.try_clone()?;
                let mut header = [0u8; 12];
                let read = file.read(&mut header)?;
                if ethornell_archive::detect_archive_format_from_bytes(&header[..read])
                    == ethornell_archive::ArchiveFormat::Unknown
                {
                    return Ok(());
                }
                if files.len() >= 512 {
                    return Err(io::Error::new(
                        io::ErrorKind::InvalidData,
                        "archive descriptor budget exceeded",
                    ));
                }
                file.seek(SeekFrom::Start(0))?;
                files.push((self.resource.join(rel), file));
                Ok(())
            },
        )?;
        files.sort_by(|a, b| a.0.cmp(&b.0));
        ethornell_archive::ResourceManager::from_authorized_files_with_limits(
            self.resource.clone(),
            files,
            ethornell_archive::ResourceBudget::android(),
        )
    }
    pub fn new(resource: &Path, user: &Path) -> io::Result<Self> {
        fn open_root(path: &Path, stage: u32) -> io::Result<(PathBuf, std::fs::File)> {
            use std::{
                ffi::CString,
                os::{
                    fd::{AsRawFd, FromRawFd},
                    unix::{ffi::OsStrExt, fs::{OpenOptionsExt, MetadataExt}},
                },
            };
            // Resolve a host-authorized alias once (e.g. /sdcard or macOS
            // /var), then bind every canonical component without following
            // links. A replaced ancestor cannot redirect the subsequent open.
            let absolute = path.canonicalize().map_err(|e|policy_error(stage,e))?;
            let expected = std::fs::metadata(&absolute).map_err(|e|policy_error(stage+1,e))?;
            // Android's /data and /data/user/0 can be execute-only to an app.
            // O_RDONLY would require unrelated ancestor listing permission.
            // O_PATH retains only traversal capability; nofollow still rejects
            // a substituted symlink at every component. Only the final root is
            // reopened O_RDONLY to prove/read its actual directory contents.
            #[cfg(any(target_os="android", target_os="linux"))]
            let ancestor_mode = libc::O_PATH;
            #[cfg(not(any(target_os="android", target_os="linux")))]
            let ancestor_mode = libc::O_RDONLY;
            let mut dir = std::fs::OpenOptions::new()
                .read(true)
                .custom_flags(ancestor_mode | libc::O_DIRECTORY | libc::O_NOFOLLOW | libc::O_CLOEXEC)
                .open("/").map_err(|e|policy_error(stage+2,e))?;
            let mut normalized = PathBuf::from("/");
            for component in absolute.components() {
                match component {
                    Component::RootDir | Component::CurDir => {}
                    Component::Normal(part) => {
                        let c = CString::new(part.as_bytes())?;
                        let fd = unsafe {
                            libc::openat(
                                dir.as_raw_fd(),
                                c.as_ptr(),
                                ancestor_mode
                                    | libc::O_DIRECTORY
                                    | libc::O_NOFOLLOW
                                    | libc::O_CLOEXEC,
                            )
                        };
                        if fd < 0 {
                            return Err(policy_error(stage+3, io::Error::last_os_error()));
                        }
                        dir = unsafe { std::fs::File::from_raw_fd(fd) };
                        normalized.push(part);
                    }
                    _ => return Err(io::Error::from(io::ErrorKind::PermissionDenied)),
                }
            }
            let dot = CString::new(".").unwrap();
            let fd = unsafe {libc::openat(dir.as_raw_fd(),dot.as_ptr(),libc::O_RDONLY|libc::O_DIRECTORY|libc::O_NOFOLLOW|libc::O_CLOEXEC)};
            if fd<0 {return Err(policy_error(stage+4,io::Error::last_os_error()));}
            let dir = unsafe {std::fs::File::from_raw_fd(fd)};
            let bound = dir.metadata().map_err(|e|policy_error(stage+5,e))?;
            if !bound.is_dir() || (bound.dev(),bound.ino()) != (expected.dev(),expected.ino()) {
                return Err(policy_error(stage+6,io::Error::from(io::ErrorKind::PermissionDenied)));
            }
            Ok((normalized, dir))
        }
        let (resource, resource_dir) = open_root(resource,101)?;
        let (user, user_dir) = open_root(user,111)?;
        if user.starts_with(&resource) || resource.starts_with(&user) {
            return Err(io::Error::new(
                io::ErrorKind::PermissionDenied,
                "host_policy:120:0",
            ));
        }
        Ok(Self {
            resource,
            user,
            resource_dir: std::sync::Arc::new(resource_dir),
            user_dir: std::sync::Arc::new(user_dir),
        })
    }
    /// ResourceManager caches archive descriptors. Reject links before opening
    /// its index so a game cannot smuggle an outside archive into that cache.
    pub fn validate_resource_tree(&self) -> io::Result<()> {
        walk_directory(
            &self.resource_dir,
            Path::new(""),
            0,
            &mut 0,
            true,
            &mut |_, _| Ok(()),
        )
    }
    pub fn metadata(&self, name: &str) -> Option<PolicyMetadata> {
        let normalized = name.replace('\\', "/");
        let (root, name, explicit) = if let Some(name) = normalized.strip_prefix("bgi-user/") {
            (&self.user_dir, name, true)
        } else if let Some(name) = normalized.strip_prefix("bgi-resource/") {
            (&self.resource_dir, name, true)
        } else {
            (&self.user_dir, normalized.as_str(), false)
        };
        for directory in [false, true] {
            if let Ok(file) = self.open_relative(root, name, directory) {
                return file.metadata().ok().map(PolicyMetadata::from);
            }
        }
        if !explicit
            && self
                .open_relative(root, name, true)
                .is_err_and(|e| e.kind() == io::ErrorKind::NotFound)
        {
            for directory in [false, true] {
                if let Ok(file) = self.open_relative(&self.resource_dir, name, directory) {
                    return file.metadata().ok().map(PolicyMetadata::from);
                }
            }
        }
        None
    }
    pub fn user_metadata(&self,name:&str)->Option<PolicyMetadata> {self.metadata(&user_data_alias(name))}
    pub fn enumerate_user(&self,pattern:&str,recursive:bool,max_count:usize,directories:bool)->io::Result<Vec<String>> {self.enumerate(&user_data_alias(pattern),recursive,max_count,directories)}
    pub fn read_loose(&self, name: &str) -> io::Result<Vec<u8>> {
        use std::io::Read;
        let normalized = name.replace('\\', "/");
        let (file, limit) = if let Some(name) = normalized.strip_prefix("bgi-resource/") {
            (self.open_resource(name)?, 256 * 1024 * 1024)
        } else if let Some(name) = normalized.strip_prefix("bgi-user/") {
            (
                self.open_relative(&self.user_dir, name, false)?,
                64 * 1024 * 1024,
            )
        } else {
            match self.open_relative(&self.user_dir, &normalized, false) {
                Ok(file) => (file, 64 * 1024 * 1024),
                Err(error) if error.kind() == io::ErrorKind::NotFound => {
                    (self.open_resource(&normalized)?, 256 * 1024 * 1024)
                }
                Err(error) => return Err(error),
            }
        };
        if file.metadata()?.len() > limit {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "loose file budget exceeded",
            ));
        }
        let mut bytes = Vec::new();
        file.take(limit + 1).read_to_end(&mut bytes)?;
        if bytes.len() as u64 > limit {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "loose file budget exceeded",
            ));
        }
        Ok(bytes)
    }
    pub fn enumerate(
        &self,
        pattern: &str,
        recursive: bool,
        max_count: usize,
        directories: bool,
    ) -> io::Result<Vec<String>> {
        let normalized = pattern.replace('\\', "/");
        let (base, name) = if let Some(name) = normalized.strip_prefix("bgi-resource/") {
            (&self.resource_dir, name)
        } else {
            (
                &self.user_dir,
                normalized.strip_prefix("bgi-user/").unwrap_or(&normalized),
            )
        };
        let path =
            relative(name).ok_or_else(|| io::Error::from(io::ErrorKind::PermissionDenied))?;
        let parent = path.parent().unwrap_or(Path::new(""));
        let wildcard = path.file_name().and_then(|p| p.to_str()).unwrap_or("*");
        let root = self.open_relative(base, &parent.to_string_lossy(), true)?;
        let mut output = Vec::new();
        walk_directory(
            &root,
            Path::new(""),
            0,
            &mut 0,
            recursive,
            &mut |name, file| {
                if (!recursive && name.components().count() != 1)
                    || output.len() >= max_count.clamp(1, 10_000) && max_count != 0
                    || output.len() >= 10_000
                {
                    return Ok(());
                }
                let leaf = name.file_name().unwrap().to_string_lossy();
                if leaf.starts_with(".ethornell-") {
                    return Ok(());
                }
                if file.metadata()?.is_dir() == directories
                    && super::windows_wildcard_matches(wildcard, &leaf)
                {
                    output.push(name.to_string_lossy().replace('/', "\\"));
                }
                Ok(())
            },
        )?;
        Ok(output)
    }
    #[cfg(fd_policy)]
    fn parent(&self, name: &str, create: bool) -> io::Result<(std::fs::File, std::ffi::CString)> {
        use std::{
            ffi::CString,
            os::{
                fd::{AsRawFd, FromRawFd},
                unix::ffi::OsStrExt,
            },
        };
        let normalized = user_data_alias(name);
        if normalized.starts_with("bgi-resource/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        let rel = relative(normalized.strip_prefix("bgi-user/").unwrap_or(&normalized))
            .ok_or_else(|| io::Error::from(io::ErrorKind::PermissionDenied))?;
        let parts = rel
            .components()
            .filter_map(|c| {
                if let Component::Normal(n) = c {
                    Some(n)
                } else {
                    None
                }
            })
            .collect::<Vec<_>>();
        if parts.is_empty() {
            return Err(io::Error::from(io::ErrorKind::InvalidInput));
        }
        let mut dir = self.user_dir.try_clone()?;
        for part in &parts[..parts.len() - 1] {
            let c = case_component(&dir, part)?;
            if create {
                let created = unsafe { libc::mkdirat(dir.as_raw_fd(), c.as_ptr(), 0o700) };
                if created == 0 {
                    dir.sync_all()?;
                }
            }
            let fd = unsafe {
                libc::openat(
                    dir.as_raw_fd(),
                    c.as_ptr(),
                    libc::O_RDONLY | libc::O_DIRECTORY | libc::O_NOFOLLOW | libc::O_CLOEXEC,
                )
            };
            if fd < 0 {
                return Err(io::Error::last_os_error());
            }
            dir = unsafe { std::fs::File::from_raw_fd(fd) };
        }
        let leaf = case_component(&dir, parts.last().unwrap())?;
        Ok((dir, leaf))
    }
    pub fn create_directory(&self, name: &str) -> io::Result<()> {
        use std::os::fd::AsRawFd;
        let (dir, name) = self.parent(name, false)?;
        if unsafe { libc::mkdirat(dir.as_raw_fd(), name.as_ptr(), 0o700) } < 0 {
            return Err(io::Error::last_os_error());
        }
        dir.sync_all()
    }
    pub fn probe_writable(&self, name: &str) -> io::Result<()> {
        use std::os::fd::{AsRawFd, FromRawFd};
        let normalized = user_data_alias(name);
        if name.replace('\\',"/").starts_with("bgi-resource/") && normalized.starts_with("bgi-user/") {
            // The original resource UserData may exist while the separate user
            // tree is empty. Materialize only this authorized user directory,
            // not the original contents, before the script's writable probe.
            let marker=format!("{}/.probe-parent",normalized.trim_end_matches('/'));
            let _=self.parent(&marker,true)?;
        }
        if normalized.starts_with("bgi-resource/") {
            return Err(io::Error::from(io::ErrorKind::PermissionDenied));
        }
        let directory = self.open_relative(
            &self.user_dir,
            normalized.strip_prefix("bgi-user/").unwrap_or(&normalized),
            true,
        )?;
        let name =
            std::ffi::CString::new(format!(".ethornell-probe-{}", std::process::id())).unwrap();
        let fd = unsafe {
            libc::openat(
                directory.as_raw_fd(),
                name.as_ptr(),
                libc::O_WRONLY | libc::O_CREAT | libc::O_EXCL | libc::O_NOFOLLOW | libc::O_CLOEXEC,
                0o600,
            )
        };
        if fd < 0 {
            return Err(io::Error::last_os_error());
        }
        drop(unsafe { std::fs::File::from_raw_fd(fd) });
        if unsafe { libc::unlinkat(directory.as_raw_fd(), name.as_ptr(), 0) } < 0 {
            return Err(io::Error::last_os_error());
        }
        Ok(())
    }
    #[cfg(fd_policy)]
    pub fn rename(&self, from: &str, to: &str) -> io::Result<()> {
        use std::os::fd::AsRawFd;
        // Both source and destination are resolved under the USER root; a
        // resource file cannot be moved into user storage or overwritten.
        let (from_dir, from) = self.parent(from, false)?;
        let (to_dir, to) = self.parent(to, true)?;
        if unsafe {
            libc::renameat(
                from_dir.as_raw_fd(),
                from.as_ptr(),
                to_dir.as_raw_fd(),
                to.as_ptr(),
            )
        } < 0
        {
            return Err(io::Error::last_os_error());
        }
        from_dir
            .sync_all()
            .and_then(|_| to_dir.sync_all())
            .map_err(|e| io::Error::new(e.kind(), "rename committed; durability uncertain"))
    }
    #[cfg(fd_policy)]
    pub fn delete(&self, name: &str) -> io::Result<()> {
        use std::os::fd::AsRawFd;
        let (dir, name) = self.parent(name, false)?;
        if unsafe { libc::unlinkat(dir.as_raw_fd(), name.as_ptr(), 0) } < 0 {
            return Err(io::Error::last_os_error());
        }
        dir.sync_all()
    }
    #[cfg(fd_policy)]
    pub fn read_user(&self, name: &str) -> io::Result<Vec<u8>> {
        use std::{
            io::Read,
            os::fd::{AsRawFd, FromRawFd},
        };
        let (dir, name) = self.parent(name, false)?;
        let fd = unsafe {
            libc::openat(
                dir.as_raw_fd(),
                name.as_ptr(),
                libc::O_RDONLY | libc::O_NOFOLLOW | libc::O_CLOEXEC | libc::O_NONBLOCK,
            )
        };
        if fd < 0 {
            return Err(io::Error::last_os_error());
        }
        let file = unsafe { std::fs::File::from_raw_fd(fd) };
        if !file.metadata()?.is_file() || file.metadata()?.len() > 64 * 1024 * 1024 {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "user file budget exceeded",
            ));
        }
        let mut bytes = Vec::new();
        file.take(64 * 1024 * 1024 + 1).read_to_end(&mut bytes)?;
        if bytes.len() > 64 * 1024 * 1024 {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "user file budget exceeded",
            ));
        }
        Ok(bytes)
    }
    /// Save writes use directory FDs + O_NOFOLLOW for every path component and
    /// atomic rename within that directory; no symlink parent can redirect IO.
    #[cfg(fd_policy)]
    pub fn atomic_write(&self, name: &str, bytes: &[u8]) -> io::Result<()> {
        use std::{
            ffi::CString,
            os::fd::{AsRawFd, FromRawFd, OwnedFd},
        };
        if bytes.len() > 64 * 1024 * 1024 {
            return Err(io::Error::new(io::ErrorKind::InvalidData, "save too large"));
        }
        let (dir, dest) = self.parent(name, true)?;
        static SERIAL: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(1);
        let temp = CString::new(format!(
            ".ethornell-save-{}-{}",
            std::process::id(),
            SERIAL.fetch_add(1, std::sync::atomic::Ordering::Relaxed)
        ))?;
        let fd = unsafe {
            libc::openat(
                dir.as_raw_fd(),
                temp.as_ptr(),
                libc::O_WRONLY | libc::O_CREAT | libc::O_EXCL | libc::O_NOFOLLOW | libc::O_CLOEXEC,
                0o600,
            )
        };
        if fd < 0 {
            return Err(io::Error::last_os_error());
        }
        let mut file = std::fs::File::from(unsafe { OwnedFd::from_raw_fd(fd) });
        let result = (|| {
            file.write_all(bytes)?;
            file.sync_all()?;
            if unsafe {
                libc::renameat(
                    dir.as_raw_fd(),
                    temp.as_ptr(),
                    dir.as_raw_fd(),
                    dest.as_ptr(),
                )
            } < 0
            {
                return Err(io::Error::last_os_error());
            }
            dir.sync_all()
                .map_err(|e| io::Error::new(e.kind(), "save committed; durability uncertain"))
        })();
        if result.is_err() {
            unsafe {
                libc::unlinkat(dir.as_raw_fd(), temp.as_ptr(), 0);
            }
        }
        result
    }
}

// Host-driven IO is currently available on Unix (Android/macOS/Linux). Keep
// Windows desktop compiling without accidentally selecting a permissive host
// implementation. Desktop paths never install this policy.
#[cfg(not(any(fd_policy, hosted_files)))]
impl FilePolicy {
    fn unavailable<T>() -> io::Result<T> {
        Err(io::Error::new(
            io::ErrorKind::Unsupported,
            "restricted host IO unsupported on this platform",
        ))
    }
    pub fn new(_: &Path, _: &Path) -> io::Result<Self> {
        Self::unavailable()
    }
    pub fn lock_writer(&self) -> io::Result<WriterLock> {
        Self::unavailable()
    }
    pub fn read_resource(&self, _: &str, _: u64) -> io::Result<Vec<u8>> {
        Self::unavailable()
    }
    pub fn open_manager(&self) -> ethornell_core::Result<ethornell_archive::ResourceManager> {
        Err(ethornell_core::EthornellError::Io(io::Error::new(
            io::ErrorKind::Unsupported,
            "restricted host IO unsupported",
        )))
    }
    pub fn validate_resource_tree(&self) -> io::Result<()> {
        Self::unavailable()
    }
    pub fn metadata(&self, _: &str) -> Option<PolicyMetadata> {
        None
    }
    pub fn user_metadata(&self,_:&str)->Option<PolicyMetadata> {None}
    pub fn enumerate_user(&self,_:&str,_:bool,_:usize,_:bool)->io::Result<Vec<String>> {Self::unavailable()}
    pub fn read_loose(&self, _: &str) -> io::Result<Vec<u8>> {
        Self::unavailable()
    }
    pub fn enumerate(&self, _: &str, _: bool, _: usize, _: bool) -> io::Result<Vec<String>> {
        Self::unavailable()
    }
    pub fn create_directory(&self, _: &str) -> io::Result<()> {
        Self::unavailable()
    }
    pub fn probe_writable(&self, _: &str) -> io::Result<()> {
        Self::unavailable()
    }
    pub fn rename(&self, _: &str, _: &str) -> io::Result<()> {
        Self::unavailable()
    }
    pub fn delete(&self, _: &str) -> io::Result<()> {
        Self::unavailable()
    }
    pub fn read_user(&self, _: &str) -> io::Result<Vec<u8>> {
        Self::unavailable()
    }
    pub fn atomic_write(&self, _: &str, _: &[u8]) -> io::Result<()> {
        Self::unavailable()
    }
}

#[cfg(all(test, fd_policy))]
mod tests {
    use super::*;
    #[test]
    fn rejects_escape_forms() {
        for p in [
            "../save",
            "/tmp/x",
            "C:\\x",
            "\\\\server\\file",
            "a/../../x",
        ] {
            assert!(relative(p).is_none(), "{p}");
        }
        assert_eq!(relative("save\\a.dat"), Some(PathBuf::from("save/a.dat")));
    }
    fn roots(label: &str) -> (PathBuf, FilePolicy) {
        static N: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(1);
        let base = std::env::temp_dir().join(format!(
            "ethornell-policy-{}-{label}-{}",
            std::process::id(),
            N.fetch_add(1, std::sync::atomic::Ordering::Relaxed)
        ));
        std::fs::create_dir_all(base.join("resource")).unwrap();
        std::fs::create_dir_all(base.join("user")).unwrap();
        let policy = FilePolicy::new(&base.join("resource"), &base.join("user")).unwrap();
        (base, policy)
    }
    #[test]
    fn saves_are_atomic_and_rename_both_ends_are_contained() {
        let (base, p) = roots("atomic");
        std::fs::write(p.resource.join("story"), b"resource").unwrap();
        p.atomic_write("save/slot.dat", b"old").unwrap();
        p.atomic_write("save/slot.dat", b"new").unwrap();
        assert_eq!(p.read_user("save/slot.dat").unwrap(), b"new");
        assert!(p.rename("../resource/story", "stolen").is_err());
        assert!(p.rename("save/slot.dat", "../escape").is_err());
        p.rename("save/slot.dat", "save/renamed.dat").unwrap();
        assert_eq!(p.read_user("save/renamed.dat").unwrap(), b"new");
        p.delete("save/renamed.dat").unwrap();
        assert!(!p.user.join("save/renamed.dat").exists());
        assert_eq!(
            std::fs::read(p.resource.join("story")).unwrap(),
            b"resource"
        );
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn reserved_internal_names_cannot_replace_the_active_writer_lock() {
        let (base, p) = roots("lock-reserved");
        let lock = p.lock_writer().unwrap();
        for name in [
            ".ethornell-session.lock",
            ".ETHORNELL-session.lock",
            "nested/.ethornell-save-1",
            "bgi-user/.ethornell-session.lock",
        ] {
            assert!(p.atomic_write(name, b"bad").is_err());
            assert!(p.delete(name).is_err());
            assert!(p.rename(name, "stolen").is_err());
            assert!(p.rename("slot", name).is_err());
        }
        assert!(p.lock_writer().is_err());
        assert!(p.enumerate("*", true, 0, false).unwrap().is_empty());
        drop(lock);
        assert!(p.lock_writer().is_ok());
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn install_userdata_alias_is_real_user_io_not_resource_write_or_original_save_import() {
        let (base,p)=roots("userdata-alias");
        std::fs::create_dir(p.resource.join("UserData")).unwrap();
        std::fs::write(p.resource.join("UserData/original.sud"),b"original").unwrap();
        assert!(p.read_user("bgi-resource\\UserData/original.sud").is_err());
        assert!(p.user_metadata("bgi-resource\\UserData/original.sud").is_none());
        assert_eq!(p.read_loose("bgi-resource\\UserData/original.sud").unwrap(),b"original");
        p.probe_writable("bgi-resource\\userdata").unwrap();
        assert!(p.user_metadata("bgi-resource\\USERDATA").unwrap().is_dir());
        assert!(!p.user.join("UserData/original.sud").exists());
        p.atomic_write("bgi-resource\\UserData/new.sud",b"new").unwrap();
        assert_eq!(p.read_user("bgi-user\\userdata/NEW.sud").unwrap(),b"new");
        assert_eq!(p.read_user("bgi-resource\\USERDATA/new.sud").unwrap(),b"new");
        assert!(p.read_loose("bgi-resource\\userdata/new.sud").is_err());
        assert_eq!(p.user_metadata("bgi-resource\\userdata/new.sud").unwrap().len(),3);
        p.atomic_write("bgi-resource\\UserData/original.sud",b"private replacement").unwrap();
        assert_eq!(p.read_loose("bgi-resource\\UserData/original.sud").unwrap(),b"original");
        assert_eq!(p.read_user("bgi-resource\\UserData/original.sud").unwrap(),b"private replacement");
        p.create_directory("bgi-resource\\userdata/Nested").unwrap();
        assert!(p.metadata("bgi-user\\UserData/nested").unwrap().is_dir());
        assert_eq!(p.enumerate("bgi-resource\\UserData/*",false,0,false).unwrap(),vec!["original.sud"]);
        assert_eq!(p.enumerate_user("bgi-resource\\UserData/*",false,0,false).unwrap(),vec!["new.sud","original.sud"]);
        for denied in ["bgi-resource/story.arc","bgi-resource/UserData/../../story.arc","bgi-resource/UserData/.ethornell-session.lock","bgi-resource/UserDataX/slot"] {assert!(p.atomic_write(denied,b"bad").is_err());}
        assert!(!p.resource.join("UserData/new.sud").exists());
        assert_eq!(std::fs::read(p.resource.join("UserData/original.sud")).unwrap(),b"original");
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn ambiguous_ascii_case_alias_does_not_select_arbitrary_directory_entry() {
        let (base, p) = roots("case-ambiguity");
        std::fs::write(p.user.join("Save"), b"first").unwrap();
        std::fs::write(p.user.join("SAVE"), b"second").unwrap();
        // Case-insensitive host filesystems cannot create both names; exercise
        // ambiguity on case-sensitive hosts, and exact-match behavior everywhere.
        let count = std::fs::read_dir(&p.user).unwrap().count();
        if count == 2 {
            assert!(p.read_user("save").is_err());
            assert!(p.atomic_write("save", b"ambiguous").is_err());
            assert_eq!(p.read_user("Save").unwrap(), b"first");
            assert_eq!(p.read_user("SAVE").unwrap(), b"second");
        } else {
            assert!(p.read_user("Save").is_ok());
        }
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn overlay_case_virtual_roots_and_native_file_calls_are_consistent() {
        use ethornell_vm::{NativeCallFrame, NativeOpcode, SysApi, Value};
        let (base, p) = roots("full-chain");
        std::fs::create_dir(p.resource.join("UserData")).unwrap();
        std::fs::write(p.resource.join("UserData/Slot.sud"), b"resource").unwrap();
        let manager = p.open_manager().unwrap();
        assert_eq!(
            manager.limits(),
            ethornell_archive::ResourceBudget::android()
        );
        let mut api = super::super::RuntimeTraceApi::new_with_native_root(
            manager.clone(),
            p.resource.clone(),
        );
        api.host.enabled = true;
        scope(&p, || {
            assert_eq!(
                api.load_file_bytes("", "userdata/slot.SUD").unwrap(),
                b"resource"
            );
            assert_eq!(api.file_size("", "userdata/slot.SUD"), 8);
            let mut call = NativeCallFrame::new(
                NativeOpcode {
                    group: 0x80,
                    id: 0x28,
                },
                vec![Value::Str("UserData".into())],
            );
            assert_eq!(api.call_sys(&mut call).unwrap(), Value::Int(1));
            assert!(api.probe_directory_writable("bgi-user\\UserData"));
            assert!(api.write_file_bytes("bgi-user\\userdata/slot.SUD", b"user overlay"));
            for archive in ["", "bgi-user\\"] {
                assert!(api.file_exists(archive, "UserData/Slot.sud"));
                assert_eq!(api.file_size(archive, "UserData/Slot.sud"), 12);
                assert_eq!(
                    api.load_file_bytes(archive, "UserData/Slot.sud").unwrap(),
                    b"user overlay"
                );
            }
            assert_eq!(
                api.load_file_bytes("bgi-resource\\", "UserData/Slot.sud")
                    .unwrap(),
                b"resource"
            );
            assert_eq!(api.file_size("bgi-resource\\", "UserData/Slot.sud"), 8);
            assert!(api.load_file_bytes("bgi-user\\", "missing").is_none());
            assert!(
                api.load_file_bytes("/outside", "UserData/Slot.sud")
                    .is_none()
            );
            assert_eq!(
                api.enumerate_user_files("userdata/*", true, 0),
                vec!["slot.SUD"]
            );
            p.rename("UserData/Slot.sud", "UserData/Renamed.sud")
                .unwrap();
            assert_eq!(
                api.load_file_bytes("", "userdata/renamed.SUD").unwrap(),
                b"user overlay"
            );
            assert!(api.delete_file("", "UserData/Renamed.sud"));
            assert!(!api.file_exists("bgi-user\\", "UserData/Renamed.sud"));
            for bad in ["../outside", "/outside", "C:\\outside", "\\\\server\\file"] {
                assert!(api.load_file_bytes("", bad).is_none());
                assert!(!api.write_file_bytes(bad, b"bad"));
                assert!(
                    api.enumerate_user_files(&format!("{bad}/*"), true, 0)
                        .is_empty()
                );
            }
            assert_eq!(api.native_system.bootstrap_request, None);
            api.select_bootstrap(Some("../outside"), "system.arc", "ipl._bp");
            assert_eq!(api.native_system.bootstrap_request, None);
            assert!(super::super::host_dialog::set_wallpaper("/outside", 0, 0) == false);
        });
        assert!(p.read_user("UserData/slot.sud").is_err());
        p.atomic_write("UserData/reentry.sud", b"user overlay")
            .unwrap();
        let reopened = FilePolicy::new(&p.resource, &p.user).unwrap();
        assert_eq!(
            reopened.read_user("userdata/REENTRY.sud").unwrap(),
            b"user overlay"
        );
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn root_descriptors_survive_path_replacement_without_following_new_symlink() {
        use std::os::unix::fs::symlink;
        let (base, p) = roots("root-swap");
        std::fs::write(p.resource.join("story"), b"authorized").unwrap();
        std::fs::rename(&p.resource, base.join("old-resource")).unwrap();
        std::fs::create_dir(base.join("outside")).unwrap();
        std::fs::write(base.join("outside/story"), b"outside").unwrap();
        symlink(base.join("outside"), &p.resource).unwrap();
        assert_eq!(p.read_loose("story").unwrap(), b"authorized");
        std::fs::rename(&p.user, base.join("old-user")).unwrap();
        symlink(base.join("outside"), &p.user).unwrap();
        p.atomic_write("slot", b"contained").unwrap();
        assert_eq!(
            std::fs::read(base.join("old-user/slot")).unwrap(),
            b"contained"
        );
        assert!(!base.join("outside/slot").exists());
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn host_authorized_root_alias_binds_the_same_canonical_directory_inode() {
        use std::os::unix::fs::{symlink,MetadataExt};
        let (base,p)=roots("canonical-alias");
        symlink(&p.resource,base.join("resource-alias")).unwrap();
        symlink(&p.user,base.join("user-alias")).unwrap();
        let alias=FilePolicy::new(&base.join("resource-alias"),&base.join("user-alias")).unwrap();
        assert_eq!(alias.resource_dir.metadata().unwrap().ino(),p.resource_dir.metadata().unwrap().ino());
        assert_eq!(alias.user_dir.metadata().unwrap().ino(),p.user_dir.metadata().unwrap().ino());
        assert_eq!(alias.resource,p.resource);assert_eq!(alias.user,p.user);
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    #[cfg(any(target_os="linux",target_os="android"))]
    fn execute_only_ancestor_does_not_require_directory_listing_permission() {
        use std::os::unix::fs::PermissionsExt;
        let (base,p)=roots("execute-only");
        let nested=base.join("traverse");
        std::fs::create_dir_all(nested.join("resource")).unwrap();
        std::fs::create_dir_all(nested.join("user")).unwrap();
        std::fs::set_permissions(&nested,std::fs::Permissions::from_mode(0o111)).unwrap();
        let bound=FilePolicy::new(&nested.join("resource"),&nested.join("user"));
        std::fs::set_permissions(&nested,std::fs::Permissions::from_mode(0o700)).unwrap();
        let bound=bound.unwrap();bound.atomic_write("slot",b"safe").unwrap();
        assert_eq!(bound.read_user("slot").unwrap(),b"safe");
        drop(p);drop(bound);std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn archive_family_and_explicit_root_names_use_only_authorized_descriptors() {
        use ethornell_vm::SysApi;
        let (base, p) = roots("archive-family");
        let payload = b"script";
        let mut arc = b"PackFile    ".to_vec();
        arc.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 16];
        name[..4].copy_from_slice(b"main");
        arc.extend_from_slice(&name);
        arc.extend_from_slice(&0u32.to_le_bytes());
        arc.extend_from_slice(&(payload.len() as u32).to_le_bytes());
        arc.extend_from_slice(&[0; 8]);
        arc.extend_from_slice(payload);
        std::fs::write(p.resource.join("Data02000.arc"), &arc).unwrap();
        let manager = p.open_manager().unwrap();
        let mut api =
            super::super::RuntimeTraceApi::new_with_native_root(manager, p.resource.clone());
        api.host.enabled = true;
        scope(&p, || {
            p.atomic_write("main", b"loose collision").unwrap();
            assert!(api.file_exists("", "data02xxx.arc"));
            assert_eq!(api.file_size("", "data02xxx.arc"), arc.len() as i32);
            assert_eq!(api.load_file_bytes("", "data02xxx.arc").unwrap(), arc);
            assert_eq!(
                api.load_file_bytes("data02xxx.arc", "main").unwrap(),
                payload
            );
            assert_eq!(api.load_file_bytes("data02xxx.arc", "").unwrap(), payload);
            assert!(
                api.load_file_bytes("bgi-user\\", "bgi-resource\\Data02000.arc")
                    .is_none()
            );
            assert!(!api.file_exists("bgi-user\\", "bgi-resource\\Data02000.arc"));
            assert_eq!(
                api.file_size("bgi-user\\", "bgi-resource\\Data02000.arc"),
                -1
            );
            assert!(
                api.load_file_bytes("bgi-resource\\", "bgi-user\\main")
                    .is_none()
            );
            assert!(!api.file_exists("bgi-user\\", "data02xxx.arc"));
            assert!(api.load_file_bytes("bgi-user\\", "data02xxx.arc").is_none());
            assert_eq!(api.file_size("bgi-user\\", "data02xxx.arc"), -1);
            assert_eq!(
                api.load_file_bytes("bgi-resource\\", "data02000.ARC")
                    .unwrap(),
                arc
            );
            assert!(!api.file_exists("bgi-resource\\", "main"));
            assert!(api.load_file_bytes("bgi-resource\\", "main").is_none());
            assert!(api.load_file_bytes("../Data02000.arc", "main").is_none());
        });
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    #[cfg(fd_policy)]
    fn nofollow_blocks_symlink_parents_and_leaves_old_save() {
        use std::os::unix::fs::symlink;
        let (base, p) = roots("link");
        std::fs::create_dir(base.join("outside")).unwrap();
        symlink(base.join("outside"), p.user.join("link")).unwrap();
        assert!(p.atomic_write("link/escape", b"bad").is_err());
        assert!(!base.join("outside/escape").exists());
        p.atomic_write("slot", b"old").unwrap();
        assert!(p.atomic_write("slot/invalid", b"bad").is_err());
        assert_eq!(p.read_user("slot").unwrap(), b"old");
        symlink(base.join("outside"), p.resource.join("link")).unwrap();
        assert!(p.validate_resource_tree().is_err());
        std::fs::remove_dir_all(base).unwrap();
    }
    #[test]
    fn scoped_policy_is_restored_on_unwind() {
        let (base, p) = roots("scope");
        assert!(!restricted());
        let _ = std::panic::catch_unwind(|| {
            scope(&p, || {
                assert!(restricted());
                panic!("injected")
            })
        });
        assert!(!restricted());
        std::fs::remove_dir_all(base).unwrap();
    }
}
