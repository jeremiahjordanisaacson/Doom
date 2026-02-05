# DOOM Desktop Port (SDL2)

Cross-platform desktop port of DOOM using SDL2 for window management, input,
audio, and joystick support.  The port is a thin frontend that delegates all
game logic to the shared **doom_engine** library.

## Supported Platforms

- Linux (x86_64, aarch64)
- Windows (x86_64)
- macOS (x86_64, Apple Silicon)

## Build Requirements

| Requirement | Minimum Version |
|-------------|-----------------|
| CMake       | 3.10+           |
| SDL2        | 2.0.x           |
| C compiler  | C99-capable     |

## Installing SDL2 Development Libraries

**Debian / Ubuntu**

```
sudo apt install libsdl2-dev
```

**macOS (Homebrew)**

```
brew install sdl2
```

**Windows (Chocolatey)**

```
choco install sdl2
```

On Windows you can also download the SDL2 development libraries directly from
https://github.com/libsdl-org/SDL/releases and point CMake at them with
`-DSDL2_DIR=<path>`.

## Building

```
mkdir build && cd build
cmake ..
make
```

On Windows with Visual Studio you can substitute the last step:

```
cmake --build . --config Release
```

To build a macOS `.app` bundle, pass the bundle option:

```
cmake .. -DDOOM_BUILD_APP_BUNDLE=ON
make
```

## Usage

```
./doom -iwad /path/to/doom1.wad
```

You need a valid DOOM WAD file (e.g. `doom1.wad`, `doom2.wad`).  The shareware
`doom1.wad` is freely available from id Software.

### Common Options

| Flag                  | Description                        |
|-----------------------|------------------------------------|
| `-iwad <file>`        | Path to the game IWAD              |
| `-file <file> ...`    | Load one or more PWAD patches      |
| `-warp <e> <m>`       | Warp to episode e, map m           |
| `-skill <n>`          | Set difficulty (1-5)               |
| `-fullscreen`         | Start in fullscreen mode           |
| `-window`             | Start in windowed mode             |
| `-width <n>`          | Set horizontal resolution          |
| `-height <n>`         | Set vertical resolution            |
| `-nosound`            | Disable all sound                  |
| `-nomusic`            | Disable music                      |
| `-nosfx`              | Disable sound effects              |
| `-config <file>`      | Use an alternative config file     |

## Controls

| Action            | Key                |
|-------------------|--------------------|
| Move forward      | W / Up arrow       |
| Move backward     | S / Down arrow     |
| Strafe left       | A                  |
| Strafe right      | D                  |
| Turn left         | Left arrow         |
| Turn right        | Right arrow        |
| Fire              | Ctrl / Left mouse  |
| Use / Open        | Space / E          |
| Sprint            | Shift              |
| Strafe modifier   | Alt                |
| Weapon 1-7        | 1-7                |
| Toggle automap    | Tab                |
| Pause             | Pause              |
| Menu              | Escape             |

Joystick and gamepad input is also supported when an SDL2-compatible controller
is connected.

## License

This project is licensed under the **GNU General Public License v2.0**.
See the [LICENSE](../../LICENSE) file in the repository root for full terms.
