/* MPL-2.0. Ethornell's draw API on the deko3d layer from siglus_rs. */
#include "deko.h"
#include "ethornell_console.h"
#include <deko3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint32_t vertex_program, fragment_program;
static bool frame_started;
static bool start_uploads(void) {
  if (frame_started)
    return true;
  frame_started = ethornell_deko_begin_frame();
  return frame_started;
}
int ethornell_switch_gpu_init(void) {
  ethornell_deko_init();
  vertex_program = ethornell_deko_program("ethornell_vsh");
  fragment_program = ethornell_deko_program("ethornell_fsh");
  return vertex_program && fragment_program ? 0 : -1;
}
void ethornell_switch_gpu_exit(void) { ethornell_deko_exit(); }
uint64_t ethornell_gpu_create(uint32_t w, uint32_t h, const uint8_t *rgba) {
  if (!start_uploads())
    return 0;
  const int32_t id = ethornell_deko_texture_create(w, h, 1, 0);
  if (id < 0)
    return 0;
  ethornell_deko_texture_upload(id, 0, rgba, w, h);
  return (uint64_t)id + 1;
}
int32_t ethornell_gpu_update(uint64_t id, uint32_t w, uint32_t h,
                             const uint8_t *rgba) {
  if (!id || !rgba || !start_uploads())
    return -1;
  ethornell_deko_texture_upload((int32_t)(id - 1), 0, rgba, w, h);
  return 0;
}
void ethornell_gpu_destroy(uint64_t id) {
  if (id)
    ethornell_deko_texture_destroy((int32_t)(id - 1));
}
int32_t ethornell_gpu_begin(const float *clear) {
  if (!start_uploads())
    return -1;
  ethornell_deko_begin_pass(-1, clear, true, 0);
  return 0;
}
int32_t ethornell_gpu_draw(const EthornellDraw *source) {
  if (!source || !source->texture || source->blend > 4)
    return -1;
  EthornellDekoDraw d = {0};
  d.vertex_program = vertex_program;
  d.fragment_program = fragment_program;
  d.attribs[0] = (EthornellDekoVertexAttrib){0, 2};
  d.attribs[1] = (EthornellDekoVertexAttrib){8, 2};
  d.attribs[2] = (EthornellDekoVertexAttrib){16, 1};
  d.attribs[3] = (EthornellDekoVertexAttrib){20, 1};
  d.stride = sizeof(EthornellVertex);
  d.vertices = source->vertices;
  d.vertex_count = 6;
  d.blend_enable = source->blend != 1;
  d.color_write_mask = DkColorMask_RGBA;
  d.color_op = source->blend == 3 ? DkBlendOp_RevSub : DkBlendOp_Add;
  d.color_src = DkBlendFactor_SrcAlpha;
  d.color_dst = (source->blend == 2 || source->blend == 3)
                    ? DkBlendFactor_One
                    : DkBlendFactor_InvSrcAlpha;
  d.alpha_op = DkBlendOp_Add;
  d.alpha_src = source->blend == 4 ? DkBlendFactor_Zero : DkBlendFactor_One;
  d.alpha_dst =
      source->blend == 4 ? DkBlendFactor_One : DkBlendFactor_InvSrcAlpha;
  d.depth_compare = DkCompareOp_Always;
  d.cull_mode = DkFace_None;
  d.front_face = DkFrontFace_CCW;
  for (unsigned i = 0; i < EthornellDekoMaxTextureUnits; i++)
    d.textures[i] = -1;
  d.textures[0] = (int32_t)(source->texture - 1);
  d.samplers[0] =
      source->linear ? EthornellDekoSampler_Linear : EthornellDekoSampler_Point;
  d.viewport[2] = ethornell_deko_display_width();
  d.viewport[3] = ethornell_deko_display_height();
  for (unsigned i = 0; i < 4; i++)
    d.scissor[i] = (int32_t)source->scissor[i];
  ethornell_deko_draw(&d);
  return 0;
}
int32_t ethornell_gpu_end(void) {
  /* One-shot GPU readback requested from the CLI, without desktop input. */
  const char *request = "sdmc:/switch/ethornell/capture.request";
  FILE *marker = fopen(request, "rb");
  if (marker) {
    fclose(marker);
    remove(request);
    const uint32_t w = ethornell_deko_display_width(),
                   h = ethornell_deko_display_height();
    uint8_t *rgba = malloc((size_t)w * h * 4);
    if (rgba && ethornell_deko_display_read(rgba)) {
      FILE *out = fopen("sdmc:/switch/ethornell/capture.ppm", "wb");
      if (out) {
        fprintf(out, "P6\n%u %u\n255\n", w, h);
        for (uint32_t y = 0; y < h; y++) {
          uint8_t row[1280 * 3];
          for (uint32_t x = 0; x < w; x++)
            memcpy(row + x * 3, rgba + ((size_t)y * w + x) * 4, 3);
          fwrite(row, 3, w, out);
        }
        fclose(out);
      }
    }
    free(rgba);
  }

  ethornell_deko_end_frame(true);
  frame_started = false;
  return 0;
}
