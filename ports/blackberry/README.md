# DOOM - BlackBerry 10 / QNX Port

**Status: Scaffold**

## Description

BlackBerry 10 and QNX port of PrBoom using the BB10 NDK. Targets BlackBerry 10 devices (Z10, Z30, Q10, Passport) and QNX-based embedded systems with touchscreen and keyboard input support.

## Target Hardware / OS

- BlackBerry Z10, Z30, Q10, Q5, Passport, Classic
- BlackBerry 10 OS (10.2+)
- QNX Neutrino RTOS 6.6+ (for embedded targets)
- QNX SDP (Software Development Platform)

## Build Toolchain

- BlackBerry 10 Native SDK (BB10 NDK)
- QNX Momentics IDE (or command-line tools)
- qcc (QNX C compiler, GCC-based)
- CMake 3.16+ or Make
- BlackBerry code signing keys (for device deployment)

## Build Instructions

```bash
# Source BB10 NDK environment
source /opt/bbndk/bbndk-env_10_3_1.sh

# Configure
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/bb10-toolchain.cmake \
  -DQNX_TARGET=ON

# Build
cmake --build build

# Package BAR file
blackberry-nativepackager -package doom.bar bar-descriptor.xml -devMode

# Deploy to device
blackberry-deploy -installApp -device 169.254.0.1 -password <pw> doom.bar
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- BlackBerry 10 uses a QNX microkernel; standard POSIX APIs are well supported.
- Screen rendering uses the BB10 Screen API (EGL + OpenGL ES 2.0).
- On keyboard-equipped devices (Q10, Passport), physical keys are mapped to DOOM controls.
- Touchscreen devices use a virtual D-pad overlay similar to the iOS port.
- The BlackBerry 10 platform reached end of life in January 2022; this port is for historical and educational purposes.
- QNX RTOS builds can target automotive and industrial embedded systems.
- Audio uses the QNX Audio Manager (libasound).

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
