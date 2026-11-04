use ethornell_app::hosted_files::{self, ResourceFiles, UserStore};
use ethornell_app::host_runtime::{HostConfig, HostRuntime};
use ethornell_archive::ArchiveSource;
use ethornell_core::clock::{self, WallTime};
use std::io::{self, Read, Seek, SeekFrom};
use std::sync::Arc;
use std::time::Duration;
use wasm_bindgen::prelude::*;

// Provided by worker.js on the worker's global scope.
#[wasm_bindgen]
extern "C" {
    /// Reads `out.len()` bytes of file `id` at `offset`; returns the count.
    #[wasm_bindgen(catch, js_name = ethornellReadAt)]
    fn js_read_at(id: u32, offset: f64, out: &mut [u8]) -> Result<u32, JsValue>;
    /// Stores a user-data file (a save) for this game.
    #[wasm_bindgen(js_name = ethornellSaveUserFile)]
    fn js_save_user_file(path: &str, bytes: &[u8]);
    #[wasm_bindgen(js_name = ethornellRemoveUserFile)]
    fn js_remove_user_file(path: &str);
    #[wasm_bindgen(js_namespace = console, js_name = error)]
    fn console_error(message: &str);
}

fn js_error(error: JsValue) -> io::Error {
    io::Error::other(
        error
            .as_string()
            .or_else(|| js_sys::Error::from(error).message().as_string())
            .unwrap_or_else(|| "JavaScript error".into()),
    )
}

/// One picked file, read in place through the worker's `File` table.
#[derive(Debug)]
struct JsFile {
    id: u32,
    len: u64,
    position: u64,
}

impl Read for JsFile {
    fn read(&mut self, out: &mut [u8]) -> io::Result<usize> {
        let count = (out.len() as u64).min(self.len.saturating_sub(self.position)) as usize;
        if count == 0 {
            return Ok(0);
        }
        let read = js_read_at(self.id, self.position as f64, &mut out[..count]).map_err(js_error)?;
        self.position += u64::from(read);
        Ok(read as usize)
    }
}

impl Seek for JsFile {
    fn seek(&mut self, from: SeekFrom) -> io::Result<u64> {
        let target = match from {
            SeekFrom::Start(offset) => Some(offset),
            SeekFrom::End(delta) => self.len.checked_add_signed(delta),
            SeekFrom::Current(delta) => self.position.checked_add_signed(delta),
        };
        self.position = target
            .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidInput, "seek before start"))?;
        Ok(self.position)
    }
}

impl ArchiveSource for JsFile {
    fn byte_len(&mut self) -> io::Result<u64> {
        Ok(self.len)
    }
}

struct PickedFolder {
    files: Vec<(String, u64, u32)>,
}

impl ResourceFiles for PickedFolder {
    fn list(&self) -> Vec<(String, u64)> {
        self.files.iter().map(|(path, len, _)| (path.clone(), *len)).collect()
    }

    fn open(&self, path: &str) -> io::Result<Box<dyn ArchiveSource>> {
        let (_, len, id) = self
            .files
            .iter()
            .find(|(candidate, _, _)| candidate == path)
            .ok_or_else(|| io::Error::from(io::ErrorKind::NotFound))?;
        Ok(Box::new(JsFile { id: *id, len: *len, position: 0 }))
    }
}

struct PageStorage;

impl UserStore for PageStorage {
    fn persist(&self, path: &str, bytes: Option<&[u8]>) {
        match bytes {
            Some(bytes) => js_save_user_file(path, bytes),
            None => js_remove_user_file(path),
        }
    }
}

fn browser_clock() -> WallTime {
    let date = js_sys::Date::new_0();
    WallTime {
        since_epoch: Duration::from_secs_f64(date.get_time().max(0.0) / 1000.0),
        // getTimezoneOffset is UTC minus local, in minutes.
        utc_offset_minutes: -(date.get_timezone_offset() as i32),
    }
}

fn install_panic_hook() {
    static ONCE: std::sync::Once = std::sync::Once::new();
    ONCE.call_once(|| {
        std::panic::set_hook(Box::new(|info| console_error(&format!("ethornell panic: {info}"))));
    });
}

/// The game text font, handed to `createRenderer`.
#[wasm_bindgen]
pub struct WebFont(ab_glyph::FontArc);

/// A renderer on the page's canvas, ready for `WebEngine::attach`.
#[wasm_bindgen]
pub struct WebRenderer {
    renderer: ethornell_render::Renderer<'static>,
    width: u32,
    height: u32,
}

/// Creates the renderer for a transferred canvas (WebGPU, else WebGL2).
#[wasm_bindgen(js_name = createRenderer)]
pub async fn create_renderer(
    canvas: web_sys::OffscreenCanvas,
    width: u32,
    height: u32,
    font: WebFont,
) -> Result<WebRenderer, JsValue> {
    let renderer = ethornell_render::Renderer::new_web(canvas, width, height, font.0)
        .await
        .map_err(|error| JsValue::from_str(&error.to_string()))?;
    Ok(WebRenderer { renderer, width, height })
}

/// The AudioContext's rate; call before creating the engine.
#[wasm_bindgen(js_name = setAudioSampleRate)]
pub fn set_audio_sample_rate(rate: u32) {
    ethornell_audio::pump::set_sample_rate(rate);
}

/// Renders interleaved stereo frames for the page's AudioWorklet.
#[wasm_bindgen(js_name = renderAudio)]
pub fn render_audio(out: &mut [f32]) {
    ethornell_audio::pump::render(out);
}

