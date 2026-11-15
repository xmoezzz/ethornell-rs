use super::{INPUT_DESCRIPTOR_MOUSE_LEFT, RuntimeTraceApi, RuntimeUserControl, value_to_i32};
use ethornell_vm::{GraphInputDescriptor, Value};

impl RuntimeTraceApi {
    pub(crate) fn hit_test_user_controls(&self, point: (f32, f32)) -> Option<i32> {
        self.hit_test_user_control(point).map(|control| control.id)
    }

    pub(crate) fn hit_test_user_control(&self, point: (f32, f32)) -> Option<&RuntimeUserControl> {
        if self.title_ui_active && !self.has_title_child_render_context() {
            return self
                .user_controls
                .iter()
                .rev()
                .filter_map(|(_, control)| is_primary_title_control(control).then_some(control))
                .find(|control| control.contains(point, true));
        }
        self.user_controls
            .iter()
            .rev()
            .filter_map(|(_, control)| (!control.title_only).then_some(control))
            .find(|control| control.contains(point, self.title_ui_active))
    }

    pub(crate) fn hit_test_user_control_for_object(
        &self,
        point: (f32, f32),
        object: i32,
    ) -> Option<&RuntimeUserControl> {
        if self.title_ui_active && !self.has_title_child_render_context() {
            return self
                .user_controls
                .iter()
                .rev()
                .filter_map(|(_, control)| is_primary_title_control(control).then_some(control))
                .find(|control| {
                    control.contains(point, true)
                        && (object == 0
                            || object == control.id
                            || object == control.owner_id
                            || object == control.payload)
                });
        }
        self.user_controls
            .iter()
            .rev()
            .filter_map(|(_, control)| (!control.title_only).then_some(control))
            .find(|control| {
                control.contains(point, self.title_ui_active)
                    && (object == 0
                        || object == control.id
                        || object == control.owner_id
                        || object == control.payload)
            })
    }

    pub(crate) fn observe_loaded_program_for_title(&mut self, file: &str) {
        if !self.title_ui_active {
            return;
        }
        if file.eq_ignore_ascii_case("scrmain._bp") {
            if !self.title_child_program_active
                && self.title_scenario_requested
                && self.last_hit_control != 0
                && is_title_scenario_callback(self.last_hit_payload)
            {
                self.depart_title_ui_after_script(file);
            } else {
                self.trace_graph("scrmain preload observed while title ui remains active");
            }
        } else if file.eq_ignore_ascii_case("omake._bp")
            || file.eq_ignore_ascii_case("syswnd._bp")
            || file.eq_ignore_ascii_case("cnfgwnd._bp")
            || file.eq_ignore_ascii_case("usdtwnd._bp")
        {
            let key = file.to_ascii_lowercase();
            self.title_child_program_active = true;
            self.title_scenario_requested = false;
            if self.title_child_program.as_deref() != Some(key.as_str()) {
                let cleared = self.user_controls.len();
                self.user_controls.clear();
                self.title_child_program = Some(key);
                self.trace_graph(format!(
                    "title child controls reset for {file} cleared={cleared}"
                ));
            }
        }
    }

    pub(crate) fn observe_running_program_for_title(&mut self, program: &str) {
        if !self.title_ui_active {
            return;
        }
        if is_scenario_driver_program_report(program) {
            if should_depart_for_scenario(self.title_child_program_active) {
                self.depart_title_ui_for_scenario(program);
            } else {
                self.trace_graph(format!(
                    "scenario driver kept title ui active child_active={} requested={}",
                    self.title_child_program_active, self.title_scenario_requested
                ));
            }
            return;
        }
        if !self.title_child_program_active {
            return;
        }
        if !is_title_program_report(program) {
            return;
        }
        let child = self.title_child_program.take();
        self.title_child_program_active = false;
        self.user_controls.retain(|_, control| control.title_only);
        self.trace_graph(format!(
            "title child program returned child={}",
            child.as_deref().unwrap_or("<unknown>")
        ));
    }

    fn depart_title_ui_after_script(&mut self, file: &str) {
        self.depart_title_ui_for_scenario(file);
    }

