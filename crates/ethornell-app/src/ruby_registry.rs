//! Target ruby registry (`unk_565BB4`).
//!
//! One ordered list serves three producers and one consumer:
//!
//! * `91:94` (`sub_463270`) registers, removes or clears permanent entries;
//! * `91:96` (`sub_434A40`) registers newline-delimited `base\reading` records;
//! * the `<r reading>` and `<ruby base,reading>` message tags (`sub_435290`
//!   cases 5 and 6) register *one-shot* entries through `sub_434520(.., 1)`;
//! * the message layout and `91:95` (`sub_4348C0`/`sub_434920`) look the list
//!   up by prefix at every glyph position.
//!
//! The list order is observable, so insertion follows `sub_434520` exactly:
//! one-shot entries precede permanent entries, and permanent entries are kept
//! in non-increasing target byte-length order with the newest first among equal
//! lengths.

/// Target `sub_434520` stores `strlen(base) + 1`; the length decides the
/// position of permanent entries.
fn target_byte_len(text: &str) -> usize {
    encoding_rs::SHIFT_JIS.encode(text).0.len() + 1
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct RubyEntry {
    base: String,
    reading: String,
    base_len: usize,
    one_shot: bool,
    /// Target entry `+32`: incremented by a successful one-shot lookup. An
    /// entry whose counter is non-zero is never matched again. Entries are not
    /// unlinked on use; only `remove`/`clear` delete them.
    uses: u32,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub(crate) struct RubyRegistry {
    entries: Vec<RubyEntry>,
}

/// A successful registry lookup: the matched base text and its reading.
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct RubyMatch {
    pub(crate) base: String,
    pub(crate) reading: String,
}

impl RubyRegistry {
    /// `sub_434520(registry, base, reading, one_shot)`.
    pub(crate) fn register(&mut self, base: &str, reading: &str, one_shot: bool) {
        if !one_shot
            && let Some(existing) = self.entries.iter_mut().find(|entry| entry.base == base)
        {
            // The target rewrites only the reading; flag and counter stay.
            existing.reading = reading.to_string();
            return;
        }
        let base_len = target_byte_len(base);
        let index = self
            .entries
            .iter()
            .position(|entry| {
                if one_shot {
                    !entry.one_shot
                } else {
                    base_len >= entry.base_len
                }
            })
            .unwrap_or(self.entries.len());
        self.entries.insert(
            index,
            RubyEntry {
                base: base.to_string(),
                reading: reading.to_string(),
                base_len,
                one_shot,
                uses: 0,
            },
        );
    }

    /// `sub_434730(base, registry, 0)`: remove the first entry with exactly
    /// this base, whatever its kind. Returns whether one was removed.
    pub(crate) fn remove(&mut self, base: &str) -> bool {
        match self.entries.iter().position(|entry| entry.base == base) {
            Some(index) => {
                self.entries.remove(index);
                true
            }
            None => false,
        }
    }

    /// `sub_434810`: remove every entry.
    pub(crate) fn clear(&mut self) {
        self.entries.clear();
    }

    /// `sub_4348C0`: first unused entry whose base is a prefix of `tail`.
    /// A match consumes a one-shot entry.
    pub(crate) fn lookup(&mut self, tail: &str) -> Option<RubyMatch> {
        let entry = self
            .entries
            .iter_mut()
            .find(|entry| entry.uses == 0 && tail.starts_with(entry.base.as_str()))?;
        if entry.one_shot {
            entry.uses += 1;
        }
        Some(RubyMatch {
            base: entry.base.clone(),
            reading: entry.reading.clone(),
        })
    }

    /// `sub_434840`: first entry with exactly this base, used or not.
    fn find_exact(&self, base: &str) -> Option<&RubyEntry> {
        self.entries.iter().find(|entry| entry.base == base)
    }

    /// `sub_434A40` (`91:96`): register `base\reading` lines as permanent
    /// entries. The target loops forever on a line without a separator or with
    /// an empty side; the portable runtime rejects it instead, keeping the
    /// lines registered before it.
    pub(crate) fn register_records(&mut self, records: &str) -> bool {
        for line in records.lines() {
            let Some((base, reading)) = line.split_once('\\') else {
                return false;
            };
            if base.is_empty() || reading.is_empty() {
                return false;
            }
            self.register(base, reading, false);
        }
        true
    }

    /// `sub_434920` (`91:95`): scan `text` and return the `base\reading\n`
    /// records of every match plus the match count. Matched characters are
    /// skipped, and the scan consumes one-shot entries like a layout pass.
    pub(crate) fn collect_records(&mut self, text: &str) -> (String, i32) {
        let mut records = String::new();
        let mut matches = 0_i32;
        let mut offset = 0;
        while offset < text.len() {
            let tail = &text[offset..];
            if let Some(hit) = self.lookup(tail) {
                let reading = self
                    .find_exact(&hit.base)
                    .map(|entry| entry.reading.as_str())
                    .unwrap_or(hit.reading.as_str());
                records.push_str(&hit.base);
                records.push('\\');
                records.push_str(reading);
                records.push('\n');
                matches = matches.saturating_add(1);
                // An empty base cannot advance the scan; step one character
                // instead of looping.
                offset += hit
                    .base
                    .len()
                    .max(tail.chars().next().map(char::len_utf8).unwrap_or(1));
            } else {
                offset += tail.chars().next().map(char::len_utf8).unwrap_or(1);
            }
        }
        (records, matches)
    }

    #[cfg(test)]
    fn bases(&self) -> Vec<&str> {
        self.entries
            .iter()
            .map(|entry| entry.base.as_str())
            .collect()
    }
}

#[cfg(test)]
mod tests {
    use super::RubyRegistry;

    #[test]
    fn permanent_entries_are_ordered_by_descending_length_newest_first() {
        let mut registry = RubyRegistry::default();
        registry.register("町", "ちょう", false);
        registry.register("矢古民町", "やこたみちょう", false);
        registry.register("矢古", "やこ", false);
        registry.register("民町", "みんちょう", false);
        // Lengths (SJIS bytes): 2, 8, 4, 4. Equal lengths insert before the
        // existing entry, so the newer "民町" precedes "矢古".
        assert_eq!(registry.bases(), ["矢古民町", "民町", "矢古", "町"]);
    }

    #[test]
    fn re_registering_a_permanent_base_only_replaces_its_reading() {
        let mut registry = RubyRegistry::default();
        registry.register("A", "x", false);
        registry.register("BB", "y", false);
        registry.register("A", "z", false);
        assert_eq!(registry.bases(), ["BB", "A"]);
        assert_eq!(registry.lookup("A").unwrap().reading, "z");
    }

    #[test]
    fn one_shot_entries_precede_permanent_ones_and_match_once() {
        let mut registry = RubyRegistry::default();
        registry.register("神気", "permanent", false);
        registry.register("神", "first", true);
        registry.register("神気", "second", true);
        assert_eq!(registry.bases(), ["神", "神気", "神気"]);
        // The one-shot "神" shadows the longer permanent entry once.
        assert_eq!(registry.lookup("神気だ").unwrap().reading, "first");
        assert_eq!(registry.lookup("神気だ").unwrap().reading, "second");
        assert_eq!(registry.lookup("神気だ").unwrap().reading, "permanent");
        assert_eq!(registry.lookup("神気だ").unwrap().reading, "permanent");
    }

    #[test]
    fn used_one_shot_entries_stay_listed_until_removed() {
        let mut registry = RubyRegistry::default();
        registry.register("A", "x", true);
        assert!(registry.lookup("A").is_some());
        assert!(registry.lookup("A").is_none());
        assert_eq!(registry.bases(), ["A"]);
        assert!(registry.remove("A"));
        assert!(!registry.remove("A"));
        registry.register("B", "y", false);
        registry.clear();
        assert!(registry.lookup("B").is_none());
    }

    #[test]
    fn lookup_is_a_prefix_match_on_the_remaining_text() {
        let mut registry = RubyRegistry::default();
        registry.register("矢古民", "やこたみ", false);
        assert!(registry.lookup("町矢古民").is_none());
        assert_eq!(registry.lookup("矢古民町").unwrap().base, "矢古民");
    }

    #[test]
    fn record_parser_and_collector_follow_the_native_line_format() {
        let mut registry = RubyRegistry::default();
        assert!(registry.register_records("矢古民\\やこたみ\n町\\ちょう\n"));
        assert_eq!(
            registry.collect_records("矢古民町"),
            ("矢古民\\やこたみ\n町\\ちょう\n".into(), 2)
        );
        assert!(!registry.register_records("missing separator"));
        assert!(!registry.register_records("base\\\n"));
    }

    #[test]
    fn counting_consumes_one_shot_entries_like_a_layout_pass() {
        let mut registry = RubyRegistry::default();
        registry.register("A", "x", true);
        assert_eq!(registry.collect_records("AA").1, 1);
        assert_eq!(registry.collect_records("AA").1, 0);
    }
}
