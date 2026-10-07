use crate::ruby_registry::RubyRegistry;

#[derive(Debug, Clone)]
pub(crate) struct TextRuntime {
    pub(crate) full_text: String,
    pub(crate) visible_chars: usize,
    /// Character-count boundary reached after each timed glyph. Newline
    /// control records are folded into the preceding boundary because target
    /// sub_433960 processes them without consuming a glyph-delay interval.
    reveal_boundaries: Vec<usize>,
    /// Number of timed glyph boundaries already revealed.
    revealed_glyphs: usize,
    /// Remaining real milliseconds until the next glyph reveal.
    pub(crate) next_glyph_remaining_ms: u32,
    /// Script-configured glyph cadence from Graph90:94.
    pub(crate) glyph_delay_ms: u32,
    pub(crate) target_node: Option<i32>,
    pub(crate) history: Vec<String>,
    pub(crate) ruby_spans: Vec<RuntimeRubySpan>,
    pub(crate) style_spans: Vec<RuntimeTextStyleSpan>,
    pub(crate) link_spans: Vec<RuntimeLinkSpan>,
    /// Glyph-delay units to wait before each timed glyph. Without `<t>`
    /// records every entry is one.
    reveal_wait_units: Vec<u32>,
    timeline_events: Vec<TimelineEvent>,
    /// Real milliseconds since the message started revealing.
    timeline_ms: u64,
    fired_events: Vec<[i32; 3]>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct RuntimeRubySpan {
    pub(crate) start_char: usize,
    pub(crate) end_char: usize,
    pub(crate) reading: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub(crate) struct RuntimeTextStyle {
    pub(crate) packed_rgb: Option<u32>,
    pub(crate) bold: bool,
    pub(crate) italic: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct RuntimeTextStyleSpan {
    pub(crate) start_char: usize,
    pub(crate) end_char: usize,
    pub(crate) style: RuntimeTextStyle,
}

/// One `<l>`..`</l>` fragment. The target keeps it in a 16-entry global table
/// (`sub_437E40`) that `92:9E` drains.
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct RuntimeLinkSpan {
    pub(crate) start_char: usize,
    pub(crate) end_char: usize,
    /// Raw message text between the tags (markup included), cut to the
    /// target's 95-byte record limit.
    pub(crate) text: String,
}

/// Side-band records `sub_435290` appends to the glyph chain without drawing
/// a glyph. `at_char` is the number of visible characters that precede them.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum RuntimeTimelineMark {
    /// `<t N>`: the running reveal time (`v180`) becomes `N` glyph delays.
    SetTime { at_char: usize, units: u32 },
    /// `<ev N>`: posts host event `0x30000001` with the per-message ordinal
    /// and `N` (`-1` when absent) once the running reveal time is reached.
    Event {
        at_char: usize,
        ordinal: i32,
        id: i32,
    },
}

/// Host event code posted by a reveal-time `<ev>` record (`sub_437620`).
pub(crate) const MESSAGE_EVENT_CODE: i32 = 0x3000_0001;

#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub(crate) struct ParsedMessageMarkup {
    pub(crate) text: String,
    pub(crate) ruby_spans: Vec<RuntimeRubySpan>,
    pub(crate) style_spans: Vec<RuntimeTextStyleSpan>,
    pub(crate) link_spans: Vec<RuntimeLinkSpan>,
    pub(crate) timeline: Vec<RuntimeTimelineMark>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct TimelineEvent {
    /// Reveal time, in glyph-delay units, at which the record fires.
    fire_unit: u32,
    ordinal: i32,
    id: i32,
    fired: bool,
}

#[derive(Debug, Default)]
struct RevealPlan {
    initial_visible_chars: usize,
    boundaries: Vec<usize>,
    wait_units: Vec<u32>,
    events: Vec<TimelineEvent>,
}

impl Default for TextRuntime {
    fn default() -> Self {
        Self {
            full_text: String::new(),
            visible_chars: 0,
            reveal_boundaries: Vec::new(),
            revealed_glyphs: 0,
            next_glyph_remaining_ms: 0,
            glyph_delay_ms: 16,
            target_node: None,
            history: Vec::new(),
            ruby_spans: Vec::new(),
            style_spans: Vec::new(),
            link_spans: Vec::new(),
            reveal_wait_units: Vec::new(),
            timeline_events: Vec::new(),
            timeline_ms: 0,
            fired_events: Vec::new(),
        }
    }
}

impl TextRuntime {
    pub(crate) fn start_message(&mut self, text: String, target_node: i32) {
        let parsed = parse_message_markup_styled(&text);
        self.start_styled_message(parsed, target_node);
    }

    pub(crate) fn set_glyph_delay_ms(&mut self, delay_ms: i32) {
        self.glyph_delay_ms = delay_ms.max(0) as u32;
        if self.revealed_glyphs < self.reveal_boundaries.len() {
            self.next_glyph_remaining_ms = self.glyph_wait_ms(self.revealed_glyphs);
        }
    }

    /// Milliseconds the glyph at `index` waits after its predecessor.
    fn glyph_wait_ms(&self, index: usize) -> u32 {
        let units = self.reveal_wait_units.get(index).copied().unwrap_or(1);
        self.glyph_delay_ms.saturating_mul(units)
    }

    pub(crate) fn start_styled_message(&mut self, parsed: ParsedMessageMarkup, target_node: i32) {
        self.full_text = parsed.text;
        let plan = build_reveal_plan(&self.full_text, &parsed.timeline);
        self.visible_chars = plan.initial_visible_chars;
        self.reveal_boundaries = plan.boundaries;
        self.reveal_wait_units = plan.wait_units;
        self.timeline_events = plan.events;
        self.timeline_ms = 0;
        self.fired_events.clear();
        self.revealed_glyphs = 0;
        self.next_glyph_remaining_ms = if self.reveal_boundaries.is_empty() {
            0
        } else {
            self.glyph_wait_ms(0)
        };
        self.target_node = Some(target_node);
        self.ruby_spans = parsed.ruby_spans;
        self.style_spans = parsed.style_spans;
        self.link_spans = parsed.link_spans;
        if !self.full_text.is_empty() {
            self.history.push(self.full_text.clone());
            if self.history.len() > 200 {
                self.history.remove(0);
            }
        }
    }

    /// Force completion (`CProcDspMsg` `+0x74`): every glyph appears and every
    /// pending `<ev>` record fires, as `sub_437620` does when its force flag
    /// is set.
    pub(crate) fn reveal_all(&mut self) -> Option<(i32, String)> {
        let target = self.target_node?;
        self.fire_events(true);
        if self.revealed_glyphs < self.reveal_boundaries.len() {
            self.revealed_glyphs = self.reveal_boundaries.len();
            self.visible_chars = self.full_text.chars().count();
            self.next_glyph_remaining_ms = 0;
            return Some((target, self.full_text.clone()));
        }
        None
    }

    pub(crate) fn current_visible_text(&self) -> String {
        self.full_text
            .chars()
            .take(self.visible_chars)
            .collect::<String>()
    }

    pub(crate) fn is_animating(&self) -> bool {
        self.revealed_glyphs < self.reveal_boundaries.len()
            || self.timeline_events.iter().any(|event| !event.fired)
    }

    pub(crate) fn has_current_message(&self) -> bool {
        self.target_node.is_some() && !self.full_text.is_empty()
    }

    pub(crate) fn visible_ruby_spans(&self) -> Vec<RuntimeRubySpan> {
        self.ruby_spans
            .iter()
            .filter(|span| span.end_char <= self.visible_chars)
            .cloned()
            .collect()
    }

    pub(crate) fn visible_style_spans(&self) -> Vec<RuntimeTextStyleSpan> {
        self.style_spans
            .iter()
            .filter_map(|span| {
                let end_char = span.end_char.min(self.visible_chars);
                (span.start_char < end_char).then_some(RuntimeTextStyleSpan {
                    start_char: span.start_char,
                    end_char,
                    style: span.style,
                })
            })
            .collect()
    }

    pub(crate) fn duration_ms(&self) -> i32 {
        let glyph_units: u64 = self.reveal_wait_units.iter().map(|&u| u64::from(u)).sum();
        let event_units = self
            .timeline_events
            .iter()
            .map(|event| u64::from(event.fire_unit) + 1)
            .max()
            .unwrap_or(0);
        let units = glyph_units.max(event_units);
        if units == 0 {
            return 0;
        }
        let delay = u64::from(self.glyph_delay_ms.max(1));
        units.saturating_mul(delay).min(i32::MAX as u64) as i32
    }

    /// Host events fired by `<ev>` records since the last call, as the
    /// `[code, ordinal, id]` triples the system event queue stores.
    pub(crate) fn take_fired_events(&mut self) -> Vec<[i32; 3]> {
        std::mem::take(&mut self.fired_events)
    }

    fn fire_events(&mut self, force: bool) {
        let delay = u64::from(self.glyph_delay_ms);
        let elapsed = self.timeline_ms;
        for event in self.timeline_events.iter_mut().filter(|event| !event.fired) {
            // Reveal time is expressed against the same one-delay offset the
            // glyph boundaries use: unit `n` is reached at `(n + 1)` delays.
            let due = force || delay == 0 || elapsed >= (u64::from(event.fire_unit) + 1) * delay;
            if due {
                event.fired = true;
                self.fired_events
                    .push([MESSAGE_EVENT_CODE, event.ordinal, event.id]);
            }
        }
    }

    /// Advance typewriter reveal using the same pause-adjusted real
    /// millisecond clock as the native message procedure. Presentation
    /// cadence must not change glyph speed.
    pub(crate) fn tick(&mut self, elapsed_ms: u64) -> Option<(i32, String)> {
        let target = self.target_node?;
        self.timeline_ms = self.timeline_ms.saturating_add(elapsed_ms);
        let before = self.revealed_glyphs;
        if self.revealed_glyphs < self.reveal_boundaries.len() {
            let mut budget = elapsed_ms.min(u64::from(u32::MAX)) as u32;
            if self.glyph_delay_ms == 0 {
                self.revealed_glyphs = self.reveal_boundaries.len();
                self.visible_chars = self.full_text.chars().count();
                self.next_glyph_remaining_ms = 0;
            } else {
                while self.revealed_glyphs < self.reveal_boundaries.len()
                    && budget >= self.next_glyph_remaining_ms
                {
                    budget -= self.next_glyph_remaining_ms;
                    self.visible_chars = self.reveal_boundaries[self.revealed_glyphs];
                    self.revealed_glyphs += 1;
                    self.next_glyph_remaining_ms = self.glyph_wait_ms(self.revealed_glyphs);
                }
                if self.revealed_glyphs < self.reveal_boundaries.len() {
                    self.next_glyph_remaining_ms =
                        self.next_glyph_remaining_ms.saturating_sub(budget);
                } else {
                    self.next_glyph_remaining_ms = 0;
                }
            }
        }
        self.fire_events(false);

        if self.revealed_glyphs == before {
            return None;
        }
        let text = self
            .full_text
            .chars()
            .take(self.visible_chars)
            .collect::<String>();
        Some((target, text))
    }
}

/// Build the target-shaped timed reveal plan for plain message characters.
/// `CProcDspMsg::Parse` dispatches byte 0x0A to sub_433960, which performs
/// the line-break/layout mutation and returns success immediately.  Therefore
/// a newline must never consume a separate Graph90:94 glyph interval.
///
/// Every glyph node carries the running reveal time `v180`, which advances by
/// one glyph delay per glyph and is overwritten by `<t N>`. Glyph `k` becomes
/// visible `(start + 1)` delays after the message starts; a backwards `<t>`
/// never makes a glyph appear before its predecessor because the portable
/// reveal is a visible-prefix model.
fn build_reveal_plan(text: &str, marks: &[RuntimeTimelineMark]) -> RevealPlan {
    let chars = text.chars().collect::<Vec<_>>();
    let mut plan = RevealPlan::default();
    let mut next_mark = 0usize;
    let mut unit = 0u32;
    let mut envelope: i64 = -1;
    let mut index = 0usize;
    while index < chars.len() && chars[index] == '\n' {
        apply_marks(marks, &mut next_mark, index, &mut unit, &mut plan);
        index += 1;
    }
    plan.initial_visible_chars = index;
    while index < chars.len() {
        apply_marks(marks, &mut next_mark, index, &mut unit, &mut plan);
        if chars[index] == '\n' {
            // Defensive fallback for a control reached without a preceding
            // timed glyph.  It is still consumed immediately.
            index += 1;
            if let Some(last) = plan.boundaries.last_mut() {
                *last = index;
            }
            continue;
        }
        index += 1;
        while index < chars.len() && chars[index] == '\n' {
            index += 1;
        }
        let start = i64::from(unit);
        unit = unit.saturating_add(1);
        let reached = envelope.max(start);
        plan.wait_units.push((reached - envelope) as u32);
        envelope = reached;
        plan.boundaries.push(index);
    }
    apply_marks(marks, &mut next_mark, usize::MAX, &mut unit, &mut plan);
    plan
}

/// Apply every pending timeline mark positioned at or before `upto`.
fn apply_marks(
    marks: &[RuntimeTimelineMark],
    next_mark: &mut usize,
    upto: usize,
    unit: &mut u32,
    plan: &mut RevealPlan,
) {
    while let Some(&mark) = marks.get(*next_mark) {
        match mark {
            RuntimeTimelineMark::SetTime { at_char, units } if at_char <= upto => *unit = units,
            RuntimeTimelineMark::Event {
                at_char,
                ordinal,
                id,
            } if at_char <= upto => plan.events.push(TimelineEvent {
                fire_unit: *unit,
                ordinal,
                id,
                fired: false,
            }),
            _ => break,
        }
        *next_mark += 1;
    }
}

pub(crate) fn normalize_message_text(text: &str) -> String {
    parse_message_markup(text).0
}

pub(crate) fn parse_message_markup(text: &str) -> (String, Vec<RuntimeRubySpan>) {
    let parsed = parse_message_markup_styled(text);
    (parsed.text, parsed.ruby_spans)
}

/// Parse with a private registry and no link colour. Callers that own the
/// process-wide ruby registry use [`parse_message_markup_with`].
pub(crate) fn parse_message_markup_styled(text: &str) -> ParsedMessageMarkup {
    parse_message_markup_with(text, &mut RubyRegistry::default(), None)
}

/// Target tag names in `sub_435290` comparison order. Index 0 is compared
/// exactly; the rest are prefix matches, so `ruby` precedes `r` and `cr`
/// precedes `c`.
const TARGET_TAGS: [&str; 15] = [
    "/", "b", "/b", "i", "/i", "ruby", "r", "/r", "cr", "c", "/c", "l", "/l", "t", "ev",
];

/// `sub_437E40` keeps at most sixteen 128-byte records, each holding a text
/// field of 96 bytes of which at most 95 are copied.
const MAX_LINK_RECORDS: usize = 16;
const LINK_TEXT_MAX_BYTES: usize = 95;

fn target_markup_command(tag: &str) -> Option<(&'static str, &str)> {
    if tag == TARGET_TAGS[0] {
        return Some((TARGET_TAGS[0], ""));
    }
    TARGET_TAGS.iter().skip(1).find_map(|&command| {
        tag.strip_prefix(command)
            .map(|argument| (command, argument))
    })
}

/// A ruby base found by the registry whose span has not closed yet.
struct OpenRuby {
    start_char: usize,
    /// Raw character index at which the base text ends.
    raw_end: usize,
    reading: String,
}

struct OpenLink {
    start_char: usize,
    raw_start: usize,
}

/// `atoi` over the leading ASCII digits of `text`; `None` when there are none.
fn leading_number(text: &str) -> Option<i32> {
    let digits = text
        .trim_start_matches(' ')
        .chars()
        .take_while(char::is_ascii_digit)
        .collect::<String>();
    (!digits.is_empty()).then(|| {
        digits
            .parse::<i64>()
            .unwrap_or(i64::MAX)
            .min(i64::from(i32::MAX)) as i32
    })
}

fn truncate_to_target_bytes(text: &str, max_bytes: usize) -> String {
    let mut bytes = 0usize;
    let mut out = String::new();
    for ch in text.chars() {
        let width = encoding_rs::SHIFT_JIS
            .encode(ch.encode_utf8(&mut [0; 4]))
            .0
            .len();
        if bytes + width > max_bytes {
            break;
        }
        bytes += width;
        out.push(ch);
    }
    out
}

/// Target `sub_435290` markup front end.
///
/// The stream is processed one character at a time like the target:
///
/// * control characters below 0x20 draw nothing, and only `\n` breaks a line;
/// * `<` starts a tag only when a `>` follows and the tag is non-empty,
///   otherwise it is an ordinary glyph;
/// * the lower-cased tag selects one of [`TARGET_TAGS`]; unknown tags are
///   dropped;
/// * `</>` makes the next well-formed tag literal text;
/// * at every glyph position the registry is queried with the remaining raw
///   text, so `<r>`/`<ruby>` one-shot entries and `91:94/96` dictionary
///   entries produce the same ruby spans.
///
/// `link_color` is the `92:9F` value (`dword_507650`); `None` is the target's
/// `-1`, which leaves link glyphs in the surrounding colour.
pub(crate) fn parse_message_markup_with(
    text: &str,
    registry: &mut RubyRegistry,
    link_color: Option<u32>,
) -> ParsedMessageMarkup {
    let mut parser = MarkupParser::new(text);
    parser.run(registry, link_color);
    parser.finish()
}

struct MarkupParser<'t> {
    text: &'t str,
    chars: Vec<char>,
    byte_offsets: Vec<usize>,
    parsed: ParsedMessageMarkup,
    out_chars: usize,
    style: RuntimeTextStyle,
    /// Shared by `<c>` and `<l>` (target `v158`/`v167`).
    color_stack: Vec<Option<u32>>,
    open_ruby: Option<OpenRuby>,
    open_link: Option<OpenLink>,
    ruby_skip_until: usize,
    event_ordinal: i32,
    escaped: bool,
}

impl<'t> MarkupParser<'t> {
    fn new(text: &'t str) -> Self {
        let mut byte_offsets = text.char_indices().map(|(at, _)| at).collect::<Vec<_>>();
        byte_offsets.push(text.len());
        Self {
            text,
            chars: text.chars().collect(),
            byte_offsets,
            parsed: ParsedMessageMarkup::default(),
            out_chars: 0,
            style: RuntimeTextStyle::default(),
            color_stack: Vec::new(),
            open_ruby: None,
            open_link: None,
            ruby_skip_until: 0,
            event_ordinal: 0,
            escaped: false,
        }
    }

