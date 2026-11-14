//! Graph extensions recovered from BGI 1.653 / 1.687, not game patches.
//! 1.687 dispatch: 91=140162D80, 92=140163580. Procedure results are
//! deferred through the VM scheduler; the host owns decoded images and media.
use super::*;
pub(super) const MF_MOVIE_HANDLE: i32 = 0x4001;
pub(super) const DS_MOVIE_HANDLE: i32 = 0x4000;

use ethornell_vm::native_call::NativeProcedureClass;
use ethornell_vm::{GraphApi, NativeCallFrame, Value, VmResult};

pub(super) struct VersionGraphState {
    pub(super) mf_handle: Option<i32>,
    mf_volume: i32,
    next_text_record: i32,
    text_records: VecDeque<(i32, Vec<i32>)>,
    pub(super) icon_paths: BTreeMap<(i32, i32, i32), IconPath>,
}
impl Default for VersionGraphState {
    fn default() -> Self {
        Self {
            mf_handle: None,
            mf_volume: 128,
            next_text_record: 0,
            text_records: VecDeque::new(),
            icon_paths: BTreeMap::new(),
        }
    }
}

impl RuntimeTraceApi {
    pub(super) fn store_line_height_record(&mut self, values: Vec<i32>) -> i32 {
        let state = &mut self.version_graph;
        state.next_text_record = state.next_text_record.wrapping_add(1);
        let token = state.next_text_record;
        state.text_records.push_back((token, values));
        if state.text_records.len() > 16 {
            state.text_records.pop_front();
        }
        token
    }

    pub(super) fn get_line_height_record(&mut self, token: i32, drain: bool) -> Vec<i32> {
        let records = &mut self.version_graph.text_records;
        let Some(index) = records.iter().position(|(id, _)| *id == token) else {
            return Vec::new();
        };
        if drain {
            records.remove(index).unwrap().1
        } else {
            records[index].1.clone()
        }
    }

    pub(super) fn register_decoded_bitmap(
        &mut self,
        bitmap: i32,
        bytes: &[u8],
        namespace: Option<&str>,
        name: Option<&str>,
        bg_store: bool,
    ) -> i32 {
        if bitmap != -1 {
            if !(0..0x4000).contains(&bitmap) {
                return 1;
            }
            let Ok(image) = decode_image(bytes) else {
                return 2;
            };
            let format = raw_bgi_native_bitmap_format(bytes).unwrap_or(2);
            let pair = raw_bgi_embedded_point(bytes);
            self.store_runtime_bitmap(bitmap, image, format);
            self.reset_bitmap_auxiliary_pair(bitmap);
            if let Some(pair) = pair {
                self.bitmap_auxiliary_pairs.insert(bitmap, pair);
            }
        }
        if let Some(name) = name {
            if bg_store {
                self.graph_blob_cache
                    .entry((
                        namespace.map(str::to_ascii_lowercase),
                        name.to_ascii_lowercase(),
                    ))
                    .or_insert_with(|| bytes.to_vec());
            } else if let Some(cache) = self.graph_dc_cache.as_mut() {
                let now = self.clock_base_ms.wrapping_add(self.engine_time_ms as u32);
                let _ = cache.store(namespace, Some(name), bytes, now);
            }
        }
        0
    }

    pub(super) fn release_native_display_object(&mut self, object: i32) -> VmResult<bool> {
        let Some(kind) = self.display_tree.kind(object) else {
            return Ok(false);
        };
        match kind {
            NativeDisplayKind::Root
            | NativeDisplayKind::Landscape
            | NativeDisplayKind::Map
            | NativeDisplayKind::Generic => Ok(false),
            NativeDisplayKind::Knob | NativeDisplayKind::Group => {
                let id = if kind == NativeDisplayKind::Knob {
                    0xd1
                } else {
                    0xe1
                };
                let mut release = NativeCallFrame::new(
                    ethornell_vm::NativeOpcode { group: 0x90, id },
                    vec![Value::Int(object)],
                );
                GraphApi::call_graph(self, &mut release)?;
                Ok(true)
            }
            NativeDisplayKind::Window => Ok(self.remove_graph_surface(object)),
            _ => Ok(self.remove_graph_object(object)),
        }
    }

    pub(super) fn close_mf_movie(&mut self) -> bool {
        let Some(handle) = self.version_graph.mf_handle.take() else {
            return false;
        };
        self.graph_effects.release(handle);
        self.graph_process_handles.remove(&handle);
        let key = format!("runtime:movie:{handle}");
        self.graph_images.remove(&key);
        self.graph_image_revisions.remove(&key);
        self.sync_movie_audio();
        true
    }

