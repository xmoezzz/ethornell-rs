use super::*;

#[cfg(test)]
const OWNED_A0_IDS: &[u16] = &[
    0x00, 0x08, 0x09, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1C, 0x20, 0x21, 0x22,
    0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2C, 0x2F, 0x80, 0x81, 0x84, 0x85, 0x86, 0xC0,
];

impl RuntimeTraceApi {
    pub(super) fn dispatch_native_sound(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group != 0xA0 {
            return None;
        }

        // BGI 1.622 sub_4984C0/498510/498560/4985B0 validate as
        // unsigned, even when no buffer is loaded. Parameter checks precede
        // channel checks (and consume no output values on failure).
        let channel_limit = match id {
            0x08 | 0x14 | 0x16 | 0x17 | 0x18 | 0x19 | 0x1C => Some(16),
            0x09 | 0x22 | 0x24 | 0x25 | 0x26 | 0x2C | 0x2F => Some(64),
            _ => None,
        };
        if let Some(limit) = channel_limit {
            let args = stack.iter().map(value_to_i32).collect::<Vec<_>>();
            let channel = args.first().copied().unwrap_or_default();
            let parameters: &[(&str, usize)] = match id {
                0x24 => &[("pan", 2), ("volume", 1)],
                0x17 => &[("pan", 1)],
                0x08 | 0x09 | 0x16 | 0x1C | 0x2C => &[("volume", 1)],
                _ => &[],
            };
            for &(name, index) in parameters {
                let value = args.get(index).copied().unwrap_or_default();
                if value as u32 > 128 {
                    // sub_498510 (pan) / sub_498560 (volume).
                    let message = if name == "pan" {
                        format!("無効なパンポット（定位） [ {value} ] が指定されました")
                    } else {
                        format!("無効なボリューム（音量） [ {value} ] が指定されました")
                    };
                    return Some(Err(ethornell_vm::VmError::Runtime(message)));
                }
            }
            if channel as u32 >= limit {
                // sub_4985B0 (BGM, 16) / sub_4984C0 (SE, 64).
                let message = if limit == 16 {
                    format!("無効な音楽チャンネル番号 [ {channel} ] が指定されました")
                } else {
                    format!("無効な効果音番号 [ {channel} ] が指定されました")
                };
                return Some(Err(ethornell_vm::VmError::Runtime(message)));
            }
        }

        let value = match id {
            // sub_487010 pushes the engine's fixed software channel count.
            0x00 => ethornell_vm::Value::Int(20),
            // sub_493B60/sub_493B90 update the primary BGM/SE gain banks.
            // The target retains a second independent bank at A0:1C/A0:2C.
            0x08 => {
                let volume = pop_int_value(stack).unwrap_or(128);
                let channel = pop_int_value(stack).unwrap_or_default();
                self.set_native_bgm_gain_bank(channel, volume, true);
                ethornell_vm::Value::None
            }
            0x09 => {
                let volume = pop_int_value(stack).unwrap_or(128);
                let channel = pop_int_value(stack).unwrap_or_default();
                self.set_native_sound_gain_bank(channel, volume, true);
                ethornell_vm::Value::None
            }
            // sub_4870B0 -> sub_493C00 installs one resident BGM buffer.
            // Loading never starts playback; A0:14 owns that transition.
            0x10 => {
                let volume = pop_int_value(stack).unwrap_or(128);
                let file = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_487BB0: volume, channel, then sub_4946F0's 12/14.
                if let Err(error) = target_checks::volume(volume)
                    .and_then(|()| target_checks::bgm_channel(channel))
                    .and_then(|()| self.native_bgm_asset_check(&[("", &file)], &file))
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_bgm_slot(channel, "", &file, false);
                self.set_native_bgm_play_volume(channel, volume);
                tracing::debug!(channel, file, volume, loaded, "SoundLoadNativeChannel");
                ethornell_vm::Value::None
            }
            // sub_487180 -> sub_493DB0 installs an archive-aware BGM buffer.
            // The target applies initial volume/pan but still waits for A0:14.
            0x11 => {
                let pan = pop_int_value(stack).unwrap_or(64);
                let volume = pop_int_value(stack).unwrap_or(128);
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_487C80: pan, volume, channel, then 12/14.
                if let Err(error) = target_checks::pan(pan)
                    .and_then(|()| target_checks::volume(volume))
                    .and_then(|()| target_checks::bgm_channel(channel))
                    .and_then(|()| {
                        self.native_bgm_asset_check(&[(&archive, &file)], &format!("{archive} : {file}"))
                    })
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_bgm_slot(channel, &archive, &file, true);
                self.set_native_bgm_play_volume(channel, volume);
                if let Some(slot) = self.bgm_slots.get_mut(&channel) {
                    slot.panning = native_audio_panning(pan);
                }
                tracing::info!(channel, archive, file, volume, pan, loaded, "SoundLoadBgm");
                ethornell_vm::Value::None
            }
            // sub_487280 -> sub_4940D0 -> sub_4A3810 loads the native
            // two-stream BGM form. The first stream plays once and the second
            // stream is the loop region when the mode is non-zero.
            0x12 => {
                let pan = pop_int_value(stack).unwrap_or(64);
                let volume = pop_int_value(stack).unwrap_or(128);
                let looped = pop_int_value(stack).unwrap_or_default() != 0;
                let loop_file = pop_string_value(stack).unwrap_or_default();
                let intro_file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_487D80: pan, volume, channel, then 12/14 for either
                // stream.
                let label = format!("{archive} : {intro_file} / {loop_file}");
                let loop_name = if loop_file.is_empty() { &intro_file } else { &loop_file };
                if let Err(error) = target_checks::pan(pan)
                    .and_then(|()| target_checks::volume(volume))
                    .and_then(|()| target_checks::bgm_channel(channel))
                    .and_then(|()| {
                        self.native_bgm_asset_check(
                            &[(&archive, &intro_file), (&archive, loop_name)],
                            &label,
                        )
                    })
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_bgm_pair(
                    channel,
                    &archive,
                    &intro_file,
                    &loop_file,
                    looped,
                    native_audio_panning(pan),
                );
                self.set_native_bgm_play_volume(channel, volume);
                tracing::info!(
                    channel,
                    archive,
                    intro_file,
                    loop_file,
                    looped,
                    volume,
                    pan,
                    loaded,
                    "SoundLoadBgmPair"
                );
                ethornell_vm::Value::None
            }
            // sub_4A3150 forwards pause/resume to the resident BGM buffer.
            0x14 => {
                // sub_4873C0: channel >= 16 is fatal (sub_497A40); sub_4A2AB0
                // ignores an unloaded channel and calls the stream's
                // vtable+12 with pause = (value == 0).
                let action = pop_int_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                if action == 0 {
                    if self.bgm_slots.contains_key(&channel) {
                        self.audio_requests.push_back(AudioCommand::Pause {
                            channel,
                            paused: true,
                            fade_ms: 0,
                        });
                        set_bgm_channel_active(&mut self.bgm_channel_active, channel, false);
                        self.set_bgm_clock_paused(channel, true);
                    }
                } else if self.play_loaded_native_bgm(channel) {
                    set_bgm_channel_active(&mut self.bgm_channel_active, channel, true);
                    self.start_bgm_clock(channel, 1.0);
                } else if self.bgm_slots.contains_key(&channel) {
                    // Once a resident buffer has consumed its restart latch,
                    // non-zero control resumes it.  An unknown channel must
                    // remain untouched, matching sub_4A3150's validation path.
                    self.audio_requests.push_back(AudioCommand::Pause {
                        channel,
                        paused: false,
                        fade_ms: 0,
                    });
                    set_bgm_channel_active(&mut self.bgm_channel_active, channel, true);
                    self.set_bgm_clock_paused(channel, false);
                }
                ethornell_vm::Value::None
            }
            // VM mediation handles the optional output cell. Keep this branch
            // for direct API callers and return the native completion status.
            0x15 => {
                let _state_destination = stack.pop();
                let channel = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.bgm_status(channel))
            }
            0x16 => {
                // sub_487450: volume > 128 (sub_4979F0) and channel >= 16
                // (sub_497A40) are fatal; sub_4A2AE0 ignores an unloaded
                // channel and otherwise ramps linearly from the current
                // volume to the new one over `duration` ms.
                let duration = pop_int_value(stack).unwrap_or_default();
                let volume = pop_int_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                if !self.bgm_slots.contains_key(&channel) {
                    return Some(Ok(ethornell_vm::Value::None));
                }
                let index = channel as usize;
                self.bgm_play_ramps[index] = Some(NativeSoundRamp::new(
                    self.native_sound_tick,
                    duration,
                    self.bgm_play_volumes[index],
                    volume as u8,
                ));
                ethornell_vm::Value::None
            }
            // sub_4A2C20 maps 0..128 around center 64 to DirectSound pan.
            0x17 => {
                let pan = pop_int_value(stack).unwrap_or(64);
                let channel = pop_int_value(stack).unwrap_or_default();
                let Some(slot) = self.bgm_slots.get_mut(&channel) else {
                    return Some(Ok(ethornell_vm::Value::None));
                };
                slot.panning = native_audio_panning(pan);
                self.audio_requests.push_back(AudioCommand::SetPanning {
                    channel,
                    panning: native_audio_panning(pan),
                    fade_ms: 0,
                });
                ethornell_vm::Value::None
            }
            // sub_4A29F0 and sub_4A2990 schedule fades to full/silent.
            0x18 => {
                let duration = pop_int_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                if self.bgm_slots.contains_key(&channel) {
                    let index = channel as usize;
                    self.bgm_fade_ramps[index] = Some(NativeSoundRamp::new(
                        self.native_sound_tick,
                        duration,
                        self.bgm_fade_volumes[index],
                        128,
                    ));
                }
                ethornell_vm::Value::None
            }
            0x19 => {
                let duration = pop_int_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                if self.bgm_slots.contains_key(&channel) {
                    let index = channel as usize;
                    self.bgm_fade_ramps[index] = Some(NativeSoundRamp::new(
                        self.native_sound_tick,
                        duration,
                        self.bgm_fade_volumes[index],
                        0,
                    ));
                }
                ethornell_vm::Value::None
            }
            // sub_4A2BB0 changes the independent secondary BGM gain bank.
            0x1C => {
                let volume = pop_int_value(stack).unwrap_or(128);
                let channel = pop_int_value(stack).unwrap_or_default();
                self.set_native_bgm_gain_bank(channel, volume, false);
                ethornell_vm::Value::None
            }
            // sub_4875A0/sub_487650/sub_487770/sub_487960 all construct
            // CProcLoadSound.  The fixed-point fields are decoder gain and
            // playback-rate scaling; they are not loop flags.
            0x20 => {
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_4984C0, then CProcLoadSound's BW and mono checks.
                if let Err(error) = target_checks::se_channel(channel)
                    .and_then(|()| self.native_se_asset_check(&archive, &file))
                {
                    return Some(Err(error));
                }
                let loaded =
                    self.load_native_sound_slot_from_archive(channel, &archive, &file, 1.0, 1.0, 0);
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::LoadSound,
                    i32::from(!loaded),
                );
                self.sound_load_process_handle(channel, loaded)
            }
            0x21 => {
                let decode_gain = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let native_start_parameter = pop_int_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_4984C0, then CProcLoadSound's BW and mono checks.
                if let Err(error) = target_checks::se_channel(channel)
                    .and_then(|()| self.native_se_asset_check(&archive, &file))
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_sound_slot_from_archive(
                    channel,
                    &archive,
                    &file,
                    decode_gain,
                    1.0,
                    native_start_parameter,
                );
                tracing::info!(
                    channel,
                    archive,
                    file,
                    native_start_parameter,
                    decode_gain,
                    loaded,
                    "SoundLoadSlotEx"
                );
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::LoadSound,
                    i32::from(!loaded),
                );
                self.sound_load_process_handle(channel, loaded)
            }
            // sub_487740 releases a resident static sound buffer.
            0x22 => {
                let channel = pop_int_value(stack).unwrap_or_default();
                self.release_native_sound_playback(channel);
                self.sound_slots.remove(&channel);
                ethornell_vm::Value::None
            }
            0x23 => {
                let decode_gain = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let native_start_parameter = pop_int_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_4984C0, then CProcLoadSound's BW and mono checks.
                if let Err(error) = target_checks::se_channel(channel)
                    .and_then(|()| self.native_se_asset_check(&archive, &file))
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_sound_slot_from_archive(
                    channel,
                    &archive,
                    &file,
                    decode_gain,
                    2.0,
                    native_start_parameter,
                );
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::LoadSound,
                    i32::from(!loaded),
                );
                self.sound_load_process_handle(channel, loaded)
            }
            // sub_487860 starts the resident buffer and returns its position.
            0x24 => {
                // sub_487860 pops pan, volume, channel and checks pan <= 128
                // (sub_4979A0), volume <= 128 (sub_4979F0) and channel < 0x40
                // (sub_497950). An unloaded channel pushes 0; otherwise the
                // SE restarts and the length in ms is pushed (sub_4943E0).
                let pan = pop_int_value(stack).unwrap_or(64);
                let volume = pop_int_value(stack).unwrap_or(128);
                let channel = pop_int_value(stack).unwrap_or_default();
                if !self.sound_slots.contains_key(&channel) {
                    return Some(Ok(ethornell_vm::Value::Int(0)));
                }
                // sub_4A4050 always stops/starts the resident wave and
                // cancels its fade; it never resumes the previous playback.
                self.play_native_sound_slot(channel, volume, pan);
                ethornell_vm::Value::Int(self.sound_length_ms(channel))
            }
            // sub_4878F0 stops without freeing the resident buffer.
            0x25 => {
                let channel = pop_int_value(stack).unwrap_or_default();
                if self.sound_slots.contains_key(&channel) {
                    self.release_native_sound_playback(channel);
                }
                ethornell_vm::Value::None
            }
            // sub_4A30C0 schedules a per-channel fade to silence.
            0x26 => {
                let duration = pop_int_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                if self.sound_slots.contains_key(&channel) {
                    let index = channel as usize;
                    self.sound_fade_ramps[index] = Some(NativeSoundRamp::new(
                        self.native_sound_tick,
                        duration,
                        self.sound_fade_volumes[index],
                        0,
                    ));
                }
                ethornell_vm::Value::None
            }
            0x27 => {
                let playback_rate = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let decode_gain = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let native_start_parameter = pop_int_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let channel = pop_int_value(stack).unwrap_or_default();
                // sub_4984C0, then CProcLoadSound's BW and mono checks.
                if let Err(error) = target_checks::se_channel(channel)
                    .and_then(|()| self.native_se_asset_check(&archive, &file))
                {
                    return Some(Err(error));
                }
                let loaded = self.load_native_sound_slot_from_archive(
                    channel,
                    &archive,
                    &file,
                    decode_gain,
                    playback_rate,
                    native_start_parameter,
                );
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::LoadSound,
                    i32::from(!loaded),
                );
                self.sound_load_process_handle(channel, loaded)
            }
            // sub_452530 constructs the dynamic registration process. Its
            // sound descriptor is VM-owned; retain the process identity.
            0x28 => {
                let playback_rate = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let decode_gain = native_audio_fixed(pop_int_value(stack).unwrap_or(65_536));
                let descriptor = pop_int_value(stack).unwrap_or_default();
                let source = stack.pop();
                let channel = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = target_checks::se_channel(channel) {
                    return Some(Err(error));
                }
                let valid = source.is_some();
                tracing::debug!(
                    channel,
                    descriptor,
                    decode_gain,
                    playback_rate,
                    valid,
                    "SoundRegisterDynamicBuffer"
                );
                call.complete_procedure(
                    ethornell_vm::native_call::NativeProcedureClass::RegisterSound,
                    i32::from(!valid),
                );
                ethornell_vm::Value::None
            }
            // sub_4A2890 changes the independent secondary SE gain bank.
            0x2C => {
                let volume = pop_int_value(stack).unwrap_or(128);
                let channel = pop_int_value(stack).unwrap_or_default();
                self.set_native_sound_gain_bank(channel, volume, false);
                ethornell_vm::Value::None
            }
            // sub_487BA0 -> sub_4943E0: the loaded SE's length in ms (from
            // the copied BURIKO wave header, scaled by the playback rate);
            // channel >= 0x40 is fatal.
            0x2F => {
                let channel = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.sound_length_ms(channel))
            }
            // sub_48D640 opens the target MCI CD-audio device and selects
            // TMSF mode. Portable hosts expose the same lifecycle through a
            // real virtual CD-DA device populated from track audio assets.
            0x80 => {
                let opened = self.open_native_cd_audio();
                tracing::info!(
                    opened,
                    tracks = self.native_cd_audio.tracks.len(),
                    "SoundOpenCdAudio"
                );
                ethornell_vm::Value::None
            }
            0x81 => {
                self.close_native_cd_audio();
                ethornell_vm::Value::None
            }
            0x84 => {
                // sub_487C00 pops the notify option first, then the one-based
                // TMSF track number passed to sub_48D850.
                let option = pop_int_value(stack).unwrap_or_default();
                let track = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(self.play_native_cd_track(track, option)))
            }
            0x85 => {
                self.stop_native_cd_audio();
                ethornell_vm::Value::None
            }
            // The VM owns the output pointer. Direct app-layer callers still
            // consume it and receive the target query-success boolean.
            0x86 => {
                let _destination = stack.pop();
                ethornell_vm::Value::Int(i32::from(self.query_native_cd_audio_mode().is_some()))
            }
            // sub_4944D0 is the legacy asynchronous PlaySound path.
            0xC0 => {
                let file = pop_string_value(stack).unwrap_or_default();
                let channel = 63;
                let loaded = self.load_native_sound_slot(channel, &file, 1.0, 1.0, 0);
                if loaded {
                    self.play_native_sound_slot(channel, 128, 64);
                }
                ethornell_vm::Value::Int(i32::from(loaded))
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    fn open_native_cd_audio(&mut self) -> bool {
        // Native sub_48D640 treats an already-open MCI device as success and
        // leaves its current state untouched.
        if self.native_cd_audio.opened {
            return true;
        }
        let tracks = discover_native_cd_audio_tracks(&self.manager);
        if tracks.is_empty() {
            self.native_cd_audio = NativeCdAudioState::default();
            return false;
        }
        self.native_cd_audio = NativeCdAudioState {
            opened: true,
            mode: 3, // MCI_MODE_STOP -> target public mode 3.
            current_track: None,
            notify_requested: false,
            tracks,
        };
        true
    }

    fn close_native_cd_audio(&mut self) -> bool {
        if !self.native_cd_audio.opened {
            return false;
        }
        self.audio_requests.push_back(AudioCommand::Stop {
            channel: NATIVE_CD_AUDIO_CHANNEL,
            fade_ms: 0,
        });
        self.native_cd_audio = NativeCdAudioState::default();
        true
    }

    fn play_native_cd_track(&mut self, track: i32, option: i32) -> bool {
        if !self.native_cd_audio.opened || self.native_cd_audio.tracks.is_empty() {
            return false;
        }
        // sub_48D850 truncates both FROM=track and TO=track+1 to one byte.
        let track = track as u8;
        self.native_cd_audio.current_track = Some(track);
        self.native_cd_audio.notify_requested = option != 0;
        let Some(asset) = self.native_cd_audio.tracks.get(&track).cloned() else {
            return false;
        };
        self.audio_requests.push_back(AudioCommand::Play {
            asset,
            loop_asset: None,
            channel: NATIVE_CD_AUDIO_CHANNEL,
            looped: false,
            volume: 1.0,
            decode_gain: 1.0,
            playback_rate: 1.0,
            panning: 0.5,
            fade_ms: 0,
            restart: true,
        });
        self.native_cd_audio.mode = 2; // MCI_MODE_PLAY.
        true
    }

    fn stop_native_cd_audio(&mut self) -> bool {
        if !self.native_cd_audio.opened {
            return false;
        }
        self.audio_requests.push_back(AudioCommand::Stop {
            channel: NATIVE_CD_AUDIO_CHANNEL,
            fade_ms: 0,
        });
        self.native_cd_audio.mode = 3; // MCI_MODE_STOP.
        true
    }

    pub(super) fn query_native_cd_audio_mode(&self) -> Option<i32> {
        self.native_cd_audio
            .opened
            .then_some(self.native_cd_audio.mode)
    }

    fn load_native_sound_slot(
        &mut self,
        channel: i32,
        file: &str,
        decode_gain: f64,
        playback_rate: f64,
        native_start_parameter: i32,
    ) -> bool {
        let Some(entry) = self.manager.find(file) else {
            return false;
        };
        let Ok(bytes) = self.manager.read_by_entry_decoded(&entry) else {
            return false;
        };
        let archive = entry
            .archive_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or_default()
            .to_string();
        self.sound_slots.insert(
            channel,
            SoundSlot {
                asset: AudioAsset {
                    archive,
                    file: file.to_string(),
                    bytes,
                },
                loop_asset: None,
                looped: false,
                decode_gain,
                playback_rate,
                panning: 0.5,
                native_start_parameter,
                needs_restart: true,
            },
        );
        self.set_native_sound_play_volume(channel, 128);
        self.set_native_sound_fade_volume(channel, 128);
        self.release_native_sound_playback(channel);
        true
    }

    fn load_native_bgm_slot(
        &mut self,
        channel: i32,
        archive: &str,
        file: &str,
        looped: bool,
    ) -> bool {
        let entry = if archive.is_empty() {
            self.manager.find(file)
        } else {
            find_runtime_resource(&self.manager, archive, file)
        };
        let Some(entry) = entry else {
            return false;
        };
        let Ok(bytes) = self.manager.read_by_entry_decoded(&entry) else {
            return false;
        };
        let resolved_archive = entry
            .archive_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or(archive)
            .to_string();
        self.bgm_slots.insert(
            channel,
            SoundSlot {
                asset: AudioAsset {
                    archive: resolved_archive,
                    file: file.to_string(),
                    bytes,
                },
                loop_asset: None,
                looped,
                decode_gain: 1.0,
                playback_rate: 1.0,
                panning: 0.5,
                native_start_parameter: 0,
                needs_restart: true,
            },
        );
        self.audio_requests.push_back(AudioCommand::Stop {
            channel,
            fade_ms: 0,
        });
        set_bgm_channel_active(&mut self.bgm_channel_active, channel, false);
        self.set_native_bgm_fade_volume(channel, 128);
        true
    }

    fn load_native_bgm_pair(
        &mut self,
        channel: i32,
        archive: &str,
        intro_file: &str,
        loop_file: &str,
        looped: bool,
        panning: f64,
    ) -> bool {
        let Some(intro) = self.load_native_audio_asset(archive, intro_file) else {
            return false;
        };
        let loop_file = if loop_file.is_empty() {
            intro_file
        } else {
            loop_file
        };
        let loop_asset = if loop_file.eq_ignore_ascii_case(intro_file) {
            None
        } else {
            let Some(asset) = self.load_native_audio_asset(archive, loop_file) else {
                return false;
            };
            Some(asset)
        };
        self.bgm_slots.insert(
            channel,
            SoundSlot {
                asset: intro,
                loop_asset,
                looped,
                decode_gain: 1.0,
                playback_rate: 1.0,
                panning,
                native_start_parameter: 0,
                needs_restart: true,
            },
        );
        self.audio_requests.push_back(AudioCommand::Stop {
            channel,
            fade_ms: 0,
        });
        set_bgm_channel_active(&mut self.bgm_channel_active, channel, false);
        self.set_native_bgm_fade_volume(channel, 128);
        true
    }

    /// sub_4A4B00 / sub_4A40E0: a missing file throws 12 ("BWファイル ..
    /// は存在しません"); data no decoder takes throws 14 ("BWファイルではない").
    /// Ogg and RIFF payloads without a BW header stay accepted.
    fn native_bgm_asset_check(
        &self,
        files: &[(&str, &str)],
        label: &str,
    ) -> ethornell_vm::VmResult<()> {
        for &(archive, file) in files {
            let Some(asset) = self.load_native_audio_asset(archive, file) else {
                return Err(ethornell_vm::VmError::Runtime(format!(
                    "指定されたBWファイル [ {label} ] は存在しません"
                )));
            };
            if asset.bytes.len() < 0x40
                && !matches!(
                    ethornell_audio::probe_audio(&asset.bytes).kind,
                    ethornell_audio::AudioKind::Ogg | ethornell_audio::AudioKind::RiffWave
                )
                || ethornell_audio::probe_audio(&asset.bytes).kind == ethornell_audio::AudioKind::Unknown
            {
                return Err(ethornell_vm::VmError::Runtime(format!(
                    "指定されたファイル [ {label} ] はBWファイルではないようです"
                )));
            }
        }
        Ok(())
    }

    /// CProcLoadSound (sub_43A3C0): data that is not a BW wave is a script
    /// error. A missing file is the load procedure's own failure.
    fn native_se_asset_check(&self, archive: &str, file: &str) -> ethornell_vm::VmResult<()> {
        let Some(asset) = self.load_native_audio_asset(archive, file) else {
            return Ok(());
        };
        let info = ethornell_audio::probe_audio(&asset.bytes);
        if info.kind == ethornell_audio::AudioKind::Unknown {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定されたファイル [ {archive} : {file} ] はBWファイルではないようです"
            )));
        }
        // The worker's "はモノラルではない" refusal (0x80000002) is not a
        // plain channel-count test: stereo Ogg (type 3) SE play in 1.622.
        Ok(())
    }

    fn load_native_audio_asset(&self, archive: &str, file: &str) -> Option<AudioAsset> {
        let entry = if archive.is_empty() {
            self.manager.find(file)
        } else {
            find_runtime_resource(&self.manager, archive, file)
        }?;
        let bytes = self.manager.read_by_entry_decoded(&entry).ok()?;
        let resolved_archive = entry
            .archive_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or(archive)
            .to_string();
        Some(AudioAsset {
            archive: resolved_archive,
            file: file.to_string(),
            bytes,
        })
    }

    fn set_native_bgm_gain_bank(&mut self, channel: i32, volume: i32, primary: bool) {
        let Some(index) = usize::try_from(channel)
            .ok()
            .filter(|index| *index < self.bgm_primary_volumes.len())
        else {
            return;
        };
        let volume = volume.clamp(0, 128) as u8;
        if primary {
            self.bgm_primary_volumes[index] = volume;
        } else {
            self.bgm_secondary_volumes[index] = volume;
        }
        if self.bgm_slots.contains_key(&channel) {
            self.apply_native_bgm_volume(channel);
        }
    }

    fn set_native_bgm_play_volume(&mut self, channel: i32, volume: i32) {
        if let Some(slot) = usize::try_from(channel)
            .ok()
            .and_then(|index| self.bgm_play_volumes.get_mut(index))
        {
            *slot = volume.clamp(0, 128) as u8;
            self.bgm_play_ramps[channel as usize] = None;
        }
    }

    fn set_native_bgm_fade_volume(&mut self, channel: i32, volume: i32) {
        if let Some(slot) = usize::try_from(channel)
            .ok()
            .and_then(|index| self.bgm_fade_volumes.get_mut(index))
        {
            *slot = volume.clamp(0, 128) as u8;
            self.bgm_fade_ramps[channel as usize] = None;
        }
    }

    fn native_bgm_effective_volume(&self, channel: i32) -> f64 {
        let Some(index) = usize::try_from(channel)
            .ok()
            .filter(|index| *index < self.bgm_primary_volumes.len())
        else {
            return 0.0;
        };
        native_mixer_volume([
            // sub_494F50 applies 0 to every primary bank while minimized.
            if self.sound_minimize_muted { 0 } else { self.bgm_primary_volumes[index] },
            self.bgm_secondary_volumes[index],
            self.bgm_play_volumes[index],
            self.bgm_fade_volumes[index],
        ])
    }

    /// sub_494F50 (muted) / sub_4946B0 (restored): the primary BGM and SE
    /// banks apply 0 while the window is minimized and the stored values
    /// again afterwards; A0:08/A0:09 keep storing in between.
    pub(super) fn set_sound_minimize_muted(&mut self, muted: bool) {
        if self.sound_minimize_muted == muted {
            return;
        }
        self.sound_minimize_muted = muted;
        let bgm = self.bgm_slots.keys().copied().collect::<Vec<_>>();
        for channel in bgm {
            self.apply_native_bgm_volume(channel);
        }
        let sounds = self.sound_slots.keys().copied().collect::<Vec<_>>();
        for channel in sounds {
            self.apply_native_sound_volume(channel);
        }
    }

    fn apply_native_bgm_volume(&mut self, channel: i32) {
        let volume = self.native_bgm_effective_volume(channel);
        self.audio_requests.push_back(AudioCommand::SetVolume {
            channel,
            volume,
            fade_ms: 0,
        });
    }

    fn set_native_sound_gain_bank(&mut self, channel: i32, volume: i32, primary: bool) {
        let Some(index) = usize::try_from(channel)
            .ok()
            .filter(|index| *index < self.sound_primary_volumes.len())
        else {
            return;
        };
        let volume = volume.clamp(0, 128) as u8;
        if primary {
            self.sound_primary_volumes[index] = volume;
        } else {
            self.sound_secondary_volumes[index] = volume;
        }
        if self.sound_slots.contains_key(&channel) {
            self.apply_native_sound_volume(channel);
        }
    }

    fn set_native_sound_play_volume(&mut self, channel: i32, volume: i32) {
        if let Some(slot) = usize::try_from(channel)
            .ok()
            .and_then(|index| self.sound_play_volumes.get_mut(index))
        {
            *slot = volume.clamp(0, 128) as u8;
        }
    }

    fn set_native_sound_fade_volume(&mut self, channel: i32, volume: i32) {
        if let Some(slot) = usize::try_from(channel)
            .ok()
            .and_then(|index| self.sound_fade_volumes.get_mut(index))
        {
            *slot = volume.clamp(0, 128) as u8;
            self.sound_fade_ramps[channel as usize] = None;
        }
    }

    fn native_sound_volume_factors(&self, channel: i32) -> [u8; 4] {
        let Some(index) = usize::try_from(channel)
            .ok()
            .filter(|index| *index < self.sound_primary_volumes.len())
        else {
            return [0; 4];
        };
        [
            if self.sound_minimize_muted { 0 } else { self.sound_primary_volumes[index] },
            self.sound_secondary_volumes[index],
            self.sound_play_volumes[index],
            self.sound_fade_volumes[index],
        ]
    }

    fn native_sound_effective_volume(&self, channel: i32) -> f64 {
        native_mixer_volume(self.native_sound_volume_factors(channel))
    }

    fn apply_native_sound_volume(&mut self, channel: i32) {
        let volume = self.native_sound_effective_volume(channel);
        self.audio_requests.push_back(AudioCommand::SetVolume {
            channel: native_se_audio_channel(channel),
            volume,
            fade_ms: 0,
        });
    }

    fn play_loaded_native_bgm(&mut self, channel: i32) -> bool {
        let Some((sound, restart)) = self.bgm_slots.get_mut(&channel).map(|sound| {
            let restart = std::mem::take(&mut sound.needs_restart);
            (sound.clone(), restart)
        }) else {
            return false;
        };
        if !restart {
            return false;
        }
        let volume = self.native_bgm_effective_volume(channel);
        self.audio_requests.push_back(AudioCommand::Play {
            asset: sound.asset,
            loop_asset: sound.loop_asset,
            channel,
            looped: sound.looped,
            volume,
            decode_gain: sound.decode_gain,
            playback_rate: sound.playback_rate,
            panning: sound.panning,
            fade_ms: 0,
            restart,
        });
        true
    }

    pub(super) fn register_native_sound_memory(
        &mut self,
        channel: i32,
        block: &[u8],
        native_start_parameter: i32,
        decode_gain: f64,
        playback_rate: f64,
    ) -> bool {
        if !(0..64).contains(&channel) || block.len() < 64 {
            return false;
        }
        self.sound_slots.insert(
            channel,
            SoundSlot {
                asset: AudioAsset {
                    archive: "<vm-memory>".to_string(),
                    file: format!("SoundA0_28_{channel}"),
                    bytes: block.to_vec(),
                },
                loop_asset: None,
                looped: false,
                decode_gain,
                playback_rate,
                panning: 0.5,
                native_start_parameter,
                needs_restart: true,
            },
        );
        self.set_native_sound_play_volume(channel, 128);
        self.set_native_sound_fade_volume(channel, 128);
        self.release_native_sound_playback(channel);
        true
    }

    fn load_native_sound_slot_from_archive(
        &mut self,
        channel: i32,
        archive: &str,
        file: &str,
        decode_gain: f64,
        playback_rate: f64,
        native_start_parameter: i32,
    ) -> bool {
        let Some(entry) = find_runtime_resource(&self.manager, archive, file) else {
            return false;
        };
        let Ok(bytes) = self.manager.read_by_entry_decoded(&entry) else {
            return false;
        };
        self.sound_slots.insert(
            channel,
            SoundSlot {
                asset: AudioAsset {
                    archive: archive.to_string(),
                    file: file.to_string(),
                    bytes,
                },
                loop_asset: None,
                looped: false,
                decode_gain,
                playback_rate,
                panning: 0.5,
                native_start_parameter,
                needs_restart: true,
            },
        );
        self.set_native_sound_play_volume(channel, 128);
        self.set_native_sound_fade_volume(channel, 128);
        self.release_native_sound_playback(channel);
        true
    }

    fn release_native_sound_playback(&mut self, channel: i32) {
        self.audio_requests.push_back(AudioCommand::Stop {
            channel: native_se_audio_channel(channel),
            fade_ms: 0,
        });
        set_sound_channel_active(&mut self.sound_channel_active, channel, false);
        self.stop_sound_clock(channel);
    }

    fn sound_load_process_handle(&mut self, channel: i32, loaded: bool) -> ethornell_vm::Value {
        let handle = self.alloc_object();
        self.graph_process_handles.insert(handle);
        tracing::debug!(channel, handle, loaded, "SoundLoadProcess");
        ethornell_vm::Value::Int(handle)
    }

    fn play_native_sound_slot(&mut self, channel: i32, volume: i32, pan: i32) {
        if !self.sound_slots.contains_key(&channel) {
            return;
        }
        self.set_native_sound_play_volume(channel, volume);
        self.set_native_sound_fade_volume(channel, 128);
        let Some(sound) = self.sound_slots.get_mut(&channel).map(|sound| {
            sound.needs_restart = false;
            sound.panning = native_audio_panning(pan);
            sound.clone()
        }) else {
            return;
        };
        let effective_volume = self.native_sound_effective_volume(channel);
        self.audio_requests.push_back(AudioCommand::Play {
            asset: sound.asset,
            loop_asset: sound.loop_asset,
            channel: native_se_audio_channel(channel),
            looped: sound.looped,
            volume: effective_volume,
            decode_gain: sound.decode_gain,
            playback_rate: sound.playback_rate,
            panning: sound.panning,
            fade_ms: 0,
            restart: true,
        });
        set_sound_channel_active(&mut self.sound_channel_active, channel, true);
        self.start_sound_clock(channel, sound.playback_rate);
    }

    pub(super) fn advance_audio_clocks(&mut self, elapsed_ms: u64) {
        // Envelopes use the unnormalized host tick (GetTickCount), including
        // pause gaps. Playback clock capping below preserves its existing ABI.
        self.native_sound_tick = self.native_sound_tick.wrapping_add(elapsed_ms as u32);
        self.poll_native_sound_envelopes();
        for clock in &mut self.bgm_playback_clocks {
            clock.advance(elapsed_ms.min(500));
        }
        for clock in &mut self.sound_playback_clocks {
            clock.advance(elapsed_ms.min(500));
        }
    }

    /// sub_4A2E70: poll both envelopes before composing gain. Stopped
    /// resident buffers still participate; reaching zero does not stop them.
    fn poll_native_sound_envelopes(&mut self) {
        for channel in 0..16 {
            if !self.bgm_slots.contains_key(&(channel as i32)) {
                continue;
            }
            let play = poll_sound_ramp(
                &mut self.bgm_play_ramps[channel],
                &mut self.bgm_play_volumes[channel],
                self.native_sound_tick,
            );
            let fade = poll_sound_ramp(
                &mut self.bgm_fade_ramps[channel],
                &mut self.bgm_fade_volumes[channel],
                self.native_sound_tick,
            );
            if play || fade {
                self.apply_native_bgm_volume(channel as i32);
            }
        }
        for channel in 0..64 {
            if !self.sound_slots.contains_key(&(channel as i32)) {
                continue;
            }
            if poll_sound_ramp(
                &mut self.sound_fade_ramps[channel],
                &mut self.sound_fade_volumes[channel],
                self.native_sound_tick,
            ) {
                self.apply_native_sound_volume(channel as i32);
            }
        }
    }

    fn start_bgm_clock(&mut self, channel: i32, playback_rate: f64) {
        if let Some(clock) = native_clock_mut(&mut self.bgm_playback_clocks, channel) {
            clock.start(playback_rate);
        }
    }

    fn set_bgm_clock_paused(&mut self, channel: i32, paused: bool) {
        if let Some(clock) = native_clock_mut(&mut self.bgm_playback_clocks, channel) {
            if paused {
                clock.pause();
            } else {
                clock.resume();
            }
        }
    }

    pub(super) fn bgm_position(&self, channel: i32) -> i32 {
        native_clock(&self.bgm_playback_clocks, channel)
            .filter(|clock| clock.is_running())
            .map(NativeAudioClock::position_ms)
            .unwrap_or_default()
    }

    pub(super) fn bgm_status(&self, channel: i32) -> i32 {
        i32::from(
            usize::try_from(channel)
                .ok()
                .and_then(|index| self.bgm_channel_active.get(index))
                .is_none_or(|active| !*active),
        )
    }

    fn start_sound_clock(&mut self, channel: i32, playback_rate: f64) {
        if let Some(clock) = native_clock_mut(&mut self.sound_playback_clocks, channel) {
            clock.start(playback_rate);
        }
    }

    fn stop_sound_clock(&mut self, channel: i32) {
        if let Some(clock) = native_clock_mut(&mut self.sound_playback_clocks, channel) {
            clock.stop();
        }
    }

    /// sub_4943E0: record +0x0C (samples) * 1000 / record +0x10 (rate)
    /// times the fixed 65536 / playback-rate scale, then >> 16. The record
    /// is the 64-byte BURIKO wave header copied at load (sub_494300).
    fn sound_length_ms(&self, channel: i32) -> i32 {
        let Some(slot) = self.sound_slots.get(&channel) else {
            return 0;
        };
        let info = ethornell_audio::probe_audio(&slot.asset.bytes);
        let (Some(samples), Some(frequency)) = (info.sample_len, info.frequency) else {
            return 0;
        };
        if frequency == 0 {
            return 0;
        }
        let scale = (65536.0 / slot.playback_rate) as i64 as u32;
        let value = f64::from(samples) * 1000.0 / f64::from(frequency) * f64::from(scale);
        ((value as i64 as u32) >> 16) as i32
    }

}

