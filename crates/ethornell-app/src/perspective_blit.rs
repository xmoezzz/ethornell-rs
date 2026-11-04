//! Graph90:C8 (sub_404FE0 -> sub_40FA40): place a bitmap's corners in 3D,
//! rotate and project them, rasterize the quad with perspective-correct
//! texture coordinates (sub_40EB10) and sample the source bilinearly into the
//! destination (sub_40FC60, sub_410110, sub_4104F0).
//!
//! The target samples with SSE: `rcpps` plus one Newton step for 1/w and
//! 15-bit fractions for the bilinear weights. `rcpps` differs between CPU
//! vendors, so an exact f32 reciprocal stands in for it. Coordinates that land
//! left of or above the source make the target read before the pixel buffer;
//! those samples count as transparent here.

use ethornell_image::DecodedImage;

const FIXED: f64 = 1.0 / 65536.0;
/// Angles are 16.16 degrees: pi / (180 << 16).
const ANGLE: f64 = std::f64::consts::PI / 11_796_480.0;
const SNAP: f64 = 1.0 / 1024.0;

/// Graph90:C8 operands after the destination and source handles, in the
/// order the script pushes them.
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct PerspectiveBlit {
    pub dest_x: i32,
    pub dest_y: i32,
    pub anchor_x: i32,
    pub anchor_y: i32,
    pub scale_x: i32,
    pub scale_y: i32,
    pub translate_x: i32,
    pub translate_y: i32,
    pub translate_z: i32,
    pub pitch: i32,
    pub heading: i32,
    pub bank: i32,
    pub order: i32,
    pub distance: i32,
    pub mode: i32,
    pub parameter: i32,
}

/// One projected corner: x, y, w and the source u, v premultiplied by w.
type Corner = [f32; 5];

/// sub_40F700: the source corners (0,0), (0,h-1), (w-1,h-1), (w-1,0) around
/// the scaled anchor, rotated in the selected axis order and projected.
pub(crate) fn project_corners(width: u32, height: u32, blit: &PerspectiveBlit) -> [Corner; 4] {
    let scale_x = blit.scale_x as u32 as f64 * FIXED;
    let scale_y = blit.scale_y as u32 as f64 * FIXED;
    let left = blit.anchor_x as f64 * FIXED * scale_x;
    let top = blit.anchor_y as f64 * FIXED * scale_y;
    let right = scale_x * width.wrapping_sub(1) as f64 - left;
    let bottom = scale_y * height.wrapping_sub(1) as f64 - top;
    let (last_u, last_v) = (width.wrapping_sub(1) as f32, height.wrapping_sub(1) as f32);
    let mut corners = [
        [-left as f32, -top as f32, 0.0, 0.0, 0.0],
        [-left as f32, bottom as f32, 0.0, 0.0, last_v],
        [right as f32, bottom as f32, 0.0, last_u, last_v],
        [right as f32, -top as f32, 0.0, last_u, 0.0],
    ];
    let order = ["phb", "pbh", "hpb", "hbp", "bph", "bhp"][(blit.order as u32 % 6) as usize];
    let pitch = blit.pitch.wrapping_neg() as f64 * ANGLE;
    let heading = blit.heading as f64 * ANGLE;
    let bank = blit.bank.wrapping_neg() as f64 * ANGLE;
    let (translate_x, translate_y, translate_z) = (
        blit.translate_x as f64 * FIXED,
        blit.translate_y as f64 * FIXED,
        blit.translate_z as f64 * FIXED,
    );
    let (dest_x, dest_y) = (blit.dest_x as f64 * FIXED, blit.dest_y as f64 * FIXED);
    for corner in &mut corners {
        let (mut x, mut y, mut z) = (corner[0] as f64, corner[1] as f64, corner[2] as f64);
        for axis in order.bytes() {
            match axis {
                b'b' => {
                    let (sin, cos) = bank.sin_cos();
                    (x, y) = (cos * x - sin * y, x * sin + y * cos);
                }
                b'h' => {
                    let (sin, cos) = heading.sin_cos();
                    (z, x) = (cos * z - sin * x, z * sin + x * cos);
                }
                _ => {
                    let (sin, cos) = pitch.sin_cos();
                    (y, z) = (cos * y - sin * z, y * sin + z * cos);
                }
            }
        }
        let depth = translate_z + z;
        let w = if depth == 0.0 || blit.distance == 0 {
            1.0
        } else {
            let distance = blit.distance as f64;
            if depth <= 0.0 {
                (distance - depth) / distance
            } else {
                distance / (depth + distance)
            }
        };
        corner[0] = ((translate_x + x) * w + dest_x) as f32;
        corner[1] = ((translate_y + y) * w + dest_y) as f32;
        corner[2] = w as f32;
        corner[3] *= w as f32;
        corner[4] *= w as f32;
    }
    corners
}

