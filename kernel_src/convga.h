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

#ifndef _CONVGA_H
#define _CONVGA_H

#include "console.h"

/* text mode colors */
#define VGA_WHITE_ON_BLACK        0x0f
#define VGA_WHITE_ON_BLACK_BLINK  0x8f
#define VGA_BLACK_ON_BLACK        0x00
#define VGA_RED_ON_BLACK          0x0c
#define VGA_DRED_ON_BLACK         0x04
#define VGA_GREEN_ON_BLACK        0x0a
#define VGA_DGREEN_ON_BLACK       0x02
#define VGA_BLUE_ON_BLACK         0x09
#define VGA_DBLUE_ON_BLACK        0x01
#define VGA_CYAN_ON_BLACK         0x0b
#define VGA_DCYAN_ON_BLACK        0x03
#define VGA_DGREY_ON_BLACK        0x08


extern int vgatxt_rows;
extern int vgatxt_cols;
#define VGATXTXY(x, y)  (2 * ((vgatxt_cols * (y)) + (x)))

/* Computation operations */
int convga_xy_offset(int x, int y);

int convga_get_cursor_off();
/* Buffer operations */
void convga_set_char_c(char c, int off, unsigned char color);
/* Interface operations */
void convga_clear();
void convga_set_color(int c);
void convga_set_char(char c, int x, int y);
void convga_scroll_line();
void convga_set_cursor(int x, int y);

const console_interface_t *convga_init(unsigned char mode, unsigned char font);

#endif /* _CONVGA_H */
