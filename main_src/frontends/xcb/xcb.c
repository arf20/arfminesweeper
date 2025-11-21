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
#include <stdint.h>
#include <unistd.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_icccm.h>

#include <common/frontconf.h>
#include <common/game.h>

#include "xcbutil.h"

#include "xcb.h"

#define TXT_OFFX    5
#define TXT_OFFY    15

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static xcb_connection_t *c;
static xcb_screen_t *s;
static xcb_window_t w;
static xcb_gcontext_t gc;

static uint32_t blue, green, red, darkblue, darkred, darkcyan, /*black*/ darkgrey;
static xcb_font_t font;


static void
drawFlag(int x, int y) {
    static xcb_point_t flag[5] = {
        {CELL_SIZE/4, 2},
        {CELL_SIZE/4, CELL_SIZE-2},
        {(CELL_SIZE/4) + 2, CELL_SIZE-2},
        {(CELL_SIZE/4) + 2, CELL_SIZE/2},
        {(3*CELL_SIZE/4) + 2, (CELL_SIZE/2)-3},
    };
    static xcb_point_t flagTrans[5];
    for (int i = 0; i < 5; i++) {
        flagTrans[i].x = flag[i].x + x;
        flagTrans[i].y = flag[i].y + y;
    }
    xcb_fill_poly(c, w, gc, XCB_POLY_SHAPE_COMPLEX, XCB_COORD_MODE_ORIGIN, 5, flagTrans);
}

static void
drawTextMultiline(int x, int y, const char *str) {
    const char *line = str, *next = NULL;
    int len = strlen(str), i = 0;
    while (line < str + len) {
        next = strchr(line, '\n');
        xcb_image_text_8(c, next - line, w, gc, x, y + (i * TXT_OFFY), line);
        line = next + 1;
        i++;
    }
}

void
render() {
    /* clear screen */
    /* xcb_clear_area(c, 1, w, 0, 0, wWidth, wHeight); */ /*BROKEN*/
    xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &s->black_pixel);
    xcb_poly_fill_rectangle(c, w, gc, 1,
        &(xcb_rectangle_t){ 0, 0, wWidth, wHeight });
    /* set foreground and font */
    xcb_change_gc(c, gc, XCB_GC_FOREGROUND | XCB_GC_FONT, (uint32_t[]){ s->white_pixel, font });

    /* draw title */
    xcb_image_text_8(c, strlen(TXT_TITLE), w, gc, 5, 15, TXT_TITLE);

    /* check game state*/
    switch (gameGetState()) {
        case STATE_LOST: {
            drawTextMultiline(5, 45, TXT_LOST);
            return;
        } break;
        case STATE_WON: {
            drawTextMultiline(5, 45, TXT_WON);
            return;
        } break;
    }

    /* print flags left */
    static char buff[256];
    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    xcb_image_text_8(c, strlen(buff), w, gc, wWidth - 25, 35, buff);

    /* render cell matrix */
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            /* if clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    snprintf(buff, 256, "%d", n);
                    switch (n) {
                        case 1: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &blue); break;
                        case 2: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &green); break;
                        case 3: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &red); break;
                        case 4: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &darkblue); break;
                        case 5: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &darkred); break;
                        case 6: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &darkcyan); break;
                        case 7: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &s->black_pixel); break;
                        case 8: xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &darkgrey); break;
                    }
                    xcb_image_text_8(c, strlen(buff), w, gc, cX + TXT_OFFX, cY + TXT_OFFY, buff);
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &s->white_pixel);
                xcb_rectangle_t rect = { cX, cY, CELL_SIZE, CELL_SIZE };
                xcb_poly_fill_rectangle(c, w, gc, 1, &rect);
                xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &red);
                drawFlag(cX, cY);
            }
            /* Otherwise just a tile */
            else {
                xcb_change_gc(c, gc, XCB_GC_FOREGROUND, &s->white_pixel);
                xcb_rectangle_t rect = { cX, cY, CELL_SIZE, CELL_SIZE };
                xcb_poly_fill_rectangle(c, w, gc, 1, &rect);
            }
        }
    }
}

