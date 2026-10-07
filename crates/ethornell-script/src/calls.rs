use crate::{BpInstruction, BpOperand, BpProgram, candidate_call_name};
use serde::Serialize;
use std::collections::{BTreeMap, BTreeSet};

pub const DISPATCH_GROUPS: [u8; 8] = [0x80, 0x81, 0x90, 0x91, 0x92, 0xa0, 0xb0, 0xc0];

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Serialize)]
pub enum CallDomain {
    System,
    Graph,
    Sound,
    User,
}

impl CallDomain {
    pub fn from_group(group: u8) -> Option<Self> {
        match group {
            0x80 | 0x81 => Some(Self::System),
            0x90..=0x92 => Some(Self::Graph),
            0xa0 => Some(Self::Sound),
            0xb0 | 0xc0 => Some(Self::User),
            _ => None,
        }
    }

    pub fn label(self) -> &'static str {
        match self {
            Self::System => "system",
            Self::Graph => "graph",
            Self::Sound => "sound",
            Self::User => "user",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Serialize)]
pub struct CallKey {
    pub group: u8,
    pub id: u16,
}

impl CallKey {
    pub fn domain(self) -> Option<CallDomain> {
        CallDomain::from_group(self.group)
    }

    /// Diagnostic candidate label. This is not a target-recovery status.
    pub fn name(self) -> Option<&'static str> {
        candidate_call_name(self.group, self.id)
    }

    pub fn arg_count(self) -> Option<usize> {
        known_call_arg_count(self.group, self.id)
    }

    pub fn returns_value(self) -> bool {
        known_call_returns_value(self.group, self.id)
    }
}

#[derive(Debug, Clone, Serialize)]
pub struct CallSite {
    pub script: Option<String>,
    pub offset: u64,
    pub key: CallKey,
    pub name: Option<&'static str>,
    pub arg_count: Option<usize>,
}

#[derive(Debug, Clone, Serialize)]
pub struct CallSummary {
    pub key: CallKey,
    pub domain: Option<CallDomain>,
    pub name: Option<&'static str>,
    pub count: usize,
    pub arg_count: Option<usize>,
    pub inferred_arg_counts: Vec<InferredArgCount>,
    pub scripts: Vec<String>,
    pub known: bool,
    pub registered: bool,
    pub generic: bool,
    pub used: bool,
}

#[derive(Debug, Clone, Serialize)]
pub struct InferredArgCount {
    pub argc: usize,
    pub count: usize,
}

pub fn instruction_call_key(instruction: &BpInstruction) -> Option<CallKey> {
    let group = instruction.opcode.code();
    if !DISPATCH_GROUPS.contains(&group) {
        return None;
    }
    let Some(BpOperand::U8(id)) = instruction.operands.first() else {
        return None;
    };
    Some(CallKey {
        group,
        id: *id as u16,
    })
}

pub fn scan_program_calls(program: &BpProgram) -> Vec<CallSite> {
    program
        .instructions
        .iter()
        .filter_map(|instruction| {
            let key = instruction_call_key(instruction)?;
            Some(CallSite {
                script: program.script_name.clone(),
                offset: instruction.offset,
                key,
                name: key.name(),
                arg_count: key.arg_count(),
            })
        })
        .collect()
}

pub fn summarize_call_sites(sites: impl IntoIterator<Item = CallSite>) -> Vec<CallSummary> {
    let mut counts: BTreeMap<CallKey, (usize, BTreeSet<String>)> = BTreeMap::new();
    for site in sites {
        let (count, scripts) = counts.entry(site.key).or_default();
        *count += 1;
        if let Some(script) = site.script {
            scripts.insert(script);
        }
    }
    counts
        .into_iter()
        .map(|(key, (count, scripts))| {
            let name = key.name();
            CallSummary {
                key,
                domain: key.domain(),
                name,
                count,
                arg_count: key.arg_count(),
                inferred_arg_counts: Vec::new(),
                scripts: scripts.into_iter().take(12).collect(),
                known: name.is_some(),
                registered: crate::native_abi::lookup(key.group, key.id).is_some(),
                generic: is_generic_native_name(key),
                used: true,
            }
        })
        .collect()
}