    fn finish(mut self) -> ParsedMessageMarkup {
        self.close_ruby();
        self.parsed
    }

    fn run(&mut self, registry: &mut RubyRegistry, link_color: Option<u32>) {
        let mut i = 0usize;
        while i < self.chars.len() {
            if self
                .open_ruby
                .as_ref()
                .is_some_and(|ruby| ruby.raw_end <= i)
            {
                self.close_ruby();
            }
            let ch = self.chars[i];
            if ch < ' ' {
                if ch == '\n' {
                    self.push_char(ch);
                }
                i += 1;
                continue;
            }
            if ch == '<'
                && let Some(close) = self.tag_end(i)
            {
                if self.escaped {
                    // `</>` turns this otherwise valid tag into a literal `<`.
                    self.escaped = false;
                } else {
                    let tag = self.chars[i + 1..close]
                        .iter()
                        .collect::<String>()
                        .to_ascii_lowercase();
                    self.apply_tag(&tag, i, close + 1, registry, link_color);
                    i = close + 1;
                    continue;
                }
            }
            self.glyph(i, registry);
            i += 1;
        }
    }

    /// Index of the `>` closing a tag that starts at `start`, if the target
    /// would treat it as a tag: a `>` must follow and the body is non-empty.
    fn tag_end(&self, start: usize) -> Option<usize> {
        let close = self.chars[start + 1..]
            .iter()
            .position(|&c| c == '>')
            .map(|offset| start + 1 + offset)?;
        (close > start + 1).then_some(close)
    }

