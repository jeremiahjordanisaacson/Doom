# DOOM for Android

**Version 9.0 (2026 Edition)**

A full Android port of the legendary DOOM game, originally created by id Software in 1993. This project brings the classic first-person shooter experience to modern Android devices.

---

## A Tribute to the Johns

This project exists because of two programming legends whose work changed gaming forever:

### John Carmack

The technical wizard whose revolutionary rendering techniques made DOOM possible. His innovations in BSP trees, binary space partitioning, and optimized rendering pipelines didn't just create a game - they laid the foundation for the entire first-person shooter genre. Carmack's relentless pursuit of technical excellence and his willingness to share knowledge through open-sourcing the DOOM engine has inspired countless programmers worldwide.

### John Romero

The design mastermind whose level design and creative vision gave DOOM its soul. Romero's masterful crafting of E1M1 through the depths of Hell created an experience that still resonates today. His understanding of pacing, atmosphere, and player psychology made DOOM not just technically impressive, but genuinely fun and terrifying to play.

**To both Johns: Thank you.** Your collaboration produced something that transcended gaming and became a cultural phenomenon. Your decision to release the source code gave generations of programmers the opportunity to learn, experiment, and create. This Android port stands as a small tribute to your massive contribution to computing and entertainment.

*"In the first age, in the first battle, when the shadows first lengthened, one stood."*

---

## Features

- Full DOOM gameplay on Android devices
- Support for original WAD files (DOOM, DOOM II, Plutonia, TNT)
- Touch screen controls with on-screen buttons
- Keyboard/D-Pad support for external controllers
- Background music and sound effects
- Portrait and landscape orientation support

## System Requirements

- **Minimum Android Version:** 7.0 (Nougat, API 24)
- **Target Android Version:** 14 (API 34)
- **Supported Architectures:** ARM, ARM64, x86, x86_64
- **Storage:** ~50MB for app + WAD files

## Compatibility

This 2026 update ensures compatibility with **all modern Android devices**:

- Android 7.0 through Android 14+
- 32-bit and 64-bit devices
- Phones and tablets
- Chromebooks with Android support
- Android TV devices

### Key Updates for 2026

- **Gradle Build System:** Migrated from legacy Eclipse/Ant to modern Gradle
- **Scoped Storage:** Full Android 10+ scoped storage compliance
- **FileProvider:** Secure file sharing for Android 7.0+
- **Runtime Permissions:** Proper permission handling for all Android versions
- **64-bit Support:** Native library support for ARM64 and x86_64
- **Modern APIs:** Replaced all deprecated Android APIs

## Installation

### From Source

1. Clone this repository:
   ```bash
   git clone https://github.com/jeremiahjordanisaacson/Java---Android---Doom.git
   ```

2. Open in Android Studio (2023.1 or later recommended)

3. Build and run on your device or emulator

### WAD Files

You'll need a DOOM WAD file to play. Options include:

- **DOOM Shareware (doom1.wad):** Free, available from many sources
- **Full DOOM (doom.wad):** Requires original game purchase
- **DOOM II (doom2.wad):** Requires original game purchase
- **Final DOOM (plutonia.wad, tnt.wad):** Requires original game purchase

Place WAD files in the app's data directory or use the in-app downloader for shareware.

## Controls

### Touch Screen Mode
- **D-Pad:** Movement (up/down/left/right)
- **Fire Button:** Shoot
- **Open/Use:** Interact with doors/switches
- **Map:** Toggle automap
- **Strafe:** Side-step left/right

### Keyboard Mode
- **1AQW** or **D-Pad:** Movement
- **Shift:** Run
- **Alt:** Strafe
- **Space:** Use/Open
- **Tab:** Map
- **Enter:** Menu select
- **Escape/Back:** Menu

## Project Structure

```
Java---Android---Doom/
├── app/
│   ├── src/main/
│   │   ├── java/
│   │   │   ├── com/doom/android/    # Main application
│   │   │   └── doom/                # Game modules
│   │   │       ├── audio/           # Sound management
│   │   │       ├── jni/             # Native bindings
│   │   │       └── util/            # Utilities
│   │   ├── res/                     # Android resources
│   │   ├── jniLibs/                 # Native libraries
│   │   └── AndroidManifest.xml
│   └── build.gradle.kts
├── native/prboom/                   # PrBoom C source
├── build.gradle.kts
├── settings.gradle.kts
└── README.md
```

## Technical Details

This port is based on **PrBoom**, a highly portable and accurate DOOM engine implementation. The Android integration uses JNI (Java Native Interface) to bridge between Java/Android and the C game engine.

### Architecture
- **Java Layer:** Android UI, input handling, audio playback
- **JNI Bridge:** Event passing between Java and native code
- **Native Layer:** PrBoom game engine (C)

## Credits

### Original DOOM (1993)
- **id Software**
- John Carmack (Lead Programmer)
- John Romero (Lead Designer)
- Tom Hall, Sandy Petersen (Design)
- Adrian Carmack, Kevin Cloud (Art)
- Bobby Prince (Music/Sound)

### PrBoom Engine
- The PrBoom Team

### Android Port
- **Original Port (2010):** Savant - Jeremiah Isaacson & Justin Turney
- **2026 Modernization:** Updated for all Android devices

## License

This program is free software: you can redistribute it and/or modify it under the terms of the **GNU General Public License** as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

## Links

- [Original DOOM Source Code](https://github.com/id-Software/DOOM)
- [PrBoom](http://prboom.sourceforge.net/)
- [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html)

---

*"RIP AND TEAR, UNTIL IT IS DONE."*

**Happy Demon Slaying!**
