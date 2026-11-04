//! Immediate bitmap operations that formerly guessed handles from arguments.
//! BGI 1.622 90:19..1D and 92:1A, native_all.c/.asm 4027E0..402EA0,403C20.
use super::*;
use ethornell_vm::GraphApi;
impl RuntimeTraceApi {
    pub(super) fn execute_bitmap_transform(
        &mut self,
        group: u8,
        id: u16,
        a: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let fail = |reason: &str| {
            ethornell_vm::VmError::Runtime(format!("{group:02X}:{id:02X}: {reason}"))
        };
        let image = |api: &Self, id| {
            api.graph_bitmap_image(id)
                .ok_or_else(|| fail("bitmap is absent"))
        };
        let format = |api: &Self, id| api.bitmap_formats.get(&id).copied().unwrap_or(2);
        if group == 0x92 {
            // sub_486560 -> sub_403C20: first source (0x8000000A), second
            // source unless -1 (0x8000000B), the screen-sized format-3
            // destination (0x80000009), then sub_418BE0's format match
            // (0x8000000C).
            let target = |text: String| ethornell_vm::VmError::Runtime(text);
            let source_error =
                |bitmap: i32| target(format!("参照元に指定されたビットマップ [ {bitmap} ] は無効です"));
            let primary = self.graph_bitmap_image(a[3]).ok_or_else(|| source_error(a[3]))?;
            let pf = format(self, a[3]);
            let other = if a[4] == -1 {
                None
            } else {
                Some(self.graph_bitmap_image(a[4]).ok_or_else(|| source_error(a[4]))?)
            };
            let (width, height) = snapshot::runtime_frame_size(self);
            if a[0] as u32 >= 0x4000 {
                return Err(target(format!("生成先に指定されたビットマップ [ {} ] は無効です", a[0])));
            }
            if other.is_some() && format(self, a[4]) != pf {
                return Err(target(format!(
                    "参照元に指定されたビットマップ [ {} ] 及び [ {} ] のピクセルモードが一致しません",
                    a[3], a[4]
                )));
            }
            let mut out = DecodedImage {
                width,
                height,
                rgba: vec![0; width as usize * height as usize * 4],
            };
            if pf == 2 {
                for y in 0..height {
                    for x in 0..width {
                        let (sx, sy) = (x as i64 - a[1] as i64, y as i64 - a[2] as i64);
                        if sx < 0
                            || sy < 0
                            || sx >= primary.width as i64
                            || sy >= primary.height as i64
                        {
                            continue;
                        }
                        if other
                            .as_ref()
                            .is_some_and(|o| sx >= o.width as i64 || sy >= o.height as i64)
                        {
                            continue;
                        }
                        let alpha = primary.rgba
                            [(sy as usize * primary.width as usize + sx as usize) * 4 + 3];
                        let alpha = if let Some(other) = &other {
                            let b = other.rgba
                                [(sy as usize * other.width as usize + sx as usize) * 4 + 3];
                            let product = (a[5] as u16)
                                .wrapping_mul((i32::from(b) - i32::from(alpha)) as u16);
                            alpha.wrapping_add((product >> 8) as u8)
                        } else {
                            alpha
                        };
                        out.rgba[(y as usize * width as usize + x as usize) * 4..][..4]
                            .copy_from_slice(&[alpha, alpha, alpha, 255]);
                    }
                }
            }
            self.store_runtime_bitmap(a[0], out, 3);
            return Ok(());
        }
        let destination = a[0];
        let err = |message: String| ethornell_vm::VmError::Runtime(message);
        let missing_destination = || err(format!("格納先として指定されたビットマップ [ {destination} ] は無効です"));
        let missing_source = |id: i32| err(format!("参照元として指定されたビットマップ [ {id} ] は無効です"));
        if id == 0x19 {
            // sub_47A8A0: bitmap checks and the level first; then
            // sub_4027E0's codes 1..6.
            crate::target_checks::bitmap(destination)?;
            crate::target_checks::bitmap(a[3])?;
            crate::target_checks::effect_level(a[6])?;
            if self.graph_bitmap_image(destination).is_none() {
                return Err(err(format!("指定された出力先ビットマップ [ {destination} ] は存在しません")));
            }
            if self.graph_bitmap_image(a[3]).is_none() {
                return Err(err(format!("指定された出力元ビットマップ [ {} ] は存在しません", a[3])));
            }
        } else if self.graph_bitmap_image(destination).is_none() {
            return Err(missing_destination());
        }
        let mut out = image(self, destination)?;
        let df = format(self, destination);
        match id {
            0x19 => {
                let source = image(self, a[3])?;
                let sf = format(self, a[3]);
                let mask = self.graph_bitmap_image(a[4]);
                if df != sf {
                    return Err(err(format!(
                        "出力先ビットマップ [ {destination} ] と出力元ビットマップ [ {} ] のピクセルモードには互換性がありません",
                        a[3]
                    )));
                }
                if let Some(mask) = &mask {
                    if format(self, a[4]) != 3 {
                        return Err(err(format!("指定されたビットマップ [ {} ] はグレイスケールではありません", a[4])));
                    }
                    if (mask.width, mask.height) != (source.width, source.height) {
                        return Err(err(format!("指定された参照元グレイスケール [ {} ] はサイズが無効です", a[4])));
                    }
                }
                if !matches!(df, 1 | 2) || mask.is_some() && df != 1 {
                    return Ok(());
                }
                let left = a[1].max(0);
                let top = a[2].max(0);
                let right = (a[1] as i64 + source.width as i64).min(out.width as i64);
                let bottom = (a[2] as i64 + source.height as i64).min(out.height as i64);
                for y in top as i64..bottom {
                    for x in left as i64..right {
                        let sx = (x - a[1] as i64) as usize;
                        let sy = (y - a[2] as i64) as usize;
                        let so = (sy * source.width as usize + sx) * 4;
                        let dst = (y as usize * out.width as usize + x as usize) * 4;
                        let p: [u8; 4] = out.rgba[dst..dst + 4].try_into().unwrap();
                        let s: [u8; 4] = source.rgba[so..so + 4].try_into().unwrap();
                        let color = if let Some(mask) = &mask {
                            let weight = backf_mask_weight(mask.rgba[so], a[5], a[6], 0) as i32;
                            let mut pixel = [0; 4];
                            for c in 0..4 {
                                let d = p[c] as i32;
                                let s = s[c] as i32;
                                pixel[c] = if weight >= 256 {
                                    s as u8
                                } else {
                                    (d + (((s - d) * (weight >> 1)) as i16 as i32 >> 7))
                                        .clamp(0, 255) as u8
                                };
                            }
                            pixel
                        } else {
                            crate::bitmap_blend::crossfade_pixel(p, s, df, a[6])
                        };
                        out.rgba[dst..dst + 4].copy_from_slice(&color);
                    }
                }
            }
            0x1A => {
                // sub_402970: destination, source, maps (1, 2, 4, 6), then
                // sub_412AB0: level (8), first map (5), second map (7),
                // formats (3).
                let source = self.graph_bitmap_image(a[1]).ok_or_else(|| missing_source(a[1]))?;
                let first = self.query_bitmap_info(a[2]).ok_or_else(|| {
                    err(format!("第一ベクトルマップとして指定されたビットマップ [ {} ] は無効です", a[2]))
                })?;
                let second = if a[3] != -1 {
                    Some(self.query_bitmap_info(a[3]).ok_or_else(|| {
                        err(format!("第二ベクトルマップとして指定されたビットマップ [ {} ] は無効です", a[3]))
                    })?)
                } else {
                    None
                };
                if a[4] as u32 > 256 {
                    return Err(err(format!("無効なエフェクトレベル [ {} ] が指定されました", a[4])));
                }
                if first.format != 4 || first.width < out.width || first.height < out.height {
                    return Err(err(format!(
                        "第一ベクトルマップとして指定されたビットマップ [ {} ] はベクトルマップではない、\n\n若しくは格納先ビットマップ [ {destination} ] とサイズが一致していません",
                        a[2]
                    )));
                }
                if let Some(second) = second
                    && (second.format != 4 || (second.width, second.height) != (first.width, first.height))
                {
                    return Err(err(format!(
                        "第二ベクトルマップとして指定されたビットマップ [ {} ] はベクトルマップではない、\n\n若しくは格納先ビットマップ [ {destination} ] とサイズが一致していません",
                        a[3]
                    )));
                }
                if df != format(self, a[1]) {
                    return Err(err(format!(
                        "格納先ビットマップ [ {destination} ] と参照元ビットマップ [ {} ] のピクセルモードが一致していません",
                        a[1]
                    )));
                }
                if !matches!(df, 1 | 2) {
                    return Ok(());
                }
                out = self
                    .effects
                    .warp_displacement_into(
                        &source,
                        [out.width, out.height],
                        a[2],
                        (a[3] != -1).then_some(a[3]),
                        a[4],
                        a[5] != 0,
                    )
                    .ok_or_else(|| fail("displacement map pixels are absent"))?;
            }
            0x1B => {
                let source = self.graph_bitmap_image(a[1]).ok_or_else(|| missing_source(a[1]))?;
                if df != format(self, a[1])
                    || (source.width, source.height) != (out.width, out.height)
                {
                    return Err(err(format!(
                        "格納先ビットマップ [ {destination} ] と参照元ビットマップ [ {} ] のサイズ又はピクセルモードが一致していません",
                        a[1]
                    )));
                }
                if a[2] as u32 > 1 {
                    return Err(err(format!("無効なグラデーションタイプ [ {} ] が指定されました", a[2])));
                }
                if a[3] as u32 > 256 {
                    return Err(err(format!("無効なエフェクトレベル [ {} ] が指定されました", a[3])));
                }
                if !matches!(df, 1 | 2) {
                    return Ok(());
                }
                out = crate::native_background_effect::native_horizontal_blur(
                    &source,
                    a[3],
                    a[2] != 0,
                );
            }
            0x1C => {
                // sub_402C90: 1, 2, output range (5), reference range (6),
                // then the formats (8) in sub_414DF0.
                let source = self.graph_bitmap_image(a[5]).ok_or_else(|| missing_source(a[5]))?;
                if (a[3] as u32) < 2 || (a[4] as u32) < 2 {
                    return Err(err(format!("無効な出力範囲 [ {}, {} ] が指定されました", a[3], a[4])));
                }
                if (a[8] as u32) < 2 || (a[9] as u32) < 2 {
                    return Err(err(format!("無効な参照範囲 [ {}, {} ] が指定されました", a[8], a[9])));
                }
                if df != format(self, a[5]) {
                    return Err(err(format!(
                        "格納先ビットマップ [ {destination} ] と参照元ビットマップ [ {} ] のピクセルモードが一致していません",
                        a[5]
                    )));
                }
                if !matches!(df, 1 | 2) {
                    return Ok(());
                }
                let left = a[1].max(0) as i64;
                let top = a[2].max(0) as i64;
                let right = (a[1] as i64 + a[3] as u32 as i64).min(out.width as i64);
                let bottom = (a[2] as i64 + a[4] as u32 as i64).min(out.height as i64);
                if right <= left || bottom <= top {
                    return Ok(());
                }
                let scale = [
                    (a[3] as u32 as f64 * 65540.0 / a[8] as u32 as f64) as u64 as u32,
                    (a[4] as u32 as f64 * 65540.0 / a[9] as u32 as f64) as u64 as u32,
                ];
                if scale.contains(&0) {
                    return Ok(());
                }
                let size = [(right - left) as u32, (bottom - top) as u32];
                let center = [
                    a[6].wrapping_shl(16)
                        .wrapping_add((a[8] as u32).wrapping_shl(15) as i32),
                    a[7].wrapping_shl(16)
                        .wrapping_add((a[9] as u32).wrapping_shl(15) as i32),
                ];
                let start = std::array::from_fn::<_, 2, _>(|i| {
                    let inv = 65536.0 / scale[i] as f64;
                    (center[i] as f64 - size[i] as f64 * 32768.0 * inv
                        + if scale[i] < 65536 {
                            (inv * 0.5 - 0.5) * 65536.0
                        } else {
                            0.0
                        }) as i64 as i32
                });
                let step = scale.map(|s| (65536.0 / s as f64 * 65536.0) as i32);
                for y in 0..size[1] {
                    for x in 0..size[0] {
                        let sx = start[0].wrapping_add(step[0].wrapping_mul(x as i32));
                        let sy = start[1].wrapping_add(step[1].wrapping_mul(y as i32));
                        let p = crate::native_background_effect::sample_background_pixel(
                            &source,
                            sx >> 16,
                            sy >> 16,
                            ((sx as u32 >> 9) & 127) as i32,
                            ((sy as u32 >> 9) & 127) as i32,
                            128,
                        );
                        let at = (((top as u32 + y) * out.width + left as u32 + x) * 4) as usize;
                        out.rgba[at..at + 4].copy_from_slice(&p);
                    }
                }
            }
            0x1D => {
                let source = self.graph_bitmap_image(a[1]).ok_or_else(|| missing_source(a[1]))?;
                if df != format(self, a[1]) {
                    return Err(err(format!(
                        "ビットマップ [ {destination} ] とビットマップ [ {} ] には互換性がありません",
                        a[1]
                    )));
                }
                if a[2] == 0 {
                    return Err(err(format!("無効な伸縮倍率 [ {} ] が指定されました", a[2])));
                }
                if !matches!(df, 1 | 2) {
                    return Ok(());
                }
                out = crate::native_background_effect::native_rotate_background(
                    &source,
                    [out.width, out.height],
                    a[2] as u32,
                    a[3],
                );
            }
            _ => unreachable!(),
        }
        self.replace_graph_bitmap_pixels(destination, out);
        self.clear_bitmap_text(destination);
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn immediate_bitmap_paths_use_positional_arguments_and_native_formats() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 2;
        api.screen_height = 2;
        let put = |api: &mut RuntimeTraceApi, id, format, pixels: Vec<u8>| {
            api.store_runtime_bitmap(
                id,
                DecodedImage {
                    width: 2,
                    height: 2,
                    rgba: pixels,
                },
                format,
            )
        };
        put(&mut api, 100, 1, [20, 0, 0, 255].repeat(4));
        put(&mut api, 101, 1, [100, 0, 0, 255].repeat(4));
        put(&mut api, 102, 4, vec![0; 16]);
        api.effects.create_displacement_map(102, 2, 2);
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
        call(&mut api, 0x90, 0x19, &[100, 1, 0, 101, -1, 0, 128]);
        let out = api.graph_bitmap_image(100).unwrap();
        assert_eq!((out.rgba[0], out.rgba[4]), (20, 60));
        call(&mut api, 0x90, 0x1A, &[100, 101, 102, -1, 256, 0]);
        assert_eq!(
            api.graph_bitmap_image(100).unwrap().rgba,
            [100, 0, 0, 255].repeat(4)
        );
        call(&mut api, 0x90, 0x1B, &[100, 101, 0, 1]);
        assert_eq!(api.graph_bitmap_image(100).unwrap().rgba[0], 66);
        call(&mut api, 0x90, 0x1C, &[100, 0, 0, 2, 2, 101, 0, 0, 2, 2]);
        assert_eq!(api.graph_bitmap_image(100).unwrap().rgba[0], 100);
        call(&mut api, 0x90, 0x1D, &[100, 101, 65536, 0]);
        assert_eq!(
            api.graph_bitmap_image(100).unwrap().rgba,
            [100, 0, 0, 255].repeat(4)
        );
        put(
            &mut api,
            105,
            2,
            vec![1, 2, 3, 0, 1, 2, 3, 64, 1, 2, 3, 128, 1, 2, 3, 255],
        );
        call(&mut api, 0x92, 0x1A, &[104, 1, 0, 105, -1, 0]);
        let alpha = api.graph_bitmap_image(104).unwrap();
        assert_eq!(api.bitmap_formats[&104], 3);
        assert_eq!(
            alpha
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .map(|p| p[0])
                .collect::<Vec<_>>(),
            [0, 0, 0, 128]
        );
        assert!(
            api.execute_bitmap_transform(0x90, 0x1B, &[100, 101, 2, 0])
                .is_err()
        );
    }
}
