use std::collections::BTreeMap;

pub(crate) const NATIVE_INVALID_ARGUMENT: i32 = i32::MIN + 1;
pub(crate) const NATIVE_NOT_FOUND: i32 = i32::MIN + 2;
pub(crate) const NATIVE_OPERATION_FAILED: i32 = i32::MIN + 3;
pub(crate) const NATIVE_INVALID_INDEX: i32 = i32::MIN + 4;

#[derive(Debug, Clone)]
pub(crate) struct ResourceStream {
    pub(crate) path: String,
    pub(crate) mode: i32,
    pub(crate) bytes: Vec<u8>,
    pub(crate) cursor: usize,
}

/// sub_4478A0: `capacity` slots of (data, size); a slot is empty while its
/// size is 0.
#[derive(Debug, Clone, Default)]
pub(crate) struct BlobTable {
    pub(crate) slots: Vec<Option<Vec<u8>>>,
}

impl BlobTable {
    pub(crate) fn with_capacity(capacity: usize) -> Self {
        Self { slots: vec![None; capacity] }
    }

    /// sub_447910: the first empty slot, or the capacity when all are used.
    pub(crate) fn first_free(&self) -> u32 {
        self.slots.iter().position(Option::is_none).unwrap_or(self.slots.len()) as u32
    }

    /// sub_447960: an index past the end doubles the capacity until it fits.
    pub(crate) fn store(&mut self, index: u32, bytes: Vec<u8>) {
        let index = index as usize;
        if index >= self.slots.len() {
            let mut capacity = self.slots.len();
            while index >= capacity {
                capacity *= 2;
            }
            self.slots.resize(capacity, None);
        }
        self.slots[index] = Some(bytes);
    }

    /// sub_447A20.
    pub(crate) fn remove(&mut self, index: u32) -> bool {
        self.slots.get_mut(index as usize).and_then(Option::take).is_some()
    }

    pub(crate) fn get(&self, index: u32) -> Option<&[u8]> {
        self.slots.get(index as usize).and_then(Option::as_deref)
    }

    /// sub_447AA0: (index, size) of the stored slots in index order.
    pub(crate) fn entries(&self) -> Vec<(u32, u32)> {
        self.slots
            .iter()
            .enumerate()
            .filter_map(|(index, slot)| slot.as_ref().map(|bytes| (index as u32, bytes.len() as u32)))
            .collect()
    }
}

#[derive(Debug)]
pub(crate) struct System81SharedState {
    pub(crate) native_utf8_override: Option<bool>,
    pub(crate) graph91_resource_mode: i32,
    pub(crate) sentence_table: crate::sentence_table::SentenceTable,
    pub(crate) clock_jump_threshold_ms: i32,
    pub(crate) input_binding_values: [i32; 256],
    pub(crate) keyboard_polling_override: i32,
    /// Target dword_507690 shared by CProcedure/CProcDspMsg input filtering.
    pub(crate) message_auxiliary_input_mask: i32,
    /// dword_5666B4 / dbl_5666C0: touch history capacity and minimum step.
    pub(crate) touch_history_capacity: u32,
    pub(crate) touch_history_step: f64,
    pub(crate) controller_wake_entries: [i32; 36],
    pub(crate) resource_streams: BTreeMap<u32, ResourceStream>,
    pub(crate) next_resource_stream_handle: u32,
    pub(crate) blob_tables: BTreeMap<u32, BlobTable>,
    pub(crate) last_blob_table_handle: u32,
    /// dword_566988: handle (++dword_56697C) -> host mutex token.
    pub(crate) named_mutexes: BTreeMap<u32, u64>,
    pub(crate) last_mutex_handle: u32,
    pub(crate) window_position_override: i32,
    pub(crate) pause_on_deactivate: i32,
    pub(crate) print_screen_hotkeys_enabled: bool,
    pub(crate) error_capture_mode: i32,
    /// sub_4649E0 reads a nullable error buffer; `None` is the null pointer.
    pub(crate) captured_error: Option<String>,
    pub(crate) pixel_shader_version: u16,
}

impl Default for System81SharedState {
    fn default() -> Self {
        Self {
            native_utf8_override: None,
            graph91_resource_mode: 0,
            sentence_table: crate::sentence_table::SentenceTable::default(),
            // dword_503DE8
            clock_jump_threshold_ms: 500,
            input_binding_values: [0; 256],
            keyboard_polling_override: 0,
            // dword_507690
            message_auxiliary_input_mask: 0x2000,
            touch_history_capacity: 0,
            touch_history_step: 0.0,
            controller_wake_entries: [0; 36],
            resource_streams: BTreeMap::new(),
            next_resource_stream_handle: 1,
            blob_tables: BTreeMap::new(),
            last_blob_table_handle: 0,
            named_mutexes: BTreeMap::new(),
            last_mutex_handle: 0,
            window_position_override: 0,
            pause_on_deactivate: 0,
            print_screen_hotkeys_enabled: false,
            error_capture_mode: 0,
            captured_error: None,
            pixel_shader_version: 0,
        }
    }
}

