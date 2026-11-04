// 0xD0:00..2D — strategy-game grid field (DCTELgclGrdFldMngr / DCTELgclGrdFld).
// Cleaned from src/raw/bgi1622/d0.{c,asm} (BGI 1.622). Every register argument
// below was taken from the disassembly; Hex-Rays mixes them up for the
// __usercall helpers. See src/adr/0002-d0-logic-extension.md.
//
// Coordinates are tile indices; "height" is the tile's integer height; angles
// are 16.16 degrees (0x5A0000 = 90°). Statuses are the internal 0x8000xxxx
// codes; the script sees them through status_to_script() (sub_497120).
#include "../include/engine_types.h"

// ---------------------------------------------------------------------------
// Script-visible status (sub_497120).
int status_to_script(uint32_t s)
{
    switch (s) {
    case 0:          return 0;
    case 0x80000000: return 1;  case 0x80000001: return 2;
    case 0x80000002: return 3;  case 0x80000003: return 4;
    case 0x80000004: return 5;  case 0x80000005: return 6;
    case 0x80000006: return 7;  case 0x80000007: return 8;
    case 0x80000008: return 9;  case 0x80000009: return 10;
    case 0x8000000A: return 11; case 0x8000000B: return 12;
    case 0x90000002: return 16; case 0x90000003: return 17;
    case 0xA0000000: return 1;  case 0xA0000001: return 18;
    case 0xA0000002: return 19; case 0xA0000003: return 20;
    case 0xA0000004: return 8;  case 0xA0000005: return 21;
    case 0xA0000006: return 22;
    case 0xFFFFFFFF: return -1; case 0xFFFFFFFE: return -2;
    default:         return -1;
    }
}

// ---------------------------------------------------------------------------
// Data.

struct Tile {            // 16 bytes, the script's map array (D0:04)
    int32_t  height;     // +0
    int32_t  cost;       // +4  extra move cost; the active cost table is added in
    uint32_t flags;      // +8  1 occupied (set per search), 2 wall, 4 no-target, 8 no-entry
    uint32_t extra;      // +C  bits 28..31: height above `height` (a roof/edge);
                         //     low byte: zone-of-control team bits (set per search)
};
#define TILE_TOP(t) ((t)->height + (int32_t)((t)->extra >> 28))

struct CostTable {       // 0x0C, list at mngr+0x1C (newest first)
    int32_t id;          // from mngr->next_table_id (starts 0x10000000)
    int32_t *cost;       // w*h ints, added to Tile.cost
    struct CostTable *next;
};

struct Unit {            // 0x34, list at mngr+0x50 (newest first)
    int32_t id;          // +00 from mngr->next_unit_id (starts 0x20000000)
    int32_t x, y;        // +04 +08 (-1 until D0:14)
    int32_t dir;         // +0C 0 or 2..5 (D0:18)
    int32_t height;      // +10 3   body height (line of fire)
    int32_t radius;      // +14 128 hit radius against the sub-tile distance table
    uint32_t move_flags; // +18 bit0 flies, bit1 ignores zone of control, bit2 flat cost
    uint32_t team;       // +1C low byte: zone-of-control bits this unit projects
    int32_t table_id;    // +20 cost table (0 = none)
    int32_t zoc_climb;   // +24 1   max height difference its zone of control reaches
    struct GrdFld *field;// +28 last movement search (D0:20)
    void   *cache;       // +2C list freed by sub_454320; only the evaluator fills it
    struct Unit *next;   // +30
};

struct GrdFldMngr {      // 0x498
    void   *vtbl;
    int32_t scale;       // +04 height units per tile (D0:00 arg 2); distance divides by it
    int32_t w, h;        // +08 +0C
    struct Tile *tiles;  // +10
    int32_t next_table_id;   // +14
    int32_t pad18;
    struct CostTable *tables;// +1C
    int32_t next_unit_id;    // +20 ... +50 cleared by the base ctor
    int32_t pad24[11];
    struct Unit *units;      // +50
    int32_t corner_rect[4][4]; // +54 sub-tile corner squares, 16-bit fractions:
                               // {0,0,0x2000,0x2000} {0xE000,0,0x10000,0x2000}
                               // {0,0xE000,0x2000,0x10000} {0xE000,0xE000,0x10000,0x10000}
    int32_t center_dist[16][16]; // +94 trunc(sqrt(dx²+dy²))*32, dx/dy = 8..1,1..8
    int32_t (*corner)[4];        // +494 per tile: height of each inside corner (sub_455220)
};

