use super::*;
use crate::snapshot::TextNodeShadow;
use ethornell_render::TextGlow;

const MAX_FRAME_ITEMS: usize = 65_536;
const MAX_FRAME_TEXTURE_BYTES: usize = 256 * 1024 * 1024;
const MAX_FRAME_TEXT_BYTES: usize = 8 * 1024 * 1024;

fn check_frame_budget(count: usize, texture_bytes: usize, text_bytes: usize) -> Result<()> {
    if count > MAX_FRAME_ITEMS
        || texture_bytes > MAX_FRAME_TEXTURE_BYTES
        || text_bytes > MAX_FRAME_TEXT_BYTES
    {
        return Err(EthornellError::Other(
            "frame presentation budget exceeded".into(),
        ));
    }
    Ok(())
}

fn image_bytes(image: &DecodedImage) -> Result<usize> {
    let bytes = (image.width as usize)
        .checked_mul(image.height as usize)
        .and_then(|n| n.checked_mul(4));
    match bytes {
        Some(bytes) if image.width > 0 && image.height > 0 && bytes == image.rgba.len() => {
            Ok(bytes)
        }
        _ => Err(EthornellError::Other(
            "invalid frame RGBA image dimensions".into(),
        )),
    }
}

fn validate_text(text: &str, size: f32) -> Result<()> {
    if !size.is_finite() || !(0.0..=1024.0).contains(&size) || text.len() > 256 * 1024 {
        return Err(EthornellError::Other(
            "frame text raster budget exceeded".into(),
        ));
    }
    Ok(())
}

/// One compositor for the window and host-driven frontends. Texture revisions,
/// flattened display priority, bitmap alpha/blending, clipped text and ruby all
/// originate in the same native display tree.
pub(crate) fn prepare_frame(
    renderer: &mut Renderer<'_>,
    api: &RuntimeTraceApi,
    textures: &mut BTreeMap<String, CachedGraphTexture>,
) -> Result<Vec<RenderCommand>> {
    // Validate source budgets before changing caches or submitting any work.
    // Renderer owns backend/device-specific texture-size checks.
    check_frame_budget(
        api.graph_layers.len().saturating_add(api.text_nodes.len()),
        0,
        0,
    )?;
    let mut items = api.graph_output_draw_items();
    items.extend(api.directshow_movie_draw_item());
    let used = items
        .iter()
        .map(|i| i.key.as_str())
        .collect::<BTreeSet<_>>();
    let mut texture_bytes = 0usize;
    for key in &used {
        if let Some(image) = api.graph_images.get(*key) {
            texture_bytes = texture_bytes.saturating_add(image_bytes(image)?);
        }
    }
    let mut text_bytes = 0usize;
    let mut command_count = items.len().saturating_add(1);
    for node in api
        .text_nodes
        .values()
        .filter(|node| api.should_draw_text_node(node))
    {
        validate_text(&node.text, node.size)?;
        if node.style_spans.len() > MAX_FRAME_ITEMS || node.ruby_spans.len() > MAX_FRAME_ITEMS {
            return Err(EthornellError::Other(
                "frame text span budget exceeded".into(),
            ));
        }
        text_bytes = text_bytes.saturating_add(node.text.len());
        command_count = command_count
            .saturating_add(2)
            .saturating_add(node.ruby_spans.len().saturating_mul(2));
        for ruby in &node.ruby_spans {
            text_bytes = text_bytes.saturating_add(ruby.reading.len());
            if ruby.reading.len() > 256 * 1024 {
                return Err(EthornellError::Other(
                    "frame ruby text budget exceeded".into(),
                ));
            }
        }
    }
    check_frame_budget(command_count, texture_bytes, text_bytes.saturating_mul(2))?;
    let obsolete = textures
        .keys()
        .filter(|k| !used.contains(k.as_str()))
        .cloned()
        .collect::<Vec<_>>();
    for key in obsolete {
        if let Some(cached) = textures.remove(&key) {
            renderer.remove_texture(cached.handle.id);
        }
    }
    for key in used {
        let Some(image) = api.graph_images.get(key) else {
            continue;
        };
        let revision = api.graph_image_revision(key);
        match textures.get_mut(key) {
            Some(cached) if cached.revision != revision => {
                cached.handle = renderer.update_rgba(&cached.handle, image)?;
                cached.revision = revision;
            }
            Some(_) => {}
            None => {
                textures.insert(
                    key.to_owned(),
                    CachedGraphTexture {
                        handle: renderer.insert_rgba(image)?,
                        revision,
                    },
                );
            }
        }
    }
    compose_commands(api, items, textures)
}

