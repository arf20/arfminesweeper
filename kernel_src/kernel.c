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
#include "kfrontends/vgatxt.h"

void
kmain() {
    /* Defaults */
    int size = 8, mines = 10;
    char ibuf[256];
    memset(ibuf, 0, 256);

start:
    /* clear screen, set up terminal */
    vga_init();

    /* set heap at the start of Extended Memory (>1MiB), 1MiB in size */
    alloc_init((void*)0x00100000, (void*)0x001fffff);

    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);
    kprintf("Current config: %dx%d size, %d mines\n", size, size, mines);

    while (1) {
        char sel = keyb_getc();
        
        switch (sel) {
            case '0': return;
            case 's': {
                kprintf("new size> ");
                getsn(ibuf, 256);
                size = atoi(ibuf);
                goto start;
            } break;
            case 'm': {
                kprintf("new mines> ");
                getsn(ibuf, 256);
                mines = atoi(ibuf);
                goto start;
            } break;
            case '1': {
                kprintf("Starting game with vgacon frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                gameSetState(STATE_GOING);
                vgacon_start(gameGetBoard(), size);
                goto start;
            } break;
            case '2': {
                kprintf("Starting game with vgatxt frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                gameSetState(STATE_GOING);
                vgatxt_start(gameGetBoard(), size);
                goto start;
            } break;
            default: kprintf("Wrong key "); break;
        }
    }
}
