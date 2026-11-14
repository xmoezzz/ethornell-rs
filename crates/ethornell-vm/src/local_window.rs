//! Compact snapshots of idle threads' fixed data/code windows.
//! Dirty spans are retained; every byte outside them is zero when restored,
//! including bytes cleared after a write.

const REGION_BYTES: usize = super::CODE_WINDOW_OFFSET as usize;
const CODE_BYTES: usize = super::CODE_WINDOW_SIZE as usize;
const REGION_LIMITS: [usize; 2] = [REGION_BYTES, CODE_BYTES];
const WINDOW_BYTES: usize = super::HEAP_OFFSET_BASE as usize;
const MAX_REGION_SPANS: usize = 16;

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct RegionSpans {
    ranges: [[usize; 2]; MAX_REGION_SPANS],
    len: usize,
}

impl RegionSpans {
    fn prefix(end: usize) -> Self {
        let mut spans = Self::default();
        if end != 0 {
            spans.ranges[0] = [0, end];
            spans.len = 1;
        }
        spans
    }

    fn full(limit: usize) -> Self {
        Self::prefix(limit)
    }

    fn as_slice(&self) -> &[[usize; 2]] {
        &self.ranges[..self.len]
    }

    fn end(&self) -> usize {
        self.as_slice().last().map_or(0, |range| range[1])
    }

    fn byte_len(&self) -> usize {
        self.as_slice()
            .iter()
            .map(|range| range[1] - range[0])
            .sum()
    }

    fn insert(&mut self, start: usize, end: usize, limit: usize) {
        let start = start.min(limit);
        let end = end.min(limit);
        if start >= end {
            return;
        }
        if self
            .as_slice()
            .iter()
            .any(|range| range[0] <= start && end <= range[1])
        {
            return;
        }
        let mut merged_start = start;
        let mut merged_end = end;
        let mut first = 0;
        while first < self.len && self.ranges[first][1] < merged_start {
            first += 1;
        }
        let mut last = first;
        while last < self.len && self.ranges[last][0] <= merged_end {
            merged_start = merged_start.min(self.ranges[last][0]);
            merged_end = merged_end.max(self.ranges[last][1]);
            last += 1;
        }
        if first == last {
            if self.len == MAX_REGION_SPANS {
                *self = Self::prefix(self.end().max(end));
                return;
            }
            if first < self.len {
                self.ranges.copy_within(first..self.len, first + 1);
            }
            self.len += 1;
        } else {
            self.ranges.copy_within(last..self.len, first + 1);
            self.len -= last - first - 1;
        }
        self.ranges[first] = [merged_start, merged_end];
    }
}

#[cfg(not(target_os = "vita"))]
static ZERO_PAGE: [u8; 4096] = [0; 4096];

/// Process memory with conservative tracking for the active thread's two local
/// regions. Indexed writes extend only the affected spans. Unrestricted Vec
/// mutation marks both regions fully dirty for compatibility with direct host
/// writes without requiring callers to remember a separate dirty notification.
#[derive(Debug, Default, Clone)]
pub struct VmMemory {
    bytes: Vec<u8>,
    local_bounds: [usize; 2],
    local_spans: [RegionSpans; 2],
}

impl From<Vec<u8>> for VmMemory {
    fn from(bytes: Vec<u8>) -> Self {
        Self {
            bytes,
            local_bounds: REGION_LIMITS,
            local_spans: [
                RegionSpans::full(REGION_BYTES),
                RegionSpans::full(CODE_BYTES),
            ],
        }
    }
}

impl VmMemory {
    // The VM constructors pass only a zero-filled or empty process buffer.
    pub(crate) fn from_zeroed(bytes: Vec<u8>) -> Self {
        Self {
            bytes,
            local_bounds: [0, 0],
            local_spans: [RegionSpans::default(); 2],
        }
    }
}

