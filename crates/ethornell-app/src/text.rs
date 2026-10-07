use super::{NATIVE_DISPLAY_Z, RuntimeTraceApi, value_to_i32};
use crate::display_tree::NativeDisplayKind;
use crate::graph::{RuntimeGraphLayer, RuntimeUserControl};
use crate::ruby_registry::RubyRegistry;
use crate::text_anim::{
    ParsedMessageMarkup, RuntimeLinkSpan, RuntimeRubySpan, RuntimeTextStyleSpan,
    RuntimeTimelineMark, normalize_message_text, parse_message_markup_with,
};
use std::collections::BTreeMap;

const MESSAGE_TEXT_NODE_ID: i32 = -20_000;
const MESSAGE_NAME_TEXT_NODE_ID: i32 = -20_001;
const MESSAGE_OVERLAY_TEXT_NODE_ID: i32 = -20_050;
pub(crate) const MESSAGE_WINDOW_LAYER_ID: i32 = -20_010;
pub(crate) const MESSAGE_NAME_WINDOW_LAYER_ID: i32 = -20_011;
const MESSAGE_WINDOW_KEY: &str = "sysgrp.arc:SGMsgWnd800000";
const MESSAGE_WINDOW_RESOURCE: &str = "SGMsgWnd800000";
const MESSAGE_NAME_WINDOW_KEY: &str = "sysgrp.arc:SGMsgWnd700000";
const MESSAGE_NAME_WINDOW_RESOURCE: &str = "SGMsgWnd700000";
const MESSAGE_WINDOW_Z: i32 = 900;
const MESSAGE_NAME_WINDOW_Z: i32 = 901;
pub(crate) const MESSAGE_TEXT_Z: i32 = 3_000;
pub(crate) const MESSAGE_NAME_TEXT_Z: i32 = 3_001;
const MESSAGE_INPUT_OBJECT_ID: i32 = 50_000;
const MESSAGE_NAME_WINDOW_X: f32 = 0.0;
const MESSAGE_NAME_WINDOW_Y: f32 = 466.0;
const MESSAGE_NAME_TEXT_X: f32 = 72.0;
const MESSAGE_NAME_TEXT_Y: f32 = 525.0;
pub(crate) const MESSAGE_CONTROL_OWNER_ID: i32 = -20_020;
const MESSAGE_CONTROL_NORMAL_RESOURCE: &str = "SGMsgWnd000000";
const MESSAGE_CONTROL_HOVER_RESOURCE: &str = "SGMsgWnd000001";
const MESSAGE_CONTROL_ACTIVE_RESOURCE: &str = "SGMsgWnd000002";
const MESSAGE_CONTROL_DISABLED_RESOURCE: &str = "SGMsgWnd000003";

const MESSAGE_CONTROL_TEMPLATES: [MessageControlTemplate; 11] = [
    MessageControlTemplate::new(1036, 8, 6.0, 553.0, 380.0, 60.0),
    MessageControlTemplate::new(1052, 9, 413.0, 549.0, 65.0, 55.0),
    MessageControlTemplate::new(1048, 10, 488.0, 549.0, 55.0, 55.0),
    MessageControlTemplate::new(1056, 11, 554.0, 549.0, 55.0, 55.0),
    MessageControlTemplate::new(1060, 12, 620.0, 549.0, 81.0, 55.0),
    MessageControlTemplate::new(1024, 13, 731.0, 549.0, 65.0, 55.0),
    MessageControlTemplate::new(1040, 14, 803.0, 549.0, 76.0, 55.0),
    MessageControlTemplate::new(1028, 15, 890.0, 549.0, 67.0, 55.0),
    MessageControlTemplate::new(1044, 16, 963.0, 549.0, 79.0, 55.0),
    MessageControlTemplate::new(1032, 17, 1075.0, 549.0, 75.0, 55.0),
    MessageControlTemplate::new(1072, 18, 1168.0, 549.0, 67.0, 55.0),
];

#[derive(Debug, Clone, Copy)]
struct MessageControlTemplate {
    layer_id: i32,
    payload: i32,
    x: f32,
    y: f32,
    width: f32,
    height: f32,
}

impl MessageControlTemplate {
    const fn new(layer_id: i32, payload: i32, x: f32, y: f32, width: f32, height: f32) -> Self {
        Self {
            layer_id,
            payload,
            x,
            y,
            width,
            height,
        }
    }
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeTextNode {
    pub(crate) text: String,
    pub(crate) enabled: bool,
    pub(crate) owner_object: Option<i32>,
    pub(crate) screen_attached: bool,
    pub(crate) target_surface: Option<i32>,
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) size: f32,
    pub(crate) line_height: f32,
    pub(crate) formatted_layout: bool,
    pub(crate) color: [f32; 4],
    pub(crate) z: i32,
    pub(crate) ruby_spans: Vec<RuntimeRubySpan>,
    pub(crate) style_spans: Vec<RuntimeTextStyleSpan>,
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub(crate) struct RuntimeFormattedTextLayout {
    pub(crate) body_y_offset: f32,
    pub(crate) line_height: f32,
    ruby_height: f32,
    ruby_x_offset: f32,
    ruby_y_offset: f32,
}

impl RuntimeFormattedTextLayout {
    pub(crate) fn plain(node: &RuntimeTextNode) -> Self {
        Self {
            body_y_offset: 0.0,
            line_height: node.line_height.max(node.size).max(1.0),
            ruby_height: 0.0,
            ruby_x_offset: 0.0,
            ruby_y_offset: 0.0,
        }
    }
}

#[derive(Debug, Clone)]
pub(crate) struct TextState {
    pub(crate) x: f32,
    pub(crate) y: f32,
    pub(crate) width: f32,
    pub(crate) height: f32,
    pub(crate) font_size: f32,
    pub(crate) color: [f32; 4],
    pub(crate) line_height: f32,
}

impl Default for TextState {
    fn default() -> Self {
        Self {
            x: 54.0,
            y: 616.0,
            width: 1160.0,
            height: 82.0,
            font_size: 26.0,
            color: [1.0, 1.0, 1.0, 1.0],
            line_height: 32.0,
        }
    }
}

impl TextState {
    pub(crate) fn message_window() -> Self {
        Self::default()
    }
}

impl RuntimeTraceApi {
    pub(crate) fn observe_user_message(
        &mut self,
        group: u8,
        id: u16,
        stack: &[ethornell_vm::Value],
    ) {
        if (group, id) != (0xb0, 0x80) {
            return;
        }
        let text = stack.iter().rev().find_map(value_to_text_string);
        self.trace_graph(format!("native message box suppressed text={text:?}"));
    }

