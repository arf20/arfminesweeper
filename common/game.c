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

#include "game.h"

#ifdef __KERNEL__
    #include <linux/module.h>
    #include <linux/kernel.h>
    #include <linux/slab.h>  /* kmalloc */
    #define malloc(size)    kmalloc(size, GFP_KERNEL)
    #define free            kfree
#elif FRONTENDS_KERNEL
    #include <stddef.h>
    #include <stdint.h>
    #include "../kernel_src/alloc.h"
    #include "../kernel_src/random.h"
    #include "../kernel_src/plibc.h"
    #include "../kernel_src/rtc_time.h"
    #define rand    prng_rand
    #define srand   prng_srand
    #define malloc  kmalloc
    #define free    kfree
    #define time    rtc_time
#else
    #include <stdlib.h>
    #include <time.h>
#endif

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
    #ifndef __KERNEL__
    srand(time(NULL));
    #endif

    int x = 0, y = 0, n = 0;
    while (n < mines) {
        #ifndef __KERNEL__
        x = rand() % size;
        y = rand() % size;
        #else
        get_random_bytes(&x, sizeof(x));
        get_random_bytes(&y, sizeof(y));
        x %= size;  y %= size;
        #endif

        /* If there is already a mine, regenerate location */
        if (CHECK_MINE(BOARDXY(x, y)))
            continue;
        
        /* Place mine */
        BOARDXY(x, y) = CELL_MINED;

        n++;
    }

    return 0;
}

void
gameDestroy() {
    free(board);
}

const int * 
gameGetBoard() {
    return board;
}

int
gameGetState() {
    return state;
}

void
gameSetState(int s) {
    state = s;
}

int
gameGetFlagsLeft() {
    return flagsLeft;
}

int
gameGetSurroundingMines(int x, int y) {
    int n = 0;
    FOREACH_SURROUNDING(x, y, n++)
    return n;
}

int
checkWin(void) {
    for (int i = 0; i < size * size; i++)
        if ((!CHECK_CLEAR(board[i]) && !CHECK_MINE(board[i]))
            || (!CHECK_FLAG(board[i]) && CHECK_MINE(board[i]))) return 0;
    return 1;
}

/* Cell clearing recursive algorithm */
void
gameClearCell(int x, int y) {
    if (CHECK_CLEAR(BOARDXY(x, y)) || CHECK_FLAG(BOARDXY(x, y))) {
        return;
    }
    else if (CHECK_MINE(BOARDXY(x, y))) {
        state = STATE_LOST;
    } else {
        /* Set clear bit */
        BOARDXY(x, y) |= 1 << CELL_BIT_CLEAR;

        /* If no mine near, propagate surrounding cells */
        if (gameGetSurroundingMines(x, y) == 0) {
            if (x > 0 && y > 0 &&
                !CHECK_CLEAR(BOARDXY(x - 1, y - 1)) &&
                !CHECK_MINE(BOARDXY(x - 1, y - 1)))
                    gameClearCell(x - 1, y - 1);
            if (y > 0 &&
                !CHECK_CLEAR(BOARDXY(x    , y - 1)) &&
                !CHECK_MINE(BOARDXY(x    , y - 1)))
                    gameClearCell(x    , y - 1);
            if (x < size - 1 && y > 0        &&
                !CHECK_CLEAR(BOARDXY(x + 1, y - 1)) &&
                !CHECK_MINE(BOARDXY(x + 1, y - 1)))
                    gameClearCell(x + 1, y - 1);
            if (x > 0                        &&
                !CHECK_CLEAR(BOARDXY(x - 1, y    )) &&
                !CHECK_MINE(BOARDXY(x - 1, y    )))
                    gameClearCell(x - 1, y    );
            if (x < size - 1                 &&
                !CHECK_CLEAR(BOARDXY(x + 1, y    )) &&
                !CHECK_MINE(BOARDXY(x + 1, y    )))
                    gameClearCell(x + 1, y    );
            if (x > 0        && y < size - 1 &&
                !CHECK_CLEAR(BOARDXY(x - 1, y + 1)) &&
                !CHECK_MINE(BOARDXY(x - 1, y + 1)))
                    gameClearCell(x - 1, y + 1);
            if (                y < size - 1 &&
                !CHECK_CLEAR(BOARDXY(x    , y + 1)) &&
                !CHECK_MINE(BOARDXY(x    , y + 1)))
                    gameClearCell(x    , y + 1);
            if (x < size - 1 && y < size - 1 &&
                !CHECK_CLEAR(BOARDXY(x + 1, y + 1)) &&
                !CHECK_MINE(BOARDXY(x + 1, y + 1)))
                    gameClearCell(x + 1, y + 1);
        }

        if (checkWin()) state = STATE_WON;
    }
}

/* Toggle flag bit */
void
gameFlagCell(int x, int y) {
    if (CHECK_CLEAR(BOARDXY(x, y))) return;
    BOARDXY(x, y) ^= 1 << CELL_BIT_FLAG;
    CHECK_FLAG(BOARDXY(x, y)) ? flagsLeft-- : flagsLeft++;
    if (checkWin()) state = STATE_WON;
}
