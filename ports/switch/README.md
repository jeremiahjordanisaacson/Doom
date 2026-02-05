# DOOM - Nintendo Switch Homebrew Port

**Status: Scaffold**

## Description

Nintendo Switch homebrew port of PrBoom using the devkitPro/libnx toolchain. Runs on Switch consoles with custom firmware (CFW) installed. Supports Joy-Con controls, handheld and docked modes.

## Target Hardware / OS

- Nintendo Switch (all hardware revisions)
- Atmosphere CFW (or compatible custom firmware)
- HOS (Horizon OS) via libnx homebrew layer

## Build Toolchain

- devkitPro (devkitA64 toolchain)
- libnx (Switch homebrew library)
- switch-sdl2, switch-sdl2_mixer, switch-sdl2_net (via devkitPro pacman)
- CMake 3.16+ or Make

## Build Instructions

```bash
# Install devkitPro (see https://devkitpro.org/wiki/Getting_Started)
# Install Switch portlibs
dkp-pacman -S switch-sdl2 switch-sdl2_mixer switch-sdl2_net switch-mesa

# Configure and build
source $DEVKITPRO/switchvars.sh
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake

cmake --build build

# Output: doom.nro (homebrew binary)
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- Output is an `.nro` file, launchable from the Homebrew Menu (hbmenu).
- Joy-Con controls are mapped to standard DOOM bindings (analog stick for movement, buttons for actions).
- Supports both handheld (720p) and docked (1080p) display modes.
- WAD files should be placed in `sdmc:/switch/doom/` on the SD card.
- Touchscreen input is available as an alternative control method in handheld mode.
- HD Rumble support is planned for haptic feedback.
- This port is for educational and preservation purposes only. A Nintendo Switch with CFW is required for testing.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
