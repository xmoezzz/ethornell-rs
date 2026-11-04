/* BGI1.622 BackML, raw native_all.c/.asm41DE30..41E870.
 * Rust native_backml.rs;91:40..4A configure eight84-byte private records.
 * The last argument of40/47 is bilinear sampling, not transparency.
 * Source/destination anchors and velocity coefficients are signed16.16.
 * Each record: configured,enabled,destXY,blend,strength,bitmap,generation,
 * sourceXY,rotation,scaleXY,rotation_curve,scale_curve,source_deltaXY,
 * rotation_delta,scale_deltaXY,bilinear. */
/* 40 reuses an existing BackML and configures only record0. It does not
 * destroy the other seven records.41 selects the record used by virtual
 * position/alpha/property setters.43 sets a particular private destination;
 * base SetPosition/SetFixedPosition also writes selected destination only.
 * +72 SetAlpha and +76 GetAlpha address selected strength, not base alpha.
 * 46 captures bitmap generation and resets source deltas;47 resets rotation
 * and scale deltas.48 supplies TWO curve selectors.49/4A coefficients are
 * consumed at draw time, not timers measured in milliseconds. */
static int interpolated_scale(int base,int delta,int curve_fixed16) {
    double reciprocal=65536.0/base;
    double target=65536.0/(base+delta);
    return trunc(65536.0/(reciprocal+(target-reciprocal)*curve_fixed16/65536.0));
}
/* 41E4B0 processes0..7 with generation checks. Source positions advance
 * coefficient*unsigned8.24progress>>24. Rotation uses first curve; scales
 * use reciprocal interpolation through second curve.4169D0 prepares affine
 * coordinates, nearest or four-bit bilinear. Record0 replaces screen, fading
 * RGB with its strength and ignoring its blend selector. Later modes0/1/20
 * use416B60 alpha composition (integer identity has clipped-copy fast path).
 * Other later modes render a temporary and call the exact bitmap blitter.
 * The final eight-layer result enters the global chain as ONE background;
 * private layer indices are not global render priorities. */
/* Native uninitialized temporary/border pixels are zero in the portable
 * allocation. CPU font/GPU-independent rendering is shared with Effectors. */
