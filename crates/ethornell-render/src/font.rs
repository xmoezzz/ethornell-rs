use ab_glyph::{Font, FontArc, FontVec, PxScale, ScaleFont};
use ethornell_core::{EthornellError, Result};
use std::collections::BTreeSet;
use std::fs::File;
use std::io::{Read, Seek, SeekFrom};
use std::path::{Path, PathBuf};

/// How deep to descend into font directories. Distro layouts nest one or two
/// levels (`truetype/noto`, `opentype/noto`), vendor bundles rarely more.
const FONT_SCAN_DEPTH: usize = 4;
/// Directory entries the scan may visit, fonts or not, so a pathological
/// font tree cannot stall bootstrap.
const FONT_SCAN_ENTRY_LIMIT: usize = 16384;
/// Discovered files tried at most when no face matches.
const FONT_SCAN_FILE_LIMIT: usize = 4096;
/// Faces checked per TrueType/OpenType collection.
const MAX_COLLECTION_FACES: u32 = 32;

/// Stateless font loading shared by GPU text, CPU snapshots and text metrics.
/// Keep the returned FontArc in the session, not a process-wide OnceLock.
pub struct FontProvider;

impl FontProvider {
    /// BGI's logical font size measures a full-width em, not the substitute
    /// face's ascent+descent. PxScale measures the latter: using the script
    /// size directly shrinks Noto CJK (1448 height units per 1000-unit em).
    /// Keep this conversion shared by rasterization and advance measurement.
    pub fn pixel_em_scale(font: &FontArc, size: f32) -> PxScale {
        let size = size.max(1.0);
        let height = font.height_unscaled();
        let factor = font
            .units_per_em()
            .filter(|em| em.is_finite() && *em > 0.0 && height.is_finite() && height > 0.0)
            .map_or(1.0, |em| height / em);
        PxScale::from(size * factor)
    }

    /// Align replacement outlines to BGI's logical cell rather than the face's
    /// extra vertical headroom. The em-font path in sub_42DF40 subtracts
    /// max(tmAscent - 7 * height / 8, 0) from the rasterized glyph origin.
    /// Apply that same baseline limit before drawing into fixed-height cells.
    pub fn cell_ascent(font: &FontArc, size: f32) -> f32 {
        let size = size.max(1.0);
        font.as_scaled(Self::pixel_em_scale(font, size))
            .ascent()
            .min(size * 7.0 / 8.0)
    }

    /// Loads exactly the requested face (TTC/OTC indices are zero-based).
    /// Explicit selection never silently falls back to a different file/face.
    pub fn load_file(path: impl AsRef<Path>, face_index: u32) -> Result<FontArc> {
        let bytes = std::fs::read(path)?;
        Self::load_bytes(bytes, face_index)
    }

    pub fn load_bytes(bytes: Vec<u8>, face_index: u32) -> Result<FontArc> {
        FontVec::try_from_vec_and_index(bytes, face_index)
            .map(FontArc::new)
            .map_err(|_| EthornellError::Other(format!("invalid font or face index {face_index}")))
    }

    /// Unique missing printable codepoints, in deterministic order. This is a
    /// cmap check, not a guarantee of shaping or of coverage of an entire script.
    pub fn missing_glyphs(font: &FontArc, text: &str) -> Vec<char> {
        required_chars(text)
            .into_iter()
            .filter(|ch| font.glyph_id(*ch).0 == 0)
            .collect()
    }

    pub fn require_coverage(font: &FontArc, text: &str) -> Result<()> {
        let missing = Self::missing_glyphs(font, text);
        if missing.is_empty() {
            Ok(())
        } else {
            Err(EthornellError::Other(format!(
                "font missing {} required glyphs: {:?}",
                missing.len(),
                missing
            )))
        }
    }

    pub fn load_system_cjk_font() -> Result<FontArc> {
        Self::load_system_for_text("A日本語中文")
    }

    /// Hosts should pass their own required SJIS/GBK repertoire/sample here,
    /// or validate their explicit font with require_coverage before bootstrap.
    pub fn load_system_for_text(required_text: &str) -> Result<FontArc> {
        Self::load_system_for_text_preferring(&[], required_text)
    }

