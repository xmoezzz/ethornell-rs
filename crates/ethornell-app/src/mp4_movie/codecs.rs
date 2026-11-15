//! MP4 VP9/AV1 extensions. The MP4 dependency leaves `vp09` unidentified and
//! omits video configuration records, so inspect only the bounded sample-entry
//! metadata here. Sample tables, edit lists and packets remain demuxer-owned.
use super::{Color, MAX_FRAME_PIXELS, MAX_PENDING_FRAMES, Planar, planar_to_image};
use ethornell_image::DecodedImage;
use rff_core::CodecId;

pub(super) enum Picture {
    Vp9(rusty_vp9::DecodedFrame, Color),
    Av1(rusty_av1d::rust_api::Picture),
}
impl Picture {
    pub(super) fn to_image(&self) -> Option<DecodedImage> {
        match self {
            Self::Vp9(frame, color) => planar_to_image(
                Planar {
                    size: (frame.width, frame.height),
                    depth: frame.bit_depth as usize,
                    sub: (frame.subsampling_x as usize, frame.subsampling_y as usize),
                    strides: [
                        *frame.strides.first()?,
                        *frame.strides.get(1)?,
                        *frame.strides.get(2)?,
                    ],
                    planes: [
                        frame.planes.first()?,
                        frame.planes.get(1)?,
                        frame.planes.get(2)?,
                    ],
                    monochrome: false,
                },
                *color,
            ),
            Self::Av1(frame) => {
                use rusty_av1d::{PixelLayout, rust_api::PlanarImageComponent as P};
                let (sub, mono) = match frame.pixel_layout() {
                    PixelLayout::I400 => ((0, 0), true),
                    PixelLayout::I420 => ((1, 1), false),
                    PixelLayout::I422 => ((1, 0), false),
                    PixelLayout::I444 => ((0, 0), false),
                };
                // Despite its "pixels" doc comment, this wrapper forwards
                // Rav1dPicture.stride verbatim: BYTES even for 10/12-bit planes.
                let strides = [
                    frame.stride(P::Y) as usize,
                    frame.stride(P::U) as usize,
                    frame.stride(P::V) as usize,
                ];
                let planes = [
                    frame.plane(P::Y),
                    if mono { &[] } else { frame.plane(P::U) },
                    if mono { &[] } else { frame.plane(P::V) },
                ];
                planar_to_image(
                    Planar {
                        size: (frame.width(), frame.height()),
                        depth: frame.bits_per_component()?.0 as usize,
                        sub,
                        strides,
                        planes,
                        monochrome: mono,
                    },
                    Color {
                        matrix: frame.matrix_coefficients() as u8,
                        full_range: frame.color_range() as u8 == 1,
                    },
                )
            }
        }
    }
}

