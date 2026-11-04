use crate::{FontProvider, TextureId};
use ab_glyph::{Font, FontArc, PxScale, ScaleFont};
use ethornell_core::{EthornellError, Result};
use ethornell_image::DecodedImage;
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct TextStyleSpan {
    pub start_char: usize,
    pub end_char: usize,
    pub packed_rgb: Option<u32>,
    pub bold: bool,
    pub italic: bool,
}

/// A glow under drawn text: the target's text style mode 2 (`sub_434E30`).
/// The glyphs move `radius_x`, `radius_y` pixels right and down onto a halo
/// in `rgb` whose alpha is their coverage summed over a box of twice the
/// radius (capped at opaque), blended at `concentration / 256`.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct TextGlow {
    pub radius_x: u32,
    pub radius_y: u32,
    pub rgb: [u8; 3],
    pub concentration: u32,
}

#[derive(Debug, Clone)]
pub enum RenderCommand {
    Clear {
        color: [f32; 4],
    },
    DrawTexture {
        texture: TextureId,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        /// Normalized texture coordinates in 0..=1, already divided by image size.
        src_x: f32,
        src_y: f32,
        src_width: f32,
        src_height: f32,
        opacity: f32,
        /// Ignore sampled texture alpha. Native bitmap format 1 is XRGB; its
        /// fourth byte is not source coverage, while object transparency is
        /// still supplied separately through `opacity`.
        ignore_source_alpha: bool,
        /// Native CDspObj blend selector. Common recovered modes are routed
        /// to dedicated pipelines; unknown values retain ordinary alpha.
        blend_mode: i32,
        rotation_degrees: f32,
        /// Optional affine destination footprint in virtual/game coordinates,
        /// ordered TL, BL, BR, TR. Native mode 5 supplies this directly from
        /// the inverse of sub_416750 rather than rotating a raster bbox.
        destination_quad: Option<[[f32; 2]; 4]>,
        /// Texture interpolation selector. Native mode 5 recovers this from
        /// CDspObjSprite+0x280 (0 nearest, nonzero bilinear).
        linear_sampling: bool,
        clip: Option<[f32; 4]>,
        z: i32,
    },
    DrawText {
        text: String,
        x: f32,
        y: f32,
        color: [f32; 4],
        size: f32,
        /// Distance between the tops of consecutive lines, e.g. a window's
        /// font height plus its Graph91:89 spacing; `None` for the font's own
        /// line height.
        line_height: Option<f32>,
        styles: Vec<TextStyleSpan>,
        glow: Option<TextGlow>,
        clip: Option<[f32; 4]>,
        z: i32,
    },
}

#[derive(Debug, Clone)]
pub struct TextureHandle {
    pub id: TextureId,
    pub width: u32,
    pub height: u32,
}

/// How the game frame fills the surface in full screen (sub_45F110 /
/// sub_45EE30 by the effective Sys81:61 mode).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PresentationScale {
    /// Mode 0: uniform scale, centred (letterbox).
    Fit,
    /// Mode 1: each axis scaled to the surface.
    Stretch,
    /// Mode 2: 1:1, centred.
    Center,
}

pub(crate) fn presentation_viewport(
    mode: PresentationScale,
    surface_width: f32,
    surface_height: f32,
    virtual_width: f32,
    virtual_height: f32,
) -> (f32, f32, f32, f32) {
    match mode {
        PresentationScale::Fit => {
            let (scale, offset_x, offset_y) =
                viewport_transform(surface_width, surface_height, virtual_width, virtual_height);
            (scale, scale, offset_x, offset_y)
        }
        PresentationScale::Stretch => (
            (surface_width / virtual_width).max(0.01),
            (surface_height / virtual_height).max(0.01),
            0.0,
            0.0,
        ),
        PresentationScale::Center => (
            1.0,
            1.0,
            ((surface_width - virtual_width) * 0.5).floor(),
            ((surface_height - virtual_height) * 0.5).floor(),
        ),
    }
}

pub(crate) fn viewport_to_virtual_point(
    x: f32,
    y: f32,
    (scale_x, scale_y, offset_x, offset_y): (f32, f32, f32, f32),
    virtual_width: f32,
    virtual_height: f32,
) -> Option<(f32, f32)> {
    let virtual_x = (x - offset_x) / scale_x;
    let virtual_y = (y - offset_y) / scale_y;
    ((0.0..=virtual_width).contains(&virtual_x) && (0.0..=virtual_height).contains(&virtual_y))
        .then_some((virtual_x, virtual_y))
}

