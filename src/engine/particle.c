// BGI 1.622 particle implementation recovery.
// Raw: native_all.{c,asm}, particle_poll.{c,asm}. Rust: native_particle.rs
// and native_effect.rs. The source 1.622 addresses supersede the old export.
#include <stdint.h>
#include <math.h>

// C0:00/01/04/05: 8 screens, 0xC0000000 | slot. C0:05 source operands:
// screen,x,y,BLEND,INVERSE_ALPHA,PRIORITY (the old z/blend/alpha names were wrong).
// C0:06: 1..16 depth planes. first[] contains depth interval lengths;
// cumulative(first[i]<<8) is the upper depth bound for plane i. second[]
// contains display-priority OFFSETS, not clear colors. Count one always
// ignores second[]; null first[] selects 0x7FFF. Planes are format-2 ARGB.
// C0:0A writes the dirty-rectangle threshold (+368), not particle capacity.
// C0:0B: camera XYZ (>>8), Euler XYZ (16.16 degrees), positive focal distance,
// projection center XY. C0:0C sets the simulation interval (0..100ms), not
// emission percentage. Zero stops simulation. Default interval is 10ms.
// C0:08 rebuilds the plane images; it does NOT advance simulation.
// Global 491610 -> 441470 -> 424870 -> 44E710 advances every allocated screen,
// independently of enable state and C0:09's automatic raster-commit list.
// C0:09 commits visible screens when its wrapping unsigned deadline expires;
// it advances the deadline beyond now and commits once, without replaying
// missed raster commits. C0:0D warms the simulation for N milliseconds and
// restores its clock and next-emission arrays. C0:0F removes live particles
// and clears current counts/deadlines, retaining requested emitter counts.

// 44DEF0: two calls to the SAME Microsoft CRT rand used by SYS80:00/01.
// The zero range still consumes both random values. Original divisor zero
// for range==INT_MAX is undefined; the port returns zero in that case.
static int particle_rand(int range) {
    uint32_t r = ((uint32_t)rand() << 15) | rand();
    uint32_t d = abs32(range + 1);
    int n = d ? r % d : 0;
    return range < 0 ? -n : n;
}
static int spread(int base,int radius) {
    return base-radius+particle_rand(2*radius);
}

// C0:18 defines primary animation for kind 0 (snow) or 1 (firefly).
// C0:24/2C are their single-frame specializations. A bank owns copied source
// pixels and 32 prefiltered scales (32-shrink)/32. At most 32 equal-shaped
// RGB/ARGB frames, 64 banks per kind. C0:1A defines a secondary firefly bank;
// C0:1B updates its mixing factor. Replacing the secondary bank also updates
// active fireflies' second animation speed/frame-count (not their first phase).
// C0:1F changes animation timing conversion, not the pixel filter:
static void animation_steps(unsigned mode,unsigned period,unsigned variation,
                            unsigned numerator,int *base,int *radius) {
    unsigned n=numerator<<16;
    *base=period ? n/period : 0;
    switch(mode) {
    case 0:*radius=variation ? n/variation : 0;break;
    case 1:*radius=period && variation ? n/(period+variation)-*base : 0;break;
    case 2:*radius=period ? (uint64_t)(unsigned)*base*variation/period : 0;break;
    }
}

// 44DCE0 allocates floor(scale*width/65536), but 495880 computes a ROUNDED
// scaled sampling footprint and centers it inside that allocation. Resizing
// is not nearest-neighbor: exact scale 1 and 1/2 use 495E10 bilinear, scales
// strictly between them use 495CA0 weighted-area accumulation (saturating
// 16-bit SIMD lanes), smaller scales use 495B60's box average. The box filter
// averages RGB over nonzero-alpha pixels (all pixels for format 1), and alpha
// over the complete box. RGB alpha payload is ignored at composition time.

// 44E080 owns 32768 particle slots. Per kind/bank it retains requested count,
// live count, emission spacing and next deadline. Sum(requested)<=32768.
// C0:20/28 configure those four fields for snow/firefly respectively.
// 44E840 first advances existing particles in interval-sized ticks, then emits
// new particles bank-by-bank. A >500ms gap resets the clock/deadlines, avoiding
// replay of the backlog. New particles fill the first empty slot.

// C0:25 (4505C0), snow style source operands:
// bank, abs(x_radius)>>8, y>>8, abs(z_radius)>>8,
// vx>>8,abs(vx_radius)>>8,vy>>8,abs(vy_radius)>>8,
// vz>>8,abs(vz_radius)>>8,(*wind_coefficient)>>8.
// The last source operand is a BP DWORD POINTER, dereferenced by the VM.
// DCPSnow advances each coordinate by its sampled velocity every tick and
// applies wind*coefficient>>8. It has no lifetime or automatic recycling.

// C0:2D (44FB40), firefly style source operands:
// bank,lifetime,lifetime_radius,x_radius,y,z_radius,
// vx,vx_radius,vy,vy_radius,vz,vz_radius,segment,segment_radius,
// wind_coefficient POINTER,fade_in,fade_out,blend.
// Positions/velocities/coefficient shift >>8; spatial/velocity radii are
// absolute. Fade periods zero become one. Each particle retains the bank,
// position, age, lifetime, old/new velocity, segment progress and two animation
// phases. C0:29 configures the screen-local 14-DWORD emitter bank:
// source[2..15] -> emitter[0..13]; arithmetic >>8 at 0,2,12;
// abs>>8 at 1,4,5,13; abs at 6,7,8; remaining slots preserve raw integers.
// Slots 9/10/11 gate lifetime scaling/velocity progress/angular reorientation.

// 44FFD0: old unsigned age is compared against lifetime, then velocity is
// interpolated with wrapping 32-bit multiplication and truncating division.
// At segment completion 450060 selects a new random or rotated velocity.
// 450250 optionally normalizes its magnitude with per-axis attenuation and
// optionally rescales lifetime. The port uses f64 for original x87 arithmetic.
// 44FF80: firefly alpha follows fade-in, then fade-out when remaining life is
// below its period; fade-out takes precedence. Both phases return inverse alpha.
static unsigned firefly_transparency(unsigned age,unsigned life,
                                    unsigned fade_in,unsigned fade_out) {
    unsigned remaining=life-age;
    if(remaining<fade_out)return 256-(remaining<<8)/fade_out;
    if(age<fade_in)return 256-(age<<8)/fade_in;
    return 0;
}

// C0:10 configures DCParticleEnvAir (44F6A0). Fields XYZ and radii are >>8;
// four timing inputs are divided by simulation interval as unsigned integers.
// It alternates a held wind vector and a linearly interpolated transition,
// choosing new endpoints through particle_rand. Wind is applied before each
// particle's own velocity. Pausing the environment outputs zero wind.

// 44EF30: subtract camera, rotate X then Y then Z (truncate after each axis),
// discard negative Z, sort far-to-near. 44F190 computes shrink index
//   32 - 32*focal/(focal+(z>>8))
// and chooses the first cumulative plane depth >= Z. Its centered screen point:
//   x=center_x+focal*x/(z+(focal<<8))-bitmap_width/2
//   y=center_y-focal*y/(z+(focal<<8))-bitmap_height/2
// Pixels then pass through the ordinary native bitmap compositor with the
// particle's blend/transparency. Portable composition shares the app backend;
// blend paths still marked Partial there are not promoted by this port.
