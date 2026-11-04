/* BGI 1.622: immutable native_all.c/.asm; Rust native_map_filter.rs.
 * CDspObjMap 423C50..4246C0, CDspObjFilter 420D40..421100.
 * Public source order, not CThread's reverse-pop order, is used below. */

/* 90:76 map,columns,rows,cell_width,cell_height. Tile dimensions 1..256;
 * columns<=1024/cell_width, rows<=768/cell_height, all nonzero.
 * Clears atlas and viewport; retains a previously uploaded source map.
 * Owns a private bitmap and two u16 grids of (columns+1)*(rows+1).
 * Current grid starts FFFF, desired starts 0000. */
/* 90:78 map,width,height,BP_pointer -> 424010. Copies width*height*2
 * bytes; disables the viewport and invalidates the current grid. The VM
 * translates descriptors and reads u16s before passing the table to host. */
/* 90:75 map,x,y,atlas,blend,alpha,priority -> 423E20 -> 424470.
 * Requires an initialized grid and at least one whole tile in each axis.
 * Atlas descriptors are row-major views into the live bitmap; generation
 * is retained. Allocates private cache in atlas format, clears old atlas,
 * but does NOT invalidate current tile IDs. */
/* 90:79 map,source_x,source_y,pixel_offset_x,pixel_offset_y,wrap.
 * Source coordinates must be in uploaded map and pixel offsets in a tile.
 * If source cell changed, build the extra-row/column desired grid: wrap
 * each source axis or fill out-of-range cells FFFF. Changing wrap alone
 * does not rebuild it. Visible source rectangle begins at pixel_offset
 * and has columns*cell_width by rows*cell_height pixels. */
/* 424320 redraws only cells whose current ID differs from desired.
 * FFFF/out-of-atlas clears cell (mode41); valid cells raw-copy (mode80).
 * Losing the atlas generation clears the private bitmap only on a redraw.
 * Therefore atlas pixel edits require explicit 7A invalidation to affect
 * cached cells, exactly as in the native retained bitmap. */
static void map_invalidate_id(Map *m,int id) { /* 90:7A -> 424240 */
    unsigned cursor=0;
    for(unsigned i=0;i<m->cache_cells;++i) {
        if(m->current[i]==id) {
            m->current[i]=~m->desired[cursor++];
        }
        ++cursor; /* additional advance on matches is visible in assembly */
    }
    m->dirty=1;
}
/* Native can overread desired here. Portable overread supplies zero. */

/* 90:65 filter,color,alpha,priority selects mode0, no mask.
 * 90:66 filter,mode,color,mask,mask_shift,alpha,priority.
 * The second argument of 66 is NOT reserved/ignored. Mask -1 disables;
 * otherwise requires format3 with screen extent and retains generation.
 * Configured objects enter CObjectManager at sort class7. Their draw
 * virtual changes the destination already drawn at that point. */
static int filter_channel(int d,int color,int strength,int mode,int luma) {
    switch(mode) {
    case 0:return (d*(256-strength)+color*strength)>>8;
    case 1:return clamp(d+((color*strength)>>8),0,255);
    case 2:return clamp(d+((((luma*color)>>16)-d)*(strength>>1)>>7),0,255);
    case 3:return clamp(d+(((d^color)-d)*(strength>>1)>>7),0,255);
    default:return d;
    }
}
/* Mode1's all-zero color selects saturating subtraction of white*strength.
 * Luma is R*77+G*150+B*29 (4E4220, raw filter_tables.json).
 * With mask, mode is ignored. keep=256-strength*((1<<(shift&31))+1)
 * +(mask<<(shift&31)); keep>=256 leaves pixels; keep<=0 copies color;
 * otherwise color+((destination-color)*(keep>>1)>>7), 411B50.
 * Rasterization rebuilds the chain prefix before each filter; later filters
 * consume earlier results. Object alpha/mask-alpha/multiplier are applied
 * in the native kernel once, not again to the synthesized copy layer. */
