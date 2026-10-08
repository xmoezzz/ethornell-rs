use std::collections::BTreeMap;
use std::sync::OnceLock;
use std::sync::atomic::{AtomicU64, Ordering};

use crate::native_background::NativeBackgroundState;
use crate::native_display::CDspObjLayout32;
use ethornell_core::composite_native_rgba;
use ethornell_image::DecodedImage;

pub(crate) const NATIVE_DISPLAY_Z: i32 = 100;
// `sub_442850 -> sub_430570` passes 1024 as CObjectManager's +0x44
// workspace/index size.  It is not a recovered display-domain priority split;
// the draw-time priority cursor is the independent +0x48 field set by
// `sub_430D20` and consumed by `sub_431550`.
// The stock sysprg modules query slot 3790 as the host-provided screen bitmap
// without creating it through 90:11. The native bitmap manager supplies this
// slot before BP execution; save, message, and system scripts all depend on it.
pub(crate) const NATIVE_SCREEN_BITMAP: i32 = 3790;

#[derive(Debug, Clone)]
pub(crate) struct RuntimeGraphObjectProperties {
    /// Exact target base field image.  The semantic fields below are portable
    /// mirrors until their concrete `CDspObj` member offsets are closed.
    pub(crate) native: CDspObjLayout32,
    /// Exact dynamic `CDspObjBack` subclass selected by `sub_43E190`.
    pub(crate) background: Option<NativeBackgroundState>,
    pub(crate) blend_mode: i32,
    pub(crate) mask_alpha: i32,
    pub(crate) alpha_multiplier: i32,
    /// Primary display bitmap for Sprite/Background subclasses. Keep this
    /// separate from CDspObj hit masks and CDspObjSprite auxiliary bitmaps.
    pub(crate) format_resource: Option<i32>,
    /// CDspObjSprite +0x13C auxiliary bitmap installed by Graph90:55 /
    /// sub_427F80. This modifies Sprite rendering but is not its primary.
    pub(crate) aux_resource: Option<i32>,
    /// Base CDspObj generated hit-mask source installed by Graph90:3C /
    /// sub_443A40 -> sub_41BC70. It controls pointer acceptance only.
    pub(crate) hit_mask_resource: Option<i32>,
    pub(crate) properties: BTreeMap<u32, (i32, i32)>,
    pub(crate) named_properties: BTreeMap<String, i32>,
}

impl Default for RuntimeGraphObjectProperties {
    fn default() -> Self {
        Self {
            native: CDspObjLayout32::default(),
            background: None,
            // CDspObj::CDspObj (sub_41A400) installs these exact defaults.
            blend_mode: 128,
            mask_alpha: 0,
            alpha_multiplier: 256,
            format_resource: None,
            aux_resource: None,
            hit_mask_resource: None,
            properties: BTreeMap::new(),
            named_properties: BTreeMap::new(),
        }
    }
}

impl RuntimeGraphObjectProperties {
    pub(crate) fn set_property(&mut self, property: u32, value: i32, extra: i32) {
        match property {
            1 => self.blend_mode = value,
            2 => self.set_alpha_parameter(value),
            // CDspObj::SetProperty selector 196 reaches sub_41ADF0 and writes
            // CDspObj+0x48. sub_41C0E0 consults this gate before adding the
            // global display offset configured by Graph91:06.
            196 => self.native.global_display_offset_enabled = value as u32,
            // CDspObj::SetProperty 0x8000 dispatches to sub_41BEB0(value,
            // extra), which writes +0x80/+0x84. sub_41B370 then uses the pair
            // to decide whether X/Y fixed coordinates are rounded to whole
            // 16.16 pixels before they are stored and propagated.
            0x8000 => {
                self.native.fixed_position_rounding_enabled = value as u32;
                self.native.fixed_position_rounding_mode = extra as u32;
            }
            // Property 0x8100 reaches sub_41BF10 and writes CDspObj+0x24,
            // an additive component of the vtable+0x1C manager sort key.
            0x8100 => self.native.sort_bias = value,
            _ => {
                self.properties.insert(property, (value, extra));
            }
        }
    }

    pub(crate) fn alpha_parameter(&self) -> i32 {
        self.native.alpha_parameter
    }

    /// CDspObj::SetAlpha (`sub_41B620`) stores the supplied DWORD without
    /// range validation. Callers whose native wrapper validates 0..=256 do
    /// that before reaching this setter.
    pub(crate) fn set_alpha_parameter(&mut self, value: i32) {
        self.native.alpha_parameter = value;
    }

    /// `CDspObj::GetMaskAlpha` (`sub_41B770`) in target transparency units.
    /// `0` means fully source-visible and `256` means fully transparent for
    /// the BackF mode-1 compositor.
    pub(crate) fn transparency_parameter(&self) -> i32 {
        // CDspObj's drawable predicate (sub_41AE30) rejects a fully masked
        // object before asking GetMaskAlpha for its blend-specific value.
        if self.mask_alpha >= 256 {
            return 256;
        }
        // CDspObj::GetMaskAlpha (sub_41B770) returns transparency in 1/256
        // units. Preserve its integer arithmetic exactly here so native
        // compositors can consume the integer result before any f32
        // conversion performed by the renderer.
        match self.blend_mode {
            1 | 0x20..=0x24 => {
                256 - ((self.alpha_multiplier
                    * (256 - self.alpha_parameter())
                    * (256 - self.mask_alpha))
                    >> 16)
            }
            2 | 3 | 4 | 0xc0 | 0xc1 => {
                (self.alpha_parameter() * self.alpha_multiplier * (256 - self.mask_alpha)) >> 16
            }
            _ => self.alpha_parameter(),
        }
        .clamp(0, 256)
    }

    pub(crate) fn opacity(&self) -> f32 {
        let transparency = self.transparency_parameter();
        (1.0 - transparency as f32 / 256.0).clamp(0.0, 1.0)
    }
}