/// A clip rectangle in game coordinates as a surface scissor, cut to the
/// game frame: nothing is drawn into the letterbox bars.
#[cfg(test)]
pub(crate) fn game_clip_to_scissor(
    clip: [f32; 4],
    surface_width: f32,
    surface_height: f32,
    virtual_width: f32,
    virtual_height: f32,
) -> Option<(u32, u32, u32, u32)> {
    let viewport = presentation_viewport(
        PresentationScale::Fit,
        surface_width,
        surface_height,
        virtual_width,
        virtual_height,
    );
    game_clip_to_scissor_in(
        clip,
        viewport,
        surface_width,
        surface_height,
        virtual_width,
        virtual_height,
    )
}

pub(crate) fn game_clip_to_scissor_in(
    clip: [f32; 4],
    (scale_x, scale_y, offset_x, offset_y): (f32, f32, f32, f32),
    surface_width: f32,
    surface_height: f32,
    virtual_width: f32,
    virtual_height: f32,
) -> Option<(u32, u32, u32, u32)> {
    let left = clip[0].max(0.0);
    let top = clip[1].max(0.0);
    let right = (clip[0] + clip[2]).min(virtual_width);
    let bottom = (clip[1] + clip[3]).min(virtual_height);
    if right <= left || bottom <= top {
        return None;
    }
    let x0 = (offset_x + left * scale_x)
        .floor()
        .max(0.0)
        .min(surface_width) as u32;
    let y0 = (offset_y + top * scale_y)
        .floor()
        .max(0.0)
        .min(surface_height) as u32;
    let x1 = (offset_x + right * scale_x)
        .ceil()
        .max(0.0)
        .min(surface_width) as u32;
    let y1 = (offset_y + bottom * scale_y)
        .ceil()
        .max(0.0)
        .min(surface_height) as u32;
    (x1 > x0 && y1 > y0).then_some((x0, y0, x1 - x0, y1 - y0))
}

pub(crate) fn viewport_transform(
    surface_width: f32,
    surface_height: f32,
    virtual_width: f32,
    virtual_height: f32,
) -> (f32, f32, f32) {
    let scale = (surface_width / virtual_width)
        .min(surface_height / virtual_height)
        .max(0.01);
    let offset_x = (surface_width - virtual_width * scale) * 0.5;
    let offset_y = (surface_height - virtual_height * scale) * 0.5;
    (scale, offset_x, offset_y)
}

#[cfg(test)]
pub(crate) fn surface_to_virtual_point(
    x: f32,
    y: f32,
    surface_width: f32,
    surface_height: f32,
    virtual_width: f32,
    virtual_height: f32,
) -> Option<(f32, f32)> {
    let (scale, offset_x, offset_y) =
        viewport_transform(surface_width, surface_height, virtual_width, virtual_height);
    let virtual_x = (x - offset_x) / scale;
    let virtual_y = (y - offset_y) / scale;
    if (0.0..=virtual_width).contains(&virtual_x) && (0.0..=virtual_height).contains(&virtual_y) {
        Some((virtual_x, virtual_y))
    } else {
        None
    }
}

/// Loads the text font: the file named by `ETHORNELL_FONT` when it is set
/// and usable, else the first usable system font of the candidate list.
/// Games ask for fonts such as MS Gothic by name, which other systems lack;
/// a port can point this at a substitute with the same metrics.
pub fn load_system_cjk_font() -> Result<FontArc> {
    load_text_font(std::env::var_os("ETHORNELL_FONT"))
}

/// Loads the text font from `requested` (the value of `ETHORNELL_FONT`) when
/// it names a usable font file, else the first usable system font.
pub fn load_text_font(requested: Option<std::ffi::OsString>) -> Result<FontArc> {
    if let Some(path) = requested {
        let path = std::path::PathBuf::from(path);
        match std::fs::read(&path)
            .map_err(|err| err.to_string())
            .and_then(|bytes| FontArc::try_from_vec(bytes).map_err(|err| err.to_string()))
        {
            Ok(font) => {
                tracing::info!(path = %path.display(), "loaded text font from ETHORNELL_FONT");
                return Ok(font);
            }
            Err(err) => tracing::warn!(
                path = %path.display(),
                %err,
                "ETHORNELL_FONT is unusable; using a system font"
            ),
        }
    }
    FontProvider::load_system_cjk_font()
}

