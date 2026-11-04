#!/usr/bin/env python3
"""Run BGI 1.622 bitmap kernels under Unicorn and write reference vectors
for the ignored test bitmap_kernels::tests::kernels_match_the_target_vectors.

Usage: python3 -m venv v && v/bin/pip install unicorn pefile
       v/bin/python src/tools/emu_kernels.py OUT_DIR
       ETHORNELL_KERNEL_VECTORS=OUT_DIR cargo test -p ethornell-app kernels_match -- --ignored

Each vector file <kernel>_<n>.bin is little-endian u32 words
[kind, src_w, src_h, dst_w, dst_h, p0, p1, p2] followed by the source
pixels, the initial destination pixels and the destination after the
target ran (B, G, R, A bytes, rows packed).
"""
import os, random, struct, sys
import pefile
from unicorn import *
from unicorn.x86_const import *

EXE = "/Users/xmoe/Documents/千の刃濤、桃花染の皇姫 体験版/senmomo_trial/BGI.exe"
OUT = sys.argv[1]
pe = pefile.PE(EXE)
base = pe.OPTIONAL_HEADER.ImageBase
mu = Uc(UC_ARCH_X86, UC_MODE_32)
size = (pe.OPTIONAL_HEADER.SizeOfImage + 0xFFF) & ~0xFFF
mu.mem_map(base, size)
mu.mem_write(base, pe.get_memory_mapped_image())
STACK, HEAP, RET = 0x10000000, 0x20000000, 0x40000000
mu.mem_map(STACK, 0x100000)
mu.mem_map(HEAP, 0x4000000)
mu.mem_map(RET, 0x1000)

heap_top = HEAP

# A GDT whose entry 1 is a data segment over a fake TEB, loaded into FS:
# the SEH prologues read and write fs:[0].
GDT, TEB = 0x60000000, 0x61000000
mu.mem_map(GDT, 0x1000)
mu.mem_map(TEB, 0x1000)


def descriptor(base, limit, access, flags):
    return struct.pack("<HHBBBB", limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, access,
                       ((limit >> 16) & 0xF) | (flags << 4), (base >> 24) & 0xFF)


mu.mem_write(GDT + 8, descriptor(0, 0xFFFFF, 0x92, 0xC))
mu.mem_write(GDT + 16, descriptor(TEB, 0xFFF, 0x92, 0x4))
mu.mem_write(TEB, struct.pack("<I", 0xFFFFFFFF))
mu.reg_write(UC_X86_REG_GDTR, (0, GDT, 0xFF, 0))
for segment in (UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS):
    mu.reg_write(segment, 1 << 3)
mu.reg_write(UC_X86_REG_FS, 2 << 3)

# Windows imports the kernels call are stubbed: each IAT slot points at a
# hooked address in STUBS.
STUBS = 0x50000000
mu.mem_map(STUBS, 0x2000)
stubs = {}


stub_slots = {}


def stub(name, argc, impl):
    for entry in pe.DIRECTORY_ENTRY_IMPORT:
        for imp in entry.imports:
            if imp.name and imp.name.decode() == name:
                at = stub_slots.setdefault(imp.address, STUBS + 4 * len(stub_slots))
                mu.mem_write(at, b"\xC3")
                mu.mem_write(imp.address, struct.pack("<I", at))
                stubs[at] = (argc, impl)


def on_stub(uc, address, size, user):
    if address in stubs:
        argc, impl = stubs[address]
        sp = uc.reg_read(UC_X86_REG_ESP)
        ret, *args = struct.unpack("<%dI" % (argc + 1), uc.mem_read(sp, 4 * (argc + 1)))
        try:
            value = impl(*args)
        except RuntimeError as error:
            words = struct.unpack("<256I", uc.mem_read(sp, 1024))
            code = [hex(w) for w in words if 0x401000 <= w < 0x4D0000]
            raise RuntimeError("%s (called from %X; stack %s)" % (error, ret, " ".join(code[:16])))
        uc.reg_write(UC_X86_REG_EAX, value & 0xFFFFFFFF)
        uc.reg_write(UC_X86_REG_ESP, sp + 4 * (argc + 1))
        uc.reg_write(UC_X86_REG_EIP, ret)


