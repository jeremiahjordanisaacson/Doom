# DOOM - TI-83/84 Calculator Port (Proof of Concept)

**Status: Proof of Concept**

## Description

TI-83/84 graphing calculator proof-of-concept port. Features a simplified raycaster on the calculator's monochrome 96x64 pixel display. Extremely limited hardware, but it is technically DOOM (or at least, DOOM-inspired) running on a calculator.

## Target Hardware / OS

- TI-83 Plus / TI-83 Plus Silver Edition
- TI-84 Plus / TI-84 Plus Silver Edition
- TI-84 Plus CE (color display variant, 320x240)
- TI-OS 1.15+ (TI-83+) or TI-OS 5.0+ (TI-84+ CE)
- Emulators: TilEm, CEmu, jsTIfied

## Build Toolchain

- z88dk (Z80 C compiler) - for TI-83+/84+
  - OR SDCC (Small Device C Compiler)
- fasmg or spasm-ng (Z80 assembler for performance-critical sections)
- convbin / binpac8x (TI binary packaging tools)
- Python 3 (for asset conversion)
- TI Connect CE or TILP (for calculator transfer)

## Build Instructions

```bash
# Install z88dk
# See https://github.com/z88dk/z88dk

# Build for TI-83+ (monochrome, 96x64)
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/ti83-z88dk-toolchain.cmake \
  -DTI_MODEL=TI83P

cmake --build build

# Output: DOOM.8xp (TI-83+ program file)

# Transfer to calculator via TI Connect or TILP
tilp build/DOOM.8xp
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The TI-83+ has a 6 MHz Z80 CPU and 32KB RAM (24KB usable); this is an extreme constraints port.
- The monochrome 96x64 display uses 1-bit graphics; dithering simulates grayscale.
- The TI-84+ CE variant has a 48 MHz eZ80 CPU and 320x240 color display, allowing significantly better visuals.
- Raycasting is heavily simplified with reduced draw distance and resolution.
- Calculator keypad is mapped: arrow keys for movement, 2nd for shoot, Alpha for use/open.
- The program runs as a TI-BASIC shell app or native ASM program.
- Levels are compact, stored as simple grid maps in ROM.
- No sound output (TI-83/84 has no speaker; TI-84+ CE has a basic piezo).
- This is primarily an educational exercise in extreme optimization.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
