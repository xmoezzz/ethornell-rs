/* BGI1.622 CDspObjLandscape421AB0..423C50, raw native_all.c/.asm.
 * 91:78 copies five-DWORD part rectangles into PRIVATE images. Each34-DWORD
 * column contains count1..32,32 part IDs (each -1 or valid), boundary1..32.
 * Column W=max(partW), H=max(partH+level*column_step); alpha-over each part
 * centered at(H-level*column_step-partH). A boundary before the last level
 * copies current alpha into a private hit bitmap. A flag0 part contributes
 * occluding height until missing/flagged/boundary; boundary adds part_spacing.
 * OffsetX=(cell_width-W)>>1; occluding height=offsetX>0?0:count*column_step.
 * Extra depth=max(1,H/column_step-boundary-2) if a hit snapshot was created.
 * 91:7A guide rectangles are copied as well; source release is irrelevant.
 * 91:7C copies a private part and rebuilds every column containing it. */
/* 91:79 creates rows*columns cells. For row r,column c, front-row neighbors
 * are(r+1,c+(r odd?0:-1)),(r+1,c+(r odd?1:0)), and(r+2,c).
 * Crop from bottom by min positive neighbor occluding heights minus row step
 * (two row steps for third neighbor), requiring both first neighbors>0.
 * Position: x=c*cell_width+odd_row*half_width+column_offsetX;
 * y=baseline+r*row_step-column_image_height.
 * Packed key=(object_key&FFFFE01F)+32*(r+((r*row_priority+
 * frame_priority*(extra_depth+boundary-1))<<11)). Invisible height=>key0.
 * 7E returns high16 of key. Replacing a cell7D rebuilds itself and three
 * cells behind it, resetting their guide/shade records just like4237D0.
 * Manager registers all distinct packed keys; other object classes can draw
 * between terrain cells.91:75 priority validates whole row-priority range. */
/* 422E70: at each registered key, draw row-major matching cells with the
 * base native blend/alpha. Silhouette type1 alpha-blends a flat RGB silhouette
 * preserving original alpha; type2 adds it. Four copied guides draw after
 * cell, centered horizontally at boundary height; guide transparency combines
 * with base alpha using256-((256-base)*(256-guide)>>8).
 * 422C50 hit-test scans candidate rows back-to-front, staggered candidate
 * column, visible cell bounds, then actual RGBA alpha or saved boundary mask.
 * Output is(column,row). Native RGB does not accept a hit.7F recreates the
 * destination and copies the boundary part, not a live source-atlas rectangle.
 * Native malformed/undefined reads are zero/bounds failures in the port. */