    pub(crate) fn depart_title_ui_for_scenario(&mut self, file: &str) {
        if self.title_ui_departed {
            self.start_pending_scenario_bootstrap();
            return;
        }
        self.title_ui_active = false;
        self.title_ui_departed = true;
        self.title_child_program_active = false;
        self.title_child_program = None;
        self.title_scenario_requested = false;
        self.user_controls
            .retain(|_, control| control.owner_id == crate::text::MESSAGE_CONTROL_OWNER_ID);
        self.clear_transient_screen_text_nodes();
        self.clear_title_graph_layers();
        self.trace_graph(format!(
            "title ui departed after control #{} loaded {file}",
            self.last_hit_control
        ));
        self.start_pending_scenario_bootstrap();
    }

    pub(crate) fn maybe_consume_stale_title_click(&mut self) {
        // Title input is script-driven. Let title._bp consume clicks through
        // GraphPollObjectEvent so settings/omake follow the original callback table.
    }

    pub(crate) fn consume_title_control_event(&mut self, hit: i32, payload: i32, source: &str) {
        self.last_hit_control = hit;
        self.last_hit_payload = payload;
        self.title_scenario_requested = is_title_scenario_callback(payload);
        self.pending_click = None;
        self.pending_click_age_frames = 0;
        self.pending_object_state = None;
        self.pending_input_state = None;
        self.pending_input_descriptor = None;
        self.pending_title_payload_override = None;
        self.mouse_pressed = false;
        self.auto_title_release_after_state = false;
        self.trace_graph(format!(
            "title control event delivered to script source={source} hit=#{hit} payload={payload}"
        ));
    }

    pub(crate) fn observe_title_graph_event(&mut self, event: [i32; 3]) {
        if event[0] != 0x1000_0006 || !self.title_ui_active || self.has_title_child_render_context()
        {
            return;
        }
        let payload = event[1] & 0xffff;
        self.last_hit_control = event[1];
        self.last_hit_payload = payload;
        self.title_scenario_requested = is_title_scenario_callback(payload);
        self.trace_graph(format!(
            "title graph release payload={payload} scenario={}",
            self.title_scenario_requested
        ));
    }

    pub(crate) fn apply_title_graph_payload_override(&mut self, packed: i32) -> i32 {
        if !self.title_ui_active || self.has_title_child_render_context() {
            return packed;
        }
        let Some(payload) = self.pending_title_payload_override.take() else {
            return packed;
        };
        (packed & !0xffff) | (payload & 0xffff)
    }

    pub(crate) fn sync_title_controls_from_input(
        &mut self,
        owner_id: i32,
        descriptor: &GraphInputDescriptor,
    ) {
        if !self.title_ui_active || self.has_title_child_render_context() {
            return;
        }
        // These RuntimeUserControl entries predate the native DCIPIcon path.
        // Once a real Graph90/91 input descriptor exists they would create a
        // second title hit-test/event system beside DCIPIcon. In particular,
        // generic poll_object_state/event can then consume the same mouse edge
        // independently of Graph90:BC/BF. Keep this code only as an explicit
        // legacy fallback for bring-up/debugging; target execution uses the
        // descriptor through graph_input_objects and surface control layers.
        if std::env::var("ETHORNELL_LEGACY_TITLE_CONTROLS")
            .ok()
            .as_deref()
            != Some("1")
        {
            self.user_controls.retain(|_, control| !control.title_only);
            self.trace_graph(format!(
                "native title input object=#{owner_id} owns {} regions; legacy title controls suppressed",
                descriptor.regions.len()
            ));
            return;
        }
        self.user_controls.retain(|_, control| !control.title_only);
        for region in descriptor
            .regions
            .iter()
            .filter(|region| region.enabled_depth != 0 && region.width > 0 && region.height > 0)
        {
            let payload = region.index & 0xffff;
            let id = title_object_for_payload(payload)
                .unwrap_or_else(|| owner_id.saturating_mul(100).saturating_add(payload));
            self.user_controls.insert(
                id,
                RuntimeUserControl {
                    id,
                    owner_id,
                    payload,
                    x: region.x as f32,
                    y: region.y as f32,
                    width: region.width as f32,
                    height: region.height as f32,
                    normal_resource: region.normal_resource,
                    // RuntimeUserControl::selected_resource is the compatibility
                    // renderer's hover bitmap.  The compact DCIPIcon descriptor
                    // keeps pointer-hover at item+0x14, not item+0x10 (the
                    // per-group current/selected bitmap).
                    selected_resource: if region.hover_resource >= 0 {
                        region.hover_resource
                    } else {
                        region.selected_resource
                    },
                    enabled: true,
                    title_only: true,
                },
            );
            self.trace_graph(format!(
                "title input object=#{owner_id} control=#{id} payload={payload} x={} y={} w={} h={} normal=#{} hover=#{} selected=#{}",
                region.x,
                region.y,
                region.width,
                region.height,
                region.normal_resource,
                region.hover_resource,
                region.selected_resource
            ));
        }
    }

