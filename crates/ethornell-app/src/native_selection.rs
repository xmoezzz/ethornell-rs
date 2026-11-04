//! CProcSelectItem / CProcSelectIcon families, from engine/selection.c.
use super::*;
use ethornell_vm::native_selection::{
    NativeSelectionCompletion, NativeSelectionData, NativeSelectionRequest,
};

#[derive(Debug, Default)]
pub(crate) struct SelectionRegistry {
    next_id: u64,
    states: BTreeMap<u64, Selection>,
    overlays: BTreeMap<(i32, usize), i32>,
}

#[derive(Debug)]
struct VirtualItem {
    scope: u32,
    rect: [i32; 4],
    mask: Option<(DecodedImage, i32)>,
}

#[derive(Debug)]
struct Selection {
    request: NativeSelectionRequest,
    scope: i32,
    current: i32,
    previous_hover: i32,
    pointer_eligible: bool,
    previous_pointer: Option<(f32, f32)>,
    local: [i32; 2],
    rects: Vec<[i32; 4]>,
    bracket_images: Option<[Option<DecodedImage>; 2]>,
    virtuals: Vec<VirtualItem>,
    pointer_plain: bool,
    keyboard_plain: bool,
    pointer_window: bool,
    focus: bool,
    initialized: bool,
    blink: usize,
    deadline: u64,
    confirmation_ticks: Option<usize>,
}

