#!/usr/bin/env python3
"""Read initialised data from the target image: rdata.py <hex va> <count> [u8|u16|i16|u32|i32]"""
import struct, sys
from pathlib import Path
d = (Path(__file__).resolve().parents[2] / "docs/reverse/Tayutama2_trial_TG.exe").read_bytes()
pe = struct.unpack_from("<I", d, 0x3C)[0]
n = struct.unpack_from("<H", d, pe + 6)[0]
opt = struct.unpack_from("<H", d, pe + 20)[0]
secs = [struct.unpack_from("<8sIIII", d, pe + 24 + opt + 40 * i) for i in range(n)]
def rd(va, k):
    rva = va - 0x400000
    for _, vs, sva, rs, rp in secs:
        if sva <= rva < sva + max(vs, rs):
            off = rva - sva
            raw = d[rp + off: rp + min(off + k, rs)]
            return raw + bytes(k - len(raw))  # .bss tail reads as zero
    raise SystemExit("address not mapped")
va = int(sys.argv[1], 16); cnt = int(sys.argv[2]); fmt = sys.argv[3] if len(sys.argv) > 3 else "u32"
code = {"u8": "B", "u16": "H", "i16": "h", "u32": "I", "i32": "i"}[fmt]
size = struct.calcsize(code)
print(list(struct.unpack("<%d%s" % (cnt, code), rd(va, cnt * size))))
