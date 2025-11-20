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

#ifndef _AWT_H
#define _AWT_H

#ifdef _MODULE_
#define awt_start   start
#define awt_destroy destroy
#define awt_name    name
#endif

int awt_start(const int *lboard, int lsize);
void awt_destroy();
const char *awt_name();

const int *awt_getboard();
int awt_getsize();

#endif /* _AWT_H */

