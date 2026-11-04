// BGI 1.622 Graph92:1C/1D immediate text, 4866A0/486850 -> 403600/403680
// -> 4035A0 + 4097F0. Raw functions in native_all.c/.asm; hanging string in
// text_tables.json at 4E50F4. Rust: snapshot::layout_styled_bitmap_text.
// Source: bitmap,x,y,text,font,height,width_percent,bold,proportional,RGB,
// and (1D only) extra_line_pitch_percent. Height 4..200, width 25..200.
// These are plain SJIS strings: angle brackets/control bytes are glyphs,
// except LF. They do not use message markup, tags or ruby processing.
// 409280 caches the font and 4092D0 produces antialiased glyph cells with
// packed RGB and the glyph mask as alpha. Composition is native mode 0.
// Host font outlines come from the configured CPU font session, replacing GDI.

// Fixed advance is (height*width_percent/100) for DBCS, half for SBCS.
// Proportional mode crops to the glyph's ink rectangle and adds two margins.
static int glyph_margin(int ink_width,int height) { // 409700
    static const int table[8]={4,4,4,3,3,3,3,2};
    int ratio=16*ink_width/height;
    return ratio>=8 ? 1 : table[ratio];
}
// 4097A0: newline advances height + height*extra_percent/100, truncating.
// 1C returns the MAXIMUM drawn line advance (not total across all lines).
// 1D reserves one full-width glyph at the bitmap's right edge before deciding
// to wrap. It allows one hanging punctuation character past this bound; the
// next overflow wraps. The new line starts at the original caller X.
// A glyph entirely outside the bitmap terminates the loop. Outputs update
// only AFTER successful glyph composition: empty text returns zero, and a
// final LF alone does not increase 1D's returned count.
// 4E50F4 hanging characters:
// ,.､｡:;?!ﾞﾟ･、。，．：；？！゛゜‐]})）〕］｝〉》」』】‘“
