//! DCTChildThread (Sys81:44, sub_48CA50): a thread that runs the parent's
//! code with its own operand ring and a block of the parent's regions.
//!
//! The child gets no code or data region of its own (sub_444BA0 with zero
//! sizes). sub_453040 reserves `code_bytes` / `data_bytes` at the top of the
//! parent's regions (CThread vtable +28, sub_445710) and points the child at
//! the parent's buffers: code base = the block's start, code limit = start +
//! `code_bytes`, frame stack from the data block's start. Segment 1 and 2
//! addresses are therefore the parent's, and the child's `ret` ends the thread
//! when its frame stack is back at the block's start (sub_474020). A child
//! forwards reservations to the thread it was made from (vtable +24 gives the
//! non-child ancestor), and its destructor (sub_452FD0) releases its blocks
//! (sub_445850), which leaves the ancestor's free limits where they were.
//!
//! In the port the child is a separate `Vm` that clones the parent's module
//! table and borrows the owner's local window (`LOCAL_MEMORY_BASE..
//! HEAP_MEMORY_BASE`) while it runs (`lend_local_window`).

use crate::{Value, Vm, VmError, VmResult};
use std::sync::Arc;

/// One block of sub_445710's lists: (owner tag, start, size).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct Reservation {
    tag: i32,
    start: u32,
    size: u32,
}

/// A thread's reservation state (CThread +0x1C/+0x24/+0x38, +0x3C/+0x44,
/// +0x64 count, +0x68 code list, +0x6C data list).
#[derive(Debug, Default)]
pub(crate) struct RegionReservations {
    code_size: u32,
    data_size: u32,
    code_used_end: u32,
    /// +0x24 / +0x44 once a block was reserved (the size before).
    code_free_limit: Option<u32>,
    data_free_limit: Option<u32>,
    code: Vec<Reservation>,
    data: Vec<Reservation>,
    count: i32,
}

/// sub_445710's error codes.
pub(crate) const RESERVE_CODE_FULL: i32 = 0x8000_0002_u32 as i32;
pub(crate) const RESERVE_DATA_FULL: i32 = 0x8000_0003_u32 as i32;

impl RegionReservations {
    pub(crate) fn set_sizes(&mut self, code_size: u32, data_size: u32) {
        self.code_size = code_size;
        self.data_size = data_size;
    }

    pub(crate) fn set_code_used_end(&mut self, code_used_end: u32) {
        self.code_used_end = code_used_end;
    }

    /// +0x44: the data limit of a non-child thread.
    pub(crate) fn data_free_limit(&self) -> u32 {
        self.data_free_limit.unwrap_or(self.data_size)
    }

    /// Where a block of `size` goes: below the first block (in list order)
    /// that leaves room under the running top. Returns (top, insert index).
    fn place(list: &[Reservation], mut top: u32, size: u32) -> (u32, usize) {
        for (index, block) in list.iter().enumerate() {
            if size.wrapping_add(block.start).wrapping_add(block.size) <= top {
                return (top, index);
            }
            top = block.start;
        }
        (top, list.len())
    }

    /// sub_445710: (code start, data start), or RESERVE_CODE_FULL when the
    /// code block would go below the loaded modules (signed compare),
    /// RESERVE_DATA_FULL when the data block does not fit.
    pub(crate) fn reserve(&mut self, tag: i32, code_bytes: u32, data_bytes: u32) -> Result<(u32, u32), i32> {
        let (code_top, code_index) = Self::place(&self.code, self.code_size, code_bytes);
        let (data_top, data_index) = Self::place(&self.data, self.data_size, data_bytes);
        let code_start = code_top.wrapping_sub(code_bytes);
        if (code_start as i32) < self.code_used_end as i32 {
            return Err(RESERVE_CODE_FULL);
        }
        if data_top < data_bytes {
            return Err(RESERVE_DATA_FULL);
        }
        self.count += 1;
        self.code.insert(code_index, Reservation { tag, start: code_start, size: code_bytes });
        self.code_free_limit =
            Some(self.code.iter().map(|block| block.start).fold(self.code_size, u32::min));
        let data_start = data_top - data_bytes;
        self.data.insert(data_index, Reservation { tag, start: data_start, size: data_bytes });
        self.data_free_limit =
            Some(self.data.iter().map(|block| block.start).fold(self.data_size, u32::min));
        Ok((code_start, data_start))
    }

    /// sub_445850: drops the first code and data block of `tag`; the free
    /// limits stay.
    pub(crate) fn release(&mut self, tag: i32) -> bool {
        let code = self.code.iter().position(|block| block.tag == tag).map(|index| self.code.remove(index));
        let data = self.data.iter().position(|block| block.tag == tag).map(|index| self.data.remove(index));
        let found = code.is_some() || data.is_some();
        if found {
            self.count -= 1;
        }
        found
    }
}

/// Swaps the local windows of two VMs: a child runs in its owner's.
pub(crate) fn lend_local_window(owner: &mut Vm, child: &mut Vm) {
    if owner.parked_local.is_some() && child.parked_local.is_some() {
        std::mem::swap(&mut owner.parked_local, &mut child.parked_local);
    } else if let Some(window) = owner.parked_local.as_mut() {
        crate::async_program::exchange_saved_local(child, window);
    } else if let Some(window) = child.parked_local.as_mut() {
        crate::async_program::exchange_saved_local(owner, window);
    } else {
        let owner_window = crate::async_program::local_window_mut(owner);
        owner_window.swap_with_slice(crate::async_program::local_window_mut(child));
    }
}

