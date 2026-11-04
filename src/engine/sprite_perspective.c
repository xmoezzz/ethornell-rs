// BGI 1.622 Sprite mode 6, Graph90:5D.
// Evidence: native_all.c/.asm 47DA60 -> 43EF60 -> 4275A0 -> 428120;
// 429FD0,42A200,42A2B0,42A4F0,40F700,40EB10,40FB20,428C90.
// Reuses the C8 projection and scanline kernels (perspective_blit.rs).
// Source operands: object, fixed XYZ, primary, secondary, transition,
// transition_selector, integer anchor XY, pitch, heading, bank, axis_order,
// perspective_distance, project_position, stored_sampling_flag, blend,
// inverse_alpha, priority. Angles are 16.16 degrees; order%6 selects
// phb,pbh,hpb,hbp,bph,bhp. Native case 6 always bilinearly samples its spans.
// Optional secondary must match the primary's size/format. A mismatch returns
// status 9, which the public wrapper ignores after applying common state.

// 4275A0 clears coefficients at Sprite+712..748 on each full configuration.
// 428120 sets four scales to 65536, clears coupled-scaling mode, copies the
// two bitmap generation IDs, and forces transition=0/selector=-1 when there
// is no secondary. Property 0x10 rebuilds with a new primary and drops the
// secondary; properties and live fixed position are otherwise retained.
// 428940 properties: 40 anchor XY; 41 bank; 42 base scales; 43 pitch/heading;
// 80 anchor deltas; 81 bank/uniform-scale deltas; 82 XY scale deltas;
// 83 pitch/heading deltas; 8F angle curve. 41 and 42 rebuild immediately;
// other coefficient setters are consumed by subsequent fixed-parameter work.
// Fixed parameter high word updates transition only when selector==3.

// 429FD0 returns anchor XY (linear 8.24 progress), three angles (curve-shaped
// progress), and scales. There is no mode-5 perspective scale in this step:
// per-corner depth projection occurs later in 40F700.
static unsigned mul_fixed(unsigned a,unsigned b) {
    return (unsigned)(((unsigned long long)a*b)>>16);
}
static void mode6_scales(unsigned base_x,unsigned base_y,int coupled,
                        int dx,int dy,int uniform,unsigned *x,unsigned *y) {
    if(coupled) {
        *x=mul_fixed(base_x,65536+dx+uniform);
        *y=mul_fixed(base_y,65536+dy+uniform);
    } else {
        *x=mul_fixed(65536+dx,base_x+uniform);
        *y=mul_fixed(65536+dy,base_y+uniform);
    }
}

// 42A200: valid enabled graph center overrides screen half-width/half-height.
// With project_position, the center is the output origin and resolved XYZ
// (all three CDspObj fixed vector banks) is the 3D translation. Otherwise XY
// is added to the center after perspective; translation retains Z only.
// 40F700 rotates each corner in the selected order. Its perspective factor:
// depth<=0: (distance-depth)/distance; depth>0: distance/(distance+depth).
// 40EB10 snaps within 1/1024, retains homogeneous w/u*w/v*w, and builds the
// inclusive scanline bbox. The rasterized RGBA image is placed at its bbox
// origin; ordinary integer offset banks/global display offsets apply later.
// Primary/secondary generation checks happen before use; pixel revisions
// refresh the texture using the same fixed geometry. Optional auxiliary masks
// sample at final screen coordinates after this projection.

// 428C90 getter 0x10000000 always returns FIVE DWORDs, even for mode 6:
// anchor_x,anchor_y,BANK,scale_x,scale_y (pitch/heading are computed but skipped).
// Getter 41 returns the stored base bank; 10000100 returns source width/height
// and the last rebuilt raster width/height. The mode-5 getters cannot stand
// in for mode 6: they read different constructor records and rotation slots.
