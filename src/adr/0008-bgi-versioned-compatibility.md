# 0008. Select native behavior by BGI compatibility and engine version

**Status:** Accepted

**Date:** 2026-11-15

## Context

BGI declares both an engine version and, in later executables, a
`Compatibility` version. These are BGI's own identifiers. `Modern`,
`ModernFused` and the other Rust profile names are internal implementation
labels, not versions declared by BGI.

Equal compatibility declarations do not establish identical native call
contracts. The inspected original engines 1.653.3, 1.686.4 and 1.687 all
declare `Compatibility 1.72`. Their `90:9A` handlers consume three operands,
but 1.653.3 pushes no result while the two inspected x64 builds push a
status. No verified public BURIKO specification establishes complete native
ABI equivalence for equal `Compatibility` values. This observation does not
establish that BGI violates its own compatibility promise; the promise's
precise scope is not known.

Older engines also vary in their declarations and dispatch layouts. Version
and compatibility fields can be separated by a newline or a hyphen. Some
early engines have no compatibility declaration. Early 1.69 dispatchers can
lack group `81`, so requiring a later dispatch shape rejects valid old games.

## Decision

Use BGI's declared compatibility version as the compatibility-family baseline.
Refine that baseline using the actual engine version, build and architecture,
then confirm the relevant behavior against the original engine. Future support
must be organized by these engine identities rather than individual games.

1. Read the runtime's version resources as bounded data. Keep `FileVersion`,
   the fixed file-version components and `Compatibility` distinct. Do not use
   `ProductVersion` as a substitute for the engine version. Preserve build
   components and accept the observed legacy text formats. Do not interpret
   dotted versions as floating-point numbers or assume SemVer guarantees.
2. Identify the actual runtime, including a validated embedded runtime when
   the outer executable is only a launcher. Missing compatibility metadata
   stays unknown; an engine version of `1.69` must not become an invented
   `Compatibility 1.69` declaration.
3. Select a versioned behavior family using compatibility, engine version/build
   and architecture together. Shared behavior belongs in common code; confirmed
   changes belong in explicit version-family overrides. Do not infer a
   transition point or a supported version interval from one sample.
4. Confirm opcode availability, pointer representation and the relevant native
   handlers independently. A matching version string, instruction subset or
   imported DLL alone does not prove an entire ABI. File names, directory
   names, game titles, language and replacement media do not select a profile.
5. For each changed native contract, confirm operand consumption, immediate
   outputs, value interpretation, procedure/wait behavior and error paths.
   Distinguish BP stack effects from the host function's calling convention.
   Identical selectors may share an implementation only where their contracts
   agree. Correct a mistaken common contract centrally instead of adding a
   game-specific exception.
6. Report conflicting declarations or unsupported layouts explicitly. Do not
   silently select the nearest version or broaden an ABI because one game
   reaches its title. Existing resource-based recognition may remain a legacy
   fallback for already supported formats when executable metadata is absent;
   it is not a substitute for version-based onboarding of new engines.

Keep the selected identity and observed capabilities available in diagnostics.
Keep detailed native findings beside the relevant code. This ADR defines the
selection policy, not a per-game allowlist or a claim that every operation in
a compatibility family has been implemented.

### Missing compatibility declarations

An absent `Compatibility` field is not an error by itself. Preserve it as
unknown and use the actual engine version/build, architecture and recognized
dispatch layout. For example, the early 1.69 reference has a usable engine
version without declaring a compatibility level. Do not reject it solely for
that omission or assign a later compatibility level to make it pass.

If no runtime can be identified from executable metadata and layout, the
current implementation retains its existing resource-based selection or
legacy default and emits a warning. That warning must distinguish this path
from a confirmed versioned selection. This fallback preserves existing old
game behavior; it does not establish compatibility for an unidentified new
engine. New onboarding must resolve the missing evidence instead of treating
the fallback as a new supported version.

Keep an available original runtime executable alongside the game resources
for static identification. Reading its metadata does not load or execute its
native code.

Malformed declarations, contradictory version information and unsupported
layouts remain errors. They must not be reclassified as absent metadata to
reach the legacy fallback.

## Implementation and confirmation

[Native profile detection](../../crates/ethornell-app/src/native_profile.rs)
reads and checks the declarations, inspects embedded executables and verifies
dispatch layouts. The current compact-family recognition explicitly covers
the inspected 1.686.4 and 1.687 builds; that does not certify all later builds
declaring 1.72. Existing structural matchers and legacy resource fallback
remain implementation boundaries to refine under this policy.

[Native ABI contracts](../../crates/ethornell-script/src/native_abi.rs)
keep common contracts and profile overrides separate. For example, `80:41`
consumes no operands and pushes one result in all four inspected reference
engines; its former one-operand entry was our common-table error. `90:9A`
requires the version-family distinction described above.

When changing selection rules, check declaration parsing and conflicts,
caller-stack preservation, and representative older and newer families.
Keep compilation, contract assertions and scoped game execution evidence
separate. A successful startup or screenshot does not establish full-game,
full-ABI or cross-platform compatibility. Documentation-only changes do not
require rerunning game replays.

## Consequences

New games using an already confirmed engine family reuse its behavior without
another title-specific patch. A new build in the same compatibility family
still requires confirmation of its changed behavior before enabling new
contracts. Older families retain their own layouts and return conventions.

This extends ADR 0005's reference value model to version-family selection and
updates the selection policy of ADR 0006. The native limits and behavior
findings in those records remain valid within their stated reference scope.
