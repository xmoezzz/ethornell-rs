# Android

An app with a game list and a player, around the engine library
`libethornell_mobile.so` (`crates/ethornell-mobile`). Android 8.0 (API 26) or later,
arm64-v8a, and x86_64 for the emulator.

## Playing

1. Install `ethornell-android.apk` from the releases.
2. **Import game folder** and pick the folder that holds the game's `.arc`
   files; the app copies it into its own storage. Alternatively copy the
   folder over USB into `Android/data/com.xmoezzz.ethornell/files/games/`.
3. Tap the game. Long-press it to choose the text encoding (GBK for Chinese
   translations) or to delete it.

| Gesture | Game input |
| --- | --- |
| tap, drag | left mouse button |
| two-finger tap, Back | right click (the game menu) |
| two-finger vertical drag | mouse wheel (the backlog) |

Saves are kept in the app's private storage, apart from the game files, and
survive deleting the game. A hardware keyboard or gamepad works too (Enter,
Escape, Space, Ctrl, arrows, Page Up/Down).

Published release APKs use a fixed signing key so later releases can update the
installed app. PR and feature-branch artifacts use debug signing. Switching from
an older debug-signed release to the fixed release key requires backing up data
and uninstalling the old app once; Android does not accept an update signed by
a different key.

## Building

```sh
platform/android/build.sh                 # debug APK
VARIANT=release platform/android/build.sh # release APK (sign it yourself)
ABIS=arm64-v8a platform/android/build.sh  # one ABI
```

Needs rustup, a JDK 17 or 21, and the Android SDK (`ANDROID_HOME`) with an
NDK (`ANDROID_NDK_HOME`, or the newest under `$ANDROID_HOME/ndk`). The script
builds `libethornell_mobile.so` with cargo-ndk into `app/src/main/jniLibs`, checks
its ELF layout (`verify_elf.py`: 16 KB pages, RELRO, exports), then runs
Gradle.

To sign a local release, set `ANDROID_KEYSTORE` to your keystore path and supply
`ANDROID_KEY_ALIAS`, `ANDROID_KEYSTORE_PASSWORD`, and `ANDROID_KEY_PASSWORD`, then
run `bash platform/android/sign.sh INPUT.apk OUTPUT.apk`. Signing verifies the
result and preserves 16 KiB alignment for uncompressed native libraries.

CI uses the same script. Main/tag releases require repository secrets
`ANDROID_KEY_BASE64` (the base64-encoded keystore), `ANDROID_KEY_ALIAS`,
`ANDROID_KEYSTORE_PASSWORD`, and `ANDROID_KEY_PASSWORD`. Missing secrets or a
failed signature verification stop publication; CI does not generate temporary
release keys. Keep an independent backup of the release keystore and passwords.

## Layout

| Path | |
| --- | --- |
| `app/src/main/java/com/xmoezzz/ethornell/` | launcher, player, game library |
| `include/ethornell_android.h`, `include/ABI.md` | the engine's C ABI and its contract |
| `include/STORAGE.md` | how game files are read and user data written |
| `../../crates/ethornell-mobile/src/android_jni.rs` | the JNI functions of `NativeEngine`, over the C ABI |

The C ABI is the stable interface for other hosts; the app itself goes
through the JNI functions in the same library.