impl RuntimeTraceApi {
    pub(super) fn start_selection(
        &mut self,
        request: NativeSelectionRequest,
    ) -> ethornell_vm::VmResult<u64> {
        let selector = request.opcode.id;
        let error = |reason: &str| {
            ethornell_vm::VmError::Runtime(format!("Graph90:{:02X} {reason}", selector))
        };
        let window = request.window;
        if !self.graph_surfaces.contains_key(&window) {
            return Err(error("invalid Window"));
        }
        let scope = self
            .graph90_native_sort_key(window)
            .ok_or_else(|| error("Window has no native sort key"))? as i32;
        let mut state = Selection {
            scope,
            current: -1,
            previous_hover: -1,
            pointer_eligible: false,
            bracket_images: None,
            previous_pointer: self.mouse_pos,
            local: [0; 2],
            rects: Vec::new(),
            virtuals: Vec::new(),
            pointer_plain: false,
            keyboard_plain: false,
            pointer_window: false,
            focus: false,
            initialized: false,
            blink: 0,
            deadline: self.engine_time_ms,
            confirmation_ticks: None,
            request,
        };
        self.clear_selection_overlays(window);
        match &state.request.data {
            NativeSelectionData::Items {
                text,
                columns,
                centered,
                color,
                initial,
                overlays,
                ..
            } => {
                if overlays.is_some() {
                    for [bitmap, _, _] in overlays.as_ref().unwrap() {
                        if *bitmap != -1 && self.graph_bitmap_image(*bitmap).is_none() {
                            return Err(error("selection bracket bitmap does not exist"));
                        }
                    }
                }
                if self
                    .surface_text_states
                    .get(&window)
                    .is_none_or(|s| s.font_size <= 0)
                {
                    return Err(error("Window font is not configured"));
                }
                if !self.draw_window_item_grid(window, text, *columns, *centered, *color) {
                    return Err(error("could not draw selection text"));
                }
                let glyph = self
                    .window_text_state(Some(window))
                    .font_size
                    .round()
                    .max(1.0) as i32;
                if let Some(nodes) = self.item_grid_nodes.get(&window) {
                    for node in nodes.iter().filter_map(|id| self.text_nodes.get(id)) {
                        let width = selection_text_width(&node.text, glyph);
                        state.rects.push([
                            node.x as i32,
                            node.y as i32,
                            (node.x as i32).wrapping_add(width).wrapping_sub(1),
                            (node.y as i32).wrapping_add(glyph).wrapping_sub(1),
                        ]);
                    }
                }
                state.bracket_images =
                    overlays.map(|brackets| brackets.map(|b| self.graph_bitmap_image(b[0])));
                state.current = *initial;
                if matches!(
                    state.request.data,
                    NativeSelectionData::Items {
                        blink_confirmation: true,
                        ..
                    }
                ) {
                    state.confirmation_ticks = Some(0);
                }
                state.pointer_plain = true;
                state.keyboard_plain = true;
                state.focus = true;
            }
            NativeSelectionData::Icons {
                records,
                extended,
                key_assignments,
                input_mode,
                alpha_hit_test,
            } => {
                let surface = &self.graph_surfaces[&window];
                let origin = [surface.valid_left, surface.valid_top];
                let mut icons = Vec::new();
                for record in records {
                    let image = self
                        .graph_bitmap_image(record[2])
                        .ok_or_else(|| error("icon bitmap does not exist"))?;
                    let x = origin[0].wrapping_add(record[0]);
                    let y = origin[1].wrapping_add(record[1]);
                    let rect = [
                        x,
                        y,
                        x.wrapping_add(image.width as i32).wrapping_sub(1),
                        y.wrapping_add(image.height as i32).wrapping_sub(1),
                    ];
                    let mask_bitmap = if *extended && record[15] != -1 {
                        record[15]
                    } else {
                        record[2]
                    };
                    let mask = if *alpha_hit_test {
                        self.graph_bitmap_image(mask_bitmap).map(|image| {
                            (
                                image,
                                self.bitmap_formats.get(&mask_bitmap).copied().unwrap_or(2),
                            )
                        })
                    } else {
                        None
                    };
                    state.virtuals.push(VirtualItem {
                        scope: (scope as u32).wrapping_add(0x8000),
                        rect,
                        mask,
                    });
                    state.rects.push(rect);
                    icons.push(ethornell_vm::GraphIconRecord {
                        x: record[0],
                        y: record[1],
                        bitmap: record[2],
                        parameter: if *extended { record[2] } else { record[3] },
                    });
                }
                if !ethornell_vm::GraphApi::draw_graph_icon_batch(self, window, &icons) {
                    return Err(error("could not draw icon selection"));
                }
                state.pointer_plain = *input_mode == 3;
                state.keyboard_plain = *input_mode == 3 || key_assignments.is_some();
                state.pointer_window = matches!(*input_mode, 1 | 2);
                state.focus = *input_mode == 3;
                if let Some(keys) = key_assignments {
                    for key in keys {
                        let _ = drain_native_input_descriptor(self, *key);
                    }
                }
            }
        }
        if state.pointer_plain {
            self.registered_pointer_input_scopes.register(scope);
        }
        if state.keyboard_plain {
            self.registered_keyboard_input_scopes.register(scope);
        }
        if state.pointer_window {
            self.register_pointer_object_node(window);
        }
        self.native_selections.next_id = self.native_selections.next_id.wrapping_add(1).max(1);
        let id = self.native_selections.next_id;
        self.native_selections.states.insert(id, state);
        // Drain constructor-time Window/Virtual input after all nodes exist.
        let _ = query_runtime_input_event_bits(self, scope, false, true);
        if let Some(scope) = self.native_selections.states[&id]
            .virtuals
            .first()
            .map(|v| v.scope as i32)
        {
            let _ = query_runtime_input_event_bits(self, scope, false, true);
        }
        if matches!(
            self.native_selections.states[&id].request.data,
            NativeSelectionData::Icons { .. }
        ) {
            let mut state = self.native_selections.states.remove(&id).unwrap();
            let hit = self.selection_icon_hit(&mut state);
            self.update_selection_icon(&mut state, hit);
            self.native_selections.states.insert(id, state);
        }
        Ok(id)
    }

