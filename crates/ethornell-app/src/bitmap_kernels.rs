//! Literal ports of BGI 1.622 bitmap kernels, run on the target's pixel
//! layout (B, G, R, A bytes; rows `stride` bytes apart) so that each one
//! can be compared with the original under Unicorn
//! (`src/tools/emu_kernels.py`).
use super::DecodedImage;

/// A bitmap in the target's memory layout (the 24-byte descriptor of
/// `sub_407F10`: pixels, stride, width, height, format).
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct TargetBitmap {
    pub(crate) pixels: Vec<u8>,
    pub(crate) stride: usize,
    pub(crate) width: usize,
    pub(crate) height: usize,
}

impl TargetBitmap {
    /// A 32-bit (format 1 or 2) bitmap from an app raster.
    pub(crate) fn from_image(image: &DecodedImage) -> Self {
        let mut pixels = Vec::with_capacity(image.rgba.len());
        for [r, g, b, a] in image.rgba.as_chunks::<4>().0.iter().copied() {
            pixels.extend_from_slice(&[b, g, r, a]);
        }
        Self {
            pixels,
            stride: image.width as usize * 4,
            width: image.width as usize,
            height: image.height as usize,
        }
    }

    /// An 8-bit (format 3) bitmap from an app raster, whose four channels
    /// all hold the grey level.
    pub(crate) fn gray_from_image(image: &DecodedImage) -> Self {
        Self {
            pixels: image.rgba.iter().step_by(4).copied().collect(),
            stride: image.width as usize,
            width: image.width as usize,
            height: image.height as usize,
        }
    }

    /// An 8-bit bitmap back to an app raster.
    pub(crate) fn gray_to_image(&self) -> DecodedImage {
        let mut rgba = Vec::with_capacity(self.width * self.height * 4);
        for y in 0..self.height {
            for &level in &self.pixels[y * self.stride..y * self.stride + self.width] {
                rgba.extend_from_slice(&[level; 4]);
            }
        }
        DecodedImage { width: self.width as u32, height: self.height as u32, rgba }
    }

    /// Back to an app raster; `opaque` forces alpha 255 (format 1 keeps no
    /// alpha).
    pub(crate) fn to_image(&self, opaque: bool) -> DecodedImage {
        let mut rgba = Vec::with_capacity(self.width * self.height * 4);
        for y in 0..self.height {
            let row = &self.pixels[y * self.stride..y * self.stride + self.width * 4];
            for [b, g, r, a] in row.as_chunks::<4>().0.iter().copied() {
                rgba.extend_from_slice(&[r, g, b, if opaque { 255 } else { a }]);
            }
        }
        DecodedImage {
            width: self.width as u32,
            height: self.height as u32,
            rgba,
        }
    }
}

/// `sub_418F60` (91:1B, both bitmaps format 1): the destination is cleared,
/// then every source pixel is added (saturating) into the destination at
/// a position that shrinks towards the centre: `x_factor` / `y_factor` are
/// the 16.16 shrink amounts, `attenuation` 0..=256 dims the result. The
/// MMX weight computation is kept as is, including its `packssdw`
/// saturation, which sends any pixel with a nonzero horizontal fraction
/// wholly to the right-hand pair of neighbours.
pub(crate) fn concentrate(
    destination: &mut TargetBitmap,
    source: &TargetBitmap,
    attenuation: u32,
    x_factor: u32,
    y_factor: u32,
) {
    let mut table = [[0u16; 4]; 128];
    let mut level = 256u32.wrapping_sub(attenuation).wrapping_shl(7);
    let step = attenuation.wrapping_sub(256);
    for entry in table.iter_mut() {
        let t = (level >> 8) as u16;
        level = level.wrapping_add(step);
        *entry = [t, t, t, 0];
    }
    destination.pixels.fill(0);
    let row_x = (source.width as u32).wrapping_sub(1).wrapping_mul(x_factor) >> 1;
    let mut row_y = (source.height as u32).wrapping_sub(1).wrapping_mul(y_factor) >> 1;
    let dx = 0x10000u32.wrapping_sub(x_factor);
    // cdq: the sign of dx is added to the y lane on every step.
    let dx_carry = if (dx as i32) < 0 { u32::MAX } else { 0 };
    let dy = 0x10000u32.wrapping_sub(y_factor);
    let stride = destination.stride as u32;
    let sat16 = |value: u32| -> u16 {
        let value = value as i32;
        value.clamp(i16::MIN as i32, i16::MAX as i32) as i16 as u16
    };
    for sy in 0..source.height {
        let (mut x, mut y) = (row_x, row_y);
        row_y = row_y.wrapping_add(dy);
        for sx in 0..source.width {
            // psrad 16, packssdw, pmaddwd with [4, stride].
            let ix = sat16(((x as i32) >> 16) as u32) as i16 as i32;
            let iy = sat16(((y as i32) >> 16) as u32) as i16 as i32;
            let base = (ix * 4 + iy * stride as u16 as i16 as i32) as isize;
            let fx = ((x & 0xFE00) >> 9) as u16;
            let fy = ((y & 0xFE00) >> 9) as u16;
            x = x.wrapping_add(dx);
            y = y.wrapping_add(dx_carry);
            let a = [0x80u16.wrapping_sub(fx), fx, 0x80u16.wrapping_sub(fx), fx];
            let b = [0x80u16.wrapping_sub(fy), 0x80u16.wrapping_sub(fy), fy, fy];
            let w: [u16; 4] = std::array::from_fn(|i| a[i].wrapping_mul(b[i]) >> 7);
            let s0 = sat16(u32::from(w[0]) | u32::from(w[1]) << 16);
            let s1 = sat16(u32::from(w[2]) | u32::from(w[3]) << 16);
            let word0 = 0x8080u16.wrapping_sub(s0);
            let word1 = 0x8080u16.wrapping_sub(s1);
            let offset = sy * source.stride + sx * 4;
            let pixel: [u8; 4] = source.pixels[offset..offset + 4].try_into().unwrap();
            // The row step (`add ebx, stride`) sits inside the bottom-left
            // branch: when that neighbour is skipped, bottom-right lands in
            // the top row.
            let bottom = if (word1 as u8) < 0x80 { base + stride as isize } else { base };
            let targets = [
                (word0 as u8, base),
                ((word0 >> 8) as u8, base + 4),
                (word1 as u8, bottom),
                ((word1 >> 8) as u8, bottom + 4),
            ];
            for (index, at) in targets {
                if index >= 0x80 || at < 0 {
                    continue;
                }
                let at = at as usize;
                let Some(target) = destination.pixels.get_mut(at..at + 4) else {
                    continue;
                };
                let weights = table[index as usize];
                for lane in 0..4 {
                    let add = (weights[lane].wrapping_mul(u16::from(pixel[lane])) >> 7).min(255) as u8;
                    target[lane] = target[lane].saturating_add(add);
                }
            }
        }
    }
}

