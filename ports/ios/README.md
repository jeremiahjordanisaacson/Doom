# DOOM - iOS Port

**Status: Scaffold**

## Description

iOS port of PrBoom using SDL2 with a UIKit wrapper and Xcode project. Features touch controls with a virtual D-pad overlay, optimized for iPhone and iPad displays with full support for modern iOS features.

## Target Hardware / OS

- iOS 15.0+ / iPadOS 15.0+
- iPhone 8 and later
- iPad (6th generation) and later
- Apple Silicon and A-series processors

## Build Toolchain

- Xcode 15+
- iOS 15+ SDK
- CMake 3.20+ (for library builds)
- CocoaPods or Swift Package Manager (for dependencies)
- Apple Developer account (required for device testing and distribution)

## Build Instructions

```bash
# Install dependencies
# SDL2 iOS framework must be built or obtained separately

# Open Xcode project
open ports/ios/DOOM.xcodeproj

# Or build from command line
xcodebuild -project DOOM.xcodeproj \
  -scheme DOOM \
  -sdk iphoneos \
  -configuration Release \
  build

# For simulator testing (no developer account needed)
xcodebuild -project DOOM.xcodeproj \
  -scheme DOOM \
  -sdk iphonesimulator \
  -configuration Debug \
  build
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- Touch controls feature a virtual D-pad (left side) and action buttons (right side).
- MFi and Bluetooth game controllers are supported via the Game Controller framework.
- WAD files are loaded from the app's Documents directory, accessible via the Files app.
- Supports both portrait and landscape orientations; landscape is recommended for gameplay.
- Metal rendering is used where available for optimal performance.
- Background audio is paused and resumed correctly on app lifecycle events.
- Haptic feedback (Taptic Engine) is used for weapon fire and damage.
- App Store distribution requires additional review compliance work.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