pub(super) enum Decoder {
    Vp9 {
        decoder: Box<rusty_vp9::Vp9Decoder>,
        color: Color,
        refs: [(u32, u32); 8],
    },
    Av1 {
        decoder: rusty_av1d::rust_api::Decoder,
        prefix: Vec<u8>,
        first: bool,
    },
}
impl Decoder {
    pub(super) fn new(source: &[u8], codec: CodecId, size: (u32, u32)) -> Option<Self> {
        let config = video_config(source)?;
        if config.size != size {
            return None;
        }
        match &config.fourcc {
            b"vp09" if codec == CodecId::None || codec == CodecId::Vp9 => Some(Self::Vp9 {
                decoder: Box::new(rusty_vp9::Vp9Decoder::new()),
                color: config.vp9_color?,
                refs: [(0, 0); 8],
            }),
            b"av01" if codec == CodecId::Avif => Some(Self::Av1 {
                decoder: av1_decoder()?,
                prefix: config.av1_prefix?,
                first: true,
            }),
            _ => {
                tracing::warn!(?codec, fourcc=?config.fourcc, "Unsupported MP4 codec or configuration");
                None
            }
        }
    }
    pub(super) fn reset(&mut self) -> Option<()> {
        match self {
            Self::Vp9 { decoder, refs, .. } => {
                *decoder = Box::new(rusty_vp9::Vp9Decoder::new());
                *refs = [(0, 0); 8];
            }
            Self::Av1 { decoder, first, .. } => {
                *decoder = av1_decoder()?;
                *first = true;
            }
        }
        Some(())
    }
    pub(super) fn decode(
        &mut self,
        data: &[u8],
        pts: i64,
        _duration: i64,
    ) -> Option<Vec<(Picture, i64)>> {
        if data.len() > 64 * 1024 * 1024 {
            return None;
        }
        match self {
            Self::Vp9 {
                decoder,
                refs,
                color,
            } => {
                let frames = vp9_parts(data)?;
                // Validate every coded frame, including hidden reference frames,
                // before the codec allocates its reference planes.
                for frame in &frames {
                    let h = rusty_vp9::parse_uncompressed_header(
                        &mut rusty_vp9::BitReader::new(frame),
                        refs,
                    )
                    .ok()?;
                    if !h.show_existing_frame {
                        let pixels = (h.width as usize).checked_mul(h.height as usize)?;
                        if pixels == 0 || pixels > MAX_FRAME_PIXELS {
                            return None;
                        }
                        let mask = if h.key_frame {
                            255
                        } else {
                            h.refresh_frame_flags
                        };
                        for (i, dims) in refs.iter_mut().enumerate() {
                            if mask & (1 << i) != 0 {
                                *dims = (h.width, h.height);
                            }
                        }
                    }
                }
                decoder.push(data, Some(pts)).ok()?;
                let mut output = Vec::new();
                // Again can mean a hidden frame was consumed, with another
                // superframe component still queued. Do not stop at that Again.
                for _ in &frames {
                    match decoder.next_frame() {
                        Ok(frame) => {
                            let pts = frame.pts?;
                            output.push((Picture::Vp9(frame, *color), pts));
                        }
                        Err(rusty_vp9::Error::Again) => (),
                        Err(_) => return None,
                    }
                }
                Some(output)
            }
            Self::Av1 {
                decoder,
                prefix,
                first,
            } => {
                let packet = if *first {
                    *first = false;
                    let mut packet = Vec::with_capacity(prefix.len().checked_add(data.len())?);
                    packet.extend_from_slice(prefix);
                    packet.extend_from_slice(data);
                    packet
                } else {
                    data.to_vec()
                };
                let mut result =
                    decoder.send_data(packet.into_boxed_slice(), None, Some(pts), Some(_duration));
                let mut output = Vec::new();
                for _ in 0..256 {
                    match result {
                        Ok(()) => {
                            drain_av1(decoder, &mut output)?;
                            return Some(output);
                        }
                        Err(rusty_av1d::Rav1dError::TryAgain) => {
                            drain_av1(decoder, &mut output)?;
                            // The wrapper owns unconsumed input after EAGAIN.
                            // Never call send_data again until it is consumed.
                            result = decoder.send_pending_data();
                        }
                        Err(_) => return None,
                    }
                }
                None // bounded failure if the decoder cannot make progress
            }
        }
    }
    pub(super) fn drain(&mut self) -> Option<Vec<(Picture, i64)>> {
        let mut output = Vec::new();
        match self {
            Self::Vp9 { .. } => (), // each packet's superframe queue was consumed
            Self::Av1 { decoder, .. } => {
                drain_av1(decoder, &mut output)?;
            }
        }
        // rav1d flush DISCARDS state. EOF drains get_picture; reset rebuilds it.
        Some(output)
    }
}
fn av1_decoder() -> Option<rusty_av1d::rust_api::Decoder> {
    let mut settings = rusty_av1d::rust_api::Settings::new();
    settings.set_n_threads(1);
    settings.set_max_frame_delay(1);
    settings.set_frame_size_limit(MAX_FRAME_PIXELS as u32);
    rusty_av1d::rust_api::Decoder::with_settings(&settings).ok()
}
fn drain_av1(
    decoder: &mut rusty_av1d::rust_api::Decoder,
    output: &mut Vec<(Picture, i64)>,
) -> Option<()> {
    loop {
        match decoder.get_picture() {
            Ok(picture) => {
                if output.len() >= MAX_PENDING_FRAMES {
                    return None;
                }
                let pts = picture.timestamp()?;
                output.push((Picture::Av1(picture), pts));
            }
            Err(rusty_av1d::Rav1dError::TryAgain) => return Some(()),
            Err(_) => return None,
        }
    }
}