fn read_pixel(bitmap: &TargetBitmap, offset: usize) -> [u8; 4] {
    bitmap
        .pixels
        .get(offset..offset + 4)
        .map_or([0; 4], |bytes| bytes.try_into().unwrap())
}

fn write_pixel(bitmap: &mut TargetBitmap, offset: usize, value: u32) {
    if let Some(target) = bitmap.pixels.get_mut(offset..offset + 4) {
        target.copy_from_slice(&value.to_le_bytes());
    }
}

/// x87 `fild`, `fdiv`, `fmul`, truncating `fistp` (control word set to
/// round toward zero).
fn x87_scaled(numerator: i32, denominator: u32, factor: f64) -> i64 {
    (f64::from(numerator) / f64::from(denominator) * factor) as i64
}

/// `sub_495680` (91:1C without interpolation): nearest sampling of the
/// source scaled by `x_scale` / `y_scale` (16.16) and centred in the
/// destination.
pub(crate) fn scale_nearest(
    destination: &mut TargetBitmap,
    source: &TargetBitmap,
    x_scale: u32,
    y_scale: u32,
) {
    let source_width = (source.width as u32) << 16;
    let source_height = (source.height as u32) << 16;
    let scaled_width = x_scale.wrapping_mul(source.width as u32).wrapping_add(0x8000) >> 16;
    let scaled_height = (source.height as u32).wrapping_mul(y_scale).wrapping_add(0x8000) >> 16;
    if scaled_width == 0 || scaled_height == 0 {
        return;
    }
    let left = (destination.width as i32 >> 1)
        - (x_scale.wrapping_mul(source.width as u32).wrapping_add(0x8000) >> 17) as i32;
    let top = (destination.height as i32 >> 1)
        - ((source.height as u32).wrapping_mul(y_scale).wrapping_add(0x8000) >> 17) as i32;
    let right = (left + scaled_width as i32).min(destination.width as i32);
    let bottom = (top + scaled_height as i32).min(destination.height as i32);
    let first_x = left.max(0);
    let first_y = top.max(0);
    let step_x = source_width / scaled_width;
    let step_y = source_height / scaled_height;
    let start_y = x87_scaled(first_y - top, scaled_height, f64::from(source_height as i32));
    let start_x = x87_scaled(first_x - left, scaled_width, f64::from(source_width as i32)) as u32;
    let mut source_row = source.stride.wrapping_mul((start_y as u32 >> 16) as usize);
    let mut fraction_y = start_y as u32 & 0xFFFF;
    let destination_stride = 4 * (destination.stride >> 2);
    let mut destination_row = first_y as usize * destination.stride;
    for _ in first_y..bottom {
        let mut position = start_x;
        for x in first_x..right {
            let pixel = read_pixel(source, source_row + 4 * (position >> 16) as usize);
            position = position.wrapping_add(step_x);
            write_pixel(destination, destination_row + 4 * x as usize, u32::from_le_bytes(pixel));
        }
        fraction_y = fraction_y.wrapping_add(step_y);
        destination_row += destination_stride;
        source_row += 4 * ((source.stride as u32).wrapping_mul(fraction_y >> 16) >> 2) as usize;
        fraction_y &= 0xFFFF;
    }
}

/// The weight table of `sub_495640`: entry k is k in every lane.
fn area_weight(k: u32) -> u16 {
    k.min(256) as u16
}

/// `sub_495E10`: bilinear sample at 16.16 (x, y); neighbours past the
/// last row or column read the zero pixel `dword_566AE0`.
fn sample_bilinear(source: &TargetBitmap, x: u32, y: u32) -> u32 {
    let fx = (x >> 8) & 0xFF;
    let fy = (y >> 8) & 0xFF;
    let (ix, iy) = ((x >> 16) as usize, (y >> 16) as usize);
    let at = iy * source.stride + 4 * ix;
    let p00 = read_pixel(source, at);
    let mut p01 = read_pixel(source, at + 4);
    let mut p10 = read_pixel(source, at + source.stride);
    let mut p11 = read_pixel(source, at + source.stride + 4);
    if ix >= source.width.wrapping_sub(1) {
        p01 = [0; 4];
        p11 = [0; 4];
    }
    if iy >= source.height.wrapping_sub(1) {
        p10 = [0; 4];
        p11 = [0; 4];
    }
    let (gx, gy) = (256 - fx, 256 - fy);
    let channel = |c: usize| {
        gy.wrapping_mul(gx * u32::from(p00[c]) + fx * u32::from(p01[c]))
            .wrapping_add(fy.wrapping_mul(gx * u32::from(p10[c]) + fx * u32::from(p11[c])))
    };
    let (b, g, r, a) = (channel(0), channel(1), channel(2), channel(3));
    (((b >> 8) | (g & 0xFFFF_00FF)) >> 8) | (r & 0xFFFF_0000) | ((a << 8) & 0xFF00_0000)
}

/// `sub_495B60`: average over the footprint rectangle, colour only from
/// pixels with alpha (or any pixel of a format-1 source).
fn sample_box(source: &TargetBitmap, format: u32, x: u32, y: u32, width: u32, height: u32) -> u32 {
    let left = (x >> 16) as i32;
    let top = (y >> 16) as i32;
    let right = (left + ((width.wrapping_add(255)) >> 8) as i32).min(source.width as i32);
    let bottom = (top + ((height.wrapping_add(255)) >> 8) as i32).min(source.height as i32);
    let (left, top) = (left.max(0), top.max(0));
    let (mut b, mut g, mut r, mut a, mut counted, mut total) = (0u32, 0u32, 0u32, 0u32, 0u32, 0u32);
    for row in top..bottom {
        if (left as u32) < right as u32 {
            total = total.wrapping_add((right - left) as u32);
        }
        for column in left..right {
            let pixel = read_pixel(source, row as usize * source.stride + 4 * column as usize);
            if pixel[3] != 0 || format == 1 {
                b += u32::from(pixel[0]);
                g += u32::from(pixel[1]);
                r += u32::from(pixel[2]);
                a += u32::from(pixel[3]);
                counted += 1;
            }
        }
    }
    let counted = counted.max(1);
    let alpha = if total == 0 { 0 } else { a / total };
    ((((g / counted) | (((r / counted) | (alpha << 8)) << 8)) << 8)) | (b / counted)
}

