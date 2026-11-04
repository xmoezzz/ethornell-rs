import CoreText
import Foundation

/// The engine in libethornell_mobile.a (crates/ethornell-mobile), through the C ABI
/// of platform/ios/include/ethornell_ios.h. Use it from the main thread only.
final class Engine {
    enum InputKind: UInt32 {
        case move = 1, buttonDown, buttonUp, keyDown, keyUp, wheel, cancel
    }

    /// Windows button descriptors.
    static let leftButton: Int32 = 1
    static let rightButton: Int32 = 2

    struct Event {
        enum Kind: UInt32 {
            case diagnostic = 1, confirmation, closed, title, dialog
        }
        let kind: Kind
        let requestId: UInt64
        /// For a confirmation: 1 when it takes a yes/no result.
        let code: Int32
        let text: String
    }

    struct Failure: Error, CustomStringConvertible {
        let description: String
    }

    private(set) var handle: UInt64 = 0
    private var generation: UInt64 = 0

    init(resourceRoot: URL, userRoot: URL, gbk: Bool) throws {
        let font = Engine.systemFont(postScriptName: gbk ? "PingFangSC-Regular" : "HiraginoSans-W3")
        let status = Engine.withStringV1(resourceRoot.path) { resource in
            Engine.withStringV1(userRoot.path) { user in
                Engine.withStringV1(font?.path ?? "") { fontPath in
                    var config = EthornellConfigV1(
                        struct_size: UInt32(MemoryLayout<EthornellConfigV1>.size),
                        abi_version: ETHORNELL_IOS_ABI_VERSION,
                        resource_root: resource,
                        user_root: user,
                        font_path: fontPath,
                        text_encoding: gbk ? 1 : 0,
                        font_face_index: font?.face ?? 0,
                        backend: 0,
                        flags: 0,
                        reserved: (0, 0, 0, 0))
                    return ethornell_ios_create(&config, &handle)
                }
            }
        }
        if status != 0 {
            throw Failure(description: Engine.lastError())
        }
    }

    deinit {
        destroy()
    }

    func destroy() {
        if handle != 0 {
            _ = ethornell_ios_destroy(handle)
            handle = 0
        }
    }

    /// Attaches `view` at a new size; the view must stay alive until
    /// `detachSurface` or `destroy`.
    func setSurface(view: UnsafeMutableRawPointer, widthPx: UInt32, heightPx: UInt32) throws {
        generation += 1
        if ethornell_ios_set_surface(handle, view, widthPx, heightPx, generation) != 0 {
            throw Failure(description: Engine.lastError())
        }
    }

    func detachSurface() {
        _ = ethornell_ios_detach_surface(handle, generation)
    }

    func step() throws {
        if ethornell_ios_step(handle, clock_gettime_nsec_np(CLOCK_UPTIME_RAW)) < 0 {
            throw Failure(description: Engine.lastError())
        }
    }

    func input(_ kind: InputKind, x: Float = 0, y: Float = 0, code: Int32 = 0, value: Float = 0) {
        var input = EthornellInputV1(
            struct_size: UInt32(MemoryLayout<EthornellInputV1>.size),
            kind: kind.rawValue, x: x, y: y, code: code, value: value, reserved: (0, 0))
        _ = ethornell_ios_input(handle, &input)
    }

    func setPaused(_ paused: Bool) {
        _ = ethornell_ios_set_paused(handle, paused ? 1 : 0)
    }

    func requestClose() {
        _ = ethornell_ios_request_close(handle)
    }

    func pollEvent() -> Event? {
        var event = EthornellEventV1()
        event.struct_size = UInt32(MemoryLayout<EthornellEventV1>.size)
        guard ethornell_ios_poll_event(handle, &event) == 0,
              let kind = Event.Kind(rawValue: event.kind) else {
            return nil
        }
        let length = Int(event.text_len)
        let text = withUnsafeBytes(of: &event.text) { bytes in
            String(decoding: bytes.prefix(length), as: UTF8.self)
        }
        return Event(kind: kind, requestId: event.request_id, code: event.code, text: text)
    }

    func completeRequest(_ requestId: UInt64, result: Int32) {
        var response = EthornellResponseV1(
            struct_size: UInt32(MemoryLayout<EthornellResponseV1>.size),
            result: result,
            text: EthornellStringV1(ptr: nil, len: 0, reserved: 0))
        _ = ethornell_ios_complete_request(handle, requestId, &response)
    }

    /// The JSON description of a dialog request (kind 5).
    func requestPayload(_ requestId: UInt64) -> [String: Any]? {
        let size = ethornell_ios_request_payload(handle, requestId, nil, 0)
        guard size > 1 else { return nil }
        var buffer = [CChar](repeating: 0, count: Int(size))
        guard ethornell_ios_request_payload(handle, requestId, &buffer, UInt32(size)) == size else {
            return nil
        }
        let data = Data(String(cString: buffer).utf8)
        return (try? JSONSerialization.jsonObject(with: data)) as? [String: Any]
    }

    /// Answers a dialog request; `answer` nil cancels it.
    func completeDialog(_ requestId: UInt64, answer: [String: Any]?) {
        var object = answer ?? [:]
        object["accepted"] = answer != nil
        guard let data = try? JSONSerialization.data(withJSONObject: object),
              let json = String(data: data, encoding: .utf8) else { return }
        _ = Engine.withStringV1(json) { text in
            var response = EthornellResponseV1(
                struct_size: UInt32(MemoryLayout<EthornellResponseV1>.size),
                result: 0,
                text: text)
            return ethornell_ios_complete_request(handle, requestId, &response)
        }
    }

    static func lastError() -> String {
        let size = ethornell_ios_last_error(nil, 0)
        guard size > 1 else { return "unknown error" }
        var buffer = [CChar](repeating: 0, count: Int(size))
        _ = ethornell_ios_last_error(&buffer, UInt32(size))
        return String(cString: buffer)
    }

    private static func withStringV1<T>(_ text: String, _ body: (EthornellStringV1) -> T) -> T {
        var bytes = Array(text.utf8)
        return bytes.withUnsafeMutableBufferPointer { buffer in
            body(EthornellStringV1(
                ptr: UnsafePointer(buffer.baseAddress), len: UInt32(buffer.count), reserved: 0))
        }
    }

    /// The file and face index of an installed font. iOS keeps PingFang
    /// outside /System/Library/Fonts, so the engine's own search can't
    /// find it.
    private static func systemFont(postScriptName: String) -> (path: String, face: UInt32)? {
        let font = CTFontCreateWithName(postScriptName as CFString, 16, nil)
        guard CTFontCopyPostScriptName(font) as String == postScriptName,
              let url = CTFontCopyAttribute(font, kCTFontURLAttribute) as? URL else {
            return nil
        }
        let faces = CTFontManagerCreateFontDescriptorsFromURL(url as CFURL) as? [CTFontDescriptor] ?? []
        let face = faces.firstIndex { descriptor in
            CTFontDescriptorCopyAttribute(descriptor, kCTFontNameAttribute) as? String == postScriptName
        } ?? 0
        return (url.path, UInt32(face))
    }
}
