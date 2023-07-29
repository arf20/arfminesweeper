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

    wayland.c: Wayland frontend

*/

#include "raylib.h"
#include "common.h"

#include "libraylib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

static void
render() {
    ClearBackground(RAYWHITE);
    DrawText("test", 190, 200, 20, LIGHTGRAY);
}

int
raylibStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    InitWindow(wWidth, wHeight, "arfminesweeper");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        render();
        EndDrawing();
    }
}

void
raylibDestroy() {
    CloseWindow();
}
