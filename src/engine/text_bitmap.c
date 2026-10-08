// Font registry and bitmap text: hand-cleaned from src/raw/text_468000.c,
// text_400000.c, text_42C000.c, text_434000.c, text_484000.c.
#include "../include/engine_types.h"

extern uint32_t pop(struct CThread *t);
extern void *pop_ptr(struct CThread *t);              // sub_48DF50
extern void push(struct CThread *t, uint32_t v);
extern void script_error(const char *msg, struct CThread *t);
extern int bitmap_get(uint32_t handle, struct BitmapDesc *out);
extern int blit(struct BitmapDesc *src, struct BitmapDesc *dst, int y, int x,
                int mode, int param);                 // sub_40A530 (returns 4 if clipped away)

// ------------------------------------------------------- font registry ---

static struct FontName *g_fonts;   // dword_56631C
static int g_next_font_id;         // dword_566314

// sub_468A70: intern `name`; option 0 / 1 also registers charset metadata
// (sub_42D780 with 128 / 0), -1 and other values only intern.
int font_intern(int option, const char *name)
{
    struct FontName *f;
    if (option == 0) register_font_charset(name, 128);
    else if (option == 1) register_font_charset(name, 0);
    for (f = g_fonts; f; f = f->next)
        if (!strcmp(f->name, name))
            return f->id;
    f = new_font_name(g_next_font_id++, strdup(name));
    f->next = g_fonts;
    g_fonts = f;
    return f->id;
}

// sub_468BB0: face name for an id, NULL when unknown.
const char *font_face(int id)
{
    struct FontName *f;
    for (f = g_fonts; f; f = f->next)
        if (f->id == id) return f->name;
    return 0;
}

// sub_468B70 (startup, from text_46C000.c): ids 0 and 1 are the stock
// faces. sub_46F6F0 = (GetSystemDefaultLangID() & 0x3FF) == LANG_JAPANESE.
void font_registry_reset(void)
{
    free_font_names();                       // sub_468B30, id counter -> 0
    font_intern(-1, is_japanese_system() ? "ＭＳ ゴシック" : "MS Gothic");
    font_intern(-1, is_japanese_system() ? "ＭＳ 明朝" : "MS Mincho");
}

// sub_497AF0: a font id that is not registered is a script error.
static const char *require_font(int id, struct CThread *t)
{
    const char *face = font_face(id);
    if (!face) script_error("font is not registered", t);   // byte_4E82C4
    return face;
}

// -------------------------------------------- Graph92:1E DrawBitmapText ---

// CGlyphFont (sub_42F3F0, 0x78 bytes): +0 created, +4 cell height,
// +0x4C HFONT, +0x54 memory DC, +0x58 1-bpp DIB section, +0x5C bits,
// +0x60 DIB pitch. sub_42F5F0: CreateFontA(h, h/2, 0, 0, bold ? 700 : 400,
// 0, 0, 0, japanese ? SHIFTJIS : ANSI, 0, 0, DRAFT_QUALITY, FIXED_PITCH),
// text colour index 1, transparent background.
// sub_42F4F0: heights 8..200 (else 0x80000002), creation failure 0x80000004.

// sub_42FA80: Shift-JIS lead byte.
static int is_lead(uint8_t c) { return c >= 0x80 && (c < 0xA0 || c >= 0xE0); }

// sub_42F790: TextOut one code at (0, 0) into the h x h DIB. 0x7F is drawn
// as U+2014, 0xEF40 as two U+2014; other codes >= 0xEF40 draw nothing.

// sub_403710: expand the 1-bpp cell into `glyph` (format from sub_409080(1):
// the default format, 1 promoted to 2). Set bits: format 0 -> 555 colour,
// 1 -> colour, 2 -> colour | 0xFF000000; clear bits -> all zero.

// sub_403840: draw `text` at (x, y). Returns the accumulated advance in *out.
static void draw_text_cells(const uint8_t *text, struct BitmapDesc *dst, int x0, int y,
                            struct CGlyphFont *font, int spacing, uint32_t color, int *out)
{
    int h = font->height, x = x0, pitch_percent = 100, wrap = 0;
    struct BitmapDesc glyph;
    *out = 0;
    while (*text) {
        int dbcs = is_lead(*text);
        unsigned code = dbcs ? (text[0] << 8) | text[1] : text[0];
        if (code >= 0x20) {
            int w;
            render_glyph(font, code, &glyph, color);     // sub_403710
            w = dbcs ? glyph.width : glyph.width >> 1;  // half cell for 1 byte
            glyph.width = w;
            if (wrap && (unsigned)(x + w) > (unsigned)wrap) {
                x = x0;
                y += h * pitch_percent / 100;
            }
            if (blit(&glyph, dst, y, x, 0, 0))           // fully clipped: stop
                break;
            x += w + spacing;
            *out += w + spacing;
        } else if (code == 3) {                          // 0x03 n: pitch n%
            pitch_percent = text[1];
            ++text;
        } else if (code == 4) {                          // wrap at the width
            wrap = dst->width;
        } else if (code == 10) {
            x = x0;
            y += h * pitch_percent / 100;
        }
        text += dbcs + 1;
    }
}