    pub(crate) fn observe_user_control(
        &mut self,
        group: u8,
        id: u16,
        stack: &[ethornell_vm::Value],
    ) {
        let args: Vec<i32> = stack.iter().rev().take(20).map(value_to_i32).collect();
        if self.debug_graph {
            self.trace_graph(format!("user 0x{group:02X}:0x{id:02X} args(top)={args:?}"));
        }
    }

    pub(crate) fn call_user_control(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> ethornell_vm::VmResult<Option<Value>> {
        if let Some(result) = self.dispatch_native_effect(call) {
            return result.map(Some);
        }
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group == 0xb0 {
            match id {
                0x02 => {
                    // The default user subsystem initializer is used when IPL
                    // has no host-specific pair to pass to 0x03.
                    return Ok(Some(Value::Int(0)));
                }
                0x03 => {
                    let _host_context = pop_user_args(stack, 2)?;
                    return Ok(Some(Value::Int(0)));
                }
                0x06 => {
                    // Every testcase callsite uses this zero-argument boolean
                    // immediately before processing window or pointer input.
                    let enabled = !self.input_requires_focus || self.window_focused;
                    return Ok(Some(Value::Int(i32::from(enabled))));
                }
                _ => {}
            }
            let argc = match id {
                0x08 => 7,
                0x10 => 5,
                0x11 | 0x17 => 1,
                0x14 | 0x1c => 2,
                0x19 => 6,
                0x82 => 3,
                0x8c => 4,
                _ => return Ok(None),
            };
            let args = pop_user_args(stack, argc)?;
            let result = match id {
                0x08 => Some(Value::Int(self.alloc_object())),
                0x10 | 0x82 | 0x8c => Some(Value::Int(0)),
                0x17 => {
                    let (x, y) = self
                        .mouse_pos
                        .map(|(x, y)| (x.round() as i32, y.round() as i32))
                        .unwrap_or_default();
                    stack.push(Value::Int(x));
                    Some(Value::Int(y))
                }
                _ => Some(Value::None),
            };
            if self.debug_graph {
                self.trace_graph(format!(
                    "debug user 0x{id:02X} args={:?}",
                    args.iter().map(value_to_i32).collect::<Vec<_>>()
                ));
            }
            return Ok(result);
        }
        if group != 0xc0 {
            return Ok(None);
        }
        let argc = match id {
            0x00 => 2,
            0x01 => 1,
            0x04 => 2,
            0x05 => 6,
            0x09 | 0x0a | 0x0c | 0x0d => 2,
            0x0b => 10,
            0x17 => 1,
            0x0f | 0x1f | 0x41 => 1,
            0x18 => 7,
            0x28 => 4,
            0x29 => 16,
            0x2d => 18,
            0x10 => 12,
            0x1a => 8,
            0x1b => 3,
            0x20 => 4,
            0x40 | 0x42 | 0x43 => 2,
            0x44 => 6,
            0x45 => 7,
            0x46 | 0x47 | 0x48 | 0x49 | 0x4a | 0x4b | 0x4e => 2,
            0x4c | 0x4d => 4,
            0x4f => 2,
            _ => return Ok(None),
        };
        let args = pop_user_args(stack, argc)?;
        if id == 0x00 {
            let height = args.first().map(value_to_i32).unwrap_or(720).max(1);
            let width = args.get(1).map(value_to_i32).unwrap_or(1280).max(1);
            self.screen_width = width;
            self.screen_height = height;
            self.graph90_refresh_background_screen_layout();
            let handle = self.alloc_object();
            self.set_current_graph_object(handle);
            self.trace_graph(format!(
                "create screen graph object #{handle} {width}x{height}"
            ));
            return Ok(Some(Value::Int(handle)));
        }
        if id == 0x17 {
            let target = args.first().map(value_to_i32).unwrap_or_default();
            let (x, y) = self
                .graph_object_layers
                .get(&target)
                .and_then(|layers| layers.iter().next())
                .and_then(|layer| self.graph_layers.get(layer))
                .map(|layer| (layer.x.round() as i32, layer.y.round() as i32))
                .unwrap_or_default();
            stack.push(Value::Int(x));
            return Ok(Some(Value::Int(y)));
        }
        if self.title_ui_active && !self.title_child_program_active {
            if self.debug_graph {
                self.trace_graph(format!("title preload ignored user2 0x{id:02X}"));
            }
            return Ok(Some(Value::None));
        }
        match self.effects.call(group, id, &args) {
            crate::effects::EffectCall::Handled(value) => {
                self.trace_graph(format!(
                    "effect user 0x{id:02X} args={:?} result={value:?}",
                    args.iter().map(value_to_i32).collect::<Vec<_>>()
                ));
                return Ok(Some(value.map(Value::Int).unwrap_or(Value::None)));
            }
            crate::effects::EffectCall::Unhandled => {}
        }
        match id {
            0x01 => {
                if let Some(target) = args.first().map(value_to_i32) {
                    self.user_controls
                        .retain(|_, control| control.title_only || control.owner_id != target);
                    self.trace_graph(format!("user object release #{target}"));
                }
            }
            0x41 => {
                if let Some(target) = args.first().map(value_to_i32) {
                    self.user_controls
                        .retain(|_, control| control.title_only || control.owner_id != target);
                    self.trace_graph(format!("user object release-ex #{target}"));
                }
            }
            0x0f => {
                if let Some(target) = args.first().map(value_to_i32) {
                    self.trace_graph(format!("user object commit #{target}"));
                }
            }
            0x04 if args.len() >= 2 => {
                let enabled = value_to_i32(&args[0]) != 0;
                let target = value_to_i32(&args[1]);
                let control = self.user_controls.entry(target).or_default();
                control.id = target;
                control.owner_id = target;
                control.payload = target;
                control.enabled = enabled;
                control.title_only = false;
            }
            0x0d if args.len() >= 2 => {
                let target = value_to_i32(&args[1]);
                self.consume_user_control_event_for_object(target, "user2-duration");
            }
            0x28 if args.len() >= 4 => {
                let target = value_to_i32(&args[3]);
                let slot = value_to_i32(&args[2]);
                let (key, payload, x, y, width, height) = match avg_control_bounds(slot) {
                    Some((x, y, width, height)) => {
                        self.user_controls.retain(|_, control| {
                            control.title_only
                                || control.owner_id != target
                                || control.payload != slot
                        });
                        (
                            target.saturating_mul(100).saturating_add(slot),
                            slot,
                            x,
                            y,
                            width,
                            height,
                        )
                    }
                    None => {
                        let (x, y, width, height) = fallback_user_control_bounds(&args);
                        self.trace_graph(format!(
                            "fallback user bounds slot={slot} owner=#{target} x={x:.0} y={y:.0} w={width:.0} h={height:.0}"
                        ));
                        (
                            target.saturating_mul(100).saturating_add(slot),
                            slot,
                            x,
                            y,
                            width,
                            height,
                        )
                    }
                };
                let trace = {
                    let control = self.user_controls.entry(key).or_default();
                    control.id = key;
                    control.owner_id = target;
                    control.payload = payload;
                    control.x = x;
                    control.y = y;
                    control.width = width;
                    control.height = height;
                    control.enabled = true;
                    control.title_only = false;
                    format!(
                        "user bounds #{} owner=#{} payload={} x={} y={} w={} h={}",
                        control.id,
                        control.owner_id,
                        control.payload,
                        control.x,
                        control.y,
                        control.width,
                        control.height
                    )
                };
                self.trace_graph(trace);
            }
            0x4f if args.len() >= 2 => {
                let step = value_to_i32(&args[0]).max(0);
                let target = value_to_i32(&args[1]);
                let control = self.user_controls.entry(target).or_default();
                control.id = target;
                control.owner_id = target;
                control.payload = target;
                control.enabled = true;
                self.trace_graph(format!("user step #{target} step={step}"));
            }
            _ => {}
        }
        Ok(Some(Value::None))
    }

    fn consume_user_control_event_for_object(&mut self, object: i32, source: &str) -> bool {
        let Some(point) = self.pending_click.or(self.pending_object_state) else {
            return false;
        };
        let Some(control) = self
            .hit_test_user_control_for_object(point, object)
            .copied()
        else {
            return false;
        };
        if control.title_only {
            return false;
        }
        self.last_hit_control = control.id;
        self.last_hit_payload = control.payload;
        self.pending_click = None;
        self.pending_click_age_frames = 0;
        self.pending_object_state = None;
        self.pending_input_state = None;
        self.pending_input_descriptor = None;
        self.mouse_pressed = false;
        self.auto_title_release_after_state = false;
        self.trace_graph(format!(
            "user control event consumed source={source} object=#{object} hit=#{} payload={} at x={:.0} y={:.0}",
            control.id, control.payload, point.0, point.1
        ));
        true
    }

    pub(crate) fn maybe_auto_click_title(&mut self) {
        if !self.auto_title_click
            || self.auto_title_click_done
            || !self.title_ui_resources_loaded
            || !self.title_ui_active
        {
            return;
        }
        let Some(control) = self.user_controls.values().find(|control| {
            is_primary_title_control(control) && control.id == self.auto_title_click_id
        }) else {
            return;
        };
        let point = (
            control.x + control.width * 0.5,
            control.y + control.height * 0.5,
        );
        self.mouse_pos = Some(point);
        if self.auto_title_hovered_frames < self.auto_title_hover_frames {
            self.auto_title_hovered_frames += 1;
            self.trace_graph(format!(
                "auto title hover frame {}/{} at x={:.0} y={:.0}",
                self.auto_title_hovered_frames, self.auto_title_hover_frames, point.0, point.1
            ));
            return;
        }
        self.mouse_pressed = true;
        self.pending_click = Some(point);
        self.pending_click_age_frames = 0;
        self.pending_object_state = Some(point);
        self.pending_title_payload_override = self.auto_title_payload_override;
        self.pending_input_state = Some(0x1000_0002);
        self.pending_input_descriptor = Some(INPUT_DESCRIPTOR_MOUSE_LEFT);
        self.pending_input_consumed = false;
        self.auto_title_click_done = true;
        self.auto_title_release_after_state = true;
        tracing::info!(
            id = self.auto_title_click_id,
            payload_override = ?self.auto_title_payload_override,
            x = point.0,
            y = point.1,
            "auto title click injected"
        );
        self.trace_graph(format!(
            "auto title click at x={:.0} y={:.0}",
            point.0, point.1
        ));
    }
}

pub(crate) fn title_payload_for_control_text(text: &str) -> Option<i32> {
    let compact = text.trim();
    if compact.contains("オートセーブ") {
        Some(5)
    } else if compact.contains("シナリオを序章") {
        Some(0)
    } else if compact.contains("データを選んで再開") {
        Some(1)
    } else if compact.contains("好きな章") || compact.contains("Ｈシーン") {
        Some(7)
    } else if compact.contains("各種環境設定") {
        Some(2)
    } else if compact.contains("おまけ") || compact.eq_ignore_ascii_case("omake") {
        Some(3)
    } else if compact.contains("プログラムを終了") {
        Some(6)
    } else {
        None
    }
}

pub(crate) fn title_object_for_control_text(text: &str) -> Option<i32> {
    title_object_for_payload(title_payload_for_control_text(text)?)
}

fn title_object_for_payload(payload: i32) -> Option<i32> {
    match payload & 0xffff {
        0 => Some(6154),
        1 => Some(6159),
        2 => Some(6169),
        3 => Some(6174),
        5 => Some(6164),
        6 => Some(6179),
        7 => Some(6184),
        _ => None,
    }
}

pub(crate) fn title_payload_is_scenario(payload: i32) -> bool {
    matches!(payload & 0xffff, 0 | 5 | 8)
}

fn is_title_scenario_callback(payload: i32) -> bool {
    title_payload_is_scenario(payload)
}

fn is_primary_title_control(control: &RuntimeUserControl) -> bool {
    control.title_only
}

fn is_title_program_report(program: &str) -> bool {
    program_file_name(program).eq_ignore_ascii_case("title._bp")
}

fn should_depart_for_scenario(child_active: bool) -> bool {
    !child_active
}

fn is_scenario_driver_program_report(program: &str) -> bool {
    matches!(
        program_file_name(program).to_ascii_lowercase().as_str(),
        "scrdrv._bp" | "scrdrv2._bp"
    )
}

fn program_file_name(program: &str) -> &str {
    program
        .split('#')
        .next()
        .unwrap_or(program)
        .rsplit_once(':')
        .map(|(_, file)| file)
        .unwrap_or(program)
}

fn avg_control_bounds(slot: i32) -> Option<(f32, f32, f32, f32)> {
    match slot {
        8 => Some((48.0, 528.0, 104.0, 50.0)),
        9 => Some((410.0, 548.0, 70.0, 54.0)),
        10 => Some((486.0, 548.0, 68.0, 54.0)),
        11 => Some((556.0, 548.0, 78.0, 54.0)),
        12 => Some((632.0, 548.0, 98.0, 54.0)),
        13 => Some((742.0, 548.0, 76.0, 54.0)),
        14 => Some((812.0, 548.0, 92.0, 54.0)),
        15 => Some((910.0, 548.0, 82.0, 54.0)),
        16 => Some((988.0, 548.0, 96.0, 54.0)),
        17 => Some((1072.0, 548.0, 108.0, 54.0)),
        18 => Some((1178.0, 548.0, 78.0, 54.0)),
        19 => Some((48.0, 598.0, 104.0, 54.0)),
        20 => Some((1072.0, 598.0, 84.0, 54.0)),
        21 => Some((1154.0, 598.0, 72.0, 54.0)),
        22 => Some((1224.0, 598.0, 52.0, 54.0)),
        _ => None,
    }
}

fn fallback_user_control_bounds(args: &[Value]) -> (f32, f32, f32, f32) {
    let raw_width = value_to_i32(&args[0]).abs().max(1);
    let raw_y = value_to_i32(&args[1]);
    let raw_x = value_to_i32(&args[2]);

    let x = raw_x.clamp(0, 1279) as f32;
    let y = raw_y.clamp(0, 719) as f32;
    let max_width = (1280.0 - x).max(1.0);
    let width = (raw_width as f32).clamp(1.0, max_width);
    (x, y, width, 48.0)
}

fn pop_user_args(stack: &mut Vec<Value>, count: usize) -> ethornell_vm::VmResult<Vec<Value>> {
    let mut args = Vec::with_capacity(count);
    for _ in 0..count {
        args.push(stack.pop().ok_or(ethornell_vm::VmError::StackUnderflow)?);
    }
    Ok(args)
}

#[cfg(test)]
mod tests {
    use super::{
        is_scenario_driver_program_report, is_title_program_report, should_depart_for_scenario,
        title_object_for_control_text, title_payload_for_control_text,
    };

