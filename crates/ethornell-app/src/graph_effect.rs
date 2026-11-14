use std::collections::{BTreeMap, VecDeque};
use std::sync::Arc;

use ethornell_image::DecodedImage;
use na_mpeg2_decoder::{
    Decoder, Frame, StreamType, frame_to_rgba_bt601_limited, packet_ranges,
};

use crate::MovieBytes;
use crate::audio_runtime::{AudioCommand, StreamBytes};
use crate::movie_container::ContainerMovieDecoder;

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
    source: Option<MovieBytes>,
    packets: Vec<MoviePacket>,
    /// The MPEG audio elementary stream (Layer II in BGI movies), played as
    /// one stream by the audio frontend.
    audio: Option<StreamBytes>,
    decoder: Decoder,
    container: Option<ContainerMovieDecoder>,
    pending_frames: VecDeque<Arc<Frame>>,
    packet_index: usize,
    frame_index: Option<usize>,
    current_frame: Option<Arc<Frame>>,
    container_current_frame: Option<DecodedImage>,
    /// Index of the frame last handed to the bitmap. The target's renderer
    /// (DCVideoImageSender, sub_45C260) copies each sample once.
    delivered_index: Option<usize>,
    frame_interval_ms: i32,
    /// Picture size from the first sequence header.
    size: Option<(u32, u32)>,
    duration_ms: i32,
    flushed: bool,
    failed: bool,
}

#[derive(Debug)]
struct MoviePacket {
    pts_90k: Option<i64>,
    range: std::ops::Range<usize>,
}

#[derive(Debug)]
enum DecodedMovieFrame {
    Mpeg(Arc<Frame>),
    Image(DecodedImage),
}

