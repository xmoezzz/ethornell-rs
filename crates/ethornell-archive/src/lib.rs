use byteorder::{LittleEndian, ReadBytesExt};
use encoding_rs::SHIFT_JIS;
use ethornell_core::{EthornellError, GameRoot, Result};
use serde::Serialize;
use std::collections::HashMap;
use std::fs;
use std::io::{Read, Seek, SeekFrom, Write};
use std::path::{Component, Path, PathBuf};
use std::sync::{Arc, Mutex, RwLock};

pub const MAGIC_PACK_FILE: &[u8] = b"PackFile";
pub const MAGIC_PACK_FILE_FULL: &[u8] = b"PackFile    ";
pub const MAGIC_BURIKO_ARC20: &[u8] = b"BURIKO ARC20";
pub const MAGIC_COMPRESSED_BG: &[u8] = b"CompressedBG___";
pub const MAGIC_DSC_FORMAT: &[u8] = b"DSC FORMAT 1.00";
pub const MAGIC_COMPILED_SCRIPT: &[u8] = b"BurikoCompiledScriptVer1.00";

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
pub enum ArchiveFormat {
    PackFile,
    BurikoArc20,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, Serialize)]
pub enum MagicKind {
    PackFile,
    BurikoArc20,
    CompressedBg,
    DscFormat,
    BurikoCompiledScript,
    Bse,
    Ogg,
    RiffWave,
    Png,
    Jpeg,
    Bp,
    Unknown,
}

impl MagicKind {
    pub fn label(self) -> &'static str {
        match self {
            Self::PackFile => "PackFile",
            Self::BurikoArc20 => "BURIKO ARC20",
            Self::CompressedBg => "CompressedBG___",
            Self::DscFormat => "DSC FORMAT 1.00",
            Self::BurikoCompiledScript => "BurikoCompiledScriptVer1.00",
            Self::Bse => "BSE",
            Self::Ogg => "OggS",
            Self::RiffWave => "RIFF/WAVE",
            Self::Png => "PNG",
            Self::Jpeg => "JPEG",
            Self::Bp => "._bp",
            Self::Unknown => "Unknown",
        }
    }
}

#[derive(Debug, Clone, Serialize)]
pub struct ArchiveEntry {
    pub name: String,
    pub offset: u64,
    pub packed_size: u64,
    pub unpacked_size: Option<u64>,
    pub flags: u32,
    pub method: Option<String>,
}

pub trait ArchiveReader {
    fn entries(&self) -> Result<Vec<ArchiveEntry>>;
    fn read_entry(&mut self, name: &str) -> Result<Vec<u8>>;
}

#[derive(Debug, Clone, Serialize)]
pub struct ArchiveIndex {
    pub path: PathBuf,
    pub format: ArchiveFormat,
    pub entries: Vec<ArchiveEntry>,
}

#[derive(Debug, Clone, Serialize)]
pub struct GameArchiveSet {
    pub root: PathBuf,
    pub archives: Vec<ArchiveIndex>,
}

#[derive(Debug, Clone, Serialize)]
pub struct ResourceEntry {
    pub archive_path: PathBuf,
    pub entry_name: String,
    pub packed_size: u64,
    pub unpacked_size: Option<u64>,
    pub flags: u32,
}

/// Per-resource allocation limits and cumulative index limits for one manager.
/// Index bytes conservatively charge both on-disk records and resident entries,
/// decoded/normalized names and lookup bookkeeping; this is not a process-wide
/// memory limit (callers must separately budget caches and concurrent reads).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ResourceBudget {
    pub max_packed_bytes: u64,
    pub max_decoded_bytes: u64,
    pub max_total_index_entries: u64,
    pub max_total_index_bytes: u64,
    pub max_archives: usize,
}

impl ResourceBudget {
    pub const fn android() -> Self {
        Self {
            max_packed_bytes: 256 * 1024 * 1024,
            max_decoded_bytes: 256 * 1024 * 1024,
            max_total_index_entries: 262_144,
            max_total_index_bytes: 64 * 1024 * 1024,
            max_archives: 1024,
        }
    }
}

impl Default for ResourceBudget {
    fn default() -> Self {
        Self {
            // Archive sizes are u32. Retain desktop compatibility while bounding
            // stand-alone helpers that accept caller-constructed u64 metadata.
            max_packed_bytes: u32::MAX as u64,
            max_decoded_bytes: u32::MAX as u64,
            max_total_index_entries: 4 * 1024 * 1024,
            max_total_index_bytes: 1024 * 1024 * 1024,
            max_archives: 4096,
        }
    }
}

/// Alternate name for callers configuring read limits.
pub type ReadLimits = ResourceBudget;

#[derive(Default)]
struct IndexUsage {
    entries: u64,
    bytes: u64,
    archives: usize,
}

fn check_limit(value: u64, limit: u64, kind: &str) -> Result<()> {
    if value > limit {
        return Err(EthornellError::Parse(format!(
            "resource budget exceeded ({kind}): {value} > {limit}"
        )));
    }
    Ok(())
}

fn checked_buffer_size(size: u64, limit: u64, kind: &str) -> Result<usize> {
    check_limit(size, limit, kind)?;
    let size = usize::try_from(size)
        .map_err(|_| EthornellError::Parse(format!("{kind} does not fit usize")))?;
    if size > isize::MAX as usize {
        return Err(EthornellError::Parse(format!(
            "{kind} exceeds allocation range"
        )));
    }
    Ok(size)
}

#[cfg(test)]
thread_local! {
    // Per-test-thread hook proves rejection precedes payload/index reservation,
    // without attempting huge allocations or relying on allocator/OOM behavior.
    static ALLOCATION_ATTEMPTS: std::cell::Cell<usize> = const { std::cell::Cell::new(0) };
}

fn zeroed_buffer(size: usize) -> Result<Vec<u8>> {
    #[cfg(test)]
    ALLOCATION_ATTEMPTS.with(|count| count.set(count.get() + 1));
    let mut data = Vec::new();
    data.try_reserve_exact(size)
        .map_err(|err| EthornellError::Parse(format!("resource allocation failed: {err}")))?;
    data.resize(size, 0);
    Ok(data)
}

/// An open archive: seekable bytes of a known length.
pub trait ArchiveSource: Read + Seek + Send + std::fmt::Debug {
    fn byte_len(&mut self) -> std::io::Result<u64>;
}

impl ArchiveSource for fs::File {
    fn byte_len(&mut self) -> std::io::Result<u64> {
        Ok(self.metadata()?.len())
    }
}

#[derive(Debug)]
struct ResourceManagerInner {
    limits: ResourceBudget,
    set: GameArchiveSet,
    /// Normalized resource name -> stable locators into `set.archives[*].entries[*]`.
    /// The archive index remains part of the locator so duplicate filenames in
    /// different archives preserve the original archive ordering semantics.
    by_name: HashMap<String, Vec<(usize, usize)>>,
    /// Lower-cased archive filename -> first archive with that filename.
    archive_by_name: HashMap<String, usize>,
    /// Full archive path -> archive index. This avoids rescanning every archive
    /// when callers hand a ResourceEntry back to us.
    archive_by_path: HashMap<PathBuf, usize>,
    /// Runtime archive handles. Resource reads are overwhelmingly random seeks
    /// into a small, stable set of ARC files; reopening the archive for every
    /// BP/image/audio entry is substantially more expensive than the seek/read.
    archive_files: Vec<Mutex<Box<dyn ArchiveSource>>>,
    /// Search state the scripts configure at run time (Sys80:36/37/38).
    search: RwLock<RuntimeSearch>,
}

#[derive(Debug, Default)]
struct RuntimeSearch {
    /// DCArchiveComplex records: lower-cased name -> member archives in the
    /// order the script registered them. sub_445F50 and its siblings try the
    /// members in that order and stop at the first hit.
    composites: Vec<(String, Vec<String>)>,
    /// dword_506BE0: whether loose-file reads fall back to `loose_paths`.
    loose_fallback_enabled: bool,
    /// dword_566648: Sys80:37 prepends, so the newest directory comes first.
    loose_paths: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct ResourceManager {
    inner: Arc<ResourceManagerInner>,
}

impl ResourceManager {
    /// Host-policy entry point. The host opens and authorizes every descriptor;
    /// archives are parsed and retained from those exact descriptors, never
    /// reopened through game-controlled paths after a containment check.
    pub fn from_authorized_files(root: PathBuf, files: Vec<(PathBuf, fs::File)>) -> Result<Self> {
        Self::from_authorized_files_with_limits(root, files, ResourceBudget::default())
    }

    /// Like `from_authorized_files`, with limits applied before index/data
    /// allocation. Descriptor cursors are reset; no archive path is reopened.
    pub fn from_authorized_files_with_limits(
        root: PathBuf,
        files: Vec<(PathBuf, fs::File)>,
        limits: ResourceBudget,
    ) -> Result<Self> {
        let sources = files
            .into_iter()
            .map(|(path, file)| (path, Box::new(file) as Box<dyn ArchiveSource>))
            .collect();
        Self::from_authorized_sources_with_limits(root, sources, limits)
    }

