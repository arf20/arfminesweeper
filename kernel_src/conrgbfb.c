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

    conrgbfb.c: 8888RGBX graphic framebuffer console

*/

#include "conrgbfb.h"

static unsigned int width = 0, height = 0;
unsigned int *fb = 0;

void
fb_clear() {
    while (1) {
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            fb[(width * y) + x] = 0xffffffff;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            fb[(width * y) + x] = 0x000000ff;
    }
}

void
fb_print_char(char c, int off) {

}

void
fb_init(void *fbaddr, unsigned int _width, unsigned int _height) {
    fb = (unsigned int*)fbaddr;
    width = _width;
    height = _height;
}