    /// Like [`Self::load_system_for_text`], but first takes an installed face
    /// whose family name is one of `families`, as GDI's CreateFont resolves
    /// the face a game names. Family names (name ID 1) match in any language
    /// the face records, without case, as GDI matches `lfFaceName`. A named
    /// face that lacks `required_text` is passed over.
    pub fn load_system_for_text_preferring(
        families: &[&str],
        required_text: &str,
    ) -> Result<FontArc> {
        let mut search = SystemFontSearch::new(required_text);
        for family in families {
            if let Some(font) = search.first_match(Some(&family.trim().to_lowercase())) {
                tracing::info!(family, "loaded the requested font face from the system");
                return Ok(font);
            }
            tracing::info!(
                family,
                "requested font face is not installed; using a substitute"
            );
        }
        search.first_match(None).ok_or_else(|| {
            EthornellError::Other(format!(
                "no usable system font with required glyph coverage (tried {} faces in {} files; set ETHORNELL_FONT to a CJK-capable font file)",
                search.faces, search.files
            ))
        })
    }
}

/// The printable chars of `text`, each once, in order.
fn required_chars(text: &str) -> Vec<char> {
    text.chars()
        .filter(|ch| !ch.is_control())
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect()
}

/// Tries system font files for the first face covering the required text,
/// and counts what the last search read for the failure message.
struct SystemFontSearch {
    chars: Vec<char>,
    candidates: Vec<PathBuf>,
    /// Font files under the system roots, minus the candidates; walked only
    /// when a search misses every candidate.
    discovered: Option<Vec<PathBuf>>,
    files: usize,
    faces: usize,
}

impl SystemFontSearch {
    fn new(required: &str) -> Self {
        Self {
            chars: required_chars(required),
            candidates: super::SYSTEM_CJK_FONT_CANDIDATES
                .iter()
                .filter_map(|path| std::fs::canonicalize(path).ok())
                .collect(),
            discovered: None,
            files: 0,
            faces: 0,
        }
    }

    /// The well-known paths usually hold a match, so the font directories
    /// are walked only when none of them does. A TTC can contain regional
    /// faces, so every face of each file is checked rather than assuming
    /// face 0 supports the session's language. Only the winning file is read
    /// whole and parsed. `family` is lowercased.
    fn first_match(&mut self, family: Option<&str>) -> Option<FontArc> {
        let Self {
            chars,
            candidates,
            discovered,
            files,
            faces,
        } = self;
        (*files, *faces) = (0, 0);
        let mut check = |path: &PathBuf| {
            *files += 1;
            let face = covering_face(path, family, chars, faces)?;
            let font = FontProvider::load_file(path, face).ok()?;
            // Confirm on the parsed face, so a disagreement with the cmap
            // check can never pick a font that lacks a glyph.
            chars.iter().all(|&ch| font.glyph_id(ch).0 != 0).then(|| {
                tracing::debug!(path = %path.display(), face, "loaded system CJK font");
                font
            })
        };
        candidates.iter().find_map(&mut check).or_else(|| {
            discovered
                .get_or_insert_with(|| {
                    let mut files = discovered_font_files(&system_font_roots());
                    files.retain(|path| !candidates.contains(path));
                    files
                })
                .iter()
                .find_map(&mut check)
        })
    }
}

/// Font files discovered under `roots`, in path order. Fontconfig installs
/// are not required to place CJK fonts at the paths of
/// `SYSTEM_CJK_FONT_CANDIDATES`, so desktop hosts need this fallback
/// (`/usr/share/fonts/droid/...`, Nix profiles, etc.). Checking a file reads
/// only its `cmap`, so no name heuristic is needed to try CJK fonts first.
fn discovered_font_files(roots: &[PathBuf]) -> Vec<PathBuf> {
    let mut found = BTreeSet::new();
    let mut budget = FONT_SCAN_ENTRY_LIMIT;
    for root in roots {
        collect_font_files(root, FONT_SCAN_DEPTH, &mut budget, &mut found);
    }
    found.into_iter().take(FONT_SCAN_FILE_LIMIT).collect()
}

