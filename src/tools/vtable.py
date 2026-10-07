#!/usr/bin/env python3
"""Dump the virtual methods of a class: python3 src/tools/vtable.py CProcWaitTimingEx [--code]"""
import re, sqlite3, struct, sys
from pathlib import Path
root = Path(__file__).resolve().parents[2]
d = (root / "docs/reverse/Tayutama2_trial_TG.exe").read_bytes()
pe = struct.unpack_from("<I", d, 0x3C)[0]
nsec = struct.unpack_from("<H", d, pe + 6)[0]
opt = struct.unpack_from("<H", d, pe + 20)[0]
base = 0x400000
secs = [struct.unpack_from("<8sIIII", d, pe + 24 + opt + 40 * i) for i in range(nsec)]
def dword(va):
    rva = va - base
    for _, vs, sva, rs, rp in secs:
        if sva <= rva < sva + max(vs, rs):
            return struct.unpack_from("<I", d, rp + rva - sva)[0]
db = sqlite3.connect(root / "reverse/BGI.sqlite")
cls = sys.argv[1]
row = db.execute("select ea_hex from names where name like ?", (f"??_7{cls}@@6B@",)).fetchone()
va = int(row[0], 16)
print(f"# {cls} vftable @ 0x{va:X}")
code = {int(a, 16): (n, p) for a, n, p in db.execute("select start_ea_hex,name,pseudocode from decompilations where status='ok'")}
slot = 0
while True:
    target = dword(va + 4 * slot)
    if target is None or not (0x401000 <= target < 0x4DB000):
        break
    # stop at the next vtable (an RTTI-named address follows each table)
    name = code.get(target, ("?",))[0]
    print(f"slot {slot:2d} (+0x{4*slot:02X}) -> 0x{target:06X} {name}")
    if "--code" in sys.argv and target in code:
        txt = re.sub(r"\n\s*(_DWORD|int|unsigned|char|BOOL|_BYTE|const|void|size_t|signed|_WORD|float|double|struct)[^=(\n]*; //[^\n]*", "", code[target][1])
        print(txt.rstrip(), "\n")
    slot += 1
    if slot > 40: break
