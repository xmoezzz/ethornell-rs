//! Line-by-line port of the target `CThread` (`src/engine/cthread.c`, itself a
//! cleaned copy of the Hex-Rays output for 0x444000..0x445500).
//!
//! Unlike [`crate::native_thread::CThread`], which records audit handles next
//! to a `Vec<Value>` operand stack, this type owns the real byte regions and
//! the real ring buffer, so every observable quirk of the original (wrapping
//! pops, the reservation arithmetic, error codes) is reproduced. It is not yet
//! driven by the interpreter; `docs`-level notes in `src/README.md` describe
//! the integration plan.

use std::collections::VecDeque;

/// Returned by [`TargetThread::reserve`] when the code region cannot hold the
/// block (`sub_445340`, `-2147483646`).
pub const ERR_CODE_REGION_FULL: u32 = 0x8000_0002;
/// Returned when the data region cannot hold the block (`-2147483645`).
pub const ERR_DATA_REGION_FULL: u32 = 0x8000_0003;
/// `sub_444D80` result when no module is loaded (`-2147483647`).
pub const NO_MODULE: u32 = 0x8000_0001;
/// Result `sub_444CE0` yields when the image does not fit (checked by the
/// handler `sub_488C00` against `0x80000000`).
pub const LOAD_FAILED: u32 = 0x8000_0000;

/// Loaded BP image record (`struct Module`, 0x10 bytes).
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Module {
    pub name: String,
    pub size: u32,
    pub offset: u32,
}

/// Region reservation record (`struct Reservation`).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Reservation {
    pub tag: u32,
    pub start: u32,
    pub size: u32,
}

#[derive(Debug, Clone)]
pub struct TargetThread {
    pub thread_id: u32,
    operand_slots: u32,
    operand: Vec<u32>,
    operand_sp: u32,
    code_size: u32,
    code_reserved_low: u32,
    code_free_limit: u32,
    code: Vec<u8>,
    /// Newest first, like the target's singly linked list.
    modules: Vec<Module>,
    code_used: u32,
    data_size: u32,
    data_reserved_low: u32,
    data_free_limit: u32,
    data: Vec<u8>,
    frames: Vec<u32>,
    callbacks: VecDeque<u32>,
    reservation_count: u32,
    code_reservations: Vec<Reservation>,
    data_reservations: Vec<Reservation>,
    pub flags: u32,
    insn_start: u32,
    ip: u32,
    data_sp: u32,
    deadline: u32,
}

impl TargetThread {
    /// `sub_4447C0`. The caller supplies the id the global counter handed out.
    pub fn new(thread_id: u32, slots: u32, code_bytes: u32, data_bytes: u32) -> Self {
        Self {
            thread_id,
            operand_slots: slots,
            operand: vec![0; slots as usize],
            operand_sp: 0,
            code_size: code_bytes,
            code_reserved_low: 0,
            code_free_limit: code_bytes,
            code: vec![0; code_bytes as usize],
            modules: Vec::new(),
            code_used: 0,
            data_size: data_bytes,
            data_reserved_low: 0,
            data_free_limit: data_bytes,
            data: vec![0; data_bytes as usize],
            frames: Vec::new(),
            callbacks: VecDeque::new(),
            reservation_count: 0,
            code_reservations: Vec::new(),
            data_reservations: Vec::new(),
            flags: 0,
            insn_start: 0,
            ip: 0,
            data_sp: 0,
            deadline: 0,
        }
    }

    // ---- operand ring (sub_4450B0 / sub_4450D0) ---------------------------

    /// Pop one DWORD. At index 0 the ring wraps to the last slot, so an
    /// underflow returns stale data instead of failing. A zero-slot stack
    /// would fault in the original; here it reads as 0.
    pub fn pop(&mut self) -> u32 {
        if self.operand_slots == 0 {
            return 0;
        }
        let index = if self.operand_sp == 0 {
            self.operand_slots
        } else {
            self.operand_sp
        } - 1;
        self.operand_sp = index;
        self.operand[index as usize]
    }

    pub fn push(&mut self, value: u32) {
        if self.operand_slots == 0 {
            return;
        }
        self.operand[self.operand_sp as usize] = value;
        self.operand_sp = if self.operand_sp + 1 < self.operand_slots {
            self.operand_sp + 1
        } else {
            0
        };
    }

