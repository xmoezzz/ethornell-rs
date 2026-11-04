use ab_glyph::FontArc;
#[cfg(test)]
use ab_glyph::Font;
use ethornell_core::{EthornellError, NativeBlendPath, Result, classify_native_blend};
use ethornell_image::DecodedImage;
use std::collections::{BTreeMap, BTreeSet};
#[cfg(feature = "desktop-window")]
use winit::window::Window;

use crate::shared::*;
use crate::{FontProvider, TextureId};

fn validate_surface_size(width: u32, height: u32, limit: u32) -> Result<()> {
    if width == 0 || height == 0 || width > limit || height > limit {
        return Err(EthornellError::Other(format!(
            "invalid GPU dimensions {width}x{height} (limit {limit})"
        )));
    }
    Ok(())
}

fn renderer_limits(adapter: &wgpu::Limits) -> Result<wgpu::Limits> {
    let required = wgpu::Limits::downlevel_webgl2_defaults()
        .using_resolution(adapter.clone())
        .using_alignment(adapter.clone());
    let mut failures = Vec::new();
    required.check_limits_with_fail_fn(adapter, false, |name, requested, available| {
        failures.push(format!(
            "{name}: requested {requested}, available {available}"
        ));
    });
    if !failures.is_empty() {
        return Err(EthornellError::Other(format!(
            "adapter does not meet 2D renderer limits: {}",
            failures.join(", ")
        )));
    }
    Ok(required)
}

fn surface_configuration(
    caps: &wgpu::SurfaceCapabilities,
    width: u32,
    height: u32,
    limit: u32,
) -> Result<wgpu::SurfaceConfiguration> {
    validate_surface_size(width, height, limit)?;
    let format = (!caps.formats.is_empty())
        .then(|| gamma_space_surface_format(&caps.formats))
        .ok_or_else(|| EthornellError::Other("surface has no supported formats".into()))?;
    let present_mode = caps
        .present_modes
        .iter()
        .copied()
        .find(|mode| *mode == wgpu::PresentMode::Fifo)
        .or_else(|| caps.present_modes.first().copied())
        .ok_or_else(|| EthornellError::Other("surface has no supported present modes".into()))?;
    let alpha_mode = caps
        .alpha_modes
        .iter()
        .copied()
        .find(|mode| *mode == wgpu::CompositeAlphaMode::Opaque)
        .or_else(|| caps.alpha_modes.first().copied())
        .ok_or_else(|| EthornellError::Other("surface has no supported alpha modes".into()))?;
    if !caps.usages.contains(wgpu::TextureUsages::RENDER_ATTACHMENT) {
        return Err(EthornellError::Other(
            "surface does not support rendering".into(),
        ));
    }
    Ok(wgpu::SurfaceConfiguration {
        usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
        format,
        width,
        height,
        present_mode,
        alpha_mode,
        view_formats: vec![],
        desired_maximum_frame_latency: 2,
    })
}

struct TextureRecord {
    texture: wgpu::Texture,
    linear_bind_group: wgpu::BindGroup,
    nearest_bind_group: wgpu::BindGroup,
    width: u32,
    height: u32,
    opaque: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
struct TextCacheKey {
    text: String,
    size_bits: u32,
    line_height_bits: Option<u32>,
    color_bits: [u32; 4],
    styles: Vec<TextStyleSpan>,
    glow: Option<TextGlow>,
}

impl TextCacheKey {
    fn new(
        text: &str,
        size: f32,
        line_height: Option<f32>,
        color: [f32; 4],
        styles: &[TextStyleSpan],
        glow: Option<TextGlow>,
    ) -> Self {
        Self {
            text: text.to_string(),
            size_bits: size.to_bits(),
            line_height_bits: line_height.map(f32::to_bits),
            color_bits: color.map(f32::to_bits),
            styles: styles.to_vec(),
            glow,
        }
    }
}

#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
struct Vertex {
    position: [f32; 2],
    texcoord: [f32; 2],
    opacity: f32,
    ignore_source_alpha: f32,
}

impl Vertex {
    fn layout<'a>() -> wgpu::VertexBufferLayout<'a> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<Vertex>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Vertex,
            attributes: &[
                wgpu::VertexAttribute {
                    offset: 0,
                    shader_location: 0,
                    format: wgpu::VertexFormat::Float32x2,
                },
                wgpu::VertexAttribute {
                    offset: 8,
                    shader_location: 1,
                    format: wgpu::VertexFormat::Float32x2,
                },
                wgpu::VertexAttribute {
                    offset: 16,
                    shader_location: 2,
                    format: wgpu::VertexFormat::Float32,
                },
                wgpu::VertexAttribute {
                    offset: 20,
                    shader_location: 3,
                    format: wgpu::VertexFormat::Float32,
                },
            ],
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum NativePipelineKind {
    Alpha,
    Replace,
    Additive,
    Subtractive,
    ConstantInterpolation,
}

fn native_pipeline_kind(blend_mode: i32, opacity: f32, texture_opaque: bool) -> NativePipelineKind {
    match classify_native_blend(blend_mode) {
        // CDspObj's default copy selector may bypass destination blending only
        // when the source is provably opaque.  RGBA input remains source-over.
        NativeBlendPath::DefaultCopy if opacity >= 0.999 && texture_opaque => {
            NativePipelineKind::Replace
        }
        NativeBlendPath::Additive => NativePipelineKind::Additive,
        NativeBlendPath::Subtractive => NativePipelineKind::Subtractive,
        NativeBlendPath::ConstantInterpolation => NativePipelineKind::ConstantInterpolation,
        NativeBlendPath::DefaultCopy | NativeBlendPath::Unrecovered(_) => NativePipelineKind::Alpha,
    }
}

pub struct Renderer<'w> {
    /// The adapter's driver description (wgpu `AdapterInfo::name`).
    adapter_name: String,
    /// How the game frame fills the surface (Sys81:61's effective mode).
    presentation_scale: PresentationScale,
    surface: wgpu::Surface<'w>,
    device: wgpu::Device,
    queue: wgpu::Queue,
    config: wgpu::SurfaceConfiguration,
    clear_color: wgpu::Color,
    alpha_pipeline: wgpu::RenderPipeline,
    replace_pipeline: wgpu::RenderPipeline,
    additive_pipeline: wgpu::RenderPipeline,
    subtractive_pipeline: wgpu::RenderPipeline,
    constant_interpolation_pipeline: wgpu::RenderPipeline,
    bind_group_layout: wgpu::BindGroupLayout,
    linear_sampler: wgpu::Sampler,
    nearest_sampler: wgpu::Sampler,
    textures: BTreeMap<TextureId, TextureRecord>,
    text_cache: BTreeMap<TextCacheKey, TextureHandle>,
    next_texture_id: TextureId,
    commands: Vec<RenderCommand>,
    font: FontArc,
    virtual_width: f32,
    virtual_height: f32,
    surface_suspended: bool,
    gpu_fault: std::sync::Arc<std::sync::Mutex<Option<String>>>,
}

impl<'w> Renderer<'w> {
    /// The adapter's description, the host counterpart of
    /// D3DADAPTER_IDENTIFIER9::Description.
    pub fn adapter_name(&self) -> &str {
        &self.adapter_name
    }

