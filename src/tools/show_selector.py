#!/usr/bin/env python3
"""Print the target handler (and one level of callees) for a native selector.

Usage: python3 src/tools/show_selector.py 0x80:0x5C [--depth N]
"""
import csv, re, sqlite3, sys
from pathlib import Path
root = Path(__file__).resolve().parents[2]
sel = sys.argv[1].upper().replace("0X", "0x")
depth = int(sys.argv[sys.argv.index("--depth") + 1]) if "--depth" in sys.argv else 1
db = sqlite3.connect(root / "reverse/BGI.sqlite")
rows = {r["opcode"]: r for r in csv.DictReader(open(root / "docs/reverse/target_dispatch_exe_sqlite.csv"))}
st = {r["opcode"]: r for r in csv.DictReader(open(root / "docs/reverse/native_handler_recovery_status.csv"))}
r = rows[sel]
print(f"# {sel} {r['symbol']} handler {r['handler_name']}  inputs={r['inputs']} outputs={r['outputs']} procedure={r['procedure']}")
print("# note:", st[sel]["notes"][:600])
seen = set()
def show(name, d):
    if name in seen or d > depth:
        return
    seen.add(name)
    row = db.execute("select pseudocode from decompilations where name=? and status='ok'", (name,)).fetchone()
    if not row:
        return
    code = re.sub(r"\n\s*(_DWORD|int|unsigned|char|BOOL|_BYTE|const|void|size_t|signed|_WORD|float|double|struct)[^=(\n]*; //[^\n]*", "", row[0])
    print(f"\n// ===== {name} =====\n{code.rstrip()}")
    for callee in re.findall(r"\b(sub_[0-9A-F]{6})\b", row[0]):
        if callee not in ("sub_4450B0", "sub_4450D0", "sub_48DF50", "sub_4646F0"):
            show(callee, d + 1)
show(r["handler_name"], 0)
