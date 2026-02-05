# DOOM - Apple Watch Port (Proof of Concept)

**Status: Proof of Concept**

## Description

Apple Watch proof-of-concept port of PrBoom with a watchOS SwiftUI wrapper. Uses the Digital Crown for movement and tap gestures for shooting. Because why not.

## Target Hardware / OS

- Apple Watch Series 5 or later (S5 chip minimum)
- watchOS 9.0+
- Paired iPhone with iOS 16+ required for installation
- Apple Watch Ultra recommended for larger display

## Build Toolchain

- Xcode 15+
- watchOS 9+ SDK
- Swift 5.9+ (SwiftUI wrapper)
- C/Objective-C (DOOM engine core)
- Apple Developer account (required for device deployment)

## Build Instructions

```bash
# Open Xcode project (watchOS target)
open ports/watchos/DOOM-Watch.xcodeproj

# Build from command line
xcodebuild -project DOOM-Watch.xcodeproj \
  -scheme "DOOM WatchKit App" \
  -sdk watchos \
  -configuration Release \
  build

# Deploy via Xcode to paired Apple Watch
# (Simulator testing also available)
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The Apple Watch display is small (40-49mm); rendering resolution is scaled down accordingly.
- **Digital Crown**: Rotate to move forward/backward. Press to toggle strafe mode.
- **Tap gestures**: Tap to shoot. Swipe left/right to turn. Swipe up to open doors.
- **Force Touch / Long Press**: Weapon switch menu.
- Frame rate is limited by the watchOS display refresh rate and thermal constraints.
- The DOOM engine runs as a C library called from Swift via a bridging header.
- WAD data is bundled in the Watch app (embedded in the .ipa); only shareware WAD is practical due to storage constraints.
- Battery impact is significant; expect 30-45 minutes of gameplay per charge.
- Haptic feedback via the Taptic Engine is used for weapon fire, damage, and item pickup.
- Background audio is not available on watchOS; sound is played through the Watch speaker.
- This port exists primarily to answer the question "But can it run DOOM?" with "Yes, even on your wrist."

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
