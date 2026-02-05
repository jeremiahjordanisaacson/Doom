/* ==========================================================================
 * DOOM Desktop Port — SDL2 Frontend
 * ==========================================================================
 *
 * This file is the main entry point for the cross-platform desktop build of
 * DOOM.  It initialises SDL2 (video, audio, joystick), hands control over to
 * the shared engine via D_DoomMain(), and performs cleanup on exit.
 *
 * The actual game logic, rendering, and networking all live in the shared
 * engine library (doom_engine).  This file is intentionally thin: its only
 * job is to set up the host platform and call into the engine.
 *
 * Supported platforms: Linux, Windows, macOS
 * Build system:        CMake 3.10+
 * Dependencies:        SDL2, doom_engine (shared library)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

/* Engine headers */
#include "d_main.h"
#include "i_main.h"
#include "i_system.h"
#include "i_video.h"
#include "m_argv.h"
#include "lprintf.h"

/* -----------------------------------------------------------------------
 * main
 *
 * SDL_main-compatible signature.  On Windows, SDL2main rewrites the real
 * WinMain entry point so that this ordinary main() is called instead.
 * ----------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    /* Store command-line arguments so the engine can inspect them later
     * through myargc / myargv (m_argv). */
    myargc = argc;
    myargv = argv;

    /* -----------------------------------------------------------------
     * Initialise SDL2 subsystems
     * ----------------------------------------------------------------- */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* -----------------------------------------------------------------
     * Hand off to the engine.  D_DoomMain() never returns under normal
     * operation — it contains the main game loop.  If it *does* return
     * we treat that as a clean shutdown.
     * ----------------------------------------------------------------- */
    D_DoomMain();

    /* -----------------------------------------------------------------
     * Cleanup — reached only if the engine exits gracefully.
     * ----------------------------------------------------------------- */
    SDL_Quit();

    return EXIT_SUCCESS;
}