    #[test]
    fn title_text_uses_native_descriptor_callback_order() {
        let cases = [
            ("シナリオを序章からはじめる", 0, 6154),
            ("データを選んで再開", 1, 6159),
            ("オートセーブデータから再開", 5, 6164),
            ("各種環境設定", 2, 6169),
            ("おまけ", 3, 6174),
            ("プログラムを終了します", 6, 6179),
            ("好きな章から始めることができます", 7, 6184),
        ];
        for (text, payload, object) in cases {
            assert_eq!(title_payload_for_control_text(text), Some(payload));
            assert_eq!(title_object_for_control_text(text), Some(object));
        }
    }

    #[test]
    fn running_title_program_is_recognized_after_a_child_returns() {
        assert!(is_title_program_report("sysprg.arc:title._bp#instance=8"));
        assert!(is_title_program_report("TITLE._BP"));
        assert!(!is_title_program_report(
            "sysprg.arc:cnfgwnd._bp#instance=64"
        ));
    }

    #[test]
    fn nested_title_children_keep_the_title_ui_when_scrdrv_runs() {
        assert!(!should_depart_for_scenario(true));
        assert!(should_depart_for_scenario(false));
    }

    #[test]
    fn scenario_drivers_are_recognized_after_loading_save_data() {
        assert!(is_scenario_driver_program_report(
            "sysprg.arc:scrdrv._bp#instance=75"
        ));
        assert!(is_scenario_driver_program_report("SCRDRV2._BP"));
        assert!(!is_scenario_driver_program_report(
            "sysprg.arc:usdtwnd._bp#instance=64"
        ));
    }
}
