use ethornell_core::{EthornellError, Result};
use kira::dsp::Frame;
use kira::manager::backend::Backend;
#[cfg(not(any(target_arch="wasm32",target_os="vita",target_os="horizon")))]
use kira::manager::backend::DefaultBackend;

#[cfg(any(target_arch="wasm32",target_os="vita",target_os="horizon"))]
pub mod pump;
#[cfg(not(target_arch = "wasm32"))]
mod streamed;
/// Desktop/mobile use cpal; web and native consoles pull the shared mixer.
#[cfg(not(any(target_arch="wasm32",target_os="vita",target_os="horizon")))]
pub type PlatformBackend = DefaultBackend;
#[cfg(any(target_arch="wasm32",target_os="vita",target_os="horizon"))]
pub type PlatformBackend = pump::PumpBackend;
use kira::manager::{AudioManager, AudioManagerSettings};
use kira::sound::{
    FromFileError, PlaybackPosition, PlaybackState,
    static_sound::{StaticSoundData, StaticSoundHandle, StaticSoundSettings},
};
#[cfg(not(target_arch = "wasm32"))]
use kira::sound::streaming::{StreamingSoundData, StreamingSoundHandle, StreamingSoundSettings};

/// A sound decoded while it plays. kira streams on a decoder thread, which
/// the web lacks; there the sound is decoded up front instead.
#[cfg(not(target_arch = "wasm32"))]
type StreamHandle = StreamingSoundHandle<FromFileError>;
#[cfg(target_arch = "wasm32")]
type StreamHandle = StaticSoundHandle;
use kira::tween::Tween;
use serde::Serialize;
use std::collections::BTreeMap;
use std::io::Cursor;
use std::sync::Arc;
use std::time::Duration;

#[derive(Debug, Clone, Serialize)]
pub struct AudioInfo {
    pub kind: AudioKind,
    pub header_len: Option<u32>,
    pub file_size: Option<u32>,
    pub sample_len: Option<u32>,
    pub frequency: Option<u32>,
    pub channels: Option<u32>,
    pub payload_offset: Option<usize>,
    pub payload_kind: Option<AudioKind>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
pub enum AudioKind {
    Ogg,
    RiffWave,
    BurikoWaveBoxOgg,
    BurikoWaveBoxUnknown,
    Unknown,
}

pub fn probe_audio(data: &[u8]) -> AudioInfo {
    if data.starts_with(b"OggS") {
        return simple_info(AudioKind::Ogg);
    }
    if data.starts_with(b"RIFF") {
        return simple_info(AudioKind::RiffWave);
    }
    if data.len() >= 0x40 && &data[4..8] == b"bw  " {
        let header_len = u32::from_le_bytes(data[0..4].try_into().unwrap());
        let payload_offset = header_len as usize;
        let payload_kind = data
            .get(payload_offset..payload_offset + 4)
            .map(|magic| {
                if magic == b"OggS" {
                    AudioKind::Ogg
                } else if magic == b"RIFF" {
                    AudioKind::RiffWave
                } else {
                    AudioKind::Unknown
                }
            })
            .unwrap_or(AudioKind::Unknown);
        return AudioInfo {
            kind: if payload_kind == AudioKind::Ogg {
                AudioKind::BurikoWaveBoxOgg
            } else {
                AudioKind::BurikoWaveBoxUnknown
            },
            header_len: Some(header_len),
            file_size: read_u32(data, 8),
            sample_len: read_u32(data, 12),
            frequency: read_u32(data, 16),
            channels: read_u32(data, 20),
            payload_offset: Some(payload_offset),
            payload_kind: Some(payload_kind),
        };
    }
    AudioInfo {
        kind: AudioKind::Unknown,
        header_len: None,
        file_size: None,
        sample_len: None,
        frequency: None,
        channels: None,
        payload_offset: None,
        payload_kind: None,
    }
}

pub fn unwrap_buriko_wave_ogg(data: &[u8]) -> Option<&[u8]> {
    let info = probe_audio(data);
    if info.kind != AudioKind::BurikoWaveBoxOgg {
        return None;
    }
    let offset = info.payload_offset?;
    data.get(offset..)
}

fn simple_info(kind: AudioKind) -> AudioInfo {
    AudioInfo {
        kind,
        header_len: None,
        file_size: None,
        sample_len: None,
        frequency: None,
        channels: None,
        payload_offset: Some(0),
        payload_kind: Some(kind),
    }
}

fn read_u32(data: &[u8], offset: usize) -> Option<u32> {
    let bytes: [u8; 4] = data.get(offset..offset + 4)?.try_into().ok()?;
    Some(u32::from_le_bytes(bytes))
}

pub struct AudioSystem<B: Backend = PlatformBackend> {
    manager: AudioManager<B>,
    volume: f64,
    channels: BTreeMap<i32, StaticSoundHandle>,
    /// Channels playing a sound that is decoded while it plays (movie
    /// soundtracks). A channel is in at most one of the two maps.
    streams: BTreeMap<i32, StreamHandle>,
    next_unmanaged_channel: i32,
    host_suspended: bool,
}

impl AudioSystem {
    pub fn new() -> Result<Self> {
        Self::with_settings(AudioManagerSettings::default())
    }
}

impl<B: Backend> AudioSystem<B> {
    /// Accepts real Kira backend/capacity settings; a mock backend can exercise
    /// the same pause commands without a device. Android must initialize
    /// ndk-context before constructing the default CPAL/Oboe backend.
    pub fn with_settings(settings: AudioManagerSettings<B>) -> Result<Self>
    where
        B::Error: std::fmt::Debug,
    {
        let manager = AudioManager::new(settings)
            .map_err(|err| EthornellError::Other(format!("audio init failed: {err:?}")))?;
        Ok(Self {
            manager,
            volume: 1.0,
            channels: BTreeMap::new(),
            streams: BTreeMap::new(),
            next_unmanaged_channel: i32::MIN,
            host_suspended: false,
        })
    }