    pub(super) fn poll_selection(
        &mut self,
        id: u64,
        callbacks: &[[i32; 2]],
        cancelled: bool,
    ) -> Option<NativeSelectionCompletion> {
        let state = self.native_selections.states.get(&id)?;
        let scope = state.scope;
        let window = state.request.window;
        let icon = matches!(state.request.data, NativeSelectionData::Icons { .. });
        let full_input = match &state.request.data {NativeSelectionData::Items {..}=>true,NativeSelectionData::Icons {input_mode,..}=>*input_mode==3};
        let window_input = match &state.request.data {
            NativeSelectionData::Items { .. } => true,
            NativeSelectionData::Icons { input_mode, .. } => *input_mode != 0,
        };
        let virtual_scope = state.virtuals.first().map(|v| v.scope as i32);
        let focus = self.selection_focus_eligible(scope as u32);
        let keyboard = native_keyboard_scope_eligible(self, scope);
        let mut bits = if window_input {
            query_runtime_input_event_bits_split(self,if full_input {scope}else{0},scope,true) as u32 & 0x7fff_ffff
        } else {
            0
        };
        if let Some(vscope) = virtual_scope {
            bits |= query_runtime_input_event_bits_split(self,0,vscope,true) as u32 & 0x7fff_ffff;
        }
        let mut state = self.native_selections.states.remove(&id).unwrap();
        let pointer_eligible =
            focus && (self.graph_interactive_procedure_poll_gate == 0 || self.window_focused);
        let moved = state.previous_pointer != self.mouse_pos;
        let eligible_changed = state.pointer_eligible != pointer_eligible;
        state.previous_pointer = self.mouse_pos;
        state.pointer_eligible = pointer_eligible;
        if !state.request.allow_cancel {
            bits &= !0x202;
        }
        let mut finished = cancelled;
        if icon {
            let mut forced = false;
            for &[code, value] in callbacks {
                if code == 512 {
                    forced = true;
                    let count = state.rects.len() as i32;
                    if (0..count).contains(&value) {
                        state.current = value;
                        state.local = [0, 0];
                    } else {
                        state.current = -1;
                    }
                }
            }
            finished |= forced;
            if !finished && keyboard {
                if let NativeSelectionData::Icons {
                    key_assignments: Some(keys),
                    ..
                } = &state.request.data
                {
                    if let Some(index) = keys.iter().position(|key| {
                        drain_native_input_descriptor(self, *key) != 0
                            || crate::native_input_chain::native_action_descriptor_bit(bits,*key)
                    }) {
                        self.update_selection_icon(&mut state, index as i32);
                        state.local = [0, 0];
                        finished = true;
                    }
                }
            }
            if !finished && (bits != 0 || moved || eligible_changed) {
                if bits & 0x202 != 0 {
                    self.update_selection_icon(&mut state, -1);
                    finished = true;
                } else {
                    let hit = self.selection_icon_hit(&mut state);
                    if hit != state.current {
                        let highlight = match &state.request.data {
                            NativeSelectionData::Icons {
                                records, extended, ..
                            } => hit >= 0 && (*extended || records[hit as usize][3] != -1),
                            _ => false,
                        };
                        self.queued_system_events.push_back([
                            0x2000_0001,
                            state.request.owner_thread_id,
                            (hit as u16 as i32) | ((highlight as i32) << 16),
                        ]);
                        self.update_selection_icon(&mut state, hit);
                    }
                    finished = bits & 1 != 0 && state.current != -1;
                }
            }
        } else {
            let extended = matches!(
                state.request.data,
                NativeSelectionData::Items {
                    overlays: Some(_),
                    ..
                }
            );
            bits &= if extended { 0x03ff_0203 } else { 0x03ff_f3c3 };
            if state.confirmation_ticks.is_some() {
                finished |= self.tick_selection_confirmation(&mut state);
            } else if !finished {
                let old = state.current;
                let mut action = 0;
                if bits != 0 {
                    action = self.selection_item_action(&mut state, bits);
                } else if (moved || eligible_changed) && (extended || pointer_eligible) {
                    let hit = self.selection_item_hit(&state);
                    if extended || hit >= 0 {
                        if state.current != hit {
                            state.current = hit;
                            action = 2;
                        }
                    }
                    if (extended && old != hit) || (!extended && hit != state.previous_hover) {
                        state.previous_hover = hit;
                        self.queued_system_events.push_back([
                            0x1000_0002,
                            state.request.owner_thread_id,
                            hit,
                        ]);
                    }
                }
                if old != state.current {
                    state.blink = 0;
                }
                if !state.initialized || action != 0 || old != state.current {
                    self.update_selection_item(&mut state);
                    state.initialized = true;
                }
                if action == 1 {
                    let blink = matches!(
                        state.request.data,
                        NativeSelectionData::Items {
                            blink_confirmation: true,
                            ..
                        }
                    );
                    if blink && state.current != -1 {
                        state.confirmation_ticks = Some(0);
                        finished = self.tick_selection_confirmation(&mut state);
                    } else {
                        finished = true;
                    }
                } else if action == 2 {
                    self.queued_system_events.push_back([
                        0x1000_0001,
                        state.request.owner_thread_id,
                        state.current,
                    ]);
                } else if !extended && self.engine_time_ms >= state.deadline {
                    state.blink ^= 1;
                    self.update_selection_item(&mut state);
                }
            }
        }
        if finished {
            if state.pointer_plain {
                self.registered_pointer_input_scopes.unregister_one(scope);
            }
            if state.keyboard_plain {
                self.registered_keyboard_input_scopes.unregister_one(scope);
            }
            if state.pointer_window {
                self.unregister_pointer_object_node(window);
            }
            let values = if icon {
                [
                    state.local[0],
                    state.local[1],
                    if cancelled { -1 } else { state.current },
                ]
            } else {
                [state.current, if cancelled { -1 } else { state.current }, 0]
            };
            Some(NativeSelectionCompletion {
                values,
                count: if icon { 3 } else { 2 },
            })
        } else {
            self.native_selections.states.insert(id, state);
            None
        }
    }

