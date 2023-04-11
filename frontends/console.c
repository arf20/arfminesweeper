
/*
 * console.c: Basic console frontend
 */

#include <stdio.h>

#include "console.h"
#include "../game.h"

int size = 0;
const int *board = NULL;

void
printBoard() {
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

int
conStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    printBoard();
}