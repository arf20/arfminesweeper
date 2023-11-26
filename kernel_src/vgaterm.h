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

#ifndef _VGATERM_H
#define _VGATERM_H

/* Computation operations */
int vga_offset_row(int off);
int vga_row_col_offset(int col, int row);
/* Cursor operations */
void vga_set_cursor_off(int offset);
int vga_get_cursor_off();
/* Buffer operations */
void vga_set_char(char c, int off);
void vga_print_char(char c, int off);
void vga_print_string(const char *str, int off);
void vga_init();

#endif /* _VGATERM_H */
