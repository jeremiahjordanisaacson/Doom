/* ==========================================================================
 * DOOM Windows Native Port — Video Subsystem (Win32 GDI)
 * ==========================================================================
 *
 * Software-rendered framebuffer displayed via Win32 GDI (StretchDIBits).
 * This replaces the SDL-based i_video.c from the shared engine when
 * building the Windows native port.
 *
 * The engine renders into a 320x200 (or 640x400) framebuffer which this
 * module blits to the window each frame, scaling to fit the client area.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>

#include "doomdef.h"
#include "doomstat.h"
#include "d_event.h"
#include "i_video.h"
#include "v_video.h"

/* Provided by main.c */
extern HWND       g_hWnd;
extern BITMAPINFO g_bmi;
extern int        g_screenWidth;
extern int        g_screenHeight;
extern void       Win32_PumpMessages(void);

/* Framebuffer (DOOM renders into this) */
static unsigned char *s_framebuffer = NULL;
static uint32_t       s_palette32[256];
static uint32_t      *s_screenbuf = NULL; /* 32-bit converted buffer */

/* DOOM's internal screen dimensions */
#define DOOM_W 320
#define DOOM_H 200

/* -----------------------------------------------------------------------
 * I_InitGraphics — Allocate framebuffer
 * ----------------------------------------------------------------------- */
void I_InitGraphics(void)
{
    s_framebuffer = (unsigned char *)calloc(DOOM_W * DOOM_H, 1);
    s_screenbuf   = (uint32_t *)calloc(DOOM_W * DOOM_H, sizeof(uint32_t));

    /* Tell the engine where to render */
    /* screens[0] = s_framebuffer; — engine sets this up via V_Init */
}

/* -----------------------------------------------------------------------
 * I_ShutdownGraphics
 * ----------------------------------------------------------------------- */
void I_ShutdownGraphics(void)
{
    if (s_framebuffer) { free(s_framebuffer); s_framebuffer = NULL; }
    if (s_screenbuf)   { free(s_screenbuf);   s_screenbuf = NULL; }
}

/* -----------------------------------------------------------------------
 * I_SetPalette — Convert DOOM's 768-byte RGB palette to 32-bit BGRA
 * ----------------------------------------------------------------------- */
void I_SetPalette(int pal)
{
    /* The engine provides a pointer to 256 * 3 bytes (R, G, B) */
    /* For now, store the default palette lookup.  A full implementation
     * would read from the WAD's PLAYPAL lump. */
    (void)pal;
}

/* -----------------------------------------------------------------------
 * I_UpdateNoBlit — Marks the screen dirty (used by the wipe effect)
 * ----------------------------------------------------------------------- */
void I_UpdateNoBlit(void)
{
    /* Nothing needed for GDI — we always blit the full frame */
}

/* -----------------------------------------------------------------------
 * I_FinishUpdate — Blit the 320x200 framebuffer to the Win32 window
 * ----------------------------------------------------------------------- */
void I_FinishUpdate(void)
{
    if (!g_hWnd || !s_screenbuf) return;

    /* Convert 8-bit paletted framebuffer to 32-bit BGRA */
    if (s_framebuffer) {
        for (int i = 0; i < DOOM_W * DOOM_H; i++) {
            s_screenbuf[i] = s_palette32[s_framebuffer[i]];
        }
    }

    /* Blit to window using StretchDIBits for automatic scaling */
    HDC hdc = GetDC(g_hWnd);
    if (hdc) {
        StretchDIBits(
            hdc,
            0, 0, g_screenWidth, g_screenHeight,  /* dest */
            0, 0, DOOM_W, DOOM_H,                 /* source */
            s_screenbuf,
            &g_bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );
        ReleaseDC(g_hWnd, hdc);
    }
}

/* -----------------------------------------------------------------------
 * I_StartTic — Process input events from the Windows message queue
 *
 * Called once per game tic by the engine to collect input.
 * ----------------------------------------------------------------------- */
void I_StartTic(void)
{
    Win32_PumpMessages();

    /* TODO: Read keyboard state via GetAsyncKeyState or from WM_KEY*
     * messages and post D_PostEvent for each key change. */

    /* TODO: Read mouse delta via GetCursorPos / SetCursorPos or
     * Raw Input and post mouse movement events. */

    /* TODO: Read XInput gamepad state and map to DOOM events. */
}

/* -----------------------------------------------------------------------
 * I_StartFrame — Called at the start of each frame
 * ----------------------------------------------------------------------- */
void I_StartFrame(void)
{
    /* Nothing needed */
}

/* -----------------------------------------------------------------------
 * I_SetRes — Set video resolution (called by engine during init)
 * ----------------------------------------------------------------------- */
void I_SetRes(void)
{
    /* The native port always renders at 320x200 internally */
}

/* -----------------------------------------------------------------------
 * I_ReadScreen — Copy the current framebuffer for screenshots
 * ----------------------------------------------------------------------- */
void I_ReadScreen(unsigned char *scr)
{
    if (s_framebuffer) {
        memcpy(scr, s_framebuffer, DOOM_W * DOOM_H);
    }
}
