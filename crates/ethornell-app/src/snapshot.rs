use crate::{
    RuntimeTraceApi,
    graph::{RuntimeGraphDrawItem, RuntimeGraphLayer},
};
use ab_glyph::{Font, FontArc, PxScale, ScaleFont, point};
use ethornell_core::{EthornellError, Result, composite_native_rgba};
use ethornell_image::{DecodedImage, write_rgba_png};
use std::path::Path;

thread_local! {
    static SESSION_FONT: std::cell::RefCell<Option<FontArc>> = const { std::cell::RefCell::new(None) };
    static MISSING_GLYPHS: std::cell::Cell<bool> = const { std::cell::Cell::new(false) };
    static RASTER_ERROR: std::cell::RefCell<Option<String>> = const { std::cell::RefCell::new(None) };
}

pub(crate) fn take_missing_glyphs() -> bool {
    MISSING_GLYPHS.with(|m| m.replace(false))
}

pub(crate) fn take_raster_error() -> Option<String> {
    RASTER_ERROR.with(|error| error.borrow_mut().take())
}

fn record_raster_error(message: impl Into<String>) {
    RASTER_ERROR.with(|error| {
        let mut error = error.borrow_mut();
        if error.is_none() {
            *error = Some(message.into());
        }
    });
}

fn check_session_coverage(text: &str) {
    SESSION_FONT.with(|s| {
        if let Some(font) = s.borrow().as_ref() {
            check_font_coverage(font, text.chars());
        }
    });
}

fn check_font_coverage(font: &FontArc, text: impl IntoIterator<Item = char>) {
    if text
        .into_iter()
        .any(|ch| !ch.is_control() && font.glyph_id(ch).0 == 0)
    {
        MISSING_GLYPHS.with(|missing| missing.set(true));
    }
}

/// Desktop/headless configuration is read once at the session boundary. An
/// explicitly selected invalid font must fail, not silently select another.
pub(crate) fn load_default_font() -> Result<FontArc> {
    if let Some(font) = SESSION_FONT.with(|session| session.borrow().clone()) {
        return Ok(font);
    }
    default_font(
        std::env::var_os("ETHORNELL_SNAPSHOT_FONT"),
        std::env::var_os("ETHORNELL_FONT"),
    )
}

/// The snapshot font when one is named, else the text font: the file
/// `ETHORNELL_FONT` names (a substitute for the font the game asks for), else
/// a system font. The window's renderer draws with this session font too.
fn default_font(
    snapshot: Option<std::ffi::OsString>,
    text: Option<std::ffi::OsString>,
) -> Result<FontArc> {
    match snapshot {
        Some(path) => ethornell_render::FontProvider::load_file(path, 0),
        None => ethornell_render::load_text_font(text),
    }
}

pub(crate) struct FontSessionGuard {
    previous: Option<FontArc>,
    previous_missing: bool,
    previous_error: Option<String>,
    // TLS scopes must be exited on the thread which entered them.
    _not_send: std::marker::PhantomData<std::rc::Rc<()>>,
}

impl Drop for FontSessionGuard {
    fn drop(&mut self) {
        SESSION_FONT.with(|s| *s.borrow_mut() = self.previous.take());
        MISSING_GLYPHS.with(|missing| missing.set(self.previous_missing));
        RASTER_ERROR.with(|error| *error.borrow_mut() = self.previous_error.take());
    }
}

/// Hold through bootstrap, VM ticks, CPU snapshots and teardown. No font bytes
/// or failed provider lookup survive this scope unless owned by the caller.
pub(crate) fn enter_font_session(font: &FontArc) -> FontSessionGuard {
    FontSessionGuard {
        previous: SESSION_FONT.with(|s| s.replace(Some(font.clone()))),
        previous_missing: MISSING_GLYPHS.with(|missing| missing.replace(false)),
        previous_error: take_raster_error(),
        _not_send: std::marker::PhantomData,
    }
}

pub(crate) fn with_font<T>(font: &FontArc, f: impl FnOnce() -> T) -> T {
    let _restore = enter_font_session(font);
    f()
}

const MAX_RASTER_BYTES: usize = 128 * 1024 * 1024;
const MAX_TEXT_GLYPHS: usize = 65_536;

fn raster_byte_len(width: u32, height: u32) -> Result<usize> {
    let bytes = (width as usize)
        .checked_mul(height as usize)
        .and_then(|n| n.checked_mul(4));
    match bytes {
        Some(bytes) if width <= 16_384 && height <= 16_384 && bytes <= MAX_RASTER_BYTES => {
            Ok(bytes)
        }
        _ => Err(EthornellError::Other(
            "CPU raster image budget exceeded".into(),
        )),
    }
}

fn new_raster(width: u32, height: u32) -> Option<DecodedImage> {
    let result = raster_byte_len(width, height).and_then(|bytes| {
        let mut rgba = Vec::new();
        rgba.try_reserve_exact(bytes)
            .map_err(|_| EthornellError::Other("CPU raster allocation failed".into()))?;
        rgba.resize(bytes, 0);
        Ok(DecodedImage {
            width,
            height,
            rgba,
        })
    });
    match result {
        Ok(image) => Some(image),
        Err(error) => {
            record_raster_error(error.to_string());
            None
        }
    }
}

fn valid_text_raster(size: f32, glyph_count: usize) -> bool {
    if !size.is_finite() || !(0.0..=1024.0).contains(&size) || glyph_count > MAX_TEXT_GLYPHS {
        record_raster_error("CPU text raster budget exceeded");
        return false;
    }
    true
}

fn valid_raster(image: &DecodedImage) -> bool {
    if raster_byte_len(image.width, image.height).ok() != Some(image.rgba.len()) {
        record_raster_error("invalid CPU raster RGBA image");
        return false;
    }
    true
}

fn valid_text_position(x: f32, y: f32) -> bool {
    if !x.is_finite() || !y.is_finite() || x.abs() > 1_000_000.0 || y.abs() > 1_000_000.0 {
        record_raster_error("CPU text position out of range");
        return false;
    }
    true
}

fn valid_glyph_bounds(bounds: ab_glyph::Rect) -> bool {
    let width = bounds.width();
    let height = bounds.height();
    if !valid_text_position(bounds.min.x, bounds.min.y)
        || !valid_text_position(bounds.max.x, bounds.max.y)
        || !(0.0..=4096.0).contains(&width)
        || !(0.0..=4096.0).contains(&height)
        || width * height > 4_194_304.0
    {
        record_raster_error("glyph outline raster budget exceeded");
        return false;
    }
    true
}

const SNAPSHOT_WIDTH: u32 = 1280;
const SNAPSHOT_HEIGHT: u32 = 720;

#[derive(Clone)]
pub(crate) struct RuntimeFrameComposition {
    pub(crate) framebuffer: DecodedImage,
    pub(crate) render_tree_nodes: usize,
    pub(crate) framebuffer_hash: u64,
}

pub(crate) fn write_runtime_snapshot(api: &RuntimeTraceApi, path: &Path) -> Result<()> {
    let out = compose_runtime_frame(api, true);
    if let Some(error) = take_raster_error() {
        return Err(EthornellError::Other(error));
    }
    write_composed_runtime_snapshot(api, &out, path)
}

pub(crate) fn write_composed_runtime_snapshot(
    api: &RuntimeTraceApi,
    framebuffer: &DecodedImage,
    path: &Path,
) -> Result<()> {
    if !valid_raster(framebuffer) {
        return Err(EthornellError::Other("invalid snapshot framebuffer".into()));
    }
    if let Some(directory) = std::env::var_os("ETHORNELL_HEADLESS_DUMP_IMAGES_DIR") {
        let directory = std::path::PathBuf::from(directory);
        let selected_keys = std::env::var("ETHORNELL_HEADLESS_DUMP_IMAGE_KEYS")
            .ok()
            .map(|value| {
                value
                    .split(',')
                    .map(str::trim)
                    .filter(|key| !key.is_empty())
                    .map(str::to_owned)
                    .collect::<std::collections::BTreeSet<_>>()
            });
        std::fs::create_dir_all(&directory)?;
        for (key, image) in &api.graph_images {
            if selected_keys
                .as_ref()
                .is_some_and(|selected| !selected.contains(key.as_str()))
            {
                continue;
            }
            let name = key
                .chars()
                .map(|ch| if ch.is_ascii_alphanumeric() { ch } else { '_' })
                .collect::<String>();
            write_rgba_png(image, &directory.join(format!("{name}.png")))?;
        }
    }
    write_rgba_png(framebuffer, path)
}

pub(crate) fn runtime_frame_size(api: &RuntimeTraceApi) -> (u32, u32) {
    let width = u32::try_from(api.screen_width)
        .ok()
        .filter(|width| *width > 0)
        .unwrap_or(SNAPSHOT_WIDTH);
    let height = u32::try_from(api.screen_height)
        .ok()
        .filter(|height| *height > 0)
        .unwrap_or(SNAPSHOT_HEIGHT);
    (width, height)
}

pub(crate) fn compose_runtime_frame(api: &RuntimeTraceApi, include_text: bool) -> DecodedImage {
    compose_runtime_frame_through_priority(api, include_text, None)
}

pub(crate) fn compose_runtime_frame_with_diagnostics(
    api: &RuntimeTraceApi,
    include_text: bool,
) -> RuntimeFrameComposition {
    let render_tree_nodes = api.graph_output_draw_items().len();
    let framebuffer = compose_runtime_frame(api, include_text);
    let framebuffer_hash = stable_framebuffer_hash(&framebuffer);
    RuntimeFrameComposition {
        framebuffer,
        render_tree_nodes,
        framebuffer_hash,
    }
}

pub(crate) fn stable_framebuffer_hash(framebuffer: &DecodedImage) -> u64 {
    // A fixed FNV-1a digest keeps frame diagnostics stable across platforms and
    // process invocations without introducing a rendering dependency.
    let mut hash = 0xcbf2_9ce4_8422_2325_u64;
    for byte in framebuffer
        .width
        .to_le_bytes()
        .into_iter()
        .chain(framebuffer.height.to_le_bytes())
        .chain(framebuffer.rgba.iter().copied())
    {
        hash ^= u64::from(byte);
        hash = hash.wrapping_mul(0x0000_0100_0000_01b3);
    }
    hash
}

