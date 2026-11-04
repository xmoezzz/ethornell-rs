// BGI 1.622 remaining CDspObjBack subclasses and vector-wave rendering.
// Immutable evidence: raw/bgi1622/native_all.{c,asm}, not old-build offsets.
#include <stdint.h>

// 90:44 / 41CC60: 2..32 screen-sized, screen-format bitmap bindings. Each
// binding snapshots its registry generation. Alpha (vtable+4C) is a direct
// frame index in 41D0A0, not ordinary transparency. Out-of-range draws none.
// 90:45 / 41D230: screen bitmap, format-4 map, optional second format-4 map,
// alpha, interpolation flag. 412AB0 selects 412D60 (nearest, alpha-scaled
// map), 412F80/413370 (bilinear, full/scaled), 4137B0 (nearest between maps)
// or 4138D0 (bilinear between maps). Map words are signed sixteenth-pixels.
// 90:46 / 413DD0: horizontal box blur; mode 0 black-pads, mode 1 extends
// the end pixels. Both call the same integer reciprocal filter below.
// 90:48 / 41FD90: interpolate a source rectangle with alpha/256, stretch it
// to the destination using 7-bit fractional interpolation. Source x/y are
// 16.16, width/height integers. Scale uses source_dimension*0x10004/extent.
// Property 102: value low/high signed words are target x/y; extra low/high
// UNSIGNED words are target width/height (>=2). Hex-Rays loses these register
// arguments; 41FCF0/41FD50 assembly establishes the ordering.
// 90:49 / 41F560: unsigned nonzero 16.16 scale and signed 16.16 angle.
// Property 80 sets scale/angle modulation; 101 resets scale/angle and clears
// modulation. Alpha controls the sinusoidal scale modulation, not coverage.
// 90:4A / 41EA80: mosaic primary and optional secondary; 7000/7001 mean
// black/white. Blocks are alpha+1 pixels. Flag 1 first draws secondary at
// block size 257-alpha, then blends primary mosaic with transparency alpha.
// These classes write the framebuffer directly; their scalar alpha is not
// applied a second time by the portable layer compositor.

// 413EA0/414010 MMX scalar equivalent for one channel of one output pixel.
static unsigned blur_channel(const uint8_t *row,int width,int x,int radius,int repeat)
{
    if (radius>255) radius=255;
    unsigned count=2*radius+1;
    unsigned shift=count<64 ? 0 : count<128 ? 1 : 2;
    uint16_t sum=0;
    for (int i=x-radius;i<=x+radius;++i) {
        int index=i;
        if (repeat) {if (index<0) index=0;if (index>=width) index=width-1;}
        if (index>=0 && index<width) sum+=(uint16_t)(row[4*index]>>shift);
    }
    int value=(((int16_t)sum*(int)(0x10003/count))>>16)<<shift;
    return value<0 ? 0 : value>255 ? 255 : value;
}

// 409F30: each sixteen-byte record contains four duplicated i16 samples.
// Its selector is a wave-table SLOT (0..7), not a BP record-table pointer.
// 409FE0: start=current-4*offset; below loop_begin wraps to
// loop_begin+span-(loop_begin-start)%span, including span for remainder zero.
static int16_t scale_wave(int16_t sample,int alpha)
{
    return (int16_t)(((uint32_t)alpha*(uint32_t)(int)sample)>>8);
}

// 414820/414930 multiply-high, followed by signed-word multiply-add.
// Sprite mode 4 wraps the full source allocation, rather than clamping x/y.
// BackRPL 414300 clips out-of-range samples and optionally bilinearly samples
// using the high four bits of each low product. Alpha changes these wave
// coefficients; Sprite coverage is (256-mask_alpha)*alpha_multiplier/256.
static int vector_source_index(int x,int y,int width,int16_t vx,int16_t vy,int16_t wave)
{
    int dx=((int)vx*wave)>>16,dy=((int)vy*wave)>>16;
    return x+dx+(y+dy)*width;
}

// Sprite mode 3, 427B10: require a format-3 mask, allocate a primary-sized
// zero bitmap and copy/clamp the mask into it. Later mask registry changes
// do not affect that copy. 427420 stores shift and fixed parameter separately;
// the threshold is GetFixedParameter's UNSIGNED high word, not GetAlpha.
// 412850/412900 materialize coverage into an intermediate format-2 image:
static unsigned sprite_mask_coverage(unsigned threshold,unsigned shift,unsigned mask)
{
    if (threshold>=256) return 256;
    shift&=31; // x86 variable shift-count semantics
    int32_t value=(int32_t)(threshold*((1u<<shift)+1u)-(mask<<shift));
    return value<0 ? 0 : value>256 ? 256 : (unsigned)value;
}
// RGB source -> min(coverage,255); RGBA -> (source_alpha*coverage)>>8.
// Direct RGB compositor 412510 additionally quantizes coverage to seven
// bits. The portable intermediate follows 412850/412900's eight-bit path.
