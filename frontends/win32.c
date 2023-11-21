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

    win32.c: Win32 frontend

*/

#include <stdio.h>

#include <windows.h>

#include "common.h"
#include "win32.h"
#include "../game.h"


static const int* board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

HWND mainhWnd = NULL;
HWND titleLabel = NULL;
HWND flagsLeftLabel = NULL;
HWND *buttons = NULL;
HWND *labels = NULL;


void
updateButtons() {
    static char buff[256];

    /* Show flags left */
    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    SendMessage(flagsLeftLabel, WM_SETTEXT, 0, buff);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            /* Variables stuff */
            int btni = (x * size) + y;

            /* If clear, hide the button, count surrounding cells and print
                n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                ShowWindow(buttons[btni], SW_HIDE);
                ShowWindow(labels[btni], SW_SHOW);
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                
            }
            /* Otherwise just a tile */
            else {
                /* Clear flag if applicable */
                ShowWindow(buttons[btni], SW_SHOW);
                ShowWindow(labels[btni], SW_HIDE);
            }
        }
    }

    RedrawWindow(mainhWnd, NULL, NULL, RDW_ALLCHILDREN);
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: DestroyWindow(hwnd); break;
        case WM_DESTROY: PostQuitMessage(0); break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        } break;
        case WM_COMMAND: {
            int btni = -1;
            for (int i = 0; i < size * size; i++) if (buttons[i] == lParam) btni = i;
            
            switch (HIWORD(wParam)) {
                case BN_CLICKED: {
                    gameClearCell(btni / size, btni % size);
                    printf("Button %d clicked\n", btni);
                } break;

            }
            updateButtons();
        } break;
        default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
}

int
Win32Start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* Win32 stuff */
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS winclass = { 0 };

    const char className[] = TXT_TITLE;

    winclass.lpfnWndProc = WindowProc;
    winclass.hInstance = hInstance;
    winclass.lpszClassName = className;
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&winclass);

    /* Create window */
    RECT crect = { 0, 0, wWidth, wHeight};
    AdjustWindowRectEx(&crect, WS_OVERLAPPEDWINDOW, 0, 0);

    mainhWnd = CreateWindowEx(0, className, TXT_TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, crect.right - crect.left, crect.bottom - crect.top,
        NULL, 0, hInstance, NULL);

    titleLabel = CreateWindowEx(0, "STATIC", TXT_TITLE, WS_CHILD | WS_VISIBLE,
        5, 2, 110, 20, mainhWnd, NULL, hInstance, NULL);

    flagsLeftLabel = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE,
        wWidth - 25, 25, 20, 20, mainhWnd, NULL, hInstance, NULL);

    /* Allocate object arrays */
    buttons = malloc(sizeof(HWND) * size * size);
    labels = malloc(sizeof(HWND) * size * size);
    
    /* Add UI matrix */
    char buff[256];
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            /* Variables stuff */
            btni = (size * y) + x;
            int cX = W_MARGIN + (x * (CELL_SIZE + CELL_MARGIN));
            int cY = HEADER_HEIGHT + (y * (CELL_SIZE + CELL_MARGIN));

            /* Button */
            buttons[btni] = CreateWindowEx(0, "BUTTON", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                cX, cY, CELL_SIZE, CELL_SIZE, mainhWnd, (HMENU)btni, hInstance, NULL);

            /* Label */
            int n = gameGetSurroundingMines(x, y);
            if (n) snprintf(buff, 256, "%d", n);
            else *buff = 0;
            labels[btni] = CreateWindowEx(0, "STATIC", buff, WS_VISIBLE | WS_CHILD,
                cX, cY, CELL_SIZE, CELL_SIZE, mainhWnd, NULL, hInstance, NULL);
        }
    }

    updateButtons();

    ShowWindow(mainhWnd, SW_SHOWDEFAULT);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}

void
Win32Destroy() {
    
}