const NATIVE_CD_AUDIO_CHANNEL: i32 = 0x1FE;
const NATIVE_CD_AUDIO_EXTENSIONS: &[&str] = &["ogg", "wav"];

fn discover_native_cd_audio_tracks(
    manager: &ethornell_archive::ResourceManager,
) -> std::collections::BTreeMap<u8, AudioAsset> {
    if file_policy::restricted() {return std::collections::BTreeMap::new();}
    let mut tracks = std::collections::BTreeMap::new();
    let root = game_root_path(manager);
    let mut directories = Vec::new();
    if !file_policy::restricted() && let Some(path) = std::env::var_os("ETHORNELL_CD_AUDIO_ROOT") {
        directories.push(std::path::PathBuf::from(path));
    }
    for relative in ["cdda", "CDDA", "cd", "CD", "audio/cdda", "Audio/CDDA"] {
        directories.push(root.join(relative));
    }
    directories.sort();
    directories.dedup();

    for directory in directories {
        let Ok(entries) = std::fs::read_dir(&directory) else {
            continue;
        };
        let mut files = entries
            .flatten()
            .map(|entry| entry.path())
            .filter(|path| path.is_file())
            .collect::<Vec<_>>();
        files.sort();
        for path in files {
            let extension = path
                .extension()
                .and_then(|extension| extension.to_str())
                .map(str::to_ascii_lowercase)
                .unwrap_or_default();
            if !NATIVE_CD_AUDIO_EXTENSIONS.contains(&extension.as_str()) {
                continue;
            }
            let Some(track) = path
                .file_stem()
                .and_then(|stem| stem.to_str())
                .and_then(parse_native_cd_track_number)
            else {
                continue;
            };
            let bytes = if let Some(policy)=file_policy::active() {
                let Some(rel)=path.strip_prefix(&policy.resource).ok() else {continue;};
                policy.read_resource(&rel.to_string_lossy(), 128*1024*1024)
            } else {std::fs::read(&path)};
            let Ok(bytes) = bytes else {
                continue;
            };
            if !is_native_cd_audio_payload(&bytes) {
                continue;
            }
            tracks.entry(track).or_insert_with(|| AudioAsset {
                archive: directory.to_string_lossy().into_owned(),
                file: path
                    .file_name()
                    .and_then(|name| name.to_str())
                    .unwrap_or_default()
                    .to_string(),
                bytes,
            });
        }
    }

    // Also accept packed virtual CD tracks. This keeps old games that shipped
    // extracted CD-DA replacements inside an archive usable without requiring
    // a physical optical drive. Loose files win over archive entries.
    let mut resources = manager.list();
    resources.sort_by(|left, right| {
        left.archive_path
            .cmp(&right.archive_path)
            .then_with(|| left.entry_name.cmp(&right.entry_name))
    });
    for entry in resources {
        let Some(track) = parse_native_cd_track_resource_name(&entry.entry_name) else {
            continue;
        };
        if tracks.contains_key(&track) {
            continue;
        }
        let Ok(bytes) = manager.read_by_entry_decoded(&entry) else {
            continue;
        };
        if !is_native_cd_audio_payload(&bytes) {
            continue;
        }
        tracks.insert(
            track,
            AudioAsset {
                archive: entry
                    .archive_path
                    .file_name()
                    .and_then(|name| name.to_str())
                    .unwrap_or_default()
                    .to_string(),
                file: entry.entry_name,
                bytes,
            },
        );
    }
    tracks
}