def set_rect(rect, left, top, right, bottom):
    mu.mem_write(rect, struct.pack("<4I", left, top, right, bottom))
    return 1


# Any other import traps with its name.
def unknown_import(name):
    def trap(*args):
        raise RuntimeError("unstubbed import " + name)
    return trap


for entry in pe.DIRECTORY_ENTRY_IMPORT:
    for imp in entry.imports:
        if imp.name:
            stub(imp.name.decode(), 0, unknown_import(imp.name.decode()))
stub("SetRect", 5, set_rect)
# A C++ throw ends the call: the encoders catch their own throws and fail.
thrown = [False]


def raise_exception(*args):
    thrown[0] = True
    mu.emu_stop()
    return 0


stub("RaiseException", 4, raise_exception)
tick = [0]
stub("GetTickCount", 0, lambda: tick[0])
mu.hook_add(UC_HOOK_CODE, on_stub, begin=STUBS, end=STUBS + 0xFFF)

# Statically linked CRT allocators are replaced by cdecl hooks.
internal = {}


def on_internal(uc, address, size, user):
    if address in internal:
        sp = uc.reg_read(UC_X86_REG_ESP)
        ret, arg = struct.unpack("<2I", uc.mem_read(sp, 8))
        uc.reg_write(UC_X86_REG_EAX, internal[address](arg) & 0xFFFFFFFF)
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EIP, ret)


def zeroed(n):
    # Zero-filled, so that sub_469B70's one-byte over-read sees 0.
    at = alloc(n + 16)
    mu.mem_write(at, bytes(n + 16))
    return at


# operator new / new[], _malloc, _aligned_malloc (16-byte aligned anyway).
for address in (0x4ABF7F, 0x4ABF18, 0x4AF088, 0x4ACB4D):
    internal[address] = zeroed
# operator delete[] / delete, _free, _aligned_free.
for address in (0x4ABF23, 0x4ABF0D, 0x4AEF55, 0x4ACB33):
    internal[address] = lambda p: 0
for address in internal:
    mu.hook_add(UC_HOOK_CODE, on_internal, begin=address, end=address)


def alloc(n):
    global heap_top
    at = heap_top
    heap_top += (n + 0xFFF) & ~0xFFF
    return at


def bitmap(width, height, pixels, fmt, bpp=4):
    """A 24-byte descriptor: pixels, stride, width, height, format, bpp."""
    data = alloc(max(1, width * height * bpp) + 64)
    mu.mem_write(data, bytes(pixels))
    desc = alloc(24)
    mu.mem_write(desc, struct.pack("<6I", data, width * bpp, width, height, fmt, bpp))
    return desc, data


def call(address, stack_args, regs):
    sp = STACK + 0xF0000
    for value in reversed(stack_args):
        sp -= 4
        mu.mem_write(sp, struct.pack("<I", value & 0xFFFFFFFF))
    sp -= 4
    mu.mem_write(sp, struct.pack("<I", RET))
    mu.reg_write(UC_X86_REG_ESP, sp)
    for reg, value in regs.items():
        mu.reg_write(reg, value & 0xFFFFFFFF)
    try:
        mu.emu_start(address, RET)
    except UcError:
        print("fault in %X at eip %X" % (address, mu.reg_read(UC_X86_REG_EIP)), file=sys.stderr)
        raise


rng = random.Random(1622)


def rand_pixels(n, alpha=None):
    out = bytearray()
    for _ in range(n):
        out += bytes(rng.randrange(256) for _ in range(3))
        out.append(rng.randrange(256) if alpha is None else alpha)
    return out


def emit(name, kind, sw, sh, dw, dh, params, src, dst_before, dst_after):
    with open(os.path.join(OUT, name + ".bin"), "wb") as f:
        f.write(struct.pack("<8I", kind, sw, sh, dw, dh, *[p & 0xFFFFFFFF for p in params]))
        f.write(bytes(src) + bytes(dst_before) + bytes(dst_after))


global_heap = heap_top

