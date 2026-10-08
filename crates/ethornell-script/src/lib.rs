use byteorder::{LittleEndian, ReadBytesExt};
use encoding_rs::SHIFT_JIS;
use ethornell_core::Result;
use serde::Serialize;
use std::collections::HashMap;
use std::io::{Cursor, Read};
use std::path::Path;

pub mod bcs;
pub mod calls;
pub mod decompile;
pub mod native_abi;
pub mod native_contract_reference;
pub mod vm_opcode;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
pub enum ScriptFormat {
    Bp,
    BurikoCompiledScriptV1,
    HeaderlessScenario,
    Unknown,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub enum BpOpcode {
    Known { code: u8, name: &'static str },
    Unknown(u8),
}

impl BpOpcode {
    pub fn from_byte(code: u8) -> Self {
        match opcode_name(code) {
            Some(name) => Self::Known { code, name },
            None => Self::Unknown(code),
        }
    }

    pub fn code(&self) -> u8 {
        match self {
            Self::Known { code, .. } | Self::Unknown(code) => *code,
        }
    }

    pub fn name(&self) -> &str {
        match self {
            Self::Known { name, .. } => name,
            Self::Unknown(_) => "unknown",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub enum BpOperand {
    U8(u8),
    U16(u16),
    U32(u32),
    I32(i32),
    Offset(u32),
    String(String),
    Raw(Vec<u8>),
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub struct BpInstruction {
    pub offset: u64,
    pub opcode: BpOpcode,
    pub opcode_hex: String,
    pub opcode_name: String,
    pub operands: Vec<BpOperand>,
    pub known_call: Option<&'static str>,
    pub raw: Vec<u8>,
    pub warning: Option<String>,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub struct BpProgram {
    pub script_name: Option<String>,
    pub functions: Vec<BpFunction>,
    pub strings: Vec<String>,
    pub instructions: Vec<BpInstruction>,
    pub labels: HashMap<u32, usize>,
    pub warnings: Vec<String>,
    /// Code length from the BP header (`a2[1]` of sub_465AB0): the number of
    /// bytes the module occupies in a thread's code region, including data
    /// that follows the last instruction. Zero when unknown.
    pub module_size: u32,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub struct BpFunction {
    pub name: Option<String>,
    pub offset: u32,
}

#[derive(Debug, Clone, Copy)]
struct CodeRange {
    start: usize,
    end: usize,
}

pub fn detect_script_format(path: &Path, buf: &[u8]) -> ScriptFormat {
    if path
        .to_string_lossy()
        .to_ascii_lowercase()
        .ends_with("._bp")
    {
        ScriptFormat::Bp
    } else if buf.starts_with(b"BurikoCompiledScriptVer1.00") {
        ScriptFormat::BurikoCompiledScriptV1
    } else if !buf.is_empty() && path.extension().is_none() {
        ScriptFormat::HeaderlessScenario
    } else {
        ScriptFormat::Unknown
    }
}

pub fn opcode_name(code: u8) -> Option<&'static str> {
    vm_opcode::lookup_bp_opcode(code).map(|spec| spec.symbol)
}

/// Non-null entries in the shipped interpreter table at `0x506300`.
pub const NATIVE_REGISTERED_OPCODES: &[u8] = &[
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x10, 0x11, 0x14, 0x15, 0x16,
    0x17, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x38, 0x39, 0x3a, 0x40, 0x42, 0x43, 0x44, 0x48, 0x49, 0x50, 0x51, 0x52, 0x53,
    0x54, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
    0x6f, 0x70, 0x71, 0x74, 0x75, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81,
    0x90, 0x91, 0x92, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xff,
];

/// Return a diagnostic candidate label for a native selector.
///
/// This table intentionally mixes target-confirmed labels with older port and
/// third-party reverse hints. Callers must consult `ethornell-vm` recovery and
/// implementation classifications before treating a label as target semantics.
pub fn candidate_call_name(group: u8, id: u16) -> Option<&'static str> {
    match (group, id) {
        (0x80, 0x00) => Some("Srand"),
        (0x80, 0x01) => Some("Rand"),
        (0x80, 0x02) => Some("RandMax"),
        (0x80, 0x04) => Some("GetTickCount"),
        (0x80, 0x05) => Some("QueryPerformanceCounter"),
        (0x80, 0x06) => Some("SetPerformanceProfiling"),
        (0x80, 0x07) => Some("ReadPerformanceMetric"),
        (0x80, 0x08) => Some("ReadCursorPoint"),
        (0x80, 0x09) => Some("QueryPresentationState"),
        (0x80, 0x0a) => Some("CopyGraphicsCapabilities"),
        (0x80, 0x0b) => Some("QueryGraphicsMemoryMetric"),
        (0x80, 0x0c) => Some("GetLocalTime"),
        (0x80, 0x0d) => Some("GetPhysicalMemory"),
        (0x80, 0x0e) => Some("QueryWindowMinimizeLatch"),
        (0x80, 0x0f) => Some("QueryWindowActive"),
        (0x80, 0x10) => Some("ResetInputConfiguration"),
        (0x80, 0x11) => Some("ReadKeyState"),
        (0x80, 0x12) => Some("ReadInputState"),
        (0x80, 0x13) => Some("GetInputMessageSerial"),
        (0x80, 0x14) => Some("SetInputMasterGate"),
        (0x80, 0x15) => Some("SetInputLatchedState"),
        (0x80, 0x16) => Some("SampleConfiguredInput"),
        (0x80, 0x17) => Some("QueryConfiguredInputGate"),
        (0x80, 0x18) => Some("RegisterInputScope"),
        (0x80, 0x19) => Some("QueryAndUnregisterInputScope"),
        (0x80, 0x1a) => Some("QueryInputEventBits"),
        (0x80, 0x1b) => Some("RegisterInputClassDescriptors"),
        (0x80, 0x1c) => Some("QueryInputClassLevel"),
        (0x80, 0x1d) => Some("QueryScopedInputEvent"),
        (0x80, 0x1e) => Some("SetMouseButtonMappingMode"),
        (0x80, 0x1f) => Some("ConfigureCursorMotion"),
        (0x80, 0x20) => Some("Alloc"),
        (0x80, 0x21) => Some("Free"),
        (0x80, 0x24) => Some("CountFiles"),
        (0x80, 0x25) => Some("EnumerateFiles"),
        (0x80, 0x26) => Some("EnumerateDirectories"),
        (0x80, 0x27) => Some("MoveFile"),
        (0x80, 0x28) => Some("CreateDirectory"),
        (0x80, 0x29) => Some("RemoveDirectory"),
        (0x80, 0x2a) => Some("DirectoryExists"),
        (0x80, 0x2b) => Some("SplitPath"),
        (0x80, 0x2c) => Some("GetFileAttributes"),
        (0x80, 0x2d) => Some("SetFileAttributes"),
        (0x80, 0x2f) => Some("CopyFile"),
        (0x80, 0x30) => Some("ReadFileBytes"),
        (0x80, 0x31) => Some("ReadFileRange"),
        (0x80, 0x32) => Some("WriteFileBytes"),
        (0x80, 0x33) => Some("DeleteFile"),
        (0x80, 0x34) => Some("FileExists"),
        (0x80, 0x35) => Some("GetFileSize"),
        (0x80, 0x36) => Some("SetAdditionalResourceSearch"),
        (0x80, 0x37) => Some("PrependResourceSearchPath"),
        (0x80, 0x38) => Some("RegisterCompositeArchive"),
        (0x80, 0x39) => Some("SetValidatedFileRoot"),
        (0x80, 0x3a) => Some("GetSpecialFolder"),
        (0x80, 0x3b) => Some("OpenFileDialog"),
        (0x80, 0x3c) => Some("RequireResourceFile"),
        (0x80, 0x3d) => Some("GetConfiguredRoot"),
        (0x80, 0x3e) => Some("SetPrimaryRoot"),
        (0x80, 0x3f) => Some("ConfigureRemovableArchive"),
        (0x80, 0x40) => Some("LoadProgramModule"),
        (0x80, 0x45) => Some("RtcNoop"),
        (0x80, 0x41) => Some("FreeLastProgramModule"),
        (0x80, 0x44) => Some("LoadProgramThread"),
        (0x80, 0x46) => Some("CurrentThreadId"),
        (0x80, 0x47) => Some("ThreadExists"),
        (0x80, 0x48) => Some("ProgramPostMessage"),
        (0x80, 0x49) => Some("ProgramReceiveMessage"),
        (0x80, 0x4a) => Some("ProgramPostMessages"),
        (0x80, 0x4b) => Some("ProgramReceiveMessages"),
        (0x80, 0x4c) => Some("ProgramInvokeCallback"),
        (0x80, 0x50) => Some("SetSystemWaitState"),
        (0x80, 0x52) => Some("SetMainLoopWaitOverride"),
        (0x80, 0x53) => Some("ArmNextBinaryAsync"),
        (0x80, 0x59) => Some("SetAndQueryThreadTimer"),
        (0x80, 0x54) => Some("WaitWndMsg"),
        (0x80, 0x58) => Some("SetThreadTimer"),
        (0x80, 0x5a) => Some("WaitTiming"),
        (0x80, 0x5c) => Some("WaitTimingOrInput"),
        (0x80, 0x5d) => Some("SetExclusiveThread"),
        (0x80, 0x5e) => Some("Sys80_5E_SwitchThread"),
        (0x80, 0x5f) => Some("Sys80_5F_Yield"),
        (0x80, 0x60) => Some("ConfigureDisplayMode"),
        (0x80, 0x61) => Some("QueryFullscreen"),
        (0x80, 0x62) => Some("ConfigureFullscreenHotkeys"),
        (0x80, 0x63) => Some("SetAspectPreservingScaling"),
        (0x80, 0x64) => Some("SetWindowVisible"),
        (0x80, 0x65) => Some("MinimizeMainWindow"),
        (0x80, 0x66) => Some("SetWindowTitle"),
        (0x80, 0x67) => Some("SetCursorIndex"),
        (0x80, 0x68) => Some("SetNativeCloseMode"),
        (0x80, 0x69) => Some("RequestWindowClose"),
        (0x80, 0x6a) => Some("TerminateInterpreter"),
        (0x80, 0x6b) => Some("SelectBootstrap"),
        (0x80, 0x6c) => Some("SetFileDropEnabled"),
        (0x80, 0x6d) => Some("QueryDroppedFile"),
        (0x80, 0x6e) => Some("SetRasterWaitEnabled"),
        (0x80, 0x6f) => Some("QueryDisplayAspectMismatch"),
        (0x80, 0x70) => Some("AllocateGlobalConfig"),
        (0x80, 0x71) => Some("ClearGlobalConfig"),
        (0x80, 0x74) => Some("SetSaveDataIntegrity"),
        (0x80, 0x78) => Some("SaveConfigSlot"),
        (0x80, 0x79) => Some("LoadConfigSlot"),
        (0x80, 0x7a) => Some("ReadConfigSlotHeader"),
        (0x80, 0x7b) => Some("ValidateConfigSlot"),
        (0x80, 0x80) => Some("LoadGlobalUserData"),
        (0x80, 0x81) => Some("SaveGlobalUserData"),
        (0x80, 0x82) => Some("WriteGlobalDataBlock"),
        (0x80, 0x83) => Some("ReadGlobalDataBlock"),
        (0x80, 0x84) => Some("InternResourceName"),
        (0x80, 0x85) => Some("ResourceNameExists"),
        (0x80, 0x88) => Some("CreateOrResizeReadFlagTable"),
        (0x80, 0x89) => Some("SetReadFlagBit"),
        (0x80, 0x8a) => Some("SetReadFlagRange"),
        (0x80, 0x8b) => Some("QueryReadFlagBit"),
        (0x80, 0x90) => Some("ResetStructuredHistory"),
        (0x80, 0x91) => Some("StructuredHistoryCount"),
        (0x80, 0x94) => Some("AppendStructuredHistoryFields"),
        (0x80, 0x95) => Some("ReadStructuredHistory"),
        (0x80, 0x96) => Some("AppendStructuredHistoryRecord"),
        (0x80, 0x97) => Some("ReadStructuredHistoryExtended"),
        (0x80, 0x98) => Some("IndexedRecordOpen"),
        (0x80, 0x99) => Some("IndexedRecordClose"),
        (0x80, 0x9a) => Some("IndexedRecordCount"),
        (0x80, 0x9c) => Some("IndexedRecordPush"),
        (0x80, 0x9d) => Some("IndexedRecordLoad"),
        (0x80, 0x9e) => Some("IndexedRecordRemove"),
        (0x80, 0xa0) => Some("PollQueuedEvent"),
        (0x80, 0xa1) => Some("PostQueuedEvent"),
        (0x80, 0xa8) => Some("SetRegisteredObjectState"),
        (0x80, 0xa9) => Some("GetRegisteredObjectState"),
        (0x80, 0xac) => Some("QueueRegisteredObjectMessage"),
        (0x80, 0xaf) => Some("SetSystemModeFlag"),
        (0x80, 0xb0) => Some("CreateExclusionSection"),
        (0x80, 0xb1) => Some("DeleteExclusionSection"),
        (0x80, 0xb4) => Some("WaitExclusionSection"),
        (0x80, 0xb5) => Some("LeaveExclusionSection"),
        (0x80, 0xb6) => Some("QueryExclusionSection"),
        (0x80, 0xc0) => Some("UserDataEncodeFlush"),
        (0x80, 0xc1) => Some("SdcDecodeBuffer"),
        (0x80, 0xc4) => Some("UserDataEncodeChunk"),
        (0x80, 0xc5) => Some("SdcDecodeStructArray"),
        (0x80, 0xd0) => Some("RecordTableOpen"),
        (0x80, 0xd1) => Some("RecordTableClose"),
        (0x80, 0xd2) => Some("RecordTableInsert"),
        (0x80, 0xd3) => Some("RecordTableRemove"),
        (0x80, 0xd4) => Some("RecordTableFetch"),
        (0x80, 0xd8) => Some("DispatchPendingCallbacks"),
        (0x80, 0xd9) => Some("UserDataEncodeTrailer"),
        (0x80, 0xda) => Some("InstallStringNamespace"),
        (0x80, 0xdb) => Some("SerializeStringNamespace"),
        (0x80, 0xdc) => Some("InternStringInNamespace"),
        (0x80, 0xdd) => Some("CopyIndexedNamespaceRecord"),
        (0x80, 0xe0) => Some("LaunchProcess"),
        (0x80, 0xe1) => Some("RestartWithCommand"),
        (0x80, 0xe2) => Some("LaunchProcessWithArgs"),
        (0x80, 0xe3) => Some("ShellOpen"),
        (0x80, 0xed) => Some("SystemExtensionQuery"),
        (0x80, 0xee) => Some("SystemExtensionReset"),
        (0x80, 0xef) => Some("SystemExtensionSet"),
        (0x80, 0xe8) => Some("GetGameId"),
        (0x80, 0xf0) => Some("ShowInputDialog"),
        (0x80, 0xf2) => Some("ShowInstallerDialog"),
        (0x80, 0xf7) => Some("CreateShortcut"),
        (0x80, 0xf8) => Some("ReadInstalledFolder"),
        (0x80, 0xfd) => Some("IsLauncher"),
        (0x81, 0x0e) => Some("GetAdjustedDesktopDimensions"),
        (0x81, 0x0f) => Some("IsMainWindowMinimized"),
        (0x81, 0x18) => Some("RegisterTouchInput"),
        (0x81, 0x19) => Some("CopyTouchRecords"),
        (0x81, 0x1f) => Some("SysSetMessageAuxiliaryInputMask"),
        (0x81, 0x2f) => Some("TestPathWritable"),
        (0x81, 0x30) => Some("ReadResourceToBuffer"),
        (0x81, 0x31) => Some("InternetRead"),
        (0x81, 0x35) => Some("OpenResourceHandle"),
        (0x81, 0x36) => Some("EnumerateDriveTypes"),
        (0x81, 0x37) => Some("GetDiskFreeMegabytes"),
        (0x81, 0x60) => Some("ConfigureScreen"),
        (0x81, 0x62) => Some("SetWindowMonitorAdapterMode"),
        (0x81, 0x63) => Some("SetConfigInputMode"),
        (0x81, 0x64) => Some("ConfigureScreenSize"),
        (0x81, 0x6f) => Some("SetShaderEffectEnabled"),
        (0x81, 0xf7) => Some("CreateSpecialFolderShortcut"),
        (0x90, 0x00) => Some("GraphRequestRedraw"),
        (0x90, 0x01) => Some("GraphSetSchedulerGate"),
        (0x90, 0x02) => Some("GraphSetFrameRate"),
        (0x90, 0x03) => Some("GraphInitializeBitmapMemoryManager"),
        (0x90, 0x04) => Some("GraphCreateWorkBitmap"),
        (0x90, 0x05) => Some("GraphCreatePrioritizedWorkBitmap"),
        (0x90, 0x06) => Some("GraphSetCenter"),
        (0x90, 0x07) => Some("GraphSetSyncLoadHold"),
        (0x90, 0x08) => Some("GraphSetDisplayEnabled"),
        (0x90, 0x09) => Some("GraphSetDefaultPriority"),
        (0x90, 0x0a) => Some("GraphSetObjectUpdateRedrawPolicy"),
        (0x90, 0x0b) => Some("GraphSetCurrentBitmap"),
        (0x90, 0x0c) => Some("GraphSetDisplayOptions"),
        (0x90, 0x0d) => Some("GraphSetRasterFormatMode"),
        (0x90, 0x0e) => Some("GraphRegisterFont"),
        (0x90, 0x0f) => Some("GraphSetBitmapUnblendColor"),
        (0x90, 0x10) => Some("GraphLoadBitmap"),
        (0x90, 0x11) => Some("GraphCreateBitmap"),
        (0x90, 0x12) => Some("GraphReleaseBitmap"),
        (0x90, 0x13) => Some("GraphFillBitmap"),
        (0x90, 0x14) => Some("GraphCreateBitmapFromPixels"),
        (0x90, 0x15) => Some("GraphCopyBitmapPixels"),
        (0x90, 0x16) => Some("GraphQueryBitmapInfo"),
        (0x90, 0x17) => Some("GraphValidateBitmapFormat"),
        (0x90, 0x18) => Some("GraphBlitBitmap"),
        (0x90, 0x19) => Some("GraphSynthesizeBitmap"),
        (0x90, 0x1a) => Some("GraphCompositeBitmaps"),
        (0x90, 0x1b) => Some("GraphCopyBitmap"),
        (0x90, 0x1c) => Some("GraphScaleBitmapRegion"),
        (0x90, 0x1d) => Some("GraphTransformBitmap"),
        (0x90, 0x1e) => Some("GraphBlitBitmapRegion"),
        (0x90, 0x1f) => Some("GraphCreateBitmapRegion"),
        (0x90, 0x20) => Some("GraphStartAlphaControl"),
        (0x90, 0x21) => Some("GraphStartMoveControl"),
        (0x90, 0x22) => Some("GraphStartAlphaControlEx"),
        (0x90, 0x23) => Some("GraphStartMoveControlEx"),
        (0x90, 0x24) => Some("GraphStartSpecialObjectControl"),
        (0x90, 0x28) => Some("GraphStartObjectMotionControl"),
        (0x90, 0x29) => Some("GraphStartSplineObjectControl"),
        (0x90, 0x2c) => Some("GraphStartShakeObjectControl"),
        (0x90, 0x30) => Some("GraphSetObjectDrawEnabled"),
        (0x90, 0x31) => Some("GraphSetObjectEnabled"),
        (0x90, 0x32) => Some("Graph90_32_SetObjectAlpha"),
        (0x90, 0x33) => Some("GraphSetObjectPosition"),
        (0x90, 0x34) => Some("GraphSetObjectMaskAlpha"),
        (0x90, 0x35) => Some("GraphSetObjectFixedParameter"),
        (0x90, 0x36) => Some("GraphSetObjectSecondaryOffset"),
        (0x90, 0x37) => Some("GraphSetObjectPrimaryOffset"),
        (0x90, 0x38) => Some("GraphSetObjectProperty"),
        (0x90, 0x39) => Some("GraphSetObjectAlphaMultiplier"),
        (0x90, 0x3a) => Some("GraphSetObjectPriority"),
        (0x90, 0x3c) => Some("GraphSetObjectHitMaskBitmap"),
        (0x90, 0x3d) => Some("GraphHitTestObjectAtPointer"),
        (0x90, 0x3f) => Some("GraphInvokeUnsupportedObjectExtension"),
        (0x90, 0x40) => Some("GraphSetCurrentObjectBitmap"),
        (0x90, 0x43) => Some("GraphConfigureCurrentObjectSpriteMask"),
        (0x90, 0x47) => Some("GraphConfigureCurrentObjectVmEffect"),
        (0x90, 0x4a) => Some("GraphConfigureCurrentObjectBlitSources"),
        (0x90, 0x4c) => Some("GraphSetCurrentObjectRenderControls"),
        (0x90, 0x4d) => Some("GraphGetCurrentObjectMode"),
        (0x90, 0x50) => Some("GraphCreateSpriteObject"),
        (0x90, 0x51) => Some("GraphReleaseSpriteObject"),
        (0x90, 0x53) => Some("GraphRefreshSpriteObject"),
        (0x90, 0x54) => Some("GraphSetSpriteDrawEnabled"),
        (0x90, 0x55) => Some("GraphSetSpriteAuxBitmap"),
        (0x90, 0x56) => Some("GraphConfigureSpriteSingleBitmap"),
        (0x90, 0x57) => Some("GraphReplaceSpriteBitmap"),
        (0x90, 0x58) => Some("GraphConfigureSpriteDualBitmap"),
        (0x90, 0x5a) => Some("GraphConfigureSpriteMaskedBitmap"),
        (0x90, 0x5c) => Some("GraphConfigureSpriteTransformMode5"),
        (0x90, 0x5d) => Some("GraphConfigureSpriteTransformMode6"),
        (0x90, 0x60) => Some("GraphCreateFilterObject"),
        (0x90, 0x61) => Some("GraphReleaseFilterObject"),
        (0x90, 0x64) => Some("GraphSetFilterEnabled"),
        (0x90, 0x65) => Some("GraphConfigureFilter"),
        (0x90, 0x66) => Some("GraphConfigureFilterWithMask"),
        (0x90, 0x41) => Some("GraphConfigureCurrentObjectDualBitmap"),
        (0x90, 0x42) => Some("GraphConfigureCurrentObjectQuadBitmap"),
        (0x90, 0x44) => Some("GraphConfigureCurrentObjectFrameTable"),
        (0x90, 0x45) => Some("GraphConfigureCurrentObjectResourceTriplet"),
        (0x90, 0x46) => Some("GraphConfigureCurrentObjectBitmapMode"),
        (0x90, 0x48) => Some("GraphConfigureCurrentObjectBitmapSizePosition"),
        (0x90, 0x49) => Some("GraphConfigureCurrentObjectBitmapSize"),
        (0x90, 0x59) => Some("GraphConfigureSpriteScaledBitmap"),
        (0x90, 0x5b) => Some("GraphConfigureSpriteVmEffect"),
        (0x90, 0x70) => Some("GraphCreateMapObject"),
        (0x90, 0x71) => Some("GraphReleaseMapObject"),
        (0x90, 0x74) => Some("GraphSetMapEnabled"),
        (0x90, 0x75) => Some("GraphConfigureMapObject"),
        (0x90, 0x76) => Some("GraphInitializeMapGrid"),
        (0x90, 0x78) => Some("GraphUploadMapTileData"),
        (0x90, 0x79) => Some("GraphSetMapViewport"),
        (0x90, 0x7a) => Some("GraphReplaceMapTileId"),
        (0x90, 0x80) => Some("GraphCreateWindowObject"),
        (0x90, 0x81) => Some("GraphReleaseWindowObject"),
        (0x90, 0x82) => Some("GraphSetWindowCompositionOrder"),
        (0x90, 0x83) => Some("GraphRenderWindowToBitmap"),
        (0x90, 0x84) => Some("GraphSetWindowDrawEnabled"),
        (0x90, 0x85) => Some("GraphConfigureWindowObject"),
        (0x90, 0x86) => Some("GraphSetWindowBackgroundBitmap"),
        (0x90, 0x87) => Some("GraphSetWindowIsolatedComposition"),
        (0x90, 0x88) => Some("GraphSetWindowValidRegion"),
        (0x90, 0x89) => Some("GraphGetWindowValidRegion"),
        (0x90, 0x90) => Some("GraphStartWindowMessageProcedure"),
        (0x90, 0x91) => Some("GraphSetMessageInputScope"),
        (0x90, 0x92) => Some("GraphSetMessageInputFilter"),
        (0x90, 0x94) => Some("GraphSetGlyphRevealDelay"),
        (0x90, 0x95) => Some("GraphSetTextRevealAnimation"),
        (0x90, 0x96) => Some("GraphSetTextSettleAnimation"),
        (0x90, 0x97) => Some("GraphSetTextAutoAdvance"),
        (0x90, 0x98) => Some("GraphConfigureMessageCaretFrames"),
        (0x90, 0x99) => Some("GraphSetMessageCaretFrameDelay"),
        (0x90, 0x9a) => Some("GraphSetMessageCaretPosition"),
        (0x90, 0x9b) => Some("GraphSetMessageStartDelay"),
        (0x90, 0x9c) => Some("GraphSetTextShadowEnabled"),
        (0x90, 0x9d) => Some("GraphSetTextShadowParameters"),
        (0x90, 0x9e) => Some("GraphRegisterFullwidthGlyphStrip"),
        (0x90, 0x9f) => Some("GraphSetMessageInputForcesCompletion"),
        (0x90, 0xa0) => Some("GraphStartItemSelection"),
        (0x90, 0xa1) => Some("GraphDrawItemSelectionGrid"),
        (0x90, 0xa2) => Some("GraphStartItemSelectionEx"),
        (0x90, 0xa3) => Some("GraphStartItemSelectionExBlink"),
        (0x90, 0xa4) => Some("GraphSetItemSelectionHighlightStyles"),
        (0x90, 0xa5) => Some("GraphSetItemSelectionInputMask"),
        (0x90, 0xa6) => Some("GraphSetItemSelectionNavigationParameters"),
        (0x90, 0xa7) => Some("GraphSetItemSelectionColumnLayout"),
        (0x90, 0xaf) => Some("GraphSetInteractiveProcedurePollGate"),
        (0x90, 0xb0) => Some("GraphStartIconSelection"),
        (0x90, 0xb1) => Some("GraphStartIconSelectionEx"),
        (0x90, 0xb4) => Some("GraphDrawIconBatch"),
        (0x90, 0xb5) => Some("GraphDrawExtendedIconBatch"),
        (0x90, 0xb6) => Some("GraphApplyIconInputLayout"),
        (0x90, 0xb7) => Some("GraphApplyIconInputLayoutEx"),
        (0x90, 0xb8) => Some("GraphCreateIconInputProcessor"),
        (0x90, 0xb9) => Some("GraphReleaseIconInputProcessor"),
        (0x90, 0xba) => Some("GraphConfigureIconInputProcessor"),
        (0x90, 0xbc) => Some("GraphGetIconInputState"),
        (0x90, 0xbd) => Some("GraphGetIconInputCurrentGroup"),
        (0x90, 0xbe) => Some("GraphGetIconInputSelections"),
        (0x90, 0xbf) => Some("GraphPopIconInputEvent"),
        (0x90, 0xc0) => Some("GraphLoadBgBitmapResource"),
        (0x90, 0xc2) => Some("GraphFlipBitmap"),
        (0x90, 0xc3) => Some("GraphDownsampleBitmapHalf"),
        (0x90, 0xc4) => Some("GraphImportExternalImage"),
        (0x90, 0xc5) => Some("GraphSaveBitmapToImageFile"),
        (0x90, 0xc6) => Some("GraphRegisterBgResourceData"),
        (0x90, 0xc7) => Some("GraphLoadCachedBgBitmap"),
        (0x90, 0xc8) => Some("GraphTransformBitmapGeneral"),
        (0x90, 0xca) => Some("GraphScaleBitmapAspectFit"),
        (0x90, 0xcc) => Some("GraphRegisterToneCurve"),
        (0x90, 0xcd) => Some("GraphApplyToneCurveEffect"),
        (0x90, 0xce) => Some("GraphEncodeBitmapToBuffer"),
        (0x90, 0xd0) => Some("GraphCreateKnobObject"),
        (0x90, 0xd1) => Some("GraphReleaseKnobObject"),
        (0x90, 0xd4) => Some("GraphSetKnobEnabled"),
        (0x90, 0xd5) => Some("GraphSetKnobBasePosition"),
        (0x90, 0xd6) => Some("GraphSetKnobPosition"),
        (0x90, 0xd7) => Some("GraphGetKnobPosition"),
        (0x90, 0xd8) => Some("GraphSetKnobMovementPrecision"),
        (0x90, 0xd9) => Some("GraphSetKnobMovementRange"),
        (0x90, 0xda) => Some("GraphTakeKnobVerticalEvent"),
        (0x90, 0xdb) => Some("GraphTakeChangedKnobHandle"),
        (0x90, 0xdc) => Some("GraphSetKnobRelativeMode"),
        (0x90, 0xdd) => Some("GraphSwapKnobInputMode"),
        (0x90, 0xde) => Some("GraphWatchKnobObject"),
        (0x90, 0xdf) => Some("GraphUnwatchKnobObject"),
        (0x90, 0xe0) => Some("GraphCreateGroupObject"),
        (0x90, 0xe1) => Some("GraphReleaseGroupObject"),
        (0x90, 0xe4) => Some("GraphSetGroupEnabled"),
        (0x90, 0xe5) => Some("GraphConfigureGroupObject"),
        (0x90, 0xe8) => Some("GraphAddObjectToGroup"),
        (0x90, 0xe9) => Some("GraphRemoveObjectFromGroup"),
        (0x90, 0xf0) => Some("GraphOpenDirectShowMovie"),
        (0x90, 0xf1) => Some("GraphCloseDirectShowMovie"),
        (0x90, 0xf2) => Some("GraphIsDirectShowMoviePlaying"),
        (0x90, 0xf3) => Some("GraphSetDirectShowMovieVolume"),
        (0x90, 0xf4) => Some("GraphLoadBurikoMovieResource"),
        (0x90, 0xf5) => Some("GraphReleaseBurikoMovieResource"),
        (0x90, 0xf6) => Some("GraphDecodeBurikoMovieFrame"),
        (0x90, 0xf7) => Some("GraphAttachBurikoMovieResource"),
        (0x90, 0xf8) => Some("GraphClearSpriteTargets"),
        (0x90, 0xfa) => Some("GraphRegisterSpriteTarget"),
        (0x90, 0xfb) => Some("GraphUnregisterSpriteTarget"),
        (0x90, 0xfc) => Some("GraphHitTestSpriteTargets"),
        (0x90, 0xfd) => Some("GraphGetSpriteTargetState"),
        (0x91, 0x03) => Some("GraphCacheBinaryResource"),
        (0x91, 0x06) => Some("GraphSetGlobalDisplayOffset"),
        (0x91, 0x0b) => Some("GraphSetScriptBitmapContextBindingEnabled"),
        (0x91, 0x0c) => Some("GraphSetGlyphCoverageMode"),
        (0x91, 0x0d) => Some("GraphSetFontPitchDetectionEnabled"),
        (0x91, 0x0e) => Some("GraphRegisterNamedFontTransform"),
        (0x91, 0x0f) => Some("GraphConfigureNativeFont"),
        (0x91, 0x10) => Some("GraphGenerateAffineDisplacementMap"),
        (0x91, 0x11) => Some("GraphGenerateRandomDisplacementMap"),
        (0x91, 0x12) => Some("GraphGenerateRippleDisplacementMap"),
        (0x91, 0x13) => Some("GraphGeneratePerspectiveBendMap"),
        (0x91, 0x14) => Some("GraphGenerateCurvatureDisplacementMap"),
        (0x91, 0x15) => Some("GraphGenerateRadialLensDisplacementMap"),
        (0x91, 0x16) => Some("GraphGenerateSineDisplacementMap"),
        (0x91, 0x17) => Some("GraphGenerateRadialWarpDisplacementMap"),
        (0x91, 0x18) => Some("GraphCompositeBitmapRectAlpha"),
        (0x91, 0x19) => Some("GraphCompositeBitmapRectConverted"),
        (0x91, 0x1a) => Some("GraphReplaceBitmapRgbPreserveAlpha"),
        (0x91, 0x1b) => Some("GraphConcentrateBitmap"),
        (0x91, 0x1c) => Some("GraphScaleTrueColorBitmap"),
        (0x91, 0x1d) => Some("GraphProcessBitmap"),
        (0x91, 0x1e) => Some("GraphApplyGrayscaleMask"),
        (0x91, 0x1f) => Some("GraphCloneBitmap"),
        (0x91, 0x31) => Some("GraphSetObjectSuppressed"),
        (0x91, 0x33) => Some("GraphSetObjectFixedPosition"),
        (0x91, 0x36) => Some("GraphSetObjectSecondaryVector"),
        (0x91, 0x37) => Some("GraphSetObjectPrimaryVector"),
        (0x91, 0x38) => Some("GraphGetObjectProperty"),
        (0x91, 0x3d) => Some("GraphGetObjectCompositePosition"),
        (0x91, 0x3e) => Some("GraphAttachChildObject"),
        (0x91, 0x3f) => Some("GraphDetachChildObject"),
        (0x91, 0x40) => Some("GraphInitializeMultiLayerBackground"),
        (0x91, 0x41) => Some("GraphSelectMultiLayer"),
        (0x91, 0x42) => Some("GraphSetMultiLayerEnabled"),
        (0x91, 0x43) => Some("GraphSetMultiLayerPosition"),
        (0x91, 0x44) => Some("GraphSetMultiLayerBlendMode"),
        (0x91, 0x45) => Some("GraphSetMultiLayerBlendParameter"),
        (0x91, 0x46) => Some("GraphSetMultiLayerBitmap"),
        (0x91, 0x47) => Some("GraphSetMultiLayerTransform"),
        (0x91, 0x48) => Some("GraphSetMultiLayerAuxiliaryPair"),
        (0x91, 0x49) => Some("GraphSetMultiLayerSourceVelocity"),
        (0x91, 0x4a) => Some("GraphSetMultiLayerTransformVelocity"),
        (0x91, 0x55) => Some("Graph91_55_SetSpriteRelation"),
        (0x91, 0x60) => Some("Graph91_60_CreateEffector"),
        (0x91, 0x61) => Some("Graph91_61_ReleaseEffector"),
        (0x91, 0x64) => Some("Graph91_64_SetEffectorEnabled"),
        (0x91, 0x65) => Some("Graph91_65_ConfigureDualVectorMapEffector"),
        (0x91, 0x66) => Some("Graph91_66_ConfigureGradientEffector"),
        (0x91, 0x67) => Some("Graph91_67_ConfigureRippleEffector"),
        (0x91, 0x68) => Some("Graph91_68_ConfigureTransformEffector"),
        (0x91, 0x69) => Some("Graph91_69_ConfigureSurfaceEffector"),
        (0x91, 0x70) => Some("Graph91_70_CreateLandscape"),
        (0x91, 0x71) => Some("Graph91_71_ReleaseLandscape"),
        (0x91, 0x73) => Some("Graph91_73_HitTestLandscapeAtPointer"),
        (0x91, 0x74) => Some("Graph91_74_SetLandscapeEnabled"),
        (0x91, 0x75) => Some("Graph91_75_ConfigureLandscapeObject"),
        (0x91, 0x76) => Some("Graph91_76_SetLandscapeCellSilhouette"),
        (0x91, 0x78) => Some("Graph91_78_ConfigureLandscapePartsAndColumns"),
        (0x91, 0x79) => Some("Graph91_79_ConfigureLandscapeMap"),
        (0x91, 0x7A) => Some("Graph91_7A_ConfigureLandscapeGuides"),
        (0x91, 0x7B) => Some("Graph91_7B_SetLandscapeCellGuides"),
        (0x91, 0x7C) => Some("Graph91_7C_CopyLandscapePart"),
        (0x91, 0x7D) => Some("Graph91_7D_SetLandscapeMapCellColumn"),
        (0x91, 0x7E) => Some("Graph91_7E_GetLandscapeCellValue"),
        (0x91, 0x7F) => Some("Graph91_7F_CopyLandscapeCellImage"),
        (0x91, 0x88) => Some("Graph91_88_ConfigureWindowFont"),
        (0x91, 0x89) => Some("Graph91_89_SetWindowLineSpacingPercent"),
        (0x91, 0x8A) => Some("Graph91_8A_SetWindowMessageVariant"),
        (0x91, 0x8B) => Some("Graph91_8B_SetWindowTextLayoutMode"),
        (0x91, 0x8C) => Some("Graph91_8C_SetWindowTextCursor"),
        (0x91, 0x8D) => Some("Graph91_8D_GetWindowTextCursor"),
        (0x91, 0x8E) => Some("Graph91_8E_IsWindowTextCursorAtBoundary"),
        (0x91, 0x90) => Some("Graph91_90_StartExtendedMessage"),
        (0x91, 0x91) => Some("Graph91_91_RenderWindowText"),
        (0x91, 0x92) => Some("Graph91_92_StartExtendedMessageWithOption"),
        (0x91, 0x93) => Some("Graph91_93_RenderWindowTextWithStyleMode"),
        (0x91, 0x94) => Some("Graph91_94_UpdateTextSubstitution"),
        (0x91, 0x95) => Some("Graph91_95_CountTextSubstitutionMatches"),
        (0x91, 0x96) => Some("Graph91_96_RegisterTextSubstitutionRecords"),
        (0x91, 0x97) => Some("Graph91_97_SetPersistentTextStyle"),
        (0x91, 0x98) => Some("Graph91_98_ConfigureTextLayoutGlobals"),
        (0x91, 0x99) => Some("Graph91_99_SetTextScaleDivisor"),
        (0x91, 0x9A) => Some("Graph91_9A_SetTextGlobalProperty"),
        (0x91, 0x9B) => Some("Graph91_9B_MeasureText"),
        (0x91, 0x9C) => Some("Graph91_9C_DrawText"),
        (0x91, 0x9D) => Some("Graph91_9D_DrawTextWithStyleMode"),
        (0x91, 0x9E) => Some("Graph91_9E_ExtractTextLabels"),
        (0x91, 0x9F) => Some("Graph91_9F_StripTextMarkup"),
        (0x91, 0xB8) => Some("Graph91_B8_CreateExtendedIconInputProcessor"),
        (0x91, 0xBA) => Some("Graph91_BA_ConfigureExtendedIconInputProcessor"),
        (0x91, 0xBB) => Some("Graph91_BB_SetExtendedIconInputItemState"),
        (0x91, 0xBF) => Some("Graph91_BF_RegisterKeyAssignmentTable"),
        (0x91, 0xDB) => Some("Graph91_DB_GetActiveKnobHandle"),
        (0x91, 0xF0) => Some("Graph91_F0_OpenDirectShowMovie"),
        (0x91, 0xF1) => Some("Graph91_F1_StartDirectShowMovie"),
        (0x91, 0xF2) => Some("Graph91_F2_CloseDirectShowMovie"),
        (0x91, 0xF3) => Some("Graph91_F3_SetDirectShowMoviePaused"),
        (0x91, 0xF4) => Some("Graph91_F4_CreateFlashControl"),
        (0x91, 0xF5) => Some("Graph91_F5_StartFlashControl"),
        (0x91, 0xF6) => Some("Graph91_F6_CaptureAndReleaseFlashControl"),
        (0x91, 0xF7) => Some("Graph91_F7_GetMoviePosition"),
        (0x92, 0x00) => Some("GraphConfigureCompactWaveTable"),
        (0x92, 0x01) => Some("GraphConfigureWaveTable"),
        (0x92, 0x10) => Some("GraphGenerateRadialVectorMap"),
        (0x92, 0x11) => Some("GraphGenerateAxisVectorMap"),
        (0x92, 0x12) => Some("GraphSetBitmapAuxiliaryPair"),
        (0x92, 0x13) => Some("GraphReplaceBitmapColor"),
        (0x92, 0x14) => Some("GraphPreloadBitmapResource"),
        (0x92, 0x15) => Some("GraphCancelPendingBitmapPreloads"),
        (0x92, 0x16) => Some("GraphGetBitmapAuxiliaryPair"),
        (0x92, 0x17) => Some("GraphReadBitmapPixelValue"),
        (0x92, 0x18) => Some("GraphConvertBitmapToAlphaDescriptor"),
        (0x92, 0x19) => Some("GraphInvertAlphaBitmap"),
        (0x92, 0x1a) => Some("GraphComposeBitmapAlphaAtOffset"),
        (0x92, 0x1c) => Some("GraphDrawBitmapTextMeasure"),
        (0x92, 0x1d) => Some("GraphDrawWrappedBitmapText"),
        (0x92, 0x1e) => Some("GraphDrawBitmapText"),
        (0x92, 0x1f) => Some("GraphLoadExternalBmp"),
        (0x92, 0x88) => Some("GraphSetTextObjectValue"),
        (0x92, 0x89) => Some("GraphCompositeResourceIntoTextObject"),
        (0x92, 0x8a) => Some("GraphSetTextObjectAuxiliaryValue"),
        (0x92, 0x8c) => Some("GraphSetTextObjectUpdateFlag"),
        (0x92, 0x8d) => Some("GraphApplyEffectResource"),
        (0x92, 0x8e) => Some("GraphResetTextObject"),
        (0x92, 0x90) => Some("GraphStartStyledMessage"),
        (0x92, 0x91) => Some("GraphDrawFormattedText"),
        (0x92, 0x97) => Some("GraphConfigureTextStyleDefaults"),
        (0x92, 0x98) => Some("GraphConfigureTextBitmapSlot"),
        (0x92, 0x9b) => Some("GraphGetTextOutputPair"),
        (0x92, 0x9c) => Some("GraphRenderText"),
        (0x92, 0x9d) => Some("GraphConfigureFontOverride"),
        (0x92, 0x9e) => Some("GraphDrainTextFragmentRecords"),
        (0x92, 0x9f) => Some("GraphSetTextRenderOverride"),
        (0x92, 0xf0) => Some("MovieOpen"),
        (0x92, 0xf1) => Some("GraphCreateMediaLoader"),
        (0x92, 0xf2) => Some("MovieOpenBitmap"),
        (0x92, 0xf4) => Some("MovieSeek"),
        (0x92, 0xf5) => Some("MovieGetPosition"),
        (0x92, 0xf6) => Some("MovieSetBitmapVolume"),
        (0xa0, 0x00) => Some("SoundQueryChannelCount"),
        (0xa0, 0x08) => Some("SoundSetBgmPrimaryVolume"),
        (0xa0, 0x09) => Some("SoundSetSePrimaryVolume"),
        (0xa0, 0x10) => Some("SoundLoadBgmFile"),
        (0xa0, 0x11) => Some("SoundLoadBgmArchive"),
        (0xa0, 0x12) => Some("SoundLoadBgmPair"),
        (0xa0, 0x14) => Some("SoundControlBgm"),
        (0xa0, 0x15) => Some("SoundQueryBgmState"),
        (0xa0, 0x16) => Some("SoundSetBgmVolume"),
        (0xa0, 0x17) => Some("SoundSetBgmPan"),
        (0xa0, 0x18) => Some("SoundFadeBgmToFull"),
        (0xa0, 0x19) => Some("SoundFadeBgmToSilence"),
        (0xa0, 0x1c) => Some("SoundSetBgmSecondaryVolume"),
        (0xa0, 0x20) => Some("SoundLoadSe"),
        (0xa0, 0x21) => Some("SoundLoadSeScaled"),
        (0xa0, 0x22) => Some("SoundReleaseSe"),
        (0xa0, 0x23) => Some("SoundLoadSeDoubleRate"),
        (0xa0, 0x24) => Some("SoundPlaySe"),
        (0xa0, 0x25) => Some("SoundStopSe"),
        (0xa0, 0x26) => Some("SoundFadeSeToSilence"),
        (0xa0, 0x27) => Some("SoundLoadSeCustomRate"),
        (0xa0, 0x28) => Some("SoundRegisterSeMemory"),
        (0xa0, 0x2c) => Some("SoundSetSeSecondaryVolume"),
        (0xa0, 0x2f) => Some("SoundGetSePosition"),
        (0xa0, 0x80) => Some("SoundOpenCdAudio"),
        (0xa0, 0x81) => Some("SoundCloseCdAudio"),
        (0xa0, 0x84) => Some("SoundPlayCdTrack"),
        (0xa0, 0x85) => Some("SoundStopCdAudio"),
        (0xa0, 0x86) => Some("SoundQueryCdMode"),
        (0xa0, 0xc0) => Some("SoundPlayWaveAsync"),
        (0xb0, 0x00) => Some("UserB0_00_DrawBitmapToWindow"),
        (0xb0, 0x02) => Some("UserB0_02_CenterMainWindow"),
        (0xb0, 0x03) => Some("UserB0_03_SetMainWindowPosition"),
        (0xb0, 0x04) => Some("UserB0_04_BindCursorObject"),
        (0xb0, 0x05) => Some("UserB0_05_SetCursorIdleTimeout"),
        (0xb0, 0x06) => Some("UserB0_06_QueryCursorVisible"),
        (0xb0, 0x08) => Some("UserB0_08_ShakeScreen"),
        (0xb0, 0x10) => Some("UserB0_10_CreateDebugWindow"),
        (0xb0, 0x11) => Some("UserB0_11_CloseDebugWindow"),
        (0xb0, 0x14) => Some("UserB0_14_SetDebugWindowVisible"),
        (0xb0, 0x15) => Some("UserB0_15_SetDebugWindowTitle"),
        (0xb0, 0x16) => Some("UserB0_16_MoveDebugWindow"),
        (0xb0, 0x17) => Some("UserB0_17_GetDebugWindowPosition"),
        (0xb0, 0x18) => Some("UserB0_18_ClearDebugWindow"),
        (0xb0, 0x19) => Some("UserB0_19_DrawDebugBitmap"),
        (0xb0, 0x1a) => Some("UserB0_1A_DrawDebugText"),
        (0xb0, 0x1c) => Some("UserB0_1C_SetDebugWindowCloseMessage"),
        (0xb0, 0x20) => Some("UserB0_20_CreateEditControl"),
        (0xb0, 0x21) => Some("UserB0_21_DestroyEditControl"),
        (0xb0, 0x22) => Some("UserB0_22_SetEditFontScale"),
        (0xb0, 0x23) => Some("UserB0_23_QueryEditActive"),
        (0xb0, 0x24) => Some("UserB0_24_SetEditVisible"),
        (0xb0, 0x25) => Some("UserB0_25_SetEditColor"),
        (0xb0, 0x26) => Some("UserB0_26_SetEditText"),
        (0xb0, 0x27) => Some("UserB0_27_GetEditText"),
        (0xb0, 0x28) => Some("UserB0_28_SetEditHideOnEnter"),
        (0xb0, 0x29) => Some("UserB0_29_SetEditPrintableInput"),
        (0xb0, 0x80) => Some("UserB0_80_ShowMessage"),
        (0xb0, 0x81) => Some("UserB0_81_ShowYesNoMessage"),
        (0xb0, 0x82) => Some("UserB0_82_ShowTypedMessage"),
        (0xb0, 0x83) => Some("UserB0_83_SetMessageTitle"),
        (0xb0, 0x84) => Some("UserB0_84_ShowInputDialog"),
        (0xb0, 0x85) => Some("UserB0_85_ShowMultiFieldDialog"),
        (0xb0, 0x86) => Some("UserB0_86_ShowSelectionDialog"),
        (0xb0, 0x87) => Some("UserB0_87_ShowExtendedDialog"),
        (0xb0, 0x8c) => Some("UserB0_8C_ShowPathDialog"),
        (0xb0, 0x8f) => Some("UserB0_8F_ShowSixFieldDialog"),
        (0xb0, 0xa0) => Some("UserB0_A0_CreateModelessDialog"),
        (0xb0, 0xa1) => Some("UserB0_A1_CloseModelessDialog"),
        (0xb0, 0xa2) => Some("UserB0_A2_SetModelessDialogVisible"),
        (0xb0, 0xa3) => Some("UserB0_A3_PollModelessDialog"),
        (0xb0, 0xc0) => Some("UserB0_C0_InternFontName"),
        (0xb0, 0xc1) => Some("UserB0_C1_InternFontNameWithOption"),
        (0xb0, 0xc2) => Some("UserB0_C2_RegisterFontResource"),
        (0xb0, 0xc3) => Some("UserB0_C3_RegisterArchiveFontResource"),
        (0xb0, 0xc4) => Some("UserB0_C4_QueryFontAvailable"),
        (0xb0, 0xc6) => Some("UserB0_C6_QueryFontCapability"),
        (0xb0, 0xc7) => Some("UserB0_C7_SetFontAlias"),
        (0xb0, 0xf0) => Some("UserB0_F0_SetDesktopWallpaper"),
        (0xc0, 0x00) => Some("UserC0_00_CreateParticleScreen"),
        (0xc0, 0x01) => Some("UserC0_01_ReleaseParticleScreen"),
        (0xc0, 0x04) => Some("UserC0_04_SetParticleScreenEnabled"),
        (0xc0, 0x05) => Some("UserC0_05_ConfigureParticleScreenDisplay"),
        (0xc0, 0x06) => Some("UserC0_06_ConfigureParticleFrameTables"),
        (0xc0, 0x08) => Some("UserC0_08_CommitParticleScreen"),
        (0xc0, 0x09) => Some("UserC0_09_SetParticleAutoUpdate"),
        (0xc0, 0x0a) => Some("UserC0_0A_SetParticleCapacity"),
        (0xc0, 0x0b) => Some("UserC0_0B_ConfigureParticleEmitterTransform"),
        (0xc0, 0x0c) => Some("UserC0_0C_SetParticleEmissionPercent"),
        (0xc0, 0x0d) => Some("UserC0_0D_AdvanceParticleScreen"),
        (0xc0, 0x0f) => Some("UserC0_0F_ResetParticleScreen"),
        (0xc0, 0x10) => Some("UserC0_10_ConfigureParticleEmitter"),
        (0xc0, 0x18) => Some("UserC0_18_ConfigureParticleAnimationBank"),
        (0xc0, 0x1a) => Some("UserC0_1A_LoadParticleAnimationFrames"),
        (0xc0, 0x1b) => Some("UserC0_1B_CommitParticleAnimationFrames"),
        (0xc0, 0x1f) => Some("UserC0_1F_SetParticleInterpolationMode"),
        (0xc0, 0x20) => Some("UserC0_20_SetParticleStyle0"),
        (0xc0, 0x24) => Some("UserC0_24_DefineParticleStyle0"),
        (0xc0, 0x25) => Some("UserC0_25_ConfigureParticleStyle0"),
        (0xc0, 0x28) => Some("UserC0_28_SetParticleStyle1"),
        (0xc0, 0x29) => Some("UserC0_29_ConfigureParticleStyle1"),
        (0xc0, 0x2c) => Some("UserC0_2C_DefineParticleStyle1"),
        (0xc0, 0x2d) => Some("UserC0_2D_ConfigureParticleAdvancedStyle"),
        (0xc0, 0x40) => Some("UserC0_40_CreateRainScreen"),
        (0xc0, 0x41) => Some("UserC0_41_ReleaseRainScreen"),
        (0xc0, 0x42) => Some("UserC0_42_InitializeRainScreen"),
        (0xc0, 0x43) => Some("UserC0_43_SetRainTexture"),
        (0xc0, 0x44) => Some("UserC0_44_SetRainEnabled"),
        (0xc0, 0x45) => Some("UserC0_45_ConfigureRainDisplay"),
        (0xc0, 0x46) => Some("UserC0_46_SetRainVolumeBounds"),
        (0xc0, 0x47) => Some("UserC0_47_SetRainDropWidth"),
        (0xc0, 0x48) => Some("UserC0_48_SetRainDropHeight"),
        (0xc0, 0x49) => Some("UserC0_49_SetRainColor"),
        (0xc0, 0x4a) => Some("UserC0_4A_SetRainDensity"),
        (0xc0, 0x4b) => Some("UserC0_4B_SetRainSpeed"),
        (0xc0, 0x4c) => Some("UserC0_4C_SetRainOrigin"),
        (0xc0, 0x4d) => Some("UserC0_4D_SetRainDirection"),
        (0xc0, 0x4e) => Some("UserC0_4E_SetRainLength"),
        (0xc0, 0x4f) => Some("UserC0_4F_SetRainStep"),
        (0xc0, 0xc0) => Some("UserC0_C0_CreateSpline"),
        (0xc0, 0xc1) => Some("UserC0_C1_ReleaseSpline"),
        (0xc0, 0xc2) => Some("UserC0_C2_ConfigureSpline"),
        (0xc0, 0xc3) => Some("UserC0_C3_SampleSpline"),
        (0xc0, 0xf0) => Some("UserC0_F0_LoadBwefTable"),
        _ => native_abi::generic_name(group, id),
    }
}

/// Compatibility alias for older callers.
///
/// The returned label is not necessarily target-confirmed. New code must use
/// [`candidate_call_name`] and consult the native recovery classification
/// before treating a label as semantic evidence.
#[deprecated(note = "use candidate_call_name; names are diagnostic candidates, not target proof")]
pub fn known_call_name(group: u8, id: u16) -> Option<&'static str> {
    candidate_call_name(group, id)
}

pub fn disassemble_bp(buf: &[u8]) -> Vec<BpInstruction> {
    parse_bp_program(None, buf).instructions
}

pub fn parse_bp_program(script_name: Option<String>, buf: &[u8]) -> BpProgram {
    let range = detect_code_range(buf);
    let module_size = declared_module_size(buf);
    let mut cursor = Cursor::new(&buf[range.start..range.end]);
    let mut instructions = Vec::new();
    let mut labels = HashMap::new();
    let mut strings = Vec::new();
    let mut warnings = Vec::new();

    while (cursor.position() as usize) < range.end - range.start {
        let relative = cursor.position() as usize;
        let offset = (range.start + relative) as u64;
        let opcode_byte = match cursor.read_u8() {
            Ok(op) => op,
            Err(_) => break,
        };
        let mut raw = vec![opcode_byte];
        let mut operands = Vec::new();
        let mut warning = None;
        let mut known_call = None;
        let mut opcode_name_override = None;

        match opcode_byte {
            0x00 | 0x08 | 0x09 | 0x0a => {
                if let Some(value) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U8(value));
                }
            }
            0x01 => {
                if let Some(value) = read_u16_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U16(value));
                }
            }
            0x02 => {
                if let Some(value) = read_u32_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U32(value));
                }
            }
            0x04..=0x06 => {
                if let Some(value) = read_u16_operand(&mut cursor, &mut raw, &mut warning) {
                    if opcode_byte == 0x04 {
                        operands.push(BpOperand::U16(value));
                    } else if opcode_byte == 0x05 {
                        let target = (offset as i64 + value as i16 as i64) as usize;
                        if let Some(text) = read_c_string(buf, target) {
                            strings.push(text.clone());
                            operands.push(BpOperand::String(text));
                        } else {
                            operands.push(BpOperand::Offset(target as u32));
                            warning = Some(format!("unresolved string reference 0x{target:08x}"));
                        }
                    } else {
                        let target = (offset as i64 + value as i16 as i64) as u32;
                        operands.push(BpOperand::Offset(target));
                    }
                }
            }
            0x0b => {
                if let Some(count) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    let mut bytes = Vec::new();
                    for _ in 0..count {
                        if let Some(byte) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                            bytes.push(byte);
                        }
                    }
                    operands.push(BpOperand::Raw(bytes));
                }
            }
            0x0c => {
                if let Some(width) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U8(width));
                }
                if let Some(count) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U8(count));
                }
            }
            0x15 | 0x80 | 0x81 | 0x90 | 0x91 | 0x92 | 0xa0 | 0xb0 | 0xc0 | 0xd0 | 0xe0 => {
                if let Some(value) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U8(value));
                    if !matches!(opcode_byte, 0xd0 | 0xe0) {
                        known_call = candidate_call_name(opcode_byte, value as u16);
                    }
                }
            }
            0xff => {
                if let Some(value) = read_u8_operand(&mut cursor, &mut raw, &mut warning) {
                    operands.push(BpOperand::U8(value));
                    opcode_name_override = Some(match value {
                        0xf0 => "script_load",
                        0xf1 => "script_free",
                        0xf8 => "script_ret",
                        _ => "script_call",
                    });
                }
            }
            0x14 | 0x16 | 0x17 => {}
            _ => {}
        }

        let opcode = if let Some(name) = opcode_name_override {
            BpOpcode::Known {
                code: opcode_byte,
                name,
            }
        } else {
            BpOpcode::from_byte(opcode_byte)
        };
        let instruction_index = instructions.len();
        labels.insert(offset as u32, instruction_index);
        if let Some(warning_text) = &warning {
            warnings.push(format!("0x{offset:08X}: {warning_text}"));
        }
        instructions.push(BpInstruction {
            offset,
            opcode_hex: format!("0x{opcode_byte:02X}"),
            opcode_name: opcode.name().to_string(),
            opcode,
            operands,
            known_call,
            raw,
            warning,
        });
    }

    let mut functions = Vec::new();
    for inst in &instructions {
        for operand in &inst.operands {
            if let BpOperand::Offset(offset) = operand
                && labels.contains_key(offset)
            {
                functions.push(BpFunction {
                    name: Some(format!("sub_{offset:08X}")),
                    offset: *offset,
                });
            }
        }
    }
    functions.sort_by_key(|f| f.offset);
    functions.dedup_by_key(|f| f.offset);

    BpProgram {
        script_name,
        functions,
        strings,
        instructions,
        labels,
        warnings,
        module_size,
    }
}

