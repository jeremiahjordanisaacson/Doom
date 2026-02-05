# DOOM - Windows Native Port

**Status: Scaffold**

## Description

Native Windows port of PrBoom using Win32 API and DirectX rendering. Built with MSVC for optimal Windows integration, producing a standalone `.exe` with an optional installer for clean distribution.

## Target Hardware / OS

- Windows 10 (1903+) and Windows 11
- x86_64 architecture
- DirectX 11 capable GPU (DirectX 12 optional)

## Build Toolchain

- Visual Studio 2019+ (with C++ Desktop Development workload)
- CMake 3.20+
- DirectX SDK (included in Windows SDK 10.0.18362+)
- Optional: NSIS or WiX for installer creation

## Build Instructions

```bash
# Configure with CMake (from Developer Command Prompt)
cmake -B build -G "Visual Studio 17 2022" -A x64 \
  -DWINDOWS_NATIVE=ON

# Build
cmake --build build --config Release

# Create installer (optional)
cmake --build build --target package
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- Uses Win32 API directly for window management and input handling.
- DirectX 11 is the primary rendering backend; DirectX 12 support is planned.
- XInput and DirectInput are supported for game controllers.
- The installer registers file associations for `.wad` files.
- High DPI awareness is declared in the application manifest.
- Windows Defender / SmartScreen may flag unsigned builds; code signing is recommended for distribution.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
