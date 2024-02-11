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

#include "frontends/console.h"
#include "frontends/fbdev.h"
#include "frontends/xlib.h"
#include "frontends/motif.h"
#include "frontends/xforms.h"
#include "frontends/gtk3.h"
#include "frontends/qt5.hpp"
#include "frontends/sdl2.h"
#include "frontends/sdl1.h"
#include "frontends/gl11.h"
#include "frontends/gl33.h"
#include "frontends/vk.h"
#include "frontends/win32.h"
#include "frontends/httpd.h"
#include "frontends/wayland.h"
#include "frontends/xcb.h"
#include "frontends/gdi.h"
#include "frontends/d2d.h"
#include "frontends/xaw.h"

#include <common/frontconf.h>

#ifdef _WIN32
    #include <windows.h>
#endif

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
    #ifdef FRONTEND_FBDEV
        printf("fbdev ");
    #endif
    #ifdef FRONTEND_XLIB
        printf("xlib ");
    #endif
    #ifdef FRONTEND_MOTIF
        printf("motif ");
    #endif
    #ifdef FRONTEND_XFORMS
        printf("xforms ");
    #endif
    #ifdef FRONTEND_GTK3
        printf("gtk3 ");
    #endif
    #ifdef FRONTEND_QT5
        printf("qt5 ");
    #endif
    #ifdef FRONTEND_SDL2
        printf("sdl2 ");
    #endif
    #ifdef FRONTEND_SDL1
        printf("sdl12 ");
    #endif
    #ifdef FRONTEND_GL11
        printf("gl11 ");
    #endif
    #ifdef FRONTEND_GL33
        printf("gl33 ");
    #endif
    #ifdef FRONTEND_VK
        printf("vk ");
    #endif
    #ifdef FRONTEND_WIN32
        printf("win32 ");
    #endif
    #ifdef FRONTEND_HTTPD
        printf("webapp ");
    #endif
    #ifdef FRONTEND_WAYLAND
        printf("wayland ");
    #endif
    #ifdef FRONTEND_XCB
        printf("xcb ");
    #endif
    #ifdef FRONTEND_GDI
        printf("gdi ");
    #endif
    #ifdef FRONTEND_D2D
        printf("d2d ");
    #endif
    #ifdef FRONTEND_XAW
        printf("xaw ");
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
    else if (!strcmp(frontend, "xlib")) {
        #ifdef FRONTEND_XLIB
        XlibStart(gameGetBoard(), size);
        XlibDestroy();
        #else
        printf("Error: Frontend xlib not built\n");
        #endif
    }
    else if (!strcmp(frontend, "fbdev")) {
        #ifdef FRONTEND_FBDEV
        fbdevStart(gameGetBoard(), size);
        fbdevDestroy();
        #else
        printf("Error: Frontend fbdev not built\n");
        #endif
    }
    else if (!strcmp(frontend, "motif")) {
        #ifdef FRONTEND_MOTIF
        motifStart(gameGetBoard(), size);
        motifDestroy();
        #else
        printf("Error: Frontend motif not built\n");
        #endif
    }
    else if (!strcmp(frontend, "xforms")) {
        #ifdef FRONTEND_XFORMS
        xformsStart(gameGetBoard(), size);
        xformsDestroy();
        #else
        printf("Error: Frontend xforms not built\n");
        #endif
    }
    else if (!strcmp(frontend, "gtk3")) {
        #ifdef FRONTEND_GTK3
        Gtk3Start(gameGetBoard(), size);
        Gtk3Destroy();
        #else
        printf("Error: Frontend gtk not built\n");
        #endif
    }
    else if (!strcmp(frontend, "qt5")) {
        #ifdef FRONTEND_QT5
        Qt5Start(gameGetBoard(), size);
        Qt5Destroy();
        #else
        printf("Error: Frontend qt5 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "sdl2")) {
        #ifdef FRONTEND_SDL2
        SDL2Start(gameGetBoard(), size);
        SDL2Destroy();
        #else
        printf("Error: Frontend sdl2 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "sdl1")) {
        #ifdef FRONTEND_SDL1
        SDL1Start(gameGetBoard(), size);
        SDL1Destroy();
        #else
        printf("Error: Frontend sdl1 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "gl11")) {
        #ifdef FRONTEND_GL11
        GL11Start(gameGetBoard(), size);
        GL11Destroy();
        #else
        printf("Error: Frontend gl11 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "gl33")) {
        #ifdef FRONTEND_GL33
        GL33Start(gameGetBoard(), size);
        GL33Destroy();
        #else
        printf("Error: Frontend gl11 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "vk")) {
        #ifdef FRONTEND_VK
        vkStart(gameGetBoard(), size);
        vkDestroy();
        #else
        printf("Error: Frontend vk not built\n");
        #endif
    }
    else if (!strcmp(frontend, "win32")) {
        #ifdef FRONTEND_WIN32
        Win32Start(gameGetBoard(), size);
        Win32Destroy();
        #else
        printf("Error: Frontend win32 not built\n");
        #endif
    }
    else if (!strcmp(frontend, "webapp")) {
        #ifdef FRONTEND_HTTPD
        httpdStart(gameGetBoard(), size);
        httpdDestroy();
        #else
        printf("Error: Frontend webapp not built\n");
        #endif
    }
    else if (!strcmp(frontend, "wayland")) {
        #ifdef FRONTEND_WAYLAND
        WaylandStart(gameGetBoard(), size);
        WaylandDestroy();
        #else
        printf("Error: Frontend webapp not built\n");
        #endif
    }
    else if (!strcmp(frontend, "xcb")) {
        #ifdef FRONTEND_XCB
        xcbStart(gameGetBoard(), size);
        xcbDestroy();
        #else
        printf("Error: Frontend xcb not built\n");
        #endif
    }
    else if (!strcmp(frontend, "gdi")) {
        #ifdef FRONTEND_GDI
        gdiStart(gameGetBoard(), size);
        gdiDestroy();
        #else
        printf("Error: Frontend gdi not built\n");
        #endif
    }
    else if (!strcmp(frontend, "d2d")) {
        #ifdef FRONTEND_D2D
        d2dStart(gameGetBoard(), size);
        d2dDestroy();
        #else
        printf("Error: Frontend d2d not built\n");
        #endif
    }
    else if (!strcmp(frontend, "xaw")) {
        #ifdef FRONTEND_XAW
        XawStart(gameGetBoard(), size);
        XawDestroy();
        #else
        printf("Error: Frontend xaw not built\n");
        #endif
    }
    else {
        printf("Error: Frontend not recognised: %s\n", frontend);
        printFrontends();
    }

    gameDestroy();

    return 0;
}
