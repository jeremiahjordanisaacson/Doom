# DOOM - NES Port (Proof of Concept)

**Status: Proof of Concept**

## Description

NES (Nintendo Entertainment System) proof-of-concept port. The NES has severely limited hardware (2KB RAM, 8-bit 6502 CPU at 1.79 MHz), so this is not a full PrBoom port. Instead, it implements a simplified raycaster inspired by DOOM's visual style. Intended for educational and demonstration purposes.

## Target Hardware / OS

- Nintendo Entertainment System (NES/Famicom)
- NES emulators (FCEUX, Mesen, Nestopia)
- 2KB internal RAM, 8KB VRAM
- Custom mapper for extended ROM/RAM (MMC3 or similar)

## Build Toolchain

- cc65 (6502 C compiler and assembler suite)
  - `cc65` (compiler), `ca65` (assembler), `ld65` (linker)
- CMake 3.16+ or Make
- Python 3 (for asset conversion scripts)
- NES emulator for testing (FCEUX recommended)

## Build Instructions

```bash
# Install cc65
# See https://cc65.github.io/

# Configure
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/nes-cc65-toolchain.cmake

# Build
cmake --build build

# Output: doom.nes (iNES ROM image)

# Test in emulator
fceux build/doom.nes
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- This is NOT a full DOOM port. The NES hardware cannot run the DOOM engine. This is a simplified raycaster that captures the spirit of DOOM within NES constraints.
- Rendering uses a raycasting technique similar to Wolfenstein 3D, adapted for the NES PPU tile-based display.
- Levels are simplified, hand-crafted maps -- not parsed from WAD files.
- Sprites are pre-rendered at limited angles and stored as CHR ROM data.
- The 2KB RAM limit requires extremely careful memory management; most data lives in ROM.
- A custom mapper (e.g., MMC3) provides bank switching for additional ROM space.
- Frame rate target is 30fps (half the NES's 60fps refresh) due to rendering complexity.
- Music uses the NES APU (2 pulse waves, triangle, noise, DPCM).

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