struct Cell {            // 24 bytes, DCTELogicalField search result per tile
    int32_t visited;     // +0 touched (also set on walls)
    int32_t back_dir;    // +4 direction back to the parent (1 = start)
    int32_t steps;       // +8
    int32_t cost;        // +C
    int32_t climb;       // +10 sum of height differences
    int32_t turns;       // +14
};

struct Node {            // 0x28, FIFO queue
    int32_t x, y, back_dir, height, steps, cost, climb, turns, remain;
    struct Node *next;
};

struct GrdFld {          // 0x48
    void   *vtbl;
    int32_t borrowed;    // +04 tiles not owned (always 0 here)
    int32_t inv_scale;   // +08 0x10000 / max(scale, 1) (unsigned)
    int32_t w, h;        // +0C +10
    struct Tile *tiles;  // +14 private copy with costs, occupancy and ZOC applied
    struct Cell *cells;  // +18
    struct Node  sentinel_; // +1C..+3F: only its `next` (+40) is used
    struct Node *head;   // +40 (== sentinel_.next)
    struct Node *tail;   // +44 starts at &sentinel_
};

// Directions: 2 = y-1, 3 = y+1, 4 = x-1, 5 = x+1, 1 = start, 0 = none.

// ---------------------------------------------------------------------------
// Angles.

// sub_401000: angle of (x, y) in 16.16 degrees, y up. The engine caches
// x,y in -10..10 in a 21x21 table at startup (sub_454B10); sub_454B90 reads
// the table or calls this, so both give exactly this function.
int32_t angle_of(int32_t x, int32_t y)
{
    if (x == 0) {
        if (y == 0) return 0;
        return y > 0 ? 0x5A0000 : 0x10E0000;
    }
    double r = (double)y / (double)x;
    if (x > 0) {
        if (y >= 0) return (int32_t)(atan(r) * 11796480.0 / M_PI);
        return 0x1680000 - (int32_t)(atan(r) * -11796480.0 / M_PI);
    }
    return 0xB40000 - (int32_t)(atan(r) * -11796480.0 / M_PI);
}

// ---------------------------------------------------------------------------
// Manager construction (sub_4531E0 base, sub_454940).

void mngr_init(struct GrdFldMngr *m, int32_t scale)
{
    memset(m, 0, sizeof *m);
    m->scale = scale;
    m->next_table_id = 0x10000000;
    m->next_unit_id  = 0x20000000;
    static const int32_t rect[4][4] = {
        {0, 0, 0x2000, 0x2000}, {0xE000, 0, 0x10000, 0x2000},
        {0, 0xE000, 0x2000, 0x10000}, {0xE000, 0xE000, 0x10000, 0x10000}};
    memcpy(m->corner_rect, rect, sizeof rect);
    for (int r = 0; r < 16; r++)
        for (int c = 0; c < 16; c++) {
            int dy = r < 8 ? 8 - r : r - 7, dx = c < 8 ? 8 - c : c - 7;
            m->center_dist[r][c] = ((int32_t)sqrt((double)(dx * dx + dy * dy)) << 8) / 8;
        }
}

// sub_4542C0: drop units, cost tables and the map.
void mngr_clear(struct GrdFldMngr *m)
{
    while (m->units) mngr_remove_unit(m, m->units->id);
    free_tables(m->tables); m->tables = 0;
    free(m->tiles); m->tiles = 0;
}

// D0:04 sub_453480(eax=w, ecx=m, w, h, src)
uint32_t mngr_set_map(struct GrdFldMngr *m, int32_t w, int32_t h, const struct Tile *src)
{
    if (!w || !h) return 0x80000001;
    mngr_clear(m);
    free(m->corner); m->corner = 0;            // vtbl+30 sub_455340
    m->w = w; m->h = h;
    m->tiles = memdup(src, 16 * w * h);
    mngr_build_corners(m);                     // vtbl+2C sub_455220
    return 0;
}

