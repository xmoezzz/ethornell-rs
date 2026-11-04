// 0xD0:40..7A — DCTELgclSpcMngr: round bodies in 3D, links between them,
// overlap / sweep tests and a planar path search between two bodies.
// Cleaned from src/raw/bgi1622/d0.{c,asm} (BGI 1.622). All maths is SSE
// single precision unless noted; x87 parts run at 53-bit precision.
// Statuses 0xA000000x reach the script as 18..22 (0xA0000004 -> 8).
#include "../include/engine_types.h"

struct Body {                // 96 bytes, 16-aligned
    int32_t used;            // +00
    float   radius;          // +04 >= 1; collisions and path contact
    float   speed;           // +08 velocity = dir * speed
    float   probe;           // +0C radius of the D0:74 look-ahead test
    int32_t mask;            // +10 low byte: groups this body belongs to
    int32_t priority;        // +14 sweep test: an overlap at the start is ignored
                             //     when the mover's priority is higher
    int32_t parent[2];       // +18 two independent link kinds, -1 none
    struct { uint32_t slots; int32_t *child; } list[2];  // +20 -1 = free slot
    float   pos[4];          // +30 (x, y, z, 0), snapped to integers within 1/1024
    float   dir[4];          // +40 normalised
    float   vel[4];          // +50
};

struct SpcMngr {             // 0x170
    void   *vtbl;
    uint32_t capacity;       // +10 64, doubled up to cover index < 0x1000
    struct Body *body;       // +14
    // path search scratch, +18..+168, see path_search()
};

// Helpers. len2(v) = x² + (y² + z²) in single precision.
// rsqrt_newton(s) (sub_455860 etc.): r = rsqrtss(s); r = r*1.5 + (s*r*r*r)*-0.5.
// rcp_newton(s): r = rcpss(s); r = (r + r) - s*(r*r).
// rsqrtss/rcpss are hardware approximations (Intel and AMD differ); the
// port starts the Newton step from the exact value.
// snap(x) = sub_4010B0(x, 1/1024) in double, stored as float.
// to_fixed(v) = sub_455940: cvtps2dq(v * 65536) (nearest), then a fraction
// within 0x40 of an integer snaps to it.
// norm_len(v) (sub_4558C0): s = len2(v); s <= 0 -> 0; else
// (v.xyz * r, s * r) with r = rsqrt_newton(s).

// ---------------------------------------------------------------------------
// Bodies.

// D0:60 sub_455A40(index, x,y,z, dx,dy,dz, radius, speed, probe, mask, priority)
uint32_t body_add(struct SpcMngr *m, uint32_t i, float x, float y, float z,
                  float dx, float dy, float dz, float radius, float speed,
                  float probe, int32_t mask, int32_t priority)
{
    if (i >= 0x1000) return 0xA0000001;
    grow capacity (x2 from the current) until i < capacity, new bodies zeroed;
    if (radius < 1.0) return 0xA0000002;
    if (!(uint8_t)mask) return 0xA0000003;
    struct Body *b = &m->body[i];     // overwritten as is, links not undone
    *b = (struct Body){ .used = 1, .radius = radius, .speed = speed, .probe = probe,
                        .mask = mask, .priority = priority, .parent = {-1, -1},
                        .pos = { snap(x), snap(y), snap(z), 0 } };
    body_set_dir(m, i, dx, dy, dz);
    return 0;
}

// D0:66 sub_455DF0: dir = snapped (dx,dy,dz) normalised; vel = dir * speed.
// A zero vector -> 0xA0000002. D0:64 sub_455CD0 sets pos (snapped).
// D0:65 / D0:67 (sub_455DC0 / sub_455F60) write to_fixed(pos / dir).xyz.
// Unknown or unused body: 0xA0000001.

// D0:68 sub_455F90(child, kind, parent): kind >= 2 -> 0xA0000005.
// parent == -1 detaches (removes the child from its parent's list).
// Otherwise the parent must be another used body (else 0xA0000002); the
// child is detached first, then put in the parent's first free slot; a
// full list grows to 2x (64 at first), new slots -1.
// D0:69 sub_456160: the parent. D0:6A sub_4561A0: the children (free
// slots skipped), list optional.
// D0:61 sub_455C30: detach it and all its children for both kinds, clear.

// ---------------------------------------------------------------------------
// Queries.

// sub_457340(point, radius, exclude, mask): bodies (ascending index) with
// len2(point - pos) < (radius + body.radius)² in the mask.
// D0:74 sub_456960(index, mask): point = pos + vel, radius = probe, excluding itself.
// D0:75 sub_4569C0(x, y, z, radius, exclude, mask).
// Mask 0 means 0xFF.

// D0:78 sub_456A10(index, mask): every other body in the mask as
// {index, -, -, -, to_fixed(norm_len(pos - me.pos))}, nearest first
// (stable). D0:79 sub_456C00(a, b): {b, to_fixed(norm_len(b - a))}.
// D0:7A sub_456CC0(a, x,y,z): {-1, to_fixed(norm_len(point - a))}.

