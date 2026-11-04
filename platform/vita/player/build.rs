fn main() {
    if std::env::var("CARGO_CFG_TARGET_OS").as_deref() != Ok("vita") {
        return;
    }
    let out = std::path::PathBuf::from(std::env::var_os("OUT_DIR").unwrap());
    let mut shaders = String::new();
    for name in ["emote_v", "emote_f"] {
        let path = format!("shaders/{name}.gxp");
        println!("cargo:rerun-if-changed={path}");
        shaders.push_str(&format!(
            "static const unsigned char {name}[] __attribute__((aligned(16))) = {{"
        ));
        for b in std::fs::read(path).unwrap() {
            shaders.push_str(&format!("{b},"));
        }
        shaders.push_str("};\n");
    }
    std::fs::write(out.join("shaders.h"), shaders).unwrap();
    let sdk = std::env::var("VITASDK").unwrap_or_else(|_| "/usr/local/vitasdk".into());
    cc::Build::new()
        .compiler(format!("{sdk}/bin/arm-vita-eabi-gcc"))
        .archiver(format!("{sdk}/bin/arm-vita-eabi-ar"))
        .flag("-std=gnu17")
        .file("native/gpu.c")
        .include("../../console/include")
        .include(out)
        .compile("ethornell_vita_gpu");
    println!("cargo:rerun-if-changed=native/gpu.c");
    println!("cargo:rerun-if-changed=../../console/include/ethornell_console.h");
    for lib in [
        "vita2d",
        "SceGxm_stub",
        "SceDisplay_stub",
        "SceSysmodule_stub",
        "SceCommonDialog_stub",
        "SceAppUtil_stub",
        "SceAppMgr_stub",
        "SceSysmem_stub",
        "m",
    ] {
        println!("cargo:rustc-link-lib=static={lib}");
    }
    println!("cargo:rustc-link-arg=-Wl,--defsym=__sce_headroom=0x4000");
}
