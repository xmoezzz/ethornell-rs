use crate::{FRAME_SEGMENT, Vm};
use std::{
    collections::HashMap,
    sync::{
        OnceLock,
        atomic::{AtomicBool, AtomicUsize, Ordering},
    },
};

static INSTRUCTION_PROFILE_REPORTED: AtomicBool = AtomicBool::new(false);
static INSTRUCTION_PROFILE_MATCHES: AtomicUsize = AtomicUsize::new(0);

pub(crate) struct InstructionProfile {
    enabled: bool,
    target_program: Option<String>,
    min_steps: usize,
    skip_matches: usize,
    counts: HashMap<(usize, usize), usize>,
}

impl InstructionProfile {
    pub(crate) fn from_env(trace_id: u64) -> Self {
        let enabled = std::env::var_os("PROFILE_VM").is_some()
            && std::env::var("PROFILE_VM_ID")
                .ok()
                .and_then(|value| value.parse::<u64>().ok())
                .is_none_or(|filter| filter == trace_id)
            && !INSTRUCTION_PROFILE_REPORTED.load(Ordering::Relaxed);
        Self {
            enabled,
            target_program: std::env::var("PROFILE_VM_PROGRAM").ok(),
            min_steps: std::env::var("PROFILE_VM_MIN_STEPS")
                .ok()
                .and_then(|value| value.parse().ok())
                .unwrap_or_default(),
            skip_matches: std::env::var("PROFILE_VM_SKIP_MATCHES")
                .ok()
                .and_then(|value| value.parse().ok())
                .unwrap_or_default(),
            counts: HashMap::new(),
        }
    }

    pub(crate) fn is_enabled(&self) -> bool {
        self.enabled
    }

    pub(crate) fn record(&mut self, vm: &Vm, program_index: usize, pc: usize) {
        if !self.enabled {
            return;
        }
        let program = vm.program_name(program_index);
        let base_program = program.split("#instance=").next().unwrap_or(program);
        if self
            .target_program
            .as_deref()
            .is_some_and(|target| !base_program.ends_with(target))
        {
            return;
        }
        *self.counts.entry((program_index, pc)).or_default() += 1;
    }

    pub(crate) fn report(self, vm: &Vm, slice_steps: usize) {
        if !self.enabled || self.counts.is_empty() || slice_steps < self.min_steps {
            return;
        }
        if INSTRUCTION_PROFILE_MATCHES.fetch_add(1, Ordering::Relaxed) < self.skip_matches {
            return;
        }
        if INSTRUCTION_PROFILE_REPORTED.swap(true, Ordering::Relaxed) {
            return;
        }
        let mut counts = self.counts.into_iter().collect::<Vec<_>>();
        counts.sort_unstable_by(|left, right| right.1.cmp(&left.1));
        for ((program_index, pc), count) in counts.into_iter().take(40) {
            let Some(instruction) = vm
                .programs
                .get(program_index)
                .and_then(|program| program.instructions.get(pc))
            else {
                continue;
            };
            tracing::info!(
                vm = vm.trace_id,
                program = vm.program_name(program_index),
                pc,
                offset = format_args!("0x{:08X}", instruction.offset),
                opcode = instruction.opcode.name(),
                count,
                slice_steps,
                "VM instruction profile"
            );
        }
    }
}

pub(crate) fn debug_enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("DEBUG").is_some())
}

pub(crate) fn trace_msgwnd_enabled() -> bool {
    static ENABLED: OnceLock<bool> = OnceLock::new();
    *ENABLED.get_or_init(|| std::env::var_os("TRACE_MSGWND_LOOP").is_some())
}

impl Vm {
    pub(crate) fn trace_mtn_body_dispatch(&self, offset: u64) {
        if !debug_enabled()
            || !matches!(
                offset,
                0x2B8 | 0x2C7 | 0x2D6 | 0x2EF | 0x310 | 0x31A | 0x32B
            )
            || !self
                .program_name(self.current_program)
                .split("#instance=")
                .next()
                .is_some_and(|name| name.ends_with("mtnmngr._bp"))
        {
            return;
        }

        let body_slot_addr = FRAME_SEGMENT | self.mem_ptr.saturating_sub(0x90);
        let body_record_addr = FRAME_SEGMENT | self.mem_ptr.saturating_sub(0x50);
        let body_slot = self.read_int(body_slot_addr, 2).unwrap_or_default();
        if !matches!(body_slot, 12 | 13) {
            return;
        }
        let body_record = self.read_int(body_record_addr, 2).unwrap_or_default();

        tracing::info!(
            pc = self.pc,
            offset = format_args!("0x{offset:08X}"),
            body_slot,
            body_record = format_args!("0x{body_record:08X}"),
            record_words = ?self.debug_words(body_record, 6),
            stack = ?self.stack_summary(8),
            "MtnBodyDispatchTrace"
        );
    }

    pub(crate) fn trace_msgwnd_loop(&self, offset: u32) {
        if !trace_msgwnd_enabled() {
            return;
        }
        if !matches!(offset, 0x1072 | 0x1078 | 0x1085 | 0x1095) {
            return;
        }
        let Some(program) = self
            .programs
            .get(self.current_program)
            .and_then(|program| program.script_name.as_deref())
        else {
            return;
        };
        if !program
            .split("#instance=")
            .next()
            .is_some_and(|name| name.ends_with("msgwndctrl._bp"))
        {
            return;
        }

        let counter_addr = FRAME_SEGMENT | self.mem_ptr.saturating_sub(532);
        let table_ptr_addr = FRAME_SEGMENT | self.mem_ptr.saturating_sub(2024);
        let local_record = FRAME_SEGMENT | self.mem_ptr.saturating_sub(2016);
        let counter = self.read_int(counter_addr, 2).unwrap_or_default();
        let table_ptr = self.read_int(table_ptr_addr, 2).unwrap_or_default();
        let table_count = self
            .read_int(table_ptr.saturating_add(16), 2)
            .unwrap_or_default();
        let record_addr = table_ptr
            .saturating_add(20)
            .saturating_add(counter.saturating_mul(228));
        let record_words = self.debug_words(record_addr, 10);
        let local_words = self.debug_words(local_record, 10);

        tracing::warn!(
            pc = self.pc,
            offset = format_args!("0x{offset:08X}"),
            mem_ptr = format_args!("0x{:08X}", self.mem_ptr),
            counter_addr = format_args!("0x{counter_addr:08X}"),
            counter,
            table_ptr_addr = format_args!("0x{table_ptr_addr:08X}"),
            table_ptr = format_args!("0x{table_ptr:08X}"),
            table_count,
            record_addr = format_args!("0x{record_addr:08X}"),
            local_record = format_args!("0x{local_record:08X}"),
            ?record_words,
            ?local_words,
            "MsgWndLoopTrace"
        );
    }

    fn debug_words(&self, addr: u32, count: usize) -> Vec<String> {
        (0..count)
            .map(|index| {
                let ptr = addr.saturating_add((index * 4) as u32);
                match self.read_int(ptr, 2) {
                    Ok(value) => format!("+{:02X}=0x{value:08X}", index * 4),
                    Err(_) => format!("+{:02X}=<bad>", index * 4),
                }
            })
            .collect()
    }
}