    pub(crate) fn render_graph_text(&mut self, args: &[ethornell_vm::Value]) {
        let text = match args.len() {
            // Graph91:9C/9D immediate bitmap text. pop_args stores native
            // arguments in top-to-bottom pop order; 9D has one additional
            // leading style-mode pop.
            14 => args.get(10).and_then(value_to_text_string),
            15 => args.get(11).and_then(value_to_text_string),
            // RenderText(target, x, y, text, ...).
            21 => args.get(17).and_then(value_to_text_string),
            _ => args.iter().find_map(value_to_text_string),
        };
        let Some(text) = text else {
            self.trace_graph(format!(
                "RenderText without string args={:?}",
                args.iter().map(value_to_i32).collect::<Vec<_>>()
            ));
            return;
        };
        if text.is_empty() {
            return;
        }
        if args.len() == 21 {
            let target = args.get(20).map(value_to_i32).unwrap_or_default();
            if target != -1 && self.graph_surfaces.contains_key(&target) {
                let x = args.get(19).map(value_to_i32).unwrap_or_default() as f32;
                let y = args.get(18).map(value_to_i32).unwrap_or_default() as f32;
                // sub_4867D0 pops source args 20..0. Source arg 9 is
                // therefore pop slot 11 and is the font-height operand paired
                // with source arg 10 by sub_4035A0.
                let size = args
                    .get(11)
                    .map(value_to_i32)
                    .filter(|size| (4..=256).contains(size))
                    .unwrap_or(self.text_state.font_size as i32) as f32;
                // Source arg 4 is pop slot 16 and is the primary packed RGB
                // text color. Zero is valid black and must not be treated as
                // an absent/default color.
                let color = args
                    .get(16)
                    .map(value_to_i32)
                    .filter(|color| (0..=0xFF_FFFF).contains(color))
                    .map(|color| {
                        [
                            ((color >> 16) & 0xff) as f32 / 255.0,
                            ((color >> 8) & 0xff) as f32 / 255.0,
                            (color & 0xff) as f32 / 255.0,
                            1.0,
                        ]
                    })
                    .unwrap_or(self.text_state.color);
                let normalized = normalize_message_text(&text);
                self.store_bitmap_text_run(
                    target,
                    RuntimeTextNode {
                        text: normalized.clone(),
                        enabled: true,
                        owner_object: None,
                        screen_attached: false,
                        target_surface: None,
                        x,
                        y,
                        size,
                        line_height: size * 1.35,
                        formatted_layout: false,
                        color,
                        z: target,
                        ruby_spans: Vec::new(),
                        style_spans: Vec::new(),
                    },
                );
                tracing::info!(target, x, y, size, text = %normalized, "RenderBitmapText");
                self.trace_graph(format!(
                    "render text into bitmap #{target} at ({x:.0},{y:.0}) {normalized:?}"
                ));
                return;
            }
        }
        if matches!(args.len(), 14 | 15) {
            // Graph91:9C/sub_484A30 and 91:9D/sub_484C40 both select source
            // argument 0 as the destination, then call sub_403B10 ->
            // sub_434BA0 -> sub_434C50. 9D adds one final source-order style
            // selector, which becomes args[0] in reverse-pop order and shifts
            // the renderer fields by one slot. Neither target path creates a
            // display-tree text node: glyphs are committed immediately to the
            // selected bitmap.
            let shift = if args.len() == 15 { 1 } else { 0 };
            let target = args.get(13 + shift).map(value_to_i32).unwrap_or_default();
            if target != -1 && self.graph_surfaces.contains_key(&target) {
                let x = args.get(12 + shift).map(value_to_i32).unwrap_or_default();
                let y = args.get(11 + shift).map(value_to_i32).unwrap_or_default();
                let size = args
                    .get(6 + shift)
                    .map(value_to_i32)
                    .filter(|size| (8..=96).contains(size))
                    .unwrap_or(self.text_state.font_size as i32);
                let horizontal_scale = args
                    .get(5 + shift)
                    .map(value_to_i32)
                    .filter(|scale| *scale > 0)
                    .unwrap_or(100);
                let spacing = args.get(2 + shift).map(value_to_i32).unwrap_or_default();
                let packed_color = args
                    .get(shift)
                    .map(value_to_i32)
                    .filter(|color| (0..=0xFF_FFFF).contains(color))
                    .unwrap_or_else(|| {
                        let color = self.text_state.color;
                        ((color[0].clamp(0.0, 1.0) * 255.0).round() as i32) << 16
                            | ((color[1].clamp(0.0, 1.0) * 255.0).round() as i32) << 8
                            | (color[2].clamp(0.0, 1.0) * 255.0).round() as i32
                    });
                let color = [
                    ((packed_color >> 16) & 0xff) as f32 / 255.0,
                    ((packed_color >> 8) & 0xff) as f32 / 255.0,
                    (packed_color & 0xff) as f32 / 255.0,
                    1.0,
                ];
                let normalized = normalize_message_text(&text);
                let rasterized = !normalized.is_empty()
                    && self
                        .rasterize_graph_bitmap_text(
                            target,
                            &normalized,
                            x,
                            y,
                            size as f32,
                            spacing as f32,
                            horizontal_scale as f32,
                            color,
                        )
                        .is_some();
                tracing::info!(
                    target,
                    x,
                    y,
                    size,
                    spacing,
                    horizontal_scale,
                    style_mode = ?(args.len() == 15).then(|| value_to_i32(&args[0])),
                    packed_color = format_args!("0x{packed_color:06X}"),
                    rasterized,
                    pixel_stats = ?self.graph_bitmap_pixel_stats(target),
                    text = %normalized,
                    "GraphDrawBitmapText"
                );
                self.trace_graph(format!(
                    "draw text into bitmap #{target} at ({x},{y}) rasterized={rasterized} {normalized:?}"
                ));
                return;
            }
        }
        let target = self.ensure_message_text_node();
        self.apply_text_to_node(target, normalize_message_text(&text));
        tracing::info!(
            target,
            text = %normalize_message_text(&text),
            state = ?self.text_state,
            "RenderTextNode"
        );
        self.trace_graph(format!(
            "render text node #{target} {:?}",
            normalize_message_text(&text)
        ));
    }

    pub(crate) fn store_bitmap_text_run(&mut self, bitmap: i32, node: RuntimeTextNode) {
        self.text_nodes.insert(bitmap, node.clone());
        self.bitmap_text_runs.entry(bitmap).or_default().push(node);
        self.refresh_bitmap_nodes(bitmap);
    }

    pub(crate) fn clear_bitmap_text(&mut self, bitmap: i32) {
        self.bitmap_text_runs.remove(&bitmap);
        self.text_nodes.remove(&bitmap);
        self.refresh_bitmap_nodes(bitmap);
    }

    pub(crate) fn composite_bitmap_text(
        &mut self,
        destination: i32,
        source: i32,
        x: i32,
        y: i32,
        alpha: i32,
    ) {
        if destination <= 0 || source <= 0 || destination == source {
            return;
        }
        let mut runs = self
            .bitmap_text_runs
            .get(&source)
            .cloned()
            .or_else(|| self.text_nodes.get(&source).cloned().map(|node| vec![node]))
            .unwrap_or_default();
        if runs.is_empty() {
            return;
        }
        // GraphObjectApply uses 1 for the ordinary opaque copy path and 128
        // for explicit half-byte alpha. Both forms are emitted by logwnd._bp.
        let opacity = if alpha == 1 {
            1.0
        } else {
            (alpha as f32 / 128.0).clamp(0.0, 1.0)
        };
        for run in &mut runs {
            run.x += x as f32;
            run.y += y as f32;
            run.color[3] *= opacity;
            run.screen_attached = false;
            run.target_surface = None;
        }
        self.text_nodes
            .insert(destination, runs.last().cloned().unwrap());
        self.bitmap_text_runs
            .entry(destination)
            .or_default()
            .extend(runs);
        self.refresh_bitmap_nodes(destination);
        self.trace_graph(format!(
            "bitmap text apply source=#{source} destination=#{destination} x={x} y={y} alpha={alpha}"
        ));
    }

    pub(crate) fn copy_bitmap_text(&mut self, source: i32, destination: i32) -> usize {
        if source <= 0 || destination <= 0 || source == destination {
            return 0;
        }
        let runs = self
            .bitmap_text_runs
            .get(&source)
            .cloned()
            .or_else(|| self.text_nodes.get(&source).cloned().map(|node| vec![node]))
            .unwrap_or_default();
        if runs.is_empty() {
            self.bitmap_text_runs.remove(&destination);
            self.text_nodes.remove(&destination);
            self.refresh_bitmap_nodes(destination);
            return 0;
        }
        self.text_nodes
            .insert(destination, runs.last().cloned().unwrap());
        let count = runs.len();
        self.bitmap_text_runs.insert(destination, runs);
        self.refresh_bitmap_nodes(destination);
        count
    }

    /// Portable text runs stand in for glyphs already rasterized into target
    /// bitmap storage. Region creation must therefore clip and translate them
    /// with the same source rectangle as the pixel copy.
    pub(crate) fn copy_bitmap_text_region(
        &mut self,
        source: i32,
        destination: i32,
        x: i32,
        y: i32,
        width: i32,
        height: i32,
    ) -> usize {
        if source <= 0 || destination <= 0 || source == destination || width <= 0 || height <= 0 {
            return 0;
        }
        let source_left = x as f32;
        let source_top = y as f32;
        let source_right = source_left + width as f32;
        let source_bottom = source_top + height as f32;
        let mut runs = self
            .bitmap_text_runs
            .get(&source)
            .cloned()
            .or_else(|| self.text_nodes.get(&source).cloned().map(|node| vec![node]))
            .unwrap_or_default();
        runs.retain(|run| {
            let mut line_width = 0.0f32;
            let mut maximum_width = 0.0f32;
            let mut line_count = 1usize;
            for ch in run.text.chars() {
                if ch == '\n' {
                    maximum_width = maximum_width.max(line_width);
                    line_width = 0.0;
                    line_count += 1;
                } else {
                    line_width += wrap_char_units(ch) * run.size;
                }
            }
            maximum_width = maximum_width.max(line_width).max(run.size);
            let run_height = line_count as f32 * run.line_height.max(run.size).max(1.0);
            run.x < source_right
                && run.x + maximum_width > source_left
                && run.y < source_bottom
                && run.y + run_height > source_top
        });
        for run in &mut runs {
            run.x -= source_left;
            run.y -= source_top;
            run.owner_object = None;
            run.screen_attached = false;
            run.target_surface = None;
        }
        if runs.is_empty() {
            self.bitmap_text_runs.remove(&destination);
            self.text_nodes.remove(&destination);
            self.refresh_bitmap_nodes(destination);
            return 0;
        }
        self.text_nodes
            .insert(destination, runs.last().cloned().unwrap());
        let count = runs.len();
        self.bitmap_text_runs.insert(destination, runs);
        self.refresh_bitmap_nodes(destination);
        count
    }