// sub_485F10 -> sub_4039E0. Script order: bitmap, x, y, text, font, size,
// bold, spacing, colour. Pushes the advance.
int Graph92_1E_DrawBitmapText(struct CThread *t)
{
    uint32_t color = pop(t), spacing = pop(t), bold = pop(t), size = pop(t),
             font_id = pop(t);
    const uint8_t *text = pop_ptr(t);
    int y = pop(t), x = pop(t), advance = x;      // the slot starts as x
    uint32_t bitmap = pop(t);
    struct BitmapDesc dst;
    struct CGlyphFont font;
    const char *face;
    if (bitmap >= 0x4000) script_error("bitmap handle", t);
    require_font(font_id, t);
    if (!bitmap_get(bitmap, &dst)) script_error("bitmap does not exist", t);  // 0x80000004
    face = font_face(font_id);                                                // 0x80000003
    switch (glyph_font_init(&font, size, face, bold)) {                       // sub_42F4F0
    case 0x80000002: script_error("font size", t);                            // 8..200
    case 0: draw_text_cells(text, &dst, x, y, &font, spacing, color, &advance); break;
    }
    push(t, advance);
    return 0;
}

// ----------------------------------------------- Graph92:9C RenderText ---

// sub_4867D0: 21 pops, the first discarded. Script order: bitmap, x, y,
// text, colour, align, ruby text, colour2, font, size, scale %, style,
// flag12, kinsoku, line spacing, shadow mode, shadow x%, shadow y%, shadow
// colour, concentration, (unused).
//   sub_434E30 copies the shadow style when mode <= 2, offsets <= 100 and
//   concentration <= 256 (mode 0 clears it).
//   sub_403B10: bitmap missing 0x80000004; sub_4035A0 -> sub_409290 ->
//   sub_42F0F0 -> sub_42DDF0 -> sub_42EAB0: face length 1..31 (0x80000004
//   -> "font"), size 4..200 (0x80000002 -> "size"), scale 25..200
//   (0x80000003 -> "scale"); all fatal in sub_4867D0.
//   sub_434BA0 stores (x, y) in dword_565D34/38 and runs sub_434C50:
//   sub_435290 layout -> sub_437110 ruby -> sub_437CB0 alignment
//   (1 centre, 2 right) -> sub_437940 blits every record with mode 0.
//
// sub_435290, per character record (0x48 bytes, glyph image at +0x20):
//   dx = max(1, size * x% / 100), dy = max(1, size * y% / 100)
//   mode 1: shadow = glyph with RGB replaced (sub_4188D0), blitted at
//           (dx, dy) with mode 1, parameter 256 - concentration; then the
//           glyph at (0, 0) with mode 0.
//   mode 2: glow = sub_433180: alpha(x, y) = min(255, sum of glyph
//           coverage over [x-2dx, x] x [y-2dy, y]), RGB = shadow colour;
//           blitted at (0, 0) mode 1 parameter 256 - concentration, then
//           the glyph at (dx, dy) mode 0. The record grows by 2dx x 2dy
//           (dx x dy for mode 1).

// --------------------------------------- Graph91:88 ConfigureWindowFont ---

// sub_4840A0 -> sub_462BF0 -> sub_4409C0 -> sub_42C3B0. Script order:
// window, font, size, scale %, style, layout option, render option.
int Graph91_88_ConfigureWindowFont(struct CThread *t)
{
    uint32_t render = pop(t), layout = pop(t), style = pop(t), scale = pop(t),
             size = pop(t), font_id = pop(t), window = pop(t);
    struct CDspObjWindow *w;
    const char *face = require_font(font_id, t);
    w = find_window(window);                         // sub_4406C0: 0xB0..., slot < 16
    if (!w) script_error("invalid window", t);       // 255 -> -1, byte_4E9AB8
    w->layout_option = layout;                       // +0x354 (sub_42C510)
    w->render_option = render;                       // +0x364 (sub_42C530)
    switch (window_font_create(w, face, size, scale, style)) {  // sub_409290, font at +0x350
    case 0x80000002: script_error("font size", t);   // 4..200
    case 0x80000003: script_error("font scale", t);  // 25..200
    case 0x80000004: script_error("font", t);
    }
    w->font_size = size;                             // +0x358
    w->glyph_width = scale * size / 100;             // +0x35C
    if (w->render_option) {                          // vertical text keeps a column free
        struct Rect valid = w->valid, bmp = rect_of(&w->bitmap);
        if (valid.right > bmp.right - w->glyph_width) {
            valid.right = bmp.right - w->glyph_width;
            window_set_valid_rect(w, &valid);        // sub_42B900 (+ cursor, sub_42C5B0)
        }
    }
    return 0;
}

// ------------------------------------ Graph90:B7 ApplyIconInputLayoutEx ---

// sub_47EE60 -> sub_46CDB0. Script order (window, descriptor). Pushes
// 0 ok, 1 not a window, 2 bad root, 3 bad group/item table.
//   sub_46C9D0 copies the 40-byte root, 64-byte groups and 196-byte items;
//   it checks the group count (1..256) and each group's *low word* count.
//   sub_44B260 clears the window's icon sprites, then rechecks every group
//   count dword (status 3) and, for each item with item+4 != 0 whose bitmap
//   resolves, sub_42BED0 builds a mode-5 sprite:
//     bitmap  = item+0x20, or item+0x28 for the group's current item
//               (group+0x0C) when that is not -1
//     position item+0x08 + item+0x10, item+0x0C + item+0x14, origin
//               (item+0x10, item+0x14)
//     depth   = item+4 if item+0xC0 & 0x10, item+0x0C if & 2, else the
//               running item ordinal

// ------------------------------------------------ ruby substitutions ---

// sub_484740 -> sub_4632A0 -> sub_434920. Script order (output, source).
// For every position where the ruby dictionary (unk_565BB4, sub_4348C0)
// matches, sprintf(out, "%s\\%s\n", base, reading) is appended and the
// scan skips the rest of the match (sub_434B50(0) - 1 characters).
// Returns the match count; no match leaves the buffer untouched.
int Graph91_95_CollectRubyMatches(struct CThread *t)
{
    const char *source = pop_ptr(t);
    char *out = pop_ptr(t);
    push(t, ruby_collect(source, out));
    return 0;
}
