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

    ansi.c: terminal with ANSI-color-compatible escape enhancements

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <common/frontconf.h>
#include <common/game.h>

#include "vt100.h"

static int size = 0;
static const int *board = NULL;

/* cursor */
static int curx = 0, cury = 0;

static void
printBoard() {
    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");

    for (int y = 0; y < size; y++) {
        printf("|");
        for (int x = 0; x < size; x++) {
            if (x == curx && y == cury) /* set negative at cursor */
                printf("\e[7m");

            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);

                switch (n) {
                    case 1: printf("\e[94m"); break;
                    case 2: printf("\e[92m"); break;
                    case 3: printf("\e[91m"); break;
                    case 4: printf("\e[34m"); break;
                    case 5: printf("\e[31m"); break;
                    case 6: printf("\e[36m"); break;
                    case 7: printf("\e[90m"); break;
                    case 8: printf("\e[37m"); break;
                }

                n ? printf("%d", n) : printf(" ");

                printf("\e[0m");
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                printf("\e[31mF\e[0m");
            }
            else printf("#"); /* uncleared */

            if (x == curx && y == cury) /* restore positive at cursor */
                printf("\e[27m");
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");

    /* Go home */
    for (int x = size + 2; x > 0; x--)
        printf("\e[A");
}

int
ansiStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    char buffin[256];

    /* Set terminal to non-canonical non-blocking mode mode, this may fuck it up */
    /* term stuff */
    struct termios raw;
    struct termios orig;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);

    /* Save cursor home position */
    printf("\e[s");

    /* Console game loop */
    printBoard();

    char input[8] = { 0 };
    int bytesread = 0, run = 1;
    while (run) {
        do {
            bytesread = read(STDIN_FILENO, &input, 8);
            usleep(10000);
        } while (bytesread <= 0);

        for (int i = 0; i < bytesread; i++) {
            if (input[i] == '\033') {
                if (strncmp(input + i, "\033[A", 3) == 0) cury--;
                if (strncmp(input + i, "\033[B", 3) == 0) cury++;
                if (strncmp(input + i, "\033[C", 3) == 0) curx++;
                if (strncmp(input + i, "\033[D", 3) == 0) curx--;
                i += 2;
            } else if (isalpha(input[i])) {
                input[i] = tolower(input[i]);
                switch (input[i]) {
                    case 'a': curx--; break;
                    case 'd': curx++; break;
                    case 'w': cury--; break;
                    case 's': cury++; break;
                    case 'f': gameFlagCell(curx, cury); break;
                    case 'c': gameClearCell(curx, cury); break;
                }
            }
            if (curx < 0) curx = size - 1;
            if (cury < 0) cury = size - 1;
            if (curx >= size) curx = 0;
            if (cury >= size) cury = 0;
        }

        printBoard();
        memset(input, 0, 8);

        if (gameGetState() == STATE_LOST) {
            printf(TXT_LOST);
            run = 0;
        }

        if (gameGetState() == STATE_WON) {
            printf(TXT_WON);
            run = 0;
        }
    }
}