    pub(crate) fn configure_bitmap_text_node(
        &mut self,
        node: i32,
        bitmap: i32,
        values: &[i32],
        owner_object: Option<i32>,
    ) -> bool {
        self.release_screen_text_node(node);
        let runs = self
            .bitmap_text_runs
            .get(&bitmap)
            .cloned()
            .or_else(|| self.text_nodes.get(&bitmap).cloned().map(|node| vec![node]))
            .unwrap_or_default();
        if runs.is_empty() {
            return false;
        }
        let flags = values.first().copied().unwrap_or_default();
        let (bitmap_width, bitmap_height) = self
            .graph_surfaces
            .get(&bitmap)
            .map(|surface| (surface.width, surface.height))
            .unwrap_or((0.0, 0.0));
        let origin_x = values.get(5).copied().unwrap_or_default() as f32;
        let origin_y = values.get(4).copied().unwrap_or_default() as f32;
        let z = values.first().copied().unwrap_or(NATIVE_DISPLAY_Z);
        let mut children = Vec::with_capacity(runs.len());
        for (index, mut run) in runs.into_iter().enumerate() {
            let child = if index == 0 {
                node
            } else {
                while self.text_nodes.contains_key(&self.next_screen_text_child) {
                    self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                }
                let child = self.next_screen_text_child;
                self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                child
            };
            run.screen_attached = true;
            run.owner_object = owner_object;
            run.target_surface = None;
            run.x += origin_x;
            run.y += origin_y;
            if flags & 0x4 == 0 {
                run.x -= bitmap_width;
            }
            if flags & 0x2 != 0 {
                run.y -= bitmap_height;
            }
            run.z = z;
            self.register_display_layer(child, owner_object);
            self.text_nodes.insert(child, run);
            children.push(child);
        }
        self.screen_text_children.insert(node, children);
        true
    }

    pub(crate) fn configure_image_bitmap_text_node(
        &mut self,
        node: i32,
        bitmap: i32,
        origin_x: f32,
        origin_y: f32,
        z: i32,
        owner_object: Option<i32>,
    ) -> bool {
        self.release_screen_text_node(node);
        let runs = self
            .bitmap_text_runs
            .get(&bitmap)
            .cloned()
            .or_else(|| self.text_nodes.get(&bitmap).cloned().map(|node| vec![node]))
            .unwrap_or_default();
        if runs.is_empty() {
            return false;
        }
        let mut children = Vec::with_capacity(runs.len());
        for (index, mut run) in runs.into_iter().enumerate() {
            let child = if index == 0 {
                node
            } else {
                while self.text_nodes.contains_key(&self.next_screen_text_child) {
                    self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                }
                let child = self.next_screen_text_child;
                self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                child
            };
            run.screen_attached = true;
            run.owner_object = owner_object;
            run.target_surface = None;
            run.x += origin_x;
            run.y += origin_y;
            run.z = z;
            self.register_display_layer(child, owner_object);
            self.text_nodes.insert(child, run);
            children.push(child);
        }
        self.screen_text_children.insert(node, children);
        true
    }

    pub(crate) fn configure_transition_bitmap_text_node(
        &mut self,
        node: i32,
        primary: i32,
        secondary: i32,
        alpha_parameter: i32,
        origin_x: f32,
        origin_y: f32,
        z: i32,
        owner_object: Option<i32>,
    ) -> bool {
        self.release_screen_text_node(node);
        let primary_runs = self
            .bitmap_text_runs
            .get(&primary)
            .cloned()
            .or_else(|| {
                self.text_nodes
                    .get(&primary)
                    .cloned()
                    .map(|node| vec![node])
            })
            .unwrap_or_default();
        let secondary_runs = self
            .bitmap_text_runs
            .get(&secondary)
            .cloned()
            .or_else(|| {
                self.text_nodes
                    .get(&secondary)
                    .cloned()
                    .map(|node| vec![node])
            })
            .unwrap_or_default();
        if primary_runs.is_empty() && secondary_runs.is_empty() {
            return false;
        }

        let same_content = primary_runs.len() == secondary_runs.len()
            && primary_runs
                .iter()
                .zip(&secondary_runs)
                .all(|(left, right)| {
                    left.text == right.text
                        && left.x == right.x
                        && left.y == right.y
                        && left.size == right.size
                        && left.color == right.color
                        && left.ruby_spans == right.ruby_spans
                        && left.style_spans == right.style_spans
                });
        let mut runs = if same_content {
            primary_runs
        } else {
            let secondary_weight = alpha_parameter.clamp(0, 256) as f32 / 256.0;
            let primary_weight = 1.0 - secondary_weight;
            let mut runs = primary_runs;
            for run in &mut runs {
                run.color[3] *= primary_weight;
            }
            let mut secondary_runs = secondary_runs;
            for run in &mut secondary_runs {
                run.color[3] *= secondary_weight;
            }
            runs.extend(secondary_runs);
            runs
        };
        runs.retain(|run| run.color[3] > 0.001);

        let mut children = Vec::with_capacity(runs.len());
        for (index, mut run) in runs.into_iter().enumerate() {
            // When the bitmap transition belongs to a real CDspObjSprite,
            // never reuse that native sprite handle as the compatibility text
            // node. Doing so makes release_screen_text_node() tear the sprite
            // itself out of the display tree on the next 90:58 configure.
            let use_root_handle = index == 0 && owner_object != Some(node);
            let child = if use_root_handle {
                node
            } else {
                while self.text_nodes.contains_key(&self.next_screen_text_child) {
                    self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                }
                let child = self.next_screen_text_child;
                self.next_screen_text_child = self.next_screen_text_child.saturating_sub(1);
                child
            };
            run.screen_attached = true;
            run.owner_object = owner_object;
            run.target_surface = None;
            run.x += origin_x;
            run.y += origin_y;
            run.z = z;
            self.register_display_layer(child, owner_object);
            self.text_nodes.insert(child, run);
            children.push(child);
        }
        if children.is_empty() {
            return false;
        }
        self.screen_text_children.insert(node, children);
        true
    }

    pub(crate) fn release_screen_text_node(&mut self, node: i32) -> bool {
        let root_was_text = self.text_nodes.remove(&node).is_some();
        let mut removed = root_was_text;
        // A screen-text compatibility root may share its numeric key with a
        // real native display object (notably CDspObjSprite mode 1). Only
        // remove a display-tree object when this routine actually owned a
        // text node at that handle.
        if root_was_text {
            self.display_tree.remove(node);
        }
        if let Some(children) = self.screen_text_children.remove(&node) {
            for child in children {
                let child_was_text = self.text_nodes.remove(&child).is_some();
                removed |= child_was_text;
                if child_was_text {
                    self.display_tree.remove(child);
                }
            }
        }
        removed
    }

    pub(crate) fn clear_transient_screen_text_nodes(&mut self) -> usize {
        let roots = self
            .screen_text_children
            .keys()
            .copied()
            .collect::<Vec<_>>();
        let mut removed = 0;
        for root in roots {
            let count = self
                .screen_text_children
                .get(&root)
                .map(Vec::len)
                .unwrap_or_default()
                .max(usize::from(self.text_nodes.contains_key(&root)));
            if self.release_screen_text_node(root) {
                removed += count;
            }
            self.remove_bitmap_node_binding(root);
        }

        let orphaned = self
            .text_nodes
            .iter()
            .filter_map(|(&id, node)| {
                (node.screen_attached
                    && id != MESSAGE_TEXT_NODE_ID
                    && id != MESSAGE_NAME_TEXT_NODE_ID
                    && id != MESSAGE_OVERLAY_TEXT_NODE_ID)
                    .then_some(id)
            })
            .collect::<Vec<_>>();
        removed += orphaned.len();
        for id in orphaned {
            self.text_nodes.remove(&id);
            self.display_tree.remove(id);
            self.remove_bitmap_node_binding(id);
        }
        removed
    }