    #[cfg(feature = "desktop-window")]
    pub async fn new(window: std::sync::Arc<dyn Window>) -> Result<Self> {
        Self::new_with_font(window, load_system_cjk_font()?).await
    }

    #[cfg(feature = "desktop-window")]
    pub async fn new_with_font(window: std::sync::Arc<dyn Window>, font: FontArc) -> Result<Self> {
        let size = window.surface_size();
        let instance = wgpu::Instance::default();
        let surface = instance
            .create_surface(window)
            .map_err(|err| EthornellError::Other(format!("create surface failed: {err}")))?;
        Self::from_surface(&instance, surface, size.width, size.height, font).await
    }

    /// Creates a renderer without a window/event-loop dependency. The caller
    /// owns any raw surface target and must keep it alive until this is dropped.
    pub async fn from_surface(
        instance: &wgpu::Instance,
        surface: wgpu::Surface<'w>,
        width: u32,
        height: u32,
        font: FontArc,
    ) -> Result<Self> {
        validate_surface_size(width, height, u32::MAX)?;
        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::HighPerformance,
                compatible_surface: Some(&surface),
                force_fallback_adapter: false,
            })
            .await
            .ok_or_else(|| EthornellError::Other("no compatible wgpu adapter".into()))?;
        let adapter_name = adapter.get_info().name;
        let adapter_limits = adapter.limits();
        let required_limits = renderer_limits(&adapter_limits)?;
        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    label: Some("ethornell-device"),
                    required_features: wgpu::Features::empty(),
                    required_limits,
                },
                None,
            )
            .await
            .map_err(|err| EthornellError::Other(format!("request device failed: {err}")))?;
        let gpu_fault = std::sync::Arc::new(std::sync::Mutex::new(None));
        let lost_fault = gpu_fault.clone();
        device.set_device_lost_callback(move |reason, message| {
            if let Ok(mut fault) = lost_fault.lock() {
                *fault = Some(format!("device lost ({reason:?}): {message}"));
            }
        });
        let validation_fault = gpu_fault.clone();
        device.on_uncaptured_error(Box::new(move |error| {
            if let Ok(mut fault) = validation_fault.lock() {
                *fault = Some(format!("GPU error: {error}"));
            }
        }));
        let caps = surface.get_capabilities(&adapter);
        let config = surface_configuration(
            &caps,
            width,
            height,
            device.limits().max_texture_dimension_2d,
        )?;
        let format = config.format;
        surface.configure(&device, &config);

        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            label: Some("ethornell-texture-layout"),
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        sample_type: wgpu::TextureSampleType::Float { filterable: true },
                        view_dimension: wgpu::TextureViewDimension::D2,
                        multisampled: false,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 1,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Filtering),
                    count: None,
                },
            ],
        });
        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some("ethornell-2d-shader"),
            source: wgpu::ShaderSource::Wgsl(SHADER.into()),
        });
        let pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
            label: Some("ethornell-2d-pipeline-layout"),
            bind_group_layouts: &[&bind_group_layout],
            push_constant_ranges: &[],
        });
        let create_pipeline = |label: &'static str, blend: Option<wgpu::BlendState>| {
            device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
                label: Some(label),
                layout: Some(&pipeline_layout),
                vertex: wgpu::VertexState {
                    module: &shader,
                    entry_point: "vs_main",
                    buffers: &[Vertex::layout()],
                },
                fragment: Some(wgpu::FragmentState {
                    module: &shader,
                    entry_point: "fs_main",
                    targets: &[Some(wgpu::ColorTargetState {
                        format,
                        blend,
                        write_mask: wgpu::ColorWrites::ALL,
                    })],
                }),
                primitive: wgpu::PrimitiveState::default(),
                depth_stencil: None,
                multisample: wgpu::MultisampleState::default(),
                multiview: None,
            })
        };
        let alpha_pipeline = create_pipeline(
            "ethornell-2d-alpha-pipeline",
            Some(wgpu::BlendState::ALPHA_BLENDING),
        );
        let replace_pipeline = create_pipeline("ethornell-2d-replace-pipeline", None);
        let additive_pipeline = create_pipeline(
            "ethornell-2d-additive-pipeline",
            Some(wgpu::BlendState {
                color: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::SrcAlpha,
                    dst_factor: wgpu::BlendFactor::One,
                    operation: wgpu::BlendOperation::Add,
                },
                alpha: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::One,
                    dst_factor: wgpu::BlendFactor::OneMinusSrcAlpha,
                    operation: wgpu::BlendOperation::Add,
                },
            }),
        );
        let subtractive_pipeline = create_pipeline(
            "ethornell-2d-subtractive-pipeline",
            Some(wgpu::BlendState {
                color: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::SrcAlpha,
                    dst_factor: wgpu::BlendFactor::One,
                    operation: wgpu::BlendOperation::ReverseSubtract,
                },
                alpha: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::One,
                    dst_factor: wgpu::BlendFactor::OneMinusSrcAlpha,
                    operation: wgpu::BlendOperation::Add,
                },
            }),
        );
        let constant_interpolation_pipeline = create_pipeline(
            "ethornell-2d-constant-interpolation-pipeline",
            Some(wgpu::BlendState {
                color: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::Constant,
                    dst_factor: wgpu::BlendFactor::OneMinusConstant,
                    operation: wgpu::BlendOperation::Add,
                },
                alpha: wgpu::BlendComponent {
                    src_factor: wgpu::BlendFactor::Constant,
                    dst_factor: wgpu::BlendFactor::OneMinusConstant,
                    operation: wgpu::BlendOperation::Add,
                },
            }),
        );
        let linear_sampler = device.create_sampler(&wgpu::SamplerDescriptor {
            label: Some("ethornell-linear-sampler"),
            mag_filter: wgpu::FilterMode::Linear,
            min_filter: wgpu::FilterMode::Linear,
            ..Default::default()
        });
        let nearest_sampler = device.create_sampler(&wgpu::SamplerDescriptor {
            label: Some("ethornell-nearest-sampler"),
            mag_filter: wgpu::FilterMode::Nearest,
            min_filter: wgpu::FilterMode::Nearest,
            ..Default::default()
        });

        let renderer = Self {
            adapter_name,
            presentation_scale: PresentationScale::Fit,
            surface,
            device,
            queue,
            config,
            clear_color: wgpu::Color {
                r: 0.0,
                g: 0.0,
                b: 0.0,
                a: 1.0,
            },
            alpha_pipeline,
            replace_pipeline,
            additive_pipeline,
            subtractive_pipeline,
            constant_interpolation_pipeline,
            bind_group_layout,
            linear_sampler,
            nearest_sampler,
            textures: BTreeMap::new(),
            text_cache: BTreeMap::new(),
            next_texture_id: 1,
            commands: Vec::new(),
            font,
            virtual_width: 1280.0,
            virtual_height: 720.0,
            surface_suspended: false,
            gpu_fault,
        };
        renderer.check_gpu()?;
        Ok(renderer)
    }

    /// Borrows an ANativeWindow; no activity glue or acquire/release is used.
    ///
    /// # Safety
    /// `window_ptr` must be a valid ANativeWindow, kept alive and usable for the
    /// entire renderer lifetime. Serialize all calls on the owning engine thread.
    /// Before surfaceDestroyed returns, drop the renderer, then release the JNI
    /// window reference. A replacement surface needs a fresh renderer and fresh
    /// texture handles (re-upload CPU images); old handles are not transferable.
    #[cfg(target_os = "android")]
    pub async unsafe fn new_android(
        window_ptr: *mut std::ffi::c_void,
        width: u32,
        height: u32,
        backend: wgpu::Backends,
        font: FontArc,
    ) -> Result<Self> {
        let window = std::ptr::NonNull::new(window_ptr)
            .ok_or_else(|| EthornellError::Other("null ANativeWindow".into()))?;
        validate_surface_size(width, height, u32::MAX)?;
        if backend.is_empty() {
            return Err(EthornellError::Other("no GPU backend selected".into()));
        }
        let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: backend,
            ..Default::default()
        });
        // SAFETY: the host guarantees the raw window lifetime as documented.
        let surface = unsafe {
            instance.create_surface_unsafe(wgpu::SurfaceTargetUnsafe::RawHandle {
                raw_display_handle: raw_window_handle::RawDisplayHandle::Android(
                    raw_window_handle::AndroidDisplayHandle::new(),
                ),
                raw_window_handle: raw_window_handle::RawWindowHandle::AndroidNdk(
                    raw_window_handle::AndroidNdkWindowHandle::new(window),
                ),
            })
        }
        .map_err(|err| EthornellError::Other(format!("create Android surface failed: {err}")))?;
        Self::from_surface(&instance, surface, width, height, font).await
    }

    /// A renderer on a canvas the page transferred to the engine's worker,
    /// drawn with WebGL2.
    #[cfg(target_arch = "wasm32")]
    pub async fn new_web(
        canvas: web_sys::OffscreenCanvas,
        width: u32,
        height: u32,
        font: FontArc,
    ) -> Result<Self> {
        validate_surface_size(width, height, u32::MAX)?;
        let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: wgpu::Backends::GL,
            ..Default::default()
        });
        let surface = instance
            .create_surface(wgpu::SurfaceTarget::OffscreenCanvas(canvas))
            .map_err(|err| EthornellError::Other(format!("create canvas surface failed: {err}")))?;
        Self::from_surface(&instance, surface, width, height, font).await
    }

    /// A renderer on a UIKit view. wgpu draws through the view's
    /// `CAMetalLayer` (adding one when its layer is not).
    ///
    /// # Safety
    /// `ui_view` must be a `UIView*` that outlives the renderer, used on the
    /// main thread only.
    #[cfg(target_os = "ios")]
    pub async unsafe fn new_ios(
        ui_view: *mut std::ffi::c_void,
        width: u32,
        height: u32,
        font: FontArc,
    ) -> Result<Self> {
        let view = std::ptr::NonNull::new(ui_view)
            .ok_or_else(|| EthornellError::Other("null UIView".into()))?;
        validate_surface_size(width, height, u32::MAX)?;
        let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: wgpu::Backends::METAL,
            ..Default::default()
        });
        // SAFETY: the host guarantees the view's lifetime as documented.
        let surface = unsafe {
            instance.create_surface_unsafe(wgpu::SurfaceTargetUnsafe::RawHandle {
                raw_display_handle: raw_window_handle::RawDisplayHandle::UiKit(
                    raw_window_handle::UiKitDisplayHandle::new(),
                ),
                raw_window_handle: raw_window_handle::RawWindowHandle::UiKit(
                    raw_window_handle::UiKitWindowHandle::new(view),
                ),
            })
        }
        .map_err(|err| EthornellError::Other(format!("create iOS surface failed: {err}")))?;
        Self::from_surface(&instance, surface, width, height, font).await
    }

    pub fn set_presentation_scale(&mut self, scale: PresentationScale) {
        self.presentation_scale = scale;
    }

    /// Game-to-surface scale (x, y) and offset (x, y).
    fn viewport(&self) -> (f32, f32, f32, f32) {
        presentation_viewport(
            self.presentation_scale,
            self.config.width as f32,
            self.config.height as f32,
            self.virtual_width,
            self.virtual_height,
        )
    }

    pub fn set_virtual_size(&mut self, width: f32, height: f32) {
        self.virtual_width = width.max(1.0);
        self.virtual_height = height.max(1.0);
    }

    pub fn surface_size(&self) -> (u32, u32) {
        (self.config.width, self.config.height)
    }

    /// Target renderer offset +0x58 is used as a maximum pixel-capacity field
    /// when large bitmaps are split into strips. The closest wgpu equivalent
    /// is the square of max_texture_dimension_2d.
    pub fn graphics_memory_metric(&self) -> i32 {
        let side = u64::from(self.device.limits().max_texture_dimension_2d);
        side.saturating_mul(side).min(i32::MAX as u64) as i32
    }

    pub fn game_to_surface_rect(
        &self,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
    ) -> (f32, f32, f32, f32) {
        let (scale_x, scale_y, offset_x, offset_y) = self.viewport();
        (
            offset_x + x * scale_x,
            offset_y + y * scale_y,
            width * scale_x,
            height * scale_y,
        )
    }

    pub fn surface_to_game_point(&self, x: f32, y: f32) -> Option<(f32, f32)> {
        let (scale_x, scale_y, offset_x, offset_y) = self.viewport();
        viewport_to_virtual_point(
            x,
            y,
            (scale_x, scale_y, offset_x, offset_y),
            self.virtual_width,
            self.virtual_height,
        )
    }

    pub fn insert_rgba(&mut self, image: &DecodedImage) -> Result<TextureHandle> {
        self.check_gpu()?;
        validate_rgba(image)?;
        self.validate_texture_size(image)?;
        let id = self.next_texture_id;
        self.next_texture_id = self
            .next_texture_id
            .checked_add(1)
            .ok_or_else(|| EthornellError::Other("texture ID space exhausted".into()))?;
        let record = self.create_texture_record(image);
        self.textures.insert(id, record);
        Ok(TextureHandle {
            id,
            width: image.width,
            height: image.height,
        })
    }

    pub fn update_rgba(
        &mut self,
        handle: &TextureHandle,
        image: &DecodedImage,
    ) -> Result<TextureHandle> {
        self.check_gpu()?;
        validate_rgba(image)?;
        self.validate_texture_size(image)?;
        let same_dimensions = self
            .textures
            .get(&handle.id)
            .is_some_and(|record| record.width == image.width && record.height == image.height);
        if same_dimensions {
            let record = self
                .textures
                .get(&handle.id)
                .ok_or_else(|| EthornellError::Parse("unknown texture handle".into()))?;
            self.write_texture(&record.texture, image);
            if let Some(record) = self.textures.get_mut(&handle.id) {
                record.opaque = image
                    .rgba
                    .as_chunks::<4>()
                    .0
                    .iter()
                    .all(|pixel| pixel[3] == 255);
            }
        } else {
            let record = self.create_texture_record(image);
            self.textures.insert(handle.id, record);
        }
        Ok(TextureHandle {
            id: handle.id,
            width: image.width,
            height: image.height,
        })
    }

    pub fn remove_texture(&mut self, texture: TextureId) {
        self.textures.remove(&texture);
        self.text_cache.retain(|_, handle| handle.id != texture);
    }

    fn validate_texture_size(&self, image: &DecodedImage) -> Result<()> {
        validate_surface_size(
            image.width,
            image.height,
            self.device.limits().max_texture_dimension_2d,
        )
    }

    /// A lost device/validation/OOM fault is terminal for this renderer. The
    /// host must report it and stop, or drop/rebuild and re-upload all textures.
    pub fn check_gpu(&self) -> Result<()> {
        self.device.poll(wgpu::Maintain::Poll);
        let fault = self
            .gpu_fault
            .lock()
            .map_err(|_| EthornellError::Other("GPU fault state poisoned".into()))?;
        if let Some(message) = fault.as_ref() {
            return Err(EthornellError::Other(message.clone()));
        }
        Ok(())
    }

    fn create_texture_record(&self, image: &DecodedImage) -> TextureRecord {
        let texture = self.device.create_texture(&wgpu::TextureDescriptor {
            label: Some("ethornell-rgba-texture"),
            size: wgpu::Extent3d {
                width: image.width,
                height: image.height,
                depth_or_array_layers: 1,
            },
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            // Stored values, not sRGB: see gamma_space_surface_format.
            format: wgpu::TextureFormat::Rgba8Unorm,
            usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST,
            view_formats: &[],
        });
        self.write_texture(&texture, image);
        let view = texture.create_view(&wgpu::TextureViewDescriptor::default());
        let make_bind_group = |label: &'static str, sampler: &wgpu::Sampler| {
            self.device.create_bind_group(&wgpu::BindGroupDescriptor {
                label: Some(label),
                layout: &self.bind_group_layout,
                entries: &[
                    wgpu::BindGroupEntry {
                        binding: 0,
                        resource: wgpu::BindingResource::TextureView(&view),
                    },
                    wgpu::BindGroupEntry {
                        binding: 1,
                        resource: wgpu::BindingResource::Sampler(sampler),
                    },
                ],
            })
        };
        let linear_bind_group =
            make_bind_group("ethornell-texture-linear-bind-group", &self.linear_sampler);
        let nearest_bind_group = make_bind_group(
            "ethornell-texture-nearest-bind-group",
            &self.nearest_sampler,
        );
        TextureRecord {
            texture,
            linear_bind_group,
            nearest_bind_group,
            width: image.width,
            height: image.height,
            opaque: image
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .all(|pixel| pixel[3] == 255),
        }
    }

    fn write_texture(&self, texture: &wgpu::Texture, image: &DecodedImage) {
        self.queue.write_texture(
            wgpu::ImageCopyTexture {
                texture,
                mip_level: 0,
                origin: wgpu::Origin3d::ZERO,
                aspect: wgpu::TextureAspect::All,
            },
            &image.rgba,
            wgpu::ImageDataLayout {
                offset: 0,
                bytes_per_row: Some(image.width * 4),
                rows_per_image: Some(image.height),
            },
            wgpu::Extent3d {
                width: image.width,
                height: image.height,
                depth_or_array_layers: 1,
            },
        );
    }

    pub fn resize(&mut self, width: u32, height: u32) {
        if let Err(err) = self.try_resize(width, height) {
            tracing::warn!(%err, "renderer resize rejected");
        }
    }

    pub fn try_resize(&mut self, width: u32, height: u32) -> Result<()> {
        self.check_gpu()?;
        if width == 0 || height == 0 {
            self.surface_suspended = true;
            return Ok(());
        }
        validate_surface_size(width, height, self.device.limits().max_texture_dimension_2d)?;
        self.config.width = width;
        self.config.height = height;
        self.surface.configure(&self.device, &self.config);
        self.surface_suspended = false;
        Ok(())
    }

    pub fn submit(&mut self, commands: &[RenderCommand]) {
        self.commands.clear();
        for command in commands {
            if let RenderCommand::Clear { color } = command {
                self.clear_color = wgpu::Color {
                    r: color[0] as f64,
                    g: color[1] as f64,
                    b: color[2] as f64,
                    a: color[3] as f64,
                };
            }
            self.commands.push(command.clone());
        }
    }

    pub fn render(&mut self) -> Result<()> {
        self.check_gpu()?;
        if self.surface_suspended {
            return Ok(());
        }
        let frame = match self.surface.get_current_texture() {
            Ok(frame) => frame,
            Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                self.surface.configure(&self.device, &self.config);
                return Ok(());
            }
            Err(wgpu::SurfaceError::Timeout) => return Ok(()),
            Err(err) => return Err(EthornellError::Other(format!("surface error: {err}"))),
        };
        let view = frame
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());
        let mut draw_items = Vec::new();
        let mut frame_vertices = Vec::<Vertex>::new();
        let mut active_text_keys = BTreeSet::new();
        let mut draw_commands: Vec<_> = self
            .commands
            .iter()
            .filter_map(|command| match command {
                RenderCommand::DrawTexture { z, .. } => Some((*z, command.clone())),
                RenderCommand::DrawText { z, .. } => Some((*z, command.clone())),
                RenderCommand::Clear { .. } => None,
            })
            .collect();
        draw_commands.sort_by_key(|(z, _)| *z);
        for (_, command) in draw_commands {
            let (
                texture,
                x,
                y,
                width,
                height,
                src_x,
                src_y,
                src_width,
                src_height,
                opacity,
                ignore_source_alpha,
                blend_mode,
                rotation_degrees,
                destination_quad,
                linear_sampling,
                clip,
            ) = match command {
                RenderCommand::DrawTexture {
                    texture,
                    x,
                    y,
                    width,
                    height,
                    src_x,
                    src_y,
                    src_width,
                    src_height,
                    opacity,
                    ignore_source_alpha,
                    blend_mode,
                    rotation_degrees,
                    destination_quad,
                    linear_sampling,
                    clip,
                    ..
                } => {
                    if !self.textures.contains_key(&texture) {
                        continue;
                    }
                    (
                        texture,
                        x,
                        y,
                        width,
                        height,
                        src_x,
                        src_y,
                        src_width,
                        src_height,
                        opacity,
                        ignore_source_alpha,
                        blend_mode,
                        rotation_degrees,
                        destination_quad,
                        linear_sampling,
                        clip,
                    )
                }
                RenderCommand::DrawText {
                    text,
                    x,
                    y,
                    color,
                    size,
                    line_height,
                    styles,
                    glow,
                    clip,
                    ..
                } => {
                    FontProvider::require_coverage(&self.font, &text)?;
                    let key = TextCacheKey::new(&text, size, line_height, color, &styles, glow);
                    active_text_keys.insert(key.clone());
                    let handle = if let Some(handle) = self.text_cache.get(&key).cloned() {
                        handle
                    } else {
                        let mut image =
                            rasterize_text(&self.font, &text, size, line_height, color, &styles);
                        if let Some(glow) = glow {
                            image = apply_text_glow(&image, glow);
                        }
                        let handle = self.insert_rgba(&image)?;
                        self.text_cache.insert(key, handle.clone());
                        handle
                    };
                    (
                        handle.id,
                        x,
                        y,
                        handle.width as f32,
                        handle.height as f32,
                        0.0,
                        0.0,
                        handle.width as f32,
                        handle.height as f32,
                        1.0,
                        false,
                        1,
                        0.0,
                        None,
                        true,
                        clip,
                    )
                }
                RenderCommand::Clear { .. } => continue,
            };
            let surface_destination_quad = destination_quad.map(|quad| {
                let (scale_x, scale_y, offset_x, offset_y) = self.viewport();
                quad.map(|[game_x, game_y]| {
                    [offset_x + game_x * scale_x, offset_y + game_y * scale_y]
                })
            });
            let (x, y, width, height) = self.game_to_surface_rect(x, y, width, height);
            // Every draw stays inside the game frame, so the letterbox bars
            // stay black even where an image is larger than the screen.
            let scissor = match self.game_clip_to_scissor(clip.unwrap_or([
                0.0,
                0.0,
                self.virtual_width,
                self.virtual_height,
            ])) {
                Some(scissor) => Some(scissor),
                None => continue,
            };
            let texture_opaque = self
                .textures
                .get(&texture)
                .map(|record| record.opaque)
                .unwrap_or(false);
            let pipeline_kind =
                native_pipeline_kind(blend_mode, opacity, texture_opaque || ignore_source_alpha);
            let shader_opacity = if pipeline_kind == NativePipelineKind::ConstantInterpolation {
                1.0
            } else {
                opacity
            };
            let vertices = if let Some(points) = surface_destination_quad {
                quad_vertices_from_points(
                    points,
                    src_x,
                    src_y,
                    src_width,
                    src_height,
                    shader_opacity,
                    ignore_source_alpha,
                    self.config.width as f32,
                    self.config.height as f32,
                )
            } else {
                quad_vertices(
                    x,
                    y,
                    width,
                    height,
                    src_x,
                    src_y,
                    src_width,
                    src_height,
                    shader_opacity,
                    ignore_source_alpha,
                    rotation_degrees,
                    self.config.width as f32,
                    self.config.height as f32,
                )
            };
            let vertex_start = frame_vertices.len() as u32;
            frame_vertices.extend_from_slice(&vertices);
            let vertex_range = vertex_start..vertex_start + vertices.len() as u32;
            draw_items.push((
                texture,
                vertex_range,
                scissor,
                pipeline_kind,
                opacity,
                linear_sampling,
            ));
        }

        let stale_text_keys = self
            .text_cache
            .keys()
            .filter(|key| !active_text_keys.contains(*key))
            .cloned()
            .collect::<Vec<_>>();
        for key in stale_text_keys {
            if let Some(handle) = self.text_cache.remove(&key) {
                self.textures.remove(&handle.id);
            }
        }

        // The target renderer preserves the submitted composition order, but it
        // does not require one host GPU allocation per sprite.  Keep all quads
        // in one frame-local vertex buffer so a scene with many display objects
        // does not create hundreds of tiny Metal/D3D/Vulkan buffers every frame.
        let frame_vertex_buffer = (!frame_vertices.is_empty()).then(|| {
            self.device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("ethornell-frame-vertices"),
                    contents: bytemuck::cast_slice(&frame_vertices),
                    usage: wgpu::BufferUsages::VERTEX,
                })
        });

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("ethornell-render"),
            });
        {
            let mut pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("ethornell-render-pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &view,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(self.clear_color),
                        store: wgpu::StoreOp::Store,
                    },
                })],
                depth_stencil_attachment: None,
                timestamp_writes: None,
                occlusion_query_set: None,
            });
            if let Some(vertex_buffer) = frame_vertex_buffer.as_ref() {
                pass.set_vertex_buffer(0, vertex_buffer.slice(..));
            }
            for (texture, vertex_range, scissor, pipeline_kind, opacity, linear_sampling) in
                &draw_items
            {
                let pipeline = match pipeline_kind {
                    NativePipelineKind::Replace => &self.replace_pipeline,
                    NativePipelineKind::Additive => &self.additive_pipeline,
                    NativePipelineKind::Subtractive => &self.subtractive_pipeline,
                    NativePipelineKind::Alpha => &self.alpha_pipeline,
                    NativePipelineKind::ConstantInterpolation => {
                        &self.constant_interpolation_pipeline
                    }
                };
                pass.set_pipeline(pipeline);
                if *pipeline_kind == NativePipelineKind::ConstantInterpolation {
                    let factor = f64::from(opacity.clamp(0.0, 1.0));
                    pass.set_blend_constant(wgpu::Color {
                        r: factor,
                        g: factor,
                        b: factor,
                        a: factor,
                    });
                }
                if let Some((x, y, width, height)) = scissor {
                    pass.set_scissor_rect(*x, *y, *width, *height);
                } else {
                    pass.set_scissor_rect(0, 0, self.config.width, self.config.height);
                }
                if let Some(record) = self.textures.get(texture) {
                    let bind_group = if *linear_sampling {
                        &record.linear_bind_group
                    } else {
                        &record.nearest_bind_group
                    };
                    pass.set_bind_group(0, bind_group, &[]);
                    pass.draw(vertex_range.clone(), 0..1);
                }
            }
        }
        self.queue.submit(Some(encoder.finish()));
        frame.present();
        Ok(())
    }

    fn game_clip_to_scissor(&self, clip: [f32; 4]) -> Option<(u32, u32, u32, u32)> {
        game_clip_to_scissor_in(
            clip,
            self.viewport(),
            self.config.width as f32,
            self.config.height as f32,
            self.virtual_width,
            self.virtual_height,
        )
    }
}