/// Header `instr_size` of a BP image (`a2[1]` in sub_465AB0); zero for images
/// without a consistent header.
fn declared_module_size(buf: &[u8]) -> u32 {
    if buf.len() >= 8 {
        let header_size = u32::from_le_bytes([buf[0], buf[1], buf[2], buf[3]]) as usize;
        let instr_size = u32::from_le_bytes([buf[4], buf[5], buf[6], buf[7]]) as usize;
        if header_size >= 8 && header_size + instr_size == buf.len() {
            return instr_size as u32;
        }
    }
    0
}

fn detect_code_range(buf: &[u8]) -> CodeRange {
    if buf.len() >= 8 {
        let header_size = u32::from_le_bytes([buf[0], buf[1], buf[2], buf[3]]) as usize;
        let instr_size = u32::from_le_bytes([buf[4], buf[5], buf[6], buf[7]]) as usize;
        if header_size >= 8
            && header_size <= buf.len()
            && instr_size <= buf.len()
            && header_size + instr_size == buf.len()
        {
            let nominal_end = header_size + instr_size;
            let end = detect_bp_code_end(buf, header_size, nominal_end);
            return CodeRange {
                start: header_size,
                end,
            };
        }
    }
    CodeRange {
        start: 0,
        end: buf.len(),
    }
}

