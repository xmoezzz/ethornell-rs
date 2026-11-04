//! Detect the display code page from BCS text, without feeding binary code or
//! the Japanese BP framework to the detector. Explicit settings take priority.
use chardetng::EncodingDetector;
use encoding_rs::{Encoding, GBK, SHIFT_JIS};
use ethornell_archive::ResourceManager;
use std::collections::BTreeSet;

/// Display-text code page. Resource names and BP system strings remain Shift-JIS.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub enum TextEncoding {
    ShiftJis,
    Gbk,
    #[default]
    Auto,
}

impl std::str::FromStr for TextEncoding {
    type Err = String;

    fn from_str(label: &str) -> Result<Self, Self::Err> {
        match label.trim().to_ascii_lowercase().as_str() {
            "auto" => Ok(Self::Auto),
            "shift_jis" | "shift-jis" | "sjis" | "cp932" => Ok(Self::ShiftJis),
            "gbk" | "cp936" | "windows-936" => Ok(Self::Gbk),
            _ => Err(format!(
                "unsupported text encoding {label:?}; use auto, cp932 or gbk"
            )),
        }
    }
}

impl TextEncoding {
    pub(crate) fn resolve(self, manager: &ResourceManager) -> &'static Encoding {
        select(
            manager,
            Some(match self {
                Self::Auto => "auto",
                Self::ShiftJis => "cp932",
                Self::Gbk => "gbk",
            }),
        )
    }
}

const MAX_PROBES: usize = 64;
const MAX_RESOURCE_BYTES: u64 = 2 * 1024 * 1024;
const MAX_READ_BYTES: u64 = 16 * 1024 * 1024;
const MAX_SAMPLE_BYTES: usize = 64 * 1024;
const MAX_SCRIPT_SAMPLE_BYTES: usize = 8 * 1024;
const MIN_NON_ASCII_BYTES: usize = 64;

pub(crate) fn select(manager: &ResourceManager, label: Option<&str>) -> &'static Encoding {
    if let Some(label) = label {
        let encoding = match label.trim().to_ascii_lowercase().as_str() {
            "gbk" | "cp936" | "windows-936" => Some(GBK),
            "shift_jis" | "shift-jis" | "sjis" | "cp932" => Some(SHIFT_JIS),
            "" | "auto" => None,
            _ => {
                tracing::warn!(%label, "unsupported ETHORNELL_TEXT_ENCODING; detecting the text encoding");
                None
            }
        };
        if let Some(encoding) = encoding {
            tracing::info!(
                encoding = encoding.name(),
                source = "explicit",
                "game display text encoding configured"
            );
            return encoding;
        }
    }
    let mut sample = Vec::new();
    let mut probes = 0;
    let mut read_bytes = 0;
    let mut candidates = manager.list();
    // Scenario archives normally use data01xxx.arc. Other archive names are
    // supported, but images and sound must not consume the probe budget first.
    candidates.retain(|entry| {
        let name = entry.entry_name.to_ascii_lowercase();
        let archive = entry
            .archive_path
            .file_name()
            .unwrap_or_default()
            .to_string_lossy();
        !name.ends_with("._bp")
            && !archive.eq_ignore_ascii_case("sysgrp.arc")
            && !archive.eq_ignore_ascii_case("sysprg.arc")
            && !archive.eq_ignore_ascii_case("system.arc")
            && entry.packed_size <= MAX_RESOURCE_BYTES
    });
    candidates.sort_by_cached_key(|entry| {
        let archive = entry
            .archive_path
            .file_name()
            .unwrap_or_default()
            .to_string_lossy()
            .to_ascii_lowercase();
        let scenario = archive.starts_with("data01")
            || archive.contains("script")
            || archive.contains("scenario");
        (!scenario, archive, entry.entry_name.to_ascii_lowercase())
    });
    for entry in candidates {
        if probes >= MAX_PROBES || sample.len() >= MAX_SAMPLE_BYTES {
            break;
        }
        // Sample only the version selected by the runtime's archive priority.
        if manager
            .find(&entry.entry_name)
            .is_none_or(|selected| selected.archive_path != entry.archive_path)
        {
            continue;
        }
        probes += 1;
        let Ok(size) = manager.decoded_size_by_entry(&entry) else {
            continue;
        };
        if size > MAX_RESOURCE_BYTES || read_bytes + size + entry.packed_size > MAX_READ_BYTES {
            continue;
        }
        read_bytes += size + entry.packed_size;
        let Ok(bytes) = manager.read_by_entry_decoded(&entry) else {
            continue;
        };
        append_scenario_text(&bytes, &mut sample);
    }
    let encoding = detect_sample(&sample);
    tracing::info!(
        encoding = encoding.name(),
        source = "auto",
        sample_bytes = sample.len(),
        probes,
        "game display text encoding configured"
    );
    encoding
}

