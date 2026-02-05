cmake_minimum_required(VERSION 3.10)
project(DoomWeb C)

# ---------------------------------------------------------------------------
# Shared engine library (platform-independent DOOM engine)
# ---------------------------------------------------------------------------
add_subdirectory(../../engine engine_build)

# ---------------------------------------------------------------------------
# Web/WASM executable
# ---------------------------------------------------------------------------
add_executable(doom src/main.c)

target_link_libraries(doom PRIVATE doom_engine)

# ---------------------------------------------------------------------------
# Emscripten compile flags
# ---------------------------------------------------------------------------
target_compile_options(doom PRIVATE
    "SHELL:-s USE_SDL=2"
)

# ---------------------------------------------------------------------------
# Emscripten link flags
# ---------------------------------------------------------------------------
# Notes:
#   TOTAL_MEMORY   = 67108864 (64 MB) -- enough for the shareware WAD
#   ALLOW_MEMORY_GROWTH keeps things safe if a mod pushes past 64 MB
#   --preload-file bundles doom1.wad into doom.data for virtual FS access
#   --shell-file uses our custom HTML template instead of the default
# ---------------------------------------------------------------------------
set(EMSCRIPTEN_LINK_FLAGS
    "-s USE_SDL=2"
    "-s WASM=1"
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s TOTAL_MEMORY=67108864"
    "-s EXPORTED_FUNCTIONS=[\"_main\"]"
    "-s EXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\"]"
    "--preload-file ${CMAKE_SOURCE_DIR}/../../assets/doom1.wad@/doom1.wad"
    "--shell-file ${CMAKE_SOURCE_DIR}/shell.html"
)

# Join the list into a single space-separated string for the linker
string(JOIN " " EMSCRIPTEN_LINK_FLAGS_STR ${EMSCRIPTEN_LINK_FLAGS})

set_target_properties(doom PROPERTIES
    SUFFIX ".html"
    LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS_STR}"
)

# ---------------------------------------------------------------------------
# Output artifacts:
#   doom.html  -- main page (from shell.html template)
#   doom.js    -- Emscripten glue code
#   doom.wasm  -- compiled WebAssembly binary
#   doom.data  -- preloaded virtual filesystem (contains doom1.wad)
# ---------------------------------------------------------------------------
