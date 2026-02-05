# Legal Notices

## License Overview

This project is licensed under the **GNU General Public License v2.0** (GPL-2.0).
See [LICENSE](LICENSE) for details.

The DOOM engine source code was originally released by id Software on
December 23, 1997 under the GNU General Public License. PrBoom, the engine
this project is based on, carries the same GPL v2 license.

All platform-specific port code in this repository is also released under
GPL v2 unless otherwise noted.

---

## WAD File Policy

### What are WAD files?

WAD files contain DOOM's game data: levels, textures, sprites, sounds, and
music. They are **separate from the source code** and have their own
distribution rules.

### Shareware WAD (doom1.wad)

The DOOM shareware WAD (`doom1.wad`) contains Episode 1: "Knee-Deep in the
Dead" and is **freely distributable**. id Software has permitted free
distribution of the shareware WAD since DOOM's original release. This is the
WAD included in the `assets/` directory and used by the web player.

### Commercial WADs - NOT INCLUDED

The following WAD files are **commercial products** and are **NOT included**
in this repository:

| WAD File | Game | Owner |
|----------|------|-------|
| `doom.wad` | DOOM (Full Version) | id Software / Microsoft |
| `doom2.wad` | DOOM II: Hell on Earth | id Software / Microsoft |
| `plutonia.wad` | Final DOOM: The Plutonia Experiment | id Software / Microsoft |
| `tnt.wad` | Final DOOM: TNT: Evilution | id Software / Microsoft |

**Do not submit pull requests containing commercial WAD data.**

### Where to Purchase

You can legally obtain the commercial WADs from:

- **Steam**: https://store.steampowered.com/app/2280/DOOM_1993/
- **GOG**: https://www.gog.com/game/doom_1993
- **Physical copies**: Original floppy disks or CD-ROMs

### Free Alternatives

- **Freedoom** (https://freedoom.github.io/): A complete set of free IWAD
  files compatible with the DOOM engine, released under a BSD-style license.

---

## Attribution

### id Software

DOOM was created by id Software in 1993. The engine source code was released
under the GPL in 1997.

Key contributors:
- **John Carmack** - Lead Programmer
- **John Romero** - Lead Designer
- **Tom Hall** - Creative Director (early development)
- **Sandy Petersen** - Level Design
- **Adrian Carmack** - Art Director
- **Kevin Cloud** - Artist
- **Bobby Prince** - Music and Sound Effects

### PrBoom

PrBoom is a portable, enhanced DOOM source port that serves as the foundation
of this project. PrBoom is licensed under GPL v2.

- Website: http://prboom.sourceforge.net/
- Contributors: Colin Phipps, Florian Schulze, and many others

### Original Android Port

The original Android port was created by:
- **Jeremiah Isaacson** (Savant) - 2010
- **Justin Turney** - Co-developer

### Third-Party Libraries

This project uses the following third-party libraries:

| Library | License | Usage |
|---------|---------|-------|
| SDL2 | zlib License | Desktop video, audio, input |
| Emscripten | MIT License | Web/WASM compilation |
| ncurses | MIT-style | Terminal renderer |
| devkitPro | Various | Nintendo Switch homebrew toolchain |
| PSPSDK | BSD | PlayStation Portable homebrew toolchain |
| DJGPP | GPL v2 | DOS cross-compilation |
| cc65 | zlib License | 6502 cross-compilation (NES, C64) |
| z88dk | Clarified Artistic License | Z80 cross-compilation (TI-83) |
| ESP-IDF | Apache 2.0 | ESP32 development framework |

---

## Trademarks

DOOM, id Software, and related logos are trademarks of id Software LLC, a
ZeniMax Media company, now part of Microsoft. This project is an independent
open-source effort and is **not affiliated with, endorsed by, or sponsored by**
id Software, ZeniMax Media, Bethesda Softworks, or Microsoft.

Nintendo Switch is a trademark of Nintendo. PlayStation is a trademark of Sony.
All other trademarks are the property of their respective owners.

---

## DMCA / Takedown Notices

If you believe any content in this repository infringes on your copyright,
please open an issue on the GitHub repository with:

1. Identification of the copyrighted work
2. Identification of the infringing material and its location
3. Your contact information
4. A statement of good faith belief that the use is not authorized
5. A statement under penalty of perjury that the information is accurate

We will respond promptly to valid takedown requests.

---

## Contributing

By contributing code to this repository, you agree that your contributions
will be licensed under the GNU General Public License v2. You must have the
legal right to submit the code under this license.

**Do not submit:**
- Commercial WAD files or proprietary game data
- Proprietary SDK headers or tools
- Code you do not have the right to distribute under GPL v2
