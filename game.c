/*
 * game.c: The game backend implementation
*/

#include <stdlib.h>
#include <time.h>

#include "game.h"

static int *board = NULL;

static int size = 0, mines = 0;

/* Initialise the board */
int
initGame(int lsize, int lmines) {
    size = lsize;
    mines = lmines;
    
    /* Allocate square board */
    board = malloc(sizeof(int) * size * size); 

    /* Initialise clear */
    for (int i = 0; i < size * size; i++)
        board[i] = CELL_EMPTY;

    /* Add mines at random locations */
    /* Seed the rand(1) pseudorandom number generator with time(1), good enough entropy */
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
getBoard() {
    return board;
}