// vtbl+2C sub_455220: an inside corner is a vertex where tile (x,y) is lower
// than both its upper neighbour (x,y-1) and its right neighbour (x+1,y); the
// four tiles around the vertex get min(top(x,y-1), top(x+1,y)) on that corner.
uint32_t mngr_build_corners(struct GrdFldMngr *m)
{
    if (!m->tiles) return 0x80000002;
    int32_t (*c)[4] = calloc(m->w * m->h, 16);
    for (int y = 1; y < m->h; y++)
        for (int x = 0; x < m->w - 1; x++) {
            struct Tile *up = &m->tiles[(y - 1) * m->w + x], *here = &m->tiles[y * m->w + x];
            int32_t a = TILE_TOP(up), b = TILE_TOP(here + 1);
            if (here->height < a && here->height < b) {
                int32_t v = a < b ? a : b;
                c[(y - 1) * m->w + x][3] = v;      // above, bottom-right
                c[(y - 1) * m->w + x + 1][2] = v;  // above-right, bottom-left
                c[y * m->w + x][1] = v;            // here, top-right
                c[y * m->w + x + 1][0] = v;        // right, top-left
            }
        }
    m->corner = c;
    return 0;
}

// D0:05 sub_453530(esi=m, out, src)
uint32_t mngr_add_table(struct GrdFldMngr *m, int32_t *out_id, const int32_t *src)
{
    if (!m->tiles) return 0x80000002;
    struct CostTable *t = new_table(m->next_table_id, memdup(src, 4 * m->w * m->h));
    t->next = m->tables; m->tables = t;
    m->next_table_id++;
    *out_id = t->id;
    return 0;
}

// D0:10 sub_4535B0(edi=m, out)
uint32_t mngr_add_unit(struct GrdFldMngr *m, int32_t *out_id)
{
    if (!m->tiles) return 0x80000002;
    struct Unit *u = calloc(1, sizeof *u);
    *u = (struct Unit){ .id = m->next_unit_id, .x = -1, .y = -1, .height = 3,
                        .radius = 128, .zoc_climb = 1, .next = m->units };
    m->next_unit_id++;
    m->units = u;
    *out_id = u->id;
    return 0;
}

// D0:11 sub_453630(ecx=id, edx=m): also frees unit->cache (sub_454320).
// D0:12 sub_453680: frees unit->field only.
// D0:14 sub_4536B0(edx=m, ecx=id, edi=x, esi=y): 0x80000004 unless 0<=x<w, 0<=y<h.
// D0:15 sub_453790: height < 0 -> 0x80000009, radius < 0 -> 0x8000000A.
// D0:16 sub_4537D0: move_flags = p[0], team = p[1].
// D0:17 sub_453800: the table must exist (0x80000005), else unit->table_id = id.
// D0:18 sub_453720: vtbl+18 sub_454F50 accepts 0 and 2..5, else 0x8000000B.
// Unknown unit: 0x80000003 everywhere.

// ---------------------------------------------------------------------------
// Movement search (D0:20).

// vtbl+1C sub_454F80: the unit's tile and its four neighbours whose height
// differs by at most u->zoc_climb get u->team's low byte ORed into extra.
void project_zoc(struct GrdFldMngr *m, struct Tile *t, const struct Unit *u)
{
    int32_t h0 = t[u->y * m->w + u->x].height;
    static const int d[5][2] = {{0,0},{0,-1},{0,1},{-1,0},{1,0}};
    for (int i = 0; i < 5; i++) {
        int x = u->x + d[i][0], y = u->y + d[i][1];
        if (x < 0 || x >= m->w || y < 0 || y >= m->h) continue;
        struct Tile *n = &t[y * m->w + x];
        if (abs(h0 - n->height) <= u->zoc_climb) n->extra |= (uint8_t)u->team;
    }
}

// sub_454390(esi=m, field, table_id, mark_units): the field's private map.
uint32_t prepare_field(struct GrdFldMngr *m, struct GrdFld *f, int32_t table_id, int mark_units)
{
    if (!m->tiles) return 0x80000002;
    struct Tile *t = memdup(m->tiles, 16 * m->w * m->h);
    const int32_t *cost = find_table(m, table_id);   // sub_454370, null if absent
    if (cost) for (int i = 0; i < m->w * m->h; i++) t[i].cost += cost[i];
    if (mark_units)
        for (struct Unit *u = m->units; u; u = u->next) {
            t[u->y * m->w + u->x].flags |= 1;   // target: unchecked index (see port)
            project_zoc(m, t, u);
        }
    field_set_map(f, m->w, m->h, t);             // sub_457AC0: -3 if borrowed, copies
    free(t);
    return 0;
}

