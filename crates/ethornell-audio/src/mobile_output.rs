//! Mobile CPAL output with session-preserving route recovery.
//!
//! CPAL 0.15.3's Oboe callback reports a disconnection both before and after
//! closing. Kira 0.8.7's CPAL backend pushes both into a capacity-one queue
//! with `expect`, which can unwind through the native audio callback. Here
//! repeated errors coalesce into one flag. Only the output worker opens and
//! drops streams; the callback owns the same Kira renderer until it retires.
use cpal::{
    Device, Stream, StreamConfig,
    traits::{DeviceTrait, HostTrait, StreamTrait},
};
use kira::manager::backend::{Backend, Renderer};
use std::sync::{
    Arc, Mutex,
    atomic::{AtomicBool, Ordering},
    mpsc::{self, Receiver, SyncSender, TryRecvError},
};
use std::thread::{self, JoinHandle};
use std::time::Duration;

const RETRY_INTERVAL: Duration = Duration::from_millis(100);

#[derive(Default)]
struct Status {
    stopping: AtomicBool,
    ready: AtomicBool,
    current_disconnect: Mutex<Option<Arc<AtomicBool>>>,
    fatal: Mutex<Option<String>>,
}

impl Status {
    fn is_ready(&self) -> Result<bool, String> {
        if let Some(error) = self
            .fatal
            .lock()
            .map_err(|_| "audio output status poisoned")?
            .as_ref()
        {
            return Err(error.clone());
        }
        // Inspect the current callback's flag directly: VM time must freeze
        // on the next host tick, not after the worker's retry interval.
        let signal = self
            .current_disconnect
            .lock()
            .map_err(|_| "audio route status poisoned")?;
        Ok(self.ready.load(Ordering::Acquire)
            && signal.as_ref().is_some_and(|s| !s.load(Ordering::Acquire)))
    }

    fn publish_ready(&self, signal: &Arc<AtomicBool>) -> Result<(), String> {
        *self
            .current_disconnect
            .lock()
            .map_err(|_| "audio route status poisoned")? = Some(signal.clone());
        self.ready.store(true, Ordering::Release);
        Ok(())
    }
}

/// A callback's mixer lease returns exactly once, even if building or playing
/// a stream fails before the first audio callback. No lock or blocking send is
/// needed on the audio thread. Each stream has its own return channel.
struct RendererLease<T> {
    renderer: Option<T>,
    returned: SyncSender<T>,
}
impl<T> RendererLease<T> {
    fn new(renderer: T) -> (Self, Receiver<T>) {
        let (returned, receiver) = mpsc::sync_channel(1);
        (
            Self {
                renderer: Some(renderer),
                returned,
            },
            receiver,
        )
    }
}
impl<T> Drop for RendererLease<T> {
    fn drop(&mut self) {
        if let Some(renderer) = self.renderer.take() {
            // Receiver disappearance means the session has already shut down.
            let _ = self.returned.try_send(renderer);
        }
    }
}

pub struct MobileOutput {
    initial: Option<(Device, StreamConfig)>,
    status: Arc<Status>,
    worker: Option<JoinHandle<()>>,
}
impl MobileOutput {
    pub fn ready(&self) -> Result<bool, String> {
        if self.worker.as_ref().is_some_and(JoinHandle::is_finished) {
            return Err("audio output worker stopped unexpectedly".into());
        }
        self.status.is_ready()
    }
}
impl Backend for MobileOutput {
    type Settings = ();
    type Error = String;

    fn setup(_: ()) -> Result<(Self, u32), String> {
        let (device, config) = default_output()?;
        let rate = config.sample_rate.0;
        Ok((
            Self {
                initial: Some((device, config)),
                status: Arc::new(Status::default()),
                worker: None,
            },
            rate,
        ))
    }

