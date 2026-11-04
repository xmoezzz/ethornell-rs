// 0xD0:80..8E — DCTESLGEvaluator, the strategy-game AI.
// Cleaned from src/raw/bgi1622/d0.{c,asm} (BGI 1.622); register arguments
// were taken from the disassembly. Grid calls refer to
// src/engine/logic_grid_field.c; all of them run on the evaluator's own copy
// of the grid (`snap`, or a per-thread copy `G` while scoring).
// Statuses: 0x9000000x are the evaluator's (script 16/17/...), see ADR 0002.
#include "../include/engine_types.h"

// ---------------------------------------------------------------------------
// Data.

// A unit record is 567 dwords (0x8DC). The script owns the first 525
// (0x834 bytes, D0:88/8D in, D0:8E out); the rest is the evaluator's.
enum {
    R_ID = 0,          // grid unit id; 0 = empty slot
    R_TEAM = 1,        // bit mask; units sharing a bit are allies
    R_SIDE = 2,        // turn side 0..sides-1, for relation multipliers
    R_TARGET = 3,      // index of the record this unit goes after (-1 none)
    R_PATH_W0 = 4, R_PATH_W1 = 5,          // on-path bonus: w0 + w1*cost²
    R_HP = 7, R_HP_MAX = 8,
    R_STATS = 11,      // 32 ints: kind 0 uses [0..16), kind 1 [16..32):
                       //   per element e (4): atk mul, atk add, def div, def sub
    R_WAIT = 75,       // time until the next turn; lower acts first
    R_BUFF_TIME = 77,  // [77+i] remaining duration of effect i (0..31)
    R_BUFF_VALUE = 109,// [109+i] its value; pairs used by sub_459BA0:
                       //   kind 0: atk (77/78), def (79/80); kind 1: (81/82), (83/84);
                       //   256: wait (85/86) — each "first minus second if active"
    R_JUMP = 141, R_MOVE = 142,
    R_MOVE_COST0 = 143, R_MOVE_COST1 = 144,   // per-candidate cost: c0 + c1*cost
    R_W_PARTNER = 146, R_W_STAY_WAIT = 147, R_W_STAY = 148,
    R_ATK_RANGE = 149, R_ATK_BONUS = 150, R_ATK_ARC = 151,  // weapon key 0
    R_ATK_KIND = 152, R_ATK_POWER = 153,      // 4 ints
    R_W_ATK_WAIT = 157, R_W_ATK_WAIT_PATH = 158,
    R_W_HIT = 159, R_W_HIT_DMG = 160, R_W_KILL = 161,
    R_W_TAKEN = 162, R_W_TAKEN_DMG = 163, R_W_DIE = 164,   // enemy weapon
    R_W_TAKEN_SK = 165, R_W_TAKEN_SK_DMG = 166, R_W_DIE_SK = 167, // enemy skill
    R_W_BUFFED = 168, R_W_BUFFED_DMG = 169,
    R_W_ENEMY_TURN = 171,      // 0 -> evaluator default 50
    R_W_DISTANCE = 172,        // per cost step of the target to this tile
    R_SKILL = 173,             // 16 slots of 22 ints, slot s at 173 + 22*s:
                               // [0] enabled [1] effect 0 dmg 1 heal 2 cut 3/4 set
                               // [2] effect index [3] formula kind [4..8) power
                               // [8] duration [10] range [11] bonus [12] arc
                               // [13] radius (<0 self, 0 single, >0 area)
                               // [14] wait [15] wait if moved
                               // [16..19) ally weights [19..22) enemy weights (w0, w1*v, kill)
    // evaluator's part:
    R_X = 525, R_Y = 526, R_DIR = 527,         // dir - 2
    R_GOAL_X = 528, R_GOAL_Y = 529,            // where to stand to hit R_TARGET
    R_STEP_X = 530, R_STEP_Y = 531,            // furthest path tile within R_MOVE
    R_ADVANCED = 532,                          // set once it walked in a simulation
    R_MAP = 533,       // [533] reach map, [534] reach map within 2*move,
                       // [535+2s]/[536+2s] same for skill key s+1; 28 bytes/tile
};

