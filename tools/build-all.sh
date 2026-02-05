#!/bin/bash
# =============================================================================
# build-all.sh - Master Build Script for DOOM Multiplatform Mega-Repo
# =============================================================================
# Builds DOOM for every supported platform from a single entry point.
#
# Usage:
#   ./tools/build-all.sh              # build everything (same as --all)
#   ./tools/build-all.sh --desktop    # native desktop build only
#   ./tools/build-all.sh --web        # Emscripten/WASM build only
#   ./tools/build-all.sh --android    # Android NDK build only
#   ./tools/build-all.sh --clean      # remove all build directories
#   ./tools/build-all.sh --desktop --web  # combine targets
# =============================================================================

set -e

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_ROOT="$PROJECT_ROOT/build"
JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"

# ---------------------------------------------------------------------------
# Color output helpers
# ---------------------------------------------------------------------------
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

info()    { echo -e "${BLUE}[INFO]${NC}  $*"; }
success() { echo -e "${GREEN}[OK]${NC}    $*"; }
warn()    { echo -e "${YELLOW}[WARN]${NC}  $*"; }
error()   { echo -e "${RED}[ERROR]${NC} $*"; }
section() { echo -e "\n${CYAN}========== $* ==========${NC}"; }

# ---------------------------------------------------------------------------
# Dependency checks
# ---------------------------------------------------------------------------
check_tool() {
    if ! command -v "$1" &>/dev/null; then
        error "Required tool not found: $1"
        return 1
    fi
}

check_desktop_deps() {
    check_tool cmake
    check_tool gcc || check_tool cc
}

check_web_deps() {
    check_tool cmake
    check_tool emcc
    check_tool emcmake
}

check_android_deps() {
    check_tool cmake
    if [ -z "$ANDROID_NDK_HOME" ] && [ -z "$ANDROID_NDK" ]; then
        error "ANDROID_NDK_HOME or ANDROID_NDK must be set"
        return 1
    fi
}

# ---------------------------------------------------------------------------
# Build functions
# ---------------------------------------------------------------------------
build_desktop() {
    section "Desktop Build (native)"
    check_desktop_deps

    local build_dir="$BUILD_ROOT/desktop"
    mkdir -p "$build_dir"

    info "Configuring desktop build..."
    cmake -B "$build_dir" \
        -DCMAKE_BUILD_TYPE=Release \
        "$PROJECT_ROOT/ports/desktop"

    info "Compiling with $JOBS parallel jobs..."
    cmake --build "$build_dir" --parallel "$JOBS"

    success "Desktop build complete: $build_dir"
}

build_web() {
    section "Web/WASM Build (Emscripten)"
    check_web_deps

    local build_dir="$BUILD_ROOT/web"
    mkdir -p "$build_dir"

    info "Configuring Emscripten build..."
    emcmake cmake -B "$build_dir" \
        -DCMAKE_BUILD_TYPE=Release \
        "$PROJECT_ROOT/ports/web"

    info "Compiling with $JOBS parallel jobs..."
    cmake --build "$build_dir" --parallel "$JOBS"

    success "Web build complete: $build_dir"
    info "Serve with: python3 -m http.server 8080 --directory $build_dir"
}

build_android() {
    section "Android Build (NDK)"
    check_android_deps

    local ndk="${ANDROID_NDK_HOME:-$ANDROID_NDK}"
    local toolchain="$ndk/build/cmake/android.toolchain.cmake"
    local build_dir="$BUILD_ROOT/android"
    mkdir -p "$build_dir"

    info "Configuring Android build..."
    cmake -B "$build_dir" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE="$toolchain" \
        -DANDROID_ABI=arm64-v8a \
        -DANDROID_PLATFORM=android-21 \
        "$PROJECT_ROOT/ports/android"

    info "Compiling with $JOBS parallel jobs..."
    cmake --build "$build_dir" --parallel "$JOBS"

    success "Android build complete: $build_dir"
}

do_clean() {
    section "Clean"
    if [ -d "$BUILD_ROOT" ]; then
        info "Removing $BUILD_ROOT ..."
        rm -rf "$BUILD_ROOT"
        success "Clean complete."
    else
        info "Nothing to clean."
    fi
}

# ---------------------------------------------------------------------------
# Argument parsing
# ---------------------------------------------------------------------------
DO_DESKTOP=0
DO_WEB=0
DO_ANDROID=0
DO_CLEAN=0
DO_ALL=0

if [ $# -eq 0 ]; then
    DO_ALL=1
fi

while [ $# -gt 0 ]; do
    case "$1" in
        --desktop)  DO_DESKTOP=1 ;;
        --web)      DO_WEB=1 ;;
        --android)  DO_ANDROID=1 ;;
        --all)      DO_ALL=1 ;;
        --clean)    DO_CLEAN=1 ;;
        -h|--help)
            echo "Usage: $0 [--desktop] [--web] [--android] [--all] [--clean]"
            echo ""
            echo "  --desktop   Build native desktop port"
            echo "  --web       Build Emscripten/WASM port"
            echo "  --android   Build Android NDK port"
            echo "  --all       Build all platforms (default)"
            echo "  --clean     Remove all build directories"
            exit 0
            ;;
        *)
            error "Unknown argument: $1"
            exit 1
            ;;
    esac
    shift
done

# ---------------------------------------------------------------------------
# Execute
# ---------------------------------------------------------------------------
section "DOOM Multiplatform Mega-Repo - Build System"
info "Project root: $PROJECT_ROOT"
info "Build root:   $BUILD_ROOT"
info "Parallel jobs: $JOBS"

if [ "$DO_CLEAN" -eq 1 ]; then
    do_clean
fi

if [ "$DO_ALL" -eq 1 ]; then
    build_desktop
    build_web
    build_android
else
    [ "$DO_DESKTOP" -eq 1 ] && build_desktop
    [ "$DO_WEB"     -eq 1 ] && build_web
    [ "$DO_ANDROID" -eq 1 ] && build_android
fi

section "Done"
success "All requested builds finished successfully."
