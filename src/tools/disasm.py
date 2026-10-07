#!/usr/bin/env python3
"""Disassemble target code with resolved call targets: disasm.py <hex addr> <hex length>"""
import struct, subprocess, sys
from pathlib import Path
root = Path(__file__).resolve().parents[2]
d = (root / "docs/reverse/Tayutama2_trial_TG.exe").read_bytes()
pe = struct.unpack_from("<I", d, 0x3C)[0]
nsec = struct.unpack_from("<H", d, pe + 6)[0]
opt = struct.unpack_from("<H", d, pe + 20)[0]
base = 0x400000
secs = [struct.unpack_from("<8sIIII", d, pe + 24 + opt + 40 * i) for i in range(nsec)]
def rd(va, n):
    rva = va - base
    for _, vs, sva, rs, rp in secs:
        if sva <= rva < sva + max(vs, rs):
            return d[rp + rva - sva: rp + rva - sva + n]
va = int(sys.argv[1], 16); n = int(sys.argv[2], 16)
b = rd(va, n)
out = subprocess.run(["/opt/homebrew/opt/llvm/bin/llvm-mc", "--disassemble", "-triple=i386", "--output-asm-variant=1", "--show-encoding"],
                     input=" ".join("0x%02x" % x for x in b).encode(), capture_output=True).stdout.decode()
pos = 0
for line in out.split("\n"):
    if "# encoding" not in line:
        continue
    ins, enc = line.split("# encoding:")
    nb = len(enc.strip(" []").split(","))
    ins = ins.strip()
    if ins.split()[0] in ("call", "jmp", "je", "jne", "jg", "jl", "jge", "jle", "ja", "jb", "jae", "jbe", "js", "jns") and nb in (2, 5, 6):
        off = struct.unpack_from("<b" if nb == 2 else "<i", b, pos + (1 if nb in (2, 5) else 2))[0]
        ins = f"{ins.split()[0]} 0x{(va + pos + nb + off) & 0xffffffff:X}"
    print("%06X  %s" % (va + pos, ins))
    pos += nb
