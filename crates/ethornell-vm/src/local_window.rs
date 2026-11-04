//! Compact snapshots of idle threads' fixed data/code windows.
//! Both regions retain every byte through their last nonzero byte. Omitted
//! tails are restored as zero, including bytes scripts cleared after a write.

const REGION_BYTES: usize = super::CODE_WINDOW_OFFSET as usize;
const WINDOW_BYTES: usize = super::HEAP_OFFSET_BASE as usize;
#[cfg(not(target_os = "vita"))]
static ZERO_PAGE: [u8; 4096] = [0; 4096];

/// Process memory with conservative bounds for the active thread's two local
/// regions. Indexed writes extend only the affected prefix. Unrestricted Vec
/// mutation invalidates both bounds, retaining compatibility with direct host
/// writes without requiring callers to remember a separate dirty notification.
#[derive(Debug, Default, Clone)]
pub struct VmMemory {
    bytes: Vec<u8>,
    local_bounds: [usize; 2],
}

impl From<Vec<u8>> for VmMemory {
    fn from(bytes: Vec<u8>) -> Self {
        Self { bytes, local_bounds: [REGION_BYTES; 2] }
    }
}
impl std::ops::Deref for VmMemory {
    type Target = Vec<u8>;
    fn deref(&self) -> &Self::Target { &self.bytes }
}
impl std::ops::DerefMut for VmMemory {
    fn deref_mut(&mut self) -> &mut Self::Target {
        self.local_bounds = [REGION_BYTES; 2];
        &mut self.bytes
    }
}
impl<I: std::slice::SliceIndex<[u8]>> std::ops::Index<I> for VmMemory {
    type Output = I::Output;
    fn index(&self, index: I) -> &Self::Output { &self.bytes[index] }
}
impl<I: std::slice::SliceIndex<[u8]>> std::ops::IndexMut<I> for VmMemory {
    fn index_mut(&mut self, index: I) -> &mut Self::Output {
        let base = self.bytes.as_ptr() as usize;
        let output = &mut self.bytes[index];
        let start = std::ptr::from_mut(output).cast::<u8>() as usize - base;
        let end = start + std::mem::size_of_val(output);
        for (region, bound) in self.local_bounds.iter_mut().enumerate() {
            let base = super::LOCAL_MEMORY_BASE as usize + region * REGION_BYTES;
            if start < base + REGION_BYTES && end > base {
                *bound = (*bound).max((end - base).min(REGION_BYTES));
            }
        }
        output
    }
}
impl VmMemory {
    pub fn resize(&mut self, len: usize, value: u8) {
        let old_len = self.bytes.len();
        if len != old_len && len.min(old_len) < super::HEAP_MEMORY_BASE
            && len.max(old_len) > super::LOCAL_MEMORY_BASE as usize
        {
            self.local_bounds = [REGION_BYTES; 2];
        }
        self.bytes.resize(len, value);
    }
    pub(crate) fn local_bounds(&self) -> [usize; 2] { self.local_bounds }
    pub(crate) fn set_local_bounds(&mut self, bounds: [usize; 2]) { self.local_bounds = bounds; }
    pub(crate) fn into_vec(self) -> Vec<u8> { self.bytes }
}

#[derive(Debug, Default)]
pub(crate) struct LocalWindow {
    bytes: Vec<u8>,
    data_len: usize,
}

#[cfg(not(target_os = "vita"))]
fn nonzero_prefix(bytes: &[u8]) -> usize {
    let mut end = bytes.len();
    // Slice equality uses the platform's bulk comparison instead of scanning
    // the unused megabytes one byte at a time on the Vita CPU.
    for page in bytes.rchunks(ZERO_PAGE.len()) {
        if page != &ZERO_PAGE[..page.len()] {
            return end - page.len() + page.iter().rposition(|&b| b != 0).unwrap() + 1;
        }
        end -= page.len();
    }
    0
}

// VitaSDK's generic memcmp walks zero tails very slowly. Test an entire
// 32-byte block with word ORs instead, without calling libc or reading beyond
// the slice. Snapshots still find the exact last nonzero byte.
#[cfg(any(target_os = "vita", test))]
fn nonzero_prefix_words(bytes: &[u8]) -> usize {
    let mut end = bytes.len();
    let mut blocks = bytes.rchunks_exact(32);
    for block in &mut blocks {
        let words = unsafe { std::ptr::read_unaligned(block.as_ptr().cast::<[u32; 8]>()) };
        if words.into_iter().fold(0, |sum, word| sum | word) != 0 {
            return end - 32 + block.iter().rposition(|&byte| byte != 0).unwrap() + 1;
        }
        end -= 32;
    }
    blocks.remainder().iter().rposition(|&byte| byte != 0).map_or(0, |index| index + 1)
}
#[cfg(target_os = "vita")]
fn nonzero_prefix(bytes: &[u8]) -> usize { nonzero_prefix_words(bytes) }

