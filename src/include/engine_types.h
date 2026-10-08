// Structures recovered by hand from field offsets in the Hex-Rays output.
// Every field names the raw offset it came from and the functions that prove
// it. Offsets are bytes from the object start (32-bit target, MSVC thiscall).
#pragma once
#include "ida_prelude.h"

struct CThread;
struct Region;
struct Module;
struct Reservation;
struct FrameLink;
struct CallbackNode;
struct CProcedure;

// operator new(0x10) object created by sub_430260 and handed to a region as
// its allocator (vtable at +0; destroyed with `(**p)(p, 1)`).
typedef struct Allocator Allocator;

// Loaded BP image record, 0x10 bytes (sub_444CE0 / sub_444D80).
struct Module {
    char *name;            // +0   NUL-terminated copy of the file name
    uint32_t size;         // +4   code bytes of the image
    uint32_t offset;       // +8   code_used value when it was appended
    struct Module *next;   // +12  older module (singly linked, newest first)
};

// Region reservation record, 0x10 bytes (sub_445340 / sub_445480).
struct Reservation {
    uint32_t tag;          // +0   owner tag (a4 of sub_445340)
    uint32_t start;        // +4   offset of the reserved block inside the region
    uint32_t size;         // +8   reserved bytes
    struct Reservation *next; // +12 list is kept sorted from the region top downwards
};

// 8-byte nodes: call-frame record and callback record share this shape.
struct FrameLink { uint32_t value; struct FrameLink *next; };

// CThread, 0x88 bytes, vtable &CThread::`vftable' (constructor sub_4447C0).
struct CThread {
    void **vtbl;                 // +0x00
    struct CThread *root;        // +0x04  a4 of the constructor: first thread of the chain
                                 //        (sub_444A60 returns this when non-null, else `this`)
    uint32_t thread_id;          // +0x08  post-incremented global dword_565D60
    struct CThread *next_child;  // +0x0C  singly linked child chain (sub_444A70/B10/B60/BD0)

    // Operand stack: ring buffer of DWORDs (sub_4450B0 pop, sub_4450D0 push).
    uint32_t stack_slots;        // +0x10  capacity in DWORDs (constructor argument a1)
    Allocator *stack_alloc;      // +0x14
    uint32_t *stack;             // +0x18  4 * stack_slots bytes
    // +0x74 operand_sp lives below.

    // Code region: BP images are appended here (sub_444CE0).
    uint32_t code_size;          // +0x1C  constructor a5
    uint32_t code_reserved_low;  // +0x20  starts 0; sub_444C30 = +0x20 + +0x24 is the
                                 //        append limit
    uint32_t code_free_limit;    // +0x24  starts = code_size; lowered by reservations
    Allocator *code_alloc;       // +0x28
    uint8_t *code;               // +0x2C  base pointer of the code region

    struct Module *modules;      // +0x30  newest module record
    uint32_t module_count;       // +0x34
    uint32_t code_used;          // +0x38  bytes appended so far (next module offset)

    // Data region: BP frame/variable memory (sub_445110/sub_4450F0 use +0x4C).
    uint32_t data_size;          // +0x3C  constructor a6
    uint32_t data_reserved_low;  // +0x40  starts 0; sub_444C40 = +0x40 + +0x44
    uint32_t data_free_limit;    // +0x44
    Allocator *data_alloc;       // +0x48
    uint8_t *data;               // +0x4C  base pointer of the data region