// sub_458120(edi=x, esi=f, y, back_dir, height, steps, cost, climb, turns, remain)
uint32_t field_push(struct GrdFld *f, int32_t x, int32_t y, int32_t back_dir, int32_t height,
                    int32_t steps, int32_t cost, int32_t climb, int32_t turns, int32_t remain)
{
    if (x < 0 || x >= f->w || y < 0 || y >= f->h) return 0x80000005;
    struct Cell *c = &f->cells[y * f->w + x];
    // Prune: the next step costs at least 1.
    if (c->visited && !(cost + 1 < c->cost || (cost + 1 == c->cost && climb < c->climb)))
        return 0x80000006;
    struct Node *n = malloc(sizeof *n);
    *n = (struct Node){x, y, back_dir, height, steps, cost, climb, turns, remain, 0};
    f->tail->next = n; f->tail = n;
    return 0;
}

// lexicographic (cost, climb, turns) < cell
static int better(int32_t cost, int32_t climb, int32_t turns, const struct Cell *c)
{
    if (cost != c->cost) return cost < c->cost;
    if (climb != c->climb) return climb < c->climb;
    return turns < c->turns;
}

// sub_458210(esi=n, f, jump, flies, flat_cost, zoc_mask): relax one node.
// Returns 1 when the cell took the node's values.
int field_relax(struct GrdFld *f, const struct Node *n, int32_t jump, int flies,
                int flat_cost, uint32_t zoc_mask)
{
    struct Cell *c = &f->cells[n->y * f->w + n->x];
    if (c->visited && !better(n->cost + 1, n->climb, n->turns, c)) return 0;
    struct Tile *t = &f->tiles[n->y * f->w + n->x];
    int start = n->back_dir == 1;
    if ((t->flags & 2) || (!flies && ((t->flags & 0xC) || (!start && (t->flags & 1))))) {
        c->visited = 1;
        return 0;
    }
    int32_t top = TILE_TOP(t);
    int32_t from = start ? top : n->height;
    int32_t stand = (flies && from >= top) ? from : top;
    int32_t diff = abs(stand - from);
    if (jump < diff && !start) return 0;
    int32_t step = 0;
    if (!start) {
        step = t->cost + 1;
        if (!flat_cost) {
            if (flies) { if (diff >= 4) step += diff - 3; }
            else       { if (diff >= 2) step += diff - 1; }
        }
    }
    int32_t cost = n->cost + step, climb = n->climb + diff;
    if (c->visited && !better(cost, climb, n->turns, c)) return 0;
    int32_t remain = n->remain - step;
    if (remain < 0) return 0;
    *c = (struct Cell){1, n->back_dir, n->steps, cost, climb, n->turns};
    if ((remain >= 1 && !(zoc_mask & t->extra)) || start)
        grdfld_expand(f, n->x, n->y, stand, n->steps, cost, climb, n->turns, remain); // vtbl+8
    return 1;
}

// DCTELgclGrdFld vtbl+8 sub_454770: queue the four neighbours except the one
// we came from, in the order y-1, y+1, x-1, x+1. A move counts as a turn when
// its back direction differs from this cell's.
void grdfld_expand(struct GrdFld *f, int32_t x, int32_t y, int32_t height, int32_t steps,
                   int32_t cost, int32_t climb, int32_t turns, int32_t remain)
{
    int32_t back = f->cells[y * f->w + x].back_dir;  // sub_4580B0 copy
    if (back != 2) field_push(f, x, y - 1, 3, height, steps + 1, cost, climb, turns + (back != 3), remain);
    if (back != 3) field_push(f, x, y + 1, 2, height, steps + 1, cost, climb, turns + (back != 2), remain);
    if (back != 4) field_push(f, x - 1, y, 5, height, steps + 1, cost, climb, turns + (back != 5), remain);
    if (back != 5) field_push(f, x + 1, y, 4, height, steps + 1, cost, climb, turns + (back != 4), remain);
}

