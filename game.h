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
#define CELL_CLEAR          2

/* Permutations */
#define CELL_EMPTY          0
#define CELL_MINED          0b01
#define CELL_NOMINE_FLAGGED 0b10
#define CELL_MINED_FLAGGED  0b11

/* Bit field check macros */
#define CHECK_MINE(x)       (((x) << CELL_BIT_MINE) & 1)
#define CHECK_FLAG(x)       (((x) << CELL_BIT_FLAG) & 1)

int initGame(int size, int mines);
const int * getBoard();

#endif
