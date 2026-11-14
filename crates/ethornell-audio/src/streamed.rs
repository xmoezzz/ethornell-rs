//! A long sound, or a BGM pair (intro, then a loop), decoded while it plays.
//! Decoded whole, a 2.5-minute track is ~50 MB of frames, more than a PS
//! Vita can spare; kira's streaming thread keeps only a few packets ahead.
use crate::resample::{LinearResampler, output_index, output_len};
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
    output_rate: u32,
    frames: usize,
    output_frames: usize,
    /// The source frame after the last one decoded.
    position: usize,
    output_position: usize,
    /// After a seek: the source frame to discard up to.
    seeking: Option<usize>,
    /// Frames decoded by a seek, not yet returned.
    pending: Vec<Frame>,
    resampler: Option<LinearResampler>,
    resampler_flushed: bool,
}

impl Part {
    fn new(
        payload: Vec<u8>,
        declared: Option<usize>,
        output_rate: Option<u32>,
    ) -> Result<Self, FromFileError> {
        let mss = MediaSourceStream::new(Box::new(Cursor::new(payload)), Default::default());
        let format = symphonia::default::get_probe()
            .format(
                &Hint::default(),
                mss,
                &Default::default(),
                &Default::default(),
            )?
            .format;
        let track = format
            .default_track()
            .ok_or(FromFileError::NoDefaultTrack)?;
        let sample_rate = track
            .codec_params
            .sample_rate
            .ok_or(FromFileError::UnknownSampleRate)?;
        if sample_rate == 0 {
            return Err(FromFileError::UnknownSampleRate);
        }
        let stream_frames = track.codec_params.n_frames.map(|n| n as usize);
        let frames = match (declared, stream_frames) {
            (Some(declared), Some(stream)) => declared.min(stream),
            (Some(declared), None) => declared,
            (None, Some(stream)) => stream,
            (None, None) => return Err(FromFileError::UnknownSampleRate),
        };
        let decoder =
            symphonia::default::get_codecs().make(&track.codec_params, &Default::default())?;
        let track_id = track.id;
        let output_rate = output_rate.unwrap_or(sample_rate);
        if output_rate == 0 {
            return Err(FromFileError::UnknownSampleRate);
        }
        let output_frames = output_len(frames, sample_rate, output_rate);
        let resampler =
            (sample_rate != output_rate).then(|| LinearResampler::new(sample_rate, output_rate));
        Ok(Self {
            format,
            decoder,
            track_id,
            sample_rate,
            output_rate,
            frames,
            output_frames,
            position: 0,
            output_position: 0,
            seeking: None,
            pending: Vec::new(),
            resampler,
            resampler_flushed: false,
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
            let mut input = frames_of(&self.decoder.decode(&packet)?)?;
            if let Some(target) = self.seeking {
                let start = packet.ts() as usize;
                input.drain(..target.saturating_sub(start).min(input.len()));
                if input.is_empty() {
                    continue;
                }
                self.position = start.max(target);
                self.seeking = None;
            }
            input.truncate(self.frames.saturating_sub(self.position));
            self.position += input.len();
            if input.is_empty() {
                continue;
            }
            let mut output = match self.resampler.as_mut() {
                Some(resampler) => resampler.push(&input),
                None => input,
            };
            output.truncate(self.output_frames.saturating_sub(self.output_position));
            self.output_position += output.len();
            if !output.is_empty() {
                return Ok(output);
            }
        }
        if !self.resampler_flushed {
            self.resampler_flushed = true;
            if let Some(resampler) = self.resampler.as_mut() {
                let output =
                    resampler.finish(self.output_frames.saturating_sub(self.output_position));
                self.output_position += output.len();
                return Ok(output);
            }
        }
        Ok(Vec::new())
    }

    fn seek(&mut self, index: usize) -> Result<usize, FromFileError> {
        const PREROLL: usize = 8192;
        let index = index.min(self.output_frames);
        if index == self.output_frames {
            self.position = self.frames;
            self.output_position = index;
            self.seeking = None;
            self.pending.clear();
            self.resampler_flushed = true;
            if let Some(resampler) = self.resampler.as_mut() {
                resampler.reset();
            }
            return Ok(index);
        }
        let source_position = index as u128 * self.sample_rate as u128;
        let target = (source_position / self.output_rate as u128).min(self.frames as u128) as usize;
        let phase = (source_position % self.output_rate as u128) as f64 / self.output_rate as f64;
        let seeked = self.format.seek(
            SeekMode::Accurate,
            SeekTo::TimeStamp {
                ts: target.saturating_sub(PREROLL) as u64,
                track_id: self.track_id,
            },
        )?;
        self.decoder.reset();
        self.position = seeked.actual_ts as usize;
        self.output_position = index;
        self.seeking = Some(target);
        self.pending.clear();
        self.resampler_flushed = false;
        if let Some(resampler) = self.resampler.as_mut() {
            resampler.reset_at(phase);
        }
        self.pending = self.decode()?;
        Ok(self.output_position.saturating_sub(self.pending.len()))
    }
}

fn frames_of(buffer: &AudioBufferRef) -> Result<Vec<Frame>, FromFileError> {
    fn convert<S: Sample>(buffer: &AudioBuffer<S>) -> Result<Vec<Frame>, FromFileError>
    where
        f32: FromSample<S>,
    {
        match buffer.spec().channels.count() {
            1 => Ok(buffer
                .chan(0)
                .iter()
                .map(|s| Frame::from_mono((*s).into_sample()))
                .collect()),
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

/// Frames 0..intro.output_frames come from the intro, if any, the rest from
/// the body; kira's loop region seeks back into either.
pub(crate) struct StreamedDecoder {
    intro: Option<Part>,
    body: Part,
    in_body: bool,
    gain: f32,
    sample_rate: u32,
}

impl StreamedDecoder {
    pub(crate) fn new(
        intro: Option<(Vec<u8>, Option<usize>)>,
        body: (Vec<u8>, Option<usize>),
        gain: f32,
    ) -> Result<Self, FromFileError> {
        let intro = intro
            .map(|(bytes, frames)| Part::new(bytes, frames, None))
            .transpose()?;
        let output_rate = intro.as_ref().map(|intro| intro.sample_rate);
        let body = Part::new(body.0, body.1, output_rate)?;
        let sample_rate = output_rate.unwrap_or(body.sample_rate);
        let in_body = intro.is_none();
        Ok(Self {
            intro,
            body,
            in_body,
            gain,
            sample_rate,
        })
    }

    /// Where the body starts in the output-rate timeline.
    pub(crate) fn body_start(&self) -> usize {
        self.intro.as_ref().map_or(0, |intro| intro.output_frames)
    }

    /// Convert a body source-frame position to the shared output timeline.
    pub(crate) fn body_output_index(&self, source_index: usize) -> usize {
        output_index(source_index, self.body.sample_rate, self.sample_rate)
    }
}

impl Decoder for StreamedDecoder {
    type Error = FromFileError;

    fn sample_rate(&self) -> u32 {
        self.sample_rate
    }

    fn num_frames(&self) -> usize {
        self.body_start() + self.body.output_frames
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
            crate::unwrap_buriko_wave_ogg(bytes)
                .unwrap_or(bytes)
                .to_vec(),
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

    fn pcm_wave(sample_rate: u32, frames: usize) -> Vec<u8> {
        let channels = 2u16;
        let bytes_per_sample = 2u16;
        let payload_len = frames * usize::from(channels) * usize::from(bytes_per_sample);
        let mut bytes = Vec::with_capacity(44 + payload_len);
        bytes.extend_from_slice(b"RIFF");
        bytes.extend_from_slice(&(36 + payload_len as u32).to_le_bytes());
        bytes.extend_from_slice(b"WAVEfmt ");
        bytes.extend_from_slice(&16u32.to_le_bytes());
        bytes.extend_from_slice(&1u16.to_le_bytes());
        bytes.extend_from_slice(&channels.to_le_bytes());
        bytes.extend_from_slice(&sample_rate.to_le_bytes());
        bytes.extend_from_slice(
            &(sample_rate * u32::from(channels) * u32::from(bytes_per_sample)).to_le_bytes(),
        );
        bytes.extend_from_slice(&(channels * bytes_per_sample).to_le_bytes());
        bytes.extend_from_slice(&(bytes_per_sample * 8).to_le_bytes());
        bytes.extend_from_slice(b"data");
        bytes.extend_from_slice(&(payload_len as u32).to_le_bytes());
        for frame in 0..frames {
            let sample = (frame as i16).to_le_bytes();
            bytes.extend_from_slice(&sample);
            bytes.extend_from_slice(&sample);
        }
        bytes
    }

    #[test]
    fn mixed_rate_parts_are_resampled_into_one_loop_timeline() {
        let intro_frames = 44_100 / 10;
        let body_frames = 48_000 / 10;
        let mut decoder = StreamedDecoder::new(
            Some((pcm_wave(44_100, intro_frames), None)),
            (pcm_wave(48_000, body_frames), None),
            1.0,
        )
        .unwrap();
        assert_eq!(decoder.sample_rate(), 44_100);
        assert_eq!(decoder.body_start(), intro_frames);
        assert_eq!(decoder.num_frames(), intro_frames + 44_100 / 10);
        let frames = decode_all(&mut decoder);
        assert_eq!(frames.len(), decoder.num_frames());
        assert!(frames.iter().any(|frame| frame.left != 0.0));
        assert_eq!(
            decoder.seek(decoder.body_start()).unwrap(),
            decoder.body_start()
        );
        assert!(!decoder.decode().unwrap().is_empty());
    }

    #[test]
    fn mixed_rate_seeks_match_the_continuous_timeline() {
        for (intro_rate, body_rate) in [(44_100, 48_000), (48_000, 44_100)] {
            let intro = pcm_wave(intro_rate, 317);
            let body = pcm_wave(body_rate, 20_000);
            let mut decoder = StreamedDecoder::new(Some((intro, None)), (body, None), 1.0).unwrap();
            let expected = decode_all(&mut decoder);
            for at in [0, 300, 317, 318, 8_193, 14_999, expected.len() - 10] {
                assert_eq!(decoder.seek(at).unwrap(), at);
                let got = decoder.decode().unwrap();
                let count = got.len().min(expected.len() - at);
                assert!(count > 0);
                assert_same(&got[..count], &expected[at..at + count]);
            }
        }
    }

    #[test]
    #[ignore = "requires FD bgm_4160/4170 head/loop in ETHORNELL_FD_BGM"]
    fn mixed_rate_owner_fd_pairs_decode_and_seek_across_the_join() {
        let dir = std::path::PathBuf::from(std::env::var_os("ETHORNELL_FD_BGM").unwrap());
        for code in ["4160", "4170"] {
            let head = std::fs::read(dir.join(format!("bgm_{code}_head"))).unwrap();
            let body = std::fs::read(dir.join(format!("bgm_{code}_loop"))).unwrap();
            let mut decoder = StreamedDecoder::new(Some(part(&head)), part(&body), 1.0).unwrap();
            assert_eq!(decoder.intro.as_ref().unwrap().sample_rate, 44_100);
            assert_eq!(decoder.body.sample_rate, 48_000);
            assert_eq!(decoder.sample_rate(), 44_100);
            for at in [
                decoder.body_start() - 16,
                decoder.body_start(),
                decoder.body_start() + 10_001,
                decoder.num_frames() - 64,
            ] {
                assert_eq!(decoder.seek(at).unwrap(), at);
                let frames = decoder.decode().unwrap();
                assert!(!frames.is_empty());
                assert!(
                    frames
                        .iter()
                        .all(|f| f.left.is_finite() && f.right.is_finite())
                );
            }
        }
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