pub(crate) fn compose_runtime_frame_through_priority(
    api: &RuntimeTraceApi,
    include_text: bool,
    maximum_priority: Option<i32>,
) -> DecodedImage {
    let (width, height) = runtime_frame_size(api);
    let Some(mut out) = new_raster(width, height) else {
        return DecodedImage {
            width: 0,
            height: 0,
            rgba: Vec::new(),
        };
    };
    for px in out.rgba.as_chunks_mut::<4>().0 {
        px.copy_from_slice(&[0, 0, 0, 255]);
    }

    // sub_430D30 draws the same CObjectManager list as the frame, stopping at
    // the priority bound; stale Back-class resources are not part of it.
    //
    // A bounded capture starts from the current background whatever its own
    // priority. Wonderful Everyday's scrgrp2._bp captures the two frames of a
    // scene change up to priority 71 (90:05 into 130 and 128) while its BackF
    // holds the background at 72, the default priority when it was built,
    // above the scene's sprites (16 + n). Without it both captures, and so
    // the whole transition, were black.
    let draw_items = api.graph_output_draw_items();
    let (backgrounds, others): (Vec<_>, Vec<_>) = draw_items.into_iter().partition(|item| {
        maximum_priority.is_some()
            && item
                .owner_object
                .and_then(|owner| api.graph_object_properties.get(&owner))
                .is_some_and(|properties| properties.background.is_some())
    });
    let others = others
        .into_iter()
        .filter(|item| maximum_priority.is_none_or(|maximum| item.z <= maximum));
    for item in backgrounds.into_iter().chain(others) {
        let Some(src) = api.graph_images.get(&item.key) else {
            continue;
        };
        composite_runtime_item(&mut out, src, &item);
    }

    if include_text && std::env::var_os("ETHORNELL_SNAPSHOT_HIDE_TEXT").is_none() {
        for node in api.text_nodes.values() {
            if maximum_priority.is_none_or(|maximum| node.z <= maximum)
                && api.should_draw_text_node(node)
            {
                let mut positioned = node.clone();
                positioned.style_spans = api.text_node_style_spans(node);
                if let Some(surface) = node
                    .target_surface
                    .filter(|surface| api.graph_surfaces.contains_key(surface))
                {
                    let (surface_x, surface_y) = api.surface_world_position(surface);
                    positioned.x += surface_x;
                    positioned.y += surface_y;
                    positioned.color[3] *= api.surface_display_opacity(surface);
                }
                let clip = node
                    .target_surface
                    .and_then(|surface| api.surface_chain_clip(surface));
                draw_text_node(
                    &mut out,
                    &positioned,
                    api.formatted_text_layout(&positioned),
                    api.text_node_shadow(&positioned),
                    clip,
                );
            }
        }
    }

    // A playing DirectShow movie covers the whole frame, text included. It
    // is not part of the object list, so a bounded capture leaves it out.
    if maximum_priority.is_none()
        && let Some(item) = api.directshow_movie_draw_item()
        && let Some(src) = api.graph_images.get(&item.key)
    {
        composite_runtime_item(&mut out, src, &item);
    }

    out
}

pub(crate) fn compose_graph_object(
    api: &RuntimeTraceApi,
    object: i32,
    target_size: Option<(u32, u32)>,
) -> Option<DecodedImage> {
    let surface = api.graph_surfaces.get(&object);
    let selected_layers = api
        .graph_layers
        .iter()
        .filter(|(id, layer)| {
            surface.is_some_and(|_| layer.target_surface == Some(object))
                || **id == object
                || layer.owner_object == Some(object)
        })
        .collect::<Vec<_>>();
    let selected_text = api
        .text_nodes
        .values()
        .filter(|node| {
            surface.is_some_and(|_| node.target_surface == Some(object))
                || node.owner_object == Some(object)
        })
        .collect::<Vec<_>>();

    let (origin_x, origin_y, content_width, content_height) = if let Some(surface) = surface {
        let (world_x, world_y) = api.surface_world_position(surface.id);
        (
            world_x,
            world_y,
            surface.viewport_width.max(surface.width).max(1.0),
            surface.viewport_height.max(surface.height).max(1.0),
        )
    } else {
        let mut min_x = f32::INFINITY;
        let mut min_y = f32::INFINITY;
        let mut max_x = f32::NEG_INFINITY;
        let mut max_y = f32::NEG_INFINITY;
        for (layer_id, layer) in &selected_layers {
            let (x, y, _) = api.layer_world_transform(**layer_id, layer);
            min_x = min_x.min(x);
            min_y = min_y.min(y);
            max_x = max_x.max(x + (layer.width * layer.scale_x).abs());
            max_y = max_y.max(y + (layer.height * layer.scale_y).abs());
        }
        for node in &selected_text {
            let (surface_x, surface_y) = node
                .target_surface
                .filter(|id| api.graph_surfaces.contains_key(id))
                .map(|id| api.surface_world_position(id))
                .unwrap_or_default();
            let x = surface_x + node.x;
            let y = surface_y + node.y;
            let line_count = node.text.lines().count().max(1) as f32;
            let max_line_chars = node
                .text
                .lines()
                .map(|line| line.chars().count())
                .max()
                .unwrap_or_default() as f32;
            min_x = min_x.min(x);
            min_y = min_y.min(y);
            max_x = max_x.max(x + (max_line_chars * node.size * 0.6).max(node.size));
            max_y = max_y.max(y + (line_count * node.size * 1.35).max(node.size));
        }
        if !min_x.is_finite() || !min_y.is_finite() {
            return None;
        }
        (
            min_x,
            min_y,
            (max_x - min_x).ceil().max(1.0),
            (max_y - min_y).ceil().max(1.0),
        )
    };
    let (width, height) = target_size
        .filter(|(width, height)| *width > 0 && *height > 0)
        .unwrap_or((content_width.ceil() as u32, content_height.ceil() as u32));
    let mut out = new_raster(width.max(1), height.max(1))?;

    if let Some(surface) = surface
        && let Some(resource_id) = surface.resource_id
        && let Some((key, region)) = api.resource_image_region(resource_id)
        && let Some(source) = api.graph_images.get(key)
    {
        let viewport_x = surface.viewport_x.max(0.0);
        let viewport_y = surface.viewport_y.max(0.0);
        let draw_width = surface
            .viewport_width
            .min(region.width - viewport_x)
            .max(0.0);
        let draw_height = surface
            .viewport_height
            .min(region.height - viewport_y)
            .max(0.0);
        composite_nearest(
            &mut out,
            source,
            0.0,
            0.0,
            draw_width,
            draw_height,
            region.x + viewport_x,
            region.y + viewport_y,
            draw_width,
            draw_height,
            surface.opacity,
            api.graph_object_properties
                .get(&surface.id)
                .map(|properties| properties.blend_mode)
                .unwrap_or(128),
            None,
        );
    }

    let mut draw_items = selected_layers
        .into_iter()
        .filter_map(|(id, layer)| capture_draw_item(api, *id, layer, origin_x, origin_y))
        .collect::<Vec<_>>();
    draw_items.sort_by_key(crate::graph::native_draw_order);
    for item in draw_items {
        let Some(source) = api.graph_images.get(&item.key) else {
            continue;
        };
        composite_runtime_item(&mut out, source, &item);
    }

    for node in selected_text {
        if !node.enabled || !api.object_enabled_for_layer(node.owner_object) || node.text.is_empty()
        {
            continue;
        }
        let mut positioned = node.clone();
        positioned.style_spans = api.text_node_style_spans(node);
        if let Some(surface) = node
            .target_surface
            .filter(|surface| api.graph_surfaces.contains_key(surface))
        {
            let (surface_x, surface_y) = api.surface_world_position(surface);
            positioned.x += surface_x;
            positioned.y += surface_y;
        }
        positioned.x -= origin_x;
        positioned.y -= origin_y;
        let clip = node
            .target_surface
            .and_then(|surface| api.surface_chain_clip(surface))
            .map(|clip| crate::graph::RuntimeClipRect {
                x: clip.x - origin_x,
                y: clip.y - origin_y,
                width: clip.width,
                height: clip.height,
            });
        draw_text_node(
            &mut out,
            &positioned,
            api.formatted_text_layout(&positioned),
            api.text_node_shadow(&positioned),
            clip,
        );
    }

    Some(out)
}

fn capture_draw_item(
    api: &RuntimeTraceApi,
    layer_id: i32,
    layer: &RuntimeGraphLayer,
    origin_x: f32,
    origin_y: f32,
) -> Option<RuntimeGraphDrawItem> {
    if !layer.enabled || !api.object_enabled_for_layer(layer.owner_object) {
        return None;
    }
    let object_opacity =
        api.object_chain_opacity(layer.owner_object.filter(|object| *object != layer_id));
    let node_properties = api.graph_object_properties.get(&layer_id);
    let owner_properties = layer
        .owner_object
        .and_then(|object| api.graph_object_properties.get(&object));
    let node_opacity = if api.graph_transition_nodes.contains_key(&layer_id) {
        1.0
    } else {
        node_properties
            .map(crate::graph::RuntimeGraphObjectProperties::opacity)
            .unwrap_or(1.0)
    };
    let blend_mode = node_properties
        .or(owner_properties)
        .map(|properties| properties.blend_mode)
        .unwrap_or(128);
    let opacity = layer.opacity * node_opacity * object_opacity;
    if opacity <= 0.001 {
        return None;
    }
    let draw_key = api
        .message_control_layer_draw_key(layer_id, layer)
        .unwrap_or(layer.key.as_str());
    let image = api.graph_images.get(draw_key)?;
    if layer.width <= 0.0 || layer.height <= 0.0 {
        return None;
    }
    let src_x = layer.src_x.clamp(0.0, image.width.saturating_sub(1) as f32);
    let src_y = layer
        .src_y
        .clamp(0.0, image.height.saturating_sub(1) as f32);
    let src_width = layer.width.min(image.width as f32 - src_x).max(0.0);
    let src_height = layer.height.min(image.height as f32 - src_y).max(0.0);
    let width = (src_width * layer.scale_x).abs();
    let height = (src_height * layer.scale_y).abs();
    let (world_x, world_y, world_z) = api.layer_world_transform(layer_id, layer);
    let item_x = world_x - origin_x;
    let item_y = world_y - origin_y;
    // Mode5 render state belongs to the sprite's primary layer, whose layer
    // id is the sprite id. Do not leak that affine transform into any nested
    // auxiliary layer that merely shares the same owner.
    let mode5_render_state = api.graph_mode5_render_states.get(&layer_id).copied();
    let destination_quad = mode5_render_state.map(|state| {
        state
            .local_affine_quad
            .map(|[local_x, local_y]| [item_x + local_x, item_y + local_y])
    });
    let mut draw_clip = api
        .layer_display_clip(layer)
        .map(|clip| crate::graph::RuntimeClipRect {
            x: clip.x - origin_x,
            y: clip.y - origin_y,
            width: clip.width,
            height: clip.height,
        });
    if mode5_render_state.is_some() {
        let raster_clip = crate::graph::RuntimeClipRect {
            x: item_x,
            y: item_y,
            width,
            height,
        };
        draw_clip = Some(match draw_clip {
            Some(existing) => existing.intersection(raster_clip),
            None => raster_clip,
        });
    }
    (src_width > 0.0 && src_height > 0.0).then(|| RuntimeGraphDrawItem {
        owner_object: layer.owner_object.or(Some(layer_id)),
        key: draw_key.to_string(),
        x: item_x,
        y: item_y,
        width,
        height,
        src_x,
        src_y,
        src_width,
        src_height,
        opacity,
        ignore_source_alpha: api.graph_layer_ignores_source_alpha(layer_id, layer),
        rotation_degrees: layer.rotation_degrees,
        destination_quad,
        linear_sampling: mode5_render_state
            .map(|state| state.linear_sampling)
            .unwrap_or(true),
        clip: draw_clip,
        z: world_z,
        blend_mode,
        order_serial: api.display_order_serial(layer_id, layer.owner_object),
        hit_id: layer.hit_id,
    })
}