/// The target composites in gamma space: its blends act on the stored 8-bit
/// values, as the CPU compositor here does. With an sRGB surface and sRGB
/// textures the GPU blended in linear light instead, which brightens every
/// partial blend: half-transparent windows over dark scenes, fades and
/// layered images came out lighter than in the target and than the CPU
/// captures of the same frames. So the surface and the textures are not
/// sRGB.
fn gamma_space_surface_format(formats: &[wgpu::TextureFormat]) -> wgpu::TextureFormat {
    formats
        .iter()
        .copied()
        .find(|format| !format.is_srgb())
        .unwrap_or(formats[0])
}
#[cfg(test)]
mod native_blend_tests {
    use super::{NativePipelineKind, native_pipeline_kind};

    #[test]
    fn opaque_copy_uses_replace_pipeline() {
        assert_eq!(
            native_pipeline_kind(128, 1.0, true),
            NativePipelineKind::Replace
        );
    }

    #[test]
    fn transparent_copy_keeps_alpha_composition() {
        assert_eq!(
            native_pipeline_kind(128, 1.0, false),
            NativePipelineKind::Alpha
        );
        assert_eq!(
            native_pipeline_kind(128, 0.5, true),
            NativePipelineKind::Alpha
        );
    }

    #[test]
    fn additive_and_subtractive_modes_have_dedicated_pipelines() {
        assert_eq!(
            native_pipeline_kind(2, 1.0, false),
            NativePipelineKind::Additive
        );
        assert_eq!(
            native_pipeline_kind(3, 1.0, false),
            NativePipelineKind::Subtractive
        );
    }

