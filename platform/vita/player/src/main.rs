#![cfg_attr(target_os = "vita", feature(alloc_error_hook))]
#[cfg(not(target_os = "vita"))]
fn main() {
    eprintln!("Build with cargo +nightly vita build vpk -- --release");
}
#[cfg(target_os = "vita")]
#[used]
#[unsafe(export_name = "_newlib_heap_size_user")]
pub static HEAP_SIZE: u32 = 320 * 1024 * 1024;
// newlib can have tens of MiB free in separate holes yet fail to provide a
// single 720p RGBA buffer. Use the separate CDRAM pool as a last resort for
// allocations that the primary heap cannot satisfy. The header records the owner, so Vec/Arc keep their
// normal ownership and every block is freed through its original allocator.
#[cfg(target_os = "vita")]
mod memory;
#[cfg(target_os = "vita")]
mod vita {
    use ethornell_console::*;
    use std::{
        ffi::CString,
        sync::{
            Arc,
            atomic::{AtomicBool, Ordering},
        },
        time::Instant,
    };
    use vitasdk_sys::*;
    unsafe extern "C" {
        fn ethornell_vita_gpu_init() -> i32;
        fn ethornell_vita_gpu_exit();
    }
    const ROOT: &str = "ux0:data/ethornell";
    fn log(text: &str) {
        use std::io::Write;
        let _ = std::fs::create_dir_all(ROOT);
        if let Ok(mut f) = std::fs::OpenOptions::new()
            .append(true)
            .create(true)
            .open(format!("{ROOT}/player.log"))
        {
            let _ = writeln!(f, "{text}");
        }
    }
    fn failure() {
        let mut text = [0i8; 2048];
        unsafe {
            ethornell_console_last_error(text.as_mut_ptr(), text.len());
        }
        log(&unsafe { std::ffi::CStr::from_ptr(text.as_ptr()) }.to_string_lossy());
    }
    struct Audio {
        running: Arc<AtomicBool>,
        worker: Option<std::thread::JoinHandle<()>>,
    }
    impl Audio {
        fn new() -> Result<Self, String> {
            let port = unsafe {
                sceAudioOutOpenPort(
                    SCE_AUDIO_OUT_PORT_TYPE_MAIN,
                    512,
                    48000,
                    SCE_AUDIO_OUT_MODE_STEREO,
                )
            };
            if port < 0 {
                return Err(format!("audio port {port:#x}"));
            }
            let running = Arc::new(AtomicBool::new(true));
            let flag = running.clone();
            let worker = std::thread::Builder::new()
                .name("ethornell-audio".into())
                .stack_size(128 * 1024)
                .spawn(move || {
                    let mut pcm = [0i16; 1024];
                    while flag.load(Ordering::Relaxed) {
                        unsafe {
                            ethornell_console_audio(pcm.as_mut_ptr(), 512);
                            if sceAudioOutOutput(port, pcm.as_ptr().cast()) < 0 {
                                break;
                            }
                        }
                    }
                    unsafe {
                        sceAudioOutReleasePort(port);
                    }
                })
                .map_err(|e| {
                    unsafe {
                        sceAudioOutReleasePort(port);
                    }
                    e.to_string()
                })?;
            Ok(Self {
                running,
                worker: Some(worker),
            })
        }
    }
    impl Drop for Audio {
        fn drop(&mut self) {
            self.running.store(false, Ordering::Relaxed);
            if let Some(worker) = self.worker.take() {
                let _ = worker.join();
            }
        }
    }
    #[repr(C)]
    struct MallInfo {
        arena: usize,
        ordblks: usize,
        smblks: usize,
        hblks: usize,
        hblkhd: usize,
        usmblks: usize,
        fsmblks: usize,
        used: usize,
        free: usize,
        keepcost: usize,
    }
    unsafe extern "C" {
        fn mallinfo() -> MallInfo;
    }
    static RESERVE: std::sync::Mutex<Option<Vec<u8>>> = std::sync::Mutex::new(None);
    static LAST_STATUS: std::sync::Mutex<String> = std::sync::Mutex::new(String::new());
    fn memory_status(stage: &str) {
        let h = unsafe { mallinfo() };
        let mut info = SceKernelFreeMemorySizeInfo {
            size: std::mem::size_of::<SceKernelFreeMemorySizeInfo>() as i32,
            size_user: 0,
            size_cdram: 0,
            size_phycont: 0,
        };
        let rc = unsafe { sceKernelGetFreeMemorySize(&mut info) };
        log(&format!(
            "{stage}: heap arena={} used={} free={} kernel-user={} cdram={} rc={rc:#x}",
            h.arena, h.used, h.free, info.size_user, info.size_cdram
        ));
    }
    pub fn run() {
        *RESERVE.lock().unwrap() = Some(vec![0u8; 128 * 1024]);
        std::alloc::set_alloc_error_hook(|layout| {
            if let Ok(mut reserve) = RESERVE.try_lock() {
                reserve.take();
            }
            memory_status(&format!("OOM allocating {} bytes", layout.size()));
            if let Ok(last) = LAST_STATUS.try_lock() {
                log(&last);
            }
        });
        memory_status("startup");
        log("starting Ethornell Vita (GXM)");
        let rc = unsafe { ethornell_vita_gpu_init() };
        if rc < 0 {
            log(&format!("GXM initialization failed: {rc}"));
            return;
        }
        let game = CString::new(format!("{ROOT}/game")).unwrap();
        let saves = CString::new(format!("{ROOT}/saves")).unwrap();
        let font = CString::new(format!("{ROOT}/font.ttf")).unwrap();
        let engine = unsafe {
            ethornell_console_create(game.as_ptr(), saves.as_ptr(), font.as_ptr(), 960, 544)
        };
        if engine.is_null() {
            failure();
            unsafe {
                ethornell_vita_gpu_exit();
            }
            return;
        }
        memory_status("after engine init");
        let audio = match Audio::new() {
            Ok(audio) => audio,
            Err(e) => {
                log(&e);
                unsafe {
                    ethornell_console_destroy(engine);
                    ethornell_vita_gpu_exit();
                }
                return;
            }
        };
        unsafe {
            sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
            sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
        }
        let mut panel: SceTouchPanelInfo = unsafe { std::mem::zeroed() };
        unsafe {
            sceTouchGetPanelInfo(SCE_TOUCH_PORT_FRONT, &mut panel);
        }
        let mut previous = 0u32;
        let mut pointer = [480f32, 272f32];
        let _ = ethornell_console_input(engine, 1, pointer[0], pointer[1], 0, 0.);
        let mut touched = false;
        let start = Instant::now();
        let mut last = start;
        let mut quit = false;
        let mut request = 0u64;
        let mut frame = 0u64;
        let mut perf_start = Instant::now();
        while !quit {
            if frame > 0 && frame % 300 == 0 {
                let seconds = perf_start.elapsed().as_secs_f64();
                log(&format!("performance: host_loops=300 elapsed_s={seconds:.3} loop_hz={:.2}", 300.0 / seconds));
                perf_start = Instant::now();
            }
            if frame < 120 || frame % 300 == 0 {
                let mut status = [0i8; 2048];
                unsafe {
                    ethornell_console_diagnostics(engine, status.as_mut_ptr(), status.len());
                }
                let status = unsafe { std::ffi::CStr::from_ptr(status.as_ptr()) }
                    .to_string_lossy()
                    .into_owned();
                *LAST_STATUS.lock().unwrap() = format!("frame={frame} {status}");
                if frame % 10 == 0 || frame % 300 == 0 {
                    log(&LAST_STATUS.lock().unwrap());
                    memory_status("tick");
                }
            }
            frame += 1;
            let now = Instant::now();
            let elapsed = now.duration_since(last).as_secs_f32().min(0.1);
            last = now;
            let mut pad: SceCtrlData = unsafe { std::mem::zeroed() };
            unsafe {
                sceCtrlPeekBufferPositive(0, &mut pad, 1);
            }
            let down = pad.buttons & !previous;
            let up = previous & !pad.buttons;
            if pad.buttons & (SCE_CTRL_START | SCE_CTRL_SELECT)
                == (SCE_CTRL_START | SCE_CTRL_SELECT)
            {
                let _ = ethornell_console_close(engine);
            }
            if request != 0 {
                if down & SCE_CTRL_CROSS != 0 {
                    let _ = ethornell_console_respond(engine, request, 1);
                    request = 0;
                } else if down & SCE_CTRL_CIRCLE != 0 {
                    let _ = ethornell_console_respond(engine, request, 0);
                    request = 0;
                }
            } else {
                let axis = |v: u8| ((v as f32 - 128.) / 127.).clamp(-1., 1.);
                let (mut dx, mut dy) = (0f32, 0f32);
                unsafe {
                    ethornell_console_stick(axis(pad.lx), axis(pad.ly), elapsed, 960., &mut dx, &mut dy);
                }
                let moved = [
                    (pointer[0] + dx).clamp(0., 959.),
                    (pointer[1] + dy).clamp(0., 543.),
                ];
                if moved != pointer {
                    pointer = moved;
                    let _ = ethornell_console_input(engine, 1, pointer[0], pointer[1], 0, 0.);
                }
                let mut touch: SceTouchData = unsafe { std::mem::zeroed() };
                unsafe {
                    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &mut touch, 1);
                }
                let active = touch.reportNum > 0;
                if active {
                    let p = &touch.report[0];
                    let pw = (panel.maxAaX - panel.minAaX).max(1) as f32;
                    let ph = (panel.maxAaY - panel.minAaY).max(1) as f32;
                    pointer = [
                        ((p.x - panel.minAaX) as f32 / pw * 959.).clamp(0., 959.),
                        ((p.y - panel.minAaY) as f32 / ph * 543.).clamp(0., 543.),
                    ];
                    let _ = ethornell_console_input(engine, 1, pointer[0], pointer[1], 0, 0.);
                }
                let left = active || pad.buttons & SCE_CTRL_CROSS != 0;
                let old_left = touched || previous & SCE_CTRL_CROSS != 0;
                if left != old_left {
                    let _ = ethornell_console_input(
                        engine,
                        if left { 2 } else { 3 },
                        pointer[0],
                        pointer[1],
                        1,
                        0.,
                    );
                }
                touched = active;
                for (button, descriptor) in [
                    (SCE_CTRL_UP, 38),
                    (SCE_CTRL_DOWN, 40),
                    (SCE_CTRL_LEFT, 37),
                    (SCE_CTRL_RIGHT, 39),
                    (SCE_CTRL_CIRCLE, 27),
                    (SCE_CTRL_SQUARE, 32),
                    (SCE_CTRL_TRIANGLE, 13),
                    (SCE_CTRL_LTRIGGER, 17),
                ] {
                    if down & button != 0 {
                        let _ = ethornell_console_input(engine, 4, 0., 0., descriptor, 0.);
                    }
                    if up & button != 0 {
                        let _ = ethornell_console_input(engine, 5, 0., 0., descriptor, 0.);
                    }
                }
            }
            previous = pad.buttons;
            if ethornell_console_step(
                engine,
                start.elapsed().as_nanos().min(u64::MAX as u128) as u64,
            ) != 0
            {
                log(&format!("engine step failed at frame={frame}"));
                memory_status("step failure");
                failure();
                break;
            }
            loop {
                let mut code = 0;
                let mut id = 0;
                let mut text = [0i8; 2048];
                let kind = unsafe {
                    ethornell_console_event(
                        engine,
                        &mut id,
                        &mut code,
                        text.as_mut_ptr(),
                        text.len(),
                    )
                };
                if kind < 0 {
                    failure();
                    quit = true;
                    break;
                }
                if kind == 0 {
                    break;
                }
                let text = unsafe { std::ffi::CStr::from_ptr(text.as_ptr()) }.to_string_lossy();
                match kind {
                    1 | 4 => log(&text),
                    2 => {
                        log(&format!("confirm (Cross=yes/Circle=no): {text}"));
                        request = id;
                    }
                    3 => {
                        log("engine requested exit");
                        quit = true;
                    }
                    5 => {
                        log(&format!(
                            "structured dialog unsupported by Vita shell: {text}"
                        ));
                        quit = true;
                    }
                    _ => {}
                }
            }
            std::thread::sleep(std::time::Duration::from_millis(1));
        }
        let mut status = [0i8; 2048];
        unsafe {
            ethornell_console_diagnostics(engine, status.as_mut_ptr(), status.len());
        }
        log(&format!("final frame={frame} {}", unsafe {
            std::ffi::CStr::from_ptr(status.as_ptr())
        }.to_string_lossy()));
        memory_status("before shutdown");
        log("stopping audio");
        drop(audio);
        log("destroying engine");
        unsafe {
            ethornell_console_destroy(engine);
            log("destroying GXM");
            ethornell_vita_gpu_exit();
        }
        log("stopped");
    }
}
#[cfg(target_os = "vita")]
fn main() {
    vita::run();
}