pub(crate) fn draw_text_node(
    dst: &mut DecodedImage,
    node: &crate::text::RuntimeTextNode,
    layout: crate::text::RuntimeFormattedTextLayout,
    shadow: TextNodeShadow,
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    if !valid_raster(dst)
        || !valid_text_position(node.x, node.y)
        || !valid_text_raster(node.size, node.text.chars().count())
    {
        return;
    }
    check_session_coverage(&node.text);
    let Some(font) = snapshot_font() else {
        draw_fallback_text(dst, node, clip);
        return;
    };
    let (default_shadow, message_effect) = match shadow {
        TextNodeShadow::Default(shadow) => (shadow, None),
        TextNodeShadow::Message(effect) => (None, effect),
    };
    let scale = PxScale::from(node.size.max(12.0));
    let scaled = font.as_scaled(scale);
    let line_height = layout.line_height;
    let max_x = (dst.width as f32 - 36.0).max(node.x + 64.0);
    let mut x = node.x;
    let mut y = node.y + layout.body_y_offset + scaled.ascent();
    let mut char_index = 0usize;

    for ch in node.text.chars() {
        if ch == '\n' {
            x = node.x;
            y += line_height;
            char_index += 1;
            continue;
        }
        let style = runtime_text_style_at(&node.style_spans, char_index);
        let color = runtime_text_style_color(node.color, style);
        let bold = style.is_some_and(|style| style.style.bold);
        let italic = style.is_some_and(|style| style.style.italic);
        let glyph_id = font.glyph_id(ch);
        let advance = scaled.h_advance(glyph_id).max(node.size * 0.5);
        if x + advance > max_x {
            x = node.x;
            y += line_height;
        }
        if let Some(effect) = message_effect.filter(|effect| effect.mode == 2) {
            let placed = crate::text_layout::PlacedGlyph {
                ch,
                index: char_index,
                x,
                y: y - scaled.ascent(),
            };
            rasterize_placed_glyph_records(
                dst,
                1,
                &[placed],
                node.size.max(12.0),
                color,
                &|_| None,
                Some(effect),
            );
            x += advance;
            char_index += 1;
            continue;
        }
        let glyph = glyph_id.with_scale_and_position(scale, point(x, y));
        if let Some(effect) = message_effect {
            let shadow_color = [
                ((effect.rgb >> 16) & 0xff) as f32 / 255.0,
                ((effect.rgb >> 8) & 0xff) as f32 / 255.0,
                (effect.rgb & 0xff) as f32 / 255.0,
                color[3] * effect.concentration as f32 / 256.0,
            ];
            draw_glyph(
                dst,
                &font,
                glyph.clone(),
                shadow_color,
                effect.dx,
                effect.dy,
                bold,
                italic,
                clip,
            );
        } else if let Some((shadow_x, shadow_y, shadow_alpha)) =
            resolve_text_shadow(default_shadow, node.size)
        {
            draw_glyph(
                dst,
                &font,
                glyph.clone(),
                [0.0, 0.0, 0.0, color[3] * shadow_alpha],
                shadow_x,
                shadow_y,
                bold,
                italic,
                clip,
            );
        }
        draw_glyph(dst, &font, glyph, color, 0, 0, bold, italic, clip);
        x += advance;
        char_index += 1;
    }

    for (ruby, x, y, size) in crate::text::ruby_draw_runs(node, layout) {
        draw_text_run(
            dst,
            &font,
            &ruby,
            x,
            y,
            size,
            node.color,
            default_shadow,
            clip,
        );
    }
}

fn draw_text_run(
    dst: &mut DecodedImage,
    font: &FontArc,
    text: &str,
    mut x: f32,
    y: f32,
    size: f32,
    color: [f32; 4],
    shadow: Option<(i32, i32, i32)>,
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    if !valid_text_position(x, y) || !valid_text_raster(size, text.chars().count()) {
        return;
    }
    check_font_coverage(font, text.chars());
    let scale = PxScale::from(size);
    let scaled = font.as_scaled(scale);
    let baseline = y + scaled.ascent();
    for ch in text.chars() {
        let glyph_id = font.glyph_id(ch);
        let advance = scaled.h_advance(glyph_id).max(size * 0.5);
        let glyph = glyph_id.with_scale_and_position(scale, point(x, baseline));
        if let Some((shadow_x, shadow_y, shadow_alpha)) = resolve_text_shadow(shadow, size) {
            draw_glyph(
                dst,
                font,
                glyph.clone(),
                [0.0, 0.0, 0.0, color[3] * shadow_alpha],
                shadow_x,
                shadow_y,
                false,
                false,
                clip,
            );
        }
        draw_glyph(dst, font, glyph, color, 0, 0, false, false, clip);
        x += advance;
    }
}

fn resolve_text_shadow(
    shadow: Option<(i32, i32, i32)>,
    font_height: f32,
) -> Option<(i32, i32, f32)> {
    let (x_percent, y_percent, concentration) = shadow?;
    let height = font_height.max(0.0).round() as i32;
    Some((
        height.saturating_mul(x_percent) / 100,
        height.saturating_mul(y_percent) / 100,
        (256 - concentration.clamp(0, 256)) as f32 / 256.0,
    ))
}

fn snapshot_font() -> Option<FontArc> {
    // Never perform filesystem IO in a glyph/measurement hot path. Production
    // frontends initialize a session explicitly; unscoped calls are diagnostic.
    let font = SESSION_FONT.with(|s| s.borrow().clone());
    if font.is_none() {
        record_raster_error("CPU font session not initialized");
    }
    font
}

/// Rasterize the target's direct bitmap-text path into an existing RGBA
/// bitmap. Unlike `draw_text_node`, this routine intentionally has no
/// message-window margin or automatic wrapping: Graph92:9C/sub_434C50 writes
/// glyphs into the caller-provided bitmap at the supplied coordinates and
/// advances only on explicit newlines.
pub(crate) fn rasterize_bitmap_text(
    dst: &mut DecodedImage,
    text: &str,
    start_x: i32,
    start_y: i32,
    size: f32,
    spacing: f32,
    horizontal_scale_percent: f32,
    color: [f32; 4],
) -> (i32, i32) {
    if !valid_raster(dst)
        || !valid_text_position(start_x as f32, start_y as f32)
        || !valid_text_raster(size, text.chars().count())
    {
        return (start_x, start_y);
    }
    if !spacing.is_finite() || !horizontal_scale_percent.is_finite() {
        record_raster_error("non-finite CPU bitmap text spacing/scale");
        return (start_x, start_y);
    }
    check_session_coverage(text);
    let size = size.max(1.0);
    let horizontal_scale = if horizontal_scale_percent > 0.0 {
        horizontal_scale_percent / 100.0
    } else {
        1.0
    };
    let line_height = size.max(1.0);
    let mut x = start_x as f32;
    let mut line_y = start_y as f32;

    let Some(font) = snapshot_font() else {
        let char_w = (size * 0.55).max(1.0);
        let char_h = (size * 0.85).max(1.0) as i32;
        for ch in text.chars() {
            if ch == '\n' {
                x = start_x as f32;
                line_y += line_height;
                continue;
            }
            let width = (char_w * horizontal_scale).max(1.0).round() as i32;
            fill_rect(
                dst,
                x.round() as i32,
                line_y.round() as i32,
                width,
                char_h,
                color,
                None,
            );
            x += char_w * horizontal_scale + spacing;
        }
        return (x.round() as i32, line_y.round() as i32);
    };

    let scale = PxScale::from(size);
    let scaled = font.as_scaled(scale);
    for ch in text.chars() {
        if ch == '\n' {
            x = start_x as f32;
            line_y += line_height;
            continue;
        }
        let glyph_id = font.glyph_id(ch);
        let advance = scaled.h_advance(glyph_id).max(size * 0.5);
        let baseline = line_y + scaled.ascent();
        let glyph = glyph_id.with_scale_and_position(scale, point(x, baseline));
        draw_glyph(dst, &font, glyph, color, 0, 0, false, false, None);
        // Config resources use 100%; preserve the target's horizontal-scale
        // advance contract for other callers even though ab_glyph does not
        // provide an inexpensive per-glyph X-only transform here.
        x += advance * horizontal_scale + spacing;
    }
    (x.round() as i32, line_y.round() as i32)
}

/// One glyph of the Graph92:1E direct text path: a `width` x `height`
/// format-2 cell (ink is `color | 0xFF000000`, the rest transparent) to be
/// blitted with mode 0 at (`x`, `y`).
pub(crate) struct DirectTextGlyph {
    pub x: i32,
    pub y: i32,
    pub cell: DecodedImage,
}

/// sub_403840: lay out Shift-JIS `bytes` from (`start_x`, `start_y`).
/// Every character occupies a fixed cell: `size` wide for a double-byte
/// code, `size / 2` for a single byte, advancing by that width plus
/// `spacing`. 0x03 <n> sets the line pitch to n percent of `size`, 0x04
/// wraps at the bitmap width, 0x0A starts a new line; other control bytes
/// are skipped. Layout stops at the first glyph that does not overlap the
/// `bitmap_width` x `bitmap_height` bitmap (sub_40A530 returns 4).
/// Returns the glyphs and the accumulated advance.
pub(crate) fn layout_direct_bitmap_text(
    bytes: &[u8],
    start_x: i32,
    start_y: i32,
    size: u32,
    bold: bool,
    spacing: i32,
    packed_rgb: u32,
    bitmap_width: u32,
    bitmap_height: u32,
) -> (Vec<DirectTextGlyph>, i32) {
    if !valid_text_raster(size as f32, bytes.len()) {
        return (Vec::new(), 0);
    }
    let size_i = size as i32;
    let mut glyphs = Vec::new();
    let mut x = start_x;
    let mut y = start_y;
    let mut line_percent = 100_i32;
    let mut wrap: u32 = 0;
    let mut total = 0_i32;
    let mut index = 0;
    let mut allocated = 0usize;
    while index < bytes.len() && bytes[index] != 0 {
        let lead = bytes[index];
        // sub_42FA80: 0x80..0x9F and 0xE0..0xFF lead a double-byte code.
        let double = lead >= 0x80 && !(0xA0..0xE0).contains(&lead);
        let code = if double {
            (u32::from(lead) << 8) | u32::from(bytes.get(index + 1).copied().unwrap_or(0))
        } else {
            u32::from(lead)
        };
        if code >= 0x20 {
            let width = if double { size } else { size >> 1 };
            if wrap != 0 && (x as i64 + i64::from(width)) > i64::from(wrap) {
                x = start_x;
                y = y.wrapping_add(size_i * line_percent / 100);
            }
            let visible = i64::from(x) < i64::from(bitmap_width)
                && i64::from(x) + i64::from(width) > 0
                && i64::from(y) < i64::from(bitmap_height)
                && i64::from(y) + i64::from(size) > 0;
            if !visible {
                break;
            }
            let cell_bytes = width.max(1) as usize * size as usize * 4;
            allocated = allocated.saturating_add(cell_bytes);
            if allocated > MAX_RASTER_BYTES {
                record_raster_error("direct text cell budget exceeded");
                return (Vec::new(), 0);
            }
            let ink = direct_text_glyph_cell(code, size, bold);
            if ink.is_empty() && size != 0 {
                return (Vec::new(), 0);
            }
            let Some(mut cell) = new_raster(width.max(1), size) else {
                return (Vec::new(), 0);
            };
            let rgba = [
                (packed_rgb >> 16) as u8,
                (packed_rgb >> 8) as u8,
                packed_rgb as u8,
                0xFF,
            ];
            for row in 0..size {
                for col in 0..width {
                    if ink[(row * size + col) as usize] {
                        let at = ((row * cell.width + col) * 4) as usize;
                        cell.rgba[at..at + 4].copy_from_slice(&rgba);
                    }
                }
            }
            if width > 0 {
                glyphs.push(DirectTextGlyph { x, y, cell });
            }
            x = x.wrapping_add((width as i32).wrapping_add(spacing));
            total = total.wrapping_add((width as i32).wrapping_add(spacing));
        } else {
            match code {
                3 => {
                    line_percent = i32::from(bytes.get(index + 1).copied().unwrap_or(0));
                    index += 1;
                }
                4 => wrap = bitmap_width,
                10 => {
                    x = start_x;
                    y = y.wrapping_add(size_i * line_percent / 100);
                }
                _ => {}
            }
        }
        index += if double { 2 } else { 1 };
    }
    (glyphs, total)
}

