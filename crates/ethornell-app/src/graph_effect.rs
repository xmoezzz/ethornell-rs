use std::collections::{BTreeMap, VecDeque};
use std::sync::Arc;

use ethornell_image::DecodedImage;
use na_mpeg2_decoder::{Decoder, Frame, StreamType, frame_to_rgba_bt601_limited, packet_ranges};

use crate::MovieBytes;
use crate::audio_runtime::{AudioCommand, StreamBytes};

/// Audio channels for movie soundtracks: above the channels script sounds use.
const MOVIE_AUDIO_CHANNEL_BASE: i32 = 0x4000_0000;

pub(crate) fn movie_audio_channel(handle: i32) -> i32 {
    MOVIE_AUDIO_CHANNEL_BASE.saturating_add(handle.max(0))
}

/// What a movie's soundtrack should be doing, for the audio frontend.
#[derive(Debug, Clone)]
pub(crate) struct MovieAudio {
    pub(crate) handle: i32,
    /// Changes whenever the soundtrack has to (re)start: invoke, seek, loop.
    pub(crate) generation: u64,
    pub(crate) active: bool,
    pub(crate) paused: bool,
    /// Native 0..=128 movie volume.
    pub(crate) volume: i32,
    /// The picture's position, where a (re)started soundtrack begins.
    pub(crate) position_ms: i32,
    pub(crate) bytes: StreamBytes,
}

/// A movie soundtrack's state as last sent to the audio frontend.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct MovieAudioSent {
    generation: u64,
    paused: bool,
    volume: i32,
}

/// The audio commands that bring the frontend in line with the movies'
/// soundtracks: start one when its movie starts, seeks or loops, pause and
/// resume it and give it the movie's volume, stop it when the movie stops,
/// ends or is closed. `sent` remembers what was sent last, per movie handle.
pub(crate) fn movie_audio_commands(
    movies: Vec<MovieAudio>,
    sent: &mut BTreeMap<i32, MovieAudioSent>,
) -> Vec<AudioCommand> {
    let mut commands = Vec::new();
    let mut present = Vec::with_capacity(movies.len());
    for movie in movies {
        present.push(movie.handle);
        let channel = movie_audio_channel(movie.handle);
        let volume = crate::native_audio_volume(movie.volume);
        if !movie.active {
            if sent.remove(&movie.handle).is_some() {
                commands.push(AudioCommand::Stop {
                    channel,
                    fade_ms: 0,
                });
            }
            continue;
        }
        match sent.get(&movie.handle) {
            Some(last) if last.generation == movie.generation => {
                if last.paused != movie.paused {
                    commands.push(AudioCommand::Pause {
                        channel,
                        paused: movie.paused,
                        fade_ms: 0,
                    });
                }
                if last.volume != movie.volume {
                    commands.push(AudioCommand::SetVolume {
                        channel,
                        volume,
                        fade_ms: 0,
                    });
                }
            }
            _ => {
                commands.push(AudioCommand::PlayStream {
                    channel,
                    bytes: movie.bytes.clone(),
                    volume,
                    start_ms: movie.position_ms.max(0) as u64,
                });
                if movie.paused {
                    commands.push(AudioCommand::Pause {
                        channel,
                        paused: true,
                        fade_ms: 0,
                    });
                }
            }
        }
        sent.insert(
            movie.handle,
            MovieAudioSent {
                generation: movie.generation,
                paused: movie.paused,
                volume: movie.volume,
            },
        );
    }
    sent.retain(|handle, _| {
        let keep = present.contains(handle);
        if !keep {
            commands.push(AudioCommand::Stop {
                channel: movie_audio_channel(*handle),
                fade_ms: 0,
            });
        }
        keep
    });
    commands
}