    /// Host lifecycle/focus pause, independent of each script sound's state.
    /// Call on the engine's serial command thread. Success means queued, not
    /// synchronous acknowledgement by the hardware callback. No stop/restart.
    pub fn host_suspend(&mut self) -> Result<()> {
        if !self.host_suspended {
            self.manager
                .pause(immediate_tween())
                .map_err(|err| EthornellError::Other(format!("audio suspend failed: {err}")))?;
            self.host_suspended = true;
        }
        Ok(())
    }

    /// Resumes the mixer clock, not script-paused channels. Host reason masks
    /// belong to the runtime: call only when every pause reason has cleared.
    pub fn host_resume(&mut self) -> Result<()> {
        if self.host_suspended {
            self.manager
                .resume(immediate_tween())
                .map_err(|err| EthornellError::Other(format!("audio resume failed: {err}")))?;
            self.host_suspended = false;
        }
        Ok(())
    }

    pub fn is_host_suspended(&self) -> bool {
        self.host_suspended
    }

    pub fn play_ogg_file(&mut self, path: impl AsRef<std::path::Path>) -> Result<()> {
        // Track file playback too, so channel cleanup applies uniformly.
        self.play_from_bytes(&std::fs::read(path)?)
    }

    pub fn play_from_bytes(&mut self, bytes: &[u8]) -> Result<()> {
        let channel = self.next_unmanaged_channel;
        self.next_unmanaged_channel = self.next_unmanaged_channel.saturating_add(1);
        self.play_on_channel(channel, bytes, false, 1.0, 1.0, 1.0, 0.5, 0)
    }

    pub fn play_on_channel(
        &mut self,
        channel: i32,
        bytes: &[u8],
        looped: bool,
        volume: f64,
        decode_gain: f64,
        playback_rate: f64,
        panning: f64,
        fade_in_ms: u64,
    ) -> Result<()> {
        self.play_on_channel_inner(
            channel,
            bytes,
            looped,
            volume,
            decode_gain,
            playback_rate,
            panning,
            fade_in_ms,
            true,
        )
        .map(|_| ())
    }

    pub fn play_on_channel_if_stopped(
        &mut self,
        channel: i32,
        bytes: &[u8],
        looped: bool,
        volume: f64,
        decode_gain: f64,
        playback_rate: f64,
        panning: f64,
        fade_in_ms: u64,
    ) -> Result<bool> {
        self.play_on_channel_inner(
            channel,
            bytes,
            looped,
            volume,
            decode_gain,
            playback_rate,
            panning,
            fade_in_ms,
            false,
        )
    }