/// Recovered scalar coverage used by `CDspObjBackF`'s format-3 mask path
/// (`sub_411B80` -> `sub_411BD0`/`sub_411D10`, and the enabled-control
/// variants `sub_411F10`/`sub_412070`). The result is expressed in the
/// target's 0..=256 interpolation domain: 0 keeps destination, 256 copies
/// source.
///
/// `extra_control == 0` selects the default path. A non-zero value selects
/// the mask-control path produced by `sub_41D540`.
pub(crate) fn backf_mask_weight(
    mask_byte: u8,
    mask_parameter: i32,
    transparency: i32,
    extra_control: i32,
) -> u16 {
    let transparency = transparency.clamp(0, 256);
    let parameter = mask_parameter as u32;

    if parameter < 8 {
        let shift = parameter;
        let shifted_unit = 1_i32 << shift;
        let raw = 256_i32
            .saturating_sub(transparency.saturating_mul(shifted_unit + 1))
            .saturating_add(i32::from(mask_byte) << shift);
        if raw <= 0 {
            return 0;
        }
        if extra_control == 0 && raw >= 256 {
            return 256;
        }

        // The target indexes a 129-entry 0..128 table with raw>>1, then
        // performs a signed fixed-point /128 interpolation. Preserve the
        // table's one-bit quantization rather than treating raw itself as the
        // final weight.
        let table_index = raw.min(256) >> 1;
        if extra_control == 0 {
            return (table_index * 2) as u16;
        }

        // sub_411F10 rebuilds the same table with slope
        // (256-extra_control)/256. Values reaching this path through
        // CDspObjBackF::sub_41D540 are in 0..=256.
        let extra_control = extra_control.clamp(0, 256);
        let table_value = table_index * (256 - extra_control) / 256;
        return (table_value * 2) as u16;
    }

    // For parameters >= 8 the low three bits select the number of triangular
    // lobes. x86 compares the original parameter unsigned and masks it only
    // after entering this branch, so negative i32 parameters intentionally
    // arrive here too.
    let q = (parameter & 7) as i32;
    let index = 256 - 2 * transparency + i32::from(mask_byte);
    if index <= 0 {
        return 0;
    }
    if index >= 256 {
        return 256;
    }

    // sub_411D10/sub_412070 build the triangular lookup table through x87,
    // but for an integer table index its phase can be written exactly as a
    // rational with denominator 256. This avoids host-float rounding drift:
    //   lobe = floor(index * (2*q+1) / 256)
    //   phase/span = remainder/256 (reversed on odd lobes).
    let lobes = 2 * q + 1;
    let product = index * lobes;
    let lobe = product >> 8;
    let remainder = product & 0xff;
    let phase_units = if lobe & 1 != 0 {
        256 - remainder
    } else {
        remainder
    };
    if extra_control == 0 {
        return phase_units as u16;
    }
    let amplitude = extra_control.clamp(0, 256);
    ((phase_units * amplitude) >> 8).clamp(0, 256) as u16
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeGraphResource {
    pub(crate) key: String,
    pub(crate) source_rect: Option<RuntimeClipRect>,
    pub(crate) generation: u64,
}

#[derive(Debug, Clone, Copy)]
pub(crate) struct RuntimeGraphTransitionNode {
    pub(crate) primary_resource: i32,
    pub(crate) secondary_resource: i32,
    /// CDspObjSprite mode-1 transition value stored at +0x240 (this[144]).
    /// The sprite draw path uses it to combine the primary and secondary
    /// bitmaps before the base CDspObj transparency is applied.
    pub(crate) alpha_multiplier: i32,
    /// Base CDspObj transparency parameter stored independently at +0xAC.
    pub(crate) alpha_parameter: i32,
    /// Target reverse-pop argument forwarded as display priority by
    /// `sub_47C470`.
    pub(crate) render_priority: i32,
    /// Raw native blend selector supplied to the transition constructor.
    pub(crate) blend_selector: i32,
}

impl RuntimeGraphResource {
    pub(crate) fn whole(key: String) -> Self {
        static NEXT_GENERATION: AtomicU64 = AtomicU64::new(1);
        Self {
            key,
            source_rect: None,
            generation: NEXT_GENERATION.fetch_add(1, Ordering::Relaxed),
        }
    }

    pub(crate) fn subregion(&self, x: i32, y: i32, width: i32, height: i32) -> Self {
        let (base_x, base_y) = self
            .source_rect
            .map(|rect| (rect.x, rect.y))
            .unwrap_or_default();
        Self {
            key: self.key.clone(),
            source_rect: Some(RuntimeClipRect {
                x: base_x + x as f32,
                y: base_y + y as f32,
                width: width.max(0) as f32,
                height: height.max(0) as f32,
            }),
            generation: self.generation,
        }
    }
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeGraphLayer {
    pub(crate) hit_id: i32,
    pub(crate) owner_object: Option<i32>,
    pub(crate) key: String,
    pub(crate) target_surface: Option<i32>,
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
    pub(crate) src_x: f32,
    pub(crate) src_y: f32,
    pub(crate) opacity: f32,
    pub(crate) z: i32,
    pub(crate) enabled: bool,
    pub(crate) transform_x: f32,
    pub(crate) transform_y: f32,
    pub(crate) transform_z: i32,
    pub(crate) scale_x: f32,
    pub(crate) scale_y: f32,
    pub(crate) rotation_degrees: f32,
    pub(crate) clip: Option<RuntimeClipRect>,
}

impl RuntimeGraphLayer {
    pub(crate) fn screen_x(&self, surfaces: &BTreeMap<i32, RuntimeSurface>) -> f32 {
        self.target_surface
            .and_then(|surface| surfaces.get(&surface))
            .map(|surface| surface.x + self.x)
            .unwrap_or(self.x)
    }

    pub(crate) fn screen_y(&self, surfaces: &BTreeMap<i32, RuntimeSurface>) -> f32 {
        self.target_surface
            .and_then(|surface| surfaces.get(&surface))
            .map(|surface| surface.y + self.y)
            .unwrap_or(self.y)
    }

    pub(crate) fn screen_z(&self) -> i32 {
        self.z.saturating_add(self.transform_z)
    }
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeSurface {
    pub(crate) id: i32,
    pub(crate) display_attached: bool,
    pub(crate) width: f32,
    pub(crate) height: f32,
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) parent_surface: Option<i32>,
    pub(crate) local_x: f32,
    pub(crate) local_y: f32,
    pub(crate) viewport_x: f32,
    pub(crate) viewport_y: f32,
    pub(crate) viewport_width: f32,
    pub(crate) viewport_height: f32,
    /// CDspObjWindow+0x1A0..+0x1AC inclusive valid/content rectangle. This is
    /// independent from bitmap source cropping (`viewport_*`). Compact
    /// DCIPIcon construction adds valid_left/valid_top to each item offset.
    pub(crate) valid_left: i32,
    pub(crate) valid_top: i32,
    pub(crate) valid_right: i32,
    pub(crate) valid_bottom: i32,
    pub(crate) resource_id: Option<i32>,
    /// CDspObjWindow+0x3C0/+0x3C4/+0x3C8. The target stores one of six
    /// permutations of the three window composition passes.
    pub(crate) composition_order: [i32; 3],
    /// Native blend selector written by Graph90:85.
    pub(crate) blend_mode: i32,
    /// CDspObjWindow+956. When set, the backing bitmap and child layers are
    /// composed as one isolated group before the result reaches its parent.
    pub(crate) isolated_group: bool,
    /// CDspObjWindow+864. Native text layout adds this percentage of the font
    /// height to each line advance; accepted values are 0..=800.
    pub(crate) line_spacing_percent: i32,
    /// CDspObjWindow+880, set through Graph91:8A. Value 1 selects the target
    /// CProcDspMsgExVE subclass for Graph92:90; value 0 selects CProcDspMsgEx.
    pub(crate) message_variant: i32,
    pub(crate) enabled: bool,
    pub(crate) opacity: f32,
    pub(crate) z: i32,
}

impl RuntimeSurface {
    pub(crate) fn bitmap(id: i32, width: f32, height: f32) -> Self {
        Self {
            id,
            display_attached: false,
            width,
            height,
            x: 0.0,
            y: 0.0,
            parent_surface: None,
            local_x: 0.0,
            local_y: 0.0,
            viewport_x: 0.0,
            viewport_y: 0.0,
            viewport_width: width,
            viewport_height: height,
            valid_left: 0,
            valid_top: 0,
            valid_right: width.max(1.0).round() as i32 - 1,
            valid_bottom: height.max(1.0).round() as i32 - 1,
            resource_id: None,
            composition_order: [0, 1, 2],
            blend_mode: 0,
            isolated_group: false,
            line_spacing_percent: 0,
            message_variant: 0,
            enabled: true,
            opacity: 1.0,
            z: NATIVE_DISPLAY_Z,
        }
    }

    pub(crate) fn display(id: i32, width: f32, height: f32) -> Self {
        Self {
            display_attached: true,
            ..Self::bitmap(id, width, height)
        }
    }
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeGraphDrawItem {
    pub(crate) owner_object: Option<i32>,
    pub(crate) key: String,
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
    pub(crate) src_x: f32,
    pub(crate) src_y: f32,
    pub(crate) src_width: f32,
    pub(crate) src_height: f32,
    pub(crate) opacity: f32,
    /// Target bitmap format 1 is XRGB: byte 3 is software-blitter payload,
    /// not source-alpha coverage. The GPU renderer must therefore ignore the
    /// sampled alpha byte for these draw items while still applying object
    /// transparency through `opacity`.
    pub(crate) ignore_source_alpha: bool,
    pub(crate) rotation_degrees: f32,
    /// Optional target mode-5 affine footprint in final game coordinates,
    /// ordered TL, BL, BR, TR. When present, the renderer must use these
    /// vertices directly instead of rotating/stretching the raster bbox.
    pub(crate) destination_quad: Option<[[f32; 2]; 4]>,
    /// Native mode-5 interpolation selector: false = nearest, true = bilinear.
    /// Non-mode-5 draw items retain the renderer's historical linear sampling.
    pub(crate) linear_sampling: bool,
    pub(crate) clip: Option<RuntimeClipRect>,
    pub(crate) z: i32,
    /// Native CDspObj blend selector. The renderer maps the recovered common
    /// modes to separate GPU pipelines instead of flattening every object into
    /// ordinary source-alpha blending.
    pub(crate) blend_mode: i32,
    /// CObjectManager preserves insertion order for equal-depth objects. The
    /// final target traversal direction is still unrecovered, so this serial
    /// records chain position only; it is independent of a script-visible
    /// handle, which may be reused or non-monotonic.
    pub(crate) order_serial: u64,
    pub(crate) hit_id: i32,
}

fn legacy_reverse_equal_depth_order() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("ETHORNELL_LEGACY_REVERSE_EQUAL_DEPTH").is_some())
}

/// Sort key for the recovered display chain.
///
/// The target insertion helper places a new object after existing objects with
/// the same depth.  Until the final traversal loop is closed, strict mode
/// preserves that chain order instead of reversing it.  The former port order
/// remains available only for visual A/B comparison.
pub(crate) fn native_draw_order(item: &RuntimeGraphDrawItem) -> (i32, u64, i32) {
    let order = if legacy_reverse_equal_depth_order() {
        u64::MAX - item.order_serial
    } else {
        item.order_serial
    };
    let hit = if legacy_reverse_equal_depth_order() {
        i32::MAX.saturating_sub(item.hit_id)
    } else {
        item.hit_id
    };
    (item.z, order, hit)
}

#[derive(Debug, Clone, Copy)]
pub(crate) struct RuntimeClipRect {
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
}

impl RuntimeClipRect {
    pub(crate) fn intersection(self, other: Self) -> Self {
        let x0 = self.x.max(other.x);
        let y0 = self.y.max(other.y);
        let x1 = (self.x + self.width).min(other.x + other.width);
        let y1 = (self.y + self.height).min(other.y + other.height);
        Self {
            x: x0,
            y: y0,
            width: (x1 - x0).max(0.0),
            height: (y1 - y0).max(0.0),
        }
    }

    pub(crate) fn is_empty(self) -> bool {
        self.width <= 0.0 || self.height <= 0.0
    }
}

pub(crate) fn crop_decoded_image(image: &DecodedImage, region: RuntimeClipRect) -> DecodedImage {
    let x = region.x.max(0.0) as u32;
    let y = region.y.max(0.0) as u32;
    let width = (region.width.max(0.0) as u32).min(image.width.saturating_sub(x));
    let height = (region.height.max(0.0) as u32).min(image.height.saturating_sub(y));
    let mut cropped = DecodedImage {
        width,
        height,
        rgba: vec![0; width as usize * height as usize * 4],
    };
    for row in 0..height as usize {
        let src_start = ((y as usize + row) * image.width as usize + x as usize) * 4;
        let dst_start = row * width as usize * 4;
        let byte_len = width as usize * 4;
        cropped.rgba[dst_start..dst_start + byte_len]
            .copy_from_slice(&image.rgba[src_start..src_start + byte_len]);
    }
    cropped
}

fn legacy_mask_luminance_fallback() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("ETHORNELL_LEGACY_MASK_LUMA").is_some())
}