struct Candidate {     // 76 bytes
    int32_t x, y;      // where to stand
    int32_t type;      // 0 stay/move, 6 weapon, 7 skill
    int32_t skill;     // slot for type 7, else -1
    int32_t tx, ty;    // target tile, -1
    int32_t out6;      // MIN; becomes the score in the result
    int32_t face;      // [7] best facing for a move (set while scoring), -1
    int32_t arrive_dir;// [8] facing after walking here (cell back dir - 2)
    int32_t cost;      // [9] move cost to here
    int32_t partner;   // [10] the target's cost to here, -1 unreachable
    int32_t cost_w;    // [11] me[143] + cost*me[144], 0 when cost is 0
    int32_t on_path;   // [12] tile is on the path toward R_GOAL
    int32_t score[6];  // [13..18] per facing, MIN = none
};

struct Evaluator {     // 0x160
    int32_t threads;           // [2] [dword_56670C+4]
    struct Pool *pool;         // [3]
    struct GrdFldMngr *snap;   // [4] copy of the script's grid (sub_453260)
    struct GrdFldMngr **clones;// [5] per thread, made per D0:8A
    int32_t w, h;              // [6] [7]
    int32_t sides;             // [8] D0:84
    uint32_t count;            // [9] 1..64
    int32_t (*rec)[567];       // [10]
    int32_t acting;            // [11]
    int32_t order[64];         // [12..] next turns, -1 terminated
    int32_t relation[3];       // [76..78] 0x18000 next side, 0x10000 other, 0xAB85 previous
    double  facing_back;       // +0x140 = 2.0
    uint32_t ncand, next_cand; // [82] [83]
    struct Candidate *cand;    // [84]
    int32_t w_enemy_turn;      // [85] 50
    int32_t w_move_attack;     // [86] 67
};

// ---------------------------------------------------------------------------
// Small helpers.

// sub_4597C0: multiplier of an attack by `atk_side` on `tgt_side`.
int32_t relation(struct Evaluator *e, int32_t tgt_side, int32_t atk_side)
{
    int n = e->sides;
    if (tgt_side < 0 || tgt_side >= n || atk_side < 0 || atk_side >= n) return 0;
    if (tgt_side == (atk_side + 1) % n) return e->relation[0];
    if (tgt_side == (n + atk_side - 1) % n) return e->relation[2];
    return e->relation[1];
}

// sub_4010B0: snap to an integer within eps.
double snap(double x, double eps)
{
    double f = floor(x);
    if (1.0 - eps <= x - f) return ceil(x);
    if (eps < x - f) return x;
    return f;
}

// sub_459830: facing multiplier, 1.0 from the front up to facing_back from behind.
int32_t facing_mul(struct Evaluator *e, int32_t delta /* 16.16 degrees */)
{
    double v = snap(cos((double)delta / 11796480.0 * 1.5707963267948966), 1.0 / 1024);
    return (int32_t)((v + (1.0 - v) * e->facing_back) * 65536.0);
}

// sub_459BA0: active buff totals; out[0] attacker side, out[1] defender side.
// Kinds 2 and 256 only write out[0] (out[1] keeps the caller's old value).
uint32_t buffs(uint32_t kind, int32_t *atk, int32_t *out, int32_t *def)
{
    #define ON(r, i) ((r)[77 + (i)] > 0 ? (r)[109 + (i)] : 0)
    switch (kind) {
    case 0:   out[0] = ON(atk, 0) - ON(atk, 1); out[1] = ON(def, 2) - ON(def, 3); return 0;
    case 1:   out[0] = ON(atk, 4) - ON(atk, 5); out[1] = ON(def, 6) - ON(def, 7); return 0;
    case 2:   out[0] = ON(atk, 4) - ON(atk, 5); return 0;
    case 256: out[0] = ON(atk, 8) - ON(atk, 9); return 0;
    default:  out[0] = out[1] = 0; return 0x90000005;
    }
}

// sub_4598F0(eax=rel, ecx=atk stats, edi=atk buff, out, kind, power[4],
// def stats, def buff, facing): 64-bit products, high dword kept.
uint32_t damage(int32_t rel, const int32_t *a, int32_t ab, int32_t *out, int32_t kind,
                const int32_t *p, const int32_t *d, int32_t db, int32_t facing)
{
    int64_t sum = 0;
    if (kind == 0 || kind == 1) {
        int o = kind ? 16 : 0;
        for (int e = 0; e < 4; e++) {
            int32_t div = d[o + 4*e + 2] < 1 ? 1 : d[o + 4*e + 2];
            int32_t t = (a[o + 4*e + 1] + p[e] + ab) * a[o + 4*e] / div   // 32-bit
                        - d[o + 4*e + 3] - db;
            sum += t > 0 ? t : 0;
        }
    } else if (kind == 2) {                                   // heal: no defence
        for (int e = 0; e < 4; e++) sum += (int32_t)(a[16 + 4*e + 1] + p[e] + ab);
    } else { *out = 0; return 0x90000005; }
    *out = (int32_t)((uint64_t)(sum * facing * rel) >> 32);
    return 0;
}

