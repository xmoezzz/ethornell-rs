import QuartzCore
import UIKit

/// A view backed by the CAMetalLayer the engine renders into.
final class MetalView: UIView {
    override class var layerClass: AnyClass { CAMetalLayer.self }
}

/// Plays one game: steps the engine on every display refresh and forwards
/// touches, keys and the game's dialogs. One finger is the left button, a
/// two-finger tap a right click (the game menu), a two-finger vertical drag
/// the wheel (the backlog), and a three-finger tap offers to close the game.
final class GameViewController: UIViewController {
    private let resourceRoot: URL
    private let userRoot: URL
    private let gbk: Bool
    private let onClose: () -> Void

    private var engine: Engine?
    private var displayLink: CADisplayLink?
    private var surfaceSize: (UInt32, UInt32) = (0, 0)
    private var stopped = false

    // The touch gesture in progress.
    private var leftDown = false
    private var multiTouch = false
    private var multiTouchMoved = false
    private var maxTouches = 0
    private var lastAverageY: CGFloat = 0
    private var wheelTravel: CGFloat = 0

    init(resourceRoot: URL, userRoot: URL, gbk: Bool, onClose: @escaping () -> Void) {
        self.resourceRoot = resourceRoot
        self.userRoot = userRoot
        self.gbk = gbk
        self.onClose = onClose
        super.init(nibName: nil, bundle: nil)
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) is not used")
    }

    override func loadView() {
        let view = MetalView()
        view.backgroundColor = .black
        view.isMultipleTouchEnabled = true
        view.contentScaleFactor = UIScreen.main.nativeScale
        self.view = view
    }

    override var prefersStatusBarHidden: Bool { true }
    override var prefersHomeIndicatorAutoHidden: Bool { true }
    override var canBecomeFirstResponder: Bool { true }

    override func viewDidLoad() {
        super.viewDidLoad()
        do {
            engine = try Engine(resourceRoot: resourceRoot, userRoot: userRoot, gbk: gbk)
        } catch {
            fail("\(error)")
            return
        }
        let link = CADisplayLink(target: self, selector: #selector(frame))
        link.add(to: .main, forMode: .common)
        displayLink = link
        let center = NotificationCenter.default
        center.addObserver(self, selector: #selector(resignActive),
                           name: UIApplication.willResignActiveNotification, object: nil)
        center.addObserver(self, selector: #selector(becomeActive),
                           name: UIApplication.didBecomeActiveNotification, object: nil)
    }

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        becomeFirstResponder()
    }

    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        guard let engine, !stopped else { return }
        let scale = view.contentScaleFactor
        let size = (UInt32(view.bounds.width * scale), UInt32(view.bounds.height * scale))
        guard size != surfaceSize else { return }
        surfaceSize = size
        if size.0 == 0 || size.1 == 0 {
            engine.detachSurface()
            return
        }
        do {
            try engine.setSurface(
                view: Unmanaged.passUnretained(view).toOpaque(), widthPx: size.0, heightPx: size.1)
        } catch {
            fail("\(error)")
        }
    }

    @objc private func frame() {
        guard let engine, !stopped else { return }
        do {
            try engine.step()
        } catch {
            fail("\(error)")
            return
        }
        while let event = engine.pollEvent() {
            switch event.kind {
            case .diagnostic:
                NSLog("ethornell: %@", event.text)
            case .confirmation:
                showRequest(event)
            case .dialog:
                showDialog(event)
            case .closed:
                stop()
                onClose()
                return
            case .title:
                break
            }
        }
    }

    @objc private func resignActive() {
        engine?.setPaused(true)
    }

    @objc private func becomeActive() {
        engine?.setPaused(false)
    }

    private func stop() {
        stopped = true
        displayLink?.invalidate()
        displayLink = nil
        NotificationCenter.default.removeObserver(self)
        engine?.destroy()
        engine = nil
    }

    private func fail(_ message: String) {
        stop()
        let alert = UIAlertController(
            title: NSLocalizedString("The game stopped", comment: ""),
            message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default) { [onClose] _ in onClose() })
        DispatchQueue.main.async { self.present(alert, animated: true) }
    }

    /// A request that takes a result is a yes/no question (1 yes, 0 no).
    private func showRequest(_ event: Engine.Event) {
        let alert = UIAlertController(title: nil, message: event.text, preferredStyle: .alert)
        let id = event.requestId
        if event.code != 0 {
            alert.addAction(UIAlertAction(title: NSLocalizedString("Yes", comment: ""), style: .default) { [weak self] _ in
                self?.engine?.completeRequest(id, result: 1)
            })
            alert.addAction(UIAlertAction(title: NSLocalizedString("No", comment: ""), style: .cancel) { [weak self] _ in
                self?.engine?.completeRequest(id, result: 0)
            })
        } else {
            alert.addAction(UIAlertAction(title: "OK", style: .default) { [weak self] _ in
                self?.engine?.completeRequest(id, result: 0)
            })
        }
        present(alert, animated: true)
    }

    /// An input dialog (kind 5). The engine filters and limits the answers
    /// itself, so plain text fields are enough here.
    private func showDialog(_ event: Engine.Event) {
        let id = event.requestId
        guard let engine, let request = engine.requestPayload(id),
              let type = request["type"] as? String else { return }
        let title = request["title"] as? String
        let prompt = request["prompt"] as? String
        let alert = UIAlertController(title: title, message: prompt, preferredStyle: .alert)
        let cancel = UIAlertAction(title: NSLocalizedString("Cancel", comment: ""), style: .cancel) { [weak self] _ in
            self?.engine?.completeDialog(id, answer: nil)
        }
        let texts = { (alert.textFields ?? []).map { $0.text ?? "" } }
        let addField = { (placeholder: String, initial: String, numeric: Bool) in
            alert.addTextField { field in
                field.placeholder = placeholder
                field.text = initial
                field.keyboardType = numeric ? .numbersAndPunctuation : .default
            }
        }
        switch type {
        case "input":
            for field in request["fields"] as? [[String: Any]] ?? [] {
                addField(field["label"] as? String ?? "", field["initial"] as? String ?? "",
                         field["numeric"] as? Bool ?? false)
            }
            alert.addAction(UIAlertAction(title: "OK", style: .default) { [weak self] _ in
                self?.engine?.completeDialog(id, answer: ["texts": texts()])
            })
        case "segmented":
            let count = request["segments"] as? Int ?? 0
            let numeric = request["numeric"] as? Bool ?? false
            for index in 0..<count {
                addField("\(index + 1)/\(count)", "", numeric)
            }
            alert.addAction(UIAlertAction(title: "OK", style: .default) { [weak self] _ in
                self?.engine?.completeDialog(id, answer: ["texts": texts()])
            })
        case "select":
            for (index, option) in (request["options"] as? [String] ?? []).enumerated() {
                alert.addAction(UIAlertAction(title: option, style: .default) { [weak self] _ in
                    self?.engine?.completeDialog(id, answer: ["index": index])
                })
            }
        case "date":
            let fields = request["fields"] as? [String] ?? []
            for index in 0..<4 {
                addField(String(format: NSLocalizedString("Field %d", comment: ""), index + 1),
                         index < fields.count ? fields[index] : "", false)
            }
            addField(NSLocalizedString("Month (1-12)", comment: ""),
                     String((request["month"] as? Int ?? 0) + 1), true)
            addField(NSLocalizedString("Day", comment: ""),
                     String((request["day"] as? Int ?? 0) + 1), true)
            alert.addAction(UIAlertAction(title: "OK", style: .default) { [weak self] _ in
                let values = texts()
                let number = { (index: Int) in Int(values[index].trimmingCharacters(in: .whitespaces)) ?? 1 }
                self?.engine?.completeDialog(id, answer: [
                    "texts": Array(values.prefix(4)),
                    "month": number(4) - 1,
                    "day": number(5) - 1,
                ])
            })
        default:
            engine.completeDialog(id, answer: nil)
            return
        }
        alert.addAction(cancel)
        present(alert, animated: true)
    }

    private func confirmClose() {
        let alert = UIAlertController(
            title: NSLocalizedString("Close the game?", comment: ""), message: nil,
            preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: NSLocalizedString("Close", comment: ""), style: .destructive) { [weak self] _ in
            // The game may ask for its own confirmation first.
            self?.engine?.requestClose()
        })
        alert.addAction(UIAlertAction(title: NSLocalizedString("Cancel", comment: ""), style: .cancel))
        present(alert, animated: true)
    }

    // MARK: Touches

    private func pixels(_ point: CGPoint) -> (Float, Float) {
        let scale = view.contentScaleFactor
        return (Float(point.x * scale), Float(point.y * scale))
    }

    private func averageY(_ touches: Set<UITouch>) -> CGFloat {
        let ys = touches.map { $0.location(in: view).y }
        return ys.reduce(0, +) / CGFloat(max(ys.count, 1))
    }

    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let engine, let all = event?.allTouches else { return }
        maxTouches = max(maxTouches, all.count)
        if all.count == 1, let touch = touches.first {
            multiTouch = false
            leftDown = true
            let (x, y) = pixels(touch.location(in: view))
            engine.input(.move, x: x, y: y)
            engine.input(.buttonDown, x: x, y: y, code: Engine.leftButton)
        } else if !multiTouch {
            // Not a left press after all.
            multiTouch = true
            multiTouchMoved = false
            leftDown = false
            lastAverageY = averageY(all)
            wheelTravel = 0
            engine.input(.cancel)
        }
    }

    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let engine, let all = event?.allTouches else { return }
        if multiTouch {
            let y = averageY(all)
            wheelTravel += y - lastAverageY
            lastAverageY = y
            let notch: CGFloat = 48
            while abs(wheelTravel) >= notch {
                multiTouchMoved = true
                // Fingers moving down scroll up, like a wheel turned up.
                let delta: CGFloat = wheelTravel > 0 ? 1 : -1
                wheelTravel -= delta * notch
                engine.input(.wheel, value: Float(delta))
            }
        } else if leftDown, let touch = touches.first {
            let (x, y) = pixels(touch.location(in: view))
            engine.input(.move, x: x, y: y)
        }
    }

    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let engine else { return }
        let remaining = (event?.allTouches ?? []).filter { $0.phase != .ended && $0.phase != .cancelled }
        if leftDown, let touch = touches.first {
            let (x, y) = pixels(touch.location(in: view))
            engine.input(.move, x: x, y: y)
            engine.input(.buttonUp, x: x, y: y, code: Engine.leftButton)
            leftDown = false
        } else if multiTouch, !multiTouchMoved, let touch = touches.first {
            multiTouchMoved = true
            if maxTouches >= 3 {
                confirmClose()
            } else {
                let (x, y) = pixels(touch.location(in: view))
                engine.input(.buttonDown, x: x, y: y, code: Engine.rightButton)
                engine.input(.buttonUp, x: x, y: y, code: Engine.rightButton)
            }
        }
        if remaining.isEmpty {
            multiTouch = false
            maxTouches = 0
        }
    }

    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        leftDown = false
        multiTouch = false
        maxTouches = 0
        engine?.input(.cancel)
    }

    // MARK: Hardware keyboards

    /// Windows virtual-key codes, which the scripts bind input classes to.
    private static func virtualKey(_ key: UIKey) -> Int32? {
        switch key.keyCode {
        case .keyboardReturnOrEnter, .keypadEnter: return 0x0D
        case .keyboardEscape: return 0x1B
        case .keyboardSpacebar: return 0x20
        case .keyboardTab: return 0x09
        case .keyboardDeleteOrBackspace: return 0x08
        case .keyboardLeftShift, .keyboardRightShift: return 0x10
        case .keyboardLeftControl, .keyboardRightControl: return 0x11
        case .keyboardPageUp: return 0x21
        case .keyboardPageDown: return 0x22
        case .keyboardLeftArrow: return 0x25
        case .keyboardUpArrow: return 0x26
        case .keyboardRightArrow: return 0x27
        case .keyboardDownArrow: return 0x28
        default:
            let code = key.keyCode.rawValue
            if (UIKeyboardHIDUsage.keyboardF1.rawValue...UIKeyboardHIDUsage.keyboardF12.rawValue).contains(code) {
                return 0x70 + Int32(code - UIKeyboardHIDUsage.keyboardF1.rawValue)
            }
            if (UIKeyboardHIDUsage.keyboardA.rawValue...UIKeyboardHIDUsage.keyboardZ.rawValue).contains(code) {
                return 0x41 + Int32(code - UIKeyboardHIDUsage.keyboardA.rawValue)
            }
            return nil
        }
    }

    override func pressesBegan(_ presses: Set<UIPress>, with event: UIPressesEvent?) {
        var handled = false
        for press in presses {
            if let key = press.key, let vk = GameViewController.virtualKey(key) {
                engine?.input(.keyDown, code: vk)
                handled = true
            }
        }
        if !handled {
            super.pressesBegan(presses, with: event)
        }
    }

    override func pressesEnded(_ presses: Set<UIPress>, with event: UIPressesEvent?) {
        var handled = false
        for press in presses {
            if let key = press.key, let vk = GameViewController.virtualKey(key) {
                engine?.input(.keyUp, code: vk)
                handled = true
            }
        }
        if !handled {
            super.pressesEnded(presses, with: event)
        }
    }
}