# kind 1: sub_418F60 (91:1B), ecx = attenuation, edx = destination,
# stack = [source, x factor, y factor].
cases = [(16, 12, 0, 0x8000, 0x8000), (16, 12, 64, 0x4000, 0xC000), (9, 7, 256, 0, 0),
         (20, 20, 128, 0x10000, 0x10000), (13, 5, 32, 0x1234, 0x0FED)]
for _ in range(10):
    cases.append((rng.randrange(2, 24), rng.randrange(2, 24), rng.randrange(257),
                  rng.randrange(0x10001), rng.randrange(0x10001)))
for n, (w, h, att, xf, yf) in enumerate(cases):
    heap_top = global_heap
    src = rand_pixels(w * h, 0)
    dst_before = rand_pixels(w * h)
    src_desc, _ = bitmap(w, h, src, 1)
    dst_desc, dst_data = bitmap(w, h, dst_before, 1)
    call(0x418F60, [src_desc, xf, yf], {UC_X86_REG_ECX: att, UC_X86_REG_EDX: dst_desc})
    after = mu.mem_read(dst_data, w * h * 4)
    emit(f"concentrate_{n}", 1, w, h, w, h, (att, xf, yf), src, dst_before, after)
    print("concentrate", n, w, h, att, hex(xf), hex(yf))

# sub_495640 fills the 16-bit weight table used by sub_495CA0.
call(0x495640, [], {})

# kind 2: sub_495680 (91:1C nearest), cdecl (destination, source, xs, ys).
# kind 3: sub_495880 (91:1C filtered), cdecl (source, xs, ys), ecx = dest.
scale_cases = [(8, 6, 0x10000, 0x10000), (8, 6, 0x18000, 0x20000), (16, 12, 0x8000, 0x8000),
               (16, 12, 0x6000, 0x5000), (10, 10, 0xC000, 0x9000), (12, 9, 0x24000, 0xA000),
               (7, 13, 0x3000, 0x28000), (20, 4, 0x10000, 0x8800)]
for _ in range(12):
    scale_cases.append((rng.randrange(1, 24), rng.randrange(1, 24),
                        rng.randrange(0x1000, 0x40000), rng.randrange(0x1000, 0x40000)))
for kind, address in ((2, 0x495680), (3, 0x495880)):
    for n, (w, h, xs, ys) in enumerate(scale_cases):
        for fmt in (1, 2):
            heap_top = global_heap
            dw, dh = (w * xs) >> 16, (h * ys) >> 16
            if dw == 0 or dh == 0:
                continue
            src = rand_pixels(w * h, None if fmt == 2 else 0)
            if fmt == 2:
                for i in range(0, len(src), 4 * 3):
                    src[i + 3] = 0
            dst_before = rand_pixels(dw * dh)
            src_desc, _ = bitmap(w, h, src, fmt)
            dst_desc, dst_data = bitmap(dw, dh, dst_before, fmt)
            if kind == 2:
                call(address, [dst_desc, src_desc, xs, ys], {})
            else:
                call(address, [src_desc, xs, ys], {UC_X86_REG_ECX: dst_desc})
            after = mu.mem_read(dst_data, dw * dh * 4)
            emit(f"scale{kind}_{n}_{fmt}", kind, w, h, dw, dh, (xs, ys, fmt), src, dst_before, after)
print("scales done")

# kind 4: sub_419CF0 (91:1D), cdecl (destination, source, type, colour,
# alpha, threaded=0). The alpha table qword_50B0F0 is filled like the
# first loop of sub_407760 (entry k = k in the colour lanes, 0 in alpha);
# the rest of that function needs the CRT.
# p2 = type << 16 | destination format << 8 | source format.
for k in range(129):
    mu.mem_write(0x50B0F0 + 16 * k, struct.pack("<8H", k, k, k, 0, k, k, k, 0))
# ... and its other two weight tables: qword_50B900[k] (k = 0..256) and
# dword_50A8D0[k] = 0x100010001 * k.
for k in range(257):
    mu.mem_write(0x50B900 + 16 * k, struct.pack("<8H", k, k, k, 0, k, k, k, 0))
    mu.mem_write(0x50A8D0 + 8 * k, struct.pack("<Q", 0x100010001 * k))
