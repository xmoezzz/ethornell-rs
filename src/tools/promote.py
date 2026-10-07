#!/usr/bin/env python3
"""Update one native selector's implementation status and notes.

    python3 src/tools/promote.py 0x80:0x5C --notes "..."          # -> PortableEquivalent
    python3 src/tools/promote.py 0x80:0x5C --partial --notes "..." # keep Partial, new notes
    python3 src/tools/promote.py 0x80:0x5C --symbol NewName        # rename the symbol

Edits crates/ethornell-vm/src/native_call.rs (status lists and the
NativeOpcodeSpec notes/symbol) and the matching line of
crates/ethornell-script/src/native_contract_reference.rs.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CALL = ROOT / "crates/ethornell-vm/src/native_call.rs"
REF = ROOT / "crates/ethornell-script/src/native_contract_reference.rs"


def rust_str(text):
    return text.replace("\\", "\\\\").replace('"', '\\"')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("opcode")
    ap.add_argument("--notes")
    ap.add_argument("--partial", action="store_true")
    ap.add_argument("--symbol")
    args = ap.parse_args()
    group, ident = (int(x, 16) for x in args.opcode.split(":"))

    src = CALL.read_text()
    consts = [
        m.group(1)
        for m in re.finditer(
            r"pub const (\w+): NativeOpcode = NativeOpcode \{\s*group:\s*(0x[0-9A-Fa-f]+),\s*id:\s*(0x[0-9A-Fa-f]+)",
            src,
        )
        if int(m.group(2), 16) == group and int(m.group(3), 16) == ident
    ]
    if not consts:
        sys.exit(f"no constant for {args.opcode}")

    def list_span(name):
        start = src.index(f"const {name}: &[NativeOpcode] = &[")
        return start, src.index("];", start)

    if not args.partial:
        for name in consts:
            entry = f"    opcodes::{name},\n"
            p0, p1 = list_span("PARTIAL_IMPLEMENTATION_OPCODES")
            if entry in src[p0:p1]:
                idx = src.index(entry, p0)
                src = src[:idx] + src[idx + len(entry):]
            q0, q1 = list_span("PORTABLE_EQUIVALENT_OPCODES")
            if entry not in src[q0:q1]:
                src = src[:q1] + entry + src[q1:]

    spec_pat = re.compile(
        r"(NativeOpcodeSpec \{\s*opcode: opcodes::(?:%s),\s*symbol: \")([^\"]*)(\".*?notes: \")((?:[^\"\\]|\\.)*)(\",)"
        % "|".join(consts),
        re.S,
    )
    m = spec_pat.search(src)
    if not m:
        sys.exit(f"no NativeOpcodeSpec for {args.opcode}")
    old_symbol = m.group(2)
    symbol = args.symbol or old_symbol
    notes = rust_str(args.notes) if args.notes is not None else m.group(4)
    src = src[: m.start()] + m.group(1) + symbol + m.group(3) + notes + m.group(5) + src[m.end():]
    CALL.write_text(src)

    ref = REF.read_text()
    key = f"//! `0x{group:02X}:0x{ident:02X}` `"
    lines = ref.split("\n")
    for i, line in enumerate(lines):
        if line.startswith(key):
            if args.symbol:
                line = line.replace(f"`{old_symbol}`", f"`{symbol}`", 1)
            if not args.partial:
                line = line.replace("implementation=Partial", "implementation=PortableEquivalent")
            if args.notes is not None:
                head, sep, _ = line.partition("]. ")
                line = head + sep + args.notes if sep else line
            lines[i] = line
            break
    else:
        print(f"warning: {args.opcode} not in contract reference", file=sys.stderr)
    REF.write_text("\n".join(lines))
    print(f"{args.opcode} {consts} {'Partial' if args.partial else 'PortableEquivalent'}")


if __name__ == "__main__":
    main()