/// One scanline: the covered pixel columns and w, u*w, v*w at `left` with
/// their per-pixel steps.
#[derive(Debug, Clone, Copy, PartialEq)]
pub(crate) struct Span {
    pub left: i32,
    pub right: i32,
    pub values: [f32; 3],
    pub steps: [f32; 3],
}

impl Span {
    /// sub_40EB10 writes a NaN/-0.0 pair for an empty row, which every pixel
    /// loop rejects as left > right.
    const EMPTY: Span = Span {
        left: 0x7FC0_0000,
        right: i32::MIN,
        values: [0.0; 3],
        steps: [0.0; 3],
    };
}

#[derive(Debug, Clone, PartialEq)]
pub(crate) struct Spans {
    pub start: i32,
    pub rows: Vec<Span>,
}

/// sub_4010B0: snap to an integer within 1/1024.
fn snap(value: f64) -> f64 {
    let floor = value.floor();
    if 1.0 - SNAP <= value - floor {
        value.ceil()
    } else if SNAP < value - floor {
        value
    } else {
        floor
    }
}

/// 40EB10's snapped, inclusive raster bounds.
pub(crate) fn bounds(corners:&[Corner;4])->[i32;4] {
    let axis=|axis,minimum| {let values=corners.iter().map(|c|snap(c[axis] as f64));if minimum {values.fold(f64::INFINITY,f64::min).floor() as i32}else{values.fold(f64::NEG_INFINITY,f64::max).ceil() as i32}};
    [axis(0,true),axis(1,true),axis(0,false),axis(1,false)]
}

/// One side of the quad as sub_40EB10 walks it: the current edge's end
/// vertex and x, w, u*w, v*w at the top of the current row with their
/// per-row steps.
#[derive(Clone, Copy)]
struct EdgeWalk {
    end: usize,
    step: isize,
    values: [f64; 4],
    slopes: [f64; 4],
}

impl EdgeWalk {
    fn advance(&mut self, corners: &[Corner; 4], row: i32, first_row: bool, last_row: i32) {
        let row_bottom = (row + 1) as f64;
        if corners[self.end][1] as f64 >= row_bottom || row >= last_row {
            return;
        }
        let end_y = corners[self.end][1] as f64;
        if !first_row {
            let dy = end_y - row as f64;
            for (value, slope) in self.values.iter_mut().zip(self.slopes) {
                *value += dy * slope;
            }
        }
        let mut start = self.end;
        let mut taken = false;
        loop {
            if taken {
                self.values = vertex_values(&corners[self.end]);
            }
            start = self.end;
            self.end = (self.end as isize + self.step).rem_euclid(4) as usize;
            let next_y = corners[self.end][1] as f64;
            if next_y < end_y || next_y >= row_bottom {
                break;
            }
            taken = true;
        }
        let (from, to) = (&corners[start], &corners[self.end]);
        let dy = to[1] as f64 - from[1] as f64;
        let from_values = vertex_values(from);
        let to_values = vertex_values(to);
        for index in 0..4 {
            self.slopes[index] = (to_values[index] - from_values[index]) / dy;
        }
        let offset = row_bottom - from[1] as f64 - 1.0;
        for (value, slope) in self.values.iter_mut().zip(self.slopes) {
            *value += offset * slope;
        }
    }

    fn next_row(&mut self) {
        for (value, slope) in self.values.iter_mut().zip(self.slopes) {
            *value += slope;
        }
    }
}

fn vertex_values(corner: &Corner) -> [f64; 4] {
    [
        corner[0] as f64,
        corner[2] as f64,
        corner[3] as f64,
        corner[4] as f64,
    ]
}

