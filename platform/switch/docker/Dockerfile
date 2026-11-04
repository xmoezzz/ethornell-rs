# Nintendo Switch build environment for platform/switch/runtime: devkitPro's
# devkitA64 image (devkitA64, libnx, deko3d, uam, switch-tools) with the Rust
# nightly the runtime Makefile pins and the standard library source.
#
#   docker build -t ethornell-switch-build platform/switch/docker
#   docker run --rm -v "$PWD:/src" -w /src ethornell-switch-build \
#       make -C platform/switch/runtime
FROM devkitpro/devkita64:latest

ARG RUST_TOOLCHAIN=nightly-2026-01-20

ENV RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    DEVKITPRO=/opt/devkitpro \
    DEVKITA64=/opt/devkitpro/devkitA64
ENV PATH=$CARGO_HOME/bin:$DEVKITA64/bin:$DEVKITPRO/tools/bin:$PATH

RUN apt-get update \
    && apt-get install -y --no-install-recommends ca-certificates curl build-essential \
    && rm -rf /var/lib/apt/lists/*

# The Makefile runs `rustup run nightly-2026-01-20 cargo build -Zbuild-std`.
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs \
        | sh -s -- -y --no-modify-path --profile minimal \
            --default-toolchain "$RUST_TOOLCHAIN" --component rust-src \
    && chmod -R a+rwX "$RUSTUP_HOME" "$CARGO_HOME"
