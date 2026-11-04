# Architecture decision records

One file per decision, numbered in order. Each record says what was decided,
why, what evidence it rests on, and — for work in progress — where it stopped,
so a new session can resume from the record instead of re-reading old
transcripts or re-running IDA.

| # | Title | Status |
|---|---|---|
| [0001](0001-reverse-workflow-and-reference-binaries.md) | Reverse workflow and reference binaries | Accepted |
| [0002](0002-d0-logic-extension.md) | 0xD0 is the logic extension (grid field, space, search worker) | Done |
| [0003](0003-selector-exactness-audit.md) | Every native selector matches BGI 1.622 exactly | In progress |
| [0004](0004-windows-filesystem-emulation.md) | Non-Windows hosts emulate the Windows filesystem namespace | Proposed |
| [0005](0005-vm-value-model.md) | The VM uses BGI 1.622's value and address model | In progress |

Template: `# NNNN. Title`, then **Status**, **Context**, **Decision**,
**Consequences**, and (while open) **Progress / next step**.