/// Applies the currently recovered alpha-mask portion of the native graph
/// compositor.  Strict mode samples only the mask alpha channel.  The old
/// luma fallback is opt-in because the target selector/channel rule has not
/// yet been closed.
pub(crate) fn apply_alpha_mask(source: &DecodedImage, mask: &DecodedImage) -> Option<DecodedImage> {
    if source.width == 0 || source.height == 0 || mask.width == 0 || mask.height == 0 {
        return None;
    }
    let use_legacy_luminance_fallback = legacy_mask_luminance_fallback();
    let mut out = source.clone();
    for y in 0..source.height {
        let mask_y = (u64::from(y) * u64::from(mask.height) / u64::from(source.height)) as u32;
        for x in 0..source.width {
            let mask_x = (u64::from(x) * u64::from(mask.width) / u64::from(source.width)) as u32;
            let source_index = ((y * source.width + x) * 4) as usize;
            let mask_index = ((mask_y * mask.width + mask_x) * 4) as usize;
            let alpha = mask.rgba[mask_index + 3];
            let coverage = if use_legacy_luminance_fallback && alpha == 255 {
                // Compatibility with the former port.  The target mask mode
                // selector and RGB-channel formula have not been recovered,
                // so strict mode never silently substitutes luma for alpha.
                let red = u16::from(mask.rgba[mask_index]);
                let green = u16::from(mask.rgba[mask_index + 1]);
                let blue = u16::from(mask.rgba[mask_index + 2]);
                ((red * 77 + green * 150 + blue * 29) >> 8) as u8
            } else {
                alpha
            };
            out.rgba[source_index + 3] =
                ((u16::from(out.rgba[source_index + 3]) * u16::from(coverage)) / 255) as u8;
        }
    }
    Some(out)
}

pub(crate) fn fit_decoded_image_canvas(
    image: &DecodedImage,
    width: u32,
    height: u32,
) -> DecodedImage {
    if image.width == width && image.height == height {
        return image.clone();
    }
    let mut fitted = DecodedImage {
        width,
        height,
        rgba: vec![0; width as usize * height as usize * 4],
    };
    blit_decoded_image(&mut fitted, image, 0, 0, 128);
    fitted
}

pub(crate) fn blit_decoded_image(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    destination_x: i32,
    destination_y: i32,
    mode: i32,
) {
    for source_y in 0..source.height as i32 {
        let target_y = destination_y + source_y;
        if !(0..destination.height as i32).contains(&target_y) {
            continue;
        }
        for source_x in 0..source.width as i32 {
            let target_x = destination_x + source_x;
            if !(0..destination.width as i32).contains(&target_x) {
                continue;
            }
            let source_index = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let target_index =
                (target_y as usize * destination.width as usize + target_x as usize) * 4;
            let source_pixel: [u8; 4] = source.rgba[source_index..source_index + 4]
                .try_into()
                .expect("RGBA source pixel");
            let target_pixel: &mut [u8; 4] = (&mut destination.rgba
                [target_index..target_index + 4])
                .try_into()
                .expect("RGBA destination pixel");
            composite_native_rgba(target_pixel, source_pixel, 1.0, mode);
        }
    }
}

pub(crate) fn blit_decoded_image_parameter(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    destination_x: i32,
    destination_y: i32,
    mode: i32,
    alpha_parameter: i32,
) {
    let global_alpha = alpha_parameter.clamp(0, 256) as u32;
    for source_y in 0..source.height as i32 {
        let target_y = destination_y + source_y;
        if !(0..destination.height as i32).contains(&target_y) {
            continue;
        }
        for source_x in 0..source.width as i32 {
            let target_x = destination_x + source_x;
            if !(0..destination.width as i32).contains(&target_x) {
                continue;
            }
            let source_index = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let target_index =
                (target_y as usize * destination.width as usize + target_x as usize) * 4;
            let source_pixel: [u8; 4] = source.rgba[source_index..source_index + 4]
                .try_into()
                .expect("RGBA source pixel");
            let target_pixel: &mut [u8; 4] = (&mut destination.rgba
                [target_index..target_index + 4])
                .try_into()
                .expect("RGBA destination pixel");
            composite_native_rgba(
                target_pixel,
                source_pixel,
                global_alpha as f32 / 256.0,
                mode,
            );
        }
    }
}

/// Native format-2 selector 1 compositor recovered from the target's
/// `sub_40B200` path. BGI stores straight (unpremultiplied) RGB beside alpha;
/// therefore a partial source alpha must normalize RGB by the resulting alpha
/// instead of writing premultiplied-looking channel values into the bitmap.
pub(crate) fn blit_decoded_image_format2_source_over(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    destination_x: i32,
    destination_y: i32,
    transparency: i32,
) {
    let coverage = (256 - transparency.clamp(0, 256)) as u32;
    for source_y in 0..source.height as i32 {
        let target_y = destination_y + source_y;
        if !(0..destination.height as i32).contains(&target_y) {
            continue;
        }
        for source_x in 0..source.width as i32 {
            let target_x = destination_x + source_x;
            if !(0..destination.width as i32).contains(&target_x) {
                continue;
            }
            let source_index = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let target_index =
                (target_y as usize * destination.width as usize + target_x as usize) * 4;
            let source_pixel = &source.rgba[source_index..source_index + 4];
            let destination_pixel = &mut destination.rgba[target_index..target_index + 4];

            let source_alpha = (u32::from(source_pixel[3]) * coverage) >> 8;
            if source_alpha == 0 {
                continue;
            }
            if source_alpha == 255 {
                destination_pixel.copy_from_slice(source_pixel);
                continue;
            }

            let destination_alpha = u32::from(destination_pixel[3]);
            // sub_40B200 keeps the target's 1/256 arithmetic here rather
            // than converting the byte alpha to a host float.
            let destination_coverage = (256 - source_alpha) * destination_alpha;
            let source_coverage = source_alpha << 8;
            let total_coverage = destination_coverage + source_coverage;
            if total_coverage == 0 {
                destination_pixel.fill(0);
                continue;
            }

            // The native routine obtains two 8.8 weights by integer division
            // and then combines straight RGB with those quantized weights.
            let destination_weight = (destination_coverage << 8) / total_coverage;
            let source_weight = (source_alpha << 16) / total_coverage;
            for channel in 0..3 {
                let mixed = u32::from(destination_pixel[channel]) * destination_weight
                    + u32::from(source_pixel[channel]) * source_weight;
                destination_pixel[channel] = ((mixed >> 8).min(255)) as u8;
            }
            destination_pixel[3] = ((total_coverage >> 8).min(255)) as u8;
        }
    }
}

/// Native selector-128 format-1 -> format-2 conversion in `sub_40AF50`.
/// Format 1 carries RGB but no source-alpha semantics; when copied into a
/// format-2 bitmap the target preserves RGB and forces the destination alpha
/// byte to 255 (`pixel | 0xFF000000`).
pub(crate) fn blit_decoded_image_format1_to_format2(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    destination_x: i32,
    destination_y: i32,
) {
    for source_y in 0..source.height as i32 {
        let target_y = destination_y + source_y;
        if !(0..destination.height as i32).contains(&target_y) {
            continue;
        }
        for source_x in 0..source.width as i32 {
            let target_x = destination_x + source_x;
            if !(0..destination.width as i32).contains(&target_x) {
                continue;
            }
            let source_index = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let target_index =
                (target_y as usize * destination.width as usize + target_x as usize) * 4;
            destination.rgba[target_index..target_index + 3]
                .copy_from_slice(&source.rgba[source_index..source_index + 3]);
            destination.rgba[target_index + 3] = 0xff;
        }
    }
}

