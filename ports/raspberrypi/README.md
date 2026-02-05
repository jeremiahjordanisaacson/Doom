# DOOM - Raspberry Pi Port

**Status: Scaffold**

## Description

Optimized ARM build of PrBoom for Raspberry Pi with optional GPIO input support. Targets Raspberry Pi 3, 4, and 5 hardware with both SDL2 and direct framebuffer rendering modes for maximum flexibility.

## Target Hardware / OS

- Raspberry Pi 3 Model B/B+
- Raspberry Pi 4 Model B (all RAM variants)
- Raspberry Pi 5
- Raspberry Pi OS (Bookworm or later), Ubuntu, or DietPi
- Optional: Bare-metal with custom bootloader

## Build Toolchain

- GCC (ARM cross-compiler or native on Pi)
  - Cross-compile: `arm-linux-gnueabihf-gcc` or `aarch64-linux-gnu-gcc`
  - Native: standard `gcc` on Raspberry Pi OS
- CMake 3.16+
- SDL2 development libraries (for SDL2 mode)
- WiringPi or pigpio (for GPIO input support)

## Build Instructions

```bash
# --- Native build on Raspberry Pi ---
sudo apt install cmake libsdl2-dev libsdl2-mixer-dev libsdl2-net-dev

cmake -B build \
  -DRPI_OPTIMIZED=ON \
  -DGPIO_INPUT=OFF

cmake --build build -j$(nproc)

# --- Cross-compile from x86 host ---
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/rpi-toolchain.cmake \
  -DRPI_OPTIMIZED=ON

cmake --build build -j$(nproc)
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- ARM NEON SIMD instructions are used for software rendering optimizations on Pi 3/4.
- GPIO input mode allows wiring physical buttons and arcade controls directly to the Pi GPIO pins.
- On Pi 4/5, the KMS/DRM display driver is used for optimal framebuffer performance.
- Overclocking the Pi can improve frame rates; a heatsink or fan is recommended.
- The framebuffer mode (no X11) is ideal for RetroPie and similar setups.
- Pi 5 builds can target 64-bit (aarch64) for improved performance.
- Audio output via HDMI or 3.5mm jack is auto-detected.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