/// 403600/403680 -> 4097F0: immediate plain SJIS text. Font width is a
/// percentage, bold is Boolean, the ninth operand selects proportional cells,
/// and the tenth is packed RGB. Wrapping reserves one full cell at the right.
pub(crate) fn layout_styled_bitmap_text(
    bytes:&[u8],origin:[i32;2],size:u32,width_percent:i32,bold:bool,
    proportional:bool,packed_rgb:u32,wrap:bool,line_pitch_percent:i32,bitmap:[u32;2]
)->(Vec<DirectTextGlyph>,i32) {
    const HANGING:&str=",.､｡:;?!ﾞﾟ･、。，．：；？！゛゜‐]})）〕］｝〉》」』】‘“";
    let mut glyphs=Vec::new();let (mut x,mut y)=(origin[0],origin[1]);let mut lines=1i32;
    let (mut result,mut maximum)=(0i32,0i32);let mut hanging=false;let mut index=0;
    let full=(size as i32).wrapping_mul(width_percent)/100;
    let right=bitmap[0].wrapping_sub(full as u32);
    let line_step=(size as i32).wrapping_add((size as i32).wrapping_mul(line_pitch_percent)/100);
    let font=snapshot_font();let rgb=[(packed_rgb>>16) as u8,(packed_rgb>>8) as u8,packed_rgb as u8];
    let mut allocated=0usize;
    while index<bytes.len() && bytes[index]!=0 {
        let lead=bytes[index];let double=lead>=0x80&&!(0xA0..0xE0).contains(&lead);
        let n=if double {2}else{1};let end=(index+n).min(bytes.len());
        let text=encoding_rs::SHIFT_JIS.decode(&bytes[index..end]).0.into_owned();let ch=text.chars().next().unwrap_or('\0');index=end;
        if lead==10 {lines=lines.wrapping_add(1);x=origin[0];y=y.wrapping_add(line_step);hanging=false;if !wrap {maximum=maximum.max(result);result=0;}continue;}
        check_session_coverage(&text);
        let nominal=if double {full}else{full/2};
        let Some(mut cell)=new_raster((2*full).max(1) as u32,size) else{return (Vec::new(),0);};
        allocated=allocated.saturating_add(cell.rgba.len());if allocated>MAX_RASTER_BYTES {record_raster_error("styled text cell budget exceeded");return (Vec::new(),0);}
        let mut ink_left=cell.width;let mut ink_right=0u32;let mut has_ink=false;
        if let Some(font)=&font {
            let scale=PxScale{x:size as f32*width_percent as f32/100.0,y:size as f32};
            let baseline=font.as_scaled(scale).ascent();
            if let Some(outlined)=font.outline_glyph(font.glyph_id(ch).with_scale_and_position(scale,point(0.0,baseline))) {
                let bounds=outlined.px_bounds();
                if valid_glyph_bounds(bounds) {outlined.draw(|gx,gy,coverage|{
                    let py=bounds.min.y as i32+gy as i32;let px=bounds.min.x as i32+gx as i32;
                    let alpha=(coverage.clamp(0.0,1.0)*255.0).round() as u8;
                    if py<0||py>=size as i32||alpha==0 {return;}
                    for dx in 0..=i32::from(bold) {let px=px+dx;if px<0||px>=cell.width as i32 {continue;}
                        let at=(py as usize*cell.width as usize+px as usize)*4;
                        if alpha>cell.rgba[at+3] {cell.rgba[at..at+4].copy_from_slice(&[rgb[0],rgb[1],rgb[2],alpha]);}
                        has_ink=true;ink_left=ink_left.min(px as u32);ink_right=ink_right.max(px as u32);
                    }
                });}
            }
        }
        let (cell_width,crop_x,padding)=if proportional {
            let ink_width=if has_ink {(ink_right-ink_left+1) as i32}else{nominal.max(0)};
            let ratio=16*ink_width/(size as i32).max(1);let padding=if ratio>=8 {1}else{[4,4,4,3,3,3,3,2][ratio.max(0) as usize]};
            (ink_width,if has_ink {ink_left as i32}else{0},padding)
        }else{(nominal,0,0)};
        let step=cell_width.wrapping_add(2*padding);
        if wrap && x.wrapping_add(step) as u32>right {
            if hanging||!HANGING.contains(ch) {y=y.wrapping_add(line_step);lines=lines.wrapping_add(1);x=origin[0];hanging=false;}else{hanging=true;}
        }
        let draw_x=x.wrapping_add(padding);
        if draw_x as i64>=bitmap[0] as i64||y as i64>=bitmap[1] as i64||draw_x as i64+cell_width as i64<=0||y as i64+size as i64<=0 {break;}
        let cropped=crate::graph::crop_decoded_image(&cell,crate::graph::RuntimeClipRect{x:crop_x as f32,y:0.0,width:cell_width.max(1) as f32,height:size as f32});
        glyphs.push(DirectTextGlyph{x:draw_x,y,cell:cropped});x=x.wrapping_add(step);
        if wrap {result=lines;}else{result=result.wrapping_add(step);}
    }
    (glyphs,if wrap {result}else{maximum.max(result)})
}

/// sub_42F790: draw one code with TextOut at (0, 0) into the font's
/// monochrome `size` x `size` DIB. 0x7F is drawn as U+2014 and 0xEF40 as
/// two U+2014; other codes at or above 0xEF40 draw nothing.
fn direct_text_glyph_cell(code: u32, size: u32, bold: bool) -> Vec<bool> {
    if !valid_text_raster(size as f32, 1) {
        return Vec::new();
    }
    let mut ink = vec![false; (size * size) as usize];
    let text: String = match code {
        0x7F => "\u{2014}".into(),
        0xEF40 => "\u{2014}\u{2014}".into(),
        c if c >= 0xEF40 => return ink,
        c if c < 0x100 => encoding_rs::SHIFT_JIS.decode(&[c as u8]).0.into_owned(),
        c => encoding_rs::SHIFT_JIS
            .decode(&[(c >> 8) as u8, c as u8])
            .0
            .into_owned(),
    };
    check_session_coverage(&text);
    let Some(font) = snapshot_font() else {
        return ink;
    };
    let scale = PxScale::from(size as f32);
    let scaled = font.as_scaled(scale);
    let mut pen = 0.0_f32;
    for ch in text.chars() {
        let glyph = font
            .glyph_id(ch)
            .with_scale_and_position(scale, point(pen, scaled.ascent()));
        pen += scaled.h_advance(font.glyph_id(ch));
        let Some(outlined) = font.outline_glyph(glyph) else {
            continue;
        };
        let bounds = outlined.px_bounds();
        if !valid_glyph_bounds(bounds) {
            return Vec::new();
        }
        outlined.draw(|gx, gy, coverage| {
            if coverage < 0.5 {
                return;
            }
            let y = bounds.min.y as i32 + gy as i32;
            for dx in 0..=i32::from(bold) {
                let x = bounds.min.x as i32 + gx as i32 + dx;
                if (0..size as i32).contains(&x) && (0..size as i32).contains(&y) {
                    ink[(y as u32 * size + x as u32) as usize] = true;
                }
            }
        });
    }
    ink
}

/// Horizontal advance of one glyph, scaled by the horizontal percentage.
pub(crate) fn glyph_advance(ch: char, size: f32, horizontal_scale_percent: f32) -> f32 {
    if !valid_text_raster(size, 1) {
        return 0.0;
    }
    check_session_coverage(&ch.to_string());
    let scale = if horizontal_scale_percent > 0.0 {
        horizontal_scale_percent / 100.0
    } else {
        1.0
    };
    let size = size.max(1.0);
    let base = match snapshot_font() {
        Some(font) => font
            .as_scaled(PxScale::from(size))
            .h_advance(font.glyph_id(ch))
            .max(size * 0.5),
        None if ch.is_ascii() => size * 0.5,
        None => size,
    };
    base * scale
}

/// Draw glyphs placed by [`crate::text_layout::layout_text`]. `color_of` may
/// override the colour of a character (markup colour spans).
pub(crate) fn rasterize_placed_glyphs(
    dst: &mut DecodedImage,
    glyphs: &[crate::text_layout::PlacedGlyph],
    size: f32,
    color: [f32; 4],
    color_of: &dyn Fn(usize) -> Option<[f32; 4]>,
) {
    if !valid_raster(dst) || !valid_text_raster(size, glyphs.len()) {
        return;
    }
    let size = size.max(1.0);
    let scale = PxScale::from(size);
    let font = snapshot_font();
    if let Some(font) = font.as_ref() {
        check_font_coverage(font, glyphs.iter().map(|glyph| glyph.ch));
    }
    for glyph in glyphs {
        if !valid_text_position(glyph.x, glyph.y) {
            return;
        }
        let color = color_of(glyph.index).unwrap_or(color);
        match font.as_ref() {
            Some(font) => {
                let baseline = glyph.y + font.as_scaled(scale).ascent();
                let placed = font
                    .glyph_id(glyph.ch)
                    .with_scale_and_position(scale, point(glyph.x, baseline));
                draw_glyph(dst, &font, placed, color, 0, 0, false, false, None);
            }
            None => fill_rect(
                dst,
                glyph.x.round() as i32,
                glyph.y.round() as i32,
                (size * 0.5).max(1.0) as i32,
                (size * 0.85).max(1.0) as i32,
                color,
                None,
            ),
        }
    }
}