    fn tick_selection_confirmation(&mut self, state: &mut Selection) -> bool {
        let ticks = state.confirmation_ticks.unwrap_or(0);
        if ticks >= 20 || state.current == -1 {
            return true;
        }
        if self.engine_time_ms >= state.deadline {
            if ticks == 0 {
                self.update_selection_item(state);
            } else {
                state.blink ^= 1;
                self.update_selection_item(state);
            }
            state.deadline = self.engine_time_ms.saturating_add(50);
            state.confirmation_ticks = Some(ticks + 1);
        }
        false
    }

    pub(super) fn selection_focus_eligible(&self, scope: u32) -> bool {
        self.icon_input_focus_eligible(scope) && self.native_selections
            .states
            .values()
            .filter(|p| p.focus)
            .all(|p| scope >= p.scope as u32)
    }

    pub(super) fn selection_pointer_nodes_at(&self,point:Option<(f32,f32)>) -> Vec<(u32, bool)> {
        let Some((x, y)) = point else {
            return Vec::new();
        };
        if x < 0.0 || y < 0.0 || x >= snapshot::runtime_frame_size(self).0 as f32 || y >= snapshot::runtime_frame_size(self).1 as f32 {
            return Vec::new();
        }
        let mut nodes = Vec::new();
        for state in self.native_selections.states.values() {
            if !self.graph_object_is_drawable(state.request.window) {
                continue;
            }
            for item in &state.virtuals {
                nodes.push((
                    item.scope,
                    self.selection_virtual_hit_at(state.request.window, item,point)
                        .is_some(),
                ));
            }
        }
        nodes
    }

    fn selection_virtual_hit(&self, window: i32, item: &VirtualItem) -> Option<[i32; 2]> {
        self.selection_virtual_hit_at(window,item,self.mouse_pos)
    }

    fn selection_virtual_hit_at(&self, window:i32,item:&VirtualItem,point:Option<(f32,f32)>)->Option<[i32;2]> {
        let (x,y)=point?;
        let (wx, wy) = self.surface_world_position(window);
        let px = (x as i32)
            .wrapping_sub(wx as i32)
            .wrapping_sub(item.rect[0]);
        let py = (y as i32)
            .wrapping_sub(wy as i32)
            .wrapping_sub(item.rect[1]);
        if px < 0
            || py < 0
            || (x as i32).wrapping_sub(wx as i32) > item.rect[2]
            || (y as i32).wrapping_sub(wy as i32) > item.rect[3]
        {
            return None;
        }
        if let Some((mask, format)) = &item.mask {
            if px as u32 >= mask.width || py as u32 >= mask.height {
                return None;
            }
            let offset = ((py as u32 * mask.width + px as u32) * 4) as usize;
            let p = &mask.rgba[offset..offset + 4];
            let hit = match format {
                0 | 1 => p[..3].iter().any(|b| *b != 0),
                2 => p[3] != 0,
                3 => p[0] != 0,
                _ => false,
            };
            if !hit {
                return None;
            }
        }
        Some([px, py])
    }

    fn selection_icon_hit(&self, state: &mut Selection) -> i32 {
        if !state.pointer_eligible {
            return -1;
        }
        for (i, item) in state.virtuals.iter().enumerate() {
            if let Some(point) = self.selection_virtual_hit(state.request.window, item) {
                state.local = point;
                return i as i32;
            }
        }
        -1
    }

    fn selection_item_hit(&self, state: &Selection) -> i32 {
        if !state.pointer_eligible {
            return -1;
        }
        let Some((x, y)) = self.mouse_pos else {
            return -1;
        };
        let (wx, wy) = self.surface_world_position(state.request.window);
        let point = [
            (x as i32).wrapping_sub(wx as i32),
            (y as i32).wrapping_sub(wy as i32),
        ];
        state
            .rects
            .iter()
            .position(|r| {
                point[0] >= r[0] && point[0] <= r[2] && point[1] >= r[1] && point[1] <= r[3]
            })
            .map_or(-1, |i| i as i32)
    }

