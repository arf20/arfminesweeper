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

  main.c: Program entry point

*/

#include <stdio.h>

#include "game.h"

#include "frontends/console.h"

void
print_usage() {
    printf("Usage: %s [--api|-a API] [--size|-s size] [--mines|-m N of mines] [--help]\n");
}

int
main(int argc, char **argv) {
    printf("arfminesweeper Copyright 2023 arf20 License GPLv3+ <http://gnu.org/licenses/gpl.html>\n");

    int size = 0, mines = 0;
    if (argc == 1) {
        /* Defaults */
        printf("Starting default game in console, 8x8 with 10 mines\n");
        size = 8;
        mines = 10;
    }

    initGame(size, mines);

    conStart(getBoard(), size);
    conPrintBoardClear();
}
        