    /// Like `from_authorized_files_with_limits`, for archives that are not
    /// host files (the web build reads the picked folder through JavaScript).
    pub fn from_authorized_sources_with_limits(
        root: PathBuf,
        files: Vec<(PathBuf, Box<dyn ArchiveSource>)>,
        limits: ResourceBudget,
    ) -> Result<Self> {
        check_limit(
            files.len() as u64,
            limits.max_archives as u64,
            "archive count",
        )?;
        let mut archives = Vec::new();
        let mut archive_files = Vec::new();
        let mut usage = IndexUsage::default();
        for (path, mut file) in files {
            file.seek(SeekFrom::Start(0))?;
            let mut magic = [0u8; 12];
            if file.byte_len()? < magic.len() as u64 {
                continue;
            }
            file.read_exact(&mut magic)?;
            let format = detect_archive_format_from_bytes(&magic);
            if format == ArchiveFormat::Unknown {
                continue;
            }
            let index = read_index_from_file(&path, file.as_mut(), format, &limits, &mut usage)?;
            archives.push(index);
            archive_files.push(Mutex::new(file));
        }
        let set = GameArchiveSet { root, archives };
        let (by_name, archive_by_name, archive_by_path) = build_resource_lookup(&set);
        Ok(Self {
            inner: Arc::new(ResourceManagerInner {
                limits,
                set,
                by_name,
                archive_by_name,
                archive_by_path,
                archive_files,
                search: RwLock::default(),
            }),
        })
    }

    pub fn open_game(root: impl AsRef<Path>) -> Result<Self> {
        Self::open_game_with_limits(root, ResourceBudget::default())
    }

    pub fn open_game_with_limits(root: impl AsRef<Path>, limits: ResourceBudget) -> Result<Self> {
        let game = GameRoot::new(root.as_ref().to_path_buf())?;
        let mut files = Vec::new();
        for path in archive_paths(&game)? {
            check_limit(
                files.len() as u64 + 1,
                limits.max_archives as u64,
                "archive count",
            )?;
            let file = fs::File::open(&path)?;
            files.push((path, file));
        }
        Self::from_authorized_files_with_limits(game.path().to_path_buf(), files, limits)
    }

    pub fn limits(&self) -> ResourceBudget {
        self.inner.limits
    }

    pub fn archives(&self) -> &GameArchiveSet {
        &self.inner.set
    }

    pub fn resource_count(&self) -> usize {
        self.inner
            .set
            .archives
            .iter()
            .map(|archive| archive.entries.len())
            .sum()
    }

    pub fn list(&self) -> Vec<ResourceEntry> {
        let mut entries = Vec::new();
        for archive in &self.inner.set.archives {
            for entry in &archive.entries {
                entries.push(resource_entry_from_archive(archive, entry));
            }
        }
        entries
    }

    pub fn find(&self, name: &str) -> Option<ResourceEntry> {
        self.find_locator(name)
            .and_then(|(archive, entry)| self.entry_at(archive, entry))
    }

    pub fn find_all(&self, query: &str) -> Vec<ResourceEntry> {
        let key = normalize_resource_name(query);
        let mut matching_keys = self
            .inner
            .by_name
            .iter()
            .filter(|(candidate, _)| candidate.contains(&key))
            .collect::<Vec<_>>();
        // The old BTreeMap implementation exposed lexicographic normalized-key
        // order. Preserve that behavior without re-normalizing archive entries.
        matching_keys.sort_unstable_by_key(|(left, _)| *left);
        let mut out = Vec::new();
        for (_, matches) in matching_keys {
            for &(archive, entry) in matches {
                if let Some(entry) = self.entry_at(archive, entry) {
                    out.push(entry);
                }
            }
        }
        out
    }

    pub fn find_in_archive(&self, archive_name: &str, entry_name: &str) -> Option<ResourceEntry> {
        self.find_locator_in_archive(archive_name, entry_name)
            .and_then(|(archive, entry)| self.entry_at(archive, entry))
    }

    pub fn first_in_archive(&self, archive_name: &str) -> Option<ResourceEntry> {
        let first = |archive: &str| {
            let archive_index = *self.inner.archive_by_name.get(&archive_name_key(archive))?;
            self.entry_at(archive_index, 0)
        };
        match self.composite_members(archive_name) {
            Some(members) => members.iter().find_map(|member| first(member)),
            None => first(archive_name),
        }
    }

    /// Sys80:38 (sub_466DE0): register a composite archive name over member
    /// archives. Returns false when the name is already registered, like
    /// sub_406BC0, or when there are no members (sub_445C50).
    pub fn register_composite_archive(&self, name: &str, members: Vec<String>) -> bool {
        if name.is_empty() || members.is_empty() {
            return false;
        }
        let key = archive_name_key(name);
        let mut search = self.inner.search.write().expect("resource search poisoned");
        if search
            .composites
            .iter()
            .any(|(existing, _)| *existing == key)
        {
            return false;
        }
        search.composites.push((key, members));
        true
    }

    /// The registered members of a composite archive, if `archive_name` is one.
    pub fn composite_members(&self, archive_name: &str) -> Option<Vec<String>> {
        let key = archive_name_key(archive_name);
        let search = self.inner.search.read().expect("resource search poisoned");
        search
            .composites
            .iter()
            .find(|(name, _)| *name == key)
            .map(|(_, members)| members.clone())
    }

    /// Sys80:36 (sub_4657A0).
    pub fn set_loose_search_fallback(&self, enabled: bool) {
        self.inner
            .search
            .write()
            .expect("resource search poisoned")
            .loose_fallback_enabled = enabled;
    }

    /// Sys80:37 (sub_4657F0) prepends a directory to the loose-file fallback.
    pub fn prepend_loose_search_path(&self, directory: &str) {
        self.inner
            .search
            .write()
            .expect("resource search poisoned")
            .loose_paths
            .insert(0, directory.to_owned());
    }

    /// Directories a loose-file read tries after the root (sub_465E30), newest
    /// first; empty while the fallback is disabled.
    pub fn loose_search_paths(&self) -> Vec<String> {
        let search = self.inner.search.read().expect("resource search poisoned");
        if search.loose_fallback_enabled {
            search.loose_paths.clone()
        } else {
            Vec::new()
        }
    }