/// How a text node's shadow is drawn: the process-wide drop shadow (x and y
/// as percent of the font size, concentration), or the style that a
/// 92:90/92:91 message call gave the node's window (`None`: no shadow).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum TextNodeShadow {
    Default(Option<(i32, i32, i32)>),
    Message(Option<TextShadow>),
}

/// The shadow style of the target text layout (`sub_434E30`): `mode` 1 is a
/// drop shadow offset by (`dx`, `dy`), mode 2 a glow of radius (`dx`, `dy`);
/// both use `rgb` and are blended with parameter `256 - concentration`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct TextShadow {
    pub mode: i32,
    pub dx: i32,
    pub dy: i32,
    pub rgb: u32,
    pub concentration: i32,
}

/// One glyph as a format-2 image (straight alpha = coverage) and its
/// top-left position.
fn glyph_cell(
    font: &FontArc,
    ch: char,
    x: f32,
    y: f32,
    size: f32,
    rgb: [u8; 3],
) -> Option<(DecodedImage, i32, i32)> {
    if !valid_text_position(x, y) {
        return None;
    }
    let scale = PxScale::from(size);
    let baseline = y + font.as_scaled(scale).ascent();
    let outlined = font.outline_glyph(
        font.glyph_id(ch)
            .with_scale_and_position(scale, point(x, baseline)),
    )?;
    let bounds = outlined.px_bounds();
    if !valid_glyph_bounds(bounds) {
        return None;
    }
    let (left, top) = (bounds.min.x as i32, bounds.min.y as i32);
    let width = (bounds.max.x as i32 - left).max(1) as u32;
    let height = (bounds.max.y as i32 - top).max(1) as u32;
    let mut cell = new_raster(width, height)?;
    outlined.draw(|gx, gy, coverage| {
        if gx < width && gy < height {
            let at = ((gy * width + gx) * 4) as usize;
            let alpha = (coverage.clamp(0.0, 1.0) * 255.0).round() as u8;
            if alpha > cell.rgba[at + 3] {
                cell.rgba[at..at + 4].copy_from_slice(&[rgb[0], rgb[1], rgb[2], alpha]);
            }
        }
    });
    Some((cell, left, top))
}

/// sub_435290's per-character records, blitted onto `dst` (format
/// `dst_format`) in order by sub_437940 with mode 0. With a shadow, each
/// record is a transparent format-2 image holding the shadow (blend mode 1,
/// parameter 256 - concentration) and then the glyph (mode 0): mode 1 puts
/// the recoloured glyph (sub_4188D0) at (+dx, +dy); mode 2 draws the glyph
/// at (+dx, +dy) over a glow whose alpha is the clamped box sum of glyph
/// coverage within (dx, dy) (sub_433180).
pub(crate) fn rasterize_placed_glyph_records(
    dst: &mut DecodedImage,
    dst_format: i32,
    glyphs: &[crate::text_layout::PlacedGlyph],
    size: f32,
    color: [f32; 4],
    color_of: &dyn Fn(usize) -> Option<[f32; 4]>,
    shadow: Option<TextShadow>,
) {
    if !valid_raster(dst) || !valid_text_raster(size, glyphs.len()) {
        return;
    }
    let Some(font) = snapshot_font() else {
        rasterize_placed_glyphs(dst, glyphs, size, color, color_of);
        return;
    };
    check_font_coverage(&font, glyphs.iter().map(|glyph| glyph.ch));
    let size = size.max(1.0);
    let byte = |value: f32| (value.clamp(0.0, 1.0) * 255.0).round() as u8;
    let shadow = shadow.filter(|shadow| matches!(shadow.mode, 1 | 2));
    for glyph in glyphs {
        let color = color_of(glyph.index).unwrap_or(color);
        let rgb = [byte(color[0]), byte(color[1]), byte(color[2])];
        let Some((cell, left, top)) = glyph_cell(&font, glyph.ch, glyph.x, glyph.y, size, rgb)
        else {
            continue;
        };
        let Some(shadow) = shadow else {
            crate::bitmap_blend::blit_mode(dst, dst_format, &cell, 2, left, top, 0, 0);
            continue;
        };
        if shadow.dx > 1024 || shadow.dy > 1024 {
            record_raster_error("text shadow raster budget exceeded");
            return;
        }
        let (dx, dy) = (shadow.dx.max(0), shadow.dy.max(0));
        let shadow_rgb = [
            (shadow.rgb >> 16) as u8,
            (shadow.rgb >> 8) as u8,
            shadow.rgb as u8,
        ];
        let parameter = 256i32.saturating_sub(shadow.concentration);
        let (spread_x, spread_y) = if shadow.mode == 1 {
            (dx, dy)
        } else {
            (2 * dx, 2 * dy)
        };
        let width = cell.width + spread_x as u32;
        let height = cell.height + spread_y as u32;
        if shadow.mode == 2
            && u64::from(width) * u64::from(height) * (2 * dx + 1) as u64 * (2 * dy + 1) as u64
                > 50_000_000
        {
            record_raster_error("text glow work budget exceeded");
            return;
        }
        let Some(mut record) = new_raster(width, height) else {
            return;
        };
        if shadow.mode == 1 {
            let mut tinted = cell.clone();
            for pixel in tinted.rgba.as_chunks_mut::<4>().0 {
                pixel[..3].copy_from_slice(&shadow_rgb);
            }
            crate::bitmap_blend::blit_mode(&mut record, 2, &tinted, 2, dx, dy, 1, parameter);
            crate::bitmap_blend::blit_mode(&mut record, 2, &cell, 2, 0, 0, 0, 0);
        } else {
            let mut glow = record.clone();
            for y in 0..height as i32 {
                for x in 0..width as i32 {
                    let mut sum = 0u32;
                    for sy in (y - 2 * dy)..=y {
                        for sx in (x - 2 * dx)..=x {
                            if (0..cell.width as i32).contains(&sx)
                                && (0..cell.height as i32).contains(&sy)
                            {
                                sum += u32::from(
                                    cell.rgba
                                        [((sy as u32 * cell.width + sx as u32) * 4 + 3) as usize],
                                );
                            }
                        }
                    }
                    let at = ((y as u32 * width + x as u32) * 4) as usize;
                    glow.rgba[at..at + 4].copy_from_slice(&[
                        shadow_rgb[0],
                        shadow_rgb[1],
                        shadow_rgb[2],
                        sum.min(255) as u8,
                    ]);
                }
            }
            crate::bitmap_blend::blit_mode(&mut record, 2, &glow, 2, 0, 0, 1, parameter);
            crate::bitmap_blend::blit_mode(&mut record, 2, &cell, 2, dx, dy, 0, 0);
        }
        crate::bitmap_blend::blit_mode(dst, dst_format, &record, 2, left, top, 0, 0);
    }
}

pub(crate) fn measure_text_advance(text: &str, size: f32, spacing: f32) -> i32 {
    if !valid_text_raster(size, text.chars().count()) {
        return 0;
    }
    check_session_coverage(text);
    let glyph_count = text.chars().count();
    if glyph_count == 0 {
        return 0;
    }
    let width = if let Some(font) = snapshot_font() {
        let scaled = font.as_scaled(PxScale::from(size.max(1.0)));
        text.chars()
            .map(|character| scaled.h_advance(font.glyph_id(character)))
            .sum::<f32>()
    } else {
        text.chars()
            .map(|character| {
                if character.is_ascii() {
                    size * 0.5
                } else {
                    size
                }
            })
            .sum::<f32>()
    };
    (width + spacing * glyph_count as f32).round() as i32
}

fn draw_glyph(
    dst: &mut DecodedImage,
    font: &FontArc,
    glyph: ab_glyph::Glyph,
    color: [f32; 4],
    dx: i32,
    dy: i32,
    bold: bool,
    italic: bool,
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    if !valid_text_position(glyph.position.x, glyph.position.y) {
        return;
    }
    let Some(outlined) = font.outline_glyph(glyph) else {
        return;
    };
    let bounds = outlined.px_bounds();
    if !valid_glyph_bounds(bounds) {
        return;
    }
    outlined.draw(|gx, gy, coverage| {
        let italic_dx = if italic {
            ((bounds.height() - gy as f32).max(0.0) * 0.20).round() as i32
        } else {
            0
        };
        let x = (bounds.min.x as i32)
            .saturating_add(gx as i32)
            .saturating_add(dx)
            .saturating_add(italic_dx);
        let y = (bounds.min.y as i32)
            .saturating_add(gy as i32)
            .saturating_add(dy);
        for bold_dx in 0..=i32::from(bold) {
            let x = x.saturating_add(bold_dx);
            if x < 0
                || y < 0
                || x >= dst.width as i32
                || y >= dst.height as i32
                || !point_in_clip(x, y, clip)
            {
                continue;
            }
            let alpha = (coverage * color[3]).clamp(0.0, 1.0);
            let src = [
                (color[0].clamp(0.0, 1.0) * 255.0).round() as u8,
                (color[1].clamp(0.0, 1.0) * 255.0).round() as u8,
                (color[2].clamp(0.0, 1.0) * 255.0).round() as u8,
                (alpha * 255.0).round() as u8,
            ];
            let i = ((y as u32 * dst.width + x as u32) * 4) as usize;
            blend_pixel_native(&mut dst.rgba[i..i + 4], &src, 1.0, 0);
        }
    });
}

fn runtime_text_style_at(
    styles: &[crate::text_anim::RuntimeTextStyleSpan],
    char_index: usize,
) -> Option<&crate::text_anim::RuntimeTextStyleSpan> {
    styles
        .iter()
        .find(|style| style.start_char <= char_index && char_index < style.end_char)
}

fn runtime_text_style_color(
    base: [f32; 4],
    style: Option<&crate::text_anim::RuntimeTextStyleSpan>,
) -> [f32; 4] {
    style
        .and_then(|style| style.style.packed_rgb)
        .map(|packed| {
            [
                ((packed >> 16) & 0xff) as f32 / 255.0,
                ((packed >> 8) & 0xff) as f32 / 255.0,
                (packed & 0xff) as f32 / 255.0,
                base[3],
            ]
        })
        .unwrap_or(base)
}

fn draw_fallback_text(
    dst: &mut DecodedImage,
    node: &crate::text::RuntimeTextNode,
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    let mut x = node.x as i32;
    let mut y = node.y as i32;
    let char_w = (node.size * 0.55).max(8.0) as i32;
    let char_h = (node.size * 0.85).max(12.0) as i32;
    for (char_index, ch) in node.text.chars().enumerate() {
        if ch == '\n' || x + char_w >= dst.width as i32 - 36 {
            x = node.x as i32;
            y += (node.size * 1.35).max(18.0) as i32;
            if ch == '\n' {
                continue;
            }
        }
        let style = runtime_text_style_at(&node.style_spans, char_index);
        fill_rect(
            dst,
            x,
            y,
            char_w - 2 + i32::from(style.is_some_and(|style| style.style.bold)),
            char_h,
            runtime_text_style_color(node.color, style),
            clip,
        );
        x += char_w;
    }
}