    fn start(&mut self, renderer: Renderer) -> Result<(), String> {
        let initial = self.initial.take().ok_or("audio output already started")?;
        let status = self.status.clone();
        let (started, result) = mpsc::sync_channel(1);
        self.worker = Some(
            thread::Builder::new()
                .name("ethornell-audio-output".into())
                .spawn(move || {
                    output_worker(renderer, initial, status, started);
                })
                .map_err(|e| format!("audio output worker: {e}"))?,
        );
        result
            .recv()
            .map_err(|_| "audio output worker exited during startup".to_string())?
    }
}
impl Drop for MobileOutput {
    fn drop(&mut self) {
        self.status.stopping.store(true, Ordering::Release);
        if let Some(worker) = self.worker.take() {
            worker.thread().unpark();
            let _ = worker.join();
        }
    }
}

fn default_output() -> Result<(Device, StreamConfig), String> {
    let device = cpal::default_host()
        .default_output_device()
        .ok_or("no audio output device")?;
    let config = device
        .default_output_config()
        .map_err(|e| format!("audio output config: {e}"))?
        .config();
    if config.channels == 0 || config.sample_rate.0 == 0 {
        return Err("invalid audio output format".into());
    }
    Ok((device, config))
}

/// CPAL/Oboe invokes this same callback before and after closing a stream.
/// Each generation owns its flag, so late errors cannot retire a replacement.
fn disconnect_callback() -> (
    Arc<AtomicBool>,
    impl FnMut(cpal::StreamError) + Send + 'static,
) {
    let disconnected = Arc::new(AtomicBool::new(false));
    let signal = disconnected.clone();
    (disconnected, move |_error| {
        signal.store(true, Ordering::Release);
    })
}

struct OutputAttempt {
    stream: Option<Stream>,
    returned: Receiver<Renderer>,
    disconnected: Arc<AtomicBool>,
    error: Option<String>,
}
fn open_output(renderer: Renderer, device: &Device, config: &StreamConfig) -> OutputAttempt {
    let (mut lease, returned) = RendererLease::new(renderer);
    let (disconnected, error_callback) = disconnect_callback();
    let channels = usize::from(config.channels);
    let built = device.build_output_stream(
        config,
        move |data: &mut [f32], _| {
            data.fill(0.0);
            if let Some(renderer) = lease.renderer.as_mut() {
                renderer.on_start_processing();
                for frame in data.chunks_exact_mut(channels) {
                    let sample = renderer.process();
                    if channels == 1 {
                        frame[0] = (sample.left + sample.right) * 0.5;
                    } else {
                        frame[0] = sample.left;
                        frame[1] = sample.right;
                    }
                }
            }
        },
        error_callback,
        None,
    );
    let (stream, error) = match built {
        Ok(stream) => match stream.play() {
            Ok(()) => (Some(stream), None),
            Err(e) => (None, Some(format!("start audio output: {e}"))),
        },
        Err(e) => (None, Some(format!("build audio output: {e}"))),
    };
    OutputAttempt {
        stream,
        returned,
        disconnected,
        error,
    }
}

fn output_worker(
    mut renderer: Renderer,
    initial: (Device, StreamConfig),
    status: Arc<Status>,
    started: SyncSender<Result<(), String>>,
) {
    let (device, config) = initial;
    let mut rate = config.sample_rate.0;
    let mut attempt = open_output(renderer, &device, &config);
    if let Some(error) = attempt.error.as_ref() {
        let _ = started.send(Err(error.clone()));
        return;
    }
    if let Err(error) = status.publish_ready(&attempt.disconnected) {
        let _ = started.send(Err(error));
        return;
    }
    let _ = started.send(Ok(()));
    let mut waiting = None;
    let mut previous_error = None;
    while !status.stopping.load(Ordering::Acquire) {
        if attempt.stream.is_some() && attempt.disconnected.load(Ordering::Acquire) {
            status.ready.store(false, Ordering::Release);
            // Dropping the stream returns its renderer after callback ownership
            // has ended. Some native backends release the callback later.
            attempt.stream = None;
        }
        if attempt.stream.is_none() {
            if waiting.is_none() {
                match attempt.returned.try_recv() {
                    Ok(value) => waiting = Some(value),
                    Err(TryRecvError::Empty) => {}
                    Err(TryRecvError::Disconnected) => {
                        if let Ok(mut fatal) = status.fatal.lock() {
                            *fatal = Some("audio callback lost mixer ownership".into());
                        }
                        break;
                    }
                }
            }
            if waiting.is_some() {
                match default_output() {
                    Ok((device, config)) => {
                        renderer = waiting.take().expect("worker owns returned mixer");
                        if rate != config.sample_rate.0 {
                            renderer.on_change_sample_rate(config.sample_rate.0);
                            rate = config.sample_rate.0;
                        }
                        attempt = open_output(renderer, &device, &config);
                        if let Some(error) = attempt.error.as_ref() {
                            if previous_error.as_ref() != Some(error) {
                                tracing::warn!(%error, "audio route recovery will retry");
                                previous_error = Some(error.clone());
                            }
                        } else {
                            tracing::info!(rate, "audio output route recovered; mixer retained");
                            previous_error = None;
                            if let Err(error) = status.publish_ready(&attempt.disconnected) {
                                if let Ok(mut fatal) = status.fatal.lock() {
                                    *fatal = Some(error);
                                }
                                break;
                            }
                        }
                    }
                    Err(error) => {
                        if previous_error.as_ref() != Some(&error) {
                            tracing::warn!(%error, "waiting for audio output route");
                            previous_error = Some(error);
                        }
                    }
                }
            }
        }
        thread::park_timeout(RETRY_INTERVAL);
    }
    status.ready.store(false, Ordering::Release);
    drop(attempt);
}

