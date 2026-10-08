// Display-object selectors (Graph90 group): hand-cleaned from
// src/raw/text_478000.c, text_47C000.c, text_418000.c, text_43C000.c,
// text_424000.c. Names follow the Rust port (crates/ethornell-app).
// Script errors are sub_4646F0(message, thread), which does not return.
#include "../include/engine_types.h"

extern uint32_t pop(struct CThread *t);              // sub_4450B0
extern void push(struct CThread *t, uint32_t v);     // sub_4450D0
extern void script_error(const char *msg, struct CThread *t); // sub_4646F0
extern struct CDspObj *find_object(uint32_t handle); // sub_443270 / sub_43E5D0
extern int bitmap_get(uint32_t handle, struct BitmapDesc *out); // sub_407F20

// sub_497B60 / sub_497E00 / sub_497C40 / sub_497DB0 / sub_497BB0.
static void check_bitmap(uint32_t h, struct CThread *t)  { if (h >= 0x4000) script_error("bitmap handle", t); }
static void check_le_256(uint32_t v, struct CThread *t)  { if (v > 0x100) script_error("value > 256", t); }
static void check_priority(uint32_t v, struct CThread *t){ if (v >= 0x10000) script_error("priority", t); }
static void check_blend(uint32_t m, struct CThread *t)
{
    switch (m) {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
    case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    case 0x40: case 0x41: case 0x80: case 0xC0: case 0xC1: case 0xF0: case 0xFF:
        return;
    }
    script_error("blend mode", t);
}

// ------------------------------------------------- Graph90:33 position ---

// sub_41C130: rewrite the parent's member record for `child` to the child's
// current offset from the parent.
static int relink_member(struct CDspObj *parent, struct CDspObj *child)
{
    struct MemberLink *m;
    for (m = parent->members; m; m = m->next)
        if (m->obj == child) {
            m->dx = child->x - parent->x;   // both read through vtbl+48
            m->dy = child->y - parent->y;
            return 1;
        }
    return 0;
}

// sub_41B1D0, CDspObj vtbl+40. Children are moved to parent + record
// offset with relink = 0, so only the object the caller named is relinked.
void CDspObj_SetPosition(struct CDspObj *o, int x, int y, int relink, int propagate)
{
    struct MemberLink *m;
    o->x = x;
    o->y = y;
    if (relink && o->parent)
        relink_member(o->parent, o);
    if (propagate)
        for (m = o->members; m; m = m->next)
            m->obj->vtbl_SetPosition(m->obj, x + m->dx, y + m->dy, 0, 1);
}

// sub_41B1B0, vtbl+44 (Back* and Landscape override it; see native_graph.rs).
void CDspObj_SetPosition2(struct CDspObj *o, int x, int y) { CDspObj_SetPosition(o, x, y, 1, 1); }

// sub_47B1B0 -> sub_461F50 -> sub_4434C0. Script order (object, x, y).
int Graph90_33_SetObjectPosition(struct CThread *t)
{
    int y = pop(t), x = pop(t);
    struct CDspObj *o = find_object(pop(t));
    if (!o)
        script_error("invalid object", t);      // byte_4E8BC0
    o->vtbl_lock_if_busy(o);                    // vtbl+8 / vtbl+12 pair
    o->vtbl_SetPosition2(o, x, y);
    return 0;
}

// ------------------------------------- Graph90:57 ReplaceSpriteBitmap ---

// sub_4272F0: rebuild the current mode with the existing geometry.
static int sprite_replace_primary(struct CDspObjSprite *s, uint32_t bitmap)
{
    switch (s->mode) {
    case 0: return sprite_mode0(s, bitmap);                                   // sub_427410
    case 2: return sprite_mode2(s, bitmap, s->x16, s->y16, s->rotation,
                                s->scale_x, s->scale_y, s->param_280);        // sub_4275B0
    case 5: return sprite_mode5(s, bitmap, -1, 0, -1, s->x16, s->y16, s->rotation,
                                s->perspective, s->project, s->param_280);    // sub_427AA0
    case 6: return sprite_mode6(s, bitmap, -1, 0, -1, /* quad */ ...);        // sub_427D90
    default: return 0;                     // modes 1/3/4 never look the bitmap up
    }
}

// sub_47C3E0 -> sub_462510 -> sub_43ECA0. Script order (sprite, bitmap).
int Graph90_57_ReplaceSpriteBitmap(struct CThread *t)
{
    uint32_t bitmap = pop(t), sprite = pop(t), status;
    struct CDspObjSprite *s;
    int r;
    check_bitmap(bitmap, t);
    s = (struct CDspObjSprite *)find_object(sprite);
    if (!s) {
        status = 255;
    } else {
        r = sprite_replace_primary(s, bitmap);
        // 0x80000001 (sub_407F20 failed) -> 1; any other failure returns the
        // sprite handle itself, which only matters if it equals 1 or 256.
        status = r == 0 ? 0 : r == (int)0x80000001 ? 1 : sprite;
    }
    if (status == 1)   script_error("bitmap does not exist", t);   // byte_4E8DB4
    if (status == 255) script_error("invalid object", t);          // byte_4E7FAC
    return 0;
}

