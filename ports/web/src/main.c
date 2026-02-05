/* =========================================================================
 * DOOM Web Port -- Emscripten / WASM entry point
 * =========================================================================
 * Browsers cannot run a traditional blocking game loop.  Emscripten
 * provides emscripten_set_main_loop() which hands control of the loop
 * back to the browser so it can repaint, handle events, etc.
 *
 * Flow:
 *   1. SDL is initialised with an Emscripten-compatible configuration.
 *   2. D_DoomMain() performs the usual engine start-up (WAD loading,
 *      subsystem init, demo playback / title screen).
 *   3. The main loop callback is registered and runs once per
 *      requestAnimationFrame (~60 fps by default).
 * ======================================================================= */

#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>

/* Engine public API */
#include "d_main.h"
#include "doomdef.h"
#include "i_system.h"

/* -----------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------- */
static void main_loop_callback(void);
static void configure_canvas(void);

/* -----------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------- */
#define DOOM_SCREEN_WIDTH   640
#define DOOM_SCREEN_HEIGHT  400

/* -----------------------------------------------------------------------
 * Globals visible to the engine's platform layer when it needs them
 * --------------------------------------------------------------------- */
SDL_Window   *sdl_window   = NULL;
SDL_Renderer *sdl_renderer = NULL;

/* -----------------------------------------------------------------------
 * main -- entry point called by Emscripten after the runtime is ready
 * --------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    /* -- SDL init ------------------------------------------------------ */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    sdl_window = SDL_CreateWindow(
        "DOOM",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DOOM_SCREEN_WIDTH,
        DOOM_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!sdl_window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    sdl_renderer = SDL_CreateRenderer(
        sdl_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!sdl_renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    /* -- Canvas / browser helpers -------------------------------------- */
    configure_canvas();

    /* -- Engine start-up ----------------------------------------------- */
    /* D_DoomMain reads the WAD from the preloaded virtual filesystem,
     * sets up all subsystems (video, sound, input, network stubs) and
     * prepares the first game tic.                                       */
    D_DoomMain();

    /* -- Hand control to the browser ----------------------------------- */
    /* 0  = use requestAnimationFrame (match display refresh)
     * 1  = simulate_infinite_loop -- prevents main() from returning so
     *      the C stack is preserved between callbacks.                   */
    emscripten_set_main_loop(main_loop_callback, 0, 1);

    /* This line is never reached while the loop is active. */
    return 0;
}

/* -----------------------------------------------------------------------
 * main_loop_callback -- called once per animation frame by the browser
 *
 * This replaces the traditional while(1) loop.  The engine's D_RunFrame
 * (or equivalent per-tic function) advances the game by one frame:
 *   - process input events
 *   - run game logic / tic
 *   - render the frame
 * --------------------------------------------------------------------- */
static void main_loop_callback(void)
{
    /* Process any pending SDL / browser events */
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                emscripten_cancel_main_loop();
                return;

            default:
                /* Forward the event to the engine's input handler */
                I_HandleEvent(&event);
                break;
        }
    }

    /* Advance the engine by one frame (game tic + render) */
    D_RunFrame();
}

/* -----------------------------------------------------------------------
 * configure_canvas -- set up the HTML5 canvas for DOOM's rendering
 *
 * Adjusts the Emscripten canvas element so it fills the container while
 * keeping DOOM's native aspect ratio.
 * --------------------------------------------------------------------- */
static void configure_canvas(void)
{
    /* Tell Emscripten which <canvas> element to target */
    emscripten_set_canvas_element_size("#canvas",
                                       DOOM_SCREEN_WIDTH,
                                       DOOM_SCREEN_HEIGHT);

    /* Lock pointer on click for mouse-look (optional, but nice) */
    emscripten_request_pointerlock("#canvas", EM_TRUE);
}