/// Native same-format selector 128 path (`sub_40AF50 -> sub_40ADF0`).  This
/// is a byte/pixel replacement after clipping, not source-over compositing.
pub(crate) fn blit_decoded_image_raw_copy(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    destination_x: i32,
    destination_y: i32,
) {
    for source_y in 0..source.height as i32 {
        let target_y = destination_y + source_y;
        if !(0..destination.height as i32).contains(&target_y) {
            continue;
        }
        for source_x in 0..source.width as i32 {
            let target_x = destination_x + source_x;
            if !(0..destination.width as i32).contains(&target_x) {
                continue;
            }
            let source_index = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let target_index =
                (target_y as usize * destination.width as usize + target_x as usize) * 4;
            destination.rgba[target_index..target_index + 4]
                .copy_from_slice(&source.rgba[source_index..source_index + 4]);
        }
    }
}

pub(crate) fn crossfade_decoded_images(
    primary: &DecodedImage,
    secondary: &DecodedImage,
    alpha_parameter: i32,
) -> DecodedImage {
    let width = primary.width.min(secondary.width);
    let height = primary.height.min(secondary.height);
    let secondary_weight = alpha_parameter.clamp(0, 256) as u32;
    let primary_weight = 256 - secondary_weight;
    let mut rgba = vec![0; width as usize * height as usize * 4];
    for y in 0..height as usize {
        for x in 0..width as usize {
            let primary_offset = (y * primary.width as usize + x) * 4;
            let secondary_offset = (y * secondary.width as usize + x) * 4;
            let destination_offset = (y * width as usize + x) * 4;
            for channel in 0..4 {
                let value = u32::from(primary.rgba[primary_offset + channel]) * primary_weight
                    + u32::from(secondary.rgba[secondary_offset + channel]) * secondary_weight;
                rgba[destination_offset + channel] = ((value + 128) >> 8) as u8;
            }
        }
    }
    DecodedImage {
        width,
        height,
        rgba,
    }
}

pub(crate) fn scale_decoded_image_fixed(
    source: &DecodedImage,
    scale_x: i32,
    scale_y: i32,
) -> Option<DecodedImage> {
    let width = ((u64::from(source.width) * scale_x.max(0) as u64) >> 16) as u32;
    let height = ((u64::from(source.height) * scale_y.max(0) as u64) >> 16) as u32;
    if width == 0 || height == 0 {
        return None;
    }

    let mut rgba = vec![0; width as usize * height as usize * 4];
    for y in 0..height {
        let source_y = ((u64::from(y) << 16) / scale_y as u64)
            .min(u64::from(source.height.saturating_sub(1))) as u32;
        for x in 0..width {
            let source_x = ((u64::from(x) << 16) / scale_x as u64)
                .min(u64::from(source.width.saturating_sub(1))) as u32;
            let source_offset = (source_y as usize * source.width as usize + source_x as usize) * 4;
            let destination_offset = (y as usize * width as usize + x as usize) * 4;
            rgba[destination_offset..destination_offset + 4]
                .copy_from_slice(&source.rgba[source_offset..source_offset + 4]);
        }
    }
    Some(DecodedImage {
        width,
        height,
        rgba,
    })
}

pub(crate) fn blend_decoded_image_parameter(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    alpha_parameter: i32,
) {
    let source_weight = alpha_parameter.clamp(0, 256) as u32;
    let destination_weight = 256 - source_weight;
    let width = destination.width.min(source.width);
    let height = destination.height.min(source.height);
    for y in 0..height as usize {
        for x in 0..width as usize {
            let destination_offset = (y * destination.width as usize + x) * 4;
            let source_offset = (y * source.width as usize + x) * 4;
            for channel in 0..4 {
                let value = u32::from(destination.rgba[destination_offset + channel])
                    * destination_weight
                    + u32::from(source.rgba[source_offset + channel]) * source_weight;
                destination.rgba[destination_offset + channel] = ((value + 128) >> 8) as u8;
            }
        }
    }
}

#[derive(Debug, Clone, Copy)]
pub(crate) struct RuntimeUserControl {
    pub(crate) id: i32,
    pub(crate) owner_id: i32,
    pub(crate) payload: i32,
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
    pub(crate) normal_resource: i32,
    pub(crate) selected_resource: i32,
    pub(crate) enabled: bool,
    pub(crate) title_only: bool,
}

impl Default for RuntimeUserControl {
    fn default() -> Self {
        Self {
            id: 0,
            owner_id: 0,
            payload: 0,
            x: 0.0,
            y: 0.0,
            width: 1.0,
            height: 1.0,
            normal_resource: -1,
            selected_resource: -1,
            enabled: true,
            title_only: false,
        }
    }
}

impl RuntimeUserControl {
    pub(crate) fn contains(&self, point: (f32, f32), title_active: bool) -> bool {
        self.enabled
            && (!self.title_only || title_active)
            && point.0 >= self.x
            && point.0 < self.x + self.width
            && point.1 >= self.y
            && point.1 < self.y + self.height
    }
}

