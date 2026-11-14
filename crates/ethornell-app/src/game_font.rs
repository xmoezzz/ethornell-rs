//! Optional game-owned fonts, selected once per session without a global cache.
use super::*;
use ab_glyph::FontArc;

fn font_name(name: &str) -> bool {
    Path::new(name)
        .extension()
        .and_then(|s| s.to_str())
        .is_some_and(|s| {
            matches!(
                s.to_ascii_lowercase().as_str(),
                "ttf" | "otf" | "ttc" | "otc"
            )
        })
}

pub(super) fn load(root: &Path, required: &str) -> Option<FontArc> {
    let policy = file_policy::active();
    let mut names = if let Some(policy) = &policy {
        policy
            .enumerate("bgi-resource/*", false, 0, false)
            .unwrap_or_default()
    } else {
        std::fs::read_dir(root)
            .ok()?
            .filter_map(|entry| {
                let entry = entry.ok()?;
                entry
                    .file_type()
                    .ok()?
                    .is_file()
                    .then(|| entry.file_name().into_string().ok())
                    .flatten()
            })
            .collect()
    };
    names.retain(|name| font_name(name));
    names.sort_by_cached_key(|name| (name.to_lowercase(), name.clone()));
    let mut remaining = 64 * 1024 * 1024u64;
    for name in names.into_iter().take(128) {
        let limit = remaining.min(32 * 1024 * 1024);
        if limit == 0 {
            break;
        }
        let bytes = if let Some(policy) = &policy {
            policy
                .read_resource(&format!("bgi-resource/{name}"), limit)
                .ok()
        } else {
            use std::io::Read;
            std::fs::File::open(root.join(&name)).ok().and_then(|file| {
                if !file
                    .metadata()
                    .ok()
                    .is_some_and(|m| m.is_file() && m.len() <= limit)
                {
                    return None;
                }
                let mut bytes = Vec::new();
                file.take(limit + 1).read_to_end(&mut bytes).ok()?;
                (bytes.len() as u64 <= limit).then_some(bytes)
            })
        };
        let Some(bytes) = bytes else {
            tracing::warn!(
                name,
                "game font could not be read within the session budget"
            );
            continue;
        };
        remaining -= bytes.len() as u64;
        // Match the web's game-folder policy: collection face zero. A host
        // explicitly selecting another face still uses HostConfig instead.
        let Ok(font) = ethornell_render::FontProvider::load_bytes(bytes, 0) else {
            tracing::warn!(name, "invalid game font; trying the next candidate");
            continue;
        };
        if ethornell_render::FontProvider::missing_glyphs(&font, required).is_empty() {
            tracing::info!(name, "loaded game-folder font");
            return Some(font);
        }
        tracing::warn!(name, "game font lacks the session's required glyphs");
    }
    None
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    #[ignore = "reads an authorized system font at runtime; no font assets in the repository"]
    fn game_font_selection_skips_invalid_files_and_reopens_without_a_cache() {
        use ab_glyph::Font;
        let source =
            std::env::var_os("ETHORNELL_FONT_TEST_SOURCE").expect("explicit system font path");
        let bytes = std::fs::read(source).unwrap();
        let expected = ethornell_render::FontProvider::load_bytes(bytes.clone(), 0).unwrap();
        let root = std::env::temp_dir().join(format!("ethornell-game-font-{}", std::process::id()));
        std::fs::create_dir_all(&root).unwrap();
        std::fs::write(root.join("00-broken.ttf"), b"not a font").unwrap();
        std::fs::write(root.join("10-text.TTC"), bytes).unwrap();
        let font = load(&root, "A日本語中文").unwrap();
        assert_eq!(font.glyph_id('A'), expected.glyph_id('A'));
        assert_eq!(
            font.h_advance_unscaled(font.glyph_id('A')),
            expected.h_advance_unscaled(expected.glyph_id('A'))
        );
        std::fs::remove_file(root.join("10-text.TTC")).unwrap();
        assert!(load(&root, "A日本語中文").is_none());
        std::fs::remove_dir_all(root).unwrap();
    }
}
