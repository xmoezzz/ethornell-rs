#!/usr/bin/env python3
"""Run BGI 1.622 sub_493E90 (SDC LZ compressor) under Unicorn and write
<name>.in / <name>.out reference pairs into OUT_DIR, for the ignored test
user_data::tests::sdc_compressor_matches_the_target_vectors.

Usage: python3 -m venv v && v/bin/pip install unicorn pefile
       v/bin/python src/tools/emu_sdc.py OUT_DIR
       ETHORNELL_SDC_VECTORS=OUT_DIR cargo test -p ethornell-vm sdc_compressor -- --ignored
"""
import sys, os, random, pefile
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
STACK, SRC, DST, RET = 0x10000000, 0x20000000, 0x30000000, 0x40000000
mu.mem_map(STACK, 0x100000); mu.mem_map(SRC, 0x200000); mu.mem_map(DST, 0x400000); mu.mem_map(RET, 0x1000)
def compress(data):
    mu.mem_write(SRC, bytes(data) + b"\0" * 64)
    mu.mem_write(DST, b"\0" * (len(data) * 2 + 64))
    sp = STACK + 0xF0000
    sp -= 4; mu.mem_write(sp, len(data).to_bytes(4, "little"))
    sp -= 4; mu.mem_write(sp, DST.to_bytes(4, "little"))
    sp -= 4; mu.mem_write(sp, RET.to_bytes(4, "little"))
    mu.reg_write(UC_X86_REG_ESP, sp); mu.reg_write(UC_X86_REG_ECX, SRC)
    mu.emu_start(0x493E90, RET)
    n = mu.reg_read(UC_X86_REG_EAX)
    return bytes(mu.mem_read(DST, n))
rng = random.Random(1622)
vectors = {
    "empty": b"", "one": b"A", "two": b"AB", "aa": b"AA", "aaa": b"AAA",
    "zeros300": bytes(300), "zeros5000": bytes(5000),
    "abab": b"AB" * 700, "text": ("吾輩は猫である。名前はまだ無い。" * 40).encode("cp932"),
    "rand200": bytes(rng.randrange(256) for _ in range(200)),
    "rand9000_lowalpha": bytes(rng.choice(b"abcd") for _ in range(9000)),
    "mixed": bytes(rng.randrange(256) for _ in range(129)) + bytes(40) + bytes(rng.randrange(256) for _ in range(300)) + b"xyz" * 900,
}
arc = open("/Users/xmoe/Documents/千の刃濤、桃花染の皇姫 体験版/senmomo_trial/system.arc", "rb").read()
vectors["arc_head"] = arc[:20000]
vectors["arc_mid"] = arc[len(arc)//2: len(arc)//2 + 30000]
for name, data in vectors.items():
    open(os.path.join(OUT, name + ".in"), "wb").write(data)
    open(os.path.join(OUT, name + ".out"), "wb").write(compress(data))
    print(name, len(data), len(open(os.path.join(OUT, name + ".out"), "rb").read()))
