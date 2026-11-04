/* MPL-2.0. Adapted from xmoezzz/siglus_rs, commit 5f358ef1360b0439e92aed2b749e8f25d8721954. */
/* deko3d resource and submission layer; see deko.h. */
#include "deko.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>
#include <deko3d.h>

void ethornell_switch_log_message(const char* message);

enum {
    DisplayWidth = 1280,
    DisplayHeight = 720,
    FramebufferCount = 3,
    FrameSlots = 2,
    MaxTextures = 4096,
    CodeMemorySize = 2 * 1024 * 1024,
    CommandChunkSize = 1024 * 1024,
    RingSize = 32 * 1024 * 1024,
    MaxPrograms = 32,
    MaxDeferred = 1024,
};

typedef struct Texture {
    bool used;
    bool render_target;
    uint32_t width, height, mip_levels;
    DkMemBlock memory;
    DkImage image;
    DkMemBlock depth_memory;
    DkImage depth;
} Texture;

typedef struct Deferred {
    DkMemBlock memory;
    int32_t texture; /* descriptor slot to release, or -1 */
} Deferred;

typedef struct FrameSlot {
    DkCmdBuf cmdbuf;
    DkMemBlock chunks[64];
    uint32_t chunk_count;
    DkMemBlock ring;
    uint32_t ring_used;
    DkFence fence;
    bool fence_pending;
    Deferred deferred[MaxDeferred];
    uint32_t deferred_count;
} FrameSlot;

static DkDevice device;
static DkQueue queue;
static NWindow* window;
static DkMemBlock framebuffer_memory;
static DkImage framebuffers[FramebufferCount];
static DkMemBlock display_depth_memory;
static DkImage display_depth;
static DkSwapchain swapchain;
static DkMemBlock code_memory;
static uint32_t code_used;
static DkShader programs[MaxPrograms];
static char program_names[MaxPrograms][32];
static uint32_t program_count;
static DkMemBlock descriptor_memory;
static DkImageDescriptor* image_descriptors;
static DkSamplerDescriptor* sampler_descriptors;
static Texture textures[MaxTextures];
static FrameSlot slots[FrameSlots];
static uint32_t frame_number;
static FrameSlot* current;
static bool recording;
static int display_slot = -1; /* acquired swapchain image this frame */
static bool textures_dirty;     /* uploads or new descriptors since the last barrier */
static bool pass_skipped;      /* the display could not be acquired */
static unsigned acquire_failures;

static void log_result(const char* what, int value) {
    char message[160];
    snprintf(message, sizeof(message), "ethornell_switch: gpu %s %d\n", what, value);
    ethornell_switch_log_message(message);
}

static void debug_callback(void* user, const char* context, DkResult result, const char* message) {
    (void) user;
    char text[256];
    snprintf(text, sizeof(text), "ethornell_switch: deko3d context=%s result=%u message=%s\n",
             context ? context : "(none)", (unsigned) result, message ? message : "(none)");
    ethornell_switch_log_message(text);
}

