#ifndef _GAME_H
#define _GAME_H

/* Board access XY macro */
#define BOARDXY(x, y)  board[((y) * size) + (x)]

/* Cell bit field */
#define CELL_BIT_MINE       0
#define CELL_BIT_FLAG       1

/* Permutations */
#define CELL_EMPTY          0
#define CELL_MINED          0b10
#define CELL_NOMINE_FLAGGED 0b01
#define CELL_MINED_FLAGGED  0b11

/* Bit field check macros */
#define CHECK_MINE(x)       (((x) << CELL_BIT_MINE) & 1)
#define CHECK_FLAG(x)       (((x) << CELL_BIT_FLAG) & 1)

int initGame(int size, int mines);
const int * getBoard();

#endif