// sub_457B40(eax=&u->move_flags, f, x, y, jump, move, dest_x, dest_y)
uint32_t field_search(struct GrdFld *f, const struct Unit *u, int32_t jump, int32_t move,
                      int32_t dest_x, int32_t dest_y)
{
    if (!f->tiles) return 0x80000002;                // sub_458050
    if (!f->cells) f->cells = malloc(24 * f->w * f->h);
    memset(f->cells, 0, 24 * f->w * f->h);
    int flies = u->move_flags & 1, flat = u->move_flags & 4;
    uint32_t mask = (u->move_flags & 2) ? 0 : (~u->team & 0xFF);
    field_free_queue(f);                             // sub_4581E0
    field_push(f, u->x, u->y, 1, 0, 0, 0, 0, 0, move < 1 ? 0x7FFFFFFF : move);
    for (struct Node *n = f->head; n; n = n->next)
        if (field_relax(f, n, jump, flies, flat, mask) && n->x == dest_x && n->y == dest_y)
            break;
    field_free_queue(f);
    return 0;
}

// D0:20 sub_453830(eax=m, ecx=unit, jump, move, dest_x, dest_y, mark_units=1)
uint32_t mngr_search(struct GrdFldMngr *m, int32_t id, int32_t jump, int32_t move,
                     int32_t dest_x, int32_t dest_y)
{
    struct Unit *u = find_unit(m, id);
    if (!u) return 0x80000003;
    delete_field(u->field);
    u->field = new_grdfld(m->scale);                 // vtbl+24 sub_455180
    prepare_field(m, u->field, u->table_id, 1);
    return field_search(u->field, u, jump, move, dest_x, dest_y) ? 0xFFFFFFFF : 0;
}

// ---------------------------------------------------------------------------
// Reading a search back.

// D0:21 sub_453930 -> sub_457C10(eax=y, edx=x, esi=f, dirs, count):
// *count = steps to (x,y); dirs[0..steps) = forward directions from the unit.
uint32_t field_path(struct GrdFld *f, int32_t x, int32_t y, int32_t *dirs, int32_t *count)
{
    if (!f->cells) return 0x80000003;
    if (x < 0 || x >= f->w || y < 0 || y >= f->h) return 0x80000005;
    struct Cell *c = &f->cells[y * f->w + x];
    if (!c->back_dir) return 0x80000004;
    int32_t n = c->steps;
    *count = n;
    if (n >= 1 && dirs)
        while (n) {
            c = &f->cells[y * f->w + x];
            dirs[--n] = reverse_dir(c->back_dir, 0);       // vtbl+10 sub_4548E0
            if (!step_dir(&x, &y, c->back_dir)) return -2;  // vtbl+C sub_454880
        }
    return 0;
}
// sub_453930 maps: 0x80000003 -> 0x80000006, 0x80000004/5 -> 0x80000007,
// -2 -> -2, other nonzero -> -1. No unit 0x80000003, no field 0x80000006.

// D0:22 sub_453B30 -> sub_457E40: memcpy(buf, f->cells, 24*w*h).
// D0:23 sub_453B80(..., sorted=0) -> sub_457E80(eax=f, xy, count, sorted):
// every tile with cell.cost > 0 and !(tile.flags & 0xD), row-major. With
// `sorted` (evaluator only) the list is qsort()ed by cost (MSVC qsort).

// reverse_dir sub_4548E0(dir, rel): 2<->3, 4<->5, others unchanged; `rel`
// shifts 0..3 into 2..5 and back. step_dir sub_454880/sub_4548A0: 2 y--,
// 3 y++, 4 x--, 5 x++, else returns 0.

// ---------------------------------------------------------------------------
// Targets (D0:28) — vtbl+4 sub_454BC0.

// vtbl+28 sub_4551F0 -> sub_454580: distance with heights scaled.
double tile_distance(const struct GrdFldMngr *m, int32_t x0, int32_t y0, int32_t h0,
                     int32_t x1, int32_t y1, int32_t h1)
{
    double dh = (double)(h0 - h1) / (double)m->scale;
    return sqrt((double)((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1)) + dh * dh);
}

