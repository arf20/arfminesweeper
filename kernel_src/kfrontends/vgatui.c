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

    vgacon.c: VGA 80x25 (3) text mode TUI ish frontend

*/

#include "vgatui.h"

#include <stddef.h>

#define ARF_KERNEL
#include "../../common/game.h"
#include "../../common/frontconf.h"

#include "../vgacon.h"
#include "../plibc.h"

static int size = 0;
static const int *board = NULL;


#define BXOFF   1
#define BYOFF   4

static int curx = 0, cury = 0;

static void
render() {
    vga_clear();

    /* Draw title */
    vga_print_string(TXT_TITLE, 0);

    /* Check game state*/
    switch (gameGetState()) {
        case STATE_LOST: {
            vga_print_string(TXT_LOST, VGATXTXY(0, 2));
            return;
        } break;
        case STATE_WON: {
            vga_print_string(TXT_WON, VGATXTXY(0, 2));
            return;
        } break;
    }

    /* Print flags left */
    vga_print_string_c(itoa(gameGetFlagsLeft(), 10), VGATXTXY(14, 2), WHITE_ON_BLACK_BLINK);

    /* Print board */
    vga_set_char('+', VGATXTXY(BXOFF, BYOFF));
    vga_set_char('+', VGATXTXY(BXOFF + size + 1, BYOFF));
    vga_set_char('+', VGATXTXY(BXOFF, BYOFF + size + 1));
    vga_set_char('+', VGATXTXY(BXOFF + size + 1, BYOFF + size + 1));

    for (int x = 0; x < size + 2; x += size + 1)
        for (int y = 1; y < size + 1; y++)
            vga_set_char('|', VGATXTXY(BXOFF + x, BYOFF + y));

    for (int y = 0; y < size + 2; y += size + 1)
        for (int x = 1; x < size + 1; x++)
            vga_set_char('-', VGATXTXY(BXOFF + x, BYOFF + y));

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);
                unsigned char color = 0;
                switch (n) {
                    case 1: color = BLUE_ON_BLACK; break;
                    case 2: color = GREEN_ON_BLACK; break;
                    case 3: color = RED_ON_BLACK; break;
                    case 4: color = DBLUE_ON_BLACK; break;
                    case 5: color = DRED_ON_BLACK; break;
                    case 6: color = DCYAN_ON_BLACK; break;
                    case 7: color = BLACK_ON_BLACK; break;
                    case 8: color = DGREY_ON_BLACK; break;
                }
                n ? vga_set_char_c(itoa(n, 10)[0],
                    VGATXTXY(BXOFF + x + 1,  BYOFF + y + 1), color)
                    : vga_set_char(' ', VGATXTXY(BXOFF + x + 1, BYOFF + y + 1));
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                vga_set_char_c('F', VGATXTXY(BXOFF + x + 1, BYOFF + y + 1),
                    DRED_ON_BLACK);
            }
            else vga_set_char('#', VGATXTXY(BXOFF + x + 1, BYOFF + y + 1));
        }
    }
    
    vga_set_cursor_off(VGATXTXY(BXOFF + curx + 1, BYOFF + cury + 1));
}

int
vgatui_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    curx = cury = 0;

    vga_clear();
    render();

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

        render();
    }
}

void
vgatui_destroy() {

}
