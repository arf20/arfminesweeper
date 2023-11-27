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

    alloc.c: basic watermark allocator

*/

#include "alloc.h"

static void *heap_bottom, *heap_top, *freebase;


void
alloc_init(void *bottom, void *top) {
    heap_bottom = bottom;
    heap_top = top;
    freebase = heap_bottom;
}

void *
kmalloc(size_t size) {
    if (freebase + size > heap_top) return NULL;
    void *objp = freebase;
    freebase += size;
    return objp;
}

void
kfree(const void *objp) {
    /* yikes */
}
