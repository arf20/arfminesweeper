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

    kernel.c: kernel main

*/

#include "common.h"
#include "keyb.h"
#include "vgaterm.h"
#include "textdefs.h"

void
kmain() {
    vga_init();

    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);

    char sel = 0;
    do {
        sel = keyb_getc();
    } while (!(sel >= '0' && sel <= '2'));
    
    if (sel == '0') {
        return;
    }
    else if (sel == '1') {
        kprintf("Starting game with vgacon frontend\n");
    }
    else if (sel == '2') {
        kprintf("Starting game with vgatxt frontend\n");
    }
}
