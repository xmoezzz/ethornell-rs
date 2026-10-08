// CThread: the BP virtual machine's thread object.
// Hand-cleaned from src/raw/text_444000.c (0x444000..0x445500).
// Behaviour is identical to the Hex-Rays output; only names and field access
// changed. Allocation (sub_4302C0 etc.) is kept abstract.
#include "../include/engine_types.h"

extern uint32_t g_next_thread_id;          // dword_565D60
extern uint32_t now_ms(void);              // sub_498720 (timeGetTime wrapper)

#define OFFSET_ERR_CODE_FULL  0x80000002u  // -2147483646 from sub_445340
#define OFFSET_ERR_DATA_FULL  0x80000003u  // -2147483645
#define NO_MODULE             0x80000001u  // -2147483647 from sub_444D80
#define LOAD_FAILED           0x80000000u  // checked by sub_488C00 after sub_444CE0

// sub_4447C0 -- constructor. `parent` is a4; sizes: operand slots, code bytes,
// data bytes; `with_aux` selects the 0x20-byte helper object.
struct CThread *CThread_init(struct CThread *t, uint32_t slots, struct CThread *parent,
                             uint32_t code_bytes, uint32_t data_bytes, int with_aux)
{
    t->root = parent;
    t->thread_id = g_next_thread_id++;
    t->vtbl = CThread_vftable;
    t->next_child = 0;
    if (slots) {
        t->stack_slots = slots;
        t->stack_alloc = new_allocator();          // sub_430260
        t->stack = allocator_alloc(t->stack_alloc, 4 * slots); // sub_4302C0
    } else {
        t->stack_slots = 0; t->stack_alloc = 0; t->stack = 0;
    }
    t->code_reserved_low = 0;
    if (code_bytes) {
        t->code_size = code_bytes;
        t->code_free_limit = code_bytes;
        t->code_alloc = new_allocator();
        t->code = allocator_alloc(t->code_alloc, code_bytes);
    } else {
        t->code_size = 0; t->code_free_limit = 0; t->code_alloc = 0; t->code = 0;
    }
    t->modules = 0; t->module_count = 0; t->code_used = 0;
    t->data_reserved_low = 0;
    if (data_bytes) {
        t->data_size = data_bytes;
        t->data_free_limit = data_bytes;
        t->data_alloc = new_allocator();
        t->data = allocator_alloc(t->data_alloc, data_bytes);
    } else {
        t->data_size = 0; t->data_free_limit = 0; t->data_alloc = 0; t->data = 0;
    }
    t->aux = with_aux ? new_aux_0x20() : 0;        // sub_430310
    t->frames = 0; t->procedure = 0;
    t->callbacks = 0; t->reservation_count = 0;
    t->code_reservations = 0; t->data_reservations = 0;
    t->flags = 0; t->operand_sp = 0; t->insn_start = 0; t->ip = 0;
    t->data_sp = 0; t->deadline = 0;
    return t;
}

// ---------------------------------------------------------------- stacks ---

// sub_4450B0 -- pop one DWORD. The ring wraps: popping at index 0 reads the
// last slot, so an underflow never faults and returns stale data.
uint32_t CThread_pop(struct CThread *t)
{
    uint32_t i = t->operand_sp ? t->operand_sp : t->stack_slots;
    t->operand_sp = i - 1;
    return t->stack[i - 1];
}

// sub_4450D0 -- push one DWORD, wrapping to 0 at the capacity.
void CThread_push(struct CThread *t, uint32_t v)
{
    t->stack[t->operand_sp] = v;
    t->operand_sp = (t->operand_sp + 1 < t->stack_slots) ? t->operand_sp + 1 : 0;
}

// sub_445110 / sub_4450F0 -- BP frame stack in the data region (no bounds check).
void CThread_data_push(struct CThread *t, uint32_t v)
{
    *(uint32_t *)(t->data + t->data_sp) = v;
    t->data_sp += 4;
}
uint32_t CThread_data_pop(struct CThread *t)
{
    t->data_sp -= 4;
    return *(uint32_t *)(t->data + t->data_sp);
}

