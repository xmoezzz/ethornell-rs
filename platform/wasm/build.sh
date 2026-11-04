#!/usr/bin/env bash
# Builds the web version into build/wasm: the page, its worker and the
# wasm-bindgen module (pkg/). Serve that directory over HTTP(S), e.g.
#
#   platform/wasm/build.sh && python3 -m http.server -d build/wasm 8000
#
# Needs: rustup, wasm-bindgen-cli matching Cargo.lock's wasm-bindgen.
set -euo pipefail

WEB_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$WEB_DIR/../.." && pwd)"
OUT="$ROOT/build/wasm"
cd "$ROOT"

VERSION="$(awk '/^name = "wasm-bindgen"$/ { getline; gsub(/"/, "", $3); print $3; exit }' Cargo.lock)"
if [ "$(wasm-bindgen --version 2>/dev/null | awk '{print $2}')" != "$VERSION" ]; then
  echo "installing wasm-bindgen-cli $VERSION" >&2
  cargo install -f wasm-bindgen-cli --version "$VERSION" --locked
fi
rustup target add wasm32-unknown-unknown >/dev/null

cargo build -p ethornell-web --release --locked --target wasm32-unknown-unknown
rm -rf "$OUT"
mkdir -p "$OUT"
wasm-bindgen "$ROOT/target/wasm32-unknown-unknown/release/ethornell_web.wasm" \
  --target web --no-typescript --out-dir "$OUT/pkg"
cp "$WEB_DIR"/{index.html,main.js,worker.js,audio-worklet.js,style.css} "$OUT/"
echo "OK: $OUT"
