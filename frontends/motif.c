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

    motif.c: Motif frontend

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

#include "common.h"

static const int *board = NULL;
static int size = 0;

int
motifStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    char *argv[] = { TXT_TITLE, 0 };
    int argc = 1;

    XtAppContext app;
    Widget root, title, rowcol, buttons[size * size];
    Arg al[10];
    int ac = 0;

    XtSetLanguageProc(NULL, NULL, NULL);

    root = XtVaAppInitialize(&app, TXT_TITLE, NULL, 0, &argc, argv, NULL, NULL);

    XtSetArg(al[ac], XmNlabelString,
        XmStringCreate(TXT_TITLE, XmSTRING_DEFAULT_CHARSET)); ac++;
    title = XtCreateManagedWidget("title", xmLabelWidgetClass, root, al, ac);

    rowcol = XtVaCreateWidget("rowcol", xmRowColumnWidgetClass, root,
        XmNnumColumns,  size,
        XmNpacking,     XmPACK_COLUMN,
        NULL);

    char butname[64];
    int buti = 0;
    XmString btntxt;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            buti = (y * size) + x;
            snprintf(butname, 64, "button%d", buti);
            btntxt = XmStringCreateLocalized (butname);
            buttons[buti] = XtVaCreateManagedWidget(butname,
                xmPushButtonWidgetClass, rowcol, 
                XmNlabelString, btntxt, NULL);
            //printf("Created %s\n", butname);
        }
    }

    XtManageChild(rowcol);
    XtRealizeWidget(root);
    XtAppMainLoop(app);
}

void
motifDestroy() {
    
}