    fn selection_item_action(&mut self, state: &mut Selection, bits: u32) -> i32 {
        let NativeSelectionData::Items { columns, .. } = &state.request.data else {
            return 0;
        };
        let columns = *columns;
        let count = state.rects.len() as i32;
        if bits & 0x03ff_0000 != 0 {
            for i in 0..10 {
                if bits & (0x10000 << i) != 0 && i < count {
                    state.current = i;
                    return 1;
                }
            }
            return 0;
        }
        if bits & 0x303 != 0 {
            if bits & 0x202 != 0 {
                state.current = -1;
                return 1;
            }
            if bits & 1 == 0 {
                return 1;
            }
            let hit = self.selection_item_hit(state);
            if hit >= 0 {
                state.current = hit;
                return 1;
            }
            return 0;
        }
        let keys: &[u32] = if self.graph_item_selection_navigation[0] != 0 {
            &[0x40, 0x80, 0x1000, 0x2000, 0x4000, 0x8000]
        } else {
            &[0x1000, 0x2000, 0x4000, 0x8000]
        };
        let Some(key) = keys.iter().find(|k| bits & **k != 0).copied() else {
            return 0;
        };
        let mut col = state.current % columns;
        let mut row = state.current / columns;
        let rows = (count + columns - 1) / columns;
        let vertical = matches!(key, 0x40 | 0x80 | 0x1000 | 0x2000);
        for _ in 0..if vertical { rows } else { columns } {
            if vertical {
                row = (row + if key == 0x40 || key == 0x1000 { -1 } else { 1 }).rem_euclid(rows);
            } else {
                col = (col + if key == 0x4000 { -1 } else { 1 }).rem_euclid(columns);
            }
            let next = col + row * columns;
            if next < count {
                if key == 0x40 || key == 0x80 {
                    let rect = state.rects[next as usize];
                    let (wx, wy) = self.surface_world_position(state.request.window);
                    let point = (
                        wx + (rect[0] + ((rect[2] - rect[0] + 1) >> 1)) as f32,
                        wy + (rect[1] + ((rect[3] - rect[1] + 1) >> 1)) as f32,
                    );
                    self.configure_cursor_motion(
                        0,
                        self.graph_item_selection_navigation[3],
                        self.graph_item_selection_navigation[2],
                        self.graph_item_selection_navigation[1],
                        point.1 as i32,
                        point.0 as i32,
                    );
                } else {
                    state.current = next;
                }
                break;
            }
        }
        2
    }

    fn update_selection_item(&mut self, state: &mut Selection) {
        let window = state.request.window;
        if state.current < 0 {
            self.clear_selection_overlays(window);
            return;
        }
        let index = state.current as usize;
        let NativeSelectionData::Items { text, overlays, .. } = &state.request.data else {
            return;
        };
        let rect = state.rects[index];
        let style = self.graph_item_selection_highlight_styles[state.blink];
        // Window overlay slot 0 is the selected text, independently toggled
        // by the two A4 colors. Ex brackets occupy slots 1 and 2.
        let node_id = self.selection_overlay_node(window, 0);
        if style == 0 {
            self.text_nodes.remove(&node_id);
        } else {
            let size = self.window_text_state(Some(window)).font_size;
            self.text_nodes.insert(
                node_id,
                RuntimeTextNode {
                    text: text[index].clone(),
                    enabled: true,
                    owner_object: None,
                    screen_attached: false,
                    target_surface: Some(window),
                    x: rect[0] as f32,
                    y: rect[1] as f32,
                    size,
                    line_height: size,
                    formatted_layout: false,
                    color: [
                        ((style >> 16) & 255) as f32 / 255.0,
                        ((style >> 8) & 255) as f32 / 255.0,
                        (style & 255) as f32 / 255.0,
                        1.0,
                    ],
                    z: text::MESSAGE_TEXT_Z + 1,
                    ruby_spans: Vec::new(),
                    style_spans: Vec::new(),
                },
            );
        }
        if let Some(overlays) = overlays {
            // The original accidentally gates both brackets on first_present.
            if overlays[0][0] != -1 {
                for (i, &[bitmap, dx, dy]) in overlays.iter().enumerate() {
                    if bitmap != -1 {
                        let x = rect[0] + dx + if i == 1 { rect[2] - rect[0] + 1 } else { 0 };
                        if let Some(image) =
                            state.bracket_images.as_ref().and_then(|b| b[i].clone())
                        {
                            self.set_selection_image_overlay(window, i + 1, image, x, rect[1] + dy);
                        }
                    }
                }
            }
        }
        let interval = if matches!(
            state.request.data,
            NativeSelectionData::Items {
                blink_confirmation: true,
                ..
            }
        ) {
            50
        } else {
            self.graph_item_selection_input_mask
        };
        self.graph_redraw_requested = Some(false);
        state.deadline = self.engine_time_ms.wrapping_add(interval as u32 as u64);
    }

