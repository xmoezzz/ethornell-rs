use ethornell_image::DecodedImage;
use ruffle_core::limits::ExecutionLimit;
use ruffle_core::tag_utils::SwfMovie;
use ruffle_core::{FloatDuration, Player, PlayerBuilder};
use ruffle_render_wgpu::backend::{
    WgpuRenderBackend, create_wgpu_instance, request_adapter_and_device,
};
use ruffle_render_wgpu::descriptors::Descriptors;
use ruffle_render_wgpu::target::TextureTarget;
use ruffle_render_wgpu::wgpu;
use std::any::Any;
use std::collections::BTreeMap;
use std::fmt;
use std::panic::{AssertUnwindSafe, catch_unwind};
use std::sync::{Arc, Mutex};

const FLASH_BITMAP_SLOT_LIMIT: i32 = 0x4000;
const FLASH_STATUS_SUCCESS: i32 = 0;
const FLASH_STATUS_MISSING_CONTROL: i32 = 1;
const FLASH_STATUS_LOAD_FAILURE: i32 = 2;
const FLASH_STATUS_INVALID_BITMAP: i32 = 4;

type RufflePlayer = Arc<Mutex<Player>>;

pub(crate) struct RuntimeFlashControl {
    pub(crate) width: i32,
    pub(crate) height: i32,
    pub(crate) resource: String,
    pub(crate) parameter: i32,
    pub(crate) started: bool,
    failed: bool,
    player: RufflePlayer,
}

impl fmt::Debug for RuntimeFlashControl {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        formatter
            .debug_struct("RuntimeFlashControl")
            .field("width", &self.width)
            .field("height", &self.height)
            .field("resource", &self.resource)
            .field("parameter", &self.parameter)
            .field("started", &self.started)
            .field("failed", &self.failed)
            .finish_non_exhaustive()
    }
}

#[derive(Default)]
pub(crate) struct RuntimeFlashRegistry {
    descriptors: Option<Arc<Descriptors>>,
    controls: BTreeMap<i32, RuntimeFlashControl>,
}

impl fmt::Debug for RuntimeFlashRegistry {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        formatter
            .debug_struct("RuntimeFlashRegistry")
            .field("renderer_initialized", &self.descriptors.is_some())
            .field("controls", &self.controls)
            .finish()
    }
}

impl RuntimeFlashRegistry {
    pub(crate) fn create(
        &mut self,
        bitmap: i32,
        width: i32,
        height: i32,
        resource: String,
        parameter: i32,
        swf_data: &[u8],
    ) -> i32 {
        if !valid_bitmap_slot(bitmap) || width <= 0 || height <= 0 {
            return FLASH_STATUS_INVALID_BITMAP;
        }
        if resource.trim().is_empty() {
            return FLASH_STATUS_MISSING_CONTROL;
        }
        if swf_data.is_empty() {
            return FLASH_STATUS_LOAD_FAILURE;
        }

        let result = catch_unwind(AssertUnwindSafe(|| {
            self.create_control(width, height, resource.clone(), parameter, swf_data)
        }));
        let control = match result {
            Ok(Ok(control)) => control,
            Ok(Err(error)) => {
                tracing::warn!(
                    bitmap,
                    resource,
                    error,
                    "Ruffle Flash control creation failed"
                );
                return FLASH_STATUS_LOAD_FAILURE;
            }
            Err(payload) => {
                tracing::warn!(
                    bitmap,
                    resource,
                    panic = %panic_message(payload),
                    "Ruffle Flash control creation panicked"
                );
                return FLASH_STATUS_LOAD_FAILURE;
            }
        };

        self.controls.insert(bitmap, control);
        FLASH_STATUS_SUCCESS
    }