int
xcb_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* connect */
    int screenidx;
    c = xcb_connect(NULL, &screenidx);

    /* get screen */
    s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

    xcb_flush(c);

    if (xcb_connection_has_error(c)) {
        fprintf(stderr, "Error in connection with X server\n");
        return -1;
    }

    /* create window */
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = {
        s->black_pixel,
        XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
        XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
        XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE
    };

    w = xcb_generate_id(c);
    xcb_create_window(c,                /* Connection          */
        XCB_COPY_FROM_PARENT,           /* depth (same as root)*/
        w,                              /* window Id           */
        s->root,                        /* parent window       */
        0, 0,                           /* x, y                */
        wWidth, wHeight,                /* width, height       */
        5,                              /* border_width        */
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  /* class               */
        s->root_visual,                 /* visual              */
        mask, values);                  /* masks, not used yet */


    xcb_size_hints_t hints;
    xcb_icccm_size_hints_set_min_size(&hints, wWidth, wHeight);
    xcb_icccm_size_hints_set_max_size(&hints, wWidth, wHeight);
    xcb_icccm_set_wm_size_hints(c, w, XCB_ATOM_WM_NORMAL_HINTS,
        &hints);

    /* set window name */
    xcb_change_property(c, XCB_PROP_MODE_REPLACE, w,
        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        sizeof(TXT_TITLE), TXT_TITLE);

    xcb_map_window(c, w);
    xcb_flush(c);

    /* create graphic context */
    mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t gc_values[2] = {
        s->white_pixel,
        s->black_pixel
    };

    gc = xcb_generate_id(c);
    xcb_create_gc(c, gc, w, mask, gc_values);

    /* allocate colors */
    xcb_colormap_t cm = s->default_colormap;
    xcb_alloc_named_color_cookie_t __blue, __green, __red, __darkblue, __darkred, __darkcyan, /*black*/ __darkgrey;
    xcb_alloc_named_color_reply_t *_blue, *_green, *_red, *_darkblue, *_darkred, *_darkcyan, /*black*/ *_darkgrey;

    __blue = xcb_alloc_named_color(c, cm, strlen("blue"), "blue");
    _blue = xcb_alloc_named_color_reply(c, __blue, 0);
    __green = xcb_alloc_named_color(c, cm, strlen("green"), "green");
    _green = xcb_alloc_named_color_reply(c, __green, 0);
    __red = xcb_alloc_named_color(c, cm, strlen("red"), "red");
    _red = xcb_alloc_named_color_reply(c, __red, 0);
    __darkblue = xcb_alloc_named_color(c, cm, strlen("darkblue"), "darkblue");
    _darkblue = xcb_alloc_named_color_reply(c, __darkblue, 0);
    __darkred = xcb_alloc_named_color(c, cm, strlen("darkred"), "darkred");
    _darkred = xcb_alloc_named_color_reply(c, __darkred, 0);
    __darkcyan = xcb_alloc_named_color(c, cm, strlen("darkcyan"), "darkcyan");
    _darkcyan = xcb_alloc_named_color_reply(c, __darkcyan, 0);
    __darkgrey = xcb_alloc_named_color(c, cm, strlen("darkgrey"), "darkgrey");
    _darkgrey = xcb_alloc_named_color_reply(c, __darkgrey, 0);

    blue = _blue->pixel;
    green = _green->pixel;
    red = _red->pixel;
    darkblue = _darkblue->pixel;
    darkred = _darkred->pixel;
    darkcyan = _darkcyan->pixel;
    darkgrey = _darkgrey->pixel;

    free(_blue); free(_green); free(_red); free(_darkblue); free(_darkred); free(_darkcyan); free(_darkgrey);
    
    /* allocate font */
    font = xcb_generate_id(c);
    xcb_open_font_checked(c, font, strlen("9x15"), "9x15");

    /* flush X11 commands */
    xcb_flush(c);

    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(c))) {
        switch (e->response_type & ~0x80) {
            case 0: {
                xcb_generic_error_t *error = (xcb_generic_error_t*)e;

                fprintf(stderr, "XCB %s opcode %s error %d minor\n",
                    opcode_str(error->major_code),
                    error_code_str(error->error_code), error->minor_code);
            } break;
            case XCB_EXPOSE: {
                xcb_expose_event_t *exp = (xcb_expose_event_t*)e;
                render();
                xcb_flush(c);
            } break;
            case XCB_BUTTON_RELEASE: {
                xcb_button_press_event_t *rel = (xcb_button_press_event_t*)e;

                int ix = (rel->event_x - W_MARGIN) /
                    (CELL_SIZE + CELL_MARGIN);
                int iy = (rel->event_y - HEADER_HEIGHT) /
                    (CELL_SIZE + CELL_MARGIN);
                if (ix < 0 || ix >= size || iy < 0 || iy >= size) continue;

                if (rel->state & XCB_BUTTON_MASK_1)
                    gameClearCell(ix, iy);
                else if (rel->state & XCB_BUTTON_MASK_3)
                    gameFlagCell(ix, iy);

                render();
                xcb_flush(c);
            } break;
        }
    }
}

void
xcb_destroy() {
    xcb_disconnect(c);
}

const char *
xcb_name() {
    return "xcb";
}