    #[test]
    fn backb_constant_interpolation_has_a_dedicated_pipeline() {
        assert_eq!(
            native_pipeline_kind(0xf0, 0.5, false),
            NativePipelineKind::ConstantInterpolation
        );
    }
}

#[cfg(test)]
mod viewport_tests {
    use super::{
        game_clip_to_scissor, gamma_space_surface_format, surface_to_virtual_point,
        viewport_transform,
    };

    #[test]
    fn the_surface_format_stores_values_without_srgb_encoding() {
        let formats = [
            wgpu::TextureFormat::Bgra8UnormSrgb,
            wgpu::TextureFormat::Bgra8Unorm,
        ];
        assert_eq!(
            gamma_space_surface_format(&formats),
            wgpu::TextureFormat::Bgra8Unorm
        );
    }

    #[test]
    fn scissors_stay_inside_the_letterboxed_game_frame() {
        // A 4:3 game in a 16:10 surface: bars of 120 at the sides.
        let frame = game_clip_to_scissor([0.0, 0.0, 1280.0, 960.0], 1440.0, 900.0, 1280.0, 960.0);
        assert_eq!(frame, Some((120, 0, 1200, 900)));
        // An image wider than the game is cut at the frame.
        let wide = game_clip_to_scissor([-200.0, 0.0, 1680.0, 960.0], 1440.0, 900.0, 1280.0, 960.0);
        assert_eq!(wide, frame);
        // A clip entirely in the bar draws nothing.
        assert_eq!(
            game_clip_to_scissor([-200.0, 0.0, 100.0, 960.0], 1440.0, 900.0, 1280.0, 960.0),
            None
        );
    }