#[derive(Debug)]
struct MovieDecoder {
    source: MovieBytes,
    packets: Vec<MoviePacket>,
    /// The MPEG audio elementary stream (Layer II in BGI movies), played as
    /// one stream by the audio frontend.
    audio: Option<StreamBytes>,
    decoder: Decoder,
    pending_frames: VecDeque<Arc<Frame>>,
    packet_index: usize,
    frame_index: Option<usize>,
    current_frame: Option<Arc<Frame>>,
    /// Index of the frame last handed to the bitmap. The target's renderer
    /// (DCVideoImageSender, sub_45C260) copies each sample once.
    delivered_index: Option<usize>,
    frame_interval_ms: i32,
    /// Picture size from the first sequence header.
    size: Option<(u32, u32)>,
    flushed: bool,
    failed: bool,
}

#[derive(Debug)]
struct MoviePacket {
    pts_90k: Option<i64>,
    range: std::ops::Range<usize>,
}

impl MovieDecoder {
    fn new(source: MovieBytes) -> Option<Self> {
        let bytes = source.as_slice();
        let mut packets = Vec::new();
        let mut audio = Vec::new();
        for packet in packet_ranges(bytes) {
            match packet.stream_type {
                StreamType::MpegVideo => {
                    // Decode at most one bounded chunk when another frame is
                    // needed, even when the container has one enormous PES.
                    for start in (packet.range.start..packet.range.end).step_by(64 * 1024) {
                        packets.push(MoviePacket {
                            pts_90k: (start == packet.range.start)
                                .then_some(packet.pts_90k)
                                .flatten(),
                            range: start..(start + 64 * 1024).min(packet.range.end),
                        });
                    }
                }
                StreamType::MpegAudio => {
                    audio.extend_from_slice(&bytes[packet.range]);
                }
                StreamType::Unknown => {}
            }
        }
        if packets.is_empty() {
            return None;
        }
        // A file that isn't a movie (Graph90:F0 names movies by file, so a
        // script can name an image archive) can still pass the demuxer as one
        // video elementary stream through stray start codes in its data. Its
        // first packet doesn't decode: fail the open instead of playing
        // nothing for the duration the stray codes add up to.
        let mut decoder = Decoder::new();
        let first = &packets[0];
        let pending_frames = decoder
            .decode_shared(&bytes[first.range.clone()], first.pts_90k)
            .ok()?
            .into_iter()
            .collect();
        let frame_rate = packets
            .iter()
            .find_map(|packet| sequence_frame_rate(&bytes[packet.range.clone()]))
            .unwrap_or(25.0);
        let size = packets
            .iter()
            .find_map(|packet| sequence_frame_size(&bytes[packet.range.clone()]));
        Some(Self {
            source,
            packets,
            audio: (!audio.is_empty()).then(|| StreamBytes(audio.into())),
            decoder,
            pending_frames,
            packet_index: 1,
            frame_index: None,
            current_frame: None,
            delivered_index: None,
            frame_interval_ms: (1000.0 / frame_rate).round().max(1.0) as i32,
            size,
            flushed: false,
            failed: false,
        })
    }

    fn reset(&mut self) {
        self.decoder = Decoder::new();
        self.pending_frames.clear();
        self.packet_index = 0;
        self.frame_index = None;
        self.current_frame = None;
        self.delivered_index = None;
        self.flushed = false;
        self.failed = false;
    }

    fn frame_at(&mut self, position_ms: i32) -> Option<DecodedImage> {
        let target = (position_ms.max(0) / self.frame_interval_ms.max(1)) as usize;
        if self.frame_index.is_some_and(|index| target < index) {
            self.reset();
        }
        while self.frame_index.is_none_or(|index| index < target) {
            if let Some(frame) = self.next_frame() {
                self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
                self.current_frame = Some(frame);
            } else {
                break;
            }
        }
        let frame = self.current_frame.as_ref()?;
        if self.delivered_index == self.frame_index {
            return None;
        }
        self.delivered_index = self.frame_index;
        let mut rgba = vec![0; frame.width * frame.height * 4];
        frame_to_rgba_bt601_limited(frame, &mut rgba);
        Some(DecodedImage {
            width: frame.width as u32,
            height: frame.height as u32,
            rgba,
        })
    }

