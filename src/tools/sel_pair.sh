#!/bin/sh
# sel_pair.sh 80:0C [depth] : target handler closure + Rust arms (vm/app only)
cd "$(dirname "$0")/../.."
python3 src/tools/raw_fn.py "$1" --depth "${2:-1}"
echo "------- rust"
python3 src/tools/rust_arm.py "$1" --context "${3:-22}" | awk '/^=====/{p=0} /^===== crates\/ethornell-(vm|app)/{p=1} p' | grep -v '^\s*| (0x' 
