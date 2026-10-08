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
