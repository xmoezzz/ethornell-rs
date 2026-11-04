//! Sprite mode 6 (428120 -> 42A4F0 -> 40F700/40EB10/40FB20).
use super::*;
use crate::perspective_blit::{self, PerspectiveBlit, Surface};

#[derive(Debug, Clone)]
pub(crate) struct Mode6Raster {
    args: Vec<i32>,
    bindings: Vec<(i32, u64)>,
    revisions: Vec<u64>,
}
impl RuntimeTraceApi {
    pub(super) fn mode6_bindings_current(&self, object: i32) -> bool {
        self.native_mode6_rasters.get(&object).is_none_or(|s| {
            s.bindings.iter().all(|(bitmap, generation)| {
                self.graph_resources
                    .get(bitmap)
                    .is_some_and(|r| r.generation == *generation)
            })
        })
    }
    pub(super) fn tick_mode6_rasters(&mut self) {
        let changed = self
            .native_mode6_rasters
            .iter()
            .filter_map(|(&object, state)| {
                let changed = !self.mode6_bindings_current(object)
                    || state.bindings.iter().enumerate().any(|(i, (id, _))| {
                        self.graph_resources.get(id).is_some_and(|r| {
                            self.graph_image_revision(&r.key) != state.revisions[i]
                        })
                    });
                changed.then(|| state.args.clone())
            })
            .collect::<Vec<_>>();
        for args in changed {
            self.sync_mode6_raster(&args);
        }
    }
    pub(super) fn mode6_transform_parameters(&self, a: &[i32]) -> Option<[i32; 7]> {
        if a.len() != 20 {
            return None;
        }
        let properties = self.graph_object_properties.get(&a[0])?;
        let native = properties.native;
        let pair = |id| properties.properties.get(&id).copied().unwrap_or((0, 0));

        let progress = native.fixed_parameter_16_16;
        let advance = |delta: i32| ((i64::from(delta) * i64::from(progress as u32)) >> 24) as i32;
        let curve = crate::animation::target_curve_fixed_16(pair(0x8f).0, progress);
        let angle_delta = |delta: i32| ((i64::from(delta) * i64::from(curve)) >> 16) as i32;
        let anchor = properties
            .properties
            .get(&0x40)
            .copied()
            .unwrap_or((a[8], a[9]));
        let pitch_heading = properties
            .properties
            .get(&0x43)
            .copied()
            .unwrap_or((a[10], a[11]));
        let bank = properties.properties.get(&0x41).map_or(a[12], |p| p.0);
        // 429FD0 shares the two-scale equation with mode 5. Rotation has
        // three independent coefficients in mode 6 (83 pitch/heading,81 bank).
        let (base_x, base_y) = properties
            .properties
            .get(&0x42)
            .copied()
            .unwrap_or((65536, 0));
        let base_x = if base_x == 0 { 1 } else { base_x };
        let coupled = base_y != 0;
        let base_y = if base_y == 0 { base_x } else { base_y };
        let scale_delta = pair(0x82);
        let uniform = pair(0x81).1;
        let product = |a: i32, b: i32| ((u64::from(a as u32) * u64::from(b as u32)) >> 16) as i32;
        let scales = if coupled {
            [
                product(
                    base_x,
                    65536i32.wrapping_add(advance(scale_delta.0.wrapping_add(uniform))),
                ),
                product(
                    base_y,
                    65536i32.wrapping_add(advance(scale_delta.1.wrapping_add(uniform))),
                ),
            ]
        } else {
            [
                product(
                    65536i32.wrapping_add(advance(scale_delta.0)),
                    base_x.wrapping_add(advance(uniform)),
                ),
                product(
                    65536i32.wrapping_add(advance(scale_delta.1)),
                    base_y.wrapping_add(advance(uniform)),
                ),
            ]
        };
        let offset_x = anchor
            .0
            .wrapping_shl(16)
            .wrapping_add(advance(pair(0x80).0));
        let offset_y = anchor
            .1
            .wrapping_shl(16)
            .wrapping_add(advance(pair(0x80).1));
        let pitch = pitch_heading.0.wrapping_add(angle_delta(pair(0x83).0));
        let heading = pitch_heading.1.wrapping_add(angle_delta(pair(0x83).1));
        let bank = bank.wrapping_add(angle_delta(pair(0x81).0));
        Some([
            offset_x, offset_y, pitch, heading, bank, scales[0], scales[1],
        ])
    }

