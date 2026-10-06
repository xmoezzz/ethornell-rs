use super::*;
use winit::application::ApplicationHandler;
use winit::data_transfer::TypeHint;
use winit::dpi::{PhysicalPosition, PhysicalSize};
use winit::event::{
    ButtonSource, ElementState, KeyEvent, MouseButton, MouseScrollDelta, WindowEvent,
};
use winit::event_loop::DndAction;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::keyboard::PhysicalKey;
use winit::monitor::Fullscreen;
use winit::window::{Window, WindowAttributes, WindowId};

struct Graphics {
    renderer: Renderer<'static>,
    window: Arc<dyn Window>,
    texture: Option<TextureHandle>,
    graph_textures: BTreeMap<String, CachedGraphTexture>,
}

struct WindowApp {
    title: String,
    image: Option<DecodedImage>,
    text: Option<String>,
    image_size: Option<(u32, u32)>,
    runtime: Option<RuntimeEngine>,
    graphics: Option<Graphics>,
    audio: AudioSystem,
    cursor_surface_pos: Option<(f32, f32)>,
    pending_input_events: VecDeque<RuntimeInputEvent>,
    pending_drop: Option<winit::event_loop::AsyncRequestSerial>,
    frame_pacing: RuntimeFramePacing,
    next_frame: Instant,
    host_clock_origin: Instant,
    last_engine_clock_ms: u64,
    last_audio_clock_ms: u64,
    frame_index: usize,
    input_script: Option<HeadlessInputScript>,
    failure: Arc<Mutex<Option<String>>>,
}

impl ApplicationHandler for WindowApp {
    fn can_create_surfaces(&mut self, target: &dyn ActiveEventLoop) {
        if self.graphics.is_some() {
            return;
        }
        match self.create_graphics(target) {
            Ok(graphics) => self.graphics = Some(graphics),
            Err(error) => {
                *self.failure.lock().unwrap() = Some(error.to_string());
                target.exit();
            }
        }
    }

    fn destroy_surfaces(&mut self, _target: &dyn ActiveEventLoop) {
        self.graphics = None;
    }