impl std::ops::Deref for VmMemory {
    type Target = Vec<u8>;
    fn deref(&self) -> &Self::Target {
        &self.bytes
    }
}
impl std::ops::DerefMut for VmMemory {
    fn deref_mut(&mut self) -> &mut Self::Target {
        self.local_bounds = REGION_LIMITS;
        self.local_spans = [
            RegionSpans::full(REGION_BYTES),
            RegionSpans::full(CODE_BYTES),
        ];
        &mut self.bytes
    }
}
impl<I: std::slice::SliceIndex<[u8]>> std::ops::Index<I> for VmMemory {
    type Output = I::Output;
    fn index(&self, index: I) -> &Self::Output {
        &self.bytes[index]
    }
}
impl<I: std::slice::SliceIndex<[u8]>> std::ops::IndexMut<I> for VmMemory {
    fn index_mut(&mut self, index: I) -> &mut Self::Output {
        let base = self.bytes.as_ptr() as usize;
        let output = &mut self.bytes[index];
        let start = std::ptr::from_mut(output).cast::<u8>() as usize - base;
        let end = start + std::mem::size_of_val(output);
        for region in 0..2 {
            let base =
                super::LOCAL_MEMORY_BASE as usize + if region == 0 { 0 } else { REGION_BYTES };
            let limit = REGION_LIMITS[region];
            if start < base + limit && end > base {
                let from = start.saturating_sub(base).min(limit);
                let to = end.saturating_sub(base).min(limit);
                self.local_bounds[region] = self.local_bounds[region].max(to);
                self.local_spans[region].insert(from, to, limit);
            }
        }
        output
    }
}
impl VmMemory {
    pub fn resize(&mut self, len: usize, value: u8) {
        let old_len = self.bytes.len();
        if len != old_len
            && len.min(old_len) < super::HEAP_MEMORY_BASE
            && len.max(old_len) > super::LOCAL_MEMORY_BASE as usize
        {
            self.local_bounds = REGION_LIMITS;
            self.local_spans = [
                RegionSpans::full(REGION_BYTES),
                RegionSpans::full(CODE_BYTES),
            ];
        }
        self.bytes.resize(len, value);
    }
    pub(crate) fn local_bounds(&self) -> [usize; 2] {
        self.local_bounds
    }
    pub(crate) fn local_spans(&self) -> [RegionSpans; 2] {
        self.local_spans
    }
    pub(crate) fn set_local_bounds(&mut self, bounds: [usize; 2]) {
        self.local_bounds = bounds;
        self.local_spans = [
            RegionSpans::prefix(bounds[0]),
            RegionSpans::prefix(bounds[1]),
        ];
    }
    pub(crate) fn set_local_spans(&mut self, spans: [RegionSpans; 2]) {
        self.local_bounds = [spans[0].end(), spans[1].end()];
        self.local_spans = spans;
    }
    pub(crate) fn into_vec(self) -> Vec<u8> {
        self.bytes
    }
}

#[derive(Debug, Default)]
pub(crate) struct LocalWindow {
    bytes: Vec<u8>,
    data_len: usize,
    data_spans: RegionSpans,
    code_spans: RegionSpans,
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
    blocks
        .remainder()
        .iter()
        .rposition(|&byte| byte != 0)
        .map_or(0, |index| index + 1)
}
#[cfg(target_os = "vita")]
fn nonzero_prefix(bytes: &[u8]) -> usize {
    nonzero_prefix_words(bytes)
}

fn nonzero_spans(bytes: &[u8]) -> RegionSpans {
    const PAGE: usize = 4096;
    let mut spans = RegionSpans::default();
    let mut run_start = None;
    let mut run_end = 0usize;
    for (page_index, page) in bytes.chunks(PAGE).enumerate() {
        let start = page_index * PAGE;
        let end = nonzero_prefix(page);
        if end != 0 {
            if run_start.is_none() {
                run_start = Some(start + page.iter().position(|&byte| byte != 0).unwrap_or(0));
            }
            run_end = start + end;
        } else if let Some(begin) = run_start.take() {
            spans.insert(begin, run_end, bytes.len());
        }
    }
    if let Some(begin) = run_start {
        spans.insert(begin, run_end, bytes.len());
    }
    if spans.len == 1 && spans.ranges[0][0] < PAGE {
        RegionSpans::prefix(spans.end())
    } else {
        spans
    }
}

#[cfg(debug_assertions)]
fn assert_zero_outside_spans(bytes: &[u8], spans: &RegionSpans) {
    let mut position = 0;
    for &[start, end] in spans.as_slice() {
        debug_assert!(bytes[position..start].iter().all(|&byte| byte == 0));
        position = end;
    }
    debug_assert!(bytes[position..].iter().all(|&byte| byte == 0));
}

fn append_spans(target: &mut Vec<u8>, source: &[u8], spans: &RegionSpans) {
    for &[start, end] in spans.as_slice() {
        target.extend_from_slice(&source[start..end]);
    }
}

