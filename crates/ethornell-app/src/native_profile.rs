//! Read native configuration as bounded PE data; never load or execute it.
//! Allocation sizes, SE bank bounds and BP pointer tags are independent.
use super::*;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Capabilities {
    version: NativeVersion,
    regions: Option<(u32, u32)>,
    sound: Option<usize>,
    display_modes: Option<[(i32,i32);16]>,
    modern_tags: bool,
    fused_bp: bool,
    legacy_graph_extensions: bool,
    legacy_utf8_strings: bool,
}

/// BGI's declared compatibility line is a baseline, not a complete native ABI.
/// 1.653.3, 1.686.4 and 1.687 all declare 1.72 but differ in their dispatchers.
/// Keep the executable version and observed layout alongside that declaration.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
struct NativeVersion {
    engine: Option<[u16; 4]>,
    compatibility: Option<[u16; 2]>,
}

fn unsupported(message: impl Into<String>) -> EthornellError {
    EthornellError::UnsupportedFormat(format!("native BGI profile: {}", message.into()))
}

fn version_parts<const N: usize>(text: &str) -> Option<[u16; N]> {
    let mut result = [0; N];
    let mut count = 0;
    for (i, part) in text.trim().split('.').enumerate() {
        if i >= N || part.is_empty() || !part.bytes().all(|c| c.is_ascii_digit()) {
            return None;
        }
        result[i] = part.parse().ok()?;
        count += 1;
    }
    (count >= 2).then_some(result)
}

impl NativeVersion {
    fn read(fixed: Option<[u16; 4]>, text: Option<&str>) -> Result<Self> {
        let mut result = Self { engine: fixed, compatibility: None };
        let Some(text) = text else { return Ok(result) };
        let text = text.trim();
        // Older resources use "1.69" or "ver 1.69 ( build : ... )";
        // current resources use "Version : ... - Compatibility : ...".
        // Himanatsu 1.571.1 and PureConnect 1.610.1 put Compatibility on
        // the next line; later builds use a hyphen in the same field.
        let (engine, compatibility) = if let Some((engine, compatibility)) = text.split_once("Compatibility") {
            let compatibility = compatibility.trim_start().strip_prefix(':')
                .ok_or_else(|| unsupported("malformed compatibility declaration"))?.trim();
            let engine = engine.trim_end().trim_end_matches('-').trim_end();
            (engine, Some(compatibility))
        } else {
            (text, None)
        };
        let engine = engine.strip_prefix("Version : ")
            .or_else(|| engine.strip_prefix("ver ")).unwrap_or(engine)
            .split_whitespace().next().unwrap_or_default();
        if let Some(parsed) = version_parts::<4>(engine) {
            // Old "build" suffixes are separate from the displayed version.
            let agrees = fixed.is_none_or(|fixed| fixed == parsed
                || (compatibility.is_none() && fixed[..2] == parsed[..2]));
            if !agrees { return Err(unsupported("fixed and string file versions disagree")); }
            result.engine = fixed.or(Some(parsed));
        } else if compatibility.is_some() {
            return Err(unsupported("malformed engine version"));
        }
        if let Some(text) = compatibility {
            let parsed = version_parts::<2>(text)
                .ok_or_else(|| unsupported("malformed compatibility version"))?;
            if !matches!(parsed, [1, 69] | [1, 71] | [1, 72]) {
                return Err(unsupported(format!("unrecognized Compatibility {text}")));
            }
            result.compatibility = Some(parsed);
        }
        Ok(result)
    }

    fn fused(self, width: usize, complete: bool, compact: bool) -> Result<bool> {
        // NSM 1.686.4 omits D4..DF, unlike Triple Pairing 1.687. Both have
        // the x64 E2/E8..EA/F0..F2/F4/F5/F7..FB fused family and the same
        // audited native contracts. No import name or game-name whitelist.
        let audited = matches!(self.engine, Some([1, 686, 4, 0] | [1, 687, 0, 0]));
        if audited && (width != 8 || !compact || self.compatibility.is_some_and(|c| c != [1, 72])) {
            return Err(unsupported("declared 1.686.4/1.687 conflicts with native dispatch layout"));
        }
        if width == 8 && compact && !complete && !audited {
            return Err(unsupported("unreviewed compact native ABI build"));
        }
        let fused = complete || (audited && compact);
        if fused && self.compatibility.is_some_and(|c| c != [1, 72]) {
            return Err(unsupported("fused dispatch conflicts with declared compatibility"));
        }
        Ok(fused)
    }
}

