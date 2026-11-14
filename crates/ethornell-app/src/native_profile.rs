//! Read native configuration as bounded PE data; never load or execute it.
//! Allocation sizes, SE bank bounds and BP pointer tags are independent.
use super::*;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Capabilities {
    regions: Option<(u32, u32)>,
    sound: Option<usize>,
    modern_tags: bool,
    legacy_graph_extensions: bool,
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

fn inspect(bytes: &[u8]) -> Option<Capabilities> {
    let pe = pelite::PeFile::from_bytes(bytes).ok()?;
    if pe.section_headers().as_slice().len() > 96
        || pe
            .section_headers()
            .iter()
            .map(|s| u64::from(s.SizeOfRawData))
            .sum::<u64>()
            > bytes.len() as u64 * 2
    {
        return None;
    }
    let version = pe.resources().ok()?.version_info().ok()?;
    if !version
        .translation()
        .iter()
        .any(|&lang| version.value(lang, "InternalName").as_deref() == Some("Ethornell"))
    {
        return None;
    }
    let (base, width) = match pe.optional_header() {
        pelite::Wrap::T32(h) => (u64::from(h.ImageBase), 4),
        pelite::Wrap::T64(h) => (h.ImageBase, 8),
    };
    let exe = Executable { pe, base, width };
    let tables = exe.tables(
        &[
            0, 1, 2, 4, 5, 6, 8, 9, 10, 11, 12, 16, 17, 20, 21, 22, 23, 128, 129, 144, 145, 146,
            160, 176, 192, 255,
        ],
        &[7, 0x3d, 0x41, 0x72, 0x76],
    );
    let mut tags = tables.iter().filter_map(|table| {
        let code = exe.code(table[4])?;
        let code = &code[..code.len().min(40)];
        // Native BP04: OR EAX,08000000/20000000, or BTS EAX,29 on x64.
        let old = code.windows(5).any(|s| s == [0x0d, 0, 0, 0, 8]);
        let modern = code.windows(5).any(|s| s == [0x0d, 0, 0, 0, 0x20])
            || code.windows(4).any(|s| s == [0x0f, 0xba, 0xe8, 29]);
        (old != modern).then_some(modern)
    });
    let modern_tags = tags.next()?;
    if tags.any(|other| other != modern_tags) {
        return None;
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
        return None;
    }
    Some(Capabilities {
        regions: exe.regions(),
        sound: bound,
        modern_tags,
        legacy_graph_extensions: exe.legacy_graph_extensions(),
    })
}

pub(super) fn apply(manager: &ResourceManager, profile: &mut NativeRuntimeProfile) {
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
        let Some(caps) = inspect(&bytes) else {
            continue;
        };
        if detected.is_some_and(|previous| previous != caps) {
            tracing::warn!(
                "conflicting native executable configurations; retaining resource profile"
            );
            return;
        }
        detected = Some(caps);
    }
    let Some(caps) = detected else { return };
    if let Some((code, data)) = caps.regions {
        profile.main_code_bytes = code;
        profile.main_data_bytes = data;
    }
    if let Some(channels) = caps.sound {
        profile.se_channels = channels;
    }
    profile.modern_pointer_tags = caps.modern_tags;
    if caps.legacy_graph_extensions && profile.abi == ethornell_vm::NativeAbiProfile::Tayutama {
        profile.abi = ethornell_vm::NativeAbiProfile::Anipani;
    }
    if caps.modern_tags {
        profile.abi = ethornell_vm::NativeAbiProfile::Modern;
        profile.modern_gradient_blur = true;
    }
    tracing::info!(?caps, "read native runtime configuration");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn rejects_non_pe_and_truncated_executables() {
        assert_eq!(inspect(b"MZ"), None);
        assert_eq!(inspect(&[0; 1024]), None);
    }
}
