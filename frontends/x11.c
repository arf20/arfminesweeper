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

  console.c: X11 Xlib frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>

#include "x11.h"

Display *d = NULL;
Window w = 0;

int
X11Start(const int *lboard, int size) {
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }
    int s = DefaultScreen(d);

    w = XCreateSimpleWindow(d, XRootWindow(d, s), 50, 50, 600, 600, 0, BlackPixel(d, s), BlackPixel(d, s));

    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    XEvent e;
    while (1) {
        XNextEvent(d, &e);
        if (e.type == Expose) {
            
        }
        if (e.type == KeyPress) {
            
        }
   }
}

void
X11Destroy() {
    XDestroyWindow(d, w);
    XCloseDisplay(d);
}