// sub_459880: the 20-byte weapon key; 0 = basic attack, 1..16 = skill s-1.
uint32_t weapon_key(int32_t *r, int s, int32_t key[5])
{
    if (s == 0) { key[0]=0; key[1]=r[149]; key[2]=r[150]; key[3]=r[151]; key[4]=0; return 0; }
    if (s - 1 > 15) return 0x90000004;
    int32_t *k = &r[22 * s + 151];
    if (!k[0]) return 0x90000004;
    key[0]=k[1]; key[1]=k[10]; key[2]=k[11]; key[3]=k[12]; key[4]=k[13];
    return 0;
}

// sub_45A420 / sub_45A4B0: the record standing on (x, y) (first match).
// sub_45A4E0: index of the record with the same unit id, -1.

// sub_45BB60(out[ndirs], ndirs, e, tile[7], face): for each facing i
// (or only `face` when >= 0), the first side in the order back, left/right,
// front (grid vtbl+10 with rel=1) that the tile is threatened from.
// Returns tile[0] != 0. Facings without a hit keep their old value.
int threat_sides(int32_t *out, int ndirs, int32_t *tile, int face)
{
    static const int order[4][4] = {{1,2,3,0},{0,3,2,1},{3,1,0,2},{2,0,1,3}};
    if (!tile[0]) return 0;
    for (int i = 0; i < ndirs; i++) {
        if (face >= 0 && face != i) continue;
        for (int k = 0; k < ndirs; k++)
            if ((uint32_t)tile[order[i][k] + 1] >= 1) { out[i] = order[i][k]; break; }
    }
    return 1;
}

// ---------------------------------------------------------------------------
// Loading (D0:88 sub_458550, D0:8D sub_4586E0, D0:8C sub_4586B0).

uint32_t load(struct Evaluator *e, struct GrdFldMngr *grid, uint32_t n, const int32_t *src)
{
    if (!grid) { unload(e); return 0; }            // (grid 0 = clear)
    if (n - 1 > 63) return 0x90000002;
    unload(e);
    snapshot(e, grid);                              // sub_453260 clone, or none
    e->count = n;
    e->rec = calloc(n, 0x8DC);
    for (uint32_t i = 0; i < n; i++) load_one(e, i, src + 525 * i, 1);
    return 0;
}

uint32_t load_one(struct Evaluator *e, uint32_t i, const int32_t *src, int maps)
{
    if (i >= e->count) return 0x90000003;
    int32_t *r = e->rec[i];
    memcpy(r, src, 0x834);
    if (!r[0]) { r[525] = r[526] = -1; return 0; }
    grid_unit_pos(e->snap, r[0], &r[525], &r[526]);        // sub_4536F0
    int32_t dir; grid_unit_dir(e->snap, r[0], &dir);        // sub_453760
    r[527] = dir - 2;
    if (!maps) return 0;
    int32_t keys[16][5]; int nk = 1;
    weapon_key(r, 0, keys[0]);
    for (int s = 1; s < 16; s++) if (!weapon_key(r, s, keys[nk])) nk++;   // 1..15 only
    grid_threat_maps(e->snap, r[0], r[141], r[142], nk, keys, /*mark*/0, /*double*/1);
    r[533] = map_from_cache(keys[0], 0); r[534] = r[533] ? map_from_cache(keys[0], 1) : 0;
    for (int s = 0; s < 16; s++) {
        int32_t key[5];
        r[535 + 2*s] = r[536 + 2*s] = 0;
        if (weapon_key(r, s + 1, key)) continue;        // slot 16 can still match a key
        r[535 + 2*s] = map_from_cache(key, 0);
        if (r[535 + 2*s]) r[536 + 2*s] = map_from_cache(key, 1);
    }
    return 0;
}

