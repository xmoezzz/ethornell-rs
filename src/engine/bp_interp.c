// BP interpreter: scheduler loop, opcode table and the base opcodes.
// Cleaned from src/raw/text_448000.c (0x45C800, 0x473590..0x475530, 0x48CD70).
// Names below are the ones the Rust port uses (crates/ethornell-script/src/vm_opcode.rs).
#include "../include/engine_types.h"

// ---------------------------------------------------------------------------
// Pointer tags (sub_48DDD0): the top 6 bits of a pointer DWORD select a space.
//   0  global memory   dword_566758 + off      (26-bit offset)
//   1  code region     thread->code + off      (push_string 0x05: insn_start + disp | 0x04000000)
//   2  data region     thread->data + off      (push_base_offset 0x04: (data_sp - disp) | 0x08000000)
//   3  thread heap     vtbl[3](thread, off)    (malloc 0x70 returns off | 0x0C000000)
//  >=4 mapped tables   off_503E78[] (resource-backed ranges); unresolved => fatal
#define PTR_TAG(p)  ((p) >> 26)
#define PTR_OFF(p)  ((p) & 0x3FFFFFF)

// ---------------------------------------------------------------------------
// Scheduler (sub_48CD70): one pass over the thread chain.
// Returns 1 when a terminate status (6) was seen, 2 when status 5 was seen.
int scheduler_pass(struct CThread *head)
{
    int terminate = 0, yielded = 0;
    for (struct CThread *t = first_child(head); t; ) {
        int advance = 1;                       // v13
        struct CThread *dead = 0;              // v11
        if (g_exclusive_thread && t != g_exclusive_thread) goto next;   // dword_566898/56689C
        if ((int32_t)t->flags < 0) {           // bit 31: terminated
            if (!thread_try_destroy(t)) dead = t;   // sub_445500
            goto next;
        }
        if (t->flags & 1) {                    // procedure installed
            int r = CThread_tick_procedure(t); // sub_4451F0
            if (r == 0) goto next;             // still waiting
            if (r == -1) { terminate = 1; goto next; }
        }
        if (!terminate) {
            int status = 0;
            for (uint32_t i = 0; !status && i < 0x100000; i++) {
                uint8_t op = CThread_fetch_opcode(t);          // sub_445010
                status = bp_opcode_table[op](t);               // fatal "unknown opcode" if NULL
            }
            switch (status) {
            case 2: advance = 0; break;                        // stay on this thread next pass
            case 3: t = thread_by_id(head, g_switch_target);   // sub_444B90(dword_566894)
                    advance = 0; break;                        // SwitchCoroutine
            case 4: t->flags |= 0x80000000; advance = 0; break;// thread ended
            case 5: yielded = 1; break;
            case 6: terminate = 1; break;
            default: break;                                    // 1 = yield (80:5F): next thread
            }
            if (!advance) goto reap;
        }
    next:
        t = next_child(t);
    reap:
        if (dead) thread_remove(head, dead);                   // sub_444B10
    }
    return terminate ? 1 : (yielded ? 2 : 0);
}

