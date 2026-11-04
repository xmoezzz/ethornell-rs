//! DCCache (sub_446280): the byte-budgeted resource cache of Graph90:03.
//! Entries are kept newest first; a store evicts the least recently used
//! entry until the data fits (sub_4464D0/sub_446500), a lookup moves the
//! hit to the front and refreshes its time (sub_446430).

#[derive(Debug, Clone)]
struct Entry {
    namespace: Option<String>,
    name: Option<String>,
    bytes: Vec<u8>,
    time: u32,
}

#[derive(Debug, Clone)]
pub(crate) struct DcCache {
    budget: u32,
    used: u32,
    entries: Vec<Entry>,
}

impl DcCache {
    pub(crate) fn new(budget: u32) -> Self {
        Self { budget, used: 0, entries: Vec::new() }
    }

    /// sub_446310: refused when empty or larger than half the budget; a
    /// duplicate key is stored again rather than replaced.
    pub(crate) fn store(&mut self, namespace: Option<&str>, name: Option<&str>, bytes: &[u8], now: u32) -> bool {
        let size = bytes.len() as u32;
        if size == 0 || size > self.budget >> 1 {
            return false;
        }
        while size.wrapping_add(self.used) > self.budget {
            let Some(oldest) = self.oldest() else { break };
            let entry = self.entries.remove(oldest);
            self.used = self.used.wrapping_sub(entry.bytes.len() as u32);
        }
        self.entries.insert(0, Entry {
            namespace: namespace.map(str::to_owned),
            name: name.map(str::to_owned),
            bytes: bytes.to_vec(),
            time: now,
        });
        self.used = self.used.wrapping_add(size);
        true
    }

    /// sub_4464D0: the first entry (newest first) with the smallest time.
    fn oldest(&self) -> Option<usize> {
        let mut best: Option<(usize, u32)> = None;
        for (index, entry) in self.entries.iter().enumerate() {
            if best.is_none_or(|(_, time)| time > entry.time) {
                best = Some((index, entry.time));
            }
        }
        best.map(|(index, _)| index)
    }

    /// sub_446430: exact (case-sensitive) key match, null matching null.
    pub(crate) fn lookup(&mut self, namespace: Option<&str>, name: Option<&str>, now: u32) -> Option<Vec<u8>> {
        let index = self
            .entries
            .iter()
            .position(|entry| entry.namespace.as_deref() == namespace && entry.name.as_deref() == name)?;
        let mut entry = self.entries.remove(index);
        entry.time = now;
        let bytes = entry.bytes.clone();
        self.entries.insert(0, entry);
        Some(bytes)
    }
}

#[cfg(test)]
mod tests {
    use super::DcCache;

    #[test]
    fn store_evicts_least_recently_used_and_lookup_refreshes() {
        let mut cache = DcCache::new(8);
        assert!(!cache.store(Some("a"), Some("x"), &[1; 5], 0), "over half the budget");
        assert!(cache.store(Some("a"), Some("1"), &[1; 4], 1));
        assert!(cache.store(Some("a"), Some("2"), &[2; 4], 2));
        assert_eq!(cache.lookup(Some("a"), Some("1"), 3), Some(vec![1; 4]));
        assert!(cache.store(Some("a"), Some("3"), &[3; 4], 4));
        assert!(cache.lookup(Some("a"), Some("2"), 5).is_none());
        assert!(cache.lookup(Some("a"), Some("1"), 6).is_some());
    }
}