    #[allow(clippy::too_many_arguments)]
    pub fn play_intro_loop_on_channel(
        &mut self,
        channel: i32,
        intro_bytes: &[u8],
        loop_bytes: &[u8],
        looped: bool,
        volume: f64,
        decode_gain: f64,
        playback_rate: f64,
        panning: f64,
        fade_in_ms: u64,
        restart: bool,
    ) -> Result<bool> {
        validate_playback_parameters(volume, decode_gain, playback_rate, panning)?;
        if !restart && self.channel_is_active(channel) {
            return Ok(false);
        }

        #[cfg(not(target_arch = "wasm32"))]
        {
            let settings = streaming_settings(volume, playback_rate, panning, fade_in_ms);
            self.start_streamed(channel, Some(intro_bytes), loop_bytes, looped.then_some(0), decode_gain, settings)
        }
        #[cfg(target_arch = "wasm32")]
        {
            let intro = decode_static_sound(intro_bytes, StaticSoundSettings::default())?;
            let loop_sound = decode_static_sound(loop_bytes, StaticSoundSettings::default())?;
            if intro.sample_rate != loop_sound.sample_rate {
                return Err(EthornellError::Other(format!(
                    "BGM pair sample-rate mismatch: intro={}Hz loop={}Hz",
                    intro.sample_rate, loop_sound.sample_rate
                )));
            }

            let loop_start = intro.frames.len();
            let mut frames = Vec::with_capacity(loop_start.saturating_add(loop_sound.frames.len()));
            frames.extend_from_slice(&intro.frames);
            frames.extend_from_slice(&loop_sound.frames);

            let mut settings = playback_settings(volume, playback_rate, panning, fade_in_ms);
            if looped {
                settings = settings
                    .loop_region(PlaybackPosition::Samples(loop_start.min(i64::MAX as usize) as i64)..);
            }
            let mut data = StaticSoundData {
                sample_rate: intro.sample_rate,
                frames: frames.into(),
                settings,
            };
            apply_decode_gain(&mut data, decode_gain);
            self.start_static_sound(channel, data)
        }
    }

    fn play_on_channel_inner(
        &mut self,
        channel: i32,
        bytes: &[u8],
        looped: bool,
        volume: f64,
        decode_gain: f64,
        playback_rate: f64,
        panning: f64,
        fade_in_ms: u64,
        restart: bool,
    ) -> Result<bool> {
        validate_playback_parameters(volume, decode_gain, playback_rate, panning)?;
        if !restart && self.channel_is_active(channel) {
            return Ok(false);
        }
        // The Ogg box's end-of-stream listener (sub_4AA420) seeks back to the
        // header's loop start (+0x1C), not to the first sample.
        let loop_start = buriko_wave_loop_start(bytes).min(i64::MAX as u64) as i64;
        #[cfg(not(target_arch = "wasm32"))]
        if declared_frame_count(bytes).is_some_and(|frames| frames > STREAMED_FRAMES) {
            let settings = streaming_settings(volume, playback_rate, panning, fade_in_ms);
            let loop_start = looped.then_some(loop_start as usize);
            return self.start_streamed(channel, None, bytes, loop_start, decode_gain, settings);
        }
        let mut settings = playback_settings(volume, playback_rate, panning, fade_in_ms);
        if looped {
            settings = settings.loop_region(PlaybackPosition::Samples(loop_start)..);
        }
        let mut data = decode_static_sound(bytes, settings)?;
        apply_decode_gain(&mut data, decode_gain);
        self.start_static_sound(channel, data)
    }

