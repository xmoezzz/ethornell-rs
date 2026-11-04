# Web

The engine compiled to WebAssembly (`crates/ethornell-web`), running in a
web worker of a static page. Needs a browser with OffscreenCanvas, WebGL2
and module workers: current Chrome, Edge, Firefox and Safari 17+.

## Playing

1. Open the page (`ethornell-web.zip` from the releases, served over HTTP or
   HTTPS; opening `index.html` as a file does not work).
2. **Open game folder…** and pick the folder holding `system.arc` and the
   `data*.arc` archives. The files are read in place, piece by piece, as the
   game needs them; nothing is uploaded or copied, so multi-gigabyte games
   start at once.
3. The text font: the page uses, in this order, a font chosen with
   **Choose font…** (remembered), a `.ttf`/`.otf`/`.ttc` in the game folder,
   or a Japanese system font through the browser's Local Font Access (Chrome
   asks for permission). Other browsers need a chosen font, for example
   Noto Sans JP.

Mouse and keyboard work as on Windows; on touch screens a tap is a left
click. Saves are kept in the browser's IndexedDB, per game folder name.
Dialogs the game opens (name entry, choices, Yes/No questions) appear as
page dialogs.

Not available on the web: Flash (SWF) bitmaps, and soundtracks of movies
are decoded fully before they play instead of streaming.

## Building

```sh
platform/wasm/build.sh                              # build/wasm
python3 -m http.server -d build/wasm 8000           # http://localhost:8000
```

Needs rustup and `wasm-bindgen-cli` of the version in `Cargo.lock` (the
script installs it if needed).

## Layout

| File | Role |
| --- | --- |
| `index.html`, `style.css` | launcher page and dialogs |
| `main.js` | folder and font choice, input, dialogs, audio output |
| `worker.js` | runs the engine: steps it each frame, reads picked files with `FileReaderSync`, stores saves |
| `audio-worklet.js` | plays the frames the worker renders on request |
| `pkg/` | wasm-bindgen output (built) |

The worker owns the canvas (`transferControlToOffscreen`) and the engine
draws on it with WebGL2. The engine runs through the same host-driven
runtime as the mobile apps (`HostRuntime`), so it gets the same file
sandbox: the picked folder is the read-only resource root, and user data
lives in memory and is mirrored to IndexedDB on every write.