/// `sub_495CA0`: area-weighted footprint in 24.8 coordinates with the
/// saturating 16-bit SSE accumulation.
fn sample_area(source: &TargetBitmap, x: u32, y: u32, width: u32, height: u32) -> u32 {
    let mut left = (x >> 8) as i32;
    let mut top = (y >> 8) as i32;
    let mut right = left.wrapping_add(width as i32);
    let mut bottom = top.wrapping_add(height as i32);
    left = left.max(0);
    top = top.max(0);
    right = right.min((source.width as i32) << 8);
    bottom = bottom.min((source.height as i32) << 8);
    let area = (right - left).wrapping_mul(bottom - top) >> 8;
    let mut sum = [0u16; 4];
    let mut row = (top >> 8) as usize * source.stride;
    while top < bottom {
        let next = ((top + 256) as u32 & 0xFFFF_FF00) as i32;
        let next = next.min(bottom);
        let span_y = next - top;
        let mut column = left;
        while column < right {
            let end = (((column + 256) as u32 & 0xFFFF_FF00) as i32).min(right);
            let weight = if area == 0 { 0 } else { ((span_y * (end - column)) << 8) / area };
            let pixel = read_pixel(source, row + ((column as u32 >> 6) & 0x3FF_FFFC) as usize);
            let k = area_weight((weight >> 8) as u32);
            for lane in 0..4 {
                sum[lane] = sum[lane].saturating_add(u16::from(pixel[lane]).wrapping_mul(k));
            }
            column = end;
        }
        row += source.stride;
        top = next;
    }
    u32::from_le_bytes(sum.map(|value| (value >> 8).min(255) as u8))
}

/// `sub_495880` (91:1C with interpolation): the sampler is chosen by the
/// scales — bilinear when both enlarge, a box average when either shrinks
/// below one half, the area sampler otherwise.
pub(crate) fn scale_filtered(
    destination: &mut TargetBitmap,
    source: &TargetBitmap,
    format: u32,
    x_scale: u32,
    y_scale: u32,
) {
    let scaled_width = x_scale.wrapping_mul(source.width as u32).wrapping_add(0x8000) >> 16;
    let scaled_height = y_scale.wrapping_mul(source.height as u32).wrapping_add(0x8000) >> 16;
    if scaled_width == 0 || scaled_height == 0 || x_scale >> 8 == 0 || y_scale >> 8 == 0 {
        return;
    }
    let mut footprint = [0x10000 / (x_scale >> 8), 0x10000 / (y_scale >> 8)];
    let left = (destination.width as i32 >> 1) - (scaled_width >> 1) as i32;
    let top = (destination.height as i32 >> 1) - (scaled_height >> 1) as i32;
    let first_x = left.max(0);
    let first_y = top.max(0);
    let right = (left + scaled_width as i32).min(destination.width as i32);
    let bottom = (top + scaled_height as i32).min(destination.height as i32);
    let mut bias = [footprint[0] << 7, footprint[1] << 7];
    let mut shift = 8;
    #[derive(Clone, Copy)]
    enum Sampler {
        Bilinear,
        Box,
        Area,
    }
    let mut sampler = Sampler::Bilinear;
    if (x_scale >= 0x10000 || x_scale <= 0x7FFF) && y_scale.wrapping_sub(0x8000) > 0x7FFF {
        if x_scale < 0x8000 || y_scale < 0x8000 {
            sampler = Sampler::Box;
        } else {
            footprint = [512, 512];
            shift = 9;
            bias = [0, 0];
        }
    } else {
        sampler = Sampler::Area;
    }
    let span_x = ((source.width as u32).wrapping_sub(footprint[0] >> shift)) << 16;
    let span_y = ((source.height as u32).wrapping_sub(footprint[1] >> shift)) << 16;
    let step_x = span_x / scaled_width;
    let step_y = span_y / scaled_height;
    let mut y = bias[1].wrapping_add(x87_scaled(first_y - top, scaled_height, f64::from(span_y as i32)) as u32);
    let row_x = bias[0].wrapping_add(x87_scaled(first_x - left, scaled_width, f64::from(span_x as i32)) as u32);
    let mut destination_row = first_y as usize * destination.stride;
    let destination_stride = 4 * (destination.stride >> 2);
    for _ in first_y..bottom {
        let mut x = row_x;
        for column in first_x..right {
            let value = match sampler {
                Sampler::Bilinear => sample_bilinear(source, x, y),
                Sampler::Box => sample_box(source, format, x, y, footprint[0], footprint[1]),
                Sampler::Area => sample_area(source, x, y, footprint[0], footprint[1]),
            };
            x = x.wrapping_add(step_x);
            write_pixel(destination, destination_row + 4 * column as usize, value);
        }
        y = y.wrapping_add(step_y);
        destination_row += destination_stride;
    }
}

/// The overlap `sub_410D80` and the other 91:1D kernels clip to.
fn overlap(destination: &TargetBitmap, source: &TargetBitmap) -> (usize, usize) {
    (destination.width.min(source.width), destination.height.min(source.height))
}

/// `x + ((target - x) * weight >> 7)` per lane with `packus` saturation.
fn lerp7(x: u8, target: i32, weight: i32) -> u8 {
    let x = i32::from(x);
    (x + (((target - x) * weight) >> 7)).clamp(0, 255) as u8
}

/// Runs `kernel` over the overlap two pixels at a time like the SSE
/// loops; with `skip_transparent` (format 2) a pair whose two alphas are
/// zero, or an odd last pixel with alpha zero, is written as zero.
fn for_each_pair(
    destination: &mut TargetBitmap,
    source: &TargetBitmap,
    skip_transparent: bool,
    kernel: impl Fn([u8; 4]) -> [u8; 4],
) {
    let (width, height) = overlap(destination, source);
    for y in 0..height {
        let (source_row, destination_row) = (y * source.stride, y * destination.stride);
        for x in (0..width).step_by(2) {
            let pair = x + 1 < width;
            let first = read_pixel(source, source_row + 4 * x);
            let second = if pair { read_pixel(source, source_row + 4 * x + 4) } else { [0; 4] };
            let clear = skip_transparent && first[3] == 0 && (!pair || second[3] == 0);
            for (index, pixel) in [first, second].into_iter().enumerate().take(1 + pair as usize) {
                let value = if clear { [0; 4] } else { kernel(pixel) };
                write_pixel(destination, destination_row + 4 * (x + index), u32::from_le_bytes(value));
            }
        }
    }
}

