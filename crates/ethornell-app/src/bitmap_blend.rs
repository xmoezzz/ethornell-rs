//! Integer blit kernels of the target bitmap library (0x40A530 .. 0x40BC60).
//!
//! Pixel formats (`bitmap+16`): 1 = 32-bit RGB, the fourth byte is not alpha;
//! 2 = 32-bit ARGB with straight (unpremultiplied) alpha. Format 0 is a 16-bit
//! 5-5-5 colour-keyed format that the portable runtime never materialises.
//! Images are stored as RGBA bytes; every kernel is channel-wise, so the byte
//! permutation relative to the target's BGRA dwords does not change a result.
//! Lane 3 is the fourth byte of the dword (alpha for format 2).
//!
//! Modes ported here (`sub_40A9E0` dispatch):
//! * 0 (`sub_40B080`) alpha-over with 7-bit source alpha;
//! * 1 / 0x20 (`sub_40B320`) the same with a 0..255 transparency parameter;
//! * 0x80 (`sub_40AF50`) copy, with the target's format conversions;
//! * 0xF0 (`sub_40BC10`) linear interpolation by parameter;
//! * 0x40 (`sub_40DF80`) mask clear: destination pixels become 0 where the
//!   source mask is set;
//! * 0x41 (`sub_40E150`) clear the destination region;
//! * 2 / 0x21 (`sub_40CA70`) additive blend, 3 / 0x22 (`sub_40D200`) subtractive
//!   blend, 4 / 0x23 (`sub_40D440`) multiply;
//! * 5 / 0xC0 (`sub_40DAD0`) faded copy, 6 / 0x24 screen, 7 / 0x25 cut-out,
//!   8 / 0x26 overlay, 9 / 0x27 hard light, 0xC1 fade to white and 0xFF
//!   channel extraction.
//! The 0x2X aliases run with `256 - p` (the dispatcher's jump table), 0x20 and
//! 0xC0 are plain aliases of 1 and 5. A format pair a kernel does not handle
//! leaves the destination unchanged, exactly as the target does.

use ethornell_image::DecodedImage;

/// Rectangle of `source` placed at `(x, y)` in `destination`, clipped like
/// `sub_40A530` (`sub_409110` intersection, then `sub_4091B0` crop).
struct Clip {
    dst_x: usize,
    dst_y: usize,
    src_x: usize,
    src_y: usize,
    width: usize,
    height: usize,
}

fn clip(destination: &DecodedImage, source: &DecodedImage, x: i32, y: i32) -> Option<Clip> {
    let x0 = x.max(0) as i64;
    let y0 = y.max(0) as i64;
    let x1 = (i64::from(x) + i64::from(source.width)).min(i64::from(destination.width));
    let y1 = (i64::from(y) + i64::from(source.height)).min(i64::from(destination.height));
    if x0 >= x1 || y0 >= y1 {
        return None;
    }
    Some(Clip {
        dst_x: x0 as usize,
        dst_y: y0 as usize,
        src_x: (x0 - i64::from(x)) as usize,
        src_y: (y0 - i64::from(y)) as usize,
        width: (x1 - x0) as usize,
        height: (y1 - y0) as usize,
    })
}

fn for_each_pixel(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    x: i32,
    y: i32,
    mut kernel: impl FnMut(&[u8; 4], &mut [u8; 4]),
) {
    let Some(c) = clip(destination, source, x, y) else {
        return;
    };
    for row in 0..c.height {
        let s_base = ((c.src_y + row) * source.width as usize + c.src_x) * 4;
        let d_base = ((c.dst_y + row) * destination.width as usize + c.dst_x) * 4;
        for col in 0..c.width {
            let s: [u8; 4] = source.rgba[s_base + col * 4..s_base + col * 4 + 4]
                .try_into()
                .unwrap();
            let d: &mut [u8; 4] = (&mut destination.rgba[d_base + col * 4..d_base + col * 4 + 4])
                .try_into()
                .unwrap();
            kernel(&s, d);
        }
    }
}

fn clamp_u8(value: i32) -> u8 {
    value.clamp(0, 255) as u8
}

/// `_mm_add_epi16(d, _mm_srai_epi16(_mm_mullo_epi16(s - d, w), 7))` + `packus`.
fn lerp7(dst: u8, src: u8, weight: i32) -> u8 {
    let delta = (i32::from(src) - i32::from(dst)) * weight;
    // 16-bit lane arithmetic: the product wraps to i16 before the shift.
    let delta = i32::from(delta as i16) >> 7;
    clamp_u8(i32::from(dst) + delta)
}

/// Mode 0 and 1/0x20 with parameter 0. `(src, dst)` are the target formats.
pub(crate) fn blit_alpha_over(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
) -> bool {
    match (source_format, destination_format) {
        (1, 1) => {
            copy(destination, source, x, y);
            true
        }
        (1, 2) => {
            force_opaque_copy(destination, source, x, y);
            true
        }
        (2, 1) => {
            // sub_40B130: skip alpha < 2; 7-bit alpha; 127 copies the colour.
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let k = i32::from(s[3] >> 1);
                if k == 127 {
                    *d = [s[0], s[1], s[2], 0];
                } else {
                    // unk_50B0F0 weights are [k, k, k, 0]: the fourth byte
                    // keeps its value.
                    for lane in 0..3 {
                        d[lane] = lerp7(d[lane], s[lane], k);
                    }
                }
            });
            true
        }
        (2, 2) => {
            // sub_40B200: straight-alpha source-over with 8.8 weights.
            for_each_pixel(destination, source, x, y, |s, d| {
                let sa = u32::from(s[3]);
                if sa == 0 {
                    return;
                }
                if sa == 255 {
                    *d = *s;
                    return;
                }
                let weighted_dst = (256 - sa) * u32::from(d[3]);
                let total = weighted_dst + (sa << 8);
                let wd = (weighted_dst << 8) / total;
                let ws = (sa << 16) / total;
                for lane in 0..3 {
                    let mixed = (u32::from(d[lane]) * wd + u32::from(s[lane]) * ws) & 0xffff;
                    d[lane] = (mixed >> 8).min(255) as u8;
                }
                d[3] = ((total >> 8) & 0xff) as u8;
            });
            true
        }
        _ => false,
    }
}

