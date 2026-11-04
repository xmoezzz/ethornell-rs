//! A kira backend without an audio device: the host pulls rendered frames
//! with [`render`]. The web build runs the engine in a worker, which has no
//! `AudioContext`; the page plays the frames in an AudioWorklet.
use kira::manager::backend::{Backend, Renderer};
use std::sync::Mutex;
use std::sync::atomic::{AtomicU32, Ordering};

static RENDERER: Mutex<Option<Renderer>> = Mutex::new(None);
static SAMPLE_RATE: AtomicU32 = AtomicU32::new(48_000);

/// The output rate of backends set up from now on (the page's AudioContext).
pub fn set_sample_rate(rate: u32) {
    SAMPLE_RATE.store(rate.clamp(8_000, 192_000), Ordering::Relaxed);
}

/// Fills `out` with interleaved stereo frames; silence while no audio
/// manager is running.
pub fn render(out: &mut [f32]) {
    let Ok(mut renderer) = RENDERER.lock() else {
        out.fill(0.0);
        return;
    };
    let Some(renderer) = renderer.as_mut() else {
        out.fill(0.0);
        return;
    };
    renderer.on_start_processing();
    for frame in out.chunks_exact_mut(2) {
        let sample = renderer.process();
        frame[0] = sample.left;
        frame[1] = sample.right;
    }
}

pub struct PumpBackend;

impl Backend for PumpBackend {
    type Settings = ();
    type Error = std::convert::Infallible;

    fn setup(_: ()) -> Result<(Self, u32), Self::Error> {
        Ok((Self, SAMPLE_RATE.load(Ordering::Relaxed)))
    }

    fn start(&mut self, renderer: Renderer) -> Result<(), Self::Error> {
        if let Ok(mut slot) = RENDERER.lock() {
            *slot = Some(renderer);
        }
        Ok(())
    }
}

impl Drop for PumpBackend {
    fn drop(&mut self) {
        if let Ok(mut slot) = RENDERER.lock() {
            *slot = None;
        }
    }
}

/// Console audio devices accept signed 16-bit stereo. Conversion happens
/// after the shared Kira mixer, once per device callback.
pub fn render_i16(out: &mut [i16]) {
    out.fill(0);
    let Ok(mut slot) = RENDERER.lock() else {
        return;
    };
    let Some(renderer) = slot.as_mut() else {
        return;
    };
    renderer.on_start_processing();
    for stereo in out.chunks_exact_mut(2) {
        let frame = renderer.process();
        stereo[0] = (frame.left.clamp(-1.0, 1.0) * 32767.) as i16;
        stereo[1] = (frame.right.clamp(-1.0, 1.0) * 32767.) as i16;
    }
}
