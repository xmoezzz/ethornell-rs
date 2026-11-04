#!/usr/bin/env python3
"""List the Rust match arms / branches that handle one native selector.

Usage: python3 src/tools/rust_arm.py 80:24 [--context N]

Searches crates/*/src for `(0x80, 0x24)`-style tuples (any case/spacing) and
`0x24 =>` arms inside per-group dispatchers, printing N lines after each hit.
"""
import re
import sys
from pathlib import Path

root = Path(__file__).resolve().parents[2]
group, ident = (int(x, 16) for x in sys.argv[1].split(":"))
ctx = int(sys.argv[sys.argv.index("--context") + 1]) if "--context" in sys.argv else 25
pat = re.compile(
    r"\(\s*0x0*%x\s*,\s*0x0*%x\s*\)|\(\s*0x0*%x\s*,\s*[^)]*\b0x0*%x\b[^)]*\)\s*=>"
    % (group, ident, group, ident),
    re.I,
)
call = (root / "crates/ethornell-vm/src/native_call.rs").read_text()
names = [
    m.group(1)
    for m in re.finditer(
        r"pub const (\w+): NativeOpcode = NativeOpcode \{\s*group:\s*(0x[0-9A-Fa-f]+),\s*id:\s*(0x[0-9A-Fa-f]+)",
        call,
    )
    if int(m.group(2), 16) == group and int(m.group(3), 16) == ident
]
if names:
    pat = re.compile(pat.pattern + "|opcodes::(?:%s)\\s*(?:=>|\\|)" % "|".join(names), re.I)
for path in sorted(root.glob("crates/*/src/**/*.rs")):
    if "native_call.rs" in path.name or "native_contract_reference" in path.name:
        continue
    text = path.read_text(errors="replace")
    lines = text.split("\n")
    # Per-group dispatchers (`if group != 0x80 { return None }` then
    # `match id { 0x1E => ... }`).
    grouped = re.search(r"group\s*!=\s*0x0*%x\b" % group, text, re.I)
    id_arm = re.compile(r"^\s*(?:\|\s*)?(?:0x0*%x\s*\|\s*)*0x0*%x\b[^=]*=>" % (ident, ident), re.I)
    for i, line in enumerate(lines):
        hit = pat.search(line) or (grouped and id_arm.search(line))
        if hit and "#[test]" not in "".join(lines[max(0, i - 3):i]):
            rel = path.relative_to(root)
            print(f"===== {rel}:{i + 1}")
            print("\n".join(lines[i:i + ctx]))