/// Mode 1/0x20 with a transparency parameter `1..=255`
/// (`sub_40B4B0`, `sub_40B5D0`, `sub_40B6F0`, `sub_40B9B0`).
pub(crate) fn blit_alpha_over_parameter(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if !(1..=255).contains(&parameter) {
        return parameter == 0
            && blit_alpha_over(destination, destination_format, source, source_format, x, y);
    }
    let coverage = 256 - parameter as u32;
    match (source_format, destination_format) {
        (1, 1) => {
            let weight = parameter >> 1;
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..4 {
                    // dst' = src + ((dst - src) * weight >> 7)
                    d[lane] = lerp7(s[lane], d[lane], weight);
                }
            });
            true
        }
        (1, 2) => {
            let source_weight = 255 * coverage;
            for_each_pixel(destination, source, x, y, |s, d| {
                let dst_part = (u32::from(d[3]) * (0x10000 - source_weight)) >> 8;
                let total = dst_part + source_weight;
                let wd = (dst_part << 16) / total;
                let ws = (source_weight << 16) / total;
                for lane in 0..3 {
                    d[lane] = ((wd * u32::from(d[lane]) + ws * u32::from(s[lane])) >> 16) as u8;
                }
                d[3] = ((total >> 8) & 0xff) as u8;
            });
            true
        }
        (2, 1) => {
            // Weight table: w[k] = (k * coverage) >> 8 for k = alpha >> 1.
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let weight = ((u32::from(s[3] >> 1)) * coverage >> 8) as i32;
                // sub_40B6F0's table is [w, w, w, 0].
                for lane in 0..3 {
                    d[lane] = lerp7(d[lane], s[lane], weight);
                }
            });
            true
        }
        (2, 2) => {
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] == 0 {
                    return;
                }
                let cov = coverage * u32::from(s[3]);
                let dst_part = (u32::from(d[3]) * (0x10000 - cov)) >> 8;
                let total = dst_part + cov;
                let wd = (dst_part << 8) / total;
                let ws = (cov << 8) / total;
                for lane in 0..3 {
                    let mixed = (u32::from(d[lane]) * wd + u32::from(s[lane]) * ws) & 0xffff;
                    d[lane] = (mixed >> 8).min(255) as u8;
                }
                d[3] = ((total >> 8) & 0xff) as u8;
            });
            true
        }
        _ => false,
    }
}

/// Mode 0x80 (`sub_40AF50`).
pub(crate) fn blit_copy(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
) -> bool {
    match (source_format, destination_format) {
        // sub_40ADF0: equal formats are a raw row copy, whatever the format.
        (a, b) if a == b => {
            copy(destination, source, x, y);
            true
        }
        (1, 2) => {
            force_opaque_copy(destination, source, x, y);
            true
        }
        (2, 1) => {
            // colour * (alpha >> 1) >> 7 with the unk_50B0F0 weights
            // [k, k, k, 0]: the fourth byte becomes 0.
            for_each_pixel(destination, source, x, y, |s, d| {
                let k = u32::from(s[3] >> 1);
                for lane in 0..3 {
                    d[lane] = ((u32::from(s[lane]) * k) >> 7).min(255) as u8;
                }
                d[3] = 0;
            });
            true
        }
        _ => false,
    }
}

/// Mode 0xF0 with parameter `1..=255` (`sub_40BC10` -> `sub_40B4B0`).
pub(crate) fn blit_interpolate(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    x: i32,
    y: i32,
    parameter: i32,
) {
    let weight = parameter >> 1;
    for_each_pixel(destination, source, x, y, |s, d| {
        for lane in 0..4 {
            d[lane] = lerp7(s[lane], d[lane], weight);
        }
    });
}

fn weight_table(parameter: i32) -> impl Fn(u8) -> i32 {
    // w[k] = (k * p) >> 8 with k = alpha >> 1 (sub_40CDD0 builds this table;
    // for p = 256 the target substitutes the identity table, which is the
    // same value).
    move |alpha| ((i32::from(alpha >> 1)) * parameter) >> 8
}

/// Mode 2 (additive). `parameter` is the intensity 1..=256; 0 does nothing.
/// For mode 0x21 the caller passes `256 - p`.
pub(crate) fn blit_add(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 {
        return true;
    }
    let p = parameter.clamp(0, 256);
    let saturating = |d: u8, add: i32| clamp_u8(i32::from(d) + add);
    match (source_format, destination_format) {
        (1, 1 | 2) => {
            let touch_alpha = destination_format == 2;
            for_each_pixel(destination, source, x, y, |s, d| {
                if u32::from(s[0]) + u32::from(s[1]) + u32::from(s[2]) == 0 {
                    return;
                }
                for lane in 0..3 {
                    d[lane] = saturating(d[lane], (p * i32::from(s[lane])) >> 8);
                }
                if touch_alpha {
                    d[3] = saturating(d[3], p);
                }
            });
            true
        }
        (2, 1 | 2) => {
            let weight = weight_table(p);
            let touch_alpha = destination_format == 2;
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let w = weight(s[3]);
                for lane in 0..3 {
                    d[lane] = saturating(d[lane], (i32::from(s[lane]) * w) >> 7);
                }
                if touch_alpha {
                    d[3] = saturating(d[3], (i32::from(s[3]) * (p >> 1)) >> 7);
                }
            });
            true
        }
        _ => false,
    }
}