#[cfg(test)]
mod tests {
    use super::*;
    use kira::{
        dsp::Frame,
        manager::{AudioManager, AudioManagerSettings},
        sound::static_sound::{StaticSoundData, StaticSoundSettings},
    };

    struct CaptureBackend {
        renderer: Option<Renderer>,
    }
    impl Backend for CaptureBackend {
        type Settings = ();
        type Error = ();
        fn setup(_: ()) -> Result<(Self, u32), ()> {
            Ok((Self { renderer: None }, 8000))
        }
        fn start(&mut self, renderer: Renderer) -> Result<(), ()> {
            self.renderer = Some(renderer);
            Ok(())
        }
    }

    #[test]
    fn mobile_output_duplicate_errors_are_coalesced_and_do_not_affect_next_stream() {
        let status = Status::default();
        let (first, mut notify) = disconnect_callback();
        status.publish_ready(&first).unwrap();
        assert!(status.is_ready().unwrap());
        notify(cpal::StreamError::DeviceNotAvailable); // Oboe before-close
        notify(cpal::StreamError::DeviceNotAvailable); // Oboe after-close
        assert!(!status.is_ready().unwrap()); // freezes VM before worker runs
        let (replacement, _replacement_callback) = disconnect_callback();
        status.publish_ready(&replacement).unwrap();
        notify(cpal::StreamError::DeviceNotAvailable); // old generation
        assert!(status.is_ready().unwrap());
    }

    #[test]
    fn mobile_output_failed_reopens_keep_the_same_mixer_and_playback_position() {
        let mut manager =
            AudioManager::<CaptureBackend>::new(AudioManagerSettings::default()).unwrap();
        let sound = manager
            .play(StaticSoundData {
                sample_rate: 8000,
                frames: vec![Frame::from_mono(0.25); 8000].into(),
                settings: StaticSoundSettings::default(),
            })
            .unwrap();
        let mut renderer = manager.backend_mut().renderer.take().unwrap();
        renderer.on_start_processing();
        for _ in 0..800 {
            renderer.process();
        }
        renderer.on_start_processing();
        let position = sound.position();
        assert!(position > 0.09);
        for _ in 0..3 {
            let (lease, receiver) = RendererLease::new(renderer);
            drop(lease); // failed build/play: callback drops without rendering
            renderer = receiver.try_recv().unwrap();
            assert!(matches!(
                receiver.try_recv(),
                Err(TryRecvError::Disconnected)
            ));
            assert_eq!(sound.position(), position);
        }
        renderer.on_change_sample_rate(16000);
        renderer.on_start_processing();
        for _ in 0..1600 {
            renderer.process();
        }
        renderer.on_start_processing();
        assert!((sound.position() - position - 0.1).abs() < 0.001);
    }
}