impl MovieDecoder {
    fn new(source: MovieBytes, resource: &str) -> Option<Self> {
        if ContainerMovieDecoder::supports_resource(resource)
            || ContainerMovieDecoder::probe(source.as_slice())
        {
            let mut container = ContainerMovieDecoder::new(source.as_slice(), resource)?;
            let duration_ms = container.duration_ms();
            let frame_interval_ms = container.frame_interval_ms();
            let size = container.size();
            let audio = container.take_audio();
            return Some(Self {
                source: None,
                packets: Vec::new(),
                audio,
                decoder: Decoder::new(),
                container: Some(container),
                pending_frames: VecDeque::new(),
                packet_index: 0,
                frame_index: None,
                current_frame: None,
                container_current_frame: None,
                delivered_index: None,
                frame_interval_ms,
                size,
                duration_ms,
                flushed: false,
                failed: false,
            });
        }

        let bytes = source.as_slice();
        let duration_ms = probe_mpeg_duration_ms(bytes);
        let mut audio = Vec::new();

        // MPEG Program Stream splits one picture's elementary-stream data
        // across many small PES packets.  The video decoder needs the
        // reassembled ES so that start-code pairs (which delimit slices)
        // are not broken by PES boundaries.  Concatenate all video PES
        // payloads into one contiguous buffer before chunking for decode.
        let mut video_es = Vec::new();
        let mut first_video_pts = None;
        for packet in packet_ranges(bytes) {
            match packet.stream_type {
                StreamType::MpegVideo => {
                    if first_video_pts.is_none() {
                        first_video_pts = packet.pts_90k;
                    }
                    video_es.extend_from_slice(&bytes[packet.range]);
                }
                StreamType::MpegAudio => {
                    audio.extend_from_slice(&bytes[packet.range]);
                }
                StreamType::Unknown => {}
            }
        }

        if video_es.is_empty() {
            return None;
        }

        let video_es = MovieBytes::from_vec(video_es);
        let es_bytes = video_es.as_slice();

        // Decode at most one bounded chunk when another frame is needed,
        // even when the reassembled ES is one enormous buffer.
        let mut packets = Vec::new();
        for start in (0..es_bytes.len()).step_by(64 * 1024) {
            packets.push(MoviePacket {
                pts_90k: if start == 0 { first_video_pts } else { None },
                range: start..(start + 64 * 1024).min(es_bytes.len()),
            });
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
            .decode_shared(&es_bytes[first.range.clone()], first.pts_90k)
            .ok()?
            .into_iter()
            .collect();
        let frame_rate = packets
            .iter()
            .find_map(|packet| sequence_frame_rate(&es_bytes[packet.range.clone()]))
            .unwrap_or(25.0);
        let size = packets
            .iter()
            .find_map(|packet| sequence_frame_size(&es_bytes[packet.range.clone()]));
        Some(Self {
            source: Some(video_es),
            packets,
            audio: (!audio.is_empty()).then(|| StreamBytes(audio.into())),
            decoder,
            container: None,
            pending_frames,
            packet_index: 1,
            frame_index: None,
            current_frame: None,
            container_current_frame: None,
            delivered_index: None,
            frame_interval_ms: (1000.0 / frame_rate).round().max(1.0) as i32,
            size,
            duration_ms,
            flushed: false,
            failed: false,
        })
    }

    fn reset(&mut self) {
        if let Some(container) = self.container.as_mut() {
            let _ = container.reset();
        } else {
            self.decoder = Decoder::new();
        }
        self.pending_frames.clear();
        self.packet_index = 0;
        self.frame_index = None;
        self.current_frame = None;
        self.container_current_frame = None;
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
            match self.next_frame() {
                Some(DecodedMovieFrame::Mpeg(frame)) => {
                    self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
                    self.current_frame = Some(frame);
                }
                Some(DecodedMovieFrame::Image(frame)) => {
                    self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
                    self.container_current_frame = Some(frame);
                }
                None => break,
            }
        }
        if self.container.is_some() {
            let frame = self.container_current_frame.as_ref()?;
            if self.delivered_index == self.frame_index {
                return None;
            }
            self.delivered_index = self.frame_index;
            return Some(frame.clone());
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

    fn next_frame(&mut self) -> Option<DecodedMovieFrame> {
        if let Some(container) = self.container.as_mut() {
            return container.next_frame().map(DecodedMovieFrame::Image);
        }
        loop {
            if let Some(frame) = self.pending_frames.pop_front() {
                return Some(DecodedMovieFrame::Mpeg(frame));
            }
            if self.failed {
                return None;
            }
            if let Some(packet) = self.packets.get(self.packet_index) {
                self.packet_index += 1;
                let data = &self.source.as_ref()?.as_slice()[packet.range.clone()];
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
        let Some(movie) = MovieDecoder::new(bytes, &resource) else {
            return 2;
        };
        let duration_ms = movie.duration_ms;
        if crate::file_policy::restricted() && duration_ms <= 0 {
            // Do not install a permanently active zero-duration process and
            // leave its native wait procedure blocked forever.
            return 2;
        }
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
        let movie = MovieDecoder::new(MovieBytes::from_vec(bytes), "m.mpg").unwrap();
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
    fn container_movie_releases_original_input_buffer_after_demux() {
        let mut bytes = vec![0; 45];
        bytes[..4].copy_from_slice(b"DKIF");
        bytes[6..8].copy_from_slice(&32_u16.to_le_bytes());
        bytes[8..12].copy_from_slice(b"AV01");
        bytes[12..14].copy_from_slice(&1_u16.to_le_bytes());
        bytes[14..16].copy_from_slice(&1_u16.to_le_bytes());
        bytes[16..20].copy_from_slice(&30_u32.to_le_bytes());
        bytes[20..24].copy_from_slice(&1_u32.to_le_bytes());
        bytes[24..28].copy_from_slice(&1_u32.to_le_bytes());
        bytes[32..36].copy_from_slice(&1_u32.to_le_bytes());
        bytes[44] = 0;

        let movie = MovieDecoder::new(MovieBytes::from_vec(bytes), "memory-test.ivf")
            .expect("open IVF container");
        assert!(movie.container.is_some());
        assert!(movie.source.is_none());
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

    /// Count pixels with real colour. An opaque alpha channel alone must never
    /// satisfy a playback assertion: a fully black picture carries alpha 255.
    fn visible_rgb_pixels(frame: &DecodedImage) -> usize {
        frame
            .rgba
            .chunks_exact(4)
            .filter(|pixel| pixel[0] > 8 || pixel[1] > 8 || pixel[2] > 8)
            .count()
    }

    /// The container backend must not capture the MPEG movies the existing
    /// games play: neither their resource names nor their bytes may select it,
    /// and their pictures must keep arriving through the MPEG decoder.
    #[test]
    fn mpeg_movies_keep_playing_when_requested() {
        let Ok(game) = std::env::var("ETHORNELL_MPEG_TEST_GAME") else {
            return;
        };
        let spec = std::env::var("ETHORNELL_MPEG_TEST_ENTRIES").expect("archive|resource entries");
        let manager = ethornell_archive::ResourceManager::open_game(game).expect("open game");
        for item in spec.split(';').filter(|item| !item.is_empty()) {
            let (archive, resource) = item.split_once('|').expect("archive|resource item");
            let bytes = manager
                .read_decoded_from_archive(archive, resource)
                .expect("read archive movie entry");
            assert!(
                !ContainerMovieDecoder::supports_resource(resource),
                "{resource}: MPEG resource name must not select a container"
            );
            assert!(
                !ContainerMovieDecoder::probe(&bytes),
                "{resource}: MPEG bytes must not probe as a container"
            );
            let mut registry = GraphEffectRegistry::default();
            assert_eq!(
                registry.configure_media(
                    7,
                    archive.to_owned(),
                    resource.to_owned(),
                    MovieBytes::from_vec(bytes),
                ),
                0,
                "{resource}: MPEG movie failed to open"
            );
            assert!(registry.movie_size(7).is_some(), "{resource}: no picture size");
            assert_eq!(registry.invoke(7).status, 0);
            let mut frames = 0usize;
            let mut visible = 0usize;
            for _ in 0..90 {
                for update in registry.tick_movies(33) {
                    frames += 1;
                    visible = visible.max(visible_rgb_pixels(&update.image));
                }
            }
            eprintln!("MPEG {resource}: frames={frames} visible={visible}");
            assert!(frames > 1, "{resource}: MPEG playback stalled after one frame");
            assert!(visible > 0, "{resource}: MPEG pictures stayed black");
        }
    }

    #[test]
    fn archive_movie_frames_report_visible_content_when_requested() {
        let Ok(game) = std::env::var("ETHORNELL_FRAME_SCAN_GAME") else {
            return;
        };
        let spec = std::env::var("ETHORNELL_FRAME_SCAN_ENTRY").expect("archive|resource");
        let frames: usize = std::env::var("ETHORNELL_FRAME_SCAN_COUNT")
            .ok()
            .and_then(|value| value.parse().ok())
            .unwrap_or(60);
        let (archive, resource) = spec.split_once('|').expect("archive|resource");
        let manager = ethornell_archive::ResourceManager::open_game(game).expect("open game");
        let bytes = manager
            .read_decoded_from_archive(archive, resource)
            .expect("read archive entry");
        let mut decoder = ContainerMovieDecoder::new(&bytes, resource).expect("open movie");
        let frame_interval_ms = decoder.frame_interval_ms();
        let duration_ms = decoder.duration_ms();
        eprintln!("INTERVAL {frame_interval_ms} ms DURATION {duration_ms} ms");
        assert!(
            (1..=100).contains(&frame_interval_ms),
            "frame interval {frame_interval_ms} ms cannot pace video playback"
        );
        let mut decoded = 0usize;
        let mut visible_frames = 0usize;
        for index in 0..frames {
            let Some(frame) = decoder.next_frame() else {
                eprintln!("FRAME {index}: decoder returned none");
                break;
            };
            decoded += 1;
            let visible = visible_rgb_pixels(&frame);
            if visible > 0 {
                visible_frames += 1;
            }
            if index % 10 == 0 || visible > 0 {
                eprintln!("FRAME {index}: {}x{} visible={visible}", frame.width, frame.height);
            }
        }
        eprintln!("DECODED {decoded} frames, {visible_frames} with visible colour");
        assert!(decoded > 1, "decoder must advance past the first frame");
        assert!(visible_frames > 0, "no decoded frame carried visible colour");
    }

    #[test]
    fn activated_arc_movie_samples_decode_when_requested() {
        let Ok(game) = std::env::var("ETHORNELL_ARC_TEST_GAME") else {
            return;
        };
        let spec = std::env::var("ETHORNELL_ARC_TEST_ENTRIES")
            .expect("archive|resource test entries");
        let manager = ethornell_archive::ResourceManager::open_game(game).expect("open test game");
        for item in spec.split(';').filter(|item| !item.is_empty()) {
            let (archive, resource) = item.split_once('|').expect("archive|resource test item");
            let bytes = manager
                .read_decoded_from_archive(archive, resource)
                .expect("read archive movie entry");
            let mut decoder = ContainerMovieDecoder::new(&bytes, resource)
                .expect("open archive movie entry");
            let frame_interval_ms = decoder.frame_interval_ms();
            assert!(
                (1..=100).contains(&frame_interval_ms),
                "{resource}: frame interval {frame_interval_ms} ms cannot pace playback"
            );
            // A movie may open on a black fade-in, so require colour within
            // the first second of pictures rather than on the first frame.
            let mut visible = 0usize;
            let mut decoded = 0usize;
            for _ in 0..30 {
                let Some(frame) = decoder.next_frame() else {
                    break;
                };
                decoded += 1;
                assert!(frame.width > 0 && frame.height > 0);
                visible = visible.max(visible_rgb_pixels(&frame));
            }
            assert!(decoded > 1, "{resource}: decoder stalled after one frame");
            assert!(visible > 0, "{resource}: decoded pictures stayed black");
            assert!(decoder.take_audio().is_some(), "archive AAC track should be retained");
        }
    }

    #[test]
    fn external_container_samples_decode_when_requested() {
        let Ok(spec) = std::env::var("ETHORNELL_CONTAINER_TESTS") else {
            return;
        };
        for item in spec.split(';').filter(|item| !item.is_empty()) {
            let (path, resource) = item.split_once('|').expect("path|resource test item");
            let bytes = std::fs::read(path).expect("read container test sample");
            let mut decoder = ContainerMovieDecoder::new(&bytes, resource)
                .expect("open container test sample");
            let frame = decoder.next_frame().expect("decode container test frame");
            assert!(frame.width > 0 && frame.height > 0);
            assert!(visible_rgb_pixels(&frame) > 0, "{resource}: first picture is black");
        }
    }

    #[test]
    fn external_mp4_sample_decodes_when_requested() {
        let Ok(path) = std::env::var("ETHORNELL_MP4_TEST") else {
            return;
        };
        let bytes = std::fs::read(path).expect("read ETHORNELL_MP4_TEST");
        let mut decoder = ContainerMovieDecoder::new(&bytes, "sample.mpg")
            .expect("open MP4 sample");
        assert_eq!(decoder.size(), Some((1280, 720)));
        assert!(decoder.duration_ms() > 100_000);
        let frame_interval_ms = decoder.frame_interval_ms();
        assert!(
            (1..=100).contains(&frame_interval_ms),
            "frame interval {frame_interval_ms} ms cannot pace playback"
        );
        let frame = decoder.next_frame().expect("decode first MP4 frame");
        assert_eq!((frame.width, frame.height), (1280, 720));
        assert!(decoder.take_audio().is_some(), "AAC track should be retained");
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