    fn glyph(&mut self, index: usize, registry: &mut RubyRegistry) {
        if index >= self.ruby_skip_until && self.open_ruby.is_none() {
            let tail = &self.text[self.byte_offsets[index]..];
            if let Some(hit) = registry.lookup(tail) {
                let base_chars = hit.base.chars().count();
                if base_chars > 0 {
                    self.ruby_skip_until = index + base_chars;
                    self.open_ruby = Some(OpenRuby {
                        start_char: self.out_chars,
                        raw_end: index + base_chars,
                        reading: hit.reading,
                    });
                }
            }
        }
        self.push_char(self.chars[index]);
    }

    fn close_ruby(&mut self) {
        if let Some(ruby) = self.open_ruby.take()
            && self.out_chars > ruby.start_char
            && !ruby.reading.is_empty()
        {
            self.parsed.ruby_spans.push(RuntimeRubySpan {
                start_char: ruby.start_char,
                end_char: self.out_chars,
                reading: ruby.reading,
            });
        }
    }

    fn push_char(&mut self, ch: char) {
        self.parsed.text.push(ch);
        self.out_chars += 1;
        if self.style == RuntimeTextStyle::default() {
            return;
        }
        let (start_char, end_char) = (self.out_chars - 1, self.out_chars);
        match self
            .parsed
            .style_spans
            .last_mut()
            .filter(|previous| previous.end_char == start_char && previous.style == self.style)
        {
            Some(previous) => previous.end_char = end_char,
            None => self.parsed.style_spans.push(RuntimeTextStyleSpan {
                start_char,
                end_char,
                style: self.style,
            }),
        }
    }

