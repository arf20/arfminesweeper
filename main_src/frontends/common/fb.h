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

*/

#ifndef _FB_H
#define _FB_H

typedef struct {
    char b, g, r, a;
} bgra_t;

void fbRender();
void fbRenderInit(const int *_board, int _size, int _wWidth, int _wHeight,
    void *_fbp, int _sWidth, int _sHeight,
    const unsigned char *_font, int _fontw, int _fonth,
    const unsigned char *_flag, int _flagw, int _flagh,
    const int *_curx, const int *_cury);

#endif /* _FB_H */