impl Vm {
    /// The data-region limit a call checks (sub_445010: +0x40 + +0x44).
    pub(crate) fn data_region_limit(&self) -> u32 {
        if self.child_thread_tag.is_some() {
            self.thread.frame_region_base().saturating_add(self.thread.data_region_size())
        } else {
            self.region_reservations.lock().map_or(self.thread.data_region_size(), |regions| regions.data_free_limit())
        }
    }

    /// Keeps a non-child thread's sizes and loaded code end in its
    /// reservation state.
    pub(crate) fn sync_region_reservations(&self, code_used_end: u32) {
        if self.child_thread_tag.is_some() {
            return;
        }
        if let Ok(mut regions) = self.region_reservations.lock() {
            regions.set_sizes(self.thread.code_region_size(), self.thread.data_region_size());
            regions.set_code_used_end(code_used_end);
        }
    }

    /// sub_48CA50 after the pops: creates the child, reserves its blocks,
    /// checks the initial IP and returns the child's thread id.
    pub(crate) fn start_child_thread(
        &mut self,
        operand_slots: u32,
        code_bytes: u32,
        data_bytes: u32,
        initial_ip: u32,
        trace_events: bool,
    ) -> VmResult<i32> {
        // sub_444BA0 numbers the thread before sub_453040 reserves.
        let thread_id = self.next_thread_id.max(1);
        self.next_thread_id = self.next_thread_id.saturating_add(1).max(1);
        let reserved = self
            .region_reservations
            .lock()
            .map_err(|_| VmError::Runtime("thread region state poisoned".into()))?
            .reserve(thread_id, code_bytes, data_bytes);
        let (code_start, data_start) = match reserved {
            Ok(blocks) => blocks,
            Err(RESERVE_CODE_FULL) => {
                return Err(VmError::Runtime(format!(
                    "指定されたサイズ [ {} ] のコード領域を親スレッドに確保することができません",
                    code_bytes as i32
                )));
            }
            Err(_) => {
                return Err(VmError::Runtime(format!(
                    "指定されたサイズ [ {} ] のスタック領域を親スレッドに確保することができません",
                    data_bytes as i32
                )));
            }
        };
        // sub_445000: child code base + code size.
        if initial_ip >= code_start.wrapping_add(code_bytes) {
            return Err(VmError::Runtime(format!(
                "チャイルドスレッドの初期IPにコード領域のサイズを超える値 ${initial_ip:X} が設定されました"
            )));
        }
        let (program, parser_offset) = self.resolve_code_target(self.current_program, initial_ip);
        let pc = self.jump_target_index(program, parser_offset)?;

        let root = self.programs.first().cloned().unwrap_or_else(|| crate::empty_loaded_program("DCTChildThread".into()));
        let created = self.start_async_program_with_thread_id(Value::Program(Arc::new(root)), thread_id, trace_events);
        if !created {
            return Err(VmError::Runtime("child thread could not be created".into()));
        }
        let owner = self.local_window_owner.unwrap_or(self.thread.thread_id());
        let task = self.async_tasks.last_mut().expect("child task just pushed");
        let child = &mut task.vm;
        child.programs = self.programs.clone();
        child.program_code_bases = self.program_code_bases.clone();
        child.program_active = vec![false; self.programs.len()];
        child.target_loaded_programs.clear();
        child.program_cache = self.program_cache.clone();
        child.current_program = program;
        child.pc = pc;
        child.mem_ptr = data_start;
        child.code_used_floor = code_start;
        child.child_thread_tag = Some(thread_id);
        child.local_window_owner = Some(owner);
        child.region_reservations = Arc::clone(&self.region_reservations);
        child.thread.set_region_sizes(code_bytes, data_bytes, operand_slots);
        child.thread.set_child_regions(code_start, data_start);
        child.sync_thread_program_region();
        // This thread executes in its owner's local window. The temporary
        // buffer used to initialize the root module will never be read.
        child.memory = Vec::new().into();
        child.parked_local = None;
        if trace_events {
            tracing::info!(
                thread_id,
                owner,
                code_start = format_args!("0x{code_start:X}"),
                data_start = format_args!("0x{data_start:X}"),
                initial_ip = format_args!("0x{initial_ip:X}"),
                "VM child thread created"
            );
        }
        Ok(thread_id)
    }

    /// sub_452FD0: a finished child releases its blocks in the ancestor.
    pub(crate) fn release_child_thread_regions(&mut self) {
        if let Some(tag) = self.child_thread_tag.take()
            && let Ok(mut regions) = self.region_reservations.lock()
        {
            regions.release(tag);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reservations_follow_sub_445710() {
        let mut regions = RegionReservations::default();
        regions.set_sizes(0x80000, 0x40000);
        regions.set_code_used_end(0x1000);
        assert_eq!(regions.reserve(7, 0x1000, 0x800), Ok((0x7F000, 0x3F800)));
        assert_eq!(regions.reserve(8, 0x2000, 0x400), Ok((0x7D000, 0x3F400)));
        assert_eq!(regions.data_free_limit(), 0x3F400);
        // The code block may not go below the loaded modules.
        assert_eq!(regions.reserve(9, 0x7D000, 0), Err(RESERVE_CODE_FULL));
        assert_eq!(regions.reserve(9, 0, 0x3F401), Err(RESERVE_DATA_FULL));
        // Releasing keeps the limits; the hole at the top is reused first.
        assert!(regions.release(7));
        assert_eq!(regions.data_free_limit(), 0x3F400);
        assert_eq!(regions.reserve(10, 0x1000, 0x800), Ok((0x7F000, 0x3F800)));
        assert!(!regions.release(7));
    }
}
