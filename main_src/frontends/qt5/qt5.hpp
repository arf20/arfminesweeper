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

#ifndef _QT5_H
#define _QT5_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MODULE_
#define qt5_start   start
#define qt5_destroy destroy
#define qt5_name    name
#endif

int qt5_start(const int *lboard, int lsize);
void qt5_destroy();
const char *qt5_name();

#ifdef __cplusplus
}
#endif

#endif /* _QT5_H */