pub fn registered_call_summary(key: CallKey) -> Option<CallSummary> {
    crate::native_abi::lookup(key.group, key.id)?;
    let name = key.name();
    Some(CallSummary {
        key,
        domain: key.domain(),
        name,
        count: 0,
        arg_count: key.arg_count(),
        inferred_arg_counts: Vec::new(),
        scripts: Vec::new(),
        known: name.is_some(),
        registered: true,
        generic: is_generic_native_name(key),
        used: false,
    })
}

fn is_generic_native_name(key: CallKey) -> bool {
    key.name() == crate::native_abi::generic_name(key.group, key.id)
}

pub fn infer_program_call_arg_counts(program: &BpProgram) -> Vec<CallSiteArgCount> {
    let mut state = StackEffectState::default();
    let mut observations = Vec::new();
    for instruction in &program.instructions {
        state.visit(program, instruction, &mut observations);
    }
    observations
}

pub fn summarize_inferred_arg_counts(
    observations: impl IntoIterator<Item = CallSiteArgCount>,
) -> BTreeMap<CallKey, Vec<InferredArgCount>> {
    let mut counts: BTreeMap<CallKey, BTreeMap<usize, usize>> = BTreeMap::new();
    for observation in observations {
        *counts
            .entry(observation.key)
            .or_default()
            .entry(observation.arg_count)
            .or_default() += 1;
    }
    counts
        .into_iter()
        .map(|(key, values)| {
            let mut values = values
                .into_iter()
                .map(|(argc, count)| InferredArgCount { argc, count })
                .collect::<Vec<_>>();
            values.sort_by(|left, right| {
                right
                    .count
                    .cmp(&left.count)
                    .then(left.argc.cmp(&right.argc))
            });
            (key, values)
        })
        .collect()
}

#[derive(Debug, Clone, Serialize)]
pub struct CallSiteArgCount {
    pub script: Option<String>,
    pub offset: u64,
    pub key: CallKey,
    pub arg_count: usize,
}

#[derive(Default)]
struct StackEffectState {
    depth: isize,
}

impl StackEffectState {
    fn visit(
        &mut self,
        program: &BpProgram,
        instruction: &BpInstruction,
        observations: &mut Vec<CallSiteArgCount>,
    ) {
        if let Some(key) = instruction_call_key(instruction) {
            let inferred = self.depth.max(0) as usize;
            observations.push(CallSiteArgCount {
                script: program.script_name.clone(),
                offset: instruction.offset,
                key,
                arg_count: inferred,
            });
            let argc = key.arg_count().unwrap_or(inferred);
            self.pop(argc);
            if key.returns_value() {
                self.push(1);
            } else if key.arg_count().is_none() {
                self.depth = 0;
            }
            return;
        }

        match instruction.opcode_name.as_str() {
            "push_byte" | "push_word" | "push_dword" | "push_base_offset" | "push_string"
            | "push_offset" | "load_base" => self.push(1),
            "store_base" | "jmp" | "call" => self.pop(1),
            "load" => {}
            // Native opcode 0x09 writes and then pushes the assigned value.
            "move" => self.pop(1),
            "move_arg" | "jc" => self.pop(2),
            "copy_inline" => self.pop(1),
            "copy_stack" => {
                let count = instruction
                    .operands
                    .get(1)
                    .and_then(operand_as_u32)
                    .unwrap_or_default() as usize;
                self.pop(count + 1);
            }
            "ret" | "script_ret" => self.depth = 0,
            "add" | "sub" | "mul" | "div" | "mod" | "and" | "or" | "xor" | "shl" | "shr"
            | "sar" | "eq" | "neq" | "leq" | "geq" | "lt" | "gt" | "boolean_and" | "boolean_or" => {
                self.pop(1)
            }
            "not" | "bool_zero" | "sin" | "cos" => {}
            "ternary" | "muldiv" => self.pop(2),
            name => {
                if let Some((pops, pushes)) = builtin_stack_effect(name) {
                    self.pop(pops);
                    self.push(pushes);
                }
            }
        }
    }

