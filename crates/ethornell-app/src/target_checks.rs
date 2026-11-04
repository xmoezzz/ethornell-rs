//! The BGI 1.622 argument validators (sub_4984C0..sub_4989C0): each one
//! raises the script error sub_464C50 with the target's message.

use ethornell_vm::{VmError, VmResult};

fn fail(message: String) -> VmResult<()> {
    Err(VmError::Runtime(message))
}

/// sub_4984C0: SE channel < 0x40.
pub(crate) fn se_channel(value: i32) -> VmResult<()> {
    if value as u32 >= 0x40 {
        return fail(format!("無効な効果音番号 [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_498510: pan <= 0x80 (unsigned).
pub(crate) fn pan(value: i32) -> VmResult<()> {
    if value as u32 > 0x80 {
        return fail(format!("無効なパンポット（定位） [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_498560: volume <= 0x80 (unsigned).
pub(crate) fn volume(value: i32) -> VmResult<()> {
    if value as u32 > 0x80 {
        return fail(format!("無効なボリューム（音量） [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_4985B0: BGM channel < 0x10.
pub(crate) fn bgm_channel(value: i32) -> VmResult<()> {
    if value as u32 >= 0x10 {
        return fail(format!("無効な音楽チャンネル番号 [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_498600: division count 1..=256.
pub(crate) fn division_count(value: i32) -> VmResult<()> {
    if !(1..=256).contains(&value) {
        return fail(format!("無効な分割回数 [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_4986D0: bitmap handle < 0x4000.
pub(crate) fn bitmap(value: i32) -> VmResult<()> {
    if value as u32 >= 0x4000 {
        return fail(format!("無効なビットマップ番号 [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_498720: priority < 0x10000.
pub(crate) fn priority(value: i32) -> VmResult<()> {
    if value as u32 >= 0x1_0000 {
        return fail(format!("無効なプライオリティ [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_4987B0: the blend/effect modes 0..=9, 0x20..=0x27, 0x40, 0x41,
/// 0x80, 0xC0, 0xC1, 0xF0, 0xFF.
pub(crate) fn effect_mode(value: i32) -> VmResult<()> {
    if !matches!(value as u32, 0..=9 | 0x20..=0x27 | 0x40 | 0x41 | 0x80 | 0xC0 | 0xC1 | 0xF0 | 0xFF) {
        return fail(format!("無効なエフェクトモード [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_4988C0: addition level -1..=256.
pub(crate) fn addition_level(value: i32) -> VmResult<()> {
    if !(-1..=256).contains(&value) {
        return fail(format!("無効なアディションレベル [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_498920: effect level <= 0x100 (unsigned).
pub(crate) fn effect_level(value: i32) -> VmResult<()> {
    if value as u32 > 0x100 {
        return fail(format!(
            "無効なエフェクトレベル／トランスペアレンシィ／オパシティ／アディションレベル [ {value} ] が指定されました"
        ));
    }
    Ok(())
}

/// sub_498970: mixing ratio <= 0x100 (unsigned).
pub(crate) fn mixing_ratio(value: i32) -> VmResult<()> {
    if value as u32 > 0x100 {
        return fail(format!("無効な混合比 [ {value} ] が指定されました"));
    }
    Ok(())
}

/// sub_4989C0: transparency <= 0x100 (unsigned).
pub(crate) fn transparency(value: i32) -> VmResult<()> {
    if value as u32 > 0x100 {
        return fail(format!("無効な透明度 [ {value} ] が指定されました"));
    }
    Ok(())
}
