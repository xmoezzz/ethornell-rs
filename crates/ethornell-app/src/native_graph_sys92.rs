use super::*;
use ethornell_vm::GraphApi;

impl RuntimeTraceApi {
    pub(super) fn dispatch_system92_00_1f(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x92 || call.id() > 0x1f {
            return None;
        }
        let id = call.id();
        let stack = call.args_mut();
        let value = match id {
            0x00 => {
                // sub_485FA0 -> sub_462430 -> sub_409AE0. pop_args keeps
                // native reverse-pop order, which configure_compact_wave_table
                // consumes directly.
                let args = pop_args(stack, 5);
                if !self.effects.configure_compact_wave_table(&args) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効な波紋番号 [ {} ] が指定されました",
                        args.last().map(value_to_i32).unwrap_or_default()
                    ))));
                }
                ethornell_vm::Value::None
            }
            0x01 => {
                // sub_486050 -> sub_462450 -> sub_409C50.
                let args = pop_args(stack, 7);
                if !self.effects.configure_wave_table(&args) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効な波紋番号 [ {} ] が指定されました",
                        args.last().map(value_to_i32).unwrap_or_default()
                    ))));
                }
                ethornell_vm::Value::None
            }
            0x10 => {
                // Script order: bitmap, mode, center_x, center_y, period.
                let period = pop_int_value(stack).unwrap_or_default();
                let center_y = pop_int_value(stack).unwrap_or_default();
                let center_x = pop_int_value(stack).unwrap_or_default();
                let mode = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                // sub_486120 -> sub_404E20 -> sub_404CA0.
                if let Err(error) = self.system92_vector_map_target(bitmap) {
                    return Some(Err(error));
                }
                if mode as u32 >= 2 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効な波紋タイプ [ {mode} ] が指定されました"
                    ))));
                }
                self.effects.generate_ripple_map(bitmap, mode, center_x, center_y, period);
                ethornell_vm::Value::None
            }
            0x11 => {
                // Script order: bitmap, mode. The target writes one of four
                // fixed axis vector/phase fields to a format-6 map.
                let mode = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                // sub_486220 -> sub_404FA0 -> sub_404E70.
                if let Err(error) = self.system92_vector_map_target(bitmap) {
                    return Some(Err(error));
                }
                if mode as u32 >= 4 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効な振動タイプ [ {mode} ] が指定されました"
                    ))));
                }
                self.effects.generate_axis_vector_map(bitmap, mode);
                ethornell_vm::Value::None
            }
            // BP-owned pointer bridge in the VM.
            0x12 | 0x16 | 0x17 => {
                unreachable!("System92 bitmap metadata/pixel pointer bridge is VM-owned")
            }
            0x13 => {
                let replacement = pop_int_value(stack).unwrap_or_default();
                let needle = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.replace_native_bitmap_color(
                    bitmap,
                    needle as u32,
                    replacement as u32,
                ))
            }
            0x14 => {
                // sub_486370 pops the synthesis spec, then the archive:
                // DCProcImageSynth parses the spec (sub_4514C0) and loads
                // its parts from the archive (CProcLoad, sub_439EA0);
                // DCProcPreloadBmp's last step (sub_4527C0) adds the result
                // to the preload store unless it is there already. A missing
                // archive or file ends the procedure quietly (tick 1 / -1).
                let spec = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                for (index, part) in spec.split('/').enumerate() {
                    if part.split(',').next().unwrap_or_default().is_empty() {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "指定された合成ファイル名 [ {spec} ] の第{}パートにファイル名が記述されていません",
                            crate::kanji_number(index as u32 + 1)
                        ))));
                    }
                }
                // sub_451010: each part file is taken from the DCCache or
                // stored in it once loaded.
                if self.graph_dc_cache.is_some() {
                    let now = self.clock_base_ms.wrapping_add(self.engine_time_ms as u32);
                    for part in spec.split('/') {
                        let name = part.split(',').next().unwrap_or_default().trim();
                        let hit = self.graph_dc_cache.as_mut().is_some_and(|cache| {
                            cache.lookup(Some(&archive), Some(name), now).is_some()
                        });
                        if !hit
                            && let Some(entry) = find_runtime_resource(&self.manager, &archive, name)
                            && let Ok(bytes) = self.manager.read_by_entry_raw(&entry)
                            && let Some(cache) = self.graph_dc_cache.as_mut()
                        {
                            cache.store(Some(&archive), Some(name), &bytes, now);
                        }
                    }
                }
                let loaded = self.load_graph_image_resource_target(None, &archive, &spec);
                let key = format!("{archive}:{spec}");
                if loaded && let Some(image) = self.graph_images.get(&key) {
                    let format = self.graph_image_formats.get(&key).copied().unwrap_or(2);
                    let bytes = crate::preload_image_buffer(image, format);
                    let archive_key = (!archive.starts_with('0')).then(|| archive.to_ascii_lowercase());
                    self.graph_blob_cache
                        .entry((archive_key, spec.to_ascii_lowercase()))
                        .or_insert(bytes);
                }
                tracing::debug!(archive, spec, loaded, "Graph92PreloadBitmap");
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::PreloadBitmap,
                    if loaded { 1 } else { -1 },
                );
                ethornell_vm::Value::None
            }
            0x15 => {
                // sub_450940 empties the whole store: Graph92:14 preloads
                // and Graph90:C6 registrations alike.
                self.graph_blob_cache.clear();
                ethornell_vm::Value::None
            }
            0x18 => {
                let source_descriptor = pop_int_value(stack).unwrap_or_default();
                let destination_descriptor = pop_int_value(stack).unwrap_or_default();
                let status =
                    self.convert_bitmap_to_alpha_mask(source_descriptor, destination_descriptor);
                // sub_4864A0.
                if status == 9 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "生成元に指定されたビットマップ [ {source_descriptor} ] は無効です"
                    ))));
                }
                if status == 10 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "格納先に指定されたビットマップ [ {destination_descriptor} ] は無効です"
                    ))));
                }
                ethornell_vm::Value::None
            }
            0x19 => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(self.invert_system92_alpha_bitmap(bitmap)))
            }
            0x1a => {
                let mut args=pop_args(stack,6).iter().map(value_to_i32).collect::<Vec<_>>();args.reverse();
                if let Err(error)=self.execute_bitmap_transform(0x92,id,&args) {return Some(Err(error));}
                ethornell_vm::Value::None
            }
            0x1c | 0x1d => {
                let mut source=pop_args(stack,if id==0x1c {10}else{11});source.reverse();
                let int=|i:usize|source.get(i).map(value_to_i32).unwrap_or_default();
                let (bitmap,x,y,font,size,width_percent)=(int(0),int(1),int(2),int(4),int(5),int(6));
                // sub_4866A0 / sub_486850: sub_4986D0, sub_498660, then
                // sub_403600 / sub_403680: bitmap, size, width.
                let fail=|text:String|Some(Err(ethornell_vm::VmError::Runtime(text)));
                if let Err(error)=target_checks::bitmap(bitmap) {return Some(Err(error));}
                if self.native_user.font_face(font).is_none() {return fail(format!("無効なフォント番号 [ {font} ] が指定されました"));}
                let Some(mut image)=self.graph_bitmap_image(bitmap) else{return fail(format!("指定されたビットマップ [ {bitmap} ] は存在しません"));};
                if !(4..=200).contains(&size) {return fail(format!("指定されたフォントサイズ [ {size} ] は無効です"));}
                if !(25..=200).contains(&width_percent) {return fail(format!("指定されたフォント幅 [ {width_percent} ] は無効です"));}
                let text=source.get(3).and_then(value_to_string).unwrap_or_default();
                let bytes=encoding_rs::SHIFT_JIS.encode(&text).0.into_owned();
                let (glyphs,result)=snapshot::layout_styled_bitmap_text(&bytes,[x,y],size as u32,width_percent,int(7)!=0,int(8)!=0,int(9) as u32,id==0x1d,if id==0x1d {int(10)}else{0},[image.width,image.height]);
                let format=self.bitmap_formats.get(&bitmap).copied().unwrap_or(2);
                for glyph in &glyphs {crate::bitmap_blend::blit_mode(&mut image,format,&glyph.cell,2,glyph.x,glyph.y,0,0);}
                if !glyphs.is_empty()&&self.replace_graph_bitmap_pixels(bitmap,image) {self.clear_bitmap_text(bitmap);}
                ethornell_vm::Value::Int(result)
            }
            0x1e => {
                // sub_485F10 pops packed_rgb, spacing, bold, size, font,
                // text, y, x, bitmap (script order reversed).
                let mut source = pop_args(stack, 9);
                source.reverse();
                let int = |index: usize| source.get(index).map(value_to_i32).unwrap_or_default();
                let (bitmap, x, y) = (int(0), int(1), int(2));
                let text = source.get(3).and_then(value_to_string).unwrap_or_default();
                let (font, size, bold, spacing, packed_rgb) =
                    (int(4), int(5), int(6), int(7), int(8));
                let fail = |text: String| Some(Err(ethornell_vm::VmError::Runtime(text)));
                // sub_486A10: sub_4986D0, sub_498660, then sub_4039E0.
                if let Err(error) = target_checks::bitmap(bitmap) {
                    return Some(Err(error));
                }
                if self.native_user.font_face(font).is_none() {
                    return fail(format!("無効なフォント番号 [ {font} ] が指定されました"));
                }
                let Some(mut image) = self
                    .query_bitmap_info(bitmap)
                    .and_then(|_| self.graph_bitmap_image(bitmap))
                else {
                    return fail(format!("指定されたビットマップ [ {bitmap} ] は存在しません"));
                };
                // sub_42F8D0 accepts heights 8..=200.
                if (size - 8) as u32 > 0xC0 {
                    return fail(format!("指定されたフォントサイズ [ {size} ] は無効です"));
                }
                let bytes = encoding_rs::SHIFT_JIS.encode(&text).0.into_owned();
                let (glyphs, advance) = snapshot::layout_direct_bitmap_text(
                    &bytes,
                    x,
                    y,
                    size as u32,
                    bold != 0,
                    spacing,
                    packed_rgb as u32,
                    image.width,
                    image.height,
                );
                if !glyphs.is_empty() {
                    let format = self.bitmap_formats.get(&bitmap).copied().unwrap_or(2);
                    for glyph in &glyphs {
                        crate::bitmap_blend::blit_mode(
                            &mut image, format, &glyph.cell, 2, glyph.x, glyph.y, 0, 0,
                        );
                    }
                    if self.replace_graph_bitmap_pixels(bitmap, image) {
                        self.clear_bitmap_text(bitmap);
                    }
                }
                ethornell_vm::Value::Int(advance)
            }
            0x1b => {
                // Not in the reference build; inferred from logwnd._bp, which
                // blends the result over the text at (-8, -8) with
                // transparency 64: see Graph92_1B_OutlineBitmapHalo.
                let width = pop_int_value(stack).unwrap_or_default();
                let source = pop_int_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                if let Some(image) = self.graph_bitmap_image(source) {
                    self.store_runtime_bitmap(destination, outline_halo(&image, width), 2);
                }
                ethornell_vm::Value::None
            }
            0x1f => {
                // sub_4020D0 allocates a temporary input buffer, searches the
                // target resource/filesystem path for the supplied name, then
                // parses an uncompressed BMP into the destination descriptor.
                let file = pop_string_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let loaded = self.load_graph_image_resource(bitmap, "", &file);
                ethornell_vm::Value::Int(if loaded { 0 } else { -1 })
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    /// A missing bitmap (0x80000001), then anything but a non-empty format-6
    /// vector & distance map (0x80000003).
    fn system92_vector_map_target(&mut self, bitmap: i32) -> ethornell_vm::VmResult<()> {
        let Some(info) = self.query_bitmap_info(bitmap) else {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定された出力先ビットマップ [ {bitmap} ] は存在しません"
            )));
        };
        if info.format != 6 || info.width == 0 || info.height == 0 {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定された出力先ビットマップ [ {bitmap} ] はベクトル＆ディスタンスマップではない、\n\n若しくは無効なピクセル数のベクトル＆ディスタンスマップです"
            )));
        }
        Ok(())
    }

    /// sub_440A80 for the 92:88..8E window calls.
    pub(super) fn graph92_require_window(&self, window: i32) -> ethornell_vm::VmResult<()> {
        if Self::is_window_surface_handle(window) && self.graph_surfaces.contains_key(&window) {
            Ok(())
        } else {
            Err(ethornell_vm::VmError::Runtime("無効なウィンドウハンドルが指定されました".into()))
        }
    }

    /// 92:89/8D: bitmap, blend mode and level validators, the window, a
    /// missing bitmap, then sub_40A520's format test against the 32-bit
    /// window buffer, made only when the bitmap overlaps it.
    pub(super) fn graph92_check_window_draw(
        &mut self,
        window: i32,
        bitmap: i32,
        x: i32,
        y: i32,
        mode: i32,
        level: i32,
    ) -> ethornell_vm::VmResult<()> {
        target_checks::bitmap(bitmap)?;
        target_checks::effect_mode(mode)?;
        target_checks::effect_level(level)?;
        // 1.622 resolves only CDspObjWindow handles here. Wonderful
        // Everyday's scrmsg._bp draws into plain bitmaps with the same
        // selector (an older build); those handles keep the bitmap path.
        if !Self::is_window_surface_handle(window) && self.graph_resources.contains_key(&window) {
            return Ok(());
        }
        self.graph92_require_window(window)?;
        let Some(info) = self.query_bitmap_info(bitmap) else {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定されたビットマップ [ {bitmap} ] は存在しません"
            )));
        };
        let (width, height) = self
            .graph_surfaces
            .get(&window)
            .map_or((0, 0), |surface| (surface.width as i32, surface.height as i32));
        let overlaps = x < width
            && y < height
            && x.saturating_add(info.width as i32) > 0
            && y.saturating_add(info.height as i32) > 0;
        if overlaps && !matches!(info.format, 1 | 2) {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定されたビットマップ [ {bitmap} ] とスクリーンのピクセルモードが一致しません"
            )));
        }
        Ok(())
    }

    fn invert_system92_alpha_bitmap(&mut self, bitmap: i32) -> bool {
        if self.bitmap_formats.get(&bitmap).copied() != Some(3) {
            return false;
        }
        let Some(mut image) = self.graph_bitmap_image(bitmap) else {
            return false;
        };
        // sub_40E690 inverts the coverage byte (red in app rasters).
        for pixel in image.rgba.as_chunks_mut::<4>().0 {
            let value = 255_u8.wrapping_sub(pixel[0]);
            pixel.copy_from_slice(&[value, value, value, value]);
        }
        let key = format!("runtime:bitmap:{bitmap}:alpha-invert");
        self.store_graph_image(key.clone(), image);
        self.graph_resources
            .insert(bitmap, RuntimeGraphResource::whole(key));
        true
    }
}