    pub fn read_decoded_from_archive(
        &self,
        archive_name: &str,
        entry_name: &str,
    ) -> Result<Vec<u8>> {
        let (archive_index, entry_index) = self
            .find_locator_in_archive(archive_name, entry_name)
            .ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource not found in archive {archive_name}: {entry_name}"
            ))
        })?;
        self.read_locator_decoded(archive_index, entry_index)
    }

    pub fn read_raw(&self, name: &str) -> Result<Vec<u8>> {
        let (archive_index, entry_index) = self.find_locator(name).ok_or_else(|| {
            EthornellError::Parse(format!("resource not found in game archives: {name}"))
        })?;
        self.read_locator_raw(archive_index, entry_index)
    }

    pub fn read_decoded(&self, name: &str) -> Result<Vec<u8>> {
        let (archive_index, entry_index) = self.find_locator(name).ok_or_else(|| {
            EthornellError::Parse(format!("resource not found in game archives: {name}"))
        })?;
        self.read_locator_decoded(archive_index, entry_index)
    }

    pub fn read_by_entry_raw(&self, entry: &ResourceEntry) -> Result<Vec<u8>> {
        self.read_by_entry_bytes(entry, false)
    }

    fn read_by_entry_bytes(&self, entry: &ResourceEntry, decoded: bool) -> Result<Vec<u8>> {
        let archive_index = self.archive_index_for_path(&entry.archive_path)?;
        let key = normalize_resource_name(&entry.entry_name);
        let candidates = self.inner.by_name.get(&key).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry not found: {}:{}",
                entry.archive_path.display(),
                entry.entry_name
            ))
        })?;

        let entry_index = candidates
            .iter()
            .filter(|&&(candidate_archive, _)| candidate_archive == archive_index)
            .find_map(|&(_, candidate_entry)| {
                let candidate = self.inner.set.archives[archive_index]
                    .entries
                    .get(candidate_entry)?;
                (candidate.name == entry.entry_name && candidate.packed_size == entry.packed_size)
                    .then_some(candidate_entry)
            })
            .or_else(|| {
                // Preserve the previous case-/normalization-insensitive behavior
                // for ResourceEntry values constructed outside this manager.
                candidates
                    .iter()
                    .filter(|&&(candidate_archive, _)| candidate_archive == archive_index)
                    .find_map(|&(_, candidate_entry)| {
                        let candidate = self.inner.set.archives[archive_index]
                            .entries
                            .get(candidate_entry)?;
                        (candidate.packed_size == entry.packed_size).then_some(candidate_entry)
                    })
            })
            .ok_or_else(|| {
                EthornellError::Parse(format!(
                    "resource entry not found: {}:{}",
                    entry.archive_path.display(),
                    entry.entry_name
                ))
            })?;

        self.read_locator_bytes(archive_index, entry_index, decoded)
    }

    pub fn read_by_entry_decoded(&self, entry: &ResourceEntry) -> Result<Vec<u8>> {
        let raw = self.read_by_entry_bytes(entry, true)?;
        decode_payload_owned(&entry.entry_name, raw, &self.inner.limits)
    }

    /// Read a decoded entry into a caller-owned buffer.  Console movie playback
    /// keeps this buffer reserved while the heap is still contiguous, avoiding
    /// a multi-megabyte allocation in the middle of a scene transition.
    pub fn read_by_entry_decoded_into(
        &self,
        entry: &ResourceEntry,
        out: &mut Vec<u8>,
    ) -> Result<()> {
        let archive_index = self.archive_index_for_path(&entry.archive_path)?;
        let key = normalize_resource_name(&entry.entry_name);
        let candidates = self.inner.by_name.get(&key).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry not found: {}:{}",
                entry.archive_path.display(),
                entry.entry_name
            ))
        })?;
        let entry_index = candidates
            .iter()
            .filter(|&&(candidate_archive, _)| candidate_archive == archive_index)
            .find_map(|&(_, candidate_entry)| {
                let candidate = self.inner.set.archives[archive_index]
                    .entries
                    .get(candidate_entry)?;
                (candidate.name == entry.entry_name && candidate.packed_size == entry.packed_size)
                    .then_some(candidate_entry)
            })
            .ok_or_else(|| {
                EthornellError::Parse(format!(
                    "resource entry not found: {}:{}",
                    entry.archive_path.display(),
                    entry.entry_name
                ))
            })?;
        self.read_locator_decoded_into(archive_index, entry_index, out)
    }

    /// Read an ordinary decoded entry directly into caller-owned storage.
    ///
    /// This is used by the Vita movie path, whose reusable MPEG buffer lives
    /// in CDRAM rather than the application's constrained user heap.  Wrapped
    /// formats still need an owned temporary decode buffer and are rejected.
    pub fn read_by_entry_decoded_to_slice(
        &self,
        entry: &ResourceEntry,
        out: &mut [u8],
    ) -> Result<usize> {
        let archive_index = self.archive_index_for_path(&entry.archive_path)?;
        let key = normalize_resource_name(&entry.entry_name);
        let candidates = self.inner.by_name.get(&key).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry not found: {}:{}",
                entry.archive_path.display(),
                entry.entry_name
            ))
        })?;
        let entry_index = candidates
            .iter()
            .filter(|&&(candidate_archive, _)| candidate_archive == archive_index)
            .find_map(|&(_, candidate_entry)| {
                let candidate = self.inner.set.archives[archive_index]
                    .entries
                    .get(candidate_entry)?;
                (candidate.name == entry.entry_name && candidate.packed_size == entry.packed_size)
                    .then_some(candidate_entry)
            })
            .ok_or_else(|| {
                EthornellError::Parse(format!(
                    "resource entry not found: {}:{}",
                    entry.archive_path.display(),
                    entry.entry_name
                ))
            })?;
        self.read_locator_decoded_to_slice(archive_index, entry_index, out)
    }
    /// Size of the same decoded payload returned by read_by_entry_decoded.
    /// Read at most a stack header from the retained authorized archive FD;
    /// never reopen a checked path or allocate/decompress the whole resource.
    pub fn decoded_size_by_entry(&self, entry: &ResourceEntry) -> Result<u64> {
        let index = self.archive_index_for_path(&entry.archive_path)?;
        let archive = &self.inner.set.archives[index];
        let actual = archive
            .entries
            .iter()
            .find(|e| e.name == entry.entry_name && e.packed_size == entry.packed_size)
            .ok_or_else(|| EthornellError::Parse("entry not indexed".into()))?;
        checked_buffer_size(
            actual.packed_size,
            self.inner.limits.max_packed_bytes,
            "packed bytes",
        )?;
        let mut file = self.inner.archive_files[index]
            .lock()
            .map_err(|_| EthornellError::Other("archive handle poisoned".into()))?;
        if actual
            .offset
            .checked_add(actual.packed_size)
            .is_none_or(|end| end > file.byte_len().unwrap_or(0))
        {
            return Err(EthornellError::Parse("entry range invalid".into()));
        }
        file.seek(SeekFrom::Start(actual.offset))?;
        let mut header = [0u8; 32];
        let count = actual.packed_size.min(32) as usize;
        file.read_exact(&mut header[..count])?;
        Ok(check_decoded_size(&header[..count], actual.packed_size, &self.inner.limits)? as u64)
    }

    fn find_locator(&self, name: &str) -> Option<(usize, usize)> {
        let key = normalize_resource_name(name);
        if let Some(matches) = self.inner.by_name.get(&key) {
            return matches.first().copied();
        }

        // Compatibility fallback for the historical fuzzy lookup. Scan the
        // already-normalized hash keys instead of normalizing every archive
        // entry again on each miss. This keeps the slow path allocation-free.
        self.inner
            .by_name
            .iter()
            .filter(|(candidate, _)| candidate.ends_with(&key) || candidate.contains(&key))
            .min_by(|(left, _), (right, _)| left.cmp(right))
            .and_then(|(_, matches)| matches.first().copied())
    }

    fn find_locator_in_archive(
        &self,
        archive_name: &str,
        entry_name: &str,
    ) -> Option<(usize, usize)> {
        let entry_key = normalize_resource_name(entry_name);
        let locators = self.inner.by_name.get(&entry_key)?;
        let find = |archive: &str| {
            let archive_index = *self.inner.archive_by_name.get(&archive_name_key(archive))?;
            locators
                .iter()
                .copied()
                .find(|&(candidate_archive, _)| candidate_archive == archive_index)
        };
        match self.composite_members(archive_name) {
            Some(members) => members.iter().find_map(|member| find(member)),
            None => find(archive_name),
        }
    }

    fn read_locator_raw(&self, archive_index: usize, entry_index: usize) -> Result<Vec<u8>> {
        self.read_locator_bytes(archive_index, entry_index, false)
    }

    fn read_locator_bytes(
        &self,
        archive_index: usize,
        entry_index: usize,
        decoded: bool,
    ) -> Result<Vec<u8>> {
        let archive = self.inner.set.archives.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!("archive index out of range: {archive_index}"))
        })?;
        let entry = archive.entries.get(entry_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry index out of range: {archive_index}:{entry_index}"
            ))
        })?;
        let file = self.inner.archive_files.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "archive handle index out of range: {archive_index}"
            ))
        })?;
        let mut file = file.lock().map_err(|_| {
            EthornellError::Other(format!(
                "archive handle poisoned: {}",
                archive.path.display()
            ))
        })?;
        read_entry_from_file(&mut **file, entry, &self.inner.limits, decoded)
    }

    fn read_locator_decoded(&self, archive_index: usize, entry_index: usize) -> Result<Vec<u8>> {
        let archive = self.inner.set.archives.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!("archive index out of range: {archive_index}"))
        })?;
        let entry = archive.entries.get(entry_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry index out of range: {archive_index}:{entry_index}"
            ))
        })?;
        let name = entry.name.clone();
        let raw = self.read_locator_bytes(archive_index, entry_index, true)?;
        decode_payload_owned(&name, raw, &self.inner.limits)
    }

    fn read_locator_decoded_into(
        &self,
        archive_index: usize,
        entry_index: usize,
        out: &mut Vec<u8>,
    ) -> Result<()> {
        let archive = self.inner.set.archives.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!("archive index out of range: {archive_index}"))
        })?;
        let entry = archive.entries.get(entry_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry index out of range: {archive_index}:{entry_index}"
            ))
        })?;
        let file = self.inner.archive_files.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "archive handle index out of range: {archive_index}"
            ))
        })?;
        let mut file = file.lock().map_err(|_| {
            EthornellError::Other(format!(
                "archive handle poisoned: {}",
                archive.path.display()
            ))
        })?;
        read_entry_from_file_into(&mut **file, entry, &self.inner.limits, true, out)?;
        if out.starts_with(MAGIC_DSC_FORMAT) {
            let decoded = dsc_decode_with_limits(out, self.inner.limits).map_err(|err| {
                EthornellError::UnsupportedFormat(format!(
                    "DSC decode failed for {}: {err}",
                    entry.name
                ))
            })?;
            *out = decoded;
        } else if out.starts_with(b"BSE 1.") {
            return Err(EthornellError::UnsupportedFormat(format!(
                "BSE wrapped entry is not decoded yet: {}",
                entry.name
            )));
        }
        Ok(())
    }

    fn read_locator_decoded_to_slice(
        &self,
        archive_index: usize,
        entry_index: usize,
        out: &mut [u8],
    ) -> Result<usize> {
        let archive = self.inner.set.archives.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!("archive index out of range: {archive_index}"))
        })?;
        let entry = archive.entries.get(entry_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "resource entry index out of range: {archive_index}:{entry_index}"
            ))
        })?;
        let file = self.inner.archive_files.get(archive_index).ok_or_else(|| {
            EthornellError::Parse(format!(
                "archive handle index out of range: {archive_index}"
            ))
        })?;
        let mut file = file.lock().map_err(|_| {
            EthornellError::Other(format!(
                "archive handle poisoned: {}",
                archive.path.display()
            ))
        })?;
        read_entry_from_file_to_slice(&mut **file, entry, &self.inner.limits, out)
    }

    fn archive_index_for_path(&self, path: &Path) -> Result<usize> {
        self.inner
            .archive_by_path
            .get(path)
            .copied()
            .ok_or_else(|| {
                EthornellError::Parse(format!("archive not indexed: {}", path.display()))
            })
    }

    fn entry_at(&self, archive_index: usize, entry_index: usize) -> Option<ResourceEntry> {
        let archive = self.inner.set.archives.get(archive_index)?;
        let entry = archive.entries.get(entry_index)?;
        Some(resource_entry_from_archive(archive, entry))
    }
}

fn build_resource_lookup(
    set: &GameArchiveSet,
) -> (
    HashMap<String, Vec<(usize, usize)>>,
    HashMap<String, usize>,
    HashMap<PathBuf, usize>,
) {
    let mut by_name: HashMap<String, Vec<(usize, usize)>> = HashMap::new();
    let mut archive_by_name = HashMap::with_capacity(set.archives.len());
    let mut archive_by_path = HashMap::with_capacity(set.archives.len());

    for (archive_index, archive) in set.archives.iter().enumerate() {
        archive_by_path
            .entry(archive.path.clone())
            .or_insert(archive_index);
        if let Some(name) = archive.path.file_name().and_then(|name| name.to_str()) {
            archive_by_name
                .entry(archive_name_key(name))
                .or_insert(archive_index);
        }
        for (entry_index, entry) in archive.entries.iter().enumerate() {
            by_name
                .entry(normalize_resource_name(&entry.name))
                .or_default()
                .push((archive_index, entry_index));
        }
    }

    (by_name, archive_by_name, archive_by_path)
}

fn archive_name_key(name: &str) -> String {
    Path::new(name)
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or(name)
        .to_ascii_lowercase()
}

#[derive(Debug, Clone, Default, Serialize)]
pub struct ExtractSummary {
    pub archives: usize,
    pub entries: usize,
    pub written: usize,
    pub decoded: usize,
    pub raw_dumped: usize,
    pub skipped: usize,
    pub errors: Vec<String>,
}