/// `sub_40AF70` (91:1D type 0): copy over the source size (not clipped
/// to the destination); a format-1 source gains alpha 255, a format-2
/// source going to another format is premultiplied (`qword_50B0F0`) with
/// alpha 0.
fn convert_copy(destination: &mut TargetBitmap, destination_format: u32, source: &TargetBitmap, source_format: u32) {
    if destination_format == source_format {
        let row_bytes = source.width * 4;
        for y in 0..source.height {
            let from = &source.pixels[y * source.stride..(y * source.stride + row_bytes).min(source.pixels.len())];
            let at = y * destination.stride;
            let end = (at + from.len()).min(destination.pixels.len());
            if at < end {
                destination.pixels[at..end].copy_from_slice(&from[..end - at]);
            }
        }
        return;
    }
    for y in 0..source.height {
        for x in 0..source.width {
            let pixel = read_pixel(source, y * source.stride + 4 * x);
            let value = match source_format {
                1 => u32::from_le_bytes(pixel) | 0xFF00_0000,
                2 => {
                    let weight = u16::from(pixel[3] >> 1);
                    let channel = |c: u8| ((u16::from(c) * weight) >> 7) as u8;
                    u32::from_le_bytes([channel(pixel[0]), channel(pixel[1]), channel(pixel[2]), 0])
                }
                _ => return,
            };
            write_pixel(destination, y * destination.stride + 4 * x, value);
        }
    }
}

/// `sub_40DE80`: 91:1D type 3 with colour 0 (fade to black).
fn fade_to_black(destination: &mut TargetBitmap, destination_format: u32, source: &TargetBitmap, source_format: u32, alpha: u32) {
    if alpha == 0 {
        return convert_copy(destination, destination_format, source, source_format);
    }
    let keep = 256u32.wrapping_sub(alpha) as u16;
    match (source_format, destination_format) {
        // sub_40A610: clear the destination.
        (1, 1) if alpha >= 0x100 => {
            for y in 0..destination.height {
                let at = y * destination.stride;
                let end = (at + destination.width * 4).min(destination.pixels.len());
                destination.pixels[at.min(end)..end].fill(0);
            }
        }
        // sub_40DF10: scale by 256 - alpha over the source size, alpha 0.
        (1, 1) => {
            for y in 0..source.height {
                for x in 0..source.width {
                    let pixel = read_pixel(source, y * source.stride + 4 * x);
                    let channel = |c: u8| (u16::from(c).wrapping_mul(keep) >> 8).min(255) as u8;
                    let value = [channel(pixel[0]), channel(pixel[1]), channel(pixel[2]), 0];
                    write_pixel(destination, y * destination.stride + 4 * x, u32::from_le_bytes(value));
                }
            }
        }
        // sub_40E040: blend the scaled source over the destination by the
        // source alpha; pairs with both alphas zero are left alone.
        (2, 1) => {
            for y in 0..source.height {
                for x in (0..source.width).step_by(2) {
                    let pair = x + 1 < source.width;
                    let pixels = [x, x + 1].map(|column| read_pixel(source, y * source.stride + 4 * column));
                    if pair && pixels[0][3] == 0 && pixels[1][3] == 0 || !pair && pixels[0][3] >> 1 == 0 {
                        continue;
                    }
                    for (index, pixel) in pixels.into_iter().enumerate().take(1 + pair as usize) {
                        let at = y * destination.stride + 4 * (x + index);
                        let target = read_pixel(destination, at);
                        let weight = i32::from(pixel[3] >> 1);
                        let mut value = [0u8; 4];
                        for lane in 0..4 {
                            let scaled = if lane == 3 { 0 } else { i32::from((u16::from(pixel[lane]).wrapping_mul(keep)) >> 8) };
                            value[lane] = lerp7(target[lane], scaled, if lane == 3 { 0 } else { weight });
                        }
                        write_pixel(destination, at, u32::from_le_bytes(value));
                    }
                }
            }
        }
        // sub_40E220: scalar "over" with the destination alpha.
        (2, 2) => {
            let keep = 256u32.wrapping_sub(alpha);
            for y in 0..source.height {
                for x in 0..source.width {
                    let pixel = read_pixel(source, y * source.stride + 4 * x);
                    if pixel[3] == 0 {
                        continue;
                    }
                    let at = y * destination.stride + 4 * x;
                    let target = read_pixel(destination, at);
                    let sa = u32::from(pixel[3]);
                    let lower = u32::from(target[3]).wrapping_mul(256 - sa);
                    let total = lower.wrapping_add(sa << 8);
                    let source_weight = (keep.wrapping_mul(sa) << 16) / total;
                    let destination_weight = (lower << 16) / total;
                    let channel = |lane: usize| {
                        (source_weight.wrapping_mul(u32::from(pixel[lane]))
                            .wrapping_add(destination_weight.wrapping_mul(u32::from(target[lane])))
                            >> 16) as u8
                    };
                    let value = [channel(0), channel(1), channel(2), (total >> 8) as u8];
                    write_pixel(destination, at, u32::from_le_bytes(value));
                }
            }
        }
        _ => {}
    }
}

/// `sub_419CF0` (91:1D) on 32-bit bitmaps: 0 copy/convert, 1 XOR with
/// `colour`, 2 monochrome tint, 3 fade toward `colour`, 4/5 saturating
/// add/subtract of `colour * alpha >> 8`; `alpha` is 0..=256. The format-0
/// (RGB555) branches of types 3 are not reachable from the app's rasters.
pub(crate) fn process(
    destination: &mut TargetBitmap,
    destination_format: u32,
    source: &TargetBitmap,
    source_format: u32,
    kind: u32,
    colour: u32,
    alpha: u32,
) {
    let same = destination_format == source_format;
    let truecolor = matches!(source_format, 1 | 2);
    let bytes = colour.to_le_bytes();
    let half = (alpha as u16 >> 1) as i32;
    match kind {
        0 => convert_copy(destination, destination_format, source, source_format),
        // sub_410D80 / sub_410F80
        1 if same && truecolor => for_each_pair(destination, source, source_format == 2, |pixel| {
            let mut value = pixel;
            for lane in 0..3 {
                value[lane] = lerp7(pixel[lane], i32::from(pixel[lane] ^ bytes[lane]), half);
            }
            value
        }),
        // sub_4111E0 / sub_4113B0: luminance 29/150/77 times the colour.
        2 if same && truecolor => for_each_pair(destination, source, source_format == 2, |pixel| {
            let luminance = (29 * u32::from(pixel[0]) + 150 * u32::from(pixel[1]) + 77 * u32::from(pixel[2])) & 0xFFFF;
            let mut value = pixel;
            for lane in 0..3 {
                value[lane] = lerp7(pixel[lane], ((luminance * u32::from(bytes[lane])) >> 16) as i32, half);
            }
            value
        }),
        3 if colour == 0 => fade_to_black(destination, destination_format, source, source_format, alpha),
        // sub_4116F0
        3 if same && truecolor => for_each_pair(destination, source, false, |pixel| {
            let keep = 256u16.wrapping_sub(alpha as u16);
            let mut value = pixel;
            for lane in 0..3 {
                let add = (alpha as u16).wrapping_mul(u16::from(bytes[lane]));
                value[lane] = (u16::from(pixel[lane]).wrapping_mul(keep).saturating_add(add) >> 8).min(255) as u8;
            }
            value
        }),
        // sub_4118E0 / sub_411A00
        4 | 5 if same && truecolor => {
            let step = bytes.map(|c| ((u16::from(c).wrapping_mul(alpha as u16)) >> 8).min(255) as u8);
            for_each_pair(destination, source, false, |pixel| {
                let mut value = pixel;
                for lane in 0..3 {
                    value[lane] = if kind == 4 { pixel[lane].saturating_add(step[lane]) } else { pixel[lane].saturating_sub(step[lane]) };
                }
                value
            })
        }
        _ => {}
    }
}