fn append_scenario_text(bytes: &[u8], sample: &mut Vec<u8>) {
    let Some(program) = ethornell_script::bcs::parse_bcs(bytes) else {
        return;
    };
    let limit = (sample.len() + MAX_SCRIPT_SAMPLE_BYTES).min(MAX_SAMPLE_BYTES);
    let mut seen = BTreeSet::new();
    for reference in program
        .commands
        .iter()
        .flat_map(|command| &command.string_refs)
    {
        if !seen.insert(reference.offset) {
            continue;
        }
        let Some(rest) = bytes.get(reference.offset as usize..) else {
            continue;
        };
        let Some(end) = rest.iter().position(|byte| *byte == 0) else {
            continue;
        };
        let text = &rest[..end];
        if text.is_ascii() || sample.len() + text.len() + 1 > limit {
            continue;
        }
        // Read the original bytes. parse_bcs decodes its inspection strings as
        // Shift-JIS, which can already replace valid GBK with U+FFFD.
        sample.extend_from_slice(text);
        sample.push(b'\n');
    }
}

fn detect_sample(sample: &[u8]) -> &'static Encoding {
    if sample.iter().filter(|byte| **byte >= 0x80).count() < MIN_NON_ASCII_BYTES {
        return SHIFT_JIS;
    }
    let mut detector = EncodingDetector::new();
    detector.feed(sample, true);
    // These are the two supported legacy display code pages. A weak or
    // unrelated guess retains the original Japanese default.
    if detector.guess(None, false) == GBK {
        GBK
    } else {
        SHIFT_JIS
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::path::{Path, PathBuf};
    use std::sync::atomic::{AtomicU64, Ordering};

    const CHINESE: &str =
        "向日葵在阳光下开放。今天我们一起去学校吧。她微笑着对我说，这是一个漫长的暑假。\n";
    const JAPANESE: &str = "ひまわりが太陽の光を浴びて咲いている。今日は一緒に学校へ行こう。彼女は微笑みながら言った。\n";

    fn scenario(text: &str, encoding: &'static Encoding) -> Vec<u8> {
        let encoded = encoding.encode(text).0.into_owned();
        let mut bytes = b"BurikoCompiledScriptVer1.00\0".to_vec();
        bytes.extend_from_slice(&12u32.to_le_bytes());
        bytes.extend_from_slice(&0u32.to_le_bytes());
        bytes.extend_from_slice(&0u32.to_le_bytes());
        // One push_string, say and ret. String offsets are relative to 0x28.
        for word in [3u32, 16, 0x140, 0x1b] {
            bytes.extend_from_slice(&word.to_le_bytes());
        }
        bytes.extend(encoded);
        bytes.push(0);
        bytes
    }

    struct GameFixture(PathBuf);

    impl GameFixture {
        fn new() -> Self {
            static NEXT: AtomicU64 = AtomicU64::new(0);
            let root = std::env::temp_dir().join(format!(
                "ethornell-encoding-{}-{}",
                std::process::id(),
                NEXT.fetch_add(1, Ordering::Relaxed)
            ));
            std::fs::create_dir(&root).unwrap();
            Self(root)
        }

        fn archive(&self, archive: &str, name: &str, payload: &[u8]) {
            let mut bytes = b"PackFile    ".to_vec();
            bytes.extend_from_slice(&1u32.to_le_bytes());
            let mut record = [0; 32];
            record[..name.len()].copy_from_slice(name.as_bytes());
            record[20..24].copy_from_slice(&(payload.len() as u32).to_le_bytes());
            bytes.extend_from_slice(&record);
            bytes.extend_from_slice(payload);
            std::fs::write(self.0.join(archive), bytes).unwrap();
        }

        fn manager(&self) -> ResourceManager {
            ResourceManager::open_game(&self.0).unwrap()
        }
    }

    impl Drop for GameFixture {
        fn drop(&mut self) {
            std::fs::remove_dir_all(&self.0).unwrap();
        }
    }

    #[test]
    fn detects_chinese_and_japanese_from_original_scenario_bytes() {
        for (text, expected) in [(CHINESE, GBK), (JAPANESE, SHIFT_JIS)] {
            let bytes = scenario(&text.repeat(8), expected);
            let mut sample = Vec::new();
            append_scenario_text(&bytes, &mut sample);
            assert_eq!(detect_sample(&sample), expected);
            assert!(!sample.contains(&0));
        }
    }

    #[test]
    fn ascii_short_and_binary_samples_retain_the_japanese_default() {
        for bytes in [
            Vec::new(),
            b"ASCII only".to_vec(),
            scenario("向日葵", GBK),
            vec![0x80; 1000],
        ] {
            let mut sample = Vec::new();
            append_scenario_text(&bytes, &mut sample);
            assert_eq!(detect_sample(&sample), SHIFT_JIS);
        }
    }

    #[test]
    fn sample_budget_keeps_complete_strings() {
        let mut sample = vec![b'a'; MAX_SAMPLE_BYTES - 2];
        append_scenario_text(&scenario(CHINESE, GBK), &mut sample);
        assert_eq!(sample.len(), MAX_SAMPLE_BYTES - 2);
    }

    #[test]
    fn desktop_choices_preserve_explicit_code_pages() {
        for (label, choice) in [
            ("cp932", TextEncoding::ShiftJis),
            ("gbk", TextEncoding::Gbk),
            ("auto", TextEncoding::Auto),
        ] {
            assert_eq!(label.parse::<TextEncoding>(), Ok(choice));
        }
        assert!("utf8".parse::<TextEncoding>().is_err());
        let game = GameFixture::new();
        game.archive("scenario.arc", "scene", &scenario(&CHINESE.repeat(8), GBK));
        let manager = game.manager();
        assert_eq!(TextEncoding::Auto.resolve(&manager), GBK);
        assert_eq!(TextEncoding::ShiftJis.resolve(&manager), SHIFT_JIS);
        assert_eq!(TextEncoding::Gbk.resolve(&manager), GBK);
    }

    #[test]
    fn explicit_code_pages_override_detection() {
        let game = GameFixture::new();
        game.archive("scenario.arc", "scene", &scenario(&CHINESE.repeat(8), GBK));
        let manager = game.manager();
        for label in ["gbk", " CP936 ", "windows-936"] {
            assert_eq!(select(&manager, Some(label)), GBK);
        }
        for label in ["shift_jis", "shift-jis", "sjis", " CP932 "] {
            assert_eq!(select(&manager, Some(label)), SHIFT_JIS);
        }
        for label in [None, Some("auto"), Some(""), Some("unsupported")] {
            assert_eq!(select(&manager, label), GBK);
        }
    }

    #[test]
    fn ignores_shadowed_scripts_and_framework_messages() {
        let game = GameFixture::new();
        game.archive("data01000.arc", "scene", &scenario(&CHINESE.repeat(8), GBK));
        game.archive(
            "data01100.arc",
            "scene",
            &scenario(&JAPANESE.repeat(80), SHIFT_JIS),
        );
        game.archive(
            "sysprg.arc",
            "messages",
            &scenario(&JAPANESE.repeat(80), SHIFT_JIS),
        );
        game.archive(
            "scenario.arc",
            "debug._bp",
            &scenario(&JAPANESE.repeat(80), SHIFT_JIS),
        );
        let manager = game.manager();
        assert_eq!(select(&manager, None), GBK);
        assert_eq!(
            manager.find("scene").unwrap().archive_path.file_name(),
            Some(Path::new("data01000.arc").as_os_str())
        );
    }

    #[test]
    fn skips_oversized_decoded_resources_and_falls_back_without_scenarios() {
        let game = GameFixture::new();
        // A small DSC header can advertise a huge decoded allocation. Detection
        // must reject that size before attempting decompression.
        let mut payload = vec![0; 32];
        payload[..16].copy_from_slice(b"DSC FORMAT 1.00\0");
        payload[20..24].copy_from_slice(&u32::MAX.to_le_bytes());
        game.archive("data01000.arc", "invalid", &payload);
        assert_eq!(select(&game.manager(), None), SHIFT_JIS);
    }

    #[test]
    #[ignore = "requires ETHORNELL_ENCODING_REPRO_GAME with an authorized game path"]
    fn detects_authorized_game() {
        ethornell_core::init_tracing();
        let root = std::env::var_os("ETHORNELL_ENCODING_REPRO_GAME").expect("authorized game path");
        let manager = ResourceManager::open_game(root).unwrap();
        let encoding = select(&manager, None);
        eprintln!("Detected display encoding: {}", encoding.name());
        let expected = std::env::var("ETHORNELL_ENCODING_REPRO_EXPECTED")
            .unwrap_or_else(|_| "gbk".into())
            .parse::<TextEncoding>()
            .unwrap();
        assert_ne!(expected, TextEncoding::Auto);
        assert_eq!(encoding, expected.resolve(&manager));
    }
}