fn detect_bp_code_end(buf: &[u8], start: usize, nominal_end: usize) -> usize {
    let Some(last_ret_relative) = buf[start..nominal_end]
        .iter()
        .rposition(|byte| *byte == 0x17)
    else {
        return nominal_end;
    };
    start + last_ret_relative + 1
}

fn read_u8_operand(
    cursor: &mut Cursor<&[u8]>,
    raw: &mut Vec<u8>,
    warning: &mut Option<String>,
) -> Option<u8> {
    match cursor.read_u8() {
        Ok(value) => {
            raw.push(value);
            Some(value)
        }
        Err(_) => {
            *warning = Some("truncated u8 operand".into());
            None
        }
    }
}

fn read_u16_operand(
    cursor: &mut Cursor<&[u8]>,
    raw: &mut Vec<u8>,
    warning: &mut Option<String>,
) -> Option<u16> {
    let mut bytes = [0u8; 2];
    match cursor.read_exact(&mut bytes) {
        Ok(()) => {
            raw.extend_from_slice(&bytes);
            Some(u16::from_le_bytes(bytes))
        }
        Err(_) => {
            *warning = Some("truncated u16 operand".into());
            None
        }
    }
}

fn read_u32_operand(
    cursor: &mut Cursor<&[u8]>,
    raw: &mut Vec<u8>,
    warning: &mut Option<String>,
) -> Option<u32> {
    match cursor.read_u32::<LittleEndian>() {
        Ok(value) => {
            raw.extend_from_slice(&value.to_le_bytes());
            Some(value)
        }
        Err(_) => {
            *warning = Some("truncated u32 operand".into());
            None
        }
    }
}

