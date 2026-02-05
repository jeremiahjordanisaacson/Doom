# DOOM - macOS Native Port

**Status: Scaffold**

## Description

Native macOS port of PrBoom using Cocoa and Metal/OpenGL rendering backends. Wraps the SDL2-based build in a proper `.app` bundle for a native macOS experience with drag-to-install distribution.

## Target Hardware / OS

- macOS 12 (Monterey) or later
- Apple Silicon (M1/M2/M3) and Intel x86_64
- Metal-capable GPU (preferred) or OpenGL 3.3+ fallback

## Build Toolchain

- Xcode 14+ (with Command Line Tools)
- CMake 3.20+
- SDL2 (via Homebrew or bundled framework)
- Optional: MoltenVK for Vulkan compatibility layer

## Build Instructions

```bash
# Install dependencies
brew install cmake sdl2 sdl2_mixer sdl2_net

# Configure
cmake -B build -G Xcode \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DMACOS_BUNDLE=ON

# Build
cmake --build build --config Release

# Package .app bundle
cmake --build build --target package
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The `.app` bundle embeds SDL2 frameworks for zero-dependency distribution.
- Universal Binary (arm64 + x86_64) is the default build configuration.
- Metal renderer is preferred on supported hardware; falls back to OpenGL automatically.
- Code signing and notarization scripts are provided for distribution outside the App Store.
- Retina / HiDPI display support is enabled by default.
- Game controller support via the macOS Game Controller framework.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