pub(crate) fn fixed_16_to_f32(value: i32) -> f32 {
    value as f32 / 65_536.0
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct NativeMode5NodeArgs {
    pub(crate) node_id: i32,
    pub(crate) position_x: i32,
    pub(crate) position_y: i32,
    pub(crate) position_z: i32,
    pub(crate) resource_id: i32,
    pub(crate) anchor_x: i32,
    pub(crate) anchor_y: i32,
    pub(crate) rotation: i32,
    pub(crate) perspective: i32,
    /// CDspObjSprite+0x27C.  sub_429AF0 uses this as the gate for
    /// perspective-projecting the resolved X/Y position by the Z scale.
    pub(crate) project_position: bool,
    /// CDspObjSprite+0x280. sub_4258D0 passes this to sub_417730: zero
    /// selects the nearest-neighbour rasterizer (sub_418280), nonzero selects
    /// the bilinear rasterizer (sub_417C50).
    pub(crate) interpolation: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct NativeMode5DynamicState {
    /// CDspObj+0xB8. CProcCtrlDspObj writes 8.24 progress here when the
    /// fixed-parameter selector is 1.
    pub(crate) progress_8_24: i32,
    /// Sprite properties 0x80/0x81/0x82/0x8F (sub_4285A0).
    pub(crate) offset_delta_x_16_16: i32,
    pub(crate) offset_delta_y_16_16: i32,
    pub(crate) rotation_delta_16_16: i32,
    pub(crate) scale_delta_x_16_16: i32,
    pub(crate) scale_delta_y_16_16: i32,
    pub(crate) uniform_scale_delta_16_16: i32,
    pub(crate) curve: i32,
    /// Sprite property 0x42 / sub_428130. Defaults are 1.0, 1.0.
    pub(crate) base_scale_x_16_16: i32,
    pub(crate) base_scale_y_16_16: i32,
    pub(crate) coupled_scale: bool,
    /// Sprite+0x308 width-adjust parameter used by sub_429220. Property 0x60
    /// supplies it while its packed high-word gate is non-zero.
    pub(crate) width_adjust_16_16: i32,
}

impl Default for NativeMode5DynamicState {
    fn default() -> Self {
        Self {
            progress_8_24: 0,
            offset_delta_x_16_16: 0,
            offset_delta_y_16_16: 0,
            rotation_delta_16_16: 0,
            scale_delta_x_16_16: 0,
            scale_delta_y_16_16: 0,
            uniform_scale_delta_16_16: 0,
            curve: 0,
            base_scale_x_16_16: 0x10000,
            base_scale_y_16_16: 0x10000,
            coupled_scale: false,
            width_adjust_16_16: 0,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub(crate) struct NativeMode5Geometry {
    /// Raster top-left returned by sub_4281E0 after the ordinary CDspObj
    /// position (including its integer offset banks) has the mode-5 raster
    /// origin removed. The bottom-up convention belongs to the affine source
    /// transform; it does not invert the display object's screen Y axis.
    pub(crate) x: f32,
    pub(crate) y: f32,
    /// Ordinary CDspObj pixel position written by sub_429AF0 through
    /// vtable+40 before the raster-origin offset is applied.
    pub(crate) object_x: f32,
    pub(crate) object_y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
    /// CDspObjSprite+0x2DC, subtracted from resolved object X by sub_4281E0.
    pub(crate) origin_offset_x: f32,
    /// CDspObjSprite+0x2E0, subtracted from resolved object Y by sub_4281E0.
    pub(crate) origin_offset_y: f32,
    pub(crate) scale: f32,
    pub(crate) rotation_degrees: f32,
    /// Forward affine image footprint, relative to the raster top-left. The
    /// points are source-edge TL, BL, BR, TR. The target does not stretch the
    /// bitmap to `width`/`height`: those values are only the inclusive raster
    /// bounding rectangle used for clipping. sub_416750 performs the inverse
    /// mapping for each destination pixel; this quad is the algebraic inverse
    /// of that mapping.
    pub(crate) local_affine_quad: [[f32; 2]; 4],
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub(crate) struct RuntimeMode5RenderState {
    pub(crate) local_affine_quad: [[f32; 2]; 4],
    pub(crate) linear_sampling: bool,
}

impl NativeMode5NodeArgs {
    pub(crate) fn from_source_args(values: &[i32]) -> Option<Self> {
        Some(Self {
            node_id: *values.first()?,
            position_x: *values.get(1)?,
            position_y: *values.get(2)?,
            position_z: *values.get(3)?,
            resource_id: *values.get(4)?,
            anchor_x: *values.get(8)?,
            anchor_y: *values.get(9)?,
            rotation: *values.get(10)?,
            perspective: *values.get(11)?,
            project_position: *values.get(12)? != 0,
            interpolation: *values.get(13)? != 0,
        })
    }

    fn perspective_scale_fixed(self) -> u32 {
        let perspective = self.perspective.max(0) as u64;
        if perspective == 0 || self.position_z == 0 {
            return 0x1_0000;
        }
        if self.position_z < 0 {
            (((perspective << 16).saturating_add(self.position_z.unsigned_abs() as u64))
                / perspective)
                .min(u32::MAX as u64) as u32
        } else {
            ((perspective << 32) / ((self.position_z as u64).saturating_add(perspective << 16)))
                .min(u32::MAX as u64) as u32
        }
    }

    /// The five values sub_4296C0 derives from the sprite's mode-5 state:
    /// base offset X/Y (16.16) plus the 0x80 deltas, rotation with the 0x81
    /// delta through the 0x8F curve, and the two 16.16 scales. Graph91:38
    /// parameter 0x10000000 returns exactly these.
    pub(crate) fn mode5_transform_parameters(self, dynamic: NativeMode5DynamicState) -> [i32; 5] {
        #[inline]
        fn mul_progress(delta: i32, progress: i32) -> i32 {
            (((i128::from(delta) * i128::from(progress as u32)) >> 24) as i64) as i32
        }
        #[inline]
        fn mul_fixed_signed(value: i32, scale: u32) -> i32 {
            (((i128::from(value) * i128::from(scale)) >> 16) as i64) as i32
        }
        let perspective_scale_fixed = self.perspective_scale_fixed();
        let progress = dynamic.progress_8_24.clamp(0, 0x0100_0000);

        // sub_4296C0.  The base offset pair is stored at Sprite+0x248/0x24C
        // after sub_427AA0 shifts the script's integer values by 16.
        let mut offset_x_16_16 = self.anchor_x.wrapping_shl(16);
        let mut offset_y_16_16 = self.anchor_y.wrapping_shl(16);
        offset_x_16_16 =
            offset_x_16_16.wrapping_add(mul_progress(dynamic.offset_delta_x_16_16, progress));
        offset_y_16_16 =
            offset_y_16_16.wrapping_add(mul_progress(dynamic.offset_delta_y_16_16, progress));

        let curve_weight = crate::animation::target_curve_fixed_16(dynamic.curve, progress);
        let rotation = self.rotation.wrapping_add(
            (((i128::from(dynamic.rotation_delta_16_16) * i128::from(curve_weight)) >> 16) as i64)
                as i32,
        );

        let scale_x_delta = mul_progress(dynamic.scale_delta_x_16_16, progress);
        let scale_y_delta = mul_progress(dynamic.scale_delta_y_16_16, progress);
        let uniform_delta = mul_progress(dynamic.uniform_scale_delta_16_16, progress);
        let (scale_x_fixed, scale_y_fixed) = if dynamic.coupled_scale {
            let sx = dynamic.base_scale_x_16_16.wrapping_add(mul_progress(
                dynamic
                    .uniform_scale_delta_16_16
                    .wrapping_add(dynamic.scale_delta_x_16_16),
                progress,
            ));
            let sy = dynamic.base_scale_y_16_16.wrapping_add(mul_progress(
                dynamic
                    .uniform_scale_delta_16_16
                    .wrapping_add(dynamic.scale_delta_y_16_16),
                progress,
            ));
            (
                mul_fixed_signed(sx, perspective_scale_fixed) as u32,
                mul_fixed_signed(sy, perspective_scale_fixed) as u32,
            )
        } else {
            let px = (perspective_scale_fixed as i64 + i64::from(scale_x_delta)) as i32;
            let py = (perspective_scale_fixed as i64 + i64::from(scale_y_delta)) as i32;
            let bx = dynamic.base_scale_x_16_16.wrapping_add(uniform_delta);
            let by = dynamic.base_scale_y_16_16.wrapping_add(uniform_delta);
            (
                mul_fixed_signed(px, bx as u32) as u32,
                mul_fixed_signed(py, by as u32) as u32,
            )
        };

        [
            offset_x_16_16,
            offset_y_16_16,
            rotation,
            scale_x_fixed as i32,
            scale_y_fixed as i32,
        ]
    }

    /// Exact mode-5 projection recovered from
    /// sub_4296C0 -> sub_429220 -> sub_429AF0.  The constructor arguments at
    /// indices 8/9 are the bitmap-origin coordinates consumed by the native
    /// bottom-up DIB transform (stored as 16.16 by sub_427AA0).
    pub(crate) fn screen_geometry_mode5_exact(
        self,
        image_width: f32,
        image_height: f32,
        viewport_width: f32,
        viewport_height: f32,
        graph_center: (i32, i32),
        use_graph_center: bool,
        dynamic: NativeMode5DynamicState,
    ) -> NativeMode5Geometry {
        #[inline]
        fn mul_progress(delta: i32, progress: i32) -> i32 {
            (((i128::from(delta) * i128::from(progress as u32)) >> 24) as i64) as i32
        }
        #[inline]
        fn mul_fixed_signed(value: i32, scale: u32) -> i32 {
            (((i128::from(value) * i128::from(scale)) >> 16) as i64) as i32
        }
        #[inline]
        fn scaled_fraction_16(value: i32, scale: u32) -> f64 {
            // 0x429535/0x429575 call the target's signed 32x32 -> 64 helper,
            // SHRD by 16, then AND 0xffff.  Only the fractional word affects
            // the raster bound rounding.
            let product = i128::from(value) * i128::from(scale);
            let shifted = product >> 16;
            (shifted as i64 as u64 & 0xffff) as f64 / 65_536.0
        }

        let perspective_scale_fixed = self.perspective_scale_fixed();
        let perspective_scale = perspective_scale_fixed as f64 / 65_536.0;
        let [
            offset_x_16_16,
            offset_y_16_16,
            rotation,
            scale_x_fixed,
            scale_y_fixed,
        ] = self.mode5_transform_parameters(dynamic);
        let (scale_x_fixed, scale_y_fixed) = (scale_x_fixed as u32, scale_y_fixed as u32);

        // sub_429220.  BGI uses bottom-up DIB coordinates and deliberately
        // mixes floor for X with ceil for Y. Preserve that asymmetry.
        let width = image_width as f64;
        let height = image_height as f64;
        let adjusted_width =
            width * (f64::from(dynamic.width_adjust_16_16.wrapping_add(0x10000))) / 65_536.0;
        let offset_x = (adjusted_width - width) * 0.5 + f64::from(offset_x_16_16) / 65_536.0;
        let offset_y = f64::from(offset_y_16_16) / 65_536.0;
        let radians = f64::from(rotation) * std::f64::consts::PI / 11_796_480.0;
        let (sin, cos) = radians.sin_cos();
        let sx = scale_x_fixed as f64 / 65_536.0;
        let sy = scale_y_fixed as f64 / 65_536.0;
        let right = adjusted_width - offset_x - 1.0;
        let left = -offset_x;
        let bottom = offset_y * sy;
        let top = offset_y - height + 1.0;
        let corners = [
            (
                left * sx * cos - bottom * sin,
                left * sx * sin + bottom * cos,
            ),
            (
                right * sx * cos - bottom * sin,
                right * sx * sin + bottom * cos,
            ),
            (
                left * sx * cos - top * sy * sin,
                left * sx * sin + top * sy * cos,
            ),
            (
                right * sx * cos - top * sy * sin,
                right * sx * sin + top * sy * cos,
            ),
        ];
        let mut min_x = 1_000_000_000.0_f64;
        let mut max_x = -1_000_000_000.0_f64;
        let mut min_y = 1_000_000_000.0_f64;
        let mut max_y = -1_000_000_000.0_f64;
        for (x, y) in corners {
            min_x = min_x.min(x);
            max_x = max_x.max(x);
            min_y = min_y.min(y);
            max_y = max_y.max(y);
        }
        let max_x_adjusted = max_x + scaled_fraction_16(self.position_x, scale_x_fixed);
        let min_y_adjusted = min_y - scaled_fraction_16(self.position_y, scale_y_fixed);
        let min_x_floor = min_x.floor();
        let max_x_floor = max_x_adjusted.floor();
        let max_y_ceil = max_y.ceil();
        let min_y_ceil = min_y_adjusted.ceil();
        let raster_width = max_x_floor - min_x_floor + 1.0;
        let raster_height = max_y_ceil - min_y_ceil + 1.0;
        let origin_offset_x = -min_x_floor;
        let origin_offset_y = max_y_ceil;

        // sub_429AF0. The ordinary object position is centered in the display
        // (or Graph90:06 centre when enabled/valid) and only then has the
        // transformed-raster origin removed by sub_4281E0.
        let viewport_width_i32 = viewport_width.max(0.0).floor().min(i32::MAX as f32) as i32;
        let viewport_height_i32 = viewport_height.max(0.0).floor().min(i32::MAX as f32) as i32;
        let valid_graph_center = use_graph_center
            && graph_center.0 >= 0
            && graph_center.1 >= 0
            && graph_center.0 < viewport_width_i32
            && graph_center.1 < viewport_height_i32;
        let (center_x, center_y) = if valid_graph_center {
            (graph_center.0, graph_center.1)
        } else {
            (viewport_width_i32 >> 1, viewport_height_i32 >> 1)
        };
        let projected_x = if self.project_position {
            mul_fixed_signed(self.position_x, perspective_scale_fixed)
        } else {
            self.position_x
        };
        let projected_y = if self.project_position {
            mul_fixed_signed(self.position_y, perspective_scale_fixed)
        } else {
            self.position_y
        };
        let object_x_fixed = (center_x << 16).wrapping_add(projected_x);
        let object_y_fixed = (center_y << 16).wrapping_add(projected_y);
        let object_x = object_x_fixed >> 16;
        let object_y = object_y_fixed >> 16;

        // sub_4258D0 case 5 passes
        //   frac + ((raster_origin - local_clip) << 16)
        // to sub_417730. sub_416750 then inverse-maps every destination pixel
        // back into source coordinates. Algebraically inverting that mapping
        // gives the source-pixel-centre -> raster-local transform below. Keep
        // the 16-bit ordinary-position remainder: sub_429AF0 stores it at
        // Sprite+0x2A4/+0x2A8 and the case-5 rasterizer consumes it.
        let fractional_x = f64::from((object_x_fixed as u32 & 0xffff) as u16) / 65_536.0;
        let fractional_y = f64::from((object_y_fixed as u32 & 0xffff) as u16) / 65_536.0;
        let anchor_x = f64::from(offset_x_16_16) / 65_536.0;
        let anchor_y = f64::from(offset_y_16_16) / 65_536.0;
        let map_source_edge = |source_x: f64, source_y: f64| -> [f32; 2] {
            let source_dx = (source_x - anchor_x) * sx;
            let source_dy = (source_y - anchor_y) * sy;
            [
                (origin_offset_x + fractional_x + source_dx * cos + source_dy * sin + 0.5) as f32,
                (origin_offset_y + fractional_y - source_dx * sin + source_dy * cos + 0.5) as f32,
            ]
        };
        // The native rasterizer addresses destination pixels by integer pixel
        // centre (local 0 means the centre of the first pixel), whereas GPU
        // vertex coordinates address pixel edges (the first fragment centre is
        // at 0.5). The +0.5 above is only that coordinate-system bridge; it is
        // not a target half-pixel correction. Texture UV 0/1 are texel edges,
        // while native source integer coordinates denote texel centres, so the
        // source footprint expands by half a texel. The native raster bbox is
        // still applied as a scissor and remains authoritative.
        let local_affine_quad = [
            map_source_edge(-0.5, -0.5),
            map_source_edge(-0.5, height - 0.5),
            map_source_edge(width - 0.5, height - 0.5),
            map_source_edge(width - 0.5, -0.5),
        ];

        // sub_4281E0 calls sub_41B260 to resolve the ordinary screen
        // position, then subtracts Sprite+0x2DC/+0x2E0.  Do not flip this Y:
        // the target's bottom-up DIB convention is consumed by the affine
        // rasterizer (sub_417730/sub_416750), not by CDspObj screen placement.
        NativeMode5Geometry {
            x: object_x as f32 - origin_offset_x as f32,
            y: object_y as f32 - origin_offset_y as f32,
            object_x: object_x as f32,
            object_y: object_y as f32,
            width: raster_width as f32,
            height: raster_height as f32,
            origin_offset_x: origin_offset_x as f32,
            origin_offset_y: origin_offset_y as f32,
            scale: perspective_scale as f32,
            rotation_degrees: (f64::from(rotation) / 65_536.0) as f32,
            local_affine_quad,
        }
    }

    pub(crate) fn screen_geometry(
        self,
        image_width: f32,
        image_height: f32,
        viewport_width: f32,
        viewport_height: f32,
        graph_center: (i32, i32),
        use_graph_center: bool,
    ) -> NativeMode5Geometry {
        // sub_41AAA0 derives the perspective scale from Z and the projection
        // distance. sub_429220 then rotates the four bitmap corners around the
        // script-provided anchor in the target's bottom-up DIB coordinates.
        let scale_fixed = self.perspective_scale_fixed();
        let scale = scale_fixed as f64 / 65_536.0;
        let angle = self.rotation as f64 / 65_536.0;
        let radians = angle.to_radians();
        let (sin, cos) = radians.sin_cos();
        let anchor_x = self.anchor_x as f64;
        let anchor_y = self.anchor_y as f64;
        let corners = [
            (-anchor_x, anchor_y),
            (image_width as f64 - anchor_x - 1.0, anchor_y),
            (-anchor_x, anchor_y - image_height as f64 + 1.0),
            (
                image_width as f64 - anchor_x - 1.0,
                anchor_y - image_height as f64 + 1.0,
            ),
        ];
        let mut min_x = f64::INFINITY;
        let mut max_x = f64::NEG_INFINITY;
        let mut min_y = f64::INFINITY;
        let mut max_y = f64::NEG_INFINITY;
        for (x, y) in corners {
            let x = x * scale;
            let y = y * scale;
            let transformed_x = x * cos - y * sin;
            let transformed_y = x * sin + y * cos;
            min_x = min_x.min(transformed_x);
            max_x = max_x.max(transformed_x);
            min_y = min_y.min(transformed_y);
            max_y = max_y.max(transformed_y);
        }

        // The exact target uses a 32-bit multiply here. Its wrapped high word
        // contributes only the subpixel remainder used by the bound rounding.
        let x_remainder = scale_fixed
            .wrapping_mul(self.position_x as u32)
            .wrapping_shr(16) as f64
            / 65_536.0;
        let y_remainder = scale_fixed
            .wrapping_mul(self.position_y as u32)
            .wrapping_shr(16) as f64
            / 65_536.0;
        max_x += x_remainder;
        min_y -= y_remainder;

        let min_x = min_x.floor();
        let max_x = max_x.ceil();
        let min_y = min_y.floor();
        let max_y = max_y.ceil();
        let width = max_x - min_x + 1.0;
        let height = max_y - min_y + 1.0;

        // sub_429220 stores two origin offsets at sprite +0x2DC/+0x2E0:
        //   offset_x = -floor(min_x), offset_y = ceil(max_y).
        // CDspObjSprite::GetCompositePosition (sub_4281E0) first resolves the
        // normal object position through sub_41B260 and then subtracts those
        // offsets for mode 5.  Therefore the raster top-left is relative to
        // the object's ordinary screen coordinate; there is no implicit
        // viewport-centre origin and no second Y-axis inversion here.
        let origin_offset_x = -min_x;
        let origin_offset_y = max_y;

        // sub_429AF0 does NOT use the bitmap half-size as the ordinary
        // object-position base. sub_41C0A0 -> sub_442E10 returns the current
        // display dimensions and the target takes width/2,height/2. If
        // CDspObj+0x100 is nonzero, sub_41C0D0 -> sub_442E90 replaces that
        // base with Graph90:06's centre only when both coordinates are inside
        // the display. This distinction is visible with 1480x820 backgrounds:
        // using their own (740,410) centre shifts the raster down/right.
        let viewport_width_i32 = viewport_width.max(0.0).floor().min(i32::MAX as f32) as i32;
        let viewport_height_i32 = viewport_height.max(0.0).floor().min(i32::MAX as f32) as i32;
        let default_center = (
            i64::from(viewport_width_i32 >> 1),
            i64::from(viewport_height_i32 >> 1),
        );
        let valid_graph_center = use_graph_center
            && graph_center.0 >= 0
            && graph_center.1 >= 0
            && graph_center.0 < viewport_width_i32
            && graph_center.1 < viewport_height_i32;
        let (base_center_x, base_center_y) = if valid_graph_center {
            (i64::from(graph_center.0), i64::from(graph_center.1))
        } else {
            default_center
        };

        // sub_429AF0 performs this part in signed fixed-point and sends
        // v5>>16/v6>>16 through vtable+40. Keep the integer ordinary
        // position separate from the projected raster origin; retaining a
        // fractional f32 here would reintroduce subpixel state that the
        // target discarded before sub_4281E0.
        let projected_position = |value: i32| -> i64 {
            if self.project_position {
                (value as i64 * scale_fixed as i64) >> 16
            } else {
                value as i64
            }
        };
        let base_x_fixed = (base_center_x << 16) + projected_position(self.position_x);
        let base_y_fixed = (base_center_y << 16) + projected_position(self.position_y);
        let object_x = (base_x_fixed >> 16) as i32;
        let object_y = (base_y_fixed >> 16) as i32;
        let native_left = object_x as f64 - origin_offset_x;
        let native_top = object_y as f64 - origin_offset_y;

        NativeMode5Geometry {
            x: native_left as f32,
            y: native_top as f32,
            object_x: object_x as f32,
            object_y: object_y as f32,
            width: width as f32,
            height: height as f32,
            origin_offset_x: origin_offset_x as f32,
            origin_offset_y: origin_offset_y as f32,
            scale: scale as f32,
            rotation_degrees: angle as f32,
            local_affine_quad: [
                [0.0, 0.0],
                [0.0, height as f32],
                [width as f32, height as f32],
                [width as f32, 0.0],
            ],
        }
    }
}

#[cfg(test)]
mod tests {
    use super::{
        NativeMode5DynamicState, NativeMode5NodeArgs, RuntimeGraphDrawItem,
        RuntimeGraphObjectProperties, RuntimeGraphResource, backf_mask_weight,
        blend_decoded_image_parameter, blit_decoded_image, blit_decoded_image_format1_to_format2,
        blit_decoded_image_parameter, crossfade_decoded_images, native_draw_order,
        scale_decoded_image_fixed,
    };
    use ethornell_image::DecodedImage;

    #[test]
    fn mode5_arg13_selects_native_sampling_path() {
        let mut args = [0i32; 17];
        args[0] = 1;
        args[4] = 2;
        args[13] = 0;
        assert!(
            !NativeMode5NodeArgs::from_source_args(&args)
                .expect("mode-5 args")
                .interpolation
        );
        args[13] = 1;
        assert!(
            NativeMode5NodeArgs::from_source_args(&args)
                .expect("mode-5 args")
                .interpolation
        );
    }

    #[test]
    fn mode5_ordinary_position_uses_valid_graph_center_not_bitmap_center() {
        let mode5 = NativeMode5NodeArgs {
            node_id: 1,
            position_x: 0,
            position_y: 0,
            position_z: 0,
            resource_id: 2,
            anchor_x: 0,
            anchor_y: 0,
            rotation: 0,
            perspective: 0,
            project_position: true,
            interpolation: false,
        };
        let dynamic = NativeMode5DynamicState::default();
        let geometry = mode5.screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (640, 246),
            true,
            dynamic,
        );
        assert_eq!((geometry.object_x, geometry.object_y), (640.0, 246.0));

        let fallback = mode5.screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (-1, -1),
            true,
            dynamic,
        );
        assert_eq!((fallback.object_x, fallback.object_y), (640.0, 360.0));

        let gated = mode5.screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (640, 246),
            false,
            dynamic,
        );
        assert_eq!((gated.object_x, gated.object_y), (640.0, 360.0));
    }

    #[test]
    fn mode5_realistic_large_background_uses_full_width_multiply_and_raster_origin() {
        // Matches the 1480x820 / z=-128 / perspective=640 class used by the
        // Tayutama2 opening backgrounds.  The target's sub_429220 keeps the
        // full 64-bit product when deriving fractional bounds; truncating the
        // multiply to 32 bits corrupts the transformed raster extent.
        let mode5 = NativeMode5NodeArgs {
            node_id: 1,
            position_x: 0,
            position_y: 200 << 16,
            position_z: -128 << 16,
            resource_id: 2,
            anchor_x: 740,
            anchor_y: 410,
            rotation: 0,
            perspective: 640,
            project_position: true,
            interpolation: true,
        };
        let geometry = mode5.screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (-1, -1),
            true,
            NativeMode5DynamicState::default(),
        );
        assert_eq!((geometry.x, geometry.y), (-248.0, 107.0));
        assert_eq!((geometry.width, geometry.height), (1775.0, 984.0));
        assert!((geometry.scale - 1.2).abs() < 0.0001);
    }

    #[test]
    fn mode5_opening_affine_quad_maps_source_row_zero_to_first_destination_pixel() {
        fn source_center_destination(
            quad: [[f32; 2]; 4],
            width: f32,
            height: f32,
            source_x: f32,
            source_y: f32,
        ) -> [f32; 2] {
            let u = (source_x + 0.5) / width;
            let v = (source_y + 0.5) / height;
            let [tl, bl, _br, tr] = quad;
            [
                tl[0] + (tr[0] - tl[0]) * u + (bl[0] - tl[0]) * v,
                tl[1] + (tr[1] - tl[1]) * u + (bl[1] - tl[1]) * v,
            ]
        }

        // Opening background class: source anchor Y=205, ordinary Y=360+200,
        // perspective scale 1.2. Target sub_429220 gives origin_y=246, so
        // sub_4281E0 gives world_y=314. The affine path must leave those
        // values untouched while mapping source row 0 to raster-local pixel 0.
        let first = NativeMode5NodeArgs {
            node_id: 1,
            position_x: 0,
            position_y: 200 << 16,
            position_z: -128 << 16,
            resource_id: 2,
            anchor_x: 740,
            anchor_y: 205,
            rotation: 0,
            perspective: 640,
            project_position: false,
            interpolation: true,
        }
        .screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (-1, -1),
            true,
            NativeMode5DynamicState::default(),
        );
        assert_eq!((first.object_x, first.object_y), (640.0, 560.0));
        assert_eq!(
            (first.origin_offset_x, first.origin_offset_y),
            (888.0, 246.0)
        );
        assert_eq!((first.x, first.y), (-248.0, 314.0));
        let first_pixel =
            source_center_destination(first.local_affine_quad, 1480.0, 820.0, 0.0, 0.0);
        // The 16.16 perspective scale is 0x13333 rather than mathematical
        // 1.2, so target raster rounding leaves a tiny subpixel remainder.
        // Source (0,0) must still land within a few thousandths of the first
        // GPU fragment centre, never tens of pixels away.
        assert!((first_pixel[0] - 0.5).abs() < 0.005);
        assert!((first_pixel[1] - 0.5).abs() < 0.005);

        // Second opening class: scale 2.0, ordinary Y=360+100, origin_y=410.
        // The target world Y is therefore 50, again with source row 0 at the
        // first destination pixel rather than at a bbox-derived scaled offset.
        let second = NativeMode5NodeArgs {
            node_id: 1,
            position_x: 0,
            position_y: 100 << 16,
            position_z: -640 << 16,
            resource_id: 2,
            anchor_x: 740,
            anchor_y: 205,
            rotation: 0,
            perspective: 640,
            project_position: false,
            interpolation: true,
        }
        .screen_geometry_mode5_exact(
            1480.0,
            820.0,
            1280.0,
            720.0,
            (-1, -1),
            true,
            NativeMode5DynamicState::default(),
        );
        assert_eq!((second.object_x, second.object_y), (640.0, 460.0));
        assert_eq!(
            (second.origin_offset_x, second.origin_offset_y),
            (1480.0, 410.0)
        );
        assert_eq!((second.x, second.y), (-840.0, 50.0));
        let second_pixel =
            source_center_destination(second.local_affine_quad, 1480.0, 820.0, 0.0, 0.0);
        assert!((second_pixel[0] - 0.5).abs() < 0.001);
        assert!((second_pixel[1] - 0.5).abs() < 0.001);
    }

    #[test]
    fn backf_mask_low_parameter_matches_recovered_table_quantization() {
        // p=0: raw = 256 - 2*T + mask. The native table is indexed by
        // raw>>1, so odd raw values intentionally lose one coverage unit.
        assert_eq!(backf_mask_weight(0, 0, 128, 0), 0);
        assert_eq!(backf_mask_weight(1, 0, 128, 0), 0);
        assert_eq!(backf_mask_weight(2, 0, 128, 0), 2);
        assert_eq!(backf_mask_weight(255, 0, 0, 0), 256);
        assert_eq!(backf_mask_weight(255, 0, 128, 0), 254);
    }

    #[test]
    fn backf_mask_enabled_low_parameter_attenuates_table_slope() {
        // raw=128 -> table index 64. With extra_control=128 the target
        // rebuilds the table at half slope: floor(64*128/256)*2 = 64.
        assert_eq!(backf_mask_weight(128, 0, 128, 128), 64);
        // Enabled sub_411F10 clamps raw to table index 128 instead of taking
        // the default path's direct source-copy shortcut.
        assert_eq!(backf_mask_weight(255, 0, 0, 128), 128);
        assert_eq!(backf_mask_weight(255, 0, 0, 256), 0);
    }

    #[test]
    fn backf_mask_high_parameter_forms_triangular_wave() {
        // p=8 => q=0, one ramp across the 0..256 interval.
        assert_eq!(backf_mask_weight(0, 8, 64, 0), 128);
        // p=9 => q=1, three alternating half-waves. Sample a point in the
        // second lobe to ensure the odd-lobe reversal is retained.
        assert_eq!(backf_mask_weight(0, 9, 64, 0), 128);
        // The enabled path scales the same phase by its control amplitude.
        assert_eq!(backf_mask_weight(0, 9, 64, 128), 64);
        // Negative parameters take the unsigned >=8 branch and mask to q=7.
        assert_eq!(backf_mask_weight(128, -1, 128, 0), 128);
    }

    #[test]
    fn native_blit_mutates_destination_rgba_pixel() {
        let mut destination = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![0, 0, 0, 0],
        };
        let source = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![12, 34, 56, 255],
        };

        blit_decoded_image(&mut destination, &source, 0, 0, 128);
        assert_eq!(destination.rgba.as_slice(), &[12, 34, 56, 255]);

        destination.rgba.fill(0);
        blit_decoded_image_parameter(&mut destination, &source, 0, 0, 128, 128);
        assert_eq!(destination.rgba[3], 128);
    }

    #[test]
    fn native_format1_to_format2_copy_preserves_rgb_and_forces_opaque_alpha() {
        let source = DecodedImage {
            width: 2,
            height: 1,
            // Format-1 alpha bytes are not semantically source alpha. Keep
            // deliberately low values to guard against the message-window
            // regression where those bytes leaked into a format-2 canvas.
            rgba: vec![10, 20, 30, 7, 40, 50, 60, 22],
        };
        let mut destination = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![0; 8],
        };

        blit_decoded_image_format1_to_format2(&mut destination, &source, 0, 0);
        assert_eq!(destination.rgba, vec![10, 20, 30, 255, 40, 50, 60, 255]);
    }

    #[test]
    fn bitmap_subregions_compose_atlas_coordinates() {
        let atlas = RuntimeGraphResource::whole("sysgrp.arc:atlas".into());
        let button = atlas.subregion(100, 40, 160, 50);
        let hover = button.subregion(0, 50, 160, 50);

        let button_rect = button.source_rect.unwrap();
        assert_eq!((button_rect.x, button_rect.y), (100.0, 40.0));
        assert_eq!((button_rect.width, button_rect.height), (160.0, 50.0));
        let hover_rect = hover.source_rect.unwrap();
        assert_eq!((hover_rect.x, hover_rect.y), (100.0, 90.0));
        assert_eq!((hover_rect.width, hover_rect.height), (160.0, 50.0));
    }

    #[test]
    fn native_bitmap_copy_resets_then_alpha_composites_the_target() {
        let mut target = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![1, 2, 3, 4],
        };
        let base = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![20, 40, 60, 255],
        };
        let overlay = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![220, 140, 60, 128],
        };

        blit_decoded_image(&mut target, &base, 0, 0, 128);
        assert_eq!(target.rgba, [20, 40, 60, 255]);
        blit_decoded_image(&mut target, &overlay, 0, 0, 1);
        assert_eq!(target.rgba, [120, 90, 60, 255]);
    }

    #[test]
    fn parameterized_blit_applies_native_256_alpha() {
        let mut target = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![0, 0, 0, 255],
        };
        let source = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![200, 100, 50, 255],
        };

        blit_decoded_image_parameter(&mut target, &source, 0, 0, 1, 128);
        assert_eq!(target.rgba, [100, 50, 25, 255]);
    }

    #[test]
    fn native_transition_alpha_crossfades_both_bitmaps() {
        let primary = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![240, 80, 0, 255],
        };
        let secondary = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![0, 40, 200, 0],
        };

        assert_eq!(
            crossfade_decoded_images(&primary, &secondary, 0).rgba,
            primary.rgba
        );
        assert_eq!(
            crossfade_decoded_images(&primary, &secondary, 256).rgba,
            secondary.rgba
        );
        assert_eq!(
            crossfade_decoded_images(&primary, &secondary, 128).rgba,
            [120, 60, 100, 128]
        );
    }

    #[test]
    fn native_object_mask_combines_without_overwriting_layer_animation_alpha() {
        let mut properties = RuntimeGraphObjectProperties::default();
        assert_eq!(properties.opacity(), 1.0);

        properties.set_property(1, 1, 0);
        properties.mask_alpha = 128;
        assert!((properties.opacity() - 0.5).abs() < f32::EPSILON);

        properties.set_property(2, 64, 0);
        assert!((properties.opacity() - 0.375).abs() < f32::EPSILON);
    }

    #[test]
    fn native_additive_mode_uses_the_recovered_integer_formula() {
        let mut properties = RuntimeGraphObjectProperties::default();
        properties.set_property(1, 2, 0);
        properties.set_property(2, 128, 0);
        properties.mask_alpha = 0;
        assert!((properties.opacity() - 0.5).abs() < f32::EPSILON);
    }

    #[test]
    fn native_full_mask_hides_every_blend_mode() {
        let mut properties = RuntimeGraphObjectProperties::default();
        properties.mask_alpha = 256;
        assert_eq!(properties.opacity(), 0.0);

        properties.set_property(1, 2, 0);
        properties.set_property(2, 256, 0);
        assert_eq!(properties.opacity(), 0.0);
    }

    #[test]
    fn equal_depth_objects_preserve_target_insertion_order() {
        let item = |hit_id| RuntimeGraphDrawItem {
            owner_object: None,
            key: String::new(),
            x: 0.0,
            y: 0.0,
            width: 1.0,
            height: 1.0,
            src_x: 0.0,
            src_y: 0.0,
            src_width: 1.0,
            src_height: 1.0,
            opacity: 1.0,
            ignore_source_alpha: false,
            rotation_degrees: 0.0,
            destination_quad: None,
            linear_sampling: true,
            clip: None,
            z: 0,
            blend_mode: 1,
            order_serial: hit_id as u64,
            hit_id,
        };
        let mut items = [item(27), item(28)];
        items.sort_by_key(native_draw_order);
        assert_eq!(items.map(|item| item.hit_id), [27, 28]);
    }

    #[test]
    fn clip_rect_intersection_clamps_to_shared_area() {
        let left = super::RuntimeClipRect {
            x: 10.0,
            y: 20.0,
            width: 100.0,
            height: 80.0,
        };
        let right = super::RuntimeClipRect {
            x: 60.0,
            y: 10.0,
            width: 90.0,
            height: 40.0,
        };
        let intersection = left.intersection(right);
        assert_eq!(intersection.x, 60.0);
        assert_eq!(intersection.y, 20.0);
        assert_eq!(intersection.width, 50.0);
        assert_eq!(intersection.height, 30.0);
        assert!(!intersection.is_empty());
    }

    #[test]
    fn disjoint_clip_rect_is_empty() {
        let left = super::RuntimeClipRect {
            x: 0.0,
            y: 0.0,
            width: 10.0,
            height: 10.0,
        };
        let right = super::RuntimeClipRect {
            x: 20.0,
            y: 20.0,
            width: 5.0,
            height: 5.0,
        };
        assert!(left.intersection(right).is_empty());
    }

    #[test]
    fn native_fixed_scale_and_parameter_blend_match_endpoints() {
        let source = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![10, 20, 30, 255, 200, 210, 220, 128],
        };
        let scaled = scale_decoded_image_fixed(&source, 32_768, 65_536).unwrap();
        assert_eq!((scaled.width, scaled.height), (1, 1));
        assert_eq!(scaled.rgba, [10, 20, 30, 255]);

        let mut destination = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![110, 120, 130, 0],
        };
        blend_decoded_image_parameter(&mut destination, &scaled, 128);
        assert_eq!(destination.rgba, [60, 70, 80, 128]);
    }
}

#[cfg(test)]
mod alpha_mask_tests {
    use super::apply_alpha_mask;
    use ethornell_image::DecodedImage;

    #[test]
    fn opaque_grayscale_mask_does_not_invent_luminance_coverage() {
        let source = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![255, 0, 0, 255, 0, 255, 0, 128],
        };
        let mask = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![0, 0, 0, 255, 255, 255, 255, 255],
        };
        let output = apply_alpha_mask(&source, &mask).expect("masked image");
        assert_eq!(output.rgba[3], 255);
        assert_eq!(output.rgba[7], 128);
    }

    #[test]
    fn alpha_mask_uses_alpha_when_present() {
        let source = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![10, 20, 30, 200],
        };
        let mask = DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![255, 255, 255, 128],
        };
        let output = apply_alpha_mask(&source, &mask).expect("masked image");
        assert_eq!(output.rgba[3], 100);
    }
}