// ---------------------------------------------------------------------------
// Opcode table at 0x506300 (256 slots; unlisted slots are NULL and fatal).
//   index -> handler
static const void *bp_opcode_table_listing[] = {
    [0x00] = sub_473590,   // push_byte
    [0x01] = sub_4735B0,   // push_word
    [0x02] = sub_4735D0,   // push_dword
    [0x04] = sub_4735F0,   // push_base_offset
    [0x05] = sub_473620,   // push_string
    [0x06] = sub_473650,   // push_offset
    [0x08] = sub_473680,   // load
    [0x09] = sub_473710,   // move
    [0x0A] = sub_473750,   // move_arg
    [0x0B] = sub_473790,   // copy_inline
    [0x0C] = sub_4737C0,   // copy_stack
    [0x10] = sub_473880,   // load_base
    [0x11] = sub_4738A0,   // store_base
    [0x14] = sub_473910,   // jmp
    [0x15] = sub_473990,   // jc
    [0x16] = sub_473A70,   // call
    [0x17] = sub_473AC0,   // ret
    [0x20] = sub_473B00,   // add
    [0x21] = sub_473B30,   // sub
    [0x22] = sub_473B60,   // mul
    [0x23] = sub_473B90,   // div
    [0x24] = sub_473BD0,   // mod
    [0x25] = sub_473C10,   // and
    [0x26] = sub_473C40,   // or
    [0x27] = sub_473C70,   // xor
    [0x28] = sub_473CA0,   // not
    [0x29] = sub_473CC0,   // shl
    [0x2A] = sub_473CF0,   // shr
    [0x2B] = sub_473D20,   // sar
    [0x30] = sub_473D50,   // eq
    [0x31] = sub_473D80,   // neq
    [0x32] = sub_473DB0,   // leq
    [0x33] = sub_473DE0,   // geq
    [0x34] = sub_473E10,   // lt
    [0x35] = sub_473E40,   // gt
    [0x38] = sub_473E70,   // boolean_and
    [0x39] = sub_473EB0,   // boolean_or
    [0x3A] = sub_473F00,   // bool_zero
    [0x40] = sub_473F20,   // ternary
    [0x42] = sub_473F60,   // muldiv
    [0x43] = sub_473FC0,   // atan2
    [0x44] = sub_473FF0,   // vec3_length
    [0x48] = sub_474050,   // sin
    [0x49] = sub_4740A0,   // cos
    [0x50] = sub_4740F0,   // qword_add
    [0x51] = sub_474130,   // qword_sub
    [0x52] = sub_474170,   // qword_mul
    [0x53] = sub_4741C0,   // qword_div
    [0x54] = sub_474210,   // qword_mod
    [0x60] = sub_474260,   // memcpy
    [0x61] = sub_4742A0,   // memclr
    [0x62] = sub_4742D0,   // memset
    [0x63] = sub_474300,   // memory_equal
    [0x64] = sub_4743A0,   // memrepeat
    [0x65] = sub_474400,   // memfind
    [0x66] = sub_4744D0,   // strfind
    [0x67] = sub_474520,   // strreplace
    [0x68] = sub_474570,   // strlen
    [0x69] = sub_4745A0,   // streq
    [0x6A] = sub_474600,   // strcpy
    [0x6B] = sub_474630,   // strconcat
    [0x6C] = sub_474670,   // getchar
    [0x6D] = sub_4746C0,   // tolower
    [0x6E] = sub_4746E0,   // quote_string
    [0x6F] = sub_474E40,   // sprintf
    [0x70] = sub_474E70,   // malloc
    [0x71] = sub_474ED0,   // free
    [0x74] = sub_474F60,   // set_memory_mode
    [0x75] = sub_474F80,   // addmemboundary
    [0x77] = sub_474FC0,   // engine_state
    [0x78] = sub_474FF0,   // confirm
    [0x79] = sub_475050,   // message_box
    [0x7A] = sub_4750A0,   // show_number
    [0x7B] = sub_475130,   // dumpmem
    [0x7C] = sub_475370,   // modal_list
    [0x7D] = sub_4753B0,   // resource_transform
    [0x7E] = sub_4754E0,   // clipboard_set
    [0x7F] = sub_475510,   // resource_blend
    [0x80] = sub_48B2C0,   // sys1
    [0x81] = sub_48C920,   // sys2
    [0x90] = sub_480610,   // grp1
    [0x91] = sub_485430,   // grp2
    [0x92] = sub_486FA0,   // grp3
    [0xA0] = sub_487CB0,   // snd1
    [0xB0] = sub_479330,   // usr1
    [0xC0] = sub_476F30,   // usr2
    [0xD0] = sub_477FC0,   // legacy_3d
    [0xE0] = sub_45C800,   // debug_inspect
    [0xFF] = sub_498B90,   // script_extension
};

// ---------------------------------------------------------------------------
// Base opcodes. `pop`/`push` are CThread_pop/CThread_push (ring buffer).

int op_push_byte(struct CThread *t)  { CThread_push(t, (int8_t)CThread_fetch_u8(t));  return 0; }  // 473590
int op_push_word(struct CThread *t)  { CThread_push(t, (int16_t)CThread_fetch_u16(t)); return 0; } // 4735B0
int op_push_dword(struct CThread *t) { CThread_push(t, CThread_fetch_u32(t));         return 0; }  // 4735D0

