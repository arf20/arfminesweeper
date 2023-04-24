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

  fbdev.c: fbdev frontend

*/

#include <Xm/Xm.h>
#include <Xm/Label.h>

#include "common.h"

static const int *board = NULL;
static int size = 0;

int
motifStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    char *argv[] = { TXT_TITLE, 0 };
    int argc = 1;

    Widget root, msg;
    Arg al[10];
    int ai;

    root = XtInitialize(argv[0], "", NULL, 0, &argc, argv);

    XtSetArg(al[ai], XmNlabelString,
        XmStringCreate(TXT_TITLE, XmSTRING_DEFAULT_CHARSET)); ai++;
    msg = XtCreateManagedWidget("title", xmLabelWidgetClass, root, al, ai);

    XtRealizeWidget(root);
    XtMainLoop();
}

void
motifDestroy() {
    
}

