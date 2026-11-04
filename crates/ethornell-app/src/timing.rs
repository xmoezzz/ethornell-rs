/// Ethornell/BGI advances graph procedures on a 16 millisecond engine tick.
/// Durations supplied by graph and timeline calls are milliseconds, including
/// small values such as 1, 16, and 32. They must never be reinterpreted as a
/// number of rendered frames.
pub(crate) const NATIVE_TICK_MS: u64 = 16;

/// Convert a native millisecond duration to engine ticks.
///
/// A non-positive duration is immediate. Positive durations are rounded up so
/// the target value is reached no earlier than the script requested.
pub(crate) fn duration_ms_to_ticks(duration_ms: i32) -> u32 {
    if duration_ms <= 0 {
        return 0;
    }
    (duration_ms as u32).div_ceil(NATIVE_TICK_MS as u32)
}

/// Convert a wall-clock delta into target-style engine elapsed time.
///
/// sub_499290 treats a wall-clock jump larger than dword_503DE8 (initially
/// 500, Sys81:04 sets 50..=60000) as a pause and subtracts the whole gap
/// from its pause-adjusted clock.
///
/// Normal GUI frames advance by their actual integer-millisecond delta. A gap
/// larger than the threshold is a pause and contributes no engine time.
pub(crate) fn normalize_engine_elapsed_ms(elapsed_ms: u64, threshold_ms: u32) -> u64 {
    if elapsed_ms > u64::from(threshold_ms) {
        0
    } else {
        elapsed_ms
    }
}

/// Console suspension resets the host clock explicitly. A slow software
/// effect is still an active frame; bound its delta so repeated slow frames
/// cannot be discarded as pause gaps and freeze the effect's completion timer.
#[cfg(any(test, feature = "console"))]
pub(crate) fn console_engine_elapsed_ms(elapsed_ms: u64, threshold_ms: u32) -> u64 {
    elapsed_ms.min(u64::from(threshold_ms))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn small_native_durations_remain_milliseconds() {
        assert_eq!(duration_ms_to_ticks(0), 0);
        assert_eq!(duration_ms_to_ticks(1), 1);
        assert_eq!(duration_ms_to_ticks(15), 1);
        assert_eq!(duration_ms_to_ticks(16), 1);
        assert_eq!(duration_ms_to_ticks(17), 2);
        assert_eq!(duration_ms_to_ticks(32), 2);
        assert_eq!(duration_ms_to_ticks(250), 16);
    }

    #[test]
    fn long_wall_clock_gap_is_treated_as_pause() {
        assert_eq!(normalize_engine_elapsed_ms(0, 500), 0);
        assert_eq!(normalize_engine_elapsed_ms(16, 500), 16);
        assert_eq!(normalize_engine_elapsed_ms(17, 500), 17);
        assert_eq!(normalize_engine_elapsed_ms(500, 500), 500);
        assert_eq!(normalize_engine_elapsed_ms(501, 500), 0);
        assert_eq!(normalize_engine_elapsed_ms(5_000, 500), 0);
    }

    #[test]
    fn slow_console_frames_keep_timed_procedures_advancing() {
        for threshold in [50, 500, 60_000] {
            let elapsed = console_engine_elapsed_ms(u64::from(threshold) + 2_000, threshold);
            assert_eq!(
                normalize_engine_elapsed_ms(elapsed, threshold),
                u64::from(threshold)
            );
            assert_eq!(console_engine_elapsed_ms(16, threshold), 16);
        }
    }
}
