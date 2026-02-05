#!/bin/bash
# =============================================================================
# get-shareware-wad.sh - Download and verify the DOOM shareware WAD
# =============================================================================
# Downloads doom1.wad (Episode 1: Knee-Deep in the Dead) from the official
# Doomworld idgames archive and verifies its MD5 checksum.
#
# The shareware WAD is freely distributable per id Software's original release.
#
# Usage:
#   ./tools/get-shareware-wad.sh              # downloads to assets/doom1.wad
#   ./tools/get-shareware-wad.sh /some/path   # downloads to specified directory
# =============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DEFAULT_OUT="$PROJECT_ROOT/assets"

OUT_DIR="${1:-$DEFAULT_OUT}"
WAD_FILE="$OUT_DIR/doom1.wad"

# DOOM Shareware v1.9 - known good checksum
EXPECTED_MD5="f0cefca49926d00903cf57551d901abe"
EXPECTED_SIZE=4196020

# Download sources (tried in order)
URLS=(
    "https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad"
    "https://www.doomworld.com/3ddownloads/ports/shareware_doom_iwad.zip"
)

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

info()    { echo -e "${GREEN}[OK]${NC}    $*"; }
warn()    { echo -e "${YELLOW}[WARN]${NC}  $*"; }
error()   { echo -e "${RED}[ERROR]${NC} $*"; }

# Check if already downloaded
if [ -f "$WAD_FILE" ]; then
    echo "doom1.wad already exists at $WAD_FILE"
    ACTUAL_MD5=$(md5sum "$WAD_FILE" 2>/dev/null | awk '{print $1}' || md5 -q "$WAD_FILE" 2>/dev/null)
    if [ "$ACTUAL_MD5" = "$EXPECTED_MD5" ]; then
        info "Checksum verified: $ACTUAL_MD5"
        exit 0
    else
        warn "Checksum mismatch (got $ACTUAL_MD5), re-downloading..."
    fi
fi

mkdir -p "$OUT_DIR"

# Find a download tool
if command -v curl &>/dev/null; then
    DOWNLOAD="curl -L -o"
elif command -v wget &>/dev/null; then
    DOWNLOAD="wget -O"
else
    error "Neither curl nor wget found. Install one and retry."
    exit 1
fi

# Try each source
DOWNLOADED=0
for URL in "${URLS[@]}"; do
    echo "Trying: $URL"

    TMPFILE=$(mktemp)

    if $DOWNLOAD "$TMPFILE" "$URL" 2>/dev/null; then
        # Check if it's a zip file (some sources bundle the WAD in a zip)
        FILE_TYPE=$(file -b "$TMPFILE" 2>/dev/null || echo "unknown")
        if echo "$FILE_TYPE" | grep -qi "zip"; then
            echo "  Extracting from zip..."
            TMPDIR=$(mktemp -d)
            unzip -o -j "$TMPFILE" -d "$TMPDIR" 2>/dev/null
            # Find the WAD inside
            FOUND_WAD=$(find "$TMPDIR" -iname "doom1.wad" -type f | head -1)
            if [ -n "$FOUND_WAD" ]; then
                mv "$FOUND_WAD" "$WAD_FILE"
                DOWNLOADED=1
            fi
            rm -rf "$TMPDIR"
        else
            # Assume it's the raw WAD
            mv "$TMPFILE" "$WAD_FILE"
            DOWNLOADED=1
        fi
        rm -f "$TMPFILE"
    else
        rm -f "$TMPFILE"
        warn "  Failed, trying next source..."
    fi

    if [ "$DOWNLOADED" -eq 1 ]; then
        break
    fi
done

if [ "$DOWNLOADED" -eq 0 ]; then
    error "Could not download doom1.wad from any source."
    echo ""
    echo "You can manually download it from:"
    echo "  https://www.doomworld.com/files/file/7043-doom-v19-shareware/"
    echo "  https://archive.org/details/DOOMIWADFILE"
    echo ""
    echo "Then place doom1.wad in: $OUT_DIR/"
    exit 1
fi

# Verify checksum
echo "Verifying checksum..."
ACTUAL_MD5=$(md5sum "$WAD_FILE" 2>/dev/null | awk '{print $1}' || md5 -q "$WAD_FILE" 2>/dev/null)

if [ "$ACTUAL_MD5" = "$EXPECTED_MD5" ]; then
    info "doom1.wad downloaded and verified successfully!"
    info "  Path: $WAD_FILE"
    info "  MD5:  $ACTUAL_MD5"
    info "  Size: $(wc -c < "$WAD_FILE") bytes"
else
    warn "Checksum does not match expected v1.9 shareware WAD."
    warn "  Expected: $EXPECTED_MD5"
    warn "  Got:      $ACTUAL_MD5"
    warn "  The file may be a different version but could still work."
fi
