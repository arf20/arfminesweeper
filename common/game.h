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

*/

#ifndef _GAME_H
#define _GAME_H

/* Board access XY macro */
#define BOARDXY(x, y)  board[((y) * size) + (x)]

/* Cell bit field */
#define CELL_BIT_MINE       0u
#define CELL_BIT_FLAG       1u
#define CELL_BIT_CLEAR      2u

/* Permutations */
#define CELL_EMPTY          0u
#define CELL_MINED          1u
#define CELL_FLAGGED        2u
#define CELL_MINED_FLAGGED  3u
#define CELL_CLEARED        4u
/*  A cleared cell can neither be mined or flagged */

/* Bit field check macros */
#define CHECK_MINE(x)       (((x) >> CELL_BIT_MINE) & 1u)
#define CHECK_FLAG(x)       (((x) >> CELL_BIT_FLAG) & 1u)
#define CHECK_CLEAR(x)      (((x) >> CELL_BIT_CLEAR) & 1u)

/* Game state */
#define STATE_GOING         0u
#define STATE_LOST          1u
#define STATE_WON           2u

int gameInit(int size, int mines);
void gameDestroy(void);
const int *gameGetBoard(void);
int gameGetSize(void);
int gameGetState(void);
void gameSetState(int s);
int gameGetSurroundingMines(int x, int y);
int gameGetFlagsLeft(void);
void gameClearCell(int x, int y);
void gameFlagCell(int x, int y);

#endif
