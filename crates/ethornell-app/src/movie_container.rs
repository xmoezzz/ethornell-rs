use std::collections::VecDeque;
use std::fmt;
use std::io::Cursor;
use std::ops::Range;

use ethornell_image::DecodedImage;
use rff_core::{CodecId, Error as DemuxError, MediaType};
use rff_format::{Demuxer, FormatRegistry};
use rusty_av1d::rust_api::{Decoder as Av1Decoder, Picture as Av1Picture, PlanarImageComponent};
use rusty_vp9::{DecodedFrame as Vp9Frame, Error as Vp9Error, Vp9Decoder};
use rust_h264::decoder::{Frame as H264Frame, OrderedDecoder as H264Decoder};
use rust_h264::nal::parse_annex_b;

use crate::audio_runtime::StreamBytes;

struct PacketRange {
    pts: Option<i64>,
    duration: i64,
    range: Range<usize>,
}

#[derive(Clone, Copy)]
enum CodecKind {
    H264,
    Vp9,
    Av1,
}

enum VideoDecoder {
    H264(H264Decoder),
    Vp9(Vp9Decoder),
    Av1(Av1Decoder),
}

pub(crate) struct ContainerMovieDecoder {
    source: Vec<u8>,
    packets: Vec<PacketRange>,
    decoder: VideoDecoder,
    pending_frames: VecDeque<DecodedImage>,
    packet_index: usize,
    codec: CodecKind,
    width: u32,
    height: u32,
    frame_interval_ms: i32,
    duration_ms: i32,
    audio: Option<StreamBytes>,
    flushed: bool,
    failed: bool,
}

impl fmt::Debug for ContainerMovieDecoder {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let codec = match self.codec {
            CodecKind::H264 => "h264",
            CodecKind::Vp9 => "vp9",
            CodecKind::Av1 => "av1",
        };
        f.debug_struct("ContainerMovieDecoder")
            .field("codec", &codec)
            .field("width", &self.width)
            .field("height", &self.height)
            .field("duration_ms", &self.duration_ms)
            .field("packet_index", &self.packet_index)
            .field("packet_count", &self.packets.len())
            .finish_non_exhaustive()
    }
}

impl ContainerMovieDecoder {
    pub(crate) fn supports_resource(resource: &str) -> bool {
        matches!(
            resource.rsplit_once('.').map(|(_, extension)| extension.to_ascii_lowercase()),
            Some(extension) if matches!(extension.as_str(), "mp4" | "mov" | "m4v" | "mkv" | "webm" | "ivf")
        )
    }

    pub(crate) fn probe(bytes: &[u8]) -> bool {
        format_registry().probe(bytes).is_some()
    }

