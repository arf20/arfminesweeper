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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <forms.h>

#include "common.h"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

FL_FORM *form = NULL;
FL_OBJECT **buttons = NULL;

static void
buttonCallback(FL_OBJECT *btn, long btni) {
    printf("Button %d pressed\n", btni);
}

int
xformsStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    char *argv[2] = { "arfminesweeper",  NULL};
    int argc = 1;

    /* Initialize XForms */
    fl_initialize(&argc, argv, 0, 0, 0);

    /* Create form */
    form = fl_bgn_form(FL_UP_BOX, wWidth, wHeight);

    /* Add title */
    FL_OBJECT *title = fl_add_text(FL_NORMAL_TEXT, 1, 1, 120, 20, TXT_TITLE);

    /* Allocate button array */
    buttons = malloc(sizeof(FL_OBJECT*) * size * size);

    /* Add buttons */
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            btni = (size * y) + x;
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            buttons[btni] = fl_add_button(FL_NORMAL_BUTTON,
                cX, cY, CELL_SIZE, CELL_SIZE, "");
            fl_set_object_callback(buttons[btni],
                buttonCallback, btni);
        }
    }

    /* End form, then show it and run the loop */
    fl_end_form();
 
    fl_show_form(form, FL_PLACE_MOUSE, FL_FULLBORDER, TXT_TITLE);
    fl_do_forms();
    return 0;
}

void
xformsDestroy() {
    fl_hide_form(form);
    fl_finish();
}