impl LocalWindow {
    pub(crate) fn from_code(code: &[u8]) -> Self {
        let code = &code[..code.len().min(REGION_BYTES)];
        Self {
            bytes: code[..nonzero_prefix(code)].to_vec(),
            data_len: 0,
        }
    }

    pub(crate) fn from_dense(bytes: Vec<u8>) -> Self {
        debug_assert_eq!(bytes.len(), WINDOW_BYTES);
        let mut window = Self {
            bytes,
            data_len: REGION_BYTES,
        };
        let data_len = nonzero_prefix(&window.bytes[..REGION_BYTES]);
        let code_len = nonzero_prefix(&window.bytes[REGION_BYTES..]);
        window
            .bytes
            .copy_within(REGION_BYTES..REGION_BYTES + code_len, data_len);
        window.bytes.truncate(data_len + code_len);
        window.data_len = data_len;
        window.release_slack();
        window
    }

    pub(crate) fn capture(&mut self, source: &[u8]) {
        debug_assert_eq!(source.len(), WINDOW_BYTES);
        let data_len = nonzero_prefix(&source[..REGION_BYTES]);
        let code_len = nonzero_prefix(&source[REGION_BYTES..]);
        self.bytes.clear();
        self.bytes.extend_from_slice(&source[..data_len]);
        self.bytes
            .extend_from_slice(&source[REGION_BYTES..REGION_BYTES + code_len]);
        self.data_len = data_len;
        self.release_slack();
    }

    /// Exchange compact snapshots directly in the live window. The outgoing
    /// nonzero bounds prove the rest is already zero, so restoring an incoming
    /// snapshot only clears the outgoing tails that actually contained data.
    /// No full 2 MiB staging image or final full-window copy is needed.
    pub(crate) fn exchange(&mut self, live: &mut [u8], scratch: &mut Vec<u8>) {
        self.exchange_bounded(live, scratch, [REGION_BYTES; 2]);
    }

    pub(crate) fn prefix_lengths(&self) -> [usize; 2] {
        [self.data_len, self.bytes.len() - self.data_len]
    }

    pub(crate) fn exchange_bounded(&mut self, live: &mut [u8], scratch: &mut Vec<u8>, bounds: [usize; 2]) {
        debug_assert_eq!(live.len(), WINDOW_BYTES);
        let data_len = nonzero_prefix(&live[..bounds[0]]);
        let code_len = nonzero_prefix(&live[REGION_BYTES..REGION_BYTES + bounds[1]]);
        scratch.clear();
        scratch.extend_from_slice(&live[..data_len]);
        scratch.extend_from_slice(&live[REGION_BYTES..REGION_BYTES + code_len]);
        let incoming_code_len = self.bytes.len() - self.data_len;
        live[..self.data_len].copy_from_slice(&self.bytes[..self.data_len]);
        if data_len > self.data_len {
            live[self.data_len..data_len].fill(0);
        }
        live[REGION_BYTES..REGION_BYTES + incoming_code_len]
            .copy_from_slice(&self.bytes[self.data_len..]);
        if code_len > incoming_code_len {
            live[REGION_BYTES + incoming_code_len..REGION_BYTES + code_len].fill(0);
        }
        self.bytes.clear();
        self.bytes.extend_from_slice(scratch);
        self.data_len = data_len;
        self.release_slack();
    }

    pub(crate) fn restore(&self, destination: &mut [u8]) {
        debug_assert_eq!(destination.len(), WINDOW_BYTES);
        let code_len = self.bytes.len() - self.data_len;
        destination[..self.data_len].copy_from_slice(&self.bytes[..self.data_len]);
        destination[self.data_len..REGION_BYTES].fill(0);
        destination[REGION_BYTES..REGION_BYTES + code_len]
            .copy_from_slice(&self.bytes[self.data_len..]);
        destination[REGION_BYTES + code_len..].fill(0);
    }

    pub(crate) fn as_mut_slice(&mut self) -> &mut [u8] {
        let code_len = self.bytes.len() - self.data_len;
        self.bytes.resize(WINDOW_BYTES, 0);
        self.bytes
            .copy_within(self.data_len..self.data_len + code_len, REGION_BYTES);
        self.bytes[self.data_len..REGION_BYTES].fill(0);
        self.bytes[REGION_BYTES + code_len..].fill(0);
        self.data_len = REGION_BYTES;
        &mut self.bytes
    }

    fn release_slack(&mut self) {
        if self.bytes.capacity() > self.bytes.len().saturating_mul(2).saturating_add(16 * 1024) {
            self.bytes.shrink_to_fit();
        }
    }

    #[cfg(test)]
    pub(crate) fn byte(&self, offset: usize) -> u8 {
        if offset < REGION_BYTES {
            if offset < self.data_len {
                self.bytes[offset]
            } else {
                0
            }
        } else {
            self.bytes
                .get(self.data_len + offset - REGION_BYTES)
                .copied()
                .unwrap_or(0)
        }
    }

