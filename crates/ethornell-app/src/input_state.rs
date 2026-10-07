use std::collections::BTreeMap;

/// Target `sub_46D660` keeps a descending, duplicate-preserving chain of
/// packed input scopes. `sub_46D7A0/sub_46D7B0` remove one matching node.
#[derive(Debug, Default)]
pub(crate) struct NativeInputScopeRegistry {
    counts: BTreeMap<u32, u32>,
}

impl NativeInputScopeRegistry {
    pub(crate) fn register(&mut self, packed_scope: i32) {
        let count = self.counts.entry(packed_scope as u32).or_default();
        *count = count.saturating_add(1);
    }

    pub(crate) fn unregister_one(&mut self, packed_scope: i32) -> bool {
        let key = packed_scope as u32;
        let Some(count) = self.counts.get_mut(&key) else {
            return false;
        };
        *count -= 1;
        if *count == 0 {
            self.counts.remove(&key);
        }
        true
    }

    pub(crate) fn contains(&self, packed_scope: &i32) -> bool {
        self.counts.contains_key(&(*packed_scope as u32))
    }

    pub(crate) fn scopes_with_counts(&self) -> impl Iterator<Item = (u32, u32)> + '_ {
        self.counts.iter().map(|(&scope, &count)| (scope, count))
    }

    pub(crate) fn top_scope(&self) -> Option<u32> {
        self.counts.last_key_value().map(|(&scope, _)| scope)
    }
}

/// Exact class descriptor tables at target addresses 0x50670C..0x5069B8.
/// Sys80:1B replaces only the mutable classes accepted by sub_46DFA0.
const TARGET_DEFAULT_INPUT_CLASSES: &[(i32, &[i32])] = &[
    (0x0000_0001, &[1]),
    (0x0000_0002, &[2]),
    (0x0000_0004, &[4]),
    (0x0000_0010, &[5]),
    (0x0000_0020, &[6]),
    (0x0000_0040, &[14]),
    (0x0000_0080, &[15]),
    (0x0000_0100, &[13]),
    (0x0000_0200, &[32]),
    (0x0000_1000, &[38]),
    (0x0000_2000, &[40]),
    (0x0000_4000, &[37]),
    (0x0000_8000, &[39]),
    (0x0001_0000, &[49, 97]),
    (0x0002_0000, &[50, 98]),
    (0x0004_0000, &[51, 99]),
    (0x0008_0000, &[52, 100]),
    (0x0010_0000, &[53, 101]),
    (0x0020_0000, &[54, 102]),
    (0x0040_0000, &[55, 103]),
    (0x0080_0000, &[56, 104]),
    (0x0100_0000, &[57, 105]),
    (0x0200_0000, &[48, 96]),
    (0x4000_0000, &[9]),
    (i32::MIN, &[17]),
];

/// sub_46DC80 reports the nineteen keyboard/configuration classes from
/// 0x40 through 0x40000000. Primitive pointer bits 1/2/4/0x10/0x20 are added
/// separately by sub_46DF00, and the sign-bit class feeds sub_46DE30 only.
pub(crate) const TARGET_EVENT_CLASS_MASKS: &[i32] = &[
    0x0000_0040,
    0x0000_0080,
    0x0000_0100,
    0x0000_0200,
    0x0000_1000,
    0x0000_2000,
    0x0000_4000,
    0x0000_8000,
    0x0001_0000,
    0x0002_0000,
    0x0004_0000,
    0x0008_0000,
    0x0010_0000,
    0x0020_0000,
    0x0040_0000,
    0x0080_0000,
    0x0100_0000,
    0x0200_0000,
    0x4000_0000,
];

pub(crate) fn target_default_input_classes() -> BTreeMap<i32, Vec<i32>> {
    TARGET_DEFAULT_INPUT_CLASSES
        .iter()
        .map(|&(mask, descriptors)| (mask, descriptors.to_vec()))
        .collect()
}

#[cfg(test)]
mod tests {
    use super::{NativeInputScopeRegistry, TARGET_EVENT_CLASS_MASKS, target_default_input_classes};

    #[test]
    fn exact_target_default_descriptor_tables_are_complete() {
        let classes = target_default_input_classes();
        assert_eq!(classes.len(), 25);
        assert_eq!(classes[&0x100], [13]);
        assert_eq!(classes[&0x1000], [38]);
        assert_eq!(classes[&0x4000], [37]);
        assert_eq!(classes[&0x0001_0000], [49, 97]);
        assert_eq!(classes[&0x0200_0000], [48, 96]);
        assert_eq!(classes[&i32::MIN], [17]);
        assert_eq!(TARGET_EVENT_CLASS_MASKS.len(), 19);
        assert!(!TARGET_EVENT_CLASS_MASKS.contains(&i32::MIN));
    }

    #[test]
    fn scope_registry_preserves_duplicates_and_unsigned_priority() {
        let mut scopes = NativeInputScopeRegistry::default();
        scopes.register(0x0007_ffff);
        scopes.register(0x0007_ffff);
        scopes.register(0x8000_ffff_u32 as i32);

        assert_eq!(scopes.top_scope(), Some(0x8000_ffff));
        assert!(scopes.unregister_one(0x0007_ffff));
        assert!(scopes.contains(&0x0007_ffff));
        assert!(scopes.unregister_one(0x0007_ffff));
        assert!(!scopes.contains(&0x0007_ffff));
    }
}
