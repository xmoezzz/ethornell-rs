//! Shared draw/text API with desktop wgpu and native console GPU presenters.
mod font;
mod shared;
pub use font::FontProvider;
pub(crate) use shared::SYSTEM_CJK_FONT_CANDIDATES;
pub use shared::{
    PresentationScale, RenderCommand, TextGlow, TextStyleSpan, TextureHandle, load_system_cjk_font,
    load_text_font,
};
pub type TextureId = u64;
#[cfg(not(any(feature = "native-gpu", target_os = "vita", target_os = "horizon")))]
mod wgpu_renderer;
#[cfg(not(any(feature = "native-gpu", target_os = "vita", target_os = "horizon")))]
pub use wgpu_renderer::Renderer;
#[cfg(any(feature = "native-gpu", target_os = "vita", target_os = "horizon"))]
mod native_gpu;
#[cfg(any(feature = "native-gpu", target_os = "vita", target_os = "horizon"))]
pub use native_gpu::{NativeDraw, NativeGpu, NativeVertex, Renderer};
