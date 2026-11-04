use super::*;
use ethornell_vm::GraphApi;

impl RuntimeTraceApi {
    pub(super) fn dispatch_native_graph_resource(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        if group != 0x92 {
            return None;
        }
        if id <= 0x1f {
            return self.dispatch_system92_00_1f(call);
        }
        let stack = call.args_mut();

        let value = match id {
            0x8A => {
                // sub_486D20 -> sub_440F10 -> sub_42B7B0: fills the window
                // buffer (sub_40A700) with the value; a window without a
                // buffer is left alone.
                let value = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = self.graph92_require_window(object) {
                    return Some(Err(error));
                }
                self.clear_bitmap_text(object);
                self.graph_object_properties
                    .entry(object)
                    .or_default()
                    .properties
                    .insert(0x8a, (value, 0));
                tracing::debug!(object, value, "GraphSetTextObjectAuxiliaryValue");
                ethornell_vm::Value::None
            }
            // sub_487150 -> sub_433210: a glyph image for a character code
            // in 0xFF01..=0xFFFF (sub_433200). Bitmap -1 clears it; otherwise
            // the bitmap must exist and the size be nonzero. The image is the
            // (x, y, width, height) crop of the bitmap.
            0x98 => {
                let args = pop_args(stack, 6);
                let values = args.iter().map(value_to_i32).collect::<Vec<_>>();
                if let [height, width, y, x, source, code] = values.as_slice() {
                    let fail = |text: String| Some(Err(ethornell_vm::VmError::Runtime(text)));
                    if (*code as u32).wrapping_sub(0xFF01) > 0xFE {
                        return fail(format!("無効な文字コード [ {code} ] が指定されました"));
                    }
                    if *source == -1 {
                        self.graphic_resource_keys.remove(&(*code, 0));
                    } else {
                        let Some(key) = self.resolve_resource_key(*source).map(str::to_string)
                        else {
                            return fail(format!("無効なビットマップ [ {source} ] が指定されました"));
                        };
                        if *width == 0 || *height == 0 {
                            return fail(format!(
                                "無効なイメージサイズ [ {width} , {height} ] が指定されました"
                            ));
                        }
                        self.graphic_resource_keys.insert((*code, 0), key);
                    }
                    tracing::debug!(code, source, x, y, width, height, "GraphConfigureGlyphImage");
                }
                ethornell_vm::Value::None
            }
            0x9B => {
                unreachable!("Graph92:9B is handled by the VM output-pair pointer bridge")
            }
            // sub_487550 -> sub_4638B0 -> sub_438360: the face comes from a
            // font number (sub_469110; none clears it); sub_42EE90 runs with
            // zero allowed, so only a nonzero height outside 4..=200 (2) or
            // width outside 25..=200 (3) is refused, before anything is
            // stored.
            0x9D => {
                let args = pop_args(stack, 5);
                let italic = args.first().map(value_to_i32).unwrap_or_default();
                let field_565d34 = args.get(1).map(value_to_i32).unwrap_or_default();
                let width_percent = args.get(2).map(value_to_i32).unwrap_or_default();
                let height = args.get(3).map(value_to_i32).unwrap_or_default();
                let font = args.get(4).map(value_to_i32).unwrap_or_default();
                let status = if height != 0 && !(4..=200).contains(&height) {
                    2
                } else if width_percent != 0 && !(25..=200).contains(&width_percent) {
                    3
                } else {
                    let face = self.native_user.font_face(font).map(str::to_string);
                    let defaults = &mut self.graph_defaults.text_font_override;
                    defaults.face_name = face.filter(|name| !name.is_empty());
                    defaults.height = height;
                    defaults.creation_field_0 = width_percent;
                    defaults.creation_field_1 = field_565d34;
                    defaults.italic = italic != 0;
                    if (1..=200).contains(&height) {
                        self.text_state.font_size = height as f32;
                    }
                    0
                };
                tracing::debug!(font, height, width_percent, field_565d34, italic, status, "GraphConfigureFontOverride");
                ethornell_vm::Value::Int(status)
            }
            // The VM owns the destination record buffer and exact 128-byte
            // serialization. Reaching host dispatch would double-pop.
            0x9E => {
                unreachable!("Graph92:9E is handled by the VM fragment-record pointer bridge")
            }
            0x9F => {
                self.system92_text_render_override = pop_int_value(stack).unwrap_or_default();
                tracing::debug!(
                    value = self.system92_text_render_override,
                    "GraphSetTextRenderOverride"
                );
                ethornell_vm::Value::None
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn replace_native_bitmap_color(
        &mut self,
        bitmap: i32,
        needle: u32,
        replacement: u32,
    ) -> i32 {
        let Some(format) = self.bitmap_formats.get(&bitmap).copied() else {
            return 1;
        };
        let Some(bytes_per_pixel) =
            super::graph_bitmap_nodes::target_bitmap_bytes_per_pixel(format)
        else {
            return 2;
        };
        if bytes_per_pixel != 4 {
            return 2;
        }
        if !matches!(format, 1 | 2) {
            return 0;
        }
        let Some(mut image) = self.graph_bitmap_image(bitmap) else {
            return 1;
        };
        let unpack = |packed: u32| {
            [
                ((packed >> 16) & 0xff) as u8,
                ((packed >> 8) & 0xff) as u8,
                (packed & 0xff) as u8,
                (packed >> 24) as u8,
            ]
        };
        let needle_rgba = unpack(needle);
        let replacement_rgba = unpack(replacement);
        for pixel in image.rgba.as_chunks_mut::<4>().0 {
            if format == 1 {
                if pixel[..3] == needle_rgba[..3] {
                    pixel[..3].copy_from_slice(&replacement_rgba[..3]);
                    pixel[3] = 0xff;
                }
            } else if needle_rgba[3] == 0 {
                if pixel[..3] == needle_rgba[..3] {
                    pixel[..3].copy_from_slice(&replacement_rgba[..3]);
                }
            } else if *pixel == needle_rgba {
                pixel.copy_from_slice(&replacement_rgba);
            }
        }
        let key = format!("runtime:color-replace:{bitmap}");
        self.store_graph_image(key.clone(), image);
        self.graph_resources
            .insert(bitmap, RuntimeGraphResource::whole(key));
        0
    }
}
