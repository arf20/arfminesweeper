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

    frontend.c: Frontend interface

*/

#include "frontend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <dlfcn.h>

#include <common/frontconf.h>

#include <console/console.h>
#include <fbdev/fbdev.h>
#include <xlib/xlib.h>
#include <motif/motif.h>
#include <xforms/xforms.h>
#include <gtk3/gtk3.h>
#include <qt5/qt5.hpp>
#include <sdl2/sdl2.h>
#include <sdl1/sdl1.h>
#include <gl11/gl11.h>
#include <gl33/gl33.h>
#include <vk/vk.h>
#include <win32/win32.h>
#include <httpd/httpd.h>
#include <wayland/wayland.h>
#include <xcb/xcb.h>
#include <gdi/gdi.h>
#include <d2d/d2d.h>
#include <xaw/xaw.h>
#include <drmfb/drmfb.h>
#include <vt100/vt100.h>
#include <ansi/ansi.h>
#include <java/java.h>
#include <glx/glx.h>
#include <curses/curses.h>
#include <fltk/fltk.hpp>
#include <wxwidgets/wxwidgets.hpp>

typedef const char *(name_func_t)();

frontend_t *frontends = NULL;
size_t frontends_size = 0, frontends_capacity = 0;


void
frontend_init() {
    frontends = malloc(sizeof(frontend_t) * FRONTEND_VEC_INIT_SIZE);
    frontends_capacity = FRONTEND_VEC_INIT_SIZE;
    frontends_size = 0;
}

static void
frontend_insert(const frontend_t *f) {
    if (frontends_size + 1 > frontends_capacity)
        frontends = realloc(frontends, 2 * frontends_capacity);
    frontends[frontends_size++] = *f;
}

void
frontend_load_static() {
    #ifdef FRONTEND_CONSOLE
    frontend_insert(&(frontend_t){ TYPE_STATIC, console_name(), console_start, console_destroy });
    #endif
    #ifdef FRONTEND_FBDEV
    frontend_insert(&(frontend_t){ TYPE_STATIC, fbdev_name(), fbdev_start, fbdev_destroy });
    #endif
    #ifdef FRONTEND_XLIB
    frontend_insert(&(frontend_t){ TYPE_STATIC, xlib_name(), xlib_start, xlib_destroy });
    #endif
    #ifdef FRONTEND_MOTIF
    frontend_insert(&(frontend_t){ TYPE_STATIC, motif_name(), motif_start, motif_destroy });
    #endif
    #ifdef FRONTEND_XFORMS
    frontend_insert(&(frontend_t){ TYPE_STATIC, xforms_name(), xforms_start, xforms_destroy });
    #endif
    #ifdef FRONTEND_GTK3
    frontend_insert(&(frontend_t){ TYPE_STATIC, gtk3_name(), gtk3_start, gtk3_destroy });
    #endif
    #ifdef FRONTEND_QT5
    frontend_insert(&(frontend_t){ TYPE_STATIC, qt5_name(), qt5_start, qt5_destroy });
    #endif
    #ifdef FRONTEND_SDL2
    frontend_insert(&(frontend_t){ TYPE_STATIC, sdl2_name(), sdl2_start, sdl2_destroy });
    #endif
    #ifdef FRONTEND_SDL1
    frontend_insert(&(frontend_t){ TYPE_STATIC, sdl1_name(), sdl1_start, sdl1_destroy });
    #endif
    #ifdef FRONTEND_GL11
    frontend_insert(&(frontend_t){ TYPE_STATIC, gl11_name(), gl11_start, gl11_destroy });
    #endif
    #ifdef FRONTEND_GL33
    frontend_insert(&(frontend_t){ TYPE_STATIC, gl33_name(), gl33_start, gl33_destroy });
    #endif
    #ifdef FRONTEND_VK
    frontend_insert(&(frontend_t){ TYPE_STATIC, vk_name(), vk_start, vk_destroy });
    #endif
    #ifdef FRONTEND_WIN32
    frontend_insert(&(frontend_t){ TYPE_STATIC, win32_name(), win32_start, win32_destroy });
    #endif
    #ifdef FRONTEND_HTTPD
    frontend_insert(&(frontend_t){ TYPE_STATIC, httpd_name(), httpd_start, httpd_destroy });
    #endif
    #ifdef FRONTEND_WAYLAND
    frontend_insert(&(frontend_t){ TYPE_STATIC, wayland_name(), wayland_start, wayland_destroy });
    #endif
    #ifdef FRONTEND_XCB
    frontend_insert(&(frontend_t){ TYPE_STATIC, xcb_name(), xcb_start, xcb_destroy });
    #endif
    #ifdef FRONTEND_GDI
    frontend_insert(&(frontend_t){ TYPE_STATIC, gdi_name(), gdi_start, gdi_destroy });
    #endif
    #ifdef FRONTEND_D2D
    frontend_insert(&(frontend_t){ TYPE_STATIC, d2d_name(), d2d_start, d2d_destroy });
    #endif
    #ifdef FRONTEND_XAW
    frontend_insert(&(frontend_t){ TYPE_STATIC, xaw_name(), xaw_start, xaw_destroy });
    #endif
    #ifdef FRONTEND_DRMFB
    frontend_insert(&(frontend_t){ TYPE_STATIC, drmfb_name(), drmfb_start, drmfb_destroy });
    #endif
    #ifdef FRONTEND_VT100
    frontend_insert(&(frontend_t){ TYPE_STATIC, vt100_name(), vt100_start, vt100_destroy });
    #endif
    #ifdef FRONTEND_ANSI
    frontend_insert(&(frontend_t){ TYPE_STATIC, ansi_name(), ansi_start, ansi_destroy });
    #endif
    #ifdef FRONTEND_JAVA
    frontend_insert(&(frontend_t){ TYPE_STATIC, java_name(), java_start, java_destroy });
    #endif
    #ifdef FRONTEND_GLX
    frontend_insert(&(frontend_t){ TYPE_STATIC, glx_name(), glx_start, glx_destroy });
    #endif
    #ifdef FRONTEND_CURSES
    frontend_insert(&(frontend_t){ TYPE_STATIC, curses_name(), curses_start, curses_destroy });
    #endif
    #ifdef FRONTEND_FLTK
    frontend_insert(&(frontend_t){ TYPE_STATIC, fltk_name(), fltk_start, fltk_destroy });
    #endif
    #ifdef FRONTEND_WXWIDGETS
    frontend_insert(&(frontend_t){ TYPE_STATIC, wxwidgets_name(), wxwidgets_start, wxwidgets_destroy });
    #endif

}

