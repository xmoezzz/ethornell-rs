//! Five CDspObjEffector modes. Native 4201B0 consumes the already-drawn
//! destination, using a private scratch/feedback bitmap at +0x168.
use super::*;
use ethornell_vm::GraphApi;
#[derive(Debug, Clone)]
pub(crate) struct EffectorRaster {
    bindings: Vec<(i32, u64)>,
    transform: [i32; 5],
    waves: Vec<i16>,
    feedback: DecodedImage,
}
impl RuntimeTraceApi {
    pub(super) fn configure_native_effector(
        &mut self,
        id: u16,
        a: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        // sub_4830B0..sub_483790: priority and transparency checks first,
        // then the configure helper's codes (255 unknown effector, 1/2 map
        // missing, 3/4 not a vector map, 5 gradation type, 6 distance,
        // 7 ripple slot, 9 scale).
        let err = |message: String| ethornell_vm::VmError::Runtime(message);
        let object = a[0];
        let (w, h) = snapshot::runtime_frame_size(self);
        let priority = *a.last().unwrap();
        let alpha = a[a.len() - 2];
        target_checks::priority(priority)?;
        target_checks::effect_level(alpha)?;
        if !self.graph91_effectors.contains_key(&object) {
            return Err(err("無効なエフェクターハンドルが指定されました".into()));
        }
        let mode = i32::from(id - 0x65);
        let maps = match id {
            0x65 => [a[1], a[2]],
            0x67 => [a[1], -1],
            _ => [-1, -1],
        };
        if id == 0x66 && a[1] as u32 > 1 {
            return Err(err(format!("指定されたグラデーションタイプ [ {} ] は無効です", a[1])));
        }
        if id == 0x68 && (a[4] == 0 || a[5] == 0) {
            return Err(err(format!("無効な伸縮倍率 [ {} , {} ] が指定されました", a[4], a[5])));
        }
        let mut bindings = Vec::new();
        for (index, bitmap) in maps.into_iter().enumerate() {
            if bitmap == -1 && index == 1 {
                continue;
            }
            if bitmap == -1 && !matches!(id, 0x65 | 0x67) {
                continue;
            }
            let missing = || {
                err(match (id, index) {
                    (0x67, _) => format!(
                        "ベクトル＆ディスタンスマップとして指定されたビットマップ [ {bitmap} ] は無効です"
                    ),
                    (_, 0) => format!("第一ベクトルマップとして指定されたビットマップ [ {bitmap} ] は無効です"),
                    _ => format!("第二ベクトルマップとして指定されたビットマップ [ {bitmap} ] は無効です"),
                })
            };
            let info = self.query_bitmap_info(bitmap).ok_or_else(missing)?;
            let format = if id == 0x67 { 6 } else { 4 };
            if info.format != format
                || info.width < w
                || info.height < h
                || (id == 0x67 && (info.width != w || info.height != h))
            {
                return Err(err(match (id, index) {
                    (0x67, _) => format!(
                        "指定されたビットマップ [ {bitmap} ] はベクトル＆ディスタンスマップではない、\n\n或いはスクリーンサイズと一致していません"
                    ),
                    (_, 0) => format!(
                        "第一ベクトルマップとして指定されたビットマップ [ {bitmap} ] はベクトルマップではない、\n\n若しくはスクリーンサイズとピクセル数が一致していません"
                    ),
                    _ => format!(
                        "第二ベクトルマップとして指定されたビットマップ [ {bitmap} ] はベクトルマップではない、\n\n若しくはスクリーンサイズとピクセル数が一致していません"
                    ),
                }));
            }
            bindings.push((bitmap, self.graph_resources.get(&bitmap).ok_or_else(missing)?.generation));
        }
        let waves = if id == 0x67 {
            if a[2] == 0 {
                return Err(err(format!("無効なディスタンスの最大値 [ {} ] が指定されました", a[2])));
            }
            self.effects
                .vm_wave_samples(a[3], a[2], 0, alpha)
                .ok_or_else(|| err(format!("指定された波紋 [ {} ] は登録されていません", a[3])))?
        } else {
            Vec::new()
        };
        let parameters = match id {
            0x65 => [a[3], a[4], 0, 0, 0, 0, 0, 0, 0],
            0x66 => [a[1], a[2], 0, 0, 0, 0, 0, 0, 0],
            0x67 => [a[2], a[3], a[4], 0, 0, 0, 0, 0, 0],
            0x68 => [a[1], a[2], a[3], a[4], a[5], a[6], a[7], 0, 0],
            _ => [alpha, 0, 0, 0, 0, 0, 0, 0, 0],
        };
        let state = self.graph91_effectors.get_mut(&object).unwrap();
        state.mode = mode;
        state.vector_maps = maps;
        state.parameters = parameters;
        state.priority = priority;
        self.native_effector_rasters.insert(
            object,
            EffectorRaster {
                bindings,
                transform: [0, 0, 0, 65536, 65536],
                waves,
                feedback: crate::native_map_filter::blank(w, h),
            },
        );
        let properties = self.graph_object_properties.entry(object).or_default();
        if id == 0x68 {
            for key in [0x80, 0x81, 0x82, 0x8f] {
                properties.properties.remove(&key);
            }
            properties.native.fixed_parameter_16_16 = 0;
        }
        self.graph90_set_common_state(object, None, None, Some(alpha), Some(priority));
        self.sync_native_effector_parameters(object);
        let key = format!("runtime:effector:{object}");
        self.store_graph_image(key.clone(), crate::native_map_filter::blank(w, h));
        self.install_native_map_filter_layer(object, key, w, h);
        // Configuring never touches vtable+4; keep a 91:64 hide in place.
        let shown = self.graph_object_draw_enabled.get(&object).copied().unwrap_or(true);
        self.set_graph_object_draw_enabled_raw(object, shown);
        self.refresh_native_filters();
        Ok(())
    }
    /// Native +72 alpha and +80 fixed-parameter virtuals rebuild derived
    /// state. Merely assigning a coefficient does not rebuild it (4208D0).
    pub(super) fn sync_native_effector_parameters(&mut self, object: i32) {
        let Some(state) = self.graph91_effectors.get(&object) else {
            return;
        };
        let Some(raster) = self.native_effector_rasters.get_mut(&object) else {
            return;
        };
        let Some(properties) = self.graph_object_properties.get(&object) else {
            return;
        };
        let p = state.parameters;
        if state.mode == 2 {
            let offset = properties.properties.get(&0x100).map_or(0, |v| v.1);
            if let Some(waves) =
                self.effects
                    .vm_wave_samples(p[1], p[0], offset, properties.alpha_parameter())
            {
                raster.waves = waves;
            }
        }
        if state.mode != 3 {
            return;
        }
        let progress = properties.native.fixed_parameter_16_16;
        let pair = |id| properties.properties.get(&id).copied().unwrap_or_default();
        let delta = |value: i32| ((i64::from(value) * i64::from(progress as u32)) >> 24) as i32;
        let curve = crate::animation::target_curve_fixed_16(pair(0x8f).0, progress);
        let alpha = properties.alpha_parameter() as u32;
        let scale = |base: i32, coefficient: i32| {
            let change = (i128::from(coefficient) * i128::from(progress as u32)) >> 24;
            ((((i128::from(base as u32) + change - 65536) * i128::from(alpha)) >> 8) as i32)
                .wrapping_add(65536)
        };
        let angle = ((i128::from(p[2]) + ((i128::from(pair(0x81).0) * i128::from(curve)) >> 16))
            * i128::from(alpha)
            >> 8) as i32;
        raster.transform = [
            p[0].wrapping_add(delta(pair(0x80).0)),
            p[1].wrapping_add(delta(pair(0x80).1)),
            angle,
            scale(p[3], pair(0x82).0),
            scale(p[4], pair(0x82).1),
        ];
    }
    pub(super) fn set_native_effector_property(
        &mut self,
        object: i32,
        property: u32,
        value: i32,
        extra: i32,
    ) -> Option<std::result::Result<(), i32>> {
        let state = self.graph91_effectors.get(&object)?;
        match property {
            0x80 | 0x81 | 0x82 if state.mode != 3 => Some(Ok(())),
            0x80 | 0x81 | 0x82 | 0x8f | 0xff => {
                self.graph_object_properties
                    .entry(object)
                    .or_default()
                    .properties
                    .insert(property, (value, extra));
                Some(Ok(()))
            }
            0x100 => {
                if state.mode != 2 {
                    return Some(Ok(()));
                }
                let alpha = if value as u32 > 256 {
                    self.graph_object_properties[&object].alpha_parameter()
                } else {
                    value
                };
                if self
                    .effects
                    .vm_wave_samples(state.parameters[1], state.parameters[0], extra, alpha)
                    .is_none()
                {
                    return Some(Err(254));
                }
                self.graph_object_properties
                    .entry(object)
                    .or_default()
                    .properties
                    .insert(property, (alpha, extra));
                self.set_graph_object_alpha_recursive_raw(object, alpha);
                self.sync_native_effector_parameters(object);
                Some(Ok(()))
            }
            _ => None,
        }
    }
    pub(super) fn render_native_effector(
        &mut self,
        object: i32,
        source: &DecodedImage,
    ) -> Option<DecodedImage> {
        let state = self.graph91_effectors.get(&object)?.clone();
        let raster = self.native_effector_rasters.get(&object)?;
        if raster.bindings.iter().any(|(bitmap, generation)| {
            self.graph_resources
                .get(bitmap)
                .is_none_or(|r| r.generation != *generation)
        }) {
            return Some(source.clone());
        }
        let properties = self.graph_object_properties.get(&object)?;
        let alpha = properties.transparency_parameter();
        let p = state.parameters;
        let filter = properties.properties.get(&255).map(|v| v.0);
        let mut result = match state.mode {
            0 => self.effects.warp_displacement_into(
                source,
                [source.width, source.height],
                state.vector_maps[0],
                (state.vector_maps[1] != -1).then_some(state.vector_maps[1]),
                alpha,
                filter.unwrap_or(p[1]) != 0,
            )?,
            1 => crate::native_background_effect::native_horizontal_blur(source, alpha, p[0] != 0),
            2 => self.effects.warp_vector_map(
                state.vector_maps[0],
                source,
                &raster.waves,
                [0, 0],
                [source.width, source.height],
                false,
                filter.unwrap_or(0) != 0,
            )?,
            3 => effector_affine(source, raster.transform, filter.unwrap_or(p[5]) != 0),
            4 => {
                let mut image = source.clone();
                let df = self
                    .bitmap_formats
                    .get(&NATIVE_SCREEN_BITMAP)
                    .copied()
                    .unwrap_or(1);
                crate::bitmap_blend::blit_mode(
                    &mut image,
                    df,
                    &raster.feedback,
                    df,
                    0,
                    0,
                    1,
                    256 - alpha,
                );
                self.native_effector_rasters.get_mut(&object)?.feedback = image.clone();
                image
            }
            _ => source.clone(),
        };
        if self
            .bitmap_formats
            .get(&NATIVE_SCREEN_BITMAP)
            .copied()
            .unwrap_or(1)
            == 1
        {
            for pixel in result.rgba.as_chunks_mut::<4>().0 {
                pixel[3] = 255;
            }
        }
        Some(result)
    }
}
/// 4169D0 prepares start/dx/dy as independently truncated signed 16.16.
/// 418500 samples nearest; 417ED0 uses four fractional bits and black borders.
fn effector_affine(source: &DecodedImage, t: [i32; 5], bilinear: bool) -> DecodedImage {
    native_inverse_affine(
        source,
        [source.width, source.height],
        [
            ((source.width & !1) << 15) as i32,
            ((source.height & !1) << 15) as i32,
        ],
        t,
        bilinear,
    )
}
pub(super) fn native_inverse_affine(
    source: &DecodedImage,
    size: [u32; 2],
    destination: [i32; 2],
    t: [i32; 5],
    bilinear: bool,
) -> DecodedImage {
    let [anchor_x, anchor_y, angle, scale_x, scale_y] = t;
    let mut out = crate::native_map_filter::blank(size[0], size[1]);
    if scale_x == 0 || scale_y == 0 {
        return out;
    }
    let theta = -(angle as f64) * std::f64::consts::PI / 11796480.;
    let (sin, cos) = theta.sin_cos();
    let (sx, sy) = (
        65536. / (scale_x as u32 as f64),
        65536. / (scale_y as u32 as f64),
    );
    let cx = destination[0] as f64 / 65536.;
    let cy = destination[1] as f64 / 65536.;
    let start = [
        (((-cx * cos - cy * sin) * sx + anchor_x as f64 / 65536.) * 65536.) as i64 as i32,
        ((anchor_y as f64 / 65536. - (cy * cos - cx * sin) * sy) * 65536.) as i64 as i32,
    ];
    let dx = [
        (cos * sx * 65536.) as i64 as i32,
        (-sin * sy * 65536.) as i64 as i32,
    ];
    let reverse = std::f64::consts::FRAC_PI_2 - angle as f64 * std::f64::consts::PI / 11796480.;
    let dy = [
        (-reverse.cos() * sx * 65536.) as i64 as i32,
        (reverse.sin() * sy * 65536.) as i64 as i32,
    ];
    for y in 0..out.height {
        for x in 0..out.width {
            let fx = start[0]
                .wrapping_add(dx[0].wrapping_mul(x as i32))
                .wrapping_add(dy[0].wrapping_mul(y as i32));
            let fy = start[1]
                .wrapping_add(dx[1].wrapping_mul(x as i32))
                .wrapping_add(dy[1].wrapping_mul(y as i32));
            let (px, py) = (
                (fx >> 16).clamp(-32768, 32767),
                (fy >> 16).clamp(-32768, 32767),
            );
            let color = crate::native_background_effect::sample_background_pixel(
                source,
                px,
                py,
                if bilinear {
                    (fx as u32 >> 12 & 15) as i32
                } else {
                    0
                },
                if bilinear {
                    (fy as u32 >> 12 & 15) as i32
                } else {
                    0
                },
                16,
            );
            let index = ((y * out.width + x) * 4) as usize;
            out.rgba[index..index + 4].copy_from_slice(&color);
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn all_effector_modes_draw_and_transform_coefficients_wait_for_setter() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 4;
        api.screen_height = 2;
        let call = |api: &mut RuntimeTraceApi, group, id, args: &[i32]| {
            GraphApi::call_graph(
                api,
                &mut NativeCallFrame::new(
                    NativeOpcode { group, id },
                    args.iter().copied().map(Value::Int).collect(),
                ),
            )
            .unwrap()
        };
        api.store_runtime_bitmap(
            10,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [0, 0, 0, 0].repeat(8),
            },
            4,
        );
        api.effects.create_displacement_map(10, 4, 2);
        api.store_runtime_bitmap(
            11,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [0, 0, 0, 0].repeat(8),
            },
            6,
        );
        api.effects.create_vector_map(11, 4, 2);
        api.effects
            .configure_compact_wave_table(&[2, 1, 0, 1, 0].map(Value::Int));
        let effector = call(&mut api, 0x91, 0x60, &[]).as_i32();
        let source = DecodedImage {
            width: 4,
            height: 2,
            rgba: [80, 40, 20, 255].repeat(8),
        };
        for (id, args) in [
            (0x65, vec![effector, 10, -1, 0, 0, 1]),
            (0x66, vec![effector, 1, 0, 1]),
            (0x67, vec![effector, 11, 1, 0, 0, 1]),
            (
                0x68,
                vec![effector, 2 << 16, 1 << 16, 0, 65536, 65536, 0, 256, 1],
            ),
        ] {
            call(&mut api, 0x91, id, &args);
            assert_eq!(
                api.render_native_effector(effector, &source).unwrap(),
                source,
                "mode {id:02X}"
            );
        }
        // 4208D0 stores coefficients; the native +80 setter rebuilds them.
        let before = api.native_effector_rasters[&effector].transform;
        call(&mut api, 0x90, 0x38, &[effector, 0x82, -32768, 0]);
        assert_eq!(api.native_effector_rasters[&effector].transform, before);
        call(&mut api, 0x90, 0x35, &[effector, 256]);
        assert_eq!(api.native_effector_rasters[&effector].transform[3], 32768);
        call(&mut api, 0x91, 0x69, &[effector, 128, 1]);
        let first = api.render_native_effector(effector, &source).unwrap();
        let second = api.render_native_effector(effector, &source).unwrap();
        assert_eq!(&first.rgba[..3], &[40, 20, 10]);
        assert_eq!(&second.rgba[..3], &[60, 30, 15]);
    }
}