    pub(crate) fn set_screen_text_node_surface(&mut self, node: i32, surface: Option<i32>) {
        if let Some(record) = self.text_nodes.get_mut(&node) {
            record.target_surface = surface;
        }
        if let Some(children) = self.screen_text_children.get(&node) {
            for child in children {
                if let Some(record) = self.text_nodes.get_mut(child) {
                    record.target_surface = surface;
                }
            }
        }
    }

    pub(crate) fn set_screen_text_node_enabled(&mut self, node: i32, enabled: bool) {
        if let Some(record) = self.text_nodes.get_mut(&node) {
            record.enabled = enabled;
        }
        if let Some(children) = self.screen_text_children.get(&node) {
            for child in children {
                if let Some(record) = self.text_nodes.get_mut(child) {
                    record.enabled = enabled;
                }
            }
        }
    }

    pub(crate) fn start_scenario_message(&mut self, speaker: Option<String>, text: String) {
        if text.is_empty() {
            return;
        }
        self.text_state = TextState::message_window();
        self.ensure_message_window_layer();
        self.ensure_message_control_layers();
        if let Some(speaker) = speaker.filter(|speaker| !speaker.is_empty()) {
            self.ensure_message_name_layer();
            let target = self.ensure_message_name_text_node();
            self.apply_name_to_node(target, speaker);
        } else {
            self.hide_message_name();
        }
        let target = self.ensure_message_text_node();
        if let Some(node) = self.text_nodes.get_mut(&target) {
            node.line_height = self.text_state.line_height.max(node.size);
            node.formatted_layout = true;
        }
        let parsed = self.parse_message_with_registry(&text, &self.text_state.clone());
        self.text_runtime
            .set_glyph_delay_ms(self.graph_defaults.text_animation.glyph_delay_ms);
        self.text_runtime.start_styled_message(parsed, target);
        self.apply_text_to_node(target, String::new());
        tracing::info!(target, text = %normalize_message_text(&text), "ScenarioMessage");
        self.trace_graph(format!(
            "scenario message node #{target} start {:?}",
            normalize_message_text(&text)
        ));
    }

    pub(crate) fn window_text_state(&self, target_surface: Option<i32>) -> TextState {
        let mut state = self.text_state.clone();
        if let Some(surface) = target_surface.and_then(|id| self.graph_surfaces.get(&id)) {
            let cursor = self
                .surface_text_states
                .get(&surface.id)
                .copied()
                .unwrap_or_default();
            state.x = cursor.cursor_x as f32;
            state.y = cursor.cursor_y as f32;
            state.width = (surface.valid_right as f32 + 1.0 - state.x).max(1.0);
            state.height = (surface.valid_bottom as f32 + 1.0 - state.y).max(1.0);
            if cursor.font_size > 0 {
                state.font_size = cursor.font_size as f32;
            }
            state.line_height =
                (state.font_size * (100 + surface.line_spacing_percent) as f32 / 100.0).max(1.0);
        }
        state
    }

    pub(crate) fn reset_window_text_cursor(&mut self, target: i32) {
        if let Some(surface) = self.graph_surfaces.get(&target) {
            let offset = self.graph_defaults.text_layout.boundary_offset;
            let state = self.surface_text_states.entry(target).or_default();
            state.cursor_x = surface.valid_left.saturating_add(offset);
            state.cursor_y = surface.valid_top;
        }
    }

    /// Parse and wrap one message against the process-wide ruby registry.
    ///
    /// The target's layout (`sub_435290`) resets the `<l>` fragment table at
    /// its start and refills it as it runs, so every parse replaces the
    /// records `92:9E` drains.
    fn parse_message_with_registry(
        &mut self,
        text: &str,
        state: &TextState,
    ) -> ParsedMessageMarkup {
        // `92:9F` stores dword_507650; -1 leaves link glyphs uncoloured.
        let link_color = (self.system92_text_render_override != -1)
            .then(|| (self.system92_text_render_override as u32) & 0x00ff_ffff);
        let parsed = parse_and_wrap_with_registry(
            text,
            state,
            &mut self.graph_defaults.ruby_registry,
            link_color,
        );
        self.system92_text_fragment_records = link_fragment_records(&parsed, state);
        parsed
    }

    pub(crate) fn start_native_message(
        &mut self,
        text: String,
        target_surface: Option<i32>,
    ) -> i32 {
        let state = self.window_text_state(target_surface);
        let parsed = self.parse_message_with_registry(&text, &state);
        // The target wrapper installs CProcDspMsg even for an empty or
        // control-only string. In particular, a lone 0x0A is a zero-delay
        // layout control, not a synthetic one-glyph message.
        self.user_controls.remove(&MESSAGE_INPUT_OBJECT_ID);
        let target = self.ensure_message_text_node();
        if let Some(node) = self.text_nodes.get_mut(&target) {
            node.owner_object = target_surface;
            node.screen_attached = target_surface.is_none();
            node.target_surface = target_surface;
            node.x = state.x;
            node.y = state.y;
            node.size = state.font_size.max(1.0);
            node.line_height = state.line_height.max(node.size);
            node.formatted_layout = true;
            node.color = state.color;
            node.z = target_surface
                .and_then(|surface| self.graph_surfaces.get(&surface))
                .map(|surface| surface.z)
                .unwrap_or(MESSAGE_TEXT_Z);
        }
        self.text_runtime
            .set_glyph_delay_ms(self.graph_defaults.text_animation.glyph_delay_ms);
        let message_text = parsed.text.clone();
        self.text_runtime.start_styled_message(parsed, target);
        // Graph90:9B is owned by CProcDspMsg (+0x44/+0x48).  Do not also
        // delay the glyph runtime: doing so applies the native procedure
        // wait twice and incorrectly postpones typewriter reveal.
        // CProcDspMsg control-only records (notably 0x0A/newline) mutate
        // formatter/procedure state without replacing the glyphs already
        // committed to the destination surface.  Graph92:8E is the explicit
        // text-surface reset.  Treating a control-only Graph92:90 invocation
        // as a new visible string makes the completed sentence disappear as
        // soon as the following newline/wait record starts.
        let has_visible_glyph = message_text.chars().any(|ch| ch != '\n');
        let initial_visible_text = self.text_runtime.current_visible_text();
        if let Some(node) = self.text_nodes.get_mut(&target) {
            if has_visible_glyph {
                node.text = initial_visible_text;
            }
            node.enabled = true;
        }
        self.native_message_active = true;
        let duration_ms = self.text_runtime.duration_ms();
        tracing::info!(target, duration_ms, text = %message_text, "NativeMessage");
        self.trace_graph(format!(
            "native message node #{target} duration={duration_ms}ms {:?}",
            message_text
        ));
        duration_ms
    }

    pub(crate) fn formatted_text_layout(
        &self,
        node: &RuntimeTextNode,
    ) -> RuntimeFormattedTextLayout {
        if !node.formatted_layout {
            return RuntimeFormattedTextLayout::plain(node);
        }
        let explicit_height = self.graph_defaults.text_style.ruby_height;
        let ruby_height = if explicit_height > 0 {
            explicit_height
        } else {
            (node.size.max(0.0) as i32).saturating_mul(self.graph_defaults.text_layout.font_percent)
                / 100
        }
        .max(4) as f32;
        let ruby_x_offset = self.graph_defaults.text_style.ruby_x_offset as f32;
        let ruby_y_offset = self.graph_defaults.text_style.ruby_y_offset as f32;
        let apply_y_offset = self
            .graph_config
            .text_global_properties
            .get(&0x8000_0002)
            .copied()
            .unwrap_or_default()
            != 0;
        RuntimeFormattedTextLayout {
            body_y_offset: ruby_height - if apply_y_offset { ruby_y_offset } else { 0.0 },
            line_height: node.line_height.max(node.size).max(1.0),
            ruby_height,
            ruby_x_offset,
            ruby_y_offset,
        }
    }

