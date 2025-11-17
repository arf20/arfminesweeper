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

#ifndef _FRONTEND_H
#define _FRONTEND_H

#include <stddef.h>

#define FRONTEND_VEC_INIT_SIZE  256

typedef enum { TYPE_STATIC, TYPE_MODULE } frontend_type_t;

typedef int(start_func_t)(const int *, int);
typedef void(destroy_func_t )();

typedef struct {
    frontend_type_t     type;
    const char          *name;
    start_func_t        *start_func;
    destroy_func_t      *destroy_func;
} frontend_t;


extern frontend_t *frontends;
extern size_t frontends_size, frontends_capacity;


void frontend_init();
void frontend_load_static();
void frontend_load_modules();
const frontend_t *frontend_find(const char *name);

#endif /* _FRONTEND_H */

