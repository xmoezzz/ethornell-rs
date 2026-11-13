//! Native named font transforms shared by bitmap text and live text nodes.
//! The GDI backend renders into an enlarged coverage cell, crops at the
//! configured offset, then reduces it. The portable font backend applies the
//! same scale/offset boundary to its own outline coverage; fonts themselves
//! remain supplied by the host.
use super::*;
use native_graph_sys91::Graph91FontTransform;

impl Graph91FontTransform {
    pub(crate) fn scales(self) -> [f32; 2] {
        if [self.scale_x, self.scale_y, self.offset_x, self.offset_y] == [0; 4] {
            return [1.0; 2];
        }
        [
            if self.scale_x == 0 {
                self.scale_y
            } else {
                self.scale_x
            } as f32
                / 65536.0,
            self.scale_y as f32 / 65536.0,
        ]
    }
    pub(crate) fn advance_adjustment(self, size: f32) -> f32 {
        // FD 4338C0 / 43C810: 0.25 * GDI's 4x coverage height * tracking.
        size * self.scales()[1] * self.tracking as f32 / 65536.0
    }
}

impl RuntimeTraceApi {
    pub(crate) fn named_font_transform_for_id(&self, font: i32) -> Option<Graph91FontTransform> {
        let face = self.native_user.font_face(font)?;
        self.graph91_font_transforms
            .get(&face.to_ascii_lowercase())
            .copied()
    }

    pub(crate) fn named_font_transform(
        &self,
        surface: Option<i32>,
    ) -> Option<Graph91FontTransform> {
        let face = surface
            .and_then(|handle| self.surface_text_states.get(&handle))
            .and_then(|state| self.native_user.font_face(state.font))
            .or(self.graph_defaults.text_style.font_name.as_deref())?;
        self.graph91_font_transforms
            .get(&face.to_ascii_lowercase())
            .copied()
    }
}

/// Area reduction of the coverage raster. Input cells have one fixed RGB
/// colour, so only straight-alpha coverage is averaged, avoiding colour
/// fringes from transparent padding.
pub(crate) fn reduce_glyph_coverage(source: &DecodedImage, scale: [f32; 2]) -> DecodedImage {
    let width = ((source.width as f32 / scale[0]).ceil() as u32).max(1);
    let height = ((source.height as f32 / scale[1]).ceil() as u32).max(1);
    let mut out = DecodedImage {
        width,
        height,
        rgba: vec![0; width as usize * height as usize * 4],
    };
    let rgb = source
        .rgba
        .as_chunks::<4>()
        .0
        .iter()
        .find(|p| p[3] != 0)
        .map_or([0; 3], |p| [p[0], p[1], p[2]]);
    for y in 0..height {
        let y0 = y as f32 * scale[1];
        let y1 = (y0 + scale[1]).min(source.height as f32);
        for x in 0..width {
            let x0 = x as f32 * scale[0];
            let x1 = (x0 + scale[0]).min(source.width as f32);
            let mut coverage = 0.0;
            for sy in y0.floor() as u32..y1.ceil() as u32 {
                for sx in x0.floor() as u32..x1.ceil() as u32 {
                    let weight = (x1.min(sx as f32 + 1.0) - x0.max(sx as f32)).max(0.0)
                        * (y1.min(sy as f32 + 1.0) - y0.max(sy as f32)).max(0.0);
                    coverage += source.rgba
                        [(sy as usize * source.width as usize + sx as usize) * 4 + 3]
                        as f32
                        * weight;
                }
            }
            let at = (y as usize * width as usize + x as usize) * 4;
            out.rgba[at..at + 3].copy_from_slice(&rgb);
            out.rgba[at + 3] = (coverage / ((x1 - x0) * (y1 - y0)).max(f32::MIN_POSITIVE))
                .round()
                .clamp(0.0, 255.0) as u8;
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn named_font_transform_reduces_coverage_and_retains_tracking() {
        let image = DecodedImage {
            width: 2,
            height: 2,
            rgba: vec![
                255, 255, 255, 0, 255, 255, 255, 64, 255, 255, 255, 128, 255, 255, 255, 255,
            ],
        };
        assert_eq!(
            reduce_glyph_coverage(&image, [2.0; 2]).rgba,
            [255, 255, 255, 112]
        );
        let transform = Graph91FontTransform {
            scale_x: 0,
            scale_y: 0x18000,
            offset_x: 0,
            offset_y: 0,
            tracking: 0x4000,
        };
        assert_eq!(transform.scales(), [1.5; 2]);
        assert_eq!(transform.advance_adjustment(24.0), 9.0);
    }
}
