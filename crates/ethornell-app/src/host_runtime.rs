//! A host-owned session: no window event loop, no process-global environment
//! mutation, and no Send/Sync promises for VM/Ruffle/GPU state.
use super::*;
use ab_glyph::FontArc;
use std::sync::atomic::{AtomicU64, Ordering};

fn elapsed_ms_with_carry(delta_ns: u64, remainder: &mut u64) -> u64 {
    let total = delta_ns.saturating_add(*remainder);
    *remainder = total % 1_000_000;
    total / 1_000_000
}

// A bounded admission profile, not a process RSS limit. Real Amairo bootstrap
// measured 648,441,292 retained VM/shared/journal bytes at 14 VMs (before
// threads handed one heap around instead of copying it). Preserve the
// conservative profile on lower-memory or unknown Android hardware; >=4GiB
// devices admit that measured workload with ~24% headroom, never without a cap.
fn host_vm_budget(physical_bytes: Option<u64>) -> ethornell_vm::VmResourceBudget {
    ethornell_vm::VmResourceBudget {
        max_vm_count: 64,
        max_memory_capacity_bytes: if physical_bytes
            .is_some_and(|bytes| bytes >= 4 * 1024 * 1024 * 1024)
        {
            768 * 1024 * 1024
        } else {
            512 * 1024 * 1024
        },
    }
}
fn host_physical_memory() -> Option<u64> {
    #[cfg(any(target_os = "android", target_os = "ios"))]
    {
        let pages = unsafe { libc::sysconf(libc::_SC_PHYS_PAGES) };
        let size = unsafe { libc::sysconf(libc::_SC_PAGESIZE) };
        if pages <= 0 || size <= 0 {
            return None;
        }
        (pages as u64).checked_mul(size as u64)
    }
    #[cfg(target_os = "vita")]
    {
        Some(512 * 1024 * 1024)
    }
    #[cfg(target_os = "horizon")]
    {
        Some(4 * 1024 * 1024 * 1024)
    }
    #[cfg(not(any(
        target_os = "android",
        target_os = "ios",
        target_os = "vita",
        target_os = "horizon"
    )))]
    {
        Some(4 * 1024 * 1024 * 1024)
    } // host diagnostic profile, not device RAM evidence
}

// The candidate is borrowed from the JNI owner. Failure must drop it before
// returning so that the owner can release its native window immediately.
fn commit_surface_candidate<T>(
    slot: &mut Option<T>,
    candidate: T,
    ready: impl FnOnce() -> Result<()>,
) -> Result<()> {
    ready()?;
    *slot = Some(candidate);
    Ok(())
}

#[derive(Clone, Debug)]
pub struct HostConfig {
    pub resource_root: PathBuf,
    pub user_root: PathBuf,
    pub font_path: Option<PathBuf>,
    /// Font file bytes, for hosts without font files (the web); takes
    /// precedence over `font_path`.
    pub font_data: Option<Vec<u8>>,
    pub font_face_index: u32,
    pub gbk: bool,
    pub backend: u32,
    pub strict: bool,
    pub trace: bool,
    pub silent: bool,
}
#[derive(Clone, Debug)]
pub struct HostEvent {
    pub kind: u32,
    pub request_id: u64,
    pub code: i32,
    pub text: String,
}
#[derive(Default)]
pub(super) struct HostServices {
    pub(super) enabled: bool,
    events: VecDeque<HostEvent>,
    pub(super) pending: BTreeMap<u64, usize>,
    /// Pending UserB0 input dialogs, answered with a JSON response.
    pub(super) dialogs: BTreeMap<u64, ethornell_vm::UserDialogRequest>,
    /// Pending Sys80:3C prompts: the message and the file they wait for.
    retries: BTreeMap<u64, (String, String)>,
    fatal: Option<String>,
    pub(super) input_trace: bool,
    input_trace_count: u32,
}
impl HostServices {
    pub(super) fn input_gate(&mut self, values: [u32; 9]) {
        if !self.enabled || !self.input_trace || self.input_trace_count >= 64 {
            return;
        }
        self.input_trace_count += 1;
        let [
            pc,
            frame,
            event,
            payload,
            parameter,
            group,
            item,
            running,
            gate,
        ] = values;
        self.events.push_back(HostEvent {kind:1,request_id:0,code:1002,text:format!("input_gate pc={pc} frame={frame} event={event} payload={payload} parameter={parameter} group={group} item={item} running={running} gate={gate}")});
    }
    pub(super) fn input_route(&mut self, values: [u32; 7], bits: [bool; 5]) {
        if !self.enabled || !self.input_trace || self.input_trace_count >= 64 {
            return;
        }
        let [object, group, item, root, flags, current, pending] = values;
        let [pointer, hit, press, up, event6] = bits;
        self.input_trace_count += 1;
        self.events.push_back(HostEvent {kind:1,request_id:0,code:1001,text:format!("input_route object={object} group={group} item={item} root={root} flags={flags} current={current} pending={pending} pointer={} hit={} press={} up={} event6={}",u32::from(pointer),u32::from(hit),u32::from(press),u32::from(up),u32::from(event6))});
    }
    pub(super) fn unsupported(&mut self, capability: &str) {
        if !self.enabled {
            return;
        }
        self.fatal = Some(format!(
            "unsupported or denied host capability: {capability}"
        ));
    }
    /// Event kind 5 names the dialog type; the host fetches the JSON payload
    /// with `HostRuntime::request_payload` and answers with JSON text.
    pub(super) fn request_dialog(&mut self, request: ethornell_vm::UserDialogRequest) -> u64 {
        let id = Self::next_request_id();
        self.pending.insert(id, 1);
        self.events.push_back(HostEvent {
            kind: 5,
            request_id: id,
            code: 1,
            text: super::dialog_protocol::dialog_kind(&request).to_string(),
        });
        self.dialogs.insert(id, request);
        id
    }
    /// Sys80:3C's Retry/Quit prompt for a missing required file: a kind-2
    /// confirmation (1 = Retry). Retry rechecks the file and, if it is still
    /// missing, asks again under the same id.
    pub(super) fn request_retry(&mut self, message: String, file: String) -> u64 {
        let id = self.request(message.clone(), 1);
        self.retries.insert(id, (message, file));
        id
    }
    fn next_request_id() -> u64 {
        static NEXT: AtomicU64 = AtomicU64::new(1);
        NEXT.fetch_add(1, Ordering::Relaxed)
    }
    pub(super) fn request(&mut self, text: String, outputs: usize) -> u64 {
        let id = Self::next_request_id();
        self.pending.insert(id, outputs);
        self.events.push_back(HostEvent {
            kind: 2,
            request_id: id,
            code: outputs as i32,
            text,
        });
        id
    }
}

