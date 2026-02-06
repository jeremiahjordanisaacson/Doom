/* ==========================================================================
 * DOOM macOS Native Port — Rendering View
 * ==========================================================================
 *
 * NSView subclass that handles keyboard/mouse input and provides a
 * rendering surface for the DOOM framebuffer.  Uses Core Graphics
 * for software rendering with an optional Metal backend.
 *
 * The engine renders into a 320x200 framebuffer (screens[0]).  Each
 * frame, I_FinishUpdate calls -setNeedsDisplay which triggers drawRect:
 * to blit the converted 32-bit buffer to the view.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#import "DoomView.h"
#include "d_event.h"
#include "doomdef.h"

/* Framebuffer shared with i_video_cocoa.m */
extern uint32_t *g_macScreenBuf;
extern int       g_macScreenW;
extern int       g_macScreenH;

@implementation DoomView

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)canBecomeKeyView { return YES; }

- (void)drawRect:(NSRect)dirtyRect
{
    if (!g_macScreenBuf || g_macScreenW <= 0 || g_macScreenH <= 0) {
        [[NSColor blackColor] setFill];
        NSRectFill(dirtyRect);
        return;
    }

    /* Create a CGImage from the 32-bit BGRA framebuffer */
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef bitmapCtx = CGBitmapContextCreate(
        g_macScreenBuf,
        g_macScreenW, g_macScreenH,
        8,                              /* bits per component */
        g_macScreenW * 4,               /* bytes per row */
        colorSpace,
        kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little
    );
    CGImageRef image = CGBitmapContextCreateImage(bitmapCtx);
    CGContextRelease(bitmapCtx);
    CGColorSpaceRelease(colorSpace);

    if (image) {
        CGContextRef viewCtx = [[NSGraphicsContext currentContext] CGContext];
        NSRect bounds = [self bounds];

        /* Maintain 4:3 aspect ratio */
        CGFloat viewW = bounds.size.width;
        CGFloat viewH = bounds.size.height;
        CGFloat aspect = 4.0 / 3.0;
        CGFloat drawW, drawH, drawX, drawY;

        if (viewW / viewH > aspect) {
            drawH = viewH;
            drawW = drawH * aspect;
        } else {
            drawW = viewW;
            drawH = drawW / aspect;
        }
        drawX = (viewW - drawW) / 2.0;
        drawY = (viewH - drawH) / 2.0;

        /* Use nearest-neighbor interpolation for pixel-perfect scaling */
        CGContextSetInterpolationQuality(viewCtx, kCGInterpolationNone);
        CGContextDrawImage(viewCtx, CGRectMake(drawX, drawY, drawW, drawH), image);
        CGImageRelease(image);
    }
}

/* -----------------------------------------------------------------------
 * Keyboard handling — convert NSEvent keyCodes to DOOM key codes
 * ----------------------------------------------------------------------- */
static int translateKey(unsigned short keyCode)
{
    /* macOS virtual key codes -> DOOM key codes */
    switch (keyCode) {
        case 0x00: return 'a';       /* kVK_ANSI_A */
        case 0x01: return 's';
        case 0x02: return 'd';
        case 0x03: return 'f';
        case 0x04: return 'h';
        case 0x05: return 'g';
        case 0x06: return 'z';
        case 0x07: return 'x';
        case 0x08: return 'c';
        case 0x09: return 'v';
        case 0x0B: return 'b';
        case 0x0C: return 'q';
        case 0x0D: return 'w';
        case 0x0E: return 'e';
        case 0x0F: return 'r';
        case 0x10: return 'y';
        case 0x11: return 't';
        case 0x12: return '1';
        case 0x13: return '2';
        case 0x14: return '3';
        case 0x15: return '4';
        case 0x16: return '6';
        case 0x17: return '5';
        case 0x19: return '9';
        case 0x1A: return '7';
        case 0x1C: return '8';
        case 0x1D: return '0';
        case 0x1F: return 'o';
        case 0x20: return 'u';
        case 0x22: return 'i';
        case 0x23: return 'p';
        case 0x25: return 'l';
        case 0x26: return 'j';
        case 0x28: return 'k';
        case 0x2D: return 'n';
        case 0x2E: return 'm';
        case 0x24: return KEY_ENTER;     /* Return */
        case 0x30: return KEY_TAB;       /* Tab */
        case 0x31: return ' ';           /* Space */
        case 0x33: return KEY_BACKSPACE; /* Delete */
        case 0x35: return KEY_ESCAPE;    /* Escape */
        case 0x38: return KEY_RSHIFT;    /* Left Shift */
        case 0x3B: return KEY_RCTRL;     /* Left Control */
        case 0x3A: return KEY_RALT;      /* Left Option */
        case 0x7B: return KEY_LEFTARROW;
        case 0x7C: return KEY_RIGHTARROW;
        case 0x7D: return KEY_DOWNARROW;
        case 0x7E: return KEY_UPARROW;
        case 0x60: return KEY_F5;
        case 0x61: return KEY_F6;
        case 0x62: return KEY_F7;
        case 0x63: return KEY_F3;
        case 0x64: return KEY_F8;
        case 0x65: return KEY_F9;
        case 0x67: return KEY_F11;
        case 0x6D: return KEY_F10;
        case 0x76: return KEY_F4;
        case 0x78: return KEY_F2;
        case 0x7A: return KEY_F1;
        default: return 0;
    }
}

- (void)keyDown:(NSEvent *)event
{
    int key = translateKey([event keyCode]);
    if (key) {
        event_t ev = { ev_keydown, key, 0, 0 };
        D_PostEvent(&ev);
    }
}

- (void)keyUp:(NSEvent *)event
{
    int key = translateKey([event keyCode]);
    if (key) {
        event_t ev = { ev_keyup, key, 0, 0 };
        D_PostEvent(&ev);
    }
}

- (void)mouseDown:(NSEvent *)event
{
    event_t ev = { ev_keydown, KEY_RCTRL, 0, 0 }; /* Fire */
    D_PostEvent(&ev);
}

- (void)mouseUp:(NSEvent *)event
{
    event_t ev = { ev_keyup, KEY_RCTRL, 0, 0 };
    D_PostEvent(&ev);
}

- (void)mouseMoved:(NSEvent *)event
{
    event_t ev = { ev_mouse, 0, (int)[event deltaX] * 5, -(int)[event deltaY] * 5 };
    D_PostEvent(&ev);
}

- (void)mouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

@end
