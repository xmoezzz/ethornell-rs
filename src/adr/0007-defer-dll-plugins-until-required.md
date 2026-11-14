# 0007. Defer DLL plugins until required by a game

**Status:** Accepted

## Context

BGI exposes DLL extension operations through `80:EC` (load), `80:EE` (call),
and `80:ED` (unload). The original loader requires the export
`CallFunctionForEthornell` and optionally calls `SetWindowHandleOfEthornell`.
The existence of this interface does not establish that a game needs a plugin.

## Decision

Game-provided DLL plugins are currently unsupported. Do not load or execute
their native code, and do not acknowledge unsupported calls as successful.
Preserve native not-found results and report unsupported existing extensions
explicitly.

Keep the recognized VM ABI and the owned Rust extension interface. Do not
implement speculative plugins. Add a concrete plugin implementation only when
an available game actually requires it, after identifying its library, commands,
arguments, return values, state and resource lifetime.

Implement required behavior in portable Rust, reusing engine services where
appropriate. Guest handles must not expose native window or device pointers.

## Consequences

Games without DLL dependencies need no additional plugin support. A game that
requires an unimplemented extension remains unsupported at that call until its
specific behavior is implemented. Recognizing the extension ABI is not a claim
of plugin compatibility.
