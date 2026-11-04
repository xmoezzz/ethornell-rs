// Platform cfg aliases for file access and desktop text detection.
fn main() {
    println!("cargo::rustc-check-cfg=cfg(fd_policy)");
    println!("cargo::rustc-check-cfg=cfg(desktop_host)");
    println!("cargo::rustc-check-cfg=cfg(hosted_files)");
    let os = std::env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();
    let arch = std::env::var("CARGO_CFG_TARGET_ARCH").unwrap_or_default();
    // Keep startup text detection out of mobile, browser and console builds.
    // Match the desktop-only chardetng dependency in Cargo.toml.
    if matches!(
        os.as_str(),
        "windows"
            | "macos"
            | "linux"
            | "freebsd"
            | "openbsd"
            | "netbsd"
            | "dragonfly"
            | "illumos"
            | "solaris"
            | "haiku"
    ) {
        println!("cargo::rustc-cfg=desktop_host");
    }
    let unix = std::env::var_os("CARGO_CFG_UNIX").is_some();
    // The consoles are `unix` to Rust but have no directory descriptors;
    // like the web, they hand the engine their file roots instead.
    let hosted = arch == "wasm32" || os == "vita" || os == "horizon";
    if hosted {
        println!("cargo::rustc-cfg=hosted_files");
    } else if unix {
        println!("cargo::rustc-cfg=fd_policy");
    }
}
