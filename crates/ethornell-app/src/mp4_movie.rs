//! Optional MP4/H.264/AAC-LC playback. The container yields Annex-B samples,
//! including SPS/PPS. Display PTS follows each picture through B-frame reorder;
//! neither average frame duration nor integer milliseconds select pictures.
use std::collections::VecDeque;
use std::fmt;
use std::io::Cursor;

use ethornell_image::DecodedImage;
use rff_core::{CodecId, Error as DemuxError, MediaType, Rational};
use rff_format::{Demuxer, FormatRegistry, Stream};
use rust_h264::decoder::Frame;
use rust_h264::nal::{NalUnitType, parse_annex_b};
use rust_h264::threading::ThreadedDecoder;

use crate::MovieBytes;
use crate::audio_runtime::StreamBytes;

const MAX_PENDING_FRAMES: usize = 32;
const MAX_AUDIO_BYTES: usize = 32 * 1024 * 1024;
const MAX_FRAME_PIXELS: usize = 4096 * 4096;

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
struct TimedFrame {
    frame: Frame,
    timing: Timing,
}

pub(crate) struct Mp4MovieDecoder {
    source: MovieBytes,
    demuxer: Box<dyn Demuxer>,
    video_index: usize,
    time_base: Rational,
    decoder: ThreadedDecoder,
    pending: VecDeque<TimedFrame>,
    pending_timings: Vec<Timing>,
    current: Option<TimedFrame>,
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
        f.debug_struct("Mp4MovieDecoder")
            .field("size", &self.size)
            .field("duration_ms", &self.duration_ms)
            .field("pending_frames", &self.pending.len())
            .field("failed", &self.failed)
            .finish()
    }
}
fn open(source: MovieBytes) -> Option<(Box<dyn Demuxer>, Vec<Stream>)> {
    let mut registry = FormatRegistry::new();
    rff_format_mp4::register(&mut registry);
    let mut demuxer = registry
        .open_demuxer_seekable("mp4", Box::new(Cursor::new(source)))
        .ok()?;
    let streams = demuxer.read_header().ok()?;
    Some((demuxer, streams))
}
impl Mp4MovieDecoder {
    pub(crate) fn supports_resource(resource: &str) -> bool {
        resource
            .rsplit_once('.')
            .is_some_and(|(_, ext)| ext.eq_ignore_ascii_case("mp4"))
    }
    pub(crate) fn probe(bytes: &[u8]) -> bool {
        bytes.len() >= 8 && &bytes[4..8] == b"ftyp"
    }
    pub(crate) fn new(source: MovieBytes, _resource: &str) -> Option<Self> {
        let (mut scan, streams) = open(source.clone())?;
        let video = streams.iter().find(|s| s.media_type == MediaType::Video)?;
        if video.codec_id != CodecId::H264 || video.time_base.num <= 0 || video.time_base.den <= 0 {
            tracing::warn!(codec=?video.codec_id, "mp4-h264 requires an H.264 video track with valid timestamps");
            return None;
        }
        let size = (video.width, video.height);
        let pixels = (size.0 as usize).checked_mul(size.1 as usize)?;
        if pixels == 0 || pixels > MAX_FRAME_PIXELS {
            return None;
        }
        let video_index = video.index;
        let time_base = video.time_base;
        let audio_track = streams.iter().find(|s| s.media_type == MediaType::Audio);
        let audio_config = match audio_track {
            Some(track) if track.codec_id == CodecId::Aac => {
                Some(AacConfig::from_stream(&track.extradata)?)
            }
            Some(track) => {
                tracing::warn!(codec=?track.codec_id, "mp4-h264 supports AAC-LC movie audio");
                return None;
            }
            None => None,
        };
        // The existing audio host consumes compressed elementary streams.
        // Scan AAC once; do not duplicate the compressed video or retain an
        // RGBA timeline. The retained MovieBytes keeps Vita CDRAM ownership.
        let mut audio = Vec::new();
        let mut audio_start_ms = None;
        let mut duration_ms = video.duration.map(|n| end_ms(n, time_base)).unwrap_or(0);
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
        if duration_ms <= 0 {
            return None;
        }
        let (demuxer, _) = open(source.clone())?;
        let mut movie = Self {
            source,
            demuxer,
            video_index,
            time_base,
            decoder: ThreadedDecoder::new(4),
            pending: VecDeque::new(),
            pending_timings: Vec::new(),
            current: None,
            delivered: false,
            color: Color::default(),
            size,
            duration_ms,
            audio_start_ms: audio_start_ms.unwrap_or(0),
            audio: (!audio.is_empty()).then(|| StreamBytes(audio.into())),
            requested_ms: 0,
            frame_index: None,
            eof: false,
            failed: false,
        };
        // Fail configuration if no picture can be decoded, rather than
        // acknowledging a movie that will only play its soundtrack.
        let first = movie.next_frame()?;
        movie.pending.push_front(first);
        Some(movie)
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
    pub(crate) fn invalidate_delivery(&mut self) {
        self.delivered = false;
    }
    pub(crate) fn reset(&mut self) {
        self.pending.clear();
        self.pending_timings.clear();
        self.current = None;
        self.delivered = false;
        self.frame_index = None;
        self.requested_ms = 0;
        self.color = Color::default();
        self.decoder = ThreadedDecoder::new(4);
        self.eof = false;
        self.failed = false;
        match open(self.source.clone()) {
            Some((demuxer, _)) => self.demuxer = demuxer,
            None => self.failed = true,
        }
    }
    pub(crate) fn frame_at(&mut self, position_ms: i32) -> Option<DecodedImage> {
        let position_ms = position_ms.max(0);
        if position_ms < self.requested_ms {
            self.reset();
        }
        self.requested_ms = position_ms;
        let mut selected = None;
        loop {
            let frame = self.next_frame();
            let Some(frame) = frame else {
                break;
            };
            if !due(frame.timing.pts, self.time_base, position_ms) {
                self.pending.push_front(frame);
                break;
            }
            self.frame_index = Some(self.frame_index.map_or(0, |index| index + 1));
            selected = Some(frame);
        }
        if let Some(selected) = selected {
            self.current = Some(selected);
            self.delivered = false;
        }
        if self.delivered {
            return None;
        }
        let selected = self.current.as_ref()?;
        let image = h264_to_image(&selected.frame, selected.timing.color);
        if image.is_none() {
            self.failed = true;
            tracing::warn!("unsupported MP4 picture layout/color metadata");
        }
        self.delivered = image.is_some();
        image
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
                    && matches!(nal.nal_unit_type, NalUnitType::Slice | NalUnitType::SliceIdr)
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
        let mut timed = VecDeque::with_capacity(frames.len());
        for frame in frames {
            let Some(timing) = self.take_earliest_timing() else {
                self.failed = true;
                tracing::warn!("MP4 decoder produced a frame without timing");
                return false;
            };
            timed.push_back(TimedFrame { frame, timing });
        }
        self.pending.extend(timed);
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
    let width = frame.width as usize;
    let height = frame.height as usize;
    let pixels = width.checked_mul(height)?;
    if pixels == 0 || pixels > MAX_FRAME_PIXELS || !matches!(frame.bit_depth, 8 | 10) {
        return None;
    }
    let (kr, kb) = match color.matrix {
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
    let sub_y = match frame.chroma_format {
        rust_h264::decoder::ChromaFormat::Yuv420 => 1,
        rust_h264::decoder::ChromaFormat::Yuv422 => 0,
    };
    let shift = frame.bit_depth - 8;
    let sample_bytes = frame.bytes_per_sample();
    let sample = |plane: &[u8], i: usize| -> Option<i32> {
        let offset = i.checked_mul(sample_bytes)?;
        let value = if sample_bytes == 1 {
            u16::from(*plane.get(offset)?)
        } else {
            u16::from_le_bytes(plane.get(offset..offset + 2)?.try_into().ok()?)
        };
        Some(i32::from(value >> shift))
    };
    let mut rgba = vec![0; pixels.checked_mul(4)?];
    for y in 0..height {
        for x in 0..width {
            let yy = sample(&frame.y, y * width + x)? - if color.full_range { 0 } else { 16 };
            let ci = (y >> sub_y) * frame.chroma_width() as usize + (x >> 1);
            let u = sample(&frame.u, ci)? - 128;
            let v = sample(&frame.v, ci)? - 128;
            let luma = coeff[0] * yy;
            let rgb = [
                luma + coeff[1] * v,
                luma + coeff[2] * u + coeff[3] * v,
                luma + coeff[4] * u,
            ];
            let offset = (y * width + x) * 4;
            for i in 0..3 {
                rgba[offset + i] = ((rgb[i] + 8192) >> 14).clamp(0, 255) as u8;
            }
            rgba[offset + 3] = 255;
        }
    }
    Some(DecodedImage {
        width: frame.width,
        height: frame.height,
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
            let source = MovieBytes::from_vec(std::fs::read(dir.join(file)).unwrap());
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
}
