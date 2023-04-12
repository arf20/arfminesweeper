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

#include "console.h"
#include "../game.h"

int size = 0;
const int *board = NULL;

/* debug */
void
conPrintBoardClear() {
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

    
}