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

*/

#ifndef _VGAFB_H
#define _VGAFB_H

#define VGA_256_BLACK   0
#define VGA_256_WHITE   15
#define VGA_256_BLUE    32
#define VGA_256_GREEN   2
#define VGA_256_RED     40
#define VGA_256_DBLUE   1
#define VGA_256_DRED    4
#define VGA_256_DCYAN   3
#define VGA_256_DGREY   8

extern int vgafb_width, vgafb_height, vgafb_colors;

void vgafb_set_pixel_256(int x, int y, unsigned char c);
void vgafb_clear_256(char c);
void vgafb_fill_rect_256(int ix, int iy, int w, int h, unsigned char c);
void vgafb_hline_256(int ix, int l, int y, unsigned char c);
void vgafb_vline_256(int ix, int l, int y, unsigned char c);

void vgag_init(unsigned char mode);

#endif /* _VGACON_H */