fn fill_rect(
    dst: &mut DecodedImage,
    x: i32,
    y: i32,
    width: i32,
    height: i32,
    color: [f32; 4],
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    for py in y.max(0)..y.saturating_add(height).min(dst.height as i32) {
        for px in x.max(0)..x.saturating_add(width).min(dst.width as i32) {
            if !point_in_clip(px, py, clip) {
                continue;
            }
            let src = [
                (color[0].clamp(0.0, 1.0) * 255.0).round() as u8,
                (color[1].clamp(0.0, 1.0) * 255.0).round() as u8,
                (color[2].clamp(0.0, 1.0) * 255.0).round() as u8,
                (color[3].clamp(0.0, 1.0) * 255.0).round() as u8,
            ];
            let i = ((py as u32 * dst.width + px as u32) * 4) as usize;
            blend_pixel_native(&mut dst.rgba[i..i + 4], &src, 1.0, 0);
        }
    }
}

fn point_in_clip(x: i32, y: i32, clip: Option<crate::graph::RuntimeClipRect>) -> bool {
    clip.is_none_or(|clip| {
        let x = x as f32;
        let y = y as f32;
        x >= clip.x && y >= clip.y && x < clip.x + clip.width && y < clip.y + clip.height
    })
}

pub(crate) fn composite_runtime_item(dst: &mut DecodedImage, src: &DecodedImage, item: &RuntimeGraphDrawItem) {
    if !valid_raster(dst) || !valid_raster(src) {
        return;
    }
    if let Some(quad) = crate::native_sprite_filters::item_quad(item) {
        composite_affine_mode5(
            dst,
            src,
            quad,
            item.src_x,
            item.src_y,
            item.src_width,
            item.src_height,
            item.opacity,
            item.blend_mode,
            item.clip,
            item.linear_sampling,
            item.ignore_source_alpha,
        );
    } else {
        composite_nearest_with_format(
            dst,
            src,
            item.x,
            item.y,
            item.width,
            item.height,
            item.src_x,
            item.src_y,
            item.src_width,
            item.src_height,
            item.opacity,
            item.blend_mode,
            item.clip,
            item.ignore_source_alpha,
        );
    }
}

fn composite_affine_mode5(
    dst: &mut DecodedImage,
    src: &DecodedImage,
    quad: [[f32; 2]; 4],
    src_x: f32,
    src_y: f32,
    src_width: f32,
    src_height: f32,
    opacity: f32,
    blend_mode: i32,
    clip: Option<crate::graph::RuntimeClipRect>,
    linear_sampling: bool,
    ignore_source_alpha: bool,
) {
    if src.width == 0 || src.height == 0 || src_width <= 0.0 || src_height <= 0.0 || opacity <= 0.0
    {
        return;
    }
    let [top_left, bottom_left, _bottom_right, top_right] = quad;
    let ux = top_right[0] - top_left[0];
    let uy = top_right[1] - top_left[1];
    let vx = bottom_left[0] - top_left[0];
    let vy = bottom_left[1] - top_left[1];
    let determinant = ux * vy - uy * vx;
    if determinant.abs() <= f32::EPSILON {
        return;
    }

    let min_x = quad
        .iter()
        .map(|point| point[0])
        .fold(f32::INFINITY, f32::min);
    let max_x = quad
        .iter()
        .map(|point| point[0])
        .fold(f32::NEG_INFINITY, f32::max);
    let min_y = quad
        .iter()
        .map(|point| point[1])
        .fold(f32::INFINITY, f32::min);
    let max_y = quad
        .iter()
        .map(|point| point[1])
        .fold(f32::NEG_INFINITY, f32::max);
    let clip_x0 = clip.map(|clip| clip.x).unwrap_or(0.0);
    let clip_y0 = clip.map(|clip| clip.y).unwrap_or(0.0);
    let clip_x1 = clip
        .map(|clip| clip.x + clip.width)
        .unwrap_or(dst.width as f32);
    let clip_y1 = clip
        .map(|clip| clip.y + clip.height)
        .unwrap_or(dst.height as f32);
    let x0 = min_x.floor().max(0.0).max(clip_x0) as i32;
    let y0 = min_y.floor().max(0.0).max(clip_y0) as i32;
    let x1 = max_x.ceil().min(dst.width as f32).min(clip_x1) as i32;
    let y1 = max_y.ceil().min(dst.height as f32).min(clip_y1) as i32;
    if x0 >= x1 || y0 >= y1 {
        return;
    }

    let alpha_scale = opacity.clamp(0.0, 1.0);
    for dy in y0..y1 {
        for dx in x0..x1 {
            let px = dx as f32 + 0.5 - top_left[0];
            let py = dy as f32 + 0.5 - top_left[1];
            let u = (px * vy - py * vx) / determinant;
            let v = (ux * py - uy * px) / determinant;
            if !(0.0..1.0).contains(&u) || !(0.0..1.0).contains(&v) {
                continue;
            }
            // UV 0/1 represent source texel edges. Convert back to the native
            // convention where integer source coordinates are texel centres.
            let source_x = src_x + u * src_width - 0.5;
            let source_y = src_y + v * src_height - 0.5;
            let source = if linear_sampling {
                sample_mode5_bilinear(src, source_x, source_y)
            } else {
                sample_mode5_nearest(src, source_x, source_y)
            };
            let Some(mut source) = source else {
                continue;
            };
            if ignore_source_alpha {
                source[3] = 255;
            }
            let dst_i = ((dy as u32 * dst.width + dx as u32) * 4) as usize;
            blend_pixel_native(
                &mut dst.rgba[dst_i..dst_i + 4],
                &source,
                alpha_scale,
                blend_mode,
            );
        }
    }
}

fn sample_mode5_nearest(src: &DecodedImage, x: f32, y: f32) -> Option<[u8; 4]> {
    let x = x.floor() as i32;
    let y = y.floor() as i32;
    source_pixel_or_none(src, x, y)
}

fn sample_mode5_bilinear(src: &DecodedImage, x: f32, y: f32) -> Option<[u8; 4]> {
    let x0 = x.floor() as i32;
    let y0 = y.floor() as i32;
    // sub_417C50 uses the high four bits of each 16-bit fractional word for
    // its MMX interpolation table. Quantize to the same 1/16 steps.
    let fx = ((x - x.floor()) * 16.0).floor().clamp(0.0, 15.0) / 16.0;
    let fy = ((y - y.floor()) * 16.0).floor().clamp(0.0, 15.0) / 16.0;
    let p00 = source_pixel_or_zero(src, x0, y0);
    let p10 = source_pixel_or_zero(src, x0 + 1, y0);
    let p01 = source_pixel_or_zero(src, x0, y0 + 1);
    let p11 = source_pixel_or_zero(src, x0 + 1, y0 + 1);
    if p00 == [0; 4] && p10 == [0; 4] && p01 == [0; 4] && p11 == [0; 4] {
        return None;
    }
    let mut out = [0u8; 4];
    for channel in 0..4 {
        let top = p00[channel] as f32 * (1.0 - fx) + p10[channel] as f32 * fx;
        let bottom = p01[channel] as f32 * (1.0 - fx) + p11[channel] as f32 * fx;
        out[channel] = (top * (1.0 - fy) + bottom * fy).floor().clamp(0.0, 255.0) as u8;
    }
    Some(out)
}

fn source_pixel_or_none(src: &DecodedImage, x: i32, y: i32) -> Option<[u8; 4]> {
    if x < 0 || y < 0 || x >= src.width as i32 || y >= src.height as i32 {
        return None;
    }
    let index = ((y as u32 * src.width + x as u32) * 4) as usize;
    src.rgba.get(index..index + 4)?.try_into().ok()
}

fn source_pixel_or_zero(src: &DecodedImage, x: i32, y: i32) -> [u8; 4] {
    source_pixel_or_none(src, x, y).unwrap_or([0; 4])
}

fn composite_nearest(
    dst: &mut DecodedImage,
    src: &DecodedImage,
    x: f32,
    y: f32,
    width: f32,
    height: f32,
    src_x: f32,
    src_y: f32,
    src_width: f32,
    src_height: f32,
    opacity: f32,
    blend_mode: i32,
    clip: Option<crate::graph::RuntimeClipRect>,
) {
    composite_nearest_with_format(dst,src,x,y,width,height,src_x,src_y,src_width,src_height,opacity,blend_mode,clip,false);
}

fn composite_nearest_with_format(
    dst: &mut DecodedImage,
    src: &DecodedImage,
    x: f32,
    y: f32,
    width: f32,
    height: f32,
    src_x: f32,
    src_y: f32,
    src_width: f32,
    src_height: f32,
    opacity: f32,
    blend_mode: i32,
    clip: Option<crate::graph::RuntimeClipRect>,
    ignore_source_alpha:bool,
) {
    if !valid_raster(dst) || !valid_raster(src) {
        return;
    }
    if src.width == 0
        || src.height == 0
        || width <= 0.0
        || height <= 0.0
        || src_width <= 0.0
        || src_height <= 0.0
        || opacity <= 0.0
    {
        return;
    }
    let alpha_scale = opacity.clamp(0.0, 1.0);
    let clip_x0 = clip.map(|clip| clip.x).unwrap_or(0.0);
    let clip_y0 = clip.map(|clip| clip.y).unwrap_or(0.0);
    let clip_x1 = clip
        .map(|clip| clip.x + clip.width)
        .unwrap_or(dst.width as f32);
    let clip_y1 = clip
        .map(|clip| clip.y + clip.height)
        .unwrap_or(dst.height as f32);
    let x0 = x.floor().max(0.0).max(clip_x0) as i32;
    let y0 = y.floor().max(0.0).max(clip_y0) as i32;
    let x1 = (x + width).ceil().min(dst.width as f32).min(clip_x1) as i32;
    let y1 = (y + height).ceil().min(dst.height as f32).min(clip_y1) as i32;
    if x0 >= x1 || y0 >= y1 {
        return;
    }

    // The common screen/background copy has no scaling or fractional source
    // coordinates. Walk contiguous rows instead of dividing/clamping UVs and
    // evaluating the full floating-point blend pipeline for every pixel.
    if alpha_scale == 1.0 && blend_mode == 128
        && width == src_width && height == src_height
        && [x, y, src_x, src_y, width, height].iter()
            .all(|value| value.abs() <= 32768.0 && value.fract() == 0.0)
    {
        let left = src_x as i64 + i64::from(x0) - x as i64;
        let top = src_y as i64 + i64::from(y0) - y as i64;
        let rows = (y1 - y0) as usize;
        let columns = (x1 - x0) as usize;
        if left >= 0 && top >= 0 && left + columns as i64 <= i64::from(src.width)
            && top + rows as i64 <= i64::from(src.height)
        {
            for row in 0..rows {
                let source_start = ((top as usize + row) * src.width as usize + left as usize) * 4;
                let destination_start = ((y0 as usize + row) * dst.width as usize + x0 as usize) * 4;
                let source = &src.rgba[source_start..source_start + columns * 4];
                let destination = &mut dst.rgba[destination_start..destination_start + columns * 4];
                for (pixel, sample) in destination.as_chunks_mut::<4>().0.iter_mut()
                    .zip(source.as_chunks::<4>().0)
                {
                    if ignore_source_alpha || sample[3] == 255 {
                        *pixel = *sample;
                        pixel[3] = 255;
                    } else if sample[3] != 0 {
                        composite_native_rgba(pixel, *sample, 1.0, blend_mode);
                    }
                }
            }
            return;
        }
    }
    for dy in y0..y1 {
        let v = ((dy as f32 + 0.5 - y) / height).clamp(0.0, 0.999_999);
        let sy = (src_y + v * src_height).clamp(0.0, src.height.saturating_sub(1) as f32) as u32;
        for dx in x0..x1 {
            let u = ((dx as f32 + 0.5 - x) / width).clamp(0.0, 0.999_999);
            let sx = (src_x + u * src_width).clamp(0.0, src.width.saturating_sub(1) as f32) as u32;
            let src_i = ((sy * src.width + sx) * 4) as usize;
            let dst_i = ((dy as u32 * dst.width + dx as u32) * 4) as usize;
            let mut sample: [u8;4]=src.rgba[src_i..src_i+4].try_into().unwrap();
            if ignore_source_alpha {sample[3]=255;}
            blend_pixel_native(
                &mut dst.rgba[dst_i..dst_i + 4],
                &sample,
                alpha_scale,
                blend_mode,
            );
        }
    }
}