    pub(crate) fn new(bytes: &[u8], resource: &str) -> Option<Self> {
        let extension = resource.rsplit_once('.').map(|(_, extension)| extension.to_ascii_lowercase());
        if extension.as_deref() == Some("ivf") && bytes.get(8..12) == Some(b"AV01") {
            return Self::new_av1_ivf(bytes);
        }
        let registry = format_registry();
        let format_name = extension
            .as_deref()
            .and_then(|extension| registry.by_extension(extension).map(|format| format.name))
            .or_else(|| registry.probe(bytes).map(|format| format.name))?;
        let mut demuxer = registry
            .open_demuxer_seekable(format_name, Box::new(Cursor::new(bytes.to_vec())))
            .ok()?;
        let streams = demuxer.read_header().ok()?;
        let video = streams
            .iter()
            .find(|stream| stream.media_type == MediaType::Video)?;
        let codec = match video.codec_id {
            CodecId::H264 => CodecKind::H264,
            CodecId::Vp9 => CodecKind::Vp9,
            CodecId::Avif => CodecKind::Av1,
            _ => return None,
        };
        let video_index = video.index;
        let width = video.width;
        let height = video.height;
        let time_base = video.time_base;
        let duration_ms = video
            .duration
            .map(|duration| ticks_to_ms(duration, time_base))
            .unwrap_or_default();
        let frame_count = video.nb_frames;
        let decoder = new_decoder(codec)?;
        let mut source = Vec::new();
        let mut packets = Vec::new();
        let mut audio = Vec::new();
        let mut audio_written = false;
        let audio_stream = streams
            .iter()
            .find(|stream| stream.media_type == MediaType::Audio && stream.codec_id == CodecId::Aac);
        let mut audio_config = audio_stream.map(|stream| AacConfig::from_stream(stream.sample_rate, stream.channels, &stream.extradata));
        let audio_index = audio_stream.map(|stream| stream.index);
        let mut first_packet_duration = None;
        let mut last_video_end_ms = 0_i64;

        loop {
            let packet = match demuxer.read_packet() {
                Ok(packet) => packet,
                Err(DemuxError::Eof) => break,
                Err(_) => return None,
            };
            if packet.stream_index == video_index {
                if first_packet_duration.is_none() && packet.duration > 0 {
                    // Packet timestamps are expressed in the stream's time
                    // base; `Packet::time_base` is only a default the
                    // containers do not all fill in, so a 1000-tick frame at
                    // timescale 24000 must not be read as a 1000 ms frame.
                    first_packet_duration = Some(ticks_to_ms(packet.duration, time_base));
                }
                if let (Some(pts), Some(duration)) = (packet.pts, Some(packet.duration.max(0))) {
                    last_video_end_ms = last_video_end_ms.max(i64::from(ticks_to_ms(
                        pts.saturating_add(duration),
                        time_base,
                    )));
                }
                let start = source.len();
                source.extend_from_slice(&packet.data);
                packets.push(PacketRange {
                    pts: packet.pts,
                    duration: packet.duration,
                    range: start..source.len(),
                });
            } else if Some(packet.stream_index) == audio_index {
                if let Some(config) = audio_config.as_mut() {
                    if let Some(header) = config.adts_header(packet.data.len()) {
                        audio.extend_from_slice(&header);
                        audio.extend_from_slice(&packet.data);
                        audio_written = true;
                    }
                }
            }
        }
        if packets.is_empty() || source.is_empty() {
            return None;
        }
        let duration_ms = if duration_ms > 0 {
            duration_ms
        } else {
            last_video_end_ms.clamp(0, i32::MAX as i64) as i32
        };
        // Average the stream's own duration over its pictures first: it needs
        // no per-packet timing and stays right for every container. A single
        // packet duration is the last resort before the 30 fps assumption.
        let frame_interval_ms = frame_count
            .filter(|count| *count > 1)
            .map(|count| duration_ms / count.min(i32::MAX as u64) as i32)
            .filter(|interval| *interval > 0)
            .or_else(|| {
                (duration_ms > 0 && packets.len() > 1)
                    .then(|| duration_ms / packets.len() as i32)
                    .filter(|interval| *interval > 0)
            })
            .or(first_packet_duration.filter(|duration| *duration > 0))
            .unwrap_or(33)
            .clamp(1, 1000);

        Some(Self {
            source,
            packets,
            decoder,
            pending_frames: VecDeque::new(),
            packet_index: 0,
            codec,
            width,
            height,
            frame_interval_ms,
            duration_ms,
            audio: audio_written.then(|| StreamBytes(audio.into())),
            flushed: false,
            failed: false,
        })
    }

    fn new_av1_ivf(bytes: &[u8]) -> Option<Self> {
        if bytes.len() < 32 || &bytes[..4] != b"DKIF" {
            return None;
        }
        let width = u32::from(u16::from_le_bytes([bytes[12], bytes[13]]));
        let height = u32::from(u16::from_le_bytes([bytes[14], bytes[15]]));
        let rate_num = u32::from_le_bytes(bytes[16..20].try_into().ok()?);
        let rate_den = u32::from_le_bytes(bytes[20..24].try_into().ok()?);
        if width == 0 || height == 0 || rate_num == 0 || rate_den == 0 {
            return None;
        }
        let time_base = rff_core::Rational::new(rate_den.min(i32::MAX as u32) as i32, rate_num.min(i32::MAX as u32) as i32);
        let mut source = Vec::new();
        let mut packets = Vec::new();
        let mut offset = 32usize;
        while offset + 12 <= bytes.len() {
            let size = u32::from_le_bytes(bytes[offset..offset + 4].try_into().ok()?) as usize;
            let pts = u64::from_le_bytes(bytes[offset + 4..offset + 12].try_into().ok()?);
            offset += 12;
            let end = offset.checked_add(size)?;
            let data = bytes.get(offset..end)?;
            let start = source.len();
            source.extend_from_slice(data);
            packets.push(PacketRange {
                pts: Some(pts.min(i64::MAX as u64) as i64),
                duration: 1,
                range: start..source.len(),
            });
            offset = end;
        }
        if packets.is_empty() || source.is_empty() {
            return None;
        }
        let frame_interval_ms = ticks_to_ms(1, time_base).max(1);
        let duration_ms = ticks_to_ms(
            packets
                .last()
                .and_then(|packet| packet.pts)
                .unwrap_or_default()
                .saturating_add(1),
            time_base,
        );
        Some(Self {
            source,
            packets,
            decoder: new_decoder(CodecKind::Av1)?,
            pending_frames: VecDeque::new(),
            packet_index: 0,
            codec: CodecKind::Av1,
            width,
            height,
            frame_interval_ms,
            duration_ms,
            audio: None,
            flushed: false,
            failed: false,
        })
    }

