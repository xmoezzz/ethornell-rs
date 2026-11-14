//! Canonical BP VM opcode reference embedded in the source tree.
//!
//! This table is intentionally evidence graded. `high` means the target
//! executable or its dispatch table establishes the operation. `medium` means
//! the current implementation and script call sites agree but more native
//! annotation is desirable. `low` means only registration or a provisional
//! implementation name is known; those entries must not be treated as proven
//! reverse engineering results.

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct BpOpcodeSpec {
    pub code: u8,
    pub symbol: &'static str,
    pub stack_effect: &'static str,
    pub immediate: &'static str,
    pub description: &'static str,
    pub confidence: &'static str,
    pub evidence: &'static str,
}

/// Recovered opcode contracts across the supported BP interpreter revisions.
///
/// Stack effects are written in native operand-ring terms. `implementation
/// defined` marks a contract that still needs direct target recovery.
pub const BP_OPCODE_SPECS: &[BpOpcodeSpec] = &[
    BpOpcodeSpec { code: 0xd4, symbol: "load_frame_dword", stack_effect: "0 -> 1", immediate: "u16 frame displacement", description: "Load a full-width frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xd5, symbol: "load_frame_dword_add", stack_effect: "0 -> 1", immediate: "u16 frame; signed LEB addend", description: "Add an inline value to a full-width frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xd6, symbol: "load_frame_dword_indirect", stack_effect: "0 -> 1", immediate: "u16 frame; packed width/offset", description: "Dereference a frame pointer plus an inline offset.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xd7, symbol: "store_frame_indirect_immediate", stack_effect: "0 -> 0", immediate: "u16 frame; u16 offset; packed width/value", description: "Store an inline value through a frame pointer.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xd8, symbol: "load_frame_dword_madd", stack_effect: "1 -> 1", immediate: "u16 frame; signed LEB scale", description: "Add a popped value to the scaled frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xda, symbol: "compare_frame_immediate", stack_effect: "0 -> 1", immediate: "u8 predicate; u16 frame; signed LEB immediate", description: "Compare a full-width frame value to an immediate.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xdb, symbol: "compare_indirect_immediate", stack_effect: "0 -> 1", immediate: "u8 predicate; u16 frame; packed width/offset; signed LEB immediate", description: "Compare an indirect frame value to an immediate.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xdc, symbol: "compare_frame_immediate_branch", stack_effect: "0 -> 0", immediate: "u8 predicate/inversion; i16 target; u16 frame; signed LEB immediate", description: "Branch on a comparison against a full-width frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xdd, symbol: "compare_indirect_immediate_branch", stack_effect: "0 -> 0", immediate: "u8 predicate/inversion; i16 target; u16 frame; packed width/offset; signed LEB immediate", description: "Branch on an indirect frame comparison.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xde, symbol: "branch_frame_zero", stack_effect: "0 -> 0", immediate: "i16 target; u16 inversion/frame", description: "Branch on a full-width frame value; bit 15 selects nonzero.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xdf, symbol: "branch_indirect_zero", stack_effect: "0 -> 0", immediate: "i16 target; u16 inversion/frame; packed width/offset", description: "Branch on an indirect frame value; bit 15 selects nonzero.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xe2, symbol: "store_frame_batch", stack_effect: "count -> 0", immediate: "u8 count-minus-one; packed u16 frame stores", description: "Pop values in order into width-tagged frame destinations.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xe8, symbol: "store_frame_select", stack_effect: "3 -> 0", immediate: "packed u16 frame destination", description: "Pop false value, true value and condition; store the selected value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xe9, symbol: "store_frame_indirect", stack_effect: "1 -> 0", immediate: "packed u16 width/frame; u16 offset", description: "Store a popped value through a frame pointer.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xea, symbol: "store_frame_indirect_leb", stack_effect: "0 -> 0", immediate: "packed u16 width/frame; u16 offset; signed LEB value", description: "Store an inline value through a frame pointer.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf0, symbol: "store_absolute_frame_add", stack_effect: "0 -> 0", immediate: "u32 destination; packed u16 width/frame; signed LEB addend", description: "Store the sum of a frame value and immediate through an absolute pointer.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf1, symbol: "store_frame_frame_add", stack_effect: "0 -> 0", immediate: "u16 destination; packed u16 width/frame; signed LEB addend", description: "Store the sum of a frame value and immediate into another frame slot.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf2, symbol: "store_indirect_frame_add", stack_effect: "0 -> 0", immediate: "u16 pointer frame; u16 offset; packed u16 width/frame; signed LEB addend", description: "Store a frame value plus immediate through another frame pointer.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf4, symbol: "store_frame_frame_madd", stack_effect: "0 -> 0", immediate: "u16 destination; u32 addend; packed u16 width/frame; i16 scale", description: "Store an inline addend plus a scaled frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf5, symbol: "store_frame_two_frame_madd", stack_effect: "0 -> 0", immediate: "u16 destination; u16 addend frame; i16 addend; packed u16 width/frame; i16 scale", description: "Store the sum of two frame values with inline scale and addend.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf7, symbol: "load_indirect_frame_madd", stack_effect: "1 -> 1", immediate: "packed u16 width/frame; u16 offset; i16 scale", description: "Add a popped value to a scaled indirect frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf8, symbol: "load_frame_madd_fixed", stack_effect: "0 -> 1", immediate: "u32 addend; u16 frame; i16 scale", description: "Push an inline addend plus a scaled frame value.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xf9, symbol: "load_two_frame_madd", stack_effect: "0 -> 1", immediate: "u16 addend frame; i16 addend; u16 frame; i16 scale", description: "Push a scaled frame value plus another frame value and immediate.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xfa, symbol: "load_indexed_frame_fixed", stack_effect: "0 -> 1", immediate: "u32 base; packed u16 width/frame; i16 scale", description: "Load at inline base plus scaled full-width frame index.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec { code: 0xfb, symbol: "load_indexed_frame_relative", stack_effect: "0 -> 1", immediate: "u16 base frame displacement; packed u16 width/frame; i16 scale", description: "Load at a frame address plus scaled full-width frame index.", confidence: "high", evidence: "Triple Pairing BGI 1.687 x64 table 0x1401CBB70; native handlers and fetch helpers" },
    BpOpcodeSpec {
        code: 0x03,
        symbol: "push_compact_batch",
        stack_effect: "0 -> count",
        immediate: "u8 count/type; signed LEB values or fixed-width literal",
        description: "Push a batch of compact signed operands, or one signed 8/16/32/64-bit literal (32-bit operand ring).",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CD470; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x0d,
        symbol: "store_immediate",
        stack_effect: "1 -> 0",
        immediate: "signed LEB payload with low two width bits",
        description: "Store the inline value through a popped pointer.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CD130; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x0e,
        symbol: "store_frame_immediate",
        stack_effect: "0 -> 0",
        immediate: "u16 frame displacement; packed signed LEB width/value",
        description: "Store an inline value into the frame.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CD090; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x13,
        symbol: "jump_relative",
        stack_effect: "0 -> 0",
        immediate: "i16 opcode-relative target",
        description: "Jump directly to the signed opcode-relative target.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CCF20; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x36,
        symbol: "compare_immediate",
        stack_effect: "1 -> 1",
        immediate: "u8 predicate; signed LEB i32",
        description: "Compare a popped left operand with the immediate; bitwise predicates retain their integer result.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CC2D0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x37,
        symbol: "compare_immediate_branch",
        stack_effect: "1 -> 0",
        immediate: "u8 predicate/inversion; i16 target; signed LEB i32",
        description: "Compare a popped left operand with an immediate and branch relative to this opcode.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CC210; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x3e,
        symbol: "store_alu",
        stack_effect: "3 -> 0",
        immediate: "u8 width/comparison/operator",
        description: "Pop right, left, destination; compute the selected arithmetic or comparison operation and store it.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CBF80; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x3f,
        symbol: "store_alu_immediate",
        stack_effect: "2 -> 0",
        immediate: "u8 width/comparison/operator; signed LEB i32",
        description: "Compute an operation over the popped left operand and the immediate, then store through a popped destination.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400CBEE0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x45,
        symbol: "rotate_integer_vector",
        stack_effect: "5 -> 0",
        immediate: "none",
        description: "Pop z/y/x angles in 16.16 degrees, source, destination; rotate the integer vector in x/y/z order and round floor(value+0.5).",
        confidence: "high",
        evidence: "Houkago FD dispatch 0x004890E0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x46,
        symbol: "lerp_integer_to_fixed",
        stack_effect: "4 -> 1",
        immediate: "none",
        description: "Interpolate integer endpoints with clamped position/total, yielding a 16.16 value.",
        confidence: "high",
        evidence: "Houkago FD dispatch 0x004892F0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x47,
        symbol: "lerp_fixed",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Interpolate 16.16 endpoints using a clamped 16.16 progress value.",
        confidence: "high",
        evidence: "Houkago FD dispatch 0x004893D0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x55,
        symbol: "pow_fixed",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Raise a 16.16 base to a 16.16 power and convert using native SSE rounding and integer snapping.",
        confidence: "high",
        evidence: "Houkago FD dispatch 0x00489680; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x73,
        symbol: "register_installation",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Register company/product InstalledFolder metadata from three string pointers.",
        confidence: "high",
        evidence: "Houkago FD dispatch 0x0048A220; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xe3,
        symbol: "load_frame_batch",
        stack_effect: "0 -> count",
        immediate: "u8 count-minus-one; packed u16 frame loads",
        description: "Load multiple frame values in operand order.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C9A80; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xe4,
        symbol: "load_frame_indirect",
        stack_effect: "0 -> 1",
        immediate: "packed u16 frame load; packed signed LEB width/offset",
        description: "Load through frame_value + immediate.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C99D0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xe5,
        symbol: "load_frame_indexed",
        stack_effect: "1 -> 1",
        immediate: "packed u16 frame load; packed signed LEB width/scale",
        description: "Load through popped_base + immediate*frame_value.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C9910; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xe6,
        symbol: "store_frame_add",
        stack_effect: "1 -> 0",
        immediate: "packed u16 frame load; packed signed LEB width/addend",
        description: "Store frame_value + immediate through a popped destination.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C9860; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xe7,
        symbol: "store_frame_madd",
        stack_effect: "2 -> 0",
        immediate: "packed u16 frame load; packed signed LEB width/scale",
        description: "Store popped_addend + immediate*frame_value through a popped destination.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C97A0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xec,
        symbol: "copy_immediate",
        stack_effect: "2 -> 0",
        immediate: "signed LEB byte count",
        description: "Copy the immediate byte count from popped source to popped destination.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C9740; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xed,
        symbol: "copy_frame_immediate",
        stack_effect: "1 -> 0",
        immediate: "u16 frame displacement; signed LEB byte count",
        description: "Copy bytes from frame SP minus displacement to a popped destination.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C96C0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xee,
        symbol: "call_relative",
        stack_effect: "0 -> call frame",
        immediate: "i16 opcode-relative target",
        description: "Call the opcode-relative target and save the next instruction IP.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C95F0; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0xef,
        symbol: "call_absolute_indirect",
        stack_effect: "0 -> call frame",
        immediate: "u32 packed width/address",
        description: "Load a function target from the packed absolute memory operand and call it.",
        confidence: "high",
        evidence: "Dokai dispatch 0x1400C9500; original handler assembly and fetch helpers",
    },
    BpOpcodeSpec {
        code: 0x0f,
        symbol: "store_frame_compact",
        stack_effect: "1 -> 0",
        immediate: "u16 width/displacement",
        description: "Store one operand at frame SP minus the low 14-bit displacement; high bits select byte/word/dword.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488270",
    },
    BpOpcodeSpec {
        code: 0x12,
        symbol: "adjust_frame_compact",
        stack_effect: "0 -> 0",
        immediate: "signed LEB128 i32",
        description: "Add a signed compact immediate to the raw frame SP.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488370",
    },
    BpOpcodeSpec {
        code: 0x18,
        symbol: "load_absolute_compact",
        stack_effect: "0 -> 1",
        immediate: "u32 width/address",
        description: "Load signed memory at low 30-bit address; high two bits select width.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x4885B0",
    },
    BpOpcodeSpec {
        code: 0x19,
        symbol: "load_frame_compact",
        stack_effect: "0 -> 1",
        immediate: "u16 width/displacement",
        description: "Load signed memory at SP minus the low 14-bit displacement.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488610",
    },
    BpOpcodeSpec {
        code: 0x1a,
        symbol: "load_frame_add_compact",
        stack_effect: "0 -> 1",
        immediate: "u16 width/displacement, signed LEB128 i32",
        description: "Load a frame value and add the compact immediate.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488680",
    },
    BpOpcodeSpec {
        code: 0x1b,
        symbol: "load_frame_madd_compact",
        stack_effect: "1 -> 1",
        immediate: "u16 width/displacement, signed LEB128 i32",
        description: "Pop an addend and push addend plus immediate times the frame value.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488710",
    },
    BpOpcodeSpec {
        code: 0x1c,
        symbol: "load_frame_shift_compact",
        stack_effect: "0 -> 1",
        immediate: "u16 width/displacement, i8 shift",
        description: "Load a frame value; positive shift is left, negative is arithmetic right.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488790",
    },
    BpOpcodeSpec {
        code: 0x1d,
        symbol: "load_frame_mul_compact",
        stack_effect: "1 -> 1",
        immediate: "u16 width/displacement",
        description: "Multiply the popped operand by a frame value.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488810",
    },
    BpOpcodeSpec {
        code: 0x1e,
        symbol: "load_frame_div_compact",
        stack_effect: "1 -> 1",
        immediate: "u16 width/displacement",
        description: "Divide the popped operand by a frame value.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x4888A0",
    },
    BpOpcodeSpec {
        code: 0x1f,
        symbol: "load_pointer_offset_compact",
        stack_effect: "1 -> 1",
        immediate: "signed LEB128 packed offset/width",
        description: "Add the arithmetic-shifted immediate to the popped pointer and load signed memory.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488930",
    },
    BpOpcodeSpec {
        code: 0x2c,
        symbol: "add_immediate_compact",
        stack_effect: "1 -> 1",
        immediate: "signed LEB128 i32",
        description: "Add a compact immediate to the top operand.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488BA0",
    },
    BpOpcodeSpec {
        code: 0x2d,
        symbol: "mul_immediate_compact",
        stack_effect: "1 -> 1",
        immediate: "signed LEB128 i32",
        description: "Multiply the top operand by a compact immediate.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488BD0",
    },
    BpOpcodeSpec {
        code: 0x2e,
        symbol: "madd_immediate_compact",
        stack_effect: "2 -> 1",
        immediate: "signed LEB128 i32",
        description: "Pop b,a and push a plus immediate times b.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488C00",
    },
    BpOpcodeSpec {
        code: 0x2f,
        symbol: "div_immediate_compact",
        stack_effect: "1 -> 1",
        immediate: "signed LEB128 i32",
        description: "Divide the top operand by an immediate; zero gives -1.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488C40",
    },
    BpOpcodeSpec {
        code: 0x3b,
        symbol: "compare_branch_compact",
        stack_effect: "2 -> 0",
        immediate: "u8 predicate/inversion, i16 relative target",
        description: "Compare two operands and branch relative to this opcode. Bit 7 keeps the predicate; otherwise invert.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488E40",
    },
    BpOpcodeSpec {
        code: 0x3c,
        symbol: "shift_immediate_compact",
        stack_effect: "1 -> 1",
        immediate: "i8 shift",
        description: "Positive shift is left; negative is arithmetic right.",
        confidence: "high",
        evidence: "Houkago Cinderella 2 FD opcode table 0x559710 -> 0x488F70",
    },
    // 0x00 push_byte: Sign-extend an 8-bit literal into one BP value.
    BpOpcodeSpec {
        code: 0x00,
        symbol: "push_byte",
        stack_effect: "0 -> 1",
        immediate: "i8 immediate",
        description: "Sign-extend an 8-bit literal and push it as one BP value.",
        confidence: "high",
        evidence: "native opcode table and interpreter implementation",
    },
    // 0x01 push_word: Sign-extend a 16-bit literal into one BP value.
    BpOpcodeSpec {
        code: 0x01,
        symbol: "push_word",
        stack_effect: "0 -> 1",
        immediate: "i16 immediate",
        description: "Sign-extend a 16-bit literal and push it as one BP value.",
        confidence: "high",
        evidence: "native opcode table and interpreter implementation",
    },
    // 0x02 push_dword: Push 32 bit literal as one BP value.
    BpOpcodeSpec {
        code: 0x02,
        symbol: "push_dword",
        stack_effect: "0 -> 1",
        immediate: "u32 immediate",
        description: "Push 32 bit literal as one BP value.",
        confidence: "high",
        evidence: "native opcode table and interpreter implementation",
    },
    // 0x04 push_base_offset: Subtract a signed displacement from the raw frame base and tag it.
    BpOpcodeSpec {
        code: 0x04,
        symbol: "push_base_offset",
        stack_effect: "0 -> 1",
        immediate: "i16 frame displacement",
        description: "Push (raw_frame_base - displacement) tagged as a base-relative BP pointer.",
        confidence: "high",
        evidence: "native pointer resolver and opcode handler",
    },
    // 0x05 push_string: Push a tagged string pointer at a signed code-relative displacement.
    BpOpcodeSpec {
        code: 0x05,
        symbol: "push_string",
        stack_effect: "0 -> 1",
        immediate: "i16 displacement from the opcode address",
        description: "Push a tagged pointer to NUL-terminated Shift-JIS bytes in BP code memory.",
        confidence: "high",
        evidence: "native handler and script corpus",
    },
    // 0x06 push_offset: Push a raw code offset at a signed code-relative displacement.
    BpOpcodeSpec {
        code: 0x06,
        symbol: "push_offset",
        stack_effect: "0 -> 1",
        immediate: "i16 displacement from the opcode address",
        description: "Push the raw BP code offset selected by a signed relative displacement.",
        confidence: "high",
        evidence: "target sub_473650 and BP call corpus",
    },
    // 0x08 load: Pop an address and load a signed value of the selected width.
    BpOpcodeSpec {
        code: 0x08,
        symbol: "load",
        stack_effect: "1 -> 1",
        immediate: "width selector: 0=i8, 1=i16, 2=i32",
        description: "Pop an address; load signed 8-, 16-, or 32-bit data selected by width; sign-extend narrow values to one BP value.",
        confidence: "high",
        evidence: "target sub_473680: *(char *), *(__int16 *), *(int *) then sub_4450D0",
    },
    // 0x09 move: Store through a destination pointer and push the assigned value back.
    BpOpcodeSpec {
        code: 0x09,
        symbol: "move",
        stack_effect: "2 -> 1",
        immediate: "width selector",
        description: "Pop value then destination, store by width, and push the assigned value.",
        confidence: "high",
        evidence: "native assignment handler",
    },
    // 0x0A move_arg: Store a value through an argument or frame relative destination.
    BpOpcodeSpec {
        code: 0x0A,
        symbol: "move_arg",
        stack_effect: "2 -> 0",
        immediate: "width selector",
        description: "Store a value through an argument or frame relative destination.",
        confidence: "medium",
        evidence: "current VM and script call sites",
    },
    // 0x0B copy_inline: Copy an inline byte payload into one destination pointer.
    BpOpcodeSpec {
        code: 0x0B,
        symbol: "copy_inline",
        stack_effect: "1 -> 0",
        immediate: "u8 byte count followed by that many payload bytes",
        description: "Pop one destination pointer and copy the inline payload into BP memory.",
        confidence: "high",
        evidence: "target sub_473790 and instruction-stream reader",
    },
    // 0x0C copy_stack: Store a fixed count of stack values into consecutive destination slots.
    BpOpcodeSpec {
        code: 0x0C,
        symbol: "copy_stack",
        stack_effect: "count + 1 -> 0",
        immediate: "u8 width selector, u8 value count",
        description: "Pop count values and one destination, then store consecutive typed slots.",
        confidence: "high",
        evidence: "target sub_4737C0",
    },
    // 0x10 load_base: Push the current raw frame-memory offset.
    BpOpcodeSpec {
        code: 0x10,
        symbol: "load_base",
        stack_effect: "0 -> 1",
        immediate: "none",
        description: "Push the current raw frame-memory offset without applying a pointer tag.",
        confidence: "high",
        evidence: "native CThread frame helpers",
    },
    // 0x11 store_base: Replace the current raw frame-memory offset.
    BpOpcodeSpec {
        code: 0x11,
        symbol: "store_base",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Pop and validate the new raw frame-memory offset.",
        confidence: "high",
        evidence: "native CThread frame helpers",
    },
    // 0x14 jmp: Unconditional control flow transfer.
    BpOpcodeSpec {
        code: 0x14,
        symbol: "jmp",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Pop a raw BP code offset and transfer control to it.",
        confidence: "high",
        evidence: "native dispatcher",
    },
    // 0x15 jc: Pop target and condition, then apply one of six signed predicates.
    BpOpcodeSpec {
        code: 0x15,
        symbol: "jc",
        stack_effect: "2 -> 0; inline target: 1 -> 0",
        immediate: "u8 predicate selector (0..5); bit 3 adds i16 relative target",
        description: "Pop condition and branch on the selected signed predicate; the target is popped unless bit 3 supplies an opcode-relative i16 target.",
        confidence: "high",
        evidence: "native dispatcher",
    },
    // 0x16 call: Enter a BP subroutine and push a return record.
    BpOpcodeSpec {
        code: 0x16,
        symbol: "call",
        stack_effect: "1 -> call frame",
        immediate: "none",
        description: "Pop a BP subroutine offset, create a return record, and enter the callee.",
        confidence: "high",
        evidence: "native CThread return stack",
    },
    // 0x17 ret: Return from a BP subroutine; with no return record the program ends.
    BpOpcodeSpec {
        code: 0x17,
        symbol: "ret",
        stack_effect: "frame dependent",
        immediate: "none",
        description: "Return from a BP subroutine; with no return record the program ends.",
        confidence: "high",
        evidence: "native CThread return stack",
    },
    // 0x20 add: Wrapping 32 bit integer addition.
    BpOpcodeSpec {
        code: 0x20,
        symbol: "add",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Wrapping 32 bit integer addition.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x21 sub: Wrapping 32 bit integer subtraction.
    BpOpcodeSpec {
        code: 0x21,
        symbol: "sub",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Wrapping 32 bit integer subtraction.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x22 mul: Wrapping 32 bit integer multiplication.
    BpOpcodeSpec {
        code: 0x22,
        symbol: "mul",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Wrapping 32 bit integer multiplication.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x23 div: Signed integer division; target zero divisor behavior must remain target compatible.
    BpOpcodeSpec {
        code: 0x23,
        symbol: "div",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed integer division; target zero divisor behavior must remain target compatible.",
        confidence: "medium",
        evidence: "current VM and call corpus",
    },
    // 0x24 mod: Signed integer remainder.
    BpOpcodeSpec {
        code: 0x24,
        symbol: "mod",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed integer remainder.",
        confidence: "medium",
        evidence: "current VM and call corpus",
    },
    // 0x25 bit_and: Bitwise AND.
    BpOpcodeSpec {
        code: 0x25,
        symbol: "and",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Bitwise AND.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x26 bit_or: Bitwise OR.
    BpOpcodeSpec {
        code: 0x26,
        symbol: "or",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Bitwise OR.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x27 bit_xor: Bitwise XOR.
    BpOpcodeSpec {
        code: 0x27,
        symbol: "xor",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Bitwise XOR.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x28 bit_not: Bitwise complement.
    BpOpcodeSpec {
        code: 0x28,
        symbol: "not",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Bitwise complement.",
        confidence: "high",
        evidence: "native arithmetic handler",
    },
    // 0x29 shift_left: Shift left by the low five bits of the count.
    BpOpcodeSpec {
        code: 0x29,
        symbol: "shl",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Shift left by the low five bits of the count.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x2A shift_right_logical: Logical right shift.
    BpOpcodeSpec {
        code: 0x2A,
        symbol: "shr",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Logical right shift.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x2B shift_right_arithmetic: Arithmetic right shift.
    BpOpcodeSpec {
        code: 0x2B,
        symbol: "sar",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Arithmetic right shift.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x30 equal: Push 1 when operands compare equal, otherwise 0.
    BpOpcodeSpec {
        code: 0x30,
        symbol: "eq",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Push 1 when operands compare equal, otherwise 0.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x31 not_equal: Push 1 when operands differ, otherwise 0.
    BpOpcodeSpec {
        code: 0x31,
        symbol: "neq",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Push 1 when operands differ, otherwise 0.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x32 less_or_equal: Signed less than or equal comparison.
    BpOpcodeSpec {
        code: 0x32,
        symbol: "leq",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed less than or equal comparison.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x33 greater_or_equal: Signed greater than or equal comparison.
    BpOpcodeSpec {
        code: 0x33,
        symbol: "geq",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed greater than or equal comparison.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x34 less_than: Signed less than comparison.
    BpOpcodeSpec {
        code: 0x34,
        symbol: "lt",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed less than comparison.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x35 greater_than: Signed greater than comparison.
    BpOpcodeSpec {
        code: 0x35,
        symbol: "gt",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Signed greater than comparison.",
        confidence: "high",
        evidence: "native comparison handler",
    },
    // 0x38 boolean_and: Logical AND: each operand is tested for nonzero and the result is 0 or 1.
    BpOpcodeSpec {
        code: 0x38,
        symbol: "boolean_and",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Logical AND: each operand is tested for nonzero and the result is 0 or 1.",
        confidence: "high",
        evidence: "target opcode table slot 0x38 and native handler",
    },
    // 0x39 boolean_or: Logical OR: each operand is tested for nonzero and the result is 0 or 1.
    BpOpcodeSpec {
        code: 0x39,
        symbol: "boolean_or",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Logical OR: each operand is tested for nonzero and the result is 0 or 1.",
        confidence: "high",
        evidence: "target opcode table slot 0x39 and native handler",
    },
    // 0x3A boolean_not: Push 1 when the operand is zero, otherwise 0.
    BpOpcodeSpec {
        code: 0x3A,
        symbol: "bool_zero",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Push 1 when the operand is zero, otherwise 0.",
        confidence: "high",
        evidence: "native boolean handler",
    },
    // 0x40 select: Pop condition, true value and false value and push the selected value.
    BpOpcodeSpec {
        code: 0x40,
        symbol: "ternary",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Pop condition, true value and false value and push the selected value.",
        confidence: "medium",
        evidence: "current VM and script call sites",
    },
    // 0x42 mul_div: Compute multiplicand times multiplier divided by divisor using a wide intermediate.
    BpOpcodeSpec {
        code: 0x42,
        symbol: "muldiv",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Compute multiplicand times multiplier divided by divisor using a wide intermediate.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x43 atan2_fixed: Return atan2 angle in 16.16 degree representation.
    BpOpcodeSpec {
        code: 0x43,
        symbol: "atan2",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Return atan2 angle in 16.16 degree representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x44 vec3_length: Return Euclidean length of three integer components.
    BpOpcodeSpec {
        code: 0x44,
        symbol: "vec3_length",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Return Euclidean length of three integer components.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x48 sin_fixed: Sine using a 16.16 degree angle and result scale.
    BpOpcodeSpec {
        code: 0x48,
        symbol: "sin",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Sine using a 16.16 degree angle and result scale.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x49 cos_fixed: Cosine using a 16.16 degree angle and result scale.
    BpOpcodeSpec {
        code: 0x49,
        symbol: "cos",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Cosine using a 16.16 degree angle and result scale.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x50 qword_add: 64 bit addition using the BP operand representation.
    BpOpcodeSpec {
        code: 0x50,
        symbol: "qword_add",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "64 bit addition using the BP operand representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x51 qword_sub: 64 bit subtraction using the BP operand representation.
    BpOpcodeSpec {
        code: 0x51,
        symbol: "qword_sub",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "64 bit subtraction using the BP operand representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x52 qword_mul: 64 bit multiplication using the BP operand representation.
    BpOpcodeSpec {
        code: 0x52,
        symbol: "qword_mul",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "64 bit multiplication using the BP operand representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x53 qword_div: 64 bit division using the BP operand representation.
    BpOpcodeSpec {
        code: 0x53,
        symbol: "qword_div",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "64 bit division using the BP operand representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x54 qword_mod: 64 bit remainder using the BP operand representation.
    BpOpcodeSpec {
        code: 0x54,
        symbol: "qword_mod",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "64 bit remainder using the BP operand representation.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x56 fixed_mul: Multiply two 16.16 fixed-point values.
    BpOpcodeSpec {
        code: 0x56,
        symbol: "fixed_mul",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Multiply two 16.16 fixed-point values.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x57 fixed_div: Divide two 16.16 fixed-point values; a zero divisor gives 0.
    BpOpcodeSpec {
        code: 0x57,
        symbol: "fixed_div",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Divide two 16.16 fixed-point values; a zero divisor gives 0.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x58 vec4_add: Add two four-lane 16.16 vectors into a destination.
    BpOpcodeSpec {
        code: 0x58,
        symbol: "vec4_add",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Add two four-lane 16.16 vectors into a destination.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x59 vec4_sub: Subtract two four-lane 16.16 vectors into a destination.
    BpOpcodeSpec {
        code: 0x59,
        symbol: "vec4_sub",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Subtract two four-lane 16.16 vectors into a destination.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x5A vec4_mul: Multiply two four-lane 16.16 vectors lane by lane.
    BpOpcodeSpec {
        code: 0x5A,
        symbol: "vec4_mul",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Multiply two four-lane 16.16 vectors lane by lane.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x5B vec4_div: Divide two four-lane 16.16 vectors lane by lane; a zero lane gives 0.
    BpOpcodeSpec {
        code: 0x5B,
        symbol: "vec4_div",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Divide two four-lane 16.16 vectors lane by lane; a zero lane gives 0.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x5D vec3_normalize: Normalize x, y, z of a 16.16 vector and store the length in w.
    BpOpcodeSpec {
        code: 0x5D,
        symbol: "vec3_normalize",
        stack_effect: "2 -> 0",
        immediate: "none",
        description: "Normalize x, y, z of a 16.16 vector and store the length in w.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x5E vec4_scale: Multiply a four-lane 16.16 vector by a 16.16 scalar.
    BpOpcodeSpec {
        code: 0x5E,
        symbol: "vec4_scale",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Multiply a four-lane 16.16 vector by a 16.16 scalar.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x5F vec4_div_scalar: Divide a four-lane 16.16 vector by a 16.16 scalar; zero gives a zero vector.
    BpOpcodeSpec {
        code: 0x5F,
        symbol: "vec4_div_scalar",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Divide a four-lane 16.16 vector by a 16.16 scalar; zero gives a zero vector.",
        confidence: "high",
        evidence: "senmomo BGI.exe interpreter table 0x506300 (sub_4747C0..sub_474B40)",
    },
    // 0x60 memory_copy: Pop destination, source and byte count and copy bytes.
    BpOpcodeSpec {
        code: 0x60,
        symbol: "memcpy",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Pop destination, source and byte count and copy bytes.",
        confidence: "high",
        evidence: "native memory handler",
    },
    // 0x61 memory_clear: Pop address and byte count and fill with zero.
    BpOpcodeSpec {
        code: 0x61,
        symbol: "memclr",
        stack_effect: "2 -> 0",
        immediate: "none",
        description: "Pop address and byte count and fill with zero.",
        confidence: "high",
        evidence: "native memory handler",
    },
    // 0x62 memory_set: Pop address, byte count and fill byte.
    BpOpcodeSpec {
        code: 0x62,
        symbol: "memset",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Pop address, byte count and fill byte.",
        confidence: "high",
        evidence: "native memory handler",
    },
    // 0x63 memory_equal: Compare two fixed size memory ranges and push boolean equality. It is not libc memcmp.
    BpOpcodeSpec {
        code: 0x63,
        symbol: "memory_equal",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Compare two fixed size memory ranges and push boolean equality. It is not libc memcmp.",
        confidence: "high",
        evidence: "target opcode table slot 0x63 and native handler",
    },
    // 0x64 memory_repeat_copy: Repeat one source block into a destination range.
    BpOpcodeSpec {
        code: 0x64,
        symbol: "memrepeat",
        stack_effect: "4 -> 0",
        immediate: "none",
        description: "Repeat one source block into a destination range.",
        confidence: "high",
        evidence: "native handler annotation",
    },
    // 0x65 memory_find_block: Find a fixed byte block and return its offset or failure value.
    BpOpcodeSpec {
        code: 0x65,
        symbol: "memfind",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "Find a fixed byte block and return its offset or failure value.",
        confidence: "high",
        evidence: "native handler annotation",
    },
    // 0x66 string_find_offset: Find a substring and return a byte offset or failure value.
    BpOpcodeSpec {
        code: 0x66,
        symbol: "strfind",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "Find a substring and return a byte offset or failure value.",
        confidence: "high",
        evidence: "native handler annotation",
    },
    // 0x67 string_replace: Replace string content using BP managed buffers.
    BpOpcodeSpec {
        code: 0x67,
        symbol: "strreplace",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "Replace string content using BP managed buffers.",
        confidence: "medium",
        evidence: "current VM and external reference; target details pending",
    },
    // 0x68 string_length: Return byte length of a NUL terminated string.
    BpOpcodeSpec {
        code: 0x68,
        symbol: "strlen",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Return byte length of a NUL terminated string.",
        confidence: "high",
        evidence: "native string handler",
    },
    // 0x69 string_equal: Compare two NUL terminated strings for equality.
    BpOpcodeSpec {
        code: 0x69,
        symbol: "streq",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Compare two NUL terminated strings for equality.",
        confidence: "high",
        evidence: "native string handler",
    },
    // 0x6A string_copy: Copy native string bytes to destination.
    BpOpcodeSpec {
        code: 0x6A,
        symbol: "strcpy",
        stack_effect: "2 -> implementation defined",
        immediate: "none",
        description: "Copy native string bytes to destination.",
        confidence: "high",
        evidence: "target opcode handler annotation",
    },
    // 0x6B string_concat: Concatenate string values into destination.
    BpOpcodeSpec {
        code: 0x6B,
        symbol: "strconcat",
        stack_effect: "3 -> implementation defined",
        immediate: "none",
        description: "Concatenate string values into destination.",
        confidence: "medium",
        evidence: "current VM implementation",
    },
    // 0x6C get_character: Decode one character and produce character value plus cursor information.
    BpOpcodeSpec {
        code: 0x6C,
        symbol: "getchar",
        stack_effect: "1 -> 3",
        immediate: "none",
        description: "Read one Shift-JIS character at the pointer; push the character code, a double-byte flag, then a line-start/delimiter-class flag (table at 0x4E50A4).",
        confidence: "high",
        evidence: "sub_474670, sub_42EA10, sub_42EA30",
    },
    // 0x6D to_lower: Lowercase or normalize a character in place; this opcode has no BP stack output.
    BpOpcodeSpec {
        code: 0x6D,
        symbol: "tolower",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Lowercase an ASCII string in place; double-byte characters are skipped.",
        confidence: "high",
        evidence: "sub_4746C0 -> sub_42EA80",
    },
    // 0x6E quote_string: Escape or quote a string for formatted output.
    BpOpcodeSpec {
        code: 0x6E,
        symbol: "quote_string",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Pops delimiter character, source string, destination; writes delimiter + source + delimiter.",
        confidence: "high",
        evidence: "sub_4746E0 (sprintf with %c%s%c)",
    },
    // 0x6F format_string: Format text into a BP destination buffer.
    BpOpcodeSpec {
        code: 0x6F,
        symbol: "sprintf",
        stack_effect: "2+n -> 0",
        immediate: "format embedded in stack values",
        description: "Pops format and destination, then one value per %d/%c/%x/%X (integer) or %s (pointer); at most 16 conversions, flags/width limited to ' ', '0', '-', '.'; other conversions are fatal errors.",
        confidence: "high",
        evidence: "sub_474E40, sub_474720",
    },
    // 0x70 allocate: Allocate BP heap memory and push an encoded pointer.
    BpOpcodeSpec {
        code: 0x70,
        symbol: "malloc",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Allocate from the thread's virtual heap (CThread vtable slot 1); size must be 1..=0x4000000; pushes offset + 0x0C000000.",
        confidence: "high",
        evidence: "sub_474E70, sub_4978F0, sub_4303A0",
    },
    // 0x71 free_allocation: Release BP heap memory and push success status.
    BpOpcodeSpec {
        code: 0x71,
        symbol: "free",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Free a 0x0C000000-tagged heap pointer through CThread vtable slot 2; a zero result is a fatal error; pushes the result.",
        confidence: "high",
        evidence: "sub_474ED0, sub_4304B0",
    },
    // 0x74 set_memory_mode: Configure BP memory behavior. Exact target fields remain unresolved.
    BpOpcodeSpec {
        code: 0x74,
        symbol: "set_memory_mode",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Select the memory addressing variant of eight handlers (0x09, 0x0A, 0x0C, 0x60, 0x61, 0x62, 0x6A, 0x6F): non-zero installs the handle-based versions sub_48E190..sub_48E450, zero restores the plain ones.",
        confidence: "high",
        evidence: "sub_474F60 -> sub_48E4B0",
    },
    // 0x75 add_memory_boundary: Register an address boundary and return a status or handle.
    BpOpcodeSpec {
        code: 0x75,
        symbol: "addmemboundary",
        stack_effect: "3 -> 1",
        immediate: "none",
        description: "Pops a pointer, an int and an int and registers a 0x110-byte record in the list at dword_5668B8; pushes whether the last int was non-zero.",
        confidence: "high",
        evidence: "sub_474F80 -> sub_48DFE0",
    },
    // 0x77 engine_state: Query or modify an engine state value.
    BpOpcodeSpec {
        code: 0x77,
        symbol: "engine_state",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Read one renderer/debug counter selected by index (0-7, 16, 17) from the display manager object at dword_56674C; any other index yields -1.",
        confidence: "high",
        evidence: "sub_474FC0 -> sub_443180",
    },
    // 0x78 confirm: Native confirmation or diagnostic interaction.
    BpOpcodeSpec {
        code: 0x78,
        symbol: "confirm",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Pops a default-button flag then message text; shows a Yes/No MessageBox (default button No when the flag is zero) and pushes whether Yes was chosen.",
        confidence: "high",
        evidence: "sub_474FF0, sub_46BC80",
    },
    // 0x79 message_box: Native diagnostic or message display.
    BpOpcodeSpec {
        code: 0x79,
        symbol: "message_box",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Pops message text, expands it (sub_464570) and shows an information MessageBox.",
        confidence: "high",
        evidence: "sub_475050",
    },
    // 0x7A show_number: message box with an integer; never fails
    BpOpcodeSpec {
        code: 0x7A,
        symbol: "show_number",
        stack_effect: "1 -> 0",
        immediate: "none",
        description: "Pops an integer and shows \"Number : %d ( $%.8x )\" in an information MessageBox. It never asserts or terminates the script.",
        confidence: "high",
        evidence: "sub_4750A0",
    },
    // 0x7B dump_memory: Native memory diagnostic operation.
    BpOpcodeSpec {
        code: 0x7B,
        symbol: "dumpmem",
        stack_effect: "3 -> 0",
        immediate: "none",
        description: "Pops a byte count (1..=0x400), a data pointer and a title pointer and shows a 16-bytes-per-line hex dump; a count outside the range is a fatal error.",
        confidence: "high",
        evidence: "sub_475130",
    },
    // 0x7C modal_list: Native modal selection operation.
    BpOpcodeSpec {
        code: 0x7C,
        symbol: "modal_list",
        stack_effect: "2 -> 1",
        immediate: "none",
        description: "Pops a template pointer then an owner value and runs a modal dialog (DialogBoxParam); pushes the dialog result.",
        confidence: "medium",
        evidence: "sub_475370 -> sub_468E70",
    },
    // 0x7D resource_transform: Native resource transformation operation.
    BpOpcodeSpec {
        code: 0x7D,
        symbol: "resource_transform",
        stack_effect: "7 -> 0",
        immediate: "none",
        description: "Pops seven values (the fourth is a pointer) and runs the resource transform sub_401B80; size outside 1..=0x400 or the two documented error results are fatal.",
        confidence: "medium",
        evidence: "sub_4753B0 -> sub_401B80",
    },
    // 0x7E clipboard_set: Write text or data to the host clipboard.
    BpOpcodeSpec {
        code: 0x7E,
        symbol: "clipboard_set",
        stack_effect: "1 -> 1",
        immediate: "none",
        description: "Pops a string, places it on the clipboard (sub_4301E0) and pushes the result.",
        confidence: "high",
        evidence: "sub_4754E0 -> sub_4301E0",
    },
    // Legacy 0x7F. ModernFused instead reads a secondary byte and runs the
    // versioned native_buffer table (see parse_bp_program_profile).
    // 1.653 has a two-string formatted diagnostic message instead.
    // 0x7F resource_blend: Native resource blend operation.
    BpOpcodeSpec {
        code: 0x7F,
        symbol: "resource_blend",
        stack_effect: "5 -> 0",
        immediate: "none",
        description: "Pops five registered-resource ids/parameters, looks three of them up in dword_566750 and blends them (sub_40BD60) only when all lookups succeed.",
        confidence: "medium",
        evidence: "sub_475510",
    },
    // 0x80 dispatch_system80: Dispatch through the System80 native handler table.
    BpOpcodeSpec {
        code: 0x80,
        symbol: "sys1",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the System80 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0x81 dispatch_system81: Dispatch through the System81 native handler table.
    BpOpcodeSpec {
        code: 0x81,
        symbol: "sys2",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the System81 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0x90 dispatch_graph90: Dispatch through the Graph90 native handler table.
    BpOpcodeSpec {
        code: 0x90,
        symbol: "grp1",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the Graph90 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0x91 dispatch_graph91: Dispatch through the Graph91 native handler table.
    BpOpcodeSpec {
        code: 0x91,
        symbol: "grp2",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the Graph91 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0x92 dispatch_graph92: Dispatch through the Graph92 native handler table.
    BpOpcodeSpec {
        code: 0x92,
        symbol: "grp3",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the Graph92 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0xA0 dispatch_soundA0: Dispatch through the SoundA0 native handler table.
    BpOpcodeSpec {
        code: 0xA0,
        symbol: "snd1",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the SoundA0 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0xB0 dispatch_userB0: Dispatch through the UserB0 native handler table.
    BpOpcodeSpec {
        code: 0xB0,
        symbol: "usr1",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the UserB0 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0xC0 dispatch_userC0: Dispatch through the UserC0 native handler table.
    BpOpcodeSpec {
        code: 0xC0,
        symbol: "usr2",
        stack_effect: "ABI table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch through the UserC0 native handler table.",
        confidence: "high",
        evidence: "target dispatch table",
    },
    // 0xD0 legacy_3d_dispatch: Dispatch a legacy 3D extension opcode.
    BpOpcodeSpec {
        code: 0xD0,
        symbol: "legacy_3d",
        stack_effect: "table controlled",
        immediate: "u8 secondary id",
        description: "Dispatch a legacy 3D extension opcode.",
        confidence: "medium",
        evidence: "current VM ABI table",
    },
    // 0xE0 debug_inspect: Debug or inspection extension.
    BpOpcodeSpec {
        code: 0xE0,
        symbol: "debug_inspect",
        stack_effect: "implementation defined",
        immediate: "none",
        description: "Debug or inspection extension.",
        confidence: "low",
        evidence: "registered handler only",
    },
    // 0xFF script_extension: Dispatch a script extension call.
    BpOpcodeSpec {
        code: 0xFF,
        symbol: "script_extension",
        stack_effect: "extension controlled",
        immediate: "extension selector",
        description: "Dispatch a script extension call.",
        confidence: "low",
        evidence: "registered handler only",
    },
];

pub fn lookup_bp_opcode(code: u8) -> Option<&'static BpOpcodeSpec> {
    BP_OPCODE_SPECS.iter().find(|spec| spec.code == code)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn table_has_no_duplicate_opcode_values() {
        let mut codes = BP_OPCODE_SPECS
            .iter()
            .map(|spec| spec.code)
            .collect::<Vec<_>>();
        codes.sort_unstable();
        codes.dedup();
        assert_eq!(codes.len(), BP_OPCODE_SPECS.len());
    }

    #[test]
    fn target_boolean_and_memory_compare_names_are_explicit() {
        assert_eq!(lookup_bp_opcode(0x38).unwrap().symbol, "boolean_and");
        assert_eq!(lookup_bp_opcode(0x39).unwrap().symbol, "boolean_or");
        assert_eq!(lookup_bp_opcode(0x63).unwrap().symbol, "memory_equal");
    }
}
