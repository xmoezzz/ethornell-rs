pub mod clock;

use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, EthornellError>;

#[derive(Debug, thiserror::Error)]
pub enum EthornellError {
    #[error("io error: {0}")]
    Io(#[from] std::io::Error),
    #[error("invalid game root: {0}")]
    InvalidGameRoot(PathBuf),
    #[error("unsupported format: {0}")]
    UnsupportedFormat(String),
    #[error("parse error: {0}")]
    Parse(String),
    #[error("{0}")]
    Other(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct GameRoot {
    path: PathBuf,
}

impl GameRoot {
    pub fn new(path: impl Into<PathBuf>) -> Result<Self> {
        let path = path.into();
        if !path.is_dir() {
            return Err(EthornellError::InvalidGameRoot(path));
        }
        Ok(Self { path })
    }

    pub fn path(&self) -> &Path {
        &self.path
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct ResourcePath(String);

impl ResourcePath {
    pub fn new(path: impl Into<String>) -> Self {
        Self(path.into().replace('\\', "/"))
    }

    pub fn as_str(&self) -> &str {
        &self.0
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Encoding {
    Cp932,
    Utf8,
    Unknown,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Version {
    pub engine: Option<String>,
    pub compatibility: Option<String>,
}

/// Warnings and errors only by default; the per-frame engine trace is
/// `RUST_LOG=ethornell=info` (or `debug`).
pub fn init_tracing() {
    let _ = tracing_subscriber::fmt()
        .with_env_filter(
            tracing_subscriber::EnvFilter::try_from_default_env()
                .unwrap_or_else(|_| "ethornell=warn,wgpu=warn".into()),
        )
        .try_init();
}

/// Portable classification of the native `CDspObj` blend selector.
///
/// The raw selector remains authoritative.  This enum only records the pixel
/// path currently justified by target evidence.  Unsupported selectors stay
/// visible instead of being renamed to an invented blend operation.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum NativeBlendPath {
    /// Native default selector `128`.  An opaque source may use replacement;
    /// alpha-bearing sources still require source-over composition.
    DefaultCopy,
    /// Selector `2`, currently supported by both CPU and GPU compositors as
    /// additive color accumulation.
    Additive,
    /// Selector `3`, currently supported by both CPU and GPU compositors as
    /// subtractive color accumulation.
    Subtractive,
    /// Selector `0xF0`. Target `sub_40B4B0/sub_40BC60` linearly interpolates
    /// every source/destination channel by one shared 1/256 transparency.
    ConstantInterpolation,
    /// Target pixel formula has not yet been recovered.  The compatibility
    /// compositor uses source-over while retaining the original selector.
    Unrecovered(i32),
}

pub fn classify_native_blend(selector: i32) -> NativeBlendPath {
    match selector {
        128 => NativeBlendPath::DefaultCopy,
        2 => NativeBlendPath::Additive,
        3 => NativeBlendPath::Subtractive,
        0xf0 => NativeBlendPath::ConstantInterpolation,
        value => NativeBlendPath::Unrecovered(value),
    }
}

/// CPU reference compositor for the recovered native blend selectors.
///
/// The formulas match the current wgpu pipeline state. Unknown selectors use
/// source-over but retain their raw selector through [`NativeBlendPath`]; they
/// are not renamed to a native operation that has not been recovered.
#[inline]
pub fn composite_native_rgba(
    destination: &mut [u8; 4],
    source: [u8; 4],
    opacity: f32,
    selector: i32,
) {
    let opacity = opacity.clamp(0.0, 1.0);
    if matches!(
        classify_native_blend(selector),
        NativeBlendPath::ConstantInterpolation
    ) {
        let transparency = ((1.0 - opacity) * 256.0).round().clamp(0.0, 256.0) as i32;
        for channel in 0..4 {
            let source_channel = i32::from(source[channel]);
            let delta = i32::from(destination[channel]) - source_channel;
            destination[channel] =
                (source_channel + ((delta * transparency) >> 8)).clamp(0, 255) as u8;
        }
        return;
    }
    if opacity == 1.0 && source[3] == 255
        && !matches!(classify_native_blend(selector), NativeBlendPath::Additive | NativeBlendPath::Subtractive)
    {
        *destination = source;
        return;
    }
    let source_alpha = (source[3] as f32 / 255.0) * opacity;
    if source_alpha <= 0.0 {
        return;
    }
    match classify_native_blend(selector) {
        NativeBlendPath::Additive => {
            for channel in 0..3 {
                destination[channel] = (destination[channel] as f32
                    + source[channel] as f32 * source_alpha)
                    .round()
                    .clamp(0.0, 255.0) as u8;
            }
        }
        NativeBlendPath::Subtractive => {
            for channel in 0..3 {
                destination[channel] = (destination[channel] as f32
                    - source[channel] as f32 * source_alpha)
                    .round()
                    .clamp(0.0, 255.0) as u8;
            }
        }
        NativeBlendPath::DefaultCopy | NativeBlendPath::Unrecovered(_) => {
            let inverse_alpha = 1.0 - source_alpha;
            for channel in 0..3 {
                destination[channel] = (source[channel] as f32 * source_alpha
                    + destination[channel] as f32 * inverse_alpha)
                    .round()
                    .clamp(0.0, 255.0) as u8;
            }
        }
        NativeBlendPath::ConstantInterpolation => unreachable!("handled above"),
    }
    let destination_alpha = destination[3] as f32 / 255.0;
    destination[3] = ((source_alpha + destination_alpha * (1.0 - source_alpha)) * 255.0)
        .round()
        .clamp(0.0, 255.0) as u8;
}

#[cfg(test)]
mod native_blend_tests {
    use super::{NativeBlendPath, classify_native_blend, composite_native_rgba};

    #[test]
    fn recovered_blend_selectors_keep_one_shared_cpu_gpu_classification() {
        assert_eq!(classify_native_blend(128), NativeBlendPath::DefaultCopy);
        assert_eq!(classify_native_blend(2), NativeBlendPath::Additive);
        assert_eq!(classify_native_blend(3), NativeBlendPath::Subtractive);
        assert_eq!(
            classify_native_blend(0xf0),
            NativeBlendPath::ConstantInterpolation
        );
        assert_eq!(
            classify_native_blend(0x24),
            NativeBlendPath::Unrecovered(0x24)
        );
    }

    #[test]
    fn cpu_reference_matches_alpha_and_additive_pipeline_contracts() {
        let mut destination = [10, 20, 30, 64];
        composite_native_rgba(&mut destination, [100, 80, 60, 128], 1.0, 2);
        assert_eq!(destination[0], 60);
        assert_eq!(destination[1], 60);
        assert_eq!(destination[2], 60);
        assert!(destination[3] > 128 && destination[3] < 192);

        let mut transparent_copy = [0, 0, 255, 255];
        composite_native_rgba(&mut transparent_copy, [255, 0, 0, 128], 1.0, 128);
        assert_eq!(transparent_copy, [128, 0, 127, 255]);
    }

    #[test]
    fn constant_interpolation_uses_target_integer_channel_formula() {
        let mut destination = [200, 10, 80, 40];
        composite_native_rgba(&mut destination, [20, 110, 40, 240], 0.75, 0xf0);
        assert_eq!(destination, [65, 85, 50, 190]);
    }
}