/// sub_40EB10: scanline spans of the projected quad for rows below `height`.
/// None when the quad has no height or width.
pub(crate) fn rasterize(corners: &[Corner; 4], height: i32) -> Option<Spans> {
    let snapped = |index: usize, axis: usize| snap(corners[index][axis] as f64);
    let (mut top, mut min_y, mut max_y) = (0, snapped(0, 1), snapped(0, 1));
    let (mut min_x, mut max_x) = (snapped(0, 0), snapped(0, 0));
    for index in 1..4 {
        let (x, y) = (snapped(index, 0), snapped(index, 1));
        if y < min_y {
            top = index;
            min_y = y;
        }
        max_y = max_y.max(y);
        min_x = min_x.min(x);
        max_x = max_x.max(x);
    }
    if max_y == min_y || min_x == max_x {
        return None;
    }
    let top_row = min_y.floor() as i32;
    let last_row = max_y.ceil() as i32;
    if last_row < 0 {
        return None;
    }
    let (min_column, max_column) = (min_x.floor() as i32, max_x.ceil() as i32);
    let start = top_row.max(0);
    let end = last_row.min(height - 1);
    let mut rows = Vec::with_capacity((end - start + 1).max(0) as usize);

    let origin = vertex_values(&corners[top]);
    let mut first = EdgeWalk {
        end: top,
        step: 1,
        values: origin,
        slopes: [0.0; 4],
    };
    let mut second = EdgeWalk {
        end: top,
        step: -1,
        ..first
    };
    let (mut previous_left, mut previous_right) = (i32::MAX, i32::MIN);
    let mut gradient = [0.0f64; 3];
    let mut row = start;
    while row <= last_row && row < height {
        let first_row = row == top_row;
        first.advance(corners, row, first_row, last_row);
        second.advance(corners, row, first_row, last_row);

        let (mut left_x, mut left_values) = (
            first.values[0],
            [first.values[1], first.values[2], first.values[3]],
        );
        let (mut right_x, mut right_values) = (
            second.values[0],
            [second.values[1], second.values[2], second.values[3]],
        );
        if first_row && right_x == left_x {
            // A single top vertex: take the gradient one row lower.
            let next_left = left_x + first.slopes[0];
            let next_right = right_x + second.slopes[0];
            let width = next_right - next_left;
            if width != 0.0 {
                let mut next_gradient = [0.0; 3];
                for index in 0..3 {
                    next_gradient[index] = (second.values[index + 1] + second.slopes[index + 1]
                        - (first.values[index + 1] + first.slopes[index + 1]))
                        / width;
                }
                for index in 0..3 {
                    left_values[index] =
                        next_gradient[index] * first.slopes[0] + first.values[index + 1];
                    right_values[index] =
                        next_gradient[index] * second.slopes[0] + second.values[index + 1];
                }
                (left_x, right_x) = (next_left, next_right);
            }
        }
        if right_x < left_x {
            std::mem::swap(&mut left_x, &mut right_x);
            std::mem::swap(&mut left_values, &mut right_values);
        }
        let width = right_x - left_x;
        if width <= 0.0 {
            rows.push(Span::EMPTY);
        } else {
            if first_row || width >= 2.0 {
                for index in 0..3 {
                    gradient[index] = (right_values[index] - left_values[index]) / width;
                }
            }
            let left_snapped = snap(left_x);
            let left_column = left_snapped.floor() as i32;
            let right_column = snap(right_x).ceil() as i32;
            let (mut next_left_column, mut next_right_column) = (left_column, right_column);
            if row < last_row {
                let mut next_left = first.values[0] + first.slopes[0];
                let mut next_right = second.values[0] + second.slopes[0];
                if next_right < next_left {
                    std::mem::swap(&mut next_left, &mut next_right);
                }
                if next_right - next_left > 0.0 {
                    next_left_column = snap(next_left).floor() as i32;
                    next_right_column = snap(next_right).ceil() as i32;
                }
            }
            let left = previous_left
                .min(left_column)
                .min(next_left_column)
                .max(min_column);
            let right = previous_right
                .max(right_column)
                .max(next_right_column)
                .min(max_column);
            previous_left = left_column;
            previous_right = right_column;
            let offset = left_snapped - left as f64;
            let mut values = [0.0f32; 3];
            let mut steps = [0.0f32; 3];
            for index in 0..3 {
                values[index] = (left_values[index] - offset * gradient[index]) as f32;
                steps[index] = gradient[index] as f32;
            }
            rows.push(Span {
                left,
                right,
                values,
                steps,
            });
        }
        first.next_row();
        second.next_row();
        row += 1;
    }
    Some(Spans { start, rows })
}

/// Bitmap in the target's 32-bit layout with its registry format (1 opaque,
/// 2 alpha).
pub(crate) struct Surface<'a> {
    pub image: &'a DecodedImage,
    pub format: i32,
}