    /// Execute one tag. `tag_start` is the index of its `<` and `raw_after`
    /// the index just past its `>`.
    fn apply_tag(
        &mut self,
        tag: &str,
        tag_start: usize,
        raw_after: usize,
        registry: &mut RubyRegistry,
        link_color: Option<u32>,
    ) {
        let Some((command, argument)) = target_markup_command(tag) else {
            return;
        };
        match command {
            "/" => self.escaped = true,
            "b" => self.style.bold = true,
            "/b" => self.style.bold = false,
            "i" => self.style.italic = true,
            "/i" => self.style.italic = false,
            "ruby" => {
                // `<ruby base,reading>`: both halves come from the lower-cased
                // tag. A tag without a comma registers nothing.
                if let Some((base, reading)) = argument.trim_start_matches(' ').split_once(',') {
                    registry.register(base, reading, true);
                }
            }
            "r" => {
                // `<r reading>` annotates the text that follows, up to the next
                // `<` of any kind. `</r>` is a no-op. Without a terminating
                // `<`, with an empty reading or with an empty base nothing is
                // registered.
                let reading = argument.trim_start_matches(' ');
                let base_end = self.chars[raw_after..]
                    .iter()
                    .position(|&c| c == '<')
                    .map(|offset| raw_after + offset);
                if let Some(base_end) = base_end
                    && !reading.is_empty()
                    && base_end > raw_after
                {
                    let base = self.chars[raw_after..base_end].iter().collect::<String>();
                    registry.register(&base, reading, true);
                }
            }
            "cr" => {
                // The target only resets the cursor X here. The portable
                // layout has no overprint, so it keeps the established line
                // break.
                self.push_char('\n');
            }
            "c" => {
                if let Some(packed_rgb) = parse_target_color(argument) {
                    self.color_stack.push(self.style.packed_rgb);
                    self.style.packed_rgb = Some(packed_rgb);
                }
            }
            "/c" => {
                if let Some(previous) = self.color_stack.pop() {
                    self.style.packed_rgb = previous;
                }
            }
            "l" => {
                if self.open_link.is_none() {
                    self.open_link = Some(OpenLink {
                        start_char: self.out_chars,
                        raw_start: raw_after,
                    });
                    self.color_stack.push(self.style.packed_rgb);
                    if let Some(color) = link_color {
                        self.style.packed_rgb = Some(color);
                    }
                }
            }
            "/l" => {
                if let Some(link) = self.open_link.take() {
                    let raw =
                        &self.text[self.byte_offsets[link.raw_start]..self.byte_offsets[tag_start]];
                    if !raw.is_empty() && self.parsed.link_spans.len() < MAX_LINK_RECORDS {
                        self.parsed.link_spans.push(RuntimeLinkSpan {
                            start_char: link.start_char,
                            end_char: self.out_chars,
                            text: truncate_to_target_bytes(raw, LINK_TEXT_MAX_BYTES),
                        });
                    }
                    if let Some(previous) = self.color_stack.pop() {
                        self.style.packed_rgb = previous;
                    }
                }
            }
            "t" => {
                let units = leading_number(argument).unwrap_or(0).max(0) as u32;
                self.parsed.timeline.push(RuntimeTimelineMark::SetTime {
                    at_char: self.out_chars,
                    units,
                });
            }
            "ev" => {
                self.parsed.timeline.push(RuntimeTimelineMark::Event {
                    at_char: self.out_chars,
                    ordinal: self.event_ordinal,
                    id: leading_number(argument).unwrap_or(-1),
                });
                self.event_ordinal += 1;
            }
            // `/r` only terminates `<r>` visually and has no effect.
            _ => {}
        }
    }
}

fn parse_target_color(argument: &str) -> Option<u32> {
    let digits = argument.trim_start_matches(' ').as_bytes().get(..6)?;
    digits.iter().try_fold(0u32, |color, digit| {
        let nibble = match digit {
            b'0'..=b'9' => u32::from(digit - b'0'),
            b'a'..=b'f' => u32::from(digit - b'a' + 10),
            _ => return None,
        };
        Some((color << 4) | nibble)
    })
}

#[cfg(test)]
mod tests {
    use super::{
        MESSAGE_EVENT_CODE, RuntimeLinkSpan, RuntimeRubySpan, RuntimeTextStyle,
        RuntimeTextStyleSpan, RuntimeTimelineMark, TextRuntime, normalize_message_text,
        parse_message_markup, parse_message_markup_styled, parse_message_markup_with,
    };
    use crate::ruby_registry::RubyRegistry;

