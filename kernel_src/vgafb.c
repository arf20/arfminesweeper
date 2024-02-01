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

    vgafb.c: VGA graphic modes driver

*/

#include "vgafb.h"

#include "int32.h"


int vgafb_width = 0, vgafb_height = 0, vgafb_colors = 0;

static unsigned char *fb = 0;

void
vgafb_set_pixel_256(int x, int y, unsigned char c) {
    fb[(vgafb_width * y) + x] = c;
}

void
vgafb_clear_256(char c) {
    for (int i = 0; i < vgafb_width * vgafb_height; i++)
        fb[i] = c;
}

void
vgafb_fill_rect_256(int ix, int iy, int w, int h, unsigned char c) {
    if (ix < 0 || iy < 0 || ix + w >= vgafb_width || iy + h >= vgafb_height) return;
    for (int y = iy; y < iy + h; y++)
        for (int x = ix; x < ix + w; x++)
            fb[(vgafb_width * y) + x] = c;
}

void
vgafb_hline_256(int ix, int l, int y, unsigned char c) {
    if (ix < 0 || y < 0 || ix + l >= vgafb_width || y >= vgafb_height) return;
    for (int x = ix; x < ix + l; x++)
        fb[(vgafb_width * y) + x] = c;
}

void
vgafb_vline_256(int x, int l, int iy, unsigned char c) {
    if (x < 0 || iy < 0 || x >= vgafb_width || iy + l >= vgafb_height) return;
    for (int y = iy; y < iy + l; y++)
        fb[(vgafb_width * y) + x] = c;
}

void
vgafb_bios_putc(char c) {
    regs16_t regs = { 0 };
    regs.ah = 0x0e;
    regs.al = c;
    int32(0x10, &regs);
}

void
vgafb_bios_print(const char *str) {
    while (*str) {
        vgafb_bios_putc(*str);
        str++;
    }
}


void
vgag_set_mode(unsigned char mode) {
    regs16_t regs = { 0 };
    regs.al = mode;
    int32(0x10, &regs);
}

void
vgag_init(unsigned char mode) {
    vgag_set_mode(mode);

    switch (mode) {
        case 0x04: vgafb_width = 320; vgafb_height = 200; vgafb_colors = 4;   fb = (unsigned char*)0xb8000; break;
        case 0x05: vgafb_width = 320; vgafb_height = 200; vgafb_colors = 4;   fb = (unsigned char*)0xb8000; break;
        case 0x06: vgafb_width = 320; vgafb_height = 200; vgafb_colors = 2;   fb = (unsigned char*)0xb8000; break;
        case 0x0d: vgafb_width = 320; vgafb_height = 200; vgafb_colors = 16;  fb = (unsigned char*)0xa0000; break;
        case 0x0e: vgafb_width = 640; vgafb_height = 200; vgafb_colors = 16;  fb = (unsigned char*)0xa0000; break;
        case 0x0f: vgafb_width = 640; vgafb_height = 350; vgafb_colors = 2;   fb = (unsigned char*)0xa0000; break;
        case 0x10: vgafb_width = 640; vgafb_height = 350; vgafb_colors = 16;  fb = (unsigned char*)0xa0000; break;
        case 0x11: vgafb_width = 640; vgafb_height = 480; vgafb_colors = 2;   fb = (unsigned char*)0xa0000; break;
        case 0x12: vgafb_width = 640; vgafb_height = 480; vgafb_colors = 16;  fb = (unsigned char*)0xa0000; break;
        case 0x13: vgafb_width = 320; vgafb_height = 200; vgafb_colors = 256; fb = (unsigned char*)0xa0000; break;
    }
}