    pub(crate) fn duration_ms(&self) -> i32 {
        self.duration_ms
    }

    pub(crate) fn frame_interval_ms(&self) -> i32 {
        self.frame_interval_ms
    }

    pub(crate) fn size(&self) -> Option<(u32, u32)> {
        (self.width > 0 && self.height > 0).then_some((self.width, self.height))
    }

    pub(crate) fn take_audio(&mut self) -> Option<StreamBytes> {
        self.audio.take()
    }

    pub(crate) fn reset(&mut self) -> bool {
        let Some(decoder) = new_decoder(self.codec) else {
            self.failed = true;
            return false;
        };
        self.decoder = decoder;
        self.pending_frames.clear();
        self.packet_index = 0;
        self.flushed = false;
        self.failed = false;
        true
    }

    pub(crate) fn next_frame(&mut self) -> Option<DecodedImage> {
        loop {
            if let Some(frame) = self.pending_frames.pop_front() {
                return Some(frame);
            }
            if self.failed {
                return None;
            }
            if let Some(packet) = self.packets.get(self.packet_index) {
                let range = packet.range.clone();
                let pts = packet.pts;
                let duration = packet.duration;
                self.packet_index += 1;
                let data = self.source[range].to_vec();
                if !self.decode_packet(&data, pts, duration) {
                    self.failed = true;
                }
                continue;
            }
            if !self.flushed {
                self.flushed = true;
                if !self.flush_decoder() {
                    self.failed = true;
                }
                continue;
            }
            return None;
        }
    }

    fn decode_packet(&mut self, data: &[u8], pts: Option<i64>, duration: i64) -> bool {
        match &mut self.decoder {
            VideoDecoder::H264(decoder) => {
                for nal in parse_annex_b(data) {
                    let Ok(frames) = decoder.decode_nal(&nal) else {
                        return false;
                    };
                    self.pending_frames
                        .extend(frames.into_iter().filter_map(h264_to_image));
                }
                true
            }
            VideoDecoder::Vp9(decoder) => {
                if decoder.push(data, pts).is_err() {
                    return false;
                }
                drain_vp9(decoder, &mut self.pending_frames, false)
            }
            VideoDecoder::Av1(decoder) => {
                let data = data.to_vec().into_boxed_slice();
                match decoder.send_data(data, None, pts, Some(duration)) {
                    Ok(()) => drain_av1(decoder, &mut self.pending_frames),
                    Err(rusty_av1d::Rav1dError::TryAgain) => {
                        if !drain_av1(decoder, &mut self.pending_frames) {
                            return false;
                        }
                        match decoder.send_pending_data() {
                            Ok(()) => drain_av1(decoder, &mut self.pending_frames),
                            Err(rusty_av1d::Rav1dError::TryAgain) => {
                                drain_av1(decoder, &mut self.pending_frames)
                            }
                            Err(_) => false,
                        }
                    }
                    Err(_) => false,
                }
            }
        }
    }

    fn flush_decoder(&mut self) -> bool {
        match &mut self.decoder {
            VideoDecoder::H264(decoder) => {
                self.pending_frames
                    .extend(decoder.flush().into_iter().filter_map(h264_to_image));
                true
            }
            VideoDecoder::Vp9(decoder) => {
                decoder.flush();
                drain_vp9(decoder, &mut self.pending_frames, true)
            }
            VideoDecoder::Av1(decoder) => {
                decoder.flush();
                drain_av1(decoder, &mut self.pending_frames)
            }
        }
    }
}

fn format_registry() -> FormatRegistry {
    let mut registry = FormatRegistry::new();
    rff_format_mp4::register(&mut registry);
    rff_format_mkv::register(&mut registry);
    rff_format_ivf::register(&mut registry);
    registry
}

fn new_decoder(codec: CodecKind) -> Option<VideoDecoder> {
    Some(match codec {
        CodecKind::H264 => VideoDecoder::H264(H264Decoder::new()),
        CodecKind::Vp9 => VideoDecoder::Vp9(Vp9Decoder::new()),
        CodecKind::Av1 => VideoDecoder::Av1(Av1Decoder::new().ok()?),
    })
}