    /// Plays `body`, after `intro` if given, decoding while it plays. A loop
    /// returns to `loop_start` frames into the body.
    #[cfg(not(target_arch = "wasm32"))]
    fn start_streamed(
        &mut self,
        channel: i32,
        intro: Option<&[u8]>,
        body: &[u8],
        loop_start: Option<usize>,
        decode_gain: f64,
        mut settings: StreamingSoundSettings,
    ) -> Result<bool> {
        let part = |bytes: &[u8]| {
            (
                unwrap_buriko_wave_ogg(bytes).unwrap_or(bytes).to_vec(),
                declared_frame_count(bytes),
            )
        };
        let decoder =
            streamed::StreamedDecoder::new(intro.map(part), part(body), decode_gain.max(0.0) as f32)
                .map_err(|err| EthornellError::Other(format!("load audio stream failed: {err}")))?;
        let (intro_rate, body_rate) = decoder.sample_rates();
        if intro_rate != body_rate {
            return Err(EthornellError::Other(format!(
                "BGM pair sample-rate mismatch: intro={intro_rate}Hz loop={body_rate}Hz"
            )));
        }
        if let Some(loop_start) = loop_start {
            let loop_start = decoder.body_start().saturating_add(loop_start);
            settings = settings
                .loop_region(PlaybackPosition::Samples(loop_start.min(i64::MAX as usize) as i64)..);
        }
        let handle = self
            .manager
            .play(StreamingSoundData::from_decoder(decoder, settings))
            .map_err(|err| EthornellError::Other(format!("play audio stream failed: {err}")))?;
        if let Some(mut previous) = self.channels.remove(&channel) {
            let _ = previous.stop(Tween::default());
        }
        if let Some(mut previous) = self.streams.insert(channel, handle) {
            let _ = previous.stop(Tween::default());
        }
        Ok(true)
    }

    fn start_static_sound(&mut self, channel: i32, data: StaticSoundData) -> Result<bool> {
        self.channels
            .retain(|_, handle| handle.state() != PlaybackState::Stopped);
        let handle = self
            .manager
            .play(data)
            .map_err(|err| EthornellError::Other(format!("play audio bytes failed: {err}")))?;
        if let Some(mut previous) = self.channels.insert(channel, handle) {
            let _ = previous.stop(Tween::default());
        }
        if let Some(mut previous) = self.streams.remove(&channel) {
            let _ = previous.stop(Tween::default());
        }
        Ok(true)
    }

    /// Plays `bytes` (any stream Symphonia can probe, such as a movie's MPEG
    /// audio elementary stream) on `channel`, replacing what the channel
    /// played. Decoding happens on kira's streaming thread, so a long
    /// soundtrack neither stalls the caller nor is held decoded in memory.
    pub fn play_stream_on_channel(
        &mut self,
        channel: i32,
        bytes: Arc<[u8]>,
        looped: bool,
        volume: f64,
        start_ms: u64,
    ) -> Result<()> {
        #[cfg(not(target_arch = "wasm32"))]
        let (settings, load) = (StreamingSoundSettings::default(), StreamingSoundData::from_cursor);
        #[cfg(target_arch = "wasm32")]
        let (settings, load) = (StaticSoundSettings::default(), StaticSoundData::from_cursor);
        let mut settings = settings
            .volume(volume.clamp(0.0, 1.0))
            .playback_region(Duration::from_millis(start_ms).as_secs_f64()..);
        if looped {
            settings = settings.loop_region(..);
        }
        let data = load(Cursor::new(bytes), settings)
            .map_err(|err| EthornellError::Other(format!("load audio stream failed: {err}")))?;
        let handle = self
            .manager
            .play(data)
            .map_err(|err| EthornellError::Other(format!("play audio stream failed: {err}")))?;
        if let Some(mut previous) = self.channels.remove(&channel) {
            let _ = previous.stop(Tween::default());
        }
        if let Some(mut previous) = self.streams.insert(channel, handle) {
            let _ = previous.stop(Tween::default());
        }
        Ok(())
    }

    pub fn stop_all(&mut self) {
        for handle in self.channels.values_mut() {
            let _ = handle.stop(Tween::default());
        }
        self.channels.clear();
        for handle in self.streams.values_mut() {
            let _ = handle.stop(Tween::default());
        }
        self.streams.clear();
    }

    pub fn stop_channel(&mut self, channel: i32, fade_out_ms: u64) {
        if let Some(handle) = self.streams.get_mut(&channel) {
            let _ = handle.stop(Tween {
                duration: Duration::from_millis(fade_out_ms),
                ..Tween::default()
            });
            return;
        }
        let Some(handle) = self.channels.get_mut(&channel) else {
            return;
        };
        let _ = handle.stop(Tween {
            duration: Duration::from_millis(fade_out_ms),
            ..Tween::default()
        });
    }

    pub fn pause_channel(&mut self, channel: i32, paused: bool, fade_ms: u64) {
        let _ = self.try_pause_channel(channel, paused, fade_ms);
    }