    pub(super) fn call_version_graph(
        &mut self,
        call: &mut NativeCallFrame,
    ) -> Option<VmResult<Value>> {
        match (call.group(), call.id()) {
            (0x91, 0xbc) => {
                // 1400DEBB0 -> 1400C3530: class 2 DCIPIconEEx over a live
                // Window; this constructs a new processor, not a cloned VM.
                let window = match call.pop_i32("window") {
                    Ok(v) => v,
                    Err(e) => return Some(Err(e)),
                };
                if !Self::is_window_surface_handle(window)
                    || !self.graph_surfaces.contains_key(&window)
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なウィンドウハンドルが指定されました".into(),
                    )));
                }
                let id = self.alloc_input_processor_handle();
                self.graph_input_objects
                    .insert(id, RuntimeGraphInputObject::new_extra_extended(window));
                Some(Ok(Value::Int(id)))
            }
            (0x91, 0xf8) => {
                let args = pop_args(call.args_mut(), 3);
                let volume = args.first().map(value_to_i32).unwrap_or_default();
                self.version_graph.mf_volume = volume;
                let resource = args.get(1).and_then(value_to_string).unwrap_or_default();
                let archive = args.get(2).and_then(value_to_string).unwrap_or_default();
                self.close_mf_movie();
                let found = user_files::runtime_file_exists_from_root(
                    &self.manager,
                    &self.native_root,
                    &archive,
                    &resource,
                );
                let opened = if found {
                    self.open_movie_process(
                        &archive,
                        &resource,
                        volume.clamp(0, 128),
                        MF_MOVIE_HANDLE,
                    )
                } else {
                    None
                };
                if let Some((handle, _)) = opened {
                    self.version_graph.mf_handle = Some(handle);
                    // DCProcPlayMovieByMF polls playback to completion. The
                    // ABI has no immediate output; host-result mode publishes
                    // its one result after the movie ends or is cancelled.
                } else {
                    let result = if found { -4 } else { -3 };
                    call.complete_procedure_with_output(
                        NativeProcedureClass::PlayMovieMf,
                        result,
                        result,
                    );
                }
                Some(Ok(Value::None))
            }
            (0x91, 0xf9) => Some(Ok(Value::Int(if self.close_mf_movie() { 0 } else { -2 }))),
            (0x91, 0xfa) => Some(Ok(Value::Int(i32::from(
                self.version_graph
                    .mf_handle
                    .is_some_and(|h| self.graph_effects.state(h) == 0),
            )))),
            (0x91, 0xfb) => {
                let volume = match call.pop_i32("volume") {
                    Ok(v) => v,
                    Err(e) => return Some(Err(e)),
                };
                let result = if let Some(handle) = self.version_graph.mf_handle {
                    // Native 1400F0DB0 stores the requested integer, while
                    // the media backend 140110580 clamps playback to 0..128.
                    self.version_graph.mf_volume = volume;
                    if self.graph_effects.set_volume(handle, volume.clamp(0, 128)) == 0 {
                        0
                    } else {
                        -1
                    }
                } else {
                    -2
                };
                Some(Ok(Value::Int(result)))
            }
            _ => None,
        }
    }
}

pub(super) struct IconPath {
    axes: [animation::NativeNaturalCubicSpline; 3],
    endpoint: [i32; 3],
    phase: i32,
    alpha: [i32; 2],
    curves: [i32; 2],
    duration: u32,
    start: u64,
    delay: u32,
    sprite_mode: Option<i32>,
}

impl RuntimeTraceApi {
    pub(super) fn configure_eex_icon_path(
        &mut self,
        args: [i32; 10],
        points: &[[i32; 4]],
    ) -> VmResult<i32> {
        // 1400C2B10 preserves the native status mapping before touching the
        // item. CSpline accepts the first 100 XYZ points; record word 3 is
        // copied by the original caller but never enters its three splines.
        let [input, group, item, count, a0, a1, c0, c1, duration, initial] = args;
        let Some(state) = self.graph_input_objects.get(&input) else {
            return Ok(1);
        };
        if !state.extra_extended {
            return Ok(4);
        }
        if group < 0 || !state.descriptor.groups.iter().any(|g| g.index == group) {
            return Ok(2);
        }
        if !state
            .descriptor
            .regions
            .iter()
            .any(|r| r.group == group && r.index == item)
        {
            return Ok(3);
        }
        if count < 2 {
            return Ok(5);
        }
        if duration < 2 {
            return Ok(8);
        }
        let Some(first) = points.first() else {
            return Ok(5);
        };
        let last = points.last().unwrap();
        let position = if initial != 0 { *first } else { *last };
        let axes = std::array::from_fn(|axis| {
            animation::NativeNaturalCubicSpline::new(
                points.iter().take(100).map(|p| p[axis]).collect(),
            )
        });
        self.version_graph.icon_paths.insert(
            (input, group, item),
            IconPath {
                axes,
                endpoint: [last[0], last[1], last[2]],
                phase: i32::from(initial != 0),
                alpha: [a0, a1],
                curves: [c0, c1],
                duration: duration as u32,
                start: 0,
                delay: 0,
                sprite_mode: None,
            },
        );
        self.place_eex_icon(
            input,
            group,
            item,
            [position[0], position[1], position[2]],
            a0,
        );
        Ok(0)
    }

