# DOOM - Terminal / ASCII Port

**Status: Scaffold**

## Description

Terminal-based port of PrBoom that renders DOOM as ASCII art using ncurses. Converts the internal framebuffer to ASCII characters and ANSI color codes in real time. Surprisingly playable at reasonable terminal sizes.

## Target Hardware / OS

- Any system with a terminal emulator and ncurses support
- Linux, macOS, WSL, BSD
- Recommended: 256-color or truecolor terminal (kitty, alacritty, iTerm2, Windows Terminal)
- Minimum terminal size: 80x24 (recommended: 160x48 or larger)

## Build Toolchain

- GCC or Clang (C99 support)
- CMake 3.16+
- ncurses development library (`libncurses-dev` or `ncurses-devel`)
- Optional: `libncursesw` for wide character support (block characters for higher resolution)

## Build Instructions

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt install cmake libncurses-dev libncursesw5-dev

# Install dependencies (macOS)
brew install cmake ncurses

# Configure
cmake -B build -DTERMINAL_RENDER=ON

# Build
cmake --build build

# Run
./build/doom-term -iwad doom.wad
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- ASCII density mapping converts pixel brightness to characters (e.g., ` .:-=+*#%@`).
- With truecolor terminal support, foreground/background ANSI colors are used for improved visuals.
- Unicode block characters (e.g., `\u2588`, `\u2584`) can be used for pseudo-pixel rendering when wide character support is available.
- Frame rate is limited by terminal rendering speed; a fast terminal emulator is recommended.
- Input is handled via ncurses `getch()` with raw mode enabled for responsive controls.
- The `TERM` environment variable must be set correctly for color support detection.
- Pipe output to a file to create recordings, or use `script` / `asciinema` for shareable demos.
- Sound output still uses the standard audio backend (ALSA, PulseAudio, etc.).

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
