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

#include "plibc.h"
#include "keyb.h"
#include "vgaterm.h"
#include "textdefs.h"
#include "alloc.h"

#include <stdint.h>

#include "../common/game.h"
#include "kfrontends/vgacon.h"

void
kmain() {
    vga_init();

    alloc_init((void*)0x000a0000, (void*)0x000fffff);

    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);

    int *memtestmem = kmalloc(4);
    kprintf("memtest loc: %x\n", (uint32_t)memtestmem);
    kprintf("memtest val before: %x\n", *memtestmem);
    *memtestmem = 0x12345678;
    kprintf("memtest val after : %x\n", *memtestmem);

    /* Defaults */
    int size = 8, mines = 10;

    char sel = 0;
    do {
        sel = keyb_getc();
    } while (!(sel >= '0' && sel <= '2'));
    
    
    if (sel == '0') {
        return;
    }
    else if (sel == '1') {
        kprintf("Starting game with vgacon frontend, %dx%d in size with %d mines\n",
            size, size, mines);
        gameInit(size, mines);
        vgacon_start(gameGetBoard(), mines);
    }
    else if (sel == '2') {
        kprintf("Starting game with vgatxt frontend\n");
    }
}
