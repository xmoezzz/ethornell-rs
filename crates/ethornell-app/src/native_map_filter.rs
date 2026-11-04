//! CDspObjMap (423C50..4246C0) and CDspObjFilter (420D40..421100).
//! The map owns copied u16 data and a tile cache. Filters consume the display
//! chain prefix at their own sort position, rather than sampling a bitmap.
use super::*;
use ethornell_vm::GraphApi;

#[derive(Debug, Clone, Default)]
pub(crate) struct NativeMap {
    grid: Option<[u32; 4]>,
    source: Option<([u32; 2], Vec<u16>)>,
    atlas: Option<(i32, u64)>,
    viewport: Option<[u32; 4]>,
    current: Vec<u16>,
    desired: Vec<u16>,
    cache: Option<DecodedImage>,
    dirty: bool,
}
#[derive(Debug, Clone, PartialEq)]
pub(crate) struct NativeFilter {
    mode: i32,
    color: i32,
    shift: i32,
    mask: Option<(i32, u64)>,
}
#[derive(PartialEq)]
pub(crate) struct FilterCacheSignature {
    size: (u32, u32),
    items: Vec<RuntimeGraphDrawItem>,
    revisions: Vec<(String, u64)>,
    filters: Vec<(i32, NativeFilter, i32, Option<(String, u64, Option<RuntimeClipRect>)>)>,
}

pub(super) fn blank(w: u32, h: u32) -> DecodedImage {
    DecodedImage {
        width: w,
        height: h,
        rgba: vec![0; w as usize * h as usize * 4],
    }
}

