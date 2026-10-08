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