    fn create_control(
        &mut self,
        width: i32,
        height: i32,
        resource: String,
        parameter: i32,
        swf_data: &[u8],
    ) -> Result<RuntimeFlashControl, String> {
        let descriptors = self.ensure_descriptors()?;
        let movie = SwfMovie::from_data(swf_data, synthetic_movie_url(&resource), None, None)
            .map_err(|error| error.to_string())?;
        let target = TextureTarget::new(&descriptors.device, (width as u32, height as u32))
            .map_err(|error| error.to_string())?;
        let renderer =
            WgpuRenderBackend::new(descriptors, target).map_err(|error| error.to_string())?;
        let player = PlayerBuilder::new()
            .with_renderer(renderer)
            .with_movie(movie)
            .with_autoplay(false)
            .with_viewport_dimensions(width as u32, height as u32, 1.0)
            .build();

        {
            let mut player_guard = player
                .lock()
                .map_err(|_| "Ruffle player mutex was poisoned".to_string())?;
            player_guard.preload(&mut ExecutionLimit::none());
            player_guard.set_is_playing(false);
        }

        Ok(RuntimeFlashControl {
            width,
            height,
            resource,
            parameter,
            started: false,
            failed: false,
            player,
        })
    }

    fn ensure_descriptors(&mut self) -> Result<Arc<Descriptors>, String> {
        if let Some(descriptors) = &self.descriptors {
            return Ok(descriptors.clone());
        }

        // The web cannot block on adapter/device creation; Flash controls
        // fail to load there instead.
        #[cfg(target_arch = "wasm32")]
        return Err("Flash is not available in the web build".into());
        #[cfg(not(target_arch = "wasm32"))]
        {
            let backends = wgpu::Backends::all();
            let instance = create_wgpu_instance(backends, wgpu::BackendOptions::default());
            let (adapter, device, queue) = pollster::block_on(request_adapter_and_device(
                backends,
                &instance,
                None,
                wgpu::PowerPreference::HighPerformance,
            ))
            .map_err(|error| error.to_string())?;
            let descriptors = Arc::new(Descriptors::new(instance, adapter, device, queue));
            self.descriptors = Some(descriptors.clone());
            Ok(descriptors)
        }
    }

    pub(crate) fn start(&mut self, bitmap: i32) -> i32 {
        if !valid_bitmap_slot(bitmap) {
            return FLASH_STATUS_INVALID_BITMAP;
        }
        let Some(control) = self.controls.get_mut(&bitmap) else {
            return FLASH_STATUS_MISSING_CONTROL;
        };
        if control.failed {
            return FLASH_STATUS_LOAD_FAILURE;
        }

        let result = catch_unwind(AssertUnwindSafe(|| {
            let mut player = control
                .player
                .lock()
                .map_err(|_| "Ruffle player mutex was poisoned".to_string())?;
            player.preload(&mut ExecutionLimit::none());
            player.set_is_playing(true);
            player.mutate_with_update_context(|context| {
                if let Some(root) = context.stage.root_clip()
                    && let Some(movie_clip) = root.as_movie_clip()
                    && !movie_clip.playing()
                {
                    movie_clip.play();
                }
            });
            Ok::<(), String>(())
        }));

        match result {
            Ok(Ok(())) => {
                control.started = true;
                FLASH_STATUS_SUCCESS
            }
            Ok(Err(error)) => {
                control.failed = true;
                tracing::warn!(bitmap, error, "Ruffle Flash control start failed");
                FLASH_STATUS_LOAD_FAILURE
            }
            Err(payload) => {
                control.failed = true;
                tracing::warn!(
                    bitmap,
                    panic = %panic_message(payload),
                    "Ruffle Flash control start panicked"
                );
                FLASH_STATUS_LOAD_FAILURE
            }
        }
    }

    /// Advance all active SWFs using the native engine's elapsed tick. The target
    /// ActiveX control owns its live surface until command 33, so no BGI bitmap is
    /// updated here; the final frame is copied by `capture_and_release`.
    pub(crate) fn tick(&mut self, elapsed_ms: u64) {
        if elapsed_ms == 0 {
            return;
        }
        let elapsed = FloatDuration::from_millis(elapsed_ms as f64);
        for (&bitmap, control) in &mut self.controls {
            if !control.started || control.failed {
                continue;
            }
            let result = catch_unwind(AssertUnwindSafe(|| {
                let mut player = control
                    .player
                    .lock()
                    .map_err(|_| "Ruffle player mutex was poisoned".to_string())?;
                player.tick(elapsed);
                Ok::<(), String>(())
            }));
            match result {
                Ok(Ok(())) => {}
                Ok(Err(error)) => {
                    control.failed = true;
                    tracing::warn!(bitmap, error, "Ruffle Flash tick failed");
                }
                Err(payload) => {
                    control.failed = true;
                    tracing::warn!(
                        bitmap,
                        panic = %panic_message(payload),
                        "Ruffle Flash tick panicked"
                    );
                }
            }
        }
    }