fn vp9_parts(data: &[u8]) -> Option<Vec<&[u8]>> {
    let marker = *data.last()?;
    if marker & 0xe0 != 0xc0 {
        return Some(vec![data]);
    }
    let count = usize::from(marker & 7) + 1;
    let mag = usize::from((marker >> 3) & 3) + 1;
    let Some(start) = data.len().checked_sub(2 + count * mag) else {
        return Some(vec![data]);
    };
    // A coded payload may end with these bits without carrying an index.
    if data[start] != marker {
        return Some(vec![data]);
    }
    let mut offset = 0usize;
    let mut parts = Vec::with_capacity(count);
    for i in 0..count {
        let mut n = 0usize;
        for j in 0..mag {
            n |= usize::from(data[start + 1 + i * mag + j]) << (8 * j);
        }
        if n == 0 {
            return None;
        }
        let end = offset.checked_add(n)?;
        if end > start {
            return None;
        }
        parts.push(&data[offset..end]);
        offset = end;
    }
    (offset == start).then_some(parts)
}

// Read-only fixed-depth ISO BMFF metadata traversal. No payload rewriting and
// no second container/sample-table implementation. Extended sizes are checked.
struct Boxes<'a> {
    data: &'a [u8],
    count: usize,
}
impl<'a> Boxes<'a> {
    fn new(data: &'a [u8]) -> Self {
        Self { data, count: 0 }
    }
    fn next(&mut self) -> Option<Option<([u8; 4], &'a [u8])>> {
        if self.data.is_empty() {
            return Some(None);
        }
        self.count += 1;
        if self.count > 8192 {
            return None;
        }
        let size = u32::from_be_bytes(self.data.get(..4)?.try_into().ok()?);
        let tag = self.data.get(4..8)?.try_into().ok()?;
        let (size, header) = match size {
            0 => (self.data.len(), 8),
            1 => (
                usize::try_from(u64::from_be_bytes(self.data.get(8..16)?.try_into().ok()?)).ok()?,
                16,
            ),
            n => (n as usize, 8),
        };
        if size < header {
            return None;
        }
        let payload = self.data.get(header..size)?;
        self.data = self.data.get(size..)?;
        Some(Some((tag, payload)))
    }
}
fn child<'a>(data: &'a [u8], tag: &[u8; 4]) -> Option<&'a [u8]> {
    let mut boxes = Boxes::new(data);
    while let Some((kind, payload)) = boxes.next()? {
        if &kind == tag {
            return Some(payload);
        }
    }
    None
}
struct Config {
    fourcc: [u8; 4],
    size: (u32, u32),
    vp9_color: Option<Color>,
    av1_prefix: Option<Vec<u8>>,
}
fn video_config(source: &[u8]) -> Option<Config> {
    let moov = child(source, b"moov")?;
    let mut boxes = Boxes::new(moov);
    let mut config = None;
    while let Some((tag, trak)) = boxes.next()? {
        if &tag != b"trak" {
            continue;
        }
        let mdia = child(trak, b"mdia")?;
        if child(mdia, b"hdlr")?.get(8..12)? != b"vide" {
            continue;
        }
        // Multiple video tracks are outside this decoder's selection contract.
        if config.is_some() {
            return None;
        }
        let minf = child(mdia, b"minf")?;
        let stbl = child(minf, b"stbl")?;
        let stsd = child(stbl, b"stsd")?;
        if u32::from_be_bytes(stsd.get(4..8)?.try_into().ok()?) != 1 {
            return None;
        }
        let mut entries = Boxes::new(stsd.get(8..)?);
        let (fourcc, entry) = entries.next()??;
        let size = (
            u16::from_be_bytes(entry.get(24..26)?.try_into().ok()?) as u32,
            u16::from_be_bytes(entry.get(26..28)?.try_into().ok()?) as u32,
        );
        let children = entry.get(78..)?;
        let vp9_color = child(children, b"vpcC").and_then(|v| {
            // vpcC version 1: full-box prefix, profile, level, depth/chroma/range,
            // then ISO colour primaries, transfer and matrix identifiers.
            if v.first() != Some(&1) || *v.get(4)? > 3 || !matches!(*v.get(6)? >> 4, 8 | 10 | 12) {
                return None;
            }
            Some(Color {
                matrix: *v.get(9)?,
                full_range: v[6] & 1 != 0,
            })
        });
        let av1_prefix = child(children, b"av1C").and_then(|v| {
            if v.first() != Some(&0x81) || v.len() < 4 || v.len() > 1024 * 1024 {
                return None;
            }
            Some(v[4..].to_vec())
        });
        config = Some(Config {
            fourcc,
            size,
            vp9_color,
            av1_prefix,
        });
    }
    config
}