/// Mode 3 (subtractive): only an ARGB source into an RGB destination is
/// implemented by the target; every other pair leaves the destination alone.
pub(crate) fn blit_subtract(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 || (source_format, destination_format) != (2, 1) {
        return true;
    }
    let weight = weight_table(parameter.clamp(0, 256));
    for_each_pixel(destination, source, x, y, |s, d| {
        if s[3] < 2 {
            return;
        }
        let w = weight(s[3]);
        for lane in 0..3 {
            d[lane] = clamp_u8(i32::from(d[lane]) - ((i32::from(s[lane]) * w) >> 7));
        }
    });
    true
}

/// Mode 4 (multiply): RGB source (`sub_40D4A0`, `sub_40D670`), ARGB source
/// into RGB (`sub_40D820`) and the 8-bit coverage format (`sub_40DA40`).
pub(crate) fn blit_multiply(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 {
        return true;
    }
    let w = parameter.clamp(0, 256) >> 1;
    match (source_format, destination_format) {
        (1, 1) => {
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..3 {
                    let scaled = i32::from(((i32::from(s[lane]) - 256) * w) as i16);
                    let high = (scaled * (i32::from(d[lane]) * 2)) >> 16;
                    d[lane] = clamp_u8(i32::from(d[lane]) + high);
                }
            });
            true
        }
        (1, 2) => {
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..3 {
                    let product = (i32::from(d[lane]) * i32::from(s[lane])) >> 8;
                    let delta = i32::from(((product - i32::from(d[lane])) * w) as i16) >> 7;
                    d[lane] = clamp_u8(i32::from(d[lane]) + delta);
                }
            });
            true
        }
        (2, 1) => {
            // pmulhuw(d, s << 8) = d * s >> 8, mixed in with the alpha table
            // weight; the fourth lane has weight 0.
            let weight = weight_table(parameter.clamp(0, 256));
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let w = weight(s[3]);
                for lane in 0..3 {
                    let product = ((u32::from(d[lane]) * u32::from(s[lane])) >> 8) as u8;
                    d[lane] = lerp7(d[lane], product, w);
                }
            });
            true
        }
        (3, 3) => {
            // The parameter is used as a 16-bit value and the (negative)
            // product is truncated to u16 before the shift; the byte add wraps.
            let p = i32::from(parameter as i16);
            for_each_pixel(destination, source, x, y, |s, d| {
                let dv = i32::from(d[0]);
                let product = (257 * (dv * i32::from(s[0]) + 1)) >> 16;
                let step = (((p * (product - dv)) as u16) >> 8) as u8;
                d[0] = d[0].wrapping_add(step);
            });
            true
        }
        _ => false,
    }
}

/// Mode 5 / 0xC0 (`sub_40DAD0`): copy the source faded by `256 - p`.
/// `p == 0` is the 0x80 copy. RGB->RGB with `p >= 256` clears the region.
pub(crate) fn blit_fade(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 {
        return blit_copy(destination, destination_format, source, source_format, x, y);
    }
    let keep = 256 - parameter.clamp(0, 256);
    let fade = |value: u8| ((i32::from(value) * keep) >> 8) as u8;
    match (source_format, destination_format) {
        (1, 1) => {
            if parameter >= 256 {
                blit_clear_region(destination, source, x, y);
            } else {
                // sub_40DB60: the fourth lane's weight is 0.
                for_each_pixel(destination, source, x, y, |s, d| {
                    *d = [fade(s[0]), fade(s[1]), fade(s[2]), 0];
                });
            }
            true
        }
        (2, 1) => {
            // sub_40DCE0: faded colour mixed in by alpha >> 1.
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let k = i32::from(s[3] >> 1);
                for lane in 0..3 {
                    d[lane] = lerp7(d[lane], fade(s[lane]), k);
                }
            });
            true
        }
        (2, 2) => {
            // sub_40DE60: straight-alpha composite of the faded source.
            let keep = keep as u32;
            for_each_pixel(destination, source, x, y, |s, d| {
                let sa = u32::from(s[3]);
                if sa == 0 {
                    return;
                }
                let weighted_dst = u32::from(d[3]) * (256 - sa);
                let total = weighted_dst + (sa << 8);
                let ws = ((keep * sa) << 16) / total;
                let wd = (weighted_dst << 16) / total;
                for lane in 0..3 {
                    d[lane] = ((ws
                        .wrapping_mul(u32::from(s[lane]))
                        .wrapping_add(wd.wrapping_mul(u32::from(d[lane]))))
                        >> 16) as u8;
                }
                d[3] = (total >> 8) as u8;
            });
            true
        }
        _ => false,
    }
}

/// Mode 6 / 0x24 screen (`sub_414C60`, `sub_414D10`):
/// `d + s' - (d * s' >> 8)`, saturated.
pub(crate) fn blit_screen(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 {
        return true;
    }
    let p = parameter.clamp(0, 256);
    let screen = |d: u8, s: i32| {
        let d = i32::from(d);
        clamp_u8(d + s - ((d * s) >> 8))
    };
    match (source_format, destination_format) {
        (1, 1) => {
            // All four lanes; s' = pmulhw(s << 4, 16p) = s * p >> 8.
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..4 {
                    d[lane] = screen(d[lane], (i32::from(s[lane]) * p) >> 8);
                }
            });
            true
        }
        (2, 1) => {
            let weight = weight_table(p);
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let w = weight(s[3]);
                for lane in 0..3 {
                    d[lane] = screen(d[lane], (i32::from(s[lane]) * w) >> 7);
                }
            });
            true
        }
        _ => false,
    }
}