/// `sub_403190` (91:1E): `target` (x, y) is masked by `mask` (x + dx,
/// y + dy) over the overlap. Formats: a 32-bit mask (format 2) scales a
/// format-2 target's alpha (`sub_4164F0`); an 8-bit mask (format 3)
/// scales a format-3 target (`sub_415EC0`) or a format-2 target's alpha
/// (`sub_416020`). Other combinations return an error code the handler
/// ignores. 8-bit bitmaps use one byte per pixel.
pub(crate) fn grayscale_mask(
    target: &mut TargetBitmap,
    target_format: u32,
    mask: &TargetBitmap,
    mask_format: u32,
    dx: i32,
    dy: i32,
) {
    // sub_409180 / sub_409160 / sub_409100: signed 32-bit rectangles.
    let left = 0i32.wrapping_sub(dx).max(0);
    let top = 0i32.wrapping_sub(dy).max(0);
    let right = (target.width as i32 - 1).min((mask.width as i32 - 1).wrapping_sub(dx));
    let bottom = (target.height as i32 - 1).min((mask.height as i32 - 1).wrapping_sub(dy));
    if left > right || top > bottom {
        return;
    }
    let level = 256u32;
    for y in top..=bottom {
        let target_row = y as usize * target.stride;
        let mask_row = y.wrapping_add(dy) as usize * mask.stride;
        for x in left..=right {
            let (tx, mx) = (x as usize, x.wrapping_add(dx) as usize);
            match (mask_format, target_format) {
                (2, 2) => {
                    let m = u32::from(mask.pixels[mask_row + 4 * mx + 3]);
                    let at = target_row + 4 * tx + 3;
                    if m == 0 {
                        target.pixels[at] = 0;
                    } else if m * level != 65280 {
                        target.pixels[at] = ((m * level * u32::from(target.pixels[at])) >> 16) as u8;
                    }
                }
                (3, 3) => {
                    let m = u32::from(mask.pixels[mask_row + mx]);
                    let at = target_row + tx;
                    let value = u32::from(target.pixels[at]);
                    target.pixels[at] = if value != 0 && m != 0 { ((value * (m + 1)) >> 8) as u8 } else { 0 };
                }
                (3, 2) => {
                    let m = u16::from(mask.pixels[mask_row + mx]);
                    let at = target_row + 4 * tx + 3;
                    target.pixels[at] = (m.wrapping_mul(u16::from(target.pixels[at])) >> 8) as u8;
                }
                _ => return,
            }
        }
    }
}

/// x87 `fistp m64` / `__ftol2_sse` in truncating mode, low dword kept
/// (out of range gives the indefinite integer, whose low dword is 0).
fn truncate_low(value: f64) -> i32 {
    if value.is_finite() && value.abs() < 9.223_372_036_854_775_808e18 {
        value as i64 as i32
    } else {
        0
    }
}

/// `sub_4169D0`: the 16.16 source-space walk of the rotate-zoom blits,
/// as (start, step per destination column, step per destination row),
/// each (u, v). x87 at the Windows default 53-bit precision, so every
/// operation rounds like f64; the operation order is the target's.
fn affine_walk(
    dx: i32,
    dy: i32,
    sx: i32,
    sy: i32,
    angle: i32,
    x_scale: u32,
    y_scale: u32,
) -> ([i32; 2], [i32; 2], [i32; 2]) {
    const K: f64 = 65536.0;
    const Q: f64 = 1.52587890625e-05;
    let theta = -(f64::from(angle) * std::f64::consts::PI / 11796480.0);
    let (c, s) = (theta.cos(), theta.sin());
    let ix = K / f64::from(x_scale);
    let iy = K / f64::from(y_scale);
    let px = -(f64::from(dx) * Q);
    let py = f64::from(dy) * Q;
    let v0 = truncate_low((f64::from(sy) * Q - (py * c + px * s) * iy) * K);
    let u0 = truncate_low(((px * c - py * s) * ix + f64::from(sx) * Q) * K);
    let column = [truncate_low(c * ix * K), truncate_low(s * iy * -K)];
    let theta = theta + std::f64::consts::FRAC_PI_2;
    let row = [truncate_low(theta.cos() * ix * -K), truncate_low(theta.sin() * iy * K)];
    ([u0, v0], column, row)
}

/// The MMX walk shared by the rotate-zoom kernels: for every destination
/// pixel, the source position (`psrad 16` + `packssdw`, so saturated to
/// i16) and its 4-bit fractions (`pand 0xF000`, `psrlw 9`).
fn for_each_walk(
    destination: &mut TargetBitmap,
    walk: ([i32; 2], [i32; 2], [i32; 2]),
    mut kernel: impl FnMut(&mut [u8], i32, i32, usize, usize),
) {
    let (mut row_start, column, row) = walk;
    for y in 0..destination.height {
        let mut position = row_start;
        row_start = [row_start[0].wrapping_add(row[0]), row_start[1].wrapping_add(row[1])];
        for x in 0..destination.width {
            let current = position;
            position = [position[0].wrapping_add(column[0]), position[1].wrapping_add(column[1])];
            let sat = |value: i32| (value >> 16).clamp(-32768, 32767);
            let fraction = |value: i32| ((value as u32 & 0xF000) >> 12) as usize;
            let at = y * destination.stride + 4 * x;
            if let Some(pixel) = destination.pixels.get_mut(at..at + 4) {
                kernel(pixel, sat(current[0]), sat(current[1]), fraction(current[0]), fraction(current[1]));
            }
        }
    }
}

/// Source pixel (x, y) when both are unsigned-below the size, else 0;
/// `opaque` sets the alpha byte of a pixel that is read (`por 0xFF000000`).
fn sample_guarded(source: &TargetBitmap, x: i32, y: i32, opaque: bool) -> [i32; 4] {
    if (x as u32) < source.width as u32 && (y as u32) < source.height as u32 {
        let mut pixel = read_pixel(source, y as usize * source.stride + 4 * x as usize);
        if opaque {
            pixel[3] = 255;
        }
        pixel.map(i32::from)
    } else {
        [0; 4]
    }
}

