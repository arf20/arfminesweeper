/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2025 arf20 (Ángel Ruiz Fernandez)

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

#ifndef _SDL3_H
#define _SDL3_H

#ifdef _MODULE_
#define sdl3_start   start
#define sdl3_destroy destroy
#define sdl3_name    name
#endif

int sdl3_start(const int *lboard, int lsize);
void sdl3_destroy();
const char *sdl3_name();

#endif /* _SDL3_H */