    fn prepare_initial_frame(&mut self) -> Option<DecodedImage> {
        self.frame_at(0)
    }

    fn next_frame(&mut self) -> Option<Arc<Frame>> {
        loop {
            if let Some(frame) = self.pending_frames.pop_front() {
                return Some(frame);
            }
            if self.failed {
                return None;
            }
            if let Some(packet) = self.packets.get(self.packet_index) {
                self.packet_index += 1;
                let data = &self.source.as_slice()[packet.range.clone()];
                match self.decoder.decode_shared(data, packet.pts_90k) {
                    Ok(frames) => self.pending_frames.extend(frames),
                    Err(_) => self.failed = true,
                }
                continue;
            }
            if !self.flushed {
                self.flushed = true;
                match self.decoder.flush_shared() {
                    Ok(frames) => self.pending_frames.extend(frames),
                    Err(_) => self.failed = true,
                }
                continue;
            }
            return None;
        }
    }
}

#[derive(Debug)]
pub(crate) struct MovieFrameUpdate {
    pub(crate) bitmap: i32,
    pub(crate) image: DecodedImage,
}

#[derive(Debug)]
struct GraphEffectProcess {
    archive: String,
    resource: String,
    parameters: [i32; 4],
    duration_ms: i32,
    active: bool,
    paused: bool,
    looping: bool,
    position_ms: i32,
    volume: i32,
    result: i32,
    movie: Option<MovieDecoder>,
    audio_generation: u64,
}

#[derive(Debug, Default)]
pub(crate) struct GraphEffectRegistry {
    processes: BTreeMap<i32, GraphEffectProcess>,
}

impl GraphEffectRegistry {
    pub(crate) fn contains(&self, handle: i32) -> bool {
        self.processes.contains_key(&handle)
    }

    pub(crate) fn configure_loader(&mut self, handle: i32, mode: i32, values: [i32; 4]) -> i32 {
        if handle < 0 {
            return 4;
        }
        self.processes.insert(
            handle,
            GraphEffectProcess {
                archive: String::new(),
                resource: if mode == 0 {
                    "DCProcLoadBurikoMV".into()
                } else {
                    "DCProcLoadBMVHeader".into()
                },
                parameters: values,
                duration_ms: 0,
                active: false,
                paused: false,
                looping: false,
                position_ms: 0,
                volume: 128,
                result: -1,
                movie: None,
                audio_generation: 0,
            },
        );
        0
    }

    pub(crate) fn configure_media(
        &mut self,
        handle: i32,
        archive: String,
        resource: String,
        bytes: MovieBytes,
    ) -> i32 {
        if handle < 0 || resource.is_empty() || bytes.as_slice().is_empty() {
            return 4;
        }
        let duration_ms = probe_mpeg_duration_ms(bytes.as_slice());
        if crate::file_policy::restricted() && duration_ms <= 0 {
            // Do not install a permanently active zero-duration process and
            // leave its native wait procedure blocked forever.
            return 2;
        }
        let Some(movie) = MovieDecoder::new(bytes) else {
            return 2;
        };
        self.processes.insert(
            handle,
            GraphEffectProcess {
                archive,
                resource,
                parameters: [0; 4],
                duration_ms,
                active: false,
                paused: false,
                looping: false,
                position_ms: 0,
                volume: 128,
                result: -1,
                movie: Some(movie),
                audio_generation: 0,
            },
        );
        0
    }

    pub(crate) fn configure_resource(
        &mut self,
        handle: i32,
        resource: String,
        option1: i32,
        option2: i32,
    ) -> i32 {
        if handle < 0 {
            return 4;
        }
        self.processes.insert(
            handle,
            GraphEffectProcess {
                archive: String::new(),
                resource,
                parameters: [option1, option2, 0, 0],
                duration_ms: 0,
                active: false,
                paused: false,
                looping: false,
                position_ms: 0,
                volume: 128,
                result: -1,
                movie: None,
                audio_generation: 0,
            },
        );
        0
    }

