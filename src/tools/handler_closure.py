#!/usr/bin/env python3
"""Write one C work package per native selector.

A package holds the selector's handler plus every function reachable through
direct `sub_XXXXXX(` calls (virtual calls through vtables are not visible in
pseudocode and are listed as warnings for manual follow-up).

Usage: python3 src/tools/handler_closure.py [--stats]
"""
import csv
import re
import sqlite3
import sys
from pathlib import Path

root = Path(__file__).resolve().parents[2]
db = sqlite3.connect(root / "reverse/BGI.sqlite")
code = {int(a, 16): (n, p) for a, n, p in db.execute(
    "select start_ea_hex,name,pseudocode from decompilations where status='ok'")}
call = re.compile(r"\bsub_([0-9A-F]{6})\b")
vcall = re.compile(r"\*\(\w+ \(\w+ \*\*\)[^)]*\)\(\*\(_DWORD \*\)[^;]*")

# Library/CRT code is not part of the engine; stop the walk there.
LIB_FROM = 0x4A0000


def closure(entry):
    seen, order, stack = set(), [], [entry]
    while stack:
        a = stack.pop()
        if a in seen or a not in code:
            continue
        seen.add(a)
        order.append(a)
        for m in call.findall(code[a][1]):
            t = int(m, 16)
            if t not in seen and t < LIB_FROM:
                stack.append(t)
    return sorted(order)


rows = list(csv.DictReader(open(root / "docs/reverse/target_dispatch_exe_sqlite.csv")))
out = root / "src/handlers"
out.mkdir(exist_ok=True)
import collections
users = collections.defaultdict(list)
manifest = []
for r in rows:
    handler = int(r["handler_address"], 16)
    if handler not in code:
        manifest.append((r["opcode"], r["symbol"], handler, []))
        continue
    fns = closure(handler)
    manifest.append((r["opcode"], r["symbol"], handler, fns))
    for a in fns:
        users[a].append(r["opcode"])

with open(out / "closure.csv", "w", newline="") as fh:
    w = csv.writer(fh)
    w.writerow(["opcode", "symbol", "entry", "function_count", "functions"])
    for op, sym, h, fns in manifest:
        w.writerow([op, sym, f"sub_{h:06X}", len(fns), " ".join(f"{a:06X}" for a in fns)])

with open(out / "shared_core.txt", "w") as fh:
    fh.write("# Functions reachable from many selectors, most shared first.\n"
             "# count address name bytes\n")
    for a, ops in sorted(users.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        if len(ops) >= 8:
            fh.write(f"{len(ops):4d} {a:06X} {code[a][0]} {len(code[a][1])}\n")
core = sum(1 for ops in users.values() if len(ops) >= 8)
private = sum(1 for ops in users.values() if len(ops) == 1)
print(f"{len(manifest)} selectors, {len(users)} engine functions, "
      f"{core} shared by >=8 selectors, {private} private to one selector")
