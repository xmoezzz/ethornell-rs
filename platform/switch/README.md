# Nintendo Switch

Native libnx/deko3d frontend. It shares the BGI VM, display tree, resource
loading, text rasterizer and Kira mixer with the desktop player. Textures,
affine quads, clipping, filtering and blending are submitted to deko3d;
there is no software frame compositor or wgpu dependency in the NRO.

Build with devkitA64, libnx, deko3d, switch-tools (`uam` included), and the
nightly/rust-src named in `runtime/Makefile`:

```sh
./platform/switch/build.sh
```

Or build headlessly in the supplied image:

```sh
docker build --platform linux/amd64 -t ethornell-switch-build platform/switch/docker
docker run --rm --platform linux/amd64 -v "$PWD:/src" -w /src \
  ethornell-switch-build make -C platform/switch/runtime
```

Output: `runtime/ethornell_switch.nro`. Copy it to
`sdmc:/switch/ethornell/ethornell_switch.nro`, the unmodified game directory
to `sdmc:/switch/ethornell/game/`, and a Japanese/CJK TrueType font to
`sdmc:/switch/ethornell/font.ttf`. Saves go into the separate `saves/` folder;
startup/runtime errors go to `player.log`. Applet memory may be insufficient
for large games: use a full-memory homebrew launch.

The left stick moves the pointer (a light tilt for buttons, full tilt
crosses the screen in under a second); A clicks, B sends Escape, X Space, Y Enter,
ZR Control, and L/R scroll. The D-pad sends arrow keys. Touch holds/releases
the same left button as A. Plus+Minus requests the game's normal close path.
Native confirmation prompts appear over the game and accept A/B. Structured
text-entry dialogs, ActiveX Flash and desktop integration are unsupported;
they report failure instead of returning invented results.

The standalone `platform/console` lockfile is separate from desktop/mobile.
Its Horizon libc pin and checked patch match devkitA64's newlib `stat` and
`dirent` layouts, including build-std. GPU upload commands are queued before
the display pass, with fences and deferred resource destruction.

The deko3d resource/submission layer and audio-driver setup are adapted from
[xmoezzz/siglus_rs](https://github.com/xmoezzz/siglus_rs/tree/5f358ef1360b0439e92aed2b749e8f25d8721954/platform/switch),
under MPL-2.0. Ethornell's vertex/fragment programs implement its own draw ABI.
