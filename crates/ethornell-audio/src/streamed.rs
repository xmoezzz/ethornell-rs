//! A long sound, or a BGM pair (intro, then a loop), decoded while it plays.
//! Decoded whole, a 2.5-minute track is ~50 MB of frames, more than a PS
//! Vita can spare; kira's streaming thread keeps only a few packets ahead.
use kira::dsp::Frame;
use kira::sound::FromFileError;
use kira::sound::streaming::Decoder;
use std::io::Cursor;
use symphonia::core::audio::{AudioBuffer, AudioBufferRef, Signal};
use symphonia::core::codecs::Decoder as CodecDecoder;
use symphonia::core::conv::{FromSample, IntoSample};
use symphonia::core::formats::{FormatReader, SeekMode, SeekTo};
use symphonia::core::io::MediaSourceStream;
use symphonia::core::probe::Hint;
use symphonia::core::sample::Sample;

/// One Ogg stream, clamped to `frames` (the BURIKO box's sample count, as
/// the static path does).
struct Part {
    format: Box<dyn FormatReader>,
    decoder: Box<dyn CodecDecoder>,
    track_id: u32,
    sample_rate: u32,
    frames: usize,
    /// The frame after the last one decoded.
    position: usize,
    /// After a seek: the frame to discard up to.
    seeking: Option<usize>,
    /// Frames decoded by a seek, not yet returned.
    pending: Vec<Frame>,
}

impl Part {
    fn new(payload: Vec<u8>, declared: Option<usize>) -> Result<Self, FromFileError> {
        let mss = MediaSourceStream::new(Box::new(Cursor::new(payload)), Default::default());
        let format = symphonia::default::get_probe()
            .format(&Hint::default(), mss, &Default::default(), &Default::default())?
            .format;
        let track = format.default_track().ok_or(FromFileError::NoDefaultTrack)?;
        let sample_rate = track.codec_params.sample_rate.ok_or(FromFileError::UnknownSampleRate)?;
        let stream_frames = track.codec_params.n_frames.map(|n| n as usize);
        let frames = match (declared, stream_frames) {
            (Some(declared), Some(stream)) => declared.min(stream),
            (Some(declared), None) => declared,
            (None, Some(stream)) => stream,
            (None, None) => return Err(FromFileError::UnknownSampleRate),
        };
        let decoder = symphonia::default::get_codecs().make(&track.codec_params, &Default::default())?;
        let track_id = track.id;
        Ok(Self {
            format,
            decoder,
            track_id,
            sample_rate,
            frames,
            position: 0,
            seeking: None,
            pending: Vec::new(),
        })
    }

    /// The next frames, none only at the end. A Vorbis stream's first packet
    /// (and the first after a seek) only primes the decoder and yields none.
    fn decode(&mut self) -> Result<Vec<Frame>, FromFileError> {
        if !self.pending.is_empty() {
            return Ok(std::mem::take(&mut self.pending));
        }
        while self.position < self.frames {
            let packet = match self.format.next_packet() {
                Ok(packet) => packet,
                Err(symphonia::core::errors::Error::IoError(error))
                    if error.kind() == std::io::ErrorKind::UnexpectedEof =>
                {
                    self.position = self.frames;
                    break;
                }
                Err(error) => return Err(error.into()),
            };
            let mut frames = frames_of(&self.decoder.decode(&packet)?)?;
            if let Some(target) = self.seeking {
                // A packet's frames start at its timestamp.
                let start = packet.ts() as usize;
                frames.drain(..target.saturating_sub(start).min(frames.len()));
                if frames.is_empty() {
                    continue;
                }
                self.position = start.max(target);
                self.seeking = None;
            }
            frames.truncate(self.frames.saturating_sub(self.position));
            self.position += frames.len();
            if !frames.is_empty() {
                return Ok(frames);
            }
        }
        Ok(Vec::new())
    }

    fn seek(&mut self, index: usize) -> Result<usize, FromFileError> {
        // The first packet after a seek only primes the decoder: start a
        // Vorbis block (at most 8192 frames) early, then decode up to `index`.
        const PREROLL: usize = 8192;
        let seeked = self.format.seek(
            SeekMode::Accurate,
            SeekTo::TimeStamp {
                ts: index.saturating_sub(PREROLL) as u64,
                track_id: self.track_id,
            },
        )?;
        self.decoder.reset();
        self.position = seeked.actual_ts as usize;
        self.seeking = Some(index);
        self.pending.clear();
        self.pending = self.decode()?;
        Ok(self.position - self.pending.len())
    }
}

fn frames_of(buffer: &AudioBufferRef) -> Result<Vec<Frame>, FromFileError> {
    fn convert<S: Sample>(buffer: &AudioBuffer<S>) -> Result<Vec<Frame>, FromFileError>
    where
        f32: FromSample<S>,
    {
        match buffer.spec().channels.count() {
            1 => Ok(buffer.chan(0).iter().map(|s| Frame::from_mono((*s).into_sample())).collect()),
            2 => Ok(buffer
                .chan(0)
                .iter()
                .zip(buffer.chan(1))
                .map(|(l, r)| Frame::new((*l).into_sample(), (*r).into_sample()))
                .collect()),
            _ => Err(FromFileError::UnsupportedChannelConfiguration),
        }
    }
    match buffer {
        AudioBufferRef::U8(b) => convert(b),
        AudioBufferRef::U16(b) => convert(b),
        AudioBufferRef::U24(b) => convert(b),
        AudioBufferRef::U32(b) => convert(b),
        AudioBufferRef::S8(b) => convert(b),
        AudioBufferRef::S16(b) => convert(b),
        AudioBufferRef::S24(b) => convert(b),
        AudioBufferRef::S32(b) => convert(b),
        AudioBufferRef::F32(b) => convert(b),
        AudioBufferRef::F64(b) => convert(b),
    }
}

