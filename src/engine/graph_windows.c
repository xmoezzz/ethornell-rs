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