pub(crate) fn rasterize_text(
    font: &FontArc,
    text: &str,
    px_height: f32,
    line_pitch: Option<f32>,
    color: [f32; 4],
    styles: &[TextStyleSpan],
) -> DecodedImage {
    let lines: Vec<&str> = text.split('\n').collect();
    let scale = PxScale::from(px_height.max(1.0));
    let scaled = font.as_scaled(scale);
    let line_height = (scaled.ascent() - scaled.descent() + scaled.line_gap())
        .ceil()
        .max(px_height.ceil()) as u32;
    let line_pitch = line_pitch
        .filter(|pitch| pitch.is_finite() && *pitch > 0.0)
        .unwrap_or(line_height as f32);
    let widths: Vec<u32> = lines
        .iter()
        .map(|line| measure_line(font, line, px_height))
        .collect();
    let style_overhang = styles
        .iter()
        .any(|style| style.bold || style.italic)
        .then_some((px_height * 0.25).ceil() as u32 + 1)
        .unwrap_or_default();
    let width = widths
        .iter()
        .copied()
        .max()
        .unwrap_or(1)
        .saturating_add(style_overhang)
        .max(1);
    let height = ((lines.len().max(1) - 1) as f32 * line_pitch).ceil() as u32 + line_height;
    let mut rgba = vec![0u8; width as usize * height as usize * 4];

    let mut char_index = 0usize;
    for (line_index, line) in lines.iter().enumerate() {
        let baseline_y = (line_index as f32 * line_pitch).round() + scaled.ascent();
        let mut cursor_x = 0.0f32;
        for ch in line.chars() {
            let style = text_style_at(styles, char_index);
            let glyph_color = style
                .and_then(|style| style.packed_rgb)
                .map(|packed| {
                    [
                        ((packed >> 16) & 0xff) as f32 / 255.0,
                        ((packed >> 8) & 0xff) as f32 / 255.0,
                        (packed & 0xff) as f32 / 255.0,
                        color[3],
                    ]
                })
                .unwrap_or(color);
            let rgba_color = [
                (glyph_color[0].clamp(0.0, 1.0) * 255.0) as u8,
                (glyph_color[1].clamp(0.0, 1.0) * 255.0) as u8,
                (glyph_color[2].clamp(0.0, 1.0) * 255.0) as u8,
                (glyph_color[3].clamp(0.0, 1.0) * 255.0) as u8,
            ];
            let glyph_id = font.glyph_id(ch);
            let glyph =
                glyph_id.with_scale_and_position(scale, ab_glyph::point(cursor_x, baseline_y));
            cursor_x += scaled.h_advance(glyph_id);
            if let Some(outlined) = font.outline_glyph(glyph) {
                let bounds = outlined.px_bounds();
                outlined.draw(|gx, gy, coverage| {
                    let italic_dx = style
                        .filter(|style| style.italic)
                        .map(|_| ((bounds.height() - gy as f32).max(0.0) * 0.20).round() as i32)
                        .unwrap_or_default();
                    let px = bounds.min.x as i32 + gx as i32 + italic_dx;
                    let py = bounds.min.y as i32 + gy as i32;
                    let bold_width = usize::from(style.is_some_and(|style| style.bold));
                    for bold_dx in 0..=bold_width {
                        let px = px + bold_dx as i32;
                        if px < 0 || py < 0 || px >= width as i32 || py >= height as i32 {
                            continue;
                        }
                        let idx = (py as usize * width as usize + px as usize) * 4;
                        let alpha = (coverage * rgba_color[3] as f32).round() as u8;
                        if alpha >= rgba[idx + 3] {
                            rgba[idx] = rgba_color[0];
                            rgba[idx + 1] = rgba_color[1];
                            rgba[idx + 2] = rgba_color[2];
                        }
                        rgba[idx + 3] = rgba[idx + 3].max(alpha);
                    }
                });
            }
            char_index += 1;
        }
        char_index += usize::from(line_index + 1 < lines.len());
    }

    DecodedImage {
        width,
        height,
        rgba,
    }
}