// grid_threat_maps sub_453C20(G, unit, jump, move, n, keys, mark, dbl):
// clears the unit's cache, searches with move (or 2*move when dbl) and `mark`,
// then for each new key: over the unit's tile and every reachable tile
// (row-major) with cost <= move (map) — or, for key kind 0 with dbl, cost <=
// 2*move (map2) — list the tiles a shot reaches (vtbl+4: range/bonus/arc from
// the key, units_only 0, ignore_units = !mark, exclude self). For a radius
// (key[4]) > 0 every unit tile within the radius of a hit tile (ignore 0) is
// counted, side = direction_to(hit, from area tile) [or from the shooter for
// the hit tile itself]; radius 0 counts the hit tile, side = direction_to
// (shooter, from hit). tile[0] = 1, tile[side - 1]++ (sides 2..5 -> 1..4).
// The maps are cached on the grid unit under the 20-byte key.

// ---------------------------------------------------------------------------
// Choosing targets (sub_459CF0), once per unit at the start of D0:8A.

void choose_target(struct Evaluator *e, uint32_t i)
{
    int32_t *r = e->rec[i];
    if (!r[0]) return;
    grid_search(e->snap, r[0], r[141], 0, -1, -1, /*mark*/0);   // unlimited move
    int32_t dist[64], gx[64], gy[64], ex[64], ey[64];
    if (r[149] == 1) {                       // melee: cheapest tile next to the enemy
        for (uint32_t j = 0; j < e->count; j++) {
            int32_t *o = e->rec[j];
            if ((o[1] & r[1]) || !o[0]) { dist[j] = 0; continue; }
            int32_t best = 0x7FFFFFFF, x, y;
            grid_unit_pos(e->snap, o[0], &x, &y);
            int32_t steps[6], xy[6][2];
            grid_neighbors(e->snap, r[0], steps, xy, x, y);  // 4538C0, height diff 1
            for (int k = 0; k < 6; k++) {
                if (steps[k] < 0) continue;
                int32_t c; grid_cell_cost(e->snap, r[0], xy[k][0], xy[k][1], &c);
                if (c < best) { best = c; x = xy[k][0]; y = xy[k][1]; }
            }
            if (best == 0x7FFFFFFF) dist[j] = -1;
            else { dist[j] = best; gx[j] = x; gy[j] = y; }
        }
    } else {                                 // ranged: first tile (by cost) that hits
        for (uint32_t j = 0; j < e->count; j++) {
            int32_t *o = e->rec[j];
            if ((o[1] & r[1]) || !o[0]) { dist[j] = 0; continue; }
            dist[j] = -1;
            grid_unit_pos(e->snap, o[0], &ex[j], &ey[j]);
        }
        tiles = grid_reachable_sorted(e->snap, r[0]);       // MSVC qsort by cost
        for (each tile t in order, while some dist[j] < 0) {
            int32_t c; grid_cell_cost(e->snap, r[0], t.x, t.y, &c);
            hits = grid_targets(e->snap, t.x, t.y, r[149], r[150], r[151], 1, 1, 1);
            for (each hit h) for (j = first with dist[j] < 0 && (ex,ey)[j] == h)
                { dist[j] = c; gx[j] = t.x; gy[j] = t.y; }
        }
    }
    // Keep or switch the target. A target on the next side (better
    // multiplier) is preferred; a neutral one over the previous side.
    int32_t best = 0, cur_side = -1, n = e->sides;
    if (r[3] >= 0 && dist[r[3]] >= 1) { best = dist[r[3]]; cur_side = e->rec[r[3]][2]; }
    for (uint32_t j = 0; j < e->count; j++) {
        int32_t d = dist[j];
        if (d < 1) continue;
        int32_t s = e->rec[j][2], c = r[2];
        int next = s == (c + 1) % n, neutral = !next && s != (n + c - 1) % n;
        int cur_next = cur_side == (c + 1) % n, cur_prev = cur_side == (n + c - 1) % n;
        int cur_neutral = !cur_next && !cur_prev;
        int take;
        if (d < best) {
            int le = 2 * d <= best;
            take = (next && (!cur_next || le))
                || (neutral && (cur_prev || (cur_neutral && le)))
                || best == 0;
        } else if (d < best + r[142]) {
            take = (next && !cur_next) || (neutral && cur_prev) || best == 0;
        } else take = best == 0;
        if (take) { best = d; cur_side = s; r[3] = j; r[528] = gx[j]; r[529] = gy[j]; }
    }
    if (r[3] >= 0 && path to (r[528], r[529]) exists)
        for (each tile p along the path, from the unit)
            if (cost(p) <= r[142]) { r[530] = p.x; r[531] = p.y; }
}

