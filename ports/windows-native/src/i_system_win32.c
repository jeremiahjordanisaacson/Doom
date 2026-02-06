/* ==========================================================================
 * DOOM Windows Native Port — System Interface (Win32)
 * ==========================================================================
 *
 * Platform-specific system functions: timing, file I/O paths, error
 * handling, and process control for Windows.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#include "doomdef.h"
#include "doomtype.h"
#include "i_system.h"
#include "lprintf.h"

extern HWND g_hWnd;
extern int  g_running;

static LARGE_INTEGER s_perfFreq;
static LARGE_INTEGER s_perfStart;
static int           s_timerInit = 0;

/* -----------------------------------------------------------------------
 * I_GetTime_RealTime — Returns time in 1/35th second tics (DOOM ticrate)
 * ----------------------------------------------------------------------- */
int I_GetTime_RealTime(void)
{
    if (!s_timerInit) {
        QueryPerformanceFrequency(&s_perfFreq);
        QueryPerformanceCounter(&s_perfStart);
        s_timerInit = 1;
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    double elapsed = (double)(now.QuadPart - s_perfStart.QuadPart) /
                     (double)s_perfFreq.QuadPart;

    return (int)(elapsed * 35.0);
}

/* -----------------------------------------------------------------------
 * I_GetTime — Alias for the real-time timer
 * ----------------------------------------------------------------------- */
int (*I_GetTime)(void) = I_GetTime_RealTime;

/* -----------------------------------------------------------------------
 * I_GetTimeFrac — Sub-tic fraction for interpolation
 * ----------------------------------------------------------------------- */
fixed_t I_GetTimeFrac(void)
{
    return FRACUNIT;
}

/* -----------------------------------------------------------------------
 * I_GetTime_SaveMS — Microsecond timer for save-game timestamps
 * ----------------------------------------------------------------------- */
unsigned long I_GetRandomTimeSeed(void)
{
    return (unsigned long)GetTickCount();
}

/* -----------------------------------------------------------------------
 * I_Init — One-time system initialisation
 * ----------------------------------------------------------------------- */
void I_Init(void)
{
    QueryPerformanceFrequency(&s_perfFreq);
    QueryPerformanceCounter(&s_perfStart);
    s_timerInit = 1;
}

/* -----------------------------------------------------------------------
 * I_Error — Fatal error handler
 * ----------------------------------------------------------------------- */
void I_Error(const char *error, ...)
{
    char buf[2048];
    va_list ap;

    va_start(ap, error);
    vsnprintf(buf, sizeof(buf), error, ap);
    va_end(ap);

    /* Log to stderr */
    fprintf(stderr, "I_Error: %s\n", buf);

    /* Show a message box so the user sees the error */
    MessageBoxA(g_hWnd, buf, "DOOM - Fatal Error", MB_OK | MB_ICONERROR);

    exit(EXIT_FAILURE);
}

/* -----------------------------------------------------------------------
 * I_SafeExit — Clean shutdown
 * ----------------------------------------------------------------------- */
void I_SafeExit(int rc)
{
    g_running = 0;
    exit(rc);
}

/* -----------------------------------------------------------------------
 * I_Sleep — Yield the CPU for the given number of milliseconds
 * ----------------------------------------------------------------------- */
void I_Sleep(unsigned long ms)
{
    Sleep((DWORD)ms);
}

/* -----------------------------------------------------------------------
 * I_DoomExeDir — Return the directory containing the executable
 * ----------------------------------------------------------------------- */
const char *I_DoomExeDir(void)
{
    static char path[MAX_PATH] = {0};
    if (!path[0]) {
        GetModuleFileNameA(NULL, path, MAX_PATH);
        /* Strip the filename, leaving just the directory */
        char *slash = strrchr(path, '\\');
        if (slash) *slash = '\0';
    }
    return path;
}

/* -----------------------------------------------------------------------
 * I_FindFile — Locate a game data file
 * ----------------------------------------------------------------------- */
char *I_FindFile(const char *wfname, const char *ext)
{
    /* Try current directory first, then exe directory */
    static char buf[MAX_PATH];
    FILE *fp;

    snprintf(buf, sizeof(buf), "%s", wfname);
    fp = fopen(buf, "rb");
    if (fp) { fclose(fp); return buf; }

    snprintf(buf, sizeof(buf), "%s\\%s", I_DoomExeDir(), wfname);
    fp = fopen(buf, "rb");
    if (fp) { fclose(fp); return buf; }

    return NULL;
}

/* -----------------------------------------------------------------------
 * I_Read — Read from a file descriptor
 * ----------------------------------------------------------------------- */
void I_Read(int fd, void *buf, size_t count)
{
    _read(fd, buf, (unsigned int)count);
}

/* -----------------------------------------------------------------------
 * I_Filelength — Get the length of a file given its descriptor
 * ----------------------------------------------------------------------- */
int I_Filelength(int fd)
{
    long cur = _lseek(fd, 0, SEEK_CUR);
    long len = _lseek(fd, 0, SEEK_END);
    _lseek(fd, cur, SEEK_SET);
    return (int)len;
}
