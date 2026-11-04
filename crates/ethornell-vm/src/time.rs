/// Engine clock shared by the portable runtime.
///
/// Wait deadlines are not stored here.  The recovered target stores the
/// absolute deadline in `CThread+0x84`, so the portable mirror keeps it in
/// `native_thread::CThread::deadline_tick` as well.
#[derive(Debug, Clone, Default)]
pub(crate) struct VmTime {
    /// sub_499290 value when the runtime started (the host's millisecond
    /// uptime for interactive runs, 0 for deterministic replays).
    base_ms: u32,
    advanced_ms: u64,
}

impl VmTime {
    pub(crate) fn reset(&mut self) {
        self.advanced_ms = 0;
    }

    /// sub_499290: a wrapping 32-bit millisecond clock.
    pub(crate) fn tick_count(&self) -> i32 {
        self.base_ms.wrapping_add(self.advanced_ms as u32) as i32
    }

    pub(crate) fn set_base(&mut self, base_ms: u32) {
        self.base_ms = base_ms;
    }

    pub(crate) fn performance_counter(&self) -> i64 {
        ((u64::from(self.base_ms) + self.advanced_ms) as u128 * 1_000_000).min(i64::MAX as u128)
            as i64
    }

    pub(crate) fn advance(&mut self, milliseconds: u64) {
        self.advanced_ms = self.advanced_ms.saturating_add(milliseconds);
    }
}
