//! MP4/H.264, VP9 and AV1 video with AAC-LC audio. The container yields Annex-B samples,
//! including SPS/PPS. Display PTS follows each picture through B-frame reorder;
//! neither average frame duration nor integer milliseconds select pictures.
use std::collections::VecDeque;
use std::fmt;
use std::io::{self, Read, Seek, SeekFrom};
#[cfg(not(target_arch = "wasm32"))]
use std::sync::{Arc, Condvar, Mutex};
#[cfg(not(target_arch = "wasm32"))]
use std::thread::JoinHandle;

use ethornell_image::DecodedImage;
use rff_core::{CodecId, Error as DemuxError, MediaType, Rational};
use rff_format::{Demuxer, FormatRegistry, Stream};
use rust_h264::decoder::Frame;
#[cfg(target_arch = "wasm32")]
use rust_h264::decoder::OrderedDecoder as H264Decoder;
use rust_h264::nal::{NalUnitType, parse_annex_b};
#[cfg(not(target_arch = "wasm32"))]
use rust_h264::threading::ThreadedDecoder as H264Decoder;

use crate::MovieBytes;
use crate::audio_runtime::StreamBytes;

mod codecs;

const MAX_PENDING_FRAMES: usize = 32;
const MAX_AUDIO_BYTES: usize = 32 * 1024 * 1024;
const MAX_FRAME_PIXELS: usize = 4096 * 4096;
#[cfg(not(target_arch = "wasm32"))]
const PREFETCH_QUEUE_CAPACITY: usize = 3;

#[derive(Clone, Copy, Debug)]
struct Color {
    matrix: u8,
    full_range: bool,
}
impl Default for Color {
    fn default() -> Self {
        Self {
            matrix: 2,
            full_range: false,
        }
    }
}
#[derive(Clone, Copy, Debug)]
struct Timing {
    pts: i64,
    color: Color,
}
enum Picture {
    H264(Frame),
    Extra(codecs::Picture),
}
struct TimedFrame {
    frame: Picture,
    timing: Timing,
}
impl TimedFrame {
    fn into_image(self) -> Option<DecodedImage> {
        match &self.frame {
            Picture::H264(frame) => h264_to_image(frame, self.timing.color),
            Picture::Extra(frame) => frame.to_image(),
        }
    }
}

struct DecoderCore {
    source: MovieBytes,
    demuxer: Box<dyn Demuxer>,
    video_index: usize,
    decoder: H264Decoder,
    extra: Option<codecs::Decoder>,
    reorder_depth: usize,
    pending: VecDeque<TimedFrame>,
    pending_timings: Vec<Timing>,
    color: Color,
    eof: bool,
    failed: bool,
}

impl DecoderCore {
    fn reset(&mut self) {
        self.pending.clear();
        self.pending_timings.clear();
        self.color = Color::default();
        self.decoder = h264_decoder(self.reorder_depth);
        self.eof = false;
        self.failed = false;
        if let Some(extra) = self.extra.as_mut() {
            if extra.reset().is_none() {
                self.failed = true;
            }
        }
        match open(&self.source) {
            Some((demuxer, _)) => self.demuxer = demuxer,
            None => self.failed = true,
        }
    }

    fn next_frame(&mut self) -> Option<TimedFrame> {
        loop {
            if let Some(frame) = self.pending.pop_front() {
                return Some(frame);
            }
            if self.failed || self.eof {
                return None;
            }
            let packet = match self.demuxer.read_packet() {
                Ok(packet) => packet,
                Err(DemuxError::Eof) => {
                    self.eof = true;
                    if let Some(extra) = self.extra.as_mut() {
                        let frames = extra.drain();
                        if !self.queue_extra(frames) {
                            return None;
                        }
                        continue;
                    }
                    let frames = self.decoder.flush();
                    if !self.queue(frames) {
                        return None;
                    }
                    continue;
                }
                Err(error) => {
                    self.failed = true;
                    tracing::warn!(?error, "MP4 read failed");
                    return None;
                }
            };
            if packet.stream_index != self.video_index {
                continue;
            }
            let Some(pts) = packet.pts.or(packet.dts) else {
                self.failed = true;
                return None;
            };
            if let Some(extra) = self.extra.as_mut() {
                let frames = extra.decode(&packet.data, pts, packet.duration);
                if !self.queue_extra(frames) {
                    return None;
                }
                continue;
            }
            let nals = parse_annex_b(&packet.data);
            if nals.is_empty() {
                self.failed = true;
                return None;
            }
            let mut timing_added = false;
            for nal in nals {
                if nal.nal_unit_type == NalUnitType::Sps {
                    if let Some(color) = sps_color(&nal.rbsp) {
                        self.color = color;
                    } else {
                        self.failed = true;
                        return None;
                    }
                }
                if !timing_added
                    && matches!(
                        nal.nal_unit_type,
                        NalUnitType::Slice | NalUnitType::SliceIdr
                    )
                {
                    if self.pending_timings.len() >= MAX_PENDING_FRAMES {
                        self.failed = true;
                        tracing::warn!("MP4 picture timing queue exceeded its bound");
                        return None;
                    }
                    self.pending_timings.push(Timing {
                        pts,
                        color: self.color,
                    });
                    timing_added = true;
                }
                match self.decoder.decode_nal(&nal) {
                    Ok(frames) => {
                        if !self.queue(frames) {
                            return None;
                        }
                    }
                    Err(error) => {
                        self.failed = true;
                        tracing::warn!(?error, "MP4 H.264 decode failed");
                        return None;
                    }
                }
            }
        }
    }

    fn queue(&mut self, frames: Vec<Frame>) -> bool {
        if self.pending.len().saturating_add(frames.len()) > MAX_PENDING_FRAMES
            || frames.len() > self.pending_timings.len()
        {
            self.failed = true;
            tracing::warn!("MP4 picture queue exceeded its bound");
            return false;
        }
        for frame in frames {
            let Some(timing) = self.take_earliest_timing() else {
                self.failed = true;
                tracing::warn!("MP4 decoder produced a frame without timing");
                return false;
            };
            self.pending.push_back(TimedFrame {
                frame: Picture::H264(frame),
                timing,
            });
        }
        true
    }

    fn take_earliest_timing(&mut self) -> Option<Timing> {
        let index = self
            .pending_timings
            .iter()
            .enumerate()
            .min_by_key(|(_, timing)| timing.pts)
            .map(|(index, _)| index)?;
        Some(self.pending_timings.swap_remove(index))
    }

    fn queue_extra(&mut self, frames: Option<Vec<(codecs::Picture, i64)>>) -> bool {
        let Some(frames) = frames else {
            self.failed = true;
            tracing::warn!("MP4 VP9/AV1 decode failed");
            return false;
        };
        if self.pending.len().saturating_add(frames.len()) > MAX_PENDING_FRAMES {
            self.failed = true;
            return false;
        }
        self.pending
            .extend(frames.into_iter().map(|(frame, pts)| TimedFrame {
                frame: Picture::Extra(frame),
                timing: Timing {
                    pts,
                    color: Color::default(),
                },
            }));
        true
    }
}

#[cfg(not(target_arch = "wasm32"))]
struct ReadyFrame {
    pts: i64,
    color: Color,
    image: DecodedImage,
}

#[cfg(not(target_arch = "wasm32"))]
struct PrefetchState {
    queue: VecDeque<ReadyFrame>,
    eof: bool,
    failed: bool,
    shutdown: bool,
    reset_gen: u64,
    ack_gen: u64,
}

#[cfg(not(target_arch = "wasm32"))]
struct PrefetchWorker {
    shared: Arc<(Mutex<PrefetchState>, Condvar)>,
    handle: Option<JoinHandle<()>>,
}