/// Puts rasterized text (straight alpha) on its glow; see [`TextGlow`]. Like
/// the target's per-glyph records, the halo at (x, y) sums the text's
/// coverage over x - 2 * radius_x ..= x and y - 2 * radius_y ..= y, and the
/// text lands at (radius_x, radius_y).
pub(crate) fn apply_text_glow(text: &DecodedImage, glow: TextGlow) -> DecodedImage {
    let (text_width, text_height) = (text.width as usize, text.height as usize);
    let (rx, ry) = (glow.radius_x as usize, glow.radius_y as usize);
    let (width, height) = (text_width + 2 * rx, text_height + 2 * ry);
    // Summed-area table of the text's alpha, one extra row and column.
    let stride = text_width + 1;
    let mut table = vec![0u32; stride * (text_height + 1)];
    for y in 0..text_height {
        let mut row = 0u32;
        for x in 0..text_width {
            row += u32::from(text.rgba[(y * text_width + x) * 4 + 3]);
            table[(y + 1) * stride + x + 1] = table[y * stride + x + 1] + row;
        }
    }
    let box_sum = |x0: usize, y0: usize, x1: usize, y1: usize| {
        table[y1 * stride + x1] + table[y0 * stride + x0]
            - table[y0 * stride + x1]
            - table[y1 * stride + x0]
    };
    let concentration = glow.concentration.min(256);
    let mut rgba = vec![0u8; width * height * 4];
    for y in 0..height {
        let (y0, y1) = (
            y.saturating_sub(2 * ry).min(text_height),
            (y + 1).min(text_height),
        );
        for x in 0..width {
            let (x0, x1) = (
                x.saturating_sub(2 * rx).min(text_width),
                (x + 1).min(text_width),
            );
            let sum = box_sum(x0, y0, x1, y1).min(255);
            let at = (y * width + x) * 4;
            rgba[at..at + 3].copy_from_slice(&glow.rgb);
            rgba[at + 3] = (sum * concentration / 256) as u8;
        }
    }
    for y in 0..text_height {
        for x in 0..text_width {
            let source = &text.rgba[(y * text_width + x) * 4..][..4];
            let source_alpha = u32::from(source[3]);
            if source_alpha == 0 {
                continue;
            }
            let at = ((y + ry) * width + x + rx) * 4;
            let below_alpha = u32::from(rgba[at + 3]) * (255 - source_alpha) / 255;
            let alpha = source_alpha + below_alpha;
            for channel in 0..3 {
                rgba[at + channel] = ((u32::from(source[channel]) * source_alpha
                    + u32::from(rgba[at + channel]) * below_alpha)
                    / alpha) as u8;
            }
            rgba[at + 3] = alpha as u8;
        }
    }
    DecodedImage {
        width: width as u32,
        height: height as u32,
        rgba,
    }
}

fn text_style_at(styles: &[TextStyleSpan], char_index: usize) -> Option<TextStyleSpan> {
    styles
        .iter()
        .copied()
        .find(|style| style.start_char <= char_index && char_index < style.end_char)
}

fn measure_line(font: &FontArc, text: &str, px_height: f32) -> u32 {
    let scale = PxScale::from(px_height.max(1.0));
    let scaled = font.as_scaled(scale);
    text.chars()
        .map(|c| scaled.h_advance(font.glyph_id(c)))
        .sum::<f32>()
        .ceil()
        .max(1.0) as u32
}

pub(crate) const SYSTEM_CJK_FONT_CANDIDATES: &[&str] = &[
    "/system/fonts/NotoSansCJK-Regular.ttc",
    "/system/fonts/NotoSerifCJK-Regular.ttc",
    "/system/fonts/NotoSansJP-Regular.otf",
    "/system/fonts/NotoSansSC-Regular.otf",
    "/system/fonts/DroidSansFallback.ttf",
    "/System/Library/Fonts/ヒラギノ角ゴシック W8.ttc",
    "/System/Library/Fonts/Core/HiraginoKakuGothic.ttc",
    "/System/Library/Fonts/AppleSDGothicNeo.ttc",
    "/System/Library/Fonts/Hiragino Sans GB.ttc",
    "/System/Library/Fonts/STHeiti Medium.ttc",
    "/System/Library/Fonts/Supplemental/AppleGothic.ttf",
    "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
    "/Library/Fonts/Arial Unicode.ttf",
    "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
    "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
    "/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc",
    "C:/Windows/Fonts/msgothic.ttc",
    "C:/Windows/Fonts/msyh.ttc",
];

pub(crate) fn validate_rgba(image: &DecodedImage) -> Result<()> {
    if image.width == 0 || image.height == 0 {
        return Err(EthornellError::Parse("zero-sized texture".into()));
    }
    let expected_len = (image.width as usize)
        .checked_mul(image.height as usize)
        .and_then(|pixels| pixels.checked_mul(4));
    if expected_len != Some(image.rgba.len()) {
        return Err(EthornellError::Parse("texture RGBA size mismatch".into()));
    }
    Ok(())
}
