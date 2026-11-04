//! Sprite property11 ->428940/42AF80, sixteen ordered419CF0 color filters.
use super::*;
#[derive(Debug, Clone)]
pub(crate) struct FilteredSpriteDraw {
    key: String,
    x: f32,
    y: f32,
    width: u32,
    height: u32,
    input: RuntimeGraphDrawItem,
    revision: u64,
    filters: Vec<(i32, i32)>,
}
/// Mode2 stores rotation separately; mode5 supplies its native local quad.
pub(super) fn item_quad(item: &RuntimeGraphDrawItem) -> Option<[[f32; 2]; 4]> {
    if let Some(quad) = item.destination_quad {
        return Some(quad);
    }
    if item.rotation_degrees == 0. {
        return None;
    }
    let (sin, cos) = item.rotation_degrees.to_radians().sin_cos();
    let cx = item.x + item.width * 0.5;
    let cy = item.y + item.height * 0.5;
    Some(
        [[-0.5, -0.5], [-0.5, 0.5], [0.5, 0.5], [0.5, -0.5]].map(|[x, y]| {
            [
                cx + x * item.width * cos - y * item.height * sin,
                cy + x * item.width * sin + y * item.height * cos,
            ]
        }),
    )
}
impl RuntimeTraceApi {
    pub(super) fn refresh_native_sprite_filters(&mut self) {
        // Build raw draw items without applying last frame's filtered pixels.
        let mut previous = std::mem::take(&mut self.native_sprite_filtered_draws);
        let candidates = self
            .graph_draw_items()
            .into_iter()
            .filter_map(|item| {
                let object = item.owner_object?;
                if self.display_tree.kind(object) != Some(NativeDisplayKind::Sprite) {
                    return None;
                }
                let properties = self.graph_object_properties.get(&object)?;
                let filters = (0..16u32)
                    .filter_map(|i| properties.properties.get(&(0x1100 | i)).copied())
                    .filter(|(packed, _)| (*packed as u32 >> 8) & 255 != 0)
                    .collect::<Vec<_>>();
                (!filters.is_empty()).then_some((object, item, filters))
            })
            .collect::<Vec<_>>();
        for (object, item, filters) in candidates {
            let revision = self.graph_image_revision(&item.key);
            if let Some(old) = previous.remove(&object)
                && old.input == item && old.revision == revision && old.filters == filters
                && self.graph_images.contains_key(&old.key)
            {
                self.native_sprite_filtered_draws.insert(object, old);
                continue;
            }
            let Some(source) = self.graph_images.get(&item.key) else {
                continue;
            };
            let Some((mut image, x, y)) = rasterize_source(source, &item) else {
                continue;
            };
            for &(packed, color) in &filters {
                apply_color_filter(
                    &mut image,
                    ((packed as u32 >> 8) & 255) as u8,
                    color,
                    (packed as u32 >> 16) as u16,
                );
            }
            let width = image.width;
            let height = image.height;
            let key = format!("runtime:sprite-filter:{object}");
            self.store_graph_image(key.clone(), image);
            self.native_sprite_filtered_draws.insert(
                object,
                FilteredSpriteDraw {
                    key,
                    x,
                    y,
                    width,
                    height,
                    input: item,
                    revision,
                    filters,
                },
            );
        }
    }
    pub(super) fn apply_filtered_sprite_draw(&self, item: &mut RuntimeGraphDrawItem) {
        let Some(draw) = item
            .owner_object
            .and_then(|o| self.native_sprite_filtered_draws.get(&o))
        else {
            return;
        };
        item.key = draw.key.clone();
        item.x = draw.x;
        item.y = draw.y;
        item.width = draw.width as f32;
        item.height = draw.height as f32;
        item.src_x = 0.;
        item.src_y = 0.;
        item.src_width = item.width;
        item.src_height = item.height;
        item.ignore_source_alpha = false;
        item.rotation_degrees = 0.;
        item.destination_quad = None;
        item.linear_sampling = false;
    }
}
fn rasterize_source(
    source: &DecodedImage,
    item: &RuntimeGraphDrawItem,
) -> Option<(DecodedImage, f32, f32)> {
    let quad = item_quad(item);
    let (left, top, right, bottom) = if let Some(q) = quad {
        (
            q.iter().map(|p| p[0]).fold(f32::INFINITY, f32::min).floor(),
            q.iter().map(|p| p[1]).fold(f32::INFINITY, f32::min).floor(),
            q.iter()
                .map(|p| p[0])
                .fold(f32::NEG_INFINITY, f32::max)
                .ceil(),
            q.iter()
                .map(|p| p[1])
                .fold(f32::NEG_INFINITY, f32::max)
                .ceil(),
        )
    } else {
        (
            item.x.floor(),
            item.y.floor(),
            (item.x + item.width).ceil(),
            (item.y + item.height).ceil(),
        )
    };
    let width = (right - left) as u32;
    let height = (bottom - top) as u32;
    if width == 0 || height == 0 || u64::from(width) * u64::from(height) > 16 * 1024 * 1024 {
        return None;
    }
    let mut out = crate::native_map_filter::blank(width, height);
    for y in 0..height {
        for x in 0..width {
            let (dx, dy) = (left + x as f32 + 0.5, top + y as f32 + 0.5);
            let (u, v, edge) = if let Some([a, b, _, d]) = quad {
                let (ux, uy, vx, vy) = (d[0] - a[0], d[1] - a[1], b[0] - a[0], b[1] - a[1]);
                let determinant = ux * vy - uy * vx;
                if determinant.abs() <= f32::EPSILON {
                    continue;
                }
                (
                    ((dx - a[0]) * vy - (dy - a[1]) * vx) / determinant,
                    (ux * (dy - a[1]) - uy * (dx - a[0])) / determinant,
                    0.5,
                )
            } else {
                ((dx - item.x) / item.width, (dy - item.y) / item.height, 0.)
            };
            if !(0. ..1.).contains(&u) || !(0. ..1.).contains(&v) {
                continue;
            }
            let sx = item.src_x + u * item.src_width - edge;
            let sy = item.src_y + v * item.src_height - edge;
            let ix = sx.floor() as i32;
            let iy = sy.floor() as i32;
            let mut pixel = crate::native_background_effect::sample_background_pixel(
                source,
                ix,
                iy,
                if quad.is_some() && item.linear_sampling {
                    ((sx - sx.floor()) * 16.).floor() as i32
                } else {
                    0
                },
                if quad.is_some() && item.linear_sampling {
                    ((sy - sy.floor()) * 16.).floor() as i32
                } else {
                    0
                },
                16,
            );
            if item.ignore_source_alpha
                && ix >= 0
                && iy >= 0
                && ix < source.width as i32
                && iy < source.height as i32
            {
                pixel[3] = 255;
            }
            let offset = ((y * width + x) * 4) as usize;
            out.rgba[offset..offset + 4].copy_from_slice(&pixel);
        }
    }
    Some((out, left, top))
}
/// SSE2 integer channel equations from410D80/F80,4111E0/3B0,4116F0,
/// 4118E0 and411A00. Preserve signed16 multiplication and quantization.
fn apply_color_filter(image: &mut DecodedImage, kind: u8, color: i32, strength: u16) {
    let color = [
        (color as u32 >> 16) as u8,
        (color as u32 >> 8) as u8,
        color as u8,
    ];
    for pixel in image.rgba.as_chunks_mut::<4>().0 {
        let luma = 77 * u32::from(pixel[0]) + 150 * u32::from(pixel[1]) + 29 * u32::from(pixel[2]);
        for lane in 0..3 {
            let d = i16::from(pixel[lane]);
            let c = u16::from(color[lane]);
            let value = match kind {
                1 => {
                    let target = d ^ c as i16;
                    let weight = (strength as i16) >> 1;
                    d.wrapping_add(target.wrapping_sub(d).wrapping_mul(weight) >> 7) as i32
                }
                2 => {
                    let target = ((luma * u32::from(c)) >> 16) as i16;
                    let weight = (strength >> 1) as i16;
                    d.wrapping_add(target.wrapping_sub(d).wrapping_mul(weight) >> 7) as i32
                }
                3 => {
                    let base = (d as u16).wrapping_mul(256u16.wrapping_sub(strength));
                    i32::from(base.saturating_add(c.wrapping_mul(strength)) >> 8)
                }
                4 => i32::from(d) + i32::from(c.wrapping_mul(strength) >> 8),
                5 => i32::from(d) - i32::from(c.wrapping_mul(strength) >> 8),
                _ => i32::from(d),
            };
            pixel[lane] = value.clamp(0, 255) as u8;
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{GraphApi, NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn property11_decodes_three_fields_and_runs_ordered_post_raster_filters() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 2;
        api.screen_height = 1;
        api.store_runtime_bitmap(
            100,
            DecodedImage {
                width: 2,
                height: 1,
                rgba: vec![10, 20, 30, 255, 10, 20, 30, 255],
            },
            2,
        );
        let call = |api: &mut RuntimeTraceApi, id, args: &[i32]| {
            GraphApi::call_graph(
                api,
                &mut NativeCallFrame::new(
                    NativeOpcode { group: 0x90, id },
                    args.iter().copied().map(Value::Int).collect(),
                ),
            )
            .unwrap()
        };
        let object = call(&mut api, 0x50, &[]).as_i32();
        call(&mut api, 0x56, &[object, 0, 0, 100, 0, 0, 0]);
        call(&mut api,0x54,&[object,1]);
        call(
            &mut api,
            0x38,
            &[object, 0x11, (256 << 16) | (1 << 8), 0xff0000],
        );
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[245, 20, 30, 255]
        );
        call(
            &mut api,
            0x38,
            &[object, 0x11, (128 << 16) | (4 << 8) | 1, 100],
        );
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[245, 20, 80, 255]
        );
        call(
            &mut api,
            0x38,
            &[object, 0x11, (256 << 16) | (3 << 8) | 2, 0xff0000],
        );
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[255, 0, 0, 255]
        );
        let key = format!("runtime:sprite-filter:{object}");
        let revision = api.graph_image_revision(&key);
        api.refresh_native_sprite_filters();
        assert_eq!(api.graph_image_revision(&key), revision);
        // Changing the source while keeping its bitmap identity invalidates
        // the cached sprite pixels.
        let source_key = api.graph_resources[&100].key.clone();
        let original = api.graph_images[&source_key].clone();
        let mut changed = original.clone();
        changed.rgba[0] = 77;
        api.store_graph_image(source_key.clone(), changed);
        api.refresh_native_sprite_filters();
        assert_ne!(api.graph_image_revision(&key), revision);
        api.store_graph_image(source_key, original);
        api.refresh_native_sprite_filters();
        call(&mut api, 0x38, &[object, 0x11, 2, 0]);
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[245, 20, 80, 255]
        );
    }
}
