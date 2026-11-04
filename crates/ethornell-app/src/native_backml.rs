//! CDspObjBackML::Draw41E4B0, eight private layers composed as one Back.
use super::*;
impl RuntimeTraceApi {
    pub(super) fn validate_native_backml_call(
        &self,
        id: u16,
        a: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let fail = |why: &str| ethornell_vm::VmError::Runtime(format!("91:{id:02X}: {why}"));
        if id != 0x40
            && self.graph90_background_class(self.current_graph_object.unwrap_or(0))
                != Some(NativeBackgroundClass::BackMl)
        {
            return Err(fail("current background is not BackML"));
        }
        if id != 0x40 && a[0] as u32 >= 8 {
            return Err(fail("layer index exceeds seven"));
        }
        if (id == 0x40 && (a[6] == 0 || a[7] == 0)) || (id == 0x47 && (a[2] == 0 || a[3] == 0)) {
            return Err(fail("scale is zero"));
        }
        if id == 0x45 && a[1] as u32 > 256 {
            return Err(fail("blend parameter exceeds256"));
        }
        if id == 0x44 && !matches!(a[1],0..=9|0x20..=0x27|0x40|0x41|0x80|0xc0|0xc1|0xf0|0xff) {
            return Err(fail("invalid blend selector"));
        }
        if matches!(id, 0x40 | 0x46) {
            let bitmap = a[if id == 0x40 { 2 } else { 1 }];
            if bitmap as u32 >= 0x4000 || !self.graph_resources.contains_key(&bitmap) {
                return Err(fail("bitmap is missing or outside registry"));
            }
        }
        Ok(())
    }

