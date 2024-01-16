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

    vgagra.c: VGA graphics modes frontend

*/

#include "vgagra.h"

#include <stddef.h>

#define ARF_KERNEL
#include "../../common/game.h"
#include "../../common/frontconf.h"

#include "../vgafb.h"
#include "../plibc.h"

static int size = 0;
static const int *board = NULL;

static int curx = 0, cury = 0;

static void
render(unsigned char mode) {
    vgafb_clear_256(VGA_256_BLACK);
    //fbDrawString(5, 15, FB_WHITE, TXT_TITLE, sizeof(TXT_TITLE));

    /* Check game state*/
    switch (gameGetState()) {
        case STATE_LOST: {
            //drawTextMultiline(5, 45, TXT_LOST);
            return;
        } break;
        case STATE_WON: {
            //drawTextMultiline(5, 45, TXT_WON);
            return;
        } break;
    }

    /* Print flags left */
    char *buff = itoa(gameGetFlagsLeft(), 10);
    //fbDrawString(wWidth - 25, 35, FB_WHITE, buff, strlen(buff));

    unsigned char c;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    buff = itoa(n, 10);

                    switch (n) {
                        case 1: c = VGA_256_BLUE; break;
                        case 2: c = VGA_256_GREEN; break;
                        case 3: c = VGA_256_RED; break;
                        case 4: c = VGA_256_DBLUE; break;
                        case 5: c = VGA_256_DRED; break;
                        case 6: c = VGA_256_DCYAN; break;
                        case 7: c = VGA_256_BLACK; break;
                        case 8: c = VGA_256_DGREY; break;
                    }
                    //fbDrawString(cX + TXT_OFFX, cY + TXT_OFFY,
                    //    c, buff, strlen(buff));
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                vgafb_fill_rect_256(cX, cY, CELL_SIZE, CELL_SIZE, VGA_256_WHITE);
                //drawFlag(cX, cY);
            }
            /* Otherwise just a tile */
            else {
                vgafb_fill_rect_256(cX, cY, CELL_SIZE, CELL_SIZE, VGA_256_WHITE);
            }

            /* draw cursor */
            if (x == curx && y == cury) {
                vgafb_hline_256(cX, CELL_SIZE, cY, VGA_256_RED);
                vgafb_hline_256(cX, CELL_SIZE, cY + CELL_SIZE - 1, VGA_256_RED);
                vgafb_vline_256(cX, CELL_SIZE, cY, VGA_256_RED);
                vgafb_vline_256(cX + CELL_SIZE - 1, CELL_SIZE, cY, VGA_256_RED);
            }
        }
    }
}

int
vgagra_start(const int *lboard, int lsize, unsigned char mode) {
    board = lboard;
    size = lsize;

    curx = cury = 0;

    vgag_init(mode);

    render(mode);

    char c = 0;
    while (1) {
        c = getchar();
        switch (c) {
            case 'q': return 0;
            case 'a': curx--; break;
            case 'd': curx++; break;
            case 'w': cury--; break;
            case 's': cury++; break;
            case 'c': gameClearCell(curx, cury); break;
            case 'f': gameFlagCell(curx, cury); break;
        }
        if (curx < 0)       curx = size - 1;
        if (cury < 0)       cury = size - 1;
        if (curx >= size)   curx = 0;
        if (cury >= size)   cury = 0;

        render(mode);
    }
}

void
vgagra_destroy() {

}