/// `pmulhw((a - b) << 4, (16 - k) << 8) + b` per lane.
fn lerp16(a: i32, b: i32, k: usize) -> i32 {
    ((((a - b) << 4) as i16 as i32 * ((16 - k as i32) << 8)) >> 16) + b
}

/// The 4-bit bilinear sample of `sub_417ED0` / `sub_416E20` / `sub_4174E0`
/// (unsaturated 16-bit lanes). The kernels' early exits all coincide with
/// four zero neighbours.
fn sample_bilinear4(source: &TargetBitmap, x: i32, y: i32, fx: usize, fy: usize, opaque: bool) -> [i32; 4] {
    if y <= -2 || y >= source.height as i32 || (y >= 0 && (x <= -2 || x >= source.width as i32)) {
        return [0; 4];
    }
    let p00 = sample_guarded(source, x, y, opaque);
    let p01 = sample_guarded(source, x + 1, y, opaque);
    let p10 = sample_guarded(source, x, y + 1, opaque);
    let p11 = sample_guarded(source, x + 1, y + 1, opaque);
    std::array::from_fn(|lane| {
        let top = lerp16(p00[lane], p01[lane], fx);
        let bottom = lerp16(p10[lane], p11[lane], fx);
        lerp16(top, bottom, fy)
    })
}

/// The weight table of `sub_4177E0` / `sub_4174E0`: entry k (alpha >> 1)
/// is k * (256 - alpha) >> 8, entry 127 is 128 * (256 - alpha) >> 8.
fn alpha_weight(k: u32, alpha: u32) -> i32 {
    let keep = 256u32.wrapping_sub(alpha);
    let k = if k == 127 { 128 } else { k };
    (k.wrapping_mul(keep) >> 8) as u16 as i16 as i32
}

/// `d + ((v - d) * w >> 7)` on 16-bit lanes, `packuswb`.
fn blend7(d: u8, v: i32, weight: i32) -> u8 {
    (i32::from(d) + (((v - i32::from(d)) * weight) as i16 as i32 >> 7)).clamp(0, 255) as u8
}

/// `d + pmulhw((v - d) << 4, (256 - alpha) << 4)` on the colour lanes.
fn blend_keep(d: u8, v: i32, alpha: u32) -> u8 {
    let keep = (256u32.wrapping_sub(alpha) << 4) as u16 as i16 as i32;
    let diff = ((v - i32::from(d)) << 4) as i16 as i32;
    (i32::from(d) + ((diff * keep) >> 16)).clamp(0, 255) as u8
}

/// `sub_416B60` (91:18, single-threaded): rotate-zoom blit of `source` into
/// `destination`. `dx`/`dy` and `sx`/`sy` are the 16.16 anchors in the
/// destination and source, `angle` 16.16 degrees, `x_scale`/`y_scale`
/// 16.16, `alpha` 0..=256 the transparency, `filter` bilinear sampling.
/// Whole anchors, unit scales and a multiple of 360 degrees with the
/// source covering the destination take the plain blit (`sub_416890` ->
/// `sub_40B560`); otherwise only a format-1 destination is drawn, over its
/// whole area. The target splits large destinations into bands for its
/// worker threads (`sub_41A120`); this runs the single-band path.
#[allow(clippy::too_many_arguments)]
pub(crate) fn rotate_blit(
    destination: &mut TargetBitmap,
    destination_format: u32,
    source: &TargetBitmap,
    source_format: u32,
    [dx, dy, sx, sy]: [i32; 4],
    angle: i32,
    x_scale: u32,
    y_scale: u32,
    alpha: u32,
    filter: bool,
) {
    if x_scale == 0 || y_scale == 0 {
        return;
    }
    if dx as u16 == 0 && dy as u16 == 0 && sx as u16 == 0 && sy as u16 == 0 && angle % 23592960 == 0
        && x_scale == 0x10000 && y_scale == 0x10000
    {
        let (left, top) = ((sx >> 16).wrapping_sub(dx >> 16), (sy >> 16).wrapping_sub(dy >> 16));
        let right = left.wrapping_add(destination.width as i32 - 1);
        let bottom = top.wrapping_add(destination.height as i32 - 1);
        if left >= 0 && top >= 0 && right <= source.width as i32 - 1 && bottom <= source.height as i32 - 1 {
            plain_blit(destination, destination_format, source, source_format, left, top, alpha);
            return;
        }
    }
    if destination_format != 1 || alpha >= 0x100 {
        return;
    }
    let walk = affine_walk(dx, dy, sx, sy, angle, x_scale, y_scale);
    match (source_format, filter) {
        // sub_418500 / sub_4173B0: nearest, 0 outside.
        (1, false) => for_each_walk(destination, walk, |pixel, x, y, _, _| {
            let value = sample_guarded(source, x, y, false);
            for lane in 0..4 {
                pixel[lane] = if alpha == 0 {
                    value[lane] as u8
                } else if lane == 3 {
                    pixel[lane]
                } else {
                    blend_keep(pixel[lane], value[lane], alpha)
                };
            }
        }),
        // sub_417ED0 / sub_416E20: bilinear, 0 outside.
        (1, true) => for_each_walk(destination, walk, |pixel, x, y, fx, fy| {
            let value = sample_bilinear4(source, x, y, fx, fy, false);
            for lane in 0..4 {
                pixel[lane] = if alpha == 0 {
                    value[lane].clamp(0, 255) as u8
                } else if lane == 3 {
                    pixel[lane]
                } else {
                    blend_keep(pixel[lane], value[lane], alpha)
                };
            }
        }),
        // sub_4177E0: nearest, blended by the source alpha, nothing outside.
        (2, false) => for_each_walk(destination, walk, |pixel, x, y, _, _| {
            if (x as u32) >= source.width as u32 || (y as u32) >= source.height as u32 {
                return;
            }
            let value = sample_guarded(source, x, y, false);
            let k = value[3] as u32 >> 1;
            if k != 0 {
                let weight = alpha_weight(k, alpha);
                for lane in 0..3 {
                    pixel[lane] = blend7(pixel[lane], value[lane], weight);
                }
            }
        }),
        // sub_4174E0: bilinear, blended by the sampled alpha.
        (2, true) => for_each_walk(destination, walk, |pixel, x, y, fx, fy| {
            let value = sample_bilinear4(source, x, y, fx, fy, false);
            let k = value[3].clamp(0, 255) as u32 >> 1;
            if k != 0 {
                let weight = alpha_weight(k, alpha);
                for lane in 0..3 {
                    pixel[lane] = blend7(pixel[lane], value[lane], weight);
                }
            }
        }),
        _ => {}
    }
}