void
frontend_load_modules() {
    struct dirent *entry;
    DIR *dir;
    static char path[256];

    dir = opendir(MODULE_SEARCH_PATH);
    if (!dir) {
        fprintf(stderr, "Error: opening directory %s: %s\n", MODULE_SEARCH_PATH,
            strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        snprintf(path, 256, "%s/%s", MODULE_SEARCH_PATH, entry->d_name);
        void *mod = dlopen(path, RTLD_NOW);
        if (mod == NULL) {
            fprintf(stderr, "Error: loading %s\n", dlerror());
            continue;
        }
        
        name_func_t *name_func = dlsym(mod, "name");
        if (!name_func) {
            fprintf(stderr, "Error: retrieving symbol %s in %s: %s\n", "name",
                path, dlerror());
            continue;
        }

        start_func_t *start_func = dlsym(mod, "start");
        if (!start_func) {
            fprintf(stderr, "Error: retrieving symbol %s in %s: %s\n", "start",
                path, dlerror());
            continue;
        }

        destroy_func_t *destroy_func = dlsym(mod, "destroy");
        if (!start_func) {
            fprintf(stderr, "Error: retrieving symbol %s in %s: %s\n", "destroy",
                path, dlerror());
            continue;
        }

        const char *name = name_func();
        if (!name) {
            fprintf(stderr, "Error: unknown getting module name from %s\n",
                path);
            continue;
        }

        frontend_t frontend = {
            .type = TYPE_MODULE,
            .name = name_func(),
            .start_func = start_func,
            .destroy_func = destroy_func
        };

        frontend_insert(&frontend);
    }

    closedir(dir);
}

const frontend_t *
frontend_find(const char *name) {
    for (int i = 0; i < frontends_size; i++)
        if (strcmp(name, frontends[i].name) == 0)
            return &frontends[i];
    return NULL;
}