    #[test]
    fn strips_bgi_ruby_tags() {
        assert_eq!(
            normalize_message_text("<Rやこたみちょう>矢古民町</R>へ行く"),
            "矢古民町へ行く"
        );
    }

    #[test]
    fn target_markup_dispatch_is_case_insensitive_and_prefix_based() {
        assert_eq!(
            parse_message_markup("<r しんき>神気</R><CR>次").0,
            "神気\n次"
        );
        assert_eq!(
            parse_message_markup("<b>太字</b><i>斜体</i><c ff0000>赤</c>").0,
            "太字斜体赤"
        );
    }

    #[test]
    fn target_non_glyph_commands_are_not_exposed_as_text() {
        assert_eq!(
            parse_message_markup("<ruby base,reading><l>label</l><t 4><ev 3>").0,
            "label"
        );
        assert_eq!(
            parse_message_markup("before<unknown>after").0,
            "beforeafter"
        );
    }

    #[test]
    fn target_font_and_color_commands_produce_character_style_spans() {
        let parsed =
            parse_message_markup_styled("前<b>太<i>斜</i></b><c ff0000>赤<c 00ff00>緑</c>赤</c>後");
        assert_eq!(parsed.text, "前太斜赤緑赤後");
        assert_eq!(
            parsed.style_spans,
            vec![
                RuntimeTextStyleSpan {
                    start_char: 1,
                    end_char: 2,
                    style: RuntimeTextStyle {
                        bold: true,
                        ..RuntimeTextStyle::default()
                    },
                },
                RuntimeTextStyleSpan {
                    start_char: 2,
                    end_char: 3,
                    style: RuntimeTextStyle {
                        bold: true,
                        italic: true,
                        ..RuntimeTextStyle::default()
                    },
                },
                RuntimeTextStyleSpan {
                    start_char: 3,
                    end_char: 4,
                    style: RuntimeTextStyle {
                        packed_rgb: Some(0xff0000),
                        ..RuntimeTextStyle::default()
                    },
                },
                RuntimeTextStyleSpan {
                    start_char: 4,
                    end_char: 5,
                    style: RuntimeTextStyle {
                        packed_rgb: Some(0x00ff00),
                        ..RuntimeTextStyle::default()
                    },
                },
                RuntimeTextStyleSpan {
                    start_char: 5,
                    end_char: 6,
                    style: RuntimeTextStyle {
                        packed_rgb: Some(0xff0000),
                        ..RuntimeTextStyle::default()
                    },
                },
            ]
        );
    }