#[cfg(not(target_arch = "wasm32"))]
impl PrefetchWorker {
    fn spawn(
        source: MovieBytes,
        video_index: usize,
        codec_id: CodecId,
        size: (u32, u32),
        reorder_depth: usize,
    ) -> Option<Self> {
        let shared = Arc::new((
            Mutex::new(PrefetchState {
                queue: VecDeque::with_capacity(PREFETCH_QUEUE_CAPACITY),
                eof: false,
                failed: false,
                shutdown: false,
                reset_gen: 0,
                ack_gen: 0,
            }),
            Condvar::new(),
        ));
        let worker_shared = Arc::clone(&shared);
        let handle = std::thread::Builder::new()
            .name("ethornell-mp4".into())
            .spawn(move || {
                let (lock, cvar) = &*worker_shared;
                let extra = if codec_id != CodecId::H264 {
                    let Some(owned) = source.read_all_bytes() else {
                        let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                        state.failed = true;
                        cvar.notify_all();
                        return;
                    };
                    match codecs::Decoder::new(&owned, codec_id, size) {
                        Some(dec) => Some(dec),
                        None => {
                            let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                            state.failed = true;
                            cvar.notify_all();
                            return;
                        }
                    }
                } else {
                    None
                };
                let Some((demuxer, _)) = open(&source) else {
                    let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                    state.failed = true;
                    cvar.notify_all();
                    return;
                };
                let mut core = DecoderCore {
                    source,
                    demuxer,
                    video_index,
                    decoder: h264_decoder(reorder_depth),
                    extra,
                    reorder_depth,
                    pending: VecDeque::new(),
                    pending_timings: Vec::new(),
                    color: Color::default(),
                    eof: false,
                    failed: false,
                };
                let mut my_gen = 0u64;
                loop {
                    let action_gen = {
                        let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                        loop {
                            if state.shutdown {
                                return;
                            }
                            if state.reset_gen != my_gen {
                                break Some(state.reset_gen);
                            }
                            if !state.eof
                                && !state.failed
                                && state.queue.len() < PREFETCH_QUEUE_CAPACITY
                            {
                                break None;
                            }
                            state = cvar.wait(state).unwrap_or_else(|e| e.into_inner());
                        }
                    };
                    if let Some(target_gen) = action_gen {
                        core.reset();
                        my_gen = target_gen;
                        let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                        if state.reset_gen == my_gen {
                            state.queue.clear();
                            state.eof = core.eof;
                            state.failed = core.failed;
                            state.ack_gen = my_gen;
                            cvar.notify_all();
                        }
                        continue;
                    }
                    let frame = core.next_frame();
                    let Some(timed) = frame else {
                        let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                        if state.reset_gen == my_gen {
                            state.eof = core.eof;
                            state.failed = core.failed;
                            cvar.notify_all();
                        }
                        continue;
                    };
                    let pts = timed.timing.pts;
                    let color = timed.timing.color;
                    let image = timed.into_image();
                    let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                    if state.reset_gen != my_gen {
                        continue;
                    }
                    match image {
                        Some(image) => {
                            state.queue.push_back(ReadyFrame { pts, color, image });
                            cvar.notify_all();
                        }
                        None => {
                            core.failed = true;
                            state.failed = true;
                            tracing::warn!("unsupported MP4 picture layout/color metadata");
                            cvar.notify_all();
                        }
                    }
                }
            })
            .ok()?;
        Some(Self {
            shared,
            handle: Some(handle),
        })
    }

    fn request_reset(&self) {
        let (lock, cvar) = &*self.shared;
        let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
        state.reset_gen = state.reset_gen.wrapping_add(1);
        state.queue.clear();
        state.eof = false;
        state.failed = false;
        cvar.notify_all();
    }
}

#[cfg(not(target_arch = "wasm32"))]
impl Drop for PrefetchWorker {
    fn drop(&mut self) {
        {
            let (lock, cvar) = &*self.shared;
            let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
            state.shutdown = true;
            state.queue.clear();
            cvar.notify_all();
        }
        if let Some(handle) = self.handle.take() {
            let _ = handle.join();
        }
    }
}

pub(crate) struct Mp4MovieDecoder {
    #[cfg(not(target_arch = "wasm32"))]
    worker: PrefetchWorker,
    #[cfg(target_arch = "wasm32")]
    core: DecoderCore,
    time_base: Rational,
    #[cfg(test)]
    uses_extra_codec: bool,
    redeliver: Option<DecodedImage>,
    delivered: bool,
    color: Color,
    size: (u32, u32),
    duration_ms: i32,
    audio_start_ms: i32,
    audio: Option<StreamBytes>,
    requested_ms: i32,
    frame_index: Option<usize>,
    eof: bool,
    failed: bool,
}
impl fmt::Debug for Mp4MovieDecoder {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        #[cfg(not(target_arch = "wasm32"))]
        let pending_frames = self
            .worker
            .shared
            .0
            .lock()
            .map(|s| s.queue.len())
            .unwrap_or_default();
        #[cfg(target_arch = "wasm32")]
        let pending_frames = self.core.pending.len();
        f.debug_struct("Mp4MovieDecoder")
            .field("size", &self.size)
            .field("duration_ms", &self.duration_ms)
            .field("pending_frames", &pending_frames)
            .field("failed", &self.failed)
            .finish()
    }
}
fn open(source: &MovieBytes) -> Option<(Box<dyn Demuxer>, Vec<Stream>)> {
    let mut registry = FormatRegistry::new();
    rff_format_mp4::register(&mut registry);
    let input = source.open_seekable()?;
    let mut demuxer = registry.open_demuxer_seekable("mp4", input).ok()?;
    let streams = demuxer.read_header().ok()?;
    Some((demuxer, streams))
}

/// Exact minimum reorder-buffer depth (`1..=16`) needed for one GOP's decoded
/// PTS sequence to emerge in strictly non-decreasing display order under
/// `rust_h264`'s min-POC eviction rule.
fn required_gop_reorder_depth(pts_list: &[i64]) -> usize {
    if pts_list.len() <= 1 {
        return 1;
    }
    let mut expected = pts_list.to_vec();
    expected.sort_unstable();
    for depth in 1..=16 {
        let mut buf = Vec::with_capacity(depth + 1);
        let mut out_idx = 0usize;
        let mut ok = true;
        for &p in pts_list {
            buf.push(p);
            while buf.len() > depth {
                let (min_i, &min_p) = buf
                    .iter()
                    .enumerate()
                    .min_by_key(|&(_, &v)| v)
                    .expect("non-empty reorder buffer");
                buf.swap_remove(min_i);
                if min_p != expected[out_idx] {
                    ok = false;
                    break;
                }
                out_idx += 1;
            }
            if !ok {
                break;
            }
        }
        if ok {
            return depth;
        }
    }
    16
}