    #[test]
    fn hdpi_surface_uses_the_physical_device_scale() {
        assert_eq!(
            viewport_transform(2560.0, 1440.0, 1280.0, 720.0),
            (2.0, 0.0, 0.0)
        );
    }

    #[test]
    fn letterbox_offset_is_shared_by_rendering_and_input() {
        assert_eq!(
            viewport_transform(1600.0, 1200.0, 1280.0, 720.0),
            (1.25, 0.0, 150.0)
        );
    }

    #[test]
    fn hdpi_input_round_trips_to_virtual_game_coordinates() {
        assert_eq!(
            surface_to_virtual_point(1086.0, 314.0, 2560.0, 1440.0, 1280.0, 720.0),
            Some((543.0, 157.0))
        );
    }

    #[test]
    fn letterbox_input_rejects_bars_and_maps_the_viewport() {
        assert_eq!(
            surface_to_virtual_point(800.0, 150.0, 1600.0, 1200.0, 1280.0, 720.0),
            Some((640.0, 0.0))
        );
        assert_eq!(
            surface_to_virtual_point(800.0, 149.0, 1600.0, 1200.0, 1280.0, 720.0),
            None
        );
    }
}

#[cfg(test)]
mod font_tests {
    use super::*;

    #[test]
    fn requested_font_file_is_used_and_an_unusable_one_falls_back() {
        let system = SYSTEM_CJK_FONT_CANDIDATES
            .iter()
            .find(|path| std::path::Path::new(path).exists())
            .expect("a system font");
        let requested = load_text_font(Some((*system).into())).expect("requested font");
        assert_ne!(requested.glyph_id('A').0, 0);

        let missing = std::env::temp_dir().join("ethornell-no-such-font.ttf");
        let fallback = load_text_font(Some(missing.into_os_string())).expect("fallback font");
        assert_ne!(fallback.glyph_id('\u{65e5}').0, 0);
    }