fn parse_native_cd_track_resource_name(name: &str) -> Option<u8> {
    let normalized = name.replace('\\', "/");
    let leaf = normalized.rsplit('/').next().unwrap_or(&normalized);
    let stem = leaf.rsplit_once('.').map_or(leaf, |(stem, _)| stem);
    let lower = stem.trim().to_ascii_lowercase();
    if !["track", "cdda", "audio", "cd"]
        .into_iter()
        .any(|prefix| lower.starts_with(prefix))
    {
        return None;
    }
    parse_native_cd_track_number(stem)
}

fn is_native_cd_audio_payload(bytes: &[u8]) -> bool {
    !matches!(
        ethornell_audio::probe_audio(bytes).kind,
        ethornell_audio::AudioKind::Unknown | ethornell_audio::AudioKind::BurikoWaveBoxUnknown
    )
}

fn parse_native_cd_track_number(stem: &str) -> Option<u8> {
    let lower = stem.trim().to_ascii_lowercase();
    let digits = ["track", "cdda", "audio", "cd"]
        .into_iter()
        .find_map(|prefix| {
            lower
                .strip_prefix(prefix)
                .map(|suffix| suffix.trim_start_matches(['_', '-', ' ']))
        })
        .unwrap_or(lower.as_str());
    if digits.is_empty() || !digits.bytes().all(|byte| byte.is_ascii_digit()) {
        return None;
    }
    let track = digits.parse::<u16>().ok()?;
    (1..=u16::from(u8::MAX))
        .contains(&track)
        .then_some(track as u8)
}