    fn window_event(&mut self, target: &dyn ActiveEventLoop, id: WindowId, event: WindowEvent) {
        let Self {
            graphics,
            runtime,
            text,
            image_size,
            audio,
            cursor_surface_pos,
            pending_input_events,
            pending_drop,
            frame_pacing,
            host_clock_origin,
            last_engine_clock_ms,
            last_audio_clock_ms,
            frame_index,
            input_script,
            failure,
            ..
        } = self;
        let Some(Graphics {
            renderer,
            window,
            texture,
            graph_textures,
        }) = graphics.as_mut()
        else {
            return;
        };
        if id != window.id() {
            return;
        }
        match event {
            WindowEvent::CloseRequested => {
                let allow_native_close = runtime
                    .as_mut()
                    .is_none_or(|runtime| runtime.api.handle_host_window_close("window"));
                if allow_native_close {
                    target.exit();
                }
            }
            WindowEvent::SurfaceResized(size) => {
                renderer.resize(size.width, size.height);
                if let Some(runtime) = runtime.as_mut() {
                    runtime.api.window_surface_width = size.width.min(i32::MAX as u32) as i32;
                    runtime.api.window_surface_height = size.height.min(i32::MAX as u32) as i32;
                }
            }
            WindowEvent::Focused(focused) => {
                if let Some(runtime) = runtime.as_mut() {
                    runtime.api.window_focused = focused;
                    if !focused && runtime.api.input_requires_focus {
                        pending_input_events.clear();
                        runtime.api.pending_input_state = None;
                        runtime.api.pending_input_descriptor = None;
                        runtime.api.pending_click = None;
                        runtime.api.pending_object_state = None;
                        runtime.api.input_down_descriptors.clear();
                        runtime.api.input_down_reported.clear();
                    }
                }
            }
            WindowEvent::DragEntered { id, .. } => {
                if runtime
                    .as_ref()
                    .is_some_and(|runtime| runtime.api.native_system.drag_drop_enabled)
                    && target
                        .data_transfer(id)
                        .is_ok_and(|data| data.has_type(&TypeHint::UriList))
                {
                    let _ = target.set_valid_dnd_actions(id, &[DndAction::Copy]);
                }
            }
            WindowEvent::DragDropped { id, .. } => {
                if runtime
                    .as_ref()
                    .is_some_and(|runtime| runtime.api.native_system.drag_drop_enabled)
                {
                    *pending_drop = target.fetch_data_transfer(id, &TypeHint::UriList).ok();
                }
            }
            WindowEvent::DataTransferReceived { serial, value, .. } => {
                if *pending_drop == Some(serial) {
                    *pending_drop = None;
                    if let Some(runtime) = runtime.as_mut()
                        && runtime.api.native_system.drag_drop_enabled
                        && let Ok(paths) = value.try_as_file_paths()
                    {
                        runtime.api.dropped_files = paths
                            .iter()
                            .map(|path| path.to_string_lossy().into_owned())
                            .collect();
                    }
                }
            }
            WindowEvent::PointerMoved { position, .. } => {
                *cursor_surface_pos = Some((position.x as f32, position.y as f32));
                let cursor_game_pos =
                    renderer.surface_to_game_point(position.x as f32, position.y as f32);
                if runtime.is_some()
                    && let Some((x, y)) = cursor_game_pos
                {
                    queue_runtime_input_event(
                        pending_input_events,
                        RuntimeInputEvent::MouseMove { x, y },
                    );
                }
            }
            WindowEvent::KeyboardInput {
                event:
                    KeyEvent {
                        physical_key: PhysicalKey::Code(code),
                        state: ElementState::Pressed,
                        text,
                        ..
                    },
                ..
            } => {
                let modeless_consumed = runtime
                    .as_ref()
                    .is_some_and(|runtime| runtime.api.has_visible_user_modeless_dialog());
                let edit_consumed = runtime.as_mut().is_some_and(|runtime| {
                    runtime.api.handle_native_edit_key(code)
                        || text
                            .as_deref()
                            .is_some_and(|text| runtime.api.append_native_edit_text(text))
                });
                if !edit_consumed
                    && !modeless_consumed
                    && let Some(descriptor) = input_descriptor_for_keycode(code)
                {
                    if let Some(runtime) = runtime.as_mut() {
                        if runtime.api.native_system.fullscreen_hotkeys_enabled
                            && runtime
                                .api
                                .native_system
                                .fullscreen_hotkeys
                                .contains(&descriptor)
                        {
                            let fullscreen = runtime.api.window_mode == 0;
                            runtime.api.window_mode = i32::from(fullscreen);
                            runtime.api.pending_fullscreen = Some(fullscreen);
                        }
                        queue_runtime_input_event(
                            pending_input_events,
                            RuntimeInputEvent::KeyPress { descriptor },
                        );
                    }
                    tracing::info!(descriptor, "keyboard advance");
                }
            }
            WindowEvent::KeyboardInput {
                event:
                    KeyEvent {
                        physical_key: PhysicalKey::Code(code),
                        state: ElementState::Released,
                        ..
                    },
                ..
            } => {
                if let Some(descriptor) = input_descriptor_for_keycode(code)
                    && let Some(runtime) = runtime.as_mut()
                    && !runtime.api.has_visible_user_modeless_dialog()
                {
                    queue_runtime_input_event(
                        pending_input_events,
                        RuntimeInputEvent::KeyRelease { descriptor },
                    );
                }
            }
            WindowEvent::MouseWheel { delta, .. } => {
                let delta_y = match delta {
                    MouseScrollDelta::LineDelta(_, y) => y,
                    MouseScrollDelta::PixelDelta(position) => position.y as f32,
                    _ => return,
                };
                if delta_y != 0.0 && runtime.is_some() {
                    queue_runtime_input_event(
                        pending_input_events,
                        RuntimeInputEvent::MouseWheel { delta_y },
                    );
                }
            }
            WindowEvent::PointerButton {
                state: ElementState::Pressed,
                button: ButtonSource::Mouse(MouseButton::Left),
                position,
                ..
            } => {
                *cursor_surface_pos = Some((position.x as f32, position.y as f32));
                let cursor_game_pos =
                    renderer.surface_to_game_point(position.x as f32, position.y as f32);
                if let Some(runtime) = runtime.as_mut() {
                    if let Some((x, y)) = cursor_game_pos.or(runtime.api.mouse_pos) {
                        if !runtime.api.handle_user_modeless_pointer(x, y, true) {
                            queue_runtime_input_event(
                                pending_input_events,
                                RuntimeInputEvent::MousePress { x, y },
                            );
                        }
                    } else {
                        tracing::warn!("mouse press ignored before a cursor position was observed");
                    }
                }
                tracing::info!(?cursor_game_pos, "mouse advance");
            }
            WindowEvent::PointerButton {
                state: ElementState::Released,
                button: ButtonSource::Mouse(MouseButton::Left),
                position,
                ..
            } => {
                *cursor_surface_pos = Some((position.x as f32, position.y as f32));
                let cursor_game_pos =
                    renderer.surface_to_game_point(position.x as f32, position.y as f32);
                if let Some(runtime) = runtime.as_mut() {
                    if let Some((x, y)) = cursor_game_pos.or(runtime.api.mouse_pos) {
                        if !runtime.api.handle_user_modeless_pointer(x, y, false) {
                            queue_runtime_input_event(
                                pending_input_events,
                                RuntimeInputEvent::MouseRelease { x, y },
                            );
                        }
                    } else {
                        tracing::warn!(
                            "mouse release ignored before a cursor position was observed"
                        );
                    }
                }
            }
            WindowEvent::RedrawRequested => {
                *frame_index = frame_index.saturating_add(1);
                // Advance the pause-adjusted engine clock by the real elapsed
                // milliseconds, but execute exactly one cooperative scheduler
                // pass for this host main-loop iteration. Presentation remains
                // paced at roughly 16 ms; it is not the VM time quantum.
                let frame_now = Instant::now();
                let host_clock_ms = frame_now
                    .saturating_duration_since(*host_clock_origin)
                    .as_millis()
                    .min(u128::from(u64::MAX)) as u64;
                let raw_elapsed_ms = host_clock_ms.saturating_sub(*last_engine_clock_ms);
                *last_engine_clock_ms = host_clock_ms;
                let elapsed_ms = normalize_engine_elapsed_ms(raw_elapsed_ms);
                let audio_elapsed_ms = host_clock_ms.saturating_sub(*last_audio_clock_ms);
                *last_audio_clock_ms = host_clock_ms;
                let mut frame_draw_items = Vec::new();
                if let Some(runtime) = runtime.as_mut() {
                    let frame_report = runtime_frontend::drive_runtime_frame(
                        runtime,
                        pending_input_events,
                        input_script,
                        *frame_pacing,
                        elapsed_ms,
                        audio_elapsed_ms,
                        Some(audio),
                        "window",
                    );
                    if runtime.api.screen_width > 0 && runtime.api.screen_height > 0 {
                        renderer.set_virtual_size(
                            runtime.api.screen_width as f32,
                            runtime.api.screen_height as f32,
                        );
                    }
                    let last_report = frame_report.last_report;
                    if std::mem::take(&mut runtime.api.pending_window_close_request)
                        && runtime.api.handle_host_window_close("Sys80:69")
                    {
                        target.exit();
                        return;
                    }
                    if let Some(title) = runtime.api.pending_window_title.take() {
                        window.set_title(&title);
                    }
                    if let Some((x, y)) = runtime.api.pending_window_position.take() {
                        window.set_outer_position(PhysicalPosition::new(x as f64, y as f64).into());
                    }
                    if let Some(fullscreen) = runtime.api.pending_fullscreen.take() {
                        window.set_fullscreen(
                            fullscreen.then(|| Fullscreen::Borderless(window.current_monitor())),
                        );
                    }
                    if runtime.api.window_mode == 0 {
                        if let Some((width, height)) = runtime.api.pending_window_size.take() {
                            if let Some(size) =
                                window.request_surface_size(PhysicalSize::new(width, height).into())
                            {
                                renderer.resize(size.width, size.height);
                                runtime.api.window_surface_width =
                                    size.width.min(i32::MAX as u32) as i32;
                                runtime.api.window_surface_height =
                                    size.height.min(i32::MAX as u32) as i32;
                            }
                        }
                    }
                    if let Some(visible) = runtime.api.pending_window_visible.take() {
                        window.set_visible(visible);
                    }
                    if std::mem::take(&mut runtime.api.pending_window_minimize) {
                        window.set_minimized(true);
                    }
                    if let Some(visible) = runtime.api.pending_cursor_visible.take() {
                        window.set_cursor_visible(visible);
                    }
                    if let Some((x, y)) = runtime.api.pending_cursor_position.take() {
                        let (surface_x, surface_y, _, _) =
                            renderer.game_to_surface_rect(x, y, 0.0, 0.0);
                        let position = PhysicalPosition::new(surface_x as f64, surface_y as f64);
                        if let Err(error) = window.set_cursor_position(position.into()) {
                            tracing::warn!(%error, x, y, "failed to apply target cursor motion to host cursor");
                        } else {
                            *cursor_surface_pos = Some((surface_x, surface_y));
                        }
                    }
                    if runtime.api.debug_graph {
                        let report = last_report.as_ref();
                        tracing::info!(
                            steps = frame_report.total_steps,
                            scheduler_passes_per_native_tick = 1,
                            program = report.map(|report| report.program.as_str()),
                            pc = report.map(|report| report.pc),
                            offset = ?report.and_then(|report| report.offset),
                            reason = ?report.map(|report| &report.stop_reason),
                            scheduler_passes = frame_report.scheduler_passes,
                            yield_blockers = ?runtime.api.vm_after_yield_blockers(),
                            stack = runtime.vm.stack.len(),
                            "runtime frame VM tick"
                        );
                        if let Some(report) = report {
                            runtime
                                .api
                                .trace_render_snapshot(*frame_index, Some(report));
                            if report.stop_reason.is_fatal() {
                                tracing::warn!(
                                    recent_trace = ?report.recent_trace,
                                    "runtime VM halted with error"
                                );
                            }
                        }
                    }
                    if runtime.options.fail_on_stub
                        && last_report
                            .as_ref()
                            .is_some_and(|report| report.stop_reason.is_fatal())
                    {
                        let report = last_report.as_ref().expect("checked above");
                        let message = format!(
                            "VM halted under --fail-on-stub at {} pc={} offset={:?}; stubs={:?}",
                            report.program, report.pc, report.offset, report.stubs
                        );
                        tracing::error!(%message, "GUI runtime stopped on stub");
                        if let Ok(mut slot) = failure.lock() {
                            *slot = Some(message);
                        }
                        target.exit();
                        return;
                    }
                    if runtime.api.quit_requested {
                        tracing::info!("runtime quit event received");
                        target.exit();
                        return;
                    }
                    frame_draw_items = runtime.api.graph_output_draw_items();
                    for key in frame_draw_items
                        .iter()
                        .map(|item| item.key.as_str())
                        .collect::<BTreeSet<_>>()
                    {
                        let Some(image) = runtime.api.graph_images.get(key) else {
                            continue;
                        };
                        let revision = runtime.api.graph_image_revision(key);
                        match graph_textures.get_mut(key) {
                            Some(cached) if cached.revision != revision => {
                                match renderer.update_rgba(&cached.handle, image) {
                                    Ok(handle) => {
                                        cached.handle = handle;
                                        cached.revision = revision;
                                        tracing::debug!(key, revision, "updated graph texture");
                                    }
                                    Err(err) => tracing::warn!(
                                        key,
                                        revision,
                                        %err,
                                        "graph texture update failed"
                                    ),
                                }
                            }
                            Some(_) => {}
                            None => match renderer.insert_rgba(image) {
                                Ok(handle) => {
                                    tracing::info!(
                                        key,
                                        revision,
                                        width = handle.width,
                                        height = handle.height,
                                        "uploaded graph texture"
                                    );
                                    graph_textures.insert(
                                        key.to_string(),
                                        CachedGraphTexture { handle, revision },
                                    );
                                }
                                Err(err) => {
                                    tracing::warn!(key, %err, "graph texture upload failed")
                                }
                            },
                        }
                    }
                }
                let mut commands = vec![RenderCommand::Clear {
                    color: [0.0, 0.0, 0.0, 1.0],
                }];
                // The decoded standalone image is only a startup fallback for
                // games whose runtime script could not be created. A running
                // BP runtime must render exclusively from its graph draw list.
                let draw_static_fallback = runtime.is_none();
                if draw_static_fallback
                    && let (Some(texture), Some((image_width, image_height))) =
                        (texture.as_ref(), *image_size)
                {
                    push_fit_texture_command(&mut commands, texture, image_width, image_height);
                }
                if let Some(text) = text.as_ref() {
                    commands.push(RenderCommand::DrawText {
                        text: text.clone(),
                        x: 38.0,
                        y: 38.0,
                        color: [0.0, 0.0, 0.0, 0.75],
                        size: 28.0,
                        styles: Vec::new(),
                        clip: None,
                        z: 9,
                    });
                    commands.push(RenderCommand::DrawText {
                        text: text.clone(),
                        x: 36.0,
                        y: 36.0,
                        color: [1.0, 1.0, 1.0, 1.0],
                        size: 28.0,
                        styles: Vec::new(),
                        clip: None,
                        z: 10,
                    });
                }
                // `graph_output_draw_items()` preserves ordinary raw-priority
                // order and performs only the recovered BackF exception for
                // projected Sprite modes 5/6.  The renderer performs a stable
                // sort by z, so a projected sprite deliberately moved after a
                // higher-priority BackF must inherit the preceding renderer z;
                // otherwise the sort would put it behind BackF again.  Normal
                // backgrounds and transition images remain monotonic and keep
                // their raw z, allowing BackF to cover/fade them as in the
                // target compositor.
                let mut flattened_graph_z = i32::MIN;
                for item in frame_draw_items {
                    let Some(cached) = graph_textures.get(&item.key) else {
                        continue;
                    };
                    let raw_graph_z = item.z;
                    let render_graph_z = raw_graph_z.max(flattened_graph_z);
                    if render_graph_z != raw_graph_z {
                        tracing::debug!(
                            owner = ?item.owner_object,
                            key = item.key,
                            raw_graph_z,
                            render_graph_z,
                            "promoted graph render z to preserve flattened native display order"
                        );
                    }
                    flattened_graph_z = render_graph_z;
                    let texture = &cached.handle;
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
                        clip: item
                            .clip
                            .map(|clip| [clip.x, clip.y, clip.width, clip.height]),
                        z: 2 + render_graph_z,
                    });
                }
                if let Some(runtime) = runtime.as_ref() {
                    for node in runtime
                        .api
                        .text_nodes
                        .values()
                        .filter(|node| runtime.api.should_draw_text_node(node))
                    {
                        let parent = node
                            .target_surface
                            .filter(|surface| runtime.api.graph_surfaces.contains_key(surface))
                            .map(|surface| {
                                let (x, y) = runtime.api.surface_world_position(surface);
                                (x, y, runtime.api.surface_display_opacity(surface))
                            })
                            .unwrap_or((0.0, 0.0, 1.0));
                        let layout = runtime.api.formatted_text_layout(node);
                        let x = parent.0 + node.x;
                        let cursor_y = parent.1 + node.y;
                        let y = cursor_y + layout.body_y_offset;
                        let clip = node
                            .target_surface
                            .and_then(|surface| runtime.api.surface_chain_clip(surface))
                            .map(|clip| [clip.x, clip.y, clip.width, clip.height]);
                        let size = node.size.max(12.0);
                        let mut color = node.color;
                        color[3] *= parent.2;
                        let styles = node
                            .style_spans
                            .iter()
                            .map(|span| TextStyleSpan {
                                start_char: span.start_char,
                                end_char: span.end_char,
                                packed_rgb: span.style.packed_rgb,
                                bold: span.style.bold,
                                italic: span.style.italic,
                            })
                            .collect::<Vec<_>>();
                        let shadow_styles = styles
                            .iter()
                            .map(|span| TextStyleSpan {
                                packed_rgb: None,
                                ..*span
                            })
                            .collect::<Vec<_>>();
                        if let Some((shadow_x, shadow_y, shadow_alpha)) =
                            runtime.api.graph_defaults.shadow_for_height(size)
                        {
                            commands.push(RenderCommand::DrawText {
                                text: node.text.clone(),
                                x: x + shadow_x as f32,
                                y: y + shadow_y as f32,
                                color: [0.0, 0.0, 0.0, color[3] * shadow_alpha],
                                size,
                                styles: shadow_styles,
                                clip,
                                z: node.z - 1,
                            });
                        }
                        commands.push(RenderCommand::DrawText {
                            text: node.text.clone(),
                            x,
                            y,
                            color,
                            size,
                            styles,
                            clip,
                            z: node.z,
                        });
                        let mut positioned_node = node.clone();
                        positioned_node.x = x;
                        positioned_node.y = cursor_y;
                        for (ruby, ruby_x, ruby_y, ruby_size) in
                            text::ruby_draw_runs(&positioned_node, layout)
                        {
                            if let Some((shadow_x, shadow_y, shadow_alpha)) =
                                runtime.api.graph_defaults.shadow_for_height(ruby_size)
                            {
                                commands.push(RenderCommand::DrawText {
                                    text: ruby.clone(),
                                    x: ruby_x + shadow_x as f32,
                                    y: ruby_y + shadow_y as f32,
                                    color: [0.0, 0.0, 0.0, color[3] * shadow_alpha],
                                    size: ruby_size,
                                    styles: Vec::new(),
                                    clip,
                                    z: node.z,
                                });
                            }
                            commands.push(RenderCommand::DrawText {
                                text: ruby,
                                x: ruby_x,
                                y: ruby_y,
                                color,
                                size: ruby_size,
                                styles: Vec::new(),
                                clip,
                                z: node.z + 1,
                            });
                        }
                    }
                }
                if let Some(runtime) = runtime.as_ref() {
                    for (line, x, y, size) in runtime.api.user_modeless_dialog_text_lines() {
                        commands.push(RenderCommand::DrawText {
                            text: line.clone(),
                            x: x + 2.0,
                            y: y + 2.0,
                            color: [0.0, 0.0, 0.0, 0.9],
                            size,
                            styles: Vec::new(),
                            clip: None,
                            z: 29_999,
                        });
                        commands.push(RenderCommand::DrawText {
                            text: line,
                            x,
                            y,
                            color: [0.95, 0.95, 0.95, 1.0],
                            size,
                            styles: Vec::new(),
                            clip: None,
                            z: 30_000,
                        });
                    }
                }
                renderer.submit(&commands);
                let present_started = Instant::now();
                window.pre_present_notify();
                match renderer.render() {
                    Ok(()) => {
                        if let Some(runtime) = runtime.as_mut() {
                            runtime
                                .api
                                .performance_profile
                                .record_present(present_started.elapsed());
                            runtime.api.presentation_state =
                                runtime.api.engine_time_ms.min(i32::MAX as u64) as i32;
                        }
                    }
                    Err(err) => {
                        tracing::error!(%err, "render failed");
                        target.exit();
                    }
                }
            }
            _ => {}
        }
    }

    fn about_to_wait(&mut self, target: &dyn ActiveEventLoop) {
        let now = Instant::now();
        if let Some(graphics) = self.graphics.as_ref() {
            if now >= self.next_frame {
                graphics.window.request_redraw();
                // Keep the native 16 ms phase instead of accumulating render-time drift.
                while self.next_frame <= now {
                    self.next_frame += Duration::from_millis(NATIVE_TICK_MS);
                }
            }
            target.set_control_flow(ControlFlow::WaitUntil(self.next_frame));
        } else {
            target.set_control_flow(ControlFlow::Wait);
        }
    }
}