    pub fn try_pause_channel(&mut self, channel: i32, paused: bool, fade_ms: u64) -> Result<()> {
        let tween = Tween {
            duration: Duration::from_millis(fade_ms),
            ..Tween::default()
        };
        let result = if let Some(handle) = self.streams.get_mut(&channel) {
            if paused {
                handle.pause(tween)
            } else {
                handle.resume(tween)
            }
        } else if let Some(handle) = self.channels.get_mut(&channel) {
            if paused {
                handle.pause(tween)
            } else {
                handle.resume(tween)
            }
        } else {
            return Ok(());
        };
        result.map_err(|err| EthornellError::Other(format!("audio channel pause failed: {err}")))
    }

    pub fn set_channel_volume(&mut self, channel: i32, volume: f64, fade_ms: u64) {
        if let Some(handle) = self.streams.get_mut(&channel) {
            let _ = handle.set_volume(
                volume.clamp(0.0, 1.0),
                Tween {
                    duration: Duration::from_millis(fade_ms),
                    ..Tween::default()
                },
            );
            return;
        }
        let Some(handle) = self.channels.get_mut(&channel) else {
            return;
        };
        let _ = handle.set_volume(
            volume.clamp(0.0, 1.0),
            Tween {
                duration: Duration::from_millis(fade_ms),
                ..Tween::default()
            },
        );
    }

    pub fn set_channel_panning(&mut self, channel: i32, panning: f64, fade_ms: u64) {
        if let Some(handle) = self.streams.get_mut(&channel) {
            let _ = handle.set_panning(
                panning.clamp(0.0, 1.0),
                Tween {
                    duration: Duration::from_millis(fade_ms),
                    ..Tween::default()
                },
            );
            return;
        }
        let Some(handle) = self.channels.get_mut(&channel) else {
            return;
        };
        let _ = handle.set_panning(
            panning.clamp(0.0, 1.0),
            Tween {
                duration: Duration::from_millis(fade_ms),
                ..Tween::default()
            },
        );
    }

    pub fn channel_is_active(&self, channel: i32) -> bool {
        if let Some(handle) = self.streams.get(&channel) {
            return handle.state() != PlaybackState::Stopped;
        }
        self.channels
            .get(&channel)
            .is_some_and(|handle| handle.state() != PlaybackState::Stopped)
    }

    pub fn set_volume(&mut self, volume: f64) {
        let _ = self.try_set_volume(volume);
    }

    /// Sets the main mixer volume, affecting both existing and future sounds.
    pub fn try_set_volume(&mut self, volume: f64) -> Result<()> {
        if !volume.is_finite() {
            return Err(EthornellError::Other("non-finite audio volume".into()));
        }
        let volume = volume.clamp(0.0, 1.0);
        self.manager
            .main_track()
            .set_volume(volume, immediate_tween())
            .map_err(|err| EthornellError::Other(format!("audio volume failed: {err}")))?;
        self.volume = volume;
        Ok(())
    }

