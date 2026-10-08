// System input selectors (Sys80/Sys81). Hand-cleaned from
// src/raw/text_484000.c, text_48C000.c, text_45C000.c, text_468000.c.
#include "../include/engine_types.h"

extern uint32_t pop(struct CThread *t);
extern void push(struct CThread *t, uint32_t v);
extern void script_error(const char *msg, struct CThread *t);

// sub_45E8D0(out, x, y, direction): map between window client and game
// coordinates for the current display mode (direction 1 = window -> game).
//   windowed (sub_45F640() == 0): x * game_w / client_w with (int) casts
//   fullscreen stretch: same with the screen size
//   fullscreen centre (mode 2): subtract half the size difference
//   fullscreen fit: scale = min(sw / gw, sh / gh); subtract the letterbox
//   offset ((s - (int)(g * scale)) >> 1) and divide by the scale, (int)
extern void map_window_point(int out[2], int x, int y, int direction);

// sub_48E680
static void cursor_point(int out[2])
{
    RECT r; POINT p;
    if (!g_window_created) {                 // dword_5666F0, WM_CREATE / WM_DESTROY
        out[0] = out[1] = 0;
        return;
    }
    if (g_touch_points) {                    // dword_56669C, WM_TOUCH contacts
        out[0] = g_touch_points->x;          // (sub_46ADB0 stores them already
        out[1] = g_touch_points->y;          //  mapped with direction 1)
        return;
    }
    GetWindowRect(g_hwnd, &r);
    GetCursorPos(&p);
    if (is_fullscreen())                     // sub_45F640
        map_window_point(out, p.x - r.left, p.y - r.top, 1);
    else
        map_window_point(out, p.x - (r.left + GetSystemMetrics(SM_CXFIXEDFRAME)),
                         p.y - r.top - GetSystemMetrics(SM_CYFIXEDFRAME)
                             - GetSystemMetrics(SM_CYCAPTION), 1);
}

// sub_487E50. Pushes x, then y.
int Sys80_08_ReadCursorPoint(struct CThread *t)
{
    int p[2];
    cursor_point(p);
    push(t, p[0]);
    push(t, p[1]);
    return 0;
}

// sub_488320 -> sub_48E780 (duration travels in edi). Script order: x, y,
// curve, duration_ms, updates_per_second, cancel_on_user_move. Nothing
// happens while the window is minimised. The start is the current cursor
// (sub_48E680); steps = max(ups * duration / 1000, 1); step k is due at
// start_tick + duration * k / steps. sub_48E930 (every frame, while the app
// is active) cancels when cancel_on_user_move is set and the cursor moved
// more than one scaled pixel from the last generated point, else advances:
//   linear: start + (delta * ((k << 16) / steps) >> 16)        (32-bit)
//   curve 1: f = (int)((cos(a * pi / 46080) + 1) * 32768),
//            a = 46080 - 46080 * k / steps (integer); start + (delta * f >> 16)
// and moves the OS cursor there (sub_48E640); the last step lands on target.
int Sys80_1F_ConfigureCursorMotion(struct CThread *t)
{
    uint32_t cancel = pop(t), ups = pop(t), duration = pop(t), curve = pop(t);
    int y = pop(t), x = pop(t);
    cursor_motion_start(duration, x, y, curve, ups, cancel);
    return 0;
}
