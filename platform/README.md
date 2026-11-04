# Platforms

The engine is the same Rust code everywhere. Desktop systems run it as the
`ethornell` program with its own window (winit); the mobile apps drive it
through the host-driven C ABI of `crates/ethornell-mobile`, and the web page
through the same host-driven runtime compiled to WebAssembly
(`crates/ethornell-web`).

| Platform | Directory | Package | Build |
| --- | --- | --- | --- |
| Windows | — | `ethornell.exe` | `cargo build --release -p ethornell-cli` |
| Linux | — | `ethornell` | `cargo build --release -p ethornell-cli` |
| FreeBSD | — | `ethornell` | `cargo build --release -p ethornell-cli` |
| macOS | `macos/` | `Ethornell.app` (universal) | `macos/package.sh` |
| Android | `android/` | APK | `android/build.sh` |
| iOS | `ios/` | app / IPA | `ios/build.sh` |
| Nintendo Switch | `switch/` | NRO | `switch/build.sh` |
| PS Vita | `vita/` | VPK | `vita/build.sh` |
| Web | `wasm/` | static page (zip) | `wasm/build.sh` |

CI builds all nine (`.github/workflows/release.yml`) and publishes them on
the releases page. See [Switch](switch/README.md) and
[PS Vita](vita/README.md) for setup and controls.

## Desktop

Start `ethornell` (or open `Ethornell.app`) without arguments and it asks for
the game folder; `ethornell <game folder>`, or a folder dropped on the
program, plays that game. The tools and options are under
`ethornell --help`; `ethornell run --game <folder>` is the full form.

On Linux and FreeBSD the folder dialog uses `zenity` or `kdialog`, and sound needs ALSA.
The macOS app is signed ad hoc: open it the first time with right-click >
Open.

## Mobile

`android/README.md` and `ios/README.md` (the web page: `wasm/README.md`). Both apps keep a list of imported
games, store saves apart from the game files, and map touches to the mouse:
a tap is a left click, a two-finger tap a right click, a two-finger drag the
wheel.