fn compose_commands(
    api: &RuntimeTraceApi,
    items: Vec<crate::graph::RuntimeGraphDrawItem>,
    textures: &BTreeMap<String, CachedGraphTexture>,
) -> Result<Vec<RenderCommand>> {
    let mut commands = vec![RenderCommand::Clear {
        color: [0.0, 0.0, 0.0, 1.0],
    }];
    let mut flattened_z = i32::MIN;
    for item in items {
        let Some(cached) = textures.get(&item.key) else {
            continue;
        };
        let texture = &cached.handle;
        // Preserve the recovered BackF exception for projected Sprite 5/6:
        // stable renderer z sorting must not undo native display-tree order.
        flattened_z = item.z.max(flattened_z);
        commands.push(RenderCommand::DrawTexture {
            texture: texture.id,
            x: item.x,
            y: item.y,
            width: item.width,
            height: item.height,
            src_x: item.src_x / texture.width as f32,
            src_y: item.src_y / texture.height as f32,
            src_width: item.src_width / texture.width as f32,
            src_height: item.src_height / texture.height as f32,
            opacity: item.opacity,
            ignore_source_alpha: item.ignore_source_alpha,
            blend_mode: item.blend_mode,
            rotation_degrees: item.rotation_degrees,
            destination_quad: item.destination_quad,
            linear_sampling: item.linear_sampling,
            clip: item.clip.map(|c| [c.x, c.y, c.width, c.height]),
            z: graph_render_z(flattened_z),
        });
    }
    for node in api
        .text_nodes
        .values()
        .filter(|n| api.should_draw_text_node(n))
    {
        // compose_commands is also exercised without a GPU in regression tests.
        validate_text(&node.text, node.size)?;
        let parent = node
            .target_surface
            .filter(|s| api.graph_surfaces.contains_key(s))
            .map(|s| {
                let (x, y) = api.surface_world_position(s);
                (x, y, api.surface_display_opacity(s))
            })
            .unwrap_or((0.0, 0.0, 1.0));
        let layout = api.formatted_text_layout(node);
        // Laid-out (window) text starts a line every `layout.line_height`,
        // as the CPU compositor and the ruby above it do.
        let node_line_height = node.formatted_layout.then_some(layout.line_height);
        let x = parent.0 + node.x;
        let cursor_y = parent.1 + node.y;
        let y = cursor_y + layout.body_y_offset;
        let clip = node
            .target_surface
            .and_then(|s| api.surface_chain_clip(s))
            .map(|c| [c.x, c.y, c.width, c.height]);
        let size = node.size.max(12.0);
        let mut color = node.color;
        color[3] *= parent.2;
        let z = text_render_z(node.z);
        let styles = api
            .text_node_style_spans(node)
            .iter()
            .map(|s| TextStyleSpan {
                start_char: s.start_char,
                end_char: s.end_char,
                packed_rgb: s.style.packed_rgb,
                bold: s.style.bold,
                italic: s.style.italic,
            })
            .collect::<Vec<_>>();
        let shadow_styles = styles
            .iter()
            .map(|s| TextStyleSpan {
                packed_rgb: None,
                ..*s
            })
            .collect::<Vec<_>>();
        let node_shadow = api.text_node_shadow(node);
        let mut glow = None;
        match node_shadow {
            TextNodeShadow::Message(Some(effect)) if effect.mode == 2 => {
                glow = Some(TextGlow {
                    radius_x: effect.dx.max(0) as u32,
                    radius_y: effect.dy.max(0) as u32,
                    rgb: [
                        (effect.rgb >> 16) as u8,
                        (effect.rgb >> 8) as u8,
                        effect.rgb as u8,
                    ],
                    concentration: effect.concentration.max(0) as u32,
                });
            }
            TextNodeShadow::Message(Some(effect)) => {
                commands.push(RenderCommand::DrawText {
                    text: node.text.clone(),
                    x: x + effect.dx as f32,
                    y: y + effect.dy as f32,
                    color: [
                        ((effect.rgb >> 16) & 0xff) as f32 / 255.0,
                        ((effect.rgb >> 8) & 0xff) as f32 / 255.0,
                        (effect.rgb & 0xff) as f32 / 255.0,
                        color[3] * effect.concentration as f32 / 256.0,
                    ],
                    size,
                    line_height: node_line_height,
                    styles: shadow_styles,
                    glow: None,
                    clip,
                    z,
                });
            }
            TextNodeShadow::Message(None) => {}
            TextNodeShadow::Default(_) => {
                if let Some((dx, dy, alpha)) = api.graph_defaults.shadow_for_height(size) {
                    commands.push(RenderCommand::DrawText {
                        text: node.text.clone(),
                        x: x + dx as f32,
                        y: y + dy as f32,
                        color: [0.0, 0.0, 0.0, color[3] * alpha],
                        size,
                        line_height: node_line_height,
                        styles: shadow_styles,
                        glow: None,
                        clip,
                        z,
                    });
                }
            }
        }
        commands.push(RenderCommand::DrawText {
            text: node.text.clone(),
            x,
            y,
            color,
            size,
            line_height: node_line_height,
            styles,
            glow,
            clip,
            z,
        });
        let mut positioned = node.clone();
        positioned.x = x;
        positioned.y = cursor_y;
        for (ruby, x, y, size) in text::ruby_draw_runs(&positioned, layout) {
            validate_text(&ruby, size)?;
            if let TextNodeShadow::Default(_) = node_shadow
                && let Some((dx, dy, alpha)) = api.graph_defaults.shadow_for_height(size)
            {
                commands.push(RenderCommand::DrawText {
                    text: ruby.clone(),
                    x: x + dx as f32,
                    y: y + dy as f32,
                    color: [0.0, 0.0, 0.0, color[3] * alpha],
                    size,
                    line_height: None,
                    styles: Vec::new(),
                    glow: None,
                    clip,
                    z,
                });
            }
            commands.push(RenderCommand::DrawText {
                text: ruby,
                x,
                y,
                color,
                size,
                line_height: None,
                styles: Vec::new(),
                glow: None,
                clip,
                z,
            });
        }
    }
    for (text, x, y, size) in api.user_modeless_dialog_text_lines() {
        validate_text(&text, size)?;
        check_frame_budget(commands.len().saturating_add(2), 0, 0)?;
        commands.push(RenderCommand::DrawText {
            text: text.clone(),
            x: x + 2.0,
            y: y + 2.0,
            color: [0.0, 0.0, 0.0, 0.9],
            size,
            line_height: None,
            styles: Vec::new(),
            glow: None,
            clip: None,
            z: 29_999,
        });
        commands.push(RenderCommand::DrawText {
            text,
            x,
            y,
            color: [0.95, 0.95, 0.95, 1.0],
            size,
            line_height: None,
            styles: Vec::new(),
            glow: None,
            clip: None,
            z: 30_000,
        });
    }
    // Count generated shadows/ruby as well, before Renderer clones/rasterizes.
    let mut text_bytes = 0usize;
    for command in &commands {
        if let RenderCommand::DrawText { text, size, .. } = command {
            validate_text(text, *size)?;
            text_bytes = text_bytes.saturating_add(text.len());
        }
    }
    check_frame_budget(commands.len(), 0, text_bytes)?;
    Ok(commands)
}

