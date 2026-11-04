/* MPL-2.0. Adapted from xmoezzz/siglus_rs, commit 5f358ef1360b0439e92aed2b749e8f25d8721954. */
/* deko3d layer of the Switch renderer (crates/siglus_scene_vm/src/render/
 * horizon). The Rust side plans the frame exactly as the desktop wgpu
 * renderer does and draws it through these calls; every draw carries its
 * complete state. Enum values are deko3d's own (deko3d.h). */
#pragma once

#include <stdbool.h>
#include <stdint.h>

enum {
    EthornellDekoMaxTextureUnits = 8,
    EthornellDekoMaxVertexAttribs = 16,
    EthornellDekoMaxUniformBlocks = 2,
};

/* Texture flags. */
enum {
    EthornellDekoTexture_RenderTarget = 1u << 0, /* with its own Z24S8 depth-stencil */
};

/* Samplers (clamp to edge). */
enum {
    EthornellDekoSampler_Linear = 0,    /* linear, linear between mip levels */
    EthornellDekoSampler_Point = 1,
    EthornellDekoSamplerCount = 2,
};

typedef struct EthornellDekoVertexAttrib {
    uint32_t offset;
    uint32_t components; /* 32-bit floats; 0: unused location */
} EthornellDekoVertexAttrib;

typedef struct EthornellDekoUniform {
    const void* data;
    uint32_t size;
} EthornellDekoUniform;

typedef struct EthornellDekoDraw {
    uint32_t vertex_program;
    uint32_t fragment_program;
    /* attribs[location] */
    EthornellDekoVertexAttrib attribs[EthornellDekoMaxVertexAttribs];
    uint32_t stride;
    const void* vertices;
    uint32_t vertex_count;

    /* Blending (DkBlendOp / DkBlendFactor); blend_enable 0 writes as is. */
    uint8_t blend_enable;
    uint8_t color_op, color_src, color_dst;
    uint8_t alpha_op, alpha_src, alpha_dst;
    uint8_t color_write_mask; /* DkColorMask */

    uint8_t depth_test, depth_write;
    uint8_t depth_compare; /* DkCompareOp */
    uint8_t stencil_enable;
    uint8_t stencil_compare, stencil_fail, stencil_depth_fail, stencil_pass; /* DkCompareOp, DkStencilOp */
    uint8_t stencil_ref;
    uint8_t cull_mode;  /* DkFace */
    uint8_t front_face; /* DkFrontFace */

    /* Texture id per unit (-1: none) and EthornellDekoSampler_*. */
    int32_t textures[EthornellDekoMaxTextureUnits];
    uint8_t samplers[EthornellDekoMaxTextureUnits];

    EthornellDekoUniform vertex_uniforms[EthornellDekoMaxUniformBlocks];
    EthornellDekoUniform fragment_uniforms[EthornellDekoMaxUniformBlocks];

    float viewport[4]; /* x, y, width, height in target pixels */
    int32_t scissor[4]; /* x, y, width, height */
} EthornellDekoDraw;

/* Setup and frames (main.c). */
void ethornell_deko_init(void);
void ethornell_deko_exit(void);

/* Rust side. */
uint32_t ethornell_deko_display_width(void);
uint32_t ethornell_deko_display_height(void);
uint32_t ethornell_deko_program(const char* name);
int32_t ethornell_deko_texture_create(uint32_t width, uint32_t height, uint32_t mip_levels, uint32_t flags);
void ethornell_deko_texture_upload(int32_t id, uint32_t level, const uint8_t* rgba, uint32_t width, uint32_t height);
void ethornell_deko_texture_destroy(int32_t id);
bool ethornell_deko_texture_read(int32_t id, uint8_t* rgba);
bool ethornell_deko_begin_frame(void);
void ethornell_deko_begin_pass(int32_t target, const float* clear_color, bool clear_depth, int32_t clear_stencil);
void ethornell_deko_clear_stencil(uint8_t value);
void ethornell_deko_draw(const EthornellDekoDraw* draw);
void ethornell_deko_end_frame(bool present);

/* Copies the actual GPU-rendered display for CLI diagnostics. */
bool ethornell_deko_display_read(uint8_t* rgba);
