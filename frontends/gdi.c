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

    gdi.c: Win32 GDI frontend

*/

#include <stdio.h>

#include <windows.h>

#include "common.h"
#include "gdi.h"
#include "../game.h"


static const int* board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static HWND mainhWnd = NULL;

static void
render(HDC hdc) {
    HFONT hFont, hOldFont;
    hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
    if (hOldFont = (HFONT)SelectObject(hdc, hFont)) {
        TextOut(hdc, 5, 5, TXT_TITLE, sizeof(TXT_TITLE));
      
        SelectObject(hdc, hOldFont);
    }
}

LRESULT CALLBACK
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE: DestroyWindow(hWnd); break;
    case WM_DESTROY: {
        PostQuitMessage(0);
    } break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
        render(hdc);
        EndPaint(hWnd, &ps);
    } break;
    default: return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
}

int
gdiStart(const int* lboard, int lsize) {
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
    RECT crect = { 0, 0, wWidth, wHeight };
    AdjustWindowRectEx(&crect, WS_OVERLAPPEDWINDOW, 0, 0);

    mainhWnd = CreateWindowEx(0, className, TXT_TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, crect.right - crect.left, crect.bottom - crect.top,
        NULL, 0, hInstance, NULL);
    if (!mainhWnd) {
        printf("Error creating window: %d\n", GetLastError());
    }

    ShowWindow(mainhWnd, SW_SHOW);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}

void
gdiDestroy() {

}
