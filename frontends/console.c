/*

  Copyright (C) 2023 Ángel Ruiz Fernandez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, version 3.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see
  <http://www.gnu.org/licenses/>

  console.c: Basic console frontend

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "console.h"
#include "../game.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

static int size = 0;
static const int *board = NULL;

/* debug wallhack xray thing */
void
conPrintBoardDebug() {
    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");

    for (int y = 0; y < size; y++) {
        printf("|");
        for (int x = 0; x < size; x++) {
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                printf(" ");
            }
            else {
                if (CHECK_MINE(BOARDXY(x, y)) && CHECK_FLAG(BOARDXY(x, y))) printf("X");
                else if (CHECK_MINE(BOARDXY(x, y))) printf("O");
                else if (CHECK_FLAG(BOARDXY(x, y))) printf("F");
                else printf("#");
            }
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");
}

void
conPrintBoard() {
    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");

    for (int y = 0; y < size; y++) {
        printf("|");
        for (int x = 0; x < size; x++) {
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                /* If clear, count surrounding cells and print n of mines */
                int n = gameGetSurroundingMines(x, y);
                n ? printf("%d", n) : printf(" ");
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                printf("F");
            }
            else printf("#"); /* uncleared */
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");
}

void
printHelp() {
    printf("\thelp:             Get this message\n"
           "\tclear | c <x, y>: Clear cell\n"
           "\tflag  | f <x, y>: Place flag\n"
           "\tquit  | q:        Quit\n");
}

int
parseXYCommand(char *buff, int *x, int *y) {
    char *cmd = strtok(buff, " ");
    char *xstr = strtok(NULL, " ");
    char *ystr = strtok(NULL, " ");

    if (!xstr) { printf("?Missing operand\n"); return -1; }

    *x = atoi(xstr);
    *y = atoi(ystr);

    return 0;
}

int
conStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    char buffin[256];

    printf("Type `help` to get help\n");

    /* Console game loop */
    int x = 0, y = 0, r = 0;
    while (1) {
        /* Print state and prompt */
        conPrintBoard();
        printf("> ");
        memset(buffin, 256, 0);
        fgets(buffin, 256, stdin); /* safe */
       
        if (!strncmp(buffin, "help", 4) || !strncmp(buffin, "h", 1)) {
            printHelp();
        }
        else if (!strncmp(buffin, "clear", 5) || !strncmp(buffin, "c", 1)) {
            if (parseXYCommand(buffin, &x, &y)) continue;
            if (x < 0 || y < 0 || x >= size || y >= size) {
                printf("?Out of board bounds\n"); continue;
            }
            printf("Cleared (%d, %d)\n", x, y);
            gameClearCell(x, y);
        }
        else if (!strncmp(buffin, "flag", 4) || !strncmp(buffin, "f", 1)) {
            if (parseXYCommand(buffin, &x, &y)) continue;
            if (x < 0 || y < 0 || x >= size || y >= size) {
                printf("?Out of board bounds\n"); continue;
            }
            printf("Flagged (%d, %d)\n", x, y);
            gameFlagCell(x, y);
        }
        else if (!strncmp(buffin, "quit", 4) || !strncmp(buffin, "q", 1)) {
            return 0;
        }
        else printf("?\n");

        if (gameGetState() == STATE_LOST) {
            printf(TXT_LOST);
            return 0;
        }

        if (gameGetState() == STATE_WON) {
            printf(TXT_LOST);
            return 0;
        }
    }
}
