cmake_minimum_required(VERSION 3.16)
project(DoomMacOS C OBJC)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Shared engine library
# -----------------------------------------------------------------------------
add_subdirectory(../../engine engine_build)

# -----------------------------------------------------------------------------
# Find SDL2 (optional — used as fallback if Metal/OpenGL setup fails)
# -----------------------------------------------------------------------------
find_package(SDL2 QUIET)

# macOS frameworks
find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)
find_library(METAL_FRAMEWORK Metal)
find_library(IOKIT_FRAMEWORK IOKit)
find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
find_library(OPENGL_FRAMEWORK OpenGL)
find_library(GAMECONTROLLER_FRAMEWORK GameController)

# -----------------------------------------------------------------------------
# macOS native executable (.app bundle)
# -----------------------------------------------------------------------------
add_executable(doom MACOSX_BUNDLE
    src/main.m
    src/AppDelegate.m
    src/DoomView.m
    src/i_video_cocoa.m
    src/i_sound_coreaudio.c
    src/i_system_macos.c
)

target_include_directories(doom PRIVATE
    ../../engine/include
    src/
)

target_compile_definitions(doom PRIVATE
    MACOS
    MACOS_NATIVE
    UNIX
    HAVE_CONFIG_H
)

target_link_libraries(doom PRIVATE
    doom_engine
    ${COCOA_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
    ${IOKIT_FRAMEWORK}
    ${AUDIOTOOLBOX_FRAMEWORK}
    m
)

if(METAL_FRAMEWORK)
    target_link_libraries(doom PRIVATE ${METAL_FRAMEWORK})
    target_compile_definitions(doom PRIVATE USE_METAL)
endif()

if(OPENGL_FRAMEWORK)
    target_link_libraries(doom PRIVATE ${OPENGL_FRAMEWORK})
endif()

if(GAMECONTROLLER_FRAMEWORK)
    target_link_libraries(doom PRIVATE ${GAMECONTROLLER_FRAMEWORK})
    target_compile_definitions(doom PRIVATE USE_GAMECONTROLLER)
endif()

# App bundle configuration
set_target_properties(doom PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "DOOM"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.doom.macos-native"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_INFO_STRING "DOOM - GPL v2"
    MACOSX_BUNDLE_COPYRIGHT "GPL v2 - id Software / PrBoom"
    OUTPUT_NAME "DOOM"
)

# Universal Binary (Apple Silicon + Intel)
option(DOOM_UNIVERSAL "Build Universal Binary (arm64 + x86_64)" ON)
if(DOOM_UNIVERSAL)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "")
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum macOS version")

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------
install(TARGETS doom
    BUNDLE DESTINATION Applications
)