// 4735F0 sub_445040 + sub_444FF0: frame-relative pointer, tag 2 (0x08000000).
int op_push_base_offset(struct CThread *t) {
    uint16_t disp = CThread_fetch_u16(t);
    CThread_push(t, (t->data_sp - disp) | 0x08000000);
    return 0;
}
// 473620: code-relative pointer to inline string, tag 1 (0x04000000).
int op_push_string(struct CThread *t) {
    int16_t disp = (int16_t)CThread_fetch_u16(t);
    CThread_push(t, (t->insn_start + disp) | 0x04000000);
    return 0;
}
// 473650: plain code offset (function pointer / label).
int op_push_offset(struct CThread *t) {
    int16_t disp = (int16_t)CThread_fetch_u16(t);
    CThread_push(t, t->insn_start + disp);
    return 0;
}
// 473680: width selector 0=i8 1=i16 2=i32 (sign-extended); other => stale value.
int op_load(struct CThread *t) {
    int32_t *p = (int32_t *)resolve_ptr(CThread_pop(t), t);
    switch (CThread_fetch_u8(t)) {
        case 0: CThread_push(t, *(int8_t *)p);  break;
        case 1: CThread_push(t, *(int16_t *)p); break;
        case 2: CThread_push(t, *p);            break;
        default: CThread_push(t, /*uninitialised*/ 0);
    }
    return 0;
}
// 4736F0 store helper; selector 0=byte 1=word 2=dword.
// 473710 move: value = pop; ptr = pop; store; push value back.
// 473750 move_arg: ptr = pop; value = pop; store (nothing pushed).
// 473790 copy_inline: ptr = pop; n = fetch_u8; copy n bytes from the code stream.
// 4737C0 copy_stack: width = fetch_u8; n = fetch_u8; pop n values; ptr = pop;
//        store them in reverse order with stride 1/2/4.
// 473880 load_base: push data_sp.
// 4738A0 store_base: sp = pop; sp >= data_limit => fatal "SP"; data_sp = sp (no clearing).
// 473910 jmp: ip = pop; ip == 0 => fatal; ip >= code_limit => fatal "IP"; CThread_jump.
// 473990 jc:  dest = pop (unsigned); v = pop (signed); mode = fetch_u8:
//        0 v!=0, 1 v==0, 2 v>0, 3 v>=0, 4 v<=0, 5 v<0; other modes read an
//        uninitialised flag. Taken => range check like jmp, then CThread_jump.
// 473A70 call: data_sp >= data_limit => fatal; CThread_frame_push(t);
//        data_push(t, insn_start + 1); then jmp semantics (473910).
// 473AC0 ret:  data_sp <= data_reserved_low(+0x40) => return 4 (thread ends);
//        else CThread_jump(t, data_pop(t)); CThread_frame_pop_discard(t); return 0.
// Arithmetic 473B00..473D20: b = pop; a = pop; push(a OP b) in 32-bit wrapping
//        arithmetic. div/mod by zero push -1 (473B90/473BD0); shifts use the
//        x86 count mask; 2A shr is logical, 2B sar is arithmetic.
// Comparison 473D50..473E40: b = pop; a = pop; push(a CMP b) signed.
// 473E70 boolean_and, 473EB0 boolean_or, 473F00 bool_zero, 473F20 ternary
//        (else = pop; then = pop; cond = pop; push(cond ? then : else)).
// 473F60 muldiv: c = pop; b = pop; a = pop; push((int64)a*b / c).
// 473FC0 atan2 (sub_401000): b = pop; a = pop.   473FF0 vec3_length: sqrt(x*x+y*y+z*z).
// 474050 sin / 4740A0 cos: push((int)(sin(a * pi / 11796480.0) * 65536.0)).
// 4740F0..474210 qword add/sub/mul/div/mod on three pointers (src2, src1, dst).
// 474260 memcpy(size, src, dst)  474260 memclr(size, ptr)  4742D0 memset(value, size, ptr)
// 474300 memory_equal  4743A0 memrepeat  474400 memfind  4744D0 strfind (strstr)
// 474520 strreplace: pops replacement, needle, haystack, dst; pushes the count.
// 474570 strlen 4745A0 streq 474600 strcpy 474630 strconcat 474670 getchar
// 4746C0 tolower 4746E0 quote_string 474E40 sprintf (<=16 conversions)
// 474E70 malloc 474ED0 free 474F60 set_memory_mode 474F80 addmemboundary
// 474FC0 engine_state 474FF0 confirm 475050 message_box 4750A0 show_number
// 475130 dumpmem 475370 modal_list 4753B0 resource_transform 4754E0 clipboard_set
// 475510 resource_blend
