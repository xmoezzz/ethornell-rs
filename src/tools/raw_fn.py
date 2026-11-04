#!/usr/bin/env python3
"""Print functions from the checked-in BGI 1.622 export (src/raw/bgi1622).

Usage:
    python3 src/tools/raw_fn.py 46DF00 [--depth N] [--asm]
    python3 src/tools/raw_fn.py 90:38 [--depth N]     # selector -> handler

Follows sub_XXXXXX callees up to --depth levels (default 0).
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2] / "src/raw/bgi1622"
args = sys.argv[1:]
depth = int(args[args.index("--depth") + 1]) if "--depth" in args else 0
asm = "--asm" in args
target = args[0].upper()

text = (ROOT / ("native_all.asm" if asm else "native_all.c")).read_text(errors="replace")
funcs = {}
for chunk in re.split(r"\n(?=(?://|;)==== )", text):
    m = re.match(r"(?://|;)==== ([0-9A-F]+) ", chunk)
    if m:
        funcs[m.group(1)] = chunk.rstrip()

if ":" in target:
    head = (ROOT / "native_all.c").read_text(errors="replace")[:200000]
    m = re.search(r"//\s+" + re.escape(target) + r" -> ([0-9A-F]+)", head)
    if not m:
        sys.exit(f"no handler for {target}")
    target = m.group(1)

seen = set()


def show(addr, level):
    if addr in seen or addr not in funcs:
        return
    seen.add(addr)
    print(funcs[addr] + "\n")
    if level < depth:
        for callee in re.findall(r"\bsub_([0-9A-F]{6})\b", funcs[addr]):
            show(callee, level + 1)


show(target.lstrip("0X").rjust(6, "0") if len(target) < 6 else target, 0)
if not seen:
    sys.exit(f"{target} not in export")