    void *aux;                   // +0x50  operator new(0x20) object (sub_430310) or NULL
    struct FrameLink *frames;    // +0x54  saved return-address stack (sub_445130/445150)
    struct CProcedure *procedure;// +0x58  current_procedure (sub_4451C0/4451F0/445230)
    uint32_t reserved_5c;        // +0x5C  key slot of the callback-queue pseudo node
    struct FrameLink *callbacks; // +0x60  callback queue head (sub_4452C0 append, 445300 pop)
    uint32_t reservation_count;  // +0x64  incremented with each sub_445340 success
    struct Reservation *code_reservations; // +0x68
    struct Reservation *data_reservations; // +0x6C
    uint32_t flags;              // +0x70  bit0 = procedure installed (sub_444FA0/FB0/FC0)
    uint32_t operand_sp;         // +0x74  ring index (pop: if 0 then =cap; sp-1)
    uint32_t insn_start;         // +0x78  ip of the opcode byte just fetched (sub_445010)
    uint32_t ip;                 // +0x7C  cursor into code (sub_445010..445070)
    uint32_t data_sp;            // +0x80  byte offset into data region (sub_445110/4450F0)
    uint32_t deadline;           // +0x84  absolute time for wait helpers (sub_445260..4452B0)
};

// ------------------------------------------------------------ graphics ---

// Bitmap descriptor, 0x18 bytes (sub_409030 fills it, sub_407F20 copies it
// out of the bitmap library by handle).
struct BitmapDesc {
    uint8_t *bits;         // +0x00
    int32_t pitch;         // +0x04  bytes per row (width * bpp)
    int32_t width;         // +0x08
    int32_t height;        // +0x0C
    int32_t format;        // +0x10  0 555, 1 XRGB, 2 ARGB, 3 8-bit, 4/6 maps
    int32_t bpp;           // +0x14  sub_407B30(format)
};

// Inclusive rectangle used by sub_409110 (intersect), sub_409170 (offset by
// (dx in edx, dy in ecx)), sub_409190 (descriptor -> 0,0,w-1,h-1).
struct Rect { int32_t left, top, right, bottom; };

// CDspObj member record, 0x10 bytes, list at CDspObj+0x12C (sub_41AB40
// appends, sub_41B1D0 walks, sub_41C130 rewrites the offset).
struct MemberLink {
    struct CDspObj *obj;   // +0
    int32_t dx;            // +4  offset from the parent's +0x30
    int32_t dy;            // +8  offset from the parent's +0x34
    struct MemberLink *next; // +12
};

// The parts of CDspObj (constructor sub_41A400) used by the cleaned code.
struct CDspObj {
    void **vtbl;           // +0x00 (+40 SetPosition(x,y,relink,propagate),
                           //        +44 SetPosition(x,y), +48 GetPosition)
    int32_t x, y;          // +0x30 / +0x34 (vtbl index 12/13 as dwords)
    struct CDspObj *parent;// +0x11C (dword 71)
    struct MemberLink *members; // +0x12C (dword 75)
};

// CDspObjSprite fields (dword indices as Hex-Rays prints them).
//   [77]  +0x134 mode: 0 plain, 1 transition, 2 rotated, 5 perspective, 6 quad
//   [84]  +0x150 primary bitmap      [85] +0x154 secondary bitmap or -1
//   [86]  +0x158 primary generation  [87] +0x15C secondary generation
//   [136] +0x220 two-bitmap cache descriptor (sub_428E70 / sub_40C0F0)
//   [142] +0x238 transition the cache was built for
//   [144] +0x240 transition value    [145] +0x244 secondary parameter
//   [146]/[147] +0x248/+0x24C fixed x/y   [148] +0x250 rotation
//   [158] +0x278 perspective  [159] +0x27C project position  [160] +0x280
//   [167] +0x29C projected x scale (mip level selector, sub_42AA20)
//   [181]/[182] +0x2D4/+0x2D8 projected raster size (sub_4291E0/sub_429220)

// Text shadow style, 0x14 bytes (sub_434E30).
struct ShadowStyle {
    uint32_t mode;         // 0 none, 1 drop shadow, 2 glow
    uint32_t x_percent;    // <= 100: offset/radius = size * x% / 100 (>= 1)
    uint32_t y_percent;    // <= 100
    uint32_t color;        // 0x00RRGGBB
    uint32_t concentration;// <= 256: blended with parameter 256 - value
};

// Font-name registry node, 0x0C bytes, list dword_56631C, next id
// dword_566314 (sub_468A70 interns, sub_468BB0 looks up by id).
struct FontName { int32_t id; char *name; struct FontName *next; };