process_cases = []
for kind in range(6):
    for dfmt, sfmt in ((1, 1), (2, 2), (1, 2), (2, 1)):
        for colour, alpha in ((0, 0), (0, 128), (0, 256), (0x00FF8040, 77), (0x12345678, 256),
                              (0x00FFFFFF, 1), (rng.randrange(1 << 32), rng.randrange(257))):
            process_cases.append((kind, dfmt, sfmt, colour, alpha))
for n, (kind, dfmt, sfmt, colour, alpha) in enumerate(process_cases):
    heap_top = global_heap
    sw, sh = rng.randrange(1, 12), rng.randrange(1, 10)
    dw, dh = (sw, sh) if n % 3 else (rng.randrange(1, 12), rng.randrange(1, 10))
    if kind == 0 or (kind == 3 and colour == 0):
        # Kernels that write over the source size need the room.
        dw, dh = max(dw, sw), max(dh, sh)
    src = rand_pixels(sw * sh)
    for i in range(0, len(src), 4 * 3):
        src[i + 3] = 0
    if n % 4 == 0 and len(src) >= 8:
        src[3] = src[7] = 0
    dst_before = rand_pixels(dw * dh)
    src_desc, _ = bitmap(sw, sh, src, sfmt)
    dst_desc, dst_data = bitmap(dw, dh, dst_before, dfmt)
    call(0x419CF0, [dst_desc, src_desc, kind, colour, alpha, 0], {})
    after = mu.mem_read(dst_data, dw * dh * 4)
    emit(f"process_{n:03}", 4, sw, sh, dw, dh, (colour, alpha, kind << 16 | dfmt << 8 | sfmt),
         src, dst_before, after)
print("process done", len(process_cases))

# kind 5: sub_403190 (91:1E), eax = p3, ecx = target handle, stack =
# (source handle, p2). The handle table behind dword_566764 holds 72-byte
# entries: +0 in use, +4 the descriptor, +32 -1 (not a cached bitmap).
# p2 = target format << 8 | mask format; 8-bit bitmaps are 1 byte a pixel.
def handle_table(descs):
    table = alloc(16 + 72 * len(descs))
    entries = table + 16
    mu.mem_write(table, struct.pack("<4I", 0, 0, entries, len(descs)))
    for n, desc in enumerate(descs):
        entry = struct.pack("<I", 1) + bytes(mu.mem_read(desc, 24)) + struct.pack("<3i", 0, -1, 0)
        mu.mem_write(entries + 72 * n, entry)
    mu.mem_write(0x566764, struct.pack("<I", table))


mask_cases = []
for tfmt, mfmt in ((2, 2), (2, 3), (3, 3), (1, 2), (1, 3), (3, 2)):
    for _ in range(8):
        mask_cases.append((tfmt, mfmt))
for n, (tfmt, mfmt) in enumerate(mask_cases):
    heap_top = global_heap
    tw, th, mw, mh = (rng.randrange(1, 14) for _ in range(4))
    dx, dy = rng.randrange(-tw + 1, mw), rng.randrange(-th + 1, mh)
    if n % 8 == 7:
        dx, dy = rng.randrange(-mw - 2, tw + 2), rng.randrange(-mh - 2, th + 2)
    if n % 8 == 0:
        dx = dy = 0
    tb, mb = (1 if f == 3 else 4 for f in (tfmt, mfmt))
    mask = bytearray(rng.randrange(256) for _ in range(mw * mh * mb))
    for i in range(0, len(mask), 5):
        mask[i] = 0 if i % 2 else 255
    before = rand_pixels(tw * th) if tb == 4 else bytearray(rng.randrange(256) for _ in range(tw * th))
    t_desc, t_data = bitmap(tw, th, before, tfmt, tb)
    m_desc, _ = bitmap(mw, mh, mask, mfmt, mb)
    handle_table([t_desc, m_desc])
    call(0x403190, [1, dx], {UC_X86_REG_EAX: dy, UC_X86_REG_ECX: 0})
    after = mu.mem_read(t_data, tw * th * tb)
    emit(f"mask_{n:03}", 5, mw, mh, tw, th, (dx, dy, tfmt << 8 | mfmt), mask, before, after)
print("masks done", len(mask_cases))

