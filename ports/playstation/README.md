# DOOM - PlayStation Portable (PSP) Homebrew Port

**Status: Scaffold**

## Description

PlayStation Portable homebrew port of PrBoom using the PSPSDK toolchain. Targets PSP-1000, PSP-2000, and PSP-3000 hardware with optimized rendering for the PSP's 480x272 display.

## Target Hardware / OS

- PSP-1000 (original / "Fat")
- PSP-2000 (Slim)
- PSP-3000 (Brite)
- Custom firmware (PRO-C, ME, or ARK) required
- PPSSPP emulator for development testing

## Build Toolchain

- PSPSDK (PSP Software Development Kit)
- psp-gcc (MIPS cross-compiler)
- CMake 3.16+ or Make
- psp-fixup-imports, mksfo, pack-pbp (PSPSDK utilities)

## Build Instructions

```bash
# Install PSPSDK (see https://github.com/pspdev/pspsdk)
export PSPSDK=$(psp-config --pspsdk-path)

# Configure and build
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$PSPSDK/cmake/psp-toolchain.cmake

cmake --build build

# Output: EBOOT.PBP (PSP executable)
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The PSP has a 333 MHz MIPS R4000 CPU and 32MB RAM (64MB on PSP-2000/3000); performance tuning is essential.
- Display output is 480x272 pixels; the renderer is configured for this native resolution.
- Analog nub is used for movement; face buttons for actions; triggers for strafe/weapon switch.
- WAD files are loaded from `ms0:/PSP/GAME/DOOM/` on the Memory Stick.
- The EBOOT.PBP format includes the game icon, background, and metadata for the XMB.
- Sound uses the PSP's Media Engine for MP3/ADPCM decoding.
- Can be tested on PPSSPP emulator during development, but real hardware testing is recommended.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
