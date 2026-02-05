# DOOM - Standalone WASM Module

**Status: Scaffold**

## Description

Standalone WebAssembly module compiled from the PrBoom C source without the Emscripten runtime. Uses minimal JavaScript glue code for browser integration, producing a lightweight, embeddable WASM binary that can be integrated into any web application as a reusable component.

## Target Hardware / OS

- Any environment with a WASM runtime:
  - Web browsers (Chrome, Firefox, Safari, Edge)
  - Node.js / Deno / Bun
  - Wasmtime, Wasmer, WAMR (standalone runtimes)
  - WASI-compatible environments
- No OS-specific dependencies

## Build Toolchain

- Clang 15+ with `wasm32` target (via LLVM)
  - OR wasi-sdk (WASI-compatible SDK)
- wasm-ld (LLVM WebAssembly linker)
- CMake 3.20+
- wasm-opt (from Binaryen, for size optimization)
- Optional: wasm-strip (for production builds)

## Build Instructions

```bash
# Install wasi-sdk
# See https://github.com/WebAssembly/wasi-sdk

# Configure
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/wasm-standalone-toolchain.cmake \
  -DWASM_STANDALONE=ON

# Build
cmake --build build

# Optimize (reduces binary size by 20-30%)
wasm-opt -O3 build/doom.wasm -o build/doom.opt.wasm

# Output:
#   doom.wasm      - WebAssembly binary
#   doom-glue.js   - Minimal JS glue code
#   doom-glue.d.ts - TypeScript type definitions
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The WASM module exports a clean C-style API: `doom_init()`, `doom_tick()`, `doom_render()`, `doom_input()`.
- No Emscripten runtime, no `emscripten.h` includes, no Emscripten filesystem layer. Pure WASM.
- The JavaScript glue code is under 200 lines and handles only: memory management, Canvas rendering, input forwarding, and audio output.
- Framebuffer is exposed as a shared `WebAssembly.Memory` buffer for zero-copy rendering to Canvas.
- TypeScript type definitions are provided for the glue API.
- The module can be loaded via standard `WebAssembly.instantiateStreaming()`.
- WAD data is passed into the module via the imported memory buffer.
- Binary size target is under 500KB (optimized, before WAD data).
- WASI compatibility allows running the module in server-side WASM runtimes for headless game simulation.
- Designed as a drop-in component: embed DOOM in any web page with under 10 lines of code.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