// --------------------------------- Graph90:5C ConfigureSpriteMode5 -------

// sub_47CC10. Script order: sprite, x16, y16, z16, primary, secondary,
// transition, secondary_param, base_x, base_y, rotation, perspective,
// project, param13, blend, alpha, priority.
int Graph90_5C_ConfigureSpriteMode5(struct CThread *t)
{
    uint32_t a[17];
    int i, status;
    for (i = 16; i >= 0; --i) a[i] = pop(t);
    check_bitmap(a[4], t);
    check_le_256(a[6], t);
    check_blend(a[14], t);
    check_le_256(a[15], t);
    check_priority(a[16], t);
    status = sub_43EAB0(a);       // below
    switch (status) {
    case 1:   script_error("bitmap does not exist", t);       // byte_4E94B8
    case 8:   script_error("projected size too small", t);    // byte_4E9714
    case 255: script_error("invalid object", t);
    }
    return 0;                      // 9 (secondary of another size) is silent
}

// sub_43EAB0 -> sub_427170: position (vtbl+60), blend (sub_41B600), alpha
// (vtbl+72) and priority (vtbl+84) are applied *before* sub_427AA0 checks
// the bitmaps, so a rejected secondary still leaves them changed.
//   sub_427AA0: primary missing 0x80000001, secondary missing 0x80000002
//   (both -> 1); secondary width/height/format differ 0x80000003 (-> 9);
//   projected raster (sub_429220) under 2x2 0x80000004 (-> 8).
// sub_429AF0 then passes (fixed + screen centre) >> 16 to vtbl+40.

// sub_40C0F0 two-bitmap cache (sprite+0x220), transition t in 0..256:
//   format 1 (sub_40C1B0): every byte  c = p + ((s - p) * t >> 8)   (pmulhw)
//   format 2 (sub_40C430): wp = (256 - t) * ap; total = wp + t * as;
//       total == 0 -> 0; else rgb = s + ((p - s) * (wp * 128 / total) >> 7),
//       a = total >> 8
// p = primary (sprite[84]), s = secondary (sprite[85]); sub_42AAA0 may
// substitute a mip level (sprite+0x160 / +0x1C0) when the scale is <= 0.5.

// ------------------------------------------ Graph90:43 BackF (class 4) ---

// sub_47B7C0. Script order: x, y, primary, sx, sy, secondary, mask,
// mask_param, alpha. Alpha > 256 is fatal first (sub_497DB0).
// sub_43D750: sub_43E190(4) makes the current layer object a BackF, then
//   sub_41D350: primary missing -> 1, secondary missing -> 2 (secondary may
//     be 0x7000 black, 0x7001 white, 0x7FFF or -1); stores +0x13C x,
//     +0x140 y, +0x144 primary, +0x14C sx, +0x150 sy, +0x154 secondary;
//   sub_41D440: mask -1 clears +0x15C, else it must exist (3) and be
//     format 3 (4); stores +0x15C mask, +0x160 mask_param;
//   then vtbl+72 stores the alpha. Errors are fatal; state stored by the
//   first stage survives a mask-stage failure.
// Draw (sub_41D590, vtbl+0x84) with a = sub_41B770 (effective alpha):
//   secondary bitmap: copy it (mode 128) at (sx, sy); with a mask, blend
//     the primary through sub_411990(mask, mask_param, a, sub_41D540) (the
//     rule-image wipe), else the primary with mode 1 and parameter a;
//   0x7000 / 0x7001: unless the primary covers the target and there is no
//     mask, fill black / white first; the primary then uses mode 192 / 193
//     (0x7FFF and -1 use 128, or 192 when a != 0), or the masked blend.

// ------------------------------------- Graph90:4C background controls ---

// sub_47C090 -> sub_462330 -> sub_43E490. Script order (draw, active).
// The manager keeps both (+0x54/+0x58; sub_442850 starts with (1, 0)) and
// sub_43E190 re-applies them whenever it recreates the background class.
int Graph90_4C_SetCurrentObjectRenderControls(struct CThread *t)
{
    int active = pop(t), draw = pop(t);
    struct ObjectManager *m = g_objects;               // dword_56674C
    m->back_draw = draw;
    m->back_active = active;
    m->back->vtbl_SetDrawEnabled(m->back, draw);       // vtbl+4, sub_41AE00: +0x14,
                                                       // then every member
    m->back->vtbl_SetActive(m->back, active);          // vtbl+120: +0x138
    manager_invalidate(m);                             // sub_430D10
    return 0;
}

