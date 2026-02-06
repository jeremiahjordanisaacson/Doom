/* ==========================================================================
 * DOOM macOS Native Port — Video Subsystem (Cocoa/Core Graphics)
 * ==========================================================================
 *
 * Software-rendered framebuffer displayed via Core Graphics in the DoomView.
 * The engine renders into a 320x200 8-bit paletted buffer; this module
 * converts it to 32-bit BGRA and triggers a view redraw each frame.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#import <Cocoa/Cocoa.h>
#include <stdlib.h>
#include <string.h>

#include "doomdef.h"
#include "doomstat.h"
#include "d_event.h"
#include "i_video.h"
#include "v_video.h"

/* Shared with DoomView for rendering */
uint32_t *g_macScreenBuf = NULL;
int       g_macScreenW   = 320;
int       g_macScreenH   = 200;

static unsigned char *s_framebuffer = NULL;
static uint32_t       s_palette32[256];

/* -----------------------------------------------------------------------
 * I_InitGraphics
 * ----------------------------------------------------------------------- */
void I_InitGraphics(void)
{
    s_framebuffer  = (unsigned char *)calloc(g_macScreenW * g_macScreenH, 1);
    g_macScreenBuf = (uint32_t *)calloc(g_macScreenW * g_macScreenH, sizeof(uint32_t));
}

/* -----------------------------------------------------------------------
 * I_ShutdownGraphics
 * ----------------------------------------------------------------------- */
void I_ShutdownGraphics(void)
{
    free(s_framebuffer);  s_framebuffer = NULL;
    free(g_macScreenBuf); g_macScreenBuf = NULL;
}

/* -----------------------------------------------------------------------
 * I_SetPalette
 * ----------------------------------------------------------------------- */
void I_SetPalette(int pal)
{
    (void)pal;
}

void I_UpdateNoBlit(void) {}

/* -----------------------------------------------------------------------
 * I_FinishUpdate — Convert framebuffer and trigger view redraw
 * ----------------------------------------------------------------------- */
void I_FinishUpdate(void)
{
    if (!s_framebuffer || !g_macScreenBuf) return;

    /* Convert paletted to 32-bit BGRA */
    for (int i = 0; i < g_macScreenW * g_macScreenH; i++) {
        g_macScreenBuf[i] = s_palette32[s_framebuffer[i]];
    }

    /* Schedule a redraw on the main thread */
    dispatch_async(dispatch_get_main_queue(), ^{
        NSWindow *window = [[NSApplication sharedApplication] mainWindow];
        if (window) {
            [[window contentView] setNeedsDisplay:YES];
        }
    });
}

void I_StartTic(void)   {}
void I_StartFrame(void) {}
void I_SetRes(void)     {}

void I_ReadScreen(unsigned char *scr)
{
    if (s_framebuffer) {
        memcpy(scr, s_framebuffer, g_macScreenW * g_macScreenH);
    }
}