    pub fn operand_sp(&self) -> u32 {
        self.operand_sp
    }

    // ---- data-region frame stack (sub_445110 / sub_4450F0) ---------------

    pub fn data_push(&mut self, value: u32) {
        let at = self.data_sp as usize;
        if let Some(slot) = self.data.get_mut(at..at + 4) {
            slot.copy_from_slice(&value.to_le_bytes());
        }
        self.data_sp = self.data_sp.wrapping_add(4);
    }

    pub fn data_pop(&mut self) -> u32 {
        self.data_sp = self.data_sp.wrapping_sub(4);
        let at = self.data_sp as usize;
        self.data
            .get(at..at + 4)
            .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()))
            .unwrap_or(0)
    }

    pub fn data_sp(&self) -> u32 {
        self.data_sp
    }

    // ---- saved-ip list (sub_445130 / sub_445150 / sub_445190) -------------

    /// Pushes `insn_start` (`this+0x78`), the address of the opcode being run.
    pub fn frame_push(&mut self) {
        self.frames.push(self.insn_start);
    }

    pub fn frame_pop_discard(&mut self) -> bool {
        self.frames.pop().is_some()
    }

    /// Newest first, like the linked list walk.
    pub fn frame_copy(&self) -> Vec<u32> {
        self.frames.iter().rev().copied().collect()
    }

    // ---- instruction fetch (sub_445010..sub_445070) ----------------------

    pub fn fetch_opcode(&mut self) -> u8 {
        self.insn_start = self.ip;
        self.ip = self.ip.wrapping_add(1);
        self.code_byte(self.insn_start)
    }

    pub fn fetch_u8(&mut self) -> u8 {
        let value = self.code_byte(self.ip);
        self.ip = self.ip.wrapping_add(1);
        value
    }

    pub fn fetch_u16(&mut self) -> u16 {
        let value = u16::from_le_bytes([self.code_byte(self.ip), self.code_byte(self.ip + 1)]);
        self.ip = self.ip.wrapping_add(2);
        value
    }

    pub fn fetch_u32(&mut self) -> u32 {
        let mut bytes = [0u8; 4];
        for (i, b) in bytes.iter_mut().enumerate() {
            *b = self.code_byte(self.ip + i as u32);
        }
        self.ip = self.ip.wrapping_add(4);
        u32::from_le_bytes(bytes)
    }

    /// `sub_445070`: refuses to read past the code limit.
    pub fn fetch_bytes(&mut self, count: u32) -> Option<Vec<u8>> {
        if self.ip.checked_add(count)? > self.code_limit() {
            return None;
        }
        let out = (0..count).map(|i| self.code_byte(self.ip + i)).collect();
        self.ip += count;
        Some(out)
    }

    /// `sub_444FE0`.
    pub fn jump(&mut self, target: u32) {
        self.insn_start = target;
        self.ip = target;
    }

    pub fn ip(&self) -> u32 {
        self.ip
    }

    pub fn insn_start(&self) -> u32 {
        self.insn_start
    }

    fn code_byte(&self, at: u32) -> u8 {
        self.code.get(at as usize).copied().unwrap_or(0)
    }

    // ---- limits ----------------------------------------------------------

    /// `sub_444C30`.
    pub fn code_limit(&self) -> u32 {
        self.code_reserved_low.wrapping_add(self.code_free_limit)
    }

    /// `sub_444C40`.
    pub fn data_limit(&self) -> u32 {
        self.data_reserved_low.wrapping_add(self.data_free_limit)
    }

    // ---- modules (sub_444CE0 / sub_444D80) -------------------------------

    /// Append a decoded BP image; returns its offset or [`LOAD_FAILED`].
    pub fn append_module(&mut self, name: &str, image: &[u8]) -> u32 {
        let size = image.len() as u32;
        if size.wrapping_add(self.code_used) > self.code_limit() {
            return LOAD_FAILED;
        }
        let offset = self.code_used;
        let at = offset as usize;
        if let Some(dst) = self.code.get_mut(at..at + image.len()) {
            dst.copy_from_slice(image);
        }
        self.modules.insert(
            0,
            Module {
                name: name.to_string(),
                size,
                offset,
            },
        );
        self.code_used += size;
        offset
    }

    /// Remaining module count, or [`NO_MODULE`].
    pub fn free_last_module(&mut self) -> u32 {
        if self.modules.is_empty() {
            return NO_MODULE;
        }
        let module = self.modules.remove(0);
        self.code_used -= module.size;
        self.modules.len() as u32
    }

    pub fn modules(&self) -> &[Module] {
        &self.modules
    }

    pub fn code_used(&self) -> u32 {
        self.code_used
    }

    // ---- reservations (sub_445340 / sub_445480) --------------------------

    /// Carve `code_bytes`/`data_bytes` from the top of both regions for `tag`.
    /// Returns the two block offsets, or the target's error code.
    pub fn reserve(
        &mut self,
        tag: u32,
        code_bytes: u32,
        data_bytes: u32,
    ) -> Result<(u32, u32), u32> {
        let (code_at, code_top) =
            Self::find_gap(&self.code_reservations, self.code_size, code_bytes);
        let (data_at, data_top) =
            Self::find_gap(&self.data_reservations, self.data_size, data_bytes);
        let code_start = code_top.wrapping_sub(code_bytes) as i32;
        if code_start < self.code_used as i32 {
            return Err(ERR_CODE_REGION_FULL);
        }
        if data_top < data_bytes {
            return Err(ERR_DATA_REGION_FULL);
        }
        self.reservation_count += 1;
        let code_start = code_start as u32;
        self.code_reservations.insert(
            code_at,
            Reservation {
                tag,
                start: code_start,
                size: code_bytes,
            },
        );
        self.code_free_limit = self
            .code_reservations
            .iter()
            .map(|r| r.start)
            .fold(self.code_size, u32::min);
        let data_start = data_top - data_bytes;
        self.data_reservations.insert(
            data_at,
            Reservation {
                tag,
                start: data_start,
                size: data_bytes,
            },
        );
        self.data_free_limit = self
            .data_reservations
            .iter()
            .map(|r| r.start)
            .fold(self.data_size, u32::min);
        Ok((code_start, data_start))
    }

    /// Walk the descending list like the decompiled loop: stop at the first
    /// node that leaves room for `bytes` below `top`. Returns the insertion
    /// index and the top the block is carved from.
    fn find_gap(list: &[Reservation], region_size: u32, bytes: u32) -> (usize, u32) {
        let mut top = region_size;
        for (index, node) in list.iter().enumerate() {
            if bytes.wrapping_add(node.start).wrapping_add(node.size) <= top {
                return (index, top);
            }
            top = node.start;
        }
        (list.len(), top)
    }

    /// `sub_445480`: release one reservation per list for `tag`.
    pub fn release(&mut self, tag: u32) -> bool {
        let code_found = match self.code_reservations.iter().position(|r| r.tag == tag) {
            Some(index) => {
                self.code_reservations.remove(index);
                true
            }
            None => false,
        };
        let data_found = match self.data_reservations.iter().position(|r| r.tag == tag) {
            Some(index) => {
                self.data_reservations.remove(index);
                true
            }
            None => false,
        };
        if data_found || code_found {
            self.reservation_count = self.reservation_count.saturating_sub(1);
        }
        data_found || code_found
    }

    pub fn reservation_count(&self) -> u32 {
        self.reservation_count
    }

    // ---- timing (sub_445260 / sub_445290 / sub_4452B0) -------------------

    pub fn wait_remaining(&self, now: u32) -> i32 {
        let remaining = self.deadline.wrapping_sub(now) as i32;
        if remaining <= 0 { 0 } else { remaining }
    }

    pub fn set_deadline(&mut self, now: u32, ms: i32) {
        self.deadline = (ms as u32).wrapping_add(now);
    }

    pub fn extend_deadline(&mut self, ms: i32) {
        self.deadline = self.deadline.wrapping_add(ms as u32);
    }

    // ---- callbacks (sub_4452C0 / sub_445300) -----------------------------

    pub fn callback_enqueue(&mut self, value: u32) {
        self.callbacks.push_back(value);
    }

    pub fn callback_dequeue(&mut self) -> Option<u32> {
        self.callbacks.pop_front()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn thread() -> TargetThread {
        TargetThread::new(7, 4, 64, 64)
    }

    #[test]
    fn operand_pop_below_zero_wraps_to_the_last_slot() {
        let mut t = thread();
        for v in [10, 20, 30, 40] {
            t.push(v);
        }
        // Four pushes fill the ring and wrap the index back to 0.
        assert_eq!(t.operand_sp(), 0);
        assert_eq!(t.pop(), 40);
        assert_eq!(t.pop(), 30);
        t.pop();
        t.pop();
        // Underflow reads the stale last slot rather than failing.
        assert_eq!(t.pop(), 40);
    }

    #[test]
    fn push_past_capacity_overwrites_the_oldest_value() {
        let mut t = thread();
        for v in 1..=5 {
            t.push(v);
        }
        assert_eq!(t.operand_sp(), 1);
        assert_eq!(t.pop(), 5);
        assert_eq!(t.pop(), 4);
        assert_eq!(t.pop(), 3);
        assert_eq!(t.pop(), 2);
        assert_eq!(t.pop(), 5);
    }

    #[test]
    fn fetch_tracks_the_opcode_start_separately_from_the_cursor() {
        let mut t = thread();
        t.append_module("m", &[0x16, 0x34, 0x12, 0xaa, 0xbb, 0xcc, 0xdd]);
        assert_eq!(t.fetch_opcode(), 0x16);
        assert_eq!(t.fetch_u16(), 0x1234);
        assert_eq!((t.insn_start(), t.ip()), (0, 3));
        assert_eq!(t.fetch_u32(), 0xddcc_bbaa);
        t.frame_push();
        assert_eq!(t.frame_copy(), vec![0]);
        t.jump(5);
        assert_eq!((t.insn_start(), t.ip()), (5, 5));
    }

    #[test]
    fn fetch_bytes_stops_at_the_code_limit() {
        let mut t = thread();
        t.jump(60);
        assert!(t.fetch_bytes(4).is_some());
        assert_eq!(t.fetch_bytes(1), None);
    }

    #[test]
    fn modules_stack_and_roll_back_code_used() {
        let mut t = thread();
        assert_eq!(t.free_last_module(), NO_MODULE);
        assert_eq!(t.append_module("a", &[1; 10]), 0);
        assert_eq!(t.append_module("b", &[2; 6]), 10);
        assert_eq!(t.code_used(), 16);
        assert_eq!(t.modules()[0].name, "b");
        assert_eq!(t.free_last_module(), 1);
        assert_eq!(t.code_used(), 10);
        assert_eq!(t.append_module("big", &[0; 55]), LOAD_FAILED);
        assert_eq!(t.free_last_module(), 0);
    }

    #[test]
    fn reservations_grow_down_from_the_region_tops() {
        let mut t = thread();
        assert_eq!(t.reserve(1, 16, 8), Ok((48, 56)));
        assert_eq!(t.code_limit(), 48);
        assert_eq!(t.data_limit(), 56);
        assert_eq!(t.reserve(2, 8, 8), Ok((40, 48)));
        assert_eq!(t.reservation_count(), 2);
        // A module can no longer grow into the reserved top.
        assert_eq!(t.append_module("x", &[0; 41]), LOAD_FAILED);
        assert!(t.release(2));
        assert!(!t.release(2));
        assert_eq!(t.reservation_count(), 1);
    }

    #[test]
    fn reservation_errors_use_the_target_codes() {
        let mut t = thread();
        t.append_module("m", &[0; 40]);
        // 64 - 30 = 34 < code_used 40
        assert_eq!(t.reserve(1, 30, 1), Err(ERR_CODE_REGION_FULL));
        assert_eq!(t.reserve(1, 4, 65), Err(ERR_DATA_REGION_FULL));
        assert_eq!(t.reservation_count(), 0);
    }

    #[test]
    fn data_stack_and_deadline_helpers() {
        let mut t = thread();
        t.data_push(0xdead_beef);
        t.data_push(5);
        assert_eq!(t.data_sp(), 8);
        assert_eq!(t.data_pop(), 5);
        assert_eq!(t.data_pop(), 0xdead_beef);
        t.set_deadline(1000, 50);
        assert_eq!(t.wait_remaining(1020), 30);
        t.extend_deadline(10);
        assert_eq!(t.wait_remaining(1020), 40);
        assert_eq!(t.wait_remaining(2000), 0);
    }

    #[test]
    fn callbacks_are_a_fifo() {
        let mut t = thread();
        t.callback_enqueue(1);
        t.callback_enqueue(2);
        assert_eq!(t.callback_dequeue(), Some(1));
        assert_eq!(t.callback_dequeue(), Some(2));
        assert_eq!(t.callback_dequeue(), None);
    }
}