// ---------------------------------------------------------------------------
// Deciding (D0:8A sub_458A70(out_count, e, out, acting, max)).

uint32_t decide(struct Evaluator *e, uint32_t *out_count, int32_t (*out)[7],
                uint32_t acting, uint32_t max)
{
    if (acting >= e->count) return 0x90000003;
    e->acting = acting;
    for (uint32_t i = 0; i < e->count; i++) choose_target(e, i);
    grid_size(e->snap, &e->w, &e->h);
    int32_t (*R)[567] = copy(e->rec);
    int32_t *me = R[acting], *partner = 0;
    path = none;
    if (me[3] >= 0) {
        grid_search(e->snap, me[0], me[141], 0, -1, -1, 0);
        path = grid_path_points(e->snap, me[0], me[528], me[529]);   // may fail
        partner = R[me[3]];
        grid_search(e->snap, partner[0], partner[141], 0, -1, -1, 0);
    }
    grid_search(e->snap, me[0], me[141], me[142], -1, -1, /*mark*/1);
    tiles = grid_reachable_sorted(e->snap, me[0]) + [own position];
    struct Candidate tpl = { .type = 0, .skill = -1, .tx = -1, .ty = -1, .out6 = MIN,
                             .face = -1, .partner = 0, .score = {MIN x6} };
    int32_t arrive = 6;                              // keeps its last value
    for (each tile p) {
        grid_set_pos(e->snap, me[0], p.x, p.y); me[525] = p.x; me[526] = p.y;
        c = append(tpl); c.x = p.x; c.y = p.y; c.type = 0;
        grid_cell_facing(e->snap, me[0], p, &arrive); c.arrive_dir = arrive - 2;
        grid_cell_cost(e->snap, me[0], p, &c.cost);
        if (partner && grid_cell_cost(e->snap, partner[0], p, &c.partner)) c.partner = -1;
        c.cost_w = c.cost ? me[143] + c.cost * me[144] : 0;
        c.on_path = p is one of `path`;
        struct Candidate t2 = c; t2.type = 6;
        for (hit h in grid_targets(e->snap, p, me[149], me[150], me[151], 1, 0, 1)) {
            int32_t *o = record_at(R, h);
            if (o && !(o[1] & me[1])) append(t2 with tx,ty = h);
        }
        t2.type = 7;
        for (int s = 0; s < 16; s++) {
            int32_t *k = &me[173 + 22 * s];
            if (!k[0]) continue;
            t2.skill = s;
            if (k[13] < 0) { append(t2); continue; }          // self effect
            for (hit h in grid_targets(e->snap, p, k[10], k[11], k[12],
                                       /*units_only*/ k[13] == 0, 0, /*exclude*/0)) {
                int ok;
                if (k[13] == 0) {
                    int32_t *o = record_at(R, h); int ally = o[1] & me[1];
                    ok = (k[1] == 0 || k[1] == 4) ? !ally : (k[1] <= 3 ? ally : 1);
                } else {
                    area = grid_targets(e->snap, h, k[13], 0, 0, 1, 0, 0);
                    if (!area) ok = 0;
                    else { count allies / enemies among area records;
                           ok = (k[1] == 0 || k[1] == 4) ? enemies > 0
                              : (k[1] <= 3 ? allies > 0 : 1); }
                }
                if (ok) append(t2 with tx,ty = h);
            }
        }
    }
    e->ncand = n; e->next_cand = 0; e->cand = cands;
    // Turn order after this unit: by wait + wait buff, lowest first, ties by index.
    int done[64] = {0}; done[acting] = 1;
    for (uint32_t k = 0; k < e->count; k++) {
        int32_t pick = -1, low = 0x7FFFFFFF;
        for (uint32_t j = 0; j < e->count; j++) {
            if (done[j]) continue;
            if (!R[j][0]) { done[j] = 1; continue; }
            if (R[j][75] + R[j][108] < low) { low = R[j][75] + R[j][108]; pick = j; }
        }
        e->order[k] = pick;
        if (pick < 0) break;
        done[pick] = 1;
    }
    run score() for every candidate (thread pool, one grid copy per thread);
    // Results: one per (candidate, facing) with a score.
    for (each candidate c) for (int f = 0; f < 6; f++) if (c.score[f] != MIN) {
        int32_t r[7] = { c.x, c.y, c.type, c.skill, c.tx, c.ty, 0 };
        int bonus = 0;
        if (r[2] == 0) { r[2] = f; if (c.face == f) bonus = 2; if (c.arrive_dir == f) bonus = 1; }
        r[6] = c.score[f] + bonus;
        list.add(r);
    }
    qsort(list, by r[6] descending);                 // sub_45BB40, MSVC qsort
    copy min(n, max) to out; *out_count = min(n, max);
    return 0;
}

