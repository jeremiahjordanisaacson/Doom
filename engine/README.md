# DOOM Shared Engine (PrBoom)

The shared PrBoom-based DOOM engine, built as a static library that platform ports link against.

## Architecture

The engine provides the complete DOOM game logic, rendering, and data handling. Platform-specific functionality is abstracted through interface headers that each port implements:

| Interface | Purpose | Header |
|-----------|---------|--------|
| `i_video.h` | Video output (framebuffer, palette) | `include/i_video.h` |
| `i_sound.h` | Audio output (SFX, music) | `include/i_sound.h` |
| `i_system.h` | System calls (timing, memory, exit) | `include/i_system.h` |
| `i_network.h` | Networking (multiplayer) | `include/i_network.h` |
| `i_joy.h` | Joystick/gamepad input | `include/i_joy.h` |

The engine entry point is `D_DoomMain()` in `d_main.c`.

## Source Layout

```
engine/
├── src/           # C source files (~88 files)
│   ├── d_*.c      # Game startup, client/server, DeHackEd
│   ├── g_game.c   # Core game logic
│   ├── r_*.c      # Software renderer
│   ├── gl_*.c     # OpenGL renderer (optional)
│   ├── p_*.c      # Game physics, map, objects, AI
│   ├── s_*.c      # Sound management
│   ├── hu_*.c     # HUD
│   ├── st_*.c     # Status bar
│   ├── am_map.c   # Automap
│   ├── f_*.c      # Finale screens, screen wipes
│   ├── m_*.c      # Menus, misc utilities, math
│   ├── w_*.c      # WAD file loading
│   ├── v_video.c  # Video/palette management
│   ├── z_*.c      # Zone memory allocator
│   └── ...        # Info tables, sounds, etc.
├── include/       # Header files (~82 files)
├── SDL/           # SDL platform layer (reference implementation)
├── POSIX/         # POSIX platform layer
└── CMakeLists.txt # Build configuration
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

This produces `libdoom_engine.a` (static library).

### Options

| CMake Option | Default | Description |
|-------------|---------|-------------|
| `DOOM_GL_SUPPORT` | OFF | Enable OpenGL rendering |
| `DOOM_SDL2` | OFF | Find and link SDL2 |

## Linking

Platform ports add the engine as a CMake subdirectory:

```cmake
add_subdirectory(../../engine engine_build)
target_link_libraries(my_port doom_engine)
```

## Origin

Based on PrBoom 2.5.0, originally ported to Android by Jeremiah Isaacson.
Extended for multi-platform support.

## License

GNU General Public License v2 (GPL-2.0)
