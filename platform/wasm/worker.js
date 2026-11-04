// The engine worker. It owns the picked folder's File objects and reads them
// in place (FileReaderSync on slices), keeps the game's saves in IndexedDB,
// draws on the transferred canvas and steps the engine once per frame.
import init, {
  WebEngine,
  createRenderer,
  renderAudio,
  setAudioSampleRate,
} from "./pkg/ethornell_web.js";

const files = []; // id -> File, the order handed to WebEngine
const reader = new FileReaderSync();
let engine = null;
let canvas = null;
let gameId = "";
let stopped = false;

// --- Functions the wasm module imports from the worker's global scope ----

globalThis.ethornellReadAt = (id, offset, out) => {
  const file = files[id];
  if (!file) throw new Error(`no file ${id}`);
  const bytes = reader.readAsArrayBuffer(file.slice(offset, offset + out.length));
  out.set(new Uint8Array(bytes));
  return bytes.byteLength;
};

globalThis.ethornellSaveUserFile = (path, bytes) => {
  store("readwrite", (saves) => saves.put(bytes.slice(), [gameId, path]));
};

globalThis.ethornellRemoveUserFile = (path) => {
  store("readwrite", (saves) => saves.delete([gameId, path]));
};

// --- Saves: one IndexedDB record per [game, path] -------------------------

const database = new Promise((resolve, reject) => {
  const request = indexedDB.open("ethornell", 1);
  request.onupgradeneeded = () => request.result.createObjectStore("saves");
  request.onsuccess = () => resolve(request.result);
  request.onerror = () => reject(request.error);
});

function store(mode, action) {
  return database.then(
    (db) =>
      new Promise((resolve, reject) => {
        const transaction = db.transaction("saves", mode);
        const result = action(transaction.objectStore("saves"));
        transaction.oncomplete = () => resolve(result && result.result);
        transaction.onerror = () => reject(transaction.error);
      }),
  );
}

async function loadSaves(id) {
  const range = IDBKeyRange.bound([id, ""], [id, "￿"]);
  const db = await database;
  return new Promise((resolve, reject) => {
    const saves = [];
    const request = db.transaction("saves").objectStore("saves").openCursor(range);
    request.onsuccess = () => {
      const cursor = request.result;
      if (!cursor) return resolve(saves);
      saves.push([cursor.key[1], new Uint8Array(cursor.value)]);
      cursor.continue();
    };
    request.onerror = () => reject(request.error);
  });
}

// --- Engine loop ----------------------------------------------------------

const nextFrame = globalThis.requestAnimationFrame
  ? (callback) => requestAnimationFrame(callback)
  : (callback) => setTimeout(() => callback(performance.now()), 16);

function report(error) {
  stopped = true;
  postMessage({ type: "error", message: String(error && error.message ? error.message : error) });
}

function drainEvents() {
  for (;;) {
    const event = engine.pollEvent();
    if (!event) return;
    const { kind, requestId, code, text } = event;
    event.free();
    if (kind === 5) {
      postMessage({ type: "dialog", id: requestId, payload: engine.requestPayload(requestId) });
    } else if (kind === 2) {
      postMessage({ type: "confirm", id: requestId, code, text });
    } else if (kind === 3) {
      stopped = true;
      postMessage({ type: "closed" });
    } else if (kind === 4) {
      postMessage({ type: "title", text });
    } else {
      console.info(`ethornell: ${text}`);
    }
  }
}

function frame(now) {
  if (stopped) return;
  try {
    engine.step(now);
    drainEvents();
  } catch (error) {
    report(error);
    return;
  }
  nextFrame(frame);
}

function guarded(action) {
  if (!engine || stopped) return;
  try {
    action();
    drainEvents();
  } catch (error) {
    report(error);
  }
}

async function start(message) {
  await init();
  setAudioSampleRate(message.sampleRate || 48000);
  gameId = message.gameId;
  files.push(...message.files);
  const listing = message.files.map((file, index) => [message.paths[index], file.size]);
  const saves = await loadSaves(gameId);
  engine = new WebEngine(listing, saves, new Uint8Array(message.font), message.gbk, message.silent);
  canvas = message.canvas;
  canvas.width = message.width;
  canvas.height = message.height;
  const renderer = await createRenderer(canvas, message.width, message.height, engine.font());
  engine.attach(renderer);
  if (message.audioPort) {
    // The AudioWorklet asks for frames; render and transfer them.
    message.audioPort.onmessage = (event) => {
      const chunk = new Float32Array(event.data * 2);
      renderAudio(chunk);
      message.audioPort.postMessage(chunk, [chunk.buffer]);
    };
  }
  drainEvents();
  postMessage({ type: "started" });
  nextFrame(frame);
}

onmessage = (event) => {
  const message = event.data;
  switch (message.type) {
    case "start":
      start(message).catch(report);
      break;
    case "input":
      guarded(() => engine.input(message.kind, message.x, message.y, message.code, message.value));
      break;
    case "resize":
      guarded(() => {
        canvas.width = message.width;
        canvas.height = message.height;
        engine.resize(message.width, message.height);
      });
      break;
    case "pause":
      guarded(() => engine.pause(message.mask));
      break;
    case "answer":
      guarded(() => engine.completeRequest(message.id, message.result));
      break;
    case "dialog":
      guarded(() => engine.completeDialog(message.id, message.response));
      break;
    case "close":
      guarded(() => engine.requestClose());
      break;
  }
};