struct Executable<'a> {
    pe: pelite::PeFile<'a>,
    base: u64,
    width: usize,
}

impl<'a> Executable<'a> {
    fn code(&self, va: u64) -> Option<&'a [u8]> {
        let rva = u32::try_from(va.checked_sub(self.base)?).ok()?;
        let section = self.pe.section_headers().iter().find(|s| {
            s.Characteristics & 0x20000000 != 0
                && rva >= s.VirtualAddress
                && rva - s.VirtualAddress < s.SizeOfRawData
        })?;
        self.pe
            .get_section_bytes(section)
            .ok()?
            .get((rva - section.VirtualAddress) as usize..)
    }

    fn tables(&self, required: &[usize], absent: &[usize]) -> Vec<Vec<u64>> {
        let mut found = Vec::new();
        for section in self.pe.section_headers() {
            let Ok(bytes) = self.pe.get_section_bytes(section) else {
                continue;
            };
            let size = 256 * self.width;
            for start in (0..bytes.len().saturating_sub(size).saturating_add(1)).step_by(self.width)
            {
                let Some(table) = bytes.get(start..start + size) else {
                    continue;
                };
                let pointer = |index: usize| {
                    table[index * self.width..(index + 1) * self.width]
                        .iter()
                        .enumerate()
                        .fold(0u64, |v, (i, b)| v | (u64::from(*b) << (i * 8)))
                };
                if absent.iter().all(|&i| pointer(i) == 0)
                    && required.iter().all(|&i| self.code(pointer(i)).is_some())
                {
                    found.push((0..256).map(pointer).collect());
                    if found.len() > 8 {
                        return Vec::new();
                    }
                }
            }
        }
        found
    }

    fn calls(&self, function: u64) -> Vec<u64> {
        let Some(bytes) = self.code(function) else {
            return Vec::new();
        };
        bytes[..bytes.len().min(96)]
            .windows(5)
            .enumerate()
            .filter_map(|(offset, bytes)| {
                if bytes[0] != 0xe8 {
                    return None;
                }
                let displacement = i32::from_le_bytes(bytes[1..5].try_into().ok()?);
                let address = function
                    .checked_add(offset as u64 + 5)?
                    .checked_add_signed(i64::from(displacement))?;
                self.code(address).map(|_| address)
            })
            .collect()
    }

    fn sound_bound(&self, function: u64) -> Option<usize> {
        let calls = self.calls(function);
        // A0:09 pops volume and channel, checks volume, then checks SE index.
        // FD 49B0B0, Lovepical 4E5F30, Dokai 1400E3210 use this order.
        if calls.len() < 4 || calls[0] != calls[1] {
            return None;
        }
        let bytes = self.code(calls[3])?;
        let mut bounds = bytes[..bytes.len().min(40)].windows(7).filter_map(|s| {
            match s {
                [0x81, 0xf9, a, b, c, d, 0x73] => {
                    Some(u32::from_le_bytes([*a, *b, *c, *d]) as usize)
                }
                [0x3d, a, b, c, d, 0x72, _] => Some(u32::from_le_bytes([*a, *b, *c, *d]) as usize),
                [0x83, 0xf8, bound, 0x72, ..] if *bound < 128 => Some(*bound as usize),
                _ => None,
            }
            .filter(|n| matches!(n, 64 | 128))
        });
        let bound = bounds.next()?;
        bounds.all(|other| other == bound).then_some(bound)
    }

    fn follow_thunks(&self, mut address: u64) -> Option<&'a [u8]> {
        for _ in 0..4 {
            let bytes = self.code(address)?;
            if bytes.first() != Some(&0xe9) {
                return Some(bytes);
            }
            let displacement = i32::from_le_bytes(bytes.get(1..5)?.try_into().ok()?);
            address = address
                .checked_add(5)?
                .checked_add_signed(i64::from(displacement))?;
        }
        None
    }

    fn legacy_graph_extensions(&self) -> bool {
        if self.width != 4 {
            return false;
        }
        // Confirm the old extended Graph91 table's two actual algorithms,
        // independently of IPL/title bytes (translations can change those).
        self.tables(&[0, 3, 6, 9, 10, 11, 12, 13, 14, 15], &[1, 2, 4, 5, 7, 8])
            .iter()
            .any(|table| {
                let mode = self.calls(table[9]);
                let option = self.calls(table[10]);
                if mode.len() < 3
                    || option.len() < 4
                    || option[0] != option[1]
                    || option[0] != mode[0]
                {
                    return false;
                }
                let (Some(mode), Some(option)) =
                    (self.follow_thunks(mode[1]), self.follow_thunks(option[2]))
                else {
                    return false;
                };
                let mode = &mode[..mode.len().min(32)];
                let option = &option[..option.len().min(80)];
                // 464000 and 4631B0 implement the same unsigned mode<=1
                // policy with separate-return and shared-return codegen.
                // 408840 / 407F30 accept exactly these two keys, return 25
                // otherwise, and store both supplied values unchanged.
                let mode_guard = (mode.starts_with(&[0x83, 0xf9, 1, 0x77])
                    && mode.windows(3).any(|s| s == [0x33, 0xc0, 0xc3]))
                    || mode.starts_with(&[0x33, 0xc0, 0x83, 0xf9, 1, 0x77]);
                mode_guard
                    && mode.windows(2).any(|s| s == [0x89, 0x0d])
                    && mode.windows(6).any(|s| s == [0xb8, 1, 0, 0, 0, 0xc3])
                    && option.windows(6).any(|s| s == [0x81, 0xf9, 0, 0, 0x22, 1])
                    && option
                        .windows(6)
                        .any(|s| s == [0x81, 0xf9, 0, 0x16, 0x11, 0x80])
                    && option.windows(5).any(|s| s == [0xb8, 25, 0, 0, 0])
                    && option.windows(3).any(|s| s == [0x33, 0xc0, 0xc3])
                    && option.windows(2).filter(|s| *s == [0x89, 0x15]).count() >= 2
            })
    }

    fn legacy_utf8_strings(&self) -> bool {
        if self.width != 4 { return false; }
        // ESchoolLife 1.683: 81:00 -> 494970 -> 49E510. Identify the
        // original CP932/UTF-8 selector independently of translated IPL/title
        // bytes and of unrelated Graph91 additions in the same release.
        self.tables(
            &[0, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0x20, 0x21, 0x27, 0x28, 0x30, 0x60, 0x62, 0x68],
            &[5, 0x12, 0x13, 0x15, 0x1a, 0x1c, 0x22, 0x23, 0x24, 0x25, 0x26],
        ).iter().any(|table| {
            let calls = self.calls(table[0]);
            let Some(code) = calls.get(1).and_then(|&call| self.follow_thunks(call)) else { return false };
            let code = &code[..code.len().min(80)];
            let stores: Vec<(u32, u32)> = code.windows(10).filter_map(|bytes| {
                (bytes[..2] == [0xc7, 0x05]).then(|| (
                    u32::from_le_bytes(bytes[2..6].try_into().unwrap()),
                    u32::from_le_bytes(bytes[6..10].try_into().unwrap()),
                ))
            }).collect();
            stores.iter().any(|&(address, value)| value == 65001 && stores.contains(&(address, 932)))
                && stores.iter().any(|&(address, value)| value == 1 && stores.contains(&(address, 0)))
                && code.windows(3).any(|bytes| bytes == [0x83, 0xe9, 1])
                && code.windows(3).any(|bytes| bytes == [0x33, 0xc0, 0xc3])
                && code.windows(5).any(|bytes| bytes == [0xb8, 1, 0, 0, 0])
        })
    }

    fn display_modes(&self) -> Option<[(i32,i32);16]> {
        // 1.687 heights[16] at 1401CB780, widths[16] at 1401CB7C0.
        // Discover their values rather than treating later size IDs as aliases
        // for old modes. Both prefixes and all sixteen dimensions must agree.
        for section in self.pe.section_headers() {
            if section.Characteristics & 0x20000000 != 0 {continue;}
            let Ok(bytes)=self.pe.get_section_bytes(section) else {continue};
            for data in bytes.windows(128).step_by(4) {
                let word=|i:usize|i32::from_le_bytes(data[i*4..i*4+4].try_into().unwrap());
                if (0..5).any(|i|word(i)!=[240,480,600,768,576][i] || word(16+i)!=[320,640,800,1024,1024][i]) {continue;}
                let sizes=std::array::from_fn(|i|(word(16+i),word(i)));
                if sizes.iter().all(|&(w,h)|(1..=16384).contains(&w)&&(1..=16384).contains(&h)) {return Some(sizes);}
            }
        }
        None
    }

    fn regions(&self) -> Option<(u32, u32)> {
        let mut found = None;
        for section in self
            .pe
            .section_headers()
            .iter()
            .filter(|s| s.Characteristics & 0x20000000 != 0)
        {
            let Ok(bytes) = self.pe.get_section_bytes(section) else {
                continue;
            };
            for s in bytes.windows(48) {
                // x86: push data; push code; push operand-ring; two stack LEAs;
                // call initialization. x64: stack data, r9d code, r8d ring.
                let pair = if self.width == 4
                    && s[0] == 0x68
                    && s[5] == 0x68
                    && s[10..15] == [0x68, 0, 0x10, 0, 0]
                    && s[15..17] == [0x8d, 0x95]
                    && s[21..23] == [0x8d, 0x8d]
                    && s[27] == 0xe8
                {
                    Some((
                        u32::from_le_bytes(s[6..10].try_into().ok()?),
                        u32::from_le_bytes(s[1..5].try_into().ok()?),
                    ))
                } else if self.width == 8
                    && s[..4] == [0xc7, 0x44, 0x24, 0x20]
                    && s[8..10] == [0x41, 0xb9]
                    && s[14..20] == [0x41, 0xb8, 0, 0x10, 0, 0]
                    && s[20..23] == [0x48, 0x8d, 0x54]
                    && s[25..29] == [0x48, 0x8d, 0x8c, 0x24]
                    && s[33] == 0xe8
                {
                    Some((
                        u32::from_le_bytes(s[10..14].try_into().ok()?),
                        u32::from_le_bytes(s[4..8].try_into().ok()?),
                    ))
                } else {
                    None
                };
                if let Some(pair) =
                    pair.filter(|&(code, data)| code == 0x800000 && data == 0x400000)
                {
                    if found.is_some_and(|old| old != pair) {
                        return None;
                    }
                    found = Some(pair);
                }
            }
        }
        found
    }
}

