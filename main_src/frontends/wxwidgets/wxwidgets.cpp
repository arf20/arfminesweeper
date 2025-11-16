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

static wxColor C_WHITE   (0xff, 0xff, 0xff);
static wxColor C_BLACK   (0x00, 0x00, 0x00);
static wxColor C_BLUE    (0x00, 0x00, 0xff);
static wxColor C_GREEN   (0x00, 0xff, 0x00);
static wxColor C_RED     (0xff, 0x00, 0x00);
static wxColor C_DARKBLUE(0x00, 0x00, 0x8b);
static wxColor C_DARKRED (0x8b, 0x00, 0x00);
static wxColor C_DARKCYAN(0x00, 0x8b, 0x8b);
static wxColor C_DARKGREY(0xa9, 0xa9, 0xa9);

class BoardFrame : public wxFrame {
public:
    BoardFrame();
protected:
    void onLeftButton(wxMouseEvent& event);
    void onRightButton(wxMouseEvent& event);
private:
    void updateButtons();
    wxStaticText *title = NULL, *flagsLeft = NULL;
    wxBitmap *flag = NULL;
    wxButton **buttonsflag = NULL;
    wxButton **buttons = NULL;
    wxStaticText **numbers = NULL;
    wxWindowID firstId = 0;
};

BoardFrame::BoardFrame() : wxFrame(nullptr, wxID_ANY, TXT_TITLE, wxDefaultPosition,
    wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    SetClientSize(wWidth, wHeight);

    flag = new wxBitmap(FLAG_PNG_PATH, wxBITMAP_TYPE_PNG);

    title = new wxStaticText(this, wxID_ANY, TXT_TITLE, wxPoint(5, 15), wxDefaultSize);
    flagsLeft = new wxStaticText(this, wxID_ANY, "", wxPoint(wWidth - 25, 35), wxDefaultSize);

    buttons = (wxButton**)malloc(sizeof(wxButton*) * size * size);
    buttonsflag = (wxButton**)malloc(sizeof(wxButton*) * size * size);
    firstId = wxIdManager::ReserveId(size * size);
    
    numbers = (wxStaticText**)malloc(sizeof(wxStaticText*) * size * size);

    static char buff[256];
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            btni = (size * y) + x;
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));
            
            buttons[btni] = new wxButton(this, firstId + btni, "",
                wxPoint(cX, cY), wxSize(CELL_SIZE+2, CELL_SIZE+2), 0/*wxNO_BORDER*/);
            buttons[btni]->Bind(wxEVT_LEFT_UP, &BoardFrame::onLeftButton, this);
            buttons[btni]->Bind(wxEVT_RIGHT_UP, &BoardFrame::onRightButton, this);


            buttonsflag[btni] = new wxButton(this, firstId + btni, "",
                wxPoint(cX, cY), wxSize(CELL_SIZE+2, CELL_SIZE+2), 0/*wxNO_BORDER*/);
            buttonsflag[btni]->Bind(wxEVT_LEFT_UP, &BoardFrame::onLeftButton, this);
            buttonsflag[btni]->Bind(wxEVT_RIGHT_UP, &BoardFrame::onRightButton, this);
            buttonsflag[btni]->SetBitmap(*flag);
            buttonsflag[btni]->Hide();

            int n = gameGetSurroundingMines(x, y);
            snprintf(buff, 256, "%d", n);
            numbers[btni] = new wxStaticText(this, wxID_ANY, buff,
                wxPoint(cX, cY), wxSize(CELL_SIZE+2, CELL_SIZE+2), 0);
            switch (n) {
                case 1: numbers[btni]->SetForegroundColour(C_BLUE); break;
                case 2: numbers[btni]->SetForegroundColour(C_GREEN); break;
                case 3: numbers[btni]->SetForegroundColour(C_RED); break;
                case 4: numbers[btni]->SetForegroundColour(C_DARKCYAN); break;
                case 5: numbers[btni]->SetForegroundColour(C_DARKRED); break;
                case 6: numbers[btni]->SetForegroundColour(C_DARKCYAN); break;
                case 7: numbers[btni]->SetForegroundColour(C_BLACK); break;
                case 8: numbers[btni]->SetForegroundColour(C_DARKGREY); break;
            }
            numbers[btni]->Hide();
        }
    }

    updateButtons();
}

void BoardFrame::onLeftButton(wxMouseEvent& event) {
    int btni = event.GetId() - firstId;
    gameClearCell(btni % size, btni / size);
    updateButtons();
}

void BoardFrame::onRightButton(wxMouseEvent& event) {
    int btni = event.GetId() - firstId;
    gameFlagCell(btni % size, btni / size);
    updateButtons();
}

void BoardFrame::updateButtons() {
    flagsLeft->SetLabel(std::to_string(gameGetFlagsLeft()).c_str());

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int btni = (size * y) + x;

            if (CHECK_CLEAR(BOARDXY(x, y))) {
                buttons[btni]->Hide();
                if (gameGetSurroundingMines(x, y)) {
                    numbers[btni]->Show();
                }
            }
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                /* show flag */
                buttons[btni]->Hide();
                buttonsflag[btni]->Show();
            }
            else {
                /* normal */
                buttons[btni]->Show();
                buttonsflag[btni]->Hide();
            }
        }
    }

    /* Check state */
    switch (gameGetState()) {
        case STATE_LOST: {
            wxMessageBox(TXT_LOST, TXT_TITLE, wxICON_ERROR);
            Close();
        } break;
        case STATE_WON: {
            wxMessageBox(TXT_WON, TXT_TITLE, wxICON_INFORMATION);
            Close();
        } break;
    }
}

class MineApp : public wxApp
{
public:
    bool OnInit() override {
        BoardFrame *frame = new BoardFrame();
        SetTopWindow(frame);
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
    return 0;
}



void
wxDestroy() {

}

