cmake_minimum_required(VERSION 3.10)
project(DoomTerminal C)

set(CMAKE_C_STANDARD 99)

# Find ncurses
find_package(Curses REQUIRED)

# Add shared engine
add_subdirectory(../../engine engine_build)

# Terminal port sources
set(TERMINAL_SOURCES
    src/i_video_terminal.c
    src/main.c
)

add_executable(doom-terminal ${TERMINAL_SOURCES})

target_include_directories(doom-terminal PRIVATE
    ${CURSES_INCLUDE_DIR}
    ../../engine/include
)

target_link_libraries(doom-terminal
    doom_engine
    ${CURSES_LIBRARIES}
    m
)

target_compile_definitions(doom-terminal PRIVATE
    TERMINAL_RENDERER
    HAVE_CONFIG_H
)

install(TARGETS doom-terminal DESTINATION bin)