    pub(crate) fn configure_rect(
        &mut self,
        handle: i32,
        descriptor: i32,
        parameters: [i32; 3],
    ) -> i32 {
        if handle < 0 {
            return 4;
        }
        self.processes.insert(
            handle,
            GraphEffectProcess {
                archive: String::new(),
                resource: String::new(),
                parameters: [descriptor, parameters[0], parameters[1], parameters[2]],
                duration_ms: 0,
                active: false,
                paused: false,
                looping: false,
                position_ms: 0,
                volume: 128,
                result: -1,
                movie: None,
                audio_generation: 0,
            },
        );
        0
    }

    pub(crate) fn invoke(&mut self, handle: i32) -> ethornell_vm::GraphEffectInvocation {
        let Some(process) = self.processes.get_mut(&handle) else {
            return ethornell_vm::GraphEffectInvocation {
                status: 4,
                duration_ms: 0,
            };
        };
        process.active = true;
        process.paused = false;
        process.position_ms = 0;
        process.result = -1;
        process.audio_generation += 1;
        if let Some(movie) = process.movie.as_mut() {
            movie.reset();
        }
        ethornell_vm::GraphEffectInvocation {
            status: 0,
            duration_ms: process.duration_ms,
        }
    }

    pub(crate) fn cancel(&mut self, handle: i32) -> i32 {
        let Some(process) = self.processes.get_mut(&handle) else {
            return 4;
        };
        if !process.active {
            return 1;
        }
        process.active = false;
        0
    }

    pub(crate) fn state(&self, handle: i32) -> i32 {
        match self.processes.get(&handle) {
            Some(process) if process.active => 0,
            Some(_) => 1,
            None => 4,
        }
    }

    pub(crate) fn seek(&mut self, handle: i32, position_ms: i32) -> i32 {
        let Some(process) = self.processes.get_mut(&handle) else {
            return 4;
        };
        if process.movie.is_none() {
            return 1;
        }
        let position_ms = position_ms.max(0).min(process.duration_ms.max(0));
        process.position_ms = position_ms;
        process.audio_generation += 1;
        if let Some(movie) = process.movie.as_mut() {
            let _ = movie.frame_at(position_ms);
            // A seek on a running graph renders the sample at the new
            // position again.
            movie.delivered_index = None;
        }
        0
    }

    pub(crate) fn position_ms(&self, handle: i32) -> Result<i32, i32> {
        let Some(process) = self.processes.get(&handle) else {
            return Err(4);
        };
        if process.movie.is_none() {
            return Err(1);
        }
        Ok(process.position_ms)
    }

    pub(crate) fn set_volume(&mut self, handle: i32, volume: i32) -> i32 {
        let Some(process) = self.processes.get_mut(&handle) else {
            return 4;
        };
        if process.movie.is_none() {
            return 1;
        }
        if !(0..=128).contains(&volume) {
            return 3;
        }
        process.volume = volume;
        0
    }

    pub(crate) fn set_movie_options(&mut self, handle: i32, looping: bool, volume: i32) -> i32 {
        if !(0..=128).contains(&volume) {
            return 3;
        }
        let Some(process) = self.processes.get_mut(&handle) else {
            return 4;
        };
        if process.movie.is_none() {
            return 1;
        }
        process.looping = looping;
        process.volume = volume;
        0
    }

    pub(crate) fn set_paused(&mut self, handle: i32, paused: bool) -> i32 {
        if !(0..0x4000).contains(&handle) {
            return 4;
        }
        let Some(process) = self.processes.get_mut(&handle) else {
            return 1;
        };
        if process.movie.is_none() {
            return 1;
        }
        process.paused = paused;
        0
    }