fn inspect_image(bytes: &[u8]) -> Result<Option<Capabilities>> {
    let Ok(pe) = pelite::PeFile::from_bytes(bytes) else { return Ok(None) };
    if pe.section_headers().as_slice().len() > 96
        || pe
            .section_headers()
            .iter()
            .map(|s| u64::from(s.SizeOfRawData))
            .sum::<u64>()
            > bytes.len() as u64 * 2
    {
        return Ok(None);
    }
    let Ok(resources) = pe.resources() else { return Ok(None) };
    let Ok(version) = resources.version_info() else { return Ok(None) };
    if !version
        .translation()
        .iter()
        .any(|&lang| version.value(lang, "InternalName").as_deref() == Some("Ethornell"))
    {
        return Ok(None);
    }
    let fixed = version.fixed().map(|fixed| {
        let v = fixed.dwFileVersion;
        [v.Major, v.Minor, v.Patch, v.Build]
    });
    let mut declared = None;
    for &lang in version.translation() {
        if let Some(text) = version.value(lang, "FileVersion") {
            let parsed = NativeVersion::read(fixed, Some(&text))?;
            if declared.is_some_and(|old| old != parsed) {
                return Err(unsupported("conflicting localized version declarations"));
            }
            declared = Some(parsed);
        }
    }
    let declared = declared.unwrap_or(NativeVersion::read(fixed, None)?);
    inspect_layout(pe, declared).map(Some)
}

