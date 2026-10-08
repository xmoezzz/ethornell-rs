// Window objects (CDspObjWindow, handles 0xB0000000 | slot, 16 slots at
// dword_56674C + 4 * (573 + slot), live count at +589 dwords).
// Hand-cleaned from src/raw/text_440000.c, text_46C000.c, text_47C000.c.
#include "../include/engine_types.h"

extern uint32_t pop(struct CThread *t);
extern void push(struct CThread *t, uint32_t v);
extern void script_error(const char *msg, struct CThread *t);
extern struct CDspObj *find_object(uint32_t handle);   // sub_443270

// sub_4406C0: resolve a window handle.
struct CDspObjWindow *find_window(uint32_t handle)
{
    uint32_t slot = handle & 0xFFFFFF;                  // sub_443260
    if ((handle & 0xFF000000) != 0xB0000000 || slot >= 16)
        return 0;
    return g_objects->windows[slot];
}

// sub_47DB60. Script order (window).
int Graph90_81_ReleaseWindowObject(struct CThread *t)
{
    uint32_t handle = pop(t);
    struct CDspObj *o = find_object(handle);
    if (o && o->input_lock)                 // +0x130 (sub_41AD50), set by DCIPIcon
        script_error("window is used by an input processor", t);   // byte_4E9A48
    if (o && o->parent)                     // +0x11C (sub_41ACE0)
        script_error("window is attached to a parent", t);         // byte_4E9A88
    if (!window_release(handle))            // sub_440700: unlink from the
        script_error("invalid window", t);  // manager (sub_430770), delete, --count
    return 0;
}

// -------------------------------------------- DCIPIcon input processors ---
// Handle registry: list dword_5667E8 of {handle, object, next}, count
// dword_5667E0 (sub_46C4A0 registers, sub_46C4F0 removes).

// sub_47EEA0 -> sub_46C630(window, 0): DCIPIcon (0xA4 bytes, sub_447990);
// kind 1 is DCIPIconEx (0xD8 bytes, sub_44A7C0). The constructor locks the
// window (+0x130), creates the overlay child (sub_42AC50) and links it to
// the window at (0, 0) (sub_41AB40). A non-window handle passes NULL.
int Graph90_B8_CreateIconInputProcessor(struct CThread *t)
{
    struct CDspObjWindow *w = find_window(pop(t));
    struct DCIPIcon *p = DCIPIcon_new(w);
    push(t, p ? icon_registry_add(p) : 0);
    return 0;
}

// sub_47EED0 -> sub_46C4F0. Pushes 1 if the handle was registered.
//   ~DCIPIcon (sub_447B10): sub_44A050 drops the window/overlay input scopes
//   (sub_46DF00), unlinks (sub_41AC40) and deletes every item sprite and its
//   pointer node (sub_46D800); when active (+0x40) it also removes its
//   keyboard-chain, pointer-chain and scope nodes (sub_46D7A0, sub_46D7B0,
//   sub_46E550); then the overlay child is unlinked and deleted and the
//   window lock is cleared (sub_447740 -> sub_41AD10).
int Graph90_B9_ReleaseIconInputProcessor(struct CThread *t)
{
    push(t, icon_registry_remove(pop(t)));
    return 0;
}

// ---------------------------------------- DCIndProc state and messages ---
// DCIndProc (sub_4476D0): +0x04 handle (++dword_565D74), +0x08 128,
// +0x0C window, +0x10 enabled (1), +0x18..+0x20 message queue
// {count, words, next} (sub_4477E0 appends, sub_447930 pops).

// sub_48A1B0. Script order (handle, value). Pushes whether it exists.
int Sys80_A8_SetRegisteredObjectState(struct CThread *t)
{
    uint32_t value = pop(t);
    struct DCIndProc *p = icon_registry_find(pop(t));     // sub_46C4D0
    if (p) p->enabled = value;                           // sub_41ADE0
    push(t, p != 0);
    return 0;
}