/// Margin of a Graph92:1B outline on every side of its source.
const OUTLINE_HALO_MARGIN: usize = 8;

/// White, with the alpha of the source's coverage summed over a
/// `(2 * width + 1)` square box (capped at opaque) where the source itself
/// is not covered, on a bitmap `OUTLINE_HALO_MARGIN` pixels larger on every
/// side: blended over the source at the margin's offset it outlines the
/// source's glyphs without covering them.
fn outline_halo(source: &DecodedImage, width: i32) -> DecodedImage {
    let radius = width.clamp(0, OUTLINE_HALO_MARGIN as i32) as usize;
    let (source_width, source_height) = (source.width as usize, source.height as usize);
    let margin = OUTLINE_HALO_MARGIN;
    let (out_width, out_height) = (source_width + 2 * margin, source_height + 2 * margin);
    let alpha_at = |x: isize, y: isize| -> u32 {
        if x < 0 || y < 0 || x >= source_width as isize || y >= source_height as isize {
            return 0;
        }
        u32::from(source.rgba[(y as usize * source_width + x as usize) * 4 + 3])
    };
    let mut rgba = vec![0u8; out_width * out_height * 4];
    for y in 0..out_height {
        for x in 0..out_width {
            let (sx, sy) = (x as isize - margin as isize, y as isize - margin as isize);
            let own = alpha_at(sx, sy);
            if radius == 0 || own == 255 {
                continue;
            }
            let r = radius as isize;
            let mut sum = 0u32;
            for dy in -r..=r {
                for dx in -r..=r {
                    sum += alpha_at(sx + dx, sy + dy);
                }
            }
            let at = (y * out_width + x) * 4;
            rgba[at..at + 3].copy_from_slice(&[255, 255, 255]);
            rgba[at + 3] = (sum.min(255) * (255 - own) / 255) as u8;
        }
    }
    DecodedImage {
        width: out_width as u32,
        height: out_height as u32,
        rgba,
    }
}
