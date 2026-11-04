# PS Vita build environment for platform/vita/player: VitaSDK with
# libvita2d, a pinned Rust nightly with the standard library source, and
# cargo-vita.
#
#   docker build -t ethornell-vita-build platform/vita/docker
#   docker run --rm -v "$PWD:/src" -w /src/platform/vita/player \
#       ethornell-vita-build cargo vita build vpk -- --release
FROM ubuntu:22.04

ARG RUST_TOOLCHAIN=nightly-2026-09-18
ARG CARGO_VITA_VERSION=0.2.2

ENV DEBIAN_FRONTEND=noninteractive \
    VITASDK=/usr/local/vitasdk \
    RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo
ENV PATH=$VITASDK/bin:$CARGO_HOME/bin:$PATH

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential ca-certificates curl git python3 xz-utils zstd unzip \
    && rm -rf /var/lib/apt/lists/*

# VitaSDK and its package client, then libvita2d. The clone's own `vdpm`
# script is the legacy frontend; the SDK installs the current client.
RUN git clone --depth 1 https://github.com/vitasdk/vdpm /tmp/vdpm \
    && /tmp/vdpm/bootstrap-vitasdk.sh \
    && VDPM_NONINTERACTIVE=1 "$VITASDK/bin/vdpm" install libvita2d \
    && test -f "$VITASDK/arm-vita-eabi/lib/libvita2d.a" \
    && rm -rf /tmp/vdpm "$VITASDK/var/cache/pacman/pkg"/*

# The Vita target needs nightly (build-std, profile-rustflags). It is the
# default toolchain, so `cargo` needs no `+toolchain`.
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs \
        | sh -s -- -y --no-modify-path --profile minimal \
            --default-toolchain "$RUST_TOOLCHAIN" --component rust-src \
    && cargo install cargo-vita --version "$CARGO_VITA_VERSION" --locked \
    && rm -rf "$CARGO_HOME/registry" "$CARGO_HOME/git" \
    && chmod -R a+rwX "$RUSTUP_HOME" "$CARGO_HOME"
