use super::*;
use crate::native_graph::NativeBitmapOperation;
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
                // sub_4854A0 -> sub_461ED0 -> sub_409AF0. pop_args keeps
                // native reverse-pop order, which configure_compact_wave_table
                // consumes directly.
                let args = pop_args(stack, 5);
                if !self.effects.configure_compact_wave_table(&args) {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph92:00 invalid wave-table slot".to_string(),
                    )));
                }
                ethornell_vm::Value::None
            }
            0x01 => {
                // sub_485550 -> sub_461EF0 -> sub_409C60.
                let args = pop_args(stack, 7);
                if !self.effects.configure_wave_table(&args) {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph92:01 invalid wave-table slot".to_string(),
                    )));
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
                let Some(info) = self.query_bitmap_info(bitmap) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:10 invalid bitmap #{bitmap}"
                    ))));
                };
                if info.format != 6 || info.width == 0 || info.height == 0 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:10 bitmap #{bitmap} is not a non-empty format-6 vector map"
                    ))));
                }
                if !(0..=1).contains(&mode)
                    || !self
                        .effects
                        .generate_ripple_map(bitmap, mode, center_x, center_y, period)
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:10 invalid radial vector mode {mode}"
                    ))));
                }
                ethornell_vm::Value::None
            }
            0x11 => {
                // Script order: bitmap, mode. The target writes one of four
                // fixed axis vector/phase fields to a format-6 map.
                let mode = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let Some(info) = self.query_bitmap_info(bitmap) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:11 invalid bitmap #{bitmap}"
                    ))));
                };
                if info.format != 6 || info.width == 0 || info.height == 0 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:11 bitmap #{bitmap} is not a non-empty format-6 vector map"
                    ))));
                }
                if !(0..=3).contains(&mode) || !self.effects.generate_axis_vector_map(bitmap, mode)
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:11 invalid axis vector mode {mode}"
                    ))));
                }
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
                // sub_485870 converts both script strings, but DCProcPreloadBmp
                // receives only the first source argument. The second remains
                // diagnostic/context input in this target build.
                let mut source = pop_args(stack, 2);
                source.reverse();
                let resource = source.first().and_then(value_to_string).unwrap_or_default();
                let context = source.get(1).and_then(value_to_string).unwrap_or_default();
                let loaded = !resource.is_empty()
                    && self.load_graph_image_resource_target(None, &context, &resource);
                if loaded {
                    self.preloaded_bitmap_keys.insert(format!("{context}:{resource}"));
                }
                tracing::debug!(resource, context, loaded, "Graph92PreloadBitmap");
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::PreloadBitmap,
                    if loaded { 0 } else { 1 },
                );
                ethornell_vm::Value::None
            }
            0x15 => {
                // sub_4504F0 removes every pending DCProcPreloadBmp node.
                // Portable preloads complete synchronously, so there is no
                // pending node after dispatch; retain the exact queue-drain
                // boundary as an explicit no-op.
                tracing::debug!("Graph92CancelPendingBitmapPreloads");
                ethornell_vm::Value::None
            }
            0x18 => {
                let source_descriptor = pop_int_value(stack).unwrap_or_default();
                let destination_descriptor = pop_int_value(stack).unwrap_or_default();
                let status =
                    self.convert_bitmap_to_alpha_mask(source_descriptor, destination_descriptor);
                if status == 9 || status == 10 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph92:18 alpha conversion failed with target status {status}"
                    ))));
                }
                ethornell_vm::Value::None
            }
            0x19 => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(self.invert_system92_alpha_bitmap(bitmap)))
            }
            0x1a => {
                // Source order: temporary_alpha, offset_x, offset_y,
                // destination, optional_source_or_minus_one, blend_parameter.
                // The proprietary helper composes/copies alpha in the
                // intersected rectangle and clears destination areas outside
                // the translated overlap. Preserve all six arguments and the
                // target validation boundary in the portable operation log.
                let args = pop_args(stack, 6);
                self.apply_native_bitmap_operation(&args, NativeBitmapOperation::Composite);
                ethornell_vm::Value::None
            }
            0x1c | 0x1d => {
                let count = if id == 0x1c { 10 } else { 11 };
                let mut source = pop_args(stack, count);
                source.reverse();
                let destination = source.first().map(value_to_i32).unwrap_or_default();
                let x = source.get(1).map(value_to_i32).unwrap_or_default();
                let y = source.get(2).map(value_to_i32).unwrap_or_default();
                let text = source.get(3).and_then(value_to_string).unwrap_or_default();
                let size = source
                    .get(5)
                    .map(value_to_i32)
                    .filter(|value| (4..=256).contains(value))
                    .unwrap_or(self.text_state.font_size as i32);
                let spacing = source.get(7).map(value_to_i32).unwrap_or_default();
                let normalized = text_anim::normalize_message_text(&text);
                if destination > 0 && !normalized.is_empty() {
                    self.store_bitmap_text_run(
                        destination,
                        RuntimeTextNode {
                            text: normalized.clone(),
                            enabled: true,
                            owner_object: None,
                            screen_attached: false,
                            target_surface: None,
                            x: x as f32,
                            y: y as f32,
                            size: size as f32,
                            line_height: size as f32 * 1.35,
                            formatted_layout: false,
                            color: [1.0, 1.0, 1.0, 1.0],
                            z: destination,
                            ruby_spans: Vec::new(),
                            style_spans: Vec::new(),
                        },
                    );
                }
                let advance =
                    snapshot::measure_text_advance(&normalized, size as f32, spacing as f32);
                let result = if id == 0x1c {
                    advance
                } else {
                    let width = self
                        .query_bitmap_info(destination)
                        .map(|info| info.width as i32)
                        .unwrap_or_default();
                    if width <= 0 {
                        0
                    } else {
                        ((x.saturating_add(advance).saturating_sub(1)) / width + 1).max(1)
                    }
                };
                ethornell_vm::Value::Int(result)
            }
            0x1e => {
                // Script order: destination, x, y, text, font, size, style,
                // spacing, packed_rgb. This is the direct bitmap text path.
                let mut source = pop_args(stack, 9);
                source.reverse();
                let destination = source.first().map(value_to_i32).unwrap_or_default();
                let x = source.get(1).map(value_to_i32).unwrap_or_default();
                let y = source.get(2).map(value_to_i32).unwrap_or_default();
                let text = source.get(3).and_then(value_to_string).unwrap_or_default();
                let size = source
                    .get(5)
                    .map(value_to_i32)
                    .filter(|value| (4..=256).contains(value))
                    .unwrap_or(self.text_state.font_size as i32);
                let spacing = source.get(7).map(value_to_i32).unwrap_or_default();
                let packed_rgb = source.get(8).map(value_to_i32).unwrap_or(0x00ff_ffff);
                let color = [
                    ((packed_rgb >> 16) & 0xff) as f32 / 255.0,
                    ((packed_rgb >> 8) & 0xff) as f32 / 255.0,
                    (packed_rgb & 0xff) as f32 / 255.0,
                    1.0,
                ];
                // sub_485F10 -> sub_4039E0 -> sub_403840 writes glyph
                // coverage straight into the selected bitmap descriptor and
                // returns the accumulated advance. The string is not markup.
                let mut advance = 0;
                if destination > 0 && !text.is_empty() {
                    if let Some(mut image) = self.graph_bitmap_image(destination) {
                        advance = snapshot::rasterize_direct_bitmap_text(
                            &mut image,
                            &text,
                            x,
                            y,
                            size as f32,
                            spacing as f32,
                            color,
                        );
                        if self.replace_graph_bitmap_pixels(destination, image) {
                            self.clear_bitmap_text(destination);
                        }
                    }
                }
                ethornell_vm::Value::Int(advance)
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

    fn invert_system92_alpha_bitmap(&mut self, bitmap: i32) -> bool {
        if self.bitmap_formats.get(&bitmap).copied() != Some(3) {
            return false;
        }
        let Some(mut image) = self.graph_bitmap_image(bitmap) else {
            return false;
        };
        for pixel in image.rgba.as_chunks_mut::<4>().0 {
            let value = 255_u8.wrapping_sub(pixel[3]);
            pixel.copy_from_slice(&[value, value, value, value]);
        }
        let key = format!("runtime:bitmap:{bitmap}:alpha-invert");
        self.store_graph_image(key.clone(), image);
        self.graph_resources
            .insert(bitmap, RuntimeGraphResource::whole(key));
        true
    }
}