fn ticks_to_ms(ticks: i64, time_base: rff_core::Rational) -> i32 {
    if time_base.den == 0 {
        return 0;
    }
    (ticks as i128 * time_base.num as i128 * 1000 / time_base.den as i128)
        .clamp(0, i32::MAX as i128) as i32
}

fn drain_vp9(decoder: &mut Vp9Decoder, pending: &mut VecDeque<DecodedImage>, flushing: bool) -> bool {
    loop {
        match decoder.next_frame() {
            Ok(frame) => {
                let Some(image) = vp9_to_image(&frame) else {
                    return false;
                };
                pending.push_back(image);
            }
            Err(Vp9Error::Again) => return true,
            Err(Vp9Error::Eof) if flushing => return true,
            Err(_) => return false,
        }
    }
}

fn drain_av1(decoder: &mut Av1Decoder, pending: &mut VecDeque<DecodedImage>) -> bool {
    loop {
        match decoder.get_picture() {
            Ok(picture) => {
                let Some(image) = av1_to_image(&picture) else {
                    return false;
                };
                pending.push_back(image);
            }
            Err(rusty_av1d::Rav1dError::TryAgain) => return true,
            Err(_) => return false,
        }
    }
}

fn h264_to_image(frame: H264Frame) -> Option<DecodedImage> {
    let (sub_x, sub_y) = match frame.chroma_format {
        rust_h264::decoder::ChromaFormat::Yuv420 => (1, 1),
        rust_h264::decoder::ChromaFormat::Yuv422 => (1, 0),
    };
    let bytes_per_sample = frame.bytes_per_sample();
    let chroma_width = (frame.width as usize) >> sub_x;
    let chroma_height = (frame.height as usize) >> sub_y;
    yuv_to_image(
        frame.width,
        frame.height,
        frame.bit_depth as usize,
        sub_x,
        sub_y,
        frame.width as usize * bytes_per_sample,
        chroma_width * bytes_per_sample,
        chroma_width * bytes_per_sample,
        Some(&frame.y),
        Some(&frame.u),
        Some(&frame.v),
        chroma_height,
    )
}

fn vp9_to_image(frame: &Vp9Frame) -> Option<DecodedImage> {
    let chroma_height = (frame.height as usize) >> frame.subsampling_y;
    yuv_to_image(
        frame.width,
        frame.height,
        frame.bit_depth as usize,
        frame.subsampling_x as usize,
        frame.subsampling_y as usize,
        *frame.strides.first()?,
        *frame.strides.get(1)?,
        *frame.strides.get(2)?,
        frame.planes.first().map(Vec::as_slice),
        frame.planes.get(1).map(Vec::as_slice),
        frame.planes.get(2).map(Vec::as_slice),
        chroma_height,
    )
}

fn av1_to_image(frame: &Av1Picture) -> Option<DecodedImage> {
    let (sub_x, sub_y) = match frame.pixel_layout() {
        rusty_av1d::PixelLayout::I400 => (0, 0),
        rusty_av1d::PixelLayout::I420 => (1, 1),
        rusty_av1d::PixelLayout::I422 => (1, 0),
        rusty_av1d::PixelLayout::I444 => (0, 0),
    };
    let bytes_per_sample = if frame.bit_depth() > 8 { 2 } else { 1 };
    let component = [
        PlanarImageComponent::Y,
        PlanarImageComponent::U,
        PlanarImageComponent::V,
    ];
    let planes = component.map(|component| frame.plane(component));
    yuv_to_image(
        frame.width(),
        frame.height(),
        frame.bits_per_component()?.0 as usize,
        sub_x,
        sub_y,
        frame.stride(PlanarImageComponent::Y) as usize * bytes_per_sample,
        frame.stride(PlanarImageComponent::U) as usize * bytes_per_sample,
        frame.stride(PlanarImageComponent::V) as usize * bytes_per_sample,
        Some(planes[0]),
        (frame.pixel_layout() != rusty_av1d::PixelLayout::I400).then_some(planes[1]),
        (frame.pixel_layout() != rusty_av1d::PixelLayout::I400).then_some(planes[2]),
        (frame.height() as usize) >> sub_y,
    )
}

