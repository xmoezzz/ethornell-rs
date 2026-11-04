use super::*;

pub(crate) fn drive_runtime_frame(
    runtime: &mut RuntimeEngine,
    pending_input_events: &mut VecDeque<RuntimeInputEvent>,
    input_script: &mut Option<HeadlessInputScript>,
    pacing: RuntimeFramePacing,
    elapsed_ms: u64,
    audio_elapsed_ms: u64,
    mut audio: Option<&mut AudioSystem>,
    frontend: &'static str,
) -> RuntimeFrameReport {
    let report = drive_frame(
        runtime,
        pending_input_events,
        input_script,
        pacing,
        elapsed_ms,
        audio_elapsed_ms,
    );
    while let Some(request) = runtime.api.audio_requests.pop_front() {
        execute_audio_command(audio.as_deref_mut(), request, frontend);
    }
    report
}

/// Strict host path. An absent backend is allowed only for an explicitly
/// silent session. On error, no success report is returned and remaining
/// commands stay queued; the host must fail/close rather than resume this VM.
pub(crate) fn drive_runtime_frame_checked(
    runtime: &mut RuntimeEngine,
    pending_input_events: &mut VecDeque<RuntimeInputEvent>,
    input_script: &mut Option<HeadlessInputScript>,
    pacing: RuntimeFramePacing,
    elapsed_ms: u64,
    audio_elapsed_ms: u64,
    mut audio: Option<&mut AudioSystem>,
    frontend: &'static str,
    audio_required: bool,
) -> Result<RuntimeFrameReport> {
    if audio_required && audio.is_none() {
        return Err(EthornellError::Other(
            "required audio backend unavailable".into(),
        ));
    }
    let report = drive_frame(
        runtime,
        pending_input_events,
        input_script,
        pacing,
        elapsed_ms,
        audio_elapsed_ms,
    );
    while let Some(request) = runtime.api.audio_requests.pop_front() {
        if !audio_required && audio.is_none() {
            continue;
        }
        audio_runtime::execute_audio_command_checked(audio.as_deref_mut(), request, frontend)?;
    }
    Ok(report)
}

fn drive_frame(
    runtime: &mut RuntimeEngine,
    pending_input_events: &mut VecDeque<RuntimeInputEvent>,
    input_script: &mut Option<HeadlessInputScript>,
    pacing: RuntimeFramePacing,
    elapsed_ms: u64,
    audio_elapsed_ms: u64,
) -> RuntimeFrameReport {
    // The target message pump processes the host messages accumulated before
    // the native tick. Consuming only one message per redraw can leave a
    // MouseDown/MouseUp edge stranded behind pointer motion for several VM
    // passes, so preserve FIFO order while draining the complete host queue.
    while let Some(event) = pending_input_events.pop_front() {
        apply_runtime_input_event(&mut runtime.api, event);
    }
    if let Some(script) = input_script.as_mut()
        && let Some(event) = script.tick()
    {
        apply_headless_input_event(&mut runtime.api, event);
    }

    // sub_48D480 times the update pass and keeps it pending (sub_401830(0));
    // the frontend's composition pass commits the sample.
    runtime.api.performance_profile.begin();
    let report = runtime.run_frame(pacing, elapsed_ms, audio_elapsed_ms);
    runtime.api.performance_profile.end(false);
    if let Some(restarted) = runtime.take_restart() {
        *runtime = restarted;
    }
    report
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn required_audio_failure_does_not_consume_input_or_advance_vm() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut runtime = RuntimeEngine {
            vm: ethornell_vm::Vm::new(),
            api: RuntimeTraceApi::new(manager),
            options: ethornell_vm::VmRunOptions::default(),
            last_report: None,
        };
        let mut inputs = VecDeque::from([RuntimeInputEvent::MouseMove { x: 4.0, y: 5.0 }]);
        let result = drive_runtime_frame_checked(
            &mut runtime,
            &mut inputs,
            &mut None,
            RuntimeFramePacing,
            16,
            16,
            None,
            "test",
            true,
        );
        assert!(result.is_err());
        assert_eq!(inputs.len(), 1);
        assert_eq!(runtime.api.engine_time_ms, 0);
    }
}