fn blend_pixel_native(dst: &mut [u8], src: &[u8], alpha_scale: f32, blend_mode: i32) {
    let destination: &mut [u8; 4] = dst.try_into().expect("RGBA destination pixel");
    let source: [u8; 4] = src.try_into().expect("RGBA source pixel");
    composite_native_rgba(destination, source, alpha_scale, blend_mode);
}

#[cfg(test)]
mod font_session_tests {
    use super::*;

    // Preserve real font metrics/outlines while hiding cmap, giving a valid
    // font with deliberately absent glyph coverage (including SJIS ASCII).
    fn missing_font(font: &FontArc) -> FontArc {
        let mut bytes = font.font_data().to_vec();
        let face = if bytes.starts_with(b"ttcf") {
            u32::from_be_bytes(bytes[12..16].try_into().unwrap()) as usize
        } else {
            0
        };
        let tables = u16::from_be_bytes(bytes[face + 4..face + 6].try_into().unwrap()) as usize;
        let cmap = (0..tables)
            .map(|index| face + 12 + index * 16)
            .find(|offset| &bytes[*offset..*offset + 4] == b"cmap")
            .expect("cmap table");
        let cmap_offset =
            u32::from_be_bytes(bytes[cmap + 8..cmap + 12].try_into().unwrap()) as usize;
        // Keep the sorted directory intact; zero cmap encoding subtables.
        bytes[cmap_offset + 2..cmap_offset + 4].copy_from_slice(&[0, 0]);
        ethornell_render::FontProvider::load_bytes(bytes, 0).expect("font without cmap")
    }

    #[test]
    fn the_session_font_is_the_requested_text_font() {
        // ETHORNELL_FONT names a substitute for the font a game asks for. The
        // window draws with the session font, so the request has to reach it.
        let system =
            ethornell_render::FontProvider::load_system_cjk_font().expect("system CJK font");
        assert_ne!(system.glyph_id('A').0, 0);
        let path = std::env::temp_dir().join(format!(
            "ethornell-requested-text-font-{}.ttf",
            std::process::id()
        ));
        std::fs::write(&path, missing_font(&system).font_data()).unwrap();
        let font = default_font(None, Some(path.clone().into_os_string()));
        std::fs::remove_file(&path).unwrap();
        assert_eq!(font.expect("requested text font").glyph_id('A').0, 0);
    }

    #[test]
    fn session_font_is_reused_and_diagnostics_are_nested_and_cleared() {
        let font = ethornell_render::FontProvider::load_system_cjk_font().expect("system CJK font");
        let before = SESSION_FONT.with(|font| font.borrow().is_some());
        {
            let _session = enter_font_session(&font);
            for _ in 0..100 {
                assert_eq!(
                    glyph_advance('A', 20.0, 100.0),
                    glyph_advance('A', 20.0, 100.0)
                );
            }
            assert!(take_raster_error().is_none());
            check_session_coverage("\u{10ffff}");
            record_raster_error("outer");
            with_font(&font, || {
                assert!(!take_missing_glyphs());
                assert!(take_raster_error().is_none());
                check_session_coverage("\u{10ffff}");
                record_raster_error("inner");
            });
            assert!(take_missing_glyphs());
            assert_eq!(take_raster_error().as_deref(), Some("outer"));
        }
        assert_eq!(SESSION_FONT.with(|font| font.borrow().is_some()), before);
        with_font(&font, || {
            assert!(!take_missing_glyphs());
            assert!(take_raster_error().is_none());
        });
    }

    #[test]
    fn cpu_bitmap_node_and_placed_glyph_paths_report_missing_coverage() {
        let font = ethornell_render::FontProvider::load_system_cjk_font().expect("system CJK font");
        let incomplete = missing_font(&font);
        with_font(&incomplete, || {
            direct_text_glyph_cell(u32::from(b'A'), 20, false);
            assert!(take_missing_glyphs());
            glyph_advance('A', 20.0, 100.0);
            assert!(take_missing_glyphs());
        });
        with_font(&font, || {
            let mut image = new_raster(40, 40).unwrap();
            rasterize_bitmap_text(&mut image, "\u{10ffff}", 0, 0, 20.0, 0.0, 100.0, [1.0; 4]);
            assert!(take_missing_glyphs());
            let node = crate::text::RuntimeTextNode {
                text: "\u{10ffff}".into(),
                enabled: true,
                owner_object: None,
                screen_attached: true,
                target_surface: None,
                x: 0.0,
                y: 0.0,
                size: 20.0,
                line_height: 20.0,
                formatted_layout: false,
                color: [1.0; 4],
                z: 0,
                ruby_spans: vec![],
                style_spans: vec![],
            };
            draw_text_node(
                &mut image,
                &node,
                crate::text::RuntimeFormattedTextLayout::plain(&node),
                TextNodeShadow::Default(None),
                None,
            );
            assert!(take_missing_glyphs());
            let glyph = crate::text_layout::PlacedGlyph {
                ch: '\u{10ffff}',
                index: 0,
                x: 0.0,
                y: 0.0,
            };
            rasterize_placed_glyphs(&mut image, &[glyph], 20.0, [1.0; 4], &|_| None);
            assert!(take_missing_glyphs());
        });
        with_font(&font, || {
            glyph_advance('A', 20.0, 100.0);
            assert!(
                !take_missing_glyphs(),
                "previous session must not poison this font"
            );
        });
    }

    #[test]
    fn cpu_allocation_and_text_budgets_fail_without_panicking() {
        assert!(raster_byte_len(u32::MAX, u32::MAX).is_err());
        assert!(new_raster(u32::MAX, u32::MAX).is_none());
        assert!(take_raster_error().is_some());
        assert!(direct_text_glyph_cell(65, u32::MAX, false).is_empty());
        assert!(take_raster_error().is_some());
        assert!(
            layout_direct_bitmap_text(b"A", i32::MAX, i32::MAX, u32::MAX, false, i32::MAX, 0, 1, 1)
                .0
                .is_empty()
        );
        assert!(take_raster_error().is_some());
    }
}

#[cfg(test)]
mod native_compositor_tests {
    use super::{composite_affine_mode5, composite_nearest, stable_framebuffer_hash};
    use crate::graph::RuntimeClipRect;
    use ethornell_image::DecodedImage;

    fn pixel(r: u8, g: u8, b: u8, a: u8) -> DecodedImage {
        DecodedImage {
            width: 1,
            height: 1,
            rgba: vec![r, g, b, a],
        }
    }

    #[test]
    fn integer_copy_matches_pixel_reference_with_crop_clip_and_source_alpha() {
        let source = DecodedImage {
            width: 7, height: 5,
            rgba: (0..35).flat_map(|index| [index * 7, 100, 30, [0, 1, 64, 128, 254, 255][index as usize % 6]]).collect(),
        };
        for ignore_alpha in [false, true] {
            for (x, y) in [(-1, -1), (0, 0), (1, 2)] {
                for clip in [None, Some(RuntimeClipRect { x: 2.0, y: 1.0, width: 3.0, height: 4.0 })] {
                    let mut actual = DecodedImage { width: 9, height: 7, rgba: [40, 70, 90, 64].repeat(63) };
                    let mut expected = actual.clone();
                    super::composite_nearest_with_format(&mut actual, &source,
                        x as f32, y as f32, 5.0, 3.0, 1.0, 1.0, 5.0, 3.0,
                        1.0, 128, clip, ignore_alpha);
                    for dy in 0..7i32 {
                        for dx in 0..9i32 {
                            if dx < x || dx >= x + 5 || dy < y || dy >= y + 3
                                || !super::point_in_clip(dx, dy, clip) { continue; }
                            let offset = (((dy - y + 1) * 7 + dx - x + 1) * 4) as usize;
                            let mut pixel: [u8; 4] = source.rgba[offset..offset + 4].try_into().unwrap();
                            if ignore_alpha { pixel[3] = 255; }
                            let target = ((dy * 9 + dx) * 4) as usize;
                            ethornell_core::composite_native_rgba(
                                (&mut expected.rgba[target..target + 4]).try_into().unwrap(), pixel, 1.0, 128);
                        }
                    }
                    assert_eq!(actual, expected, "offset=({x},{y}) clip={clip:?} ignore_alpha={ignore_alpha}");
                }
            }
        }
    }

    #[test]
    fn mode5_identity_affine_preserves_source_rows() {
        let source = DecodedImage {
            width: 2,
            height: 2,
            rgba: vec![
                255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255,
            ],
        };
        let mut destination = DecodedImage {
            width: 2,
            height: 2,
            rgba: vec![0; 2 * 2 * 4],
        };
        composite_affine_mode5(
            &mut destination,
            &source,
            [[0.0, 0.0], [0.0, 2.0], [2.0, 2.0], [2.0, 0.0]],
            0.0,
            0.0,
            2.0,
            2.0,
            1.0,
            1,
            None,
            false,
            false,
        );
        assert_eq!(destination.rgba, source.rgba);
    }

    #[test]
    fn additive_mode_accumulates_source_color() {
        let mut destination = pixel(10, 20, 30, 255);
        let source = pixel(100, 80, 60, 128);
        composite_nearest(
            &mut destination,
            &source,
            0.0,
            0.0,
            1.0,
            1.0,
            0.0,
            0.0,
            1.0,
            1.0,
            1.0,
            2,
            None,
        );
        assert_eq!(destination.rgba, [60, 60, 60, 255]);
    }