/// `sub_4179B0` (91:19, single-threaded): the rotate-zoom of 91:18 as a
/// copy faded by `alpha` (`[256 - alpha] * colour >> 8`, alpha kept),
/// written over the whole destination (0 outside the source). The plain
/// path (`sub_418A80`) converts (`sub_40AF70`) or fades (`sub_418AB0`) the
/// covering source rectangle; otherwise equal formats 1/2 are drawn
/// (`sub_418500`/`sub_4183E0` nearest, `sub_417ED0`/`sub_417C20` bilinear,
/// alpha 256 clears through `sub_40A610`) and a format-1 source into a
/// format-2 destination becomes opaque (`sub_4185F0` / `sub_418140`).
#[allow(clippy::too_many_arguments)]
pub(crate) fn rotate_copy(
    destination: &mut TargetBitmap,
    destination_format: u32,
    source: &TargetBitmap,
    source_format: u32,
    [dx, dy, sx, sy]: [i32; 4],
    angle: i32,
    x_scale: u32,
    y_scale: u32,
    alpha: u32,
    filter: bool,
) {
    if x_scale == 0 || y_scale == 0 {
        return;
    }
    let keep = 256u32.wrapping_sub(alpha) as u16;
    let fade = |value: [i32; 4]| -> [u8; 4] {
        std::array::from_fn(|lane| {
            let weight = if lane == 3 { 256 } else { keep };
            ((value[lane] as u16).wrapping_mul(weight) >> 8).min(255) as u8
        })
    };
    if dx as u16 == 0 && dy as u16 == 0 && sx as u16 == 0 && sy as u16 == 0 && angle % 23592960 == 0
        && x_scale == 0x10000 && y_scale == 0x10000
    {
        let (left, top) = ((sx >> 16).wrapping_sub(dx >> 16), (sy >> 16).wrapping_sub(dy >> 16));
        let right = left.wrapping_add(destination.width as i32 - 1);
        let bottom = top.wrapping_add(destination.height as i32 - 1);
        if left >= 0 && top >= 0 && right <= source.width as i32 - 1 && bottom <= source.height as i32 - 1 {
            let view = TargetBitmap {
                pixels: source.pixels[top as usize * source.stride + 4 * left as usize..].to_vec(),
                stride: source.stride,
                width: destination.width,
                height: destination.height,
            };
            if alpha == 0 {
                convert_copy(destination, destination_format, &view, source_format);
            } else if destination_format == source_format && matches!(source_format, 1 | 2) {
                for y in 0..destination.height {
                    for x in 0..destination.width {
                        let pixel = read_pixel(&view, y * view.stride + 4 * x).map(i32::from);
                        write_pixel(destination, y * destination.stride + 4 * x, u32::from_le_bytes(fade(pixel)));
                    }
                }
            }
            return;
        }
    }
    let walk = affine_walk(dx, dy, sx, sy, angle, x_scale, y_scale);
    let write = |pixel: &mut [u8], value: [u8; 4]| pixel.copy_from_slice(&value);
    if destination_format != source_format {
        if source_format == 1 && destination_format == 2 {
            for_each_walk(destination, walk, |pixel, x, y, fx, fy| {
                let value = if filter {
                    sample_bilinear4(source, x, y, fx, fy, true)
                } else {
                    sample_guarded(source, x, y, true)
                };
                write(pixel, value.map(|lane| lane.clamp(0, 255) as u8));
            });
        }
        return;
    }
    if !matches!(source_format, 1 | 2) {
        return;
    }
    if alpha >= 0x100 {
        for y in 0..destination.height {
            let at = y * destination.stride;
            let end = (at + destination.width * 4).min(destination.pixels.len());
            destination.pixels[at.min(end)..end].fill(0);
        }
        return;
    }
    for_each_walk(destination, walk, |pixel, x, y, fx, fy| {
        let value = if filter {
            sample_bilinear4(source, x, y, fx, fy, false)
        } else {
            sample_guarded(source, x, y, false)
        };
        let value = if alpha == 0 { value.map(|lane| lane.clamp(0, 255) as u8) } else { fade(value) };
        write(pixel, value);
    });
}

/// `sub_4193D0` (90:CD, single-threaded) on equal 32-bit formats over the
/// overlap: monochrome tint (luminance 29/150/77 times `mono_colour`,
/// mixed by `mono_level`), the tone-curve lookup (`tables` by BGRA lane,
/// ORed into one pixel), then the colour film (multiply below 128, screen
/// above, against `film_colour`) mixed by `film_level`. Format 2 keeps the
/// source alpha, and a pair of fully transparent pixels (or an odd last
/// one) becomes 0 (`sub_4198E0`); format 1 runs `sub_419550`.
#[allow(clippy::too_many_arguments)]
pub(crate) fn tone_curve(
    destination: &mut TargetBitmap,
    source: &TargetBitmap,
    format: u32,
    tables: &[[u8; 256]; 3],
    mono_colour: u32,
    mono_level: u32,
    film_colour: u32,
    film_level: u32,
) {
    let mono = mono_colour.to_le_bytes().map(u32::from);
    let film = film_colour.to_le_bytes().map(i32::from);
    let mono_weight = |lane: usize| if lane == 3 { 128 } else { (mono_level as u16 >> 1) as i32 };
    let film_weight = |lane: usize| if lane == 3 { 128 } else { (film_level as u16 >> 1) as i32 };
    let mix = |x: i32, target: i32, weight: i32| x + (((target - x) * weight) as i16 as i32 >> 7);
    let kernel = |pixel: [u8; 4]| -> [u8; 4] {
        let x = pixel.map(i32::from);
        let luminance = (29 * x[0] + 150 * x[1] + 77 * x[2]) as u32 & 0xFFFF;
        let toned: [i32; 4] =
            std::array::from_fn(|lane| mix(x[lane], ((luminance * mono[lane]) >> 16) as i32, mono_weight(lane)));
        let index = |lane: usize| (toned[lane] as u16 as usize).min(255);
        let mapped = [tables[0][index(0)], tables[1][index(1)], tables[2][index(2)], 0].map(i32::from);
        std::array::from_fn(|lane| {
            let v = mapped[lane];
            let high = if v > if lane == 3 { 255 } else { 127 } { 0xFF } else { 0 };
            let filmed = ((((v ^ high) * (film[lane] ^ high)) as u16 >> 7) as i32) ^ high;
            mix(v, filmed, film_weight(lane)).clamp(0, 255) as u8
        })
    };
    let (width, height) = overlap(destination, source);
    for y in 0..height {
        let (source_row, destination_row) = (y * source.stride, y * destination.stride);
        for x in (0..width).step_by(2) {
            let pair = x + 1 < width;
            let pixels = [x, x + 1].map(|column| read_pixel(source, source_row + 4 * column));
            let clear = format == 2 && pixels[0][3] == 0 && (!pair || pixels[1][3] == 0);
            for (index, pixel) in pixels.into_iter().enumerate().take(1 + pair as usize) {
                let mut value = if clear { [0; 4] } else { kernel(pixel) };
                if format == 2 && !clear {
                    value[3] = pixel[3];
                }
                write_pixel(destination, destination_row + 4 * (x + index), u32::from_le_bytes(value));
            }
        }
    }
}