/// Frames 0..intro.frames come from the intro, if any, the rest from the
/// body; kira's loop region seeks back into either.
pub(crate) struct StreamedDecoder {
    intro: Option<Part>,
    body: Part,
    in_body: bool,
    gain: f32,
}

impl StreamedDecoder {
    pub(crate) fn new(
        intro: Option<(Vec<u8>, Option<usize>)>,
        body: (Vec<u8>, Option<usize>),
        gain: f32,
    ) -> Result<Self, FromFileError> {
        let intro = intro.map(|(bytes, frames)| Part::new(bytes, frames)).transpose()?;
        let body = Part::new(body.0, body.1)?;
        let in_body = intro.is_none();
        Ok(Self { intro, body, in_body, gain })
    }

    /// The intro's and the body's sample rates.
    pub(crate) fn sample_rates(&self) -> (u32, u32) {
        let body = self.body.sample_rate;
        (self.intro.as_ref().map_or(body, |intro| intro.sample_rate), body)
    }

    /// Where the body starts.
    pub(crate) fn body_start(&self) -> usize {
        self.intro.as_ref().map_or(0, |intro| intro.frames)
    }
}

impl Decoder for StreamedDecoder {
    type Error = FromFileError;

    fn sample_rate(&self) -> u32 {
        self.sample_rates().0
    }

    fn num_frames(&self) -> usize {
        self.body_start() + self.body.frames
    }

    fn decode(&mut self) -> Result<Vec<Frame>, Self::Error> {
        let mut frames = match self.intro.as_mut().filter(|_| !self.in_body) {
            Some(intro) => intro.decode()?,
            None => Vec::new(),
        };
        if frames.is_empty() {
            if !self.in_body {
                self.in_body = true;
                self.body.seek(0)?;
            }
            frames = self.body.decode()?;
        }
        if (self.gain - 1.0).abs() > f32::EPSILON {
            for frame in &mut frames {
                *frame = Frame::new(
                    (frame.left * self.gain).clamp(-1.0, 1.0),
                    (frame.right * self.gain).clamp(-1.0, 1.0),
                );
            }
        }
        Ok(frames)
    }

    fn seek(&mut self, index: usize) -> Result<usize, Self::Error> {
        let body_start = self.body_start();
        match self.intro.as_mut() {
            Some(intro) if index < body_start => {
                self.in_body = false;
                intro.seek(index)
            }
            _ => {
                self.in_body = true;
                Ok(body_start + self.body.seek(index - body_start)?)
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn part(bytes: &[u8]) -> (Vec<u8>, Option<usize>) {
        (
            crate::unwrap_buriko_wave_ogg(bytes).unwrap_or(bytes).to_vec(),
            crate::declared_frame_count(bytes),
        )
    }

    fn decode_all(d: &mut StreamedDecoder) -> Vec<Frame> {
        let mut out = Vec::new();
        loop {
            let frames = d.decode().unwrap();
            if frames.is_empty() {
                return out;
            }
            out.extend(frames);
        }
    }

    fn assert_same(got: &[Frame], expected: &[Frame]) {
        assert_eq!(got.len(), expected.len());
        let max = got
            .iter()
            .zip(expected)
            .map(|(a, b)| (a.left - b.left).abs().max((a.right - b.right).abs()))
            .fold(0.0f32, f32::max);
        assert!(max < 1e-6, "max diff {max}");
    }

    #[test]
    #[ignore = "needs ETHORNELL_BGM_DIR with bgm_4040_head / bgm_4040_loop"]
    fn streamed_matches_static_decode() {
        let dir = std::path::PathBuf::from(std::env::var("ETHORNELL_BGM_DIR").unwrap());
        let head = std::fs::read(dir.join("bgm_4040_head")).unwrap();
        let tail = std::fs::read(dir.join("bgm_4040_loop")).unwrap();
        let s = |b: &[u8]| crate::decode_static_sound(b, Default::default()).unwrap();
        let (sh, sl) = (s(&head), s(&tail));

        // A pair: the intro, then the loop part; the loop seam seeks back
        // to the loop part's first frame, as kira does.
        let mut d = StreamedDecoder::new(Some(part(&head)), part(&tail), 1.0).unwrap();
        assert_eq!(d.body_start(), sh.frames.len());
        let expected: Vec<Frame> = sh.frames.iter().chain(sl.frames.iter()).copied().collect();
        assert_same(&decode_all(&mut d), &expected);
        assert_eq!(d.seek(d.body_start()).unwrap(), d.body_start());
        let again = d.decode().unwrap();
        assert_same(&again, &sl.frames[..again.len()]);

        // One file, and a seek into its middle.
        let mut d = StreamedDecoder::new(None, part(&tail), 1.0).unwrap();
        assert_same(&decode_all(&mut d), &sl.frames);
        for at in [1_000_000, 5, 4096] {
            assert_eq!(d.seek(at).unwrap(), at);
            let frames = d.decode().unwrap();
            assert_same(&frames, &sl.frames[at..at + frames.len()]);
        }
    }
}