/// Canonical font roots, user directories first as fontconfig orders them.
fn system_font_roots() -> Vec<PathBuf> {
    let mut roots = Vec::new();
    if cfg!(target_os = "linux") || cfg!(target_os = "freebsd") {
        if let Some(data) = std::env::var_os("XDG_DATA_HOME") {
            roots.push(PathBuf::from(data).join("fonts"));
        }
        if let Some(home) = std::env::var_os("HOME") {
            let home = PathBuf::from(home);
            roots.push(home.join(".local/share/fonts"));
            roots.push(home.join(".fonts"));
        }
        roots.extend(
            [
                "/usr/local/share/fonts",
                "/usr/share/fonts",
                "/run/current-system/sw/share/fonts",
            ]
            .map(PathBuf::from),
        );
    } else if cfg!(target_os = "macos") {
        if let Some(home) = std::env::var_os("HOME") {
            roots.push(PathBuf::from(home).join("Library/Fonts"));
        }
        roots.extend(
            [
                "/Library/Fonts",
                "/System/Library/Fonts",
                "/System/Library/AssetsV2/com_apple_MobileAsset_Font7",
            ]
            .map(PathBuf::from),
        );
    } else if cfg!(windows) {
        if let Some(local) = std::env::var_os("LOCALAPPDATA") {
            roots.push(PathBuf::from(local).join("Microsoft/Windows/Fonts"));
        }
        if let Some(windir) = std::env::var_os("WINDIR") {
            roots.push(PathBuf::from(windir).join("Fonts"));
        }
    }
    // Android/iOS/consoles ship fonts at fixed paths already covered by the
    // candidate list; scanning their system roots is neither useful nor safe.
    let mut seen_roots = BTreeSet::new();
    roots
        .into_iter()
        .filter_map(|root| std::fs::canonicalize(root).ok())
        .filter(|root| root.is_dir() && seen_roots.insert(root.clone()))
        .collect()
}

/// Collects canonical font paths below `dir`, which must itself be canonical.
fn collect_font_files(dir: &Path, depth: usize, budget: &mut usize, files: &mut BTreeSet<PathBuf>) {
    if depth == 0 {
        return;
    }
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        // Every visited entry counts, not only fonts, so deep directories
        // and trees of bitmap or Type 1 fonts stay bounded too.
        let Some(left) = budget.checked_sub(1) else {
            return;
        };
        *budget = left;
        let Ok(kind) = entry.file_type() else {
            continue;
        };
        let name = entry.path();
        // Nix profiles and Debian alternatives link fonts, and whole font
        // directories, from elsewhere. Follow links to their targets: the
        // depth and entry limits bound a link back into its own tree, and
        // the canonical target dedups a font reached by two paths.
        let (path, is_dir, is_file) = if kind.is_symlink() {
            let Ok(target) = std::fs::canonicalize(&name) else {
                continue;
            };
            let (is_dir, is_file) = (target.is_dir(), target.is_file());
            (target, is_dir, is_file)
        } else {
            (name.clone(), kind.is_dir(), kind.is_file())
        };
        if is_dir {
            collect_font_files(&path, depth - 1, budget, files);
        } else if is_file && (is_font_file(&name) || is_font_file(&path)) {
            files.insert(path);
        }
    }
}

fn is_font_file(path: &Path) -> bool {
    path.extension()
        .and_then(|extension| extension.to_str())
        .is_some_and(|extension| {
            matches!(
                extension.to_ascii_lowercase().as_str(),
                "ttf" | "otf" | "ttc" | "otc"
            )
        })
}

