/* ==========================================================================
 * DOOM macOS Native Port — App Delegate
 * ==========================================================================
 *
 * Creates the main window with a DoomView (NSView subclass), sets up the
 * menu bar, and launches the engine on a background thread.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#import "AppDelegate.h"
#import "DoomView.h"

#include "d_main.h"
#include "i_system.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    /* Create main window */
    NSRect frame = NSMakeRect(100, 100, 640, 400);
    self.window = [[NSWindow alloc]
        initWithContentRect:frame
        styleMask:(NSWindowStyleMaskTitled |
                   NSWindowStyleMaskClosable |
                   NSWindowStyleMaskMiniaturizable |
                   NSWindowStyleMaskResizable)
        backing:NSBackingStoreBuffered
        defer:NO];

    [self.window setTitle:@"DOOM"];
    [self.window setMinSize:NSMakeSize(320, 200)];
    [self.window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];

    /* Create the DOOM rendering view */
    DoomView *doomView = [[DoomView alloc] initWithFrame:frame];
    [self.window setContentView:doomView];
    [self.window makeFirstResponder:doomView];

    [self.window center];
    [self.window makeKeyAndOrderFront:nil];

    /* Build application menu */
    [self setupMenu];

    /* Launch engine on a background thread so the Cocoa event loop
     * stays responsive. */
    [NSThread detachNewThreadWithBlock:^{
        @autoreleasepool {
            D_DoomMain();
        }
    }];
}

- (void)setupMenu
{
    NSMenu *menuBar = [[NSMenu alloc] init];
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [menuBar addItem:appMenuItem];

    NSMenu *appMenu = [[NSMenu alloc] initWithTitle:@"DOOM"];
    [appMenu addItemWithTitle:@"About DOOM"
                       action:@selector(orderFrontStandardAboutPanel:)
                keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:@"Quit DOOM"
                       action:@selector(terminate:)
                keyEquivalent:@"q"];
    [appMenuItem setSubmenu:appMenu];

    [NSApp setMainMenu:menuBar];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    return NSTerminateNow;
}

@end