    #[test]
    fn selected_system_font_contains_japanese_glyphs() {
        let font = load_system_cjk_font().expect("system CJK font");
        assert_ne!(font.glyph_id('\u{65e5}').0, 0);
    }

    #[test]
    fn lines_are_a_given_pitch_apart() {
        // A message window at font 28 with Graph91:89 spacing 25 starts a
        // line every 35 pixels (sub_42C4A0), not at the font's own height.
        let font = load_system_cjk_font().expect("system CJK font");
        let ink_rows = |image: &DecodedImage| {
            (0..image.height)
                .filter(|&y| {
                    (0..image.width)
                        .any(|x| image.rgba[((y * image.width + x) * 4 + 3) as usize] > 128)
                })
                .collect::<Vec<_>>()
        };
        let one = rasterize_text(&font, "H", 28.0, Some(35.0), [1.0; 4], &[]);
        let two = rasterize_text(&font, "H\nH", 28.0, Some(35.0), [1.0; 4], &[]);
        let first = ink_rows(&one);
        let both = ink_rows(&two);
        assert_eq!(both.len(), 2 * first.len());
        assert_eq!(both[first.len()], first[0] + 35);
        assert_eq!(two.height, one.height + 35);
    }

    #[test]
    fn styled_text_rasterizer_applies_per_character_color_and_shape() {
        let font = load_system_cjk_font().expect("system CJK font");
        let image = rasterize_text(
            &font,
            "AB",
            32.0,
            None,
            [1.0; 4],
            &[TextStyleSpan {
                start_char: 1,
                end_char: 2,
                packed_rgb: Some(0xff0000),
                bold: true,
                italic: true,
            }],
        );
        assert!(
            image
                .rgba
                .chunks_exact(4)
                .any(|pixel| pixel[0] > 200 && pixel[1] > 200 && pixel[2] > 200 && pixel[3] > 0)
        );
        assert!(
            image
                .rgba
                .chunks_exact(4)
                .any(|pixel| pixel[0] > 200 && pixel[1] < 20 && pixel[2] < 20 && pixel[3] > 0)
        );
    }
}

