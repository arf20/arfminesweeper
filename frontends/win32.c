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

#include <windows.h>

#include "common.h"
#include "win32.h"
#include "../game.h"


static const int* board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

HWND mainhWnd = NULL;
HWND titleLabel = NULL;

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: DestroyWindow(hwnd); break;
        case WM_DESTROY: PostQuitMessage(0); break;
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

    RegisterClass(&winclass);

    mainhWnd = CreateWindowEx(0, className, TXT_TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight,
        NULL, 0, hInstance, NULL);

    titleLabel = CreateWindowEx(0, "STATIC", TXT_TITLE, WS_CHILD | WS_VISIBLE,
        5, 15, 110, 20, mainhWnd, NULL, hInstance, NULL);

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