// ---------------------------------------------------------------------------
// Sweep test sub_456D70(A, B, radius, priority, exclude, mask, sweep):
// 0xA0000004 when a body of radius r moving from A to B hits another.
// A == B (all four lanes) never hits.
//   e1 = (float)atan2(dy, dx) + pi/2, e2 = (float)atan2(dz, dx) + pi/2 (double)
//   for k in {0, pi}: o1 = (cos, sin, 0, 0)(e1 + k) * r, o2 = (cos, 0, sin, 0)(e2 + k) * r
//     S = A + o, E = B + o, D = E - S, L = |D|² over the plane's two lanes, iL = rcp_newton(L)
//   for each other body C in the mask:
//     rr = (r + C.radius)²; dA = len2(A - C.pos); dB = len2(B - C.pos)
//     dB < rr: hit if dA >= rr or priority <= C.priority
//     dA < rr or !sweep: no hit
//     else for each plane p (XY, XZ) and edge k: v = C.pos - S, t = D·v;
//       if 0 < t < L: m = |v|² - t*iL*t; m < C.radius² -> plane hit,
//       else keep m (the last kept value per (p, k) survives across bodies)
//     a plane with no hit counts as hit when both kept m < (2r)²;
//     hit when both planes hit.

// ---------------------------------------------------------------------------
// D0:72 path search sub_456280(out, count, from, to, range, step, mask,
// sweep, resolution, threaded): a FIFO search on a square grid of half-step
// cells centred on `from`, in the XY plane, for a place where `from`
// touches `to`. Output: {to_fixed(x, y, z, segment length)} per move.
//   step 0 -> 1; inv = rcp_newton(step); n = 2 * max(1, |ceil(range * inv)|);
//   N = 2n + 1; target cell t = cvtps2dq((to - from) * 2 * inv) + n; outside -> 0xA0000006
//   G = |ceil((from.r + to.r) * 2 * inv)|; H = G < 4 ? G : (int)((G + 0.7) * 0.7071067)
//   goals (16): it = 0, 1: (t.x+G+it, t.y), (t.x+H+it, t.y+H+it), (t.x, t.y+G+it),
//     (t.x-H-it, t.y+H+it), (t.x-G-it, t.y), (t.x-H-it, t.y-H-it), (t.x, t.y-G-it),
//     (t.x+H+it, t.y-H-it)
//   16 moves: the perimeter of a 5x5 square from (2,0) anticlockwise
//     (2,0) (2,1) (2,2) (1,2) (0,2) (-1,2) (-2,2) (-2,1) (-2,0) ... (2,-1);
//     float move = cells * step * 0.5
//   resolution r (4 << r > 16 -> 2) picks every 16 >> (r + 2)th direction.
//   enqueue(A, B, cell, dir, cost, len): rejected off the grid, when
//     |B - from| > range, or when the cell has a cost and cost < cost + len + step;
//     node cost = cost + len.
//   start: enqueue(from, from, (n, n), -1, 0, 0).
//   pop: sweep-test A -> B (B.w = segment length); blocked nodes are dropped;
//     a cell with a cost below node cost + step keeps it; else the cell takes
//     {dir, cost, B}; a goal cell ends the search; else expand from dir - 4
//     over 9 slots (16 at the start) every (16 >> (r + 2)): B' = B + move,
//     len = |move|.
//   no goal -> 0xA0000006. Backtrack from the goal cell through
//   dir + 8 (mod 16) until dir == -1.
// The thread pool only pre-computes sweep tests for queued nodes; the
// order of acceptance is the main thread's FIFO, so it is not reproduced.
//
// Handlers (script args in call order; fixed 16.16 arguments become floats):
//   D0:40 (out*) create; raw 0          D0:41 (h) destroy; raw 0 / 1
//   D0:60 (h, i, x,y,z, dx,dy,dz, radius, speed, probe, mask, priority)
//   D0:61 (h, i)                        D0:64 (h, i, x, y, z)
//   D0:65 (out*, h, i)                  D0:66 (h, i, dx, dy, dz)
//   D0:67 (out*, h, i)                  D0:68 (h, child, kind, parent)
//   D0:69 (out*, h, i, kind)            D0:6A (list*, count*, h, i, kind)
//   D0:70 (h, i, x, y, z, mask, sweep)  sweep test of body i to (x, y, z)
//   D0:72 (out*, count*, h, from, to, range, step, mask, sweep, resolution, threaded)
//   D0:74 (list*, count*, h, i, mask)
//   D0:75 (list*, count*, h, x, y, z, radius, exclude, mask)
//   D0:78 (out*, count*, h, i, mask)    D0:79 (out*, h, a, b)
//   D0:7A (out*, h, i, x, y, z)
// Unknown handle: 0xA0000000 (script 1). Handles from ++dword_5669F0.