    #[test]
    fn target_color_parser_lowercases_hex_before_color_stack_updates() {
        let parsed = parse_message_markup_styled("<c FF0000>白</c><c ff0000>赤</c>");
        assert_eq!(parsed.text, "白赤");
        assert_eq!(
            parsed.style_spans,
            vec![RuntimeTextStyleSpan {
                start_char: 0,
                end_char: 2,
                style: RuntimeTextStyle {
                    packed_rgb: Some(0xff0000),
                    ..RuntimeTextStyle::default()
                },
            }]
        );
    }

    #[test]
    fn preserves_bgi_ruby_character_range() {
        assert_eq!(
            parse_message_markup("前<Rやこたみ>矢古民</R>後"),
            (
                "前矢古民後".to_string(),
                vec![RuntimeRubySpan {
                    start_char: 1,
                    end_char: 4,
                    reading: "やこたみ".to_string(),
                }]
            )
        );
    }

    #[test]
    fn r_tag_without_a_following_tag_registers_nothing_and_is_dropped() {
        // sub_435290 case `r` copies the base up to the next `<`; at the end
        // of the string it skips the tag without registering anything.
        let parsed = parse_message_markup_styled("<Rfoo>bar");
        assert_eq!(parsed.text, "bar");
        assert!(parsed.ruby_spans.is_empty());
    }

    #[test]
    fn r_tag_takes_its_base_up_to_the_next_tag_and_closing_tag_is_a_no_op() {
        let parsed = parse_message_markup_styled("<r しんき>神気<b>体</r>");
        assert_eq!(parsed.text, "神気体");
        assert_eq!(
            parsed.ruby_spans,
            vec![RuntimeRubySpan {
                start_char: 0,
                end_char: 2,
                reading: "しんき".to_string(),
            }]
        );
        // An empty reading or an empty base registers nothing.
        assert!(
            parse_message_markup_styled("<r >神気</r>")
                .ruby_spans
                .is_empty()
        );
        assert!(
            parse_message_markup_styled("<r a><b>x")
                .ruby_spans
                .is_empty()
        );
    }