fn texel(image: &DecodedImage, x: i32, y: i32) -> Option<[i32; 4]> {
    (x >= 0 && y >= 0 && x < image.width as i32 && y < image.height as i32).then(|| {
        let offset = (y as usize * image.width as usize + x as usize) * 4;
        let pixel = &image.rgba[offset..offset + 4];
        [pixel[0], pixel[1], pixel[2], pixel[3]].map(i32::from)
    })
}

/// `a + mulhi((b - a) << 1, fraction)` on four 16-bit lanes.
fn lerp(a: [i32; 4], b: [i32; 4], fraction: i32) -> [i32; 4] {
    std::array::from_fn(|lane| {
        a[lane] + ((((b[lane] - a[lane]) << 1) as i16 as i32 * fraction) >> 16)
    })
}

/// cvtps2dq in the default round-to-nearest mode.
fn to_fixed(value: f32) -> i32 {
    let rounded = value.round_ties_even();
    if rounded >= -2_147_483_648.0 && rounded < 2_147_483_648.0 {
        rounded as i32
    } else {
        i32::MIN
    }
}

struct Sample {
    /// The two source rows, or None where the row is out of range.
    rows: [[i32; 4]; 2],
    vertical: i32,
    /// sub_410110 writes only after it read at least one texel.
    any_texel: bool,
}

fn sample(source: &DecodedImage, values: [f32; 3]) -> Sample {
    let inverse = 1.0 / values[0];
    let u = to_fixed(inverse * values[1] * 65536.0);
    let v = to_fixed(inverse * values[2] * 65536.0);
    let (x, y) = (u >> 16, v >> 16);
    let horizontal = (u & 0xFFFF) >> 1;
    let vertical = (v & 0xFFFF) >> 1;
    let mut any_texel = false;
    let mut row = |y: i32| {
        if y >= source.height as i32 {
            return [0; 4];
        }
        let left = texel(source, x, y);
        let right = texel(source, x + 1, y);
        any_texel |= left.is_some() || right.is_some();
        lerp(left.unwrap_or([0; 4]), right.unwrap_or([0; 4]), horizontal)
    };
    let rows = [row(y), row(y + 1)];
    Sample {
        rows,
        vertical,
        any_texel,
    }
}

fn pack(lanes: [i32; 4]) -> [u8; 4] {
    lanes.map(|lane| lane.clamp(0, 255) as u8)
}