fn yuv_to_image(
    width: u32,
    height: u32,
    bit_depth: usize,
    sub_x: usize,
    sub_y: usize,
    y_stride: usize,
    u_stride: usize,
    v_stride: usize,
    y_plane: Option<&[u8]>,
    u_plane: Option<&[u8]>,
    v_plane: Option<&[u8]>,
    _chroma_height: usize,
) -> Option<DecodedImage> {
    let width_usize = width as usize;
    let height_usize = height as usize;
    let sample_bytes = if bit_depth > 8 { 2 } else { 1 };
    let shift = bit_depth.saturating_sub(8) as u32;
    let mut rgba = vec![0; width_usize.checked_mul(height_usize)?.checked_mul(4)?];
    for y in 0..height_usize {
        for x in 0..width_usize {
            let yy = plane_sample(y_plane?, y, x, y_stride, sample_bytes)? >> shift;
            let cx = x >> sub_x;
            let cy = y >> sub_y;
            let uu = if let Some(plane) = u_plane {
                plane_sample(plane, cy, cx, u_stride, sample_bytes)? >> shift
            } else {
                128
            };
            let vv = if let Some(plane) = v_plane {
                plane_sample(plane, cy, cx, v_stride, sample_bytes)? >> shift
            } else {
                128
            };
            let c = (yy as i32 - 16).max(0);
            let d = uu as i32 - 128;
            let e = vv as i32 - 128;
            let r = (298 * c + 459 * e + 128) >> 8;
            let g = (298 * c - 55 * d - 136 * e + 128) >> 8;
            let b = (298 * c + 541 * d + 128) >> 8;
            let offset = (y * width_usize + x) * 4;
            rgba[offset] = r.clamp(0, 255) as u8;
            rgba[offset + 1] = g.clamp(0, 255) as u8;
            rgba[offset + 2] = b.clamp(0, 255) as u8;
            rgba[offset + 3] = 255;
        }
    }
    Some(DecodedImage { width, height, rgba })
}

fn plane_sample(plane: &[u8], y: usize, x: usize, stride: usize, sample_bytes: usize) -> Option<u16> {
    let offset = y.checked_mul(stride)?.checked_add(x.checked_mul(sample_bytes)?)?;
    if sample_bytes == 1 {
        Some(u16::from(*plane.get(offset)?))
    } else {
        Some(u16::from_le_bytes(plane.get(offset..offset + 2)?.try_into().ok()?))
    }
}

struct AacConfig {
    object_type: u8,
    frequency_index: u8,
    channels: u8,
}

impl AacConfig {
    fn from_stream(sample_rate: u32, channels: u16, extra: &[u8]) -> Self {
        let rates = [96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350];
        let fallback_frequency = rates.iter().position(|rate| *rate == sample_rate).unwrap_or(4) as u8;
        if extra.len() >= 2 {
            let object_type = extra[0] >> 3;
            let frequency_index = ((extra[0] & 7) << 1) | (extra[1] >> 7);
            let channel_config = (extra[1] >> 3) & 0x0f;
            if (1..=4).contains(&object_type) && frequency_index < 13 && channel_config > 0 {
                return Self {
                    object_type,
                    frequency_index,
                    channels: channel_config,
                };
            }
        }
        Self {
            object_type: 2,
            frequency_index: fallback_frequency,
            channels: channels.clamp(1, 7) as u8,
        }
    }

    fn adts_header(&self, payload_len: usize) -> Option<[u8; 7]> {
        let frame_len = payload_len.checked_add(7)?;
        if frame_len > 0x1fff || self.channels == 0 || self.channels > 7 {
            return None;
        }
        let profile = self.object_type.checked_sub(1)? & 3;
        Some([
            0xff,
            0xf1,
            (profile << 6) | ((self.frequency_index & 0x0f) << 2) | (self.channels >> 2),
            ((self.channels & 3) << 6) | ((frame_len >> 11) as u8 & 3),
            (frame_len >> 3) as u8,
            ((frame_len as u8 & 7) << 5) | 0x1f,
            0xfc,
        ])
    }
}

#[cfg(test)]
mod tests {
    use super::AacConfig;

    #[test]
    fn adts_header_encodes_aac_lc_stereo_frame_length() {
        let config = AacConfig {
            object_type: 2,
            frequency_index: 4,
            channels: 2,
        };
        let header = config.adts_header(100).unwrap();
        assert_eq!(header[0..2], [0xff, 0xf1]);
        assert_eq!(header[2] >> 6, 1);
        assert_eq!(header[3] >> 6, 2);
        assert_eq!(header[6], 0xfc);
    }
}
