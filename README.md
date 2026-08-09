# DOOM Multiplatform Mega-Repo

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](LICENSE)
[![Platforms](https://img.shields.io/badge/platforms-21-orange)]()

```
______   ___   ___  __  __
|  _ \ / _ \ / _ \|  \/  |
| | | | | | | | | | |\/| |
| |_| | |_| | |_| | |  | |
|____/ \___/ \___/|_|  |_|

M U L T I P L A T F O R M   M E G A - R E P O
```

A collection of DOOM ports for **every platform imaginable**, based on the
GPL-licensed [PrBoom](http://prboom.sourceforge.net/) engine. One repository,
one codebase, dozens of targets -- from desktop PCs to graphing calculators.

---

## Quick Links

| | Link |
|---|---|
| **Play in Browser** | [Launch JS renderer](https://jeremiahjordanisaacson.github.io/Doom/play.html) |
| **Website** | [Project homepage](https://jeremiahjordanisaacson.github.io/Doom/) |
| **Documentation** | [docs/](docs/) |

---

## Platform Support

| Port | Directory | Status |
|---|---|---|
| Desktop (SDL2) | `ports/desktop/` | **Working** |
| Web / WASM | `ports/web/` | **Working** |
| Android | `ports/android/` | **Working** |
| macOS | `ports/macos-native/` | In Progress |
| Windows | `ports/windows-native/` | In Progress |
| Linux Framebuffer | `ports/linux-framebuffer/` | In Progress |
| iOS | `ports/ios/` | In Progress |
| Raspberry Pi | `ports/raspberrypi/` | In Progress |
| Terminal (Curses) | `ports/terminal/` | In Progress |
| Nintendo Switch | `ports/switch/` | Scaffold |
| PlayStation | `ports/playstation/` | Scaffold |
| DOS | `ports/dos/` | Scaffold |
| BlackBerry | `ports/blackberry/` | Scaffold |
| Xbox | `ports/xbox/` | Scaffold |
| NES | `ports/nes/` | Proof of Concept |
| TI-83 Calculator | `ports/ti83/` | Proof of Concept |
| ESP32 | `ports/esp32/` | Proof of Concept |
| watchOS | `ports/watchos/` | Proof of Concept |
| Commodore 64 | `ports/commodore64/` | Proof of Concept |
| WASM Standalone | `ports/wasm-standalone/` | Proof of Concept |
| Pure JavaScript | `ports/javascript/` | Proof of Concept |

---

## Directory Structure

```
Doom/
  README.md              This file
  LICENSE                 GNU General Public License v2
  LEGAL.md               Detailed legal notices and attribution
  engine/                Shared DOOM engine source (PrBoom-based)
    src/                 C source files (79 files)
    include/             Public headers (82 files)
    CMakeLists.txt       Engine static library build
  ports/                 Platform-specific port implementations
    desktop/            Desktop SDL2 (Linux, macOS, Windows)
    web/                Emscripten / WebAssembly build
    wasm-standalone/    Standalone WASM build
    android/            Android NDK + Java wrapper
    macos-native/       Native macOS (AppKit / Metal)
    windows-native/     Native Win32 / DirectX
    linux-framebuffer/  Raw Linux framebuffer
    ios/                iOS (UIKit / Metal)
    raspberrypi/        Raspberry Pi (bare-metal & Linux)
    terminal/           Terminal / ncurses renderer
    switch/             Nintendo Switch (libnx)
    playstation/        PlayStation homebrew
    dos/                DOS (DJGPP / Watcom)
    blackberry/         BlackBerry QNX
    xbox/               Xbox homebrew
    nes/                NES (cc65)
    ti83/               TI-83 calculator (z80)
    esp32/              ESP32 microcontroller
    watchos/            Apple watchOS
    commodore64/        Commodore 64 (6502)
    javascript/         Pure JavaScript BSP renderer (educational)
  docs/                  GitHub Pages website
  tools/                 Build helpers, WAD utilities, scripts
  docker/                Docker build environments
```

---

## Quick Start

### Prerequisites

- CMake 3.16 or later
- A C99-compatible compiler (GCC, Clang, MSVC)
- SDL2 development libraries (for the desktop port)

### Build (Desktop SDL2)

```bash
# Configure
cmake -B build -S ports/desktop

# Build
cmake --build build -j$(nproc)

# Run (you'll need a WAD file -- see below)
./build/doom -iwad doom1.wad
```

### Get the Shareware WAD

```bash
# Download DOOM1.WAD (shareware, freely distributable)
./tools/get-shareware-wad.sh
```

### Build (other ports)

Each port directory contains its own `README.md` with platform-specific
instructions, toolchain requirements, and flashing/deployment steps.

---

## Legal

This project is licensed under the **GNU General Public License v2** -- see
[LICENSE](LICENSE) for the full text. Additional legal notices, attribution,
and WAD distribution policies are documented in [LEGAL.md](LEGAL.md).

**Key points:**

- The DOOM **engine source code** is free software under GPL v2.
- The **shareware WAD** (`doom1.wad`) is freely distributable. Use
  `tools/get-shareware-wad.sh` to download it.
- **Commercial WAD files** (`doom.wad`, `doom2.wad`, `plutonia.wad`,
  `tnt.wad`) are copyrighted by id Software / ZeniMax Media and are **NOT**
  included. You must purchase them legally to use them with this engine.

---

## Credits

- **id Software** -- for creating DOOM and releasing the engine source code
  under the GPL. Special thanks to John Carmack and John Romero.
- **PrBoom Team** -- for the portable, clean PrBoom engine that serves as the
  foundation of this project.
- **Jeremiah Isaacson** -- original Android port and maintainer of this
  multiplatform mega-repo.
- All contributors who have submitted patches, ports, and bug fixes.

---

## See also

- [doom-flow](https://github.com/godofecht/doom-flow): DOOM in the [Flow](https://github.com/flooooooooooow/flow) language, compiled to WebAssembly.

---

## License

```
Copyright (C) 1993-1996  id Software, Inc.
Copyright (C) 1999-2006  PrBoom contributors
Copyright (C) 2024-2026  Jeremiah Isaacson and contributors

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
```

See [LICENSE](LICENSE) for the full license text.