/// `sub_40B560`: the source rectangle at (left, top) of the destination's
/// size blitted over the whole destination with transparency `alpha`
/// (bitmap_blend's mode-1 ports; their kernels are channel-wise, so the
/// BGRA layout can be handed over as is).
fn plain_blit(
    destination: &mut TargetBitmap,
    destination_format: u32,
    source: &TargetBitmap,
    source_format: u32,
    left: i32,
    top: i32,
    alpha: u32,
) {
    let as_image = |bitmap: &TargetBitmap| {
        let mut rgba = Vec::with_capacity(bitmap.width * bitmap.height * 4);
        for y in 0..bitmap.height {
            rgba.extend_from_slice(&bitmap.pixels[y * bitmap.stride..y * bitmap.stride + bitmap.width * 4]);
        }
        DecodedImage { width: bitmap.width as u32, height: bitmap.height as u32, rgba }
    };
    let mut image = as_image(destination);
    crate::bitmap_blend::blit_alpha_over_parameter(
        &mut image,
        destination_format as i32,
        &as_image(source),
        source_format as i32,
        -left,
        -top,
        alpha as i32,
    );
    for y in 0..destination.height {
        let row = &image.rgba[y * destination.width * 4..(y + 1) * destination.width * 4];
        destination.pixels[y * destination.stride..y * destination.stride + row.len()].copy_from_slice(row);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn words(bytes: &[u8], count: usize) -> Vec<u32> {
        bytes[..count * 4]
            .chunks_exact(4)
            .map(|word| u32::from_le_bytes(word.try_into().unwrap()))
            .collect()
    }

    /// Vectors from `src/tools/emu_kernels.py` (the target under Unicorn).
    #[test]
    #[ignore]
    fn kernels_match_the_target_vectors() {
        let directory = std::env::var("ETHORNELL_KERNEL_VECTORS")
            .expect("ETHORNELL_KERNEL_VECTORS names the vector directory");
        let mut checked = 0;
        let mut failed = Vec::new();
        let mut entries = std::fs::read_dir(&directory)
            .unwrap()
            .map(|entry| entry.unwrap().path())
            .collect::<Vec<_>>();
        entries.sort();
        for path in entries {
            let data = std::fs::read(&path).unwrap();
            let header = words(&data, 8);
            let [kind, sw, sh, dw, dh, p0, p1, p2] = header[..] else {
                unreachable!()
            };
            let (sw, sh, dw, dh) = (sw as usize, sh as usize, dw as usize, dh as usize);
            // Kinds 6..=8 carry p2 more words after the header.
            let extra = if matches!(kind, 6..=8) { words(&data[32..], p2 as usize) } else { Vec::new() };
            let mut at = 32 + 4 * extra.len();
            let mut take = |n: usize| {
                let slice = data[at..at + n].to_vec();
                at += n;
                slice
            };
            // Kind 5 packs the formats in p2; format 3 is one byte a pixel.
            let bpp = |format: u32| if kind == 5 && format == 3 || matches!(kind, 9 | 10) { 1 } else { 4 };
            let (sb, db) = (bpp(p2 & 0xFF), bpp(p2 >> 8 & 0xFF));
            let source = TargetBitmap { pixels: take(sw * sh * sb), stride: sw * sb, width: sw, height: sh };
            let before = take(dw * dh * db);
            let expected = take(dw * dh * db);
            let mut destination = TargetBitmap { pixels: before, stride: dw * db, width: dw, height: dh };
            match kind {
                1 => concentrate(&mut destination, &source, p0, p1, p2),
                2 => scale_nearest(&mut destination, &source, p0, p1),
                3 => scale_filtered(&mut destination, &source, p2, p0, p1),
                6 => rotate_blit(
                    &mut destination,
                    p0 >> 8 & 0xFF,
                    &source,
                    p0 & 0xFF,
                    [extra[0] as i32, extra[1] as i32, extra[2] as i32, extra[3] as i32],
                    extra[4] as i32,
                    extra[5],
                    extra[6],
                    p1,
                    p0 >> 16 != 0,
                ),
                7 => rotate_copy(
                    &mut destination,
                    p0 >> 8 & 0xFF,
                    &source,
                    p0 & 0xFF,
                    [extra[0] as i32, extra[1] as i32, extra[2] as i32, extra[3] as i32],
                    extra[4] as i32,
                    extra[5],
                    extra[6],
                    p1,
                    p0 >> 16 != 0,
                ),
                8 => {
                    let mut points = [[0i32; 2]; 3];
                    for (index, point) in points.iter_mut().enumerate() {
                        *point = [extra[2 * index] as i32, extra[2 * index + 1] as i32];
                    }
                    let lut = crate::graph_color::RuntimeColorLut::from_control_points(points).unwrap();
                    tone_curve(&mut destination, &source, p0 & 0xFF, lut.lane_tables(), extra[6], extra[7], extra[8], extra[9]);
                }
                // The encoders' output replaces the zeroed "destination";
                // an empty one stands for a rejected encoding (p2 = 1).
                10 => {
                    let header: [u8; 16] = source.pixels[..16].try_into().unwrap();
                    let encoded = ethornell_image::encode_cbg_v2(&header, &source.pixels[16..], p1, p0);
                    assert_eq!(encoded.is_none(), p2 == 1, "{}", path.display());
                    destination.pixels = encoded.unwrap_or_default();
                }
                9 => {
                    let header: [u8; 16] = source.pixels[..16].try_into().unwrap();
                    destination.pixels = ethornell_image::encode_cbg_v1(&header, &source.pixels[16..], p0);
                }
                5 => grayscale_mask(&mut destination, p2 >> 8 & 0xFF, &source, p2 & 0xFF, p0 as i32, p1 as i32),
                4 => process(&mut destination, p2 >> 8 & 0xFF, &source, p2 & 0xFF, p2 >> 16, p0, p1),
                _ => panic!("unknown kernel {kind}"),
            }
            if destination.pixels != expected {
                failed.push(path.file_name().unwrap().to_string_lossy().into_owned());
            }
            checked += 1;
        }
        assert!(checked > 0);
        assert!(failed.is_empty(), "{} of {checked} vectors differ: {failed:?}", failed.len());
    }
}