    pub(crate) fn native_message_procedure_config(
        &self,
        class: ethornell_vm::NativeMessageProcedureClass,
        display_object: i32,
    ) -> ethornell_vm::NativeMessageProcedureConfig {
        ethornell_vm::NativeMessageProcedureConfig {
            class,
            initial_delay_enabled: self.graph_defaults.message_delay_enabled,
            initial_delay_ms: self
                .graph_defaults
                .message_delay_enabled
                .then_some(self.graph_defaults.message_delay_ms.max(0))
                .unwrap_or(0),
            reveal_duration_ms: self.text_runtime.duration_ms(),
            reveal_steps: self.graph_defaults.text_animation.reveal_steps,
            reveal_step_delay_ms: self.graph_defaults.text_animation.reveal_step_delay_ms,
            settle_steps: self.graph_defaults.text_animation.settle_steps,
            settle_step_delay_ms: self.graph_defaults.text_animation.settle_step_delay_ms,
            auto_advance_delay_ms: self
                .graph_defaults
                .text_animation
                .auto_advance_enabled
                .then_some(
                    self.graph_defaults
                        .text_animation
                        .auto_advance_delay_ms
                        .max(0),
                ),
            input_scope: self
                .graph_defaults
                .resolved_message_input_scope(display_object),
            completion_control: 0,
            end_wait_policy: 0,
            allow_high_bit_input: false,
            allow_auxiliary_input: true,
            auxiliary_input_mask: self.message_auxiliary_input_mask,
            input_forces_completion: self.graph_defaults.message_input_forces_completion,
        }
    }

    pub(crate) fn render_native_message_text(&mut self, text: &str) {
        if text.is_empty() {
            return;
        }
        self.ensure_message_window_layer();
        self.ensure_message_control_layers();
        let target = self.ensure_message_text_node();
        let parsed = self.parse_message_with_registry(text, &self.text_state.clone());
        self.apply_text_to_node(target, parsed.text);
        if let Some(node) = self.text_nodes.get_mut(&target) {
            node.line_height = self.text_state.line_height.max(node.size);
            node.formatted_layout = true;
            node.ruby_spans = parsed.ruby_spans;
            node.style_spans = parsed.style_spans;
        }
        self.native_message_active = true;
    }

    pub(crate) fn reset_native_message_text(&mut self) {
        self.text_runtime = Default::default();
        self.text_runtime
            .set_glyph_delay_ms(self.graph_defaults.text_animation.glyph_delay_ms);
        if let Some(node) = self.text_nodes.get_mut(&MESSAGE_TEXT_NODE_ID) {
            node.text.clear();
            node.ruby_spans.clear();
            node.style_spans.clear();
        }
        self.native_message_active = false;
    }

    pub(crate) fn tick_text(&mut self, elapsed_ms: u64) {
        if let Some((target, text)) = self.text_runtime.tick(elapsed_ms) {
            self.apply_text_to_node(target, text);
            self.apply_visible_ruby_to_node(target);
            self.apply_visible_styles_to_node(target);
        }
        self.post_message_timeline_events();
    }

    /// `<ev N>` records enqueue host event `0x30000001` (`sub_496540`), which
    /// scripts read back through the system event queue (`80:A0`).
    fn post_message_timeline_events(&mut self) {
        for event in self.text_runtime.take_fired_events() {
            self.queued_system_events.push_back(event);
        }
    }

    pub(crate) fn handle_message_control_input(&mut self) -> bool {
        // Native BCS programs own message-window control dispatch through
        // GraphPollObjectEvent. These fallback actions are only for the
        // optional shadow scenario interpreter.
        if self.scenario_playback.is_none() {
            return false;
        }
        let Some(point) = self.pending_click else {
            return false;
        };
        let Some(control) = self
            .user_controls
            .values()
            .rev()
            .find(|control| {
                control.owner_id == MESSAGE_CONTROL_OWNER_ID
                    && control.contains(point, false)
                    && self.has_active_message_window()
            })
            .copied()
        else {
            return false;
        };

        self.pending_click = None;
        self.pending_object_state = None;
        self.pending_input_state = None;
        self.pending_input_descriptor = None;
        self.pending_input_consumed = false;
        self.mouse_pressed = false;
        self.last_hit_control = control.id;
        self.last_hit_payload = control.payload;

        match control.payload {
            8 => {
                if let Some(sound) = self.last_scenario_sound.clone() {
                    self.queue_scenario_sound(&sound);
                    self.trace_graph(format!("message control voice replay {sound}"));
                } else {
                    self.trace_graph("message control voice replay without cached voice");
                }
            }
            9 => {
                self.scenario_auto_mode = !self.scenario_auto_mode;
                self.scenario_skip_mode = false;
                self.scenario_auto_wait_frames = 0;
                self.trace_graph(format!(
                    "message control auto mode={}",
                    self.scenario_auto_mode
                ));
            }
            10 => {
                self.scenario_skip_mode = !self.scenario_skip_mode;
                self.scenario_auto_mode = false;
                self.scenario_auto_wait_frames = 0;
                self.trace_graph(format!(
                    "message control skip mode={}",
                    self.scenario_skip_mode
                ));
            }
            11 => {
                self.show_message_history_overlay();
            }
            12 => {
                self.hide_message_overlay();
                self.scenario_auto_mode = false;
                self.scenario_skip_mode = false;
                self.trace_graph("message control return");
            }
            13 => {
                self.show_message_overlay(
                    "SAVE",
                    "Save data slots are available from this menu path.\nPersistent slot encoding is still pending.",
                );
                self.trace_graph("message control save overlay");
            }
            14 => {
                self.show_message_overlay(
                    "QUICK SAVE",
                    "Quick save request accepted.\nPersistent slot encoding is still pending.",
                );
                self.trace_graph("message control quick-save overlay");
            }
            15 => {
                self.show_message_overlay(
                    "LOAD",
                    "Load slots are available from this menu path.\nPersistent slot decoding is still pending.",
                );
                self.trace_graph("message control load overlay");
            }
            16 => {
                self.show_message_overlay(
                    "QUICK LOAD",
                    "Quick load request accepted.\nPersistent slot decoding is still pending.",
                );
                self.trace_graph("message control quick-load overlay");
            }
            17 => {
                let auto = if self.scenario_auto_mode { "ON" } else { "OFF" };
                let skip = if self.scenario_skip_mode { "ON" } else { "OFF" };
                self.show_message_overlay(
                    "SYSTEM",
                    &format!("Auto: {auto}\nSkip: {skip}\nUse RETURN to close this panel."),
                );
                self.trace_graph("message control system overlay");
            }
            18 => {
                self.show_message_overlay("LEAF", "Leaf/system menu request accepted.");
                self.trace_graph("message control leaf overlay");
            }
            _ => {
                self.trace_graph(format!(
                    "message control payload={} ignored",
                    control.payload
                ));
            }
        }
        true
    }

    pub(crate) fn reveal_text_on_input(&mut self) {
        if let Some((target, text)) = self.text_runtime.reveal_all() {
            self.apply_text_to_node(target, text);
            self.apply_visible_ruby_to_node(target);
            self.apply_visible_styles_to_node(target);
            self.trace_graph(format!("message node #{target} reveal all"));
        }
        self.post_message_timeline_events();
    }

    fn ensure_message_text_node(&mut self) -> i32 {
        if let Some(target) = self.text_runtime.target_node.filter(|target| {
            *target == MESSAGE_TEXT_NODE_ID && self.text_nodes.contains_key(target)
        }) {
            return target;
        }
        let target = MESSAGE_TEXT_NODE_ID;
        self.text_nodes.insert(
            target,
            RuntimeTextNode {
                text: String::new(),
                enabled: true,
                owner_object: None,
                screen_attached: true,
                target_surface: None,
                x: self.text_state.x,
                y: self.text_state.y,
                size: self.text_state.font_size,
                line_height: self.text_state.line_height,
                formatted_layout: false,
                color: self.text_state.color,
                z: MESSAGE_TEXT_Z,
                ruby_spans: Vec::new(),
                style_spans: Vec::new(),
            },
        );
        self.text_runtime.target_node = Some(target);
        self.trace_graph(format!("create message text node #{target}"));
        target
    }

