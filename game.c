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

  game.c: The game backend implementation

*/

#include <stdlib.h>
#include <time.h>

#include "game.h"

static int *board = NULL;

static int size = 0, mines = 0, flagsLeft = 0, state = 0;

#define FOREACH_SURROUNDING(x, y, a) \
    if (x > 0        && y > 0        && CHECK_MINE(BOARDXY(x - 1, y - 1))) a; \
    if (                y > 0        && CHECK_MINE(BOARDXY(x    , y - 1))) a; \
    if (x < size - 1 && y > 0        && CHECK_MINE(BOARDXY(x + 1, y - 1))) a; \
    if (x > 0                        && CHECK_MINE(BOARDXY(x - 1, y    ))) a; \
    if (x < size - 1                 && CHECK_MINE(BOARDXY(x + 1, y    ))) a; \
    if (x > 0        && y < size - 1 && CHECK_MINE(BOARDXY(x - 1, y + 1))) a; \
    if (                y < size - 1 && CHECK_MINE(BOARDXY(x    , y + 1))) a; \
    if (x < size - 1 && y < size - 1 && CHECK_MINE(BOARDXY(x + 1, y + 1))) a;

#define FOREACH_SURROUNDING_FXY(x, y, a) \
    if (x > 0        && y > 0        && CHECK_MINE(BOARDXY(x - 1, y - 1))) \
        a(x - 1, y - 1); \
    if (                y > 0        && CHECK_MINE(BOARDXY(x    , y - 1))) \
        a(x    , y - 1); \
    if (x < size - 1 && y > 0        && CHECK_MINE(BOARDXY(x + 1, y - 1))) \
        a(x + 1, y - 1); \
    if (x > 0                        && CHECK_MINE(BOARDXY(x - 1, y    ))) \
        a(x - 1, y    ); \
    if (x < size - 1                 && CHECK_MINE(BOARDXY(x + 1, y    ))) \
        a(x + 1, y    ); \
    if (x > 0        && y < size - 1 && CHECK_MINE(BOARDXY(x - 1, y + 1))) \
        a(x - 1, y + 1); \
    if (                y < size - 1 && CHECK_MINE(BOARDXY(x    , y + 1))) \
        a(x    , y + 1); \
    if (x < size - 1 && y < size - 1 && CHECK_MINE(BOARDXY(x + 1, y + 1))) \
        a(x + 1, y + 1);

/* Initialise the board */
int
gameInit(int lsize, int lmines) {
    size = lsize;
    mines = lmines;
    flagsLeft = 10;
    
    /* Allocate square board */
    board = malloc(sizeof(int) * size * size); 

    /* Initialise clear */
    for (int i = 0; i < size * size; i++)
        board[i] = CELL_EMPTY;

    /* Add mines at random locations */
    /* Seed the rand(3) pseudorandom number generator with time(2), good
        enough entropy */
    srand(time(NULL));

    int x = 0, y = 0, n = 0;
    while (n < mines) {
        x = rand() % size;
        y = rand() % size;

        /* If there is already a mine, regenerate location */
        if (CHECK_MINE(BOARDXY(x, y)))
            continue;
        
        /* Place mine */
        BOARDXY(x, y) = CELL_MINED;

        n++;
    }

    return 0;
}

const int * 
gameGetBoard() {
    return board;
}

int
gameGetSurroundingMines(int x, int y) {
    int n = 0;
    FOREACH_SURROUNDING(x, y, n++)
    return n;
}

/* Cell clearing recursive algorithm */
void
gameClearCell(int x, int y) {
    if (CHECK_MINE(BOARDXY(x, y))) {
        state = STATE_LOST;
    } else {
        /* Set clear bit and propagate surrounding cells */
        BOARDXY(x, y) |= 1 << CELL_BIT_CLEAR;
        FOREACH_SURROUNDING_FXY(x, y, gameClearCell)
    }
}

/* Toggle flag bit */
void
gameFlagCell(int x, int y) {
    BOARDXY(x, y) ^= 1 << CELL_BIT_FLAG;
    CHECK_FLAG(BOARDXY(x, y)) ? flagsLeft-- : flagsLeft++;
}
