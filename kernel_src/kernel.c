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
#include "vgacon.h"
#include "textdefs.h"
#include "alloc.h"

#include <stdint.h>

#include "../common/game.h"
#include "kfrontends/vgacli.h"
#include "kfrontends/vgatui.h"
#include "kfrontends/vgagra.h"

void
kmain() {
    /* Defaults */
    unsigned char vgamode = 0x03, vgagmode = 0x13, vgafont = 0x04;
    int size = 8, mines = 10;
    char ibuf[256];
    memset(ibuf, 0, 256);

cold_start:
    /* clear screen, set up terminal */
    vga_init(vgamode, vgafont);

    /* set heap at the start of Extended Memory (>1MiB), 1MiB in size */
    alloc_init((void*)0x00100000, (void*)0x001fffff);

warm_start:
    vga_clear();
    kprintf("%s\n%s", TXT_HELLO, TXT_MENU);
    kprintf("\nCurrent config: %dx%d size, %d mines, text mode %2Xh, graphic mode %2Xh\n", size, size, mines, vgamode, vgagmode);

    while (1) {
        char sel = keyb_getc();
        
        switch (sel) {
            case 'h': return;
            case 's': {
                kprintf("size> ");
                getsn(ibuf, 256);
                size = atoi(ibuf);
                goto warm_start;
            } break;
            case 'm': {
                kprintf("mines> ");
                getsn(ibuf, 256);
                mines = atoi(ibuf);
                goto warm_start;
            } break;
            case 'v': {
                vga_clear();
                kprintf(TXT_TEXT_MODES);
                getsn(ibuf, 256);
                vgamode = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'f': {
                vga_clear();
                kprintf(TXT_FONT);
                getsn(ibuf, 256);
                vgafont = strtol(ibuf, NULL, 16);
                goto cold_start;
            } break;
            case 'g': {
                vga_clear();
                kprintf(TXT_GRAPHIC_MODES);
                getsn(ibuf, 256);
                vgagmode = strtol(ibuf, NULL, 16);
                goto warm_start;
            } break;

            case '1': {
                kprintf("Starting game with vgacon frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgacli_start(gameGetBoard(), size);
                goto warm_start;
            } break;
            case '2': {
                kprintf("Starting game with vgatxt frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgatui_start(gameGetBoard(), size, 0);
                goto warm_start;
            } break;
            case '3': {
                kprintf("Starting game with vgatgr frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgatui_start(gameGetBoard(), size, 1);
                goto warm_start;
            } break;
            case '4': {
                kprintf("Starting game with vgagra frontend, %dx%d in size with %d mines\n",
                    size, size, mines);
                gameInit(size, mines);
                vgagra_start(gameGetBoard(), size, vgagmode);
                goto cold_start;
            } break;
            default: kprintf("Wrong key "); break;
        }
    }
}
