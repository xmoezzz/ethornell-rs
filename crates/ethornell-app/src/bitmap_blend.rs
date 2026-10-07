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
//! * 0xF0 (`sub_40BC10`) linear interpolation by parameter.
//! Other selectors keep their previous float approximation.

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
                    for lane in 0..4 {
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
                for lane in 0..4 {
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
        (a, b) if a == b && (a == 1 || a == 2) => {
            copy(destination, source, x, y);
            true
        }
        (1, 2) => {
            force_opaque_copy(destination, source, x, y);
            true
        }
        (2, 1) => {
            // colour * (alpha >> 1) >> 7 on every lane, packus-saturated.
            for_each_pixel(destination, source, x, y, |s, d| {
                let k = u32::from(s[3] >> 1);
                for lane in 0..4 {
                    d[lane] = ((u32::from(s[lane]) * k) >> 7).min(255) as u8;
                }
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

fn copy(destination: &mut DecodedImage, source: &DecodedImage, x: i32, y: i32) {
    for_each_pixel(destination, source, x, y, |s, d| *d = *s);
}

fn force_opaque_copy(destination: &mut DecodedImage, source: &DecodedImage, x: i32, y: i32) {
    for_each_pixel(destination, source, x, y, |s, d| *d = [s[0], s[1], s[2], 0xff]);
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
}