fn inspect_layout(pe: pelite::PeFile<'_>, version: NativeVersion) -> Result<Capabilities> {
    let (base, width) = match pe.optional_header() {
        pelite::Wrap::T32(h) => (u64::from(h.ImageBase), 4),
        pelite::Wrap::T64(h) => (h.ImageBase, 8),
    };
    let exe = Executable { pe, base, width };
    let tables = exe.tables(
        &[
            0, 1, 2, 4, 5, 6, 8, 9, 10, 11, 12, 16, 17, 20, 21, 22, 23, 128, 144, 145, 146,
            160, 176, 192, 255,
        ],
        &[7, 0x3d, 0x41, 0x72, 0x76],
    );
    // Early 1.69 (KiraKira table 484490) predates the 81 dispatcher.
    // Later compatibility-declaring builds still require that dispatcher.
    let tables: Vec<_> = tables.into_iter().filter(|t|
        version.compatibility.is_none() || exe.code(t[0x81]).is_some()).collect();
    let mut tags = tables.iter().filter_map(|table| {
        let code = exe.code(table[4])?;
        let code = &code[..code.len().min(40)];
        // Native BP04: OR EAX,08000000/20000000, or BTS EAX,29 on x64.
        let old = code.windows(5).any(|s| s == [0x0d, 0, 0, 0, 8])
            // KiraKira BP04 at 43E670 uses the early 24-bit 0x12 frame
            // tag. Retain this family's existing canonical VM handling;
            // it must not be classified as the later 28-bit pointer ABI.
            || (version.compatibility.is_none()
                && matches!(version.engine, Some([1,69,_,_]))
                && code.windows(5).any(|s| s == [0x0d, 0, 0, 0, 0x12]));
        let modern = code.windows(5).any(|s| s == [0x0d, 0, 0, 0, 0x20])
            || code.windows(4).any(|s| s == [0x0f, 0xba, 0xe8, 29]);
        (old != modern).then_some(modern)
    });
    let modern_tags = tags.next().ok_or_else(|| unsupported("unrecognized BP pointer layout"))?;
    if tags.any(|other| other != modern_tags) {
        return Err(unsupported("conflicting BP pointer layouts"));
    }
    let sound_tables = exe.tables(
        &[
            0, 8, 9, 16, 17, 18, 20, 21, 22, 23, 24, 25, 28, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            44, 47, 128, 129, 132, 133, 134, 192,
        ],
        &[1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 19],
    );
    let mut sound = sound_tables
        .iter()
        .filter_map(|table| exe.sound_bound(table[9]));
    let bound = sound.next();
    if sound.any(|other| Some(other) != bound) {
        return Err(unsupported("conflicting sound channel bounds"));
    }
    let complete = tables.iter().any(|t| [0xd4,0xd5,0xd6,0xd7,0xd8,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xf4,0xf5,0xfa,0xfb].iter().all(|&i| exe.code(t[i]).is_some()));
    let compact = tables.iter().any(|t| [0xe2,0xe8,0xe9,0xea,0xf0,0xf1,0xf2,0xf4,0xf5,0xf7,0xf8,0xf9,0xfa,0xfb].iter().all(|&i| exe.code(t[i]).is_some()));
    let fused_bp = version.fused(width, complete, compact)?;
    if fused_bp && !modern_tags {
        return Err(unsupported("fused dispatch conflicts with BP pointer layout"));
    }
    Ok(Capabilities {
        version,
        regions: exe.regions(),
        sound: bound,
        display_modes: exe.display_modes(),
        modern_tags,
        fused_bp,
        legacy_graph_extensions: exe.legacy_graph_extensions(),
        legacy_utf8_strings: exe.legacy_utf8_strings(),
    })
}

