use kira::dsp::Frame;

pub(crate) struct LinearResampler {
    source_step: f64,
    position: f64,
    input: Vec<Frame>,
}

impl LinearResampler {
    pub(crate) fn new(source_rate: u32, target_rate: u32) -> Self {
        debug_assert_ne!(source_rate, 0);
        debug_assert_ne!(target_rate, 0);
        Self {
            source_step: source_rate as f64 / target_rate as f64,
            position: 0.0,
            input: Vec::new(),
        }
    }

    pub(crate) fn push(&mut self, frames: &[Frame]) -> Vec<Frame> {
        if frames.is_empty() {
            return Vec::new();
        }
        self.input.extend_from_slice(frames);
        let mut output = Vec::with_capacity(
            ((frames.len() as f64 / self.source_step).ceil() as usize).saturating_add(2),
        );
        while self.position + 1.0 < self.input.len() as f64 {
            let index = self.position.floor() as usize;
            let fraction = (self.position - index as f64) as f32;
            output.push(interpolate(
                self.input[index],
                self.input[index + 1],
                fraction,
            ));
            self.position += self.source_step;
        }
        let consumed = (self.position.floor() as usize).min(self.input.len().saturating_sub(1));
        if consumed != 0 {
            self.input.drain(..consumed);
            self.position -= consumed as f64;
        }
        output
    }

    pub(crate) fn finish(&mut self, remaining: usize) -> Vec<Frame> {
        let mut output = Vec::with_capacity(remaining);
        while output.len() < remaining && !self.input.is_empty() {
            let index = (self.position.floor() as usize).min(self.input.len() - 1);
            let next = (index + 1).min(self.input.len() - 1);
            let fraction = (self.position - index as f64).clamp(0.0, 1.0) as f32;
            output.push(interpolate(self.input[index], self.input[next], fraction));
            self.position += self.source_step;
        }
        self.reset();
        output
    }

    pub(crate) fn reset(&mut self) {
        self.reset_at(0.0);
    }

    pub(crate) fn reset_at(&mut self, fractional_source_position: f64) {
        self.position = fractional_source_position.clamp(0.0, 1.0);
        self.input.clear();
    }
}

pub(crate) fn output_len(input_frames: usize, source_rate: u32, target_rate: u32) -> usize {
    if input_frames == 0 || source_rate == 0 || target_rate == 0 {
        return 0;
    }
    if source_rate == target_rate {
        return input_frames;
    }
    ((input_frames as u128 * target_rate as u128 + source_rate as u128 - 1) / source_rate as u128)
        .min(usize::MAX as u128) as usize
}

pub(crate) fn output_index(input_index: usize, source_rate: u32, target_rate: u32) -> usize {
    if source_rate == 0 || target_rate == 0 || source_rate == target_rate {
        return input_index;
    }
    ((input_index as u128 * target_rate as u128 + source_rate as u128 / 2) / source_rate as u128)
        .min(usize::MAX as u128) as usize
}

#[cfg(any(target_arch = "wasm32", test))]
pub(crate) fn resample_all(frames: &[Frame], source_rate: u32, target_rate: u32) -> Vec<Frame> {
    if source_rate == target_rate || frames.is_empty() {
        return frames.to_vec();
    }
    let target_len = output_len(frames.len(), source_rate, target_rate);
    let mut resampler = LinearResampler::new(source_rate, target_rate);
    let mut output = resampler.push(frames);
    if output.len() < target_len {
        let remaining = target_len - output.len();
        output.extend(resampler.finish(remaining));
    }
    output.truncate(target_len);
    output
}

fn interpolate(left: Frame, right: Frame, fraction: f32) -> Frame {
    Frame::new(
        left.left + (right.left - left.left) * fraction,
        left.right + (right.right - left.right) * fraction,
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn resample_all_changes_rate_and_keeps_stereo_channels() {
        let input = [Frame::new(0.0, 1.0), Frame::new(1.0, -1.0)];
        let output = resample_all(&input, 2, 4);
        assert_eq!(output.len(), 4);
        assert_eq!((output[0].left, output[0].right), (0.0, 1.0));
        assert!((output[1].left - 0.5).abs() < 1e-6);
        assert!((output[1].right - 0.0).abs() < 1e-6);
        assert_eq!((output[2].left, output[2].right), (1.0, -1.0));
        assert_eq!((output[3].left, output[3].right), (1.0, -1.0));
    }

    #[test]
    fn chunked_resampling_matches_single_chunk_and_converts_loop_positions() {
        let input = (0..20)
            .map(|index| Frame::new(index as f32 / 20.0, -(index as f32) / 20.0))
            .collect::<Vec<_>>();
        let expected = resample_all(&input, 48_000, 44_100);
        let mut resampler = LinearResampler::new(48_000, 44_100);
        let mut output = Vec::new();
        output.extend(resampler.push(&input[..3]));
        output.extend(resampler.push(&input[3..11]));
        output.extend(resampler.push(&input[11..]));
        output.extend(resampler.finish(expected.len().saturating_sub(output.len())));
        assert_eq!(output.len(), expected.len());
        for (got, expected) in output.iter().zip(expected) {
            assert!((got.left - expected.left).abs() < 1e-6);
            assert!((got.right - expected.right).abs() < 1e-6);
        }
        assert_eq!(output_len(48_000, 48_000, 44_100), 44_100);
        assert_eq!(output_index(48_000, 48_000, 44_100), 44_100);
        assert_eq!(output_index(24_000, 48_000, 44_100), 22_050);
    }

    #[test]
    fn same_rate_keeps_frames_unchanged() {
        let input = [Frame::new(0.25, -0.5), Frame::new(0.75, -1.0)];
        let output = resample_all(&input, 48_000, 48_000);
        assert_eq!(output.len(), input.len());
        for (got, want) in output.iter().zip(input) {
            assert_eq!((got.left, got.right), (want.left, want.right));
        }
    }
}
