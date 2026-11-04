//! CDspObjBackD/DST/GRD/STR/RTT/MSC, from native_all.c and native_all.asm.
use super::*;
use ethornell_vm::GraphApi;

#[derive(Debug, Clone)]
pub(crate) struct BackgroundEffect {
    selector: u16,
    args: Vec<i32>,
    bindings: Vec<(i32, u64)>,
    signature: Vec<u64>,
}

impl RuntimeTraceApi {
    pub(super) fn configure_background_effect(
        &mut self,
        selector: u16,
        args: &[i32],
        frames: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let class = NativeBackgroundClass::for_graph90_selector(selector).unwrap();
        let object = self.graph90_prepare_current_background(class);
        // sub_47C580 / sub_47C6E0 / sub_47C7C0 check the transparency
        // (sub_498920) before anything else.
        if let Some(index) = match selector {
            0x45 => Some(3),
            0x46 => Some(2),
            0x47 => Some(4),
            _ => None,
        } {
            target_checks::effect_level(args[index])?;
        }
        let err = |message: String| ethornell_vm::VmError::Runtime(message);
        let missing = |bitmap: i32| err(format!("指定されたビットマップ [ {bitmap} ] は存在しません"));
        let mut resources = if selector == 0x44 {
            frames.to_vec()
        } else {
            vec![args[0]]
        };
        if selector == 0x44 && !(2..=32).contains(&frames.len()) {
            return Err(err(format!("無効なビットマップ数 [ {} ] が指定されました", frames.len())));
        }
        match selector {
            0x45 => {
                resources.push(args[1]);
                if args[2] != -1 {
                    resources.push(args[2]);
                }
            }
            0x4A if !matches!(args[1], 0x7000 | 0x7001) => resources.push(args[1]),
            _ => {}
        }
        // The resources first (sub_4627xx checks them in argument order),
        // then the selector's own parameters.
        let mut bindings = Vec::new();
        let screen_format = self
            .query_bitmap_info(NATIVE_SCREEN_BITMAP)
            .map_or(1, |i| i.format);
        let invalid = |index: usize, bitmap: i32, exists: bool| -> ethornell_vm::VmError {
            match selector {
                0x44 => {
                    let list = frames.iter().map(i32::to_string).collect::<Vec<_>>().join(" , ");
                    err(format!(
                        "指定されたビットマップ [ {list} ] の中に無効もしくは背景として表示できないものが含まれています"
                    ))
                }
                0x4A if index == 0 => err(format!(
                    "指定された表側ビットマップ [ {bitmap} ] は存在しないか、背景には使用できません"
                )),
                0x4A => err(format!(
                    "指定された裏側ビットマップ [ {bitmap} ] は存在しないか、背景には使用できません"
                )),
                _ if !exists => missing(bitmap),
                0x45 if index != 0 => err(format!(
                    "指定されたビットマップ [ {bitmap} ] はベクトルマップではない、\n\n或いはスクリーンサイズと一致していません"
                )),
                _ => err(format!("指定されたビットマップ [ {bitmap} ] はスクリーンサイズと一致していません")),
            }
        };
        for (index, bitmap) in resources.into_iter().enumerate() {
            let info = self.query_bitmap_info(bitmap).ok_or_else(|| invalid(index, bitmap, false))?;
            if !matches!(selector, 0x48 | 0x49) {
                let format = if selector == 0x45 && index != 0 {
                    4
                } else {
                    screen_format
                };
                if info.width != self.screen_width as u32
                    || info.height != self.screen_height as u32
                    || !(info.format == format
                        || (matches!(info.format, 1 | 2) && matches!(format, 1 | 2)))
                {
                    return Err(invalid(index, bitmap, true));
                }
            }
            let generation = self
                .graph_resources
                .get(&bitmap)
                .ok_or_else(|| invalid(index, bitmap, false))?
                .generation;
            bindings.push((bitmap, generation));
        }
        match selector {
            0x46 if args[1] as u32 > 1 => {
                return Err(err(format!("無効なグラデーションタイプ [ {} ] が指定されました", args[1])));
            }
            0x48 if (args[3] as u32) < 2 || (args[4] as u32) < 2 => {
                return Err(err(format!("無効な参照範囲 [ {} , {} ] が指定されました", args[3], args[4])));
            }
            0x49 if args[1] == 0 => {
                return Err(err(format!("無効な伸縮倍率 [ {} ] が指定されました", args[1])));
            }
            0x4A if args[2] != 0 => {
                return Err(err(format!("無効なスタイル [ {} ] が指定されました", args[2])));
            }
            0x4A if args[4] as u32 > 1 => {
                return Err(err(format!("無効な連動状態 [ {} ] が指定されました", args[4])));
            }
            _ => {}
        }
        if selector != 0x44 {
            self.graph_object_properties
                .entry(object)
                .or_default()
                .format_resource = Some(args[0]);
        }
        if selector == 0x48 {
            if let Some(background) = self
                .graph_object_properties
                .get_mut(&object)
                .and_then(|p| p.background.as_mut())
            {
                background.set_position(
                    args[1].wrapping_shl(16),
                    args[2].wrapping_shl(16),
                    self.screen_width,
                    self.screen_height,
                );
            }
        }
        if selector == 0x49 {
            let properties = self.graph_object_properties.get_mut(&object).unwrap();
            properties.properties.remove(&0x80);
            properties.properties.remove(&0x101);
        }
        self.background_effects.insert(
            object,
            BackgroundEffect {
                selector,
                args: args.to_vec(),
                bindings,
                signature: Vec::new(),
            },
        );
        if selector == 0x44 {
            self.graph_object_properties
                .entry(object)
                .or_default()
                .set_alpha_parameter(args[2]);
        }
        self.refresh_background_effect(object);
        Ok(())
    }