/// The first face of the font file at `path` whose `cmap` maps every char of
/// `required` and, when `family` (lowercased) is given, whose `name` table
/// records that family. Reads only the collection header, each face's table
/// directory and the tables checked, not the whole file: glyph outlines are
/// most of a CJK font, so a miss costs a few KiB instead of megabytes. Counts
/// the faces it checks.
fn covering_face(
    path: &Path,
    family: Option<&str>,
    required: &[char],
    faces: &mut usize,
) -> Option<u32> {
    let mut file = File::open(path).ok()?;
    let len = file.metadata().ok()?.len();
    let header = read_at(&mut file, len, 0, 12)?;
    let offsets: Vec<u64> = match ttf_parser::fonts_in_collection(&header) {
        Some(count) => read_at(
            &mut file,
            len,
            12,
            count.min(MAX_COLLECTION_FACES) as usize * 4,
        )?
        .chunks_exact(4)
        .map(|offset| u64::from(be_u32(offset)))
        .collect(),
        None => vec![0],
    };
    // Collection faces often share one cmap; check each table once.
    let mut checked: Option<((u64, u64), bool)> = None;
    for (face, offset) in offsets.into_iter().enumerate() {
        let Some(tables) = table_directory(&mut file, len, offset) else {
            continue;
        };
        let Some(cmap) = table_location(&tables, b"cmap") else {
            continue;
        };
        *faces += 1;
        if let Some(family) = family {
            let named = table_location(&tables, b"name")
                .and_then(|(at, size)| read_at(&mut file, len, at, size as usize))
                .is_some_and(|data| names_family(&data, family));
            if !named {
                continue;
            }
        }
        let covers = match checked {
            Some((location, covers)) if location == cmap => covers,
            _ => read_at(&mut file, len, cmap.0, cmap.1 as usize)
                .is_some_and(|data| cmap_covers(&data, required)),
        };
        if covers {
            return u32::try_from(face).ok();
        }
        checked = Some((cmap, covers));
    }
    None
}

/// The table records of the face whose table directory starts at
/// `face_offset`, 16 bytes each.
fn table_directory(file: &mut File, len: u64, face_offset: u64) -> Option<Vec<u8>> {
    let directory = read_at(file, len, face_offset, 12)?;
    let tables = usize::from(u16::from_be_bytes([directory[4], directory[5]]));
    read_at(file, len, face_offset + 12, tables * 16)
}

/// Offset and length of the table `tag` among `records`. Table offsets count
/// from the start of the file, in collections too.
fn table_location(records: &[u8], tag: &[u8; 4]) -> Option<(u64, u64)> {
    records
        .chunks_exact(16)
        .find(|record| &record[..4] == tag)
        .map(|record| {
            (
                be_u32(&record[8..12]).into(),
                be_u32(&record[12..16]).into(),
            )
        })
}

/// Whether a `name` table records `family` (lowercased) as a family name
/// (name ID 1) in any language. A face carries localized family names, such
/// as both "MS Gothic" and "ＭＳ ゴシック", and GDI accepts either.
fn names_family(name: &[u8], family: &str) -> bool {
    let Some(table) = ttf_parser::name::Table::parse(name) else {
        return false;
    };
    table.names.into_iter().any(|record| {
        record.name_id == ttf_parser::name_id::FAMILY
            && record
                .to_string()
                .is_some_and(|name| name.trim().to_lowercase() == family)
    })
}

/// Mirrors ttf-parser's `Face::glyph_index`, which ab_glyph's `glyph_id`
/// uses: a char is covered when any Unicode subtable maps it to a glyph.
fn cmap_covers(cmap: &[u8], required: &[char]) -> bool {
    let Some(table) = ttf_parser::cmap::Table::parse(cmap) else {
        return false;
    };
    required.iter().all(|&ch| {
        table
            .subtables
            .into_iter()
            .any(|subtable| subtable.is_unicode() && subtable.glyph_index(u32::from(ch)).is_some())
    })
}

/// `len` bytes at `offset`, refusing ranges past the end of the file so a
/// corrupt length cannot request a huge allocation.
fn read_at(file: &mut File, file_len: u64, offset: u64, len: usize) -> Option<Vec<u8>> {
    if offset.checked_add(len as u64)? > file_len {
        return None;
    }
    file.seek(SeekFrom::Start(offset)).ok()?;
    let mut bytes = vec![0; len];
    file.read_exact(&mut bytes).ok()?;
    Some(bytes)
}