#[derive(Debug, Clone, Serialize)]
pub struct FileProbe {
    pub path: PathBuf,
    pub size: u64,
    pub extension: Option<String>,
    pub magic: MagicKind,
    pub archive_format: ArchiveFormat,
}

#[derive(Debug, Clone, Default, Serialize)]
pub struct ScanReport {
    pub game_root: PathBuf,
    pub file_count: usize,
    pub arc_candidates: usize,
    pub bp_scripts: usize,
    pub scenario_candidates: usize,
    pub compressed_bg_candidates: usize,
    pub ogg_candidates: usize,
    pub unknown_count: usize,
    pub files: Vec<FileProbe>,
}

pub fn detect_magic(buf: &[u8]) -> MagicKind {
    if buf.starts_with(MAGIC_BURIKO_ARC20) {
        MagicKind::BurikoArc20
    } else if buf.starts_with(MAGIC_PACK_FILE) {
        MagicKind::PackFile
    } else if buf.starts_with(MAGIC_COMPRESSED_BG) {
        MagicKind::CompressedBg
    } else if buf.starts_with(MAGIC_DSC_FORMAT) {
        MagicKind::DscFormat
    } else if buf.starts_with(MAGIC_COMPILED_SCRIPT) {
        MagicKind::BurikoCompiledScript
    } else if buf.starts_with(b"BSE 1.") {
        MagicKind::Bse
    } else if buf.starts_with(b"OggS") {
        MagicKind::Ogg
    } else if buf.len() >= 12 && buf.starts_with(b"RIFF") && &buf[8..12] == b"WAVE" {
        MagicKind::RiffWave
    } else if buf.starts_with(b"\x89PNG\r\n\x1a\n") {
        MagicKind::Png
    } else if buf.starts_with(b"\xff\xd8\xff") {
        MagicKind::Jpeg
    } else {
        MagicKind::Unknown
    }
}

pub fn detect_file_magic(path: &Path, buf: &[u8]) -> MagicKind {
    if path
        .to_string_lossy()
        .to_ascii_lowercase()
        .ends_with("._bp")
    {
        MagicKind::Bp
    } else {
        detect_magic(buf)
    }
}

pub fn detect_archive_format_from_bytes(buf: &[u8]) -> ArchiveFormat {
    match detect_magic(buf) {
        MagicKind::PackFile => ArchiveFormat::PackFile,
        MagicKind::BurikoArc20 => ArchiveFormat::BurikoArc20,
        _ => ArchiveFormat::Unknown,
    }
}

pub fn detect_archive_format(path: &Path) -> Result<ArchiveFormat> {
    let mut file = fs::File::open(path)?;
    let mut header = [0u8; 12];
    let read = file.read(&mut header)?;
    Ok(detect_archive_format_from_bytes(&header[..read]))
}

pub fn read_archive_index(path: &Path) -> Result<ArchiveIndex> {
    read_archive_index_with_limits(path, ResourceBudget::default())
}

pub fn read_archive_index_with_limits(path: &Path, limits: ResourceBudget) -> Result<ArchiveIndex> {
    let mut file = fs::File::open(path)?;
    let mut magic = [0u8; 12];
    file.read_exact(&mut magic)?;
    let format = detect_archive_format_from_bytes(&magic);
    read_index_from_file(path, &mut file, format, &limits, &mut IndexUsage::default())
}

fn read_index_from_file<R: ArchiveSource + ?Sized>(
    path: &Path,
    file: &mut R,
    format: ArchiveFormat,
    limits: &ResourceBudget,
    usage: &mut IndexUsage,
) -> Result<ArchiveIndex> {
    let (record_size, name_size) = match format {
        ArchiveFormat::PackFile => (0x20u64, 0x10usize),
        ArchiveFormat::BurikoArc20 => (0x80u64, 0x60usize),
        ArchiveFormat::Unknown => {
            return Err(EthornellError::UnsupportedFormat(format!(
                "{} is not a known BGI archive",
                path.display()
            )));
        }
    };
    let count = file.read_u32::<LittleEndian>()?;
    validate_entry_count(count)?;
    let overflow = || EthornellError::Parse("archive index size overflows".into());
    let disk_bytes = u64::from(count)
        .checked_mul(record_size)
        .ok_or_else(overflow)?;
    let base_offset = 0x10u64.checked_add(disk_bytes).ok_or_else(overflow)?;
    let file_len = file.byte_len()?;
    // Reject bogus counts in a short file before reserving the entry vector.
    if base_offset > file_len {
        return Err(EthornellError::Parse(
            "archive index extends past archive end".into(),
        ));
    }
    let total_entries = usage
        .entries
        .checked_add(u64::from(count))
        .ok_or_else(overflow)?;
    check_limit(
        total_entries,
        limits.max_total_index_entries,
        "total index entries",
    )?;
    // Worst-case SHIFT_JIS replacement/UTF-8 expansion is 3x; retain both name
    // and normalized lookup key. Include lookup/allocator bookkeeping too.
    let per_entry = record_size
        .checked_add(std::mem::size_of::<ArchiveEntry>() as u64)
        .and_then(|n| n.checked_add(name_size as u64 * 6 + 256))
        .ok_or_else(overflow)?;
    let path_bytes = (path.as_os_str().len() as u64)
        .checked_mul(8)
        .and_then(|n| n.checked_add(1024))
        .ok_or_else(overflow)?;
    let index_bytes = u64::from(count)
        .checked_mul(per_entry)
        .and_then(|n| n.checked_add(path_bytes))
        .ok_or_else(overflow)?;
    let total_bytes = usage.bytes.checked_add(index_bytes).ok_or_else(overflow)?;
    check_limit(
        total_bytes,
        limits.max_total_index_bytes,
        "total index bytes",
    )?;
    let archives = usage.archives.checked_add(1).ok_or_else(overflow)?;
    check_limit(archives as u64, limits.max_archives as u64, "archive count")?;
    let mut entries = Vec::new();
    #[cfg(test)]
    ALLOCATION_ATTEMPTS.with(|count| count.set(count.get() + 1));
    entries
        .try_reserve_exact(count as usize)
        .map_err(|err| EthornellError::Parse(format!("archive index allocation failed: {err}")))?;
    for _ in 0..count {
        let name = read_name(file, name_size)?;
        let relative_offset = file.read_u32::<LittleEndian>()?;
        let size = file.read_u32::<LittleEndian>()?;
        let mut tail = [0u8; 24];
        file.read_exact(&mut tail[..record_size as usize - name_size - 8])?;
        entries.push(ArchiveEntry {
            name,
            offset: base_offset
                .checked_add(u64::from(relative_offset))
                .ok_or_else(overflow)?,
            packed_size: size as u64,
            unpacked_size: None,
            flags: 0,
            method: None,
        });
    }
    validate_entries(file_len, &entries)?;
    usage.entries = total_entries;
    usage.bytes = total_bytes;
    usage.archives = archives;
    Ok(ArchiveIndex {
        path: path.to_path_buf(),
        format,
        entries,
    })
}

fn read_name<R: ArchiveSource + ?Sized>(file: &mut R, len: usize) -> Result<String> {
    let mut bytes = vec![0u8; len];
    file.read_exact(&mut bytes)?;
    let end = bytes.iter().position(|&b| b == 0).unwrap_or(bytes.len());
    let (decoded, _, _) = SHIFT_JIS.decode(&bytes[..end]);
    Ok(decoded.trim_end_matches('\0').to_string())
}

fn validate_entry_count(count: u32) -> Result<()> {
    if count > 0x0f_ffff {
        return Err(EthornellError::Parse(format!(
            "unreasonable archive entry count: {count}"
        )));
    }
    Ok(())
}

fn validate_entries(len: u64, entries: &[ArchiveEntry]) -> Result<()> {
    for entry in entries {
        let end = entry
            .offset
            .checked_add(entry.packed_size)
            .ok_or_else(|| EthornellError::Parse(format!("entry {} overflows", entry.name)))?;
        if end > len {
            return Err(EthornellError::Parse(format!(
                "entry {} extends past archive end: {} > {}",
                entry.name, end, len
            )));
        }
    }
    Ok(())
}

pub fn extract_entry(path: &Path, entry: &ArchiveEntry) -> Result<Vec<u8>> {
    let limits = ResourceBudget::default();
    let raw = read_entry_from_file(&mut fs::File::open(path)?, entry, &limits, true)?;
    decode_payload_owned(&entry.name, raw, &limits)
}

pub fn decode_payload(name: &str, raw: &[u8]) -> Result<Vec<u8>> {
    decode_payload_with_limits(name, raw, ResourceBudget::default())
}

pub fn decode_payload_with_limits(
    name: &str,
    raw: &[u8],
    limits: ResourceBudget,
) -> Result<Vec<u8>> {
    checked_buffer_size(raw.len() as u64, limits.max_packed_bytes, "packed bytes")?;
    check_decoded_size(raw, raw.len() as u64, &limits)?;
    if raw.starts_with(MAGIC_DSC_FORMAT) {
        dsc_decode_with_limits(raw, limits).map_err(|err| {
            EthornellError::UnsupportedFormat(format!("DSC decode failed for {name}: {err}"))
        })
    } else if raw.starts_with(b"BSE 1.") {
        Err(EthornellError::UnsupportedFormat(format!(
            "BSE wrapped entry is not decoded yet: {name}"
        )))
    } else {
        let mut data = zeroed_buffer(raw.len())?;
        data.copy_from_slice(raw);
        Ok(data)
    }
}