    pub(crate) fn capture_and_release(&mut self, bitmap: i32) -> (i32, Option<DecodedImage>) {
        if !valid_bitmap_slot(bitmap) {
            return (FLASH_STATUS_INVALID_BITMAP, None);
        }
        let Some(control) = self.controls.remove(&bitmap) else {
            return (FLASH_STATUS_MISSING_CONTROL, None);
        };
        if control.failed {
            return (FLASH_STATUS_MISSING_CONTROL, None);
        }

        let result = catch_unwind(AssertUnwindSafe(|| capture_player_frame(&control.player)));
        match result {
            Ok(Some(image)) => (FLASH_STATUS_SUCCESS, Some(image)),
            Ok(None) => {
                tracing::warn!(bitmap, "Ruffle Flash control produced no capture frame");
                (FLASH_STATUS_MISSING_CONTROL, None)
            }
            Err(payload) => {
                tracing::warn!(
                    bitmap,
                    panic = %panic_message(payload),
                    "Ruffle Flash capture panicked"
                );
                (FLASH_STATUS_MISSING_CONTROL, None)
            }
        }
    }

    #[cfg(test)]
    pub(crate) fn get(&self, bitmap: i32) -> Option<&RuntimeFlashControl> {
        self.controls.get(&bitmap)
    }
}

fn capture_player_frame(player: &RufflePlayer) -> Option<DecodedImage> {
    let mut player = player.lock().ok()?;
    player.render();
    let renderer =
        <dyn Any>::downcast_mut::<WgpuRenderBackend<TextureTarget>>(player.renderer_mut())?;
    // ruffle reads frames back with a blocking buffer map, which the web
    // does not have; there a Flash control yields no frame.
    #[cfg(target_arch = "wasm32")]
    let image: Option<image::RgbaImage> = {
        let _ = renderer;
        None
    };
    #[cfg(not(target_arch = "wasm32"))]
    let image = renderer.capture_frame();
    let image = image?;
    Some(DecodedImage {
        width: image.width(),
        height: image.height(),
        rgba: image.into_raw(),
    })
}

fn valid_bitmap_slot(bitmap: i32) -> bool {
    (0..FLASH_BITMAP_SLOT_LIMIT).contains(&bitmap)
}

fn synthetic_movie_url(resource: &str) -> String {
    let file_name = resource
        .replace('\\', "/")
        .rsplit('/')
        .next()
        .filter(|name| !name.is_empty())
        .unwrap_or("movie.swf")
        .replace(' ', "%20");
    format!("file:///bgi/{file_name}")
}

fn panic_message(payload: Box<dyn Any + Send>) -> String {
    if let Some(message) = payload.downcast_ref::<&str>() {
        (*message).to_string()
    } else if let Some(message) = payload.downcast_ref::<String>() {
        message.clone()
    } else {
        "non-string panic payload".to_string()
    }
}

#[cfg(test)]
mod tests {
    use super::{
        FLASH_STATUS_INVALID_BITMAP, FLASH_STATUS_LOAD_FAILURE, FLASH_STATUS_MISSING_CONTROL,
        RuntimeFlashRegistry, synthetic_movie_url,
    };

    #[test]
    fn invalid_slots_and_missing_controls_preserve_target_status_classes() {
        let mut registry = RuntimeFlashRegistry::default();
        assert_eq!(registry.start(-1), FLASH_STATUS_INVALID_BITMAP);
        assert_eq!(registry.start(0x4000), FLASH_STATUS_INVALID_BITMAP);
        assert_eq!(registry.start(3), FLASH_STATUS_MISSING_CONTROL);
        let (status, image) = registry.capture_and_release(3);
        assert_eq!(status, FLASH_STATUS_MISSING_CONTROL);
        assert!(image.is_none());
        assert_eq!(
            registry.create(3, 640, 480, "title.swf".into(), 7, &[]),
            FLASH_STATUS_LOAD_FAILURE
        );
    }

    #[test]
    fn archived_resource_names_receive_a_valid_synthetic_file_url() {
        assert_eq!(
            synthetic_movie_url("graph\\flash title.swf"),
            "file:///bgi/flash%20title.swf"
        );
    }
}
