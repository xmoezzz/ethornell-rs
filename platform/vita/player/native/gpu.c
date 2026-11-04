/* MPL-2.0. GXM submission on vita2d's context, following xmoezzz/siglus_rs.
 * The embedded E-mote programs are reused under MPL-2.0; their wipe-alpha
 * stage implements XRGB bypass without rewriting texture bytes. */
#include "ethornell_console.h"
#include "shaders.h"
#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>
#include <stdlib.h>
#include <string.h>
#include <vita2d.h>
static SceGxmContext *ctx;
static SceGxmShaderPatcher *patcher;
static SceGxmShaderPatcherId vp_id, fp_id;
static SceGxmVertexProgram *vp;
static SceGxmFragmentProgram *fp[5];
static uint32_t frame;
static SceUID ring_ids[3];
static void *rings[3];
static uint32_t ring_used;
#define RING_BYTES (2 * 1024 * 1024)
static void *ring_alloc(uint32_t size, uint32_t align) {
  uint32_t at = (ring_used + align - 1) & ~(align - 1);
  if (at + size > RING_BYTES)
    return NULL;
  ring_used = at + size;
  return (uint8_t *)rings[frame % 3] + at;
}

typedef struct Texture {
  vita2d_texture *tex;
  uint32_t retired;
  struct Texture *next;
} Texture;
static Texture *retired;
typedef struct Vertex {
  float position[2], model[2], uv[2], color[4], blend, clip[4], wipe[3];
} Vertex;
int ethornell_vita_gpu_init(void) {
  if (vita2d_init_advanced(2 * 1024 * 1024) < 0)
    return -1;
  ctx = vita2d_get_context();
  patcher = vita2d_get_shader_patcher();
  if (!ctx || !patcher)
    return -2;
  if (sceGxmShaderPatcherRegisterProgram(
          patcher, (const SceGxmProgram *)emote_v, &vp_id) < 0)
    return -3;
  if (sceGxmShaderPatcherRegisterProgram(
          patcher, (const SceGxmProgram *)emote_f, &fp_id) < 0)
    return -4;
  const SceGxmProgram *program = sceGxmShaderPatcherGetProgramFromId(vp_id);
  const char *names[] = {"clip_position", "model_position", "texcoord", "color",
                         "blend_mode",    "clip_rect",      "wipe"};
  const uint16_t offsets[] = {0, 8, 16, 24, 40, 44, 60};
  const uint8_t counts[] = {2, 2, 2, 4, 1, 4, 3};
  SceGxmVertexAttribute attrs[7] = {0};
  for (unsigned i = 0; i < 7; i++) {
    const SceGxmProgramParameter *p =
        sceGxmProgramFindParameterByName(program, names[i]);
    if (!p)
      return -5;
    attrs[i].streamIndex = 0;
    attrs[i].offset = offsets[i];
    attrs[i].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
    attrs[i].componentCount = counts[i];
    attrs[i].regIndex = sceGxmProgramParameterGetResourceIndex(p);
  }
  const SceGxmVertexStream stream = {sizeof(Vertex),
                                     SCE_GXM_INDEX_SOURCE_INDEX_16BIT};
  if (sceGxmShaderPatcherCreateVertexProgram(patcher, vp_id, attrs, 7, &stream,
                                             1, &vp) < 0)
    return -6;
  for (unsigned mode = 0; mode < 5; mode++) {
    SceGxmBlendInfo blend = {0};
    blend.colorMask = SCE_GXM_COLOR_MASK_ALL;
    blend.colorFunc = mode == 3 ? SCE_GXM_BLEND_FUNC_REVERSE_SUBTRACT
                                : SCE_GXM_BLEND_FUNC_ADD;
    blend.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
    blend.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
    blend.colorDst = (mode == 2 || mode == 3)
                         ? SCE_GXM_BLEND_FACTOR_ONE
                         : SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend.alphaSrc =
        mode == 4 ? SCE_GXM_BLEND_FACTOR_ZERO : SCE_GXM_BLEND_FACTOR_ONE;
    blend.alphaDst = mode == 4 ? SCE_GXM_BLEND_FACTOR_ONE
                               : SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    if (sceGxmShaderPatcherCreateFragmentProgram(
            patcher, fp_id, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
            SCE_GXM_MULTISAMPLE_NONE, mode == 1 ? NULL : &blend, program,
            &fp[mode]) < 0)
      return -7;
  }
  for (unsigned i = 0; i < 3; i++) {
    ring_ids[i] = sceKernelAllocMemBlock(
        "ethornell-vertices", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE,
        RING_BYTES, NULL);
    if (ring_ids[i] < 0 ||
        sceKernelGetMemBlockBase(ring_ids[i], &rings[i]) < 0 ||
        sceGxmMapMemory(rings[i], RING_BYTES, SCE_GXM_MEMORY_ATTRIB_READ) < 0)
      return -8;
  }
  vita2d_set_vblank_wait(1);
  return 0;
}
static void free_retired(bool all) {
  Texture **next = &retired;
  while (*next) {
    Texture *t = *next;
    if (all || frame - t->retired >= 4) {
      *next = t->next;
      vita2d_free_texture(t->tex);
      free(t);
    } else {
      next = &t->next;
    }
  }
}
void ethornell_vita_gpu_exit(void) {
  vita2d_wait_rendering_done();
  free_retired(true);
  for (unsigned i = 0; i < 5; i++)
    if (fp[i])
      sceGxmShaderPatcherReleaseFragmentProgram(patcher, fp[i]);
  if (vp)
    sceGxmShaderPatcherReleaseVertexProgram(patcher, vp);
  if (fp_id)
    sceGxmShaderPatcherUnregisterProgram(patcher, fp_id);
  if (vp_id)
    sceGxmShaderPatcherUnregisterProgram(patcher, vp_id);
  for (unsigned i = 0; i < 3; i++) {
    if (rings[i])
      sceGxmUnmapMemory(rings[i]);
    if (ring_ids[i] > 0)
      sceKernelFreeMemBlock(ring_ids[i]);
  }
  vita2d_fini();
}
uint64_t ethornell_gpu_create(uint32_t w, uint32_t h, const uint8_t *rgba) {
  if (!w || !h || w > 4096 || h > 4096 || !rgba)
    return 0;
  Texture *t = calloc(1, sizeof(*t));
  if (!t)
    return 0;
  vita2d_texture_set_alloc_memblock_type(
      w * h >= 65536 ? SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW
                     : SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE);
  t->tex =
      vita2d_create_empty_texture_format(w, h, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8);
  if (!t->tex) {
    vita2d_texture_set_alloc_memblock_type(
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE);
    t->tex = vita2d_create_empty_texture_format(
        w, h, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8);
  }
  if (!t->tex) {
    free(t);
    return 0;
  }
  /* Plain RGBA textures own none of the optional render-target resources.
   * Some libvita2d builds leave these malloc-backed fields uninitialized;
   * its destructor nevertheless reads them and may free unrelated blocks. */
  t->tex->gxm_rtgt = NULL;
  t->tex->depth_UID = 0;
  t->tex->palette_UID = 0;
  const uint32_t stride = vita2d_texture_get_stride(t->tex);
  uint8_t *dest = vita2d_texture_get_datap(t->tex);
  for (uint32_t y = 0; y < h; y++)
    memcpy(dest + y * stride, rgba + y * w * 4, w * 4);
  return (uint64_t)(uintptr_t)t;
}
int32_t ethornell_gpu_update(uint64_t id, uint32_t w, uint32_t h,
                             const uint8_t *rgba) {
  Texture *t = (Texture *)(uintptr_t)id;
  if (!t || !rgba || vita2d_texture_get_width(t->tex) != w ||
      vita2d_texture_get_height(t->tex) != h)
    return -1;
  vita2d_wait_rendering_done();
  uint8_t *dest = vita2d_texture_get_datap(t->tex);
  const uint32_t stride = vita2d_texture_get_stride(t->tex);
  for (uint32_t y = 0; y < h; y++) {
    memcpy(dest + y * stride, rgba + y * w * 4, w * 4);
  }
  return 0;
}
void ethornell_gpu_destroy(uint64_t id) {
  Texture *t = (Texture *)(uintptr_t)id;
  if (!t)
    return;
  t->retired = frame;
  t->next = retired;
  retired = t;
}
int32_t ethornell_gpu_begin(const float *clear) {
  frame++;
  ring_used = 0;
  free_retired(false);
  vita2d_pool_reset();
  vita2d_start_drawing();
  vita2d_set_clear_color(
      RGBA8((unsigned)(clear[0] * 255), (unsigned)(clear[1] * 255),
            (unsigned)(clear[2] * 255), (unsigned)(clear[3] * 255)));
  vita2d_clear_screen();
  return 0;
}
int32_t ethornell_gpu_draw(const EthornellDraw *d) {
  if (!d || !d->texture || d->blend >= 5)
    return -1;
  Vertex *v = ring_alloc(6 * sizeof(Vertex), 16);
  uint16_t *indices = ring_alloc(6 * sizeof(uint16_t), 2);
  if (!v || !indices)
    return -2;
  for (unsigned i = 0; i < 6; i++) {
    memset(&v[i], 0, sizeof(v[i]));
    const EthornellVertex *s = &d->vertices[i];
    memcpy(v[i].position, s->position, sizeof(s->position));
    v[i].model[0] = (s->position[0] + 1) * 480;
    v[i].model[1] = (1 - s->position[1]) * 272;
    memcpy(v[i].uv, s->uv, sizeof(s->uv));
    v[i].color[0] = v[i].color[1] = v[i].color[2] = 1;
    v[i].color[3] = s->opacity;
    v[i].clip[0] = d->scissor[0];
    v[i].clip[1] = d->scissor[1];
    v[i].clip[2] = d->scissor[0] + d->scissor[2];
    v[i].clip[3] = d->scissor[1] + d->scissor[3];
    if (s->ignore_alpha > 0.5f) {
      v[i].wipe[0] = 0;
      v[i].wipe[1] = s->opacity;
      v[i].wipe[2] = 1;
    }
    indices[i] = i;
  }
  Texture *t = (Texture *)(uintptr_t)d->texture;
  SceGxmTexture texture = t->tex->gxm_tex;
  sceGxmTextureSetMinFilter(&texture, d->linear ? SCE_GXM_TEXTURE_FILTER_LINEAR
                                                : SCE_GXM_TEXTURE_FILTER_POINT);
  sceGxmTextureSetMagFilter(&texture, d->linear ? SCE_GXM_TEXTURE_FILTER_LINEAR
                                                : SCE_GXM_TEXTURE_FILTER_POINT);
  sceGxmTextureSetUAddrMode(&texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
  sceGxmTextureSetVAddrMode(&texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
  sceGxmSetVertexProgram(ctx, vp);
  sceGxmSetFragmentProgram(ctx, fp[d->blend]);
  sceGxmSetViewport(ctx, 480, 480, 272, -272, 0, 1);
  sceGxmSetViewportEnable(ctx, SCE_GXM_VIEWPORT_ENABLED);
  sceGxmSetCullMode(ctx, SCE_GXM_CULL_NONE);
  sceGxmSetFrontDepthFunc(ctx, SCE_GXM_DEPTH_FUNC_ALWAYS);
  sceGxmSetFrontDepthWriteEnable(ctx, SCE_GXM_DEPTH_WRITE_DISABLED);
  sceGxmSetFrontStencilFunc(ctx, SCE_GXM_STENCIL_FUNC_ALWAYS,
                            SCE_GXM_STENCIL_OP_KEEP, SCE_GXM_STENCIL_OP_KEEP,
                            SCE_GXM_STENCIL_OP_KEEP, 0xff, 0xff);
  sceGxmSetFragmentTexture(ctx, 0, &texture);
  sceGxmSetVertexStream(ctx, 0, v);
  return sceGxmDraw(ctx, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16,
                    indices, 6);
}
int32_t ethornell_gpu_end(void) {
  vita2d_end_drawing();
  vita2d_wait_rendering_done();
  vita2d_swap_buffers();
  return 0;
}
