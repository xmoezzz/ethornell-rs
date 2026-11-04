//! Browser entry points. Everything here runs in the page's engine worker
//! (platform/wasm/worker.js): the worker owns the picked game folder's
//! `File`s, reads them in place with `FileReaderSync`, and relays events,
//! dialogs and audio to the page.
#[cfg(all(target_arch = "wasm32", target_os = "unknown"))]
mod web;
#[cfg(all(target_arch = "wasm32", target_os = "unknown"))]
pub use web::*;