impl WindowApp {
    fn create_graphics(&mut self, target: &dyn ActiveEventLoop) -> Result<Graphics> {
        let attributes = WindowAttributes::default()
            .with_title(&self.title)
            .with_surface_size(winit::dpi::LogicalSize::new(1280.0, 720.0));
        let attributes = if let Some(runtime) = self.runtime.as_ref() {
            super::window_icon::configure(attributes, &game_root_path(&runtime.api.manager))
        } else {
            attributes
        };
        let window: Arc<dyn Window> = target
            .create_window(attributes)
            .map_err(|err| EthornellError::Other(format!("window init failed: {err}")))?
            .into();
        let mut renderer = pollster::block_on(Renderer::new(window.clone()))?;
        if let Some(runtime) = self.runtime.as_mut() {
            runtime.api.graphics_memory_metric = renderer.graphics_memory_metric();
            let size = window.surface_size();
            runtime.api.window_surface_width = size.width.min(i32::MAX as u32) as i32;
            runtime.api.window_surface_height = size.height.min(i32::MAX as u32) as i32;
        }
        let texture = self
            .image
            .as_ref()
            .map(|image| renderer.insert_rgba(image))
            .transpose()?;
        self.next_frame = Instant::now();
        Ok(Graphics {
            renderer,
            window,
            texture,
            graph_textures: BTreeMap::new(),
        })
    }
}