    #[test]
    fn ruby_tag_registers_a_one_shot_entry_for_the_next_matching_text() {
        let parsed = parse_message_markup_styled("前<ruby 神,かみ>後神神");
        assert_eq!(parsed.text, "前後神神");
        // Only the first later occurrence is annotated.
        assert_eq!(
            parsed.ruby_spans,
            vec![RuntimeRubySpan {
                start_char: 2,
                end_char: 3,
                reading: "かみ".to_string(),
            }]
        );
        // Without a comma nothing is registered.
        assert!(
            parse_message_markup_styled("<ruby 神>神")
                .ruby_spans
                .is_empty()
        );
    }

    #[test]
    fn registry_entries_annotate_every_matching_position() {
        let mut registry = RubyRegistry::default();
        registry.register("矢古民", "やこたみ", false);
        registry.register("矢古", "やこ", false);
        let parsed = parse_message_markup_with("前矢古民と矢古", &mut registry, None);
        assert_eq!(parsed.text, "前矢古民と矢古");
        assert_eq!(
            parsed.ruby_spans,
            vec![
                RuntimeRubySpan {
                    start_char: 1,
                    end_char: 4,
                    reading: "やこたみ".to_string(),
                },
                RuntimeRubySpan {
                    start_char: 5,
                    end_char: 7,
                    reading: "やこ".to_string(),
                },
            ]
        );
    }

    #[test]
    fn one_shot_tag_entries_take_precedence_over_dictionary_entries() {
        let mut registry = RubyRegistry::default();
        registry.register("神", "dictionary", false);
        let parsed = parse_message_markup_with("<ruby 神,tag>神神", &mut registry, None);
        let readings = parsed
            .ruby_spans
            .iter()
            .map(|span| span.reading.as_str())
            .collect::<Vec<_>>();
        assert_eq!(readings, ["tag", "dictionary"]);
    }

    #[test]
    fn slash_tag_escapes_the_next_well_formed_tag() {
        assert_eq!(parse_message_markup_styled("a</><b>c").text, "a<b>c");
        // The escape is spent by the first well-formed tag, not by text.
        assert_eq!(parse_message_markup_styled("</>x<b>y<b>z").text, "x<b>yz");
        // A `<` that is not a well-formed tag does not spend it.
        assert_eq!(parse_message_markup_styled("</>a<>b<i>c").text, "a<>b<i>c");
    }

    #[test]
    fn malformed_angle_brackets_are_ordinary_glyphs() {
        assert_eq!(parse_message_markup_styled("a<>b").text, "a<>b");
        assert_eq!(parse_message_markup_styled("a<b").text, "a<b");
        assert_eq!(parse_message_markup_styled("<").text, "<");
        // A tag body runs to the first `>` even across another `<`, so `<<b>`
        // is one unknown tag and is dropped.
        assert_eq!(parse_message_markup_styled("<<b>x").text, "x");
        // After a literal `<`, scanning resumes on the next character.
        assert_eq!(parse_message_markup_styled("<<").text, "<<");
    }

    #[test]
    fn control_characters_draw_nothing_and_only_newline_breaks_lines() {
        assert_eq!(
            parse_message_markup_styled("a\rb\x02c\x0cd\r\ne").text,
            "abcd\ne"
        );
    }

    #[test]
    fn link_tags_record_a_fragment_and_share_the_colour_stack() {
        let mut registry = RubyRegistry::default();
        let parsed = parse_message_markup_with(
            "<c ff0000>a<l>b<b>c</b></l>d</c>e",
            &mut registry,
            Some(0x112233),
        );
        assert_eq!(parsed.text, "abcde");
        assert_eq!(
            parsed.link_spans,
            vec![RuntimeLinkSpan {
                start_char: 1,
                end_char: 3,
                text: "b<b>c</b>".to_string(),
            }]
        );
        let color_at = |index: usize| {
            parsed
                .style_spans
                .iter()
                .find(|span| span.start_char <= index && index < span.end_char)
                .and_then(|span| span.style.packed_rgb)
        };
        assert_eq!(color_at(0), Some(0xff0000));
        assert_eq!(color_at(1), Some(0x112233));
        assert_eq!(color_at(2), Some(0x112233));
        assert_eq!(color_at(3), Some(0xff0000));
        assert_eq!(color_at(4), None);
    }

    #[test]
    fn link_color_minus_one_leaves_link_glyphs_uncoloured() {
        let parsed = parse_message_markup_styled("<l>a</l>");
        assert!(parsed.style_spans.is_empty());
        assert_eq!(parsed.link_spans.len(), 1);
        // A second `<l>` while one is open, and a stray `</l>`, do nothing.
        let parsed = parse_message_markup_styled("</l><l>a<l>b</l>c</l>");
        assert_eq!(parsed.text, "abc");
        assert_eq!(parsed.link_spans.len(), 1);
        assert_eq!(parsed.link_spans[0].end_char, 2);
    }