fn poll_sound_ramp(ramp: &mut Option<NativeSoundRamp>, volume: &mut u8, now: u32) -> bool {
    let Some(active) = *ramp else {
        return false;
    };
    let (sample, done) = active.sample(now);
    *volume = sample;
    if done {
        *ramp = None;
    }
    true
}

/// Mirrors sub_4A21C0's four-factor attenuation composition, then converts
/// the target 0..=128 attenuation domain to the portable backend's 0.0..=1.0
/// gain domain. Each native factor is independently clamped before combining.
fn native_mixer_volume(factors: [u8; 4]) -> f64 {
    let deficits = factors.map(|value| 128_u32.saturating_sub(u32::from(value.min(128))));
    if deficits
        .into_iter()
        .any(|deficit| f64::from(deficit) / 2.666_666_66 >= 48.0)
    {
        return 0.0;
    }
    let deficit = deficits.into_iter().sum::<u32>();
    let attenuation = ((f64::from(deficit) / 2.666_666_66) as i32).clamp(0, 128);
    f64::from(128 - attenuation) / 128.0
}

fn native_audio_fixed(value: i32) -> f64 {
    f64::from(value) / 65_536.0
}

fn native_se_audio_channel(channel: i32) -> i32 {
    0x100 + channel
}

fn set_bgm_channel_active(active: &mut [bool; 16], channel: i32, value: bool) {
    if let Some(slot) = usize::try_from(channel)
        .ok()
        .and_then(|index| active.get_mut(index))
    {
        *slot = value;
    }
}

