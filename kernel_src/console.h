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

#ifndef _CONSOLE_H
#define _CONSOLE_H

/*
 * Console abstraction interface
 * _clear()
 * _set_char(char c, int x, int y)  // in text coordinates
 * _scroll_line()
 */
typedef struct {
    void(*clear)();
    void(*set_color)(int);
    void(*set_char)(char, int, int);
    void(*scroll_line)();
    void(*set_cursor)(int, int);
    int width, height;
} console_interface_t;


void con_init_convga(unsigned char mode, unsigned char font);
void con_init_fbrgb(void *fbaddr, int width, int height);

void con_clear();
void con_print_char(char c);
void con_print_string(const char *s);
void con_move_cursor(int x, int y);
void con_set_device_color(int c);

#endif /* _CONSOLE_H */