    #[test]
    fn subtractive_mode_removes_source_color() {
        let mut destination = pixel(100, 100, 100, 255);
        let source = pixel(80, 40, 20, 128);
        composite_nearest(
            &mut destination,
            &source,
            0.0,
            0.0,
            1.0,
            1.0,
            0.0,
            0.0,
            1.0,
            1.0,
            1.0,
            3,
            None,
        );
        assert_eq!(destination.rgba, [60, 80, 90, 255]);
    }

    #[test]
    fn compositor_respects_surface_clip() {
        let mut destination = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![0, 0, 0, 255, 0, 0, 0, 255],
        };
        let source = pixel(255, 255, 255, 255);
        composite_nearest(
            &mut destination,
            &source,
            0.0,
            0.0,
            2.0,
            1.0,
            0.0,
            0.0,
            1.0,
            1.0,
            1.0,
            1,
            Some(RuntimeClipRect {
                x: 1.0,
                y: 0.0,
                width: 1.0,
                height: 1.0,
            }),
        );
        assert_eq!(destination.rgba, [0, 0, 0, 255, 255, 255, 255, 255]);
    }

    #[test]
    fn framebuffer_hash_covers_geometry_and_pixels() {
        let black = pixel(0, 0, 0, 255);
        let mut white = pixel(0, 0, 0, 255);
        white.rgba[0] = 255;
        let wider = DecodedImage {
            width: 2,
            height: 1,
            rgba: vec![0, 0, 0, 255, 0, 0, 0, 255],
        };

        assert_eq!(
            stable_framebuffer_hash(&black),
            stable_framebuffer_hash(&black)
        );
        assert_ne!(
            stable_framebuffer_hash(&black),
            stable_framebuffer_hash(&white)
        );
        assert_ne!(
            stable_framebuffer_hash(&black),
            stable_framebuffer_hash(&wider)
        );
    }
}

#[cfg(test)]
mod styled_text_snapshot_tests {
    use super::{TextNodeShadow, TextShadow, draw_text_node};
    use crate::text::{RuntimeFormattedTextLayout, RuntimeTextNode};
    use crate::text_anim::{RuntimeTextStyle, RuntimeTextStyleSpan};
    use ethornell_image::DecodedImage;

    #[test]
    fn message_glow_draws_a_light_halo_around_dark_text() {
        let font = super::load_default_font().expect("test requires system CJK font");
        let _font_session = super::enter_font_session(&font);
        let node = RuntimeTextNode {
            text: "AB".into(),
            enabled: true,
            owner_object: None,
            screen_attached: true,
            target_surface: None,
            x: 8.0,
            y: 8.0,
            size: 32.0,
            line_height: 40.0,
            formatted_layout: false,
            color: [0.0, 0.0, 0.0, 1.0],
            z: 0,
            ruby_spans: Vec::new(),
            style_spans: Vec::new(),
        };
        let layout = RuntimeFormattedTextLayout::plain(&node);
        let mut image = DecodedImage {
            width: 128,
            height: 64,
            rgba: [128, 128, 128, 255].repeat(128 * 64),
        };
        let glow = TextShadow {
            mode: 2,
            dx: 1,
            dy: 1,
            rgb: 0xffffff,
            concentration: 192,
        };

        draw_text_node(
            &mut image,
            &node,
            layout,
            TextNodeShadow::Message(Some(glow)),
            None,
        );

        let pixels = || image.rgba.chunks_exact(4);
        assert!(pixels().any(|pixel| pixel[0] < 40), "dark glyphs");
        assert!(pixels().any(|pixel| pixel[0] > 200), "light halo");
    }

    #[test]
    fn cpu_framebuffer_applies_the_same_per_character_color_span() {
        let font = super::load_default_font().expect("test requires system CJK font");
        let _font_session = super::enter_font_session(&font);
        let node = RuntimeTextNode {
            text: "AB".into(),
            enabled: true,
            owner_object: None,
            screen_attached: true,
            target_surface: None,
            x: 8.0,
            y: 8.0,
            size: 32.0,
            line_height: 40.0,
            formatted_layout: false,
            color: [1.0; 4],
            z: 0,
            ruby_spans: Vec::new(),
            style_spans: vec![RuntimeTextStyleSpan {
                start_char: 1,
                end_char: 2,
                style: RuntimeTextStyle {
                    packed_rgb: Some(0xff0000),
                    bold: true,
                    italic: true,
                },
            }],
        };
        let layout = RuntimeFormattedTextLayout::plain(&node);
        let mut image = DecodedImage {
            width: 128,
            height: 64,
            rgba: vec![0; 128 * 64 * 4],
        };
        draw_text_node(
            &mut image,
            &node,
            layout,
            TextNodeShadow::Default(None),
            None,
        );
        assert!(
            image
                .rgba
                .chunks_exact(4)
                .any(|pixel| pixel[0] > 200 && pixel[1] > 200 && pixel[2] > 200 && pixel[3] > 0)
        );
        assert!(
            image
                .rgba
                .chunks_exact(4)
                .any(|pixel| pixel[0] > 200 && pixel[1] < 20 && pixel[2] < 20 && pixel[3] > 0)
        );
    }
}

#[cfg(test)]
mod glyph_record_tests {
    use super::{TextShadow, rasterize_placed_glyph_records};
    use crate::text_layout::PlacedGlyph;
    use ethornell_image::DecodedImage;

    fn render(shadow: Option<TextShadow>) -> DecodedImage {
        let font = super::load_default_font().expect("test requires system CJK font");
        let _session = super::enter_font_session(&font);
        let mut image = DecodedImage {
            width: 40,
            height: 40,
            rgba: vec![0; 40 * 40 * 4],
        };
        let glyph = PlacedGlyph {
            ch: '■',
            index: 0,
            x: 5.0,
            y: 5.0,
        };
        rasterize_placed_glyph_records(&mut image, 2, &[glyph], 20.0, [1.0; 4], &|_| None, shadow);
        image
    }

    fn ink(image: &DecodedImage) -> Vec<(u32, u32, [u8; 4])> {
        (0..image.height)
            .flat_map(|y| (0..image.width).map(move |x| (x, y)))
            .map(|(x, y)| {
                let at = ((y * image.width + x) * 4) as usize;
                let pixel: [u8; 4] = image.rgba[at..at + 4].try_into().unwrap();
                (x, y, pixel)
            })
            .filter(|(_, _, pixel)| pixel[3] != 0)
            .collect()
    }

    #[test]
    fn drop_shadow_and_glow_extend_the_glyph_in_the_shadow_colour() {
        let plain = ink(&render(None));
        if plain.is_empty() {
            return; // no system font
        }
        let max_x = plain.iter().map(|p| p.0).max().unwrap();
        let max_y = plain.iter().map(|p| p.1).max().unwrap();
        let blue = |dx, dy, mode| {
            ink(&render(Some(TextShadow {
                mode,
                dx,
                dy,
                rgb: 0x0000FF,
                concentration: 128,
            })))
        };
        // Mode 1: blue at half strength right of and below the glyph.
        let drop = blue(2, 2, 1);
        let edge = drop
            .iter()
            .filter(|p| p.0 > max_x && p.1 <= max_y)
            .max_by_key(|p| p.2[3])
            .expect("shadow edge");
        assert_eq!(&edge.2[..3], &[0, 0, 255]);
        assert!((120..=135).contains(&edge.2[3]), "{:?}", edge.2);
        // Mode 2: the glyph moves by (dx, dy) and the glow surrounds it.
        let glow = blue(2, 2, 2);
        assert!(glow.iter().any(|p| p.0 == max_x + 4 && p.2[2] == 255));
        let min_x = plain.iter().map(|p| p.0).min().unwrap();
        // The glow reaches dx left of the moved glyph, i.e. the old edge.
        assert_eq!(glow.iter().map(|p| p.0).min().unwrap(), min_x);
    }
}

#[cfg(test)]
mod direct_bitmap_text_tests {
    use super::layout_direct_bitmap_text;

    fn sjis(text: &str) -> Vec<u8> {
        encoding_rs::SHIFT_JIS.encode(text).0.into_owned()
    }

    #[test]
    fn styled_text_uses_width_color_native_wrapping_and_plain_strings() {
        let font=super::load_default_font().expect("test requires session font");let _session=super::enter_font_session(&font);
        let (glyphs,advance)=super::layout_styled_bitmap_text(b"AA\nA",[0,0],20,200,false,false,0x123456,false,0,[200,100]);
        assert_eq!(advance,40);assert_eq!((glyphs[1].x,glyphs[2].y),(20,20));
        assert!(glyphs.iter().flat_map(|g|g.cell.rgba.as_chunks::<4>().0).any(|p|p[3]>0&&p[..3]==[0x12,0x34,0x56]));
        let (wrapped,lines)=super::layout_styled_bitmap_text(b"AAA",[0,0],20,200,false,false,0x123456,true,50,[70,100]);
        assert_eq!(lines,3);assert_eq!(wrapped.iter().map(|g|g.y).collect::<Vec<_>>(),[0,30,60]);
        let (_,literal)=super::layout_styled_bitmap_text(b"<b>",[0,0],20,200,false,false,0xFFFFFF,false,0,[200,100]);assert_eq!(literal,60);
    }

    #[test]
    fn cells_are_fixed_width_and_stop_at_the_first_invisible_glyph() {
        let (glyphs, advance) =
            layout_direct_bitmap_text(&sjis("あa"), 0, 0, 12, false, 1, 0xFFFFFF, 100, 20);
        assert_eq!(glyphs.len(), 2);
        assert_eq!((glyphs[0].x, glyphs[0].cell.width), (0, 12));
        assert_eq!((glyphs[1].x, glyphs[1].cell.width), (13, 6));
        assert_eq!(advance, 13 + 7);
        // The third glyph starts at x=30, past a 30-pixel bitmap: stop.
        let (glyphs, advance) =
            layout_direct_bitmap_text(&sjis("あいう"), 0, 0, 12, false, 3, 0xFFFFFF, 30, 20);
        assert_eq!(glyphs.len(), 2);
        assert_eq!(advance, 30);
    }

    #[test]
    fn control_codes_wrap_and_set_the_line_pitch() {
        let (glyphs, _) =
            layout_direct_bitmap_text(&sjis("\u{4}あいう"), 0, 0, 12, false, 0, 0, 30, 60);
        assert_eq!((glyphs[2].x, glyphs[2].y), (0, 12));
        let mut bytes = vec![3, 150];
        bytes.extend(sjis("あ\nあ"));
        let (glyphs, _) = layout_direct_bitmap_text(&bytes, 5, 0, 12, false, 0, 0, 40, 80);
        assert_eq!((glyphs[1].x, glyphs[1].y), (5, 18));
    }

    #[test]
    fn angle_brackets_are_glyphs_not_markup() {
        let (glyphs, advance) =
            layout_direct_bitmap_text(b"<b>x</b>", 0, 0, 12, false, 0, 0, 80, 20);
        assert_eq!(glyphs.len(), 8);
        assert_eq!(advance, 48);
    }
}
