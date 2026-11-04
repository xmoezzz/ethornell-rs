#!/usr/bin/env python3
"""Mark selectors verified in src/adr/0003-selector-audit.csv.

Usage: python3 src/tools/audit_mark.py 80:00 80:01 ...   (or --todo to list)
       python3 src/tools/audit_mark.py --defer 80:20 ...   (blocked on the VM model)
"""
import sys
from pathlib import Path

path = Path(__file__).resolve().parents[2] / "src/adr/0003-selector-audit.csv"
lines = path.read_text().splitlines()
if sys.argv[1:] == ["--todo"]:
    todo = [l.split(",")[0] for l in lines[1:] if l.endswith(",todo")]
    print(len(todo), " ".join(todo))
    sys.exit()
status = "verified"
args = sys.argv[1:]
if args and args[0] == "--defer":
    status, args = "deferred", args[1:]
wanted = {a.upper() for a in args}
out = [lines[0]]
for line in lines[1:]:
    fields = line.split(",")
    if fields[0].upper() in wanted:
        fields[-1] = status
        wanted.discard(fields[0].upper())
    out.append(",".join(fields))
path.write_text("\n".join(out) + "\n")
if wanted:
    sys.exit(f"unknown: {' '.join(sorted(wanted))}")
