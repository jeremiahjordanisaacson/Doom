cmake_minimum_required(VERSION 3.16)
project(DoomWindows C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Shared engine library (lives two levels up)
# -----------------------------------------------------------------------------
add_subdirectory(../../engine engine_build)

# -----------------------------------------------------------------------------
# Windows native executable
# -----------------------------------------------------------------------------
add_executable(doom WIN32
    src/main.c
    src/i_video_win32.c
    src/i_sound_win32.c
    src/i_system_win32.c
    src/doom.rc
)

target_include_directories(doom PRIVATE
    ../../engine/include
)

target_compile_definitions(doom PRIVATE
    _WIN32
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    WINDOWS_NATIVE
    HAVE_CONFIG_H
    _CRT_SECURE_NO_WARNINGS
)

target_link_libraries(doom PRIVATE
    doom_engine
    user32
    gdi32
    winmm
    dsound
    dxguid
    xinput
    ole32
)

# High DPI manifest and icon
if(MSVC)
    set_target_properties(doom PROPERTIES
        VS_DPI_AWARE "PerMonitor"
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /MANIFEST:EMBED"
    )
endif()

# -----------------------------------------------------------------------------
# Output name
# -----------------------------------------------------------------------------
set_target_properties(doom PROPERTIES
    OUTPUT_NAME "doom-win32"
)

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------
install(TARGETS doom RUNTIME DESTINATION bin)
