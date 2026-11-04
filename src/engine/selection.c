// BGI 1.622 CProcSelectItem{,Ex,ExBlink} / CProcSelectIcon{,Ex}.
// Evidence: raw/bgi1622/native_all.{c,asm}, virtual tables 4E590C..4E59BC.
// Selection is a per-CThread procedure, not a global "last input" result.
#include <stdint.h>

typedef struct RectI { int left, top, right, bottom; } RectI;
typedef struct SelectItem {
    // +00..+1F CProcedure; +20 owning Window; +24 input; +28 cursor moved.
    int window, input, moved;
    int current;                // +2C
    int count, previous_hover, columns; // +30/+34/+38
    char *text[16];             // +3C
    RectI rect[16];             // +7C
    int centered, color;        // +17C/+180
    int allow_cancel;           // +184
    int initialized, blink, pointer_eligible; // +188/+18C/+190
    int overlay_present[2];     // Ex +194/+198
    int overlay_offset[4];      // +19C..+1A8
    int confirmation_ticks;     // ExBlink +1AC
} SelectItem;

// +20 window; +24 first Virtual item; +28 input mode; +2C input;
// +30/+34 hit-local coordinates; +38 count; +3C 64*16-byte records;
// +43C 64 Virtual pointers; +53C keyboard-chain registered;
// +540 key assignments enabled; +544 64 key descriptors;
// +644 live Virtual count; +648 current item; +64C allow cancel;
// +650 forced-completion latch; +654 pointer eligible;
// Ex +658 64*64-byte extended records.
typedef struct SelectIcon {
    int window, input_mode, input, local_x, local_y, count;
    int current, allow_cancel, forced, pointer_eligible;
    int key[64];
    int record[64][16];
} SelectIcon;

// 491F30: count 1..16, initial 0..count-1, columns <=16 (unsigned).
// columns==0 passes validation and divides by zero in 43BA30; portable
// execution rejects that invalid operation instead of faulting the host.
// Item constructor 43B740 installs pointer+keyboard scope nodes at the
// Window's full sort key, drains one input sample, registers focus, and
// clears Window overlays. Destructor 43B870 removes exactly those nodes.
// 43B9A0 copies strings and 43BA30 draws row-major, storing inclusive text
// rectangles. 42CBB0 is a 16-DWORD PER-ITEM COLOR table, not column anchors.

// 43BF00: priority is number keys, then confirm/cancel, then navigation.
// Return 0=no action, 1=finished, 2=selection changed/navigation handled.
static int item_action(SelectItem *p, int alternate_navigation)
{
    uint32_t bits = p->input;
    if (bits & 0x03FF0000) {
        for (int i = 0; i < 10; ++i)
            if ((bits & (0x10000u << i)) && i < p->count) {
                p->current = i;
                return 1;
            }
        return 0;
    }
    if (bits & 0x303) {
        if (bits & 0x202) { p->current = -1; return 1; }
        if (!(bits & 1)) return 1; // Enter confirms current index
        extern int item_pointer_hit(const SelectItem *);
        int hit = item_pointer_hit(p);
        if (hit >= 0) { p->current = hit; return 1; }
        return 0;
    }
    static const uint32_t normal[] = {0x1000,0x2000,0x4000,0x8000,0};
    static const uint32_t alternate[] = {0x40,0x80,0x1000,0x2000,0x4000,0x8000,0};
    const uint32_t *keys = alternate_navigation ? alternate : normal;
    uint32_t key = 0;
    for (int i=0; keys[i]; ++i) if (bits & keys[i]) { key=keys[i]; break; }
    if (!key) return 0;
    int col = p->current % p->columns, row = p->current / p->columns;
    int rows = (p->count + p->columns - 1) / p->columns;
    int vertical = key==0x40 || key==0x80 || key==0x1000 || key==0x2000;
    int limit = vertical ? rows : p->columns;
    for (int i=0; i<limit; ++i) {
        if (vertical) {
            row += key==0x40 || key==0x1000 ? -1 : 1;
            if (row<0) row=rows-1;
            if (row>=rows) row=0;
        } else {
            col += key==0x4000 ? -1 : 1;
            if (col<0) col=p->columns-1;
            if (col>=p->columns) col=0;
        }
        int next=col+row*p->columns;
        if (next<p->count) {
            if (key==0x40 || key==0x80) {
                extern void item_move_cursor_to_center(const SelectItem *,int);
                item_move_cursor_to_center(p,next);
            } else p->current=next;
            break;
        }
    }
    return 2;
}

// 43BCB0: poll Window scope; base masks input with 03FFF3C3, Ex with
// 03FF0203; !allow_cancel clears 0202. Pointer eligibility requires the
// focus-chain top key and (AF==0 || main Window active). Cursor motion or a
// changed eligibility triggers hover, even with no new input bits.
// 43C580 base: initialize/update highlight, process input, then blink on
// timer; selection changes queue event 10000001, hover changes 10000002.
// 43C9A0 Ex processes selection; A2 completes immediately on confirm.
// A3 installs 43CB00 instead of 43C9A0: it automatically blinks the initial
// selection every 50ms for 20 ticks, without waiting for confirm input.
// On completion push current, then alive ? current : -1 (exactly 2).
// Callback 512 has a null handler for this family and MUST be ignored.

// 43A510: input_mode 0=no Window node; 1/2=Window object pointer node;
// 3=Window full-key plain pointer+keyboard nodes and focus entry.
// Key assignment table (43AB70) adds the keyboard node if absent, samples
// each assigned descriptor, and is independent of input_mode.
// 43A7C0 / 43B290: resolve every primary bitmap or fail; draw at valid
// origin+(x,y); create independent CDspObjVirtual with configure-time extent,
// snapshot its sort key and register a pointer node. Alpha-hit mode copies
// the primary mask (base) or record[15]'s mask (Ex, -1 means primary).
// 43B010: scan Virtual items in source order; first mask hit wins, updates
// +30/+34 local coordinates. The visual overlay does not define this hit.
// 43ADA0: when keyboard eligible, assigned key edge OR action-map class
// selects its item, zeros local coordinates, and completes before pointer.
// 43B110: cancel bits 0202 -> -1; else recompute pointer selection, queue
// 20000001 with low word=index and high word=has-highlight, complete only
// for input bit 1 with a live selected item.
// 43ABF0 callback 512 forces completion; valid index sets local=(0,0),
// invalid index sets current=-1 while retaining old local coordinates.
// 43AC40 completion pushes local_x, local_y, alive ? current : -1 (3 values).
// Destructor unregisters its Window/Virtual nodes, assigned-key node and
// callback; it does not substitute a two-value generic selection result.
