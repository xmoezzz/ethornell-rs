/*
 * BGI 1.622 font enumeration, cleaned from src/raw/bgi1622/native_all.c
 * and native_all.asm. Covers B0:C4 sub_479D60 -> sub_4694B0 ->
 * sub_42DEB0 and callback sub_42F030.
 *
 * The argument is an output pointer. It is not a font family filter.
 * The Rust VM writes the list from SoundApi::user_font_families.
 * The portable app lists its two stock faces through its CJK substitute.
 */

typedef struct FontEnumeration {
    unsigned count;
    char *destination;
    unsigned bytes;
} FontEnumeration;

/* sub_42F030: EnumFontFamiliesExA callback for charset 128. */
int enumerate_font_callback(const LOGFONTA *font, const TEXTMETRICA *metrics,
                            unsigned type, FontEnumeration *state)
{
    unsigned length = strlen(font->lfFaceName) + 1;
    ++state->count;
    if (state->destination) {
        memcpy(state->destination, font->lfFaceName, length);
        state->destination += length;
    }
    state->bytes += length;
    return 1;
}

/* sub_42DEB0: return bytes for null, otherwise write names and return count. */
unsigned enumerate_fonts(char *destination)
{
    LOGFONTA filter = {0};
    filter.lfCharSet = 128;
    FontEnumeration state = {0, destination, 0};
    HDC dc = GetDC(0);
    EnumFontFamiliesExA(dc, &filter, enumerate_font_callback, &state, 0);
    ReleaseDC(0, dc);
    return destination ? state.count : state.bytes;
}

/* sub_479D60: one pointer argument, one result. */
int user_b0_c4(CThread *thread)
{
    char *destination = pop_pointer(thread);
    push_int(thread, enumerate_fonts(destination));
    return 0;
}