// sub_445130 / sub_445150 / sub_445190 -- saved-ip list used by call/ret.
void CThread_frame_push(struct CThread *t)       // pushes insn_start (+0x78)
{
    struct FrameLink *n = new_node8();
    n->value = t->insn_start;
    n->next = t->frames;
    t->frames = n;
}
int CThread_frame_pop_discard(struct CThread *t) // returns whether one existed
{
    struct FrameLink *n = t->frames;
    if (!n) return 0;
    t->frames = n->next;
    free(n);
    return 1;
}
int CThread_frame_copy(struct CThread *t, uint32_t *out) // count; out may be NULL
{
    int n = 0;
    for (struct FrameLink *p = t->frames; p; p = p->next, n++)
        if (out) out[n] = p->value;
    return n;
}

// -------------------------------------------------------------- fetching ---

uint8_t  CThread_fetch_opcode(struct CThread *t)   // sub_445010
{
    t->insn_start = t->ip;
    t->ip += 1;
    return t->code[t->insn_start];
}
uint8_t  CThread_fetch_u8 (struct CThread *t) { uint8_t  v = t->code[t->ip]; t->ip += 1; return v; } // 445030
uint16_t CThread_fetch_u16(struct CThread *t) { uint16_t v = *(uint16_t *)(t->code + t->ip); t->ip += 2; return v; } // 445040
uint32_t CThread_fetch_u32(struct CThread *t) { uint32_t v = *(uint32_t *)(t->code + t->ip); t->ip += 4; return v; } // 445060
int CThread_fetch_bytes(struct CThread *t, void *dst, uint32_t n) // sub_445070
{
    if (t->ip + n > CThread_code_limit(t)) return 0;
    memcpy(dst, t->code + t->ip, n);
    t->ip += n;
    return 1;
}
void CThread_jump(struct CThread *t, uint32_t target)  // sub_444FE0
{
    t->insn_start = target;
    t->ip = target;
}

// ---------------------------------------------------------------- limits ---

uint32_t CThread_code_limit(struct CThread *t) { return t->code_reserved_low + t->code_free_limit; } // 444C30
uint32_t CThread_data_limit(struct CThread *t) { return t->data_reserved_low + t->data_free_limit; } // 444C40

// --------------------------------------------------------------- modules ---

// sub_444CE0 -- append a decoded BP image (`image[0]` = byte offset of the
// code, `image[1]` = code size). Returns the offset it was placed at.
uint32_t CThread_append_module(struct CThread *t, const uint32_t *image, const char *name)
{
    if (image[1] + t->code_used > CThread_code_limit(t))
        return LOAD_FAILED;      // (decompiler shows an uninitialised EDX; the
                                 //  caller tests for 0x80000000)
    struct Module *m = malloc(sizeof *m);
    m->name = strdup(name);
    m->size = image[1];
    m->offset = t->code_used;
    m->next = t->modules;
    t->modules = m;
    memcpy(t->code + m->offset, (const char *)image + image[0], image[1]);
    t->code_used += image[1];
    t->module_count++;
    return m->offset;
}

// sub_444D80 -- drop the newest module; returns the remaining count.
uint32_t CThread_free_last_module(struct CThread *t)
{
    struct Module *m = t->modules;
    if (!m) return NO_MODULE;
    t->modules = m->next;
    t->code_used -= m->size;
    t->module_count--;
    free(m->name); free(m);
    return t->module_count;
}

// ----------------------------------------------------------- reservations ---

// sub_445340 -- carve a frame block of `code_bytes`/`data_bytes` from the top of
// both regions for owner `tag`. Outputs the block offsets.
uint32_t CThread_reserve(struct CThread *t, uint32_t *code_out, uint32_t *data_out,
                         uint32_t tag, uint32_t code_bytes, uint32_t data_bytes)
{
    struct Reservation **cp = &t->code_reservations, *c = *cp;
    uint32_t code_top = t->code_size;
    while (c) {
        if (code_bytes + c->start + c->size <= code_top) break;
        cp = &c->next; code_top = c->start; c = c->next; // see note below
    }
    struct Reservation **dp = &t->data_reservations, *d = *dp;
    uint32_t data_top = t->data_size;
    while (d) {
        if (data_bytes + d->start + d->size <= data_top) break;
        dp = &d->next; data_top = d->start; d = d->next;
    }
    int32_t code_start = (int32_t)(code_top - code_bytes);
    // Compared against code_used (this[14]), not code_reserved_low (this[8]).
    if (code_start < (int32_t)t->code_used)        return OFFSET_ERR_CODE_FULL;
    if (data_top < data_bytes)                      return OFFSET_ERR_DATA_FULL;
    t->reservation_count++;
    struct Reservation *nc = malloc(sizeof *nc);
    nc->tag = tag; nc->start = code_start; nc->size = code_bytes; nc->next = c;
    *cp = nc;
    *code_out = nc->start;
    uint32_t low = t->code_size;
    for (struct Reservation *p = t->code_reservations; p; p = p->next)
        if (p->start < low) low = p->start;
    t->code_free_limit = low;
    struct Reservation *nd = malloc(sizeof *nd);
    nd->tag = tag; nd->start = data_top - data_bytes; nd->size = data_bytes; nd->next = d;
    *dp = nd;
    *data_out = nd->start;
    low = t->data_size;
    for (struct Reservation *p = t->data_reservations; p; p = p->next)
        if (p->start < low) low = p->start;
    t->data_free_limit = low;
    return 0;
}

