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
#include "fbrgb.h"


static const console_interface_t *con = 0;

static int x = 0, y = 0, c = 0;

void
con_init_convga(unsigned char mode, unsigned char font) {
    con = convga_init(mode, font);
    con->set_cursor(x, y);
}

void
con_init_fbrgb(void *fbaddr, int width, int height) {
    con = fbrgb_init(fbaddr, width, height);
    con->set_cursor(x, y);
}

void
con_clear() {
    con->clear();
    x = 0;
    y = 0;
    con->set_cursor(x, y);
}

static void
print_char_nocur(char c) {
    /* handle control characters */
    if (c == '\n') {
        //con->set_char(' ', x, y);
        y++;
        x = 0;
    } else if (c == '\b') {
        x--;
        if (x<0) {
            y--;
            x = con->width - 1;
        }
    } else {
        con->set_char(c, x, y);
        x++;
        if (x >= con->width) {
            y++;
            x = 0;
        }
    }

    /* if after printing, cursor is ouside the screen, do scroll */
    if (y >= con->height) {
        con->scroll_line();
        y--;
    }
}

void
con_print_char(char c) {
    print_char_nocur(c);
    con->set_cursor(x, y);
}

void
con_print_string(const char *s) {
    while (*s)
        print_char_nocur(*s++);
    con->set_cursor(x, y);
}

void
con_move_cursor(int _x, int _y) {
    x = _x;
    y = _y;
    con->set_cursor(x, y);
}

void
con_set_device_color(int c) {
    con->set_color(c);
}

void
con_swap_buffers() {
    if (con->swap)
        con->swap();
}