    #[test]
    fn link_records_are_limited_to_sixteen_with_95_byte_text() {
        let long = "あ".repeat(60);
        let parsed = parse_message_markup_styled(&format!("<l>{long}</l>"));
        // 95 bytes hold 47 two-byte characters.
        assert_eq!(parsed.link_spans[0].text.chars().count(), 47);
        let many = "<l>x</l>".repeat(20);
        assert_eq!(parse_message_markup_styled(&many).link_spans.len(), 16);
    }

    #[test]
    fn time_and_event_records_are_positioned_between_glyphs() {
        let parsed = parse_message_markup_styled("<t 4>a<ev 7>b<ev>");
        assert_eq!(parsed.text, "ab");
        assert_eq!(
            parsed.timeline,
            vec![
                RuntimeTimelineMark::SetTime {
                    at_char: 0,
                    units: 4
                },
                RuntimeTimelineMark::Event {
                    at_char: 1,
                    ordinal: 0,
                    id: 7
                },
                RuntimeTimelineMark::Event {
                    at_char: 2,
                    ordinal: 1,
                    id: -1
                },
            ]
        );
    }

    #[test]
    fn time_record_delays_the_following_glyph_in_glyph_delay_units() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("<t 4>a<ev 7>b".to_string(), 1);
        // Glyph delay is 16 ms; `a` starts at unit 4, so it appears at 5 delays.
        assert_eq!(runtime.tick(79), None);
        assert_eq!(runtime.tick(1), Some((1, "a".to_string())));
        assert!(runtime.take_fired_events().is_empty());
        // `b` and the event are both due at unit 5 + 1 delays.
        assert_eq!(runtime.tick(16), Some((1, "ab".to_string())));
        assert_eq!(
            runtime.take_fired_events(),
            vec![[MESSAGE_EVENT_CODE, 0, 7]]
        );
        assert!(!runtime.is_animating());
    }

    #[test]
    fn backwards_time_record_never_reveals_before_the_previous_glyph() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("ab<t 0>cd".to_string(), 1);
        assert_eq!(runtime.duration_ms(), 32);
        assert_eq!(runtime.tick(16), Some((1, "a".to_string())));
        assert_eq!(runtime.tick(16), Some((1, "abcd".to_string())));
    }

    #[test]
    fn force_completion_fires_every_pending_event() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("a<t 50><ev 1>b<ev 2>".to_string(), 1);
        assert!(runtime.is_animating());
        assert_eq!(
            runtime.reveal_all().map(|(_, text)| text),
            Some("ab".into())
        );
        assert_eq!(
            runtime.take_fired_events(),
            vec![[MESSAGE_EVENT_CODE, 0, 1], [MESSAGE_EVENT_CODE, 1, 2]]
        );
        assert!(!runtime.is_animating());
    }

    #[test]
    fn events_without_glyphs_still_hold_the_message_open() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("<t 3><ev 9>".to_string(), 1);
        assert!(runtime.is_animating());
        assert_eq!(runtime.duration_ms(), 64);
        assert_eq!(runtime.tick(63), None);
        assert!(runtime.take_fired_events().is_empty());
        assert_eq!(runtime.tick(1), None);
        assert_eq!(
            runtime.take_fired_events(),
            vec![[MESSAGE_EVENT_CODE, 0, 9]]
        );
        assert!(!runtime.is_animating());
    }

    #[test]
    fn first_glyph_appears_after_one_glyph_delay() {
        // The initial delay belongs to CProcDspMsg (Graph90:9B); the glyph
        // runtime only paces glyphs.
        let mut runtime = TextRuntime::default();
        runtime.start_message("ab".to_string(), 1);
        assert_eq!(runtime.tick(15), None);
        assert_eq!(runtime.visible_chars, 0);
        assert_eq!(runtime.tick(1), Some((1, "a".to_string())));
    }

    #[test]
    fn message_duration_tracks_typewriter_frames() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("abc".to_string(), 1);
        assert_eq!(runtime.duration_ms(), 48);
        runtime.set_glyph_delay_ms(33);
        assert_eq!(runtime.duration_ms(), 99);
    }

    #[test]
    fn glyph_delay_uses_real_milliseconds_not_render_frames() {
        let mut runtime = TextRuntime::default();
        runtime.set_glyph_delay_ms(39);
        runtime.start_message("ab".to_string(), 1);
        assert_eq!(runtime.tick(16), None);
        assert_eq!(runtime.tick(16), None);
        assert_eq!(runtime.tick(7), Some((1, "a".to_string())));
        assert_eq!(runtime.tick(38), None);
        assert_eq!(runtime.tick(1), Some((1, "ab".to_string())));
    }

    #[test]
    fn typewriter_visibility_clips_style_spans_to_revealed_characters() {
        let mut runtime = TextRuntime::default();
        runtime.start_message("<c ff0000>赤字</c>".to_string(), 7);
        assert_eq!(runtime.tick(16), Some((7, "赤".to_string())));
        assert_eq!(
            runtime.visible_style_spans(),
            vec![RuntimeTextStyleSpan {
                start_char: 0,
                end_char: 1,
                style: RuntimeTextStyle {
                    packed_rgb: Some(0xff0000),
                    ..RuntimeTextStyle::default()
                },
            }]
        );
        assert_eq!(runtime.tick(16), Some((7, "赤字".to_string())));
        assert_eq!(runtime.visible_style_spans()[0].end_char, 2);
    }
}