fn restore_region(destination: &mut [u8], spans: &RegionSpans, packed: &[u8], cursor: &mut usize) {
    let mut position = 0;
    for &[start, end] in spans.as_slice() {
        debug_assert!(position <= start && start < end && end <= destination.len());
        destination[position..start].fill(0);
        let len = end - start;
        destination[start..end].copy_from_slice(&packed[*cursor..*cursor + len]);
        *cursor += len;
        position = end;
    }
    destination[position..].fill(0);
}

impl LocalWindow {
    pub(crate) fn from_code(code: &[u8]) -> Self {
        let code = &code[..code.len().min(CODE_BYTES)];
        let code_len = nonzero_prefix(code);
        Self {
            bytes: code[..code_len].to_vec(),
            data_len: 0,
            data_spans: RegionSpans::default(),
            code_spans: RegionSpans::prefix(code_len),
        }
    }

    pub(crate) fn from_dense(mut bytes: Vec<u8>) -> Self {
        debug_assert_eq!(bytes.len(), WINDOW_BYTES);
        let data_spans = nonzero_spans(&bytes[..REGION_BYTES]);
        let code_spans = nonzero_spans(&bytes[REGION_BYTES..]);
        let data_prefix = data_spans.len <= 1
            && data_spans
                .as_slice()
                .first()
                .is_none_or(|range| range[0] == 0);
        let code_prefix = code_spans.len <= 1
            && code_spans
                .as_slice()
                .first()
                .is_none_or(|range| range[0] == 0);
        if data_prefix && code_prefix {
            let data_len = data_spans.byte_len();
            let code_len = code_spans.byte_len();
            bytes.copy_within(REGION_BYTES..REGION_BYTES + code_len, data_len);
            bytes.truncate(data_len + code_len);
            let mut window = Self {
                bytes,
                data_len,
                data_spans,
                code_spans,
            };
            window.release_slack();
            return window;
        }
        let mut packed = Vec::with_capacity(data_spans.byte_len() + code_spans.byte_len());
        append_spans(&mut packed, &bytes[..REGION_BYTES], &data_spans);
        let data_len = packed.len();
        append_spans(&mut packed, &bytes[REGION_BYTES..], &code_spans);
        let mut window = Self {
            bytes: packed,
            data_len,
            data_spans,
            code_spans,
        };
        window.release_slack();
        window
    }

    pub(crate) fn capture(&mut self, source: &[u8]) {
        debug_assert_eq!(source.len(), WINDOW_BYTES);
        let data_spans = nonzero_spans(&source[..REGION_BYTES]);
        let code_spans = nonzero_spans(&source[REGION_BYTES..]);
        self.bytes.clear();
        append_spans(&mut self.bytes, &source[..REGION_BYTES], &data_spans);
        self.data_len = self.bytes.len();
        append_spans(&mut self.bytes, &source[REGION_BYTES..], &code_spans);
        self.data_spans = data_spans;
        self.code_spans = code_spans;
        self.release_slack();
    }

    /// Exchange compact snapshots directly in the live window. Outgoing dirty
    /// spans cover every possibly nonzero byte, so only those ranges need to be
    /// cleared before restoring incoming spans. No full-window staging image or
    /// final full-window copy is needed.
    pub(crate) fn exchange(&mut self, live: &mut [u8], scratch: &mut Vec<u8>) {
        self.exchange_bounded(live, scratch, REGION_LIMITS);
    }

    pub(crate) fn spans(&self) -> [RegionSpans; 2] {
        [self.data_spans, self.code_spans]
    }

    pub(crate) fn exchange_bounded(
        &mut self,
        live: &mut [u8],
        scratch: &mut Vec<u8>,
        bounds: [usize; 2],
    ) {
        debug_assert_eq!(live.len(), WINDOW_BYTES);
        let data_spans = nonzero_spans(&live[..bounds[0]]);
        let code_spans = nonzero_spans(&live[REGION_BYTES..REGION_BYTES + bounds[1]]);
        self.exchange_spanned(live, scratch, [data_spans, code_spans]);
    }

