# DOOM - macOS Native Port

**Status: In Progress**

## Description

Native macOS port using Cocoa (AppKit) for window management and Core Graphics
for software rendering.  Audio via Core Audio (AudioQueue).  Builds as a proper
`.app` bundle with Universal Binary support (Apple Silicon + Intel).

## Architecture

```
src/
  main.m              NSApplication entry point
  AppDelegate.h/m     App delegate (window creation, menu bar, engine launch)
  DoomView.h/m        NSView subclass (rendering, keyboard/mouse input)
  i_video_cocoa.m     Video subsystem (Core Graphics framebuffer blitting)
  i_sound_coreaudio.c Audio subsystem (AudioQueue output)
  i_system_macos.c    System interface (mach_absolute_time, bundle paths)
```

The port links against the shared `doom_engine` static library and provides
macOS-specific implementations of the `I_*` platform interface.  The engine
runs on a background thread while the main thread handles the Cocoa event loop.

## Target Hardware / OS

- macOS 12 (Monterey) or later
- Apple Silicon (M1/M2/M3/M4) and Intel x86_64
- Any GPU (software rendered via Core Graphics)

## Build Toolchain

- Xcode 14+ (with Command Line Tools)
- CMake 3.16+

## Build Instructions

```bash
# From this directory
cmake -B build
cmake --build build --config Release

# Run the .app bundle
open build/DOOM.app

# Or run directly
build/DOOM.app/Contents/MacOS/DOOM -iwad doom1.wad
```

## Features

- Universal Binary (arm64 + x86_64) by default
- Proper .app bundle with Info.plist
- Retina / HiDPI display support (pixel-perfect scaling with aspect ratio)
- Full keyboard input with macOS virtual key code translation
- Mouse look support via NSEvent deltaX/deltaY
- Game Controller framework integration (TODO: in progress)
- Native menu bar (About, Quit with Cmd+Q)
- Core Audio output at 11025 Hz (DOOM native sample rate)
- Engine runs on background thread for responsive UI

## Platform-Specific Notes

- Uses mach_absolute_time for sub-microsecond timing
- WAD files searched in: current dir, bundle Resources, ~/.doom/
- Nearest-neighbor interpolation for authentic pixel scaling
- The Cocoa event loop runs on the main thread per Apple requirements
- Code signing and notarization possible via standard Xcode workflow

## License

GNU General Public License v2.0 — see [LICENSE](../../LICENSE).
