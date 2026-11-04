//! The heap address space of BGI 1.622 (sub_48E760 alloc, sub_48E800 free,
//! sub_48E8C0 translation; tables at 0x503E78..0x503EE4, ADR 0005).
//!
//! Five classes of blocks; a block's address is its class's segment base
//! plus its slot index, with the low `shift` bits left for the offset inside
//! the block. The bytes themselves live in the VM's backing buffer; this
//! table maps each live slot to its backing offset.

/// (largest block, slot count, offset bits, first segment) per class.
const CLASSES: [(u32, u32, u32, u32); 5] = [
    (0x1000, 0x10000, 12, 0x10),
    (0x10000, 0x1000, 16, 0x14),
    (0x10_0000, 0x100, 20, 0x18),
    (0x100_0000, 0x10, 24, 0x1C),
    (0x400_0000, 0x20, 26, 0x20),
];

#[derive(Debug, Clone, Default)]
pub(crate) struct TargetHeap {
    /// Backing offset + 1 of each slot (0: free), grown on demand.
    slots: [Vec<u32>; 5],
    /// No slot below this index is free.
    lowest_free: [usize; 5],
}

impl TargetHeap {
    /// sub_48E760: the first class whose blocks hold `size` bytes and its
    /// lowest free slot; `None` when the class is full (the target then
    /// tries no other class) or `size` is above 64 MiB.
    pub(crate) fn alloc(&mut self, size: u32, backing: u32) -> Option<u32> {
        let class = CLASSES.iter().position(|&(max, ..)| size <= max)?;
        let (_, count, shift, base) = CLASSES[class];
        let slots = &mut self.slots[class];
        let start = self.lowest_free[class];
        let slot = slots[start.min(slots.len())..]
            .iter()
            .position(|&entry| entry == 0)
            .map_or(slots.len(), |index| start + index);
        if slot as u32 >= count {
            return None;
        }
        if slot == slots.len() {
            slots.push(0);
        }
        slots[slot] = backing + 1;
        self.lowest_free[class] = slot + 1;
        let slot = slot as u32;
        let per_segment = 26 - shift;
        Some(((slot & ((1 << per_segment) - 1)) << shift) | ((base + (slot >> per_segment)) << 26))
    }

    /// Whether `alloc(size, _)` would find a slot.
    pub(crate) fn has_room(&self, size: u32) -> bool {
        CLASSES.iter().position(|&(max, ..)| size <= max).is_some_and(|class| {
            let slots = &self.slots[class];
            (slots.len() as u32) < CLASSES[class].1
                || slots[self.lowest_free[class].min(slots.len())..].contains(&0)
        })
    }

    fn decode(ptr: u32) -> Option<(usize, usize, u32)> {
        let segment = ptr >> 26;
        CLASSES.iter().enumerate().find_map(|(class, &(_, count, shift, base))| {
            let limit = base + (count << shift >> 26).max(1);
            if !(base..limit).contains(&segment) {
                return None;
            }
            let slot = ((ptr & 0x03FF_FFFF) >> shift) | ((segment - base) << (26 - shift));
            Some((class, slot as usize, ptr & ((1u32 << shift) - 1)))
        })
    }

    /// sub_48E8C0 for heap segments: (backing offset of the block, offset
    /// inside it), or `None` for a free slot or a non-heap address.
    pub(crate) fn translate(&self, ptr: u32) -> Option<(u32, u32)> {
        let (class, slot, offset) = Self::decode(ptr)?;
        let entry = *self.slots[class].get(slot)?;
        (entry != 0).then(|| (entry - 1, offset))
    }

    /// sub_48E800: frees the block `ptr` starts; the offset bits must be 0.
    /// Returns its backing offset.
    pub(crate) fn free(&mut self, ptr: u32) -> Option<u32> {
        let (class, slot, offset) = Self::decode(ptr)?;
        if offset != 0 {
            return None;
        }
        let entry = self.slots[class].get_mut(slot)?;
        let backing = entry.checked_sub(1)?;
        *entry = 0;
        self.lowest_free[class] = self.lowest_free[class].min(slot);
        Some(backing)
    }

    /// The script address of backing offset `backing`, inside one of the
    /// live blocks (`blocks`: backing offset -> size).
    pub(crate) fn address_of(&self, backing: u32, blocks: &std::collections::BTreeMap<u32, u32>) -> Option<u32> {
        let (&start, &size) = blocks.range(..=backing).next_back()?;
        if backing >= start + size {
            return None;
        }
        self.slots.iter().enumerate().find_map(|(class, slots)| {
            let slot = slots.iter().position(|&entry| entry == start + 1)? as u32;
            let (_, _, shift, base) = CLASSES[class];
            let per_segment = 26 - shift;
            let block = ((slot & ((1 << per_segment) - 1)) << shift) | ((base + (slot >> per_segment)) << 26);
            Some(block + (backing - start))
        })
    }

    pub(crate) fn is_heap_address(ptr: u32) -> bool {
        (0x10..0x40).contains(&(ptr >> 26))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn addresses_follow_the_target_classes() {
        let mut heap = TargetHeap::default();
        // Class 0: segment 0x10, 4 KiB apart.
        assert_eq!(heap.alloc(16, 100), Some(0x4000_0000));
        assert_eq!(heap.alloc(0x1000, 200), Some(0x4000_1000));
        // Class 1 starts at segment 0x14, class 4 at 0x20.
        assert_eq!(heap.alloc(0x1001, 300), Some(0x5000_0000));
        assert_eq!(heap.alloc(0x200_0000, 400), Some(0x8000_0000));
        assert_eq!(heap.alloc(0x400_0001, 500), None);
        assert_eq!(heap.translate(0x4000_1010), Some((200, 0x10)));
        assert_eq!(heap.translate(0x4000_2000), None);
        // Free needs the block start; the slot is then reused first.
        assert_eq!(heap.free(0x4000_0004), None);
        assert_eq!(heap.free(0x4000_0000), Some(100));
        assert_eq!(heap.alloc(8, 600), Some(0x4000_0000));
        // Slot 0x4000 of class 0 is the first block of segment 0x11.
        let mut heap = TargetHeap::default();
        for backing in 0..0x4001 {
            heap.alloc(1, backing);
        }
        assert_eq!(heap.translate(0x4400_0000), Some((0x4000, 0)));
    }
}