// (out_xy, out_dist, out_count, x, y, range, height_bonus, arc, units_only,
//  ignore_units, exclude_self=1)
uint32_t mngr_targets(struct GrdFldMngr *m, int32_t *xy, int32_t *dist, int32_t *count,
                      int32_t x, int32_t y, int32_t range, int32_t bonus, int32_t arc,
                      int units_only, int ignore_units, int exclude_self)
{
    if (!m->tiles) return 0x80000002;
    int32_t h0 = m->tiles[y * m->w + x].height;
    // Reach per target height: higher ground in front shortens it.
    int32_t step = (range * bonus) << 8, v = (range << 16) + step * h0, thr[32];
    for (int i = 0; i < 32; i++, v -= step) thr[i] = v < 0 ? 0 : v + 0x4000;
    int32_t n = 0;
    for (int ty = 0; ty < m->h; ty++)
        for (int tx = 0; tx < m->w; tx++) {
            int32_t th = m->tiles[ty * m->w + tx].height;
            int32_t d = (int32_t)(tile_distance(m, tx, ty, th, x, y, h0) * 65536.0);
            if (d >= thr[th]) continue;             // th outside 0..31 reads past thr
            if (units_only && !unit_at(m, tx, ty)) continue;
            int ok = 0;
            mngr_line_of_fire(m, &ok, tx, ty, x, y, arc, ignore_units);   // vtbl+34
            if (!ok && (exclude_self || tx != x || ty != y)) continue;
            xy[2 * n] = tx; xy[2 * n + 1] = ty;
            if (dist) dist[n] = d;
            n++;
        }
    *count = n;
    return 0;
}

// vtbl+34 sub_455360: can a shot from (x1,y1) reach (x0,y0)? A CSpline runs
// from the shooter's chest to the target's: z = (tile.height << 16) + (unit
// height or 3) << 15, with a raised midpoint when arc >= 1. It is sampled at
// t = 0..steps, steps = trunc(distance * 8):
//   wall (flags & 2) or terrain top above z           -> blocked
//   an inside corner above z inside that corner square -> blocked
//   unless ignore_units, a unit on a tile other than the shooter's whose
//   top (tile height + unit height) is above z and whose radius reaches the
//   sample's sub-tile: the target's own tile -> hit (1), else blocked.
// Reaching the end: ok = !(target tile flags & 4).
uint32_t mngr_line_of_fire(struct GrdFldMngr *m, int *ok, int32_t x0, int32_t y0,
                           int32_t x1, int32_t y1, int32_t arc, int ignore_units)
{
    if (!m->tiles) return 0x80000002;
    if (x0 < 0 || x0 >= m->w || y0 < 0 || y0 >= m->h || (x1 == x0 && y1 == y0))
        return 0x80000004;
    if (x1 < 0 || x1 >= m->w || y1 < 0 || y1 >= m->h) return 0x80000008;
    struct CSpline *s = cspline_new();
    struct Tile *to = &m->tiles[y0 * m->w + x0], *from = &m->tiles[y1 * m->w + x1];
    double dist = tile_distance(m, x0, y0, to->height, x1, y1, from->height);
    int64_t steps = (int64_t)(dist * 8.0);                  // fistp, truncating
    struct Unit *u1 = unit_at(m, x1, y1), *u0 = unit_at(m, x0, y0);
    int32_t lift1 = (u1 ? u1->height : 3) << 15, lift0 = (u0 ? u0->height : 3) << 15;
    cspline_reset(s);
    cspline_add(s, (x1 << 16) + 0x8000, (y1 << 16) + 0x8000, (from->height << 16) + lift1);
    if (arc >= 1)
        cspline_add(s, (x0 + x1 + 1) << 15, (y0 + y1 + 1) << 15,
                    ((from->height + to->height) << 15)
                    - (int32_t)((double)arc * dist * -0.00390625 * 65536.0)
                    + ((lift0 + lift1) >> 1));
    cspline_add(s, (x0 << 16) + 0x8000, (y0 << 16) + 0x8000, (to->height << 16) + lift0);
    cspline_set_duration(s, (int32_t)steps);              // start 0
    int result = 1, hit = 0;
    for (uint32_t t = 0; t <= (uint32_t)steps; t++) {
        int32_t p[3] = {0, 0, 0};
        if (!cspline_sample(s, t, p)) continue;
        int32_t tx = p[0] >> 16, ty = p[1] >> 16, z = p[2] >> 16;
        struct Tile *k = &m->tiles[ty * m->w + tx];
        if ((k->flags & 2) || TILE_TOP(k) > z) { result = 0; goto done; }
        uint32_t fx = p[0] & 0xFFFF, fy = p[1] & 0xFFFF;
        for (int c = 0; c < 4; c++)
            if (m->corner[ty * m->w + tx][c] > z
                && m->corner_rect[c][0] <= (int)fx && (int)fx < m->corner_rect[c][2]
                && m->corner_rect[c][1] <= (int)fy && (int)fy < m->corner_rect[c][3]) {
                result = 0; goto done;
            }
        if (!ignore_units && (tx != x1 || ty != y1)) {
            struct Unit *u = unit_at(m, tx, ty);
            if (u && u->height + k->height > z
                && u->radius >= m->center_dist[fy >> 12][fx >> 12]) {
                result = tx == x0 && ty == y0; hit = 1; goto done;
            }
        }
    }
    result = !(to->flags & 4);
done:
    (void)hit;
    *ok = result;
    cspline_delete(s);
    return 0;
}