    pub(crate) fn close_movie_slot(&mut self, handle: i32) -> i32 {
        if !(0..0x4000).contains(&handle) {
            return 4;
        }
        match self.processes.get(&handle) {
            Some(process) if process.movie.is_some() => {
                self.processes.remove(&handle);
                0
            }
            _ => 1,
        }
    }

    /// Whether a bitmap-slot movie is running (sub_44DAF0).
    pub(crate) fn movie_playing(&self, handle: i32) -> bool {
        self.processes
            .get(&handle)
            .is_some_and(|process| process.movie.is_some() && process.active && !process.paused)
    }

    /// The picture size the target's renderer (DCVideoImageSender,
    /// sub_45C1C0) gives the bitmap slot when the graph connects.
    pub(crate) fn movie_size(&self, handle: i32) -> Option<(u32, u32)> {
        self.processes.get(&handle)?.movie.as_ref()?.size
    }

    /// Decode the already-buffered opening frame before the target creates a
    /// bitmap slot.  This lets consoles install the real image directly,
    /// avoiding a temporary 1280x720 blank RGBA surface at movie start.
    pub(crate) fn prepare_initial_movie_frame(&mut self, handle: i32) -> Option<MovieFrameUpdate> {
        let movie = self.processes.get_mut(&handle)?.movie.as_mut()?;
        let image = movie.prepare_initial_frame()?;
        Some(MovieFrameUpdate {
            bitmap: handle,
            image,
        })
    }

    pub(crate) fn is_movie(&self, handle: i32) -> bool {
        self.processes
            .get(&handle)
            .is_some_and(|process| process.movie.is_some())
    }

    pub(crate) fn tick_movies(&mut self, elapsed_ms: i32) -> Vec<MovieFrameUpdate> {
        let mut updates = Vec::new();
        for (&bitmap, process) in &mut self.processes {
            if !process.active || process.paused {
                continue;
            }
            let Some(movie) = process.movie.as_mut() else {
                continue;
            };
            process.position_ms = process
                .position_ms
                .saturating_add(elapsed_ms.max(0))
                .min(process.duration_ms.max(0));
            if let Some(image) = movie.frame_at(process.position_ms) {
                updates.push(MovieFrameUpdate { bitmap, image });
            }
            if process.duration_ms > 0 && process.position_ms >= process.duration_ms {
                if process.looping {
                    process.position_ms = 0;
                    process.result = -1;
                    // Restart the soundtrack with the picture rather than
                    // looping it on its own, so the two can't drift apart.
                    process.audio_generation += 1;
                    if let Some(movie) = process.movie.as_mut() {
                        movie.reset();
                    }
                } else {
                    process.active = false;
                    process.result = 0;
                }
            }
        }
        updates
    }

    pub(crate) fn movie_diagnostics(&self) -> Vec<(i32, bool, i32, Option<usize>)> {
        self.processes
            .iter()
            .filter_map(|(&handle, process)| {
                let movie = process.movie.as_ref()?;
                Some((
                    handle,
                    process.active,
                    process.position_ms,
                    movie.frame_index,
                ))
            })
            .collect()
    }

    pub(crate) fn movie_audio(&self) -> Vec<MovieAudio> {
        self.processes
            .iter()
            .filter_map(|(&handle, process)| {
                let bytes = process.movie.as_ref()?.audio.clone()?;
                Some(MovieAudio {
                    handle,
                    generation: process.audio_generation,
                    active: process.active,
                    paused: process.paused,
                    volume: process.volume,
                    position_ms: process.position_ms,
                    bytes,
                })
            })
            .collect()
    }

    pub(crate) fn release(&mut self, handle: i32) -> i32 {
        if self.processes.remove(&handle).is_some() {
            0
        } else {
            4
        }
    }

    pub(crate) fn stop_all_movies(&mut self) {
        for process in self.processes.values_mut() {
            if process.movie.is_some() {
                process.active = false;
                process.result = 0;
            }
        }
    }

    pub(crate) fn set_all_movie_volumes(&mut self, volume: i32) -> i32 {
        if !(0..=128).contains(&volume) {
            return 3;
        }
        for process in self.processes.values_mut() {
            if process.movie.is_some() {
                process.volume = volume;
            }
        }
        0
    }

