// The launcher page: picks the game folder and a font, starts the engine
// worker on a transferred canvas, forwards input, plays the worker's audio
// and shows the game's dialogs.
const $ = (id) => document.getElementById(id);
const status = (text) => ($("status").textContent = text);

// Input kinds and buttons of the host ABI (platform/android/include/ABI.md).
const MOVE = 1, DOWN = 2, UP = 3, KEY_DOWN = 4, KEY_UP = 5, WHEEL = 6, CANCEL = 7;
const LEFT = 1, RIGHT = 2;

// --- Settings (the chosen font) ------------------------------------------

const settings = new Promise((resolve, reject) => {
  const request = indexedDB.open("ethornell-settings", 1);
  request.onupgradeneeded = () => request.result.createObjectStore("settings");
  request.onsuccess = () => resolve(request.result);
  request.onerror = () => reject(request.error);
});

async function setting(key, value) {
  const db = await settings;
  return new Promise((resolve, reject) => {
    const transaction = db.transaction("settings", value === undefined ? "readonly" : "readwrite");
    const objects = transaction.objectStore("settings");
    const request = value === undefined ? objects.get(key) : objects.put(value, key);
    transaction.oncomplete = () => resolve(request.result);
    transaction.onerror = () => reject(transaction.error);
  });
}

let storedFont = null; // { name, bytes: ArrayBuffer }

async function showFont() {
  storedFont = (await setting("font").catch(() => null)) || null;
  $("font-status").textContent = storedFont
    ? `Font: ${storedFont.name}`
    : "No font chosen: the game's own font is used, else a Japanese system font (the browser may ask).";
}

$("font").addEventListener("change", async (event) => {
  const file = event.target.files[0];
  if (!file) return;
  await setting("font", { name: file.name, bytes: await file.arrayBuffer() });
  await showFont();
});

// Local Font Access (Chromium): a Japanese/Chinese system font.
const SYSTEM_FONTS = [
  "Noto Sans CJK JP", "Noto Sans JP", "Yu Gothic", "Meiryo", "MS Gothic",
  "Hiragino Sans", "Hiragino Kaku Gothic ProN", "Source Han Sans JP",
  "Microsoft YaHei", "PingFang SC", "Noto Sans CJK SC",
];

async function systemFont() {
  if (!globalThis.queryLocalFonts) return null;
  const fonts = await queryLocalFonts().catch(() => []);
  for (const family of SYSTEM_FONTS) {
    const font = fonts.find((candidate) => candidate.family === family && /regular|w3|normal/i.test(candidate.style))
      || fonts.find((candidate) => candidate.family === family);
    if (font) return { name: font.fullName, bytes: await (await font.blob()).arrayBuffer() };
  }
  return null;
}

async function chooseFont(entries) {
  if (storedFont) return storedFont;
  const own = entries.find(({ path }) => !path.includes("/") && /\.(ttf|otf|ttc)$/i.test(path));
  if (own) return { name: own.path, bytes: await own.file.arrayBuffer() };
  return systemFont();
}

// --- Starting a game -----------------------------------------------------

$("folder").addEventListener("change", async (event) => {
  const picked = [...event.target.files];
  event.target.value = "";
  if (picked.length === 0) return;
  // webkitRelativePath starts with the picked folder's own name.
  const gameId = picked[0].webkitRelativePath.split("/")[0];
  const entries = picked.map((file) => ({
    file,
    path: file.webkitRelativePath.split("/").slice(1).join("/"),
  }));
  if (!entries.some(({ path }) => path.toLowerCase() === "system.arc")) {
    status(`${gameId} has no system.arc; pick the folder the game's .exe is in.`);
    return;
  }
  status("Starting…");
  try {
    const font = await chooseFont(entries);
    if (!font) {
      status("No font available: choose a Japanese font file (for example NotoSansJP-Regular.otf).");
      return;
    }
    await start(gameId, entries, font);
  } catch (error) {
    status(String(error));
  }
});

let worker = null;
let audio = null;

async function startAudio() {
  if ($("silent").checked) return null;
  const context = new AudioContext();
  await context.audioWorklet.addModule("audio-worklet.js");
  const node = new AudioWorkletNode(context, "ethornell-output", { outputChannelCount: [2] });
  node.connect(context.destination);
  const channel = new MessageChannel();
  node.port.postMessage({ port: channel.port1 }, [channel.port1]);
  return { context, port: channel.port2 };
}