    fn apply_text_to_node(&mut self, target: i32, text: String) {
        let state = self.text_state.clone();
        let node = self
            .text_nodes
            .entry(target)
            .or_insert_with(|| RuntimeTextNode {
                text: String::new(),
                enabled: true,
                owner_object: None,
                screen_attached: true,
                target_surface: None,
                x: state.x,
                y: state.y,
                size: state.font_size,
                line_height: state.line_height,
                formatted_layout: false,
                color: state.color,
                z: MESSAGE_TEXT_Z,
                ruby_spans: Vec::new(),
                style_spans: Vec::new(),
            });
        if node.target_surface.is_some() {
            node.text = text;
            node.enabled = true;
            return;
        }
        node.text = wrap_text_to_state(&text, &state);
        node.enabled = true;
        node.x = if state.x > 0.0 { state.x } else { 36.0 };
        node.y = if state.y > 0.0 { state.y } else { 520.0 };
        node.size = state.font_size.max(18.0);
        node.line_height = state.line_height.max(node.size);
        node.color = state.color;
        node.z = MESSAGE_TEXT_Z;
    }

    fn apply_visible_ruby_to_node(&mut self, target: i32) {
        let spans = self.text_runtime.visible_ruby_spans();
        if let Some(node) = self.text_nodes.get_mut(&target) {
            node.ruby_spans = spans;
        }
    }

    fn apply_visible_styles_to_node(&mut self, target: i32) {
        let spans = self.text_runtime.visible_style_spans();
        if let Some(node) = self.text_nodes.get_mut(&target) {
            node.style_spans = spans;
        }
    }

