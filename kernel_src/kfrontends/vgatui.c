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

    vgatui.c: VGA text modes TUI-ish frontend

*/

#include "vgatui.h"

#include <stddef.h>

#define ARF_KERNEL
#include "../../common/game.h"
#include "../../common/frontconf.h"

#include "../convga.h"
#include "../plibc.h"
#include "../console.h"

static int size = 0;
static const int *board = NULL;


#define BXOFF   1
#define BYOFF   4

static int curx = 0, cury = 0;

const unsigned char charsets[2][9] = {
    {   /* ASCII 7-bit */
        '+',    /* Top left corner*/
        '+',    /* Top right corner*/
        '+',    /* Bottom left corner */
        '+',    /* Bottom right corner */
        '|',    /* Vertical line */
        '-',    /* Horizontal line */
        ' ',    /* Cleared cell */
        'F',    /* Flagged cell */
        '#'     /* Uncleared cell */
    },
    {   /* CP437 graphic characters */
        '\xc9', /* Top left corner*/
        '\xbb', /* Top right corner*/
        '\xc8', /* Bottom left corner */
        '\xbc', /* Bottom right corner */
        '\xba', /* Vertical line */
        '\xcd', /* Horizontal line */
        ' ',    /* Cleared cell */
        '\x14', /* Flagged cell */
        '\xb1'  /* Uncleared cell */
    }
};

static void
render(int charset) {
    con_clear();

    /* Draw title */
    con_move_cursor(0, 0);
    con_set_device_color(VGA_WHITE_ON_BLACK);
    con_print_string(TXT_TITLE);

    /* Check game state*/
    con_move_cursor(0, 2);
    switch (gameGetState()) {
        case STATE_LOST: {
            con_print_string(TXT_LOST);
            return;
        } break;
        case STATE_WON: {
            con_print_string(TXT_WON);
            return;
        } break;
    }

    /* Print flags left */
    con_move_cursor(14, 2);
    con_set_device_color(VGA_WHITE_ON_BLACK_BLINK);
    con_print_string(itoa(gameGetFlagsLeft(), 10));
    con_set_device_color(VGA_WHITE_ON_BLACK);

    /* Print board */
    con_move_cursor(BXOFF, BYOFF);
    con_print_char(charsets[charset][0]);
    con_move_cursor(BXOFF + size + 1, BYOFF);
    con_print_char(charsets[charset][1]);
    con_move_cursor(BXOFF, BYOFF + size + 1);
    con_print_char(charsets[charset][2]);
    con_move_cursor(BXOFF + size + 1, BYOFF + size + 1);
    con_print_char(charsets[charset][3]);

    for (int x = 0; x < size + 2; x += size + 1) {
        for (int y = 1; y < size + 1; y++) {
            con_move_cursor(BXOFF + x, BYOFF + y);
            con_print_char(charsets[charset][4]);
        }
    }

    for (int y = 0; y < size + 2; y += size + 1) {
        for (int x = 1; x < size + 1; x++) {
            con_move_cursor(BXOFF + x, BYOFF + y);
            con_print_char(charsets[charset][5]);
        }
    }

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            con_move_cursor(BXOFF + x + 1,  BYOFF + y + 1);
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);
                switch (n) {
                    case 1: con_set_device_color(VGA_BLUE_ON_BLACK); break;
                    case 2: con_set_device_color(VGA_GREEN_ON_BLACK); break;
                    case 3: con_set_device_color(VGA_RED_ON_BLACK); break;
                    case 4: con_set_device_color(VGA_DBLUE_ON_BLACK); break;
                    case 5: con_set_device_color(VGA_DRED_ON_BLACK); break;
                    case 6: con_set_device_color(VGA_DCYAN_ON_BLACK); break;
                    case 7: con_set_device_color(VGA_BLACK_ON_BLACK); break;
                    case 8: con_set_device_color(VGA_DGREY_ON_BLACK); break;
                }
                if (n)
                    con_print_char(itoa(n, 10)[0]);
                else {
                    con_set_device_color(VGA_WHITE_ON_BLACK);
                    con_print_char(charsets[charset][6]);
                }
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                con_set_device_color(VGA_DRED_ON_BLACK);
                con_print_char(charsets[charset][7]);
            }
            else {
                con_set_device_color(VGA_WHITE_ON_BLACK);
                con_print_char(charsets[charset][8]);
            }
        }
    }
    
    con_move_cursor(BXOFF + curx + 1, BYOFF + cury + 1);
}

int
vgatui_start(const int *lboard, int lsize, int charset) {
    board = lboard;
    size = lsize;

    curx = cury = 0;

    render(charset);

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

        render(charset);
    }
}

void
vgatui_destroy() {
    con_set_device_color(VGA_WHITE_ON_BLACK);
}