    pub fn volume(&self) -> f64 {
        self.volume
    }
}

fn immediate_tween() -> Tween {
    Tween {
        duration: Duration::ZERO,
        ..Tween::default()
    }
}

fn validate_playback_parameters(volume: f64, gain: f64, rate: f64, panning: f64) -> Result<()> {
    if ![volume, gain, rate, panning]
        .into_iter()
        .all(f64::is_finite)
    {
        return Err(EthornellError::Other(
            "non-finite audio playback parameter".into(),
        ));
    }
    Ok(())
}

/// Decodes a whole sound, cut to the length its file declares
/// (`declared_frame_count`).
/// BURIKO wave box header +0x1C: the sample a looping stream returns to.
fn buriko_wave_loop_start(bytes: &[u8]) -> u64 {
    if probe_audio(bytes).kind == AudioKind::BurikoWaveBoxOgg {
        read_u32(bytes, 0x1C).map_or(0, u64::from)
    } else {
        0
    }
}

fn decode_static_sound(bytes: &[u8], settings: StaticSoundSettings) -> Result<StaticSoundData> {
    let payload = unwrap_buriko_wave_ogg(bytes).unwrap_or(bytes);
    let mut data = StaticSoundData::from_cursor(Cursor::new(payload.to_vec()), settings)
        .map_err(|err| EthornellError::Other(format!("load audio bytes failed: {err}")))?;
    if let Some(frames) = declared_frame_count(bytes)
        && data.frames.len() > frames
    {
        data.frames = data.frames[..frames].into();
    }
    Ok(data)
}

/// How many frames a sound plays. For a BURIKO wave box, the header's sample
/// count (+0x0C): the target reads the 64-byte header (sub_4A8330), sizes
/// the decoded buffer as samples * channels * 2 bytes and asks for that many
/// samples (sub_4A91F0), and its Ogg reader clamps every request to the
/// samples left before that count (sub_4A8F40, through vorbisfile's ov_read,
/// sub_4BCAC0). For a bare Ogg stream, its end position. Symphonia, opened by
/// kira without gapless mode, decodes the last Vorbis block whole, so a sound
/// ran past its end position (which the box's count equals) by up to half a
/// block: 1023 frames with 2048-frame blocks.
fn declared_frame_count(bytes: &[u8]) -> Option<usize> {
    let info = probe_audio(bytes);
    let frames = match info.kind {
        AudioKind::BurikoWaveBoxOgg => u64::from(info.sample_len?),
        AudioKind::Ogg => ogg_end_position(bytes)?,
        _ => return None,
    };
    usize::try_from(frames).ok()
}

/// The granule position of an Ogg stream's last page, if the data ends with
/// a whole page of the stream the first page starts. For Vorbis that is the
/// number of frames up to the end of the stream.
fn ogg_end_position(data: &[u8]) -> Option<u64> {
    const MAX_PAGE_LEN: usize = 27 + 255 + 255 * 255;
    let serial = data.get(14..18)?;
    let first = data.len().saturating_sub(MAX_PAGE_LEN);
    let page = (first..data.len()).rev().find_map(|start| {
        let page = &data[start..];
        let segments = usize::from(*page.get(26)?);
        let body: usize = page
            .get(27..27 + segments)?
            .iter()
            .map(|&len| usize::from(len))
            .sum();
        (page.starts_with(b"OggS") && page[4] == 0 && 27 + segments + body == page.len())
            .then_some(page)
    })?;
    if &page[14..18] != serial {
        return None;
    }
    let granule = u64::from_le_bytes(page[6..14].try_into().ok()?);
    (granule != u64::MAX).then_some(granule)
}

/// Sounds longer than this (~20 s at 48 kHz, 8 MB decoded) are decoded
/// while they play, not up front.
#[cfg(not(target_arch = "wasm32"))]
const STREAMED_FRAMES: usize = 1 << 20;

#[cfg(not(target_arch = "wasm32"))]
fn streaming_settings(
    volume: f64,
    playback_rate: f64,
    panning: f64,
    fade_in_ms: u64,
) -> StreamingSoundSettings {
    let mut settings = StreamingSoundSettings::default()
        .volume(volume.clamp(0.0, 1.0))
        .playback_rate(playback_rate.clamp(0.01, 16.0))
        .panning(panning.clamp(0.0, 1.0));
    if fade_in_ms > 0 {
        settings = settings.fade_in_tween(Tween {
            duration: Duration::from_millis(fade_in_ms),
            ..Tween::default()
        });
    }
    settings
}

fn playback_settings(
    volume: f64,
    playback_rate: f64,
    panning: f64,
    fade_in_ms: u64,
) -> StaticSoundSettings {
    let mut settings = StaticSoundSettings::default()
        .volume(volume.clamp(0.0, 1.0))
        .playback_rate(playback_rate.clamp(0.01, 16.0))
        .panning(panning.clamp(0.0, 1.0));
    if fade_in_ms > 0 {
        settings = settings.fade_in_tween(Tween {
            duration: Duration::from_millis(fade_in_ms),
            ..Tween::default()
        });
    }
    settings
}

fn apply_decode_gain(data: &mut StaticSoundData, decode_gain: f64) {
    let decode_gain = decode_gain.max(0.0) as f32;
    if (decode_gain - 1.0).abs() <= f32::EPSILON {
        return;
    }
    data.frames = data
        .frames
        .iter()
        .map(|frame| {
            Frame::new(
                (frame.left * decode_gain).clamp(-1.0, 1.0),
                (frame.right * decode_gain).clamp(-1.0, 1.0),
            )
        })
        .collect::<Vec<_>>()
        .into();
}

#[cfg(test)]
mod tests {
    use super::*;
    use kira::manager::backend::mock::{MockBackend, MockBackendSettings};

