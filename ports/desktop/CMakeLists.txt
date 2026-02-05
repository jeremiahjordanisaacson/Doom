cmake_minimum_required(VERSION 3.10)
project(DoomDesktop C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Find SDL2
# -----------------------------------------------------------------------------
find_package(SDL2 REQUIRED)

# -----------------------------------------------------------------------------
# Shared engine library (lives two levels up)
# -----------------------------------------------------------------------------
add_subdirectory(../../engine engine_build)

# -----------------------------------------------------------------------------
# Desktop executable
# -----------------------------------------------------------------------------
add_executable(doom src/main.c)

target_include_directories(doom PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ../../engine/include
)

target_link_libraries(doom PRIVATE
    doom_engine
    ${SDL2_LIBRARIES}
)

# -----------------------------------------------------------------------------
# Platform-specific settings
# -----------------------------------------------------------------------------
if(WIN32)
    # On Windows we need SDL2main for the WinMain -> main trampoline
    target_link_libraries(doom PRIVATE SDL2main)
elseif(APPLE)
    option(DOOM_BUILD_APP_BUNDLE "Build a macOS .app bundle" OFF)
    if(DOOM_BUILD_APP_BUNDLE)
        set_target_properties(doom PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "DOOM"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.doom.desktop"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        )
    endif()
elseif(UNIX)
    # Math library is needed on UNIX / Linux
    target_link_libraries(doom PRIVATE m)
endif()

# -----------------------------------------------------------------------------
# Output name
# -----------------------------------------------------------------------------
set_target_properties(doom PROPERTIES
    OUTPUT_NAME "doom"
)

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------
install(TARGETS doom
    RUNTIME DESTINATION bin
    BUNDLE  DESTINATION Applications
)