impl RuntimeTraceApi {
    pub(super) fn configure_native_map(
        &mut self,
        id: u16,
        a: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let err = |message: String| ethornell_vm::VmError::Runtime(message);
        let object = a[0];
        // sub_47E210: priority, transparency, blend mode, bitmap checks
        // come before the map lookup.
        if id == 0x75 {
            target_checks::priority(a[6])?;
            target_checks::effect_level(a[5])?;
            target_checks::effect_mode(a[4])?;
            target_checks::bitmap(a[3])?;
        }
        if self.display_tree.kind(object) != Some(NativeDisplayKind::Map) {
            return Err(err("無効なマップハンドルが指定されました".into()));
        }
        let unfit = || err(format!("ビットマップ [ {} ] は指定されたマップオブジェクトに適合しません", a[3]));
        match id {
            0x75 => {
                let info = self.query_bitmap_info(a[3]).ok_or_else(unfit)?;
                let generation = self.graph_resources.get(&a[3]).ok_or_else(unfit)?.generation;
                let map = self.native_maps.entry(object).or_default();
                let Some([_, _, cw, ch]) = map.grid else {
                    return Err(unfit());
                };
                if info.width / cw == 0 || info.height / ch == 0 {
                    return Err(unfit());
                }
                // 409200 allocates a new private bitmap with the atlas format.
                let [cols, rows, _, _] = map.grid.unwrap();
                map.cache = Some(blank((cols + 1) * cw, (rows + 1) * ch));
                map.atlas = Some((a[3], generation));
                map.dirty = true;
                self.graph_object_properties
                    .entry(object)
                    .or_default()
                    .format_resource = Some(a[3]);
                self.graph90_set_common_state(
                    object,
                    Some((a[1], a[2])),
                    Some(a[4]),
                    Some(a[5]),
                    Some(a[6]),
                );
                self.set_graph_object_draw_enabled_raw(object, true);
            }
            0x76 => {
                let (cols, rows, cw, ch) = (a[1] as u32, a[2] as u32, a[3] as u32, a[4] as u32);
                if cw.wrapping_sub(1) > 255 || ch.wrapping_sub(1) > 255 {
                    return Err(err(format!("無効なチップサイズ [ {} , {} ] が指定されました", a[3], a[4])));
                }
                if cols == 0 || rows == 0 || cols > 1024 / cw || rows > 768 / ch {
                    return Err(err(format!("無効なビューサイズ [ {} , {} ] が指定されました", a[1], a[2])));
                }
                let map = self.native_maps.entry(object).or_default();
                map.grid = Some([cols, rows, cw, ch]);
                map.atlas = None;
                map.viewport = None;
                map.current = vec![u16::MAX; ((cols + 1) * (rows + 1)) as usize];
                map.desired = vec![0; map.current.len()];
                map.cache = None;
                map.dirty = false;
                self.graph_layers.remove(&object);
            }
            0x79 => {
                let map = self.native_maps.entry(object).or_default();
                let invalid = || {
                    err(format!(
                        "無効なマップ表示座標が指定されました\n\nマップ座標 [ {} , {} ] , チップ座標 [ {} , {} ]",
                        a[1], a[2], a[3], a[4]
                    ))
                };
                let Some([cols, rows, cw, ch]) = map.grid else {
                    return Err(invalid());
                };
                let Some(([sw, sh], data)) = &map.source else {
                    return Err(invalid());
                };
                let [sx, sy, ox, oy] = [a[1] as u32, a[2] as u32, a[3] as u32, a[4] as u32];
                if sx >= *sw || sy >= *sh || ox >= cw || oy >= ch {
                    return Err(invalid());
                }
                // Wrap is consulted only when the source cell changes. Changing
                // it alone preserves the old cached secondary grid (4240A0).
                if map.viewport.is_none_or(|v| v[0] != sx || v[1] != sy) {
                    if a[5] == 0 {
                        map.desired.fill(u16::MAX);
                    }
                    for y in 0..=rows {
                        let mut yy = sy + y;
                        if yy >= *sh {
                            if a[5] == 0 {
                                break;
                            }
                            yy %= *sh;
                        }
                        for x in 0..=cols {
                            let mut xx = sx + x;
                            if xx >= *sw {
                                if a[5] == 0 {
                                    break;
                                }
                                xx %= *sw;
                            }
                            map.desired[(y * (cols + 1) + x) as usize] =
                                data[(yy * sw + xx) as usize];
                        }
                    }
                    map.dirty = true;
                }
                map.viewport = Some([sx, sy, ox, oy]);
            }
            0x7a => {
                let map = self.native_maps.entry(object).or_default();
                if map.grid.is_none() {
                    return Err(err("指定されたオブジェクトは初期化が完了していません".into()));
                }
                // 424278..424288 advances ESI twice on a match. The
                // native overread past the allocation is defined as zero here.
                let mut cursor = 0;
                for current in &mut map.current {
                    if i32::from(*current) == a[1] {
                        *current = !map.desired.get(cursor).copied().unwrap_or(0);
                        cursor += 1;
                    }
                    cursor += 1;
                }
                map.dirty = true;
            }
            _ => unreachable!(),
        }
        self.refresh_native_map(object);
        Ok(())
    }
    pub(super) fn upload_native_map(
        &mut self,
        object: i32,
        width: u32,
        height: u32,
        tiles: &[u16],
    ) -> bool {
        if self.display_tree.kind(object) != Some(NativeDisplayKind::Map)
            || width == 0
            || height == 0
            || width
                .checked_mul(height)
                .is_none_or(|n| n as usize != tiles.len())
        {
            return false;
        }
        let map = self.native_maps.entry(object).or_default();
        map.source = Some(([width, height], tiles.to_vec()));
        map.viewport = None;
        for (current, desired) in map.current.iter_mut().zip(&map.desired) {
            *current = !*desired;
        }
        self.graph_layers.remove(&object);
        true
    }
    pub(super) fn refresh_native_map(&mut self, object: i32) {
        let Some(mut map) = self.native_maps.remove(&object) else {
            return;
        };
        let result = (|| {
            let [cols, rows, cw, ch] = map.grid?;
            let [_, _, ox, oy] = map.viewport?;
            let (atlas, generation) = map.atlas?;
            let current = self
                .graph_resources
                .get(&atlas)
                .is_some_and(|r| r.generation == generation);
            if map.dirty {
                if current {
                    let atlas_image = self.graph_bitmap_image(atlas)?;
                    let across = atlas_image.width / cw;
                    let count = across * (atlas_image.height / ch);
                    let cache = map.cache.as_mut()?;
                    for y in 0..=rows {
                        for x in 0..=cols {
                            let index = (y * (cols + 1) + x) as usize;
                            if map.current[index] == map.desired[index] {
                                continue;
                            }
                            let tile = u32::from(map.desired[index]);
                            for dy in 0..ch {
                                for dx in 0..cw {
                                    let out =
                                        (((y * ch + dy) * cache.width + x * cw + dx) * 4) as usize;
                                    if tile == 65535 || tile >= count {
                                        cache.rgba[out..out + 4].fill(0);
                                    } else {
                                        let source = ((((tile / across) * ch + dy)
                                            * atlas_image.width
                                            + (tile % across) * cw
                                            + dx)
                                            * 4)
                                            as usize;
                                        cache.rgba[out..out + 4]
                                            .copy_from_slice(&atlas_image.rgba[source..source + 4]);
                                    }
                                }
                            }
                            map.current[index] = map.desired[index];
                        }
                    }
                } else if let Some(cache) = map.cache.as_mut() {
                    cache.rgba.fill(0);
                }
                map.dirty = false;
            }
            let cache = map.cache.as_ref()?;
            let mut image = blank(cols * cw, rows * ch);
            for y in 0..image.height {
                let source = (((y + oy) * cache.width + ox) * 4) as usize;
                let dest = (y * image.width * 4) as usize;
                let bytes = (image.width * 4) as usize;
                image.rgba[dest..dest + bytes].copy_from_slice(&cache.rgba[source..source + bytes]);
            }
            let key = format!("runtime:map:{object}");
            self.store_graph_image(key.clone(), image);
            self.install_native_map_filter_layer(object, key, cols * cw, rows * ch);
            Some(())
        })();
        if result.is_none() {
            self.graph_layers.remove(&object);
        }
        self.native_maps.insert(object, map);
    }
    pub(super) fn install_native_map_filter_layer(
        &mut self,
        object: i32,
        key: String,
        w: u32,
        h: u32,
    ) {
        let (x, y, z) = self
            .graph_layers
            .get(&object)
            .map(|l| (l.x, l.y, l.z))
            .unwrap_or_else(|| {
                let p = self.graph_object_properties.get(&object);
                p.map_or((0., 0., 0), |p| {
                    (
                        p.native.position_x as f32,
                        p.native.position_y as f32,
                        p.native.priority as i32,
                    )
                })
            });
        self.graph_layers.insert(
            object,
            RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key,
                target_surface: None,
                x,
                y,
                width: w as f32,
                height: h as f32,
                src_x: 0.,
                src_y: 0.,
                opacity: 1.,
                z,
                enabled: true,
                transform_x: 0.,
                transform_y: 0.,
                transform_z: 0,
                scale_x: 1.,
                scale_y: 1.,
                rotation_degrees: 0.,
                clip: None,
            },
        );
    }
    pub(super) fn configure_native_filter(
        &mut self,
        id: u16,
        a: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        // sub_47DE90 / sub_47DF00: priority and transparency checks, then
        // sub_462AE0: unknown filter (255), mask missing (1), not grey-scale
        // (2), not screen-sized (3).
        let err = |message: String| ethornell_vm::VmError::Runtime(message);
        let object = a[0];
        let (mode, color, mask, shift, alpha, priority) = if id == 0x65 {
            (0, a[1], -1, 0, a[2], a[3])
        } else {
            (a[1], a[2], a[3], a[4], a[5], a[6])
        };
        target_checks::priority(priority)?;
        target_checks::effect_level(alpha)?;
        if self.display_tree.kind(object) != Some(NativeDisplayKind::Filter) {
            return Err(err("無効なフィルターハンドルが指定されました".into()));
        }
        let (w, h) = snapshot::runtime_frame_size(self);
        let missing = || err(format!("無効なビットマップ番号 [ {mask} ] が指定されました"));
        let mask = if mask == -1 {
            None
        } else {
            let info = self.query_bitmap_info(mask).ok_or_else(missing)?;
            if info.format != 3 {
                return Err(err(format!("指定されたビットマップ [ {mask} ] はグレイスケールではありません")));
            }
            if info.width != w || info.height != h {
                return Err(err(format!("指定されたビットマップ [ {mask} ] はスクリーンサイズではありません")));
            }
            Some((mask, self.graph_resources.get(&mask).ok_or_else(missing)?.generation))
        };
        self.graph90_set_common_state(object, None, None, Some(alpha), Some(priority));
        self.native_filters.insert(
            object,
            NativeFilter {
                mode,
                color,
                shift,
                mask,
            },
        );
        let key = format!("runtime:filter:{object}");
        self.store_graph_image(key.clone(), blank(w, h));
        self.install_native_map_filter_layer(object, key, w, h);
        self.set_graph_object_draw_enabled_raw(object, true);
        self.refresh_native_filters();
        Ok(())
    }
    /// Rebuild each filter from precisely the prefix preceding it. Later
    /// filters see earlier filters' results, never their own previous raster.
    pub(super) fn refresh_native_filters(&mut self) {
        if self.native_filters.is_empty()
            && self.native_effector_rasters.is_empty()
            && self.native_landscapes.is_empty()
        {
            return;
        }
        let mut items = self.graph_output_draw_items();
        // Only the prefix up to the last visible destination-reading effect
        // contributes to any result. Registered but hidden filters need no
        // CPU frame, and ordinary sprites after the last filter are GPU work.
        let Some(last_effect) = items.iter().rposition(|item| {
            item.owner_object.is_some_and(|object| {
                self.native_filters.contains_key(&object)
                    || self.native_effector_rasters.contains_key(&object)
            }) || item.key.starts_with("runtime:landscape:")
        }) else { return; };
        items.truncate(last_effect + 1);
        let (w, h) = snapshot::runtime_frame_size(self);
        // Feedback effectors and landscapes must keep ticking even with
        // unchanged inputs. Cache only ordinary filters, whose output is a
        // pure function of this display prefix and its resource revisions.
        let cacheable = items.iter().all(|item| {
            !item.key.starts_with("runtime:landscape:")
                && !item.owner_object.is_some_and(|o| self.native_effector_rasters.contains_key(&o))
        });
        let mut signature = cacheable.then(|| {
            let mut revisions = items.iter().map(|item| {
                (item.key.clone(), self.graph_image_revision(&item.key))
            }).collect::<Vec<_>>();
            let filters = items.iter().filter_map(|item| {
                let object = item.owner_object?;
                let state = self.native_filters.get(&object)?.clone();
                let parameter = self.graph_object_properties.get(&object)
                    .map_or(0, |p| p.transparency_parameter());
                let mask = state.mask.and_then(|(bitmap, _)| self.graph_resources.get(&bitmap))
                    .map(|resource| (resource.key.clone(), resource.generation, resource.source_rect));
                if let Some((key, _, _)) = &mask {
                    revisions.push((key.clone(), self.graph_image_revision(key)));
                }
                Some((object, state, parameter, mask))
            }).collect();
            FilterCacheSignature { size: (w, h), items: items.clone(), revisions, filters }
        });
        if signature.is_some() && self.native_filter_cache == signature {
            return;
        }
        self.native_filter_cache = None;
        let mut frame = blank(w, h);
        for px in frame.rgba.as_chunks_mut::<4>().0 {
            px[3] = 255;
        }
        for item in items {
            if let Some(object) = item
                .owner_object
                .filter(|o| self.native_filters.contains_key(o))
            {
                let state = self.native_filters[&object].clone();
                let mut image = blank(item.src_width as u32, item.src_height as u32);
                let mask = state.mask.and_then(|(bitmap, generation)| {
                    self.graph_resources
                        .get(&bitmap)
                        .filter(|r| r.generation == generation)?;
                    self.graph_bitmap_image(bitmap)
                });
                let parameter = self
                    .graph_object_properties
                    .get(&object)
                    .map_or(0, |p| p.transparency_parameter());
                for y in 0..image.height {
                    for x in 0..image.width {
                        let dx = item.x as i64 + x as i64;
                        let dy = item.y as i64 + y as i64;
                        if dx < 0 || dy < 0 || dx >= w as i64 || dy >= h as i64 {
                            continue;
                        }
                        let from = ((dy * w as i64 + dx) * 4) as usize;
                        let to = ((y * image.width + x) * 4) as usize;
                        let pixel = &mut image.rgba[to..to + 4];
                        pixel.copy_from_slice(&frame.rgba[from..from + 4]);
                        if state.mask.is_some() && mask.is_none() {
                            continue;
                        }
                        let mask_value = mask
                            .as_ref()
                            .and_then(|m| m.rgba.get(((y * m.width + x) * 4) as usize))
                            .copied();
                        filter_pixel(
                            pixel,
                            state.mode,
                            state.color,
                            parameter,
                            state.shift,
                            mask_value,
                        );
                    }
                }
                snapshot::composite_runtime_item(&mut frame, &image, &item);
                self.store_graph_image(item.key.clone(), image);
            } else if let Some(object) = item
                .owner_object
                .filter(|o| self.native_effector_rasters.contains_key(o))
            {
                if let Some(image) = self.render_native_effector(object, &frame) {
                    snapshot::composite_runtime_item(&mut frame, &image, &item);
                    self.store_graph_image(item.key.clone(), image);
                }
            } else if item.key.starts_with("runtime:landscape:") {
                if let (Some(object), Some(depth)) = (
                    item.owner_object,
                    item.key
                        .rsplit(':')
                        .next()
                        .and_then(|s| s.parse::<u32>().ok()),
                ) {
                    if let Some(image) = self.render_native_landscape_stage(object, depth, &frame) {
                        snapshot::composite_runtime_item(&mut frame, &image, &item);
                        self.store_graph_image(item.key.clone(), image);
                    }
                }
            } else if let Some(image) = self.graph_images.get(&item.key) {
                snapshot::composite_runtime_item(&mut frame, image, &item);
            }
        }
        if let Some(signature) = signature.as_mut() {
            // These include generated filter outputs. Record their revisions
            // after producing them so our own writes do not invalidate reuse.
            for (key, revision) in &mut signature.revisions {
                *revision = self.graph_image_revision(key);
            }
        }
        self.native_filter_cache = signature;
    }
}
/// Scalar equivalents of 4116F0/4118E0/411A00/4111E0/410D80/411B50.
/// A mask overrides the filter mode and performs only a color blend.
fn filter_pixel(pixel: &mut [u8], mode: i32, color: i32, p: i32, shift: i32, mask: Option<u8>) {
    let color = [
        ((color as u32 >> 16) & 255) as i32,
        ((color as u32 >> 8) & 255) as i32,
        (color as u32 & 255) as i32,
    ];
    if let Some(mask) = mask {
        let shift = (shift as u32) & 31;
        let keep = 256i32
            .wrapping_sub(p.wrapping_mul(1i32.wrapping_shl(shift).wrapping_add(1)))
            .wrapping_add(i32::from(mask).wrapping_shl(shift));
        if keep >= 256 {
            return;
        }
        for c in 0..3 {
            pixel[c] = if keep <= 0 {
                color[c] as u8
            } else {
                (color[c] + ((i32::from(pixel[c]) - color[c]) * (keep >> 1) >> 7)).clamp(0, 255)
                    as u8
            };
        }
        return;
    }
    let luminance = 77 * i32::from(pixel[0]) + 150 * i32::from(pixel[1]) + 29 * i32::from(pixel[2]);
    for c in 0..3 {
        let old = i32::from(pixel[c]);
        let value = match mode {
            0 => (old * (256 - p) + color[c] * p) >> 8,
            1 => {
                if color == [0, 0, 0] {
                    old - ((255 * p) >> 8)
                } else {
                    old + ((color[c] * p) >> 8)
                }
            }
            2 => old + (((luminance * color[c] >> 16) - old) * (p >> 1) >> 7),
            3 => old + (((old ^ color[c]) - old) * (p >> 1) >> 7),
            _ => old,
        };
        pixel[c] = value.clamp(0, 255) as u8;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn map_scroll_cache_invalidation_and_filter_chain_execute_pixels() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 4;
        api.screen_height = 2;
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
        // Two 2x2 atlas cells. The map owns an independent copy of its IDs.
        api.store_runtime_bitmap(
            100,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [vec![200, 0, 0, 0].repeat(2), vec![0, 100, 0, 0].repeat(2)]
                    .concat()
                    .repeat(2),
            },
            1,
        );
        let map = call(&mut api, 0x70, &[]).as_i32();
        call(&mut api, 0x76, &[map, 2, 1, 2, 2]);
        assert!(GraphApi::upload_map_tile_data(&mut api, map, 2, 1, &[0, 1]));
        call(&mut api, 0x75, &[map, 0, 0, 100, 0x80, 0, 0]);
        call(&mut api, 0x79, &[map, 0, 0, 1, 0, 1]);
        let frame = snapshot::compose_runtime_frame(&api, false);
        assert_eq!(
            &frame.rgba[..16],
            &[
                200, 0, 0, 255, 0, 100, 0, 255, 0, 100, 0, 255, 200, 0, 0, 255
            ]
        );
        // Source pixels change without re-rendering an already-cached tile.
        api.store_graph_image(
            api.graph_resources[&100].key.clone(),
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [vec![0, 0, 240, 0].repeat(2), vec![0, 100, 0, 0].repeat(2)]
                    .concat()
                    .repeat(2),
            },
        );
        api.refresh_native_map(map);
        assert_eq!(api.graph_images[&format!("runtime:map:{map}")].rgba[0], 200);
        call(&mut api, 0x7a, &[map, 0]);
        assert_eq!(api.graph_images[&format!("runtime:map:{map}")].rgba[2], 240);
        let filter = call(&mut api, 0x60, &[]).as_i32();
        call(&mut api, 0x65, &[filter, 0xff0000, 128, 1]);
        let frame = snapshot::compose_runtime_frame(&api, false);
        assert_eq!(&frame.rgba[..4], &[127, 0, 120, 255]);
        // A subsequent full inverse filter sees the first one's result.
        let inverse = call(&mut api, 0x60, &[]).as_i32();
        call(&mut api, 0x66, &[inverse, 3, 0xffffff, -1, 0, 256, 2]);
        let frame = snapshot::compose_runtime_frame(&api, false);
        assert_eq!(&frame.rgba[..4], &[128, 255, 135, 255]);
        // Mask mode ignores filter selector and blends using native 7-bit keep.
        api.store_runtime_bitmap(
            101,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [0, 0, 0, 255].repeat(8),
            },
            3,
        );
        call(&mut api, 0x66, &[inverse, 3, 0x0000ff, 101, 0, 256, 2]);
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[0, 0, 255, 255]
        );
    }

    #[test]
    fn a_released_filter_leaves_the_display_list() {
        // senmomo's scene start fades in through filter 0x90000002 and
        // releases it; its last raster (black) must not stay on screen.
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 4;
        api.screen_height = 2;
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
        let filter = call(&mut api, 0x60, &[]).as_i32();
        call(&mut api, 0x66, &[filter, 0, 0, -1, 0, 0, 1216]);
        call(&mut api, 0x64, &[filter, 1]);
        assert!(api.graph_output_draw_items().iter().any(|item| item.owner_object == Some(filter)));
        call(&mut api, 0x61, &[filter]);
        assert!(!api.graph_layers.contains_key(&filter));
        assert!(api.graph_output_draw_items().iter().all(|item| item.owner_object != Some(filter)));
    }
}