    #[cfg(test)]
    pub(crate) fn is_empty(&self) -> bool {
        self.bytes.is_empty()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn indexed_writes_bound_scans_and_unrestricted_mutation_invalidates_bounds() {
        let mut live = VmMemory::from(vec![0; super::super::INITIAL_MEMORY_SIZE]);
        live.set_local_bounds([0, 0]);
        let base = super::super::LOCAL_MEMORY_BASE as usize;
        live[0..16].fill(255);
        assert_eq!(live.local_bounds(), [0, 0]);
        live[base + 5] = 7;
        live[base + REGION_BYTES + 11..base + REGION_BYTES + 15].copy_from_slice(&[1, 2, 3, 4]);
        assert_eq!(live.local_bounds(), [6, 15]);
        let _ = live[base + WINDOW_BYTES - 1];
        assert_eq!(live.local_bounds(), [6, 15]);
        let mut saved = LocalWindow::default();
        let mut scratch = Vec::new();
        let bounds = live.local_bounds();
        saved.exchange_bounded(&mut live[base..base + WINDOW_BYTES], &mut scratch, bounds);
        live.set_local_bounds([0, 0]);
        assert_eq!(saved.byte(5), 7);
        assert_eq!(saved.byte(REGION_BYTES + 14), 4);
        assert!(live[base..base + WINDOW_BYTES].iter().all(|&byte| byte == 0));
        // get_mut goes through DerefMut, conservatively covering arbitrary
        // host changes through a Vec or mutable slice.
        *live.get_mut(base + WINDOW_BYTES - 1).unwrap() = 99;
        assert_eq!(live.local_bounds(), [REGION_BYTES; 2]);
        let bounds = live.local_bounds();
        saved.exchange_bounded(&mut live[base..base + WINDOW_BYTES], &mut scratch, bounds);
        assert_eq!(saved.byte(WINDOW_BYTES - 1), 99);
        assert_eq!(live[base + 5], 7);
        assert_eq!(live[base + WINDOW_BYTES - 1], 0);
    }

    #[test]
    fn word_scan_matches_byte_scan_for_unaligned_slices_and_partial_blocks() {
        let mut bytes = vec![0; 300];
        for offset in 0..8 {
            for len in 0..256 {
                assert_eq!(nonzero_prefix_words(&bytes[offset..offset + len]), 0);
                for index in 0..len {
                    bytes[offset + index] = 128;
                    assert_eq!(nonzero_prefix_words(&bytes[offset..offset + len]), index + 1);
                    bytes[offset + index] = 0;
                }
            }
        }
    }

    #[test]
    fn idle_window_keeps_both_regions_and_cleared_tails() {
        let mut original = vec![0; WINDOW_BYTES];
        original[0] = 1;
        original[4096] = 2;
        original[REGION_BYTES + 33] = 3;
        let mut window = LocalWindow::from_dense(original.clone());
        assert_eq!(window.bytes.len(), 4097 + 34);
        assert!(window.bytes.capacity() < 16 * 1024);
        let mut restored = vec![0xff; WINDOW_BYTES];
        window.restore(&mut restored);
        assert_eq!(original, restored);
        original[4096] = 0;
        original[REGION_BYTES + 33] = 0;
        window.capture(&original);
        assert_eq!(window.bytes, [1]);
        window.restore(&mut restored);
        assert_eq!(original, restored);
    }

    #[test]
    fn exchanges_clear_only_old_tails_and_preserve_both_windows() {
        let mut a = vec![0; WINDOW_BYTES];
        let mut b = vec![0; WINDOW_BYTES];
        for (offset, value) in [(0, 1), (4096, 2), (REGION_BYTES + 50, 3)] {
            a[offset] = value;
        }
        for (offset, value) in [(77, 4), (REGION_BYTES - 1, 5), (WINDOW_BYTES - 1, 6)] {
            b[offset] = value;
        }
        let mut saved = LocalWindow::from_dense(a.clone());
        let mut live = b.clone();
        let mut scratch = Vec::new();
        for i in 0..20 {
            saved.exchange(&mut live, &mut scratch);
            assert_eq!(&live, if i % 2 == 0 { &a } else { &b });
            let mut other = vec![255; WINDOW_BYTES];
            saved.restore(&mut other);
            assert_eq!(&other, if i % 2 == 0 { &b } else { &a });
        }
    }

    #[test]
    fn child_borrowing_and_last_byte_writes_round_trip() {
        let mut window = LocalWindow::default();
        let bytes = window.as_mut_slice();
        bytes[REGION_BYTES - 1] = 0x12;
        bytes[WINDOW_BYTES - 1] = 0x34;
        let mut restored = vec![0; WINDOW_BYTES];
        window.restore(&mut restored);
        assert_eq!(restored[REGION_BYTES - 1], 0x12);
        assert_eq!(restored[WINDOW_BYTES - 1], 0x34);
        let compact = LocalWindow::from_dense(restored.clone());
        assert_eq!(compact.bytes.len(), WINDOW_BYTES);
        let mut copied = vec![0; WINDOW_BYTES];
        compact.restore(&mut copied);
        assert_eq!(copied, restored);
    }
}