fn native_clock<const N: usize>(
    clocks: &[NativeAudioClock; N],
    channel: i32,
) -> Option<NativeAudioClock> {
    usize::try_from(channel)
        .ok()
        .and_then(|index| clocks.get(index))
        .copied()
}

fn native_clock_mut<const N: usize>(
    clocks: &mut [NativeAudioClock; N],
    channel: i32,
) -> Option<&mut NativeAudioClock> {
    usize::try_from(channel)
        .ok()
        .and_then(|index| clocks.get_mut(index))
}

#[cfg(test)]
mod tests {
    use super::super::{AudioCommand, RuntimeTraceApi};
    use super::{
        OWNED_A0_IDS, native_audio_fixed, native_mixer_volume, native_se_audio_channel,
        parse_native_cd_track_number,
    };
    use ethornell_vm::{NativeCallFrame, NativeOpcode, SoundApi, Value};

    fn api_with_resident_channel() -> RuntimeTraceApi {
        let manager =
            ethornell_archive::ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let mut header = [0u8; 64];
        header[0..4].copy_from_slice(&64u32.to_le_bytes());
        header[4..8].copy_from_slice(b"bw  ");
        header[12..16].copy_from_slice(&44_100u32.to_le_bytes());
        header[16..20].copy_from_slice(&44_100u32.to_le_bytes());
        assert!(api.register_native_sound_memory(3, &header, 0, 1.0, 1.0));
        api.bgm_slots.insert(3, api.sound_slots[&3].clone());
        api.audio_requests.clear();
        api
    }