    pub(crate) fn result(&self, handle: i32) -> Option<i32> {
        self.processes.get(&handle).map(|process| process.result)
    }

    pub(crate) fn duration_ms(&self, handle: i32) -> i32 {
        self.processes
            .get(&handle)
            .map(|process| process.duration_ms)
            .unwrap_or_default()
    }

    pub(crate) fn source(&self, handle: i32) -> Option<(&str, &str)> {
        self.processes
            .get(&handle)
            .map(|process| (process.archive.as_str(), process.resource.as_str()))
    }

    #[cfg(test)]
    pub(crate) fn parameters(&self, handle: i32) -> Option<[i32; 4]> {
        self.processes
            .get(&handle)
            .map(|process| process.parameters)
    }
}

fn probe_mpeg_duration_ms(bytes: &[u8]) -> i32 {
    let packets = packet_ranges(bytes);
    let mut first_pts = None;
    let mut last_pts = None;
    let mut pictures = 0_u64;
    let mut frame_rate = None;

    for packet in packets
        .iter()
        .filter(|packet| packet.stream_type == StreamType::MpegVideo)
    {
        if let Some(pts) = packet.pts_90k {
            first_pts.get_or_insert(pts);
            last_pts = Some(pts);
        }
        pictures += count_start_codes(&bytes[packet.range.clone()], 0x00) as u64;
        frame_rate = frame_rate.or_else(|| sequence_frame_rate(&bytes[packet.range.clone()]));
    }

    let frame_ms = frame_rate
        .map(|fps| (1000.0 / fps).round() as i64)
        .unwrap_or(40);
    let duration = match (first_pts, last_pts) {
        (Some(first), Some(last)) if last > first => {
            ((last - first) * 1000 / 90_000).saturating_add(frame_ms)
        }
        _ if pictures > 0 => {
            ((pictures as f64 * 1000.0) / frame_rate.unwrap_or(25.0)).round() as i64
        }
        _ => 0,
    };
    duration.clamp(0, i32::MAX as i64) as i32
}

fn count_start_codes(bytes: &[u8], code: u8) -> usize {
    bytes
        .windows(4)
        .filter(|window| *window == [0, 0, 1, code])
        .count()
}

fn sequence_frame_rate(bytes: &[u8]) -> Option<f64> {
    const RATES: [f64; 16] = [
        0.0,
        24000.0 / 1001.0,
        24.0,
        25.0,
        30000.0 / 1001.0,
        30.0,
        50.0,
        60000.0 / 1001.0,
        60.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
    ];
    bytes.windows(8).find_map(|window| {
        (window[..4] == [0, 0, 1, 0xB3])
            .then(|| RATES[(window[7] & 0x0F) as usize])
            .filter(|rate| *rate > 0.0)
    })
}

