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

    xaw.c: X11 Xaw frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
/*#include <X11/Xaw/AllWidgets.h>*/
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>

#include <common/frontconf.h>

#include "xaw.h"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

int
xaw_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    XtSetLanguageProc(NULL, NULL, NULL);



    Arg arglist[10];

    /*XtAppContext app_context_return;*/
    char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;
    /*Widget mw = XtAppInitialize(app_context_return, "arfminesweeper",
        arglist, XtNumber(arglist), &argc, argv, NULL, NULL, 0);*/

    Widget mw = XtInitialize(TXT_TITLE, TXT_TITLE, NULL,
	    0, &argc, argv);


    Widget box = XtCreateManagedWidget("box", boxWidgetClass,
		 mw, NULL, 0);

    int n = 0;
    XtSetArg(arglist[n],XtNorientation,XtorientVertical); n++;
    XtSetArg(arglist[n],XtNvSpace,10); n++;
    XtSetValues(box, arglist, n);

    n = 0;
    XtSetArg(arglist[n], XtNheight, wHeight); n++;
    XtSetArg(arglist[n], XtNwidth, wWidth); n++;

    Widget title = XtCreateManagedWidget(TXT_TITLE,
		 labelWidgetClass, box, NULL, 0);

    char **btnnames = malloc(sizeof(char*) * size * size);
    for (int i = 0; i < size * size; i++) {
        btnnames[i] = malloc(15);
        snprintf(btnnames[i], 15, "button%d", i);
    }

    Widget *buttons = malloc(sizeof(Widget) * size * size);
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            buttons[(size * y) + x] = XtCreateManagedWidget("",
		 commandWidgetClass, box, NULL, 0);

    /*XtSetValues(title, arglist, n);*/

    XtRealizeWidget(mw);

    XtMainLoop();
}

void
xaw_destroy() {

}

const char *
xaw_name() {
    return "xaw";
}