# kind 6: sub_416B60 (91:18), cdecl (destination, dx, dy, source, sx, sy,
# angle, x scale, y scale, alpha, filter, threaded=0). The x87 control
# word is set to the Windows default (53-bit precision) first.
# p0 = filter << 16 | destination format << 8 | source format, p1 = alpha,
# p2 = 7 extra words: dx, dy, sx, sy, angle, x scale, y scale.
# (In the stub page: the per-case heap is reused.)
FPU = STUBS + 0x1800
mu.mem_write(FPU, b"\xD9\x2D" + struct.pack("<I", FPU + 8) + b"\xC3" + b"\x00" + struct.pack("<H", 0x27F))


def emit_extra(name, kind, sw, sh, dw, dh, params, extra, src, dst_before, dst_after):
    with open(os.path.join(OUT, name + ".bin"), "wb") as f:
        f.write(struct.pack("<8I", kind, sw, sh, dw, dh, *[p & 0xFFFFFFFF for p in params]))
        f.write(struct.pack("<%dI" % len(extra), *[e & 0xFFFFFFFF for e in extra]))
        f.write(bytes(src) + bytes(dst_before) + bytes(dst_after))


rotate_cases = []
for sfmt in (1, 2):
    for dfmt in (1, 2):
        for filt in (0, 1):
            for alpha in (0, 96, 255, 256):
                for shape in range(3):
                    rotate_cases.append((sfmt, dfmt, filt, alpha, shape))
# More plain blits of format 2 over format 2 (sub_40B330's SSE pairs and
# its scalar odd pixel).
rotate_cases += [(2, 2, 0, 0, 0)] * 24
# kind 7: sub_4179B0 (91:19), same arguments and layout as kind 6.
rotate_cases = [(6, 0x416B60) + case for case in rotate_cases]
rotate_cases += [(7, 0x4179B0, sfmt, dfmt, filt, alpha, shape)
                 for sfmt in (1, 2) for dfmt in (1, 2) for filt in (0, 1)
                 for alpha in (0, 96, 255, 256) for shape in range(3)]
for n, (kind, address, sfmt, dfmt, filt, alpha, shape) in enumerate(rotate_cases):
    heap_top = global_heap
    sw, sh = rng.randrange(1, 16), rng.randrange(1, 16)
    dw, dh = rng.randrange(1, 16), rng.randrange(1, 16)
    if shape == 0:
        # The plain-blit path: whole anchors, unit scale, 360 * k degrees.
        dw, dh = min(dw, sw), min(dh, sh)
        sx, sy = rng.randrange(0, sw - dw + 1) << 16, rng.randrange(0, sh - dh + 1) << 16
        params = (0, 0, sx, sy, 23592960 * rng.randrange(-2, 3), 0x10000, 0x10000)
    elif shape == 1:
        params = (rng.randrange(-4 << 16, 20 << 16), rng.randrange(-4 << 16, 20 << 16),
                  rng.randrange(-4 << 16, 20 << 16), rng.randrange(-4 << 16, 20 << 16),
                  rng.randrange(-400 << 16, 400 << 16), rng.randrange(0x4000, 0x30000),
                  rng.randrange(0x4000, 0x30000))
    else:
        params = (dw << 15, dh << 15, sw << 15, sh << 15, rng.choice((90, 180, -90, 45, 30)) << 16,
                  rng.choice((0x10000, 0x8000, 0x20000)), rng.choice((0x10000, 0x18000)))
    src = rand_pixels(sw * sh)
    for i in range(0, len(src), 4 * 5):
        src[i + 3] = 0
    for i in range(8, len(src), 4 * 7):
        src[i + 3] = 255
    dst_before = rand_pixels(dw * dh)
    for i in range(4, len(dst_before), 4 * 6):
        dst_before[i + 3] = 0
    src_desc, _ = bitmap(sw, sh, src, sfmt)
    dst_desc, dst_data = bitmap(dw, dh, dst_before, dfmt)
    call(FPU, [], {})
    dx, dy, sx, sy, angle, xs, ys = params
    call(address, [dst_desc, dx, dy, src_desc, sx, sy, angle, xs, ys, alpha, filt, 0], {})
    after = mu.mem_read(dst_data, dw * dh * 4)
    emit_extra(f"rotate_{n:03}", kind, sw, sh, dw, dh, (filt << 16 | dfmt << 8 | sfmt, alpha, 7), params,
               src, dst_before, after)
