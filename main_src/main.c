/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    main.c: Program entry point

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/game.h>

#include "frontend.h"

#include <common/frontconf.h>

#ifdef _WIN32
    #include <windows.h>
#endif

void
print_usage(const char *self) {
    printf("Usage: %s [--frontend|-f API] [--size|-s size]\n"
        "\t[--mines|-m N of mines] [--help]\n\n"
        "\t--help | -h:     Get this message\n"
        "\t--frontend | -f: Frontend to use, see compiled\n"
        "\t--size | -s:     Board size (square side length)\n"
        "\t--mines | -m:    Number of random mines to place\n", self);
}

void
print_frontends() {
    printf("Available frontends:\n");
    if (frontends_size == 0)
        printf("\tNo frontends available.");
    else {
        printf("\t%s", frontends[0].name);
        for (int i = 1; i < frontends_size; i++)
            printf(", %s", frontends[i].name);
    }
    printf("\n");
}

int
main(int argc, char **argv) {
    printf("arfminesweeper " ARFMINESWEEPER_VERSION "-"
        ARFMINESWEEPER_NUM_COMMIT "\n"
        "Copyright (C) 2023-25 Ángel Ruiz Fernandez <arf20@arf20.com>\n"
        "License GPLv3+ <http://gnu.org/licenses/gpl.html>\n\n");

    frontend_init();
    frontend_load_static();
    print_frontends();

    const char *frontend_name = NULL;
    int size = 0, mines = 0;

    /* Parse command-line options */
    if (argc == 1) {
        /* Default */
    }
    else if (argc == 2 || argc == 4 || argc == 6) {
        /* Unpossible */
        print_usage(argv[0]);
        exit(1);
    }
    else {
        for (int i = 1; i < argc; i += 2) {
            if (!strcmp(argv[i], "--frontend") || !strcmp(argv[i], "-f"))
                frontend_name = argv[i + 1];
            if (!strcmp(argv[i], "--size") || !strcmp(argv[i], "-s"))
                size = atoi(argv[i + 1]);
            if (!strcmp(argv[i], "--mines") || !strcmp(argv[i], "-m"))
                mines = atoi(argv[i + 1]);
        }
    }

    if (frontend_name == NULL) frontend_name = "console";
    if (size == 0) size = 8;
    if (mines == 0) mines = 10;

    const frontend_t *frontend = frontend_find(frontend_name);
    if (!frontend) {
        printf("Error: Frontend not recognised: %s\n", frontend_name);
        print_frontends();
        exit(1);
    }

    printf("Starting game with %s frontend, %dx%d in size with %d mines\n",
        frontend->name, size, size, mines);
    gameInit(size, mines);

    frontend->start_func(gameGetBoard(), size);
    frontend->destroy_func();

    gameDestroy();

    return 0;
}