fn be_u32(bytes: &[u8]) -> u32 {
    u32::from_be_bytes([bytes[0], bytes[1], bytes[2], bytes[3]])
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn invalid_font_and_face_fail_explicitly() {
        assert!(FontProvider::load_bytes(vec![0; 64], 0).is_err());
        assert!(FontProvider::load_file("/nonexistent/ethornell-font.ttf", 0).is_err());
    }

    #[test]
    fn coverage_reports_missing_without_process_cache() {
        let font = FontProvider::load_system_cjk_font().expect("system CJK font");
        assert!(FontProvider::missing_glyphs(&font, "A日本語中文\n").is_empty());
        assert_eq!(
            FontProvider::missing_glyphs(&font, "\u{10ffff}\u{10ffff}"),
            vec!['\u{10ffff}']
        );
        assert!(FontProvider::require_coverage(&font, "\u{10ffff}").is_err());
        assert!(FontProvider::require_coverage(&font, "日本語").is_ok());
    }

    /// A Nix-style tree: fonts and font directories linked from a store
    /// outside the root, plus a link back into the root itself.
    #[cfg(unix)]
    #[test]
    fn font_scan_follows_links_and_dedups_targets() {
        use std::os::unix::fs::symlink;
        let root = std::env::temp_dir().join(format!("ethornell-font-scan-{}", std::process::id()));
        let _ = std::fs::remove_dir_all(&root);
        let store = root.join("store");
        let fonts = root.join("fonts");
        std::fs::create_dir_all(store.join("wqy")).unwrap();
        std::fs::create_dir_all(fonts.join("latin")).unwrap();
        for file in [
            fonts.join("latin/Arimo.ttf"),
            fonts.join("readme.txt"),
            store.join("DroidSansFallback.ttf"),
            store.join("wqy/wqy-zenhei.ttc"),
        ] {
            std::fs::write(file, b"").unwrap();
        }
        symlink(
            store.join("DroidSansFallback.ttf"),
            fonts.join("DroidSansFallback.ttf"),
        )
        .unwrap();
        symlink(store.join("wqy"), fonts.join("wqy")).unwrap();
        symlink(&fonts, fonts.join("loop")).unwrap();

        let files = discovered_font_files(&[std::fs::canonicalize(&fonts).unwrap()]);
        let _ = std::fs::remove_dir_all(&root);
        let names: Vec<_> = files
            .iter()
            .map(|path| path.strip_prefix(std::fs::canonicalize(&root).unwrap_or(root.clone())))
            .map(|path| path.unwrap().to_string_lossy().into_owned())
            .collect();
        assert_eq!(
            names,
            [
                "fonts/latin/Arimo.ttf",
                "store/DroidSansFallback.ttf",
                "store/wqy/wqy-zenhei.ttc"
            ]
        );
    }

    /// The header-and-cmap check must pick the same face as parsing the
    /// whole font with ab_glyph would.
    #[test]
    fn cmap_check_agrees_with_parsed_faces() {
        let files: Vec<PathBuf> = super::super::SYSTEM_CJK_FONT_CANDIDATES
            .iter()
            .filter_map(|path| std::fs::canonicalize(path).ok())
            .chain(
                discovered_font_files(&system_font_roots())
                    .into_iter()
                    .take(64),
            )
            .collect();
        assert!(!files.is_empty(), "no system font files to compare");
        for required in ["A日本語中文", "A日本語中文简体繁體", "Ag", "\u{0378}"] {
            for path in &files {
                let bytes = std::fs::read(path).unwrap();
                let parsed = (0..MAX_COLLECTION_FACES)
                    .map_while(|face| {
                        Some((face, FontProvider::load_bytes(bytes.clone(), face).ok()?))
                    })
                    .find(|(_, font)| FontProvider::missing_glyphs(font, required).is_empty())
                    .map(|(face, _)| face);
                let checked = covering_face(path, None, &required_chars(required), &mut 0);
                assert_eq!(checked, parsed, "{} for {required:?}", path.display());
            }
        }
    }

    /// A `name` table (format 0) of Windows Unicode records.
    fn name_table(records: &[(u16, u16, &str)]) -> Vec<u8> {
        let strings: Vec<Vec<u8>> = records
            .iter()
            .map(|(_, _, text)| text.encode_utf16().flat_map(u16::to_be_bytes).collect())
            .collect();
        let count = records.len() as u16;
        let mut table = Vec::new();
        for value in [0, count, 6 + 12 * count] {
            table.extend(value.to_be_bytes());
        }
        let mut offset = 0u16;
        for ((language, name_id, _), bytes) in records.iter().zip(&strings) {
            for value in [3, 1, *language, *name_id, bytes.len() as u16, offset] {
                table.extend(value.to_be_bytes());
            }
            offset += bytes.len() as u16;
        }
        strings.iter().for_each(|bytes| table.extend(bytes));
        table
    }

    #[test]
    fn family_matches_any_localized_name_without_case() {
        let table = name_table(&[
            (0x409, 1, "MS Gothic"),
            (0x411, 1, "ＭＳ ゴシック"),
            (0x409, 4, "MS Mincho"),
        ]);
        assert!(names_family(&table, &"ＭＳ ゴシック".to_lowercase()));
        assert!(names_family(&table, &"MS GOTHIC".to_lowercase()));
        // Only family names (ID 1) count, not the full name (ID 4).
        assert!(!names_family(&table, "ms mincho"));
        assert!(!names_family(&table, "ms"));
        assert!(!names_family(&[0, 0], "ms gothic"));
    }

    #[test]
    fn installed_face_is_found_by_family_name() {
        let path = super::super::SYSTEM_CJK_FONT_CANDIDATES
            .iter()
            .filter_map(|path| std::fs::canonicalize(path).ok())
            .chain(discovered_font_files(&system_font_roots()))
            .next()
            .expect("system font file");
        let bytes = std::fs::read(&path).unwrap();
        let face = ttf_parser::Face::parse(&bytes, 0).unwrap();
        let family = face
            .names()
            .into_iter()
            .find(|name| name.name_id == ttf_parser::name_id::FAMILY)
            .and_then(|name| name.to_string())
            .expect("family name");
        let mut search = SystemFontSearch::new("A");
        assert!(search.first_match(Some(&family.to_lowercase())).is_some());
        assert!(search.first_match(Some("ethornell no such face")).is_none());
        // A missing face falls back to the coverage search.
        assert!(
            FontProvider::load_system_for_text_preferring(&["Ethornell No Such Face"], "A").is_ok()
        );
    }

    #[test]
    fn truncated_or_corrupt_font_headers_are_rejected() {
        let dir = std::env::temp_dir().join(format!("ethornell-font-bad-{}", std::process::id()));
        std::fs::create_dir_all(&dir).unwrap();
        let mut huge_collection = b"ttcf\0\x01\0\0\xff\xff\xff\xff".to_vec();
        huge_collection.extend([0; 8]);
        let mut cmap_past_end = vec![0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0];
        cmap_past_end.extend(b"cmap\0\0\0\0\0\0\0\x1c\x7f\xff\xff\xff");
        for (name, bytes) in [
            ("empty.ttf", Vec::new()),
            ("short.ttf", b"\0\x01\0\0".to_vec()),
            ("collection.ttc", huge_collection),
            ("cmap.ttf", cmap_past_end),
        ] {
            let path = dir.join(name);
            std::fs::write(&path, bytes).unwrap();
            assert_eq!(covering_face(&path, None, &['A'], &mut 0), None, "{name}");
        }
        let _ = std::fs::remove_dir_all(&dir);
    }

    #[test]
    fn explicit_ttc_face_and_invalid_index_are_respected() {
        let path = super::super::SYSTEM_CJK_FONT_CANDIDATES
            .iter()
            .find(|path| Path::new(path).is_file())
            .expect("system font file");
        let bytes = std::fs::read(path).unwrap();
        assert!(FontProvider::load_bytes(bytes.clone(), 0).is_ok());
        assert!(FontProvider::load_bytes(bytes, u32::MAX).is_err());
    }
}