fn decode_payload_owned(name: &str, raw: Vec<u8>, limits: &ResourceBudget) -> Result<Vec<u8>> {
    checked_buffer_size(raw.len() as u64, limits.max_packed_bytes, "packed bytes")?;
    check_decoded_size(&raw, raw.len() as u64, limits)?;
    if raw.starts_with(MAGIC_DSC_FORMAT) {
        dsc_decode_with_limits(&raw, *limits).map_err(|err| {
            EthornellError::UnsupportedFormat(format!("DSC decode failed for {name}: {err}"))
        })
    } else if raw.starts_with(b"BSE 1.") {
        Err(EthornellError::UnsupportedFormat(format!(
            "BSE wrapped entry is not decoded yet: {name}"
        )))
    } else {
        // The ResourceManager already owns the entry bytes. Returning that
        // allocation directly avoids a second full-buffer copy for ordinary
        // BP/image/audio resources.
        Ok(raw)
    }
}

pub fn read_entry_raw(path: &Path, entry: &ArchiveEntry) -> Result<Vec<u8>> {
    read_entry_raw_with_limits(path, entry, ResourceBudget::default())
}

pub fn read_entry_raw_with_limits(
    path: &Path,
    entry: &ArchiveEntry,
    limits: ResourceBudget,
) -> Result<Vec<u8>> {
    read_entry_from_file(&mut fs::File::open(path)?, entry, &limits, false)
}

fn check_decoded_size(header: &[u8], packed_size: u64, limits: &ResourceBudget) -> Result<usize> {
    let is_dsc = header.starts_with(MAGIC_DSC_FORMAT);
    let decoded_size = if is_dsc {
        if packed_size < 0x220 || header.len() < 0x18 {
            return Err(EthornellError::Parse("truncated DSC header".into()));
        }
        u64::from(u32::from_le_bytes(header[0x14..0x18].try_into().unwrap()))
    } else {
        packed_size
    };
    // Preserve the pre-existing DSC hard cap even for desktop callers, without
    // newly restricting large uncompressed movie/audio entries to that cap.
    let limit = if is_dsc {
        limits.max_decoded_bytes.min(512 * 1024 * 1024)
    } else {
        limits.max_decoded_bytes
    };
    checked_buffer_size(decoded_size, limit, "decoded bytes")
}

fn read_entry_from_file<R: ArchiveSource + ?Sized>(
    file: &mut R,
    entry: &ArchiveEntry,
    limits: &ResourceBudget,
    decoded: bool,
) -> Result<Vec<u8>> {
    let size = checked_buffer_size(entry.packed_size, limits.max_packed_bytes, "packed bytes")?;
    let end = entry
        .offset
        .checked_add(entry.packed_size)
        .ok_or_else(|| EthornellError::Parse("entry range overflows".into()))?;
    if end > file.byte_len()? {
        return Err(EthornellError::Parse(
            "entry extends past archive end".into(),
        ));
    }
    file.seek(SeekFrom::Start(entry.offset))?;
    if decoded {
        if let Some(size) = entry.unpacked_size {
            checked_buffer_size(size, limits.max_decoded_bytes, "declared decoded bytes")?;
        }
        // Inspect only a stack header before allocating the entire packed entry.
        let mut header = [0u8; 0x20];
        let header_len = size.min(header.len());
        file.read_exact(&mut header[..header_len])?;
        check_decoded_size(&header[..header_len], entry.packed_size, limits)?;
        file.seek(SeekFrom::Start(entry.offset))?;
    }
    let mut data = zeroed_buffer(size)?;
    file.read_exact(&mut data)?;
    Ok(data)
}

fn read_entry_from_file_into<R: ArchiveSource + ?Sized>(
    file: &mut R,
    entry: &ArchiveEntry,
    limits: &ResourceBudget,
    decoded: bool,
    out: &mut Vec<u8>,
) -> Result<()> {
    let size = checked_buffer_size(entry.packed_size, limits.max_packed_bytes, "packed bytes")?;
    let end = entry
        .offset
        .checked_add(entry.packed_size)
        .ok_or_else(|| EthornellError::Parse("entry range overflows".into()))?;
    if end > file.byte_len()? {
        return Err(EthornellError::Parse(
            "entry extends past archive end".into(),
        ));
    }
    file.seek(SeekFrom::Start(entry.offset))?;
    if decoded {
        if let Some(size) = entry.unpacked_size {
            checked_buffer_size(size, limits.max_decoded_bytes, "declared decoded bytes")?;
        }
        let mut header = [0u8; 0x20];
        let header_len = size.min(header.len());
        file.read_exact(&mut header[..header_len])?;
        check_decoded_size(&header[..header_len], entry.packed_size, limits)?;
        file.seek(SeekFrom::Start(entry.offset))?;
    }
    out.clear();
    out.try_reserve_exact(size)
        .map_err(|err| EthornellError::Parse(format!("resource allocation failed: {err}")))?;
    out.resize(size, 0);
    file.read_exact(out)?;
    Ok(())
}

fn read_entry_from_file_to_slice<R: ArchiveSource + ?Sized>(
    file: &mut R,
    entry: &ArchiveEntry,
    limits: &ResourceBudget,
    out: &mut [u8],
) -> Result<usize> {
    let size = checked_buffer_size(entry.packed_size, limits.max_packed_bytes, "packed bytes")?;
    if size > out.len() {
        return Err(EthornellError::Parse(format!(
            "caller buffer is too small for {}: {} < {size}",
            entry.name,
            out.len()
        )));
    }
    let end = entry
        .offset
        .checked_add(entry.packed_size)
        .ok_or_else(|| EthornellError::Parse("entry range overflows".into()))?;
    if end > file.byte_len()? {
        return Err(EthornellError::Parse(
            "entry extends past archive end".into(),
        ));
    }
    if let Some(size) = entry.unpacked_size {
        checked_buffer_size(size, limits.max_decoded_bytes, "declared decoded bytes")?;
    }
    file.seek(SeekFrom::Start(entry.offset))?;
    let mut header = [0u8; 0x20];
    let header_len = size.min(header.len());
    file.read_exact(&mut header[..header_len])?;
    check_decoded_size(&header[..header_len], entry.packed_size, limits)?;
    if header[..header_len].starts_with(MAGIC_DSC_FORMAT)
        || header[..header_len].starts_with(b"BSE 1.")
    {
        return Err(EthornellError::UnsupportedFormat(format!(
            "entry requires an owned decode buffer: {}",
            entry.name
        )));
    }
    file.seek(SeekFrom::Start(entry.offset))?;
    file.read_exact(&mut out[..size])?;
    Ok(size)
}

pub fn extract_archive(path: &Path, output_dir: &Path) -> Result<ExtractSummary> {
    extract_archive_with_options(path, output_dir, false)
}

pub fn extract_archive_with_options(
    path: &Path,
    output_dir: &Path,
    raw: bool,
) -> Result<ExtractSummary> {
    let index = read_archive_index(path)?;
    let mut summary = ExtractSummary {
        archives: 1,
        entries: index.entries.len(),
        ..ExtractSummary::default()
    };
    fs::create_dir_all(output_dir)?;
    for entry in &index.entries {
        match if raw {
            read_entry_raw(path, entry)
        } else {
            extract_entry(path, entry)
        } {
            Ok(data) => {
                let out_path = unique_output_path(&safe_output_path(output_dir, &entry.name)?);
                if let Some(parent) = out_path.parent() {
                    fs::create_dir_all(parent)?;
                }
                let mut out = fs::File::create(out_path)?;
                out.write_all(&data)?;
                summary.written += 1;
                if raw {
                    summary.raw_dumped += 1;
                } else {
                    summary.decoded += 1;
                }
            }
            Err(err) => {
                summary.skipped += 1;
                summary.errors.push(format!("{}: {err}", entry.name));
            }
        }
    }
    Ok(summary)
}

pub fn extract_game_archives(
    game: &GameRoot,
    output_dir: &Path,
    raw: bool,
) -> Result<ExtractSummary> {
    let archives = archive_paths(game)?;
    let mut total = ExtractSummary {
        archives: archives.len(),
        ..ExtractSummary::default()
    };
    fs::create_dir_all(output_dir)?;
    for archive in archives {
        let stem = archive
            .file_stem()
            .and_then(|s| s.to_str())
            .unwrap_or("archive");
        let archive_out = safe_output_path(output_dir, stem)?;
        match extract_archive_with_options(&archive, &archive_out, raw) {
            Ok(summary) => {
                total.entries += summary.entries;
                total.written += summary.written;
                total.decoded += summary.decoded;
                total.raw_dumped += summary.raw_dumped;
                total.skipped += summary.skipped;
                total.errors.extend(summary.errors);
            }
            Err(err) => total.errors.push(format!("{}: {err}", archive.display())),
        }
    }
    Ok(total)
}

pub fn archive_paths(game: &GameRoot) -> Result<Vec<PathBuf>> {
    let mut paths = Vec::new();
    for file in walk_files(game.path())? {
        if detect_archive_format(&file)? != ArchiveFormat::Unknown {
            paths.push(file);
        }
    }
    paths.sort();
    Ok(paths)
}

pub fn safe_output_path(output_dir: &Path, entry_name: &str) -> Result<PathBuf> {
    let mut out = output_dir.to_path_buf();
    let normalized = entry_name.replace('\\', "/");
    for part in Path::new(&normalized).components() {
        match part {
            Component::Normal(name) => {
                let name = name.to_string_lossy();
                let cleaned = clean_component(&name);
                if !cleaned.is_empty() {
                    out.push(cleaned);
                }
            }
            Component::CurDir => {}
            Component::ParentDir | Component::RootDir | Component::Prefix(_) => {
                return Err(EthornellError::Parse(format!(
                    "unsafe archive entry path: {entry_name}"
                )));
            }
        }
    }
    if out == output_dir {
        out.push("_");
    }
    Ok(out)
}

