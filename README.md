# ethornell-rs

`ethornell-rs` is a reimplementation of the Ethornell / BURIKO General Interpreter (a.k.a. "BGI") visual novel engine.

To run a game, use the `ethornell-cli` binary:

```sh
cargo run -p ethornell-cli -- run --game /path/to/game
```

Builds for Windows (x86_64), Linux (x86_64 and AArch64), macOS (a universal
`Ethornell.app`), Android (APK) and iOS (unsigned IPA) are published on
[GitHub Releases](https://github.com/asukaminato0721/ethornell-rs/releases).
The `pre-release` entry is updated after successful branch pushes; `v*` tags
publish versioned releases.
See [`platform/`](platform/README.md) for building and using each platform,
including the Android and iOS apps.

The window uses the game's icon from `icon.png`, an `.ico` file, or the icon
resources embedded in its Windows executable, in that order.

For a GBK Chinese translation, also set `ETHORNELL_TEXT_ENCODING=gbk`.
The default is Shift-JIS (`cp932`). It does not change the encoding of resource names or BP system strings.


## Claims
<p align="center">
  <img src="icon/crysirror.svg" width="160" height="160" alt="CrySirror logo">
</p>

This project belongs to `CrySirror` [project](https://github.com/xmoezzz/CrySirror), which is a unified engine backend for visual novels and 3D games. 

1. Bug fixes and improvements for the original Ethornell / BURIKO General Interpreter (a.k.a. "BGI") engine is acceptable in this repository. 

2. If you want to support a new platform (e.g. game console), please create an issue or pull request in the `CrySirror` repository instead of this one. 

