use super::*;

#[test]
#[ignore = "requires ETHORNELL_REPRO_GAME and ETHORNELL_REPRO_BITMAPS=base,first,next"]
fn bitmap_reload_repro() {
    let root = PathBuf::from(std::env::var_os("ETHORNELL_REPRO_GAME").expect("game copy"));
    let names = std::env::var("ETHORNELL_REPRO_BITMAPS").expect("base,first,next resources");
    let names: Vec<_> = names.split(',').map(str::trim).collect();
    assert_eq!(names.len(), 3, "expected base,first,next resources");
    let manager = ResourceManager::open_game(&root).unwrap();
    let mut api = RuntimeTraceApi::new_with_native_root(manager, root);
    for face in &names[1..] {
        assert!(api.load_graph_image_resource(1, "", names[0]));
        assert!(api.load_graph_image_resource(2, "", face));
        assert!(api.composite_graph_bitmap(1, 2, 0, 0, 1, 0).is_some());
    }
    let reloaded = api.graph_bitmap_image(1).unwrap();
    assert!(api.load_graph_image_resource(3, "", names[0]));
    assert!(api.composite_graph_bitmap(3, 2, 0, 0, 1, 0).is_some());
    let fresh = api.graph_bitmap_image(3).unwrap();
    if let Some(path) = std::env::var_os("ETHORNELL_HEADLESS_SNAPSHOT") {
        let path = PathBuf::from(path);
        ethornell_image::write_rgba_png(&reloaded, &path).unwrap();
        let stem = path.file_stem().unwrap().to_string_lossy();
        ethornell_image::write_rgba_png(&fresh, &path.with_file_name(format!("{stem}-fresh.png")))
            .unwrap();
    }
    assert_eq!(
        (reloaded.width, reloaded.height),
        (fresh.width, fresh.height)
    );
    let changed = reloaded
        .rgba
        .chunks_exact(4)
        .zip(fresh.rgba.chunks_exact(4))
        .filter(|(actual, expected)| actual != expected)
        .count();
    assert_eq!(
        changed, 0,
        "reloading must match a fresh composition pixel for pixel"
    );
}

#[test]
#[ignore = "requires a writable copy of a game via ETHORNELL_REPRO_GAME"]
fn game_input_repro() {
    ethornell_core::init_tracing();
    let font = snapshot::load_default_font().unwrap();
    let _font_session = snapshot::enter_font_session(&font);
    let root = PathBuf::from(std::env::var_os("ETHORNELL_REPRO_GAME").expect("game copy"));
    let manager = ResourceManager::open_game(&root).unwrap();
    let bytes = manager
        .read_decoded_from_archive("system.arc", "ipl._bp")
        .unwrap();
    #[cfg(desktop_host)]
    let encoding = text_encoding::select(
        &manager,
        std::env::var("ETHORNELL_TEXT_ENCODING").ok().as_deref(),
    );
    #[cfg(not(desktop_host))]
    let encoding = encoding_rs::SHIFT_JIS;
    let mut runtime = RuntimeEngine::new(
        "system.arc:ipl._bp",
        &bytes,
        manager,
        root,
        false,
        false,
        false,
        encoding,
    );
    let mut script = HeadlessInputScript::from_env();
    let mut events = VecDeque::new();
    let pacing = RuntimeFramePacing::from_env();
    let frames = parse_usize_env("ETHORNELL_HEADLESS_FRAMES").unwrap_or(1000);
    let snapshot_path = std::env::var_os("ETHORNELL_HEADLESS_SNAPSHOT").map(PathBuf::from);
    let snapshot_frames: Vec<usize> = std::env::var("ETHORNELL_HEADLESS_SNAPSHOT_FRAME")
        .unwrap_or_default()
        .split(',')
        .filter_map(|frame| frame.trim().parse().ok())
        .collect();
    // Optional external batches keep the same game alive while a repro
    // navigates settings, holds Ctrl, or handles a route's choices.
    // Each JSON batch is {"frames":N,"script":"...","tick_ms":optional};
    // {"stop":true} exits. The default clock step is the native tick.
    let control = std::env::var_os("ETHORNELL_REPRO_CONTROL").map(PathBuf::from);
    let mut batch_frames = 0usize;
    let mut tick_ms = NATIVE_TICK_MS;
    for frame in 0..frames {
        if let Some(control) = control.as_ref() {
            if batch_frames == 0 || control.exists() {
                if let Some(path) = snapshot_path.as_ref() {
                    snapshot::write_runtime_snapshot(&runtime.api, path).unwrap();
                }
                eprintln!("control paused frame={frame}");
                let batch = loop {
                    if let Ok(source) = std::fs::read_to_string(control) {
                        let batch: serde_json::Value = serde_json::from_str(&source).unwrap();
                        std::fs::remove_file(control).unwrap();
                        break batch;
                    }
                    std::thread::sleep(std::time::Duration::from_millis(50));
                };
                if batch["stop"].as_bool() == Some(true) {
                    break;
                }
                batch_frames = batch["frames"].as_u64().expect("batch frames") as usize;
                assert!(batch_frames > 0);
                if let Some(value) = batch["tick_ms"].as_u64() {
                    assert!(value > 0);
                    tick_ms = value;
                }
                script = batch["script"].as_str().map(|source| {
                    HeadlessInputScript::parse(source).expect("batch input script")
                });
                eprintln!("control batch frame={frame} frames={batch_frames}");
            }
            batch_frames -= 1;
        }
        let report = runtime_frontend::drive_runtime_frame(
            &mut runtime,
            &mut events,
            &mut script,
            pacing,
            tick_ms,
            tick_ms,
            None,
            "repro",
        );
        if let Some(report) = report.last_report.as_ref() {
            assert!(!report.stop_reason.is_fatal(), "frame {frame}: {report:?}");
        }
        if frame % 100 == 0 {
            eprintln!("repro frame {frame}");
        }
        if snapshot_frames.contains(&frame)
            && let Some(path) = snapshot_path.as_ref()
        {
            let stem = path.file_stem().unwrap().to_string_lossy();
            let path = path.with_file_name(format!("{stem}-{frame}.png"));
            snapshot::write_runtime_snapshot(&runtime.api, &path).unwrap();
            eprintln!("repro snapshot frame={frame} path={}", path.display());
        }
    }
    if let Some(path) = snapshot_path {
        snapshot::write_runtime_snapshot(&runtime.api, &path).unwrap();
    }
    for (id, input) in &runtime.api.graph_input_objects {
        eprintln!("INPUT {id}: {input:?}");
    }
}