impl System81SharedState {
    pub(crate) fn configure_touch_history(&mut self, capacity: u32, step: u32) -> bool {
        if capacity > 0x200 {
            return false;
        }
        self.touch_history_capacity = capacity;
        self.touch_history_step = f64::from(step);
        true
    }

    pub(crate) fn open_resource_stream(
        &mut self,
        path: String,
        mode: i32,
        bytes: Vec<u8>,
    ) -> Result<u32, i32> {
        if !(0..=2).contains(&mode) {
            return Err(NATIVE_INVALID_ARGUMENT);
        }
        if self
            .resource_streams
            .values()
            .any(|stream| stream.path == path)
        {
            return Err(NATIVE_NOT_FOUND);
        }
        let handle = self.next_resource_stream_handle.max(1);
        self.next_resource_stream_handle = handle.wrapping_add(1).max(1);
        self.resource_streams.insert(
            handle,
            ResourceStream {
                path,
                mode,
                bytes,
                cursor: 0,
            },
        );
        Ok(handle)
    }

    /// sub_46AE10: handles count up from dword_566684 (0) and wrap.
    pub(crate) fn create_blob_table(&mut self, capacity: u32) -> Option<u32> {
        if capacity <= 1 {
            return None;
        }
        self.last_blob_table_handle = self.last_blob_table_handle.wrapping_add(1);
        let handle = self.last_blob_table_handle;
        self.blob_tables.insert(handle, BlobTable::with_capacity(capacity as usize));
        Some(handle)
    }

}

pub(crate) fn md5_digest(input: &[u8]) -> [u8; 16] {
    const S: [u32; 64] = [
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5,
        9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10,
        15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
    ];
    const K: [u32; 64] = [
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613,
        0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193,
        0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d,
        0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
        0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
        0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244,
        0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb,
        0xeb86d391,
    ];

    let bit_len = (input.len() as u64).wrapping_mul(8);
    let mut message = input.to_vec();
    message.push(0x80);
    while message.len() % 64 != 56 {
        message.push(0);
    }
    message.extend_from_slice(&bit_len.to_le_bytes());

    let mut a0 = 0x67452301u32;
    let mut b0 = 0xefcdab89u32;
    let mut c0 = 0x98badcfeu32;
    let mut d0 = 0x10325476u32;

    for chunk in message.chunks_exact(64) {
        let mut words = [0u32; 16];
        for (index, word) in words.iter_mut().enumerate() {
            let start = index * 4;
            *word = u32::from_le_bytes(chunk[start..start + 4].try_into().unwrap());
        }
        let (mut a, mut b, mut c, mut d) = (a0, b0, c0, d0);
        for index in 0..64 {
            let (f, g) = match index {
                0..=15 => ((b & c) | ((!b) & d), index),
                16..=31 => ((d & b) | ((!d) & c), (5 * index + 1) % 16),
                32..=47 => (b ^ c ^ d, (3 * index + 5) % 16),
                _ => (c ^ (b | !d), (7 * index) % 16),
            };
            let next = a
                .wrapping_add(f)
                .wrapping_add(K[index])
                .wrapping_add(words[g])
                .rotate_left(S[index])
                .wrapping_add(b);
            a = d;
            d = c;
            c = b;
            b = next;
        }
        a0 = a0.wrapping_add(a);
        b0 = b0.wrapping_add(b);
        c0 = c0.wrapping_add(c);
        d0 = d0.wrapping_add(d);
    }

    let mut digest = [0u8; 16];
    digest[0..4].copy_from_slice(&a0.to_le_bytes());
    digest[4..8].copy_from_slice(&b0.to_le_bytes());
    digest[8..12].copy_from_slice(&c0.to_le_bytes());
    digest[12..16].copy_from_slice(&d0.to_le_bytes());
    digest
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn md5_matches_standard_vectors() {
        assert_eq!(
            md5_digest(b""),
            [
                0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8,
                0x42, 0x7e,
            ]
        );
        assert_eq!(
            md5_digest(b"abc"),
            [
                0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1,
                0x7f, 0x72,
            ]
        );
    }

    #[test]
    fn blob_slots_grow_by_doubling_and_reuse_first_free_entry() {
        let mut table = BlobTable::with_capacity(2);
        table.store(1, vec![1, 2]);
        assert_eq!(table.first_free(), 0);
        table.store(0, vec![3]);
        assert_eq!(table.first_free(), 2);
        table.store(5, vec![4, 5, 6]);
        assert_eq!(table.slots.len(), 8);
        assert_eq!(table.entries(), vec![(0, 1), (1, 2), (5, 3)]);
    }
}