#[cfg(test)]
mod surface_tests {
    use super::*;

    fn capabilities() -> wgpu::SurfaceCapabilities {
        wgpu::SurfaceCapabilities {
            formats: vec![
                wgpu::TextureFormat::Rgba8Unorm,
                wgpu::TextureFormat::Bgra8UnormSrgb,
            ],
            present_modes: vec![wgpu::PresentMode::Immediate, wgpu::PresentMode::Fifo],
            alpha_modes: vec![
                wgpu::CompositeAlphaMode::PreMultiplied,
                wgpu::CompositeAlphaMode::Opaque,
            ],
            usages: wgpu::TextureUsages::RENDER_ATTACHMENT,
        }
    }

    #[test]
    fn surface_prefers_gamma_space_fifo_and_opaque() {
        let config = surface_configuration(&capabilities(), 800, 600, 2048).unwrap();
        assert_eq!(config.format, wgpu::TextureFormat::Rgba8Unorm);
        assert_eq!(config.present_mode, wgpu::PresentMode::Fifo);
        assert_eq!(config.alpha_mode, wgpu::CompositeAlphaMode::Opaque);
    }

    #[test]
    fn empty_capabilities_and_invalid_dimensions_return_errors() {
        let mut caps = capabilities();
        caps.formats.clear();
        assert!(surface_configuration(&caps, 800, 600, 2048).is_err());
        caps = capabilities();
        caps.present_modes.clear();
        assert!(surface_configuration(&caps, 800, 600, 2048).is_err());
        caps = capabilities();
        caps.alpha_modes.clear();
        assert!(surface_configuration(&caps, 800, 600, 2048).is_err());
        caps = capabilities();
        caps.usages = wgpu::TextureUsages::empty();
        assert!(surface_configuration(&caps, 800, 600, 2048).is_err());
        for (width, height) in [(0, 600), (800, 0), (2049, 600), (800, 2049)] {
            assert!(surface_configuration(&capabilities(), width, height, 2048).is_err());
        }
    }

    #[test]
    fn nonpreferred_capabilities_are_valid_fallbacks() {
        let mut caps = capabilities();
        caps.formats.truncate(1);
        caps.present_modes.truncate(1);
        caps.alpha_modes.truncate(1);
        assert!(surface_configuration(&caps, 800, 600, 2048).is_ok());
    }