fn read_c_string(buf: &[u8], offset: usize) -> Option<String> {
    if offset >= buf.len() {
        return None;
    }
    let end = buf[offset..]
        .iter()
        .position(|&b| b == 0)
        .map(|pos| offset + pos)?;
    let (decoded, _, had_errors) = SHIFT_JIS.decode(&buf[offset..end]);
    if had_errors {
        None
    } else {
        Some(decoded.to_string())
    }
}

pub fn disassemble_file(path: &Path) -> Result<Vec<BpInstruction>> {
    let bytes = std::fs::read(path)?;
    Ok(disassemble_bp(&bytes))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn maps_required_opcode_names() {
        assert_eq!(opcode_name(0x00), Some("push_byte"));
        assert_eq!(opcode_name(0x91), Some("grp2"));
        assert_eq!(opcode_name(0x43), Some("atan2"));
        assert_eq!(opcode_name(0x44), Some("vec3_length"));
        assert_eq!(opcode_name(0x77), Some("engine_state"));
        assert_eq!(opcode_name(0xff), Some("script_extension"));
    }

    #[test]
    fn names_every_opcode_registered_by_the_native_interpreter() {
        assert_eq!(NATIVE_REGISTERED_OPCODES.len(), 89);
        assert!(
            NATIVE_REGISTERED_OPCODES
                .iter()
                .all(|opcode| opcode_name(*opcode).is_some())
        );
    }

    #[test]
    fn secondary_opcode_domains_consume_their_selector() {
        let instructions = disassemble_bp(&[0xd0, 0x40, 0xe0, 0x80, 0x17]);
        assert_eq!(instructions.len(), 3);
        assert_eq!(instructions[0].raw, vec![0xd0, 0x40]);
        assert_eq!(instructions[0].opcode.name(), "legacy_3d");
        assert_eq!(instructions[1].raw, vec![0xe0, 0x80]);
        assert_eq!(instructions[1].opcode.name(), "debug_inspect");
    }

    #[test]
    fn target_inline_copy_uses_count_followed_by_exact_payload() {
        let instructions = disassemble_bp(&[0x0b, 3, 0xaa, 0xbb, 0xcc, 0x44, 0x17]);

        assert_eq!(instructions.len(), 3);
        assert_eq!(instructions[0].raw, vec![0x0b, 3, 0xaa, 0xbb, 0xcc]);
        assert_eq!(
            instructions[0].operands,
            vec![BpOperand::Raw(vec![0xaa, 0xbb, 0xcc])]
        );
        assert_eq!(instructions[1].opcode.name(), "vec3_length");
    }

    #[test]
    fn opcode_reference_matches_target_stack_and_immediate_abi() {
        let move_spec = vm_opcode::lookup_bp_opcode(0x09).unwrap();
        assert_eq!(move_spec.stack_effect, "2 -> 1");
        let inline_copy = vm_opcode::lookup_bp_opcode(0x0b).unwrap();
        assert_eq!(inline_copy.stack_effect, "1 -> 0");
        assert!(inline_copy.immediate.contains("payload"));
        let jump = vm_opcode::lookup_bp_opcode(0x14).unwrap();
        assert_eq!(jump.stack_effect, "1 -> 0");
        assert_eq!(jump.immediate, "none");
        let conditional = vm_opcode::lookup_bp_opcode(0x15).unwrap();
        assert_eq!(conditional.stack_effect, "2 -> 0");
        assert_eq!(conditional.immediate, "u8 predicate selector (0..5)");
    }

    #[test]
    fn decodes_conservatively() {
        let instructions = disassemble_bp(&[0x00, 0x7f, 0xff, 0x17]);
        assert_eq!(instructions.len(), 2);
        assert_eq!(instructions[0].raw, vec![0x00, 0x7f]);
        assert_eq!(instructions[1].raw, vec![0xff, 0x17]);
        assert_eq!(instructions[1].opcode.name(), "script_call");
    }

    #[test]
    fn short_input_does_not_panic() {
        let instructions = disassemble_bp(&[0x02, 0x01]);
        assert_eq!(instructions.len(), 1);
        assert!(instructions[0].warning.is_some());
    }

    #[test]
    fn bp_alignment_padding_after_the_final_return_is_not_code() {
        let mut bytes = vec![0; 16];
        bytes[..4].copy_from_slice(&16_u32.to_le_bytes());
        bytes[4..8].copy_from_slice(&5_u32.to_le_bytes());
        bytes.extend([0x17, 0, 0, 0, 0]);

        let instructions = disassemble_bp(&bytes);

        assert_eq!(instructions.len(), 1);
        assert_eq!(instructions[0].opcode.name(), "ret");
        assert_eq!(instructions[0].offset, 0x10);
    }

    #[test]
    fn known_call_table_is_seeded() {
        assert_eq!(
            candidate_call_name(0x91, 0x88),
            Some("Graph91_88_ConfigureWindowFont")
        );
        assert_eq!(candidate_call_name(0x92, 0x9c), Some("GraphRenderText"));
    }
}