    fn update_selection_icon(&mut self, state: &mut Selection, selected: i32) {
        let window = state.request.window;
        let NativeSelectionData::Icons {
            records, extended, ..
        } = &state.request.data
        else {
            return;
        };
        if *extended {
            self.clear_selection_overlays(window);
            if selected >= 0 {
                let surface = &self.graph_surfaces[&window];
                let origin = [surface.valid_left, surface.valid_top];
                // The native loop reads eight triples from a 64-byte record,
                // spilling into the following record. Preserve those reads
                // within the copied table; absent/uninitialized words are 0.
                let words = |i: usize| records.get(i / 16).map_or(0, |r| r[i % 16]);
                for slot in 0..8 {
                    let base = selected as usize * 16 + 3 + slot * 3;
                    self.set_selection_bitmap_overlay(
                        window,
                        slot,
                        words(base + 2),
                        origin[0].wrapping_add(words(base)),
                        origin[1].wrapping_add(words(base + 1)),
                    );
                }
            }
        } else {
            if state.current >= 0 {
                let r = records[state.current as usize];
                // 43AE20 clears/redraws only when the old highlight resolves.
                if self.graph_bitmap_image(r[3]).is_some() {
                    if let Some(surface) = self.graph_surfaces.get(&window) {
                        let (x, y) = (surface.valid_left + r[0], surface.valid_top + r[1]);
                        let _ = self.composite_graph_bitmap(window, r[3], x, y, 0x40, 1);
                        let _ = self.composite_graph_bitmap(window, r[2], x, y, 0, 0);
                    }
                }
            }
            if selected >= 0 {
                let r = records[selected as usize];
                if self.graph_bitmap_image(r[3]).is_some() {
                    if let Some(surface) = self.graph_surfaces.get(&window) {
                        let (x, y) = (surface.valid_left + r[0], surface.valid_top + r[1]);
                        let _ = self.composite_graph_bitmap(window, r[2], x, y, 0x40, 1);
                        let _ = self.composite_graph_bitmap(window, r[3], x, y, 0, 0);
                    }
                }
            }
        }
        state.current = selected;
        self.graph_redraw_requested = Some(false);
    }

    fn selection_overlay_node(&mut self, window: i32, slot: usize) -> i32 {
        if let Some(node) = self.native_selections.overlays.get(&(window, slot)) {
            return *node;
        }
        let node = self.alloc_node();
        self.native_selections.overlays.insert((window, slot), node);
        node
    }

    fn clear_selection_overlays(&mut self, window: i32) {
        let ids = self
            .native_selections
            .overlays
            .iter()
            .filter_map(|(&(w, _), &id)| (w == window).then_some(id))
            .collect::<Vec<_>>();
        for id in ids {
            self.graph_layers.remove(&id);
            self.release_screen_text_node(id);
        }
    }

    fn set_selection_bitmap_overlay(
        &mut self,
        window: i32,
        slot: usize,
        bitmap: i32,
        x: i32,
        y: i32,
    ) {
        let Some(image) = self.graph_bitmap_image(bitmap) else {
            if let Some(node) = self.native_selections.overlays.get(&(window, slot)) {
                self.graph_layers.remove(node);
            }
            return;
        };
        self.set_selection_image_overlay(window, slot, image, x, y);
    }

