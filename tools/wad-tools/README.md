# WAD Tools

Utilities for working with DOOM WAD files.

## Overview

The WAD (Where's All the Data?) file format is the archive format used by DOOM
and related id Software games to store game assets -- levels, textures, sprites,
sounds, and more. These tools let you inspect, extract, and validate WAD files
from the command line.

## Tools

| Tool           | Status  | Description                              |
|----------------|---------|------------------------------------------|
| `wad-info`     | Working | Display WAD header and lump directory    |
| `wad-extract`  | Planned | Extract individual lumps to files        |
| `wad-validate` | Planned | Check WAD structural integrity           |

### wad-info

Reads a WAD file and prints its type (IWAD or PWAD), number of lumps, directory
offset, and a full listing of every lump with its name and size.

```
Usage: ./wad-info <file.wad>

Example:
  $ ./wad-info doom1.wad
  WAD Type:          IWAD
  Number of lumps:   1264
  Directory offset:  4175796

  Lump Directory:
  #       Name            Size
  ----------------------------------------
  0       PLAYPAL         10752
  1       COLORMAP        8704
  2       ENDOOM          4000
  ...
```

### wad-extract (planned)

Will extract one or more lumps from a WAD file to individual files on disk.

### wad-validate (planned)

Will verify that a WAD file is structurally valid: correct magic bytes, sane
directory offsets, no overlapping lumps, etc.

## Building

From the repository root:

```bash
cd tools/wad-tools
cmake -B build .
cmake --build build
```

Or compile directly:

```bash
gcc -O2 -o wad-info wad-info.c
```

## License

This project is licensed under the GNU General Public License v2.0.
See the repository root LICENSE file for details.