    fn sound_call(
        api: &mut RuntimeTraceApi,
        id: u16,
        args: &[i32],
    ) -> ethornell_vm::VmResult<Value> {
        let mut call = NativeCallFrame::new(
            NativeOpcode { group: 0xA0, id },
            args.iter().copied().map(Value::Int).collect(),
        );
        let result = SoundApi::call_sound(api, &mut call);
        if result.is_ok() {
            assert!(call.args().is_empty());
        }
        result
    }

    #[test]
    fn sound_controls_reject_unsigned_out_of_range_before_side_effects() {
        let mut api = api_with_resident_channel();
        for (id, args) in [
            (0x08, vec![-1, 128]),
            (0x08, vec![16, 128]),
            (0x08, vec![0, -1]),
            (0x09, vec![64, 128]),
            (0x09, vec![0, 129]),
            (0x14, vec![16, 1]),
            (0x16, vec![16, 128, 100]),
            (0x17, vec![0, -1]),
            (0x17, vec![16, 64]),
            (0x18, vec![-1, 100]),
            (0x19, vec![16, 100]),
            (0x1C, vec![0, 129]),
            (0x22, vec![64]),
            (0x25, vec![-1]),
            (0x25, vec![64]),
            (0x26, vec![64, 100]),
            (0x2C, vec![0, -1]),
        ] {
            assert!(
                sound_call(&mut api, id, &args).is_err(),
                "A0:{id:02X} {args:?}"
            );
            assert!(api.audio_requests.is_empty());
            assert!(api.sound_slots.contains_key(&3));
        }
        let error = sound_call(&mut api, 0x17, &[16, 129])
            .unwrap_err()
            .to_string();
        assert!(
            error.contains("パンポット"),
            "parameter validation must precede channel validation: {error}"
        );
    }