impl Mp4MovieDecoder {
    pub(crate) fn supports_resource(resource: &str) -> bool {
        resource
            .rsplit_once('.')
            .is_some_and(|(_, ext)| ext.eq_ignore_ascii_case("mp4"))
    }
    pub(crate) fn probe(mut input: impl Read + Seek) -> io::Result<bool> {
        // Walk top-level boxes by size so leading padding is allowed and large
        // media boxes can be skipped without reading their contents.
        let len = input.seek(SeekFrom::End(0))?;
        let mut offset = 0;
        let mut header = [0; 8];
        while len - offset >= 8 {
            input.seek(SeekFrom::Start(offset))?;
            input.read_exact(&mut header)?;
            let size = u32::from_be_bytes(header[..4].try_into().unwrap());
            let (size, header_size) = match size {
                0 => (len - offset, 8),
                1 => {
                    let mut extended = [0; 8];
                    input.read_exact(&mut extended)?;
                    (u64::from_be_bytes(extended), 16)
                }
                size => (u64::from(size), 8),
            };
            if size < header_size || size > len - offset {
                return Ok(false);
            }
            if matches!(&header[4..], b"ftyp" | b"moov") && size >= header_size + 8 {
                return Ok(true);
            }
            offset += size;
        }
        Ok(false)
    }
    pub(crate) fn new(source: MovieBytes, _resource: &str) -> Option<Self> {
        let (mut scan, streams) = open(&source)?;
        let video = streams.iter().find(|s| s.media_type == MediaType::Video)?;
        if video.time_base.num <= 0 || video.time_base.den <= 0 {
            tracing::warn!(codec=?video.codec_id, "MP4 requires valid timestamps");
            return None;
        }
        let size = (video.width, video.height);
        let pixels = (size.0 as usize).checked_mul(size.1 as usize)?;
        if pixels == 0 || pixels > MAX_FRAME_PIXELS {
            return None;
        }
        let video_codec_id = video.codec_id;
        #[cfg(test)]
        let uses_extra_codec = video_codec_id != CodecId::H264;
        let video_index = video.index;
        let time_base = video.time_base;
        let audio_track = streams.iter().find(|s| s.media_type == MediaType::Audio);
        let audio_config = match audio_track {
            Some(track) if track.codec_id == CodecId::Aac => {
                Some(AacConfig::from_stream(&track.extradata)?)
            }
            Some(track) => {
                tracing::warn!(codec=?track.codec_id, "MP4 supports AAC-LC movie audio");
                return None;
            }
            None => None,
        };
        // The existing audio host consumes compressed elementary streams.
        // Scan once to extract AAC, duration, and the exact B-frame reorder
        // depth without retaining compressed video or an RGBA timeline.
        let mut audio = Vec::new();
        let mut audio_start_ms = None;
        let mut duration_ms = video.duration.map(|n| end_ms(n, time_base)).unwrap_or(0);
        let mut gop_pts = Vec::new();
        let mut reorder_depth = 1usize;
        let mut has_reordered_pts = false;
        let mut prev_video_pts: Option<i64> = None;
        loop {
            let packet = match scan.read_packet() {
                Ok(packet) => packet,
                Err(DemuxError::Eof) => break,
                Err(error) => {
                    tracing::warn!(?error, "MP4 packet scan failed");
                    return None;
                }
            };
            if packet.stream_index == video_index {
                let pts = packet.pts.or(packet.dts)?;
                duration_ms = duration_ms.max(end_ms(
                    pts.saturating_add(packet.duration.max(0)),
                    time_base,
                ));
                if packet.flags.keyframe && !gop_pts.is_empty() {
                    reorder_depth = reorder_depth.max(required_gop_reorder_depth(&gop_pts));
                    gop_pts.clear();
                    prev_video_pts = None;
                }
                if prev_video_pts.is_some_and(|prev| pts < prev) {
                    has_reordered_pts = true;
                }
                prev_video_pts = Some(pts);
                gop_pts.push(pts);
            } else if let Some(track) = audio_track.filter(|s| s.index == packet.stream_index) {
                if track.time_base.num <= 0 || track.time_base.den <= 0 {
                    return None;
                }
                let pts = packet.pts.or(packet.dts)?;
                audio_start_ms.get_or_insert(signed_ms(pts, track.time_base));
                duration_ms = duration_ms.max(end_ms(
                    pts.saturating_add(packet.duration.max(0)),
                    track.time_base,
                ));
                let header = audio_config.as_ref()?.adts_header(packet.data.len())?;
                if audio.len().checked_add(header.len() + packet.data.len())? > MAX_AUDIO_BYTES {
                    return None;
                }
                audio.extend_from_slice(&header);
                audio.extend_from_slice(&packet.data);
            }
        }
        drop(scan);
        reorder_depth = reorder_depth.max(required_gop_reorder_depth(&gop_pts));
        if has_reordered_pts {
            reorder_depth = reorder_depth.max(2).min(16);
        }
        if duration_ms <= 0 {
            return None;
        }
        #[cfg(not(target_arch = "wasm32"))]
        {
            let worker = PrefetchWorker::spawn(
                source,
                video_index,
                video_codec_id,
                size,
                reorder_depth,
            )?;
            // Fail configuration if no picture can be decoded, rather than
            // acknowledging a movie that will only play its soundtrack.
            let initial_color = {
                let (lock, cvar) = &*worker.shared;
                let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
                while state.queue.is_empty() && !state.eof && !state.failed {
                    state = cvar.wait(state).unwrap_or_else(|e| e.into_inner());
                }
                let first = state.queue.front()?;
                first.color
            };
            Some(Self {
                worker,
                time_base,
                #[cfg(test)]
                uses_extra_codec,
                redeliver: None,
                delivered: false,
                color: initial_color,
                size,
                duration_ms,
                audio_start_ms: audio_start_ms.unwrap_or(0),
                audio: (!audio.is_empty()).then(|| StreamBytes(audio.into())),
                requested_ms: 0,
                frame_index: None,
                eof: false,
                failed: false,
            })
        }
        #[cfg(target_arch = "wasm32")]
        {
            let extra = if video_codec_id != CodecId::H264 {
                let owned = source.read_all_bytes()?;
                Some(codecs::Decoder::new(&owned, video_codec_id, size)?)
            } else {
                None
            };
            let (demuxer, _) = open(&source)?;
            let mut core = DecoderCore {
                source,
                demuxer,
                video_index,
                decoder: h264_decoder(reorder_depth),
                extra,
                reorder_depth,
                pending: VecDeque::new(),
                pending_timings: Vec::new(),
                color: Color::default(),
                eof: false,
                failed: false,
            };
            let first = core.next_frame()?;
            let initial_color = first.timing.color;
            core.pending.push_front(first);
            Some(Self {
                core,
                time_base,
                #[cfg(test)]
                uses_extra_codec,
                redeliver: None,
                delivered: false,
                color: initial_color,
                size,
                duration_ms,
                audio_start_ms: audio_start_ms.unwrap_or(0),
                audio: (!audio.is_empty()).then(|| StreamBytes(audio.into())),
                requested_ms: 0,
                frame_index: None,
                eof: false,
                failed: false,
            })
        }
    }
    pub(crate) fn duration_ms(&self) -> i32 {
        self.duration_ms
    }
    pub(crate) fn audio_start_ms(&self) -> i32 {
        self.audio_start_ms
    }
    pub(crate) fn size(&self) -> Option<(u32, u32)> {
        Some(self.size)
    }
    pub(crate) fn take_audio(&mut self) -> Option<StreamBytes> {
        self.audio.take()
    }
    pub(crate) fn frame_index(&self) -> Option<usize> {
        self.frame_index
    }
    pub(crate) fn is_pristine_at_start(&self) -> bool {
        self.requested_ms == 0
            && !self.failed
            && !self.eof
            && matches!(self.frame_index, None | Some(0))
    }
    pub(crate) fn invalidate_delivery(&mut self) {
        self.delivered = false;
    }
    pub(crate) fn stash_redeliver(&mut self, image: DecodedImage) {
        self.redeliver = Some(image);
        self.delivered = false;
    }
    pub(crate) fn seek_to(&mut self, position_ms: i32) {
        let position_ms = position_ms.max(0);
        if position_ms <= self.requested_ms && self.frame_index.is_some() {
            self.reset();
        }
        if let Some(image) = self.frame_at(position_ms) {
            self.stash_redeliver(image);
        } else {
            self.delivered = false;
        }
    }
    pub(crate) fn reset(&mut self) {
        self.redeliver = None;
        self.delivered = false;
        self.frame_index = None;
        self.requested_ms = 0;
        self.color = Color::default();
        self.eof = false;
        self.failed = false;
        #[cfg(not(target_arch = "wasm32"))]
        {
            self.worker.request_reset();
        }
        #[cfg(target_arch = "wasm32")]
        {
            self.core.reset();
            self.failed = self.core.failed;
        }
    }
    pub(crate) fn frame_at(&mut self, position_ms: i32) -> Option<DecodedImage> {
        let position_ms = position_ms.max(0);
        if position_ms < self.requested_ms {
            self.reset();
        }
        self.requested_ms = position_ms;
        #[cfg(not(target_arch = "wasm32"))]
        {
            let mut selected: Option<ReadyFrame> = None;
            let (lock, cvar) = &*self.worker.shared;
            let mut state = lock.lock().unwrap_or_else(|e| e.into_inner());
            loop {
                if state.ack_gen != state.reset_gen {
                    state = cvar.wait(state).unwrap_or_else(|e| e.into_inner());
                    continue;
                }
                if let Some(front) = state.queue.front() {
                    if !due(front.pts, self.time_base, position_ms) {
                        break;
                    }
                    let frame = state.queue.pop_front().expect("checked front");
                    cvar.notify_one();
                    self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
                    selected = Some(frame);
                    continue;
                }
                if state.eof || state.failed {
                    self.eof = state.eof;
                    self.failed = state.failed;
                    break;
                }
                state = cvar.wait(state).unwrap_or_else(|e| e.into_inner());
            }
            drop(state);
            if let Some(selected) = selected {
                self.color = selected.color;
                self.redeliver = None;
                self.delivered = true;
                return Some(selected.image);
            }
            if !self.delivered
                && let Some(image) = self.redeliver.take()
            {
                self.delivered = true;
                return Some(image);
            }
            None
        }
        #[cfg(target_arch = "wasm32")]
        {
            let mut selected = None;
            loop {
                let frame = self.core.next_frame();
                let Some(frame) = frame else {
                    self.eof = self.core.eof;
                    self.failed = self.core.failed;
                    break;
                };
                if !due(frame.timing.pts, self.time_base, position_ms) {
                    self.core.pending.push_front(frame);
                    break;
                }
                self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
                selected = Some(frame);
            }
            if let Some(selected) = selected {
                self.color = selected.timing.color;
                self.redeliver = None;
                let image = selected.into_image();
                if image.is_none() {
                    self.failed = true;
                    tracing::warn!("unsupported MP4 picture layout/color metadata");
                }
                self.delivered = image.is_some();
                return image;
            }
            if !self.delivered
                && let Some(image) = self.redeliver.take()
            {
                self.delivered = true;
                return Some(image);
            }
            None
        }
    }
}
fn h264_decoder(reorder_depth: usize) -> H264Decoder {
    // Preserve native picture-worker decoding with the exact stream reorder
    // depth (instead of a hardcoded 16-frame buffer); browsers without Rust
    // threads keep the serial implementation and the same display-order contract.
    #[cfg(not(target_arch = "wasm32"))]
    {
        let threads = std::thread::available_parallelism()
            .map_or(2, |n| n.get().clamp(2, 4));
        H264Decoder::with_max_depth(threads, reorder_depth)
    }
    #[cfg(target_arch = "wasm32")]
    {
        H264Decoder::with_max_depth(reorder_depth)
    }
}

