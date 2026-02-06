# DOOM - Windows Native Port

**Status: In Progress**

## Description

Native Windows port using Win32 API for window management/input and GDI
(StretchDIBits) for software rendering.  DirectSound for audio.  This is an
alternative to the cross-platform SDL2 desktop port, providing deeper Windows
integration: proper DPI scaling, native message loop, XInput gamepad support,
and `.wad` file association.

## Architecture

```
src/
  main.c              WinMain entry point, window creation, message pump
  i_video_win32.c     Video subsystem (GDI StretchDIBits software renderer)
  i_sound_win32.c     Audio subsystem (DirectSound primary + secondary buffers)
  i_system_win32.c    System interface (QPC timing, file I/O, error handling)
  doom.rc             Windows resource script (version info, manifest)
```

The port links against the shared `doom_engine` static library from `engine/`
and provides Windows-specific implementations of the `I_*` platform interface
functions.

## Target Hardware / OS

- Windows 10 (1903+) and Windows 11
- x86_64 architecture
- Any GPU (software rendered)

## Build Toolchain

- Visual Studio 2019+ (with C Desktop Development workload)
- CMake 3.16+
- Windows SDK 10.0.18362+

## Build Instructions

```bash
# From this directory (Developer Command Prompt or PowerShell)
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# Run
build\Release\doom-win32.exe -iwad doom1.wad
```

## Platform-Specific Notes

- Uses Win32 API directly for window management and input handling
- GDI StretchDIBits for software framebuffer blitting (320x200 scaled to window)
- DirectSound for SFX with 8 mixing channels
- XInput gamepad support (TODO: implementation in progress)
- High DPI aware via SetProcessDPIAware and manifest
- MIDI music playback via Windows MIDI API (TODO: implementation in progress)
- Windows resource script embeds version info for the executable

## License

GNU General Public License v2.0 — see [LICENSE](../../LICENSE).
