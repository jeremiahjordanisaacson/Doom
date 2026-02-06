/* ==========================================================================
 * DOOM macOS Native Port — System Interface
 * ==========================================================================
 *
 * macOS-specific system functions: high-resolution timing via mach
 * absolute time, file path resolution, and error handling.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#include <mach/mach_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <sys/stat.h>

#include "doomdef.h"
#include "doomtype.h"
#include "i_system.h"
#include "lprintf.h"

static mach_timebase_info_data_t s_timebaseInfo;
static uint64_t                  s_startTime;
static int                       s_timerInit = 0;

/* -----------------------------------------------------------------------
 * I_GetTime_RealTime — Returns time in 1/35th second tics
 * ----------------------------------------------------------------------- */
int I_GetTime_RealTime(void)
{
    if (!s_timerInit) {
        mach_timebase_info(&s_timebaseInfo);
        s_startTime = mach_absolute_time();
        s_timerInit = 1;
    }

    uint64_t now = mach_absolute_time();
    uint64_t elapsed = now - s_startTime;

    /* Convert to nanoseconds */
    double ns = (double)elapsed * s_timebaseInfo.numer / s_timebaseInfo.denom;

    /* Convert to 35 Hz tics */
    return (int)(ns / (1000000000.0 / 35.0));
}

int (*I_GetTime)(void) = I_GetTime_RealTime;

fixed_t I_GetTimeFrac(void)
{
    return FRACUNIT;
}

unsigned long I_GetRandomTimeSeed(void)
{
    return (unsigned long)mach_absolute_time();
}

/* -----------------------------------------------------------------------
 * I_Init
 * ----------------------------------------------------------------------- */
void I_Init(void)
{
    mach_timebase_info(&s_timebaseInfo);
    s_startTime = mach_absolute_time();
    s_timerInit = 1;
}

/* -----------------------------------------------------------------------
 * I_Error — Fatal error with dialog
 * ----------------------------------------------------------------------- */
void I_Error(const char *error, ...)
{
    char buf[2048];
    va_list ap;

    va_start(ap, error);
    vsnprintf(buf, sizeof(buf), error, ap);
    va_end(ap);

    fprintf(stderr, "I_Error: %s\n", buf);

    /* On macOS we could use NSAlert here, but that requires ObjC.
     * For the C system module, just print to stderr and exit. */
    exit(EXIT_FAILURE);
}

/* -----------------------------------------------------------------------
 * I_SafeExit
 * ----------------------------------------------------------------------- */
void I_SafeExit(int rc)
{
    exit(rc);
}

/* -----------------------------------------------------------------------
 * I_Sleep
 * ----------------------------------------------------------------------- */
void I_Sleep(unsigned long ms)
{
    usleep(ms * 1000);
}

/* -----------------------------------------------------------------------
 * I_DoomExeDir — Return the .app bundle's Resources directory
 * ----------------------------------------------------------------------- */
const char *I_DoomExeDir(void)
{
    static char path[PATH_MAX] = {0};
    if (!path[0]) {
        /* Try to get the executable path */
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            /* Go up from Contents/MacOS/doom to the .app parent */
            char *dir = dirname(path);
            snprintf(path, sizeof(path), "%s/../Resources", dir);
        } else {
            getcwd(path, sizeof(path));
        }
    }
    return path;
}

/* -----------------------------------------------------------------------
 * I_FindFile
 * ----------------------------------------------------------------------- */
char *I_FindFile(const char *wfname, const char *ext)
{
    static char buf[PATH_MAX];
    struct stat st;

    /* Current directory */
    snprintf(buf, sizeof(buf), "%s", wfname);
    if (stat(buf, &st) == 0) return buf;

    /* Exe directory (bundle resources) */
    snprintf(buf, sizeof(buf), "%s/%s", I_DoomExeDir(), wfname);
    if (stat(buf, &st) == 0) return buf;

    /* Home directory */
    const char *home = getenv("HOME");
    if (home) {
        snprintf(buf, sizeof(buf), "%s/.doom/%s", home, wfname);
        if (stat(buf, &st) == 0) return buf;
    }

    return NULL;
}

void I_Read(int fd, void *buf, size_t count)
{
    read(fd, buf, count);
}

int I_Filelength(int fd)
{
    long cur = lseek(fd, 0, SEEK_CUR);
    long len = lseek(fd, 0, SEEK_END);
    lseek(fd, cur, SEEK_SET);
    return (int)len;
}