    fn place_eex_icon(&mut self, input: i32, group: i32, item: i32, point: [i32; 3], alpha: i32) {
        let Some(state) = self.graph_input_objects.get_mut(&input) else {
            return;
        };
        let ordinal = state
            .descriptor
            .regions
            .iter()
            .position(|r| r.group == group && r.index == item);
        if let Some(index) = ordinal {
            state.descriptor.regions[index].x = point[0];
            state.descriptor.regions[index].y = point[1];
        }
        state.item_depths.insert((group, item), point[2]);
        self.refresh_graph_input_control_layers(input);
        let layers = self
            .graph_layers
            .iter()
            .filter(|(id, l)| {
                ordinal == usize::try_from(l.hit_id).ok()
                    && self
                        .surface_controls
                        .contains_layer_for_owner(SurfaceControlOwner::InputObject(input), **id)
            })
            .map(|(&id, _)| id)
            .collect::<Vec<_>>();
        let mode = self
            .version_graph
            .icon_paths
            .get(&(input, group, item))
            .and_then(|p| p.sprite_mode);
        for id in layers {
            self.graph90_set_common_state(id, None, None, Some(alpha), None);
            if let Some(mode) = mode {
                self.graph_object_properties
                    .entry(id)
                    .or_default()
                    .named_properties
                    .insert("target-object-mode".into(), mode);
            }
        }
    }

    pub(super) fn trigger_eex_icon_path(&mut self, input: i32, words: &[i32]) -> bool {
        if words.len() != 5 || words[0] != 0x3000_0000 {
            return false;
        }
        let flags = self
            .graph_input_objects
            .get(&input)
            .and_then(|i| i.descriptor.groups.iter().find(|g| g.index == words[1]))
            .map_or(0, |g| g.extended_flags);
        let now = self.engine_time_ms;
        let Some(path) = self
            .version_graph
            .icon_paths
            .get_mut(&(input, words[1], words[2]))
        else {
            return false;
        };
        let reverse = words[3] != 0;
        let delay = words[4] as u32;
        if !reverse {
            path.sprite_mode = Some(i32::from(flags & 0x10000 == 0));
        }
        match (reverse, path.phase) {
            (true, 0) | (false, 1) => {
                path.phase = if reverse { 2 } else { 3 };
                path.start = now;
                if delay != 0 {
                    path.delay = delay;
                }
            }
            (true, 3) | (false, 2) => {
                if path.delay != 0 {
                    path.phase = if reverse { 1 } else { 0 };
                    path.delay = 0;
                    if reverse {
                        path.sprite_mode = Some(1);
                    }
                } else {
                    path.start = now
                        .wrapping_mul(2)
                        .wrapping_sub(u64::from(path.duration))
                        .wrapping_sub(path.start);
                    path.phase = if reverse { 2 } else { 3 };
                }
            }
            (true, 2) | (false, 3) => {
                if path.delay != 0 {
                    path.delay = delay;
                }
            }
            _ => {}
        }
        true
    }

