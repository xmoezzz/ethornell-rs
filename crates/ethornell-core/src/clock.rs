//! Wall-clock time. Native targets read the system clock; the web build has
//! no clock in `std` (it panics), so the page installs one with [`install`].
use std::sync::OnceLock;
use std::time::Duration;

/// The current time and the local zone's offset from UTC.
#[derive(Debug, Clone, Copy)]
pub struct WallTime {
    pub since_epoch: Duration,
    pub utc_offset_minutes: i32,
}

static SOURCE: OnceLock<fn() -> WallTime> = OnceLock::new();

/// Installs the clock used instead of the system clock (first call wins).
pub fn install(source: fn() -> WallTime) {
    let _ = SOURCE.set(source);
}

/// The installed clock, if any.
pub fn installed() -> Option<WallTime> {
    SOURCE.get().map(|source| source())
}

/// Time since the Unix epoch, or zero when no clock is available.
pub fn since_epoch() -> Duration {
    if let Some(time) = installed() {
        return time.since_epoch;
    }
    #[cfg(not(target_arch = "wasm32"))]
    {
        std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap_or_default()
    }
    #[cfg(target_arch = "wasm32")]
    {
        Duration::ZERO
    }
}