/// Mode 7 / 0x25 cut-out by source alpha (`sub_415900`, `sub_415A30`,
/// `sub_415AB0`). Only an ARGB source does anything.
pub(crate) fn blit_cut_out(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if parameter == 0 {
        return true;
    }
    let p = parameter.clamp(0, 256) as u32;
    let keep = |alpha: u8| 256 - ((u32::from(alpha) * p) >> 8);
    match (source_format, destination_format) {
        (2, 1) => {
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] == 0 {
                    return;
                }
                let k = keep(s[3]);
                for lane in 0..3 {
                    d[lane] = ((u32::from(d[lane]) * k) >> 8) as u8;
                }
                d[3] = 0;
            });
            true
        }
        (2, 2) => {
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] != 0 {
                    d[3] = ((keep(s[3]) * u32::from(d[3])) >> 8) as u8;
                }
            });
            true
        }
        _ => false,
    }
}

/// Shared core of modes 8 (overlay, mask from the destination) and 9 (hard
/// light, mask from the source): channels above 127 are mirrored through
/// 256 before the `pmulhw(x << 5, y << 4)` product.
fn overlay_core(s: u8, d: u8, mask: u8) -> u8 {
    let (s, d) = (i32::from(s), i32::from(d));
    if mask > 127 {
        let product = (((256 - s) << 5) * ((256 - d) << 4)) >> 16;
        clamp_u8(255 - product)
    } else {
        clamp_u8(((s << 5) * (d << 4)) >> 16)
    }
}

/// Modes 8 / 0x26 and 9 / 0x27 (`sub_414F50`, `sub_415000`, `sub_4150E0`;
/// `sub_4152A0`, `sub_415350`, `sub_415430`).
pub(crate) fn blit_overlay(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
    hard_light: bool,
) -> bool {
    if parameter == 0 {
        return true;
    }
    let lane_result = move |s: u8, d: u8| overlay_core(s, d, if hard_light { s } else { d });
    match (source_format, destination_format) {
        (1, 1) if parameter >= 256 => {
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..4 {
                    d[lane] = lane_result(s[lane], d[lane]);
                }
            });
            true
        }
        (1, 1) => {
            let q = parameter >> 1;
            for_each_pixel(destination, source, x, y, |s, d| {
                for lane in 0..3 {
                    d[lane] = lerp7(d[lane], lane_result(s[lane], d[lane]), q);
                }
            });
            true
        }
        (2, 1) => {
            let weight = weight_table(parameter.clamp(0, 256));
            for_each_pixel(destination, source, x, y, |s, d| {
                if s[3] < 2 {
                    return;
                }
                let w = weight(s[3]);
                for lane in 0..3 {
                    d[lane] = lerp7(d[lane], lane_result(s[lane], d[lane]), w);
                }
            });
            true
        }
        _ => false,
    }
}

/// Mode 0xC1 (`sub_40E190` with colour 0xFFFFFF): fade the RGB source toward
/// white, `s * (256 - p) >> 8 + 255 * p >> 8`; the fourth lane becomes 0.
pub(crate) fn blit_fade_to_white(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if (source_format, destination_format) != (1, 1) {
        return false;
    }
    let p = parameter.clamp(0, 256);
    let white = (255 * p) >> 8;
    for_each_pixel(destination, source, x, y, |s, d| {
        for lane in 0..3 {
            d[lane] = clamp_u8(((i32::from(s[lane]) * (256 - p)) >> 8) + white);
        }
        d[3] = 0;
    });
    true
}

/// Mode 0xFF (`sub_413900`): `p >= 4` is mode 0; otherwise one source channel
/// is written as an opaque pixel (`sub_413930`). `p` selects the target's
/// dword byte: 0 = blue, 1 = green, 2 = red (kept in place), 3 = alpha copied
/// to all three colour channels.
pub(crate) fn blit_extract_channel(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    if !(0..4).contains(&parameter) {
        return blit_alpha_over(destination, destination_format, source, source_format, x, y);
    }
    if !matches!(source_format, 1 | 2) {
        return false;
    }
    for_each_pixel(destination, source, x, y, |s, d| {
        *d = match parameter {
            0 => [0, 0, s[2], 255],
            1 => [0, s[1], 0, 255],
            2 => [s[0], 0, 0, 255],
            _ => [s[3], s[3], s[3], 255],
        };
    });
    true
}

/// Mode 0x40. A format-1 source masks where `R + G + B > 0`; a format-2
/// source masks where `alpha == 255` (parameter 0) or `alpha != 0`.
pub(crate) fn blit_mask_clear(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    source_format: i32,
    x: i32,
    y: i32,
    parameter: i32,
) -> bool {
    match source_format {
        1 => for_each_pixel(destination, source, x, y, |s, d| {
            if u32::from(s[0]) + u32::from(s[1]) + u32::from(s[2]) > 0 {
                *d = [0; 4];
            }
        }),
        2 => for_each_pixel(destination, source, x, y, |s, d| {
            let masked = if parameter != 0 {
                s[3] != 0
            } else {
                s[3] == 255
            };
            if masked {
                *d = [0; 4];
            }
        }),
        _ => return false,
    }
    true
}

/// Mode 0x41: zero the destination pixels covered by the blit.
pub(crate) fn blit_clear_region(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    x: i32,
    y: i32,
) {
    for_each_pixel(destination, source, x, y, |_, d| *d = [0; 4]);
}

fn copy(destination: &mut DecodedImage, source: &DecodedImage, x: i32, y: i32) {
    for_each_pixel(destination, source, x, y, |s, d| *d = *s);
}