function backingSize(canvas) {
  const scale = window.devicePixelRatio || 1;
  return {
    width: Math.max(1, Math.round(canvas.clientWidth * scale)),
    height: Math.max(1, Math.round(canvas.clientHeight * scale)),
  };
}

async function start(gameId, entries, font) {
  audio = await startAudio().catch((error) => {
    console.warn("audio unavailable", error);
    return null;
  });
  $("launcher").hidden = true;
  $("stage").hidden = false;
  const canvas = $("screen");
  const size = backingSize(canvas);
  const offscreen = canvas.transferControlToOffscreen();
  worker = new Worker("worker.js", { type: "module" });
  worker.onmessage = onWorkerMessage;
  worker.onerror = (event) => fail(event.message);
  const transfer = [offscreen];
  if (audio) transfer.push(audio.port);
  worker.postMessage(
    {
      type: "start",
      gameId,
      files: entries.map(({ file }) => file),
      paths: entries.map(({ path }) => path),
      font: font.bytes,
      gbk: $("gbk").checked,
      silent: !audio,
      sampleRate: audio ? audio.context.sampleRate : 48000,
      canvas: offscreen,
      audioPort: audio ? audio.port : null,
      ...size,
    },
    transfer,
  );
  attachInput(canvas);
  canvas.focus();
}

function fail(message) {
  $("stage").hidden = true;
  $("launcher").hidden = false;
  status(`The game stopped: ${message}`);
  if (audio) audio.context.close();
}

function onWorkerMessage(event) {
  const message = event.data;
  switch (message.type) {
    case "started":
      status("");
      break;
    case "title":
      document.title = message.text || "Ethornell";
      break;
    case "confirm":
      confirmPrompt(message);
      break;
    case "dialog":
      inputPrompt(message.id, JSON.parse(message.payload));
      break;
    case "closed":
      location.reload();
      break;
    case "error":
      fail(message.message);
      break;
  }
}

// --- Input ---------------------------------------------------------------

function send(kind, x = 0, y = 0, code = 0, value = 0) {
  worker.postMessage({ type: "input", kind, x, y, code, value });
}

function attachInput(canvas) {
  const point = (event) => {
    const scale = window.devicePixelRatio || 1;
    return [event.offsetX * scale, event.offsetY * scale];
  };
  const button = (event) => (event.button === 2 ? RIGHT : event.button === 0 ? LEFT : 0);
  canvas.addEventListener("pointermove", (event) => send(MOVE, ...point(event)));
  canvas.addEventListener("pointerdown", (event) => {
    const code = button(event);
    if (!code) return;
    canvas.setPointerCapture(event.pointerId);
    send(MOVE, ...point(event));
    send(DOWN, ...point(event), code);
  });
  canvas.addEventListener("pointerup", (event) => {
    const code = button(event);
    if (code) send(UP, ...point(event), code);
  });
  canvas.addEventListener("pointercancel", (event) => send(CANCEL, ...point(event)));
  canvas.addEventListener("contextmenu", (event) => event.preventDefault());
  canvas.addEventListener(
    "wheel",
    (event) => {
      event.preventDefault();
      if (event.deltaY) send(WHEEL, ...point(event), 0, event.deltaY < 0 ? 1 : -1);
    },
    { passive: false },
  );
  // Keys use Windows virtual-key codes, which keyCode still reports.
  const key = (kind) => (event) => {
    if ($("prompt").open || event.metaKey || !event.keyCode) return;
    if ([9, 13, 27, 32, 33, 34, 37, 38, 39, 40].includes(event.keyCode)) event.preventDefault();
    send(kind, 0, 0, event.keyCode);
  };
  window.addEventListener("keydown", key(KEY_DOWN));
  window.addEventListener("keyup", key(KEY_UP));
  let resizing = 0;
  new ResizeObserver(() => {
    clearTimeout(resizing);
    resizing = setTimeout(() => worker.postMessage({ type: "resize", ...backingSize(canvas) }), 100);
  }).observe(canvas);
  document.addEventListener("visibilitychange", () => {
    worker.postMessage({ type: "pause", mask: document.hidden ? 1 : 0 });
    if (audio) document.hidden ? audio.context.suspend() : audio.context.resume();
  });
}

