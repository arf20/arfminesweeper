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

    fbrgb.c: 8888RGBX graphic framebuffer console

*/

#include "fbrgb.h"

#include "font_8x16.h"

#include "plibc.h"

#define FBXY(x, y)  fb[(fbwidth*(y))+(x)]

static unsigned int fbwidth = 0, fbheight = 0;
static unsigned int conwidth = 0, conheight = 0;
static unsigned int color = 0x00ffffff;
unsigned int *fb = 0;


void
fbrgb_clear() {
    for (unsigned int y = 0; y < fbheight; y++)
        for (unsigned int x = 0; x < fbwidth; x++)
            FBXY(x, y) = 0x00000000;

}

void
fbrgb_set_color(int _color) {
    color = _color;
}

void
fbrgb_set_char(char c, int ox, int oy) {
    ox *= FWIDTH; oy *= FHEIGHT;
    int coff = FWIDTH*FHEIGHT*(int)c;

    for (int y = 0; y < FHEIGHT; y++) {
        for (int x = 0; x < FWIDTH; x++) {
            int poff = coff + (y*FWIDTH)+x;
            int b = fontdata_8x16[poff/8];
            int p = (b >> (7-(poff%8))) & 0x1;
            if (p)
                FBXY(ox + x, oy + y) = color;
            else
                FBXY(ox + x, oy + y) = 0x00000000;
        }
    }
}

void
fbrgb_scroll_line() {
    /* Move buffer */
    memcpy(
        (char*)(&FBXY(FWIDTH*0, FHEIGHT*1)),
        (char*)(&FBXY(FWIDTH*0, FHEIGHT*0)),
        conwidth*fbwidth * ((conheight - 1)*fbheight)
    );

    /* Clear bottom line */
    memset(
        (char*)(&FBXY(FWIDTH*0, FHEIGHT*(conheight-1))),
        ((fbwidth*fbheight*4) - 1),
        0x00
    );
}

void
fbrgb_set_cursor(int ox, int oy) {
    ox *= FWIDTH; oy *= FHEIGHT;
    for (int y = 0; y < FHEIGHT; y++) {
        FBXY(ox + 0, oy + y) = 0x00ffffff;
        FBXY(ox + FWIDTH - 1, oy + y) = 0x00ffffff;
    }
    for (int x = 0; x < FWIDTH; x++) {
        FBXY(ox + x, oy + 0) = 0x00ffffff;
        FBXY(ox + x, oy + FHEIGHT - 1) = 0x00ffffff;
    }
}


console_interface_t fbrgb_con = {
    fbrgb_clear,
    fbrgb_set_color,
    fbrgb_set_char,
    fbrgb_scroll_line,
    fbrgb_set_cursor,
    0, 0
};

const console_interface_t *
fbrgb_init(void *fbaddr, unsigned int _fbwidth, unsigned int _fbheight) {
    fb = (unsigned int*)fbaddr;
    fbwidth = _fbwidth;
    fbheight = _fbheight;

    conwidth = fbwidth / FWIDTH;
    conheight = fbheight / FHEIGHT;

    fbrgb_con.width = conwidth;
    fbrgb_con.height = conheight;
    return &fbrgb_con;
}

