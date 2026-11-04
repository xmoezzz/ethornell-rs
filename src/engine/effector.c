/* BGI 1.622 CDspObjEffector, 41FFC0..420D30, raw native_all.c/.asm.
 * Rust native_effector.rs + native_map_filter::refresh_native_filters.
 * Each object owns a full-screen scratch bitmap and enters manager class6.
 * Draw virtual4201B0 copies the already-rendered prefix into scratch, then
 * transforms it back. Only mode4 retains scratch across draws. */
/* 91:65 object,map1,map2_or_minus1,strength,bilinear,priority ->420430.
 * Format4 maps must each cover screen; their extents need not match each
 * other. Capture generations. Calls412AB0 with the current native alpha. */
/* 91:66 object,edge_mode,radius,priority ->420570. edge_mode0/1; radius
 * is native alpha, driving413D90 horizontal black-pad/edge-clamp blur. */
/* 91:67 object,vector_map,sample_count,wave_slot,amplitude,priority.
 * 4205C0 requires screen-sized format6 and a valid nonempty wave window.
 * Allocate sample_count*16 bytes;409F30 duplicates each amplitude-scaled
 * signed wave sample into four lanes.414300 applies that window to captured
 * prefix with flat-allocation clipping, optionally four-bit bilinear.
 * +72 SetAlpha regenerates the copied window. Property100 selects wave
 * offset and optionally alpha; validate before changing either. */
/* 91:68 object,source_anchor_x,source_anchor_y,angle,scale_x,scale_y,
 * bilinear,strength,priority ->420730. Scales are nonzero unsigned16.16;
 * anchors/angle signed16.16. Clears coefficients80/81/82 and curve8F,
 * resets fixed parameter.420BF0 derives:
 *   anchor = base + coefficient*unsigned_progress >>24
 *   angle = (base_angle + rotation_delta*curve(progress)>>16)*alpha >>8
 *   scale = 65536 + (unsigned_base + coefficient*progress>>24 -65536)
 *                      *unsigned_alpha >>8
 * Preserve full signed64 intermediates before truncating the result.
 * +72 alpha and +80 fixed-parameter setters rebuild derived fields;
 * assigning coefficients alone stores them without rebuilding. */
/* 4169D0 inverse affine setup uses destination even-rounded center:
 * theta=-angle*pi/11796480; sx=65536/unsigned_scale_x, sy likewise.
 * start_x=(-cx*cos(theta)-cy*sin(theta))*sx+anchor_x
 * start_y=anchor_y-(cy*cos(theta)-cx*sin(theta))*sy
 * dx=(cos(theta)*sx,-sin(theta)*sy)
 * dy=(-cos(pi/2-angle)*sx,sin(pi/2-angle)*sy)
 * Each component independently truncates to signed16.16. Accumulation wraps
 * DWORDs.418500 nearest;417ED0 four fractional bits and black padding. */
/* 91:69 object,retention,priority ->4207F0. Clear retained bitmap to zero.
 * On draw, blend retained pixels over prefix with mode1/transparency
 * 256-retention, then copy result back into retained bitmap. */
/* PropertyFF sets all three mode-specific bilinear flags89/108/120.
 * Losing a vector bitmap generation suppresses its effect while preserving
 * the prefix. Native nearest out-of-allocation loads can retain undefined
 * register contents; the portable wave path supplies zero there. */