// The 32-bit launcher in Triple Pairing 1.687 embeds the complete x64
// Ethornell PE at file offset 0x48000. Inspect it as data, never load it.
// Require the same version-resource identity and verified dispatch shapes
// as a standalone engine; an arbitrary MZ sequence is not a native profile.
fn inspect(bytes: &[u8]) -> Result<Option<Capabilities>> {
    // A launcher may carry Ethornell's version resource but no VM. Give its
    // bounded embedded runtime a chance before reporting the outer error.
    let (mut detected, outer_error) = match inspect_image(bytes) {
        Ok(caps) => (caps, None),
        Err(error) => (None, Some(error)),
    };
    let mut candidates = 0;
    for (offset, magic) in bytes.windows(2).enumerate().skip(1) {
        if magic != b"MZ" || offset % 16 != 0 { continue; }
        let nested = &bytes[offset..];
        let Some(pe_offset) = nested.get(0x3c..0x40).and_then(|s| <[u8;4]>::try_from(s).ok()).map(u32::from_le_bytes) else { continue };
        if nested.get(pe_offset as usize..).and_then(|s| s.get(..4)) != Some(b"PE\0\0") { continue; }
        candidates += 1;
        if candidates > 8 { return Err(unsupported("too many embedded PE candidates")); }
        let Some(caps) = inspect_image(nested)? else { continue };
        if detected.is_some_and(|old| old != caps) { return Err(unsupported("conflicting embedded runtime profiles")); }
        detected = Some(caps);
    }
    if detected.is_none() && let Some(error) = outer_error { return Err(error); }
    Ok(detected)
}