print("rotates done", len(rotate_cases))

# kind 8: sub_4193D0 (90:CD), cdecl (destination, source, mono colour,
# mono level, curve record, film colour, mode, film level, threaded=0).
# The record comes from sub_40A060 (thiscall: ecx = three (x, y) control
# points, stack = id) into the list at dword_566764 + 216.
# p0 = format; extra = the six control values, mono colour, mono level,
# film colour, film level.
tone_cases = []
for fmt in (1, 2):
    for _ in range(24):
        tone_cases.append(fmt)
for n, fmt in enumerate(tone_cases):
    heap_top = global_heap
    w, h = rng.randrange(1, 14), rng.randrange(1, 10)
    points = [v for _ in range(3) for v in (rng.randrange(1, 255), rng.randrange(256))]
    mono_colour, film_colour = rng.randrange(1 << 32), rng.randrange(1 << 32)
    mono_level, film_level = rng.choice((0, 256, rng.randrange(257))), rng.choice((0, 256, rng.randrange(257)))
    manager = alloc(256)
    mu.mem_write(manager, bytes(256))
    mu.mem_write(0x566764, struct.pack("<I", manager))
    control = alloc(24)
    mu.mem_write(control, struct.pack("<6i", *points))
    call(FPU, [], {})
    call(0x40A060, [7], {UC_X86_REG_ECX: control})
    record = struct.unpack("<I", mu.mem_read(manager + 216, 4))[0]
    src = rand_pixels(w * h)
    for i in range(0, len(src), 4 * 4):
        src[i + 3] = 0
    if n % 3 == 0 and w >= 2:
        src[3] = src[7] = 0
    dst_before = rand_pixels(w * h)
    src_desc, _ = bitmap(w, h, src, fmt)
    dst_desc, dst_data = bitmap(w, h, dst_before, fmt)
    call(0x4193D0, [dst_desc, src_desc, mono_colour, mono_level, record, film_colour, 8, film_level, 0], {})
    after = mu.mem_read(dst_data, w * h * 4)
    emit_extra(f"tone_{n:03}", 8, w, h, w, h, (fmt, 0, 10),
               points + [mono_colour, mono_level, film_colour, film_level], src, dst_before, after)
print("tones done", len(tone_cases))

# kind 9: sub_46A320 (90:CE format 0, CompressedBG v1), cdecl (output,
# input = 16-byte descriptor + packed pixels), ecx = output length.
# GetTickCount (the key) is stubbed. sw = input bytes, dw = output bytes,
# both as one-byte rows; p0 = key.
cbg_cases = []
for bpp in (24, 32):
    for _ in range(12):
        cbg_cases.append(bpp)