    pub(crate) fn exchange_spanned(
        &mut self,
        live: &mut [u8],
        scratch: &mut Vec<u8>,
        outgoing: [RegionSpans; 2],
    ) {
        debug_assert_eq!(live.len(), WINDOW_BYTES);
        scratch.clear();
        append_spans(scratch, &live[..REGION_BYTES], &outgoing[0]);
        let data_len = scratch.len();
        append_spans(scratch, &live[REGION_BYTES..], &outgoing[1]);

        for &[start, end] in outgoing[0].as_slice() {
            live[start..end].fill(0);
        }
        for &[start, end] in outgoing[1].as_slice() {
            live[REGION_BYTES + start..REGION_BYTES + end].fill(0);
        }

        let incoming_code_len = self.bytes.len() - self.data_len;
        let mut cursor = 0;
        for &[start, end] in self.data_spans.as_slice() {
            let len = end - start;
            live[start..end].copy_from_slice(&self.bytes[cursor..cursor + len]);
            cursor += len;
        }
        debug_assert_eq!(cursor, self.data_len);
        for &[start, end] in self.code_spans.as_slice() {
            let len = end - start;
            live[REGION_BYTES + start..REGION_BYTES + end]
                .copy_from_slice(&self.bytes[cursor..cursor + len]);
            cursor += len;
        }
        debug_assert_eq!(cursor, self.data_len + incoming_code_len);
        #[cfg(debug_assertions)]
        {
            assert_zero_outside_spans(&live[..REGION_BYTES], &self.data_spans);
            assert_zero_outside_spans(&live[REGION_BYTES..], &self.code_spans);
        }

        self.bytes.clear();
        self.bytes.extend_from_slice(scratch);
        self.data_len = data_len;
        self.data_spans = outgoing[0];
        self.code_spans = outgoing[1];
        self.release_slack();
    }

    pub(crate) fn restore(&self, destination: &mut [u8]) {
        debug_assert_eq!(destination.len(), WINDOW_BYTES);
        let mut cursor = 0;
        restore_region(
            &mut destination[..REGION_BYTES],
            &self.data_spans,
            &self.bytes,
            &mut cursor,
        );
        restore_region(
            &mut destination[REGION_BYTES..],
            &self.code_spans,
            &self.bytes,
            &mut cursor,
        );
        debug_assert_eq!(cursor, self.bytes.len());
    }

    pub(crate) fn as_mut_slice(&mut self) -> &mut [u8] {
        let data_prefix = self.data_spans.len <= 1
            && self
                .data_spans
                .as_slice()
                .first()
                .is_none_or(|range| range[0] == 0);
        let code_prefix = self.code_spans.len <= 1
            && self
                .code_spans
                .as_slice()
                .first()
                .is_none_or(|range| range[0] == 0);
        if data_prefix && code_prefix {
            let code_len = self.bytes.len() - self.data_len;
            self.bytes.resize(WINDOW_BYTES, 0);
            self.bytes
                .copy_within(self.data_len..self.data_len + code_len, REGION_BYTES);
            self.bytes[self.data_len..REGION_BYTES].fill(0);
            self.bytes[REGION_BYTES + code_len..].fill(0);
        } else {
            let mut dense = vec![0; WINDOW_BYTES];
            self.restore(&mut dense);
            self.bytes = dense;
        }
        self.data_len = REGION_BYTES;
        self.data_spans = RegionSpans::full(REGION_BYTES);
        self.code_spans = RegionSpans::full(CODE_BYTES);
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
            let mut packed = 0;
            for &[start, end] in self.data_spans.as_slice() {
                if (start..end).contains(&offset) {
                    return self.bytes[packed + offset - start];
                }
                packed += end - start;
            }
            0
        } else {
            let offset = offset - REGION_BYTES;
            let mut packed = self.data_len;
            for &[start, end] in self.code_spans.as_slice() {
                if (start..end).contains(&offset) {
                    return self.bytes[packed + offset - start];
                }
                packed += end - start;
            }
            0
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
        assert!(
            live[base..base + WINDOW_BYTES]
                .iter()
                .all(|&byte| byte == 0)
        );
        // get_mut goes through DerefMut, conservatively covering arbitrary
        // host changes through a Vec or mutable slice.
        *live.get_mut(base + WINDOW_BYTES - 1).unwrap() = 99;
        assert_eq!(live.local_bounds(), REGION_LIMITS);
        let bounds = live.local_bounds();
        saved.exchange_bounded(&mut live[base..base + WINDOW_BYTES], &mut scratch, bounds);
        assert_eq!(saved.byte(WINDOW_BYTES - 1), 99);
        assert_eq!(live[base + 5], 7);
        assert_eq!(live[base + WINDOW_BYTES - 1], 0);
    }

