# DOOM - Pure JavaScript Port (Educational)

**Status: Scaffold**

## Description

Hand-written, pure JavaScript port of the DOOM renderer. This is NOT an Emscripten compilation -- it is an idiomatic JavaScript reimplementation of the DOOM rendering pipeline, written for educational purposes to help developers understand how DOOM's BSP rendering, visplane system, and other subsystems work.

## Target Hardware / OS

- Any modern web browser (Chrome, Firefox, Safari, Edge)
- Node.js 18+ (headless / server-side rendering mode)
- No compilation step required
- No native dependencies

## Build Toolchain

- No build toolchain required (vanilla JavaScript)
- Optional: Node.js 18+ (for development server and testing)
- Optional: npm (for linting and development utilities)
- Optional: JSDoc (for documentation generation)

## Build Instructions

```bash
# No build step needed! Just serve the files.

# Option 1: Open directly in browser
open ports/javascript/index.html

# Option 2: Development server with live reload
cd ports/javascript
npx serve .

# Option 3: Run linting and tests
npm install
npm run lint
npm test
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- This is an educational reimplementation, not a C-to-JS transpilation. The code is written in idiomatic JavaScript to be readable and instructional.
- The WAD parser reads original DOOM WAD files directly using `ArrayBuffer` and `DataView`.
- Rendering uses the HTML5 Canvas 2D API with `ImageData` for pixel-level framebuffer access.
- The BSP tree traversal, visplane rendering, and seg clipping are faithfully reimplemented.
- Performance will not match compiled ports, but is sufficient for playable frame rates on modern hardware.
- Web Audio API is used for sound effects and music playback (including OPL2 emulation for MIDI).
- Gamepad API support allows standard game controllers in the browser.
- The code is extensively commented to explain the rendering algorithms.
- WAD files must be provided by the user (drag-and-drop or file picker).
- No WebGL is used; this is pure software rendering to Canvas for educational clarity.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