/// sub_40FB20 for one destination: mode 0 replaces the destination with the
/// projected source (matching formats 1 or 2); mode 1 blends into a format-1
/// destination, by `parameter`/256 for an opaque source or by source alpha
/// scaled by (256 - parameter) for an alpha source. Returns whether the
/// destination changed.
pub(crate) fn draw(
    destination: &mut DecodedImage,
    destination_format: i32,
    source: &Surface<'_>,
    spans: Option<&Spans>,
    blit: &PerspectiveBlit,
) -> bool {
    let Some(spans) = spans else {
        // sub_40FA40 clears the whole destination in mode 0 when the quad is
        // degenerate, whatever the formats.
        if blit.mode == 0 {
            destination.rgba.fill(0);
            return true;
        }
        return false;
    };
    let kernel = match (blit.mode, destination_format, source.format) {
        (0, 1, 1) | (0, 2, 2) => Kernel::Replace,
        (1, 1, 1) => Kernel::Constant,
        (1, 1, 2) => Kernel::SourceAlpha,
        _ => return false,
    };
    let width = destination.width as i32;
    let height = destination.height as i32;
    let row_bytes = destination.width as usize * 4;
    let clear_rows = |destination: &mut DecodedImage, from: i32, to: i32| {
        let (from, to) = (from.clamp(0, height) as usize, to.clamp(0, height) as usize);
        if from < to {
            destination.rgba[from * row_bytes..to * row_bytes].fill(0);
        }
    };
    let first = spans.start.max(0);
    let end = if spans.rows.is_empty() {
        0
    } else {
        (spans.start + spans.rows.len() as i32).min(height)
    };
    if kernel == Kernel::Replace && spans.start >= 1 {
        clear_rows(destination, 0, spans.start);
    }
    for y in first..end {
        let span = spans.rows[(y - spans.start) as usize];
        let row = y as usize * row_bytes;
        if span.left > span.right || span.left >= width || span.right < 0 {
            if kernel == Kernel::Replace {
                destination.rgba[row..row + row_bytes].fill(0);
            }
            continue;
        }
        let (start, mut values) = if span.left < 1 {
            let left = span.left as f32;
            (
                0,
                std::array::from_fn(|index| span.values[index] - span.steps[index] * left),
            )
        } else {
            (span.left, span.values)
        };
        if kernel == Kernel::Replace {
            destination.rgba[row..row + start as usize * 4].fill(0);
        }
        let stop = if span.right >= width {
            width
        } else {
            span.right + 1
        };
        for x in start..stop {
            let offset = row + x as usize * 4;
            let sample = sample(source.image, values);
            let pixel = &mut destination.rgba[offset..offset + 4];
            match kernel {
                Kernel::Replace => {
                    let [top, bottom] = sample.rows;
                    let value = if top.iter().chain(&bottom).any(|lane| *lane != 0) {
                        pack(lerp(top, bottom, sample.vertical))
                    } else {
                        [0; 4]
                    };
                    pixel.copy_from_slice(&value);
                }
                Kernel::Constant if sample.any_texel => {
                    let color = lerp(sample.rows[0], sample.rows[1], sample.vertical);
                    let weight = (blit.parameter.wrapping_mul(16)) as i16 as i32;
                    let blended: [i32; 4] = std::array::from_fn(|lane| {
                        let difference =
                            ((i32::from(pixel[lane]) - color[lane]) << 4) as i16 as i32;
                        color[lane] + ((difference * weight) >> 16)
                    });
                    pixel.copy_from_slice(&pack(blended));
                }
                Kernel::SourceAlpha if sample.rows[0][3] | sample.rows[1][3] != 0 => {
                    let color = lerp(sample.rows[0], sample.rows[1], sample.vertical);
                    let opacity = 256i32.wrapping_sub(blit.parameter) as i16 as u16;
                    let weight = ((color[3] as u16).wrapping_mul(opacity) >> 1) as i16 as i32;
                    let blended: [i32; 4] = std::array::from_fn(|lane| {
                        let current = i32::from(pixel[lane]);
                        let difference = ((color[lane] - current) << 1) as i16 as i32;
                        current + ((difference * weight) >> 16)
                    });
                    pixel.copy_from_slice(&pack(blended));
                }
                _ => {}
            }
            for index in 0..3 {
                values[index] += span.steps[index];
            }
        }
        if kernel == Kernel::Replace && stop < width {
            destination.rgba[row + stop as usize * 4..row + row_bytes].fill(0);
        }
    }
    if kernel == Kernel::Replace && end < height && first < height {
        clear_rows(destination, end, height);
    }
    true
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Kernel {
    Replace,
    Constant,
    SourceAlpha,
}

#[cfg(test)]
mod tests {
    use super::*;

    fn identity(_width: u32, _height: u32) -> PerspectiveBlit {
        PerspectiveBlit {
            scale_x: 0x10000,
            scale_y: 0x10000,
            ..Default::default()
        }
    }

    fn image(width: u32, height: u32, pixel: impl Fn(u32, u32) -> [u8; 4]) -> DecodedImage {
        let mut rgba = Vec::new();
        for y in 0..height {
            for x in 0..width {
                rgba.extend_from_slice(&pixel(x, y));
            }
        }
        DecodedImage {
            width,
            height,
            rgba,
        }
    }

    #[test]
    fn identity_transform_keeps_corners_on_source_pixels() {
        let corners = project_corners(4, 3, &identity(4, 3));
        assert_eq!(corners[0], [0.0, 0.0, 1.0, 0.0, 0.0]);
        assert_eq!(corners[2], [3.0, 2.0, 1.0, 3.0, 2.0]);
    }

    #[test]
    fn heading_of_180_degrees_mirrors_x() {
        let blit = PerspectiveBlit {
            heading: 180 << 16,
            ..identity(4, 3)
        };
        let corners = project_corners(4, 3, &blit);
        assert!((corners[2][0] + 3.0).abs() < 1e-5);
        assert!((corners[2][1] - 2.0).abs() < 1e-5);
    }

    #[test]
    fn perspective_shrinks_points_behind_the_plane() {
        let blit = PerspectiveBlit {
            translate_z: 100 << 16,
            distance: 100,
            ..identity(4, 3)
        };
        let corners = project_corners(4, 3, &blit);
        assert!((corners[2][2] - 0.5).abs() < 1e-6);
        assert!((corners[2][0] - 1.5).abs() < 1e-5);
    }

    #[test]
    fn replace_mode_copies_an_untransformed_bitmap_and_clears_the_rest() {
        let source = image(4, 3, |x, y| [x as u8 * 10 + 1, y as u8 * 10 + 1, 7, 255]);
        let mut destination = image(6, 5, |_, _| [9, 9, 9, 9]);
        let blit = identity(4, 3);
        let spans = rasterize(&project_corners(4, 3, &blit), 5);
        let surface = Surface {
            image: &source,
            format: 2,
        };
        assert!(draw(&mut destination, 2, &surface, spans.as_ref(), &blit));
        for y in 0..5u32 {
            for x in 0..6u32 {
                let offset = ((y * 6 + x) * 4) as usize;
                let expected = if x < 4 && y < 3 {
                    [x as u8 * 10 + 1, y as u8 * 10 + 1, 7, 255]
                } else {
                    [0; 4]
                };
                assert_eq!(
                    &destination.rgba[offset..offset + 4],
                    &expected,
                    "pixel {x},{y}"
                );
            }
        }
    }

    #[test]
    fn bank_of_90_degrees_rotates_around_the_anchor() {
        let source = image(3, 3, |x, y| [x as u8 * 3 + y as u8 + 1, 0, 0, 255]);
        let mut destination = image(8, 8, |_, _| [0; 4]);
        let blit = PerspectiveBlit {
            dest_x: 4 << 16,
            dest_y: 4 << 16,
            anchor_x: 1 << 16,
            anchor_y: 1 << 16,
            bank: 90 << 16,
            ..identity(3, 3)
        };
        let spans = rasterize(&project_corners(3, 3, &blit), 8);
        let surface = Surface {
            image: &source,
            format: 2,
        };
        assert!(draw(&mut destination, 2, &surface, spans.as_ref(), &blit));
        for sy in 0..3u32 {
            for sx in 0..3u32 {
                let (x, y) = (3 + sy, 5 - sx);
                let offset = ((y * 8 + x) * 4) as usize;
                assert_eq!(
                    destination.rgba[offset],
                    sx as u8 * 3 + sy as u8 + 1,
                    "source {sx},{sy}"
                );
            }
        }
    }

    #[test]
    fn degenerate_quad_clears_only_in_replace_mode() {
        let source = image(4, 3, |_, _| [1, 2, 3, 255]);
        let blit = PerspectiveBlit {
            scale_x: 0,
            ..identity(4, 3)
        };
        let corners = project_corners(4, 3, &blit);
        assert!(rasterize(&corners, 5).is_none());
        let surface = Surface {
            image: &source,
            format: 1,
        };
        let mut destination = image(2, 2, |_, _| [5; 4]);
        assert!(!draw(
            &mut destination,
            1,
            &surface,
            None,
            &PerspectiveBlit { mode: 1, ..blit }
        ));
        assert_eq!(destination.rgba, vec![5; 16]);
        assert!(draw(&mut destination, 1, &surface, None, &blit));
        assert_eq!(destination.rgba, vec![0; 16]);
    }

    #[test]
    fn constant_blend_keeps_parameter_share_of_the_destination() {
        let source = image(2, 2, |_, _| [200, 200, 200, 255]);
        let mut destination = image(2, 2, |_, _| [100, 100, 100, 255]);
        let blit = PerspectiveBlit {
            mode: 1,
            parameter: 128,
            ..identity(2, 2)
        };
        let spans = rasterize(&project_corners(2, 2, &blit), 2);
        let surface = Surface {
            image: &source,
            format: 1,
        };
        assert!(draw(&mut destination, 1, &surface, spans.as_ref(), &blit));
        assert_eq!(&destination.rgba[0..4], &[150, 150, 150, 255]);
    }

    #[test]
    fn source_alpha_blend_skips_transparent_texels() {
        let source = image(2, 1, |x, _| {
            if x == 0 {
                [200, 0, 0, 0]
            } else {
                [200, 0, 0, 255]
            }
        });
        let mut destination = image(2, 1, |_, _| [0, 0, 0, 255]);
        let blit = PerspectiveBlit {
            mode: 1,
            ..identity(2, 1)
        };
        let corners = [
            [0.0, 0.0, 1.0, 0.0, 0.0],
            [0.0, 1.0, 1.0, 0.0, 0.0],
            [2.0, 1.0, 1.0, 1.0, 0.0],
            [2.0, 0.0, 1.0, 1.0, 0.0],
        ];
        let spans = rasterize(&corners, 1);
        let surface = Surface {
            image: &source,
            format: 2,
        };
        assert!(draw(&mut destination, 1, &surface, spans.as_ref(), &blit));
        assert_eq!(&destination.rgba[0..4], &[0, 0, 0, 255]);
        assert!(destination.rgba[4] > 0);
    }
}
