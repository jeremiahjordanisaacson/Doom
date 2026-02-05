/* i_video_terminal.c - Terminal/ASCII video output for DOOM
 *
 * Converts the DOOM framebuffer to ASCII art using ncurses.
 * Each pixel is mapped to an ASCII character based on brightness.
 *
 * Part of the DOOM Multiplatform Mega-Repo
 * License: GPL v2
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "i_video.h"
#include "v_video.h"
#include "d_main.h"
#include "doomdef.h"
#include "doomtype.h"
#include "lprintf.h"

/* ASCII brightness ramp (dark to light) */
static const char ASCII_RAMP[] = " .:-=+*#%@";
#define RAMP_LEN (sizeof(ASCII_RAMP) - 1)

/* Terminal dimensions */
static int term_width = 80;
static int term_height = 50;

/* Doom framebuffer (set by engine) */
static unsigned char *doom_framebuffer = NULL;
static int fb_width = 320;
static int fb_height = 200;

/* Doom palette (768 bytes: 256 RGB triplets) */
static unsigned char current_palette[768];

void I_InitGraphics(void)
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    getmaxyx(stdscr, term_height, term_width);

    /* Use color if available */
    if (has_colors()) {
        start_color();
        use_default_colors();
        /* Basic DOOM color pairs */
        init_pair(1, COLOR_RED, -1);      /* Blood, walls */
        init_pair(2, COLOR_GREEN, -1);    /* Armor, health */
        init_pair(3, COLOR_YELLOW, -1);   /* Gold, keys */
        init_pair(4, COLOR_CYAN, -1);     /* Blue areas */
        init_pair(5, COLOR_WHITE, -1);    /* Bright areas */
    }

    lprintf(LO_INFO, "Terminal video initialized: %dx%d\n",
            term_width, term_height);
}

void I_ShutdownGraphics(void)
{
    endwin();
}

void I_SetPalette(int pal)
{
    /* Palette is set by the engine via I_UploadNewPalette */
    (void)pal;
}

void I_UploadNewPalette(int pal)
{
    /* Store palette for brightness calculation */
    (void)pal;
}

void I_UpdateNoBlit(void)
{
    /* No-op for terminal */
}

void I_FinishUpdate(void)
{
    if (!doom_framebuffer)
        return;

    int x, y;
    float x_scale = (float)fb_width / term_width;
    float y_scale = (float)fb_height / term_height;

    for (y = 0; y < term_height - 1; y++) {
        for (x = 0; x < term_width; x++) {
            /* Sample from doom framebuffer */
            int src_x = (int)(x * x_scale);
            int src_y = (int)(y * y_scale);

            if (src_x >= fb_width) src_x = fb_width - 1;
            if (src_y >= fb_height) src_y = fb_height - 1;

            unsigned char pixel = doom_framebuffer[src_y * fb_width + src_x];

            /* Convert palette index to brightness */
            int r = current_palette[pixel * 3 + 0];
            int g = current_palette[pixel * 3 + 1];
            int b = current_palette[pixel * 3 + 2];
            int brightness = (r * 299 + g * 587 + b * 114) / 1000;

            /* Map brightness to ASCII character */
            int ramp_idx = (brightness * (RAMP_LEN - 1)) / 255;
            char ch = ASCII_RAMP[ramp_idx];

            /* Choose color pair based on dominant channel */
            int color_pair = 5; /* default white */
            if (has_colors()) {
                if (r > g && r > b) color_pair = 1;       /* red */
                else if (g > r && g > b) color_pair = 2;  /* green */
                else if (r > 200 && g > 200) color_pair = 3; /* yellow */
                else if (b > r && b > g) color_pair = 4;  /* cyan/blue */
            }

            if (has_colors()) attron(COLOR_PAIR(color_pair));
            mvaddch(y, x, ch);
            if (has_colors()) attroff(COLOR_PAIR(color_pair));
        }
    }

    /* Status line */
    mvprintw(term_height - 1, 0, " DOOM Terminal | WASD:Move SPACE:Shoot E:Open Q:Quit ");

    refresh();
}

void I_ReadScreen(byte *scr)
{
    if (doom_framebuffer)
        memcpy(scr, doom_framebuffer, fb_width * fb_height);
}

void I_SetRes(void)
{
    /* Terminal resolution is fixed to terminal size */
}

/* Called by engine to set the framebuffer pointer */
void I_SetFramebuffer(unsigned char *fb, int width, int height)
{
    doom_framebuffer = fb;
    fb_width = width;
    fb_height = height;
}