// CDspObjBack (sub_41C220): vtbl+0x0C (sub_41C330) invalidates every frame
// while +0x138 is set; vtbl+0x18 (sub_41C340) clears the target to 0
// (sub_40A620) when +0x138 is 0 or the class draw (vtbl+0x84) fails.

// ------------------------------------------------------ work bitmaps ---

// sub_4794C0 -> sub_442EE0. Script order (bitmap). Creates the bitmap with
// the back buffer's size and format (sub_442E10) and copies the current
// back buffer into it (sub_40ADF0) when it can be locked (sub_442FF0),
// otherwise clears it.
int Graph90_04_CreateWorkBitmap(struct CThread *t)
{
    uint32_t bitmap = pop(t);
    check_bitmap(bitmap, t);
    work_bitmap_capture_screen(bitmap);
    return 0;
}

// sub_4794F0 -> sub_442F80. Script order (bitmap, priority). Creates the
// same bitmap and renders, once, every object whose sort key is <=
// (priority << 16 | 0xFFFF) into it (sub_430D30).
int Graph90_05_CreatePrioritizedWorkBitmap(struct CThread *t)
{
    uint32_t priority = pop(t), bitmap = pop(t);
    check_priority(priority, t);
    check_bitmap(bitmap, t);
    work_bitmap_render_through(bitmap, priority);
    return 0;
}

// ------------------------------------- Graph91:33/36/37 fixed vectors ---

// sub_4819A0 -> sub_443520. Script order (object, x16, y16, z16). Missing
// object is fatal. vtbl+60 (base sub_41B370): when +0x80 (and z == 0 or
// +0x84 == 1) round x/y to whole pixels ((v + 0x8000) & ~0xFFFF); store
// +0x4C/+0x50/+0x54; with +0x7C also vtbl+40 (x >> 16, y >> 16, 1, 1);
// then every member gets vtbl+60(member fixed + new - old). Sprite
// (sub_4282B0) reprojects modes 5/6; BackML (sub_41E000) re-lays out.
// A changed sort key re-sorts the object (sub_443300).

// sub_481A40 -> vtbl+68 (base sub_41B520): primary vector +0x5C..+0x64,
// copied to every member recursively. Missing object is fatal.

// sub_4819F0 -> sub_41B580: secondary vector +0x6C..+0x74, recursively for
// members, then vtbl+68 with this object's own primary vector -- so every
// descendant ends with the root's primary vector. Missing object is fatal.

// sub_47C170 -> sub_462550 -> sub_43EEE0 (x/y travel in esi/edi). Script
// order (sprite, x, y, w, h). Missing sprite -> 255 (fatal). sub_428C00:
// modes 0/1/3 offset (x, y, x+w-1, y+h-1) by the sprite rect (vtbl+36)
// and invalidate it, an empty rect returning 0 -> status 10 (fatal,
// byte_4E9420); mode 5 re-rasterizes its projected cache in that area
// (sub_42A330 / sub_42A770); other modes rebuild the cache for 5/6
// (sub_42A650) and invalidate the whole sprite (vtbl+12).
int Graph90_53_RefreshSpriteRect(struct CThread *t)
{
    int h = pop(t), w = pop(t), y = pop(t), x = pop(t);
    switch (sprite_refresh_rect(pop(t), x, y, w, h)) {
    case 10:  script_error("invalid update region", t);
    case 255: script_error("invalid object", t);
    }
    return 0;
}

// ------------------------------------------- Graph90:28 motion control ---

// sub_47AC80 (12 pops; the object travels in ecx). Script order: object,
// x, y, position_curve, alpha, alpha_curve, fixed_param, duration_ms,
// divisor, numerator, input_enabled, input_priority. Checks: priority <
// 0x10000, fixed_param in -1..256, alpha <= 256. sub_491D60: divisor 0
// (0x80000001) and a missing object (-1) are fatal; it builds a
// CProcCtrlDspObj (sub_431BD0) and sub_431D90 stores the start values
// (vtbl+48 position, vtbl+76 alpha, sub_41B740 fixed parameter) and the
// deltas (fixed_param -1 keeps the current value), the duration (0 -> 1)
// and the update interval 1000 * numerator / divisor; sub_431E80 registers
// the input scope. Returns 2 (procedure installed).
// Tick (sub_432160): with a non-zero numerator the elapsed time is capped
// at the next deadline, which then advances by the interval; progress
// (t << 24) / duration goes through the curve (sub_41A690, linear = / 256)
// and each value = start + (delta * p >> 16).