static uint32_t align_up(uint32_t value, uint32_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

static DkMemBlock make_memory(uint32_t size, uint32_t flags) {
    DkMemBlockMaker maker;
    dkMemBlockMakerDefaults(&maker, device, align_up(size, DK_MEMBLOCK_ALIGNMENT));
    maker.flags = flags;
    return dkMemBlockCreate(&maker);
}

/* A command buffer that ran out of memory gets another chunk (freed when
 * the frame's slot is reused). */
static void add_command_memory(void* user, DkCmdBuf cmdbuf, size_t min_size) {
    FrameSlot* slot = (FrameSlot*) user;
    const uint32_t size = align_up((uint32_t) (min_size > CommandChunkSize ? min_size : CommandChunkSize),
                                   DK_MEMBLOCK_ALIGNMENT);
    if (slot->chunk_count >= 64) {
        ethornell_switch_log_message("ethornell_switch: gpu command memory exhausted\n");
        return;
    }
    DkMemBlock memory = make_memory(size, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    slot->chunks[slot->chunk_count++] = memory;
    dkCmdBufAddMemory(cmdbuf, memory, 0, size);
}

/* Room in this frame's ring (CPU-written, GPU-read), or 0 when full. */
static DkGpuAddr ring_alloc(uint32_t size, uint32_t alignment, void** cpu) {
    FrameSlot* slot = current;
    const uint32_t start = align_up(slot->ring_used, alignment);
    if (start + size > RingSize) return 0;
    slot->ring_used = start + size;
    *cpu = (uint8_t*) dkMemBlockGetCpuAddr(slot->ring) + start;
    return dkMemBlockGetGpuAddr(slot->ring) + start;
}

/* Frees after the GPU is done with every frame that could use it: during
 * a frame, with that frame's slot; between frames, with the slot of the
 * frame just finished (the other one). */
static void defer(DkMemBlock memory, int32_t texture) {
    FrameSlot* slot = recording ? current : &slots[(frame_number + 1) % FrameSlots];
    if (slot->deferred_count >= MaxDeferred) {
        /* Rare: wait for the GPU and free right away. */
        dkQueueWaitIdle(queue);
        if (memory) dkMemBlockDestroy(memory);
        if (texture >= 0) textures[texture].used = false;
        return;
    }
    slot->deferred[slot->deferred_count++] = (Deferred) { memory, texture };
}

static void release_deferred(FrameSlot* slot) {
    for (uint32_t i = 0; i < slot->deferred_count; ++i) {
        if (slot->deferred[i].memory) dkMemBlockDestroy(slot->deferred[i].memory);
        if (slot->deferred[i].texture >= 0) textures[slot->deferred[i].texture].used = false;
    }
    slot->deferred_count = 0;
}

static void init_image(DkImage* image, DkMemBlock* memory, uint32_t width, uint32_t height,
                       uint32_t mip_levels, DkImageFormat format, uint32_t flags) {
    DkImageLayoutMaker maker;
    dkImageLayoutMakerDefaults(&maker, device);
    maker.flags = flags;
    maker.format = format;
    maker.dimensions[0] = width;
    maker.dimensions[1] = height;
    maker.mipLevels = mip_levels;
    DkImageLayout layout;
    dkImageLayoutInitialize(&layout, &maker);
    const uint32_t alignment = dkImageLayoutGetAlignment(&layout);
    const uint32_t size = align_up((uint32_t) dkImageLayoutGetSize(&layout),
                                   alignment > DK_MEMBLOCK_ALIGNMENT ? alignment : DK_MEMBLOCK_ALIGNMENT);
    *memory = make_memory(size, DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image);
    dkImageInitialize(image, &layout, *memory, 0);
}

static void write_descriptor(int32_t id) {
    DkImageView view;
    dkImageViewDefaults(&view, &textures[id].image);
    dkImageDescriptorInitialize(&image_descriptors[id], &view, false, false);
}

static void start_recording(void) {
    FrameSlot* slot = current;
    dkCmdBufClear(slot->cmdbuf);
    /* The first chunk is kept; chunks added when a frame needed more go. */
    for (uint32_t i = 1; i < slot->chunk_count; ++i) dkMemBlockDestroy(slot->chunks[i]);
    if (slot->chunk_count == 0) {
        add_command_memory(slot, slot->cmdbuf, CommandChunkSize);
    } else {
        slot->chunk_count = 1;
        dkCmdBufAddMemory(slot->cmdbuf, slot->chunks[0], 0, CommandChunkSize);
    }
    dkCmdBufBindImageDescriptorSet(slot->cmdbuf, dkMemBlockGetGpuAddr(descriptor_memory), MaxTextures);
    dkCmdBufBindSamplerDescriptorSet(slot->cmdbuf,
        dkMemBlockGetGpuAddr(descriptor_memory) + MaxTextures * sizeof(DkImageDescriptor),
        EthornellDekoSamplerCount);
    /* The ring is rewritten by the CPU every other frame: nothing the GPU
     * cached from its last use (vertices, uniforms, upload sources) or
     * from reused descriptor slots may survive. */
    dkCmdBufBarrier(slot->cmdbuf, DkBarrier_None,
                    DkInvalidateFlags_Image | DkInvalidateFlags_Shader | DkInvalidateFlags_Descriptors |
                        DkInvalidateFlags_L2Cache);
    recording = true;
}

/* Submits what has been recorded so far and keeps recording. */
static void submit_recorded(void) {
    DkCmdList list = dkCmdBufFinishList(current->cmdbuf);
    dkQueueSubmitCommands(queue, list);
}

void ethornell_deko_init(void) {
    DkDeviceMaker device_maker;
    dkDeviceMakerDefaults(&device_maker);
    device_maker.cbDebug = debug_callback;
    /* Default clip space: depth 0..1, y up, matching Ethornell's draw ABI. */
    device = dkDeviceCreate(&device_maker);

    DkQueueMaker queue_maker;
    dkQueueMakerDefaults(&queue_maker, device);
    queue_maker.flags = DkQueueFlags_Graphics;
    queue = dkQueueCreate(&queue_maker);

    DkImageLayoutMaker fb_maker;
    dkImageLayoutMakerDefaults(&fb_maker, device);
    fb_maker.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression;
    fb_maker.format = DkImageFormat_RGBA8_Unorm;
    fb_maker.dimensions[0] = DisplayWidth;
    fb_maker.dimensions[1] = DisplayHeight;
    DkImageLayout fb_layout;
    dkImageLayoutInitialize(&fb_layout, &fb_maker);
    const uint32_t fb_alignment = dkImageLayoutGetAlignment(&fb_layout);
    const uint32_t fb_size = align_up((uint32_t) dkImageLayoutGetSize(&fb_layout), fb_alignment);
    framebuffer_memory = make_memory(FramebufferCount * fb_size, DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image);
    const DkImage* images[FramebufferCount];
    for (unsigned i = 0; i < FramebufferCount; ++i) {
        dkImageInitialize(&framebuffers[i], &fb_layout, framebuffer_memory, i * fb_size);
        images[i] = &framebuffers[i];
    }
    init_image(&display_depth, &display_depth_memory, DisplayWidth, DisplayHeight, 1,
               DkImageFormat_Z24S8, DkImageFlags_UsageRender | DkImageFlags_HwCompression);
    DkSwapchainMaker swapchain_maker;
    window = nwindowGetDefault();
    dkSwapchainMakerDefaults(&swapchain_maker, device, window, images, FramebufferCount);
    swapchain = dkSwapchainCreate(&swapchain_maker);
    dkSwapchainSetSwapInterval(swapchain, 1);

    code_memory = make_memory(CodeMemorySize, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached | DkMemBlockFlags_Code);
    code_used = 0;

    const uint32_t descriptor_size = MaxTextures * sizeof(DkImageDescriptor) +
                                     EthornellDekoSamplerCount * sizeof(DkSamplerDescriptor);
    descriptor_memory = make_memory(descriptor_size, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    image_descriptors = (DkImageDescriptor*) dkMemBlockGetCpuAddr(descriptor_memory);
    sampler_descriptors = (DkSamplerDescriptor*) (image_descriptors + MaxTextures);
    for (int i = 0; i < EthornellDekoSamplerCount; ++i) {
        DkSampler sampler;
        dkSamplerDefaults(&sampler);
        sampler.wrapMode[0] = DkWrapMode_ClampToEdge;
        sampler.wrapMode[1] = DkWrapMode_ClampToEdge;
        sampler.wrapMode[2] = DkWrapMode_ClampToEdge;
        if (i == EthornellDekoSampler_Linear) {
            sampler.minFilter = DkFilter_Linear;
            sampler.magFilter = DkFilter_Linear;
            sampler.mipFilter = DkMipFilter_Linear;
        }
        dkSamplerDescriptorInitialize(&sampler_descriptors[i], &sampler);
    }

    for (unsigned i = 0; i < FrameSlots; ++i) {
        DkCmdBufMaker maker;
        dkCmdBufMakerDefaults(&maker, device);
        maker.userData = &slots[i];
        maker.cbAddMem = add_command_memory;
        slots[i].cmdbuf = dkCmdBufCreate(&maker);
        slots[i].ring = make_memory(RingSize, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
    }
    ethornell_switch_log_message("ethornell_switch: gpu ready\n");
}

void ethornell_deko_exit(void) {
    /* Horizon reclaims the device and its memory when the process ends;
     * waiting on a swapchain the compositor holds can block forever (see
     * main.c). */
    for (unsigned i = 0; i < FrameSlots; ++i) {
        if (slots[i].fence_pending) dkFenceWait(&slots[i].fence, 250 * 1000 * 1000LL);
    }
}

uint32_t ethornell_deko_display_width(void) { return DisplayWidth; }
uint32_t ethornell_deko_display_height(void) { return DisplayHeight; }

uint32_t ethornell_deko_program(const char* name) {
    for (uint32_t i = 0; i < program_count; ++i) {
        if (strcmp(program_names[i], name) == 0) return i + 1;
    }
    if (program_count >= MaxPrograms) return 0;
    char path[96];
    snprintf(path, sizeof(path), "romfs:/shaders/%s.dksh", name);
    FILE* file = fopen(path, "rb");
    if (!file) {
        log_result("missing shader (see log above)", 0);
        ethornell_switch_log_message(path);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    const long length = ftell(file);
    rewind(file);
    if (length <= 0 || (uint32_t) length > CodeMemorySize - code_used) {
        fclose(file);
        log_result("shader too large", (int) length);
        return 0;
    }
    const uint32_t offset = code_used;
    code_used += align_up((uint32_t) length, DK_SHADER_CODE_ALIGNMENT);
    fread((uint8_t*) dkMemBlockGetCpuAddr(code_memory) + offset, (size_t) length, 1, file);
    fclose(file);
    DkShaderMaker maker;
    dkShaderMakerDefaults(&maker, code_memory, offset);
    dkShaderInitialize(&programs[program_count], &maker);
    snprintf(program_names[program_count], sizeof(program_names[0]), "%s", name);
    return ++program_count;
}

int32_t ethornell_deko_texture_create(uint32_t width, uint32_t height, uint32_t mip_levels, uint32_t flags) {
    if (width == 0 || height == 0 || mip_levels == 0) return -1;
    int32_t id = -1;
    for (int32_t i = 0; i < MaxTextures; ++i) {
        if (!textures[i].used) {
            id = i;
            break;
        }
    }
    if (id < 0) {
        ethornell_switch_log_message("ethornell_switch: gpu texture slots exhausted\n");
        return -1;
    }
    Texture* texture = &textures[id];
    memset(texture, 0, sizeof(*texture));
    texture->used = true;
    texture->width = width;
    texture->height = height;
    texture->mip_levels = mip_levels;
    texture->render_target = (flags & EthornellDekoTexture_RenderTarget) != 0;
    init_image(&texture->image, &texture->memory, width, height, mip_levels, DkImageFormat_RGBA8_Unorm,
               texture->render_target ? DkImageFlags_UsageRender | DkImageFlags_HwCompression : 0);
    if (texture->render_target) {
        init_image(&texture->depth, &texture->depth_memory, width, height, 1, DkImageFormat_Z24S8,
                   DkImageFlags_UsageRender | DkImageFlags_HwCompression);
    }
    write_descriptor(id);
    textures_dirty = true;
    return id;
}

void ethornell_deko_texture_upload(int32_t id, uint32_t level, const uint8_t* rgba, uint32_t width, uint32_t height) {
    if (!recording || id < 0 || id >= MaxTextures || !textures[id].used) return;
    const uint32_t size = width * height * 4;
    void* cpu = NULL;
    DkGpuAddr addr = ring_alloc(size, 256, &cpu);
    DkMemBlock temporary = NULL;
    if (addr == 0) {
        temporary = make_memory(size, DkMemBlockFlags_CpuUncached | DkMemBlockFlags_GpuCached);
        cpu = dkMemBlockGetCpuAddr(temporary);
        addr = dkMemBlockGetGpuAddr(temporary);
    }
    memcpy(cpu, rgba, size);
    DkImageView view;
    dkImageViewDefaults(&view, &textures[id].image);
    view.mipLevelOffset = (uint8_t) level;
    view.mipLevelCount = 1;
    const DkCopyBuf src = { addr, 0, 0 }; /* tightly packed */
    const DkImageRect rect = { 0, 0, 0, width, height, 1 };
    dkCmdBufCopyBufferToImage(current->cmdbuf, &src, &view, &rect, 0);
    if (temporary) defer(temporary, -1);
    textures_dirty = true;
}

void ethornell_deko_texture_destroy(int32_t id) {
    if (id < 0 || id >= MaxTextures || !textures[id].used) return;
    Texture* texture = &textures[id];
    defer(texture->memory, -1);
    if (texture->render_target) defer(texture->depth_memory, -1);
    /* The slot is reused only after the frames that may read it are done. */
    defer(NULL, id);
    texture->memory = NULL;
    texture->depth_memory = NULL;
}

bool ethornell_deko_texture_read(int32_t id, uint8_t* rgba) {
    if (!recording || id < 0 || id >= MaxTextures || !textures[id].used) return false;
    Texture* texture = &textures[id];
    const uint32_t size = texture->width * texture->height * 4;
    DkMemBlock staging = make_memory(size, DkMemBlockFlags_CpuCached | DkMemBlockFlags_GpuCached);
    dkCmdBufBarrier(current->cmdbuf, DkBarrier_Fragments, DkInvalidateFlags_Image);
    DkImageView view;
    dkImageViewDefaults(&view, &texture->image);
    const DkImageRect rect = { 0, 0, 0, texture->width, texture->height, 1 };
    const DkCopyBuf dst = { dkMemBlockGetGpuAddr(staging), 0, 0 };
    dkCmdBufCopyImageToBuffer(current->cmdbuf, &view, &rect, &dst, 0);
    submit_recorded();
    dkQueueWaitIdle(queue);
    dkMemBlockFlushCpuCache(staging, 0, size);
    memcpy(rgba, dkMemBlockGetCpuAddr(staging), size);
    dkMemBlockDestroy(staging);
    return true;
}

bool ethornell_deko_display_read(uint8_t* rgba) {
    if (!recording || display_slot < 0) return false;
    const uint32_t size = DisplayWidth * DisplayHeight * 4;
    DkMemBlock staging = make_memory(size, DkMemBlockFlags_CpuCached | DkMemBlockFlags_GpuCached);
    dkCmdBufBarrier(current->cmdbuf, DkBarrier_Fragments, DkInvalidateFlags_Image);
    DkImageView view;
    dkImageViewDefaults(&view, &framebuffers[display_slot]);
    const DkImageRect rect = { 0, 0, 0, DisplayWidth, DisplayHeight, 1 };
    const DkCopyBuf dst = { dkMemBlockGetGpuAddr(staging), 0, 0 };
    dkCmdBufCopyImageToBuffer(current->cmdbuf, &view, &rect, &dst, 0);
    submit_recorded();
    dkQueueWaitIdle(queue);
    dkMemBlockFlushCpuCache(staging, 0, size);
    memcpy(rgba, dkMemBlockGetCpuAddr(staging), size);
    dkMemBlockDestroy(staging);
    return true;
}

bool ethornell_deko_begin_frame(void) {
    FrameSlot* slot = &slots[frame_number % FrameSlots];
    if (slot->fence_pending) {
        /* Ryujinx can spend hundreds of milliseconds compiling a pipeline;
         * one second avoids treating that as a wedged queue. */
        const DkResult result = dkFenceWait(&slot->fence, 1000 * 1000 * 1000LL);
        if (result != DkResult_Success) {
            log_result("frame fence wait", (int) result);
            return false;
        }
        slot->fence_pending = false;
    }
    release_deferred(slot);
    slot->ring_used = 0;
    current = slot;
    display_slot = -1;
    start_recording();
    return true;
}

/* dkQueueAcquireImage treats nwindowDequeueBuffer failure as fatal, and
 * Ryujinx fails it transiently while its window is resized or focused:
 * acquire as deko3d does, keeping that failure recoverable. */
static bool acquire_display(void) {
    int slot = -1;
    DkFence acquire_fence = {0};
    *(uint32_t*) acquire_fence._storage = 2; /* deko3d Fence::Status_Waiting */
    NvMultiFence* const nv_fence = (NvMultiFence*) (void*) (acquire_fence._storage + sizeof(uint32_t));
    const Result result = nwindowDequeueBuffer(window, &slot, nv_fence);
    if (R_FAILED(result) || slot < 0 || slot >= FramebufferCount) {
        if (acquire_failures++ == 0) log_result("display acquire failed", (int) result);
        return false;
    }
    acquire_failures = 0;
    /* Work recorded so far (offscreen passes) goes first; the display
     * pass waits for the compositor to release the image. */
    submit_recorded();
    dkQueueWaitFence(queue, &acquire_fence);
    display_slot = slot;
    return true;
}

void ethornell_deko_begin_pass(int32_t target, const float* clear_color, bool clear_depth, int32_t clear_stencil) {
    if (!recording) return;
    DkCmdBuf cmdbuf = current->cmdbuf;
    DkImageView color_view;
    DkImageView depth_view;
    uint32_t width, height;
    pass_skipped = false;
    if (target < 0) {
        if (display_slot < 0 && !acquire_display()) {
            /* The compositor keeps its images; this frame is not shown. */
            pass_skipped = true;
            return;
        }
        dkImageViewDefaults(&color_view, &framebuffers[display_slot]);
        dkImageViewDefaults(&depth_view, &display_depth);
        width = DisplayWidth;
        height = DisplayHeight;
    } else {
        if (target >= MaxTextures || !textures[target].used || !textures[target].render_target) {
            pass_skipped = true;
            return;
        }
        dkImageViewDefaults(&color_view, &textures[target].image);
        dkImageViewDefaults(&depth_view, &textures[target].depth);
        width = textures[target].width;
        height = textures[target].height;
    }
    /* Earlier passes' targets and uploads are read from here on; new
     * texture descriptors are visible. */
    dkCmdBufBarrier(cmdbuf, DkBarrier_Full, DkInvalidateFlags_Image | DkInvalidateFlags_Descriptors);
    textures_dirty = false;
    dkCmdBufBindRenderTarget(cmdbuf, &color_view, &depth_view);
    const DkScissor scissor = { 0, 0, width, height };
    dkCmdBufSetScissors(cmdbuf, 0, &scissor, 1);
    if (clear_color) {
        dkCmdBufClearColorFloat(cmdbuf, 0, DkColorMask_RGBA, clear_color[0], clear_color[1],
                                clear_color[2], clear_color[3]);
    }
    if (clear_depth || clear_stencil >= 0) {
        dkCmdBufClearDepthStencil(cmdbuf, clear_depth, 1.0f, clear_stencil >= 0 ? 0xff : 0,
                                  clear_stencil >= 0 ? (uint8_t) clear_stencil : 0);
    }
}

void ethornell_deko_clear_stencil(uint8_t value) {
    if (!recording || pass_skipped) return;
    dkCmdBufClearDepthStencil(current->cmdbuf, false, 1.0f, 0xff, value);
}

void ethornell_deko_draw(const EthornellDekoDraw* d) {
    if (!recording || pass_skipped || d->vertex_count == 0 || d->vertex_program == 0 || d->fragment_program == 0 ||
        d->vertex_program > program_count || d->fragment_program > program_count) {
        return;
    }
    DkCmdBuf cmdbuf = current->cmdbuf;
    if (textures_dirty) {
        /* A texture was created or uploaded inside this pass (copies run
         * on their own engine): finish them and drop stale cache lines. */
        dkCmdBufBarrier(cmdbuf, DkBarrier_Full, DkInvalidateFlags_Image | DkInvalidateFlags_Descriptors);
        textures_dirty = false;
    }
    const DkShader* shaders[2] = { &programs[d->vertex_program - 1], &programs[d->fragment_program - 1] };
    dkCmdBufBindShaders(cmdbuf, DkStageFlag_GraphicsMask, shaders, 2);

    DkRasterizerState rasterizer;
    dkRasterizerStateDefaults(&rasterizer);
    rasterizer.cullMode = (DkFace) d->cull_mode;
    rasterizer.frontFace = (DkFrontFace) d->front_face;
    dkCmdBufBindRasterizerState(cmdbuf, &rasterizer);

    DkColorState color_state;
    dkColorStateDefaults(&color_state);
    color_state.blendEnableMask = d->blend_enable ? 1 : 0;
    dkCmdBufBindColorState(cmdbuf, &color_state);
    DkColorWriteState color_write;
    dkColorWriteStateDefaults(&color_write);
    dkColorWriteStateSetMask(&color_write, 0, d->color_write_mask);
    dkCmdBufBindColorWriteState(cmdbuf, &color_write);
    if (d->blend_enable) {
        DkBlendState blend;
        dkBlendStateDefaults(&blend);
        blend.colorBlendOp = (DkBlendOp) d->color_op;
        blend.srcColorBlendFactor = (DkBlendFactor) d->color_src;
        blend.dstColorBlendFactor = (DkBlendFactor) d->color_dst;
        blend.alphaBlendOp = (DkBlendOp) d->alpha_op;
        blend.srcAlphaBlendFactor = (DkBlendFactor) d->alpha_src;
        blend.dstAlphaBlendFactor = (DkBlendFactor) d->alpha_dst;
        dkCmdBufBindBlendState(cmdbuf, 0, &blend);
    }

    DkDepthStencilState depth;
    dkDepthStencilStateDefaults(&depth);
    depth.depthTestEnable = d->depth_test;
    depth.depthWriteEnable = d->depth_write;
    depth.depthCompareOp = (DkCompareOp) d->depth_compare;
    depth.stencilTestEnable = d->stencil_enable;
    depth.stencilFrontCompareOp = depth.stencilBackCompareOp = (DkCompareOp) d->stencil_compare;
    depth.stencilFrontFailOp = depth.stencilBackFailOp = (DkStencilOp) d->stencil_fail;
    depth.stencilFrontDepthFailOp = depth.stencilBackDepthFailOp = (DkStencilOp) d->stencil_depth_fail;
    depth.stencilFrontPassOp = depth.stencilBackPassOp = (DkStencilOp) d->stencil_pass;
    dkCmdBufBindDepthStencilState(cmdbuf, &depth);
    if (d->stencil_enable) dkCmdBufSetStencil(cmdbuf, DkFace_FrontAndBack, 0xff, d->stencil_ref, 0xff);

    /* Attributes by location; unused locations in between read zero. */
    DkVtxAttribState attribs[EthornellDekoMaxVertexAttribs];
    uint32_t attrib_count = 0;
    for (uint32_t i = 0; i < EthornellDekoMaxVertexAttribs; ++i) {
        if (d->attribs[i].components) attrib_count = i + 1;
    }
    static const DkVtxAttribSize sizes[5] = { DkVtxAttribSize_1x32, DkVtxAttribSize_1x32, DkVtxAttribSize_2x32,
                                              DkVtxAttribSize_3x32, DkVtxAttribSize_4x32 };
    for (uint32_t i = 0; i < attrib_count; ++i) {
        memset(&attribs[i], 0, sizeof(attribs[i]));
        const uint32_t components = d->attribs[i].components;
        attribs[i].bufferId = 0;
        attribs[i].isFixed = components == 0;
        attribs[i].offset = components ? d->attribs[i].offset : 0;
        attribs[i].size = sizes[components > 4 ? 4 : components];
        attribs[i].type = DkVtxAttribType_Float;
    }
    dkCmdBufBindVtxAttribState(cmdbuf, attribs, attrib_count);
    const DkVtxBufferState buffer_state = { d->stride, 0 };
    dkCmdBufBindVtxBufferState(cmdbuf, &buffer_state, 1);
    const uint32_t vertex_bytes = d->vertex_count * d->stride;
    void* cpu = NULL;
    const DkGpuAddr vertices = ring_alloc(vertex_bytes, 16, &cpu);
    if (vertices == 0) {
        static bool logged;
        if (!logged) ethornell_switch_log_message("ethornell_switch: gpu frame ring full; draws skipped\n");
        logged = true;
        return;
    }
    memcpy(cpu, d->vertices, vertex_bytes);
    dkCmdBufBindVtxBuffer(cmdbuf, 0, vertices, vertex_bytes);

    DkResHandle handles[EthornellDekoMaxTextureUnits];
    for (uint32_t i = 0; i < EthornellDekoMaxTextureUnits; ++i) {
        int32_t texture = d->textures[i];
        if (texture < 0 || texture >= MaxTextures || !textures[texture].used) texture = 0;
        handles[i] = dkMakeTextureHandle((uint32_t) texture, d->samplers[i]);
    }
    dkCmdBufBindTextures(cmdbuf, DkStage_Fragment, 0, handles, EthornellDekoMaxTextureUnits);

    for (int stage = 0; stage < 2; ++stage) {
        const EthornellDekoUniform* uniforms = stage == 0 ? d->vertex_uniforms : d->fragment_uniforms;
        for (uint32_t i = 0; i < EthornellDekoMaxUniformBlocks; ++i) {
            if (!uniforms[i].data || !uniforms[i].size) continue;
            const uint32_t size = align_up(uniforms[i].size, DK_UNIFORM_BUF_ALIGNMENT);
            void* dst = NULL;
            const DkGpuAddr addr = ring_alloc(size, DK_UNIFORM_BUF_ALIGNMENT, &dst);
            if (addr == 0) return;
            memcpy(dst, uniforms[i].data, uniforms[i].size);
            dkCmdBufBindUniformBuffer(cmdbuf, stage == 0 ? DkStage_Vertex : DkStage_Fragment, i, addr, size);
        }
    }

    const DkViewport viewport = { d->viewport[0], d->viewport[1], d->viewport[2], d->viewport[3], 0.0f, 1.0f };
    dkCmdBufSetViewports(cmdbuf, 0, &viewport, 1);
    const DkScissor scissor = {
        (uint32_t) (d->scissor[0] < 0 ? 0 : d->scissor[0]), (uint32_t) (d->scissor[1] < 0 ? 0 : d->scissor[1]),
        (uint32_t) (d->scissor[2] < 0 ? 0 : d->scissor[2]), (uint32_t) (d->scissor[3] < 0 ? 0 : d->scissor[3]),
    };
    dkCmdBufSetScissors(cmdbuf, 0, &scissor, 1);
    dkCmdBufDraw(cmdbuf, DkPrimitive_Triangles, d->vertex_count, 1, 0, 0);
}

void ethornell_deko_end_frame(bool present) {
    if (!recording) return;
    submit_recorded();
    dkQueueSignalFence(queue, &current->fence, true);
    current->fence_pending = true;
    if (present && display_slot >= 0) {
        dkQueuePresentImage(queue, swapchain, display_slot);
    } else {
        dkQueueFlush(queue);
    }
    recording = false;
    frame_number++;
}