// ---------------------------------------------------------------------------
// Scoring one candidate (sub_45A530 on thread t; returns 0 when none left).
// `score` is the action's value, `wait` the unit's next turn time; afterwards
// every enemy (and ally) that acts before that time is simulated.

int score(struct Evaluator *e, int t)
{
    struct Candidate *c = take_next_candidate(e);        // under the pool lock
    if (!c) return 0;
    struct GrdFldMngr *G = e->clones[t];
    int32_t (*R)[567] = copy(e->rec);
    int32_t *me = R[e->acting];
    int32_t *tgt = me[3] >= 0 ? R[me[3]] : 0;
    me[525] = c->x; me[526] = c->y;
    for (each record r with r[0]) grid_set_pos(G, r[0], r[525], r[526]);
    int32_t order[...] = e->order;
    int32_t score = 0, wait = c->cost_w, face = -1, buf[2], dmg, delta;
    switch (c->type) {
    case 0: wait += me[147]; score = me[148]; break;
    case 6: {
        int32_t *t = record_at(R, c->tx, c->ty);
        buffs(me[152], me, buf, t);
        grid_unit_facing_delta(G, t[0], c->x, c->y, &delta);       // 453BE0
        damage(relation(e, t[2], me[2]), me + 11, buf[0], &dmg, me[152], me + 153,
               t + 11, buf[1], facing_mul(e, delta));
        if (dmg > t[7]) dmg = t[7];
        t[7] -= dmg;
        score = me[159] + me[160] * dmg + (t[7] <= 0 ? me[161] : 0);
        wait += me[157] + (c->cost ? me[158] : 0);
        face = direction_to(c->tx, c->ty, c->x, c->y) - 2;
        break; }
    case 7: {
        int32_t *k = &me[173 + 22 * c->skill];
        if (k[13] < 0) { score = me[159]; }
        else {
            hits = k[13] == 0 ? [(c->tx, c->ty)]
                 : grid_targets(G, c->tx, c->ty, k[13], 0, 0, 1, 0,
                                /*exclude*/ (c->tx,c->ty) != (c->x,c->y));
            for (each hit h) {
                int32_t *t = record_at(R, h);
                buffs(k[3], me, buf, t);
                int32_t fm = 0x10000;
                if (k[1] == 0) {
                    from = h == (c->tx,c->ty) ? (c->x, c->y) : (c->tx, c->ty);
                    grid_unit_facing_delta(G, t[0], from, &delta);
                    fm = facing_mul(e, delta);
                }
                damage(relation(e, t[2], me[2]), me + 11, buf[0], &dmg, k[3], k + 4,
                       t + 11, buf[1], fm);
                int32_t *w = (t[1] & me[1]) ? k + 16 : k + 19;
                switch (k[1]) {
                case 0: if (dmg > t[7]) dmg = t[7]; t[7] -= dmg;
                        score += w[0] + w[1] * dmg + (t[7] <= 0 ? w[2] : 0); break;
                case 1: if (dmg > t[8] - t[7]) dmg = t[8] - t[7]; t[7] += dmg;
                        score += w[0] + w[1] * dmg; break;
                case 2: if (dmg > t[77 + k[2]]) dmg = t[77 + k[2]];
                        t[77 + k[2]] -= dmg; score += w[0] + w[1] * dmg;
                        if (k[2] is 8, 9 or 31) requeue(t); break;
                case 3: case 4: t[77 + k[2]] = k[8]; t[109 + k[2]] = dmg;
                        score += w[0] + w[1] * dmg;
                        if (k[2] is 8, 9 or 31) requeue(t); break;
                }
            }
        }
        wait += k[14] + (c->cost ? k[15] : 0);
        face = direction_to(c->tx, c->ty, c->x, c->y) - 2;
        break; }
    }
    // requeue(t): unless t is `me`, t's new wait = max(0, t[75] + t[108] - buff256);
    // if it changed, move t's index in `order` to keep it sorted by R[.][75]
    // (stable: before the first entry with a larger wait, see sub_45A530 AAAA).
    score += me[146] * c->cost;
    if (c->partner >= 1) score += me[172] * c->partner;
    else if (c->partner < 0) score += me[172] * (e->w + e->h);
    int walk_attack = 0;
    if (c->on_path) {
        score += me[4] + me[5] * c->cost * c->cost;
        walk_attack = c->type == 0 && me[149] == 1;
    }
    me[75] = wait;
    buffs(256, me, buf, 0);
    wait += me[108] - buf[0];
    int ndirs = 4;                                        // grid vtbl+8
    int32_t sum[6] = {0}, hp[4] = {me[7] x4}, side[6], sides[16][6], skill_hit[16];
    int32_t tile = c->x + e->w * c->y;
    int32_t *map = scratch(28 * w * h);
    for (int i = 0; order[i] >= 0; i++) {
        int32_t *u = R[order[i]];
        if (u[7] <= 0) continue;
        if (u[75] > wait || (u[75] == wait && e->acting < order[i])) break;
        if (u[1] & me[1]) {                                  // ally: help with the target
            if (!tgt || !u[533] || !u.map533[tgt tile].covered) continue;
            fresh map for key 0 (grid_threat_maps(G, u, u[141], u[142], 1, key0, 1, 0));
            if (!threat_sides(side, ndirs, map[tgt tile], tgt[527])) continue;
            alive = tgt[7] >= 1;
            buffs(u[152], u, buf, tgt);
            from = grid_step_from_unit(G, tgt[0], side[tgt[527]], rel 1);
            grid_unit_facing_delta(G, tgt[0], from, &delta);
            damage(relation(e, tgt[2], u[2]), u + 11, buf[0], &dmg, u[152], u + 153,
                   tgt + 11, buf[1], facing_mul(e, delta));
            if (dmg > tgt[7]) dmg = alive ? tgt[7] : 0;
            tgt[7] -= dmg;
            score += me[159] + me[160] * dmg + (tgt[7] <= 0 && alive ? me[161] : 0);
            continue;
        }
        int sel = u[532] != 0;                               // advanced: double-move map
        int threat = 0, any;
        if (u.maps[533 + sel][tile].covered) {
            fresh map for key 0 with mark;  threat = threat_sides(side, ndirs, map[tile], face);
        }
        any = threat;
        for (int s = 0; s < 16; s++) {
            skill_hit[s] = 0;
            if (!u[173 + 22*s] || !u.maps[535 + sel + 2*s][tile].covered) continue;
            fresh map for key s+1 with mark;
            if (u[174 + 22*s] == 0) skill_hit[s] = threat_sides(sides[s], ndirs, map[tile], face);
            else if (u[174 + 22*s] == 4) skill_hit[s] = map[tile].covered;
            any = any || skill_hit[s];
        }
        int32_t wgt = u[532] ? (me[171] ? me[171] : e->w_enemy_turn) : 100;
        for (int f = 0; f < ndirs; f++) {
            if (face >= 0 && face != f) continue;
            grid_set_dir(G, me[0], f + 2);
            if (threat) {
                alive = hp[f] >= 1;
                buffs(u[152], u, buf, me);
                from = grid_step_from_unit(G, me[0], side[f], 1);
                grid_unit_facing_delta(G, me[0], from, &delta);
                damage(relation(e, me[2], u[2]), u + 11, buf[0], &dmg, u[152], u + 153,
                       me + 11, buf[1], facing_mul(e, delta));
                hp[f] -= dmg;
                sum[f] += wgt * (me[162] + me[163] * dmg + (hp[f] <= 0 && alive ? me[164] : 0)) / 100;
            }
            for (int s = 0; s < 16; s++) {
                if (!skill_hit[s]) continue;
                int32_t *k = &u[173 + 22 * s];
                if (k[1] == 0) {
                    alive = hp[f] >= 1;
                    buffs(k[3], u, buf, me);
                    from = grid_step_from_unit(G, me[0], sides[s][f], 1);
                    grid_unit_facing_delta(G, me[0], from, &delta);
                    damage(relation(e, me[2], u[2]), u + 11, buf[0], &dmg, k[3], k + 4,
                           me + 11, buf[1], facing_mul(e, delta));
                    hp[f] -= dmg;
                    sum[f] += wgt * (me[165] + me[166] * dmg + (hp[f] <= 0 && alive ? me[167] : 0)) / 100;
                } else if (k[1] == 4) {
                    buffs(k[3], u, buf, me);
                    damage(relation(e, me[2], u[2]), u + 11, buf[0], &dmg, k[3], k + 4,
                           me + 11, buf[1], 0x10000);
                    me[77 + k[2]] = k[8]; me[109 + k[2]] = dmg;
                    sum[f] += wgt * (me[168] + me[169] * dmg) / 100;
                }
            }
        }
        if (!any && !u[532]) {
            // Out of reach: it walks toward its target and acts again later.
            u[75] += u[143] + u[144] * ((3 * u[142]) >> 2);
            insert order[i] again before the first later entry with a larger
            wait (or at the end); u[532] = 1;
            if (u[3] >= 0) { u[525] = u[530]; u[526] = u[531]; }
        }
        if (c->type == 0 && any && c->face < 0) {
            // A plain move picks the facing that is hit least from behind:
            // per facing, the hits on this tile from that side (weapon map and
            // damage skills of every slot whose map is in reach).
            int32_t hits[6] = {0};
            for (int f = 0; f < ndirs; f++) {
                hits[f] += u.maps[533 + sel][tile][1 + f];
                for (int s = 0; s < 16; s++)
                    if (skill_hit[s] && u[174 + 22*s] == 0)
                        hits[f] += u.maps[535 + sel + 2*s][tile][1 + f];
            }
            uint32_t most = max over hits (unsigned);
            if (exactly one f has hits[f] == most) c->face = f;
            else { d = direction_to(u[525], u[526], c->x, c->y) - 2;
                   c->face = hits[d] == most ? d : last f with hits[f] == most; }
        }
    }
    if (walk_attack) {
        // Melee unit walking along its path: the best hit it could make next
        // to the target, weighted by w_move_attack/100.
        grid_search(G, me[0], me[141], me[142], -1, -1, 1);
        grid_neighbors(G, me[0], steps, xy, tgt[525], tgt[526]);
        int32_t best = MIN;
        for (int k = 0; k < ndirs; k++) {
            if (steps[k] < 0) continue;
            alive = tgt[7] >= 1;
            buffs(me[152], me, buf, tgt);
            grid_unit_facing_delta(G, tgt[0], xy[k], &delta);
            damage(relation(e, tgt[2], me[2]), me + 11, buf[0], &dmg, me[152], me + 153,
                   tgt + 11, buf[1], facing_mul(e, delta));
            if (dmg > tgt[7]) dmg = alive ? tgt[7] : 0;
            tgt[7] -= dmg;                           // cumulative across k
            int32_t v = me[159] + me[160] * dmg + (tgt[7] <= 0 && alive ? me[161] : 0);
            if (best < v) best = v;
        }
        if (best != MIN) score += e->w_move_attack * best / 100;
    }
    for (int f = 0; f < ndirs; f++)
        if (face < 0 || face == f) c->score[f] = score + sum[f];
    return 1;
}

// ---------------------------------------------------------------------------
// Handlers (script args in call order). Jobs run on a worker thread in the
// target; the port runs them at once (ADR 0002 decision 3).
//
//   D0:80 (out*)                      new evaluator; no value pushed
//   D0:81 (h)                         raw 0 ok / 1 unknown
//   D0:84 (h, sides)                  sides < 1 -> 0x90000002
//   D0:87 (out*, h)                   *out = last job result, then 0xFFFF0000; raw 0
//   D0:88 (h, grid, n, units*, async) load; unknown grid/evaluator -> raw 1
//   D0:8A (out*, count*, h, acting, max)   decide (always a job); 28-byte results
//   D0:8C (h, grid)                   re-snapshot; unknown -> raw 1
//   D0:8D (h, index, unit*, maps, async)   load_one
//   D0:8E (out*, h, index)            copy record 0x834 out
// "busy" (14) never happens in the port. Results of jobs go through
// status_to_script; the synchronous paths return it directly.
