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
    let mut requests =
        coalesce_channel_replacements(std::mem::take(&mut runtime.api.audio_requests));
    while let Some(request) = requests.pop_front() {
        if audio_runtime::execute_audio_command(audio.as_deref_mut(), request.clone(), frontend)
            .is_err()
            && audio.is_some()
        {
            requests.push_front(request);
            runtime.api.audio_requests = requests;
            break;
        }
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
    let mut requests =
        coalesce_channel_replacements(std::mem::take(&mut runtime.api.audio_requests));
    while let Some(request) = requests.pop_front() {
        if !audio_required && audio.is_none() {
            continue;
        }
        if let Err(error) = audio_runtime::execute_audio_command_checked(
            audio.as_deref_mut(),
            request.clone(),
            frontend,
        ) {
            requests.push_front(request);
            runtime.api.audio_requests = requests;
            return Err(error);
        }
    }
    Ok(report)
}

fn coalesce_channel_replacements(commands: VecDeque<AudioCommand>) -> VecDeque<AudioCommand> {
    let commands = commands.into_iter().collect::<Vec<_>>();
    let mut last_play = std::collections::BTreeMap::new();
    let mut last_stop = std::collections::BTreeMap::new();
    for (index, command) in commands.iter().enumerate() {
        match command {
            AudioCommand::Play { channel, .. } | AudioCommand::PlayStream { channel, .. } => {
                last_play.insert(*channel, index);
            }
            AudioCommand::Stop { channel, .. } => {
                last_stop.insert(*channel, index);
            }
            _ => {}
        }
    }

    let mut keep = vec![true; commands.len()];
    let channels = last_play
        .keys()
        .chain(last_stop.keys())
        .copied()
        .collect::<std::collections::BTreeSet<_>>();
    for channel in channels {
        let latest_play = last_play.get(&channel).copied();
        let latest_stop = last_stop.get(&channel).copied();
        let keep_play = latest_play.filter(|play| latest_stop.is_none_or(|stop| stop < *play));
        for (index, command) in commands.iter().enumerate() {
            match command {
                AudioCommand::Play {
                    channel: current, ..
                }
                | AudioCommand::PlayStream {
                    channel: current, ..
                } if *current == channel && Some(index) != keep_play => {
                    keep[index] = false;
                }
                AudioCommand::Stop {
                    channel: current, ..
                } if *current == channel && Some(index) != latest_stop => {
                    keep[index] = false;
                }
                _ => {}
            }
        }
    }
    commands
        .into_iter()
        .zip(keep)
        .filter_map(|(command, keep)| keep.then_some(command))
        .collect()
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

    fn test_play(channel: i32, file: &str) -> AudioCommand {
        AudioCommand::Play {
            asset: AudioAsset {
                archive: "bgm.arc".into(),
                file: file.into(),
                bytes: Vec::new(),
            },
            loop_asset: None,
            channel,
            looped: true,
            volume: 1.0,
            decode_gain: 1.0,
            playback_rate: 1.0,
            panning: 0.5,
            fade_ms: 0,
            restart: true,
        }
    }

    #[test]
    fn rapid_same_channel_replacements_keep_only_the_latest_track() {
        let requests = VecDeque::from([
            AudioCommand::Stop {
                channel: 1,
                fade_ms: 0,
            },
            test_play(1, "first"),
            AudioCommand::Stop {
                channel: 1,
                fade_ms: 0,
            },
            test_play(1, "latest"),
            test_play(2, "other-channel"),
        ]);

        let requests = coalesce_channel_replacements(requests);
        assert_eq!(requests.len(), 3);
        assert!(matches!(requests[0], AudioCommand::Stop { channel: 1, .. }));
        match &requests[1] {
            AudioCommand::Play { channel, asset, .. } => {
                assert_eq!(*channel, 1);
                assert_eq!(asset.file, "latest");
            }
            command => panic!("unexpected command: {command:?}"),
        }
        match &requests[2] {
            AudioCommand::Play { channel, asset, .. } => {
                assert_eq!(*channel, 2);
                assert_eq!(asset.file, "other-channel");
            }
            command => panic!("unexpected command: {command:?}"),
        }
    }

    #[test]
    fn a_final_stop_suppresses_queued_same_channel_plays() {
        let requests = coalesce_channel_replacements(VecDeque::from([
            test_play(1, "obsolete"),
            AudioCommand::Stop {
                channel: 1,
                fade_ms: 0,
            },
        ]));

        assert_eq!(requests.len(), 1);
        assert!(matches!(requests[0], AudioCommand::Stop { channel: 1, .. }));
    }

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
