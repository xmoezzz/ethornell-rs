//! Line layout of the target text engine for text drawn into a bitmap
//! (`Graph91:9C` and `Graph92:9C`: `sub_403B10 -> sub_434BA0 -> sub_434C50 ->
//! sub_435290`).
//!
//! The pixel rasteriser is separate; this module only decides where each
//! glyph goes and how many lines the text needs. The line count is what the
//! selectors return to the script (`sub_435290` writes `1` through its first
//! argument and increments it for every wrap or newline).
//!
//! Rules recovered from `sub_435290`:
//! * the wrap bound is the destination bitmap's right edge (`rect.right + 1`);
//! * a following run of line-start-prohibited characters (`unk_4E52C0`) must
//!   stay with the current glyph, and an opening bracket (`unk_4E5374`) must
//!   not end a line, so their widths are added to the width a glyph needs;
//! * with kinsoku on, a glyph whose run does not end in a hanging character
//!   (`unk_4E5328`) must also leave one font height free at the right edge;
//! * a first glyph in `「　（『` (`unk_4E5318`) indents every later line by its
//!   own width;
//! * a new line starts at the rectangle's left edge (plus the indent), and
//!   the pitch is `height + height * percent / 100` (`sub_4097B0`).

const LINE_START_PROHIBITED: &str =
    "\"':;?!ﾞﾟ･，．、。：；？！”゛゜‐]})）〕］｝〉≫》」』】ヽヾゝゞ々ー～っゃゅょッャュョ";
const LINE_END_PROHIBITED: &str = "[{(（〔［｛〈≪《「『【“";
pub(crate) const HANGING: &str = "?!ﾞﾟ，．、。？！”゛゜]})）〕］｝〉≫》」』】ヽヾゝゞ々ー～っゃゅょッャュョ";
const INDENT_OPENERS: &str = "「　（『";

