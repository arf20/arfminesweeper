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

#ifndef _VGACON_H
#define _VGACON_H

/* text mode colors */
#define WHITE_ON_BLACK  0x0f
#define WHITE_ON_BLACK_BLINK  0x8f
#define BLACK_ON_BLACK  0x00
#define RED_ON_BLACK    0x0c
#define DRED_ON_BLACK   0x04
#define GREEN_ON_BLACK  0x0a
#define DGREEN_ON_BLACK 0x02
#define BLUE_ON_BLACK   0x09
#define DBLUE_ON_BLACK  0x01
#define CYAN_ON_BLACK   0x0b
#define DCYAN_ON_BLACK  0x03
#define DGREY_ON_BLACK  0x08


extern int vgatxt_rows;
extern int vgatxt_cols;
#define VGATXTXY(x, y)  (2 * ((vgatxt_cols * (y)) + (x)))

/* Computation operations */
int vga_offset_row(int off);
int vga_row_col_offset(int col, int row);
/* Cursor operations */
void vga_set_cursor_off(int offset);
int vga_get_cursor_off();
/* Buffer operations */
void vga_clear();
void vga_set_char(char c, int off);
void vga_print_char(char c, int off);
void vga_print_string(const char *str, int off);
void vga_set_char_c(char c, int off, unsigned char color);
void vga_print_char_c(char c, int off, unsigned char color);
void vga_print_string_c(const char *str, int off, unsigned char color);
void vga_init(unsigned char mode, unsigned char font);
void vgag_init(unsigned char mode);

#endif /* _VGACON_H */