pub struct HostRuntime {
    runtime: RuntimeEngine,
    policy: file_policy::FilePolicy,
    _writer_lock: file_policy::WriterLock,
    renderer: Option<Renderer<'static>>,
    textures: BTreeMap<String, CachedGraphTexture>,
    font: FontArc,
    audio: Option<AudioSystem>,
    backend: u32,
    input_queue: VecDeque<RuntimeInputEvent>,
    /// Scripted input for unattended runs (`set_input_script`).
    input_script: Option<HeadlessInputScript>,
    generation: u64,
    paused: u32,
    last_tick: Option<u64>,
    tick_remainder_ns: u64,
    engine_frames: u64,
    closed: bool,
    audio_suspended: bool,
    vm_budget: ethornell_vm::VmResourceBudget,
}
impl HostRuntime {
    pub fn create(config: HostConfig) -> Result<Self> {
        if config.backend > 2 {
            return Err(EthornellError::Other("invalid GPU backend".into()));
        }
        let policy = file_policy::FilePolicy::new(&config.resource_root, &config.user_root)?;
        let writer_lock = policy.lock_writer()?;
        policy.validate_resource_tree().map_err(|e| {
            EthornellError::Other(format!("host_policy:301:{}", e.raw_os_error().unwrap_or(0)))
        })?;
        let font = match (config.font_data, config.font_path.as_deref()) {
            (Some(bytes), _) => {
                ethornell_render::FontProvider::load_bytes(bytes, config.font_face_index)?
            }
            (None, Some(path)) => {
                ethornell_render::FontProvider::load_file(path, config.font_face_index)?
            }
            (None, None) => ethornell_render::FontProvider::load_system_for_text(if config.gbk {
                "A日本語中文简体繁體"
            } else {
                "A日本語中文"
            })?,
        };
        let manager = policy.open_manager()?;
        let bytes = manager.read_decoded_from_archive("system.arc", "ipl._bp")?;
        let mut runtime = file_policy::scope(&policy, || {
            snapshot::with_font(&font, || {
                let runtime = RuntimeEngine::new_explicit(
                    "system.arc:ipl._bp",
                    &bytes,
                    manager,
                    policy.resource.clone(),
                    false,
                    config.strict,
                    if config.gbk {
                        encoding_rs::GBK
                    } else {
                        encoding_rs::SHIFT_JIS
                    },
                );
                if let Some(error) = snapshot::take_raster_error() {
                    return Err(EthornellError::Other(error));
                }
                if snapshot::take_missing_glyphs() {
                    return Err(EthornellError::Other("font lacks bootstrap glyphs".into()));
                }
                Ok(runtime)
            })
        })?;
        runtime.use_host_clock();
        runtime.api.host.enabled = true;
        runtime.api.host.input_trace = config.trace;
        runtime.api.native_dialog_presentation = false;
        // Desktop diagnostic environment knobs are not session configuration.
        // None may inject synthetic input or filesystem diagnostics in a host.
        runtime.api.debug_graph = false;
        runtime.api.trace_render_tree = false;
        runtime.api.auto_title_click = false;
        runtime.api.auto_user_click = false;
        runtime.api.title_compatibility_render_fallback = false;
        runtime.api.legacy_scenario_mode_input = false;
        runtime.api.call_coverage_enabled = false;
        let audio = if config.silent {
            None
        } else {
            Some(AudioSystem::new()?)
        };
        runtime.api.host.events.push_back(HostEvent {
            kind: 1,
            request_id: 0,
            code: 0,
            text: if config.silent {
                "explicit silent mode; MPEG is video-only; full SWF/WMV/BMV/MP4/IME/CDDA not supported"
            } else {
                "MPEG is video-only; full SWF/WMV/BMV/MP4/IME/CDDA not supported"
            }
            .into(),
        });
        Ok(Self {
            runtime,
            policy,
            _writer_lock: writer_lock,
            renderer: None,
            textures: BTreeMap::new(),
            font,
            audio,
            backend: config.backend,
            input_queue: VecDeque::new(),
            input_script: None,
            generation: 0,
            paused: 0,
            last_tick: None,
            tick_remainder_ns: 0,
            engine_frames: 0,
            closed: false,
            audio_suspended: false,
            vm_budget: host_vm_budget(host_physical_memory()),
        })
    }
    pub fn scope<T>(&mut self, f: impl FnOnce(&mut Self) -> T) -> T {
        let policy = self.policy.clone();
        let font = self.font.clone();
        file_policy::scope(&policy, || snapshot::with_font(&font, || f(self)))
    }
    fn sync_audio(&mut self) -> Result<()> {
        let suspended = self.paused != 0
            || self.renderer.is_none()
            || !self.runtime.api.host.pending.is_empty()
            || self.closed;
        if suspended != self.audio_suspended {
            if let Some(audio) = self.audio.as_mut() {
                if suspended {
                    audio.host_suspend()?;
                } else {
                    audio.host_resume()?;
                }
            }
            self.audio_suspended = suspended;
        }
        Ok(())
    }
    /// Caller retains the ANativeWindow (Android) or UIView (iOS) until
    /// detach/destroy has returned.
    #[cfg(any(target_os = "android", target_os = "ios"))]
    pub unsafe fn set_surface(
        &mut self,
        ptr: *mut std::ffi::c_void,
        w: u32,
        h: u32,
        generation: u64,
    ) -> Result<()> {
        if generation <= self.generation || ptr.is_null() || w == 0 || h == 0 {
            return Err(EthornellError::Other("invalid/stale surface".into()));
        }
        self.renderer = None;
        self.textures.clear();
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        #[cfg(target_os = "android")]
        let renderer = {
            let backend = match self.backend {
                1 => wgpu::Backends::VULKAN,
                2 => wgpu::Backends::GL,
                _ => wgpu::Backends::VULKAN | wgpu::Backends::GL,
            };
            pollster::block_on(unsafe {
                Renderer::new_android(ptr, w, h, backend, self.font.clone())
            })?
        };
        #[cfg(target_os = "ios")]
        let renderer =
            pollster::block_on(unsafe { Renderer::new_ios(ptr, w, h, self.font.clone()) })?;
        self.install_renderer(renderer, w, h, generation)
    }
    #[cfg(not(any(target_os = "android", target_os = "ios", target_arch = "wasm32")))]
    pub unsafe fn set_surface(
        &mut self,
        _: *mut std::ffi::c_void,
        _: u32,
        _: u32,
        _: u64,
    ) -> Result<()> {
        Err(EthornellError::Other(
            "no host surface on this target".into(),
        ))
    }
    /// Attach a host-created renderer. Web creates it asynchronously; native
    /// consoles supply their platform GPU callbacks and the session font.
    #[cfg(any(
        target_arch = "wasm32",
        feature = "console",
        target_os = "vita",
        target_os = "horizon"
    ))]
    pub fn attach_renderer(
        &mut self,
        renderer: Renderer<'static>,
        w: u32,
        h: u32,
        generation: u64,
    ) -> Result<()> {
        if generation <= self.generation || w == 0 || h == 0 {
            return Err(EthornellError::Other("invalid/stale surface".into()));
        }
        self.renderer = None;
        self.textures.clear();
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        self.install_renderer(renderer, w, h, generation)
    }
    /// Console chrome is submitted as ordinary native GPU draw commands.
    #[cfg(feature = "console")]
    pub fn show_console_notice(&mut self, text: Option<String>) -> Result<()> {
        if let Some(renderer) = self.renderer.as_mut() {
            renderer.set_notice(text);
            renderer.render()?;
        }
        Ok(())
    }
    /// The attached surface changed size (the web canvas follows the page).
    pub fn resize_surface(&mut self, w: u32, h: u32) -> Result<()> {
        if let Some(renderer) = self.renderer.as_mut() {
            renderer.try_resize(w, h)?;
            self.runtime.api.window_surface_width = w.min(i32::MAX as u32) as i32;
            self.runtime.api.window_surface_height = h.min(i32::MAX as u32) as i32;
        }
        Ok(())
    }
    /// One line of state for host-side debugging: the last VM stop, the
    /// engine clock and what the presenter has to draw.
    pub fn diagnostics(&self) -> String {
        let report = self.runtime.last_report.as_ref();
        format!(
            "stop={:?} pc={} steps={} time_ms={} screen={}x{} surfaces={} textures={} renderer={} pending={} program={:?} vm={:?} image_bytes={} blob_bytes={} program_cache={} cached_instructions={} vm_programs={} vm_instructions={} movies={:?} engine_frames={}",
            report.map(|r| r.stop_reason),
            report.map_or(0, |r| r.pc),
            report.map_or(0, |r| r.steps),
            self.runtime.api.engine_time_ms,
            self.runtime.api.screen_width,
            self.runtime.api.screen_height,
            self.runtime.api.graph_surfaces.len(),
            self.textures.len(),
            self.renderer.is_some(),
            self.runtime.api.host.pending.len(),
            report.map(|r| r.program.as_str()),
            self.runtime.vm.resource_usage(),
            self.runtime
                .api
                .graph_images
                .values()
                .map(|i| i.rgba.len())
                .sum::<usize>(),
            self.runtime
                .api
                .graph_blob_cache
                .values()
                .map(Vec::len)
                .sum::<usize>(),
            self.runtime.api.program_cache.len(),
            self.runtime
                .api
                .program_cache
                .values()
                .map(|p| p.instructions.len())
                .sum::<usize>(),
            self.runtime.vm.programs.len(),
            self.runtime
                .vm
                .programs
                .iter()
                .map(|p| p.instructions.len())
                .sum::<usize>(),
            self.runtime.api.graph_effects.movie_diagnostics(),
            self.engine_frames,
        )
    }
    /// The game text font, for renderers the host builds itself.
    pub fn font(&self) -> FontArc {
        self.font.clone()
    }
    #[cfg(any(
        target_os = "android",
        target_os = "ios",
        target_arch = "wasm32",
        feature = "console",
        target_os = "vita",
        target_os = "horizon"
    ))]
    fn install_renderer(
        &mut self,
        mut renderer: Renderer<'static>,
        w: u32,
        h: u32,
        generation: u64,
    ) -> Result<()> {
        renderer.set_virtual_size(
            self.runtime.api.screen_width.max(1) as f32,
            self.runtime.api.screen_height.max(1) as f32,
        );
        self.runtime.api.window_surface_width = w.min(i32::MAX as u32) as i32;
        self.runtime.api.window_surface_height = h.min(i32::MAX as u32) as i32;
        let should_suspend =
            self.paused != 0 || !self.runtime.api.host.pending.is_empty() || self.closed;
        let audio = self.audio.as_mut();
        let audio_suspended = &mut self.audio_suspended;
        commit_surface_candidate(&mut self.renderer, renderer, || {
            if *audio_suspended != should_suspend {
                if let Some(audio) = audio {
                    if should_suspend {
                        audio.host_suspend()?;
                    } else {
                        audio.host_resume()?;
                    }
                }
                *audio_suspended = should_suspend;
            }
            Ok(())
        })?;
        self.generation = generation;
        Ok(())
    }
    pub fn detach_surface(&mut self, generation: u64) -> Result<()> {
        if generation == self.generation {
            self.renderer = None;
            self.textures.clear();
            self.clear_input();
            self.last_tick = None;
            self.tick_remainder_ns = 0;
        }
        self.sync_audio()
    }
    pub fn pause(&mut self, mask: u32) -> Result<()> {
        self.paused = mask;
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        if mask != 0 {
            self.clear_input();
        }
        self.sync_audio()
    }
    pub fn clear_input(&mut self) {
        self.input_queue.clear();
        self.runtime.api.input_physically_held.clear();
        self.runtime.api.input_event_counts.clear();
        self.runtime.api.input_descriptor_accumulators.clear();
        self.runtime.api.window_messages.clear();
        self.runtime.api.pending_input_state = None;
        self.runtime.api.pending_input_descriptor = None;
        self.runtime.api.pending_click = None;
        self.runtime.api.pending_object_state = None;
        self.runtime.api.input_down_descriptors.clear();
        self.runtime.api.input_down_reported.clear();
        self.runtime.api.mouse_pressed = false;
        self.runtime.api.last_button_press = None;
        self.runtime.api.suppress_message_mouse_release = false;
        for input in self.runtime.api.graph_input_objects.values_mut() {
            input.clear_deferred_pointer_activation();
        }
        self.runtime.api.finish_graph_knob_drag();
    }
    /// Plays an `ETHORNELL_INPUT_SCRIPT` script (`wait:N,click:x:y,key:enter,...`,
    /// game coordinates, one step per engine frame) for unattended runs on
    /// hosts without environment variables.
    pub fn set_input_script(&mut self, source: &str) -> Result<()> {
        self.input_script =
            Some(HeadlessInputScript::parse(source).map_err(EthornellError::Other)?);
        Ok(())
    }
    pub fn input(&mut self, kind: u32, x: f32, y: f32, code: i32, value: f32) -> Result<()> {
        if kind == 7 {
            self.clear_input();
            return Ok(());
        }
        // The drawn pointer follows the stick even while input is held back.
        #[cfg(feature = "console")]
        if kind == 1 {
            if let Some(renderer) = self.renderer.as_mut() {
                renderer.set_cursor(x, y);
            }
        }
        if self.closed || self.paused != 0 || !self.runtime.api.host.pending.is_empty() {
            return Ok(());
        }
        let Some(renderer) = self.renderer.as_ref() else {
            return Ok(());
        };
        let point = renderer.surface_to_game_point(x, y);
        if kind == 3 && point.is_none() {
            // CANCEL, not a normal UP at the previous in-content coordinate:
            // an outside release must never activate a deferred button click.
            self.clear_input();
            return Ok(());
        }
        let event = match kind {
            1 => point.map(|(x, y)| RuntimeInputEvent::MouseMove { x, y }),
            2 | 3 => {
                // An outside release still clears a held button, but an outside
                // press never maps a black bar onto game content.
                let p = point;
                p.map(|(x, y)| match (kind, code) {
                    (2, 1) => RuntimeInputEvent::MousePress { x, y },
                    (3, 1) => RuntimeInputEvent::MouseRelease { x, y },
                    (2, _) => RuntimeInputEvent::ButtonPress { button: code, x, y },
                    _ => RuntimeInputEvent::ButtonRelease { button: code, x, y },
                })
            }
            4 => Some(RuntimeInputEvent::KeyPress { descriptor: code }),
            5 => Some(RuntimeInputEvent::KeyRelease { descriptor: code }),
            6 => Some(RuntimeInputEvent::MouseWheel { delta_y: value }),
            _ => return Err(EthornellError::Other("unknown input kind".into())),
        };
        // The engine-drawn UserB0:A0 dialog takes its own clicks, as on desktop.
        let event = event.filter(|event| match *event {
            RuntimeInputEvent::MousePress { x, y } => {
                !self.runtime.api.handle_user_modeless_pointer(x, y, true)
            }
            RuntimeInputEvent::MouseRelease { x, y } => {
                !self.runtime.api.handle_user_modeless_pointer(x, y, false)
            }
            _ => true,
        });
        // Deliver edges in FIFO call order, including down/up in one frame.
        if let Some(event) = event {
            if self.input_queue.len() >= 4096 {
                return Err(EthornellError::Other(
                    "input queue capacity exceeded".into(),
                ));
            }
            queue_runtime_input_event(&mut self.input_queue, event);
        }
        Ok(())
    }
    pub fn step(&mut self, now: u64) -> Result<()> {
        self.scope(|this| {
            this.step_inner(now).map_err(|error| {
                let report = this.runtime.last_report.as_ref();
                let reason = report
                    .map(|r| match r.stop_reason {
                        ethornell_vm::VmStopReason::Completed => 1,
                        ethornell_vm::VmStopReason::Yielded => 2,
                        ethornell_vm::VmStopReason::SwitchedThread => 3,
                        ethornell_vm::VmStopReason::SchedulerPassEnded => 4,
                        ethornell_vm::VmStopReason::InterpreterTerminated => 5,
                        ethornell_vm::VmStopReason::WaitingForInput => 6,
                        ethornell_vm::VmStopReason::WaitingForInputOrTime => 7,
                        ethornell_vm::VmStopReason::WaitingForTime => 8,
                        ethornell_vm::VmStopReason::WaitingForProcedure => 9,
                        ethornell_vm::VmStopReason::WaitingForHost => 10,
                        ethornell_vm::VmStopReason::QuantumExhausted => 11,
                        ethornell_vm::VmStopReason::WatchdogExceeded => 12,
                        ethornell_vm::VmStopReason::UnknownOpcode => 13,
                        ethornell_vm::VmStopReason::UnknownDispatch => 14,
                        ethornell_vm::VmStopReason::Error => 15,
                    })
                    .unwrap_or(0);
                let opcode = this.runtime.api.last_native_call;
                let cause = if error.to_string().contains("VM resource budget exceeded") {
                    7
                } else {
                    report
                        .and_then(|r| {
                            r.recent_trace.iter().rev().find(|line| {
                                line.starts_with("error:")
                                    || line.starts_with("host response failed:")
                            })
                        })
                        .map(|line| {
                            if line.contains("unsupported") {
                                1
                            } else if line.contains("not found") || line.contains("missing") {
                                2
                            } else if line.contains("memory access out of bounds") {
                                3
                            } else if line.contains("stack underflow") {
                                4
                            } else if line.contains("font") || line.contains("glyph") {
                                5
                            } else if line.contains("audio") || line.contains("sound") {
                                6
                            } else if line.contains("budget") || line.contains("limit") {
                                7
                            } else {
                                8
                            }
                        })
                        .unwrap_or(0)
                };
                let detail = report.and_then(|r| {
                    r.recent_trace.iter().rev().find(|line| {
                        line.starts_with("error:") || line.starts_with("host response failed:")
                    })
                });
                EthornellError::Other(format!(
                    "{error}; {}host_runtime_diag:{reason}:{}:{}:{}:{}:{cause}",
                    detail.map(|line| format!("{line}; ")).unwrap_or_default(),
                    report.map(|r| r.pc as u64).unwrap_or(u64::MAX),
                    report
                        .and_then(|r| r.offset)
                        .map(u64::from)
                        .unwrap_or(u64::MAX),
                    opcode.map(|o| u32::from(o.group)).unwrap_or(u32::MAX),
                    opcode.map(|o| u32::from(o.id)).unwrap_or(u32::MAX)
                ))
            })
        })
    }
    fn step_inner(&mut self, now: u64) -> Result<()> {
        self.sync_audio()?;
        if self.closed || self.audio_suspended {
            self.last_tick = None;
            self.tick_remainder_ns = 0;
            return Ok(());
        }
        let Some(last) = self.last_tick else {
            self.last_tick = Some(now);
            return Ok(());
        };
        if now < last {
            self.last_tick = Some(now);
            return Err(EthornellError::Other("non-monotonic host clock".into()));
        }
        if now - last < 16_000_000 {
            return Ok(());
        }
        self.engine_frames += 1;
        self.last_tick = Some(now);
        let raw_elapsed = elapsed_ms_with_carry(now - last, &mut self.tick_remainder_ns);
        let elapsed = raw_elapsed;
        #[cfg(feature = "console")]
        let elapsed =
            timing::console_engine_elapsed_ms(elapsed, self.runtime.vm.clock_jump_threshold_ms());
        self.runtime
            .vm
            .check_resource_budget(self.vm_budget)
            .map_err(|e| EthornellError::Other(e.to_string()))?;
        let audio_required = self.audio.is_some();
        let report = runtime_frontend::drive_runtime_frame_checked(
            &mut self.runtime,
            &mut self.input_queue,
            &mut self.input_script,
            RuntimeFramePacing,
            elapsed,
            raw_elapsed,
            self.audio.as_mut(),
            "android",
            audio_required,
        )?;
        self.runtime
            .vm
            .check_resource_budget(self.vm_budget)
            .map_err(|e| EthornellError::Other(e.to_string()))?;
        if let Some(error) = snapshot::take_raster_error() {
            return Err(EthornellError::Other(error));
        }
        if snapshot::take_missing_glyphs() {
            return Err(EthornellError::Other(
                "font lacks required CPU bitmap/text glyphs".into(),
            ));
        }
        if let Some(message) = self.runtime.api.host.fatal.take() {
            return Err(EthornellError::Other(message));
        }
        if report
            .last_report
            .as_ref()
            .is_some_and(|r| r.stop_reason.is_fatal())
        {
            return Err(EthornellError::Other(format!(
                "VM stopped: {:?}",
                report.last_report.as_ref().unwrap().stop_reason
            )));
        }
        if let Some(title) = self.runtime.api.pending_window_title.take() {
            self.runtime.api.host.events.push_back(HostEvent {
                kind: 4,
                request_id: 0,
                code: 0,
                text: title,
            });
        }
        if self.runtime.api.quit_requested {
            self.close();
            return Ok(());
        }
        if std::mem::take(&mut self.runtime.api.pending_window_close_request) {
            self.request_close()?;
        }
        let Some(renderer) = self.renderer.as_mut() else {
            return Ok(());
        };
        renderer.check_gpu()?;
        renderer.set_virtual_size(
            self.runtime.api.screen_width.max(1) as f32,
            self.runtime.api.screen_height.max(1) as f32,
        );
        // sub_462100: present a newly composed frame only when one is due.
        if self.runtime.api.take_present_due() {
            self.runtime.api.performance_profile.begin();
            let commands =
                frame_presenter::prepare_frame(renderer, &self.runtime.api, &mut self.textures)?;
            renderer.submit(&commands);
            self.runtime.api.performance_profile.end(true);
        }
        renderer.render()?;
        self.runtime.api.presentation_state =
            self.runtime
                .api
                .clock_base_ms
                .wrapping_add(self.runtime.api.engine_time_ms as u32) as i32;
        self.sync_audio()
    }
    fn close(&mut self) {
        self.closed = true;
        self.runtime.vm.cancel_host_requests();
        self.runtime.api.host.pending.clear();
        self.runtime.api.host.dialogs.clear();
        self.runtime.api.host.retries.clear();
        self.runtime.api.host.events.push_back(HostEvent {
            kind: 3,
            request_id: 0,
            code: 0,
            text: String::new(),
        });
    }
    pub fn request_close(&mut self) -> Result<()> {
        if self.closed {
            return Ok(());
        }
        // Preserve WM_CLOSE semantics: mode zero queues native event 2 so the
        // game's own confirmation procedure can run; nonzero allows close.
        // A blocked host request can be cancelled by explicit host destruction,
        // but must never receive a fabricated answer from a close gesture.
        if self.runtime.api.handle_host_window_close("android") {
            self.close();
        }
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        self.sync_audio()
    }
    pub fn poll_event(&mut self) -> Option<HostEvent> {
        self.runtime.api.host.events.pop_front()
    }
    /// The JSON description of a kind-5 dialog request.
    pub fn request_payload(&self, id: u64) -> Option<String> {
        self.runtime
            .api
            .host
            .dialogs
            .get(&id)
            .map(super::dialog_protocol::request_json)
    }
    /// Answer a kind-5 dialog request with its JSON response.
    pub fn complete_dialog(&mut self, id: u64, response: &str) -> Result<()> {
        let request = self
            .runtime
            .api
            .host
            .dialogs
            .get(&id)
            .ok_or_else(|| EthornellError::Other("unknown/stale request".into()))?;
        let response = super::dialog_protocol::response_from_json(request, response)
            .map_err(EthornellError::Other)?;
        self.runtime
            .vm
            .complete_user_dialog(id, response)
            .map_err(|e| EthornellError::Other(e.to_string()))?;
        self.runtime.api.host.dialogs.remove(&id);
        self.runtime.api.host.pending.remove(&id);
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        self.clear_input();
        self.sync_audio()
    }
    pub fn complete_request(&mut self, id: u64, result: i32) -> Result<()> {
        if self.runtime.api.host.dialogs.contains_key(&id) {
            return Err(EthornellError::Other(
                "dialog request needs a JSON response".into(),
            ));
        }
        let outputs = *self
            .runtime
            .api
            .host
            .pending
            .get(&id)
            .ok_or_else(|| EthornellError::Other("unknown/stale request".into()))?;
        let mut result = result;
        if let Some((message, file)) = self.runtime.api.host.retries.get(&id).cloned() {
            if result != 0 && !self.runtime.api.required_file_present(&file) {
                self.runtime.api.host.events.push_back(HostEvent {
                    kind: 2,
                    request_id: id,
                    code: 1,
                    text: message,
                });
                return Ok(());
            }
            result = i32::from(result != 0);
            self.runtime.api.host.retries.remove(&id);
        }
        self.runtime
            .vm
            .complete_host_request(
                id,
                ethornell_vm::HostResponse {
                    outputs: if outputs == 0 {
                        vec![]
                    } else {
                        vec![ethornell_vm::Value::Int(result)]
                    },
                    writes: vec![],
                },
            )
            .map_err(|e| EthornellError::Other(e.to_string()))?;
        self.runtime.api.host.pending.remove(&id);
        self.last_tick = None;
        self.tick_remainder_ns = 0;
        self.clear_input();
        self.sync_audio()
    }
}