    fn set_selection_image_overlay(
        &mut self,
        window: i32,
        slot: usize,
        image: DecodedImage,
        x: i32,
        y: i32,
    ) {
        let node = self.selection_overlay_node(window, slot);
        let source = RuntimeClipRect {
            x: 0.0,
            y: 0.0,
            width: image.width as f32,
            height: image.height as f32,
        };
        let key = format!("runtime:selection:{window}:{slot}");
        self.store_graph_image(key.clone(), image);
        self.graph_layers.insert(
            node,
            RuntimeGraphLayer {
                hit_id: node,
                owner_object: None,
                key,
                target_surface: Some(window),
                x: x as f32,
                y: y as f32,
                width: source.width,
                height: source.height,
                src_x: source.x,
                src_y: source.y,
                opacity: 1.0,
                z: text::MESSAGE_TEXT_Z + 2 + slot as i32,
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
    }
}

fn selection_text_width(text: &str, glyph: i32) -> i32 {
    text.chars()
        .map(|ch| {
            if encoding_rs::SHIFT_JIS
                .encode(ch.encode_utf8(&mut [0; 4]))
                .0
                .len()
                > 1
            {
                glyph
            } else {
                glyph / 2
            }
        })
        .sum()
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::NativeOpcode;

    fn runtime() -> (RuntimeTraceApi, i32) {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let window = api.alloc_window_surface(320, 96).unwrap();
        api.surface_text_states.entry(window).or_default().font_size = 20;
        (api, window)
    }

    fn items(window: i32, id: u16) -> NativeSelectionRequest {
        NativeSelectionRequest {
            owner_thread_id: 7,
            opcode: NativeOpcode { group: 0x90, id },
            window,
            allow_cancel: true,
            data: NativeSelectionData::Items {
                text: vec!["One".into(), "Two".into(), "Three".into()],
                columns: 2,
                centered: false,
                color: 0xff0000,
                initial: 0,
                overlays: (id != 0xA0).then_some([[-1, 0, 0]; 2]),
                blink_confirmation: id == 0xA3,
            },
        }
    }

    #[test]
    fn native_selection_item_navigation_and_thread_result() {
        let (mut api, window) = runtime();
        let id = api.start_selection(items(window, 0xA0)).unwrap();
        assert!(api.poll_selection(id, &[], false).is_none());
        apply_runtime_input_event(&mut api, RuntimeInputEvent::KeyPress { descriptor: 39 });
        assert!(api.poll_selection(id, &[], false).is_none());
        assert_eq!(api.native_selections.states[&id].current, 1);
        assert_eq!(
            api.queued_system_events.pop_back(),
            Some([0x10000001, 7, 1])
        );
        apply_runtime_input_event(&mut api, RuntimeInputEvent::KeyPress { descriptor: 13 });
        assert_eq!(
            api.poll_selection(id, &[], false).unwrap().values,
            [1, 1, 0]
        );
        assert!(api.native_selections.states.is_empty());
    }

    #[test]
    fn native_selection_blink_runs_without_confirmation_input() {
        let (mut api, window) = runtime();
        let id = api.start_selection(items(window, 0xA3)).unwrap();
        for tick in 0..20 {
            api.engine_time_ms = tick * 50;
            assert!(api.poll_selection(id, &[], false).is_none());
        }
        api.engine_time_ms = 1000;
        assert_eq!(
            api.poll_selection(id, &[], false).unwrap().values,
            [0, 0, 0]
        );
    }

    #[test]
    fn native_selection_icon_forced_callback_keeps_three_outputs() {
        let (mut api, window) = runtime();
        api.store_graph_image(
            "test:selection-icon".into(),
            DecodedImage {
                width: 2,
                height: 2,
                rgba: vec![255; 16],
            },
        );
        api.graph_resources
            .insert(8, RuntimeGraphResource::whole("test:selection-icon".into()));
        api.bitmap_formats.insert(8, 2);
        let mut record = [0; 16];
        record[2] = 8;
        record[3] = -1;
        let id = api
            .start_selection(NativeSelectionRequest {
                owner_thread_id: 7,
                opcode: NativeOpcode {
                    group: 0x90,
                    id: 0xB0,
                },
                window,
                allow_cancel: true,
                data: NativeSelectionData::Icons {
                    records: vec![record],
                    extended: false,
                    key_assignments: None,
                    input_mode: 3,
                    alpha_hit_test: true,
                },
            })
            .unwrap();
        let completion = api.poll_selection(id, &[[512, 0]], false).unwrap();
        assert_eq!(completion.count, 3);
        assert_eq!(completion.values, [0, 0, 0]);
        assert!(api.native_selections.states.is_empty());
    }
}
