//! ActiveX Flash is unavailable on console targets. Preserve the native
//! unsupported/load-failure status instead of allocating a blank fake control.
use ethornell_image::DecodedImage;
#[derive(Debug, Default)]
pub(crate) struct RuntimeFlashRegistry;
impl RuntimeFlashRegistry {
    pub(crate) fn create(
        &mut self,
        bitmap: i32,
        w: i32,
        h: i32,
        path: String,
        _parameter: i32,
        _data: &[u8],
    ) -> i32 {
        if !(0..0x4000).contains(&bitmap) || w <= 0 || h <= 0 {
            4
        } else if path.is_empty() {
            1
        } else {
            2
        }
    }
    pub(crate) fn start(&mut self, bitmap: i32) -> i32 {
        if !(0..0x4000).contains(&bitmap) { 4 } else { 1 }
    }
    pub(crate) fn tick(&mut self, _elapsed: u64) {}
    pub(crate) fn capture_and_release(&mut self, bitmap: i32) -> (i32, Option<DecodedImage>) {
        (self.start(bitmap), None)
    }
}
