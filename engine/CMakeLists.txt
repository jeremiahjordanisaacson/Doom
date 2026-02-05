cmake_minimum_required(VERSION 3.10)
project(DoomEngine C)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(GL_DOOM "Enable OpenGL rendering support" ON)

# ---------------------------------------------------------------------------
# C standard
# ---------------------------------------------------------------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Source files
#
# Platform-specific entry points (i_main.c, jni_doom.c) and the Android
# video backend (i_video_jni.c) are intentionally excluded.  The default
# i_video.c / i_sound.c / i_system.c / i_network.c implementations are
# kept so that ports only need to override what they replace.
# ---------------------------------------------------------------------------
set(DOOM_ENGINE_SOURCES
    src/am_map.c
    src/d_client.c
    src/d_deh.c
    src/d_items.c
    src/d_main.c
    src/d_server.c
    src/doomdef.c
    src/doomstat.c
    src/dstrings.c
    src/f_finale.c
    src/f_wipe.c
    src/g_game.c
    src/hu_lib.c
    src/hu_stuff.c
    src/i_network.c
    src/i_sound.c
    src/i_system.c
    src/i_video.c
    src/info.c
    src/lprintf.c
    src/m_argv.c
    src/m_bbox.c
    src/m_cheat.c
    src/m_menu.c
    src/m_misc.c
    src/m_random.c
    src/md5.c
    src/mmus2mid.c
    src/p_ceilng.c
    src/p_checksum.c
    src/p_doors.c
    src/p_enemy.c
    src/p_floor.c
    src/p_genlin.c
    src/p_inter.c
    src/p_lights.c
    src/p_map.c
    src/p_maputl.c
    src/p_mobj.c
    src/p_plats.c
    src/p_pspr.c
    src/p_saveg.c
    src/p_setup.c
    src/p_sight.c
    src/p_spec.c
    src/p_switch.c
    src/p_telept.c
    src/p_tick.c
    src/p_user.c
    src/r_bsp.c
    src/r_data.c
    src/r_demo.c
    src/r_draw.c
    src/r_filter.c
    src/r_fps.c
    src/r_main.c
    src/r_patch.c
    src/r_plane.c
    src/r_segs.c
    src/r_sky.c
    src/r_things.c
    src/s_sound.c
    src/sounds.c
    src/st_lib.c
    src/st_stuff.c
    src/tables.c
    src/v_video.c
    src/version.c
    src/w_memcache.c
    src/w_mmap.c
    src/w_wad.c
    src/wi_stuff.c
    src/z_bmalloc.c
    src/z_zone.c
)

# ---------------------------------------------------------------------------
# OpenGL sources (conditional)
# ---------------------------------------------------------------------------
if(GL_DOOM)
    list(APPEND DOOM_ENGINE_SOURCES
        src/gl_main.c
        src/gl_texture.c
    )
endif()

# ---------------------------------------------------------------------------
# Static library target
# ---------------------------------------------------------------------------
add_library(doom_engine STATIC ${DOOM_ENGINE_SOURCES})

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(doom_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------------------------------------------
# Compile definitions
# ---------------------------------------------------------------------------
target_compile_definitions(doom_engine PUBLIC
    HAVE_CONFIG_H
    PACKAGE_NAME="PrBoom"
    PACKAGE_VERSION="2.6.0"
)

if(GL_DOOM)
    target_compile_definitions(doom_engine PUBLIC GL_DOOM)
endif()

# ---------------------------------------------------------------------------
# Platform detection
# ---------------------------------------------------------------------------
if(WIN32)
    target_compile_definitions(doom_engine PUBLIC
        _WIN32
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
elseif(APPLE)
    target_compile_definitions(doom_engine PUBLIC
        MACOS
        UNIX
    )
elseif(UNIX)
    target_compile_definitions(doom_engine PUBLIC
        LINUX
        UNIX
    )
endif()

# ---------------------------------------------------------------------------
# SDL2 (optional -- platform ports may supply their own backend)
# ---------------------------------------------------------------------------
find_package(SDL2 QUIET)

if(SDL2_FOUND)
    target_include_directories(doom_engine PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(doom_engine PUBLIC ${SDL2_LIBRARIES})
    target_compile_definitions(doom_engine PUBLIC HAVE_SDL2)
    message(STATUS "SDL2 found -- linking against system SDL2")
else()
    message(STATUS "SDL2 not found -- ports must provide their own video/audio backend")
endif()

# ---------------------------------------------------------------------------
# OpenGL libraries (when GL_DOOM is enabled)
# ---------------------------------------------------------------------------
if(GL_DOOM)
    find_package(OpenGL QUIET)
    if(OpenGL_FOUND)
        target_link_libraries(doom_engine PUBLIC OpenGL::GL)
        message(STATUS "OpenGL found -- linking against system OpenGL")
    else()
        message(STATUS "OpenGL not found -- ports must provide their own GL context")
    endif()
endif()

# ---------------------------------------------------------------------------
# Math library on UNIX platforms
# ---------------------------------------------------------------------------
if(UNIX)
    target_link_libraries(doom_engine PUBLIC m)
endif()
