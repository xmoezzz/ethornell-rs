// Plays the engine's audio. The worker renders interleaved stereo frames on
// request; this processor asks for more whenever its queue runs low, so the
// worker's clock never drifts against the audio device's.
const CHUNK = 2048; // frames per request
const LOW = 4096; // ask for more below this many queued frames

class EthornellOutput extends AudioWorkletProcessor {
  constructor() {
    super();
    this.chunks = [];
    this.offset = 0; // frames consumed from chunks[0]
    this.queued = 0;
    this.feed = null;
    this.waiting = false;
    this.port.onmessage = (event) => {
      this.feed = event.data.port;
      this.feed.onmessage = (message) => {
        const chunk = message.data;
        this.chunks.push(chunk);
        this.queued += chunk.length / 2;
        this.waiting = false;
      };
    };
  }

  process(_inputs, outputs) {
    const [left, right] = outputs[0];
    for (let i = 0; i < left.length; i += 1) {
      const chunk = this.chunks[0];
      if (!chunk) {
        left[i] = 0;
        if (right) right[i] = 0;
        continue;
      }
      left[i] = chunk[this.offset * 2];
      if (right) right[i] = chunk[this.offset * 2 + 1];
      this.offset += 1;
      this.queued -= 1;
      if (this.offset * 2 >= chunk.length) {
        this.chunks.shift();
        this.offset = 0;
      }
    }
    if (this.feed && !this.waiting && this.queued < LOW) {
      this.waiting = true;
      this.feed.postMessage(CHUNK);
    }
    return true;
  }
}

registerProcessor("ethornell-output", EthornellOutput);