    fn push(&mut self, count: usize) {
        self.depth = (self.depth + count as isize).min(128);
    }

    fn pop(&mut self, count: usize) {
        self.depth = (self.depth - count as isize).max(0);
    }
}

/// `(pops, pushes)` of the builtin BP opcodes, from the target handlers
/// (0x473F20..0x4755AF). `sprintf` reports its fixed part only; the argument
/// count depends on the format string.
fn builtin_stack_effect(name: &str) -> Option<(usize, usize)> {
    Some(match name {
        "atan2" | "strfind" | "streq" => (2, 1),
        "vec3_length" | "memory_equal" => (3, 1),
        "qword_add" | "qword_sub" | "qword_mul" | "qword_div" | "qword_mod" => (3, 0),
        "memcpy" | "memset" | "strconcat" | "quote_string" => (3, 0),
        "memclr" | "strcpy" => (2, 0),
        "memrepeat" => (4, 0),
        "memfind" | "strreplace" => (4, 1),
        "strlen" | "malloc" | "free" | "engine_state" | "clipboard_set" => (1, 1),
        "getchar" => (1, 3),
        "tolower" | "set_memory_mode" | "message_box" | "show_number" => (1, 0),
        "sprintf" => (2, 0),
        "addmemboundary" => (3, 1),
        "confirm" | "modal_list" => (2, 1),
        "dumpmem" => (3, 0),
        "resource_transform" => (7, 0),
        "resource_blend" => (5, 0),
        _ => return None,
    })
}

fn operand_as_u32(operand: &BpOperand) -> Option<u32> {
    match operand {
        BpOperand::U8(value) => Some(*value as u32),
        BpOperand::U16(value) => Some(*value as u32),
        BpOperand::U32(value) => Some(*value),
        BpOperand::I32(value) => Some(*value as u32),
        BpOperand::Offset(value) => Some(*value),
        BpOperand::String(_) | BpOperand::Raw(_) => None,
    }
}

fn native_corrected_call_arg_count(group: u8, id: u16) -> Option<usize> {
    Some(match (group, id) {
        (0x80, 0x12)
        | (0x80, 0x47)
        | (0x90, 0x81)
        | (0x90, 0x99)
        | (0x90, 0xdd)
        | (0x91, 0x8d)
        | (0x91, 0x96) => 1,
        (0x80, 0x07)
        | (0x80, 0x38)
        | (0x80, 0x48)
        | (0x80, 0x88)
        | (0x80, 0x9a)
        | (0x80, 0x9c)
        | (0x80, 0xd3)
        | (0x90, 0x06)
        | (0x90, 0x34)
        | (0x90, 0x39)
        | (0x90, 0x4c)
        | (0x90, 0x57)
        | (0x90, 0x82)
        | (0x90, 0x89)
        | (0x90, 0x97)
        | (0x90, 0x98)
        | (0x90, 0x9b)
        | (0x90, 0xf7)
        | (0x91, 0x06)
        | (0x91, 0x3f)
        | (0x91, 0x8b)
        | (0x91, 0x9f)
        | (0x92, 0x14)
        | (0x92, 0x18)
        | (0xa0, 0x19)
        | (0xb0, 0x03)
        | (0xb0, 0xc1)
        | (0xc0, 0x44)
        | (0xc0, 0x4f) => 2,
        (0x80, 0x4a)
        | (0x81, 0x60)
        | (0x90, 0x33)
        | (0x90, 0x37)
        | (0x90, 0x9a)
        | (0x90, 0x9d)
        | (0x90, 0xd9)
        | (0x90, 0xf6)
        | (0x91, 0x8c) => 3,
        (0x90, 0x38) => 4,
        (0x92, 0xf1) | (0x92, 0xf2) => 5,
        (0x80, 0x1f) | (0x92, 0x89) | (0xc0, 0x45) => 6,
        (0x90, 0x19) | (0x92, 0x97) | (0xc0, 0x46) => 7,
        (0x90, 0x24) => 12,
        (0x90, 0x29) => 12,
        (0x90, 0x2c) => 9,
        (0x80, 0xf2) => 14,
        (0x90, 0x5d) => 20,
        (0x91, 0x60) | (0x80, 0x80) => 0,
        _ => return None,
    })
}