    pub(super) fn background_effect_bindings_current(&self, object: i32) -> bool {
        self.background_effects.get(&object).is_none_or(|state| {
            state.bindings.iter().all(|&(bitmap, generation)| {
                self.graph_resources
                    .get(&bitmap)
                    .is_some_and(|r| r.generation == generation)
            })
        })
    }

    pub(super) fn refresh_background_effect(&mut self, object: i32) -> bool {
        let Some(state) = self.background_effects.get(&object).cloned() else {
            return false;
        };
        if self.graph90_background_class(object)
            != NativeBackgroundClass::for_graph90_selector(state.selector)
        {
            return false;
        }
        let Some(properties) = self.graph_object_properties.get(&object) else {
            return false;
        };
        let alpha = properties.alpha_parameter();
        let mut signature = vec![
            alpha as u64,
            self.effects.vector_revision,
            self.screen_width as u64,
            self.screen_height as u64,
        ];
        for &(bitmap, generation) in &state.bindings {
            let Some(resource) = self.graph_resources.get(&bitmap) else {
                self.graph_layers.remove(&object);
                return false;
            };
            if resource.generation != generation {
                self.graph_layers.remove(&object);
                return false;
            }
            signature.push(*self.graph_image_revisions.get(&resource.key).unwrap_or(&0));
        }
        for &property in &[0x80u32, 0x101, 0x102] {
            let (a, b) = properties
                .properties
                .get(&property)
                .copied()
                .unwrap_or_default();
            signature.extend([a as u64, b as u64]);
        }
        if let Some((x, y)) = properties.background.and_then(|b| b.integer_position()) {
            signature.extend([x as u64, y as u64]);
        }
        if state.signature == signature {
            return true;
        }
        let a = &state.args;
        let primary = if state.selector == 0x44 {
            let Some(&(bitmap, _)) = state.bindings.get(alpha as u32 as usize) else {
                self.graph_layers.remove(&object);
                return false;
            };
            self.graph_bitmap_image(bitmap)
        } else {
            self.graph_bitmap_image(a[0])
        };
        let Some(primary) = primary else {
            return false;
        };
        let z = properties.native.priority as i32;
        let mut image = match state.selector {
            0x44 => primary,
            0x45 => {
                let Some(image) = self.effects.warp_background_displacement(
                    &primary,
                    a[1],
                    (a[2] != -1).then_some(a[2]),
                    alpha,
                    a[4] != 0,
                ) else {
                    return false;
                };
                image
            }
            0x46 => native_horizontal_blur(&primary, alpha, a[1] != 0),
            0x48 => {
                let (x, y) = properties
                    .background
                    .and_then(|b| b.integer_position())
                    .unwrap_or((a[1].wrapping_shl(16), a[2].wrapping_shl(16)));
                let target = properties.properties.get(&0x102).copied();
                let (tx, ty, tw, th) = target
                    .map(|(xy, wh)| {
                        (
                            (xy as i16) as i32,
                            (xy >> 16) as i16 as i32,
                            (wh as u16) as i32,
                            (wh as u32 >> 16) as i32,
                        )
                    })
                    .unwrap_or((x >> 16, y >> 16, a[3], a[4]));
                let t = alpha as u32 as f64 / 256.0;
                let w = a[3] as u32 as f64 + tw.wrapping_sub(a[3]) as f64 * t;
                let h = a[4] as u32 as f64 + th.wrapping_sub(a[4]) as f64 * t;
                let ox = (x as f64 + (tx.wrapping_shl(16).wrapping_sub(x)) as f64 * t) / 65536.0;
                let oy = (y as f64 + (ty.wrapping_shl(16).wrapping_sub(y)) as f64 * t) / 65536.0;
                native_stretch_background(
                    &primary,
                    [self.screen_width as u32, self.screen_height as u32],
                    [ox, oy, w, h],
                )
            }
            0x49 => {
                let (scale, angle) = properties
                    .properties
                    .get(&0x101)
                    .copied()
                    .unwrap_or((a[1], a[2]));
                let (delta_scale, delta_angle) = properties
                    .properties
                    .get(&0x80)
                    .copied()
                    .unwrap_or_default();
                let alpha = alpha as u32;
                let mut scale = scale as u32;
                if delta_scale != 0 && alpha != 0 {
                    let wave = (alpha as f64 * std::f64::consts::PI / 512.0).sin() * 256.0;
                    let sign = if delta_scale < 0 { 1.0 } else { -1.0 };
                    let magnitude = delta_scale.unsigned_abs() as f64;
                    let ratio = sign * (256.0 - wave) / 256.0
                        + delta_scale as f64 / (magnitude - (magnitude - 65536.0) * wave / 256.0);
                    scale = (ratio * 65536.0 + scale as f64) as i64 as u32;
                }
                let angle = angle.wrapping_add(
                    ((delta_angle as u64).wrapping_mul(u64::from(alpha)) >> 8) as i32,
                );
                native_rotate_background(
                    &primary,
                    [self.screen_width as u32, self.screen_height as u32],
                    scale,
                    angle,
                )
            }
            0x4A => {
                let mut image = DecodedImage {
                    width: primary.width,
                    height: primary.height,
                    rgba: vec![0; primary.rgba.len()],
                };
                if a[4] == 1 {
                    match a[1] {
                        0x7000 => image
                            .rgba
                            .as_chunks_mut::<4>()
                            .0
                            .iter_mut()
                            .for_each(|p| *p = [0, 0, 0, 255]),
                        0x7001 => image.rgba.fill(255),
                        bitmap => {
                            if let Some(other) = self.graph_bitmap_image(bitmap) {
                                native_mosaic_into(&mut image, &other, 256 - alpha, 0);
                            }
                        }
                    }
                }
                native_mosaic_into(
                    &mut image,
                    &primary,
                    alpha,
                    if a[4] == 1 { alpha } else { 0 },
                );
                image
            }
            _ => return false,
        };
        // These subclasses write the destination directly. Their alpha
        // selects an effect state, rather than ordinary sprite transparency.
        for pixel in image.rgba.as_chunks_mut::<4>().0 {
            pixel[3] = 255;
        }
        let key = format!("runtime:background-effect:{object}");
        let width = image.width as f32;
        let height = image.height as f32;
        self.store_graph_image(key.clone(), image);
        self.graph_layers.insert(
            object,
            RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key,
                target_surface: None,
                x: 0.0,
                y: 0.0,
                width,
                height,
                src_x: 0.0,
                src_y: 0.0,
                opacity: 1.0,
                z,
                enabled: true,
                transform_x: 0.0,
                transform_y: 0.0,
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
        self.background_effects.get_mut(&object).unwrap().signature = signature;
        self.graph_redraw_requested = Some(false);
        true
    }
}

pub(super) fn native_horizontal_blur(source: &DecodedImage, alpha: i32, clamp: bool) -> DecodedImage {
    let radius = (alpha as u32).min(255) as i32;
    if radius == 0 {
        return source.clone();
    }
    let count = 2 * radius + 1;
    let shift = if count < 64 {
        0
    } else if count < 128 {
        1
    } else {
        2
    };
    let reciprocal = 0x10003 / count;
    let mut output = source.clone();
    for y in 0..source.height as usize {
        let sample = |x: i32, c: usize| -> i32 {
            let x = if clamp {
                x.clamp(0, source.width as i32 - 1)
            } else if x < 0 || x >= source.width as i32 {
                return 0;
            } else {
                x
            };
            i32::from(source.rgba[(y * source.width as usize + x as usize) * 4 + c]) >> shift
        };
        let mut sums = [0i32; 4];
        for dx in -radius..=radius {
            for c in 0..4 {
                sums[c] += sample(dx, c);
            }
        }
        for x in 0..source.width as i32 {
            for c in 0..4 {
                output.rgba[(y * source.width as usize + x as usize) * 4 + c] =
                    ((((sums[c] as i16 as i32) * reciprocal) >> 16) << shift).clamp(0, 255) as u8;
                sums[c] -= sample(x - radius, c);
                sums[c] += sample(x + radius + 1, c);
            }
        }
    }
    output
}

fn native_mosaic_into(
    destination: &mut DecodedImage,
    source: &DecodedImage,
    block_parameter: i32,
    transparency: i32,
) {
    let block = (block_parameter as u32).saturating_add(1).max(1);
    for y in 0..destination.height {
        for x in 0..destination.width {
            let sx = (x / block * block).min(source.width - 1);
            let sy = (y / block * block).min(source.height - 1);
            let src = (sy as usize * source.width as usize + sx as usize) * 4;
            let dst = (y as usize * destination.width as usize + x as usize) * 4;
            for c in 0..4 {
                let s = i32::from(source.rgba[src + c]);
                let d = i32::from(destination.rgba[dst + c]);
                destination.rgba[dst + c] = (s + ((d - s) * transparency >> 8)).clamp(0, 255) as u8;
            }
        }
    }
}

pub(super) fn native_rotate_background(
    source: &DecodedImage,
    size: [u32; 2],
    scale: u32,
    angle: i32,
) -> DecodedImage {
    let mut image = DecodedImage {
        width: size[0],
        height: size[1],
        rgba: vec![0; size[0] as usize * size[1] as usize * 4],
    };
    if scale == 0 {
        return image;
    }
    let theta = -(angle as f64) * std::f64::consts::PI / (180.0 * 65536.0);
    let (sin, cos) = theta.sin_cos();
    let inverse = 65536.0 / scale as f64;
    let left = -((size[0] - 1) as f64) * 0.5;
    let top = (size[1] - 1) as f64 * 0.5;
    let start_x = ((left * cos - top * sin) * inverse * 65536.0 + source.width as f64 * 32768.0)
        as i64 as i32;
    let start_y = ((top * cos + left * sin) * -inverse * 65536.0 + source.height as f64 * 32768.0)
        as i64 as i32;
    let dx = [
        (cos * inverse * 65536.0) as i64 as i32,
        (-sin * inverse * 65536.0) as i64 as i32,
    ];
    let dy = [
        (sin * inverse * 65536.0) as i64 as i32,
        (cos * inverse * 65536.0) as i64 as i32,
    ];
    for y in 0..size[1] {
        for x in 0..size[0] {
            let sx = start_x
                .wrapping_add(dx[0].wrapping_mul(x as i32))
                .wrapping_add(dy[0].wrapping_mul(y as i32));
            let sy = start_y
                .wrapping_add(dx[1].wrapping_mul(x as i32))
                .wrapping_add(dy[1].wrapping_mul(y as i32));
            if sx >= 0
                && sy >= 0
                && (sx >> 16) < source.width as i32
                && (sy >> 16) < source.height as i32
            {
                let src = ((sy as usize >> 16) * source.width as usize + (sx as usize >> 16)) * 4;
                let dst = (y as usize * size[0] as usize + x as usize) * 4;
                image.rgba[dst..dst + 4].copy_from_slice(&source.rgba[src..src + 4]);
            }
        }
    }
    image
}

fn native_stretch_background(
    source: &DecodedImage,
    size: [u32; 2],
    rect: [f64; 4],
) -> DecodedImage {
    let mut image = DecodedImage {
        width: size[0],
        height: size[1],
        rgba: vec![0; size[0] as usize * size[1] as usize * 4],
    };
    let [x, y, w, h] = rect;
    if w < 2.0 || h < 2.0 {
        return image;
    }
    let scale_x = (source.width.wrapping_mul(0x10004) as f64 / w) as u32;
    let scale_y = (source.height.wrapping_mul(0x10004) as f64 / h) as u32;
    if scale_x == 0 || scale_y == 0 {
        return image;
    }
    let ix = 65536.0 / scale_x as f64;
    let iy = 65536.0 / scale_y as f64;
    let start = [
        (x * 65536.0 + w * 32768.0 - size[0] as f64 * 32768.0 * ix
            + if scale_x < 65536 {
                (ix * 0.5 - 0.5) * 65536.0
            } else {
                0.0
            }) as i64 as i32,
        (y * 65536.0 + h * 32768.0 - size[1] as f64 * 32768.0 * iy
            + if scale_y < 65536 {
                (iy * 0.5 - 0.5) * 65536.0
            } else {
                0.0
            }) as i64 as i32,
    ];
    let step = [(ix * 65536.0) as i32, (iy * 65536.0) as i32];
    for y in 0..size[1] {
        for x in 0..size[0] {
            let sx = start[0].wrapping_add(step[0].wrapping_mul(x as i32));
            let sy = start[1].wrapping_add(step[1].wrapping_mul(y as i32));
            let p = sample_background_pixel(
                source,
                sx >> 16,
                sy >> 16,
                ((sx as u32 >> 9) & 127) as i32,
                ((sy as u32 >> 9) & 127) as i32,
                128,
            );
            let dst = (y as usize * size[0] as usize + x as usize) * 4;
            image.rgba[dst..dst + 4].copy_from_slice(&p);
        }
    }
    image
}

pub(super) fn sample_background_pixel(
    source: &DecodedImage,
    x: i32,
    y: i32,
    fx: i32,
    fy: i32,
    denominator: i32,
) -> [u8; 4] {
    let pixel = |x: i32, y: i32| -> [u8; 4] {
        if x < 0 || y < 0 || x >= source.width as i32 || y >= source.height as i32 {
            return [0; 4];
        }
        let i = (y as usize * source.width as usize + x as usize) * 4;
        source.rgba[i..i + 4].try_into().unwrap()
    };
    let a = pixel(x, y);
    let b = pixel(x + 1, y);
    let c = pixel(x, y + 1);
    let d = pixel(x + 1, y + 1);
    let shift = denominator.trailing_zeros();
    std::array::from_fn(|i| {
        let top =
            i32::from(b[i]) + ((i32::from(a[i]) - i32::from(b[i])) * (denominator - fx) >> shift);
        let bottom =
            i32::from(d[i]) + ((i32::from(c[i]) - i32::from(d[i])) * (denominator - fx) >> shift);
        (bottom + ((top - bottom) * (denominator - fy) >> shift)).clamp(0, 255) as u8
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn background_families_materialize_and_keep_effect_alpha_separate() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 2;
        api.screen_height = 2;
        let pixels = vec![10, 0, 0, 255, 20, 0, 0, 255, 30, 0, 0, 255, 40, 0, 0, 255];
        for (bitmap, key, format) in [
            (1, "test:bg-primary", 1),
            (2, "test:bg-second", 1),
            (3, "test:bg-map", 4),
        ] {
            api.store_graph_image(
                key.into(),
                DecodedImage {
                    width: 2,
                    height: 2,
                    rgba: if bitmap == 2 {
                        vec![255; 16]
                    } else {
                        pixels.clone()
                    },
                },
            );
            api.graph_resources
                .insert(bitmap, RuntimeGraphResource::whole(key.into()));
            api.bitmap_formats.insert(bitmap, format);
            api.bitmap_dimensions.insert(bitmap, (2, 2));
        }
        api.effects.create_displacement_map(3, 2, 2);
        for (selector, args) in [
            (0x44, vec![2, 0, 0]),
            (0x45, vec![1, 3, -1, 0, 0]),
            (0x46, vec![1, 0, 0]),
            (0x48, vec![1, 0, 0, 2, 2]),
            (0x49, vec![1, 65536, 0]),
            (0x4A, vec![1, 0x7000, 0, 0, 1]),
        ] {
            api.configure_background_effect(selector, &args, &[1, 2])
                .unwrap();
            let object = *api.background_effects.keys().next().unwrap();
            let layer = &api.graph_layers[&object];
            // 414EA0 uses 0x10004 scale and truncates its 7-bit bilinear
            // weights, so the nominal identity stretch loses these low bits.
            let expected = if selector == 0x48 {
                vec![10, 0, 0, 255, 19, 0, 0, 255, 29, 0, 0, 255, 38, 0, 0, 255]
            } else {
                pixels.clone()
            };
            assert_eq!(
                api.graph_images[&layer.key].rgba, expected,
                "selector {selector:02X}"
            );
            assert_eq!(api.graph_object_properties[&object].opacity(), 1.0);
        }
    }
}
