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

#include "console.h"
#include "../game.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int size = 0;
const int *board = NULL;

/* debug */
void
conPrintBoardDebug() {
    printf("+");
    for (int x = 0; x < size; x++)
        printf("-");
    printf("+\n");

    for (int y = 0; y < size; y++) {
        printf("|");
        for (int x = 0; x < size; x++)
            CHECK_MINE(BOARDXY(x, y)) ? printf("O") : printf("#");
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
            else {
                if (CHECK_FLAG(BOARDXY(x, y))) {
                    printf("F");
                }
                else printf("#"); /* uncleared */
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
printHelp() {
    printf("\thelp:             Get this message\n"
           "\tclear | c <x, y>: Clear cell\n"
           "\tflag  | f <x, y>: Place flag\n");
}

void
parseXYCommand(char *buff, int *x, int *y) {
    char *cmd = strtok(buff, " ");
    char *xstr = strtok(NULL, " ");
    char *ystr = strtok(NULL, " ");

    *x = atoi(xstr);
    *y = atoi(ystr);
}

int
conStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    char buffin[256];

    printf("Type `help` to get help\n");

    /* Console game loop */
    int x = 0, y = 0;
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
            parseXYCommand(buffin, &x, &y);
            printf("Cleared (%d, %d)\n", x, y);
            gameClearCell(x, y);
        }
        else if (!strncmp(buffin, "flag", 4) || !strncmp(buffin, "f", 1)) {
            parseXYCommand(buffin, &x, &y);
            printf("Flagged (%d, %d)\n", x, y);
            gameFlagCell(x, y);
        }
        else printf("?\n");
    }
}
