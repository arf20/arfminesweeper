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


static const int *board = NULL;
static int size = 0;

HWND mainhWnd = NULL;


LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
}



int
Win32Start(const int *lboard, int lsize) {

}

void
Win32Destroy() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    
    WNDCLASS winclass = { 0 };

	const char className[] = TXT_TITLE;

	winclass.lpfnWndProc = WindowProc;
	winclass.hInstance = hInstance;
	winclass.lpszClassName = className;

    RegisterClass(&winclass);

    mainhWnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,                      // Window class
		TXT_TITLE,                      // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		0,          // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

    ShowWindow(mainhWnd, 0);

    MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