    #[test]
    fn se_stop_keeps_the_buffer_and_play_always_restarts_and_cancels_fade() {
        let mut api = api_with_resident_channel();
        assert_eq!(
            sound_call(&mut api, 0x24, &[3, 128, 64]).unwrap(),
            Value::Int(1000)
        );
        sound_call(&mut api, 0x26, &[3, 100]).unwrap();
        api.advance_audio_clocks(50);
        assert_eq!(api.sound_fade_volumes[3], 64);
        sound_call(&mut api, 0x25, &[3]).unwrap();
        assert!(api.sound_slots.contains_key(&3));
        assert!(api.sound_fade_ramps[3].is_some()); // stop does not cancel fade
        assert!(!api.sound_channel_active[3]);
        assert_eq!(sound_call(&mut api, 0x2F, &[3]).unwrap(), Value::Int(1000));
        for _ in 0..2 {
            api.audio_requests.clear();
            sound_call(&mut api, 0x24, &[3, 100, 0]).unwrap();
            assert_eq!(api.sound_fade_volumes[3], 128);
            assert!(api.sound_fade_ramps[3].is_none());
            assert_eq!(api.sound_playback_clocks[3].position_ms(), 0);
            assert!(matches!(
                api.audio_requests.back(),
                Some(AudioCommand::Play {
                    restart: true,
                    panning: 0.0,
                    ..
                })
            ));
            api.advance_audio_clocks(20);
            assert_eq!(api.sound_playback_clocks[3].position_ms(), 20);
        }
        sound_call(&mut api, 0x22, &[3]).unwrap();
        assert_eq!(sound_call(&mut api, 0x2F, &[3]).unwrap(), Value::Int(0));
    }