    #[test]
    fn sparse_low_and_top_ranges_round_trip_through_handover() {
        let mut live = VmMemory::from_zeroed(vec![0; super::super::INITIAL_MEMORY_SIZE]);
        let base = super::super::LOCAL_MEMORY_BASE as usize;
        live[base + 4..base + 12].copy_from_slice(b"rootdata");
        live[base + REGION_BYTES - 64] = 0x11;
        live[base + REGION_BYTES + 8..base + REGION_BYTES + 16].copy_from_slice(b"rootcode");
        live[base + WINDOW_BYTES - 32] = 0x22;
        let ranges = live.local_spans();
        assert_eq!(
            ranges[0].as_slice(),
            &[[4, 12], [REGION_BYTES - 64, REGION_BYTES - 63]]
        );
        assert_eq!(
            ranges[1].as_slice(),
            &[[8, 16], [CODE_BYTES - 32, CODE_BYTES - 31]]
        );

        let mut idle = LocalWindow::default();
        let mut scratch = Vec::new();
        let incoming = idle.spans();
        idle.exchange_spanned(&mut live[base..base + WINDOW_BYTES], &mut scratch, ranges);
        live.set_local_spans(incoming);
        assert!(idle.bytes.len() < 64);
        assert!(
            live[base..base + WINDOW_BYTES]
                .iter()
                .all(|&byte| byte == 0)
        );

        let incoming = idle.spans();
        let empty_ranges = live.local_spans();
        idle.exchange_spanned(
            &mut live[base..base + WINDOW_BYTES],
            &mut scratch,
            empty_ranges,
        );
        live.set_local_spans(incoming);
        assert_eq!(&live[base + 4..base + 12], b"rootdata");
        assert_eq!(live[base + REGION_BYTES - 64], 0x11);
        assert_eq!(
            &live[base + REGION_BYTES + 8..base + REGION_BYTES + 16],
            b"rootcode"
        );
        assert_eq!(live[base + WINDOW_BYTES - 32], 0x22);
    }
    #[test]
    fn word_scan_matches_byte_scan_for_unaligned_slices_and_partial_blocks() {
        let mut bytes = vec![0; 300];
        for offset in 0..8 {
            for len in 0..256 {
                assert_eq!(nonzero_prefix_words(&bytes[offset..offset + len]), 0);
                for index in 0..len {
                    bytes[offset + index] = 128;
                    assert_eq!(
                        nonzero_prefix_words(&bytes[offset..offset + len]),
                        index + 1
                    );
                    bytes[offset + index] = 0;
                }
            }
        }
    }

    #[test]
    fn interval_insert_merges_adjacent_overlapping_and_contained_ranges() {
        let mut spans = RegionSpans::default();
        spans.insert(10, 20, REGION_BYTES);
        spans.insert(0, 10, REGION_BYTES);
        spans.insert(15, 30, REGION_BYTES);
        spans.insert(12, 18, REGION_BYTES);
        assert_eq!(spans.as_slice(), &[[0, 30]]);
    }

    #[test]
    fn span_overflow_falls_back_to_a_safe_prefix() {
        let mut spans = RegionSpans::default();
        for index in 0..=MAX_REGION_SPANS {
            spans.insert(index * 2, index * 2 + 1, REGION_BYTES);
        }
        assert_eq!(spans.as_slice(), &[[0, MAX_REGION_SPANS * 2 + 1]]);
    }
    #[test]
    fn idle_window_keeps_both_regions_and_cleared_tails() {
        let mut original = vec![0; WINDOW_BYTES];
        original[0] = 1;
        original[4096] = 2;
        original[REGION_BYTES + 33] = 3;
        let mut window = LocalWindow::from_dense(original.clone());
        assert_eq!(window.bytes.len(), 4097 + 34);
        assert_eq!(&window.bytes[..4097], &original[..4097]);
        assert_eq!(
            &window.bytes[4097..],
            &original[REGION_BYTES..REGION_BYTES + 34]
        );
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
        assert_eq!(compact.bytes.len(), 2);
        assert_eq!(
            compact.spans()[0].as_slice(),
            &[[REGION_BYTES - 1, REGION_BYTES]]
        );
        assert_eq!(
            compact.spans()[1].as_slice(),
            &[[CODE_BYTES - 1, CODE_BYTES]]
        );
        let mut copied = vec![0; WINDOW_BYTES];
        compact.restore(&mut copied);
        assert_eq!(copied, restored);
    }
}