    /// 10007 frames of a 440 Hz sine at half scale, 44.1 kHz stereo, encoded
    /// with libvorbis's vorbisenc (VBR quality 0, one audio page). The last
    /// page's granule position is 10007; the last block ends past it.
    const TONE_OGG: &[u8] = include_bytes!("../testdata/sine-440hz-10007-frames.ogg");
    const TONE_FRAMES: usize = 10007;

    /// The 64-byte BURIKO wave box header around an Ogg payload.
    fn buriko_wave_box(samples: u32, payload: &[u8]) -> Vec<u8> {
        let mut data = vec![0; 0x40];
        data[0..4].copy_from_slice(&0x40u32.to_le_bytes());
        data[4..8].copy_from_slice(b"bw  ");
        data[8..12].copy_from_slice(&(payload.len() as u32).to_le_bytes());
        data[12..16].copy_from_slice(&samples.to_le_bytes());
        data[16..20].copy_from_slice(&44_100u32.to_le_bytes());
        data[20..24].copy_from_slice(&2u32.to_le_bytes());
        data.extend_from_slice(payload);
        data
    }

    fn untrimmed_tone() -> StaticSoundData {
        StaticSoundData::from_cursor(Cursor::new(TONE_OGG), StaticSoundSettings::default()).unwrap()
    }

    #[test]
    fn a_wave_box_sound_decodes_to_its_header_sample_count() {
        let untrimmed = untrimmed_tone();
        assert!(untrimmed.frames.len() > TONE_FRAMES);

        let data = decode_static_sound(
            &buriko_wave_box(TONE_FRAMES as u32, TONE_OGG),
            StaticSoundSettings::default(),
        )
        .unwrap();
        assert_eq!(data.frames.len(), TONE_FRAMES);
        assert_eq!(data.frames[..], untrimmed.frames[..TONE_FRAMES]);
    }

    #[test]
    fn the_header_sample_count_bounds_a_wave_box_sound() {
        let shorter = decode_static_sound(
            &buriko_wave_box(4_000, TONE_OGG),
            StaticSoundSettings::default(),
        )
        .unwrap();
        assert_eq!(shorter.frames.len(), 4_000);

        // A count past the decoded frames (a stream with missing pages)
        // adds nothing.
        let longer = decode_static_sound(
            &buriko_wave_box(1_000_000, TONE_OGG),
            StaticSoundSettings::default(),
        )
        .unwrap();
        assert_eq!(longer.frames.len(), untrimmed_tone().frames.len());
    }

    #[test]
    fn a_bare_ogg_sound_decodes_to_its_end_position() {
        assert_eq!(ogg_end_position(TONE_OGG), Some(TONE_FRAMES as u64));
        let data = decode_static_sound(TONE_OGG, StaticSoundSettings::default()).unwrap();
        assert_eq!(data.frames.len(), TONE_FRAMES);
    }

    #[test]
    fn an_ogg_stream_without_a_whole_last_page_has_no_end_position() {
        assert_eq!(ogg_end_position(&TONE_OGG[..TONE_OGG.len() - 1]), None);
        assert_eq!(ogg_end_position(b"OggS"), None);
    }

    fn mock_audio(capacity: usize) -> AudioSystem<MockBackend> {
        let mut settings = AudioManagerSettings::default();
        settings.backend_settings = MockBackendSettings { sample_rate: 1000 };
        settings.capacities.command_capacity = capacity;
        AudioSystem::with_settings(settings).unwrap()
    }

    fn process(audio: &mut AudioSystem<MockBackend>, frames: usize) -> Vec<Frame> {
        let backend = audio.manager.backend_mut();
        backend.on_start_processing();
        (0..frames).map(|_| backend.process()).collect()
    }

    fn start_tone(audio: &mut AudioSystem<MockBackend>, channel: i32) {
        audio
            .start_static_sound(
                channel,
                StaticSoundData {
                    sample_rate: 1000,
                    frames: vec![Frame::new(0.5, 0.5); 10000].into(),
                    settings: StaticSoundSettings::default(),
                },
            )
            .unwrap();
    }

