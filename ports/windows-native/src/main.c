/* ==========================================================================
 * DOOM Windows Native Port — Win32 Entry Point
 * ==========================================================================
 *
 * Native Win32 frontend for the DOOM engine.  Uses the Windows API directly
 * for window creation, input handling, and rendering (GDI/DirectDraw).
 * Audio is handled via DirectSound.
 *
 * This is an alternative to the SDL2-based desktop port, providing deeper
 * Windows integration (proper DPI scaling, native message loop, XInput
 * gamepad support, .WAD file association, etc.).
 *
 * Build:  cmake -B build -G "Visual Studio 17 2022" -A x64
 *         cmake --build build --config Release
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#ifndef _WIN32
#error "This port requires Windows"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <xinput.h>
#include <stdio.h>
#include <stdlib.h>

/* Engine headers */
#include "d_main.h"
#include "i_system.h"
#include "i_video.h"
#include "m_argv.h"
#include "lprintf.h"

/* Global state */
HWND        g_hWnd = NULL;
HINSTANCE   g_hInstance = NULL;
int         g_screenWidth = 640;
int         g_screenHeight = 400;
BITMAPINFO  g_bmi;
static int  g_running = 1;

/* Forward declarations */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void Win32_CreateWindow(HINSTANCE hInstance, int nCmdShow);
static void Win32_PumpMessages(void);
static void Win32_ParseCommandLine(LPSTR lpCmdLine);

/* Command-line storage */
static int    s_argc;
static char **s_argv;
static char   s_cmdline_buf[4096];
static char  *s_argv_ptrs[256];

/* -----------------------------------------------------------------------
 * WinMain — Win32 entry point
 * ----------------------------------------------------------------------- */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;

    g_hInstance = hInstance;

    /* Parse command line into argc/argv for the engine */
    Win32_ParseCommandLine(lpCmdLine);
    myargc = s_argc;
    myargv = s_argv;

    /* Enable high-DPI awareness */
    SetProcessDPIAware();

    /* Create the game window */
    Win32_CreateWindow(hInstance, nCmdShow);

    /* -----------------------------------------------------------------
     * Initialise and run the engine.
     *
     * D_DoomMain() sets up the WAD, loads resources, and enters the
     * main game loop.  On this port the engine calls back into our
     * I_* functions (i_video_win32.c, i_sound_win32.c, etc.) for
     * platform services.
     * ----------------------------------------------------------------- */
    D_DoomMain();

    return 0;
}

/* -----------------------------------------------------------------------
 * Win32_CreateWindow
 * ----------------------------------------------------------------------- */
static void Win32_CreateWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXA wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEXA);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "DoomWindowClass";

    RegisterClassExA(&wc);

    /* Calculate window size to fit the desired client area */
    RECT rc = { 0, 0, g_screenWidth, g_screenHeight };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindowExA(
        0,
        "DoomWindowClass",
        "DOOM",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hWnd) {
        MessageBoxA(NULL, "Failed to create window", "DOOM Error", MB_OK | MB_ICONERROR);
        ExitProcess(1);
    }

    /* Set up the BITMAPINFO for StretchDIBits (software rendering) */
    ZeroMemory(&g_bmi, sizeof(g_bmi));
    g_bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth       = 320;
    g_bmi.bmiHeader.biHeight      = -200;   /* Top-down */
    g_bmi.bmiHeader.biPlanes      = 1;
    g_bmi.bmiHeader.biBitCount    = 32;
    g_bmi.bmiHeader.biCompression = BI_RGB;

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
}

/* -----------------------------------------------------------------------
 * WndProc — Window message handler
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CLOSE:
        g_running = 0;
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        g_running = 0;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        /* Translate Win32 virtual keys to DOOM key events.
         * This is handled in i_video_win32.c via the posted
         * event queue (I_StartTic). */
        break;

    case WM_MOUSEMOVE:
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        break;

    case WM_SIZE:
        g_screenWidth  = LOWORD(lParam);
        g_screenHeight = HIWORD(lParam);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        /* Actual frame blitting is done by I_FinishUpdate in
         * i_video_win32.c using StretchDIBits. */
        EndPaint(hWnd, &ps);
        return 0;
    }
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

/* -----------------------------------------------------------------------
 * Win32_PumpMessages — Process pending Windows messages
 *
 * Called by the engine's I_StartTic / I_GetEvent functions each frame.
 * ----------------------------------------------------------------------- */
void Win32_PumpMessages(void)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            g_running = 0;
            I_SafeExit(0);
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

/* -----------------------------------------------------------------------
 * Win32_ParseCommandLine — Split command line into argc/argv
 * ----------------------------------------------------------------------- */
static void Win32_ParseCommandLine(LPSTR lpCmdLine)
{
    s_argc = 0;

    /* argv[0] = program name */
    s_argv_ptrs[s_argc++] = "doom";

    if (!lpCmdLine || !*lpCmdLine) {
        s_argv = s_argv_ptrs;
        return;
    }

    /* Copy into our buffer for tokenisation */
    strncpy(s_cmdline_buf, lpCmdLine, sizeof(s_cmdline_buf) - 1);
    s_cmdline_buf[sizeof(s_cmdline_buf) - 1] = '\0';

    char *p = s_cmdline_buf;
    while (*p && s_argc < 255) {
        while (*p == ' ') p++;
        if (!*p) break;

        if (*p == '"') {
            p++;
            s_argv_ptrs[s_argc++] = p;
            while (*p && *p != '"') p++;
        } else {
            s_argv_ptrs[s_argc++] = p;
            while (*p && *p != ' ') p++;
        }
        if (*p) *p++ = '\0';
    }

    s_argv = s_argv_ptrs;
}
