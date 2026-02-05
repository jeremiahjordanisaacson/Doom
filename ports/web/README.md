# DOOM Web Port (Emscripten/WASM)

Play DOOM in the browser. This port compiles the shared DOOM engine to
WebAssembly using [Emscripten](https://emscripten.org) and renders via
SDL 2 on an HTML5 canvas.

## Requirements

| Tool            | Version |
|-----------------|---------|
| Emscripten SDK  | 3.x+    |
| CMake           | 3.10+   |

Install the Emscripten SDK by following the official guide:
<https://emscripten.org/docs/getting_started/downloads.html>

Make sure `emcmake` and `emmake` are on your PATH (run `source ./emsdk_env.sh`
or the equivalent for your shell).

## Building

```bash
# From the repository root
cd ports/web

# Configure with Emscripten's CMake wrapper
emcmake cmake -B build -S .

# Build
emmake make -C build -j$(nproc)
```

The build produces four artifacts in `build/`:

| File        | Description                                      |
|-------------|--------------------------------------------------|
| `doom.html` | Main page (generated from `shell.html` template) |
| `doom.js`   | Emscripten glue / loader code                    |
| `doom.wasm` | Compiled WebAssembly binary                      |
| `doom.data` | Preloaded virtual filesystem (contains WAD)      |

## Running Locally

Browsers require files to be served over HTTP (not `file://`). The
simplest approach is Python's built-in server:

```bash
cd build
python -m http.server 8080
```

Then open <http://localhost:8080/doom.html> in your browser.

Any static file server will work (e.g., `npx serve`, nginx, Apache).

## Browser Compatibility

| Browser          | Status                       |
|------------------|------------------------------|
| Chrome / Edge    | Fully supported              |
| Firefox          | Fully supported              |
| Safari (16.4+)   | Supported (WebAssembly OK)  |
| Mobile browsers  | Playable but no touch input yet |

WebAssembly and SharedArrayBuffer support are required. Make sure
your server sends the appropriate COOP/COEP headers if you need
threading support in the future:

```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

## Controls

| Key         | Action         |
|-------------|----------------|
| Arrow Keys  | Move / Turn    |
| Ctrl        | Fire           |
| Space       | Open / Use     |
| Shift       | Run            |
| Alt         | Strafe         |
| 1-7         | Select Weapon  |
| Tab         | Automap        |
| Esc         | Menu           |

## License

This port is distributed under the **GNU General Public License v2.0**.
See [LICENSE](../../LICENSE) for the full text.