    #[test]
    fn host_pause_preserves_position_and_does_not_resume_script_pause() {
        let mut audio = mock_audio(128);
        start_tone(&mut audio, 1);
        start_tone(&mut audio, 2);
        process(&mut audio, 100);
        audio.try_pause_channel(2, true, 0).unwrap();
        process(&mut audio, 10);
        audio.host_suspend().unwrap();
        let silent = process(&mut audio, 100);
        assert!(
            silent
                .iter()
                .all(|frame| frame.left == 0.0 && frame.right == 0.0)
        );
        let held_position = audio.channels[&1].position();
        process(&mut audio, 100);
        assert_eq!(audio.channels[&1].position(), held_position);
        assert!(audio.is_host_suspended());
        audio.host_resume().unwrap();
        process(&mut audio, 100);
        process(&mut audio, 1); // publish positions at the next processing block
        assert!(audio.channels[&1].position() > held_position);
        assert_eq!(audio.channels[&2].state(), PlaybackState::Paused);
        assert!(!audio.is_host_suspended());
    }

    #[test]
    fn sounds_created_while_suspended_wait_for_mixer_resume() {
        let mut audio = mock_audio(128);
        audio.host_suspend().unwrap();
        start_tone(&mut audio, 1);
        assert!(
            process(&mut audio, 100)
                .iter()
                .all(|frame| frame.left == 0.0)
        );
        assert_eq!(audio.channels[&1].position(), 0.0);
        audio.host_resume().unwrap();
        assert!(
            process(&mut audio, 100)
                .iter()
                .any(|frame| frame.left > 0.0)
        );
    }

    #[test]
    fn command_failure_keeps_host_state_retryable_and_idempotent() {
        let mut audio = mock_audio(1);
        audio.try_set_volume(0.5).unwrap(); // fill the real Kira command queue
        assert!(audio.host_suspend().is_err());
        assert!(!audio.is_host_suspended());
        process(&mut audio, 1);
        audio.host_suspend().unwrap();
        audio.host_suspend().unwrap(); // no duplicate command
        assert!(audio.host_resume().is_err());
        assert!(audio.is_host_suspended());
        process(&mut audio, 1);
        audio.host_resume().unwrap();
        assert!(!audio.is_host_suspended());
    }

    #[test]
    fn master_volume_changes_existing_and_future_sounds() {
        let mut audio = mock_audio(128);
        start_tone(&mut audio, 1);
        assert!(process(&mut audio, 10).iter().any(|frame| frame.left > 0.0));
        audio.try_set_volume(0.0).unwrap();
        start_tone(&mut audio, 2);
        assert!(
            process(&mut audio, 100)
                .iter()
                .all(|frame| frame.left.abs() < 0.0001)
        );
        assert_eq!(audio.volume(), 0.0);
        assert!(audio.try_set_volume(f64::NAN).is_err());
    }

    #[test]
    fn settings_apply_and_invalid_playback_parameters_fail_before_decode() {
        let mut audio = mock_audio(128);
        assert!(
            audio
                .play_on_channel(1, b"bad", false, f64::NAN, 1.0, 1.0, 0.5, 0)
                .unwrap_err()
                .to_string()
                .contains("non-finite")
        );
        let settings = playback_settings(0.7, 1.5, 0.25, 120);
        assert_eq!(
            settings.volume,
            kira::tween::Value::Fixed(kira::Volume::Amplitude(0.7))
        );
        assert_eq!(
            settings.playback_rate,
            kira::tween::Value::Fixed(kira::sound::PlaybackRate::Factor(1.5))
        );
        assert_eq!(settings.panning, kira::tween::Value::Fixed(0.25));
        assert_eq!(
            settings.fade_in_tween.unwrap().duration,
            Duration::from_millis(120)
        );
    }

    struct FailedBackend;

    impl Backend for FailedBackend {
        type Settings = ();
        type Error = &'static str;

        fn setup(_: ()) -> std::result::Result<(Self, u32), Self::Error> {
            Err("test device unavailable")
        }

        fn start(
            &mut self,
            _: kira::manager::backend::Renderer,
        ) -> std::result::Result<(), Self::Error> {
            unreachable!()
        }
    }

    #[test]
    fn initialization_failure_is_reported_not_silent_success() {
        let result = AudioSystem::<FailedBackend>::with_settings(AudioManagerSettings::default());
        assert!(
            result
                .err()
                .unwrap()
                .to_string()
                .contains("test device unavailable")
        );
    }
}