// ---------------------------------------------------------------------------
// Directions (D0:2C, D0:2D).

// vtbl+38 sub_4557B0(out, tx, ty, ux, uy, dir): how far (16.16 degrees,
// 0..180) the target is from the way `dir` faces. dir 0 -> 0.
uint32_t facing_delta(int32_t *out, int32_t tx, int32_t ty, int32_t ux, int32_t uy, int32_t dir)
{
    if (!dir) { *out = 0; return 0; }
    int32_t base = 0; uint32_t err = 0;
    switch (dir) {
    case 2: base = 0x5A0000; break;  case 3: base = 0x10E0000; break;
    case 4: base = 0xB40000; break;  case 5: base = 0; break;
    default: err = 0x8000000B;
    }
    int32_t d = abs(angle_of(tx - ux, uy - ty) - base);
    if (d > 0xB40000) d = 0x1680000 - d;
    if (!err) *out = d;
    return err;
}

// vtbl+C sub_454DE0(tx, ty, fx, fy): the direction from (fx,fy) toward
// (tx,ty) by 90° sectors: [45,135) -> 2, [225,315) -> 3, [135,225) -> 4, else 5.
int32_t direction_to(int32_t tx, int32_t ty, int32_t fx, int32_t fy)
{
    uint32_t a = (uint32_t)(angle_of(tx - fx, fy - ty) >> 16);
    if (a - 45 <= 89) return 2;
    if (a - 225 <= 89) return 3;
    return (a - 135 > 89) + 4;
}

// ---------------------------------------------------------------------------
// Handlers. Script arguments a0..an-1 in call order (the handler pops an-1
// first). "raw" results are pushed as-is, everything else via status_to_script.
//
//   D0:00 (out*, kind, scale)       kind must be 0; creates; raw 1 / 0
//   D0:01 (h)                       release; raw 1 found / 0
//   D0:04 (h, w, hgt, tiles*)       mngr_set_map
//   D0:05 (out*, h, costs*)         mngr_add_table
//   D0:10 (out*, h)                 mngr_add_unit
//   D0:11 (h, unit)                 remove unit
//   D0:12 (h, unit)                 drop unit's search
//   D0:14 (h, unit, x, y)           position
//   D0:15 (h, unit, height, radius)
//   D0:16 (h, unit, pair*)          {move_flags, team}
//   D0:17 (h, unit, table)
//   D0:18 (h, unit, dir)
//   D0:20 (h, unit, jump, move, dest_x, dest_y)   mngr_search
//   D0:21 (dirs*, count*, h, unit, x, y)          field_path
//   D0:22 (cells*, h, unit)                       24 bytes per tile
//   D0:23 (xy*, count*, h, unit)                  reachable tiles, unsorted
//   D0:28 (xy*, dist*, count*, h, x, y, range, bonus, arc, units_only, ignore_units)
//   D0:2C (out*, tx, ty, h, unit)                 facing_delta from the unit
//   D0:2D (out*, h, tx, ty, fx, fy)               *out = direction_to; status 0
// An unknown handle gives 0x80000000 (script 1). The handle list (dword_5669EC)
// is newest first; ids count up from 0 (dword_5669E4++).