    pub(super) fn tick_eex_icon_paths(&mut self) {
        let now = self.engine_time_ms;
        let mut updates = Vec::new();
        self.version_graph.icon_paths.retain(|(input, _, _), _| {
            self.graph_input_objects
                .get(input)
                .is_some_and(|i| i.extra_extended)
        });
        for (&(input, group, item), path) in &mut self.version_graph.icon_paths {
            if !matches!(path.phase, 2 | 3)
                || self
                    .graph_input_objects
                    .get(&input)
                    .is_none_or(|i| i.enabled == 0)
            {
                continue;
            }
            let mut elapsed = now.wrapping_sub(path.start);
            if elapsed < u64::from(path.delay) {
                continue;
            }
            if path.delay != 0 {
                path.start = path.start.wrapping_add(u64::from(path.delay));
                elapsed -= u64::from(path.delay);
                path.delay = 0;
            }
            let elapsed = elapsed.min(u64::from(path.duration)) as u32;
            // 1400941E0 -> 1400566F0 uses the engine's 8.24 time curves.
            let progress =
                ((f64::from(elapsed) / f64::from(path.duration)) * 16777216.0 + 0.5).floor() as i32;
            let reverse = path.phase == 2;
            let curve = path.curves[usize::from(!reverse)];
            let eased = animation::target_curve_fixed_16(curve, progress) as u32;
            let index = ((u64::from(eased) * u64::from(path.duration)) >> 16) as u32;
            let index = if reverse {
                path.duration.wrapping_sub(index)
            } else {
                index
            };
            let point = if index >= path.duration {
                path.endpoint
            } else {
                std::array::from_fn(|axis| {
                    (path.axes[axis].sample_real(f64::from(index) / f64::from(path.duration - 1))
                        + 0.5)
                        .floor() as i32
                })
            };
            let (from, to) = if reverse {
                (path.alpha[1], path.alpha[0])
            } else {
                (path.alpha[0], path.alpha[1])
            };
            let alpha = from
                .wrapping_add(((i64::from(to.wrapping_sub(from)) * i64::from(eased)) >> 16) as i32);
            if elapsed == path.duration {
                path.phase = if reverse { 1 } else { 0 };
                path.start = 0;
                if reverse {
                    path.sprite_mode = Some(1);
                }
            }
            updates.push((input, group, item, point, alpha));
        }
        for (input, group, item, point, alpha) in updates {
            self.place_eex_icon(input, group, item, point, alpha);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_script::parse_bp_program;
    use ethornell_vm::{
        GraphInputDescriptor, GraphInputGroup, GraphInputRegion, NativeAbiProfile, Vm,
        VmRunOptions, VmStopReason,
    };

    fn api() -> RuntimeTraceApi {
        RuntimeTraceApi::new(ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap())
    }

    #[test]
    fn version_graph_global_movies_never_publish_bitmap_slots_or_close_each_other() {
        let mut api = api();
        for (handle, color) in [(DS_MOVIE_HANDLE, 10), (MF_MOVIE_HANDLE, 20)] {
            api.store_movie_frame(graph_effect::MovieFrameUpdate {
                bitmap: handle,
                image: DecodedImage {
                    width: 1,
                    height: 1,
                    rgba: vec![color, 0, 0, 255],
                },
            });
            assert!(!api.graph_resources.contains_key(&handle));
            assert!(!api.bitmap_dimensions.contains_key(&handle));
        }
        api.global_movie_handle = Some(DS_MOVIE_HANDLE);
        api.version_graph.mf_handle = Some(MF_MOVIE_HANDLE);
        api.close_global_movie();
        assert!(
            api.graph_images
                .contains_key(&format!("runtime:movie:{MF_MOVIE_HANDLE}"))
        );
        assert_eq!(api.version_graph.mf_handle, Some(MF_MOVIE_HANDLE));
        assert!(api.close_mf_movie());
        assert!(!api.close_mf_movie());
    }

    #[test]
    fn version_graph_display_mode_count_is_profile_scoped() {
        let mut api = api();
        let mut call = NativeCallFrame::new(
            ethornell_vm::NativeOpcode {
                group: 0x80,
                id: 0x60,
            },
            vec![Value::Int(8), Value::Int(1), Value::Int(0)],
        );
        assert!(ethornell_vm::SysApi::call_sys(&mut api, &mut call).is_err());
        api.display_mode_limit = 16;
        api.display_mode_sizes[8] = Some((1920, 1080));
        let mut call = NativeCallFrame::new(
            ethornell_vm::NativeOpcode {
                group: 0x80,
                id: 0x60,
            },
            vec![Value::Int(8), Value::Int(1), Value::Int(0)],
        );
        assert_eq!(
            ethornell_vm::SysApi::call_sys(&mut api, &mut call).unwrap(),
            Value::None
        );
        assert_eq!((api.screen_width, api.screen_height), (1920, 1080));
    }

    #[test]
    fn version_graph_generic_release_is_connected_through_public_dispatch() {
        let mut api = api();
        let window = api.alloc_window_surface(20, 20).unwrap();
        let opcode = ethornell_vm::NativeOpcode {
            group: 0x92,
            id: 0x31,
        };
        let mut call = NativeCallFrame::new(opcode, vec![Value::Int(window)]);
        assert_eq!(
            GraphApi::call_graph(&mut api, &mut call).unwrap(),
            Value::Int(1)
        );
        call.require_consumed().unwrap();
        assert!(!api.graph_surfaces.contains_key(&window));
        let mut call = NativeCallFrame::new(opcode, vec![Value::Int(window)]);
        assert_eq!(
            GraphApi::call_graph(&mut api, &mut call).unwrap(),
            Value::Int(0)
        );
        call.require_consumed().unwrap();
    }

    #[test]
    fn version_graph_force_present_consumes_native_argument_and_preserves_deadline() {
        let mut api = api();
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::Anipani);
        let report = vm.run(
            &parse_bp_program(None, &[0, 1, 0x91, 0, 0x17]),
            &mut api,
            &VmRunOptions {
                fail_on_stub: true,
                ..Default::default()
            },
        );
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(vm.stack.is_empty());
        assert_eq!(api.graph_force_present, 1);
        api.graph_next_present_ms = u32::MAX - 1;
        let before = api.graph_next_present_ms;
        assert!(api.take_present_due());
        assert_eq!(api.graph_next_present_ms, before);
        api.graph_force_present = 0;
        assert!(!api.take_present_due());
    }

    #[test]
    fn version_graph_line_height_tokens_are_peeked_then_consumed_and_bounded() {
        let mut api = api();
        let first = api.store_line_height_record(vec![20, 24, 20]);
        assert_eq!(api.get_line_height_record(first, false), [20, 24, 20]);
        assert_eq!(api.get_line_height_record(first, false), [20, 24, 20]);
        assert_eq!(api.get_line_height_record(first, true), [20, 24, 20]);
        assert!(api.get_line_height_record(first, false).is_empty());
        let old = api.store_line_height_record(vec![1]);
        for _ in 0..16 {
            api.store_line_height_record(vec![2]);
        }
        assert!(api.get_line_height_record(old, false).is_empty());
        assert_eq!(api.version_graph.text_records.len(), 16);
    }

    #[test]
    fn version_graph_eex_path_updates_item_geometry_and_delayed_transitions() {
        let mut api = api();
        let mut input = RuntimeGraphInputObject::new_extra_extended(0);
        input.descriptor = GraphInputDescriptor {
            groups: vec![GraphInputGroup {
                index: 0,
                ..Default::default()
            }],
            regions: vec![GraphInputRegion {
                group: 0,
                index: 0,
                ordinal: 0,
                enabled_depth: 1,
                selected: false,
                x: 0,
                y: 0,
                width: 1,
                height: 1,
                normal_resource: -1,
                selected_resource: -1,
                hover_resource: -1,
                hover_selected_resource: -1,
                mask_resource: -1,
                current_selection_hit_excluded: false,
                flags: 0,
            }],
            ..Default::default()
        };
        api.graph_input_objects.insert(1, input);
        let points = [[0, 0, 0, 99], [100, 50, 10, 88]];
        assert_eq!(
            api.configure_eex_icon_path([1, 0, 0, 2, 0, 256, 0, 0, 100, 1], &points)
                .unwrap(),
            0
        );
        assert_eq!(api.graph_input_objects[&1].descriptor.regions[0].x, 0);
        assert!(api.trigger_eex_icon_path(1, &[0x3000_0000, 0, 0, 0, 20]));
        api.engine_time_ms = 10;
        api.tick_eex_icon_paths();
        assert_eq!(api.graph_input_objects[&1].descriptor.regions[0].x, 0);
        api.engine_time_ms = 70;
        api.tick_eex_icon_paths();
        assert!((49..=52).contains(&api.graph_input_objects[&1].descriptor.regions[0].x));
        api.engine_time_ms = 120;
        api.tick_eex_icon_paths();
        assert_eq!(api.graph_input_objects[&1].descriptor.regions[0].x, 100);
        assert!(api.trigger_eex_icon_path(1, &[0x3000_0000, 0, 0, 1, 0]));
        api.engine_time_ms = 220;
        api.tick_eex_icon_paths();
        assert_eq!(api.graph_input_objects[&1].descriptor.regions[0].x, 0);
        assert_eq!(
            api.configure_eex_icon_path([1, 2, 0, 2, 0, 0, 0, 0, 100, 1], &points)
                .unwrap(),
            2
        );
        assert_eq!(
            api.configure_eex_icon_path([1, 0, 3, 2, 0, 0, 0, 0, 100, 1], &points)
                .unwrap(),
            3
        );
        assert_eq!(
            api.configure_eex_icon_path([1, 0, 0, 1, 0, 0, 0, 0, 100, 1], &points)
                .unwrap(),
            5
        );
        assert_eq!(
            api.configure_eex_icon_path([1, 0, 0, 2, 0, 0, 0, 0, 1, 1], &points)
                .unwrap(),
            8
        );
    }
}
