// BGI 1.622 DCIPIcon/DCIPIconEx input chain and action dispatch.
// Evidence: native_all.c/.asm 448070,448AD0,449EA0,44A000,
// 44A310,44A3E0,44A440,44A490,44AD40,46D830,46E460,46E7D0.
// Cleaned control flow; types below describe the recovered ownership model.
#include <stdint.h>

// Configuration registers independent nodes: flags[0] gives the Window
// keyboard/pointer/focus scope, otherwise flags[1] gives an object pointer
// node. Every materialized item owns a Virtual pointer node at window+0x8000.
// Disabling pointer processing skips the processor query, not registration.
// Virtual extents and bit masks are copied at configure time. Positions follow
// the live Virtual child. Draw eligibility and item-selection eligibility are
// separate: excluding the current item does not remove its occluding node.

static const uint32_t action_bits[24] = {
    1,2,4,16,32,64,128,256,512,4096,8192,16384,32768,65536,131072,
    262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,
    0x40000000
};
static const int action_tables[4][24] = {
    {1,2,0,0,0,0,0,3,4,7,8,5,6,0,0,0,0,0,0,0,0,0,0,19},
    {1,2,0,0,0,0,0,3,4,5,6,7,8,0,0,0,0,0,0,0,0,0,0,19},
    {1,2,0,0,0,0,0,3,4,11,12,9,10,0,0,0,0,0,0,0,0,0,0,19},
    {1,2,0,0,0,0,0,3,4,9,10,11,12,0,0,0,0,0,0,0,0,0,0,19}
};

// 46E460 takes independent keyboard and pointer scopes. Item Virtual queries
// use keyboard scope zero, not their pointer sort key.
static uint32_t query_icon_input(int full, int window_input, uint32_t scope,
    uint32_t virtual_scope, uint32_t disabled_bits) {
    uint32_t bits = 0;
    if (window_input) bits |= query_input(full ? scope : 0, scope, 1);
    bits |= query_input(0, virtual_scope, 1);
    if (!full) bits &= 0x2c3;
    return bits & ~disabled_bits;
}

// 448AD0 processes the first asserted bit in this order. Modes 4..7 use the
// four script-assigned 24-entry tables installed by 91:BF.
// Actions: 1 pointer activation, 2 pointer cancel, 3 keyboard confirm,
// 4 keyboard cancel, 5/6 linear wrap, 7/8 previous/next enabled group,
// 9/10 column wrap, 11/12 row wrap, 13/14 linear boundary,
// 15/16 column boundary, 17/18 row boundary; 19 maps Shift+Tab to 7, Tab to 8.
// Compact group count packs columns in its high word; Ex stores columns at
// group+4. Zero or oversized columns means one row. Ragged grid navigation
// skips nonexistent cells. Boundary events pack the OLD item coordinates.
// Compact item+0x38 is a direct key descriptor; its sign bit permits held
// activation after the descriptor's 500 ms deadline. Ex's projected field is
// zero. Descriptor classification is the fixed 46E7D0 table, independent of
// script-remapped raw key assignments.