pub(super) fn apply(manager: &ResourceManager, profile: &mut NativeRuntimeProfile) -> Result<()> {
    let policy = file_policy::active();
    let mut names = if let Some(policy) = &policy {
        policy
            .enumerate("bgi-resource/*.exe", false, 16, false)
            .unwrap_or_default()
    } else {
        std::fs::read_dir(&manager.archives().root)
            .into_iter()
            .flatten()
            .flatten()
            .filter_map(|e| e.file_name().into_string().ok())
            .filter(|name| name.to_ascii_lowercase().ends_with(".exe"))
            .take(16)
            .collect()
    };
    names.retain(|name| {
        let name = name.to_ascii_lowercase();
        !(name.contains("uninst")
            || name.contains("setup")
            || name == "bhvc.exe"
            || name == "bgiforinstalling.exe")
    });
    names.sort();
    let mut detected = None;
    let mut remaining = 64 * 1024 * 1024u64;
    for name in names {
        let bytes = if let Some(policy) = &policy {
            policy
                .read_resource(&format!("bgi-resource/{name}"), remaining)
                .ok()
        } else {
            let path = manager.archives().root.join(&name);
            use std::io::Read;
            std::fs::File::open(path).ok().and_then(|file| {
                if !file
                    .metadata()
                    .ok()
                    .is_some_and(|m| m.is_file() && m.len() <= remaining)
                {
                    return None;
                }
                let mut bytes = Vec::new();
                file.take(remaining + 1).read_to_end(&mut bytes).ok()?;
                (bytes.len() as u64 <= remaining).then_some(bytes)
            })
        };
        let Some(bytes) = bytes else { continue };
        remaining = remaining.saturating_sub(bytes.len() as u64);
        let Some(caps) = inspect(&bytes)? else {
            continue;
        };
        if detected.is_some_and(|previous| previous != caps) {
            return Err(unsupported("conflicting native executable configurations"));
        }
        detected = Some(caps);
    }
    let Some(caps) = detected else { return Ok(()) };
    if let Some((code, data)) = caps.regions {
        profile.main_code_bytes = code;
        profile.main_data_bytes = data;
    }
    if let Some(channels) = caps.sound {
        profile.se_channels = channels;
    }
    profile.modern_pointer_tags = caps.modern_tags;
    profile.display_modes = caps.display_modes;
    if caps.legacy_graph_extensions && profile.abi == ethornell_vm::NativeAbiProfile::Tayutama {
        profile.abi = ethornell_vm::NativeAbiProfile::Anipani;
    }
    if caps.legacy_utf8_strings && profile.abi == ethornell_vm::NativeAbiProfile::Tayutama {
        profile.abi = ethornell_vm::NativeAbiProfile::Amachoco;
    }
    if caps.modern_tags {
        profile.abi = if caps.fused_bp { ethornell_vm::NativeAbiProfile::ModernFused } else { ethornell_vm::NativeAbiProfile::Modern };
        profile.modern_gradient_blur = true;
    }
    tracing::info!(?caps, "read native runtime configuration");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn rejects_non_pe_and_truncated_executables() {
        assert!(inspect(b"MZ").unwrap().is_none());
        assert!(inspect(&[0; 1024]).unwrap().is_none());
    }

    #[test]
    fn compatibility_baseline_does_not_collapse_distinct_builds() {
        let himanatsu = NativeVersion::read(Some([1,571,1,0]), Some("Version : 1.571.1\r\nCompatibility : 1.71")).unwrap();
        assert_eq!(himanatsu.compatibility, Some([1,71]));
        assert!(!himanatsu.fused(4, false, false).unwrap());
        let pure = NativeVersion::read(Some([1,610,1,0]), Some("Version : 1.610.1\r\nCompatibility : 1.72")).unwrap();
        assert_eq!(pure.compatibility, Some([1,72]));
        let older = NativeVersion::read(Some([1,653,3,0]), Some("Version : 1.653.3 - Compatibility : 1.72")).unwrap();
        let nsm = NativeVersion::read(Some([1,686,4,0]), Some("Version : 1.686.4 - Compatibility : 1.72")).unwrap();
        let triple = NativeVersion::read(Some([1,687,0,0]), Some("Version : 1.687 - Compatibility : 1.72")).unwrap();
        assert_eq!(older.compatibility, nsm.compatibility);
        assert!(!older.fused(4, false, false).unwrap());
        assert!(nsm.fused(8, false, true).unwrap());
        assert!(triple.fused(8, true, true).unwrap());
        assert!(nsm.fused(4, false, true).is_err());
        assert!(nsm.fused(8, false, false).is_err());
        assert!(NativeVersion::read(Some([1,687,0,0]), Some("Version : 1.686.4 - Compatibility : 1.72")).is_err());
        assert!(NativeVersion::read(None, Some("Version : 1.687 - Compatibility : 1.73")).is_err());
        assert!(NativeVersion::read(None, Some("Version : 1.687 - Compatibility : 1.72.0")).is_err());
        assert_eq!(NativeVersion::read(Some([1,69,444,2]), Some("ver 1.69 ( build : 444.2 )")).unwrap().engine, Some([1,69,444,2]));
        assert!(!NativeVersion::default().fused(4, false, false).unwrap());
    }

    #[test]
    #[ignore = "requires ETHORNELL_NATIVE_TEST_SOURCE; reads owner executable as data only"]
    fn inspects_authorized_native_version() {
        let bytes = std::fs::read(std::env::var_os("ETHORNELL_NATIVE_TEST_SOURCE").unwrap()).unwrap();
        let caps = inspect(&bytes).unwrap().expect("native runtime configuration");
        eprintln!("{caps:?}");
        match caps.version.engine {
            Some([1,686,4,0] | [1,687,0,0]) => assert!(caps.fused_bp),
            Some([1,653,3,0] | [1,69,0,0]) => assert!(!caps.fused_bp),
            _ => (),
        }
    }

    #[test]
    #[ignore = "requires ETHORNELL_NATIVE_TEST_SOURCE pointing to an authorized ESchoolLife executable"]
    fn legacy_utf8_profile_does_not_require_original_bootstrap_bytes() {
        let bytes = std::fs::read(std::env::var_os("ETHORNELL_NATIVE_TEST_SOURCE").unwrap()).unwrap();
        let caps = inspect(&bytes).unwrap().expect("native runtime configuration");
        assert!(caps.legacy_utf8_strings);
        assert!(!caps.modern_tags);
    }
}