impl Drop for WindowApp {
    fn drop(&mut self) {
        let runtime = &self.runtime;

        if let Some(runtime) = runtime.as_ref() {
            runtime.api.write_call_coverage_if_requested();
            if let Some(path) = std::env::var_os("ETHORNELL_GUI_SNAPSHOT").map(PathBuf::from) {
                match snapshot::write_runtime_snapshot(&runtime.api, &path) {
                    Ok(()) => tracing::info!(
                        path = %path.display(),
                        "GUI runtime snapshot written"
                    ),
                    Err(err) => tracing::warn!(
                        path = %path.display(),
                        %err,
                        "GUI runtime snapshot failed"
                    ),
                }
            }
        }
    }
}

pub(super) fn run_window(
    title: &str,
    image: Option<DecodedImage>,
    text: Option<String>,
    bgm: Option<Vec<u8>>,
    runtime: Option<RuntimeEngine>,
) -> Result<()> {
    let event_loop =
        EventLoop::new().map_err(|err| EthornellError::Other(format!("event loop: {err}")))?;
    let mut audio = AudioSystem::new()?;
    if runtime.is_none()
        && let Some(bgm) = bgm
    {
        match audio.play_from_bytes(&bgm) {
            Ok(()) => tracing::info!("title BGM playback started"),
            Err(err) => tracing::warn!(%err, "title BGM playback failed"),
        }
    }
    let failure = Arc::new(Mutex::new(None));
    let app = WindowApp {
        title: title.to_owned(),
        image_size: image.as_ref().map(|image| (image.width, image.height)),
        image,
        text,
        runtime,
        audio,
        graphics: None,
        cursor_surface_pos: None,
        pending_input_events: VecDeque::new(),
        pending_drop: None,
        frame_pacing: RuntimeFramePacing::from_env(),
        next_frame: Instant::now(),
        host_clock_origin: Instant::now(),
        last_engine_clock_ms: 0,
        last_audio_clock_ms: 0,
        frame_index: 0,
        input_script: HeadlessInputScript::from_env(),
        failure: failure.clone(),
    };
    event_loop
        .run_app(app)
        .map_err(|err| EthornellError::Other(format!("event loop failed: {err}")))?;
    if let Some(error) = failure.lock().unwrap().take() {
        return Err(EthornellError::Other(error));
    }
    Ok(())
}
