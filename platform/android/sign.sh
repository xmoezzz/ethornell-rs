#!/usr/bin/env bash
# Align and sign an APK with the stable release key supplied by the owner/CI.
set -euo pipefail

[[ $# -eq 2 ]] || { echo "usage: sign.sh INPUT.apk OUTPUT.apk" >&2; exit 1; }
INPUT="$1"
OUTPUT="$2"
[[ -f "$INPUT" ]] || { echo "input APK not found" >&2; exit 1; }
[[ ! -e "$OUTPUT" ]] || { echo "output APK already exists" >&2; exit 1; }
: "${ANDROID_KEYSTORE:?set ANDROID_KEYSTORE to the release keystore file}"
: "${ANDROID_KEY_ALIAS:?set ANDROID_KEY_ALIAS}"
: "${ANDROID_KEYSTORE_PASSWORD:?set ANDROID_KEYSTORE_PASSWORD}"
: "${ANDROID_KEY_PASSWORD:?set ANDROID_KEY_PASSWORD}"
export ANDROID_KEYSTORE_PASSWORD ANDROID_KEY_PASSWORD

SDK="${ANDROID_HOME:-${ANDROID_SDK_ROOT:-$HOME/Library/Android/sdk}}"
TOOLS="$SDK/build-tools/${ANDROID_BUILD_TOOLS_VERSION:-36.0.0}"
[[ -f "$ANDROID_KEYSTORE" ]] || { echo "release keystore not found" >&2; exit 1; }
[[ -x "$TOOLS/zipalign" && -x "$TOOLS/apksigner" ]] || {
    echo "Android build tools not found under $TOOLS" >&2; exit 1;
}
mkdir -p "$(dirname "$OUTPUT")"
ALIGNED="$(mktemp "$(dirname "$OUTPUT")/.ethornell-aligned.XXXXXX")"
trap 'unlink "$ALIGNED"' EXIT

# Preserve 16 KiB alignment for uncompressed native libraries on Android 15+.
"$TOOLS/zipalign" -f -P 16 4 "$INPUT" "$ALIGNED"
"$TOOLS/apksigner" sign \
    --ks "$ANDROID_KEYSTORE" \
    --ks-key-alias "$ANDROID_KEY_ALIAS" \
    --ks-pass env:ANDROID_KEYSTORE_PASSWORD \
    --key-pass env:ANDROID_KEY_PASSWORD \
    --v4-signing-enabled false \
    --out "$OUTPUT" "$ALIGNED"
"$TOOLS/apksigner" verify --verbose "$OUTPUT"
"$TOOLS/zipalign" -c -P 16 4 "$OUTPUT"
