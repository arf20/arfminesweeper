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

    fb.c: draw game in raw framebuffer

*/

#include <stdio.h>
#include <string.h>

#include <common/game.h>
#include <common/frontconf.h>

#include "fb.h"

static const int *board = NULL;
static int size = 0;
static int wWidth = 0, wHeight = 0;

/* fb */

static int sWidth = 0, sHeight = 0;
static bgra_t *fbp = NULL; /* assume 32bpp */
#define FB_XY(x, y)  fbp[((y) * sWidth) + (x)]

/* assume 9x15 font that starts at space */
static const unsigned char *font = NULL;
static int fontw = 0, fonth = 0;
#define FONT_W  9
#define FONT_H  15
#define FONT_FIRSTC 0x20

#define TXT_OFFX    5
#define TXT_OFFY    4

/* flag */
static const unsigned char *flag = NULL;
static int flagw = 0, flagh = 0;

/* cursor */
static const int *curx = 0, *cury = 0;

/* colors */
bgra_t fbColor(char r, char g, char b, char a) {
    bgra_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;
    return c;
}

#define FB_TRANS    fbColor(0x00, 0x00, 0x00, 0x00)
#define FB_WHITE    fbColor(0xff, 0xff, 0xff, 0xff)
#define FB_BLACK    fbColor(0x00, 0x00, 0x00, 0xff)
#define FB_BLUE     fbColor(0x00, 0x00, 0xff, 0xff)
#define FB_GREEN    fbColor(0x00, 0xff, 0x00, 0xff)
#define FB_RED      fbColor(0xff, 0x00, 0x00, 0xff)
#define FB_DARKBLUE fbColor(0x00, 0x00, 0x8b, 0xff)
#define FB_DARKRED  fbColor(0x8b, 0x00, 0x00, 0xff)
#define FB_DARKCYAN fbColor(0x00, 0x8b, 0x8b, 0xff)
#define FB_DARKGREY fbColor(0xa9, 0xa9, 0xa9, 0xff)

/* fb utils */
void
fbClear() {
    memset(fbp, 0, sizeof(int) * sWidth * sHeight);
}

void
fbFillRect(int ix, int iy, int w, int h, bgra_t c) {
    if (ix < 0 || iy < 0 || ix + w >= sWidth || iy + h >= sHeight) return;
    for (int y = iy; y < iy + h; y++)
        for (int x = ix; x < ix + w; x++)
            FB_XY(x, y) = c;
}

void
fbHLine(int ix, int l, int y, bgra_t c) {
    if (ix < 0 || y < 0 || ix + l >= sWidth || y >= sHeight) return;
    for (int x = ix; x < ix + l; x++)
        FB_XY(x, y) = c;
}

void
fbVLine(int x, int l, int iy, bgra_t c) {
    if (x < 0 || iy < 0 || x >= sWidth || iy + l >= sHeight) return;
    for (int y = iy; y < iy + l; y++)
        FB_XY(x, y) = c;
}

void
fbRenderFont(int dx, int dy, int sx, int sy, int w, int h, bgra_t fg) {
    if (dx < 0 || dy < 0 || dx + w > sWidth || dy + h > sHeight) return;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            FB_XY(dx + x, dy + y)
                = font[((sy + y) * fontw) + (sx + x)] > 0 ? fg : FB_TRANS;
}

void
fbDrawString(int ix, int iy, bgra_t fg, const char *str, size_t len) {
    const char *ptr = str;
    int i = 0;
    while (*ptr && (i < len)) {
        i = ptr - str;
        if (*ptr < FONT_FIRSTC) { ptr++; continue; }
        fbRenderFont((FONT_W * i) + ix, iy, FONT_W * (*ptr - FONT_FIRSTC), 0,
            FONT_W, FONT_H, fg);
        ptr++;
    }
}

static void
drawTextMultiline(int x, int y, const char *str) {
    const char *line = str, *next = NULL;
    int len = strlen(str), i = 0;
    while (line < str + len) {
        next = strchr(line, '\n');
        fbDrawString(x, y + (i * FONT_H), FB_WHITE, line, next - line);
        line = next + 1;
        i++;
    }
}

static void
drawFlag(int ix, int iy) {
    for (int y = 0; y < flagh; y++)
        for (int x = 0; x < flagw; x++)
            FB_XY(ix + x + 2, iy + y + 2)
                = flag[(flagw * y) + x] > 0 ? FB_RED : FB_WHITE;
}

void
fbRender() {
    fbClear();
    fbDrawString(5, 15, FB_WHITE, TXT_TITLE, sizeof(TXT_TITLE));

    /* Check game state*/
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

    /* Print flags left */
    static char buff[256];
    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    fbDrawString(wWidth - 25, 35, FB_WHITE, buff, strlen(buff));

    bgra_t c;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    snprintf(buff, 256, "%d", n);

                    switch (n) {
                        case 1: c = FB_BLUE; break;
                        case 2: c = FB_GREEN; break;
                        case 3: c = FB_RED; break;
                        case 4: c = FB_DARKBLUE; break;
                        case 5: c = FB_DARKRED; break;
                        case 6: c = FB_DARKCYAN; break;
                        case 7: c = FB_BLACK; break;
                        case 8: c = FB_DARKGREY; break;
                    }
                    fbDrawString(cX + TXT_OFFX, cY + TXT_OFFY,
                        c, buff, strlen(buff));
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                fbFillRect(cX, cY, CELL_SIZE, CELL_SIZE, FB_WHITE);
                drawFlag(cX, cY);
            }
            /* Otherwise just a tile */
            else {
                fbFillRect(cX, cY, CELL_SIZE, CELL_SIZE, FB_WHITE);
            }

            /* draw cursor */
            if (x == *curx && y == *cury) {
                fbHLine(cX, CELL_SIZE, cY, FB_RED);
                fbHLine(cX, CELL_SIZE, cY + CELL_SIZE - 1, FB_RED);
                fbVLine(cX, CELL_SIZE, cY, FB_RED);
                fbVLine(cX + CELL_SIZE - 1, CELL_SIZE, cY, FB_RED);
            }
        }
    }
}

void
fbRenderInit(const int *_board, int _size,
    void *_fbp, int _sWidth, int _sHeight,
    const unsigned char *_font, int _fontw, int _fonth,
    const unsigned char *_flag, int _flagw, int _flagh,
    const int *_curx, const int *_cury)
{
    board = _board;
    size = _size;
    fbp = _fbp;
    sWidth = _sWidth;
    sHeight = _sHeight;
    font = _font;
    fontw = _fontw;
    fonth = _fonth;
    flag = _flag;
    flagw = _flagw;
    flagh = _flagh;
    curx = _curx;
    cury = _cury;
}