for n, bpp in enumerate(cbg_cases):
    heap_top = global_heap
    w, h = rng.randrange(1, 24), rng.randrange(1, 16)
    pixels = bytearray()
    style = n % 4
    for i in range(w * h * (bpp // 8)):
        if style == 0:
            pixels.append(rng.randrange(256))
        elif style == 1:
            pixels.append(0 if rng.random() < 0.7 else rng.randrange(256))
        elif style == 2:
            pixels.append((i // (bpp // 8)) % 7 * 30)
        else:
            pixels.append(200)
    header = struct.pack("<8H", w, h, bpp, 0, 1 if bpp == 24 else 2, 0, 0, 0)
    data = header + bytes(pixels)
    src = alloc(len(data) + 64)
    mu.mem_write(src, data)
    out = alloc(4 * len(data) + 64)
    mu.mem_write(out, bytes(4 * len(data) + 64))
    size = alloc(4)
    tick[0] = rng.randrange(1 << 32)
    call(0x46A320, [out, src], {UC_X86_REG_ECX: size})
    length = struct.unpack("<I", mu.mem_read(size, 4))[0]
    encoded = mu.mem_read(out, length)
    emit(f"cbg_{n:03}", 9, len(data), 1, length, 1, (tick[0], 0, 0), data, bytes(length), encoded)
print("cbg done", len(cbg_cases))

# kind 10: sub_4A1930 (90:CE format 1, CompressedBG v2), replayed without
# its SEH frame: sub_49BBB0 (encoder object, eax; ecx = worker pool
# dword_56670C), sub_49BE00 (eax = quality, esi = object), sub_4A1E70
# (cdecl (output, Y table = object + 32, C table = object + 96), ecx = the
# 16-byte descriptor), then sub_49C200 (ecx = &size, edx = bitmap
# descriptor, stack = (object, output + header)). The pool has one thread
# (+4 = 1, +8 = 1), so sub_446910 runs the strips inline.
# sw = input bytes (descriptor + pixels), dw = output bytes; p0 = key,
# p1 = quality.
POOL = STUBS + 0x1900
mu.mem_write(POOL, bytes(256))
mu.mem_write(POOL + 4, struct.pack("<2I", 1, 1))
mu.mem_write(0x56670C, struct.pack("<I", POOL))
v2_cases = []
for bpp in (24, 32):
    for quality in (0, 25, 49, 50, 75, 100):
        v2_cases.append((bpp, quality, 0))
for n in range(36):
    v2_cases.append((rng.choice((24, 32)), rng.choice((0, 1, 10, 33, 49, 50, 51, 70, 90, 99, 100)), 1 + n % 4))
for n, (bpp, quality, style) in enumerate(v2_cases):
    heap_top = global_heap
    w, h = rng.randrange(1, 40), rng.randrange(1, 30)
    if style == 2:
        w, h = rng.randrange(32, 90), rng.randrange(2, 50)
    pixels = bytearray()
    for y in range(h):
        for x in range(w):
            if style == 1:
                colour = [rng.randrange(256) for c in range(3)]
            elif style == 3:
                colour = [200, 100, 50]
            else:
                colour = [((x * 9 + y * 5 + c * 40 + rng.randrange(8)) & 0xFF) for c in range(3)]
            pixels += bytes(colour)
            if bpp == 32:
                if style == 3:
                    pixels.append(255)
                elif style == 4:
                    pixels.append(255 if (x // 5 + y // 3) % 2 else 0)
                else:
                    pixels.append(rng.choice((0, 255, rng.randrange(256))))
    header = struct.pack("<8H", w, h, bpp, 0, 1 if bpp == 24 else 2, 0, 0, 0)
    data = header + bytes(pixels)
    src = alloc(len(data) + 64)
    mu.mem_write(src, data)
    capacity = 4 * len(data) + 4096
    out = alloc(capacity)
    mu.mem_write(out, bytes(capacity))
    obj = alloc(0x400)
    mu.mem_write(obj, bytes(0x400))
    call(FPU, [], {})
    call(0x49BBB0, [], {UC_X86_REG_EAX: obj, UC_X86_REG_ECX: POOL})
    call(0x49BE00, [], {UC_X86_REG_EAX: quality, UC_X86_REG_ESI: obj})
    tick[0] = rng.randrange(1 << 32)
    call(0x4A1E70, [out, obj + 32, obj + 96], {UC_X86_REG_ECX: src})
    header_len = mu.reg_read(UC_X86_REG_EAX)
    desc = alloc(24)
    mu.mem_write(desc, struct.pack("<6I", src + 16, w * bpp // 8, w, h, 1 if bpp == 24 else 2, bpp // 8))
    size = alloc(4)
    mu.mem_write(size, struct.pack("<I", 2 * (bpp // 8) * ((h + 7) & ~7) * ((w + 7) & ~7)))
    thrown[0] = False
    call(0x49C200, [obj, out + header_len], {UC_X86_REG_ECX: size, UC_X86_REG_EDX: desc})
    # p2 = 1: the encoder rejected its own output (status 3).
    if thrown[0]:
        emit(f"cbg2_{n:03}", 10, len(data), 1, 0, 1, (tick[0], quality, 1), data, b"", b"")
        continue
    length = header_len + struct.unpack("<I", mu.mem_read(size, 4))[0]
    encoded = mu.mem_read(out, length)
    emit(f"cbg2_{n:03}", 10, len(data), 1, length, 1, (tick[0], quality, 0), data, bytes(length), encoded)
print("cbg2 done", len(v2_cases))