/// One host event, as `HostRuntime::poll_event` reports it.
#[wasm_bindgen(getter_with_clone)]
pub struct WebEvent {
    pub kind: u32,
    /// Request ids stay below 2^53, so a JS number holds them exactly.
    #[wasm_bindgen(js_name = requestId)]
    pub request_id: f64,
    pub code: i32,
    pub text: String,
}

/// A running game. Every call must come from the worker that created it.
#[wasm_bindgen]
pub struct WebEngine {
    host: HostRuntime,
    generation: u64,
}

fn message(error: impl std::fmt::Display) -> JsValue {
    JsValue::from_str(&error.to_string())
}

#[wasm_bindgen]
impl WebEngine {
    /// `files`: `[path, size]` pairs of the picked folder, in the order of
    /// worker.js's `File` table (an entry's index is its id). `user_files`:
    /// `[path, Uint8Array]` pairs restored from the page's storage.
    #[wasm_bindgen(constructor)]
    pub fn new(
        files: js_sys::Array,
        user_files: js_sys::Array,
        font: Vec<u8>,
        gbk: bool,
        silent: bool,
    ) -> Result<WebEngine, JsValue> {
        install_panic_hook();
        clock::install(browser_clock);
        let files = files
            .iter()
            .enumerate()
            .filter_map(|(index, entry)| {
                let entry = js_sys::Array::from(&entry);
                Some((entry.get(0).as_string()?, entry.get(1).as_f64()? as u64, index as u32))
            })
            .collect();
        let user_files = user_files
            .iter()
            .filter_map(|entry| {
                let entry = js_sys::Array::from(&entry);
                let bytes = js_sys::Uint8Array::new(&entry.get(1)).to_vec();
                Some((entry.get(0).as_string()?, bytes))
            })
            .collect();
        hosted_files::install(Arc::new(PickedFolder { files }), Arc::new(PageStorage), user_files);
        let host = HostRuntime::create(HostConfig {
            resource_root: "/game".into(),
            user_root: "/user".into(),
            font_path: None,
            font_data: Some(font),
            font_face_index: 0,
            gbk,
            backend: 0,
            strict: false,
            trace: false,
            silent,
        })
        .map_err(message)?;
        Ok(Self { host, generation: 0 })
    }

    pub fn font(&self) -> WebFont {
        WebFont(self.host.font())
    }

    /// Draws on `renderer` from now on (replacing any previous canvas).
    pub fn attach(&mut self, renderer: WebRenderer) -> Result<(), JsValue> {
        self.generation += 1;
        let generation = self.generation;
        self.host
            .scope(|host| {
                host.attach_renderer(renderer.renderer, renderer.width, renderer.height, generation)
            })
            .map_err(message)
    }

    /// The canvas backing store is now `width` x `height` pixels.
    pub fn resize(&mut self, width: u32, height: u32) -> Result<(), JsValue> {
        self.host.scope(|host| host.resize_surface(width, height)).map_err(message)
    }

    pub fn detach(&mut self) -> Result<(), JsValue> {
        let generation = self.generation;
        self.host.scope(|host| host.detach_surface(generation)).map_err(message)
    }

    /// `now_ms`: a monotonic clock (`performance.now()`).
    pub fn step(&mut self, now_ms: f64) -> Result<(), JsValue> {
        let now = (now_ms.max(0.0) * 1_000_000.0) as u64;
        self.host.scope(|host| host.step(now)).map_err(message)
    }

    /// The mobile ABI's input kinds; `x`/`y` in canvas pixels.
    pub fn input(&mut self, kind: u32, x: f32, y: f32, code: i32, value: f32) -> Result<(), JsValue> {
        self.host.scope(|host| host.input(kind, x, y, code, value)).map_err(message)
    }

    /// Bit 0: page hidden; bit 1: a dialog is open.
    pub fn pause(&mut self, mask: u32) -> Result<(), JsValue> {
        self.host.scope(|host| host.pause(mask)).map_err(message)
    }

    #[wasm_bindgen(js_name = requestClose)]
    pub fn request_close(&mut self) -> Result<(), JsValue> {
        self.host.scope(|host| host.request_close()).map_err(message)
    }

    #[wasm_bindgen(js_name = pollEvent)]
    pub fn poll_event(&mut self) -> Option<WebEvent> {
        self.host.poll_event().map(|event| WebEvent {
            kind: event.kind,
            request_id: event.request_id as f64,
            code: event.code,
            text: event.text,
        })
    }

    /// One line of engine state, for the console.
    pub fn diagnostics(&self) -> String {
        self.host.diagnostics()
    }

    /// The JSON payload of a kind-5 dialog event.
    #[wasm_bindgen(js_name = requestPayload)]
    pub fn request_payload(&self, id: f64) -> Option<String> {
        self.host.request_payload(id as u64)
    }

    #[wasm_bindgen(js_name = completeRequest)]
    pub fn complete_request(&mut self, id: f64, result: i32) -> Result<(), JsValue> {
        self.host.scope(|host| host.complete_request(id as u64, result)).map_err(message)
    }

    #[wasm_bindgen(js_name = completeDialog)]
    pub fn complete_dialog(&mut self, id: f64, response: &str) -> Result<(), JsValue> {
        self.host.scope(|host| host.complete_dialog(id as u64, response)).map_err(message)
    }
}
