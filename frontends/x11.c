/*

  Copyright (C) 2023 Ángel Ruiz Fernandez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, version 3.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see
  <http://www.gnu.org/licenses/>

  console.c: X11 Xlib frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>

#include "common.h"
#include "../game.h"
#include "x11.h"

#define TXT_OFFX    5
#define TXT_OFFY    15

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static Display *d = NULL;
static int s = 0;
static Window w = 0;
static GC gc;
static Font f;

void
drawFlag(int x, int y) {
    static XPoint flag[5] = {
        {CELL_SIZE/4, 2},
        {CELL_SIZE/4, CELL_SIZE-2},
        {(CELL_SIZE/4) + 2, CELL_SIZE-2},
        {(CELL_SIZE/4) + 2, CELL_SIZE/3},
        {(3*CELL_SIZE/4) + 2, CELL_SIZE/3},  
    };
    static XPoint flagTrans[5];
    for (int i = 0; i < 5; i++) {
        flagTrans[i].x = flag[i].x + x;
        flagTrans[i].y = flag[i].y + y;
    }
    XFillPolygon(d, w, gc, flagTrans, 5, Complex, CoordModeOrigin);
}

void
render() {
    XSetForeground(d, gc, WhitePixel(d, s));
    XSetFont(d, gc, f);

    static char buff[256];

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    snprintf(buff, 256, "%d", n);
                    XSetForeground(d, gc, WhitePixel(d, s));
                    XDrawString(d, w, gc, cX + TXT_OFFX, cY + TXT_OFFY, buff, strlen(buff));
                }
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                XSetForeground(d, gc, WhitePixel(d, s));
                XFillRectangle(d, w, gc, cX, cY, CELL_SIZE, CELL_SIZE);
                XColor c;
                c.flags = DoRed | DoGreen | DoBlue;
                c.red = 0;
                c.green = 255;
                c.blue = 0;
                XSetForeground(d, gc, c.pixel);
                drawFlag(cX, cY);
            }
            /* uncleared */
            else {
                XSetForeground(d, gc, WhitePixel(d, s));
                XFillRectangle(d, w, gc, cX, cY, CELL_SIZE, CELL_SIZE);
            }
        }
    }

    XSetForeground(d, gc, WhitePixel(d, s));
    XDrawString(d, w, gc, 5, 15, TITLE, strlen(TITLE));

    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    XDrawString(d, w, gc, wWidth - 25, 35, buff, strlen(buff));
}

int
X11Start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }
    s = DefaultScreen(d);

    w = XCreateSimpleWindow(d, XRootWindow(d, s), 50, 50, wWidth, wHeight,
        0, BlackPixel(d, s), BlackPixel(d, s));

    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    gc = XCreateGC(d, w, 0, NULL);

    f = XLoadFont(d, "9x15");

    gameFlagCell(0, 0);
    gameClearCell(1, 1);

    XEvent e;
    while (1) {
        XNextEvent(d, &e);
        switch (e.type) {
            case Expose: {
                render();
                XFlush(d);
            } break;
            case KeyPress: {
            
            } break;
            case ButtonPress: {
                switch (e.xbutton.button) {
                    case 1: { /* Left */
                    
                    } break;
                    case 3: { /* Right */
                    
                    } break;
                }
            } break;
        }
    }
}

void
X11Destroy() {
    XDestroyWindow(d, w);
    XCloseDisplay(d);
}