fn unique_output_path(path: &Path) -> PathBuf {
    if !path.exists() {
        return path.to_path_buf();
    }
    let parent = path.parent().unwrap_or_else(|| Path::new(""));
    let stem = path.file_stem().and_then(|s| s.to_str()).unwrap_or("entry");
    let ext = path.extension().and_then(|s| s.to_str());
    for i in 1.. {
        let name = match ext {
            Some(ext) if !ext.is_empty() => format!("{stem}__{i:04}.{ext}"),
            _ => format!("{stem}__{i:04}"),
        };
        let candidate = parent.join(name);
        if !candidate.exists() {
            return candidate;
        }
    }
    unreachable!("unbounded suffix search should return")
}

fn clean_component(component: &str) -> String {
    component
        .chars()
        .map(|ch| match ch {
            '\0' | ':' | '*' | '?' | '"' | '<' | '>' | '|' => '_',
            '/' | '\\' => '_',
            ch if ch.is_control() => '_',
            ch => ch,
        })
        .collect()
}

fn normalize_resource_name(name: &str) -> String {
    name.replace('\\', "/").to_ascii_lowercase()
}

fn resource_entry_from_archive(archive: &ArchiveIndex, entry: &ArchiveEntry) -> ResourceEntry {
    ResourceEntry {
        archive_path: archive.path.clone(),
        entry_name: entry.name.clone(),
        packed_size: entry.packed_size,
        unpacked_size: entry.unpacked_size,
        flags: entry.flags,
    }
}

pub fn scan_game_root(root: &GameRoot) -> Result<ScanReport> {
    let mut report = ScanReport {
        game_root: root.path().to_path_buf(),
        ..ScanReport::default()
    };
    for path in walk_files(root.path())? {
        let metadata = fs::metadata(&path)?;
        let mut header = [0u8; 64];
        let read = fs::File::open(&path)?.read(&mut header)?;
        let magic = detect_file_magic(&path, &header[..read]);
        let extension = path
            .extension()
            .and_then(|s| s.to_str())
            .map(|s| s.to_ascii_lowercase());
        let archive_format = detect_archive_format_from_bytes(&header[..read]);

        if extension.as_deref() == Some("arc") || archive_format != ArchiveFormat::Unknown {
            report.arc_candidates += 1;
        }
        if magic == MagicKind::Bp {
            report.bp_scripts += 1;
        }
        if magic == MagicKind::BurikoCompiledScript {
            report.scenario_candidates += 1;
        }
        if magic == MagicKind::CompressedBg {
            report.compressed_bg_candidates += 1;
        }
        if magic == MagicKind::Ogg || extension.as_deref() == Some("ogg") {
            report.ogg_candidates += 1;
        }

        report.files.push(FileProbe {
            path,
            size: metadata.len(),
            extension,
            magic,
            archive_format,
        });
    }
    report.file_count = report.files.len();
    report.unknown_count = report
        .files
        .iter()
        .filter(|probe| probe.magic == MagicKind::Unknown)
        .count();
    Ok(report)
}

pub fn walk_files(root: &Path) -> Result<Vec<PathBuf>> {
    let mut files = Vec::new();
    walk_files_inner(root, &mut files)?;
    files.sort();
    Ok(files)
}

fn walk_files_inner(dir: &Path, files: &mut Vec<PathBuf>) -> Result<()> {
    for entry in fs::read_dir(dir)? {
        let entry = entry?;
        let path = entry.path();
        let metadata = entry.metadata()?;
        if metadata.is_dir() {
            walk_files_inner(&path, files)?;
        } else if metadata.is_file() {
            files.push(path);
        }
    }
    Ok(())
}

#[derive(Clone, Debug)]
struct DscNode {
    is_parent: bool,
    code: Option<u16>,
    left: usize,
    right: usize,
}

#[derive(Clone, Debug, Eq, PartialEq, Ord, PartialOrd)]
struct DscCode {
    depth: u8,
    code: u16,
}

pub fn dsc_decode(data: &[u8]) -> Result<Vec<u8>> {
    dsc_decode_with_limits(data, ResourceBudget::default())
}

pub fn dsc_decode_with_limits(data: &[u8], limits: ResourceBudget) -> Result<Vec<u8>> {
    checked_buffer_size(data.len() as u64, limits.max_packed_bytes, "packed bytes")?;
    if data.len() < 0x220 || !data.starts_with(MAGIC_DSC_FORMAT) {
        return Err(EthornellError::UnsupportedFormat(
            "missing DSC FORMAT 1.00 header".into(),
        ));
    }
    let mut key = u32::from_le_bytes([data[0x10], data[0x11], data[0x12], data[0x13]]);
    let magic = (u16::from_le_bytes([data[0], data[1]]) as u32) << 16;
    let output_size = check_decoded_size(data, data.len() as u64, &limits)?;
    let dec_count = u32::from_le_bytes([data[0x18], data[0x19], data[0x1a], data[0x1b]]) as usize;
    if output_size == 0 {
        return Ok(Vec::new());
    }
    let mut codes = Vec::new();
    for i in 0..512usize {
        let depth = data[0x20 + i].wrapping_sub(update_key(&mut key, magic));
        if depth > 0 {
            codes.push(DscCode {
                depth,
                code: i as u16,
            });
        }
    }
    codes.sort();
    let nodes = create_dsc_tree(&codes)?;
    let mut reader = BitReader::new(&data[0x220..]);
    let mut output = zeroed_buffer(output_size)?;
    let mut dst = 0usize;

    for _ in 0..dec_count {
        let mut node_index = 0usize;
        loop {
            let bit = reader.next_bit()?;
            let node = nodes
                .get(node_index)
                .ok_or_else(|| EthornellError::Parse("DSC huffman node out of range".into()))?;
            node_index = if bit { node.right } else { node.left };
            let node = nodes
                .get(node_index)
                .ok_or_else(|| EthornellError::Parse("DSC huffman child out of range".into()))?;
            if !node.is_parent {
                let code = node
                    .code
                    .ok_or_else(|| EthornellError::Parse("DSC missing leaf code".into()))?;
                if (code >> 8) == 1 {
                    let offset = reader.next_bits(12)? as usize + 2;
                    let count = (code as usize & 0xff) + 2;
                    if offset > dst {
                        return Err(EthornellError::Parse(
                            "DSC back-reference before output start".into(),
                        ));
                    }
                    for _ in 0..count {
                        if dst >= output.len() {
                            break;
                        }
                        let value = output[dst - offset];
                        output[dst] = value;
                        dst += 1;
                    }
                } else if dst < output.len() {
                    output[dst] = code as u8;
                    dst += 1;
                }
                break;
            }
        }
        if dst >= output.len() {
            break;
        }
    }
    Ok(output)
}

fn create_dsc_tree(codes: &[DscCode]) -> Result<Vec<DscNode>> {
    if codes.is_empty() {
        return Err(EthornellError::Parse("DSC huffman tree is empty".into()));
    }
    let mut nodes = vec![
        DscNode {
            is_parent: false,
            code: None,
            left: 0,
            right: 0,
        };
        1024
    ];
    let mut left_index = vec![0usize; 512];
    let mut right_index = vec![0usize; 512];
    let mut next_node = 1usize;
    let mut depth_nodes = 1usize;
    let mut depth = 0u16;
    let mut left_child = true;
    let mut n = 0usize;
    while n < codes.len() {
        if depth > u8::MAX as u16 || depth_nodes > 512 || next_node > 1024 {
            return Err(EthornellError::Parse(
                "DSC huffman tree exceeds node budget".into(),
            ));
        }
        let target_left = left_child;
        left_child = !left_child;
        let mut depth_existing = 0usize;
        while n < codes.len() && u16::from(codes[n].depth) == depth {
            if depth_existing >= depth_nodes {
                return Err(EthornellError::Parse(
                    "DSC huffman tree is oversubscribed".into(),
                ));
            }
            let index = if target_left {
                left_index[depth_existing]
            } else {
                right_index[depth_existing]
            };
            nodes[index].code = Some(codes[n].code);
            n += 1;
            depth_existing += 1;
        }
        let to_create = depth_nodes.saturating_sub(depth_existing);
        if to_create > 256 || next_node + to_create * 2 > nodes.len() {
            return Err(EthornellError::Parse(
                "DSC huffman tree exceeds node budget".into(),
            ));
        }
        for i in 0..to_create {
            let index = if target_left {
                left_index[depth_existing + i]
            } else {
                right_index[depth_existing + i]
            };
            nodes[index].is_parent = true;
            if left_child {
                left_index[i * 2] = next_node;
                nodes[index].left = next_node;
                next_node += 1;
                left_index[i * 2 + 1] = next_node;
                nodes[index].right = next_node;
                next_node += 1;
            } else {
                right_index[i * 2] = next_node;
                nodes[index].left = next_node;
                next_node += 1;
                right_index[i * 2 + 1] = next_node;
                nodes[index].right = next_node;
                next_node += 1;
            }
        }
        depth += 1;
        depth_nodes = to_create * 2;
    }
    Ok(nodes)
}

fn update_key(key: &mut u32, magic: u32) -> u8 {
    let v0 = 20021u32.wrapping_mul(*key & 0xffff);
    let mut v1 = magic | (*key >> 16);
    v1 = v1
        .wrapping_mul(20021)
        .wrapping_add((*key).wrapping_mul(346));
    v1 = v1.wrapping_add(v0 >> 16) & 0xffff;
    *key = (v1 << 16).wrapping_add(v0 & 0xffff).wrapping_add(1);
    v1 as u8
}

struct BitReader<'a> {
    data: &'a [u8],
    pos: usize,
    bits: u32,
    nbits: u8,
}

impl<'a> BitReader<'a> {
    fn new(data: &'a [u8]) -> Self {
        Self {
            data,
            pos: 0,
            bits: 0,
            nbits: 0,
        }
    }

