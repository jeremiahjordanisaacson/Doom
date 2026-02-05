/* main.c - Terminal DOOM entry point
 *
 * Initializes ncurses and launches the DOOM engine with
 * ASCII art rendering.
 *
 * License: GPL v2
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "d_main.h"
#include "m_argv.h"
#include "lprintf.h"

/* Clean shutdown on signals */
static void signal_handler(int sig)
{
    endwin();
    fprintf(stderr, "\nDOOM Terminal caught signal %d, exiting.\n", sig);
    exit(1);
}

int main(int argc, char **argv)
{
    /* Set up signal handlers for clean terminal restore */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSEGV, signal_handler);

    printf("DOOM Terminal - ASCII Renderer\n");
    printf("Starting DOOM engine...\n");

    /* Set engine arguments */
    myargc = argc;
    myargv = argv;

    /* Launch DOOM */
    D_DoomMain();

    /* Clean up (shouldn't reach here - D_DoomMain loops) */
    endwin();
    return 0;
}