    #[test]
    fn malformed_texture_size_fails_without_overflow() {
        assert!(
            validate_rgba(&DecodedImage {
                width: u32::MAX,
                height: u32::MAX,
                rgba: vec![]
            })
            .is_err()
        );
        assert!(
            validate_rgba(&DecodedImage {
                width: 1,
                height: 1,
                rgba: vec![0; 3]
            })
            .is_err()
        );
    }

    #[test]
    fn mobile_limits_use_adapter_resolution_alignment_and_report_failure() {
        let mut adapter = wgpu::Limits::downlevel_defaults();
        adapter.max_texture_dimension_2d = 4096;
        adapter.min_uniform_buffer_offset_alignment = 512;
        let required = renderer_limits(&adapter).unwrap();
        assert_eq!(required.max_texture_dimension_2d, 4096);
        assert_eq!(required.min_uniform_buffer_offset_alignment, 512);
        assert!(required.check_limits(&adapter));
        adapter.max_vertex_attributes = 1;
        assert!(
            renderer_limits(&adapter)
                .unwrap_err()
                .to_string()
                .contains("max_vertex_attributes")
        );
    }
}

#[allow(clippy::too_many_arguments)]
fn quad_vertices(
    x: f32,
    y: f32,
    w: f32,
    h: f32,
    src_x: f32,
    src_y: f32,
    src_w: f32,
    src_h: f32,
    opacity: f32,
    ignore_source_alpha: bool,
    rotation_degrees: f32,
    sw: f32,
    sh: f32,
) -> [Vertex; 6] {
    let center_x = x + w * 0.5;
    let center_y = y + h * 0.5;
    let radians = rotation_degrees.to_radians();
    let sin = radians.sin();
    let cos = radians.cos();
    let rotate = |px: f32, py: f32| {
        let dx = px - center_x;
        let dy = py - center_y;
        [
            center_x + dx * cos - dy * sin,
            center_y + dx * sin + dy * cos,
        ]
    };
    quad_vertices_from_points(
        [
            rotate(x, y),
            rotate(x, y + h),
            rotate(x + w, y + h),
            rotate(x + w, y),
        ],
        src_x,
        src_y,
        src_w,
        src_h,
        opacity,
        ignore_source_alpha,
        sw,
        sh,
    )
}

fn quad_vertices_from_points(
    points: [[f32; 2]; 4],
    src_x: f32,
    src_y: f32,
    src_w: f32,
    src_h: f32,
    opacity: f32,
    ignore_source_alpha: bool,
    sw: f32,
    sh: f32,
) -> [Vertex; 6] {
    let to_ndc = |[px, py]: [f32; 2]| [px / sw * 2.0 - 1.0, 1.0 - py / sh * 2.0];
    let [top_left, bottom_left, bottom_right, top_right] = points.map(to_ndc);
    let u0 = src_x.clamp(0.0, 1.0);
    let v0 = src_y.clamp(0.0, 1.0);
    let u1 = (src_x + src_w).clamp(0.0, 1.0);
    let v1 = (src_y + src_h).clamp(0.0, 1.0);
    let ignore_source_alpha = if ignore_source_alpha { 1.0 } else { 0.0 };
    [
        Vertex {
            position: top_left,
            texcoord: [u0, v0],
            opacity,
            ignore_source_alpha,
        },
        Vertex {
            position: bottom_left,
            texcoord: [u0, v1],
            opacity,
            ignore_source_alpha,
        },
        Vertex {
            position: bottom_right,
            texcoord: [u1, v1],
            opacity,
            ignore_source_alpha,
        },
        Vertex {
            position: top_left,
            texcoord: [u0, v0],
            opacity,
            ignore_source_alpha,
        },
        Vertex {
            position: bottom_right,
            texcoord: [u1, v1],
            opacity,
            ignore_source_alpha,
        },
        Vertex {
            position: top_right,
            texcoord: [u1, v0],
            opacity,
            ignore_source_alpha,
        },
    ]
}

const SHADER: &str = r#"
struct VertexIn {
  @location(0) position: vec2<f32>,
  @location(1) texcoord: vec2<f32>,
  @location(2) opacity: f32,
  @location(3) ignore_source_alpha: f32,
};

struct VertexOut {
  @builtin(position) position: vec4<f32>,
  @location(0) texcoord: vec2<f32>,
  @location(1) opacity: f32,
  @location(2) ignore_source_alpha: f32,
};

@vertex
fn vs_main(v: VertexIn) -> VertexOut {
  var out: VertexOut;
  out.position = vec4<f32>(v.position, 0.0, 1.0);
  out.texcoord = v.texcoord;
  out.opacity = v.opacity;
  out.ignore_source_alpha = v.ignore_source_alpha;
  return out;
}

@group(0) @binding(0) var tex: texture_2d<f32>;
@group(0) @binding(1) var samp: sampler;

@fragment
fn fs_main(v: VertexOut) -> @location(0) vec4<f32> {
  let c = textureSample(tex, samp, v.texcoord);
  let source_alpha = select(c.a, 1.0, v.ignore_source_alpha > 0.5);
  return vec4<f32>(c.rgb, source_alpha * v.opacity);
}
"#;

use wgpu::util::DeviceExt;

#[cfg(test)]
mod glow_tests {
    use super::*;

    #[test]
    fn text_glow_halos_the_text_and_moves_it_by_the_radius() {
        // One opaque black pixel of text at (1, 1).
        let mut rgba = vec![0u8; 3 * 3 * 4];
        rgba[(3 + 1) * 4 + 3] = 255;
        let text = DecodedImage {
            width: 3,
            height: 3,
            rgba,
        };
        let glow = TextGlow {
            radius_x: 1,
            radius_y: 1,
            rgb: [255, 255, 255],
            concentration: 192,
        };

        let out = apply_text_glow(&text, glow);

        assert_eq!((out.width, out.height), (5, 5));
        let pixel = |x: usize, y: usize| &out.rgba[(y * 5 + x) * 4..][..4];
        assert_eq!(pixel(2, 2), &[0, 0, 0, 255]);
        assert_eq!(pixel(1, 1), &[255, 255, 255, 191]);
        assert_eq!(pixel(3, 3), &[255, 255, 255, 191]);
        assert_eq!(pixel(0, 0)[3], 0);
        assert_eq!(pixel(4, 4)[3], 0);
    }
}
