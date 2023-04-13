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
#include <stdlib.h>
#include <string.h>

#include "game.h"

#include "frontends/console.h"
#include "frontends/x11.h"

void
printUsage(const char *self) {
    printf("Usage: %s [--frontend|-f API] [--size|-s size]\n"
        "\t[--mines|-m N of mines] [--help]\n\n"
        "\t--help | -h:     Get this message\n"
        "\t--frontend | -f: Frontend to use, see compiled\n"
        "\t--size | -s:     Board size (square side length)\n"
        "\t--mines | -m:    Number of random mines to place\n", self);
}

void
printFrontends() {
    printf("Built with the following frontends:\n");
    #ifdef FRONTEND_CONSOLE
        printf("\tconsole ");
    #endif
    #ifdef FRONTEND_X11
        printf("x11 ");
    #endif

    printf("\n");
}

int
main(int argc, char **argv) {
    printf("arfminesweeper by arf20\n"
        "Copyright 2023 Ángel Ruiz Fernandez\n"
        "License GPLv3+ <http://gnu.org/licenses/gpl.html>\n\n");

    printFrontends();

    const char *frontend = NULL;
    int size = 0, mines = 0;

    /* Parse command-line options */
    if (argc == 1) {
        /* Default */
    }
    else if (argc == 2 || argc == 4 || argc == 6) {
        /* Unpossible */
        printUsage(argv[0]);
        exit(1);
    }
    else {
        for (int i = 1; i < argc; i += 2) {
            if (!strcmp(argv[i], "--frontend") || !strcmp(argv[i], "-f"))
                frontend = argv[i + 1];
            if (!strcmp(argv[i], "--size") || !strcmp(argv[i], "-s"))
                size = atoi(argv[i + 1]);
            if (!strcmp(argv[i], "--mines") || !strcmp(argv[i], "-m"))
                mines = atoi(argv[i + 1]);
        }
    }

    if (frontend == NULL) frontend = "console";
    if (size == 0) size = 8;
    if (mines == 0) mines = 10;

    printf("Starting game with %s frontend, %dx%d in size with %d mines\n",
        frontend, size, size, mines);
    gameInit(size, mines);

    if (!strcmp(frontend, "console")) {
        conStart(gameGetBoard(), size);
    }
    else if (!strcmp(frontend, "x11")) {
        X11Start(gameGetBoard(), size);
        X11Destroy();
    }
    else {
        printf("Error: Frontend not recognised: %s", frontend);
        printFrontends();
    }

    gameDestroy();

    return 0;
}
        