// sub_48A200. Script order (handle, out). Writes +0x10, pushes existence.
int Sys80_A9_GetRegisteredObjectState(struct CThread *t)
{
    uint32_t *out = pop_ptr(t);
    struct DCIndProc *p = icon_registry_find(pop(t));
    if (p) *out = p->enabled;                            // sub_4477B0
    push(t, p != 0);
    return 0;
}

// sub_48A250. Script order (handle, count, words). A count outside 1..256
// queues nothing; the result only reports whether the handle exists.
int Sys80_AC_QueueRegisteredObjectMessage(struct CThread *t)
{
    const uint32_t *words = pop_ptr(t);
    uint32_t count = pop(t);
    struct DCIndProc *p = icon_registry_find(pop(t));
    if (p && count - 1 <= 0xFF)
        indproc_queue_message(p, count, words);
    push(t, p != 0);
    return 0;
}

// Main loop, after the scheduler: sub_46C570 (or sub_46C5B0) walks the
// registry newest first and, for each processor with enabled != 0, drains
// its queue (sub_447860): a message whose first word is 0 sets `enabled`
// from word 1 when it has two words; any other goes to vtable+8.
static void indproc_drain(struct DCIndProc *p)
{
    uint32_t words[256];
    int count;
    while ((count = indproc_pop_message(p, words)) > 0) {
        if (words[0])
            p->vtbl->message(p, count, words);
        else if (count == 2)
            p->enabled = words[1];
    }
}

// sub_44A250, DCIPIcon(Ex) vtable+8. Word counts must match exactly.
//   0x10000000 (3) vt+0x2C(group = (short)(w1 >> 16), item = (short)w1, w2):
//        sub_44A000 stores +0x68/+0x6C/+0x70 (the Graph90:BC record); Ex
//        (sub_44C230) queues 0x10000007 {packed, pointer offset} when w2 != 0
//        and 0x10000006 {packed or -1, w2}; then vt+0x14 (1 base, 0 Ex)
//        clears +0x30 (running).
//   0x10000001 (2) sub_449A60(w1): current group +0x3C when the group's
//        selection flag (+0x0C) is set; exclusion via sub_449D60.
//   0x10000002 (3) vt+0x24(group, item): set the group's current item
//        (-1 clears); unchanged -> 0; exclusion clears peer groups with the
//        same key (+0x18). Base stops before the exclusion when the item's
//        normal bitmap is -1.
//   0x10000003 (2) +0x88 = w1 (pointer processing).
//   0x10000004 (5) vt+0x38(group, item, field, value): Ex fields 0..3 =
//        item +0x20/+0x24/+0x28/+0x2C (normal, hover, selected,
//        hover-selected); base fields 0 normal, 1 hover, 2 selected,
//        4 hit mask (-2 sub_41BDD0, -1 clear, else bitmap).
//   0x10000005 (4) sub_44A600 -> vt+0x3C(group, item, item x, item y, z).
//   0x10000006 (5) vt+0x40: Ex item +0x18/+0x1C = w3/w4.
//   0x10000007 (6) vt+0x3C(group, item, x, y, z): Ex moves the item and
//        sets its sprite to ((x + ox - sw/2) << 16, ..., z << 16)
//        (sub_42C0D0); the base class only validates.

// sub_47DE90 -> sub_462B20 -> sub_4408D0 -> sub_42B970. Script order
// (window, x, y, width, height). sub_42B900 accepts the rect (x, y,
// x+w-1, y+h-1) when left and right lie in [0, bitmap width) and top and
// bottom in [0, bitmap height) -- left <= right is not required -- stores
// it at +0x1A0..+0x1AC and resets the text cursor (sub_42C5B0: variant 0
// (left, top), variant 1 (right, top)). A rejected rect (4 -> 1) and a
// missing window (255 -> -1) are script errors.
int Graph90_88_SetWindowValidRegion(struct CThread *t)
{
    int h = pop(t), w = pop(t), y = pop(t), x = pop(t);
    struct CDspObjWindow *win = find_window(pop(t));
    struct Rect r = { x, y, x + w - 1, y + h - 1 };
    if (!win) script_error("invalid window", t);                 // byte_4E9AB8
    if (!window_set_valid_rect(win, &r)) script_error("valid region", t); // byte_4E9BF8
    return 0;
}