    pub(super) fn sync_mode6_raster(&mut self, a: &[i32]) -> bool {
        if a.len() != 20 {
            return false;
        }
        let object = a[0];
        if !self.mode6_bindings_current(object) {
            self.graph_layers.remove(&object);
            return false;
        }
        let Some(properties) = self.graph_object_properties.get(&object) else {
            return false;
        };
        let native = properties.native;
        let Some([offset_x, offset_y, pitch, heading, bank, scale_x, scale_y]) =
            self.mode6_transform_parameters(a)
        else {
            return false;
        };
        let vector = self.graph90_resolve_projected_sprite_vector(
            NativeMode5NodeArgs::from_source_args(a).unwrap(),
        );
        let transition = properties
            .named_properties
            .get("mode6-transition-current")
            .copied()
            .unwrap_or(a[6]);
        let Some(prepared) = self.prepare_projected_source(object,a[4],a[5],transition,crate::native_projected_source::perspective_ratio(vector.position_z,a[14] as u32),false) else {
            self.graph_layers.remove(&object);
            return false;
        };
        let Some(source)=prepared.image else { return false; };
        let offset_x=(offset_x >> prepared.level).wrapping_add(prepared.anchor_shift);
        let offset_y=offset_y >> prepared.level;
        let scale_x=scale_x.wrapping_shl(prepared.level);
        let scale_y=scale_y.wrapping_shl(prepared.level);
        let (screen_width, screen_height) = snapshot::runtime_frame_size(self);
        let center = self.graph_config.center;
        let center = if native.use_graph_center != 0
            && center.0 >= 0
            && center.1 >= 0
            && center.0 < screen_width as i32
            && center.1 < screen_height as i32
        {
            center
        } else {
            (screen_width as i32 / 2, screen_height as i32 / 2)
        };
        let (dest_x, dest_y, translate_x, translate_y) = if a[15] != 0 {
            (
                center.0.wrapping_shl(16),
                center.1.wrapping_shl(16),
                vector.position_x,
                vector.position_y,
            )
        } else {
            (
                center.0.wrapping_shl(16).wrapping_add(vector.position_x),
                center.1.wrapping_shl(16).wrapping_add(vector.position_y),
                0,
                0,
            )
        };
        let blit = PerspectiveBlit {
            dest_x,
            dest_y,
            anchor_x: offset_x,
            anchor_y: offset_y,
            scale_x,
            scale_y,
            translate_x,
            translate_y,
            translate_z: vector.position_z,
            pitch,
            heading,
            bank,
            order: a[13],
            distance: a[14],
            mode: 0,
            parameter: 0,
        };
        let mut corners = perspective_blit::project_corners(source.width, source.height, &blit);
        let bounds = perspective_blit::bounds(&corners);
        let [left, top, right, bottom] = bounds;
        let width = i64::from(right) - i64::from(left) + 1;
        let height = i64::from(bottom) - i64::from(top) + 1;
        if width <= 0
            || height <= 0
            || width
                .checked_mul(height)
                .is_none_or(|n| n > 16 * 1024 * 1024)
        {
            self.graph_layers.remove(&object);
            return false;
        }
        for corner in &mut corners {
            corner[0] -= left as f32;
            corner[1] -= top as f32;
        }
        let spans = perspective_blit::rasterize(&corners, height as i32);
        let mut image = DecodedImage {
            width: width as u32,
            height: height as u32,
            rgba: vec![0; width as usize * height as usize * 4],
        };
        // Intermediate RGB mode 6 is promoted to RGBA so uncovered pixels
        // remain transparent. The same scalar SSE kernel samples both.
        perspective_blit::draw(
            &mut image,
            2,
            &Surface {
                image: &source,
                format: 2,
            },
            spans.as_ref(),
            &blit,
        );
        let key = format!("runtime:sprite-mode6:{object}");
        self.store_graph_image(key.clone(), image);
        self.graph_layers.insert(
            object,
            RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key,
                target_surface: None,
                x: left as f32,
                y: top as f32,
                width: width as f32,
                height: height as f32,
                src_x: 0.0,
                src_y: 0.0,
                opacity: 1.0,
                z: native.priority as i32,
                enabled: true,
                transform_x: native
                    .primary_offset_x
                    .wrapping_add(native.secondary_offset_x) as f32,
                transform_y: native
                    .primary_offset_y
                    .wrapping_add(native.secondary_offset_y) as f32,
                transform_z: 0,
                scale_x: 1.0,
                scale_y: 1.0,
                rotation_degrees: 0.0,
                clip: None,
            },
        );
        self.graph_object_layers
            .entry(object)
            .or_default()
            .insert(object);
        let properties = self.graph_object_properties.get_mut(&object).unwrap();
        properties
            .named_properties
            .insert("mode6-projected-width".into(), width as i32);
        properties
            .named_properties
            .insert("mode6-projected-height".into(), height as i32);
        properties.native.position_x = left;
        properties.native.position_y = top;
        self.display_tree
            .set_local_position(object, left as f32, top as f32);
        self.display_tree
            .set_chain_depth(object, native.priority as i32);
        let bindings = [a[4], a[5]]
            .into_iter()
            .filter(|id| *id != -1)
            .filter_map(|id| self.graph_resources.get(&id).map(|r| (id, r.generation)))
            .collect::<Vec<_>>();
        let revisions = bindings
            .iter()
            .map(|(id, _)| self.graph_image_revision(&self.graph_resources[id].key))
            .collect();
        self.native_mode6_rasters.insert(
            object,
            Mode6Raster {
                args: a.to_vec(),
                bindings,
                revisions,
            },
        );
        self.graph_redraw_requested = Some(false);
        true
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{GraphApi, NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn mode6_uses_euler_projection_transition_and_live_bitmap_revision() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
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
        for (id, key, color) in [
            (1, "test:mode6-red", [255, 0, 0, 255]),
            (2, "test:mode6-blue", [0, 0, 255, 255]),
        ] {
            api.store_graph_image(
                key.into(),
                DecodedImage {
                    width: 16,
                    height: 12,
                    rgba: color.repeat(16 * 12),
                },
            );
            api.graph_resources
                .insert(id, RuntimeGraphResource::whole(key.into()));
            api.bitmap_formats.insert(id, 1);
        }
        let sprite = call(&mut api, 0x50, &[]).as_i32();
        call(
            &mut api,
            0x5D,
            &[
                sprite, 0, 0, 0, 1, 2, 256, -1, 0, 0, 0, 0, 0, 0, 256, 0, 0, 1, 0, 0,
            ],
        );
        assert_eq!(
            api.graph91_object_get_property(sprite, 0x10000000, 0)
                .unwrap(),
            vec![0, 0, 0, 65536, 65536]
        );
        let width = api.graph_layers[&sprite].width;
        let key = api.graph_layers[&sprite].key.clone();
        assert!(key.starts_with("runtime:sprite-mode6:"));
        assert!(
            api.graph_images[&key]
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .any(|p| p[2] > 200 && p[0] == 0)
        );
        assert!(!api.graph_layer_ignores_source_alpha(sprite, &api.graph_layers[&sprite]));
        // Property 43 changes the stored pitch/heading. Fixed-parameter
        // resync consumes it through the native three-axis geometry builder.
        call(&mut api, 0x38, &[sprite, 0x43, 0, 60 << 16]);
        call(&mut api, 0x35, &[sprite, 0]);
        assert!(api.graph_layers[&sprite].width < width);
        api.store_graph_image(
            "test:mode6-blue".into(),
            DecodedImage {
                width: 16,
                height: 12,
                rgba: [0, 255, 0, 255].repeat(16 * 12),
            },
        );
        api.tick_mode6_rasters();
        assert!(
            api.graph_images[&key]
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .any(|p| p[1] > 200 && p[2] == 0)
        );
        api.graph_resources.remove(&2);
        api.tick_mode6_rasters();
        assert!(!api.graph_layers.contains_key(&sprite));
    }
}
