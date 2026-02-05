# DOOM - Commodore 64 Port (Proof of Concept)

**Status: Proof of Concept**

## Description

Commodore 64 proof-of-concept port using the cc65 toolchain. The C64's 64KB RAM and 1 MHz 6510 CPU make a full DOOM port impossible, so this implements a heavily simplified renderer. More of a tech demo than a playable game, demonstrating what can be achieved on 1982-era 8-bit hardware.

## Target Hardware / OS

- Commodore 64 (PAL or NTSC)
- Commodore 128 (in C64 mode)
- C64 emulators: VICE (x64sc), CCS64, Hoxs64
- 64KB RAM, 1 MHz MOS 6510 CPU
- VIC-II graphics chip (320x200 or 160x200 multicolor)

## Build Toolchain

- cc65 (6502/6510 C compiler and assembler suite)
  - `cc65` (compiler), `ca65` (assembler), `ld65` (linker)
- CMake 3.16+ or Make
- Python 3 (for asset conversion and sprite preparation)
- VICE emulator (for testing)
- Optional: 1541 Ultimate or SD2IEC for real hardware transfer

## Build Instructions

```bash
# Install cc65
# See https://cc65.github.io/

# Configure
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/c64-cc65-toolchain.cmake

# Build
cmake --build build

# Output: doom.prg (C64 program file)
# Also: doom.d64 (1541 disk image)

# Test in VICE
x64sc build/doom.d64
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The 1 MHz 6510 CPU is approximately 1000x slower than what DOOM was designed for; extreme simplification is required.
- Rendering uses the VIC-II multicolor mode (160x200, 4 colors per 8x8 cell) for pseudo-3D visuals.
- The raycaster is heavily simplified: low draw distance, reduced resolution, minimal texturing.
- Performance-critical rendering loops are written in 6510 assembly.
- Level data is stored in a compact custom format; original WAD files are not used.
- Sprites use hardware sprite multiplexing (VIC-II supports 8 hardware sprites).
- SID chip provides sound effects and music (3-channel synthesis).
- Loading from 1541 floppy is slow; fastloader routines or cartridge loading is recommended.
- REU (RAM Expansion Unit) support could enable additional data buffering if available.
- This port exists as an exercise in extreme optimization and retro computing nostalgia.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
