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

    curses.c: curses/ncurses frontend

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/frontconf.h>
#include "curses.h"
#include <common/game.h>

#include <curses.h>


static int size = 0;
static const int *board = NULL;
int curx = 0, cury = 0;

WINDOW *win;

#define C_WHITE  1000, 1000, 1000
#define C_BLACK  0, 0, 0
#define C_RED    1000, 0, 0
#define C_GREEN  0, 1000, 0
#define C_BLUE   0, 0, 1000
#define C_DBLUE  0, 0, 545
#define C_DRED   545, 0, 0
#define C_DCYAN  0, 545, 545
#define C_DGREY  663, 663, 663

#define COLOR_DRED  8
#define COLOR_DBLUE 9
#define COLOR_DCYAN 10
#define COLOR_DGREY 11

#define CPAIR_CELL  9

void
displayBoard() {
    mvprintw(0, 0, "arfminesweeper");
    mvprintw(0, size + 7, "%d   ", gameGetFlagsLeft());

    switch (gameGetState()) {
        case STATE_LOST: {
            mvprintw(2, 0, TXT_LOST);
            return;
        } break;
        case STATE_WON: {
            mvprintw(2, 0, TXT_WON);
            return;
        } break;
    }

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int cX = 1 + x;
            int cY = 3 + y;
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                if (n) {
                    if (!(x == curx && y == cury))
                        attron(COLOR_PAIR(n));
                    mvprintw(cY, cX, "%d", n);
                    attroff(COLOR_PAIR(n));
                } else { 
                    if (x == curx && y == cury)
                        attron(COLOR_PAIR(CPAIR_CELL));
                    mvprintw(cY, cX, " ", n);
                    attroff(COLOR_PAIR(CPAIR_CELL));
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                if (!(x == curx && y == cury))
                    attron(COLOR_PAIR(3));
                mvprintw(cY, cX, "F");
                attroff(COLOR_PAIR(3));
            }
            /* Otherwise just a tile */
            else {
                if (!(x == curx && y == cury))
                    attron(COLOR_PAIR(CPAIR_CELL));
                mvprintw(cY, cX, " ");
                attroff(COLOR_PAIR(CPAIR_CELL));
            }
        }
    }

    box(win, 0, 0);
    wrefresh(win);
    refresh();
}

int
cursesStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    win = newwin(2 + size, 2 + size, 0, 0);
    mvwin(win, 2, 0);

    init_color(COLOR_BLUE, C_BLUE);
    init_color(COLOR_GREEN, C_GREEN);
    init_color(COLOR_RED, C_RED);
    init_color(COLOR_DBLUE, C_DBLUE);
    init_color(COLOR_DRED, C_DRED);
    init_color(COLOR_DCYAN, C_DCYAN);
    init_color(COLOR_BLACK, C_BLACK);
    init_color(COLOR_DGREY, C_DGREY);

    init_pair(1, COLOR_BLUE,  COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED,   COLOR_BLACK);
    init_pair(4, COLOR_DBLUE, COLOR_BLACK);
    init_pair(5, COLOR_DRED,  COLOR_BLACK);
    init_pair(6, COLOR_DCYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(8, COLOR_DGREY, COLOR_BLACK);
    init_pair(CPAIR_CELL, COLOR_WHITE, COLOR_WHITE);

    int ch = 0;
    displayBoard();
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_LEFT: curx--; break;
            case KEY_RIGHT: curx++; break;
            case KEY_UP: cury--; break;
            case KEY_DOWN: cury++; break;
            case 'f': gameFlagCell(curx, cury); break;
            case 'c': gameClearCell(curx, cury); break;
        }
        if (curx < 0) curx = size - 1;
        if (cury < 0) cury = size - 1;
        if (curx >= size) curx = 0;
        if (cury >= size) cury = 0;

        displayBoard();
    }

	endwin();

}

void
cursesDestroy() {

}