fn due(pts: i64, base: Rational, position_ms: i32) -> bool {
    pts as i128 * base.num as i128 * 1000 <= position_ms as i128 * base.den as i128
}
fn signed_ms(ticks: i64, base: Rational) -> i32 {
    (ticks as i128 * base.num as i128 * 1000 / base.den as i128)
        .clamp(i32::MIN as i128, i32::MAX as i128) as i32
}
fn end_ms(ticks: i64, base: Rational) -> i32 {
    let n = ticks as i128 * base.num as i128 * 1000;
    ((n + base.den as i128 - 1) / base.den as i128).clamp(0, i32::MAX as i128) as i32
}
fn h264_to_image(frame: &Frame, color: Color) -> Option<DecodedImage> {
    let sub_y = match frame.chroma_format {
        rust_h264::decoder::ChromaFormat::Yuv420 => 1,
        rust_h264::decoder::ChromaFormat::Yuv422 => 0,
    };
    let b = frame.bytes_per_sample();
    planar_to_image(
        Planar {
            size: (frame.width, frame.height),
            depth: frame.bit_depth as usize,
            sub: (1, sub_y),
            strides: [
                frame.width as usize * b,
                frame.chroma_width() as usize * b,
                frame.chroma_width() as usize * b,
            ],
            planes: [&frame.y, &frame.u, &frame.v],
            monochrome: false,
        },
        color,
    )
}
struct Planar<'a> {
    size: (u32, u32),
    depth: usize,
    sub: (usize, usize),
    strides: [usize; 3], // Bytes, including high-bit-depth decoder padding.
    planes: [&'a [u8]; 3],
    monochrome: bool,
}
fn planar_to_image(frame: Planar<'_>, color: Color) -> Option<DecodedImage> {
    let (width, height) = (frame.size.0 as usize, frame.size.1 as usize);
    let pixels = width.checked_mul(height)?;
    if pixels == 0
        || pixels > MAX_FRAME_PIXELS
        || !matches!(frame.depth, 8 | 10 | 12)
        || frame.sub.0 > 1
        || frame.sub.1 > 1
    {
        return None;
    }
    let identity =
        color.matrix == 0 && color.full_range && frame.sub == (0, 0) && !frame.monochrome;
    let (kr, kb) = match color.matrix {
        0 if identity => (0.0, 0.0),
        1 => (0.2126, 0.0722),
        2 if width >= 1280 || height > 576 => (0.2126, 0.0722),
        2 | 5 | 6 => (0.299, 0.114),
        4 => (0.30, 0.11),
        7 => (0.212, 0.087),
        9 => (0.2627, 0.0593),
        _ => return None,
    };
    let scale = 16384.0_f64;
    let yscale = if color.full_range { 1.0 } else { 255.0 / 219.0 };
    let cscale = if color.full_range { 1.0 } else { 255.0 / 224.0 };
    let kg = 1.0 - kr - kb;
    let coeff = [
        yscale,
        2.0 * (1.0 - kr) * cscale,
        -2.0 * kb * (1.0 - kb) / kg * cscale,
        -2.0 * kr * (1.0 - kr) / kg * cscale,
        2.0 * (1.0 - kb) * cscale,
    ]
    .map(|v| (v * scale).round() as i32);
    let shift = frame.depth - 8;
    let bytes = if frame.depth == 8 { 1 } else { 2 };
    // Check the last visible sample in each plane before allocating output.
    for p in 0..if frame.monochrome { 1 } else { 3 } {
        let sx = if p == 0 { 0 } else { frame.sub.0 };
        let sy = if p == 0 { 0 } else { frame.sub.1 };
        let w = (width + (1 << sx) - 1) >> sx;
        let h = (height + (1 << sy) - 1) >> sy;
        let row_bytes = w.checked_mul(bytes)?;
        if frame.strides[p] < row_bytes
            || (h - 1)
                .checked_mul(frame.strides[p])?
                .checked_add(row_bytes)?
                > frame.planes[p].len()
        {
            return None;
        }
    }
    let sample = |p: usize, y: usize, x: usize| -> i32 {
        let offset = y * frame.strides[p] + x * bytes;
        let plane = frame.planes[p];
        let v = if bytes == 1 {
            u16::from(plane[offset])
        } else {
            u16::from_le_bytes([plane[offset], plane[offset + 1]])
        };
        i32::from(v >> shift)
    };
    let mut rgba = vec![0; pixels.checked_mul(4)?];
    if !identity && !frame.monochrome && frame.sub == (1, 1) && (frame.depth == 8 || frame.depth == 10)
    {
        let y_off = if color.full_range { 0 } else { 16 };
        let c0 = coeff[0];
        let c1 = coeff[1];
        let c2 = coeff[2];
        let c3 = coeff[3];
        let c4 = coeff[4];
        let y_plane = frame.planes[0];
        let u_plane = frame.planes[1];
        let v_plane = frame.planes[2];
        let y_stride = frame.strides[0];
        let u_stride = frame.strides[1];
        let v_stride = frame.strides[2];
        let row_rgba = width * 4;
        let pair_w = width / 2;
        let pair_h = height / 2;

        #[inline(always)]
        fn write_pixel(dst: &mut [u8], luma: i32, cr_r: i32, cbcr_g: i32, cb_b: i32) {
            dst[0] = ((luma + cr_r) >> 14).clamp(0, 255) as u8;
            dst[1] = ((luma + cbcr_g) >> 14).clamp(0, 255) as u8;
            dst[2] = ((luma + cb_b) >> 14).clamp(0, 255) as u8;
            dst[3] = 255;
        }

        if frame.depth == 8 {
            for cy in 0..pair_h {
                let y0_row = &y_plane[cy * 2 * y_stride..cy * 2 * y_stride + width];
                let y1_row = &y_plane[(cy * 2 + 1) * y_stride..(cy * 2 + 1) * y_stride + width];
                let u_row = &u_plane[cy * u_stride..cy * u_stride + (width + 1) / 2];
                let v_row = &v_plane[cy * v_stride..cy * v_stride + (width + 1) / 2];
                let (dst0, dst1) = rgba[cy * 2 * row_rgba..(cy * 2 + 2) * row_rgba]
                    .split_at_mut(row_rgba);
                for cx in 0..pair_w {
                    let du = i32::from(u_row[cx]) - 128;
                    let dv = i32::from(v_row[cx]) - 128;
                    let cr_r = c1 * dv + 8192;
                    let cbcr_g = c2 * du + c3 * dv + 8192;
                    let cb_b = c4 * du + 8192;
                    let x = cx * 2;
                    let ox = cx * 8;
                    write_pixel(&mut dst0[ox..ox + 4], c0 * (i32::from(y0_row[x]) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst0[ox + 4..ox + 8], c0 * (i32::from(y0_row[x + 1]) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox..ox + 4], c0 * (i32::from(y1_row[x]) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox + 4..ox + 8], c0 * (i32::from(y1_row[x + 1]) - y_off), cr_r, cbcr_g, cb_b);
                }
                if width & 1 != 0 {
                    let du = i32::from(u_row[pair_w]) - 128;
                    let dv = i32::from(v_row[pair_w]) - 128;
                    let cr_r = c1 * dv + 8192;
                    let cbcr_g = c2 * du + c3 * dv + 8192;
                    let cb_b = c4 * du + 8192;
                    let x = pair_w * 2;
                    let ox = pair_w * 8;
                    write_pixel(&mut dst0[ox..ox + 4], c0 * (i32::from(y0_row[x]) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox..ox + 4], c0 * (i32::from(y1_row[x]) - y_off), cr_r, cbcr_g, cb_b);
                }
            }
            if height & 1 != 0 {
                let cy = pair_h;
                let y0_row = &y_plane[cy * 2 * y_stride..cy * 2 * y_stride + width];
                let u_row = &u_plane[cy * u_stride..cy * u_stride + (width + 1) / 2];
                let v_row = &v_plane[cy * v_stride..cy * v_stride + (width + 1) / 2];
                let dst0 = &mut rgba[cy * 2 * row_rgba..cy * 2 * row_rgba + row_rgba];
                for x in 0..width {
                    let du = i32::from(u_row[x >> 1]) - 128;
                    let dv = i32::from(v_row[x >> 1]) - 128;
                    let luma = c0 * (i32::from(y0_row[x]) - y_off);
                    write_pixel(
                        &mut dst0[x * 4..x * 4 + 4],
                        luma,
                        c1 * dv + 8192,
                        c2 * du + c3 * dv + 8192,
                        c4 * du + 8192,
                    );
                }
            }
        } else {
            // 10-bit little-endian (yuv420p10le): shift right by 2 to 8-bit luma/chroma.
            #[inline(always)]
            fn load10(row: &[u8], i: usize) -> i32 {
                i32::from(u16::from_le_bytes([row[i * 2], row[i * 2 + 1]]) >> 2)
            }
            for cy in 0..pair_h {
                let y0_row = &y_plane[cy * 2 * y_stride..cy * 2 * y_stride + width * 2];
                let y1_row = &y_plane[(cy * 2 + 1) * y_stride..(cy * 2 + 1) * y_stride + width * 2];
                let cw = (width + 1) / 2;
                let u_row = &u_plane[cy * u_stride..cy * u_stride + cw * 2];
                let v_row = &v_plane[cy * v_stride..cy * v_stride + cw * 2];
                let (dst0, dst1) = rgba[cy * 2 * row_rgba..(cy * 2 + 2) * row_rgba]
                    .split_at_mut(row_rgba);
                for cx in 0..pair_w {
                    let du = load10(u_row, cx) - 128;
                    let dv = load10(v_row, cx) - 128;
                    let cr_r = c1 * dv + 8192;
                    let cbcr_g = c2 * du + c3 * dv + 8192;
                    let cb_b = c4 * du + 8192;
                    let x = cx * 2;
                    let ox = cx * 8;
                    write_pixel(&mut dst0[ox..ox + 4], c0 * (load10(y0_row, x) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst0[ox + 4..ox + 8], c0 * (load10(y0_row, x + 1) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox..ox + 4], c0 * (load10(y1_row, x) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox + 4..ox + 8], c0 * (load10(y1_row, x + 1) - y_off), cr_r, cbcr_g, cb_b);
                }
                if width & 1 != 0 {
                    let du = load10(u_row, pair_w) - 128;
                    let dv = load10(v_row, pair_w) - 128;
                    let cr_r = c1 * dv + 8192;
                    let cbcr_g = c2 * du + c3 * dv + 8192;
                    let cb_b = c4 * du + 8192;
                    let x = pair_w * 2;
                    let ox = pair_w * 8;
                    write_pixel(&mut dst0[ox..ox + 4], c0 * (load10(y0_row, x) - y_off), cr_r, cbcr_g, cb_b);
                    write_pixel(&mut dst1[ox..ox + 4], c0 * (load10(y1_row, x) - y_off), cr_r, cbcr_g, cb_b);
                }
            }
            if height & 1 != 0 {
                let cy = pair_h;
                let y0_row = &y_plane[cy * 2 * y_stride..cy * 2 * y_stride + width * 2];
                let cw = (width + 1) / 2;
                let u_row = &u_plane[cy * u_stride..cy * u_stride + cw * 2];
                let v_row = &v_plane[cy * v_stride..cy * v_stride + cw * 2];
                let dst0 = &mut rgba[cy * 2 * row_rgba..cy * 2 * row_rgba + row_rgba];
                for x in 0..width {
                    let du = load10(u_row, x >> 1) - 128;
                    let dv = load10(v_row, x >> 1) - 128;
                    let luma = c0 * (load10(y0_row, x) - y_off);
                    write_pixel(
                        &mut dst0[x * 4..x * 4 + 4],
                        luma,
                        c1 * dv + 8192,
                        c2 * du + c3 * dv + 8192,
                        c4 * du + 8192,
                    );
                }
            }
        }
        return Some(DecodedImage {
            width: frame.size.0,
            height: frame.size.1,
            rgba,
        });
    }
    for y in 0..height {
        for x in 0..width {
            let yy = sample(0, y, x);
            let (u, v) = if frame.monochrome {
                (128, 128)
            } else {
                (
                    sample(1, y >> frame.sub.1, x >> frame.sub.0),
                    sample(2, y >> frame.sub.1, x >> frame.sub.0),
                )
            };
            let rgb = if identity {
                [v, yy, u]
            } else {
                let luma = coeff[0] * (yy - if color.full_range { 0 } else { 16 });
                [
                    luma + coeff[1] * (v - 128),
                    luma + coeff[2] * (u - 128) + coeff[3] * (v - 128),
                    luma + coeff[4] * (u - 128),
                ]
                .map(|v| (v + 8192) >> 14)
            };
            let offset = (y * width + x) * 4;
            for i in 0..3 {
                rgba[offset + i] = rgb[i].clamp(0, 255) as u8;
            }
            rgba[offset + 3] = 255;
        }
    }
    Some(DecodedImage {
        width: frame.size.0,
        height: frame.size.1,
        rgba,
    })
}

struct AacConfig {
    frequency_index: u8,
    channels: u8,
}
impl AacConfig {
    fn from_stream(extra: &[u8]) -> Option<Self> {
        if extra.len() < 2 || extra[0] >> 3 != 2 {
            return None;
        }
        let frequency_index = ((extra[0] & 7) << 1) | (extra[1] >> 7);
        let channels = (extra[1] >> 3) & 15;
        (frequency_index < 13 && (1..=2).contains(&channels)).then_some(Self {
            frequency_index,
            channels,
        })
    }
    fn adts_header(&self, payload_len: usize) -> Option<[u8; 7]> {
        let n = payload_len.checked_add(7)?;
        if n > 0x1fff {
            return None;
        }
        Some([
            0xff,
            0xf1,
            0x40 | (self.frequency_index << 2) | (self.channels >> 2),
            (self.channels << 6) | ((n >> 11) as u8 & 3),
            (n >> 3) as u8,
            ((n as u8 & 7) << 5) | 0x1f,
            0xfc,
        ])
    }
}

// Read only the SPS prefix through VUI color metadata. Keep codec internals
// private: rust_h264's `dev-internals` parser is not a stable runtime API.
struct Bits<'a> {
    bytes: &'a [u8],
    position: usize,
}
impl Bits<'_> {
    fn take(&mut self, count: usize) -> Option<u32> {
        if count > 32 || self.position.checked_add(count)? > self.bytes.len().checked_mul(8)? {
            return None;
        }
        let mut value = 0;
        for _ in 0..count {
            value = (value << 1)
                | u32::from((self.bytes[self.position / 8] >> (7 - self.position % 8)) & 1);
            self.position += 1;
        }
        Some(value)
    }
    fn ue(&mut self) -> Option<u32> {
        let mut zeros = 0;
        while self.take(1)? == 0 {
            zeros += 1;
            if zeros > 31 {
                return None;
            }
        }
        (1u32.checked_shl(zeros as u32)? - 1).checked_add(self.take(zeros)?)
    }
    fn se(&mut self) -> Option<i32> {
        let n = self.ue()?;
        let magnitude = i32::try_from((u64::from(n) + 1) / 2).ok()?;
        Some(if n & 1 == 0 { -magnitude } else { magnitude })
    }
}
fn sps_color(rbsp: &[u8]) -> Option<Color> {
    let mut r = Bits {
        bytes: rbsp,
        position: 0,
    };
    let profile = r.take(8)?;
    r.take(16)?;
    r.ue()?;
    if matches!(
        profile,
        100 | 110 | 122 | 244 | 44 | 83 | 86 | 118 | 128 | 138 | 139 | 134 | 135
    ) {
        let chroma = r.ue()?;
        if chroma > 3 {
            return None;
        }
        if chroma == 3 {
            r.take(1)?;
        }
        r.ue()?;
        r.ue()?;
        r.take(1)?;
        if r.take(1)? != 0 {
            for i in 0..if chroma == 3 { 12 } else { 8 } {
                if r.take(1)? == 0 {
                    continue;
                }
                let (mut last, mut next) = (8i32, 8i32);
                for _ in 0..if i < 6 { 16 } else { 64 } {
                    if next != 0 {
                        let delta = r.se()?;
                        if !(-128..=127).contains(&delta) {
                            return None;
                        }
                        next = (last + delta + 256) % 256;
                    }
                    if next != 0 {
                        last = next;
                    }
                }
            }
        }
    }
    r.ue()?;
    match r.ue()? {
        0 => {
            r.ue()?;
        }
        1 => {
            r.take(1)?;
            r.se()?;
            r.se()?;
            let count = r.ue()?;
            if count > 255 {
                return None;
            }
            for _ in 0..count {
                r.se()?;
            }
        }
        2 => {}
        _ => return None,
    }
    if r.ue()? > 16 {
        return None;
    } // DPB reference-picture bound
    r.take(1)?;
    let width_mbs = u64::from(r.ue()?).checked_add(1)?;
    let height_maps = u64::from(r.ue()?).checked_add(1)?;
    let frame_only = r.take(1)? != 0;
    let coded_pixels = width_mbs
        .checked_mul(height_maps)?
        .checked_mul(if frame_only { 256 } else { 512 })?;
    if coded_pixels > MAX_FRAME_PIXELS as u64 {
        return None;
    }
    if !frame_only {
        r.take(1)?;
    }
    r.take(1)?;
    if r.take(1)? != 0 {
        for _ in 0..4 {
            r.ue()?;
        }
    }
    let mut color = Color::default();
    if r.take(1)? == 0 {
        return Some(color);
    }
    if r.take(1)? != 0 && r.take(8)? == 255 {
        r.take(16)?;
        r.take(16)?;
    }
    if r.take(1)? != 0 {
        r.take(1)?;
    }
    if r.take(1)? != 0 {
        r.take(3)?;
        color.full_range = r.take(1)? != 0;
        if r.take(1)? != 0 {
            r.take(8)?;
            r.take(8)?;
            color.matrix = r.take(8)? as u8;
        }
    }
    Some(color)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn mp4_probe_reads_top_level_box_headers() {
        let probe = |bytes: &[u8]| MovieBytes::from_vec(bytes.to_vec()).probe_mp4();

        assert!(probe(b"\0\0\0\x08free\0\0\0\x10ftypisom\0\0\0\0"));
        assert!(probe(
            b"\0\0\0\x01free\0\0\0\0\0\0\0\x10\0\0\0\x10ftypisom\0\0\0\0"
        ));
        assert!(probe(b"\0\0\0\x08wide\0\0\0\x08mdat\0\0\0\0moov\0\0\0\x08mvhd"));
        assert!(!probe(b"\0\0\0\x18free\0\0\0\x10ftypisom\0\0\0\0"));
        assert!(!probe(b"\0\0\0\x07free\0\0\0\x10ftypisom\0\0\0\0"));
        assert!(!probe(b"\0\0\0\x14ftypisom\0\0\0\0"));
        assert!(!probe(b"\0\0\0\x01free\0\0\0\0"));
    }

    #[test]
    fn mp4_rational_deadlines_keep_fractional_cadence() {
        let base = Rational {
            num: 1,
            den: 30_000,
        };
        assert!(!due(1001, base, 33));
        assert!(due(1001, base, 34));
        assert!(!due(2_700_000, base, 89_999));
        assert!(due(2_700_000, base, 90_000));
        assert!(due(-1001, base, 0));
        assert_eq!(end_ms(1001, base), 34);
    }

    #[test]
    fn mp4_color_range_and_aac_configuration_are_explicit() {
        let frame = Frame {
            width: 2,
            height: 2,
            y: vec![128; 4],
            u: vec![128],
            v: vec![128],
            pic_order_cnt: 0,
            chroma_format: rust_h264::decoder::ChromaFormat::Yuv420,
            bit_depth: 8,
        };
        let full = h264_to_image(
            &frame,
            Color {
                matrix: 1,
                full_range: true,
            },
        )
        .unwrap();
        let limited = h264_to_image(
            &frame,
            Color {
                matrix: 1,
                full_range: false,
            },
        )
        .unwrap();
        assert_eq!(&full.rgba[..4], &[128, 128, 128, 255]);
        assert_eq!(&limited.rgba[..4], &[130, 130, 130, 255]);
        let mut red = frame.clone();
        red.y.fill(81);
        red.u.fill(90);
        red.v.fill(240);
        assert_eq!(
            &h264_to_image(
                &red,
                Color {
                    matrix: 6,
                    full_range: false
                }
            )
            .unwrap()
            .rgba[..4],
            &[254, 0, 0, 255]
        );
        assert_eq!(
            &h264_to_image(
                &red,
                Color {
                    matrix: 1,
                    full_range: false
                }
            )
            .unwrap()
            .rgba[..4],
            &[255, 24, 0, 255]
        );
        let ten = Frame {
            y: vec![0, 2, 0, 2, 0, 2, 0, 2],
            u: vec![0, 2],
            v: vec![0, 2],
            bit_depth: 10,
            ..frame
        };
        assert_eq!(
            &h264_to_image(
                &ten,
                Color {
                    matrix: 1,
                    full_range: true
                }
            )
            .unwrap()
            .rgba[..4],
            &[128, 128, 128, 255]
        );
        assert!(sps_color(&[]).is_none());
        assert!(sps_color(&[0; 32]).is_none());
        assert!(AacConfig::from_stream(&[0x2b, 0x92]).is_none()); // HE-AAC
        assert!(AacConfig::from_stream(&[]).is_none());
        let config = AacConfig::from_stream(&[0x12, 0x10]).unwrap();
        assert_eq!(
            config.adts_header(100).unwrap(),
            [0xff, 0xf1, 0x50, 0x80, 0x0d, 0x7f, 0xfc]
        );
    }

    #[test]
    #[ignore = "requires generated cfr.mp4/vfr.mp4 in ETHORNELL_MP4_FIXTURES"]
    fn mp4_external_timeline_reorder_seek_loop_and_aac() {
        use ethornell_audio::AudioSystem;
        use kira::manager::{AudioManagerSettings, backend::mock::MockBackend};
        let dir = std::path::PathBuf::from(std::env::var_os("ETHORNELL_MP4_FIXTURES").unwrap());
        for (file, full_range, matrix) in [("cfr.mp4", false, 1), ("vfr.mp4", true, 6)] {
            let mut bytes = std::fs::read(dir.join(file)).unwrap();
            if file == "vfr.mp4" {
                // Preserve box offsets while exercising a movie without ftyp.
                assert_eq!(&bytes[4..8], b"ftyp");
                bytes[4..8].copy_from_slice(b"free");
            }
            let source = MovieBytes::from_vec(bytes);
            assert!(source.probe_mp4());
            let mut movie = Mp4MovieDecoder::new(source.clone(), file).expect("configured movie");
            assert_eq!(movie.size(), Some((96, 64)));
            assert_eq!(movie.color.full_range, full_range);
            assert_eq!(movie.color.matrix, matrix);
            let positions = [
                0, 33, 34, 66, 67, 99, 100, 967, 999, 1000, 1050, 1100, 1500, 2000, 2200,
            ];
            for position in positions {
                let _ = movie.frame_at(position);
                let expected = (0..60)
                    .filter(|&i| {
                        let pts = if file == "cfr.mp4" {
                            i * 1001
                        } else {
                            i * 1000 + if i >= 30 { 3000 } else { 0 }
                        };
                        due(
                            pts,
                            Rational {
                                num: 1,
                                den: 30_000,
                            },
                            position,
                        )
                    })
                    .count()
                    .saturating_sub(1);
                assert_eq!(
                    movie.frame_index(),
                    Some(expected),
                    "{file} at {position}ms"
                );
                assert!(!movie.failed);
            }
            assert_eq!(movie.frame_index(), Some(59));
            let first = movie.frame_at(0).expect("backward seek");
            assert_eq!(movie.frame_index(), Some(0));
            assert!(movie.frame_at(0).is_none()); // no duplicate upload
            movie.reset();
            assert_eq!(movie.frame_at(0).unwrap().rgba, first.rgba);
            if file == "cfr.mp4" {
                let audio = movie.take_audio().expect("AAC soundtrack");
                let mut system =
                    AudioSystem::<MockBackend>::with_settings(AudioManagerSettings::default())
                        .unwrap();
                system
                    .play_stream_on_channel(1, audio.0, false, 1.0, 0)
                    .unwrap();
                let mut host = crate::graph_effect::GraphEffectRegistry::default();
                assert_eq!(
                    host.configure_media(7, String::new(), file.into(), source),
                    0
                );
                assert!(host.prepare_initial_movie_frame(7).is_some());
                assert_eq!(host.invoke(7).status, 0);
                assert!(!host.tick_movies(500).is_empty());
                assert_eq!(host.seek(7, 0), 0);
                assert!(
                    !host.tick_movies(1).is_empty(),
                    "running seek republishes its picture"
                );
                assert_eq!(host.movie_diagnostics()[0].3, Some(0));
                assert_eq!(host.set_paused(7, true), 0);
                assert!(host.tick_movies(500).is_empty());
                assert_eq!(host.position_ms(7).unwrap(), 1);
                assert_eq!(host.set_paused(7, false), 0);
                assert_eq!(host.set_movie_options(7, true, 128), 0);
                let generation = host.movie_audio()[0].generation;
                host.tick_movies(10_000);
                assert!(host.movie_playing(7));
                assert!(host.movie_audio()[0].generation > generation);
                assert_eq!(host.close_movie_slot(7), 0);
                assert!(host.movie_audio().is_empty());
            }
        }
    }
    #[test]
    #[ignore = "requires generated VP9/AV1 MP4, reference YUV and PTS in ETHORNELL_MP4_FIXTURES"]
    fn mp4_external_vp9_av1_depth_pts_reference_and_lifecycle() {
        let dir = std::path::PathBuf::from(std::env::var_os("ETHORNELL_MP4_FIXTURES").unwrap());
        for codec in ["vp9", "av1", "h264"] {
            for depth in [8, 10] {
                if codec == "h264" && depth == 10 {
                    continue;
                }
                let name = format!("{codec}-{depth}");
                let file = format!("{name}.mp4");
                let source = MovieBytes::from_vec(std::fs::read(dir.join(&file)).unwrap());
                let yuv = std::fs::read(dir.join(format!("{name}.yuv"))).unwrap();
                let pts: Vec<i64> = std::fs::read_to_string(dir.join(format!("{name}.pts")))
                    .unwrap()
                    .lines()
                    .map(|s| s.parse().unwrap())
                    .collect();
                assert_eq!(pts.len(), 60);
                let b = if depth == 8 { 1 } else { 2 };
                let y_len = 96 * 64 * b;
                let c_len = 48 * 32 * b;
                let frame_len = y_len + 2 * c_len;
                assert_eq!(yuv.len(), pts.len() * frame_len);
                let mut movie = Mp4MovieDecoder::new(source.clone(), &file).expect(&name);
                assert_eq!(movie.uses_extra_codec, codec != "h264");
                assert_eq!(movie.size(), Some((96, 64)));
                let mut first = None;
                for (i, &pts) in pts.iter().enumerate() {
                    let at = end_ms(pts, movie.time_base);
                    if i != 0 {
                        assert!(
                            movie.frame_at(at - 1).is_none(),
                            "{name}: no premature picture {i}"
                        );
                        assert_eq!(movie.frame_index(), Some(i - 1));
                    }
                    let image = movie.frame_at(at).expect(&format!("{name}: picture {i}"));
                    assert_eq!(movie.frame_index(), Some(i));
                    assert!(!movie.failed, "{name}: picture {i}");
                    let raw = &yuv[i * frame_len..(i + 1) * frame_len];
                    let expected = planar_to_image(
                        Planar {
                            size: (96, 64),
                            depth,
                            sub: (1, 1),
                            strides: [96 * b, 48 * b, 48 * b],
                            planes: [
                                &raw[..y_len],
                                &raw[y_len..y_len + c_len],
                                &raw[y_len + c_len..],
                            ],
                            monochrome: false,
                        },
                        Color {
                            matrix: 1,
                            full_range: false,
                        },
                    )
                    .unwrap();
                    // Independent FFmpeg-decoded planar reference, with identical
                    // range/matrix conversion. Checks every visible pixel, not a hash.
                    assert!(
                        image.rgba == expected.rgba,
                        "{name}: reference pixels differ at picture {i}"
                    );
                    if i == 0 {
                        first = Some(image.rgba);
                    }
                }
                movie.frame_at(movie.duration_ms() + 1);
                assert!(!movie.failed);
                assert_eq!(movie.frame_index(), Some(59));
                assert_eq!(movie.frame_at(0).unwrap().rgba, *first.as_ref().unwrap());
                movie.reset();
                assert_eq!(movie.frame_at(0).unwrap().rgba, *first.as_ref().unwrap());
                assert!(movie.take_audio().is_some());
                let mut host = crate::graph_effect::GraphEffectRegistry::default();
                assert_eq!(host.configure_media(7, String::new(), file, source), 0);
                assert!(host.prepare_initial_movie_frame(7).is_some());
                assert_eq!(host.invoke(7).status, 0);
                assert!(!host.tick_movies(500).is_empty());
                assert_eq!(host.seek(7, 0), 0);
                assert!(!host.tick_movies(1).is_empty());
                assert_eq!(host.set_paused(7, true), 0);
                assert!(host.tick_movies(500).is_empty());
                assert_eq!(host.position_ms(7).unwrap(), 1);
                assert_eq!(host.set_paused(7, false), 0);
                assert_eq!(host.set_movie_options(7, true, 128), 0);
                let generation = host.movie_audio()[0].generation;
                host.tick_movies(10_000);
                assert!(host.movie_playing(7));
                assert!(host.movie_audio()[0].generation > generation);
                assert_eq!(host.close_movie_slot(7), 0);
                assert!(host.movie_audio().is_empty());
            }
        }
    }

    #[test]
    fn mp4_odd_padded_twelve_bit_and_monochrome_planes() {
        // A 3x3 12-bit image with padding in every plane. Chroma needs ceil
        // dimensions (2x2), and strides are bytes, not sample counts.
        let mut y = vec![0xff; 24];
        let mut c = vec![0xff; 12];
        for row in 0..3 {
            for x in 0..3 {
                y[row * 8 + x * 2..row * 8 + x * 2 + 2].copy_from_slice(&2048u16.to_le_bytes());
            }
        }
        for row in 0..2 {
            for x in 0..2 {
                c[row * 6 + x * 2..row * 6 + x * 2 + 2].copy_from_slice(&2048u16.to_le_bytes());
            }
        }
        let image = planar_to_image(
            Planar {
                size: (3, 3),
                depth: 12,
                sub: (1, 1),
                strides: [8, 6, 6],
                planes: [&y, &c, &c],
                monochrome: false,
            },
            Color {
                matrix: 1,
                full_range: true,
            },
        )
        .unwrap();
        assert!(
            image
                .rgba
                .chunks_exact(4)
                .all(|p| p == [128, 128, 128, 255])
        );
        let mono = planar_to_image(
            Planar {
                size: (3, 3),
                depth: 12,
                sub: (0, 0),
                strides: [8, 0, 0],
                planes: [&y, &[], &[]],
                monochrome: true,
            },
            Color {
                matrix: 1,
                full_range: true,
            },
        )
        .unwrap();
        assert_eq!(mono.rgba, image.rgba);
        assert!(
            planar_to_image(
                Planar {
                    size: (3, 3),
                    depth: 12,
                    sub: (1, 1),
                    strides: [8, 6, 6],
                    planes: [&y, &c[..9], &c],
                    monochrome: false,
                },
                Color::default()
            )
            .is_none()
        );
    }

    #[test]
    fn reorder_depth_computation_matches_gop_b_frame_structure() {
        // I P P P (no B-frames)
        assert_eq!(required_gop_reorder_depth(&[0, 3000, 6000, 9000]), 1);
        // Classic I0 P3 B1 B2 P6 B4 B5
        assert_eq!(
            required_gop_reorder_depth(&[0, 9000, 3000, 6000, 18000, 12000, 15000]),
            1
        );
        // Hierarchical B-pyramid I0 P4 B2 B1 B3
        assert_eq!(
            required_gop_reorder_depth(&[0, 12000, 6000, 3000, 9000]),
            2
        );
        // Deeper 4-level B-pyramid
        assert_eq!(
            required_gop_reorder_depth(&[0, 24000, 12000, 6000, 3000, 9000, 18000, 15000, 21000]),
            3
        );
    }

    #[test]
    fn fast_yuv420_paths_are_bit_exact_for_8bit_and_10bit() {
        let mut seed = 0x1234_5678_9abc_def0u64;
        let mut next = || {
            seed ^= seed << 13;
            seed ^= seed >> 7;
            seed ^= seed << 17;
            seed
        };
        for &(w, h) in &[(16u32, 12u32), (17u32, 13u32), (3u32, 3u32)] {
            let cw = (w as usize).div_ceil(2);
            let ch = (h as usize).div_ceil(2);
            for &depth in &[8usize, 10usize] {
                let bps = if depth == 8 { 1 } else { 2 };
                let y_stride = w as usize * bps + 4;
                let c_stride = cw * bps + 4;
                let mut y = vec![0u8; h as usize * y_stride];
                let mut u = vec![0u8; ch * c_stride];
                let mut v = vec![0u8; ch * c_stride];
                for buf in [&mut y, &mut u, &mut v] {
                    if bps == 1 {
                        for b in buf.iter_mut() {
                            *b = (next() & 0xff) as u8;
                        }
                    } else {
                        for chunk in buf.chunks_exact_mut(2) {
                            let val = (next() & 0x3ff) as u16;
                            chunk.copy_from_slice(&val.to_le_bytes());
                        }
                    }
                }
                for &matrix in &[1u8, 6u8] {
                    for &full_range in &[false, true] {
                        let color = Color { matrix, full_range };
                        let got = planar_to_image(
                            Planar {
                                size: (w, h),
                                depth,
                                sub: (1, 1),
                                strides: [y_stride, c_stride, c_stride],
                                planes: [&y, &u, &v],
                                monochrome: false,
                            },
                            color,
                        )
                        .unwrap();
                        // Compare against 12-bit shifted generic reference (depth=12 with values << (12-depth))
                        let scale_bits = 12 - depth;
                        let mut y12 = vec![0u8; h as usize * (w as usize * 2)];
                        let mut u12 = vec![0u8; ch * (cw * 2)];
                        let mut v12 = vec![0u8; ch * (cw * 2)];
                        for row in 0..h as usize {
                            for x in 0..w as usize {
                                let raw = if bps == 1 {
                                    u16::from(y[row * y_stride + x])
                                } else {
                                    u16::from_le_bytes([
                                        y[row * y_stride + x * 2],
                                        y[row * y_stride + x * 2 + 1],
                                    ])
                                };
                                let v12_val = raw << scale_bits;
                                y12[(row * w as usize + x) * 2..(row * w as usize + x) * 2 + 2]
                                    .copy_from_slice(&v12_val.to_le_bytes());
                            }
                        }
                        for (src, dst) in [(&u, &mut u12), (&v, &mut v12)] {
                            for row in 0..ch {
                                for x in 0..cw {
                                    let raw = if bps == 1 {
                                        u16::from(src[row * c_stride + x])
                                    } else {
                                        u16::from_le_bytes([
                                            src[row * c_stride + x * 2],
                                            src[row * c_stride + x * 2 + 1],
                                        ])
                                    };
                                    let v12_val = raw << scale_bits;
                                    dst[(row * cw + x) * 2..(row * cw + x) * 2 + 2]
                                        .copy_from_slice(&v12_val.to_le_bytes());
                                }
                            }
                        }
                        let expected = planar_to_image(
                            Planar {
                                size: (w, h),
                                depth: 12,
                                sub: (1, 1),
                                strides: [w as usize * 2, cw * 2, cw * 2],
                                planes: [&y12, &u12, &v12],
                                monochrome: false,
                            },
                            color,
                        )
                        .unwrap();
                        assert_eq!(
                            got.rgba, expected.rgba,
                            "mismatch {w}x{h} depth={depth} matrix={matrix} full={full_range}"
                        );
                    }
                }
            }
        }
    }
}