pub fn known_call_arg_count(group: u8, id: u16) -> Option<usize> {
    if let Some(abi) = crate::native_abi::lookup(group, id) {
        return Some(abi.argc);
    }
    if let Some(count) = native_corrected_call_arg_count(group, id) {
        return Some(count);
    }
    Some(match (group, id) {
        // Native secondary-dispatch handlers recovered from
        // Tayutama2_trial_TG.exe. These entries correct older inferred ABI.
        (0x80, 0x1a) => 1,
        (0x90, 0x1f) => 6,
        (0x90, 0x20) => 6,
        (0x90, 0x53) | (0x91, 0x1d) => 5,
        (0x90, 0x65) | (0x90, 0xe8) | (0x92, 0x17) => 4,
        (0x91, 0x88) => 7,
        (0x92, 0x91) => 11,
        (0x80, 0xed) | (0x80, 0xef) => 1,
        (0x80, 0xee) => 0,
        (0x91, 0x1b) | (0x91, 0x1c) | (0x92, 0x10) => 5,
        (0xb0, 0x08) => 7,
        (0xb0, 0x10) => 5,
        (0xb0, 0x11) | (0xb0, 0x17) => 1,
        (0xb0, 0x14) | (0xb0, 0x1c) => 2,
        (0xb0, 0x19) => 6,
        (0xb0, 0x82) => 3,
        (0xb0, 0x8c) => 4,
        (0xc0, 0x10) => 12,
        (0xc0, 0x1a) => 8,
        (0xc0, 0x1b) => 3,
        (0xc0, 0x20) => 4,
        (0xc0, 0x40) | (0xc0, 0x42) | (0xc0, 0x43) => 2,
        (0xc0, 0x44) => 6,
        (0xc0, 0x45) => 7,
        (0xc0, 0x46)
        | (0xc0, 0x47)
        | (0xc0, 0x48)
        | (0xc0, 0x49)
        | (0xc0, 0x4a)
        | (0xc0, 0x4b)
        | (0xc0, 0x4e) => 2,
        (0xc0, 0x4c) | (0xc0, 0x4d) => 4,
        (0x90, 0x14) | (0x90, 0x4a) | (0x90, 0xf0) | (0xa0, 0x28) => 5,
        (0x90, 0x15) | (0x90, 0xf4) => 4,
        (0x90, 0x40) | (0x90, 0xdf) => 1,
        (0x90, 0x4d) => 0,
        (0x90, 0xb4) => 3,
        (0x90, 0x47) => 5,
        (0x90, 0xda) | (0x90, 0xde) | (0x90, 0xf3) => 1,
        (0x91, 0x37) => 4,
        (0x91, 0xdb) => 0,
        (0x92, 0x01) => 7,
        (0x92, 0x8d) | (0x92, 0xf0) => 6,
        (0x92, 0xf5) => 2,
        (0x80, 0x06)
        | (0x80, 0x20)
        | (0x80, 0x41)
        | (0x80, 0x49)
        | (0x80, 0x5e)
        | (0x80, 0x85)
        | (0x80, 0x88)
        | (0x80, 0xd1)
        | (0x80, 0xd9)
        | (0x80, 0xe8)
        | (0xa0, 0x22)
        | (0xa0, 0x25) => 1,
        (0x80, 0x00)
        | (0x80, 0x0c)
        | (0x80, 0x11)
        | (0x80, 0x14)
        | (0x80, 0x18)
        | (0x80, 0x19)
        | (0x80, 0x1c)
        | (0x80, 0x21)
        | (0x80, 0x28)
        | (0x80, 0x2a)
        | (0x80, 0x36)
        | (0x80, 0x37)
        | (0x80, 0x38)
        | (0x80, 0x39)
        | (0x80, 0x50)
        | (0x80, 0x52)
        | (0x80, 0x58)
        | (0x80, 0x64)
        | (0x80, 0x66)
        | (0x80, 0x67)
        | (0x80, 0x68)
        | (0x80, 0x70)
        | (0x80, 0x74)
        | (0x80, 0x80)
        | (0x80, 0x99)
        | (0x80, 0xaf)
        | (0x81, 0x0e)
        | (0x81, 0x18)
        | (0x81, 0x62)
        | (0x81, 0x63)
        | (0x81, 0x6f) => 1,
        (0x80, 0x1b)
        | (0x80, 0x33)
        | (0x80, 0x34)
        | (0x80, 0x35)
        | (0x80, 0x3a)
        | (0x80, 0x3d)
        | (0x80, 0x40)
        | (0x80, 0x62)
        | (0x80, 0xa8)
        | (0x80, 0x4b)
        | (0x80, 0x2d)
        | (0x80, 0xc1)
        | (0x80, 0xc5)
        | (0x80, 0xd0)
        | (0x80, 0xdc)
        | (0x80, 0xdb)
        | (0x81, 0x35)
        | (0x81, 0x64) => 2,
        (0x80, 0x30)
        | (0x80, 0x32)
        | (0x80, 0x47)
        | (0x80, 0x48)
        | (0x80, 0x5c)
        | (0x80, 0x60)
        | (0x80, 0x82)
        | (0x80, 0x83)
        | (0x80, 0x98)
        | (0x80, 0x9a)
        | (0x80, 0x9d)
        | (0x80, 0xac)
        | (0x80, 0xc0)
        | (0x80, 0xd2)
        | (0x80, 0xd3)
        | (0x80, 0xda)
        | (0x80, 0xdd)
        | (0x80, 0x8b) => 3,
        (0x80, 0x2b) => 5,
        (0x80, 0x3b) => 6,
        (0x80, 0x1f) => 6,
        (0x80, 0x4c) | (0x80, 0x8a) | (0x80, 0x9c) | (0x80, 0xc4) | (0x80, 0xd4) => 4,
        (0x80, 0x31) | (0x80, 0x44) | (0x81, 0x30) => 5,
        (0x80, 0x07) => 2,
        (0x80, 0x25) => 5,
        (0x80, 0x3f) => 4,
        (0x80, 0x59) => 1,
        (0x80, 0x9e) => 3,
        (0x80, 0xa0) => 1,
        (0x80, 0xe0) => 4,
        (0x80, 0xe1) | (0x80, 0xe2) => 3,
        (0x80, 0xf0) => 8,
        (0x80, 0xf2) => 16,
        (0x80, 0xf7) | (0x80, 0xf8) => 3,
        (0x81, 0x19) | (0x81, 0x2f) | (0x81, 0x36) => 1,
        (0x81, 0x31) | (0x81, 0xf7) => 4,
        (0x81, 0x37) => 2,
        (0x80, 0x01)
        | (0x80, 0x08)
        | (0x80, 0x04)
        | (0x80, 0x0d)
        | (0x80, 0x0f)
        | (0x80, 0x13)
        | (0x80, 0x16)
        | (0x80, 0x17)
        | (0x80, 0x46)
        | (0x80, 0x5f)
        | (0x80, 0x5a)
        | (0x80, 0x61)
        | (0x80, 0x65)
        | (0x80, 0x6a)
        | (0x80, 0x81)
        | (0x80, 0x45)
        | (0x80, 0xd8)
        | (0x80, 0xfd)
        | (0x81, 0x0f) => 0,
        (0x80, 0x02)
        | (0x80, 0x05)
        | (0x80, 0x15)
        | (0x80, 0x84)
        | (0x80, 0xa1)
        | (0x80, 0x29)
        | (0x80, 0x2c)
        | (0x80, 0xe3) => 1,
        (0x80, 0x2f) => 2,
        (0x80, 0x69) | (0x90, 0xdb) | (0x90, 0xf1) | (0x90, 0xf2) | (0x92, 0x15) => 0,
        (0x90, 0x00)
        | (0x90, 0x02)
        | (0x90, 0x03)
        | (0x90, 0x04)
        | (0x90, 0x07)
        | (0x90, 0x08)
        | (0x90, 0x09)
        | (0x90, 0x0d)
        | (0x90, 0x12)
        | (0x90, 0x51)
        | (0x90, 0x61)
        | (0x90, 0x94)
        | (0x90, 0x9c)
        | (0x90, 0x9f)
        | (0x90, 0xaf)
        | (0x90, 0xb9)
        | (0x90, 0xb8)
        | (0x90, 0xd0)
        | (0x90, 0xd1)
        | (0x90, 0xd7)
        | (0x90, 0xe1)
        | (0x91, 0x8e)
        | (0x91, 0x0d)
        | (0x91, 0xb8) => 1,
        (0x90, 0x06)
        | (0x90, 0x0c)
        | (0x90, 0x05)
        | (0x90, 0x13)
        | (0x90, 0x16)
        | (0x90, 0x17)
        | (0x90, 0x35)
        | (0x90, 0x30)
        | (0x90, 0x31)
        | (0x90, 0x32)
        | (0x90, 0x3a)
        | (0x90, 0x3c)
        | (0x90, 0x54)
        | (0x90, 0x55)
        | (0x90, 0x64)
        | (0x90, 0xba)
        | (0x90, 0xbe)
        | (0x90, 0x80)
        | (0x90, 0x83)
        | (0x90, 0x84)
        | (0x90, 0x87)
        | (0x90, 0x95)
        | (0x90, 0x96)
        | (0x90, 0x97)
        | (0x90, 0xb6)
        | (0x90, 0xb7)
        | (0x90, 0xbc)
        | (0x90, 0xbf)
        | (0x90, 0xd4)
        | (0x90, 0xd9)
        | (0x90, 0xe4)
        | (0x90, 0xe9)
        | (0x91, 0x06)
        | (0x91, 0x1f)
        | (0x91, 0x55)
        | (0x91, 0x89)
        | (0x91, 0x94)
        | (0x91, 0x9a)
        | (0x91, 0x95)
        | (0x91, 0xba)
        | (0x92, 0x16)
        | (0x92, 0x8c)
        | (0x92, 0xf4)
        | (0x92, 0xf6)
        | (0x92, 0x88) => 2,
        (0x90, 0xd5)
        | (0x90, 0xd6)
        | (0x90, 0xd8)
        | (0x90, 0x36)
        | (0x91, 0x38)
        | (0x91, 0x48)
        | (0x91, 0x49)
        | (0x92, 0x12)
        | (0x90, 0xf7)
        | (0x90, 0x10) => 3,
        (0x90, 0x11)
        | (0x90, 0x86)
        | (0x90, 0xe5)
        | (0x91, 0x33)
        | (0x91, 0x1e)
        | (0x91, 0x36)
        | (0x91, 0x3e)
        | (0x91, 0x4a) => 4,
        (0x90, 0x0e) | (0x90, 0x88) | (0x91, 0x0e) | (0x91, 0x10) | (0x91, 0x13) | (0x91, 0x15) => {
            5
        }
        (0x90, 0x18) | (0x90, 0x90) | (0x91, 0x12) | (0x91, 0x98) => 6,
        (0x90, 0x1e) => 8,
        (0x90, 0x56) | (0x90, 0x66) | (0x90, 0x85) | (0x91, 0x16) | (0x91, 0x9b) | (0x92, 0x97) => {
            7
        }
        (0x90, 0x58) | (0x91, 0x40) | (0x92, 0x1e) => 9,
        (0x90, 0x28) | (0x90, 0x5d) | (0x90, 0x89) => 12,
        (0x92, 0x90) => 15,
        (0x90, 0xcd) => 8,
        (0x90, 0xcc) => 2,
        (0x91, 0x19) => 11,
        (0x90, 0x23) | (0x90, 0x5a) => 10,
        (0x90, 0x43) => 9,
        (0x90, 0x5c) => 17,
        (0x90, 0x50) | (0x90, 0x60) | (0x90, 0xe0) => 0,
        (0x90, 0x01)
        | (0x90, 0x3d)
        | (0x90, 0xf5)
        | (0x91, 0x60)
        | (0x91, 0x61)
        | (0x91, 0xf2)
        | (0x91, 0xf6) => 1,
        (0x91, 0x11) | (0x91, 0x64) => 2,
        (0x91, 0x65) => 6,
        (0x91, 0x66) => 4,
        (0x91, 0x9c) => 14,
        (0x91, 0xf1) => 2,
        (0x90, 0xf6) => 5,
        (0x92, 0xf1) => 6,
        (0x92, 0xf2) => 13,
        (0x92, 0x19) | (0x92, 0x8e) => 1,
        // Tayutama2_trial_TG.exe: funcs_486FEE[0x9c] -> sub_4867D0.
        // The native handler pops 21 BP values before calling sub_403B10.
        (0x92, 0x9c) => 21,
        (0x92, 0x14) => 2,
        (0xa0, 0x08) | (0xa0, 0x09) | (0xa0, 0x14) | (0xa0, 0x26) => 2,
        (0xa0, 0x12) => 7,
        (0xa0, 0x15) => 2,
        (0xa0, 0x16) | (0xa0, 0x20) | (0xa0, 0x24) => 3,
        (0xa0, 0x11) => 5,
        (0xa0, 0x21) => 5,
        (0xb0, 0x02) | (0xb0, 0x06) => 0,
        (0xb0, 0x03) => 2,
        (0xb0, 0x05) | (0xb0, 0x80) | (0xb0, 0xc1) | (0xb0, 0xc4) => 1,
        (0xb0, 0xc7) => 2,
        (0xc0, 0x01) | (0xc0, 0x0f) | (0xc0, 0x41) => 1,
        (0xc0, 0x00) | (0xc0, 0x04) | (0xc0, 0x09) | (0xc0, 0x0a) | (0xc0, 0x0c) | (0xc0, 0x0d) => {
            2
        }
        (0xc0, 0x05) => 6,
        (0xc0, 0x0b) => 10,
        (0xc0, 0x18) => 7,
        (0xc0, 0x1f) => 1,
        (0xc0, 0x28) => 4,
        (0xc0, 0x29) => 16,
        (0xc0, 0x2d) => 18,
        _ => return None,
    })
}

