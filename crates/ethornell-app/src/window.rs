use super::*;
use winit::application::ApplicationHandler;
use winit::data_transfer::TypeHint;
use winit::dpi::{PhysicalPosition, PhysicalSize};
use winit::event::{
    ButtonSource, ElementState, KeyEvent, MouseButton, MouseScrollDelta, TouchPhase, WindowEvent,
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
    /// The game size the window was last fitted to.
    fitted_game_size: Option<(i32, i32)>,
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
    trackpad_scroll: TrackpadScroll,
    /// Alt is held: keys go out as WM_SYSKEYDOWN/WM_SYSKEYUP.
    alt_held: bool,
    failure: Arc<Mutex<Option<String>>>,
    font: ab_glyph::FontArc,
}

/// Logical points of trackpad travel that make one wheel notch.
const TRACKPAD_POINTS_PER_NOTCH: f64 = 32.0;
/// macOS starts the momentum of a swipe right after the fingers lift, and
/// winit reports both a touch and a momentum as `Started`: a start this
/// soon after a lift is momentum.
const TRACKPAD_MOMENTUM_START: Duration = Duration::from_millis(60);

/// Turns trackpad scrolling into wheel notches. The target takes each
/// WM_MOUSEWHEEL as one step (a line, a log entry), while a trackpad sends
/// a stream of small pixel deltas and, after the fingers lift, momentum.
/// Counting every event as a notch made a short swipe scroll the log by
/// dozens of entries, and its momentum ran on past the end of the log,
/// which the game takes as closing it. Finger travel now adds up to one
/// notch per `TRACKPAD_POINTS_PER_NOTCH`, and momentum is left out.
#[derive(Debug, Default)]
struct TrackpadScroll {
    travel: f64,
    momentum: bool,
    lifted_at: Option<Instant>,
}

