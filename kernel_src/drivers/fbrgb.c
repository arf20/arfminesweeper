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

#include "../font_8x16.h"

#include "../plibc.h"
#include "../alloc.h"

#define FBXY(x, y)  fb[(fbwidth*(y))+(x)]

static unsigned int fbwidth = 0, fbheight = 0;
static unsigned int conwidth = 0, conheight = 0;
static unsigned int color = 0x00ffffff;
static int px = 0, py = 0;
unsigned int *realfb = 0;
unsigned int *fb = 0;

/* rgb colors */
static const unsigned int color_map[] = {
    /* WHITE       */ 0x00ffffff,
    /* WHITE_BLINK */ 0x00ffffff,
    /* BLACK       */ 0x00000000,
    /* RED         */ 0x00ff0000,
    /* DRED        */ 0x008b0000,
    /* GREEN       */ 0x0000ff00,
    /* DGREEN      */ 0x00008b00,
    /* BLUE        */ 0x000000ff,
    /* DBLUE       */ 0x0000008b,
    /* CYAN        */ 0x0000ffff,
    /* DCYAN       */ 0x00008b8b,
    /* DGREY       */ 0x00a9a9a9
};

static unsigned int
invert_pixel(unsigned int p) {
    return
        (unsigned char)(255 - (p & 0xff)) |
        ((unsigned int)((unsigned char)(255 - ((p >> 8) & 0xff))) << 8) |
        ((unsigned int)((unsigned char)(255 - ((p >> 16) & 0xff))) << 16);
}


void
fbrgb_clear() {
    for (int y = 0; y < fbheight; y++)
        for (int x = 0; x < fbwidth; x++)
            FBXY(x, y) = 0x00000000;

    for (int y = 0; y < FHEIGHT; y++)
        for (int x = 0; x < FWIDTH; x++)
            FBXY(x, y) = 0x00ffffff;
}

void
fbrgb_set_color(int color_index) {
    color = color_map[color_index];
}

void
fbrgb_set_char(char c, int ox, int oy) {
    ox *= FWIDTH; oy *= FHEIGHT;
    unsigned int coff = FWIDTH*FHEIGHT*(unsigned char)c;

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
    /* clear cursor */
    if (FBXY(px, py) && 0xff000000)
        for (int y = 0; y < FHEIGHT; y++)
            for (int x = 0; x < FWIDTH; x++)
                FBXY(px + x, py + y) = invert_pixel(FBXY(px + x, py + y));
    /* Move buffer */
    memmove(
        &FBXY(FWIDTH*0, FHEIGHT*0),
        &FBXY(FWIDTH*0, FHEIGHT*1),
        4 * fbwidth * ((conheight - 1) * FHEIGHT)
    );

    /* Clear bottom line */
    memset(
        (char*)(&FBXY(FWIDTH*0, FHEIGHT*(conheight-1))),
        0x00,
        fbwidth*FHEIGHT*4
    );
}

void
fbrgb_set_cursor(int ox, int oy) {
    ox *= FWIDTH; oy *= FHEIGHT;

    if (FBXY(px, py) && 0xff000000)
        for (int y = 0; y < FHEIGHT; y++)
            for (int x = 0; x < FWIDTH; x++)
                FBXY(px + x, py + y) = invert_pixel(FBXY(px + x, py + y));

    for (int y = 0; y < FHEIGHT; y++)
        for (int x = 0; x < FWIDTH; x++)
            FBXY(ox + x, oy + y) = invert_pixel(FBXY(ox + x, oy + y));
    FBXY(ox, oy) = 0xff000000 | FBXY(ox, oy);
    px = ox; py = oy;
}

void
fbrgb_swap() {
    for (int i = 0; i < fbwidth * fbheight; i++)
        realfb[i] = fb[i];
}


console_interface_t fbrgb_con = {
    fbrgb_clear,
    fbrgb_set_color,
    fbrgb_set_char,
    fbrgb_scroll_line,
    fbrgb_set_cursor,
    fbrgb_swap,
    0, 0
};

const console_interface_t *
fbrgb_init(void *fbaddr, unsigned int _fbwidth, unsigned int _fbheight) {
    realfb = (unsigned int*)fbaddr;
    fbwidth = _fbwidth;
    fbheight = _fbheight;

    /* text rows/columns */
    conwidth = fbwidth / FWIDTH;
    conheight = fbheight / FHEIGHT;

    color = 0x00ffffff;

    fb = kmalloc(4 * fbwidth * fbheight);


    fbrgb_con.width = conwidth;
    fbrgb_con.height = conheight;
    return &fbrgb_con;
}

