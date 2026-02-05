# DOOM - Linux Framebuffer Port

**Status: Scaffold**

## Description

Linux framebuffer port of PrBoom with direct framebuffer rendering. No X11 or Wayland dependency required, making it ideal for embedded systems, kiosks, and minimal Linux installations where a full display server is unnecessary or undesirable.

## Target Hardware / OS

- Any Linux system with framebuffer support (`/dev/fb0`)
- Embedded Linux boards (BeagleBone, industrial SBCs)
- Kiosk and digital signage systems
- Minimal server installs with a connected display

## Build Toolchain

- GCC or Clang (C99 support)
- CMake 3.16+
- Linux kernel headers (for framebuffer and evdev interfaces)
- No SDL2 dependency (direct framebuffer access)

## Build Instructions

```bash
# Configure
cmake -B build \
  -DFRAMEBUFFER_RENDER=ON \
  -DEVDEV_INPUT=ON

# Build
cmake --build build

# Run (requires framebuffer access, typically root or video group)
sudo ./build/doom-fb -iwad doom.wad
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- Renders directly to `/dev/fb0` using `mmap` for zero-copy pixel output.
- Input is handled via evdev (`/dev/input/event*`), supporting keyboards and game controllers without X11.
- The user must be in the `video` group (or run as root) to access the framebuffer device.
- Resolution is determined by the current framebuffer mode; use `fbset` to configure before launching.
- No hardware acceleration is used; rendering is pure software.
- Console switching (Ctrl+Alt+Fn) is handled gracefully.
- Sound output uses ALSA directly (no PulseAudio dependency).

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