/// Renderer depth of a graph object at native priority `z`; depths 0 and 1
/// are below every graph object.
fn graph_render_z(z: i32) -> i32 {
    2i32.saturating_add(z)
}

/// Renderer depth of a text node at priority `z`: the depth of the images at
/// that priority. Text is submitted after the images and the renderer sorts
/// stably, so a window's text lands on its own background and under objects
/// of higher priority, as in the target, where a window composes its text
/// into its own surface. Shadows and ruby keep their order by submission.
fn text_render_z(z: i32) -> i32 {
    graph_render_z(z)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::graph::{RuntimeClipRect, RuntimeGraphDrawItem};

    #[test]
    fn window_text_is_drawn_over_its_window_and_under_higher_priorities() {
        // A message window at priority 117: its background is an image at
        // that priority and its text a node at the same priority. The text
        // was drawn at depth 117, under the background at 119; through a
        // half-transparent background black text vanished.
        let window_priority = 117;
        let background = graph_render_z(window_priority);
        let text = text_render_z(window_priority);
        // Equal depth: the text, submitted later, is drawn after it.
        assert_eq!(text, background);
        assert!(text > graph_render_z(window_priority - 1));
        assert!(text < graph_render_z(window_priority + 1));
    }

    fn api() -> RuntimeTraceApi {
        RuntimeTraceApi::new(ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap())
    }

    fn item(z: i32) -> RuntimeGraphDrawItem {
        RuntimeGraphDrawItem {
            owner_object: None,
            key: "test".into(),
            x: 10.0,
            y: 20.0,
            width: 12.0,
            height: 14.0,
            src_x: 2.0,
            src_y: 3.0,
            src_width: 6.0,
            src_height: 7.0,
            opacity: 0.5,
            ignore_source_alpha: true,
            rotation_degrees: 30.0,
            destination_quad: Some([[1.0, 2.0], [1.0, 4.0], [3.0, 4.0], [3.0, 2.0]]),
            linear_sampling: false,
            clip: Some(RuntimeClipRect {
                x: 1.0,
                y: 2.0,
                width: 3.0,
                height: 4.0,
            }),
            z,
            blend_mode: 3,
            order_serial: 0,
            hit_id: 0,
        }
    }

    #[test]
    fn shared_commands_preserve_native_order_sampling_clip_and_blend() {
        let textures = BTreeMap::from([(
            "test".into(),
            CachedGraphTexture {
                handle: TextureHandle {
                    id: 7,
                    width: 20,
                    height: 30,
                },
                revision: 1,
            },
        )]);
        let commands =
            compose_commands(&api(), vec![item(100), item(40), item(i32::MAX)], &textures).unwrap();
        let mut zs = Vec::new();
        for command in commands.into_iter().skip(1) {
            let RenderCommand::DrawTexture {
                texture,
                x,
                y,
                src_x,
                src_y,
                src_width,
                src_height,
                opacity,
                ignore_source_alpha,
                blend_mode,
                rotation_degrees,
                destination_quad,
                linear_sampling,
                clip,
                z,
                ..
            } = command
            else {
                panic!("texture command");
            };
            assert_eq!((texture, x, y), (7, 10.0, 20.0));
            assert_eq!(
                (src_x, src_y, src_width, src_height),
                (0.1, 0.1, 0.3, 7.0 / 30.0)
            );
            assert_eq!(
                (opacity, ignore_source_alpha, blend_mode, rotation_degrees),
                (0.5, true, 3, 30.0)
            );
            assert_eq!(destination_quad, item(0).destination_quad);
            assert!(!linear_sampling);
            assert_eq!(clip, Some([1.0, 2.0, 3.0, 4.0]));
            zs.push(z);
        }
        assert_eq!(zs, vec![102, 102, i32::MAX]);
    }

    #[test]
    fn malformed_images_and_frame_budgets_are_errors() {
        assert!(
            image_bytes(&DecodedImage {
                width: u32::MAX,
                height: u32::MAX,
                rgba: vec![]
            })
            .is_err()
        );
        assert!(
            image_bytes(&DecodedImage {
                width: 1,
                height: 1,
                rgba: vec![0; 3]
            })
            .is_err()
        );
        assert!(check_frame_budget(MAX_FRAME_ITEMS + 1, 0, 0).is_err());
        assert!(check_frame_budget(0, MAX_FRAME_TEXTURE_BYTES + 1, 0).is_err());
        assert!(check_frame_budget(0, 0, MAX_FRAME_TEXT_BYTES + 1).is_err());
        assert!(validate_text("test", f32::NAN).is_err());
        assert!(validate_text("test", f32::INFINITY).is_err());
    }

    #[test]
    fn window_text_lines_are_the_window_pitch_apart() {
        // Wonderful Everyday's message window: font 28 and Graph91:89
        // spacing 25 make a line every 35 pixels (sub_42C4C0); the GPU drew
        // lines at the font's own height (about 28 pixels).
        let mut api = api();
        let window = api.alloc_window_surface(1280, 300).unwrap();
        api.set_graph_object_draw_enabled(window, true);
        api.graph_surfaces.get_mut(&window).unwrap().line_spacing_percent = 25;
        api.surface_text_states.entry(window).or_default().font_size = 28;
        api.start_native_message("ab\ncd".into(), Some(window));
        api.reveal_text_on_input();
        let commands = compose_commands(&api, Vec::new(), &BTreeMap::new()).unwrap();
        let pitches = commands
            .iter()
            .filter_map(|command| match command {
                RenderCommand::DrawText {
                    text, line_height, ..
                } if text == "ab\ncd" => Some(*line_height),
                _ => None,
            })
            .collect::<Vec<_>>();
        assert!(!pitches.is_empty());
        assert!(pitches.iter().all(|pitch| *pitch == Some(35.0)));
    }

    #[test]
    fn shared_text_keeps_surface_position_opacity_clip_shadow_and_ruby() {
        let mut api = api();
        let mut surface = crate::graph::RuntimeSurface::bitmap(1, 200.0, 100.0);
        surface.display_attached = true;
        surface.x = 10.0;
        surface.y = 20.0;
        surface.opacity = 0.5;
        api.graph_surfaces.insert(1, surface);
        api.graph_defaults.shadow_enabled = true;
        api.graph_defaults.configure_shadow(10, 10, 128);
        api.graph_defaults.text_style.ruby_height = 8;
        let node = crate::text::RuntimeTextNode {
            text: "日本".into(),
            enabled: true,
            owner_object: None,
            screen_attached: false,
            target_surface: Some(1),
            x: 4.0,
            y: 6.0,
            size: 20.0,
            line_height: 24.0,
            formatted_layout: true,
            color: [1.0, 0.8, 0.6, 0.8],
            z: 400,
            ruby_spans: vec![crate::text_anim::RuntimeRubySpan {
                start_char: 0,
                end_char: 2,
                reading: "にほん".into(),
            }],
            style_spans: Vec::new(),
        };
        let layout = api.formatted_text_layout(&node);
        api.text_nodes.insert(2, node.clone());
        let commands = compose_commands(&api, Vec::new(), &BTreeMap::new()).unwrap();
        assert_eq!(commands.len(), 5);
        let RenderCommand::DrawText {
            text,
            x,
            y,
            color,
            clip,
            z,
            ..
        } = &commands[2]
        else {
            panic!("body command");
        };
        assert_eq!(text, "日本");
        assert_eq!((*x, *y), (14.0, 26.0 + layout.body_y_offset));
        assert_eq!(color[3], 0.4);
        assert_eq!(*clip, Some([10.0, 20.0, 200.0, 100.0]));
        // Text, its shadow and its ruby share the depth of the images at
        // its priority and keep their order by submission.
        assert_eq!(*z, text_render_z(400));
        assert!(matches!(
            &commands[1],
            RenderCommand::DrawText { z: 402, .. }
        ));
        assert!(matches!(
            &commands[3],
            RenderCommand::DrawText { z: 402, .. }
        ));
        let RenderCommand::DrawText {
            text,
            color,
            clip,
            z,
            size,
            ..
        } = &commands[4]
        else {
            panic!("ruby command");
        };
        assert_eq!(text, "にほん");
        assert_eq!(color[3], 0.4);
        assert_eq!(*clip, Some([10.0, 20.0, 200.0, 100.0]));
        assert_eq!((*z, *size), (402, 8.0));
    }
}