    #[test]
    fn sound_envelopes_compose_independently_and_retarget_from_last_sample() {
        let mut api = api_with_resident_channel();
        sound_call(&mut api, 0x16, &[3, 0, 100]).unwrap();
        sound_call(&mut api, 0x19, &[3, 200]).unwrap();
        assert!(api.audio_requests.is_empty()); // deferred until sound poll
        assert_eq!(
            (api.bgm_play_volumes[3], api.bgm_fade_volumes[3]),
            (128, 128)
        );
        api.advance_audio_clocks(50);
        assert_eq!((api.bgm_play_volumes[3], api.bgm_fade_volumes[3]), (64, 96));
        sound_call(&mut api, 0x08, &[3, 100]).unwrap();
        assert!(api.bgm_play_ramps[3].is_some());
        sound_call(&mut api, 0x18, &[3, 100]).unwrap();
        api.advance_audio_clocks(50);
        assert_eq!((api.bgm_play_volumes[3], api.bgm_fade_volumes[3]), (0, 112));
        api.advance_audio_clocks(50);
        assert_eq!(api.bgm_fade_volumes[3], 128);
        assert!(api.bgm_play_ramps[3].is_none() && api.bgm_fade_ramps[3].is_none());
        sound_call(&mut api, 0x16, &[3, 128, 0]).unwrap();
        assert_eq!(api.bgm_play_volumes[3], 0);
        api.advance_audio_clocks(0);
        assert_eq!(api.bgm_play_volumes[3], 128);
        sound_call(&mut api, 0x26, &[3, -1]).unwrap();
        api.advance_audio_clocks(0);
        assert_eq!(api.sound_fade_volumes[3], 0);
        assert!(api.sound_slots.contains_key(&3));
        sound_call(&mut api, 0x16, &[3, 0, 1000]).unwrap();
        api.advance_audio_clocks(1000); // sound timer is not the VM's capped clock
        assert_eq!(api.bgm_play_volumes[3], 0);
        assert!(api.bgm_play_ramps[3].is_none());
        assert!(
            api.audio_requests
                .iter()
                .all(|command| matches!(command, AudioCommand::SetVolume { fade_ms: 0, .. }))
        );
    }

    #[test]
    fn unloaded_sound_controls_leave_no_backend_commands_or_envelopes() {
        let mut api = api_with_resident_channel();
        for (id, args) in [
            (0x08, vec![15, 64]),
            (0x09, vec![63, 64]),
            (0x17, vec![15, 64]),
            (0x18, vec![15, 100]),
            (0x19, vec![15, 100]),
            (0x1C, vec![15, 64]),
            (0x25, vec![63]),
            (0x26, vec![63, 100]),
            (0x2C, vec![63, 64]),
        ] {
            sound_call(&mut api, id, &args).unwrap();
        }
        api.advance_audio_clocks(100);
        assert!(api.audio_requests.is_empty());
        assert_eq!(api.bgm_primary_volumes[15], 64);
        assert_eq!(api.sound_primary_volumes[63], 64);
        assert!(api.bgm_fade_ramps[15].is_none() && api.sound_fade_ramps[63].is_none());
    }

    #[test]
    fn owns_every_recovered_sound_dispatch_entry() {
        let registered = (0..=u8::MAX)
            .map(u16::from)
            .filter(|id| ethornell_script::native_abi::lookup(0xA0, *id).is_some())
            .collect::<Vec<_>>();
        assert_eq!(registered, OWNED_A0_IDS);
    }

    #[test]
    fn virtual_cd_track_names_follow_one_based_tmsf_numbering() {
        assert_eq!(parse_native_cd_track_number("track01"), Some(1));
        assert_eq!(parse_native_cd_track_number("CDDA_255"), Some(255));
        assert_eq!(parse_native_cd_track_number("audio-12"), Some(12));
        assert_eq!(parse_native_cd_track_number("00"), None);
        assert_eq!(parse_native_cd_track_number("track256"), None);
        assert_eq!(parse_native_cd_track_number("bgm01"), None);
    }

    #[test]
    fn preserves_native_fixed_sound_fields_and_bank_separation() {
        assert_eq!(native_audio_fixed(0x0001_0000), 1.0);
        assert_eq!(native_audio_fixed(0x0010_0000), 16.0);
        assert_eq!(native_se_audio_channel(0), 0x100);
        assert_eq!(native_se_audio_channel(24), 0x118);
        assert_eq!(native_mixer_volume([128; 4]), 1.0);
        assert_eq!(native_mixer_volume([0, 128, 128, 128]), 0.0);
        assert!(native_mixer_volume([64, 128, 128, 128]) > 0.0);
        assert!(native_mixer_volume([64, 128, 128, 128]) < 1.0);
    }
}