// --- Dialogs -------------------------------------------------------------

const dialog = $("prompt");

function openPrompt(title, text, fields, buttons) {
  $("prompt-title").textContent = title || "";
  $("prompt-text").textContent = text || "";
  $("prompt-fields").replaceChildren(...fields);
  $("prompt-buttons").replaceChildren(
    ...buttons.map(({ label, primary, action }) => {
      const element = document.createElement("button");
      element.textContent = label;
      element.value = label;
      if (primary) element.className = "primary";
      element.addEventListener("click", (event) => {
        event.preventDefault();
        dialog.close();
        action();
        $("screen").focus();
      });
      return element;
    }),
  );
  dialog.showModal();
}

// A message (code 0) or a yes/no question (code 1); for Sys80:3C, yes retries.
function confirmPrompt({ id, code, text }) {
  const answer = (result) => () => worker.postMessage({ type: "answer", id, result });
  const buttons =
    code === 0
      ? [{ label: "OK", primary: true, action: answer(1) }]
      : [
          { label: "No", action: answer(0) },
          { label: "Yes", primary: true, action: answer(1) },
        ];
  dialog.oncancel = (event) => {
    event.preventDefault();
  };
  openPrompt(document.title, text, [], buttons);
}

function textField(label, value, maxBytes, numeric) {
  const wrapper = document.createElement("label");
  wrapper.textContent = label || "";
  const input = document.createElement("input");
  input.value = value || "";
  if (numeric) input.inputMode = "numeric";
  // The engine enforces the Shift-JIS byte limit; this only hints at it.
  if (maxBytes) input.maxLength = maxBytes;
  wrapper.append(input);
  return { wrapper, input };
}

function inputPrompt(id, request) {
  const respond = (response) =>
    worker.postMessage({ type: "dialog", id, response: JSON.stringify(response) });
  const cancel = () => respond({ accepted: false });
  dialog.oncancel = (event) => {
    event.preventDefault();
    dialog.close();
    cancel();
  };
  const cancelButton = { label: "Cancel", action: cancel };
  switch (request.type) {
    case "input": {
      const fields = request.fields.map((field) =>
        textField(request.fields.length > 1 ? field.label : "", field.initial, field.max_bytes, field.numeric),
      );
      openPrompt(request.title, "", fields.map((field) => field.wrapper), [
        cancelButton,
        { label: "OK", primary: true, action: () => respond({ accepted: true, texts: fields.map((field) => field.input.value) }) },
      ]);
      break;
    }
    case "segmented": {
      const fields = Array.from({ length: request.segments }, (_, index) =>
        textField(`${index + 1}`, "", request.max_bytes, request.numeric),
      );
      openPrompt(request.title, request.prompt, fields.map((field) => field.wrapper), [
        cancelButton,
        { label: "OK", primary: true, action: () => respond({ accepted: true, texts: fields.map((field) => field.input.value) }) },
      ]);
      break;
    }
    case "select": {
      const list = document.createElement("div");
      list.className = "options";
      request.options.forEach((option, index) => {
        const element = document.createElement("button");
        element.type = "button";
        element.textContent = option;
        element.addEventListener("click", () => {
          dialog.close();
          respond({ accepted: true, index });
          $("screen").focus();
        });
        list.append(element);
      });
      openPrompt(request.title, request.prompt, [list], [cancelButton]);
      break;
    }
    case "date": {
      const fields = request.fields.map((value, index) => textField(`${index + 1}`, value, 10, false));
      const month = textField("Month (1-12)", String(request.month + 1), 2, true);
      const day = textField("Day", String(request.day + 1), 2, true);
      const all = [...fields, month, day];
      openPrompt("", "", all.map((field) => field.wrapper), [
        cancelButton,
        {
          label: "OK",
          primary: true,
          action: () =>
            respond({
              accepted: true,
              texts: fields.map((field) => field.input.value),
              month: (parseInt(month.input.value, 10) || 1) - 1,
              day: (parseInt(day.input.value, 10) || 1) - 1,
            }),
        },
      ]);
      break;
    }
    default:
      cancel();
  }
}

showFont();
if (!("transferControlToOffscreen" in HTMLCanvasElement.prototype)) {
  status("This browser cannot run the engine off the main thread (no OffscreenCanvas).");
}
