# DOOM Multiplatform Mega-Repo

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](LICENSE)
[![Platforms](https://img.shields.io/badge/platforms-20%2B-orange)]()

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
| **Play in Browser** | [Launch Web/WASM build](https://example.com/doom-wasm) |
| **Download** | [Releases page](https://github.com/example/doom-mega-repo/releases) |
| **Documentation** | [docs/](docs/) |

---

## Platform Support

| Port | Directory | Status |
|---|---|---|
| Desktop (SDL2) | `ports/sdl2/` | **Working** |
| Web / WASM | `ports/wasm/` | **Working** |
| Android | `ports/android/` | **Working** |
| macOS | `ports/macos/` | In Progress |
| Windows | `ports/windows/` | In Progress |
| Linux Framebuffer | `ports/linux-fb/` | In Progress |
| iOS | `ports/ios/` | In Progress |
| Raspberry Pi | `ports/rpi/` | In Progress |
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
| Commodore 64 | `ports/c64/` | Proof of Concept |
| Pure JavaScript | `ports/purejs/` | Proof of Concept |

---

## Directory Structure

```
Doom/
  README.md              This file
  LICENSE                 GNU General Public License v2
  LEGAL.md               Detailed legal notices and attribution
  CMakeLists.txt         Top-level CMake build
  Makefile               Convenience wrapper around CMake
  core/                  Shared DOOM engine source (PrBoom-based)
    src/                 C source files
    include/             Public headers
  ports/                 Platform-specific port implementations
    sdl2/               Desktop SDL2 (Linux, macOS, Windows)
    wasm/               Emscripten / WebAssembly build
    android/            Android NDK + Java wrapper
    macos/              Native macOS (AppKit / Metal)
    windows/            Native Win32 / DirectX
    linux-fb/           Raw Linux framebuffer
    ios/                iOS (UIKit / Metal)
    rpi/                Raspberry Pi (bare-metal & Linux)
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
    c64/                Commodore 64 (6502)
    purejs/             Pure JavaScript (no WASM)
  data/                  Shareware WAD and demo lumps
  docs/                  Documentation and guides
  tools/                 Build helpers, WAD utilities, scripts
  tests/                 Automated test suite
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
cmake -B build -DPORT=sdl2

# Build
cmake --build build -j$(nproc)

# Run with the included shareware WAD
./build/doom -iwad data/doom1.wad
```

### Build (using the Makefile wrapper)

```bash
# Desktop SDL2 (default)
make

# Web / WASM (requires Emscripten SDK)
make PORT=wasm

# Android (requires Android NDK)
make PORT=android

# Any other port
make PORT=<port-name>
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
- The **shareware WAD** (`doom1.wad`) is freely distributable and is included
  in this repository under `data/`.
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
