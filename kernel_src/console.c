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

    console.c: console abstraction

*/

#include "console.h"

#include "convga.h"
//#include "conrgbfb.h"


static void(*_print_char)(char, int) = 0;
static void(*_clear)() = 0;

void
con_init_vga(unsigned char mode, unsigned char font) {
    _print_char = vga_print_char;
    _clear = vga_clear;

    vga_init(mode, font);
}

void
con_init_fb(int width, int height) {

}

void
con_clear() {
    if (!_clear)
        return;
    _clear();
}

void
con_print_char(char c, int off) {
    if (!_print_char)
        return;
    _print_char(c, off);
}

