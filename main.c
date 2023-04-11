/*
 * main.c: Program entry point
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
    int size = 0, mines = 0;
    if (argc == 1) {
        /* Defaults */
        size = 8;
        mines = 10;
    }

    initGame(size, mines);

    conStart(getBoard(), size);
}
        