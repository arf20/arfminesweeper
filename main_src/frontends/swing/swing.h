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

#ifndef _SWING_H
#define _SWING_H

#ifdef _MODULE_
#define swing_start   start
#define swing_destroy destroy
#define swing_name    name
#endif

int swing_start(const int *lboard, int lsize);
void swing_destroy();
const char *swing_name();

const int *swing_getboard();
int swing_getsize();

#endif /* _SWING_H */

