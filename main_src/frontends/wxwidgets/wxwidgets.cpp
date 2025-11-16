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

    wxwidgets.cpp: wxWidgets frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <common/frontconf.h>

extern "C" {
#include <common/game.h>
}

#include <wx/wx.h>

#include "wxwidgets.hpp"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

class boardFrame : public wxFrame {
public:
    boardFrame() : wxFrame(nullptr, wxID_ANY, TXT_TITLE, wxDefaultPosition,
        wxSize(wWidth, wHeight), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
    {

    }
 
private:
};

class MineApp : public wxApp
{
public:
    bool OnInit() override {
        boardFrame *frame = new boardFrame();
        frame->Show();
        return true;
    }
};


int
wxStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);
    
/*
    static char buff[256];
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            btni = (size * y) + x;
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));

            
        }
    }*/

    wxInitializer();

    MineApp *app = new MineApp();

    wxApp::SetInstance(app);
    char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;
    wxEntryStart(argc, argv);
    app->CallOnInit();
    app->OnRun();
    app->OnExit();
    wxEntryCleanup();
}

void
wxDestroy() {

}

