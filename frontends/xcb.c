/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    xcb.c: X11 xcb frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <xcb/xcb.h>

#include "common.h"
#include "../game.h"
#include "xcb.h"

#define TXT_OFFX    5
#define TXT_OFFY    15

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static xcb_connection_t *c;
static xcb_window_t w;

uint32_t blue, green, red, darkblue, darkred, darkcyan, /*black*/ darkgrey;

void
render() {
    /* clear screen */
    xcb_clear_area(c, 1, w, 0, 0, wWidth, wHeight);

    //xcb_change_gc(c, gc, XCB_GC_FOREGROUND, )

    
}

int
xcbStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* connect */
    int screenidx;
    c = xcb_connect(NULL, &screenidx);

    /* get screen */
    xcb_screen_t *s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

    /* create window */
    uint32_t values[2];
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = s->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE;


    w = xcb_generate_id(c);
    xcb_create_window(c,      /* Connection          */
        XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
        w,                        /* window Id           */
        s->root,                  /* parent window       */
        0, 0,                          /* x, y                */
        wWidth, wHeight,               /* width, height       */
        10,                            /* border_width        */
        XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
        s->root_visual,                /* visual              */
        mask, values);                 /* masks, not used yet */

    xcb_map_window(c, w);

    /* set window name */
    xcb_change_property(c, XCB_PROP_MODE_REPLACE, w,
        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        sizeof(TXT_TITLE), TXT_TITLE);

    /* create graphic context */
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = s->white_pixel;
    values[1] = 0;

    xcb_gcontext_t gc = xcb_generate_id(c);
    xcb_create_gc(c, gc, w, mask, values);

    xcb_colormap_t cm = s->default_colormap;
    xcb_alloc_named_color_cookie_t __blue, __green, __red, __darkblue, __darkred, __darkcyan, /*black*/ __darkgrey;
    __blue = xcb_alloc_named_color(c, cm, sizeof("blue"), "blue");
    __green = xcb_alloc_named_color(c, cm, sizeof("green"), "green");
    __red = xcb_alloc_named_color(c, cm, sizeof("red"), "red");
    __darkblue = xcb_alloc_named_color(c, cm, sizeof("darkblue"), "darkblue");
    __darkred = xcb_alloc_named_color(c, cm, sizeof("darkred"), "darkred");
    __darkcyan = xcb_alloc_named_color(c, cm, sizeof("darkcyan"), "darkcyan");
    __darkgrey = xcb_alloc_named_color(c, cm, sizeof("darkgrey"), "darkgrey");

    xcb_alloc_named_color_reply_t *_blue, *_green, *_red, *_darkblue, *_darkred, *_darkcyan, /*black*/ *_darkgrey;
    _blue = xcb_alloc_named_color_reply(c, __blue, 0);
    _green = xcb_alloc_named_color_reply(c, __green, 0);
    _red = xcb_alloc_named_color_reply(c, __red, 0);
    _darkblue = xcb_alloc_named_color_reply(c, __darkblue, 0);
    _darkred = xcb_alloc_named_color_reply(c, __darkred, 0);
    _darkcyan = xcb_alloc_named_color_reply(c, __darkcyan, 0);
    _darkgrey = xcb_alloc_named_color_reply(c, __darkgrey, 0);

    blue = _blue->pixel;
    green = _green->pixel;
    red = _red->pixel;
    darkblue = _darkblue->pixel;
    darkred = _darkred->pixel;
    darkcyan = _darkcyan->pixel;
    darkgrey = _darkgrey->pixel;


    /* flush X11 commands */
    xcb_flush(c);

    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(c))) {
        switch (e->response_type & ~0x80) {
            case XCB_EXPOSE: {
                render();
                xcb_flush (c);
                break;
            }
        }
    }

    pause();

}

void
xcbDestroy() {
    xcb_disconnect(c);
}
