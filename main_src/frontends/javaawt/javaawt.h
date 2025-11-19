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

#ifndef _JAVAAWT_H
#define _JAVAAWT_H

#ifdef _MODULE_
#define javaawt_start   start
#define javaawt_destroy destroy
#define javaawt_name    name
#endif

int javaawt_start(const int *lboard, int lsize);
void javaawt_destroy();
const char *javaawt_name();

const int *javaawt_getboard();
int javaawt_getsize();

#endif /* _JAVAAWT_H */