// sub_445480 -- release every reservation with this tag.
int CThread_release(struct CThread *t, uint32_t tag)
{
    int found = 0;
    for (struct Reservation **pp = &t->code_reservations; *pp; pp = &(*pp)->next)
        if ((*pp)->tag == tag) { struct Reservation *r = *pp; *pp = r->next; free(r); found = 1; break; }
    for (struct Reservation **pp = &t->data_reservations; *pp; pp = &(*pp)->next)
        if ((*pp)->tag == tag) { struct Reservation *r = *pp; *pp = r->next; free(r); t->reservation_count--; return 1; }
    if (found) t->reservation_count--;
    return found;
}

// ------------------------------------------------------------- procedure ---

// sub_4451C0 -- install `p`, destroying the previous one; sets flag bit 0.
void CThread_set_procedure(struct CThread *t, struct CProcedure *p)
{
    if (t->procedure) destroy(t->procedure);
    t->procedure = p;
    t->flags |= 1;
}
// sub_4451F0 -- tick the installed procedure (vtable slot 1). A result of 1 or
// -1 uninstalls it and clears flag bit 0. Returns -1 when none is installed.
int CThread_tick_procedure(struct CThread *t)
{
    if (!t->procedure) return -1;
    int r = t->procedure->vtbl[1](t->procedure);
    if (r == 1 || r == -1) {
        destroy(t->procedure);
        t->procedure = 0;
        t->flags &= ~1u;
    }
    return r;
}

// ---------------------------------------------------------------- timing ---

int32_t CThread_wait_remaining(struct CThread *t)       // sub_445260: max(0, deadline-now)
{
    int32_t d = (int32_t)(t->deadline - now_ms());
    return d <= 0 ? 0 : d;
}
void CThread_set_deadline(struct CThread *t, int32_t ms) { t->deadline = ms + now_ms(); } // 445290
void CThread_extend_deadline(struct CThread *t, int32_t ms) { t->deadline += ms; }        // 4452B0

// -------------------------------------------------------------- callbacks ---

void CThread_callback_enqueue(struct CThread *t, uint32_t v) // sub_4452C0 (append at tail)
{
    struct FrameLink **pp = &t->callbacks;
    while (*pp) pp = &(*pp)->next;
    struct FrameLink *n = new_node8(); n->value = v; n->next = 0; *pp = n;
}
int CThread_callback_dequeue(struct CThread *t, uint32_t *out) // sub_445300
{
    struct FrameLink *n = t->callbacks;
    if (!n) return 0;
    *out = n->value; t->callbacks = n->next; free(n);
    return 1;
}

// ---------------------------------------------------- Sys80:44 threads ---

// sub_488D00 -> sub_48D080. Script order: archive, file, slots, code bytes,
// data bytes. Pushes the new thread id.
int Sys80_44_LoadProgramThread(struct CThread *t)
{
    uint32_t data_bytes = pop(t), code_bytes = pop(t), slots = pop(t);
    const char *file = pop_ptr(t), *archive = pop_ptr(t);
    uint8_t *image = new_bytes(0x20000);
    struct CThread *child, *last;
    if (!read_resource(file, image, archive))            // sub_465AB0
        script_error("cannot read program", t);          // byte_4EB960
    for (last = t; last->next_child; last = last->next_child)
        ;                                                // sub_444A70: append
    child = CThread_init(new_thread(), slots, CThread_root(last), code_bytes, data_bytes, 1);
    last->next_child = child;
    if (CThread_load_module(child, image, file) == LOAD_FAILED)  // sub_444CE0
        script_error("program does not fit", t);         // byte_4EBF48
    delete_bytes(image);
    push(t, child->thread_id);                           // sub_42D560 = +8
    return 0;
}

