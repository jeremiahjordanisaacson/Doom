# DOOM - DOS Port

**Status: Scaffold**

## Description

DOS port of PrBoom cross-compiled with DJGPP. Runs on real DOS hardware or in DOSBox/DOSBox-X emulators. Uses VGA Mode 13h (320x200, 256 colors) for authentic retro rendering, staying true to the original DOOM experience.

## Target Hardware / OS

- MS-DOS 6.22 or compatible
- DOSBox / DOSBox-X / DOSBox Staging
- FreeDOS
- Minimum: 386DX-33, 4MB RAM, VGA card
- Recommended: 486DX2-66 or Pentium, 8MB RAM, Sound Blaster compatible

## Build Toolchain

- DJGPP (DJ Delorie's GNU Programming Platform for DOS)
  - GCC cross-compiler targeting DPMI
- CMake 3.16+ or Make (host system)
- CWSDPMI (DOS Protected Mode Interface extender)
- Optional: Watcom C/C++ as alternative compiler

## Build Instructions

```bash
# Install DJGPP cross-compiler
# See https://github.com/andrewwutw/build-djgpp

# Configure
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/djgpp-toolchain.cmake \
  -DDOS_TARGET=ON

# Build
cmake --build build

# Output: DOOM.EXE (DOS DPMI executable)

# Run in DOSBox
dosbox -c "mount c build" -c "c:" -c "doom.exe"
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- Uses VGA Mode 13h (320x200, 256 colors) with page flipping where supported.
- Sound Blaster and AdLib FM synthesis are supported via direct hardware I/O.
- CWSDPMI.EXE must be present alongside the executable on real DOS systems.
- Keyboard input uses INT 9h handler; mouse via INT 33h.
- Memory management uses DPMI for access beyond the 640KB conventional memory limit.
- Real-mode DOS is required; Windows 9x DOS boxes may have compatibility issues.
- The executable can be distributed on floppy disk (1.44MB) with a WAD on a second disk.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