impl TrackpadScroll {
    /// Wheel notches for one pixel delta of `points` logical points
    /// (positive up, like a wheel), signed.
    fn notches(&mut self, points: f64, phase: TouchPhase, now: Instant) -> i32 {
        match phase {
            TouchPhase::Started => {
                self.momentum = self.lifted_at.take().is_some_and(|lifted| {
                    now.saturating_duration_since(lifted) <= TRACKPAD_MOMENTUM_START
                });
                self.travel = 0.0;
            }
            TouchPhase::Ended | TouchPhase::Cancelled => {
                if !self.momentum {
                    self.lifted_at = Some(now);
                }
                self.momentum = false;
                self.travel = 0.0;
                return 0;
            }
            TouchPhase::Moved => {}
        }
        if self.momentum {
            return 0;
        }
        self.travel += points;
        let notches = (self.travel / TRACKPAD_POINTS_PER_NOTCH).trunc();
        self.travel -= notches * TRACKPAD_POINTS_PER_NOTCH;
        notches as i32
    }
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
            trackpad_scroll,
            alt_held,
            failure,
            ..
        } = self;
        let Some(Graphics {
            renderer,
            window,
            texture,
            graph_textures,
            fitted_game_size,
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
            WindowEvent::Moved(position) => {
                // WM_WINDOWPOSCHANGING: while Sys81:65 is set, a windowed game
                // window is put back at the centre of its monitor
                // (sub_461B40).
                if let Some(runtime) = runtime.as_ref()
                    && runtime.api.window_center_lock != 0
                    && window.fullscreen().is_none()
                    && let Some(monitor) = window.current_monitor()
                    && let Some(mode) = monitor.current_video_mode()
                {
                    let origin = monitor.position().unwrap_or_default();
                    let size = window.outer_size();
                    let x = origin.x + (mode.size().width as i32 - size.width as i32) / 2;
                    let y = origin.y + (mode.size().height as i32 - size.height as i32) / 2;
                    if (position.x, position.y) != (x, y) {
                        window.set_outer_position(PhysicalPosition::new(x, y).into());
                    }
                }
            }
            WindowEvent::Focused(focused) => {
                if let Some(runtime) = runtime.as_mut() {
                    runtime
                        .api
                        .sync_host_window_state(focused, window.is_minimized().unwrap_or(false));
                    if !focused && runtime.api.input_requires_focus {
                        pending_input_events.clear();
                        runtime.api.pending_input_state = None;
                        runtime.api.pending_input_descriptor = None;
                        runtime.api.pending_click = None;
                        runtime.api.pending_object_state = None;
                        runtime.api.input_down_descriptors.clear();
                        runtime.api.input_down_reported.clear();
                        runtime.api.mouse_pressed = false;
                        runtime.api.finish_graph_knob_drag();
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
                        // WM_DROPFILES (sub_499930): event {16, 0, 0}, then
                        // sub_464010 keeps the first path when there is one.
                        runtime.api.queued_system_events.push_back([16, 0, 0]);
                        if !paths.is_empty() {
                            runtime.api.dropped_files = paths
                                .iter()
                                .map(|path| path.to_string_lossy().into_owned())
                                .collect();
                        }
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
                        repeat,
                        ..
                    },
                ..
            } => {
                if matches!(code, KeyCode::AltLeft | KeyCode::AltRight) {
                    *alt_held = true;
                }
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
                    if runtime.is_some() {
                        // Windows delivers Alt, F10 and keys pressed while Alt
                        // is held as WM_SYSKEYDOWN.
                        let event = if *alt_held || descriptor == 0x79 {
                            RuntimeInputEvent::SysKeyPress { descriptor, repeat }
                        } else {
                            RuntimeInputEvent::KeyPress { descriptor }
                        };
                        queue_runtime_input_event(pending_input_events, event);
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
                let system = *alt_held || code == KeyCode::F10;
                if matches!(code, KeyCode::AltLeft | KeyCode::AltRight) {
                    *alt_held = false;
                }
                if let Some(descriptor) = input_descriptor_for_keycode(code)
                    && let Some(runtime) = runtime.as_mut()
                    && !runtime.api.has_visible_user_modeless_dialog()
                {
                    let event = if system {
                        RuntimeInputEvent::SysKeyRelease { descriptor }
                    } else {
                        RuntimeInputEvent::KeyRelease { descriptor }
                    };
                    queue_runtime_input_event(pending_input_events, event);
                }
            }
            WindowEvent::MouseWheel { delta, phase, .. } => {
                if let MouseScrollDelta::LineDelta(x, _) = delta
                    && x != 0.0
                    && runtime.is_some()
                {
                    queue_runtime_input_event(
                        pending_input_events,
                        RuntimeInputEvent::MouseHorizontalWheel { delta_x: x },
                    );
                }
                let (delta_y, count) = match delta {
                    MouseScrollDelta::LineDelta(_, y) => (y, 1),
                    MouseScrollDelta::PixelDelta(position) => {
                        let points = position.y / window.scale_factor();
                        let notches = trackpad_scroll.notches(points, phase, Instant::now());
                        (notches.signum() as f32, notches.unsigned_abs())
                    }
                    _ => return,
                };
                if delta_y != 0.0 && runtime.is_some() {
                    for _ in 0..count {
                        queue_runtime_input_event(
                            pending_input_events,
                            RuntimeInputEvent::MouseWheel { delta_y },
                        );
                    }
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
            WindowEvent::PointerButton {
                state,
                button:
                    ButtonSource::Mouse(
                        button @ (MouseButton::Right
                        | MouseButton::Middle
                        | MouseButton::Back
                        | MouseButton::Forward),
                    ),
                position,
                ..
            } => {
                // WndProc descriptors: right 2, middle 4, XBUTTON1 5, XBUTTON2 6.
                let descriptor = match button {
                    MouseButton::Right => 2,
                    MouseButton::Middle => 4,
                    MouseButton::Back => 5,
                    _ => 6,
                };
                *cursor_surface_pos = Some((position.x as f32, position.y as f32));
                let cursor_game_pos =
                    renderer.surface_to_game_point(position.x as f32, position.y as f32);
                if let Some(runtime) = runtime.as_mut()
                    && let Some((x, y)) = cursor_game_pos.or(runtime.api.mouse_pos)
                {
                    queue_runtime_input_event(
                        pending_input_events,
                        if state == ElementState::Pressed {
                            RuntimeInputEvent::ButtonPress {
                                button: descriptor,
                                x,
                                y,
                            }
                        } else {
                            RuntimeInputEvent::ButtonRelease {
                                button: descriptor,
                                x,
                                y,
                            }
                        },
                    );
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
                // sub_499290 drops gaps above the Sys81:04 threshold in
                // `begin_native_pass`.
                let elapsed_ms = raw_elapsed_ms;
                let audio_elapsed_ms = host_clock_ms.saturating_sub(*last_audio_clock_ms);
                *last_audio_clock_ms = host_clock_ms;
                if let Some(runtime) = runtime.as_mut() {
                    let focused = runtime.api.window_focused;
                    runtime
                        .api
                        .sync_host_window_state(focused, window.is_minimized().unwrap_or(false));
                    // GetAdapterDisplayMode (dword_5177C0..C8): the monitor's
                    // size and refresh rate (sub_45E9E0).
                    let video_mode = window
                        .current_monitor()
                        .and_then(|monitor| monitor.current_video_mode());
                    runtime.api.performance_profile.refresh_rate = video_mode
                        .as_ref()
                        .and_then(|mode| mode.refresh_rate_millihertz())
                        .map_or(0, |rate| (u32::from(rate) + 500) / 1000);
                    if let Some(mode) = video_mode.as_ref() {
                        runtime.api.desktop_display_size = (mode.size().width, mode.size().height);
                    }
                    // EnumDisplayMonitors (dword_566854) and the current
                    // monitor for sub_46FE90 / sub_461B40.
                    let monitor_rect = |monitor: &winit::monitor::MonitorHandle| {
                        let origin = monitor.position()?;
                        let size = monitor.current_video_mode()?.size();
                        Some([origin.x, origin.y, origin.x + size.width as i32, origin.y + size.height as i32])
                    };
                    runtime.api.monitor_rects =
                        window.available_monitors().filter_map(|monitor| monitor_rect(&monitor)).collect();
                    runtime.api.current_monitor_rect =
                        window.current_monitor().and_then(|monitor| monitor_rect(&monitor));
                    if let Ok(position) = window.outer_position() {
                        runtime.api.window_outer_position = (position.x, position.y);
                    }
                    let (outer, inner) = (window.outer_size(), window.surface_size());
                    runtime.api.window_decoration_size = (
                        outer.width.saturating_sub(inner.width) as i32,
                        outer.height.saturating_sub(inner.height) as i32,
                    );
                    let frame_report = runtime_frontend::drive_runtime_frame(
                        runtime,
                        pending_input_events,
                        input_script,
                        *frame_pacing,
                        // sub_499290 returns the frozen time inside a pause
                        // section that froze the clock.
                        if runtime.api.engine_clock_frozen { 0 } else { elapsed_ms },
                        audio_elapsed_ms,
                        Some(audio),
                        "window",
                    );
                    let raster_error = snapshot::take_raster_error().or_else(|| {
                        snapshot::take_missing_glyphs()
                            .then(|| "font lacks required CPU text glyphs".to_owned())
                    });
                    if let Some(error) = raster_error {
                        if let Ok(mut slot) = failure.lock() {
                            *slot = Some(error);
                        }
                        target.exit();
                        return;
                    }
                    renderer.set_presentation_scale(runtime.api.presentation_scale());
                    if runtime.api.screen_width > 0 && runtime.api.screen_height > 0 {
                        renderer.set_virtual_size(
                            runtime.api.screen_width as f32,
                            runtime.api.screen_height as f32,
                        );
                        // A window, not full screen: size it to the game once
                        // per game size (the target's window is the game's
                        // size), shrunk to fit the screen. The player may
                        // resize it afterwards.
                        let game_size = (runtime.api.screen_width, runtime.api.screen_height);
                        if runtime.api.window_mode == 0 && *fitted_game_size != Some(game_size) {
                            *fitted_game_size = Some(game_size);
                            let screen = window.current_monitor().and_then(|monitor| {
                                let scale = monitor.scale_factor();
                                let mode = monitor.current_video_mode()?;
                                Some((
                                    f64::from(mode.size().width) / scale,
                                    f64::from(mode.size().height) / scale,
                                ))
                            });
                            let (width, height) = fitted_window_size(
                                (game_size.0 as u32, game_size.1 as u32),
                                screen,
                            );
                            if let Some(size) = window.request_surface_size(
                                winit::dpi::LogicalSize::new(width, height).into(),
                            ) {
                                renderer.resize(size.width, size.height);
                                runtime.api.window_surface_width =
                                    size.width.min(i32::MAX as u32) as i32;
                                runtime.api.window_surface_height =
                                    size.height.min(i32::MAX as u32) as i32;
                            }
                        }
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
                        // dword_50721C (Sys81:62): 1 takes the adapter of the
                        // window's monitor (sub_45EA30), 0 adapter 0.
                        let monitor = if runtime.api.window_monitor_adapter_mode == 1 {
                            window.current_monitor()
                        } else {
                            window.primary_monitor()
                        };
                        window.set_fullscreen(fullscreen.then(|| Fullscreen::Borderless(monitor)));
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
                    // WinMain leaves its loop once the root interpreter ends:
                    // Sys80:6A (status 6), the end of ipl._bp, or a script
                    // error (sub_464C50 reports it, then the process exits).
                    if runtime.vm.halted
                        && runtime.api.native_system.restart.is_none()
                        && last_report.as_ref().is_some_and(|report| {
                            matches!(
                                report.stop_reason,
                                ethornell_vm::VmStopReason::InterpreterTerminated
                                    | ethornell_vm::VmStopReason::Completed
                            ) || report.stop_reason.is_fatal()
                        })
                    {
                        let report = last_report.as_ref().expect("checked above");
                        if report.stop_reason.is_fatal() {
                            let message = format!(
                                "script stopped at {} pc={} offset={:?}: {:?}",
                                report.program,
                                report.pc,
                                report.offset,
                                report.recent_trace.last()
                            );
                            tracing::error!(%message, "runtime stopped");
                            if let Ok(mut slot) = failure.lock() {
                                *slot = Some(message);
                            }
                        } else {
                            tracing::info!(reason = ?report.stop_reason, "root interpreter ended");
                        }
                        target.exit();
                        return;
                    }
                    if runtime.api.quit_requested || runtime.api.native_system.restart.is_some() && runtime.vm.halted {
                        tracing::info!("runtime quit event received");
                        runtime.api.run_scheduled_restart();
                        target.exit();
                        return;
                    }
                }
                if let Some(runtime) = runtime.as_mut() {
                    runtime.api.performance_profile.begin();
                }
                // sub_462100: without a due presentation the window shows the
                // last composed frame again (WM_PAINT path).
                if runtime.as_mut().is_some_and(|runtime| !runtime.api.take_present_due()) {
                    if let Some(runtime) = runtime.as_mut() {
                        runtime.api.performance_profile.end(false);
                    }
                    window.pre_present_notify();
                    if let Err(err) = renderer.render() {
                        tracing::error!(%err, "render failed");
                        if let Ok(mut slot) = failure.lock() {
                            *slot = Some(err.to_string());
                        }
                        target.exit();
                    }
                    return;
                }
                let commands = if let Some(runtime) = runtime.as_ref() {
                    match frame_presenter::prepare_frame(renderer, &runtime.api, graph_textures) {
                        Ok(commands) => commands,
                        Err(error) => {
                            if let Ok(mut slot) = failure.lock() {
                                *slot = Some(error.to_string());
                            }
                            target.exit();
                            return;
                        }
                    }
                } else {
                    let mut commands = vec![RenderCommand::Clear {
                        color: [0.0, 0.0, 0.0, 1.0],
                    }];
                    // The decoded standalone image is only a startup fallback for
                    // games whose runtime script could not be created. A running
                    // BP runtime must render exclusively from its graph draw list.
                    if let (Some(texture), Some((image_width, image_height))) =
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
                            line_height: None,
                            styles: Vec::new(),
                            glow: None,
                            clip: None,
                            z: 9,
                        });
                        commands.push(RenderCommand::DrawText {
                            text: text.clone(),
                            x: 36.0,
                            y: 36.0,
                            color: [1.0, 1.0, 1.0, 1.0],
                            size: 28.0,
                            line_height: None,
                            styles: Vec::new(),
                            glow: None,
                            clip: None,
                            z: 10,
                        });
                    }
                    commands
                };
                renderer.submit(&commands);
                if let Some(runtime) = runtime.as_mut() {
                    runtime.api.performance_profile.end(true);
                }
                window.pre_present_notify();
                match renderer.render() {
                    Ok(()) => {
                        if let Some(runtime) = runtime.as_mut() {
                            runtime.api.presentation_state = runtime
                .api
                .clock_base_ms
                .wrapping_add(runtime.api.engine_time_ms as u32) as i32;
                        }
                    }
                    Err(err) => {
                        tracing::error!(%err, "render failed");
                        if let Ok(mut slot) = failure.lock() {
                            *slot = Some(err.to_string());
                        }
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
        let mut renderer =
            pollster::block_on(Renderer::new_with_font(window.clone(), self.font.clone()))?;
        if let Some(runtime) = self.runtime.as_mut() {
            runtime.api.display_adapter_description = renderer.adapter_name().to_owned();
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
            fitted_game_size: None,
        })
    }
}

/// The window size, in points, for a game of `game` pixels: the game's own
/// size, or, when that doesn't fit a screen of `screen` points with room
/// left for the menu bar, the title bar and the Dock, the largest size of the
/// same aspect that does.
fn fitted_window_size(game: (u32, u32), screen: Option<(f64, f64)>) -> (f64, f64) {
    let (width, height) = (f64::from(game.0.max(1)), f64::from(game.1.max(1)));
    let Some((screen_width, screen_height)) = screen else {
        return (width, height);
    };
    let scale = (screen_width * 0.95 / width)
        .min(screen_height * 0.85 / height)
        .min(1.0);
    ((width * scale).floor(), (height * scale).floor())
}

#[cfg(test)]
mod fitted_window_tests {
    use super::fitted_window_size;

    #[test]
    fn the_window_takes_the_game_size_when_it_fits() {
        assert_eq!(
            fitted_window_size((1280, 960), Some((1920.0, 1200.0))),
            (1280.0, 960.0)
        );
        assert_eq!(fitted_window_size((1280, 960), None), (1280.0, 960.0));
    }

    #[test]
    fn a_smaller_screen_gets_the_largest_window_of_the_game_aspect() {
        // A 13-inch MacBook Air: 1440x900 points.
        assert_eq!(
            fitted_window_size((1280, 960), Some((1440.0, 900.0))),
            (1020.0, 765.0)
        );
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
    let font = snapshot::load_default_font()?;
    let _font_session = snapshot::enter_font_session(&font);
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
        trackpad_scroll: TrackpadScroll::default(),
        alt_held: false,
        failure: failure.clone(),
        font,
    };
    event_loop
        .run_app(app)
        .map_err(|err| EthornellError::Other(format!("event loop failed: {err}")))?;
    if let Some(error) = failure.lock().unwrap().take() {
        return Err(EthornellError::Other(error));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::{TRACKPAD_POINTS_PER_NOTCH, TrackpadScroll};
    use std::time::{Duration, Instant};
    use winit::event::TouchPhase;

    #[test]
    fn trackpad_travel_adds_up_to_notches_and_momentum_is_left_out() {
        let mut scroll = TrackpadScroll::default();
        let start = Instant::now();
        let at = |ms| start + Duration::from_millis(ms);
        let step = TRACKPAD_POINTS_PER_NOTCH / 4.0;

        assert_eq!(scroll.notches(0.0, TouchPhase::Started, at(0)), 0);
        let swipe = (1..=10)
            .map(|ms| scroll.notches(-step, TouchPhase::Moved, at(ms)))
            .collect::<Vec<_>>();
        assert_eq!(swipe, [0, 0, 0, -1, 0, 0, 0, -1, 0, 0]);
        assert_eq!(scroll.notches(0.0, TouchPhase::Ended, at(11)), 0);

        // The swipe's momentum: started right after the lift.
        assert_eq!(scroll.notches(-step, TouchPhase::Started, at(12)), 0);
        for ms in 13..40 {
            assert_eq!(scroll.notches(-step * 3.0, TouchPhase::Moved, at(ms)), 0);
        }
        assert_eq!(scroll.notches(0.0, TouchPhase::Ended, at(40)), 0);

        // A new touch later counts again, from zero, upwards.
        assert_eq!(scroll.notches(0.0, TouchPhase::Started, at(500)), 0);
        assert_eq!(
            scroll.notches(TRACKPAD_POINTS_PER_NOTCH * 2.5, TouchPhase::Moved, at(501)),
            2
        );
    }

}
