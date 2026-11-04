use super::*;
use crate::native_landscape::{
    LANDSCAPE_HANDLE, LANDSCAPE_NO_COLUMNS, LANDSCAPE_NO_MAP, landscape_error,
};

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(super) struct Graph91FontTransform {
    pub scale_x: i32,
    pub scale_y: i32,
    pub offset_x: i32,
    pub offset_y: i32,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(super) struct Graph91ObjectTransformState {
    /// CDspObj+0x0c, tested inversely by sub_41AE30.
    pub suppressed: bool,
    /// CDspObj+0x4c/+0x50/+0x54 written by vtable slot +60.
    pub fixed_position: [i32; 3],
    /// CDspObj+0x5c/+0x60/+0x64 written by vtable slot +68.
    pub primary_vector: [i32; 3],
    /// CDspObj+0x6c/+0x70/+0x74 written by sub_41B580.
    pub secondary_vector: [i32; 3],
    /// Local X/Y supplied when the object is attached as a slave.
    pub attachment_offset: [i32; 2],
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) struct Graph91MultiLayerRecord {
    pub configured: bool,
    pub enabled: bool,
    pub x_16_16: i32,
    pub y_16_16: i32,
    pub blend_mode: i32,
    pub blend_parameter: i32,
    pub bitmap: i32,
    pub bitmap_generation: u64,
    pub source_x: i32,
    pub source_y: i32,
    pub rotation: i32,
    pub scale_x: i32,
    pub scale_y: i32,
    pub auxiliary_a: i32,
    pub auxiliary_b: i32,
    pub source_delta_x: i32,
    pub source_delta_y: i32,
    pub rotation_delta: i32,
    pub scale_delta_x: i32,
    pub scale_delta_y: i32,
    pub transparency: i32,
}

impl Default for Graph91MultiLayerRecord {
    fn default() -> Self {
        Self {
            configured: false,
            enabled: false,
            x_16_16: 0,
            y_16_16: 0,
            blend_mode: 0,
            blend_parameter: 0,
            bitmap: -1,
            bitmap_generation: 0,
            source_x: 0,
            source_y: 0,
            rotation: 0,
            scale_x: 0x1_0000,
            scale_y: 0x1_0000,
            auxiliary_a: 0,
            auxiliary_b: 0,
            source_delta_x: 0,
            source_delta_y: 0,
            rotation_delta: 0,
            scale_delta_x: 0,
            scale_delta_y: 0,
            transparency: 0,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub(super) struct Graph91MultiLayerBackgroundState {
    pub active: bool,
    pub object_x_16_16: i32,
    pub object_y_16_16: i32,
    pub selected_layer: usize,
    pub layers: [Graph91MultiLayerRecord; 8],
}

const WINDOW_HANDLE: &str = "無効なウィンドウハンドルが指定されました";
const GRAPH91_EFFECTOR_TAG: u32 = 0x9100_0000;
const GRAPH91_EFFECTOR_SLOTS: u32 = 8;
const GRAPH91_LANDSCAPE_TAG: u32 = 0xA100_0000;
const GRAPH91_LANDSCAPE_SLOTS: u32 = 4;

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct Graph91EffectorState {
    pub enabled: bool,
    pub mode: i32,
    pub vector_maps: [i32; 2],
    pub parameters: [i32; 9],
    pub priority: i32,
}

impl Default for Graph91EffectorState {
    fn default() -> Self {
        Self {
            enabled: true,
            mode: -1,
            vector_maps: [-1, -1],
            parameters: [0; 9],
            priority: 0,
        }
    }
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub(super) struct Graph91LandscapeCell {
    pub column_index: i32,
    pub silhouette_type: i32,
    pub silhouette_level: i32,
    pub silhouette_value: i32,
    pub guides: [Option<(i32, i32)>; 4],
    pub derived_value: u16,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct Graph91LandscapeState {
    pub cell_width: i32,
    pub row_step: i32,
    pub column_step: i32,
    pub baseline: i32,
    pub priority_row_factor: i32,
    pub priority_frame_factor: i32,
    pub enabled: bool,
    pub x: i32,
    pub y: i32,
    pub blend_mode: i32,
    pub alpha: i32,
    pub priority: i32,
    pub source_bitmap: i32,
    pub part_spacing: i32,
    pub parts: Vec<[i32; 5]>,
    pub columns: Vec<Vec<i32>>,
    pub map_width: i32,
    pub map_height: i32,
    pub cells: Vec<Graph91LandscapeCell>,
    pub guide_bitmap: i32,
    pub guides: Vec<[i32; 5]>,
    /// CDspObj+0x118 (sub_41C3F0): the part/pillar index reported by a
    /// failed 91:78/7A, read back through vtable+0x60 query -1.
    pub error_detail: i32,
}

impl Graph91LandscapeState {
    fn new(
        cell_width: i32,
        row_step: i32,
        column_step: i32,
        baseline: i32,
        priority_row_factor: i32,
        priority_frame_factor: i32,
    ) -> Self {
        let cell_width = if cell_width == 0 || cell_width & 1 != 0 {
            64
        } else {
            cell_width
        };
        let row_step = if row_step == 0 { 16 } else { row_step };
        let column_step = if column_step == 0 { 16 } else { column_step };
        let baseline = if baseline == 0 {
            35_i32.wrapping_mul(row_step)
        } else {
            baseline
        };
        Self {
            cell_width,
            row_step,
            column_step,
            baseline,
            priority_row_factor,
            priority_frame_factor,
            enabled: true,
            x: 0,
            y: 0,
            blend_mode: 0,
            alpha: 256,
            priority: 0,
            source_bitmap: -1,
            part_spacing: 0,
            parts: Vec::new(),
            columns: Vec::new(),
            map_width: 0,
            map_height: 0,
            cells: Vec::new(),
            guide_bitmap: -1,
            guides: Vec::new(),
            error_detail: 0,
        }
    }

    pub(super) fn cell_index(&self, line: i32, column: i32) -> Option<usize> {
        if line < 0 || column < 0 || line >= self.map_height || column >= self.map_width {
            return None;
        }
        Some(line as usize * self.map_width as usize + column as usize)
    }

    fn rebuild_derived_value(&self, line: i32, column: i32) -> u16 {
        self.baseline
            .saturating_add(line.saturating_mul(self.priority_row_factor))
            .saturating_add(column.saturating_mul(self.priority_frame_factor))
            .clamp(0, u16::MAX as i32) as u16
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(super) struct Graph91FontConfig {
    pub font_number: i32,
    pub size: i32,
    pub width_percent: i32,
    pub bold: i32,
    /// Target font record field +0x38. Its consumer is retained even though
    /// the original public name is not present in the executable.
    pub target_field_56: i32,
    /// Target font record field +0x3c.
    pub target_field_60: i32,
}

impl RuntimeTraceApi {
    pub(super) fn dispatch_system91_03_1f(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x91 || call.id() > 0x1f {
            return None;
        }
        let id = call.id();
        let stack = call.args_mut();
        let value = match id {
            // The VM owns the BP source pointer and performs the exact copy.
            0x03 => {
                unreachable!("0x91:03 is handled by the VM binary-cache pointer bridge")
            }
            0x06 => {
                // sub_4806D0 pops Y first and X second.
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                self.graph_global_offset = (x as f32, y as f32);
                // sub_443280 marks the screen object dirty: a full redraw.
                self.graph_redraw_requested = Some(true);
                tracing::info!(x, y, "GraphSetGlobalDisplayOffset");
                self.trace_graph(format!("global display offset=({x},{y}) (91:06)"));
                ethornell_vm::Value::None
            }
            0x0b => {
                self.graph91_script_bitmap_context_binding_enabled =
                    pop_int_value(stack).unwrap_or_default() != 0;
                ethornell_vm::Value::None
            }
            0x0c => {
                let mode = pop_int_value(stack).unwrap_or_default();
                if (0..=1).contains(&mode) {
                    self.graph91_glyph_coverage_mode = mode;
                    ethornell_vm::Value::Int(1)
                } else {
                    ethornell_vm::Value::Int(0)
                }
            }
            0x0d => {
                self.graph91_font_pitch_detection_enabled =
                    pop_int_value(stack).unwrap_or_default() != 0;
                ethornell_vm::Value::None
            }
            0x0e => {
                // Script order: name, scale_x, scale_y, offset_x, offset_y.
                let offset_y = pop_int_value(stack).unwrap_or_default();
                let offset_x = pop_int_value(stack).unwrap_or_default();
                let scale_y = pop_int_value(stack).unwrap_or_default();
                let scale_x = pop_int_value(stack).unwrap_or_default();
                let name = pop_string_value(stack).unwrap_or_default();
                // sub_42EF10: all zero is accepted; else the X scale is 0 or
                // 0x10000..=0x20000, the Y scale 0x10000..=0x20000 (0x80000005)
                // and each offset at most 65536 - 2^32 / scale (0x80000006).
                if !(scale_x == 0 && scale_y == 0 && offset_x == 0 && offset_y == 0) {
                    let in_range = |value: i32| (0x1_0000..=0x2_0000).contains(&value);
                    if !(scale_x == 0 || in_range(scale_x)) || !in_range(scale_y) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "無効な倍率 [ 0x{:05X} , 0x{:05X} ] が指定されました",
                            scale_x as u32, scale_y as u32
                        ))));
                    }
                    let x_scale = if scale_x == 0 { scale_y } else { scale_x };
                    let limit = |scale: i32| (65536.0 - 4294967296.0 / f64::from(scale)) as i32;
                    if offset_x > limit(x_scale) || offset_y > limit(scale_y) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "無効な座標 [ 0x{:05X} , 0x{:05X} ] が指定されました",
                            offset_x as u32, offset_y as u32
                        ))));
                    }
                }
                {
                    self.graph91_font_transforms.insert(
                        name.to_ascii_lowercase(),
                        Graph91FontTransform {
                            scale_x,
                            scale_y,
                            offset_x,
                            offset_y,
                        },
                    );
                }
                ethornell_vm::Value::None
            }
            0x0f => {
                let mut args = pop_args(stack, 6)
                    .into_iter()
                    .map(|value| value_to_i32(&value))
                    .collect::<Vec<_>>();
                args.reverse();
                // sub_481360 -> sub_462310: unknown face (0x80000004), then
                // sub_42E1C0's size (2) and width (3) checks.
                if let [font_number, size, width_percent, ..] = args.as_slice() {
                    let fatal =
                        |message: String| Some(Err(ethornell_vm::VmError::Runtime(message)));
                    if !self.native_user.font_name_known(*font_number) {
                        return fatal(format!(
                            "無効なフォント番号 [ {font_number} ] が指定されました"
                        ));
                    }
                    if !(4..=200).contains(size) {
                        return fatal(format!("無効なフォントサイズ [ {size} ] が指定されました"));
                    }
                    if !(25..=200).contains(width_percent) {
                        return fatal(format!(
                            "無効なフォント幅 [ {width_percent} ] が指定されました"
                        ));
                    }
                }
                if let [font_number, size, width_percent, bold, field_56, field_60] =
                    args.as_slice()
                    && (4..=200).contains(size)
                    && (25..=200).contains(width_percent)
                {
                    self.graph91_fonts.insert(
                        *font_number,
                        Graph91FontConfig {
                            font_number: *font_number,
                            size: *size,
                            width_percent: *width_percent,
                            bold: *bold,
                            target_field_56: *field_56,
                            target_field_60: *field_60,
                        },
                    );
                    self.text_state.font_size = *size as f32;
                }
                ethornell_vm::Value::None
            }
            0x10..=0x17 => {
                let count = match id {
                    0x10 => 5,
                    0x11 => 2,
                    0x12 => 6,
                    0x13..=0x15 => 5,
                    0x16 => 7,
                    0x17 => 6,
                    _ => unreachable!(),
                };
                let args = pop_args(stack, count);
                // sub_481470..sub_481B20: the destination must exist (1) and
                // be a non-empty vector map, format 4 (3); then each
                // generator's own zero checks.
                let source: Vec<i32> = args.iter().rev().map(value_to_i32).collect();
                let destination = source[0];
                let fatal = |message: String| Some(Err(ethornell_vm::VmError::Runtime(message)));
                let Some(info) = ethornell_vm::GraphApi::query_bitmap_info(self, destination)
                else {
                    return fatal(format!(
                        "指定された出力先ビットマップ [ {destination} ] は存在しません"
                    ));
                };
                if info.format != 4 || info.width == 0 || info.height == 0 {
                    return fatal(format!(
                        "指定された出力先ビットマップ [ {destination} ] はベクトルマップではない、\n\n若しくは無効なピクセル数のベクトルマップです"
                    ));
                }
                match id {
                    0x10 if source[3] == 0 || source[4] == 0 => {
                        return fatal(format!(
                            "無効な伸縮範囲 [ {} , {} ] が指定されました",
                            source[3], source[4]
                        ));
                    }
                    0x12 if source[3] == 0 => {
                        return fatal(format!(
                            "無効な波紋の周期 [ {} ] が指定されました",
                            source[3]
                        ));
                    }
                    0x14 if source[3] == 0 || source[4] == 0 => {
                        return fatal(format!(
                            "無効な湾曲角度 [ {} ] 、或いは半径 [ {} ] が指定されました",
                            source[3], source[4]
                        ));
                    }
                    0x15 if source[3] == 0 || source[4] == 0 => {
                        return fatal(format!(
                            "無効な湾曲度 [ {} ] 、或いは半径 [ {} ] が指定されました",
                            source[3], source[4]
                        ));
                    }
                    _ => {}
                }
                let _ = self.effects.configure_displacement_map(id, &args);
                ethornell_vm::Value::None
            }
            0x18 | 0x19 => {
                let args = Self::graph91_source_ints(stack, 11);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                self.graph91_rotate_blit(&args, id == 0x19);
                ethornell_vm::Value::None
            }
            0x1a => {
                let args = Self::graph91_source_ints(stack, 3);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                if let [destination, source, packed_rgb] = args.as_slice() {
                    self.graph91_replace_rgb(*destination, *source, *packed_rgb);
                }
                ethornell_vm::Value::None
            }
            0x1b => {
                let args = Self::graph91_source_ints(stack, 5);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                if let [
                    destination,
                    source,
                    concentration_x,
                    concentration_y,
                    attenuation,
                ] = args.as_slice()
                {
                    self.graph91_concentrate_bitmap(
                        *destination,
                        *source,
                        *concentration_x,
                        *concentration_y,
                        *attenuation,
                    );
                }
                ethornell_vm::Value::None
            }
            0x1c => {
                let args = Self::graph91_source_ints(stack, 5);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                if let [destination, source, scale_x, scale_y, interpolation] = args.as_slice() {
                    self.graph91_scale_bitmap(
                        *destination,
                        *source,
                        *scale_x,
                        *scale_y,
                        *interpolation,
                    );
                }
                ethornell_vm::Value::None
            }
            0x1d => {
                let args = Self::graph91_source_ints(stack, 5);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                if let [destination, source, process_type, parameter, alpha] = args.as_slice() {
                    self.graph91_process_bitmap(
                        *destination,
                        *source,
                        *process_type,
                        *parameter,
                        *alpha,
                    );
                }
                ethornell_vm::Value::None
            }
            0x1e => {
                let args = Self::graph91_source_ints(stack, 4);
                if let Err(error) = self.graph91_check_bitmap_operation(id, &args) {
                    return Some(Err(error));
                }
                // Target pixel (x, y) reads source (x + p2, y + p3).
                if let [target, grayscale_source, source_x, source_y] = args.as_slice() {
                    self.graph91_apply_grayscale_mask(
                        *target,
                        *grayscale_source,
                        *source_x,
                        *source_y,
                    );
                }
                ethornell_vm::Value::None
            }
            0x1f => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [destination, source] = args.as_slice() {
                    // sub_4823D0 -> sub_403450: a missing source (2), a
                    // destination that cannot be created (1) are fatal.
                    if self.graph_bitmap_image(*source).is_none() {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "指定された複写元ビットマップ [ {source} ] は無効です"
                        ))));
                    }
                    if !(0..0x4000).contains(destination) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "指定された出力先ビットマップ [ {destination} ] は無効です"
                        ))));
                    }
                    let cloned = self.clone_graph_bitmap(*source, *destination);
                    if cloned {
                        self.effects.clone_bitmap(*destination, *source);
                    }
                    tracing::info!(
                        destination = *destination,
                        source = *source,
                        cloned,
                        destination_size = ?self.bitmap_dimensions.get(destination),
                        "GraphCloneBitmap"
                    );
                }
                ethornell_vm::Value::None
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    /// The slot test shared by sub_408420/sub_4084A0/sub_408540: a bitmap
    /// outside the table or not allocated is 4, one without a movie is 1.
    pub(super) fn movie_slot(&self, bitmap: i32) -> std::result::Result<(), i32> {
        if !(0..0x4000).contains(&bitmap) || !self.graph_resources.contains_key(&bitmap) {
            return Err(4);
        }
        if !self.graph_effects.is_movie(bitmap) {
            return Err(1);
        }
        Ok(())
    }

    pub(super) fn dispatch_system91_b8_f7(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x91
            || !matches!(call.id(), 0xb8 | 0xba | 0xbb | 0xbf | 0xdb | 0xf0..=0xf7)
        {
            return None;
        }
        let id = call.id();
        let stack = call.args_mut();
        let value = match id {
            // sub_4859F0 -> sub_46CB90(window, 1): like 90:B8, the window
            // lookup result is not checked before the DCIPIconEx is built.
            0xb8 => {
                let window = pop_int_value(stack).unwrap_or_default();
                let object = self.alloc_input_processor_handle();
                self.graph_input_objects
                    .insert(object, RuntimeGraphInputObject::new_extended(window));
                self.display_changes += 1;
                tracing::debug!(window, object, "Graph91CreateExtendedIconInputProcessor");
                ethornell_vm::Value::Int(object)
            }
            0xba => {
                // The VM owns the extended descriptor pointer and performs the
                // exact 40/64/196-byte validation before host dispatch.
                let _descriptor = stack.pop();
                let object = pop_int_value(stack).unwrap_or_default();
                let status = match self.graph_input_objects.get(&object) {
                    None => 1,
                    Some(input) if !input.extended => 4,
                    Some(_) => 0,
                };
                ethornell_vm::Value::Int(status)
            }
            0xbb => {
                // Source order: object, group, item, state.
                let state = pop_int_value(stack).unwrap_or_default();
                let item = pop_int_value(stack).unwrap_or_default();
                let group = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                let status = <Self as ethornell_vm::GraphApi>::set_graph_input_item_state(
                    self, object, group, item, state,
                );
                tracing::debug!(
                    object,
                    group,
                    item,
                    state,
                    status,
                    "Graph91SetExtendedIconInputItemState"
                );
                ethornell_vm::Value::Int(status)
            }
            0xbf => {
                // The VM converts and copies the fixed 24-DWORD BP table.
                let _table = stack.pop();
                let _assignment = stack.pop();
                ethornell_vm::Value::None
            }
            0xdb => {
                let handle = self.graph91_current_active_knob_handle();
                tracing::debug!(handle, "Graph91GetActiveKnobHandle");
                ethornell_vm::Value::Int(handle)
            }
            0xf0 => {
                // Source order: bitmap slot, path, loop flag, volume.
                let volume = pop_int_value(stack).unwrap_or_default();
                let looping = pop_int_value(stack).unwrap_or_default() != 0;
                let path = pop_string_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let status = if !(0..0x4000).contains(&bitmap) {
                    4
                } else if !(0..=128).contains(&volume) {
                    3
                } else {
                    #[cfg(feature = "console")]
                    let status = {
                        self.sweep_unreferenced_archive_images();
                        if self.graph_effects.is_movie(bitmap) {
                            let _ = self.graph_effects.close_movie_slot(bitmap);
                            self.graph_process_handles.remove(&bitmap);
                        }
                        find_runtime_resource(&self.manager, "", &path)
                            .and_then(|entry| {
                                self.movie_read_buffer
                                    .load_decoded(&self.manager, &entry)
                                    .ok()
                            })
                            .map(|bytes| {
                                self.graph_effects.configure_media(
                                    bitmap,
                                    String::new(),
                                    path.clone(),
                                    bytes,
                                )
                            })
                            .unwrap_or(2)
                    };
                    #[cfg(not(feature = "console"))]
                    let status = read_runtime_bytes(&self.manager, "", &path)
                        .map(|bytes| {
                            self.graph_effects.configure_media(
                                bitmap,
                                String::new(),
                                path.clone(),
                                crate::MovieBytes::from_vec(bytes),
                            )
                        })
                        .unwrap_or(2);
                    if status == 0 {
                        let options = self
                            .graph_effects
                            .set_movie_options(bitmap, looping, volume);
                        if options == 0 {
                            self.graph_process_handles.insert(bitmap);
                        }
                        options
                    } else {
                        status
                    }
                };
                tracing::debug!(
                    bitmap,
                    path,
                    looping,
                    volume,
                    status,
                    "Graph91OpenDirectShowMovie"
                );
                ethornell_vm::Value::Int(status)
            }
            0xf1 => {
                // The VM owns the duration output pointer.
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let _duration_out = stack.pop();
                ethornell_vm::Value::Int(
                    <Self as ethornell_vm::GraphApi>::invoke_graph_effect_process(self, bitmap)
                        .status,
                )
            }
            0xf2 => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let status = match self.movie_slot(bitmap) {
                    Err(status) => status,
                    Ok(()) => self.graph_effects.close_movie_slot(bitmap),
                };
                if status == 0 {
                    self.graph_process_handles.remove(&bitmap);
                }
                tracing::debug!(bitmap, status, "Graph91CloseDirectShowMovie");
                ethornell_vm::Value::Int(status)
            }
            0xf3 => {
                let paused = pop_int_value(stack).unwrap_or_default() != 0;
                let bitmap = pop_int_value(stack).unwrap_or_default();
                // sub_408540: no "started" requirement; pausing a stopped
                // DirectShow graph succeeds.
                let status = match self.movie_slot(bitmap) {
                    Err(status) => status,
                    Ok(()) => self.graph_effects.set_paused(bitmap, paused),
                };
                tracing::debug!(bitmap, paused, status, "Graph91SetDirectShowMoviePaused");
                ethornell_vm::Value::Int(status)
            }
            0xf4 => {
                if file_policy::restricted() {
                    return Some(Err(ethornell_vm::VmError::Runtime("unsupported Android SWF control: upstream is capture-only without host audio; excluded from v1".into())));
                }
                // Source order: bitmap, width, height, SWF path, parameter.
                let parameter = pop_int_value(stack).unwrap_or_default();
                let path = pop_string_value(stack).unwrap_or_default();
                let height = pop_int_value(stack).unwrap_or_default();
                let width = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let mut status =
                    if let Some(swf_data) = read_runtime_bytes(&self.manager, "", &path) {
                        self.graph_flash_controls.create(
                            bitmap,
                            width,
                            height,
                            path.clone(),
                            parameter,
                            &swf_data,
                        )
                    } else if !(0..0x4000).contains(&bitmap) || width <= 0 || height <= 0 {
                        4
                    } else if path.trim().is_empty() {
                        1
                    } else {
                        2
                    };
                if status == 0 {
                    let pixels = usize::try_from(width)
                        .ok()
                        .and_then(|width| {
                            usize::try_from(height)
                                .ok()
                                .and_then(|height| width.checked_mul(height))
                        })
                        .and_then(|pixels| pixels.checked_mul(4));
                    if let Some(byte_len) = pixels {
                        // Preserve fallible allocation so failure releases the Flash control.
                        #[allow(clippy::slow_vector_initialization)]
                        let mut rgba = Vec::new();
                        if rgba.try_reserve_exact(byte_len).is_ok() {
                            rgba.resize(byte_len, 0);
                            self.graph91_store_bitmap(
                                bitmap,
                                DecodedImage {
                                    width: width as u32,
                                    height: height as u32,
                                    rgba,
                                },
                                1,
                            );
                        } else {
                            let _ = self.graph_flash_controls.capture_and_release(bitmap);
                            status = 1;
                        }
                    } else {
                        let _ = self.graph_flash_controls.capture_and_release(bitmap);
                        status = 1;
                    }
                }
                tracing::debug!(
                    bitmap,
                    width,
                    height,
                    path,
                    parameter,
                    status,
                    "Graph91CreateFlashControl"
                );
                ethornell_vm::Value::Int(status)
            }
            0xf5 => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let status = self.graph_flash_controls.start(bitmap);
                tracing::debug!(bitmap, status, "Graph91StartFlashControl");
                ethornell_vm::Value::Int(status)
            }
            0xf6 => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let (status, captured) = self.graph_flash_controls.capture_and_release(bitmap);
                if let Some(image) = captured {
                    self.graph91_store_bitmap(bitmap, image, 1);
                }
                tracing::debug!(bitmap, status, "Graph91CaptureAndReleaseFlashControl");
                ethornell_vm::Value::Int(status)
            }
            0xf7 => {
                // The VM owns the output pointer and writes the position.
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let _position_out = stack.pop();
                ethornell_vm::Value::Int(i32::from(
                    <Self as ethornell_vm::GraphApi>::query_movie_slot_position(self, bitmap)
                        .is_some(),
                ))
            }
            _ => unreachable!(),
        };
        Some(Ok(value))
    }

    fn graph91_current_active_knob_handle(&mut self) -> i32 {
        // Target sub_4639A0 does not perform a hover query. It returns the
        // handle stored in the manager's active-drag record (dword_565FC8),
        // or zero when no knob owns the current mouse gesture.
        let handle = self.graph_active_input_handle;
        if handle != 0
            && self
                .graph_knob_states
                .get(&handle)
                .is_some_and(|state| state.enabled)
        {
            handle
        } else {
            self.finish_graph_knob_drag();
            0
        }
    }

    /// sub_440A80: a live CDspObjWindow handle.
    pub(super) fn graph91_is_window(&self, window: i32) -> bool {
        Self::is_window_surface_handle(window) && self.graph_surfaces.contains_key(&window)
    }

    /// sub_491D20 / sub_42BB00: the window must exist and have a font from
    /// 91:88.
    pub(super) fn graph91_window_with_font(&self, window: i32) -> ethornell_vm::VmResult<()> {
        if !self.graph91_is_window(window) {
            return Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into()));
        }
        if self
            .surface_text_states
            .get(&window)
            .is_none_or(|state| state.font_size == 0)
        {
            return Err(ethornell_vm::VmError::Runtime(
                "指定されたウィンドウにはフォントが設定されていません".into(),
            ));
        }
        Ok(())
    }

    pub(super) fn dispatch_system91_88_9f(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x91 || !matches!(call.id(), 0x88..=0x8e | 0x90..=0x9f) {
            return None;
        }
        let id = call.id();
        if matches!(id, 0x90 | 0x92) {
            let count = if id == 0x90 { 10 } else { 11 };
            let args = {
                let stack = call.args_mut();
                pop_args(stack, count)
            };
            let text_index = count - 2;
            let text = args.get(text_index).and_then(value_to_string);
            let target = args.last().map(value_to_i32).unwrap_or_default();
            // sub_491D20: the window, then its font (CDspObjWindow+0x36C).
            if let Err(error) = self.graph91_window_with_font(target) {
                return Some(Err(error));
            }
            if let Some(text) = text.as_deref() {
                self.native_message_surface_target = Some(target);
                self.start_native_message(text.to_string(), Some(target));
                let mut config = self.native_message_procedure_config(
                    ethornell_vm::NativeMessageProcedureClass::DspMsgEx,
                    target,
                );
                config.allow_high_bit_input =
                    args.get(4).map(value_to_i32).unwrap_or_default() != 0;
                config.end_wait_policy = args.get(5).map(value_to_i32).unwrap_or_default();
                config.completion_control = args.get(6).map(value_to_i32).unwrap_or_default();
                config.allow_auxiliary_input =
                    args.get(3).map(value_to_i32).unwrap_or_default() != 0;
                call.start_message_procedure(config);
            }
            return Some(Ok(ethornell_vm::Value::None));
        }
        let stack = call.args_mut();
        let value = match id {
            0x88 => {
                // sub_4840A0 pops render option, layout option, style, scale
                // percent, size, font id, window (script order reversed).
                let render_option = pop_int_value(stack).unwrap_or_default();
                let layout_option = pop_int_value(stack).unwrap_or_default();
                let font_style = pop_int_value(stack).unwrap_or_default();
                let scale_percent = pop_int_value(stack).unwrap_or_default();
                let font_size = pop_int_value(stack).unwrap_or_default();
                let font_name_id = pop_int_value(stack).unwrap_or_default();
                let window = pop_int_value(stack).unwrap_or_default();
                let fail = |what: String| Some(Err(ethornell_vm::VmError::Runtime(what)));
                // sub_484BA0: sub_498660 (registered font), then sub_440D80
                // -> sub_42EE90's size and width ranges.
                if self.native_user.font_face(font_name_id).is_none() {
                    return fail(format!(
                        "無効なフォント番号 [ {font_name_id} ] が指定されました"
                    ));
                }
                if !self.graph91_is_window(window) {
                    return fail(WINDOW_HANDLE.into());
                }
                if !(4..=200).contains(&font_size) {
                    return fail(format!(
                        "指定されたフォントサイズ [ {font_size} ] は無効です"
                    ));
                }
                if !(25..=200).contains(&scale_percent) {
                    return fail(format!(
                        "指定されたフォント幅 [ {scale_percent} ] は無効です"
                    ));
                }
                let state = self.surface_text_states.entry(window).or_default();
                state.font = font_name_id;
                state.font_size = font_size;
                state.scale_percent = scale_percent;
                state.font_style = font_style;
                state.layout_option = layout_option;
                state.render_option = render_option;
                self.text_state.font_size = font_size as f32;
                self.text_state.line_height =
                    (font_size as f32 * scale_percent as f32 / 100.0).max(1.0);
                // sub_42C3B0: with window+0x364 set, keep one scaled glyph
                // width free at the right edge of the bitmap; sub_42B900
                // applies the narrowed valid rectangle and resets the cursor.
                if render_option != 0 {
                    let reserve = scale_percent * font_size / 100;
                    let narrowed = self.graph_surfaces.get_mut(&window).and_then(|surface| {
                        let limit = surface.width as i32 - 1 - reserve;
                        (surface.valid_right > limit
                            && surface.valid_left >= 0
                            && surface.valid_left < surface.width as i32
                            && (0..surface.width as i32).contains(&limit))
                        .then(|| surface.valid_right = limit)
                    });
                    if narrowed.is_some() {
                        self.reset_window_text_cursor(window);
                    }
                }
                ethornell_vm::Value::None
            }
            // sub_484CD0 -> sub_440E10 -> sub_42C870: at most 800.
            0x89 => {
                let spacing = pop_int_value(stack).unwrap_or_default();
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                if spacing as u32 > 800 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定された隙間係数 [ {spacing} ] は無効です"
                    ))));
                }
                self.graph_surfaces
                    .get_mut(&window)
                    .unwrap()
                    .line_spacing_percent = spacing;
                ethornell_vm::Value::None
            }
            // sub_484D50 -> sub_440D00 -> sub_42C930: 0 or 1, then
            // sub_42C990 restarts the cursor for the new direction.
            0x8a => {
                let variant = pop_int_value(stack).unwrap_or_default();
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                if variant as u32 > 1 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効なメッセージ描画スタイル [ {variant} ] が指定されました"
                    ))));
                }
                self.graph_surfaces
                    .get_mut(&window)
                    .unwrap()
                    .message_variant = variant;
                self.reset_window_text_cursor(window);
                ethornell_vm::Value::None
            }
            // sub_484DD0 -> sub_440D40 -> sub_42C960: 0..=2.
            0x8b => {
                let mode = pop_int_value(stack).unwrap_or_default();
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                if mode as u32 > 2 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効なメッセージスインギングスタイル [ {mode} ] が指定されました"
                    ))));
                }
                self.surface_text_states
                    .entry(window)
                    .or_default()
                    .text_layout_mode = mode;
                ethornell_vm::Value::None
            }
            // sub_484E50 -> sub_4413A0 -> sub_42CAE0 (+0x384/+0x388).
            0x8c => {
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                // The next message starts at the new cursor; the engine's
                // one message node can't keep the old text elsewhere.
                self.message_pages.remove(&window);
                let state = self.surface_text_states.entry(window).or_default();
                state.cursor_x = x;
                state.cursor_y = y;
                self.trace_graph(format!("window #{window} text-cursor=({x},{y})"));
                ethornell_vm::Value::None
            }
            // sub_484EA0 pushes 1, x, y.
            0x8d => {
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                let state = self
                    .surface_text_states
                    .get(&window)
                    .copied()
                    .unwrap_or_default();
                stack.push(ethornell_vm::Value::Int(1));
                stack.push(ethornell_vm::Value::Int(state.cursor_x));
                ethornell_vm::Value::Int(state.cursor_y)
            }
            // sub_484EF0 -> sub_441400 -> sub_42CB20.
            0x8e => {
                let window = pop_int_value(stack).unwrap_or_default();
                if !self.graph91_is_window(window) {
                    return Some(Err(ethornell_vm::VmError::Runtime(WINDOW_HANDLE.into())));
                }
                let state = self
                    .surface_text_states
                    .get(&window)
                    .copied()
                    .unwrap_or_default();
                let offset = self.graph_defaults.text_layout.boundary_offset as f32;
                let reached = self.graph_surfaces.get(&window).is_some_and(|surface| {
                    if surface.message_variant == 1 {
                        state.cursor_y as f32 == surface.valid_top as f32 + offset
                    } else {
                        state.cursor_x as f32 == surface.valid_left as f32 + offset
                    }
                });
                ethornell_vm::Value::Int(i32::from(reached))
            }
            // sub_484FF0 / sub_485150 -> sub_4412D0: window (255), then
            // sub_42BB00 refuses a window without a font (17).
            0x91 | 0x93 => {
                let count = if id == 0x91 { 5 } else { 6 };
                let args = pop_args(stack, count);
                let window = args.last().map(value_to_i32).unwrap_or_default();
                if let Err(error) = self.graph91_window_with_font(window) {
                    return Some(Err(error));
                }
                self.render_native_text_args(&args);
                ethornell_vm::Value::None
            }
            0x94 => {
                let replacement = stack.pop().as_ref().and_then(value_to_optional_string);
                let source = stack.pop().as_ref().and_then(value_to_optional_string);
                self.graph_defaults
                    .update_text_substitution(source, replacement);
                ethornell_vm::Value::None
            }
            0x95 => {
                // Direct API path: the VM writes the records to the output
                // buffer (sub_434920); this returns the match count.
                let source = pop_string_value(stack).unwrap_or_default();
                let _output = stack.pop();
                let (_, count) = self.graph_defaults.collect_ruby_records(&source);
                ethornell_vm::Value::Int(count)
            }
            0x96 => {
                let records = pop_string_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(
                    self.graph_defaults.register_ruby_records(&records),
                ))
            }
            // sub_4852A0 -> sub_463820: the first argument is a font number;
            // sub_469110 turns an unregistered one into "no face".
            0x97 => {
                let mut args = pop_args(stack, 5);
                args.reverse();
                let font_name = args
                    .first()
                    .map(value_to_i32)
                    .and_then(|font| self.native_user.font_face(font))
                    .map(str::to_string);
                let mut values = [-1_i32; 6];
                for (slot, value) in values[..4].iter_mut().zip(args.iter().skip(1)) {
                    *slot = value_to_i32(value);
                }
                self.graph_defaults.configure_text_style(font_name, values);
                ethornell_vm::Value::None
            }
            0x98 => {
                let args = Self::graph91_source_ints(stack, 6);
                if let [
                    layout_advance,
                    scale_denominator,
                    character_spacing,
                    font_percent,
                    boundary_offset,
                    mode_flag,
                ] = args.as_slice()
                {
                    // sub_485300 -> sub_4346E0.
                    if let Err(error) = self.graph_defaults.text_layout.configure([
                        *layout_advance,
                        *scale_denominator,
                        *character_spacing,
                        *font_percent,
                        *boundary_offset,
                        *mode_flag,
                    ]) {
                        return Some(Err(ethornell_vm::VmError::Runtime(match error {
                            graph_defaults::TextLayoutError::FontPercent(value) => {
                                format!("無効な読み仮名のサイズレート [ {value} ] が指定されました")
                            }
                            graph_defaults::TextLayoutError::BoundaryOffset(value) => format!(
                                "無効な読み仮名の為の余白サイズ [ {value} ] が指定されました"
                            ),
                        })));
                    }
                }
                ethornell_vm::Value::None
            }
            0x99 => {
                let divisor = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(
                    self.graph_defaults.text_layout.set_scale_divisor(divisor),
                ))
            }
            0x9a => {
                // sub_485410 -> sub_434750.
                let value = pop_int_value(stack).unwrap_or_default();
                let property = pop_int_value(stack).unwrap_or_default() as u32;
                // BGI 1.622 raises 無効なファンクション番号 for any other
                // property. Wonderful Everyday's boot script (a different BGI)
                // sets 0x80000004 and carries on in the original: an unknown
                // property is ignored.
                if matches!(property, 0 | 0x8000_0000 | 0x8000_0001 | 0x8000_0002) {
                    if property == 0x8000_0001 && value < 0 {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "無効なファンクションパラメータ [ {value} ] が指定されました"
                        ))));
                    }
                    self.graph_config
                        .text_global_properties
                        .insert(property, value);
                }
                ethornell_vm::Value::None
            }
            0x9b => {
                // The VM owns the BP output pointer and implements the target
                // write. Direct trace calls only preserve stack consumption.
                let _ = pop_args(stack, 7);
                ethornell_vm::Value::Int(0)
            }
            0x9c | 0x9d => {
                let count = if id == 0x9c { 14 } else { 15 };
                let args = pop_args(stack, count);
                match self.graph91_draw_text(&args, usize::from(id == 0x9d)) {
                    Ok(end_x) => ethornell_vm::Value::Int(end_x),
                    Err(error) => return Some(Err(error)),
                }
            }
            0x9e => {
                let source = pop_string_value(stack).unwrap_or_default();
                let _destination = stack.pop();
                ethornell_vm::Value::Int(crate::native_graph_ext::count_native_labels(&source))
            }
            0x9f => {
                let _source = pop_string_value(stack).unwrap_or_default();
                let _destination = stack.pop();
                ethornell_vm::Value::None
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn dispatch_system91_55_7f(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x91
            || !matches!(
                call.id(),
                0x55 | 0x60 | 0x61 | 0x64..=0x69 | 0x70 | 0x71 | 0x73..=0x76
                    | 0x78..=0x7f
            )
        {
            return None;
        }
        let id = call.id();
        let stack = call.args_mut();
        let value = match id {
            0x55 => {
                let args = Self::graph91_source_ints(stack, 2);
                let status = if let [sprite, linked] = args.as_slice() {
                    self.graph91_set_sprite_relation(*sprite, *linked)
                } else {
                    255
                };
                ethornell_vm::Value::Int(status)
            }
            // sub_483010 / sub_483040 / sub_483070.
            0x60 => match self.graph91_create_effector() {
                0 => {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "これ以上、エフェクターオブジェクトを生成する事は出来ません".into(),
                    )));
                }
                handle => ethornell_vm::Value::Int(handle),
            },
            0x61 => {
                let handle = Self::graph91_source_ints(stack, 1)
                    .first()
                    .copied()
                    .unwrap_or(0);
                if !self.graph91_release_effector(handle) {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なエフェクターハンドルが指定されました".into(),
                    )));
                }
                ethornell_vm::Value::None
            }
            0x64 => {
                let args = Self::graph91_source_ints(stack, 2);
                let [handle, enabled] = args[..] else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なエフェクターハンドルが指定されました".into(),
                    )));
                };
                let enabled = enabled != 0;
                let Some(state) = self.graph91_effectors.get_mut(&handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なエフェクターハンドルが指定されました".into(),
                    )));
                };
                // sub_43FAF0 dispatches vtable+4 (sub_41B120), the
                // draw-enable virtual.
                state.enabled = enabled;
                self.set_graph_object_draw_enabled(handle, enabled);
                ethornell_vm::Value::None
            }
            0x65..=0x69 => {
                let count = match id {
                    0x65 | 0x67 => 6,
                    0x66 => 4,
                    0x68 => 9,
                    _ => 3,
                };
                let args = Self::graph91_source_ints(stack, count);
                if let Err(error) = self.configure_native_effector(id, &args) {
                    return Some(Err(error));
                }
                ethornell_vm::Value::None
            }
            // sub_4837F0 -> sub_440010: four slots.
            0x70 => {
                let args = Self::graph91_source_ints(stack, 6);
                let handle = if let [
                    cell_width,
                    row_step,
                    column_step,
                    baseline,
                    row_factor,
                    frame_factor,
                ] = args.as_slice()
                {
                    self.graph91_create_landscape(
                        *cell_width,
                        *row_step,
                        *column_step,
                        *baseline,
                        *row_factor,
                        *frame_factor,
                    )
                } else {
                    0
                };
                if handle == 0 {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "これ以上、ランドスケープオブジェクトを生成する事は出来ません".into(),
                    )));
                }
                ethornell_vm::Value::Int(handle)
            }
            0x71 => {
                let handle = Self::graph91_source_ints(stack, 1)
                    .first()
                    .copied()
                    .unwrap_or(0);
                if !self.graph91_release_landscape(handle) {
                    return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                }
                ethornell_vm::Value::None
            }
            0x73 => unreachable!("0x91:73 is handled by the VM landscape hit-test pointer bridge"),
            // sub_483910 -> sub_440220: CDspObj vtable+4, the draw-enable
            // virtual (sub_41B120), like 90:54.
            0x74 => {
                let args = Self::graph91_source_ints(stack, 2);
                let [handle, enabled] = args[..] else {
                    return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                };
                let Some(state) = self.graph91_landscapes.get_mut(&handle) else {
                    return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                };
                state.enabled = enabled != 0;
                self.set_graph_object_draw_enabled(handle, enabled != 0);
                ethornell_vm::Value::None
            }
            0x75 => {
                let args = Self::graph91_source_ints(stack, 6);
                if let [handle, x, y, blend_mode, alpha, priority] = args.as_slice() {
                    // sub_483950 checks priority, alpha and blend mode, then
                    // sub_440290 -> sub_422E20. CDspObjLandscape's priority
                    // virtual (sub_423380 -> sub_4233C0) refuses a priority
                    // whose last map row would pass 0xFFFF; sub_422E20 then
                    // skips position, mode and alpha, and the call succeeds.
                    if let Err(error) = target_checks::priority(*priority)
                        .and_then(|()| target_checks::effect_level(*alpha))
                        .and_then(|()| target_checks::effect_mode(*blend_mode))
                    {
                        return Some(Err(error));
                    }
                    let Some(state) = self.graph91_landscapes.get(handle) else {
                        return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                    };
                    let rows = state.map_height.max(1) as u32;
                    let last_row = (*priority as u32)
                        .wrapping_add((state.priority_row_factor as u32).wrapping_mul(rows - 1));
                    if last_row < 0x1_0000
                        && let Some(state) = self.graph91_landscapes.get_mut(handle)
                    {
                        state.x = *x;
                        state.y = *y;
                        state.blend_mode = *blend_mode;
                        state.alpha = *alpha;
                        state.priority = *priority;
                        let properties = self.graph_object_properties.entry(*handle).or_default();
                        properties.native.priority = *priority as u32;
                        self.display_tree.set_chain_depth(*handle, *priority);
                        self.graph90_refresh_native_sort_key(*handle);
                        self.graph90_set_common_state(
                            *handle,
                            Some((*x, *y)),
                            Some(*blend_mode),
                            Some(*alpha),
                            Some(*priority),
                        );
                        self.refresh_native_landscape(*handle);
                    }
                }
                ethornell_vm::Value::None
            }
            // sub_4839F0 -> sub_440310 -> sub_422A60.
            0x76 => {
                let args = Self::graph91_source_ints(stack, 6);
                let [handle, line, column, silhouette_type, level, value] = args[..] else {
                    return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                };
                let index = match self.graph91_landscape_cell(handle, line, column) {
                    Ok(index) => index,
                    Err(error) => return Some(Err(error)),
                };
                if silhouette_type as u32 > 2 {
                    return Some(Err(landscape_error(format!(
                        "無効なシルエットタイプ [ {silhouette_type} ] が指定されました"
                    ))));
                }
                if level as u32 > 0x100 {
                    return Some(Err(landscape_error(format!(
                        "無効なシルエットレベル [ {level} ] が指定されました"
                    ))));
                }
                let state = self.graph91_landscapes.get_mut(&handle).unwrap();
                let cell = &mut state.cells[index];
                cell.silhouette_type = silhouette_type;
                cell.silhouette_level = level;
                cell.silhouette_value = value;
                self.refresh_native_landscape(handle);
                ethornell_vm::Value::None
            }
            0x78 => {
                unreachable!("0x91:78 is handled by the VM landscape descriptor pointer bridge")
            }
            0x79 => unreachable!("0x91:79 is handled by the VM landscape map pointer bridge"),
            0x7a => unreachable!("0x91:7A is handled by the VM landscape guide pointer bridge"),
            0x7b => {
                unreachable!("0x91:7B is handled by the VM landscape cell-guide pointer bridge")
            }
            0x7c => {
                let args = Self::graph91_source_ints(stack, 3);
                if let [handle, destination_part, source_part] = args.as_slice()
                    && let Err(error) =
                        self.copy_native_landscape_part(*handle, *destination_part, *source_part)
                {
                    return Some(Err(error));
                }
                ethornell_vm::Value::None
            }
            0x7d => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [handle, line, column, column_index] = args.as_slice()
                    && let Err(error) = self.graph91_set_landscape_cell_column(
                        *handle,
                        *line,
                        *column,
                        *column_index,
                    )
                {
                    return Some(Err(error));
                }
                ethornell_vm::Value::None
            }
            0x7e => unreachable!("0x91:7E is handled by the VM landscape value pointer bridge"),
            0x7f => {
                let args = Self::graph91_source_ints(stack, 4);
                let [destination, handle, line, column] = args[..] else {
                    return Some(Err(landscape_error(LANDSCAPE_HANDLE)));
                };
                match self.copy_native_landscape_cell(destination, handle, line, column) {
                    Ok(copied) => ethornell_vm::Value::Int(i32::from(copied)),
                    Err(error) => return Some(Err(error)),
                }
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn dispatch_system91_31_4a(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        if call.group() != 0x91
            || !matches!(
                call.id(),
                0x31 | 0x33 | 0x36 | 0x37 | 0x38 | 0x3d | 0x3e | 0x3f | 0x40..=0x4a
            )
        {
            return None;
        }
        let id = call.id();
        if (0x40..=0x4a).contains(&id) {
            let values = call.args().iter().map(value_to_i32).collect::<Vec<_>>();
            if let Err(error) = self.validate_native_backml_call(id, &values) {
                return Some(Err(error));
            }
        }
        let stack = call.args_mut();
        // sub_4829C0..sub_482F30: not in multi-layer mode (1) and a layer
        // outside 0..8 (2) are fatal for 91:41..4A, after the handler's own
        // mode (44), level (45) and bitmap (46) checks.
        if (0x41..=0x4a).contains(&id) {
            let args: Vec<i32> = stack.iter().map(value_to_i32).collect();
            let count = match id {
                0x41 => 1,
                0x42 | 0x44 | 0x45 => 2,
                0x43 | 0x48 | 0x49 => 3,
                0x46 | 0x4a => 4,
                _ => 5,
            };
            let source = &args[args.len().saturating_sub(count)..];
            let layer = source.first().copied().unwrap_or_default();
            let precheck = match id {
                0x44 => target_checks::effect_mode(source[1]),
                0x45 => target_checks::effect_level(source[1]),
                0x46 if source[1] != -1 => target_checks::bitmap(source[1]),
                _ => Ok(()),
            };
            if let Err(error) = precheck {
                return Some(Err(error));
            }
            if !self.graph91_multilayer_background.active {
                return Some(Err(ethornell_vm::VmError::Runtime(
                    "背景表示モードがマルチレイヤモードではありません".into(),
                )));
            }
            if !(0..8).contains(&layer) {
                return Some(Err(ethornell_vm::VmError::Runtime(format!(
                    "指定されたレイヤー [ {layer} ] は無効です"
                ))));
            }
            if id == 0x46 && source[1] != -1 && !self.graph_resources.contains_key(&source[1]) {
                return Some(Err(ethornell_vm::VmError::Runtime(format!(
                    "指定されたビットマップ [ {} ] は無効です",
                    source[1]
                ))));
            }
            if id == 0x47 && (source[2] == 0 || source[3] == 0) {
                return Some(Err(ethornell_vm::VmError::Runtime(format!(
                    "無効な伸縮倍率 [ {} , {} ] が指定されました",
                    source[2], source[3]
                ))));
            }
        }
        if id == 0x40 {
            let args: Vec<i32> = stack.iter().map(value_to_i32).collect();
            let source = &args[args.len().saturating_sub(9)..];
            if source.len() == 9 {
                if let Err(error) = target_checks::bitmap(source[2]) {
                    return Some(Err(error));
                }
                if !self.graph_resources.contains_key(&source[2]) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定されたビットマップ [ {} ] は無効です",
                        source[2]
                    ))));
                }
                if source[6] == 0 || source[7] == 0 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効な伸縮倍率 [ {} , {} ] が指定されました",
                        source[6], source[7]
                    ))));
                }
            }
        }
        let value = match id {
            0x31 => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [object, suppressed] = args.as_slice() {
                    // sub_482460 -> sub_462490: unknown object fatal.
                    if !self.graph_handle_exists(*object) {
                        return Some(Err(ethornell_vm::VmError::Runtime(
                            "無効なオブジェクトハンドルが指定されました".into(),
                        )));
                    }
                    self.graph91_set_object_suppressed(*object, *suppressed != 0);
                }
                ethornell_vm::Value::None
            }
            0x33 => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [object, x, y, z] = args.as_slice() {
                    // A missing object is a script error (byte_4E8BC0).
                    if !self.graph_handle_exists(*object) {
                        return Some(Err(ethornell_vm::VmError::Runtime(
                            "無効なオブジェクトハンドルが指定されました".into(),
                        )));
                    }
                    self.graph91_set_object_fixed_position(*object, *x, *y, *z);
                }
                ethornell_vm::Value::None
            }
            0x36 => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [object, x, y, z] = args.as_slice() {
                    // A missing object is a script error (byte_4E8BC0).
                    if !self.graph_handle_exists(*object) {
                        return Some(Err(ethornell_vm::VmError::Runtime(
                            "無効なオブジェクトハンドルが指定されました".into(),
                        )));
                    }
                    self.graph91_set_object_secondary_vector(*object, *x, *y, *z);
                }
                ethornell_vm::Value::None
            }
            0x37 => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [object, x, y, z] = args.as_slice() {
                    // A missing object is a script error (byte_4E8BC0).
                    if !self.graph_handle_exists(*object) {
                        return Some(Err(ethornell_vm::VmError::Runtime(
                            "無効なオブジェクトハンドルが指定されました".into(),
                        )));
                    }
                    self.graph91_set_object_primary_vector(*object, *x, *y, *z);
                }
                ethornell_vm::Value::None
            }
            // The VM owns the writable BP pointer for both selectors.
            0x38 => unreachable!("0x91:38 is handled by the VM object-property pointer bridge"),
            0x3d => unreachable!("0x91:3D is handled by the VM composite-position pointer bridge"),
            0x3e => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [master, slave, x, y] = args.as_slice() {
                    // sub_482690 -> sub_462660: master (255), slave (6),
                    // itself (7), slave with an owner (8) are fatal.
                    let fatal =
                        |message: &str| Some(Err(ethornell_vm::VmError::Runtime(message.into())));
                    if !self.graph_handle_exists(*master) {
                        return fatal("無効なマスターオブジェクトハンドルが指定されました");
                    }
                    if !self.graph_handle_exists(*slave) {
                        return fatal("無効なスレーブオブジェクトハンドルが指定されました");
                    }
                    if master == slave {
                        return fatal(
                            "指定されたスレーブオブジェクトはマスターオブジェクト自身です",
                        );
                    }
                    if self.graph_native_owners.contains_key(slave) {
                        return fatal("指定されたスレーブオブジェクトにはオーナーが存在します");
                    }
                    self.graph91_attach_child(*master, *slave, *x, *y);
                }
                ethornell_vm::Value::None
            }
            0x3f => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [master, slave] = args.as_slice() {
                    // sub_482830 -> sub_462680: master (255), slave (6), not
                    // attached to this master (9) are fatal.
                    let fatal =
                        |message: &str| Some(Err(ethornell_vm::VmError::Runtime(message.into())));
                    if !self.graph_handle_exists(*master) {
                        return fatal("無効なマスターオブジェクトハンドルが指定されました");
                    }
                    if !self.graph_handle_exists(*slave) {
                        return fatal("無効なスレーブオブジェクトハンドルが指定されました");
                    }
                    if self.graph_native_owners.get(slave) != Some(master) {
                        return fatal("指定されたスレーブオブジェクトは従属していません");
                    }
                    self.graph91_detach_child(*master, *slave);
                }
                ethornell_vm::Value::None
            }
            0x40 => {
                let args = Self::graph91_source_ints(stack, 9);
                if let [
                    x,
                    y,
                    bitmap,
                    source_x,
                    source_y,
                    rotation,
                    scale_x,
                    scale_y,
                    transparency,
                ] = args.as_slice()
                {
                    self.graph91_initialize_multilayer_background(
                        *x,
                        *y,
                        *bitmap,
                        *source_x,
                        *source_y,
                        *rotation,
                        *scale_x,
                        *scale_y,
                        *transparency,
                    );
                }
                ethornell_vm::Value::None
            }
            0x41 => {
                if let Some(layer) = Self::graph91_source_ints(stack, 1).first().copied()
                    && (0..8).contains(&layer)
                    && self.graph91_multilayer_background.active
                {
                    self.graph91_multilayer_background.selected_layer = layer as usize;
                }
                ethornell_vm::Value::None
            }
            0x42 => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [layer, enabled] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.enabled = *enabled != 0;
                    self.graph91_sync_multilayer_render_layer(*layer as usize);
                }
                ethornell_vm::Value::None
            }
            0x43 => {
                let args = Self::graph91_source_ints(stack, 3);
                if let [layer, x, y] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.x_16_16 = *x;
                    record.y_16_16 = *y;
                    self.graph91_sync_multilayer_render_layer(*layer as usize);
                }
                ethornell_vm::Value::None
            }
            0x44 => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [layer, blend_mode] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.blend_mode = *blend_mode;
                    self.graph91_sync_multilayer_render_layer(*layer as usize);
                }
                ethornell_vm::Value::None
            }
            0x45 => {
                let args = Self::graph91_source_ints(stack, 2);
                if let [layer, blend_parameter] = args.as_slice()
                    && (0..=256).contains(blend_parameter)
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.blend_parameter = *blend_parameter;
                    self.graph91_sync_multilayer_render_layer(*layer as usize);
                }
                ethornell_vm::Value::None
            }
            0x46 => {
                let args = Self::graph91_source_ints(stack, 4);
                // sub_482C60 checks the bitmap number first (sub_4986D0,
                // unsigned < 0x4000), so -1 is fatal here.
                if let Some(&bitmap) = args.get(1)
                    && let Err(error) = target_checks::bitmap(bitmap)
                {
                    return Some(Err(error));
                }
                if let [layer, bitmap, source_x, source_y] = args.as_slice() {
                    self.graph91_set_multilayer_bitmap(*layer, *bitmap, *source_x, *source_y);
                }
                ethornell_vm::Value::None
            }
            0x47 => {
                let args = Self::graph91_source_ints(stack, 5);
                if let [layer, rotation, scale_x, scale_y, transparency] = args.as_slice() {
                    self.graph91_set_multilayer_transform(
                        *layer,
                        *rotation,
                        *scale_x,
                        *scale_y,
                        *transparency,
                    );
                }
                ethornell_vm::Value::None
            }
            0x48 => {
                let args = Self::graph91_source_ints(stack, 3);
                if let [layer, value_a, value_b] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.auxiliary_a = *value_a;
                    record.auxiliary_b = *value_b;
                    self.refresh_native_backml();
                }
                ethornell_vm::Value::None
            }
            0x49 => {
                let args = Self::graph91_source_ints(stack, 3);
                if let [layer, delta_x, delta_y] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.source_delta_x = *delta_x;
                    record.source_delta_y = *delta_y;
                    self.refresh_native_backml();
                }
                ethornell_vm::Value::None
            }
            0x4a => {
                let args = Self::graph91_source_ints(stack, 4);
                if let [layer, rotation_delta, scale_delta_x, scale_delta_y] = args.as_slice()
                    && let Some(record) = self.graph91_multilayer_record_mut(*layer)
                {
                    record.rotation_delta = *rotation_delta;
                    record.scale_delta_x = *scale_delta_x;
                    record.scale_delta_y = *scale_delta_y;
                    self.refresh_native_backml();
                }
                ethornell_vm::Value::None
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    fn graph91_set_object_suppressed(&mut self, object: i32, suppressed: bool) {
        if !self.graph_handle_exists(object) {
            return;
        }
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.native.suppress_draw = u32::from(suppressed);
        self.graph91_object_transforms
            .entry(object)
            .or_default()
            .suppressed = suppressed;
        self.graph_redraw_requested = Some(false);
    }

    /// Raw CDspObj base fixed vector returned by vtable+0x40 / sub_41B490.
    /// This deliberately excludes the +0x5C and +0x6C vector banks: target
    /// member attachment and sub_41B370 child-delta propagation use the raw
    /// base vector, while sub_41B4C0 is the separate resolved-vector helper.
    fn graph91_base_fixed_vector(&self, object: i32) -> [i32; 3] {
        self.graph_object_properties
            .get(&object)
            .map(|properties| {
                [
                    properties.native.fixed_position_x_16_16,
                    properties.native.fixed_position_y_16_16,
                    properties.native.fixed_position_z_16_16,
                ]
            })
            .unwrap_or([0; 3])
    }

    fn graph91_uses_fixed_member_coordinates(&self, object: i32) -> bool {
        // CDspObjSprite::vtable+0x70 (sub_428BE0) returns true only for
        // sprite modes 5 and 6. Base CDspObj/Group/Back implementations use
        // nullsub_17 and return false. sub_41AB40 selects its fixed attach
        // branch only when both master and slave report true.
        self.graph_object_properties
            .get(&object)
            .and_then(|properties| properties.named_properties.get("target-object-mode"))
            .is_some_and(|mode| matches!(*mode, 5 | 6))
    }

    /// Initialize the constructor-supplied CDspObj sort fields recovered at
    /// +0x18/+0x20. The manager keeps a separate monotonically increasing
    /// ECX counter for each concrete object registry; public handle slots can
    /// be reused and therefore are not valid substitutes for +0x20.
    pub(super) fn graph90_initialize_native_constructor_sort(
        &mut self,
        object: i32,
        kind: NativeDisplayKind,
    ) {
        let sort_class = match kind {
            NativeDisplayKind::Landscape | NativeDisplayKind::Map => 1,
            NativeDisplayKind::Sprite => 2,
            NativeDisplayKind::Window => 3,
            NativeDisplayKind::ParticleScreen => 4,
            NativeDisplayKind::RainScreen => 5,
            NativeDisplayKind::Effector => 6,
            NativeDisplayKind::Filter => 7,
            NativeDisplayKind::Group => 9,
            NativeDisplayKind::Knob => 10,
            _ => return,
        };
        let sort_index = {
            let next = self
                .graph_native_constructor_serials
                .entry(kind)
                .or_default();
            let current = *next;
            *next = current.wrapping_add(1);
            current
        };
        {
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.native.sort_class = sort_class;
            properties.native.sort_index = sort_index;
        }
        self.graph90_refresh_native_sort_key(object);
    }

    /// Recovered CDspObj vtable+0x1C (`sub_41B0C0`) manager key.
    ///
    /// CObjectManager orders its linked list by this unsigned 32-bit key.
    /// Priority contributes the coarse 0x10000-sized band; the remaining
    /// fields refine order inside that band. For transformed objects whose
    /// +0x7C gate is clear, resolved fixed Z supplies the low 13 bits.
    pub(super) fn graph90_native_sort_key(&self, object: i32) -> Option<u32> {
        // CDspObjKnob overrides vtable+0x1C with sub_421090 and delegates
        // directly to its controlled display object's key. The portable knob
        // wrapper does not own a separate CDspObjLayout32 image, so preserve
        // that delegation before evaluating the base layout formula.
        if let Some(knob) = self.graph_knob_states.get(&object) {
            if knob.target == object {
                return None;
            }
            return self.graph90_native_sort_key(knob.target);
        }
        let properties = self.graph_object_properties.get(&object)?;
        let transform = self
            .graph91_object_transforms
            .get(&object)
            .copied()
            .unwrap_or_default();
        let resolved_z = properties
            .native
            .fixed_position_z_16_16
            .wrapping_add(transform.primary_vector[2])
            .wrapping_add(transform.secondary_vector[2]);
        let low = if properties.native.fixed_position_updates_integer_position != 0 {
            properties.native.sort_index
        } else {
            (4095_i32.wrapping_sub(resolved_z >> 19)) as u32
        } & 0x1fff;
        let class = properties.native.sort_class.min(7);
        let priority = properties.native.priority;
        Some(
            class
                .wrapping_add(priority.wrapping_mul(8))
                .wrapping_shl(13)
                .wrapping_add(properties.native.sort_bias as u32)
                .wrapping_add(low),
        )
    }

    pub(super) fn graph90_refresh_native_sort_key(&mut self, object: i32) -> bool {
        let Some(sort_key) = self.graph90_native_sort_key(object) else {
            return false;
        };
        let previous = self.display_tree.native_sort_key(object);
        self.display_tree.set_native_sort_key(object, sort_key);
        previous != Some(sort_key)
    }

    pub(super) fn graph91_set_object_fixed_position(
        &mut self,
        object: i32,
        x: i32,
        y: i32,
        z: i32,
    ) {
        if !self.graph_handle_exists(object) {
            return;
        }

        // sub_41B370 first applies the +0x80/+0x84 rounding policy. The
        // target uses 32-bit wrapping arithmetic before clearing the low
        // 16 bits, so preserve that behavior at signed overflow boundaries.
        let (rounding_enabled, rounding_mode, mirror_integer_position) = self
            .graph_object_properties
            .get(&object)
            .map(|properties| {
                (
                    properties.native.fixed_position_rounding_enabled,
                    properties.native.fixed_position_rounding_mode,
                    properties.native.fixed_position_updates_integer_position,
                )
            })
            .unwrap_or_default();
        let (x, y) = if rounding_enabled != 0 && (z == 0 || rounding_mode == 1) {
            (
                x.wrapping_add(0x8000) & !0xffff_i32,
                y.wrapping_add(0x8000) & !0xffff_i32,
            )
        } else {
            (x, y)
        };

        // sub_41B370 snapshots the old +0x4C/+0x50/+0x54 base vector,
        // reads every current member through vtable+0x40 (sub_41B490), writes
        // the new base, then rewrites each member as:
        //   new_parent_base + child_base - old_parent_base.
        // This is eager state propagation, not renderer-time parent matrices.
        let old_base = self
            .graph_object_properties
            .get(&object)
            .map(|properties| {
                [
                    properties.native.fixed_position_x_16_16,
                    properties.native.fixed_position_y_16_16,
                    properties.native.fixed_position_z_16_16,
                ]
            })
            .unwrap_or([0; 3]);
        let child_states = self
            .graph_native_member_children(object)
            .into_iter()
            .map(|child| (child, self.graph91_base_fixed_vector(child)))
            .collect::<Vec<_>>();

        self.graph91_object_transforms
            .entry(object)
            .or_default()
            .fixed_position = [x, y, z];
        {
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.native.fixed_position_x_16_16 = x;
            properties.native.fixed_position_y_16_16 = y;
            properties.native.fixed_position_z_16_16 = z;
        }

        // With +0x7C enabled, sub_41B370 immediately invokes vtable+0x28
        // with arithmetic X>>16/Y>>16 and propagation flags (1,1). This is
        // the ordinary integer-position/member propagation path used by base
        // CDspObj. Sprite mode 5/6 and BackML explicitly clear the gate.
        if mirror_integer_position != 0 {
            self.graph90_set_position_recursive(object, x >> 16, y >> 16);
        }

        // Sprite modes 5/6 consume the resolved fixed-vector banks before
        // projection, so their GUI-visible raster geometry must be rebuilt
        // immediately from the same shared object state.
        let _ = self.graph90_resync_fixed_sprite_geometry(object);

        for (child, resolved) in child_states {
            self.graph91_set_object_fixed_position(
                child,
                x.wrapping_add(resolved[0].wrapping_sub(old_base[0])),
                y.wrapping_add(resolved[1].wrapping_sub(old_base[1])),
                z.wrapping_add(resolved[2].wrapping_sub(old_base[2])),
            );
        }
    }

    fn graph91_set_object_secondary_vector(&mut self, object: i32, x: i32, y: i32, z: i32) {
        if !self.graph_handle_exists(object) {
            return;
        }
        self.graph91_object_transforms
            .entry(object)
            .or_default()
            .secondary_vector = [x, y, z];
        // sub_41B580 recursively copies this entire bank to current member
        // children, then reapplies the primary vector on each object.
        let children = self.graph_native_member_children(object);
        for child in children {
            self.graph91_set_object_secondary_vector(child, x, y, z);
        }
        // ... then calls vtable+0x44 with this object's own primary vector,
        // which re-propagates that vector to every member (sub_41B520).
        let primary = self
            .graph91_object_transforms
            .get(&object)
            .map(|transform| transform.primary_vector)
            .unwrap_or_default();
        self.graph91_set_object_primary_vector(object, primary[0], primary[1], primary[2]);
    }

    fn graph91_set_object_primary_vector(&mut self, object: i32, x: i32, y: i32, z: i32) {
        if !self.graph_handle_exists(object) {
            return;
        }
        self.graph91_object_transforms
            .entry(object)
            .or_default()
            .primary_vector = [x, y, z];
        // sub_41B520 invokes the same vector setter on every current member.
        let children = self.graph_native_member_children(object);
        for child in children {
            self.graph91_set_object_primary_vector(child, x, y, z);
        }
        // sub_41B520 is a vector-bank setter, not a generic renderer transform.
        // Projected sprite modes consume this bank through sub_41B4C0 and must
        // rebuild their raster geometry; ordinary 2D sprites/backgrounds do not
        // receive an extra layer translation from this setter.
        let _ = self.graph90_resync_fixed_sprite_geometry(object);
    }

    fn graph91_attach_child(&mut self, master: i32, slave: i32, x: i32, y: i32) {
        let valid = master != slave
            && self.graph_handle_exists(master)
            && self.graph_handle_exists(slave)
            && !self.graph_native_owners.contains_key(&slave);
        if !valid {
            self.trace_graph(format!(
                "graph91 attach rejected master=#{master} slave=#{slave} offset=({x},{y})"
            ));
            return;
        }
        self.graph_native_owners.insert(slave, master);
        self.graph91_object_transforms
            .entry(slave)
            .or_default()
            .attachment_offset = [x, y];
        let _ = self.graph_links.set_parent(slave, master, true, true);
        self.display_tree.register_inferred(master);
        self.display_tree.register_inferred(slave);
        let _ = self.display_tree.set_parent(slave, master);
        let fixed_attach = self.graph91_uses_fixed_member_coordinates(master)
            && self.graph91_uses_fixed_member_coordinates(slave);
        if fixed_attach {
            // sub_41AB40 fixed branch reads the master's raw base vector via
            // vtable+0x40 and writes the child through vtable+0x3C. The local
            // member offsets are already in the caller's fixed-point domain;
            // the target does not shift them here.
            let master_base = self.graph91_base_fixed_vector(master);
            self.graph91_set_object_fixed_position(
                slave,
                master_base[0].wrapping_add(x),
                master_base[1].wrapping_add(y),
                master_base[2],
            );
        } else {
            // Ordinary branch uses vtable+0x30 / sub_41B240: raw +0x30/+0x34,
            // not sub_41B260's composite position and not renderer raster x/y.
            let (master_x, master_y) =
                self.graph_native_base_position(master).unwrap_or_else(|| {
                    let (x, y) = self.graph_object_position(master);
                    (x.round() as i32, y.round() as i32)
                });
            self.set_graph_object_position(
                slave,
                master_x.wrapping_add(x) as f32,
                master_y.wrapping_add(y) as f32,
            );
        }
        // Runtime display-tree ownership is bookkeeping only for native members;
        // preserve the member-local pair for diagnostics without applying it as
        // a second renderer-time ancestor transform.
        self.display_tree
            .set_local_position(slave, x as f32, y as f32);
        let (absolute_x, absolute_y) = self.graph_native_base_position(slave).unwrap_or_default();
        self.trace_graph(format!(
            "graph91 attach master=#{master} slave=#{slave} offset=({x},{y}) fixed={fixed_attach} native_base=({absolute_x},{absolute_y})"
        ));
    }

    fn graph91_detach_child(&mut self, master: i32, slave: i32) {
        if self.graph_native_owners.get(&slave) != Some(&master)
            || self.graph_links.parent(slave) != Some(master)
        {
            return;
        }
        self.graph_native_owners.remove(&slave);
        let _ = self.graph_links.set_parent(slave, 0, true, true);
        let _ = self.display_tree.set_parent(slave, 0);
        self.graph91_object_transforms
            .entry(slave)
            .or_default()
            .attachment_offset = [0, 0];
        let (absolute_x, absolute_y) = self
            .graph_native_composite_position(slave)
            .unwrap_or_default();
        self.trace_graph(format!(
            "graph91 detach master=#{master} slave=#{slave} native_retained=({absolute_x},{absolute_y})"
        ));
    }

    fn graph91_initialize_multilayer_background(
        &mut self,
        x: i32,
        y: i32,
        bitmap: i32,
        source_x: i32,
        source_y: i32,
        rotation: i32,
        scale_x: i32,
        scale_y: i32,
        transparency: i32,
    ) {
        if !self.graph_resources.contains_key(&bitmap) || scale_x == 0 || scale_y == 0 {
            return;
        }
        if self.graph90_background_class(self.current_graph_object.unwrap_or(0))
            != Some(NativeBackgroundClass::BackMl)
        {
            self.graph91_multilayer_background = Graph91MultiLayerBackgroundState::default();
        }
        self.graph91_multilayer_background.active = true;
        self.graph91_multilayer_background.object_x_16_16 = x;
        self.graph91_multilayer_background.object_y_16_16 = y;
        let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackMl);
        if let Some(background) = self
            .graph_object_properties
            .get_mut(&object)
            .and_then(|properties| properties.background.as_mut())
        {
            background.set_position(x, y, self.screen_width, self.screen_height);
        }
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.native.fixed_position_x_16_16 = x;
        properties.native.fixed_position_y_16_16 = y;
        self.graph91_multilayer_background.selected_layer = 0;
        let record = &mut self.graph91_multilayer_background.layers[0];
        record.configured = true;
        record.enabled = true;
        record.bitmap = bitmap;
        record.bitmap_generation = self
            .graph_resources
            .get(&bitmap)
            .map_or(0, |r| r.generation);
        record.x_16_16 = x;
        record.y_16_16 = y;
        record.source_x = source_x;
        record.source_y = source_y;
        record.rotation = rotation;
        record.scale_x = scale_x;
        record.scale_y = scale_y;
        record.transparency = transparency;
        self.graph91_sync_all_multilayer_render_layers();
    }

    fn graph91_multilayer_record_mut(
        &mut self,
        layer: i32,
    ) -> Option<&mut Graph91MultiLayerRecord> {
        if !self.graph91_multilayer_background.active || !(0..8).contains(&layer) {
            return None;
        }
        Some(&mut self.graph91_multilayer_background.layers[layer as usize])
    }

    fn graph91_set_multilayer_bitmap(
        &mut self,
        layer: i32,
        bitmap: i32,
        source_x: i32,
        source_y: i32,
    ) {
        if !self.graph91_multilayer_background.active || !(0..8).contains(&layer) {
            return;
        }
        if bitmap != -1 && !self.graph_resources.contains_key(&bitmap) {
            return;
        }
        let record = &mut self.graph91_multilayer_background.layers[layer as usize];
        record.configured = bitmap != -1;
        record.bitmap = bitmap;
        record.bitmap_generation = self
            .graph_resources
            .get(&bitmap)
            .map_or(0, |r| r.generation);
        record.source_x = source_x;
        record.source_y = source_y;
        record.source_delta_x = 0;
        record.source_delta_y = 0;
        self.graph91_sync_multilayer_render_layer(layer as usize);
    }

    fn graph91_set_multilayer_transform(
        &mut self,
        layer: i32,
        rotation: i32,
        scale_x: i32,
        scale_y: i32,
        transparency: i32,
    ) {
        if scale_x == 0 || scale_y == 0 {
            return;
        }
        if let Some(record) = self.graph91_multilayer_record_mut(layer) {
            record.rotation = rotation;
            record.scale_x = scale_x;
            record.scale_y = scale_y;
            record.transparency = transparency;
            record.rotation_delta = 0;
            record.scale_delta_x = 0;
            record.scale_delta_y = 0;
            self.graph91_sync_multilayer_render_layer(layer as usize);
        }
    }

    fn graph91_multilayer_render_id(layer: usize) -> i32 {
        -0x2100_0000i32 + layer as i32
    }

    pub(super) fn graph91_sync_all_multilayer_render_layers(&mut self) {
        self.refresh_native_backml();
    }
    fn graph91_sync_multilayer_render_layer(&mut self, _layer: usize) {
        self.refresh_native_backml();
    }

    fn graph91_source_ints(stack: &mut Vec<ethornell_vm::Value>, count: usize) -> Vec<i32> {
        let mut args = pop_args(stack, count)
            .iter()
            .map(value_to_i32)
            .collect::<Vec<_>>();
        args.reverse();
        args
    }

    fn graph91_store_bitmap(&mut self, bitmap: i32, image: DecodedImage, format: i32) {
        let dimensions = (image.width, image.height);
        let key = format!("runtime:bitmap:{bitmap}");
        self.store_graph_image(key.clone(), image);
        self.graph_resources
            .insert(bitmap, RuntimeGraphResource::whole(key));
        self.graph_bindings.remove(&bitmap);
        self.bitmap_dimensions.insert(bitmap, dimensions);
        self.bitmap_formats.insert(bitmap, format);
        let mut surface = RuntimeSurface::bitmap(bitmap, dimensions.0 as f32, dimensions.1 as f32);
        surface.resource_id = Some(bitmap);
        self.graph_surfaces.insert(bitmap, surface);
        self.refresh_bitmap_nodes(bitmap);
    }

    /// The checks of sub_481C00..sub_4822F0 and their cores, in target
    /// order and with target texts.
    fn graph91_check_bitmap_operation(&mut self, id: u16, a: &[i32]) -> ethornell_vm::VmResult<()> {
        let fail = |text: String| Err(ethornell_vm::VmError::Runtime(text));
        let exists = |bitmap: i32| self.graph_resources.contains_key(&bitmap);
        match id {
            // sub_402F30 / sub_402FD0 -> sub_416B60 / sub_4179B0.
            0x18 | 0x19 => {
                target_checks::effect_level(a[9])?;
                if !exists(a[0]) {
                    return fail(format!(
                        "出力先として指定されたビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
                if !exists(a[3]) {
                    return fail(format!(
                        "参照元として指定されたビットマップ [ {} ] は無効です",
                        a[3]
                    ));
                }
                if a[7] == 0 || a[8] == 0 {
                    return fail(format!(
                        "無効な伸縮倍率 [ {} , {} ] が指定されました",
                        a[7], a[8]
                    ));
                }
            }
            // sub_4034F0.
            0x1a => {
                if !exists(a[0]) {
                    return fail(format!(
                        "指定された出力先ビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
                if !exists(a[1]) {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] は無効です",
                        a[1]
                    ));
                }
            }
            // sub_403070.
            0x1b => {
                if !exists(a[0]) {
                    return fail(format!(
                        "指定された出力先ビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
                if !exists(a[1]) {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] は無効です",
                        a[1]
                    ));
                }
                if a[3] as u32 > 0x10000 || a[2] as u32 > 0x10000 {
                    return fail(format!(
                        "指定された集合度 [ {} ／ {} ] は無効です",
                        a[2], a[3]
                    ));
                }
                if a[4] as u32 > 0x100 {
                    return fail(format!("指定された輝度減衰量 [ {} ] は無効です", a[4]));
                }
            }
            // sub_402B90: the destination is recreated at the scaled size
            // with the source format.
            0x1c => {
                let Some(info) = ethornell_vm::GraphApi::query_bitmap_info(self, a[1]) else {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] は無効です",
                        a[1]
                    ));
                };
                if !matches!(info.format, 1 | 2) {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] はTRUECOLORではありません",
                        a[1]
                    ));
                }
                let width = (a[2] as u32).wrapping_mul(info.width) >> 16;
                let height = (a[3] as u32).wrapping_mul(info.height) >> 16;
                if width == 0 || height == 0 {
                    return fail(format!(
                        "無効なストレッチレート [ {} , {} ] は無効です",
                        a[2], a[3]
                    ));
                }
                if !(0..0x4000).contains(&a[0]) {
                    return fail(format!(
                        "指定された格納先ビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
            }
            // sub_403110 -> sub_419CF0: types 0..=5.
            0x1d => {
                target_checks::effect_level(a[4])?;
                if !exists(a[0]) {
                    return fail(format!(
                        "指定された出力先ビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
                if !exists(a[1]) {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] は無効です",
                        a[1]
                    ));
                }
                if a[2] as u32 > 5 {
                    return fail(format!("指定された処理タイプ [ {} ] は無効です", a[2]));
                }
            }
            // sub_403190: the format is only checked when the source,
            // moved by (-p2, -p3), overlaps the target.
            0x1e => {
                if !exists(a[0]) {
                    return fail(format!(
                        "指定された対象ビットマップ [ {} ] は無効です",
                        a[0]
                    ));
                }
                if !exists(a[1]) {
                    return fail(format!(
                        "指定された参照元ビットマップ [ {} ] は無効です",
                        a[1]
                    ));
                }
                let target = ethornell_vm::GraphApi::query_bitmap_info(self, a[0]);
                let source = ethornell_vm::GraphApi::query_bitmap_info(self, a[1]);
                if let (Some(target), Some(source)) = (target, source) {
                    let (left, top) = (-(a[2] as i64), -(a[3] as i64));
                    let overlaps = left < target.width as i64
                        && top < target.height as i64
                        && left + source.width as i64 > 0
                        && top + source.height as i64 > 0;
                    if overlaps && !matches!(source.format, 2 | 3) {
                        return fail(format!(
                            "指定された参照元ビットマップ [ {} ] はグレイスケールではありません",
                            a[1]
                        ));
                    }
                }
            }
            _ => {}
        }
        Ok(())
    }

    /// 91:18 -> sub_402F30 -> sub_416B60 (bitmap_kernels::rotate_blit) and
    /// 91:19 -> sub_402FD0 -> sub_4179B0 (bitmap_kernels::rotate_copy):
    /// destination, dx, dy, source, sx, sy, angle, x scale, y scale,
    /// alpha, filter.
    fn graph91_rotate_blit(&mut self, args: &[i32], copy: bool) {
        let &[
            destination,
            dx,
            dy,
            source,
            sx,
            sy,
            angle,
            x_scale,
            y_scale,
            alpha,
            filter,
        ] = args
        else {
            return;
        };
        let (Some(source_image), Some(destination_image)) = (
            self.graph_bitmap_image(source),
            self.graph_bitmap_image(destination),
        ) else {
            return;
        };
        let source_format = self.bitmap_formats.get(&source).copied().unwrap_or(2);
        let destination_format = self.bitmap_formats.get(&destination).copied().unwrap_or(2);
        let source_bitmap = crate::bitmap_kernels::TargetBitmap::from_image(&source_image);
        let mut target = crate::bitmap_kernels::TargetBitmap::from_image(&destination_image);
        let kernel = if copy {
            crate::bitmap_kernels::rotate_copy
        } else {
            crate::bitmap_kernels::rotate_blit
        };
        kernel(
            &mut target,
            destination_format as u32,
            &source_bitmap,
            source_format as u32,
            [dx, dy, sx, sy],
            angle,
            x_scale as u32,
            y_scale as u32,
            alpha as u32,
            filter != 0,
        );
        self.graph91_store_bitmap(
            destination,
            target.to_image(destination_format == 1),
            destination_format,
        );
    }

    fn graph91_replace_rgb(&mut self, destination: i32, source: i32, packed_rgb: i32) {
        if self.bitmap_formats.get(&destination) != Some(&2)
            || self.bitmap_formats.get(&source) != Some(&2)
        {
            return;
        }
        let Some(mut destination_image) = self.graph_bitmap_image(destination) else {
            return;
        };
        let Some(source_image) = self.graph_bitmap_image(source) else {
            return;
        };
        let [red, green, blue, _] = native_bitmap_clear_color(packed_rgb);
        let width = destination_image.width.min(source_image.width);
        let height = destination_image.height.min(source_image.height);
        for y in 0..height {
            for x in 0..width {
                let source_offset = (y as usize * source_image.width as usize + x as usize) * 4;
                let destination_offset =
                    (y as usize * destination_image.width as usize + x as usize) * 4;
                destination_image.rgba[destination_offset] = red;
                destination_image.rgba[destination_offset + 1] = green;
                destination_image.rgba[destination_offset + 2] = blue;
                destination_image.rgba[destination_offset + 3] =
                    source_image.rgba[source_offset + 3];
            }
        }
        self.graph91_store_bitmap(destination, destination_image, 2);
    }

    /// 91:1B -> sub_403070 -> sub_418F30: both bitmaps must be format 1,
    /// otherwise nothing happens; sub_418F60 (bitmap_kernels::concentrate)
    /// takes the x factor from p3 and the y factor from p2.
    fn graph91_concentrate_bitmap(
        &mut self,
        destination: i32,
        source: i32,
        concentration_y: i32,
        concentration_x: i32,
        attenuation: i32,
    ) {
        if self.bitmap_formats.get(&destination) != Some(&1)
            || self.bitmap_formats.get(&source) != Some(&1)
        {
            return;
        }
        let (Some(source_image), Some(destination_image)) = (
            self.graph_bitmap_image(source),
            self.graph_bitmap_image(destination),
        ) else {
            return;
        };
        let source = crate::bitmap_kernels::TargetBitmap::from_image(&source_image);
        let mut target = crate::bitmap_kernels::TargetBitmap::from_image(&destination_image);
        crate::bitmap_kernels::concentrate(
            &mut target,
            &source,
            attenuation as u32,
            concentration_x as u32,
            concentration_y as u32,
        );
        self.graph91_store_bitmap(destination, target.to_image(true), 1);
    }

    /// 91:1C -> sub_402B90: the destination is reallocated at
    /// (w * p2 >> 16, h * p3 >> 16) in the source format, then
    /// sub_402C60 runs sub_495880 (p4 != 0) or sub_495680
    /// (bitmap_kernels::scale_filtered / scale_nearest).
    fn graph91_scale_bitmap(
        &mut self,
        destination: i32,
        source: i32,
        scale_x: i32,
        scale_y: i32,
        interpolation: i32,
    ) {
        let Some(source_image) = self.graph_bitmap_image(source) else {
            return;
        };
        let format = self.bitmap_formats.get(&source).copied().unwrap_or(2);
        let width = (scale_x as u32).wrapping_mul(source_image.width) >> 16;
        let height = (scale_y as u32).wrapping_mul(source_image.height) >> 16;
        let source = crate::bitmap_kernels::TargetBitmap::from_image(&source_image);
        let mut target = crate::bitmap_kernels::TargetBitmap {
            pixels: vec![0; width as usize * height as usize * 4],
            stride: width as usize * 4,
            width: width as usize,
            height: height as usize,
        };
        if interpolation != 0 {
            crate::bitmap_kernels::scale_filtered(
                &mut target,
                &source,
                format as u32,
                scale_x as u32,
                scale_y as u32,
            );
        } else {
            crate::bitmap_kernels::scale_nearest(
                &mut target,
                &source,
                scale_x as u32,
                scale_y as u32,
            );
        }
        self.graph91_store_bitmap(destination, target.to_image(format == 1), format);
    }

    /// 91:1D -> sub_403110 -> sub_419CF0 (bitmap_kernels::process).
    fn graph91_process_bitmap(
        &mut self,
        destination: i32,
        source: i32,
        process_type: i32,
        parameter: i32,
        alpha: i32,
    ) {
        let (Some(source_image), Some(destination_image)) = (
            self.graph_bitmap_image(source),
            self.graph_bitmap_image(destination),
        ) else {
            return;
        };
        let source_format = self.bitmap_formats.get(&source).copied().unwrap_or(2);
        let destination_format = self.bitmap_formats.get(&destination).copied().unwrap_or(2);
        let source_bitmap = crate::bitmap_kernels::TargetBitmap::from_image(&source_image);
        let mut target = crate::bitmap_kernels::TargetBitmap::from_image(&destination_image);
        crate::bitmap_kernels::process(
            &mut target,
            destination_format as u32,
            &source_bitmap,
            source_format as u32,
            process_type as u32,
            parameter as u32,
            alpha as u32,
        );
        self.graph91_store_bitmap(
            destination,
            target.to_image(destination_format == 1),
            destination_format,
        );
    }

    fn graph91_allocate_tagged_handle<T>(registry: &BTreeMap<i32, T>, tag: u32, slots: u32) -> i32 {
        (0..slots)
            .map(|slot| (tag | slot) as i32)
            .find(|handle| !registry.contains_key(handle))
            .unwrap_or(0)
    }

    /// sub_43F160 -> sub_429070. Linking first runs sub_428320(-1), so the
    /// grayscale mask is dropped even when the link is then refused.
    pub(super) fn graph91_set_sprite_relation(&mut self, sprite: i32, linked: i32) -> i32 {
        if !self.graph90_is_sprite_handle(sprite) {
            return 255;
        }
        if linked == sprite || (linked != 0 && !self.graph90_is_sprite_handle(linked)) {
            return 11;
        }
        if linked != 0 {
            self.graph91_clear_sprite_mask(sprite);
            if self.graph91_sprite_relations.contains_key(&sprite) {
                return 13;
            }
            if self
                .graph91_sprite_relations
                .values()
                .any(|&other| other == linked)
            {
                return 14;
            }
            self.graph91_sprite_relations.insert(sprite, linked);
            self.graph91_sprite_link_masks.insert(sprite);
            return 0;
        }
        self.graph91_unlink_sprite(sprite)
    }

    /// sub_428320(-1): Sprite+0x154 = 0, +0x158 = -1; +0x160 stays.
    pub(super) fn graph91_clear_sprite_mask(&mut self, sprite: i32) {
        if let Some(properties) = self.graph_object_properties.get_mut(&sprite) {
            properties.aux_resource = None;
        }
        self.graph91_sprite_link_masks.remove(&sprite);
    }

    /// sub_429070(sprite, 0): needs +0x154 and +0x160; 0x8000000C maps to
    /// status 13. The linked sprite's +0x164 always mirrors the relation.
    pub(super) fn graph91_unlink_sprite(&mut self, sprite: i32) -> i32 {
        let masked = self.graph91_sprite_link_masks.contains(&sprite)
            || self
                .graph_object_properties
                .get(&sprite)
                .is_some_and(|properties| properties.aux_resource.is_some());
        if !masked || !self.graph91_sprite_relations.contains_key(&sprite) {
            return 13;
        }
        self.graph91_sprite_relations.remove(&sprite);
        self.graph91_clear_sprite_mask(sprite);
        0
    }

    fn graph91_create_effector(&mut self) -> i32 {
        let handle = Self::graph91_allocate_tagged_handle(
            &self.graph91_effectors,
            GRAPH91_EFFECTOR_TAG,
            GRAPH91_EFFECTOR_SLOTS,
        );
        if handle != 0 {
            self.graph91_effectors
                .insert(handle, Graph91EffectorState::default());
            self.display_tree
                .register(handle, NativeDisplayKind::Effector);
            self.graph90_initialize_native_constructor_sort(handle, NativeDisplayKind::Effector);
            self.graph_object_enabled.insert(handle, true);
        }
        handle
    }

    fn graph91_release_effector(&mut self, handle: i32) -> bool {
        if (handle as u32).wrapping_sub(GRAPH91_EFFECTOR_TAG) >= GRAPH91_EFFECTOR_SLOTS {
            return false;
        }
        let removed = self.graph91_effectors.remove(&handle).is_some();
        if removed {
            self.remove_graph_object(handle);
        }
        removed
    }

    fn graph91_create_landscape(
        &mut self,
        cell_width: i32,
        row_step: i32,
        column_step: i32,
        baseline: i32,
        row_factor: i32,
        frame_factor: i32,
    ) -> i32 {
        let handle = Self::graph91_allocate_tagged_handle(
            &self.graph91_landscapes,
            GRAPH91_LANDSCAPE_TAG,
            GRAPH91_LANDSCAPE_SLOTS,
        );
        if handle != 0 {
            self.graph91_landscapes.insert(
                handle,
                Graph91LandscapeState::new(
                    cell_width,
                    row_step,
                    column_step,
                    baseline,
                    row_factor,
                    frame_factor,
                ),
            );
            self.display_tree
                .register(handle, NativeDisplayKind::Landscape);
            self.graph90_initialize_native_constructor_sort(handle, NativeDisplayKind::Landscape);
            self.graph_object_enabled.insert(handle, true);
        }
        handle
    }

    fn graph91_release_landscape(&mut self, handle: i32) -> bool {
        if (handle as u32).wrapping_sub(GRAPH91_LANDSCAPE_TAG) >= GRAPH91_LANDSCAPE_SLOTS {
            return false;
        }
        let removed = self.graph91_landscapes.remove(&handle).is_some();
        if removed {
            self.remove_graph_object(handle);
        }
        removed
    }

    /// The shared map/line/column checks of sub_422A60, sub_4225D0,
    /// sub_422BE0 and sub_422AF0 (statuses 255, 13, 11, 12).
    pub(super) fn graph91_landscape_cell(
        &self,
        handle: i32,
        line: i32,
        column: i32,
    ) -> ethornell_vm::VmResult<usize> {
        let state = self
            .graph91_landscapes
            .get(&handle)
            .ok_or_else(|| landscape_error(LANDSCAPE_HANDLE))?;
        if state.map_height == 0 {
            return Err(landscape_error(LANDSCAPE_NO_MAP));
        }
        if line as u32 >= state.map_height as u32 {
            return Err(landscape_error(format!(
                "無効なライン番号 [ {line} ] が指定されました"
            )));
        }
        if column as u32 >= state.map_width as u32 {
            return Err(landscape_error(format!(
                "無効なカラム番号 [ {column} ] が指定されました"
            )));
        }
        Ok(line as usize * state.map_width as usize + column as usize)
    }

    /// sub_483E90 -> sub_440480 -> sub_422190: pillars (4), size and the
    /// last row's priority (5), then every entry (6).
    pub(super) fn graph91_configure_landscape_map(
        &mut self,
        handle: i32,
        width: i32,
        height: i32,
        map: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let priority = self
            .graph_object_properties
            .get(&handle)
            .map_or(0, |p| p.native.priority);
        let Some(state) = self.graph91_landscapes.get_mut(&handle) else {
            return Err(landscape_error(LANDSCAPE_HANDLE));
        };
        if state.columns.is_empty() {
            return Err(landscape_error(LANDSCAPE_NO_COLUMNS));
        }
        if !(1..=256).contains(&width)
            || !(1..=256).contains(&height)
            || priority
                .wrapping_add((state.priority_row_factor as u32).wrapping_mul(height as u32 - 1))
                >= 0x1_0000
            || map.len() != width as usize * height as usize
        {
            return Err(landscape_error(format!(
                "無効なマップサイズ [ {width} , {height} ] が指定されました"
            )));
        }
        if map
            .iter()
            .any(|&column| column as u32 >= state.columns.len() as u32 && column != -1)
        {
            return Err(landscape_error("無効なマップ情報が指定されました"));
        }
        state.map_width = width;
        state.map_height = height;
        state.cells.clear();
        state.cells.reserve(map.len());
        for (index, &column_index) in map.iter().enumerate() {
            let line = index as i32 / width;
            let column = index as i32 % width;
            let mut cell = Graph91LandscapeCell::default();
            cell.column_index = column_index;
            cell.derived_value = state.rebuild_derived_value(line, column);
            state.cells.push(cell);
        }
        self.refresh_native_landscape(handle);
        Ok(())
    }

    /// sub_484140 checks the value (sub_498920), then sub_440590 applies the
    /// (column, line) pairs one by one through sub_4229B0, so the pairs
    /// before a failing one stay applied.
    pub(super) fn graph91_set_landscape_cell_guides(
        &mut self,
        handle: i32,
        pairs: &[i32],
        layer: i32,
        guide: i32,
        value: i32,
    ) -> ethornell_vm::VmResult<()> {
        target_checks::effect_level(value)?;
        let Some(state) = self.graph91_landscapes.get_mut(&handle) else {
            return Err(landscape_error(LANDSCAPE_HANDLE));
        };
        let mut result = Ok(());
        for pair in pairs.as_chunks::<2>().0 {
            let [column, line] = *pair;
            let error = if state.map_height == 0 {
                Some(LANDSCAPE_NO_MAP.to_string())
            } else if line as u32 >= state.map_height as u32 {
                Some("指定されたフロア情報の中に無効なライン番号が設定されているものが検出されました".into())
            } else if column as u32 >= state.map_width as u32 {
                Some("指定されたフロア情報の中に無効なカラム番号が設定されているものが検出されました".into())
            } else if layer as u32 >= 4 {
                Some(format!("無効なレイヤー番号 [ {layer} ] が指定されました"))
            } else if guide as u32 >= state.guides.len() as u32 && guide != -1 {
                Some(format!("無効なガイド番号 [ {guide} ] が指定されました"))
            } else {
                None
            };
            if let Some(text) = error {
                result = Err(landscape_error(text));
                break;
            }
            let index = line as usize * state.map_width as usize + column as usize;
            state.cells[index].guides[layer as usize] = if guide == -1 {
                None
            } else {
                Some((guide, value))
            };
        }
        self.refresh_native_landscape(handle);
        result
    }

    /// sub_484590 -> sub_440720 -> sub_4225D0: cell checks, then the pillar
    /// (status 10).
    fn graph91_set_landscape_cell_column(
        &mut self,
        handle: i32,
        line: i32,
        column: i32,
        column_index: i32,
    ) -> ethornell_vm::VmResult<()> {
        let index = self.graph91_landscape_cell(handle, line, column)?;
        let state = self.graph91_landscapes.get_mut(&handle).unwrap();
        if column_index as u32 >= state.columns.len() as u32 && column_index != -1 {
            return Err(landscape_error(format!(
                "無効な柱番号 [ {column_index} ] が指定されました"
            )));
        }
        let derived = state.rebuild_derived_value(line, column);
        let cell = &mut state.cells[index];
        cell.column_index = column_index;
        cell.derived_value = derived;
        // sub_4237D0 rebuilds the cell and the three it can hide. Only a
        // pillar cell left with visible height gets its guides and
        // silhouette cleared; empty or fully hidden cells keep them.
        self.refresh_native_landscape(handle);
        let behind = column - if line & 1 != 0 { 0 } else { 1 };
        let rebuilt = [
            (line, column),
            (line - 1, behind),
            (line - 1, behind + 1),
            (line - 2, column),
        ];
        let state = self.graph91_landscapes.get_mut(&handle).unwrap();
        let raster = self.native_landscapes.get(&handle);
        for (row, col) in rebuilt {
            let Some(index) = state.cell_index(row, col) else {
                continue;
            };
            let visible = raster
                .and_then(|raster| raster.cell_visible_height(index))
                .is_some_and(|height| height > 0);
            if visible {
                let cell = &mut state.cells[index];
                cell.guides = [None; 4];
                cell.silhouette_type = 0;
                cell.silhouette_level = 0;
                cell.silhouette_value = 0;
            }
        }
        self.refresh_native_landscape(handle);
        Ok(())
    }

    /// sub_4847A0 -> sub_4407C0 -> sub_422BE0: an empty cell (status 14)
    /// returns false without writing.
    pub(super) fn graph91_landscape_cell_value(
        &self,
        handle: i32,
        line: i32,
        column: i32,
    ) -> ethornell_vm::VmResult<Option<i32>> {
        let index = self.graph91_landscape_cell(handle, line, column)?;
        let cell = &self.graph91_landscapes[&handle].cells[index];
        Ok((cell.column_index != -1).then_some(cell.derived_value as i32))
    }

    /// 91:1E -> sub_403190 (bitmap_kernels::grayscale_mask): target
    /// (x, y) is masked by source (x + dx, y + dy).
    fn graph91_apply_grayscale_mask(
        &mut self,
        target: i32,
        grayscale_source: i32,
        dx: i32,
        dy: i32,
    ) {
        let (Some(target_image), Some(source_image)) = (
            self.graph_bitmap_image(target),
            self.graph_bitmap_image(grayscale_source),
        ) else {
            return;
        };
        let target_format = self.bitmap_formats.get(&target).copied().unwrap_or(2);
        let source_format = self
            .bitmap_formats
            .get(&grayscale_source)
            .copied()
            .unwrap_or(2);
        let bitmap = |image: &DecodedImage, format| {
            if format == 3 {
                crate::bitmap_kernels::TargetBitmap::gray_from_image(image)
            } else {
                crate::bitmap_kernels::TargetBitmap::from_image(image)
            }
        };
        let mask = bitmap(&source_image, source_format);
        let mut output = bitmap(&target_image, target_format);
        crate::bitmap_kernels::grayscale_mask(
            &mut output,
            target_format as u32,
            &mask,
            source_format as u32,
            dx,
            dy,
        );
        let image = if target_format == 3 {
            output.gray_to_image()
        } else {
            output.to_image(target_format == 1)
        };
        self.graph91_store_bitmap(target, image, target_format);
    }
}