impl RuntimeEngine {
    fn new_explicit(
        script: &str,
        bytes: &[u8],
        manager: ResourceManager,
        root: PathBuf,
        trace: bool,
        strict: bool,
        encoding: &'static encoding_rs::Encoding,
    ) -> Self {
        let program = ethornell_script::parse_bp_program(Some(script.into()), bytes);
        let mut vm = ethornell_vm::Vm::new();
        vm.set_native_abi_profile(confirmed_native_profile(&manager, bytes));
        vm.set_graph_text_encoding(encoding);
        vm.start(&program);
        Self {
            vm,
            api: RuntimeTraceApi::new_with_native_root(manager, root),
            options: ethornell_vm::VmRunOptions {
                max_steps: ethornell_vm::DEFAULT_COOPERATIVE_WATCHDOG_STEPS,
                trace,
                fail_on_stub: strict,
                collect_diagnostics: true,
            },
            last_report: None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{HostRequest, HostResponse, SysApi, Value, Vm, VmRunOptions, VmStopReason};
    fn api() -> RuntimeTraceApi {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.host.enabled = true;
        api
    }
    #[test]
    fn user_dialog_requests_become_kind_five_events_with_json_payloads() {
        let mut api = api();
        let request = ethornell_vm::UserDialogRequest::Selection {
            title: "Route".into(),
            prompt: "Pick one".into(),
            options: vec!["A".into(), "B".into()],
        };
        let id = api
            .begin_host_request(&HostRequest::UserDialog(request.clone()))
            .unwrap()
            .unwrap();
        let event = api.host.events.pop_front().unwrap();
        assert_eq!(
            (event.kind, event.request_id, event.text.as_str()),
            (5, id, "select")
        );
        assert_eq!(api.host.pending.get(&id), Some(&1));
        let payload: serde_json::Value = serde_json::from_str(
            &super::super::dialog_protocol::request_json(&api.host.dialogs[&id]),
        )
        .unwrap();
        assert_eq!(payload["options"][1], "B");
    }

    #[test]
    fn input_route_trace_is_opt_in_numeric_bounded_and_does_not_enable_vm_trace() {
        let mut h = HostServices::default();
        h.enabled = true;
        h.input_route([u32::MAX; 7], [true; 5]);
        assert!(h.events.is_empty());
        h.input_trace = true;
        for _ in 0..100 {
            h.input_route([u32::MAX; 7], [true; 5]);
        }
        assert_eq!(h.events.len(), 64);
        for e in &h.events {
            assert_eq!((e.kind, e.code, e.request_id), (1, 1001, 0));
            assert!(e.text.len() <= 384);
            assert!(e.text.starts_with("input_route object=4294967295 group="));
        }
        h.input_gate([u32::MAX; 9]);
        assert_eq!(h.events.len(), 64);
        let mut gate = HostServices::default();
        gate.enabled = true;
        gate.input_trace = true;
        gate.input_gate([u32::MAX; 9]);
        assert_eq!(gate.events[0].code, 1002);
        assert!(gate.events[0].text.len() <= 384);
    }
    #[test]
    fn measured_workload_has_bounded_headroom_only_on_eligible_device_profile() {
        let low = host_vm_budget(None);
        let small = host_vm_budget(Some(3 * 1024 * 1024 * 1024));
        let normal = host_vm_budget(Some(4 * 1024 * 1024 * 1024));
        assert_eq!(low.max_memory_capacity_bytes, 512 * 1024 * 1024);
        assert_eq!(
            small.max_memory_capacity_bytes,
            low.max_memory_capacity_bytes
        );
        assert_eq!(normal.max_memory_capacity_bytes, 768 * 1024 * 1024);
        assert_eq!(
            host_vm_budget(Some(u64::MAX)).max_memory_capacity_bytes,
            normal.max_memory_capacity_bytes
        );
        assert!(normal.max_memory_capacity_bytes > 648_441_292);
        assert!(low.max_memory_capacity_bytes < 648_441_292);
        assert_eq!(normal.max_vm_count, 64);
        let vm = Vm::new();
        let usage = vm.resource_usage();
        let capacity = usage.memory_capacity_bytes;
        assert!(
            vm.check_resource_budget(ethornell_vm::VmResourceBudget {
                max_vm_count: 64,
                max_memory_capacity_bytes: capacity - 1
            })
            .is_err()
        );
        assert!(
            vm.check_resource_budget(ethornell_vm::VmResourceBudget {
                max_vm_count: 64,
                max_memory_capacity_bytes: capacity
            })
            .is_ok()
        );
        assert!(
            vm.check_resource_budget(ethornell_vm::VmResourceBudget {
                max_vm_count: 0,
                ..normal
            })
            .is_err()
        );
    }
    #[test]
    #[ignore = "requires an authorized read-only sample via ETHORNELL_RESTRICTED_REPRO_GAME; private temporary user root"]
    #[cfg(unix)]
    fn authorized_sample_restricted_vm_baseline() {
        if std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some() {
            ethornell_core::init_tracing();
        }
        let resource = PathBuf::from(
            std::env::var_os("ETHORNELL_RESTRICTED_REPRO_GAME")
                .expect("explicit authorized sample required"),
        );
        let user = std::env::temp_dir().join(format!(
            "ethornell-restricted-repro-user-{}",
            std::process::id()
        ));
        std::fs::create_dir(&user).unwrap();
        let outcome = (|| -> Result<()> {
            let mut host = HostRuntime::create(HostConfig {
                resource_root: resource,
                user_root: user.clone(),
                font_path: None,
                font_data: None,
                font_face_index: 0,
                gbk: true,
                backend: 0,
                strict: false,
                trace: false,
                silent: true,
            })?;
            // CPU-only VM/host-policy reproduction, not a Surface/GPU test.
            // It invokes the exact shared checked frontend under the same
            // scoped policy/font, with an explicit silent audio-free backend.
            host.scope(|host| -> Result<()> {
                let mut peak_capacity=0;let mut peak_vms=0;
                let mut input=HeadlessInputScript::from_env();
                let measure_only=std::env::var_os("ETHORNELL_RESTRICTED_REPRO_MEASURE").is_some();
                let frames=std::env::var("ETHORNELL_RESTRICTED_REPRO_FRAMES").ok().and_then(|v|v.parse::<usize>().ok()).unwrap_or(1200).min(5000);
                for frame in 0..frames {
                    if frame==1950 && std::env::var_os("ETHORNELL_RESTRICTED_INVOKE_SAVE").is_some(){
                        let bytes=host.runtime.api.manager.read_decoded_from_archive("sysprg.arc",if std::env::var_os("ETHORNELL_RESTRICTED_INVOKE_ENCODE").is_some(){"encdusdt._bp"}else{"usdtwnd._bp"})?;
                        let p=ethornell_script::parse_bp_program(Some("local-only-direct-usdtwnd".into()),&bytes);
                        host.runtime.vm.diagnostic_start_with_initialized_framework(&p);host.runtime.vm.stack=vec![ethornell_vm::Value::Int(0),ethornell_vm::Value::Int(1)];
                    }
                    if frame==2010 && std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some(){for (&id,obj) in &host.runtime.api.graph_input_objects {eprintln!("local-only INPUT object={id} layer={} running={} desc={:?} hover={:?} savehit={:?}",obj.layer,obj.is_running(),obj.descriptor,obj.hovered,host.runtime.api.hit_test_graph_input_object(id,(650.0,705.0)));}}
                    if frame==2010 && std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some(){for (&id,obj) in &host.runtime.api.graph_input_objects {eprintln!("local-only SURFACE object={id} world={:?} surface={:?}",host.runtime.api.surface_world_position(obj.layer),host.runtime.api.graph_surfaces.get(&obj.layer));for y in (550..720).step_by(10){for x in (0..1280).step_by(20){if let Some((r,_,_))=host.runtime.api.hit_test_graph_input_object(id,(x as f32,y as f32)){if r.index==0 {eprintln!("local-only SAVEHIT {id} x={x} y={y}");}}}}}}
                    if frame==2010 && std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some(){for (&id,obj) in &host.runtime.api.graph_input_objects {for (&lid,layer) in &host.runtime.api.graph_layers {if layer.target_surface==Some(obj.layer) && host.runtime.api.surface_controls.contains_layer_for_owner(SurfaceControlOwner::InputObject(id),lid) {eprintln!("local-only INPUTLAYER obj={id} layer={lid} hit={} xywh=({},{} {}x{}) transform={:?} enabled={} key={}",layer.hit_id,layer.x,layer.y,layer.width,layer.height,host.runtime.api.layer_world_transform(lid,layer),layer.enabled,layer.key);}}}}
                    if frame==2010 && std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some(){for (&id,obj) in &host.runtime.api.graph_input_objects {if let Some(r)=obj.descriptor.regions.first(){eprintln!("local-only HITFACT obj={id} eligible={} mask={} visible={} configure_size={:?}",obj.pointer_hit_eligible(*r),host.runtime.api.graph_input_item_mask_accepts(id,*r,22,14),host.runtime.api.graph_surfaces.get(&obj.layer).is_some_and(|s|host.runtime.api.surface_display_chain_visible(obj.layer,s)),host.runtime.api.resource_image_region(native_surface_control_configure_resource(r)).map(|(_,r)|(r.width,r.height)));}}}
                    if frame==2010 && std::env::var_os("ETHORNELL_RESTRICTED_TRACE").is_some(){for (&id,obj) in &host.runtime.api.graph_input_objects {eprintln!("local-only VISFACT obj={id} drawable={} opacity={} props={:?}",host.runtime.api.display_tree.chain_drawable(obj.layer),host.runtime.api.surface_display_opacity(obj.layer),host.runtime.api.graph_object_properties.get(&obj.layer));}}
                    let budget=if measure_only {ethornell_vm::VmResourceBudget {max_vm_count:64,max_memory_capacity_bytes:2*1024*1024*1024}} else {host.vm_budget};
                    host.runtime.vm.check_resource_budget(budget).map_err(|_|EthornellError::Other(format!("restricted budget failed before frame={frame} usage={:?}",host.runtime.vm.resource_usage())))?;
                    let report = runtime_frontend::drive_runtime_frame_checked(&mut host.runtime,&mut host.input_queue,&mut input,RuntimeFramePacing,16,16,None,"restricted-repro",false)?;
                    host.runtime.vm.check_resource_budget(budget).map_err(|_|EthornellError::Other(format!("restricted budget failed after frame={frame} root_memory={} usage={:?} tasks={:#?}",host.runtime.vm.memory.len(),host.runtime.vm.resource_usage(),host.runtime.vm.task_memory_breakdown())))?;
                    let usage=host.runtime.vm.resource_usage();peak_capacity=peak_capacity.max(usage.memory_capacity_bytes);peak_vms=peak_vms.max(usage.vm_count);
                    if let Some(message)=host.runtime.api.host.fatal.take() {return Err(EthornellError::Other(format!("restricted host failure at frame={frame}; opcode={:?}; category={}",host.runtime.api.last_native_call,if message.contains("missing BP program") {"missing_program"} else {"host_denied"})));}
                    if let Some(report)=report.last_report.as_ref().filter(|r|r.stop_reason.is_fatal()) {
                        let unsupported=report.recent_trace.iter().any(|t|t.contains("unsupported"));
                        return Err(EthornellError::Other(format!("restricted VM fatal frame={frame} stop={:?} pc={} offset={:?} opcode={:?} unsupported={unsupported} local_trace={:?}",report.stop_reason,report.pc,report.offset,host.runtime.api.last_native_call,report.recent_trace.iter().rev().take(2).collect::<Vec<_>>())));
                    }
                    if let Some(error)=snapshot::take_raster_error() {return Err(EthornellError::Other(error));}
                    if snapshot::take_missing_glyphs() {return Err(EthornellError::Other("font missing glyph in restricted sample".into()));}
                    if host.runtime.api.quit_requested {return Err(EthornellError::Other(format!("sample requested exit frame={frame}")));}
                    if let Some(event)=host.runtime.api.host.events.iter().find(|e|e.kind==2) {
                        let save_failure=event.text.contains("保存") || event.text.contains("書き込み");
                        return Err(EthornellError::Other(format!("restricted sample blocked host_request frame={frame} save_failure={save_failure} opcode={:?} user_files={:?}",host.runtime.api.last_native_call,host.policy.enumerate("*",true,0,false).unwrap_or_default())));
                    }
                    if frame%300==0 {if let Some(r)=report.last_report.as_ref(){eprintln!("local-only restricted frame={frame} program={} pc={} stop={:?} procedure={:?} native={:?} usage={:?}",r.program,r.pc,r.stop_reason,r.thread.current_procedure,host.runtime.api.last_native_call,host.runtime.vm.resource_usage());}}
                }
                let files=host.policy.enumerate("*",true,0,false)?;
                let saved=files.iter().find(|name|name.to_ascii_lowercase().ends_with("amairochocolate101.sud")).ok_or_else(||EthornellError::Other(format!("restricted sample produced no expected user save; user files={files:?}")))?;
                let bytes=host.policy.read_user(saved)?;
                if bytes.is_empty() {return Err(EthornellError::Other("restricted sample save is empty".into()));}
                if let Some(path)=std::env::var_os("ETHORNELL_RESTRICTED_REPRO_SNAPSHOT") {snapshot::write_runtime_snapshot(&host.runtime.api,Path::new(&path))?;}
                eprintln!("restricted sample saved_bytes={} title_active={}",bytes.len(),host.runtime.api.title_ui_active);
                eprintln!("restricted sample peak_vm_capacity_bytes={peak_capacity} peak_vm_count={peak_vms} measure_only={measure_only}");
                Ok(())
            })
        })();
        // Remove only this test's private output; never mutate source sample.
        std::fs::remove_dir_all(&user).unwrap();
        outcome.unwrap();
    }
    #[test]
    fn surface_candidate_audio_failure_releases_borrow_before_error_returns() {
        struct Borrow(std::rc::Rc<std::cell::Cell<bool>>);
        impl Drop for Borrow {
            fn drop(&mut self) {
                self.0.set(true);
            }
        }
        let dropped = std::rc::Rc::new(std::cell::Cell::new(false));
        let mut slot = None;
        let result = commit_surface_candidate(&mut slot, Borrow(dropped.clone()), || {
            Err(EthornellError::Other("injected audio queue failure".into()))
        });
        assert!(result.is_err());
        assert!(slot.is_none());
        assert!(dropped.get());
    }
    #[test]
    fn display_refresh_elapsed_carries_submilliseconds_without_extra_vm_passes() {
        for hz in [60u64, 90, 120] {
            let mut last = 0;
            let mut sum = 0;
            let mut carry = 0;
            let mut passes = 0;
            for frame in 1..=hz * 10 {
                let now = frame * 1_000_000_000 / hz;
                if now - last < 16_000_000 {
                    continue;
                }
                sum += elapsed_ms_with_carry(now - last, &mut carry);
                passes += 1;
                last = now;
            }
            assert_eq!(sum, last / 1_000_000);
            assert!(passes <= 600);
            assert!(carry < 1_000_000);
        }
        assert_eq!(normalize_engine_elapsed_ms(501, 500), 0);
    }
    #[test]
    fn native_host_routing_is_group_specific_and_nonhost_queries_fall_through() {
        let mut api = api();
        for (group, id) in [(0x80, 0xFD), (0x81, 0x3C), (0x90, 0x11), (0xB0, 0x83)] {
            assert_eq!(
                api.begin_host_request(&HostRequest::Native {
                    opcode: ethornell_vm::NativeOpcode { group, id },
                    args: vec![]
                })
                .unwrap(),
                None
            );
        }
        for (group, id) in [
            (0x80, 0x3B),
            (0x80, 0xE1),
            (0x80, 0xF2),
            (0x81, 0x38),
            (0xB0, 0xF0),
            (0xB0, 0x84),
        ] {
            assert!(
                api.begin_host_request(&HostRequest::Native {
                    opcode: ethornell_vm::NativeOpcode { group, id },
                    args: vec![]
                })
                .is_err()
            );
        }
        for id in [0x80, 0x81, 0x82] {
            let request = HostRequest::Native {
                opcode: ethornell_vm::NativeOpcode { group: 0xB0, id },
                args: vec![Value::Str("normalized SJIS string".into()), Value::Int(0)],
            };
            let token = api.begin_host_request(&request).unwrap().unwrap();
            assert_eq!(api.host.pending[&token], usize::from(id != 0x80));
            assert_eq!(
                api.host.events.pop_front().unwrap().text,
                "normalized SJIS string"
            );
        }
    }
    #[test]
    #[cfg(unix)]
    fn required_file_prompt_rechecks_on_retry_and_skips_when_present() {
        let (root, mut host) = minimal_host("retry");
        let program = ethornell_script::parse_bp_program(Some("retry".into()), &[0x80, 0x3C]);
        let run = |host: &mut HostRuntime, file: &str| {
            host.runtime.vm.start(&program);
            host.runtime.vm.stack = vec![
                Value::Str(file.into()),
                Value::Str("unused".into()),
                Value::Str("insert disc".into()),
            ];
            host.runtime
                .vm
                .run_loaded(&mut host.runtime.api, &VmRunOptions::default())
        };
        while host.poll_event().is_some() {}
        assert_ne!(
            run(&mut host, "system.arc").stop_reason,
            VmStopReason::WaitingForHost
        );
        let event = host.poll_event();
        assert!(event.is_none(), "{event:?}");
        assert_eq!(host.runtime.vm.stack, vec![Value::Int(1)]);

        assert_eq!(
            run(&mut host, "late.dat").stop_reason,
            VmStopReason::WaitingForHost
        );
        let event = host.poll_event().unwrap();
        assert_eq!(
            (event.kind, event.code, event.text.as_str()),
            (2, 1, "insert disc")
        );
        host.complete_request(event.request_id, 1).unwrap();
        let again = host.poll_event().unwrap();
        assert_eq!((again.kind, again.request_id), (2, event.request_id));
        std::fs::write(root.join("resource/late.dat"), b"x").unwrap();
        host.complete_request(event.request_id, 1).unwrap();
        host.runtime
            .vm
            .run_loaded(&mut host.runtime.api, &VmRunOptions::default());
        assert_eq!(host.runtime.vm.stack, vec![Value::Int(1)]);

        assert_eq!(
            run(&mut host, "never.dat").stop_reason,
            VmStopReason::WaitingForHost
        );
        let event = host.poll_event().unwrap();
        host.complete_request(event.request_id, 0).unwrap();
        host.runtime
            .vm
            .run_loaded(&mut host.runtime.api, &VmRunOptions::default());
        assert_eq!(host.runtime.vm.stack, vec![Value::Int(0)]);
        drop(host);
        std::fs::remove_dir_all(root).unwrap();
    }
    #[cfg(unix)]
    fn minimal_host(label: &str) -> (PathBuf, HostRuntime) {
        static N: AtomicU64 = AtomicU64::new(1);
        let root = std::env::temp_dir().join(format!(
            "ethornell-host-{}-{label}-{}",
            std::process::id(),
            N.fetch_add(1, Ordering::Relaxed)
        ));
        let resource = root.join("resource");
        let user = root.join("user");
        std::fs::create_dir_all(&resource).unwrap();
        std::fs::create_dir_all(&user).unwrap();
        let mut arc = b"BURIKO ARC20".to_vec();
        arc.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 0x60];
        name[..7].copy_from_slice(b"ipl._bp");
        arc.extend_from_slice(&name);
        arc.extend_from_slice(&0u32.to_le_bytes());
        arc.extend_from_slice(&1u32.to_le_bytes());
        arc.extend_from_slice(&[0; 24]);
        arc.push(0x17);
        std::fs::write(resource.join("system.arc"), arc).unwrap();
        let runtime = HostRuntime::create(HostConfig {
            resource_root: resource,
            user_root: user,
            font_path: None,
            font_data: None,
            font_face_index: 0,
            gbk: true,
            backend: 0,
            strict: false,
            trace: false,
            silent: true,
        })
        .unwrap();
        (root, runtime)
    }
    #[test]
    #[cfg(unix)]
    fn bootstrap_restart_preserves_session_text_encoding() {
        let (root, mut host) = minimal_host("encoding-restart");
        assert_eq!(host.runtime.vm.graph_text_encoding(), encoding_rs::GBK);
        host.runtime.api.native_system.bootstrap_request = Some((
            None,
            "system.arc".into(),
            "ipl._bp".into(),
        ));
        let restarted = host.runtime.take_restart().unwrap();
        assert_eq!(restarted.vm.graph_text_encoding(), encoding_rs::GBK);
        drop(restarted);
        drop(host);
        std::fs::remove_dir_all(root).unwrap();
    }

    #[test]
    #[cfg(unix)]
    fn public_host_without_surface_halts_and_pause_cancel_clears_gesture_history() {
        let (root, mut host) = minimal_host("pause");
        let before = host.runtime.api.engine_time_ms;
        host.step(1_000_000).unwrap();
        host.step(100_000_000).unwrap();
        assert_eq!(host.runtime.api.engine_time_ms, before);
        assert!(!file_policy::restricted());
        host.runtime.api.last_button_press = Some((1, 0, 50.0, 50.0));
        host.runtime.api.suppress_message_mouse_release = true;
        host.input_queue
            .push_back(RuntimeInputEvent::MousePress { x: 50.0, y: 50.0 });
        host.pause(3).unwrap();
        assert!(host.runtime.api.last_button_press.is_none());
        assert!(!host.runtime.api.suppress_message_mouse_release);
        assert!(host.input_queue.is_empty());
        assert!(host.last_tick.is_none());
        assert_eq!(host.tick_remainder_ns, 0);
        assert!(host.complete_request(u64::MAX, 1).is_err());
        host.pause(1).unwrap();
        assert_eq!(host.paused, 1);
        host.pause(0).unwrap();
        assert!(host.audio_suspended);
        host.runtime.api.native_system.native_close_mode = 1;
        host.request_close().unwrap();
        assert!(host.closed);
        assert_eq!(host.poll_event().unwrap().kind, 1);
        assert_eq!(host.poll_event().unwrap().kind, 3);
        drop(host);
        std::fs::remove_dir_all(root).unwrap();
    }
    #[test]
    fn host_confirmation_is_a_real_vm_continuation_not_a_default_answer() {
        let mut api = api();
        let mut vm = Vm::new();
        let program = ethornell_script::parse_bp_program(Some("confirmation".into()), &[0x78]);
        vm.start(&program);
        vm.stack = vec![Value::Str("continue?".into()), Value::Int(0)];
        let report = vm.run_loaded(&mut api, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::WaitingForHost);
        assert!(vm.stack.is_empty());
        let event = api.host.events.pop_front().unwrap();
        assert_eq!(event.kind, 2);
        assert_eq!(event.text, "continue?");
        assert_eq!(vm.run_loaded(&mut api, &VmRunOptions::default()).steps, 0);
        vm.complete_host_request(
            event.request_id,
            HostResponse {
                outputs: vec![Value::Int(1)],
                writes: vec![],
            },
        )
        .unwrap();
        assert!(vm.stack.is_empty());
        assert!(
            vm.complete_host_request(event.request_id, HostResponse::default())
                .is_err()
        );
        vm.run_loaded(&mut api, &VmRunOptions::default());
        assert_eq!(vm.stack, vec![Value::Int(1)]);
    }
    #[test]
    fn host_request_ids_are_unique_across_sessions_and_unimplemented_dialogs_fail() {
        let mut a = api();
        let mut b = api();
        let request = HostRequest::Confirmation {
            text: "question".into(),
            default_yes: false,
        };
        let x = a.begin_host_request(&request).unwrap().unwrap();
        let y = b.begin_host_request(&request).unwrap().unwrap();
        assert_ne!(x, y);
        assert_ne!(x, 0);
        assert_ne!(y, 0);
        assert!(!a.allow_direct_filesystem_fallback());
        let chooser = HostRequest::Native {
            opcode: ethornell_vm::NativeOpcode {
                group: 0x80,
                id: 0x3B,
            },
            args: vec![],
        };
        assert!(a.begin_host_request(&chooser).is_err());
    }
    #[test]
    fn native_close_contract_is_not_replaced_by_unconditional_finish() {
        let mut api = api();
        api.native_system.native_close_mode = 0;
        assert!(!api.handle_host_window_close("android-test"));
        assert_eq!(api.queued_system_events.pop_front(), Some([2, 0, 0]));
        api.native_system.native_close_mode = 1;
        assert!(api.handle_host_window_close("android-test"));
    }
}
