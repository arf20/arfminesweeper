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

    glx.c: X11/GLX frontend

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/frontconf.h>
#include "glx.h"
#include <common/game.h>

#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <common/gl_ffp.h>

static int size = 0;
static const int *board = NULL;

static int wWidth = 0, wHeight = 0;

static Display *d = NULL;
static Window w;

static void
drawStringN(int x, int y, const char *str, int n,
    GLfloat r, GLfloat g, GLfloat b)
{

}

static void
drawTextMultiline(int x, int y, const char *str) {

}



static void
render() {
    glRender();
    glXSwapBuffers(d, w);
}

int
glx_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);
    
    /* Xlib is pretty self explanatory */
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }
    int s = DefaultScreen(d);

    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(d, 0, att);
    if (vi == NULL) {
        printf("No appropriate visual found\n");
        exit(1);
    } 

    Colormap cmap = XCreateColormap(d, XRootWindow(d, s), vi->visual, AllocNone);
    XSetWindowAttributes    swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;
    w = XCreateWindow(d, XRootWindow(d, s), 0, 0, wWidth, wHeight, 0, vi->depth,
        InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XStoreName(d, w, TXT_TITLE);

    XMapWindow(d, w);
    XSelectInput(d, w, ExposureMask | KeyPressMask | ButtonPressMask);

    GLXContext glc = glXCreateContext(d, vi, NULL, GL_TRUE);
    glXMakeCurrent(d, w, glc);

    glRenderInit(&drawStringN, &drawTextMultiline, board, size, wWidth, wHeight);

    /* Event loop */
    XEvent e;
    while (1) {
        XNextEvent(d, &e);
        switch (e.type) {
            case Expose: {
                render();
                XFlush(d);
            } break;
            case KeyPress: {
                render();
                XFlush(d);
            } break;
            case ButtonPress: {
                int ix = (e.xbutton.x - W_MARGIN) /
                    (CELL_SIZE + CELL_MARGIN);
                int iy = (e.xbutton.y - HEADER_HEIGHT) /
                    (CELL_SIZE + CELL_MARGIN);
                if (ix < 0 || ix >= size || iy < 0 || iy >= size) continue;

                switch (e.xbutton.button) {
                    case 1: { /* Left */
                        gameClearCell(ix, iy);
                    } break;
                    case 3: { /* Right */
                        gameFlagCell(ix, iy);
                    } break;
                }
                render();
                XFlush(d);
            } break;
        }
    }
}

void glx_destroy() {

}

const char *
glx_name() {
    return "glx";
}

