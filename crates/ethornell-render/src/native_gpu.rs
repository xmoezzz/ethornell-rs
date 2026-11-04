//! Native GPU command submission. Switch/deko3d and Vita/GXM implement this
//! synchronous command ABI; no framebuffer is composited or scaled on the CPU.
use crate::{FontProvider, TextureId, shared::*};
use ab_glyph::FontArc;
use ethornell_core::{EthornellError, Result};
use ethornell_image::DecodedImage;
use std::{
    collections::{BTreeMap, BTreeSet},
    marker::PhantomData,
};

#[repr(C)]
#[derive(Clone, Copy, Debug, Default)]
pub struct NativeVertex {
    pub position: [f32; 2],
    pub uv: [f32; 2],
    pub opacity: f32,
    pub ignore_alpha: f32,
}
#[repr(C)]
pub struct NativeDraw {
    pub texture: u64,
    pub vertices: [NativeVertex; 6],
    pub scissor: [u32; 4],
    /// 0 alpha,1 replace,2 additive,3 reverse subtract,4 constant interpolation.
    pub blend: u32,
    pub linear: u32,
}
/// Callbacks must copy commands/upload bytes before returning; GPU-owned
/// resources remain alive until destroy. A callback returns 0 on success.
#[repr(C)]
#[derive(Clone, Copy)]
pub struct NativeGpu {
    pub create: unsafe extern "C" fn(u32, u32, *const u8) -> u64,
    pub update: unsafe extern "C" fn(u64, u32, u32, *const u8) -> i32,
    pub destroy: unsafe extern "C" fn(u64),
    pub begin: unsafe extern "C" fn(*const f32) -> i32,
    pub draw: unsafe extern "C" fn(*const NativeDraw) -> i32,
    pub end: unsafe extern "C" fn() -> i32,
}
struct Texture {
    native: u64,
    width: u32,
    height: u32,
    opaque: bool,
}
#[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
struct TextKey {
    text: String,
    size: u32,
    line_height: Option<u32>,
    color: [u32; 4],
    styles: Vec<TextStyleSpan>,
    glow: Option<TextGlow>,
}
pub struct Renderer<'w> {
    gpu: NativeGpu,
    textures: BTreeMap<TextureId, Texture>,
    text_cache: BTreeMap<TextKey, TextureHandle>,
    next: TextureId,
    font: FontArc,
    commands: Vec<RenderCommand>,
    notice: Option<String>,
    cursor: Option<[f32; 2]>,
    /// The arrow, rasterized for a surface this many pixels tall.
    cursor_texture: Option<(u32, TextureHandle)>,
    notice_texture: Option<TextureHandle>,
    surface: [u32; 2],
    virtual_size: [f32; 2],
    scale: PresentationScale,
    _lifetime: PhantomData<&'w ()>,
}
/// The pointer's height on a surface `surface_height` pixels tall: ~23 px
/// on a Vita, ~30 on a handheld Switch.
fn cursor_height(surface_height: u32) -> u32 {
    (surface_height as f32 * 0.042).round().max(16.) as u32
}
/// A white arrow with a dark outline, tip at the top-left pixel, antialiased
/// by 4x4 supersampling.
fn cursor_image(height: u32) -> DecodedImage {
    const ARROW: [[f32; 2]; 7] = [
        [0., 0.],
        [0., 0.78],
        [0.18, 0.62],
        [0.30, 0.95],
        [0.47, 0.88],
        [0.35, 0.58],
        [0.58, 0.58],
    ];
    let h = height as f32;
    let width = (h * 0.6).ceil() as u32;
    let points = ARROW.map(|[x, y]| [x * (h - 1.) + 0.5, y * (h - 1.) + 0.5]);
    let outline = (h / 24.).max(1.);
    let inside = |px: f32, py: f32| {
        let mut inside = false;
        for i in 0..points.len() {
            let [ax, ay] = points[i];
            let [bx, by] = points[(i + 1) % points.len()];
            if (ay > py) != (by > py) && px < ax + (py - ay) * (bx - ax) / (by - ay) {
                inside = !inside;
            }
        }
        inside
    };
    let edge_distance = |px: f32, py: f32| {
        (0..points.len())
            .map(|i| {
                let [ax, ay] = points[i];
                let [bx, by] = points[(i + 1) % points.len()];
                let (ex, ey) = (bx - ax, by - ay);
                let t = (((px - ax) * ex + (py - ay) * ey) / (ex * ex + ey * ey)).clamp(0., 1.);
                (px - ax - t * ex).hypot(py - ay - t * ey)
            })
            .fold(f32::INFINITY, f32::min)
    };
    let mut rgba = vec![0u8; (width * height * 4) as usize];
    for y in 0..height {
        for x in 0..width {
            let (mut white, mut covered) = (0u32, 0u32);
            for sy in 0..4 {
                for sx in 0..4 {
                    let px = x as f32 + (sx as f32 + 0.5) / 4.;
                    let py = y as f32 + (sy as f32 + 0.5) / 4.;
                    if inside(px, py) {
                        covered += 1;
                        if edge_distance(px, py) > outline {
                            white += 1;
                        }
                    }
                }
            }
            if covered > 0 {
                let i = ((y * width + x) * 4) as usize;
                let shade = (white * 255 / covered) as u8;
                let alpha = (covered * 255 / 16) as u8;
                rgba[i..i + 4].copy_from_slice(&[shade, shade, shade, alpha]);
            }
        }
    }
    DecodedImage {
        width,
        height,
        rgba,
    }
}
fn check(code: i32, operation: &str) -> Result<()> {
    if code == 0 {
        Ok(())
    } else {
        Err(EthornellError::Other(format!(
            "native GPU {operation} failed ({code})"
        )))
    }
}
impl Renderer<'static> {
    /// Platform initialization and teardown surround this renderer's lifetime.
    /// The callbacks stay valid until the renderer is dropped.
    pub fn new_native(width: u32, height: u32, font: FontArc, gpu: NativeGpu) -> Result<Self> {
        if width == 0 || height == 0 {
            return Err(EthornellError::Other(
                "invalid native GPU dimensions".into(),
            ));
        }
        Ok(Self {
            gpu,
            textures: BTreeMap::new(),
            text_cache: BTreeMap::new(),
            next: 1,
            font,
            commands: Vec::new(),
            notice: None,
            cursor: None,
            cursor_texture: None,
            notice_texture: None,
            surface: [width, height],
            virtual_size: [width as f32, height as f32],
            scale: PresentationScale::Fit,
            _lifetime: PhantomData,
        })
    }
}
impl Renderer<'_> {
    pub fn adapter_name(&self) -> &str {
        if cfg!(target_os = "vita") {
            "PS Vita GXM"
        } else {
            "Nintendo Switch deko3d"
        }
    }
    pub fn set_presentation_scale(&mut self, scale: PresentationScale) {
        self.scale = scale;
    }
    pub fn set_virtual_size(&mut self, width: f32, height: f32) {
        self.virtual_size = [width.max(1.), height.max(1.)];
    }
    pub fn surface_size(&self) -> (u32, u32) {
        (self.surface[0], self.surface[1])
    }
    pub fn graphics_memory_metric(&self) -> i32 {
        self.textures
            .values()
            .map(|t| u64::from(t.width) * u64::from(t.height) * 4)
            .sum::<u64>()
            .min(i32::MAX as u64) as i32
    }
    fn viewport(&self) -> (f32, f32, f32, f32) {
        presentation_viewport(
            self.scale,
            self.surface[0] as f32,
            self.surface[1] as f32,
            self.virtual_size[0],
            self.virtual_size[1],
        )
    }
    pub fn game_to_surface_rect(&self, x: f32, y: f32, w: f32, h: f32) -> (f32, f32, f32, f32) {
        let (sx, sy, ox, oy) = self.viewport();
        (ox + x * sx, oy + y * sy, w * sx, h * sy)
    }
    pub fn surface_to_game_point(&self, x: f32, y: f32) -> Option<(f32, f32)> {
        viewport_to_virtual_point(
            x,
            y,
            self.viewport(),
            self.virtual_size[0],
            self.virtual_size[1],
        )
    }
    pub fn insert_rgba(&mut self, image: &DecodedImage) -> Result<TextureHandle> {
        validate_rgba(image)?;
        let native = unsafe { (self.gpu.create)(image.width, image.height, image.rgba.as_ptr()) };
        if native == 0 {
            return Err(EthornellError::Other(format!(
                "native GPU texture allocation failed: {}x{} RGBA",
                image.width, image.height,
            )));
        }
        let id = self.next;
        self.next = self
            .next
            .checked_add(1)
            .ok_or_else(|| EthornellError::Other("texture IDs exhausted".into()))?;
        self.textures.insert(
            id,
            Texture {
                native,
                width: image.width,
                height: image.height,
                opaque: image.rgba.chunks_exact(4).all(|p| p[3] == 255),
            },
        );
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
        validate_rgba(image)?;
        if let Some(t) = self.textures.get_mut(&handle.id) {
            if (t.width, t.height) == (image.width, image.height) {
                check(
                    unsafe { (self.gpu.update)(t.native, t.width, t.height, image.rgba.as_ptr()) },
                    "texture upload",
                )?;
                t.opaque = image.rgba.chunks_exact(4).all(|p| p[3] == 255);
                return Ok(handle.clone());
            }
        }
        let replacement = self.insert_rgba(image)?;
        self.remove_texture(handle.id);
        Ok(replacement)
    }
    pub fn remove_texture(&mut self, id: TextureId) {
        if let Some(t) = self.textures.remove(&id) {
            unsafe { (self.gpu.destroy)(t.native) }
        }
    }
    pub fn check_gpu(&self) -> Result<()> {
        Ok(())
    }
    pub fn resize(&mut self, w: u32, h: u32) {
        let _ = self.try_resize(w, h);
    }
    pub fn try_resize(&mut self, w: u32, h: u32) -> Result<()> {
        if [w, h] != self.surface {
            return Err(EthornellError::Other("native display size is fixed".into()));
        }
        Ok(())
    }
    pub fn submit(&mut self, commands: &[RenderCommand]) {
        self.commands.clear();
        self.commands.extend_from_slice(commands);
    }
    pub fn set_notice(&mut self, text: Option<String>) {
        self.notice = text;
    }
    pub fn set_cursor(&mut self, x: f32, y: f32) {
        self.cursor = Some([x, y]);
    }
    pub fn render(&mut self) -> Result<()> {
        let mut commands = self.commands.clone();
        if let Some(notice) = self.notice.clone() {
            let background = match self.notice_texture.clone() {
                Some(t) => t,
                None => {
                    let t = self.insert_rgba(&DecodedImage {
                        width: 1,
                        height: 1,
                        rgba: vec![0, 0, 0, 230],
                    })?;
                    self.notice_texture = Some(t.clone());
                    t
                }
            };
            let w = self.virtual_size[0];
            let h = self.virtual_size[1];
            commands.push(RenderCommand::DrawTexture {
                texture: background.id,
                x: 0.,
                y: 0.,
                width: w,
                height: h,
                src_x: 0.,
                src_y: 0.,
                src_width: 1.,
                src_height: 1.,
                opacity: 1.,
                ignore_source_alpha: false,
                blend_mode: 1,
                rotation_degrees: 0.,
                destination_quad: None,
                linear_sampling: false,
                clip: None,
                z: i32::MAX - 2,
            });
            let columns = ((w - 32.) / 24.).max(1.) as usize;
            let mut text = String::new();
            let mut column = 0;
            for c in notice.chars().take(2048) {
                if c == '\n' {
                    column = 0;
                } else if column == columns {
                    text.push('\n');
                    column = 0;
                }
                text.push(c);
                column += 1;
            }
            commands.push(RenderCommand::DrawText {
                text,
                x: 16.,
                y: 16.,
                color: [1.; 4],
                size: 24.,
                line_height: None,
                styles: Vec::new(),
                glow: None,
                clip: None,
                z: i32::MAX - 1,
            });
        }
        if self.notice.is_none() {
            if let Some([x, y]) = self.cursor {
                let size = cursor_height(self.surface[1]);
                let t = match self.cursor_texture.clone() {
                    Some((height, t)) if height == size => t,
                    _ => {
                        let t = self.insert_rgba(&cursor_image(size))?;
                        self.cursor_texture = Some((size, t.clone()));
                        t
                    }
                };
                let (sx, sy, ox, oy) = self.viewport();
                commands.push(RenderCommand::DrawTexture {
                    texture: t.id,
                    x: (x - ox) / sx,
                    y: (y - oy) / sy,
                    width: t.width as f32 / sx,
                    height: t.height as f32 / sy,
                    src_x: 0.,
                    src_y: 0.,
                    src_width: 1.,
                    src_height: 1.,
                    opacity: 1.,
                    ignore_source_alpha: false,
                    blend_mode: 1,
                    rotation_degrees: 0.,
                    destination_quad: None,
                    linear_sampling: true,
                    clip: None,
                    z: i32::MAX,
                });
            }
        }
        let mut active = BTreeSet::new();
        let mut clear = [0., 0., 0., 1.];
        for c in &commands {
            if let RenderCommand::Clear { color } = c {
                clear = *color;
            }
        }
        commands.sort_by_key(|c| match c {
            RenderCommand::DrawTexture { z, .. } | RenderCommand::DrawText { z, .. } => *z,
            RenderCommand::Clear { .. } => i32::MIN,
        });
        // Upload before beginning the scene; a Vita texture write waits for the
        // previous scene and must never invalidate current in-flight commands.
        let mut draws = Vec::new();
        for c in commands {
            let c = if let RenderCommand::DrawText {
                text,
                x,
                y,
                color,
                size,
                line_height,
                styles,
                glow,
                clip,
                z,
            } = c
            {
                FontProvider::require_coverage(&self.font, &text)?;
                let key = TextKey {
                    text: text.clone(),
                    size: size.to_bits(),
                    line_height: line_height.map(f32::to_bits),
                    color: color.map(f32::to_bits),
                    styles: styles.clone(),
                    glow,
                };
                active.insert(key.clone());
                let texture = if let Some(t) = self.text_cache.get(&key) {
                    t.clone()
                } else {
                    let mut img =
                        rasterize_text(&self.font, &text, size, line_height, color, &styles);
                    if let Some(glow) = glow {
                        img = apply_text_glow(&img, glow);
                    }
                    let t = self.insert_rgba(&img)?;
                    self.text_cache.insert(key, t.clone());
                    t
                };
                RenderCommand::DrawTexture {
                    texture: texture.id,
                    x,
                    y,
                    width: texture.width as f32,
                    height: texture.height as f32,
                    src_x: 0.,
                    src_y: 0.,
                    src_width: 1.,
                    src_height: 1.,
                    opacity: 1.,
                    ignore_source_alpha: false,
                    blend_mode: 1,
                    rotation_degrees: 0.,
                    destination_quad: None,
                    linear_sampling: true,
                    clip,
                    z,
                }
            } else {
                c
            };
            if let Some(draw) = self.draw_command(c) {
                draws.push(draw);
            }
        }
        let stale = self
            .text_cache
            .keys()
            .filter(|k| !active.contains(*k))
            .cloned()
            .collect::<Vec<_>>();
        for key in stale {
            if let Some(t) = self.text_cache.remove(&key) {
                self.remove_texture(t.id);
            }
        }
        check(unsafe { (self.gpu.begin)(clear.as_ptr()) }, "begin")?;
        let result = draws
            .iter()
            .try_for_each(|d| check(unsafe { (self.gpu.draw)(d) }, "draw"));
        let end = check(unsafe { (self.gpu.end)() }, "present");
        result.and(end)
    }
    fn draw_command(&self, c: RenderCommand) -> Option<NativeDraw> {
        let RenderCommand::DrawTexture {
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
        } = c
        else {
            return None;
        };
        let t = self.textures.get(&texture)?;
        let viewport = self.viewport();
        let scissor = game_clip_to_scissor_in(
            clip.unwrap_or([0., 0., self.virtual_size[0], self.virtual_size[1]]),
            viewport,
            self.surface[0] as f32,
            self.surface[1] as f32,
            self.virtual_size[0],
            self.virtual_size[1],
        )?;
        let quad = destination_quad.unwrap_or_else(|| {
            let (sn, cs) = rotation_degrees.to_radians().sin_cos();
            [[-0.5, -0.5], [-0.5, 0.5], [0.5, 0.5], [0.5, -0.5]].map(|[a, b]| {
                [
                    x + width * 0.5 + a * width * cs - b * height * sn,
                    y + height * 0.5 + a * width * sn + b * height * cs,
                ]
            })
        });
        let [sx, sy, ox, oy] = [viewport.0, viewport.1, viewport.2, viewport.3];
        let points = quad.map(|[x, y]| {
            [
                (ox + x * sx) * 2. / self.surface[0] as f32 - 1.,
                1. - (oy + y * sy) * 2. / self.surface[1] as f32,
            ]
        });
        let uv = [
            [src_x, src_y],
            [src_x, src_y + src_height],
            [src_x + src_width, src_y + src_height],
            [src_x + src_width, src_y],
        ]
        .map(|[u, v]| [u.clamp(0., 1.), v.clamp(0., 1.)]);
        let vertices = [0, 1, 2, 0, 2, 3].map(|i| NativeVertex {
            position: points[i],
            uv: uv[i],
            opacity: opacity.clamp(0., 1.),
            ignore_alpha: if ignore_source_alpha || blend_mode == 0xf0 {
                1.
            } else {
                0.
            },
        });
        let blend = match blend_mode {
            128 if opacity >= 0.999 && (t.opaque || ignore_source_alpha) => 1,
            2 => 2,
            3 => 3,
            0xf0 => 4,
            _ => 0,
        };
        Some(NativeDraw {
            texture: t.native,
            vertices,
            scissor: [scissor.0, scissor.1, scissor.2, scissor.3],
            blend,
            linear: u32::from(linear_sampling),
        })
    }
}
impl Drop for Renderer<'_> {
    fn drop(&mut self) {
        let textures = std::mem::take(&mut self.textures);
        for (_, t) in textures {
            unsafe { (self.gpu.destroy)(t.native) }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::atomic::{AtomicU64, Ordering};
    #[test]
    fn the_cursor_is_an_outlined_arrow_sized_to_the_surface() {
        assert_eq!(cursor_height(544), 23);
        assert_eq!(cursor_height(720), 30);
        let image = cursor_image(23);
        assert_eq!((image.width, image.height), (14, 23));
        let pixel = |x: u32, y: u32| &image.rgba[((y * image.width + x) * 4) as usize..][..4];
        // Tip at the top-left, dark outline, white body, nothing to the right.
        assert!(pixel(0, 1)[3] > 0 && pixel(0, 1)[0] < 60);
        assert!(pixel(3, 8)[3] == 255 && pixel(3, 8)[0] == 255);
        assert_eq!(pixel(13, 2)[3], 0);
    }
    static NEXT: AtomicU64 = AtomicU64::new(1);
    unsafe extern "C" fn create(_: u32, _: u32, _: *const u8) -> u64 {
        NEXT.fetch_add(1, Ordering::Relaxed)
    }
    unsafe extern "C" fn update(_: u64, _: u32, _: u32, _: *const u8) -> i32 {
        0
    }
    unsafe extern "C" fn destroy(_: u64) {}
    unsafe extern "C" fn begin(_: *const f32) -> i32 {
        0
    }
    unsafe extern "C" fn draw(_: *const NativeDraw) -> i32 {
        0
    }
    unsafe extern "C" fn end() -> i32 {
        0
    }
    #[test]
    fn console_draw_abi_preserves_affine_clip_sampling_and_constant_blend() {
        assert_eq!(std::mem::size_of::<NativeVertex>(), 24);
        assert_eq!(std::mem::size_of::<NativeDraw>(), 176);
        let font = FontProvider::load_system_cjk_font().unwrap();
        let mut renderer = Renderer::new_native(
            960,
            544,
            font,
            NativeGpu {
                create,
                update,
                destroy,
                begin,
                draw,
                end,
            },
        )
        .unwrap();
        renderer.set_virtual_size(640., 480.);
        assert_eq!(renderer.surface_to_game_point(0., 272.), None);
        let texture = renderer
            .insert_rgba(&DecodedImage {
                width: 2,
                height: 2,
                rgba: [255, 0, 0, 0].repeat(4),
            })
            .unwrap();
        let mut command = RenderCommand::DrawTexture {
            texture: texture.id,
            x: 0.,
            y: 0.,
            width: 10.,
            height: 10.,
            src_x: 0.25,
            src_y: 0.,
            src_width: 0.5,
            src_height: 1.,
            opacity: 0.5,
            ignore_source_alpha: true,
            blend_mode: 0xf0,
            rotation_degrees: 0.,
            destination_quad: Some([[0., 0.], [0., 10.], [20., 10.], [20., 0.]]),
            linear_sampling: false,
            clip: Some([0., 0., 8., 8.]),
            z: 0,
        };
        let d = renderer.draw_command(command.clone()).unwrap();
        assert_eq!(d.blend, 4);
        assert_eq!(d.linear, 0);
        assert_eq!(d.vertices[0].ignore_alpha, 1.);
        assert_eq!(d.vertices[0].opacity, 0.5);
        assert_eq!(d.vertices[0].uv, [0.25, 0.]);
        assert_eq!(d.vertices[2].uv, [0.75, 1.]);
        assert!(d.scissor[0] > 100);
        assert!(d.scissor[2] < 12);
        if let RenderCommand::DrawTexture {
            blend_mode,
            ignore_source_alpha,
            ..
        } = &mut command
        {
            *blend_mode = 128;
            *ignore_source_alpha = false;
        }
        assert_eq!(renderer.draw_command(command.clone()).unwrap().blend, 0);
        if let RenderCommand::DrawTexture {
            opacity,
            ignore_source_alpha,
            ..
        } = &mut command
        {
            *opacity = 1.;
            *ignore_source_alpha = true;
        }
        assert_eq!(renderer.draw_command(command).unwrap().blend, 1);
        renderer.remove_texture(texture.id);
        assert!(renderer.textures.is_empty());
    }
}