    pub(super) fn refresh_native_backml(&mut self) {
        let object = self.current_graph_object.unwrap_or(0);
        if self.graph90_background_class(object) != Some(NativeBackgroundClass::BackMl)
            || !self.graph91_multilayer_background.active
        {
            return;
        }
        let (w, h) = snapshot::runtime_frame_size(self);
        let mut out = crate::native_map_filter::blank(w, h);
        let df = self
            .bitmap_formats
            .get(&NATIVE_SCREEN_BITMAP)
            .copied()
            .unwrap_or(1);
        let progress = self
            .graph_object_properties
            .get(&object)
            .map_or(0, |p| p.native.fixed_parameter_16_16);
        for (index, record) in self
            .graph91_multilayer_background
            .layers
            .into_iter()
            .enumerate()
        {
            if !record.configured || !record.enabled {
                continue;
            }
            let Some(resource) = self
                .graph_resources
                .get(&record.bitmap)
                .filter(|r| r.generation == record.bitmap_generation)
            else {
                continue;
            };
            let _ = resource;
            let Some(source) = self.graph_bitmap_image(record.bitmap) else {
                continue;
            };
            let sf = self
                .bitmap_formats
                .get(&record.bitmap)
                .copied()
                .unwrap_or(2);
            if !matches!(sf, 1 | 2) {
                continue;
            }
            let advance =
                |delta: i32| ((i64::from(delta) * i64::from(progress as u32)) >> 24) as i32;
            let source_x = record.source_x.wrapping_add(advance(record.source_delta_x));
            let source_y = record.source_y.wrapping_add(advance(record.source_delta_y));
            let angle_curve = crate::animation::target_curve_fixed_16(record.auxiliary_a, progress);
            let angle = record.rotation.wrapping_add(
                ((i64::from(record.rotation_delta) * i64::from(angle_curve)) >> 16) as i32,
            );
            let scale_curve = crate::animation::target_curve_fixed_16(record.auxiliary_b, progress)
                as f64
                / 65536.;
            // Reciprocal interpolation, not linear interpolation of scale.
            let scale = |base: i32, delta: i32| {
                let start = 65536. / base as f64;
                let end = 65536. / base.wrapping_add(delta) as f64;
                (65536. / (start + (end - start) * scale_curve)) as i64 as i32
            };
            let sx = scale(record.scale_x, record.scale_delta_x);
            let sy = scale(record.scale_y, record.scale_delta_y);
            if sx == 0 || sy == 0 {
                continue;
            }
            let image = crate::native_effector::native_inverse_affine(
                &source,
                [w, h],
                [record.x_16_16, record.y_16_16],
                [source_x, source_y, angle, sx, sy],
                record.transparency != 0,
            );
            if index == 0 {
                // 4179B0 is a replacing transform. RGB lanes fade toward
                // black; the fourth source lane remains native payload.
                let tx = i64::from(record.x_16_16) - i64::from(source_x);
                let ty = i64::from(record.y_16_16) - i64::from(source_y);
                let native_fast = source.width >= w
                    && source.height >= h
                    && tx & 65535 == 0
                    && ty & 65535 == 0
                    && tx <= 0
                    && ty <= 0
                    && tx / 65536 + i64::from(source.width) >= i64::from(w)
                    && ty / 65536 + i64::from(source.height) >= i64::from(h)
                    && angle == 0
                    && sx == 65536
                    && sy == 65536;
                if native_fast {
                    crate::bitmap_blend::blit_mode(
                        &mut out,
                        df,
                        &image,
                        sf,
                        0,
                        0,
                        5,
                        record.blend_parameter,
                    );
                } else if sf == df {
                    if record.blend_parameter as u32 >= 256 {
                        out.rgba.fill(0);
                    } else {
                        out = image;
                        for pixel in out.rgba.as_chunks_mut::<4>().0 {
                            for lane in 0..3 {
                                pixel[lane] = ((i32::from(pixel[lane])
                                    * (256 - record.blend_parameter))
                                    >> 8) as u8;
                            }
                        }
                    }
                } else if sf == 1 && df == 2 {
                    out = image;
                    for p in out.rgba.as_chunks_mut::<4>().0 {
                        p[3] = 255;
                    }
                }
            } else if matches!(record.blend_mode, 0 | 1 | 32) {
                if df != 1 {
                    continue;
                }
                if sf == 1 {
                    let p = record.blend_parameter;
                    if p as u32 >= 256 {
                        continue;
                    }
                    let fast = angle % 23592960 == 0
                        && sx == 65536
                        && sy == 65536
                        && (record.x_16_16 | record.y_16_16 | source_x | source_y) & 65535 == 0;
                    for y in 0..h {
                        for x in 0..w {
                            if fast {
                                let xx =
                                    x as i64 + (source_x as i64 - record.x_16_16 as i64) / 65536;
                                let yy =
                                    y as i64 + (source_y as i64 - record.y_16_16 as i64) / 65536;
                                if xx < 0
                                    || yy < 0
                                    || xx >= source.width as i64
                                    || yy >= source.height as i64
                                {
                                    continue;
                                }
                            }
                            let offset = ((y * w + x) * 4) as usize;
                            for c in 0..4 {
                                out.rgba[offset + c] = (i32::from(image.rgba[offset + c])
                                    + ((i32::from(out.rgba[offset + c])
                                        - i32::from(image.rgba[offset + c]))
                                        * (p >> 1)
                                        >> 7))
                                    .clamp(0, 255)
                                    as u8;
                            }
                        }
                    }
                } else {
                    crate::bitmap_blend::blit_mode(
                        &mut out,
                        df,
                        &image,
                        sf,
                        0,
                        0,
                        1,
                        record.blend_parameter,
                    );
                }
            } else {
                crate::bitmap_blend::blit_mode(
                    &mut out,
                    df,
                    &image,
                    sf,
                    0,
                    0,
                    record.blend_mode,
                    record.blend_parameter,
                );
            }
        }
        if df == 1 {
            for pixel in out.rgba.as_chunks_mut::<4>().0 {
                pixel[3] = 255;
            }
        }
        let key = format!("runtime:backml:{object}");
        self.store_graph_image(key.clone(), out);
        self.install_native_map_filter_layer(object, key, w, h);
        // Layer destinations are private transform anchors. The Back itself
        // stays at its full-screen manager location, not the selected anchor.
        if let Some(layer) = self.graph_layers.get_mut(&object) {
            layer.x = 0.;
            layer.y = 0.;
        }
    }
    pub(super) fn set_native_backml_property(
        &mut self,
        object: i32,
        property: u32,
        value: i32,
        extra: i32,
    ) -> Option<std::result::Result<(), i32>> {
        if self.graph90_background_class(object) != Some(NativeBackgroundClass::BackMl) {
            return None;
        }
        let index = self.graph91_multilayer_background.selected_layer;
        let record = &mut self.graph91_multilayer_background.layers[index];
        match property {
            0x41 => {
                if record.scale_x == 0 || record.scale_y == 0 {
                    return Some(Err(254));
                }
                record.rotation = value;
                record.rotation_delta = 0;
                record.scale_delta_x = 0;
                record.scale_delta_y = 0;
            }
            0x80 => {
                record.source_delta_x = value;
                record.source_delta_y = extra;
            }
            0x81 => record.rotation_delta = value,
            0x82 => {
                record.scale_delta_x = value;
                record.scale_delta_y = extra;
            }
            0x8f => {
                record.auxiliary_a = value;
                record.auxiliary_b = extra;
            }
            _ => return None,
        }
        self.refresh_native_backml();
        Some(Ok(()))
    }
    pub(super) fn set_native_backml_alpha(&mut self, object: i32, value: i32) -> bool {
        if self.graph90_background_class(object) != Some(NativeBackgroundClass::BackMl) {
            return false;
        }
        let index = self.graph91_multilayer_background.selected_layer;
        self.graph91_multilayer_background.layers[index].blend_parameter = value;
        self.refresh_native_backml();
        true
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{GraphApi, NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn backml_fixed_source_motion_selected_alpha_and_private_blend_execute() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 4;
        api.screen_height = 2;
        let call = |api: &mut RuntimeTraceApi, g, id, args: &[i32]| {
            GraphApi::call_graph(
                api,
                &mut NativeCallFrame::new(
                    NativeOpcode { group: g, id },
                    args.iter().copied().map(Value::Int).collect(),
                ),
            )
            .unwrap()
        };
        api.store_runtime_bitmap(
            100,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: vec![10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0, 40, 0, 0, 0].repeat(2),
            },
            1,
        );
        api.store_runtime_bitmap(
            101,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [0, 0, 100, 0].repeat(8),
            },
            1,
        );
        call(&mut api, 0x90, 0x4c, &[1, 1]);
        call(&mut api, 0x91, 0x40, &[0, 0, 100, 0, 0, 0, 65536, 65536, 0]);
        let image = |api: &RuntimeTraceApi| {
            api.graph_images[&format!("runtime:backml:{}", api.current_graph_object.unwrap_or(0))]
                .clone()
        };
        assert_eq!(image(&api).rgba[0], 10);
        call(&mut api, 0x91, 0x49, &[0, 65536, 0]);
        call(&mut api, 0x90, 0x35, &[0, 256]);
        assert_eq!(image(&api).rgba[0], 20);
        call(&mut api, 0x90, 0x32, &[0, 128]);
        assert_eq!(image(&api).rgba[0], 10);
        assert_eq!(api.graph91_object_get_property(0, 2, 0).unwrap(), vec![128]);
        call(&mut api, 0x91, 0x46, &[1, 101, 0, 0]);
        call(&mut api, 0x91, 0x47, &[1, 0, 65536, 65536, 0]);
        call(&mut api, 0x91, 0x44, &[1, 2]);
        call(&mut api, 0x91, 0x45, &[1, 128]);
        call(&mut api, 0x91, 0x42, &[1, 1]);
        assert_eq!(&image(&api).rgba[..4], &[10, 0, 50, 255]);
        // Source binding is a generation, not a reusable public slot.
        api.store_runtime_bitmap(
            101,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [0, 255, 0, 0].repeat(8),
            },
            1,
        );
        api.refresh_native_backml();
        assert_eq!(&image(&api).rgba[..4], &[10, 0, 0, 255]);
        assert_eq!(
            api.graph_output_draw_items()
                .iter()
                .filter(|i| i.key.starts_with("runtime:backml:"))
                .count(),
            1
        );
    }
}
