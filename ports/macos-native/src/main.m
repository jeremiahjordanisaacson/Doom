/* ==========================================================================
 * DOOM macOS Native Port — Main Entry Point
 * ==========================================================================
 *
 * Cocoa/AppKit entry point for the native macOS DOOM port.  Creates an
 * NSApplication, sets up the app delegate, and launches the DOOM engine
 * on a background thread while the main thread runs the Cocoa event loop.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

#include "d_main.h"
#include "m_argv.h"

int main(int argc, const char *argv[])
{
    /* Store args for the engine */
    myargc = argc;
    myargv = (char **)argv;

    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        AppDelegate *delegate = [[AppDelegate alloc] init];
        [app setDelegate:delegate];

        /* Activate as a proper foreground application */
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];

        /* Run the Cocoa event loop (engine starts from applicationDidFinishLaunching:) */
        [app run];
    }

    return 0;
}