/// Number of values written to the BP operand stack by a native dispatch.
///
/// Most value-returning handlers write one value. These six handlers were
/// recovered directly from the eight native secondary dispatch tables and
/// call `sub_4450D0` more than once on their successful path.
pub fn known_call_stack_output_count(group: u8, id: u16) -> usize {
    if let Some(abi) = crate::native_abi::lookup(group, id) {
        return abi.stack_outputs;
    }
    usize::from(known_call_returns_value(group, id))
}

pub fn known_call_returns_value(group: u8, id: u16) -> bool {
    if let Some(abi) = crate::native_abi::lookup(group, id) {
        return abi.returns_value();
    }
    matches!(
        (group, id),
        (0x80, 0x04)
            | (0x80, 0x06)
            | (0x80, 0x25)
            | (0x80, 0x3f)
            | (0x80, 0x59)
            | (0x80, 0x9e)
            | (0x80, 0xa0)
            | (0x80, 0xe0)
            | (0x80, 0xe2)
            | (0x80, 0xe3)
            | (0x80, 0xed)
            | (0x80, 0xf0)
            | (0x80, 0xf2)
            | (0x80, 0xf7)
            | (0x80, 0xf8)
            | (0x80, 0x01)
            | (0x80, 0x02)
            | (0x80, 0x05)
            | (0x80, 0x0d)
            | (0x80, 0x0f)
            | (0x80, 0x11)
            | (0x80, 0x12)
            | (0x80, 0x13)
            | (0x80, 0x14)
            | (0x80, 0x16)
            | (0x80, 0x17)
            | (0x80, 0x19)
            | (0x80, 0x20)
            | (0x80, 0x21)
            | (0x80, 0x28)
            | (0x80, 0x29)
            | (0x80, 0x2a)
            | (0x80, 0x2c)
            | (0x80, 0x30)
            | (0x80, 0x34)
            | (0x80, 0x35)
            | (0x80, 0x40)
            | (0x80, 0x44)
            | (0x80, 0x47)
            | (0x80, 0x49)
            | (0x80, 0x61)
            | (0x80, 0x3a)
            | (0x80, 0x3b)
            | (0x80, 0x80)
            | (0x80, 0x84)
            | (0x80, 0x85)
            | (0x80, 0x8b)
            | (0x80, 0xc0)
            | (0x80, 0xc1)
            | (0x80, 0xc4)
            | (0x80, 0xc5)
            | (0x80, 0xd0)
            | (0x80, 0xd3)
            | (0x80, 0xd4)
            | (0x80, 0xd9)
            | (0x80, 0xdb)
            | (0x80, 0xdc)
            | (0x80, 0xdd)
            | (0x80, 0xfd)
            | (0x81, 0x30)
            | (0x81, 0x35)
            | (0x81, 0x19)
            | (0x81, 0x2f)
            | (0x81, 0x31)
            | (0x81, 0x36)
            | (0x81, 0x37)
            | (0x81, 0xf7)
            | (0x90, 0xda)
            | (0x90, 0xdb)
            | (0x91, 0xdb)
            | (0x92, 0xf0)
            | (0x92, 0xf5)
            | (0x90, 0x4d)
            | (0x90, 0xf0)
            | (0x90, 0xf4)
            | (0xa0, 0x28)
            | (0xb0, 0x08)
            | (0xb0, 0x10)
            | (0xb0, 0x17)
            | (0xb0, 0x82)
            | (0xb0, 0x8c)
            | (0xc0, 0x40)
            | (0x90, 0x16)
            | (0x90, 0x17)
            | (0x90, 0x3d)
            | (0x92, 0x16)
            | (0x92, 0xf2)
            | (0x92, 0xf4)
            | (0x92, 0xf1)
            | (0x90, 0x50)
            | (0x90, 0x60)
            | (0x90, 0x80)
            | (0x90, 0xbc)
            | (0x90, 0xbf)
            | (0x90, 0xe0)
            | (0x90, 0xe1)
            | (0x90, 0xe9)
            | (0x91, 0x95)
            | (0x91, 0x8d)
            | (0x92, 0x9c)
            | (0x91, 0x8e)
            | (0x91, 0xb8)
            | (0x91, 0xba)
            | (0xa0, 0x22)
            | (0xa0, 0x24)
            | (0xb0, 0xc1)
            | (0xb0, 0xc4)
    )
}

#[cfg(test)]
mod tests {
    use super::known_call_arg_count;

    #[test]
    fn graph_control_selectors_follow_target_abi_descriptor() {
        assert_eq!(known_call_arg_count(0x90, 0x21), Some(9));
        assert_eq!(known_call_arg_count(0x90, 0x22), Some(7));
    }
}