/// Horizontal and vertical size of an MPEG-1/2 sequence header (12 bits each).
fn sequence_frame_size(bytes: &[u8]) -> Option<(u32, u32)> {
    bytes.windows(7).find_map(|window| {
        let width = (u32::from(window[4]) << 4) | u32::from(window[5] >> 4);
        let height = (u32::from(window[5] & 0x0F) << 8) | u32::from(window[6]);
        (window[..4] == [0, 0, 1, 0xB3] && width > 0 && height > 0).then_some((width, height))
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sequence_header_frame_size_is_decoded() {
        // 1280 = 0x500, 720 = 0x2D0.
        let bytes = [0, 0, 1, 0xB3, 0x50, 0x02, 0xD0, 3];
        assert_eq!(sequence_frame_size(&bytes), Some((1280, 720)));
    }

    #[test]
    fn sequence_header_frame_rate_is_decoded() {
        let bytes = [0, 0, 1, 0xB3, 0, 0, 0, 3];
        assert_eq!(sequence_frame_rate(&bytes), Some(25.0));
    }

    #[test]
    fn cancelling_one_effect_does_not_stop_its_peer() {
        let mut registry = GraphEffectRegistry::default();
        assert_eq!(registry.configure_resource(3, "crossfade".into(), 1, 2), 0);
        assert_eq!(registry.configure_resource(4, "mask".into(), 3, 4), 0);
        assert_eq!(registry.invoke(3).status, 0);
        assert_eq!(registry.invoke(4).status, 0);

        assert_eq!(registry.cancel(3), 0);
        assert_eq!(registry.state(3), 1);
        assert_eq!(registry.state(4), 0);
    }

    #[test]
    fn rectangular_effect_preserves_native_arguments() {
        let mut registry = GraphEffectRegistry::default();
        assert_eq!(registry.configure_rect(7, 0x1234, [10, 20, 30]), 0);
        assert_eq!(registry.parameters(7), Some([0x1234, 10, 20, 30]));
    }

    /// A minimal MPEG-1 program stream: a video packet (a sequence header)
    /// and two audio packets, none with a time stamp.
    fn tiny_movie() -> Vec<u8> {
        let mut stream = vec![0, 0, 1, 0xBA, 0x21, 0, 1, 0, 1, 0x80, 0, 1];
        let packets: [(u8, &[u8]); 3] = [
            (0xE0, &[0, 0, 1, 0xB3, 0x14, 0x00, 0xF0, 0x13]),
            (0xC0, b"first"),
            (0xC0, b"second"),
        ];
        for (id, payload) in packets {
            stream.extend_from_slice(&[0, 0, 1, id]);
            stream.extend_from_slice(&(payload.len() as u16 + 1).to_be_bytes());
            stream.push(0x0F);
            stream.extend_from_slice(payload);
        }
        stream.extend_from_slice(&[0, 0, 1, 0xB9]);
        stream
    }

    #[test]
    fn large_elementary_stream_is_decoded_incrementally() {
        // A sequence header followed by padding: startup must not feed the
        // entire input to Decoder, which would allocate a movie-sized ES buffer.
        let mut bytes = vec![0, 0, 1, 0xB3, 0x01, 0x00, 0x10, 0x13, 0, 0, 0x20, 0];
        bytes.extend_from_slice(&[0, 0, 1, 0xB7]);
        bytes.resize(2 * 1024 * 1024, 0);
        let movie = MovieDecoder::new(MovieBytes::from_vec(bytes)).unwrap();
        assert_eq!(movie.packet_index, 1);
        assert!(movie.packets.iter().all(|p| p.range.len() <= 64 * 1024));
        assert!(movie.pending_frames.is_empty());
    }

    #[test]
    fn borrowed_movie_probe_matches_owned_demux() {
        let bytes = tiny_movie();
        let mut demux = na_mpeg2_decoder::Demuxer::new_auto();
        let owned = demux.push(&bytes, None);
        let borrowed = packet_ranges(&bytes);
        assert_eq!(owned.len(), borrowed.len());
        for (owned, borrowed) in owned.iter().zip(borrowed) {
            assert_eq!(owned.stream_type, borrowed.stream_type);
            assert_eq!(owned.pts_90k, borrowed.pts_90k);
            assert_eq!(owned.data, bytes[borrowed.range]);
        }
    }

    fn command_labels(commands: Vec<AudioCommand>) -> Vec<&'static str> {
        commands
            .into_iter()
            .map(|command| match command {
                AudioCommand::PlayStream { .. } => "play",
                AudioCommand::Pause { paused: true, .. } => "pause",
                AudioCommand::Pause { paused: false, .. } => "resume",
                AudioCommand::SetVolume { .. } => "volume",
                AudioCommand::Stop { .. } => "stop",
                _ => "other",
            })
            .collect()
    }

    #[test]
    fn movie_soundtrack_is_kept_and_restarts_with_the_movie() {
        let mut registry = GraphEffectRegistry::default();
        assert_eq!(
            registry.configure_media(
                5,
                "a".into(),
                "m".into(),
                MovieBytes::from_vec(tiny_movie())
            ),
            0
        );
        // 92:F2 sizes the bitmap slot from the sequence header.
        assert_eq!(registry.movie_size(5), Some((320, 240)));
        let audio = registry.movie_audio();
        assert_eq!(audio.len(), 1);
        assert_eq!(&*audio[0].bytes.0, b"firstsecond");
        assert!(!audio[0].active);

        registry.invoke(5);
        let started = registry.movie_audio()[0].generation;
        assert_eq!(registry.seek(5, 0), 0);
        let sought = &registry.movie_audio()[0];
        assert!(sought.active);
        assert!(sought.generation > started);
        assert_eq!(sought.position_ms, 0);
    }

    #[test]
    fn movie_audio_commands_follow_the_movie() {
        let mut registry = GraphEffectRegistry::default();
        registry.configure_media(
            5,
            "a".into(),
            "m".into(),
            MovieBytes::from_vec(tiny_movie()),
        );
        let mut sent = BTreeMap::new();
        let mut step = |registry: &GraphEffectRegistry| {
            command_labels(movie_audio_commands(registry.movie_audio(), &mut sent))
        };

        assert!(
            step(&registry).is_empty(),
            "nothing plays before the movie starts"
        );
        registry.invoke(5);
        assert_eq!(step(&registry), ["play"]);
        assert!(
            step(&registry).is_empty(),
            "no command while nothing changes"
        );
        registry.set_paused(5, true);
        assert_eq!(step(&registry), ["pause"]);
        registry.set_paused(5, false);
        assert_eq!(step(&registry), ["resume"]);
        assert_eq!(registry.set_volume(5, 64), 0);
        assert_eq!(step(&registry), ["volume"]);
        registry.invoke(5);
        assert_eq!(
            step(&registry),
            ["play"],
            "starting again restarts the soundtrack"
        );
        assert_eq!(registry.cancel(5), 0);
        assert_eq!(step(&registry), ["stop"]);
        registry.invoke(5);
        assert_eq!(step(&registry), ["play"]);
        assert_eq!(registry.close_movie_slot(5), 0);
        assert_eq!(
            step(&registry),
            ["stop"],
            "closing the movie stops its soundtrack"
        );
        assert!(step(&registry).is_empty());
    }

    /// What an archive of images looks like to the demuxer: no pack header,
    /// so one video elementary stream, whose compressed data happens to hold
    /// picture start codes but no sequence header before them.
    fn image_archive() -> Vec<u8> {
        let mut bytes = b"BURIKO ARC20".to_vec();
        for chunk in 0..8u8 {
            bytes.extend_from_slice(&[0x5A ^ chunk; 61]);
            bytes.extend_from_slice(&[0, 0, 1, 0x00]);
        }
        bytes
    }

    /// Wonderful Everyday names one movie by an archive of 185 images; the
    /// stray picture start codes in it added up to a 2,903 ms movie that
    /// played nothing.
    #[test]
    fn a_file_whose_first_video_packet_does_not_decode_is_no_movie() {
        let bytes = image_archive();
        assert!(probe_mpeg_duration_ms(&bytes) > 0);
        let mut registry = GraphEffectRegistry::default();
        assert_eq!(
            registry.configure_media(5, String::new(), "m".into(), MovieBytes::from_vec(bytes)),
            2
        );
        assert_eq!(registry.state(5), 4, "no movie process");
        assert!(registry.movie_audio().is_empty());
    }

    #[test]
    fn movie_volume_uses_native_range_and_state_errors() {
        let mut registry = GraphEffectRegistry::default();
        assert_eq!(registry.set_volume(1, 64), 4);
        registry.configure_resource(1, "not-a-movie".into(), 0, 0);
        assert_eq!(registry.set_volume(1, 64), 1);
        assert_eq!(registry.set_volume(1, 129), 1);
    }
}
