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

    fltk.cpp: FLTK frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/Fl_PNG_Image.H>

#include <common/frontconf.h>

extern "C" {
#include <common/game.h>
}

#include "fltk.hpp"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

Fl_Window *window = NULL;

static Fl_Button  **buttons;
static Fl_Box     **numbers;

static Fl_PNG_Image *flagimg;

static Fl_Color C_WHITE    = fl_rgb_color(0xff, 0xff, 0xff);
static Fl_Color C_BLACK    = fl_rgb_color(0x00, 0x00, 0x00);
static Fl_Color C_BLUE     = fl_rgb_color(0x00, 0x00, 0xff);
static Fl_Color C_GREEN    = fl_rgb_color(0x00, 0xff, 0x00);
static Fl_Color C_RED      = fl_rgb_color(0xff, 0x00, 0x00);
static Fl_Color C_DARKBLUE = fl_rgb_color(0x00, 0x00, 0x8b);
static Fl_Color C_DARKRED  = fl_rgb_color(0x8b, 0x00, 0x00);
static Fl_Color C_DARKCYAN = fl_rgb_color(0x00, 0x8b, 0x8b);
static Fl_Color C_DARKGREY = fl_rgb_color(0xa9, 0xa9, 0xa9);

static void
updateButtons() {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int btni = (size * y) + x;

            if (CHECK_CLEAR(BOARDXY(x, y))) {
                buttons[btni]->hide();
                if (gameGetSurroundingMines(x, y)) {
                    numbers[btni]->show();
                }
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                /* show flag */
                buttons[btni]->image(flagimg);
            }
            else {
                /* normal */
                buttons[btni]->image(NULL);
            }
        }
    }

    /* Check state */
    switch (gameGetState()) {
        case STATE_LOST: {
            fl_message(TXT_LOST);
            window->hide();
        } break;
        case STATE_WON: {
            fl_message(TXT_WON);
            window->hide();
        } break;
    }
}

void buttonCallback(Fl_Widget *w, void *data) {
    int btni = (long)data;

    switch (Fl::event_button()) {
        case FL_LEFT_MOUSE: {
            gameClearCell(btni % size, btni / size);
        } break;
        case FL_RIGHT_MOUSE: {
            gameFlagCell(btni % size, btni / size);
        } break;
    }

    updateButtons();
}

int
FLTKStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);
    

    window = new Fl_Window(wWidth, wHeight);
    Fl_Box *title = new Fl_Box(60, 15, 0, 0, TXT_TITLE);
    title->box(FL_UP_BOX);
    title->labelsize(15);

    Fl::visual(FL_RGB);

    Fl_Box *flagsLeftLabel = new Fl_Box(wWidth - 20, 35, 0, 0, "10");
    flagsLeftLabel->labelsize(15);

    flagimg = new Fl_PNG_Image(FLAG_PNG_PATH);
    

    buttons = (Fl_Button**)malloc(sizeof(Fl_Button*) * size * size);
    numbers = (Fl_Box**)malloc(sizeof(Fl_Box*) * size * size);


    static char buff[256];
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            btni = (size * y) + x;
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));

            buttons[btni] = new Fl_Button(cX, cY, CELL_SIZE, CELL_SIZE, "");
            /* use address as data */
            buttons[btni]->callback(buttonCallback, (void*)btni);
            buttons[btni]->when(FL_WHEN_RELEASE);

            int n = gameGetSurroundingMines(x, y);
            snprintf(buff, 256, "%d", n);
            numbers[btni] = new Fl_Box(cX, cY, CELL_SIZE, CELL_SIZE, strdup(buff));
            numbers[btni]->clear_visible();
            
            switch (n) {
                case 1: numbers[btni]->labelcolor(C_BLUE); break;
                case 2: numbers[btni]->labelcolor(C_GREEN); break;
                case 3: numbers[btni]->labelcolor(C_RED); break;
                case 4: numbers[btni]->labelcolor(C_DARKCYAN); break;
                case 5: numbers[btni]->labelcolor(C_DARKRED); break;
                case 6: numbers[btni]->labelcolor(C_DARKCYAN); break;
                case 7: numbers[btni]->labelcolor(C_BLACK); break;
                case 8: numbers[btni]->labelcolor(C_DARKGREY); break;
            }
        }
    }

    window->end();
    char *argv[2] = { TXT_TITLE, NULL };
    window->show(1, argv);

    Fl::run();
    return 0;
}

void
FLTKDestroy() {

}