#[derive(Debug, Clone, Copy)]
pub(crate) struct LayoutParams {
    /// Cursor x/y at the start of the text.
    pub(crate) x: f32,
    pub(crate) y: f32,
    /// Font height in pixels.
    pub(crate) size: f32,
    /// Width of the destination bitmap (the wrap bound).
    pub(crate) bitmap_width: f32,
    /// Extra advance per glyph (`dword_565BB0`).
    pub(crate) spacing: f32,
    /// Line spacing percent of the font height (script argument).
    pub(crate) pitch_percent: i32,
    /// Script kinsoku argument (`a11` of `sub_435290`).
    pub(crate) kinsoku: bool,
    /// Global `dword_565CF0`: gates only the first-glyph bracket indent.
    pub(crate) indent_brackets: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub(crate) struct PlacedGlyph {
    pub(crate) ch: char,
    /// Index of the character in the laid-out text.
    pub(crate) index: usize,
    pub(crate) x: f32,
    pub(crate) y: f32,
}

#[derive(Debug, Clone, PartialEq)]
pub(crate) struct TextLayout {
    pub(crate) glyphs: Vec<PlacedGlyph>,
    /// Number of lines, starting at one.
    pub(crate) lines: i32,
    pub(crate) end_x: f32,
    pub(crate) end_y: f32,
}

pub(crate) fn line_pitch(size: f32, percent: i32) -> f32 {
    let height = size.max(1.0) as i32;
    (height + height * percent / 100) as f32
}

pub(crate) fn layout_text(
    text: &str,
    params: &LayoutParams,
    advance_of: &dyn Fn(char) -> f32,
) -> TextLayout {
    layout_text_variable(text,params,&|_,ch|advance_of(ch),&|_|params.size,&[],false).0
}

/// The modern renderer measures per-character font sizes and records the
/// maximum height at every wrap/newline. `mxfs` can replace that maximum;
/// ordinary `fs` only raises it. Capture changes line pitch in 074C20.
pub(crate) fn layout_text_variable(
    text:&str,params:&LayoutParams,advance_of:&dyn Fn(usize,char)->f32,
    size_of:&dyn Fn(usize)->f32,metrics:&[(usize,bool,i32)],capture:bool,
)->(TextLayout,Vec<i32>) {
    let chars = text.chars().collect::<Vec<_>>();
    let pitch = line_pitch(params.size, params.pitch_percent);
    let step = |index:usize,ch: char| advance_of(index,ch) + params.spacing;
    let right = params.bitmap_width;

    let indent = if params.kinsoku && params.indent_brackets && chars.first().is_some_and(|c| INDENT_OPENERS.contains(*c)) {
        step(0,chars[0])
    } else {
        0.0
    };
    let line_start = indent;

    let mut glyphs = Vec::with_capacity(chars.len());
    let mut x = params.x;
    let mut y = params.y;
    let mut lines = 1;
    let mut at_line_start = true;
    // Characters carried along with a hanging run keep the hanging bound, so
    // `町。` is placed (or wrapped) as a unit instead of leaving a lone `。`.
    let mut hang_through = 0usize;
    let mut maximum=params.size;
    let mut heights=Vec::new();
    let mut metric=0usize;
    let mut line_glyph_start=0;
    let finish_line=|glyphs:&mut Vec<PlacedGlyph>,start:usize,maximum:f32| {
        for glyph in &mut glyphs[start..] {glyph.y+=(maximum-size_of(glyph.index)).max(0.);}
    };
    let height_of=|maximum:f32|pitch+if capture {(maximum-params.size).max(0.)} else {0.};
    for (index, &ch) in chars.iter().enumerate() {
        while metric<metrics.len()&&metrics[metric].0<=index {
            let (_,set,size)=metrics[metric];
            maximum=if set {size as f32} else {maximum.max(size as f32)};metric+=1;
        }
        if (ch as u32) < 0x20 {
            if ch == '\n' {
                x = line_start;
                finish_line(&mut glyphs,line_glyph_start,maximum);
                let height=height_of(maximum);heights.push(height as i32);y+=height;
                line_glyph_start=glyphs.len();maximum=size_of(index);
                lines += 1;
                at_line_start = true;
            }
            continue;
        }
        let mut needed = step(index,ch);
        let mut hang = index < hang_through;
        if params.kinsoku {
            let run = chars[index + 1..]
                .iter()
                .take_while(|c| LINE_START_PROHIBITED.contains(**c))
                .copied()
                .collect::<Vec<_>>();
            if run.is_empty() {
                if LINE_END_PROHIBITED.contains(ch) {
                    if let Some(next) = chars.get(index + 1).filter(|c| (**c as u32) >= 0x20) {
                        needed += step(index+1,*next);
                    }
                }
            } else {
                needed += run.iter().enumerate().map(|(n,c)| step(index+1+n,*c)).sum::<f32>();
                hang = run.last().is_some_and(|c| HANGING.contains(*c));
                if hang {
                    hang_through = index + 1 + run.len();
                }
            }
        }
        let bound = right - if params.kinsoku && !hang { params.size } else { 0.0 };
        if !at_line_start && x + needed > bound {
            x = line_start;
            finish_line(&mut glyphs,line_glyph_start,maximum);
            let height=height_of(maximum);heights.push(height as i32);y+=height;
            line_glyph_start=glyphs.len();maximum=size_of(index);
            lines += 1;
        }
        glyphs.push(PlacedGlyph { ch, index, x, y });
        x += step(index,ch);
        at_line_start = false;
    }
    while metric<metrics.len() {
        let (_,set,size)=metrics[metric];maximum=if set {size as f32} else {maximum.max(size as f32)};metric+=1;
    }
    finish_line(&mut glyphs,line_glyph_start,maximum);
    heights.push(height_of(maximum) as i32);
    (TextLayout {glyphs,lines,end_x:x,end_y:y},heights)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn params(width: f32, kinsoku: bool) -> LayoutParams {
        LayoutParams {
            x: 0.0,
            y: 0.0,
            size: 10.0,
            bitmap_width: width,
            spacing: 0.0,
            pitch_percent: 0,
            kinsoku,
            indent_brackets: false,
        }
    }

    fn full(_: char) -> f32 {
        10.0
    }

    fn rows(layout: &TextLayout) -> Vec<String> {
        let mut out: Vec<String> = Vec::new();
        let mut last_y = f32::NAN;
        for glyph in &layout.glyphs {
            if glyph.y != last_y {
                out.push(String::new());
                last_y = glyph.y;
            }
            out.last_mut().unwrap().push(glyph.ch);
        }
        out
    }

    #[test]
    fn version_text_font_commands_capture_actual_line_pitch() {
        use crate::{text_anim::parse_message_markup_with_version,ruby_registry::RubyRegistry};
        let input="a<fs 40>b</fs>\nc<mxfs 60>d<2x>e</2x>";
        let parsed=parse_message_markup_with_version(input,&mut RubyRegistry::default(),None,true);
        let style=|i:usize|parsed.style_spans.iter().find(|s|s.start_char<=i&&i<s.end_char).map(|s|s.style).unwrap_or_default();
        let size=|i:usize|style(i).font_size.unwrap_or(20) as f32;
        let mut p=params(1000.,false);p.size=20.;p.pitch_percent=20;
        let (layout,heights)=layout_text_variable(&parsed.text,&p,&|i,_|size(i)*0.5*(1<<style(i).width_power) as f32,&size,&parsed.line_metrics,true);
        assert_eq!(heights,[44,64]);assert_eq!(layout.lines,2);
        assert_eq!(layout.glyphs[0].y,20.);assert_eq!(layout.glyphs[1].y,0.);
        assert_eq!(style(5).width_power,1);assert!(style(5).bold);
        let legacy=parse_message_markup_with_version(input,&mut RubyRegistry::default(),None,false);
        assert!(legacy.style_spans.is_empty());assert!(legacy.line_metrics.is_empty());
    }
    #[test]
    fn empty_text_is_one_line() {
        assert_eq!(layout_text("", &params(100.0, false), &full).lines, 1);
    }

    #[test]
    fn wraps_at_the_bitmap_width_and_counts_lines() {
        let layout = layout_text("あいうえおかきくけこ", &params(50.0, false), &full);
        assert_eq!(rows(&layout), ["あいうえお", "かきくけこ"]);
        assert_eq!(layout.lines, 2);
        let layout = layout_text("あいうえおか", &params(50.0, false), &full);
        assert_eq!(layout.lines, 2);
        // exactly full is one line
        assert_eq!(layout_text("あいうえお", &params(50.0, false), &full).lines, 1);
    }

    #[test]
    fn explicit_newlines_count_and_return_to_the_left_edge() {
        let mut p = params(200.0, false);
        p.x = 30.0;
        let layout = layout_text("あい\nう", &p, &full);
        assert_eq!(layout.lines, 2);
        assert_eq!(layout.glyphs[0].x, 30.0);
        assert_eq!(layout.glyphs[2].x, 0.0);
        assert_eq!(layout.glyphs[2].y, 10.0);
    }

    #[test]
    fn line_pitch_adds_a_percentage_of_the_height() {
        let mut p = params(200.0, false);
        p.size = 14.0;
        p.pitch_percent = 15;
        // 14 + 14 * 15 / 100 = 16 (integer arithmetic of sub_4097B0)
        let layout = layout_text("あ\nい", &p, &full);
        assert_eq!(layout.glyphs[1].y, 16.0);
    }

    #[test]
    fn kinsoku_keeps_closing_punctuation_with_its_glyph() {
        // width 50 with kinsoku leaves one glyph (10 px) free: bound 40.
        let layout = layout_text("あいうえお。か", &params(50.0, true), &full);
        let r = rows(&layout);
        // 'お' needs お + 。(hanging, bound 50): fits exactly at x = 40 + 20 = 60? no:
        // お at x=40 needs 20 -> 60 > 50, so お moves to the next line with 。
        assert_eq!(r[0], "あいうえ");
        assert!(r[1].starts_with("お。"));
    }

    #[test]
    fn a_hanging_run_is_not_split_from_its_glyph() {
        // width 50, kinsoku: bound 40 for plain glyphs, 50 for hanging ones.
        // 'え' at x=30 plus '。' (hanging) ends at 50: both stay on line one.
        let layout = layout_text("あいう\u{3048}。か", &params(50.0, true), &full);
        let r = rows(&layout);
        assert_eq!(r[0], "あいうえ。", "{r:?}");
        assert_eq!(r[1], "か");
    }

    #[test]
    fn opening_bracket_never_ends_a_line() {
        // Without kinsoku nothing protects the bracket.
        let layout = layout_text("あいうえ「お", &params(50.0, false), &full);
        assert_eq!(rows(&layout), ["あいうえ「", "お"]);
        // With kinsoku the bracket needs its successor's width as well, so it
        // moves to the next line together with it.
        let layout = layout_text("あいうえ「お", &params(60.0, true), &full);
        let r = rows(&layout);
        assert_eq!(r.len(), 2, "{r:?}");
        assert!(r[1].starts_with('「'), "{r:?}");
    }

    #[test]
    fn leading_bracket_indents_wrapped_lines() {
        let mut p = params(60.0, true);
        p.indent_brackets = true;
        let layout = layout_text("「あいうえおかき」", &p, &full);
        assert!(layout.lines >= 2);
        let second_row_x = layout
            .glyphs
            .iter()
            .find(|g| g.y > 0.0)
            .map(|g| g.x)
            .unwrap();
        assert_eq!(second_row_x, 10.0);
    }

    #[test]
    fn spacing_is_added_to_every_glyph() {
        let mut p = params(1000.0, false);
        p.spacing = 2.0;
        let layout = layout_text("あい", &p, &full);
        assert_eq!(layout.glyphs[1].x, 12.0);
        assert_eq!(layout.end_x, 24.0);
    }
}
