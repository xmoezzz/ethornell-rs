//! BP 74/75 (EFB00/EFA70): optional write diagnostics, not a separate memory
//! layout. Native EEC50 warns about overlap and then permits the write.
use crate::{SysApi, Vm, VmError, VmResult};
#[derive(Debug)]
struct Region {
    owner: i32,
    start: u32,
    size: u32,
    name: String,
}
#[derive(Debug, Default)]
pub(crate) struct NativeMemoryGuard {
    pub(crate) enabled: bool,
    regions: Vec<Region>,
}
impl Vm {
    pub(super) fn register_native_memory_guard(&mut self) -> VmResult<i32> {
        let name = self.pop_string_lossy()?;
        let size = self.pop_int()? as u32;
        let start = self.pop_int()? as u32;
        if size == 0 {
            return Ok(0);
        }
        if self.encode_buffer_text(&name).len() > 255 {
            return Err(VmError::Runtime(
                "native protected-region name exceeds its 256-byte field".into(),
            ));
        }
        let mut state = self
            .system81_shared
            .lock()
            .expect("system81 state poisoned");
        if state.memory_guard.regions.len() >= 4096 {
            return Err(VmError::Runtime(
                "native protected-region count limit".into(),
            ));
        }
        state.memory_guard.regions.push(Region {
            owner: self.thread.thread_id(),
            start,
            size,
            name,
        });
        Ok(1)
    }
    pub(super) fn check_native_guarded_write<A: SysApi>(
        &self,
        api: &mut A,
        start: u32,
        size: usize,
    ) {
        if size == 0 {
            return;
        }
        let warning = {
            let state = self
                .system81_shared
                .lock()
                .expect("system81 state poisoned");
            if !state.memory_guard.enabled {
                return;
            }
            let end = start.wrapping_add(size as u32).wrapping_sub(1);
            // Newest region first, matching the native linked-list search.
            state.memory_guard.regions.iter().rev().find(|r|r.start<=end&&start<r.start.wrapping_add(r.size)
                    && (!(1..=3).contains(&(start>>28))||r.owner==self.thread.thread_id()))
                .map(|r|format!("Write to protected region {:?} [${:08X}, {} bytes]: address ${start:08X}, {size} bytes",r.name,r.start,r.size))
        };
        if let Some(message) = warning {
            api.message_box("Protected memory", &message);
        }
    }
}
