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
#define CELL_BIT_MINE       0
#define CELL_BIT_FLAG       1
#define CELL_BIT_CLEAR      2

/* Permutations */
#define CELL_EMPTY          0
#define CELL_MINED          0b001
#define CELL_FLAGGED        0b010
#define CELL_MINED_FLAGGED  0b011
#define CELL_CLEARED        0b100
/*  A cleared cell can neither be mined or flagged */

/* Bit field check macros */
#define CHECK_MINE(x)       (((x) << CELL_BIT_MINE) & 1)
#define CHECK_FLAG(x)       (((x) << CELL_BIT_FLAG) & 1)
#define CHECK_CLEAR(x)      (((x) << CELL_BIT_CLEAR) & 1)

int gameInit(int size, int mines);
const int * gameGetBoard();
int gameGetSurroundingMines(int x, int y);
void gameClearCell();

#endif