    fn ensure_message_window_layer(&mut self) {
        if !self.graph_images.contains_key(MESSAGE_WINDOW_KEY) {
            self.load_graph_image_resource(0, "sysgrp.arc", MESSAGE_WINDOW_RESOURCE);
        }
        let Some(image) = self.graph_images.get(MESSAGE_WINDOW_KEY) else {
            return;
        };
        let width = image.width as f32;
        let height = image.height as f32;
        self.scenario_scene_layers.insert(MESSAGE_WINDOW_LAYER_ID);
        self.display_tree
            .register(MESSAGE_WINDOW_LAYER_ID, NativeDisplayKind::Sprite);
        self.graph_layers.insert(
            MESSAGE_WINDOW_LAYER_ID,
            RuntimeGraphLayer {
                hit_id: 0,
                owner_object: None,
                key: MESSAGE_WINDOW_KEY.to_string(),
                target_surface: None,
                x: 0.0,
                y: 720.0 - height,
                width,
                height,
                src_x: 0.0,
                src_y: 0.0,
                opacity: 1.0,
                z: MESSAGE_WINDOW_Z,
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
        if self.scenario_playback.is_some() {
            self.user_controls.insert(
                MESSAGE_INPUT_OBJECT_ID,
                RuntimeUserControl {
                    id: MESSAGE_INPUT_OBJECT_ID,
                    owner_id: MESSAGE_INPUT_OBJECT_ID,
                    payload: MESSAGE_INPUT_OBJECT_ID,
                    x: 0.0,
                    y: 720.0 - height,
                    width,
                    height,
                    normal_resource: -1,
                    selected_resource: -1,
                    enabled: true,
                    title_only: false,
                },
            );
        }
        self.trace_graph(format!(
            "ensure message window layer #{MESSAGE_WINDOW_LAYER_ID} {MESSAGE_WINDOW_KEY} y={:.1}",
            720.0 - height
        ));
    }

    fn ensure_message_name_layer(&mut self) {
        if !self.graph_images.contains_key(MESSAGE_NAME_WINDOW_KEY) {
            self.load_graph_image_resource(0, "sysgrp.arc", MESSAGE_NAME_WINDOW_RESOURCE);
        }
        let Some(image) = self.graph_images.get(MESSAGE_NAME_WINDOW_KEY) else {
            return;
        };
        let width = image.width as f32;
        let height = image.height as f32;
        self.scenario_scene_layers
            .insert(MESSAGE_NAME_WINDOW_LAYER_ID);
        self.display_tree
            .register(MESSAGE_NAME_WINDOW_LAYER_ID, NativeDisplayKind::Sprite);
        self.graph_layers.insert(
            MESSAGE_NAME_WINDOW_LAYER_ID,
            RuntimeGraphLayer {
                hit_id: 0,
                owner_object: None,
                key: MESSAGE_NAME_WINDOW_KEY.to_string(),
                target_surface: None,
                x: MESSAGE_NAME_WINDOW_X,
                y: MESSAGE_NAME_WINDOW_Y,
                width,
                height,
                src_x: 0.0,
                src_y: 0.0,
                opacity: 1.0,
                z: MESSAGE_NAME_WINDOW_Z,
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
        self.trace_graph(format!(
            "ensure message name layer #{MESSAGE_NAME_WINDOW_LAYER_ID} {MESSAGE_NAME_WINDOW_KEY} x={MESSAGE_NAME_WINDOW_X:.1} y={MESSAGE_NAME_WINDOW_Y:.1}"
        ));
    }

    fn ensure_message_name_text_node(&mut self) -> i32 {
        let target = MESSAGE_NAME_TEXT_NODE_ID;
        self.text_nodes.entry(target).or_insert(RuntimeTextNode {
            text: String::new(),
            enabled: true,
            owner_object: None,
            screen_attached: true,
            target_surface: None,
            x: MESSAGE_NAME_TEXT_X,
            y: MESSAGE_NAME_TEXT_Y,
            size: 24.0,
            line_height: 32.0,
            formatted_layout: false,
            color: [1.0, 1.0, 1.0, 1.0],
            z: MESSAGE_NAME_TEXT_Z,
            ruby_spans: Vec::new(),
            style_spans: Vec::new(),
        });
        target
    }

    fn apply_name_to_node(&mut self, target: i32, text: String) {
        let node = self.text_nodes.entry(target).or_insert(RuntimeTextNode {
            text: String::new(),
            enabled: true,
            owner_object: None,
            screen_attached: true,
            target_surface: None,
            x: MESSAGE_NAME_TEXT_X,
            y: MESSAGE_NAME_TEXT_Y,
            size: 24.0,
            line_height: 32.0,
            formatted_layout: false,
            color: [1.0, 1.0, 1.0, 1.0],
            z: MESSAGE_NAME_TEXT_Z,
            ruby_spans: Vec::new(),
            style_spans: Vec::new(),
        });
        node.text = normalize_message_text(&text);
        node.enabled = true;
        node.x = MESSAGE_NAME_TEXT_X;
        node.y = MESSAGE_NAME_TEXT_Y;
        node.size = 24.0;
        node.color = [1.0, 1.0, 1.0, 1.0];
        node.z = MESSAGE_NAME_TEXT_Z;
    }

    fn hide_message_name(&mut self) {
        if let Some(layer) = self.graph_layers.get_mut(&MESSAGE_NAME_WINDOW_LAYER_ID) {
            layer.enabled = false;
        }
        if let Some(node) = self.text_nodes.get_mut(&MESSAGE_NAME_TEXT_NODE_ID) {
            node.enabled = false;
            node.text.clear();
        }
    }

    fn ensure_message_control_layers(&mut self) {
        for resource in [
            MESSAGE_CONTROL_NORMAL_RESOURCE,
            MESSAGE_CONTROL_HOVER_RESOURCE,
            MESSAGE_CONTROL_ACTIVE_RESOURCE,
            MESSAGE_CONTROL_DISABLED_RESOURCE,
        ] {
            let key = format!("sysgrp.arc:{resource}");
            if !self.graph_images.contains_key(&key) {
                self.load_graph_image_resource(0, "sysgrp.arc", resource);
            }
        }
        for template in MESSAGE_CONTROL_TEMPLATES {
            let key = format!("sysgrp.arc:{MESSAGE_CONTROL_NORMAL_RESOURCE}");
            if !self.graph_images.contains_key(&key) {
                continue;
            }
            self.display_tree
                .register(template.layer_id, NativeDisplayKind::Sprite);
            self.graph_layers.insert(
                template.layer_id,
                RuntimeGraphLayer {
                    hit_id: template.layer_id,
                    owner_object: None,
                    key,
                    target_surface: None,
                    x: template.x,
                    y: template.y,
                    width: template.width,
                    height: template.height,
                    src_x: template.x,
                    src_y: template.y,
                    opacity: 1.0,
                    z: template.layer_id,
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
            self.user_controls.insert(
                template.layer_id,
                RuntimeUserControl {
                    id: template.layer_id,
                    owner_id: MESSAGE_CONTROL_OWNER_ID,
                    payload: template.payload,
                    x: template.x,
                    y: template.y,
                    width: template.width,
                    height: template.height,
                    normal_resource: -1,
                    selected_resource: -1,
                    enabled: true,
                    title_only: false,
                },
            );
        }
        self.trace_graph(format!(
            "ensure message controls count={}",
            MESSAGE_CONTROL_TEMPLATES.len()
        ));
    }

    pub(crate) fn has_active_message_window(&self) -> bool {
        if self.native_message_active
            && self.native_message_surface_target.is_some_and(|surface| {
                self.graph_surfaces
                    .get(&surface)
                    .is_some_and(|record| self.surface_display_chain_visible(surface, record))
            })
        {
            return true;
        }
        let layer_enabled = self
            .graph_layers
            .get(&MESSAGE_WINDOW_LAYER_ID)
            .is_some_and(|layer| layer.enabled);
        if !layer_enabled {
            return false;
        }
        if self
            .text_nodes
            .get(&MESSAGE_OVERLAY_TEXT_NODE_ID)
            .is_some_and(|node| node.enabled && !node.text.is_empty())
        {
            return true;
        }
        if !self.text_runtime.has_current_message() {
            return false;
        }
        if self.native_message_active {
            return true;
        }
        if self.text_runtime.is_animating() {
            return true;
        }
        if self.scenario_bootstrapped {
            return self
                .scenario_playback
                .as_ref()
                .is_some_and(|playback| playback.is_waiting_for_input());
        }
        true
    }

    pub(crate) fn should_draw_message_control_layer(&self, layer: &RuntimeGraphLayer) -> bool {
        if !self.has_active_message_window() {
            return false;
        }
        self.user_controls
            .get(&layer.hit_id)
            .is_some_and(|control| control.owner_id == MESSAGE_CONTROL_OWNER_ID && control.enabled)
    }

    pub(crate) fn message_control_layer_draw_key(
        &self,
        layer_id: i32,
        layer: &RuntimeGraphLayer,
    ) -> Option<&'static str> {
        if self.surface_controls.contains_layer(layer_id) {
            return None;
        }
        let control = self.user_controls.get(&layer.hit_id)?;
        if control.owner_id != MESSAGE_CONTROL_OWNER_ID {
            return None;
        }
        if !control.enabled || !self.has_active_message_window() {
            return Some("sysgrp.arc:SGMsgWnd000003");
        }
        let hovered = self
            .mouse_pos
            .is_some_and(|point| control.contains(point, false));
        if hovered && self.mouse_pressed {
            Some("sysgrp.arc:SGMsgWnd000002")
        } else if (control.payload == 9 && self.scenario_auto_mode)
            || (control.payload == 10 && self.scenario_skip_mode)
        {
            Some("sysgrp.arc:SGMsgWnd000002")
        } else if hovered {
            Some("sysgrp.arc:SGMsgWnd000001")
        } else {
            Some("sysgrp.arc:SGMsgWnd000000")
        }
    }

    fn show_message_history_overlay(&mut self) {
        let history = self
            .text_runtime
            .history
            .iter()
            .rev()
            .take(8)
            .rev()
            .cloned()
            .collect::<Vec<_>>()
            .join("\n");
        let text = if history.is_empty() {
            "(no history)".to_string()
        } else {
            history
        };
        self.show_message_overlay("LOG", &text);
        self.trace_graph("message control history overlay");
    }

    fn show_message_overlay(&mut self, title: &str, body: &str) {
        self.text_nodes.insert(
            MESSAGE_OVERLAY_TEXT_NODE_ID,
            RuntimeTextNode {
                text: format!("{title}\n{body}"),
                enabled: true,
                owner_object: None,
                screen_attached: true,
                target_surface: None,
                x: 64.0,
                y: 80.0,
                size: 24.0,
                line_height: 32.0,
                formatted_layout: false,
                color: [1.0, 1.0, 1.0, 1.0],
                z: 20_000,
                ruby_spans: Vec::new(),
                style_spans: Vec::new(),
            },
        );
    }

    fn hide_message_overlay(&mut self) {
        self.text_nodes.remove(&MESSAGE_OVERLAY_TEXT_NODE_ID);
    }
}

pub(crate) fn wrap_text_to_state(text: &str, state: &TextState) -> String {
    wrap_text_to_state_with_map(text, &[], state).0
}

pub(crate) fn parse_and_wrap_message(
    text: &str,
    state: &TextState,
) -> (String, Vec<RuntimeRubySpan>) {
    let parsed = parse_and_wrap_styled_message(text, state);
    (parsed.text, parsed.ruby_spans)
}

pub(crate) fn parse_and_wrap_styled_message(text: &str, state: &TextState) -> ParsedMessageMarkup {
    parse_and_wrap_with_registry(text, state, &mut RubyRegistry::default(), None)
}

/// Parse message markup against the process-wide ruby registry, then wrap.
/// Every character index the parser produced is remapped through the wrap so
/// ruby, style, link and timeline records stay attached to their glyphs.
pub(crate) fn parse_and_wrap_with_registry(
    text: &str,
    state: &TextState,
    registry: &mut RubyRegistry,
    link_color: Option<u32>,
) -> ParsedMessageMarkup {
    let parsed = parse_message_markup_with(text, registry, link_color);
    let (wrapped, map) = wrap_text_to_state_with_map(&parsed.text, &parsed.ruby_spans, state);
    let ruby_spans = parsed
        .ruby_spans
        .into_iter()
        .filter_map(|span| {
            Some(RuntimeRubySpan {
                start_char: *map.get(span.start_char)?,
                end_char: *map.get(span.end_char)?,
                reading: span.reading,
            })
        })
        .collect();
    let style_spans = parsed
        .style_spans
        .into_iter()
        .filter_map(|span| {
            let start_char = *map.get(span.start_char)?;
            let end_char = *map.get(span.end_char)?;
            (start_char < end_char).then_some(RuntimeTextStyleSpan {
                start_char,
                end_char,
                style: span.style,
            })
        })
        .collect();
    let link_spans = parsed
        .link_spans
        .into_iter()
        .filter_map(|span| {
            Some(RuntimeLinkSpan {
                start_char: *map.get(span.start_char)?,
                end_char: *map.get(span.end_char)?,
                text: span.text,
            })
        })
        .collect();
    let remap = |at_char: usize| map.get(at_char).copied().unwrap_or(wrapped.chars().count());
    let timeline = parsed
        .timeline
        .into_iter()
        .map(|mark| match mark {
            RuntimeTimelineMark::SetTime { at_char, units } => RuntimeTimelineMark::SetTime {
                at_char: remap(at_char),
                units,
            },
            RuntimeTimelineMark::Event {
                at_char,
                ordinal,
                id,
            } => RuntimeTimelineMark::Event {
                at_char: remap(at_char),
                ordinal,
                id,
            },
        })
        .collect();
    ParsedMessageMarkup {
        text: wrapped,
        ruby_spans,
        style_spans,
        link_spans,
        timeline,
    }
}

/// Fragment records for `92:9E`. Position is estimated from the portable wrap
/// model (row and per-character advance); the target records the exact cursor.
fn link_fragment_records(
    parsed: &ParsedMessageMarkup,
    state: &TextState,
) -> Vec<ethornell_vm::System92TextFragmentRecord> {
    let chars = parsed.text.chars().collect::<Vec<_>>();
    parsed
        .link_spans
        .iter()
        .map(|span| {
            let before = &chars[..span.start_char.min(chars.len())];
            let row = before.iter().filter(|&&ch| ch == '\n').count();
            let line_start = before
                .iter()
                .rposition(|&ch| ch == '\n')
                .map_or(0, |index| index + 1);
            let advance: f32 = before[line_start..]
                .iter()
                .map(|&ch| wrap_char_units(ch) * state.font_size)
                .sum();
            ethornell_vm::System92TextFragmentRecord {
                text: span.text.clone(),
                x: (state.x + advance).round() as i32,
                y: (state.y + row as f32 * state.line_height).round() as i32,
            }
        })
        .collect()
}

fn wrap_text_to_state_with_map(
    text: &str,
    protected_spans: &[RuntimeRubySpan],
    state: &TextState,
) -> (String, Vec<usize>) {
    let max_units = (state.width.max(120.0) / state.font_size.max(8.0))
        .floor()
        .max(8.0);
    let max_lines = ((state.height.max(state.line_height) / state.line_height.max(1.0)).floor()
        as usize)
        .max(1);
    let chars = text.chars().collect::<Vec<_>>();
    let mut protected_ends = BTreeMap::new();
    for span in protected_spans {
        if span.start_char < span.end_char && span.end_char <= chars.len() {
            protected_ends.insert(span.start_char, span.end_char);
        }
    }
    let mut out = String::new();
    let mut map = vec![0usize; chars.len() + 1];
    let mut output_chars = 0usize;
    let mut line_units = 0.0f32;
    let mut lines = 1usize;
    let mut index = 0usize;
    while index < chars.len() {
        map[index] = output_chars;
        if chars[index] == '\n' {
            if lines >= max_lines {
                break;
            }
            out.push('\n');
            output_chars += 1;
            line_units = 0.0;
            lines += 1;
            index += 1;
            continue;
        }

        let chunk_end = protected_ends.get(&index).copied().unwrap_or(index + 1);
        let chunk_units = chars[index..chunk_end]
            .iter()
            .map(|ch| wrap_char_units(*ch))
            .sum::<f32>();
        let mut projected_units = chunk_units;
        if chunk_end == index + 1 && is_kinsoku_line_end(chars[index]) {
            projected_units += chars
                .get(chunk_end)
                .filter(|ch| **ch != '\n')
                .map(|ch| wrap_char_units(*ch))
                .unwrap_or_default();
        } else {
            projected_units += chars[chunk_end..]
                .iter()
                .take_while(|ch| **ch != '\n' && is_kinsoku_line_start(**ch))
                .map(|ch| wrap_char_units(*ch))
                .sum::<f32>();
        }
        if line_units > 0.0 && line_units + projected_units > max_units {
            if lines >= max_lines {
                break;
            }
            out.push('\n');
            output_chars += 1;
            line_units = 0.0;
            lines += 1;
        }
        for ch in &chars[index..chunk_end] {
            map[index] = output_chars;
            out.push(*ch);
            output_chars += 1;
            line_units += wrap_char_units(*ch);
            index += 1;
        }
    }
    map[index..].fill(output_chars);
    (out, map)
}

fn is_kinsoku_line_start(ch: char) -> bool {
    "\"':;?!ﾞﾟ･，．、。：；？！”゛゜‐]})）〕］｝〉≫》」』】ヽヾゝゞ々ー～っゃゅょッャュョ"
        .contains(ch)
}

fn is_kinsoku_line_end(ch: char) -> bool {
    "[{(（〔［｛〈≪《「『【“".contains(ch)
}

pub(crate) fn ruby_draw_runs(
    node: &RuntimeTextNode,
    layout: RuntimeFormattedTextLayout,
) -> Vec<(String, f32, f32, f32)> {
    let chars = node.text.chars().collect::<Vec<_>>();
    if layout.ruby_height <= 0.0 {
        return Vec::new();
    }
    node.ruby_spans
        .iter()
        .filter_map(|span| {
            if span.start_char >= span.end_char || span.end_char > chars.len() {
                return None;
            }
            let mut x_units = 0.0f32;
            let mut line = 0usize;
            for ch in &chars[..span.start_char] {
                if *ch == '\n' {
                    x_units = 0.0;
                    line += 1;
                } else {
                    x_units += wrap_char_units(*ch);
                }
            }
            if chars[span.start_char..span.end_char].contains(&'\n') {
                return None;
            }
            let body_units = chars[span.start_char..span.end_char]
                .iter()
                .map(|ch| wrap_char_units(*ch))
                .sum::<f32>();
            let reading_units = span.reading.chars().map(wrap_char_units).sum::<f32>();
            let body_width = body_units * node.size;
            let reading_width = reading_units * layout.ruby_height;
            let x = node.x
                + x_units * node.size
                + (body_width - reading_width) * 0.5
                + node.size * 0.125
                + layout.ruby_x_offset;
            let main_y = node.y + layout.body_y_offset + line as f32 * layout.line_height;
            let y = main_y + layout.ruby_y_offset - layout.ruby_height;
            Some((span.reading.clone(), x, y, layout.ruby_height))
        })
        .collect()
}

fn wrap_char_units(ch: char) -> f32 {
    if ch.is_ascii() {
        if ch.is_ascii_whitespace() { 0.35 } else { 0.55 }
    } else {
        1.0
    }
}

fn value_to_text_string(value: &ethornell_vm::Value) -> Option<String> {
    match value {
        ethornell_vm::Value::Str(text) => Some(text.clone()),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::{
        TextState, link_fragment_records, parse_and_wrap_message, parse_and_wrap_styled_message,
        parse_and_wrap_with_registry, wrap_text_to_state,
    };
    use crate::ruby_registry::RubyRegistry;
    use crate::text_anim::RuntimeTimelineMark;

    fn narrow_text_state() -> TextState {
        TextState {
            x: 0.0,
            y: 0.0,
            width: 120.0,
            height: 100.0,
            font_size: 15.0,
            color: [1.0; 4],
            line_height: 20.0,
        }
    }

    #[test]
    fn wrapping_keeps_target_line_start_punctuation_with_its_predecessor() {
        assert_eq!(
            wrap_text_to_state("甲乙丙丁戊己庚辛。壬", &narrow_text_state()),
            "甲乙丙丁戊己庚\n辛。壬"
        );
    }

    #[test]
    fn wrapping_does_not_leave_an_opening_bracket_at_line_end() {
        assert_eq!(
            wrap_text_to_state("甲乙丙丁戊己庚「辛壬", &narrow_text_state()),
            "甲乙丙丁戊己庚\n「辛壬"
        );
    }

    #[test]
    fn wrapping_moves_a_ruby_base_as_one_target_text_fragment() {
        let (text, spans) =
            parse_and_wrap_message("甲乙丙丁戊己庚<Rしんじん>辛壬</R>癸", &narrow_text_state());
        assert_eq!(text, "甲乙丙丁戊己庚\n辛壬癸");
        assert_eq!(spans.len(), 1);
        assert_eq!((spans[0].start_char, spans[0].end_char), (8, 10));
        assert_eq!(spans[0].reading, "しんじん");
    }

    #[test]
    fn wrapping_remaps_target_style_spans_across_inserted_newlines() {
        let parsed =
            parse_and_wrap_styled_message("甲乙丙丁戊己庚<b>辛壬</b>癸", &narrow_text_state());
        assert_eq!(parsed.text, "甲乙丙丁戊己庚辛\n壬癸");
        assert_eq!(parsed.style_spans.len(), 1);
        assert_eq!(
            (
                parsed.style_spans[0].start_char,
                parsed.style_spans[0].end_char
            ),
            (7, 10)
        );
        assert!(parsed.style_spans[0].style.bold);
    }

    #[test]
    fn registry_ruby_wraps_as_one_fragment_like_a_tag_ruby() {
        let mut registry = RubyRegistry::default();
        registry.register("辛壬", "しんじん", false);
        let parsed = parse_and_wrap_with_registry(
            "甲乙丙丁戊己庚辛壬癸",
            &narrow_text_state(),
            &mut registry,
            None,
        );
        assert_eq!(parsed.text, "甲乙丙丁戊己庚\n辛壬癸");
        assert_eq!(parsed.ruby_spans.len(), 1);
        assert_eq!(
            (
                parsed.ruby_spans[0].start_char,
                parsed.ruby_spans[0].end_char
            ),
            (8, 10)
        );
    }

    #[test]
    fn wrapping_remaps_link_and_timeline_records_across_inserted_newlines() {
        let parsed = parse_and_wrap_with_registry(
            "甲乙丙丁戊己庚<t 3><l>辛壬</l><ev 5>癸",
            &narrow_text_state(),
            &mut RubyRegistry::default(),
            None,
        );
        assert_eq!(parsed.text, "甲乙丙丁戊己庚辛\n壬癸");
        assert_eq!(
            (
                parsed.link_spans[0].start_char,
                parsed.link_spans[0].end_char
            ),
            (7, 10)
        );
        assert_eq!(
            parsed.timeline,
            vec![
                RuntimeTimelineMark::SetTime {
                    at_char: 7,
                    units: 3
                },
                RuntimeTimelineMark::Event {
                    at_char: 10,
                    ordinal: 0,
                    id: 5
                },
            ]
        );
        let records = link_fragment_records(&parsed, &narrow_text_state());
        assert_eq!(records.len(), 1);
        assert_eq!(records[0].text, "辛壬");
        // Seven full-width glyphs at 15 px on the first row.
        assert_eq!((records[0].x, records[0].y), (105, 0));
    }
}