fn force_opaque_copy(destination: &mut DecodedImage, source: &DecodedImage, x: i32, y: i32) {
    for_each_pixel(destination, source, x, y, |s, d| {
        *d = [s[0], s[1], s[2], 0xff]
    });
}

/// `sub_40A9E0` for one blit: the jump table maps 0x20 to mode 1 and 0xC0 to
/// mode 5 and runs 0x21..0x27 as modes 2..4 / 6..9 with `256 - p`. A format
/// pair a kernel does not implement leaves the destination unchanged and
/// still counts as handled.
#[allow(clippy::too_many_arguments)]
pub(crate) fn blit_mode(
    destination: &mut DecodedImage,
    df: i32,
    source: &DecodedImage,
    sf: i32,
    x: i32,
    y: i32,
    mode: i32,
    alpha_parameter: i32,
) -> bool {
    let inverted = 256 - alpha_parameter;
    match mode {
        0 => blit_alpha_over(destination, df, source, sf, x, y),
        // sub_40B320 only acts for parameters below 0x100; a fully
        // transparent source (256) leaves the destination untouched.
        1 | 0x20 if alpha_parameter >= 256 => true,
        1 | 0x20 => blit_alpha_over_parameter(destination, df, source, sf, x, y, alpha_parameter),
        2 | 0x21 => blit_add(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode == 2 { alpha_parameter } else { inverted },
        ),
        3 | 0x22 => blit_subtract(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode == 3 { alpha_parameter } else { inverted },
        ),
        4 | 0x23 => blit_multiply(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode == 4 { alpha_parameter } else { inverted },
        ),
        5 | 0xc0 => blit_fade(destination, df, source, sf, x, y, alpha_parameter),
        6 | 0x24 => blit_screen(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode == 6 { alpha_parameter } else { inverted },
        ),
        7 | 0x25 => blit_cut_out(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode == 7 { alpha_parameter } else { inverted },
        ),
        8 | 0x26 | 9 | 0x27 => blit_overlay(
            destination,
            df,
            source,
            sf,
            x,
            y,
            if mode <= 9 { alpha_parameter } else { inverted },
            matches!(mode, 9 | 0x27),
        ),
        0x40 => blit_mask_clear(destination, source, sf, x, y, alpha_parameter),
        0x41 => {
            blit_clear_region(destination, source, x, y);
            true
        }
        0x80 => blit_copy(destination, df, source, sf, x, y),
        0xc1 => blit_fade_to_white(destination, df, source, sf, x, y, alpha_parameter),
        0xf0 if alpha_parameter == 0 => blit_copy(destination, df, source, sf, x, y),
        0xf0 => {
            if alpha_parameter < 256 {
                blit_interpolate(destination, source, x, y, alpha_parameter);
            }
            true
        }
        0xff => blit_extract_channel(destination, df, source, sf, x, y, alpha_parameter),
        _ => true,
    }
}

