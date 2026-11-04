#!/bin/sh
set -eu
cd "$(dirname "$0")/player"
export VITASDK="${VITASDK:-/usr/local/vitasdk}"
export PATH="$VITASDK/bin:$PATH"
exec cargo "+${RUST_TOOLCHAIN:-nightly-2026-09-18}" vita build vpk -- --release "$@"