    fn next_bit(&mut self) -> Result<bool> {
        if self.nbits == 0 {
            let byte = self
                .data
                .get(self.pos)
                .ok_or_else(|| EthornellError::Parse("DSC bitstream exhausted".into()))?;
            self.bits = *byte as u32;
            self.pos += 1;
            self.nbits = 8;
        }
        let bit = (self.bits & 0x80) != 0;
        self.bits = (self.bits << 1) & 0xff;
        self.nbits -= 1;
        Ok(bit)
    }

    fn next_bits(&mut self, count: u8) -> Result<u32> {
        let mut value = 0u32;
        for _ in 0..count {
            value = (value << 1) | u32::from(self.next_bit()?);
        }
        Ok(value)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn decoded_length_header_rejects_oversize_and_reports_dsc_payload_not_archive_size() {
        let mut header = [0u8; 32];
        header[..super::MAGIC_DSC_FORMAT.len()].copy_from_slice(super::MAGIC_DSC_FORMAT);
        header[0x14..0x18].copy_from_slice(&1_030_814u32.to_le_bytes());
        assert_eq!(
            super::check_decoded_size(&header, 464_347, &super::ResourceBudget::android()).unwrap(),
            1_030_814
        );
        header[0x14..0x18].copy_from_slice(&u32::MAX.to_le_bytes());
        assert!(
            super::check_decoded_size(&header, 464_347, &super::ResourceBudget::android()).is_err()
        );
        assert!(
            super::check_decoded_size(&header[..16], 16, &super::ResourceBudget::android())
                .is_err()
        );
    }
    use super::*;
    use std::sync::atomic::{AtomicU64, Ordering};

    struct TestArchive(PathBuf);

    impl TestArchive {
        fn new(bytes: &[u8]) -> Self {
            static NEXT: AtomicU64 = AtomicU64::new(0);
            let path = std::env::temp_dir().join(format!(
                "ethornell-archive-budget-{}-{}.arc",
                std::process::id(),
                NEXT.fetch_add(1, Ordering::Relaxed)
            ));
            let mut file = fs::OpenOptions::new()
                .write(true)
                .create_new(true)
                .open(&path)
                .unwrap();
            file.write_all(bytes).unwrap();
            Self(path)
        }

        fn files(&self) -> Vec<(PathBuf, fs::File)> {
            vec![(self.0.clone(), fs::File::open(&self.0).unwrap())]
        }

        fn manager(&self, limits: ResourceBudget) -> ResourceManager {
            ResourceManager::from_authorized_files_with_limits(
                PathBuf::from("game"),
                self.files(),
                limits,
            )
            .unwrap()
        }
    }

    impl Drop for TestArchive {
        fn drop(&mut self) {
            let _ = fs::remove_file(&self.0);
        }
    }

    fn archive_bytes(format: ArchiveFormat, payload: &[u8]) -> Vec<u8> {
        let (magic, record_size) = match format {
            ArchiveFormat::PackFile => (MAGIC_PACK_FILE_FULL, 0x20),
            ArchiveFormat::BurikoArc20 => (MAGIC_BURIKO_ARC20, 0x80),
            ArchiveFormat::Unknown => unreachable!(),
        };
        let mut data = magic.to_vec();
        // ARC20 magic is 11 bytes followed by a terminator.
        data.resize(12, 0);
        data.extend_from_slice(&1u32.to_le_bytes());
        let mut record = vec![0; record_size];
        record[..8].copy_from_slice(b"ipl._bp\0");
        let name_size = if record_size == 0x20 { 0x10 } else { 0x60 };
        record[name_size + 4..name_size + 8].copy_from_slice(&(payload.len() as u32).to_le_bytes());
        data.extend(record);
        data.extend_from_slice(payload);
        data
    }

    fn dsc_header(output_size: u32) -> Vec<u8> {
        let mut data = vec![0; 0x220];
        data[..MAGIC_DSC_FORMAT.len()].copy_from_slice(MAGIC_DSC_FORMAT);
        data[0x14..0x18].copy_from_slice(&output_size.to_le_bytes());
        data
    }

    fn assert_budget_error<T: std::fmt::Debug>(result: Result<T>, kind: &str) {
        let err = result.unwrap_err().to_string();
        assert!(
            err.contains("resource budget exceeded") && err.contains(kind),
            "{err}"
        );
    }

    #[test]
    fn all_manager_entrypoints_enforce_packed_budget() {
        for format in [ArchiveFormat::PackFile, ArchiveFormat::BurikoArc20] {
            let fixture = TestArchive::new(&archive_bytes(format, b"12345"));
            let manager = fixture.manager(ResourceBudget {
                max_packed_bytes: 4,
                ..ResourceBudget::default()
            });
            let entry = manager.find("ipl._bp").unwrap();
            let archive = fixture.0.file_name().unwrap().to_str().unwrap();
            let before = ALLOCATION_ATTEMPTS.with(|count| count.get());
            assert_budget_error(manager.read_raw("ipl._bp"), "packed bytes");
            assert_budget_error(manager.read_decoded("ipl._bp"), "packed bytes");
            assert_budget_error(
                manager.read_decoded_from_archive(archive, "ipl._bp"),
                "packed bytes",
            );
            assert_budget_error(manager.read_by_entry_raw(&entry), "packed bytes");
            assert_budget_error(manager.read_by_entry_decoded(&entry), "packed bytes");
            assert_eq!(ALLOCATION_ATTEMPTS.with(|count| count.get()), before);
        }
    }

    #[test]
    fn all_decoded_paths_check_plain_and_dsc_sizes_before_payload_allocation() {
        for payload in [b"12345".to_vec(), dsc_header(5)] {
            let limits = ResourceBudget {
                max_decoded_bytes: 4,
                ..ResourceBudget::default()
            };
            let fixture = TestArchive::new(&archive_bytes(ArchiveFormat::PackFile, &payload));
            let manager = fixture.manager(limits);
            let entry = manager.find("ipl._bp").unwrap();
            let archive = fixture.0.file_name().unwrap().to_str().unwrap();
            // Raw reads do not decode and are intentionally only packed-limited.
            assert_eq!(manager.read_raw("ipl._bp").unwrap(), payload);
            let before = ALLOCATION_ATTEMPTS.with(|count| count.get());
            assert_budget_error(manager.read_decoded("ipl._bp"), "decoded bytes");
            assert_budget_error(
                manager.read_decoded_from_archive(archive, "ipl._bp"),
                "decoded bytes",
            );
            assert_budget_error(manager.read_by_entry_decoded(&entry), "decoded bytes");
            assert_budget_error(
                decode_payload_with_limits("ipl", &payload, limits),
                "decoded bytes",
            );
            assert_budget_error(
                decode_payload_owned("ipl", payload.clone(), &limits),
                "decoded bytes",
            );
            if payload.starts_with(MAGIC_DSC_FORMAT) {
                assert_budget_error(dsc_decode_with_limits(&payload, limits), "decoded bytes");
            }
            assert_eq!(ALLOCATION_ATTEMPTS.with(|count| count.get()), before);
        }
    }

    #[test]
    fn index_budgets_apply_before_reservation_and_across_archives() {
        for format in [ArchiveFormat::PackFile, ArchiveFormat::BurikoArc20] {
            let fixture = TestArchive::new(&archive_bytes(format, b"x"));
            for (limits, kind) in [
                (
                    ResourceBudget {
                        max_total_index_entries: 0,
                        ..ResourceBudget::default()
                    },
                    "total index entries",
                ),
                (
                    ResourceBudget {
                        max_total_index_bytes: 1,
                        ..ResourceBudget::default()
                    },
                    "total index bytes",
                ),
                (
                    ResourceBudget {
                        max_archives: 0,
                        ..ResourceBudget::default()
                    },
                    "archive count",
                ),
            ] {
                let before = ALLOCATION_ATTEMPTS.with(|count| count.get());
                assert_budget_error(
                    ResourceManager::from_authorized_files_with_limits(
                        PathBuf::new(),
                        fixture.files(),
                        limits,
                    ),
                    kind,
                );
                assert_eq!(ALLOCATION_ATTEMPTS.with(|count| count.get()), before);
            }
            let mut files = fixture.files();
            files.extend(fixture.files());
            assert_budget_error(
                ResourceManager::from_authorized_files_with_limits(
                    PathBuf::new(),
                    files,
                    ResourceBudget {
                        max_total_index_entries: 1,
                        ..ResourceBudget::default()
                    },
                ),
                "total index entries",
            );

            let mut usage = IndexUsage::default();
            let mut file = fs::File::open(&fixture.0).unwrap();
            file.seek(SeekFrom::Start(12)).unwrap();
            read_index_from_file(
                &fixture.0,
                &mut file,
                format,
                &ResourceBudget::default(),
                &mut usage,
            )
            .unwrap();
            let mut files = fixture.files();
            files.extend(fixture.files());
            assert_budget_error(
                ResourceManager::from_authorized_files_with_limits(
                    PathBuf::new(),
                    files,
                    ResourceBudget {
                        max_total_index_bytes: usage.bytes,
                        ..ResourceBudget::default()
                    },
                ),
                "total index bytes",
            );
        }
    }

    #[test]
    fn truncated_index_is_rejected_before_entry_reservation() {
        let mut data = MAGIC_PACK_FILE_FULL.to_vec();
        data.extend_from_slice(&0x0f_ffffu32.to_le_bytes());
        let fixture = TestArchive::new(&data);
        let before = ALLOCATION_ATTEMPTS.with(|count| count.get());
        let err =
            ResourceManager::from_authorized_files(PathBuf::new(), fixture.files()).unwrap_err();
        assert!(err.to_string().contains("index extends past archive end"));
        assert_eq!(ALLOCATION_ATTEMPTS.with(|count| count.get()), before);
    }

    #[test]
    fn zero_dsc_target_never_walks_tree_or_reads_bitstream() {
        let mut data = dsc_header(0);
        data[0x18..0x1c].copy_from_slice(&u32::MAX.to_le_bytes());
        let limits = ResourceBudget {
            max_decoded_bytes: 0,
            ..ResourceBudget::default()
        };
        assert!(dsc_decode_with_limits(&data, limits).unwrap().is_empty());
        let fixture = TestArchive::new(&archive_bytes(ArchiveFormat::PackFile, &data));
        assert!(
            fixture
                .manager(limits)
                .read_decoded("ipl._bp")
                .unwrap()
                .is_empty()
        );
    }

    #[test]
    fn malformed_dsc_tree_returns_error_instead_of_panicking() {
        assert!(create_dsc_tree(&[]).is_err());
        assert!(
            create_dsc_tree(&[DscCode {
                depth: 255,
                code: 0
            }])
            .is_err()
        );
        assert!(create_dsc_tree(&vec![DscCode { depth: 1, code: 0 }; 3]).is_err());
    }

    #[test]
    fn valid_dsc_respects_exact_budget_and_shared_decode_path() {
        let mut payload = dsc_header(3);
        payload[0x18..0x1c].copy_from_slice(&3u32.to_le_bytes());
        let mut key = 0;
        let magic = (u16::from_le_bytes([payload[0], payload[1]]) as u32) << 16;
        for i in 0..512 {
            let depth = u8::from(i == 65 || i == 66);
            payload[0x20 + i] = depth.wrapping_add(update_key(&mut key, magic));
        }
        payload.push(0b0100_0000); // A B A
        let limits = ResourceBudget {
            max_packed_bytes: payload.len() as u64,
            max_decoded_bytes: 3,
            ..ResourceBudget::default()
        };
        assert_eq!(dsc_decode_with_limits(&payload, limits).unwrap(), b"ABA");
        let fixture = TestArchive::new(&archive_bytes(ArchiveFormat::PackFile, &payload));
        let manager = fixture.manager(limits);
        assert_eq!(manager.read_decoded("ipl._bp").unwrap(), b"ABA");
        assert_eq!(
            manager
                .read_by_entry_decoded(&manager.find("ipl._bp").unwrap())
                .unwrap(),
            b"ABA"
        );
    }

    #[test]
    fn overlapping_entry_aliases_are_not_rejected() {
        let mut data = archive_bytes(ArchiveFormat::PackFile, b"hello");
        let record = data[0x10..0x30].to_vec();
        data.splice(0x30..0x30, record);
        data[12..16].copy_from_slice(&2u32.to_le_bytes());
        let fixture = TestArchive::new(&data);
        let manager = fixture.manager(ResourceBudget::android());
        assert_eq!(manager.resource_count(), 2);
        for entry in manager.list() {
            assert_eq!(manager.read_by_entry_decoded(&entry).unwrap(), b"hello");
        }
    }

    #[test]
    fn retained_authorized_descriptor_survives_path_removal_and_initial_cursor() {
        let fixture = TestArchive::new(&archive_bytes(ArchiveFormat::PackFile, b"hello"));
        let mut files = fixture.files();
        files[0].1.seek(SeekFrom::End(0)).unwrap();
        let manager = ResourceManager::from_authorized_files_with_limits(
            PathBuf::new(),
            files,
            ResourceBudget::android(),
        )
        .unwrap();
        fs::remove_file(&fixture.0).unwrap();
        assert_eq!(manager.read_decoded("ipl._bp").unwrap(), b"hello");
    }

    #[test]
    fn truncated_payload_and_overflowing_external_metadata_are_rejected() {
        let fixture = TestArchive::new(&archive_bytes(ArchiveFormat::PackFile, b"hello"));
        let manager = fixture.manager(ResourceBudget::default());
        fs::OpenOptions::new()
            .write(true)
            .open(&fixture.0)
            .unwrap()
            .set_len(0x30 + 2)
            .unwrap();
        assert!(manager.read_raw("ipl._bp").is_err());
        let mut entry = manager.archives().archives[0].entries[0].clone();
        entry.offset = u64::MAX;
        assert!(read_entry_raw(&fixture.0, &entry).is_err());
        entry.packed_size = u64::MAX;
        assert_budget_error(read_entry_raw(&fixture.0, &entry), "packed bytes");
    }

    #[test]
    fn detects_known_magic() {
        assert_eq!(detect_magic(b"BURIKO ARC20xxxx"), MagicKind::BurikoArc20);
        assert_eq!(
            detect_magic(b"CompressedBG___xxxx"),
            MagicKind::CompressedBg
        );
        assert_eq!(
            detect_archive_format_from_bytes(b"PackFile    "),
            ArchiveFormat::PackFile
        );
    }

    #[test]
    fn unknown_magic_is_stable() {
        assert_eq!(detect_magic(b"not bgi"), MagicKind::Unknown);
    }

    #[test]
    fn safe_path_rejects_traversal() {
        assert!(safe_output_path(Path::new("out"), "../bad").is_err());
        assert!(safe_output_path(Path::new("out"), "/bad").is_err());
        assert_eq!(
            safe_output_path(Path::new("out"), "dir\\a:b?.txt").unwrap(),
            PathBuf::from("out/dir/a_b_.txt")
        );
    }

    #[test]
    fn unique_path_keeps_first_name_when_free() {
        assert_eq!(
            unique_output_path(Path::new("out/free-name")),
            PathBuf::from("out/free-name")
        );
    }

    #[test]
    fn resource_lookup_indexes_archive_and_entry_without_listing() {
        let set = GameArchiveSet {
            root: PathBuf::from("game"),
            archives: vec![
                ArchiveIndex {
                    path: PathBuf::from("game/system.arc"),
                    format: ArchiveFormat::BurikoArc20,
                    entries: vec![ArchiveEntry {
                        name: "IPL._BP".to_string(),
                        offset: 0x100,
                        packed_size: 64,
                        unpacked_size: None,
                        flags: 0,
                        method: None,
                    }],
                },
                ArchiveIndex {
                    path: PathBuf::from("game/sysprg.arc"),
                    format: ArchiveFormat::BurikoArc20,
                    entries: vec![ArchiveEntry {
                        name: "ipl._bp".to_string(),
                        offset: 0x200,
                        packed_size: 96,
                        unpacked_size: None,
                        flags: 0,
                        method: None,
                    }],
                },
            ],
        };
        let (by_name, archive_by_name, archive_by_path) = build_resource_lookup(&set);
        let manager = ResourceManager {
            inner: Arc::new(ResourceManagerInner {
                limits: ResourceBudget::default(),
                set,
                by_name,
                archive_by_name,
                archive_by_path,
                archive_files: Vec::new(),
                search: RwLock::default(),
            }),
        };

        assert_eq!(manager.find_locator("ipl._bp"), Some((0, 0)));
        assert_eq!(
            manager.find_locator_in_archive("SYSPRG.ARC", "IPL._BP"),
            Some((1, 0))
        );
        let entry = manager.find_in_archive("system.arc", "ipl._bp").unwrap();
        assert_eq!(entry.archive_path, PathBuf::from("game/system.arc"));
        assert_eq!(entry.packed_size, 64);
    }

    fn manager_over(archives: &[(&str, &[&str])]) -> ResourceManager {
        let set = GameArchiveSet {
            root: PathBuf::from("game"),
            archives: archives
                .iter()
                .map(|(path, names)| ArchiveIndex {
                    path: PathBuf::from(format!("game/{path}")),
                    format: ArchiveFormat::BurikoArc20,
                    entries: names
                        .iter()
                        .map(|name| ArchiveEntry {
                            name: name.to_string(),
                            offset: 0,
                            packed_size: 1,
                            unpacked_size: None,
                            flags: 0,
                            method: None,
                        })
                        .collect(),
                })
                .collect(),
        };
        let (by_name, archive_by_name, archive_by_path) = build_resource_lookup(&set);
        ResourceManager {
            inner: Arc::new(ResourceManagerInner {
                limits: ResourceBudget::default(),
                set,
                by_name,
                archive_by_name,
                archive_by_path,
                archive_files: Vec::new(),
                search: RwLock::default(),
            }),
        }
    }

    #[test]
    fn composite_archive_searches_members_in_registration_order() {
        let manager = manager_over(&[
            ("data03000.arc", &["shared", "only03"]),
            ("data05000.arc", &["shared"]),
        ]);
        assert!(manager.find_in_archive("data05xxx.arc", "only03").is_none());

        let members = vec!["data05000.arc".to_string(), "data03000.arc".to_string()];
        assert!(manager.register_composite_archive("DATA05XXX.ARC", members.clone()));
        assert!(!manager.register_composite_archive("data05xxx.arc", members));
        assert!(!manager.register_composite_archive("empty.arc", Vec::new()));

        let shared = manager.find_in_archive("data05xxx.arc", "shared").unwrap();
        assert_eq!(shared.archive_path, PathBuf::from("game/data05000.arc"));
        let fallback = manager.find_in_archive("data05xxx.arc", "only03").unwrap();
        assert_eq!(fallback.archive_path, PathBuf::from("game/data03000.arc"));
        assert_eq!(
            manager
                .first_in_archive("data05xxx.arc")
                .unwrap()
                .archive_path,
            PathBuf::from("game/data05000.arc")
        );
    }

    #[test]
    fn loose_search_paths_apply_newest_first_once_enabled() {
        let manager = manager_over(&[]);
        manager.prepend_loose_search_path("Graphic\\CVTD");
        manager.prepend_loose_search_path("Script\\CVTD");
        assert!(manager.loose_search_paths().is_empty());
        manager.set_loose_search_fallback(true);
        assert_eq!(
            manager.loose_search_paths(),
            ["Script\\CVTD", "Graphic\\CVTD"]
        );
    }
}
