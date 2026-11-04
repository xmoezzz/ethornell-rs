/* BGI 1.622 immediate bitmap transforms, raw native_all.c/.asm.
 * Rust native_bitmap_transforms.rs; all parameters remain positional.
 * Each public wrapper validates every bitmap, blend/alpha/mode before core.
 * No scanning arguments for plausible bitmap handles. */
/* 90:19 dest,x,y,source,mask,mask_shift,transition (47A8A0 -> 4027E0).
 * Match source/destination format. Mask -1 or missing -> crossfade.
 * Valid mask: format3, same extent as source. Only RGB mask kernel acts;
 * mask keep uses 256-transition*((1<<shift)+1)+(gray<<shift), clamped,
 * then native signed seven-bit channel lerp. Work only on translated overlap.
 * Crossfade with RGB uses full eight-bit arithmetic, preserving pixel lane4. */
/* 90:1A dest,source,map1,map2_or_minus1,alpha,bilinear (47AA20->402970).
 * Maps use format4; first covers destination, second matches first extent.
 * Source/destination formats match; alpha<=256. Interpolate displacement
 * fields, then nearest/bilinear lookup through 412D60..4138D0. */
/* 90:1B dest,source,edge_mode,radius (47ABC0->402AC0).
 * Same format and extents, edge_mode0 black padding or1 edge clamp.
 * radius<=256; core clips 256 to255; 413EA0/414010 horizontal box kernels. */
/* 90:1C dest,dx,dy,dw,dh,source,sx,sy,sw,sh (47ACE0->402C90).
 * Rectangle dimensions>=2; format match. RGB/ARGB only. 414EA0 computes
 * source/destination scale with 0x10004 and uses seven-bit bilinear lanes.
 * Clip destination first; sampling center uses the clipped dest rectangle. */
/* 90:1D dest,source,scale,angle (47AE90->402EA0).
 * Matching formats; scale!=0. 414B70 is the same native inverse rotation
 * kernel used by CDspObjBackRTT; angle is signed 16.16 degrees. */
/* 92:1A temp_gray,x,y,primary,other_or_minus1,t (486560->403C20).
 * Allocate format3 at screen dimensions, clear outside translated overlap.
 * Sources must match; format2 extracts alpha. Secondary blends raw alpha
 * with 16-bit wrapping multiplication before shift8 (418E90); no secondary
 * copies it (418E20). Other source formats leave the allocated plane zero. */
