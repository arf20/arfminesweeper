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

/* Interface operations */
void convga_clear();
void convga_set_color(int c);
void convga_set_char(char c, int x, int y);
void convga_scroll_line();
void convga_set_cursor(int x, int y);

const console_interface_t *convga_init(unsigned char mode, unsigned char font);

#endif /* _CONVGA_H */