/// sub_40C0F0: the Sprite mode-5/6 two-bitmap cache at sprite+0x220,
/// `primary` faded towards `secondary` by `transition` (0..=256). Both
/// bitmaps must have the cache format, 1 or 2; the result covers their
/// common size.
///
/// Format 1 (sub_40C1B0) mixes all four bytes:
/// `p + floor((s - p) * t / 256)`. Format 2 (sub_40C430) weights the
/// colour by alpha: with `wp = (256 - t) * ap` and `total = wp + t * as`,
/// the colour is `s + ((p - s) * (wp * 128 / total) >> 7)` and the alpha
/// `total >> 8`; a zero total gives a zero pixel.
pub(crate) fn crossfade_cache(
    primary: &DecodedImage,
    secondary: &DecodedImage,
    format: i32,
    transition: i32,
) -> Option<DecodedImage> {
    if !matches!(format, 1 | 2) {
        return None;
    }
    let width = primary.width.min(secondary.width);
    let height = primary.height.min(secondary.height);
    let t = transition;
    let mut rgba = vec![0u8; width as usize * height as usize * 4];
    for y in 0..height as usize {
        for x in 0..width as usize {
            let p = &primary.rgba[(y * primary.width as usize + x) * 4..][..4];
            let s = &secondary.rgba[(y * secondary.width as usize + x) * 4..][..4];
            let out = &mut rgba[(y * width as usize + x) * 4..][..4];
            if format == 1 {
                for lane in 0..4 {
                    // pmulhw((s - p) << 4, t << 4): an arithmetic >> 16.
                    let delta = ((i32::from(s[lane]) - i32::from(p[lane])) << 4) as i16;
                    let product = (i32::from(delta) * i32::from((t << 4) as i16)) >> 16;
                    out[lane] = (i32::from(p[lane]) + product).clamp(0, 255) as u8;
                }
            } else {
                let weighted_primary = (256 - t) * i32::from(p[3]);
                let total = (weighted_primary + t * i32::from(s[3])) as u32;
                if total == 0 {
                    continue;
                }
                let weight = ((weighted_primary << 7) as u32 / total) as i32;
                for lane in 0..3 {
                    let delta = i32::from(p[lane]) - i32::from(s[lane]);
                    let mixed = ((delta * weight) as i16 >> 7) as i32 + i32::from(s[lane]);
                    out[lane] = mixed.clamp(0, 255) as u8;
                }
                out[3] = (total >> 8).min(255) as u8;
            }
        }
    }
    Some(DecodedImage {
        width,
        height,
        rgba,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn image(pixels: &[[u8; 4]]) -> DecodedImage {
        DecodedImage {
            width: pixels.len() as u32,
            height: 1,
            rgba: pixels.concat(),
        }
    }

    #[test]
    fn faded_copy_scales_rgb_and_clears_at_full_parameter() {
        let src = image(&[[200, 100, 4, 9]]);
        let mut dst = image(&[[1, 2, 3, 4]]);
        assert!(blit_fade(&mut dst, 1, &src, 1, 0, 0, 64));
        // keep = 192: 200 -> 150, 100 -> 75, 4 -> 3; fourth lane weight 0.
        assert_eq!(dst.rgba, [150, 75, 3, 0]);
        assert!(blit_fade(&mut dst, 1, &src, 1, 0, 0, 256));
        assert_eq!(dst.rgba, [0, 0, 0, 0]);
    }

    #[test]
    fn faded_argb_source_mixes_by_seven_bit_alpha() {
        let src = image(&[[200, 0, 50, 255]]);
        let mut dst = image(&[[100, 100, 100, 7]]);
        assert!(blit_fade(&mut dst, 1, &src, 2, 0, 0, 128));
        // faded = [100, 0, 25], k = 127; the fourth lane is untouched.
        assert_eq!(dst.rgba, [100, 0, 25, 7]);
    }

    #[test]
    fn screen_adds_and_subtracts_the_product() {
        let src = image(&[[100, 0, 255, 0]]);
        let mut dst = image(&[[100, 50, 0, 0]]);
        assert!(blit_screen(&mut dst, 1, &src, 1, 0, 0, 256));
        // 100 + 100 - (100*100 >> 8) = 161; 50 + 0 = 50; 0 + 255 = 255.
        assert_eq!(&dst.rgba[0..3], &[161, 50, 255]);
    }

    #[test]
    fn cut_out_darkens_rgb_or_scales_destination_alpha() {
        let src = image(&[[9, 9, 9, 128]]);
        let mut rgb = image(&[[200, 100, 50, 77]]);
        assert!(blit_cut_out(&mut rgb, 1, &src, 2, 0, 0, 256));
        assert_eq!(rgb.rgba, [100, 50, 25, 0]);
        let mut argb = image(&[[200, 100, 50, 200]]);
        assert!(blit_cut_out(&mut argb, 2, &src, 2, 0, 0, 256));
        assert_eq!(argb.rgba, [200, 100, 50, 100]);
        // An RGB source is not handled by the target.
        let mut untouched = image(&[[1, 2, 3, 4]]);
        assert!(!blit_cut_out(&mut untouched, 1, &src, 1, 0, 0, 256));
        assert_eq!(untouched.rgba, [1, 2, 3, 4]);
    }

    #[test]
    fn overlay_mirrors_on_the_destination_and_hard_light_on_the_source() {
        let src = image(&[[200, 100, 0, 0]]);
        let mut dst = image(&[[100, 200, 0, 0]]);
        assert!(blit_overlay(&mut dst, 1, &src, 1, 0, 0, 256, false));
        // d <= 127: (200<<5)*(100<<4) >> 16 = 156; d > 127: 255 - 68 = 187.
        assert_eq!(&dst.rgba[0..2], &[156, 187]);
        let mut dst = image(&[[100, 200, 0, 0]]);
        assert!(blit_overlay(&mut dst, 1, &src, 1, 0, 0, 256, true));
        // s > 127 mirrors: 187; s <= 127: 156.
        assert_eq!(&dst.rgba[0..2], &[187, 156]);
    }

    #[test]
    fn fade_to_white_and_channel_extraction() {
        let src = image(&[[0, 255, 10, 20]]);
        let mut dst = image(&[[5, 5, 5, 5]]);
        assert!(blit_fade_to_white(&mut dst, 1, &src, 1, 0, 0, 128));
        assert_eq!(dst.rgba, [127, 254, 132, 0]);
        let src = image(&[[1, 2, 3, 4]]);
        let mut dst = image(&[[9; 4]]);
        // Target byte 0 is blue, our lane 2.
        assert!(blit_extract_channel(&mut dst, 1, &src, 2, 0, 0, 0));
        assert_eq!(dst.rgba, [0, 0, 3, 255]);
        assert!(blit_extract_channel(&mut dst, 1, &src, 2, 0, 0, 3));
        assert_eq!(dst.rgba, [4, 4, 4, 255]);
    }

    #[test]
    fn multiply_with_argb_source_and_coverage_bytes() {
        let src = image(&[[128, 0, 0, 255]]);
        let mut dst = image(&[[200, 50, 0, 9]]);
        assert!(blit_multiply(&mut dst, 1, &src, 2, 0, 0, 256));
        // product 100, weight 127 -> 100; 50 * 0 -> 0; fourth lane kept.
        assert_eq!(dst.rgba, [100, 0, 0, 9]);
        let src = image(&[[255, 0, 0, 0], [0, 0, 0, 0]]);
        let mut dst = image(&[[100, 0, 0, 0], [100, 0, 0, 0]]);
        assert!(blit_multiply(&mut dst, 3, &src, 3, 0, 0, 256));
        // 100 * 255 keeps 100; 100 * 0 wraps through the u16 step to 0.
        assert_eq!([dst.rgba[0], dst.rgba[4]], [100, 0]);
    }

    #[test]
    fn argb_over_rgb_uses_seven_bit_alpha_and_copies_at_127() {
        let mut dst = image(&[[100, 100, 100, 0]; 3]);
        let src = image(&[[200, 0, 50, 255], [200, 0, 50, 128], [200, 0, 50, 1]]);
        assert!(blit_alpha_over(&mut dst, 1, &src, 2, 0, 0));
        // alpha 255 -> k = 127 -> exact colour, fourth byte cleared.
        assert_eq!(&dst.rgba[0..4], &[200, 0, 50, 0]);
        // alpha 128 -> k = 64: d + ((s - d) * 64 >> 7)
        assert_eq!(&dst.rgba[4..7], &[150, 50, 75]);
        // alpha 1 is skipped (the target tests `& 0xFE000000`).
        assert_eq!(&dst.rgba[8..11], &[100, 100, 100]);
    }

    #[test]
    fn argb_over_argb_matches_the_eight_dot_eight_weights() {
        let mut dst = image(&[[0, 0, 0, 255], [10, 20, 30, 0]]);
        let src = image(&[[255, 255, 255, 128], [40, 50, 60, 128]]);
        assert!(blit_alpha_over(&mut dst, 2, &src, 2, 0, 0));
        // total = (256-128)*255 + (128<<8) = 65408; wd = 32640*256/65408 = 127;
        // ws = (128<<16)/65408 = 128 ; colour = (0*127 + 255*128) >> 8 = 127
        assert_eq!(&dst.rgba[0..4], &[127, 127, 127, 255]);
        // Transparent destination: weighted_dst = 0, so the source colour wins
        // with alpha total>>8 = 128.
        assert_eq!(&dst.rgba[4..8], &[40, 50, 60, 128]);
    }

    #[test]
    fn opaque_source_never_changes_with_parameter_zero_but_blends_with_one() {
        let src = image(&[[200, 100, 0, 255]]);
        let mut a = image(&[[0, 0, 0, 255]]);
        assert!(blit_alpha_over_parameter(&mut a, 2, &src, 2, 0, 0, 0));
        assert_eq!(a.rgba, [200, 100, 0, 255]);
        // parameter 128: cov = 128*255, dst part = 255*(65536-cov)>>8 = 32767,
        // total 65407, ws = (cov<<8)/total = 127 (truncated), so 200*127>>8 = 99.
        let mut b = image(&[[0, 0, 0, 255]]);
        assert!(blit_alpha_over_parameter(&mut b, 2, &src, 2, 0, 0, 128));
        assert_eq!(b.rgba, [99, 49, 0, 255]);
    }

    #[test]
    fn parameter_blend_into_rgb_scales_weights_by_coverage() {
        let mut dst = image(&[[0, 0, 0, 0]]);
        let src = image(&[[200, 100, 40, 254]]);
        assert!(blit_alpha_over_parameter(&mut dst, 1, &src, 2, 0, 0, 128));
        // k = 127, weight = (127 * 128) >> 8 = 63 -> 200 * 63 >> 7 = 98
        assert_eq!(&dst.rgba[0..3], &[98, 49, 19]);
    }

    #[test]
    fn rgb_over_rgb_interpolates_from_source_towards_destination() {
        let mut dst = image(&[[200, 0, 0, 0]]);
        let src = image(&[[0, 0, 100, 0]]);
        assert!(blit_alpha_over_parameter(&mut dst, 1, &src, 1, 0, 0, 128));
        // weight 64 on the destination: src + ((dst - src) * 64 >> 7)
        assert_eq!(&dst.rgba[0..3], &[100, 0, 50]);
    }

    #[test]
    fn copy_converts_between_formats_like_sub_40af50() {
        let src2 = image(&[[200, 100, 50, 128]]);
        let mut dst1 = image(&[[9, 9, 9, 9]]);
        assert!(blit_copy(&mut dst1, 1, &src2, 2, 0, 0));
        assert_eq!(&dst1.rgba[0..3], &[100, 50, 25]);
        let src1 = image(&[[1, 2, 3, 77]]);
        let mut dst2 = image(&[[9, 9, 9, 9]]);
        assert!(blit_copy(&mut dst2, 2, &src1, 1, 0, 0));
        assert_eq!(dst2.rgba, [1, 2, 3, 255]);
        let mut same = image(&[[0, 0, 0, 0]]);
        assert!(blit_copy(&mut same, 2, &src2, 2, 0, 0));
        assert_eq!(same.rgba, [200, 100, 50, 128]);
    }

    #[test]
    fn mask_clear_follows_the_source_format_rules() {
        let rgb_mask = image(&[[0, 0, 0, 255], [0, 1, 0, 255]]);
        let mut dst = image(&[[9, 9, 9, 9], [9, 9, 9, 9]]);
        assert!(blit_mask_clear(&mut dst, &rgb_mask, 1, 0, 0, 0));
        assert_eq!(dst.rgba, [9, 9, 9, 9, 0, 0, 0, 0]);
        let argb_mask = image(&[[1, 1, 1, 255], [1, 1, 1, 100], [1, 1, 1, 0]]);
        let mut exact = image(&[[9, 9, 9, 9]; 3]);
        assert!(blit_mask_clear(&mut exact, &argb_mask, 2, 0, 0, 0));
        assert_eq!(&exact.rgba[0..4], &[0, 0, 0, 0]);
        assert_eq!(&exact.rgba[4..8], &[9, 9, 9, 9]);
        let mut any = image(&[[9, 9, 9, 9]; 3]);
        assert!(blit_mask_clear(&mut any, &argb_mask, 2, 0, 0, 1));
        assert_eq!(&any.rgba[4..8], &[0, 0, 0, 0]);
        assert_eq!(&any.rgba[8..12], &[9, 9, 9, 9]);
    }

    #[test]
    fn additive_blend_scales_by_parameter_and_saturates() {
        let src = image(&[[100, 200, 0, 0], [0, 0, 0, 0]]);
        let mut dst = image(&[[10, 100, 7, 50], [1, 2, 3, 4]]);
        assert!(blit_add(&mut dst, 1, &src, 1, 0, 0, 128));
        // 128 * 100 >> 8 = 50 ; 128 * 200 >> 8 = 100 ; black source is skipped
        assert_eq!(&dst.rgba[0..4], &[60, 200, 7, 50]);
        assert_eq!(&dst.rgba[4..8], &[1, 2, 3, 4]);
        let mut sat = image(&[[250, 250, 250, 250]]);
        assert!(blit_add(
            &mut sat,
            2,
            &image(&[[255, 255, 255, 0]]),
            1,
            0,
            0,
            256
        ));
        assert_eq!(sat.rgba, [255, 255, 255, 255]);
        // parameter 0 is a no-op
        let mut same = image(&[[1, 2, 3, 4]]);
        assert!(blit_add(&mut same, 1, &src, 1, 0, 0, 0));
        assert_eq!(same.rgba, [1, 2, 3, 4]);
    }

    #[test]
    fn additive_blend_with_argb_source_uses_alpha_weights() {
        let src = image(&[[200, 100, 40, 254]]);
        let mut dst = image(&[[10, 10, 10, 10]]);
        // k = 127, w = (127 * 256) >> 8 = 127: 200*127>>7 = 198
        assert!(blit_add(&mut dst, 2, &src, 2, 0, 0, 256));
        // alpha lane: (254 * (256 >> 1)) >> 7 = 254, 10 + 254 saturates at 255
        assert_eq!(dst.rgba, [208, 109, 49, 255]);
    }

    #[test]
    fn subtractive_blend_only_exists_for_argb_into_rgb() {
        let src = image(&[[200, 100, 40, 254]]);
        let mut dst = image(&[[220, 50, 100, 7]]);
        assert!(blit_subtract(&mut dst, 1, &src, 2, 0, 0, 256));
        assert_eq!(&dst.rgba[0..3], &[22, 0, 61]);
        let mut other = image(&[[220, 50, 100, 7]]);
        assert!(blit_subtract(&mut other, 2, &src, 2, 0, 0, 256));
        assert_eq!(other.rgba, [220, 50, 100, 7]);
    }

    #[test]
    fn multiply_darkens_by_the_source_and_parameter() {
        // RGB -> RGB: d + ((((s - 256) * w) * 2d) >> 16), w = p >> 1
        let src = image(&[[128, 0, 255, 0]]);
        let mut dst = image(&[[200, 200, 200, 9]]);
        assert!(blit_multiply(&mut dst, 1, &src, 1, 0, 0, 256));
        // lane 0: (128-256)*128 = -16384; * 400 = -6553600 >> 16 = -100 -> 100
        // lane 2: (255-256)*128 = -128; * 400 >> 16 = -1 -> 199
        assert_eq!(&dst.rgba[0..4], &[100, 0, 199, 9]);
        // RGB -> ARGB: d + ((((d*s)>>8) - d) * w >> 7)
        let mut dst2 = image(&[[200, 200, 200, 255]]);
        assert!(blit_multiply(&mut dst2, 2, &src, 1, 0, 0, 256));
        assert_eq!(&dst2.rgba[0..4], &[100, 0, 199, 255]);
    }

    #[test]
    fn clear_region_zeroes_only_the_covered_pixels() {
        let src = image(&[[1, 2, 3, 4]; 2]);
        let mut dst = image(&[[9, 9, 9, 9]; 4]);
        blit_clear_region(&mut dst, &src, 1, 0);
        assert_eq!(&dst.rgba[0..4], &[9, 9, 9, 9]);
        assert_eq!(&dst.rgba[4..12], &[0; 8]);
        assert_eq!(&dst.rgba[12..16], &[9, 9, 9, 9]);
    }

    #[test]
    fn blits_clip_to_the_destination() {
        let mut dst = image(&[[0, 0, 0, 255]; 2]);
        let src = image(&[[5, 6, 7, 255]; 3]);
        assert!(blit_copy(&mut dst, 2, &src, 2, -1, 0));
        assert_eq!(dst.rgba, [5, 6, 7, 255, 5, 6, 7, 255]);
        let mut dst = image(&[[0, 0, 0, 255]; 2]);
        assert!(blit_copy(&mut dst, 2, &src, 2, 5, 0));
        assert_eq!(dst.rgba, [0, 0, 0, 255, 0, 0, 0, 255]);
    }

    #[test]
    fn interpolate_matches_the_alpha_over_formula_for_opaque_formats() {
        let mut dst = image(&[[200, 0, 0, 0]]);
        let src = image(&[[0, 0, 100, 0]]);
        blit_interpolate(&mut dst, &src, 0, 0, 128);
        assert_eq!(&dst.rgba[0..3], &[100, 0, 50]);
    }

    #[test]
    fn crossfade_cache_matches_the_integer_kernels() {
        let image = |px: [u8; 4]| DecodedImage {
            width: 1,
            height: 1,
            rgba: px.to_vec(),
        };
        let p = image([200, 10, 100, 0]);
        let s = image([0, 255, 101, 255]);
        let f1 = crossfade_cache(&p, &s, 1, 64).unwrap().rgba;
        // 200 + floor(-200 * 64 / 256), 10 + floor(245 / 4), 100 + 0, 0 + 63.
        assert_eq!(f1, [150, 71, 100, 63]);
        assert_eq!(crossfade_cache(&p, &s, 1, 0).unwrap().rgba, p.rgba);
        // Format 2: a transparent primary contributes nothing.
        let f2 = crossfade_cache(&p, &s, 2, 64).unwrap().rgba;
        assert_eq!(f2, [0, 255, 101, 63]);
        let half = crossfade_cache(&image([200, 0, 0, 255]), &image([0, 0, 0, 255]), 2, 128)
            .unwrap()
            .rgba;
        assert_eq!(half, [100, 0, 0, 255]);
        assert!(crossfade_cache(&p, &s, 3, 64).is_none());
    }
}
