use ab_glyph::{Font, FontArc, FontVec};
use ethornell_core::{EthornellError, Result};
use std::collections::BTreeSet;
use std::path::Path;

/// Stateless font loading shared by GPU text, CPU snapshots and text metrics.
/// Keep the returned FontArc in the session, not a process-wide OnceLock.
pub struct FontProvider;

impl FontProvider {
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
        text.chars()
            .filter(|ch| !ch.is_control() && font.glyph_id(*ch).0 == 0)
            .collect::<BTreeSet<_>>()
            .into_iter()
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
        for path in super::SYSTEM_CJK_FONT_CANDIDATES {
            let Ok(bytes) = std::fs::read(path) else {
                continue;
            };
            // A TTC can contain regional faces. Try each existing face rather
            // than assuming face 0 supports the session's language.
            for face in 0..32 {
                let Ok(font) = Self::load_bytes(bytes.clone(), face) else {
                    break;
                };
                if Self::missing_glyphs(&font, required_text).is_empty() {
                    tracing::debug!(path, face, "loaded system CJK font");
                    return Ok(font);
                }
            }
        }
        Err(EthornellError::Other(
            "no usable system font with required glyph coverage".into(),
        ))
    }
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
