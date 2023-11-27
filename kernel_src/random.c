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

    random.c: simple pseudorandom number generator

*/

#include "random.h"

#include <stdint.h>
 
static uint32_t next = 1;
 
int
prng_rand(void) {
    next = next * 1103515245 + 12345;
    return (uint32_t)(next / 65536) % 32768;
}
 
void
prng_srand(uint32_t seed) {
    next = seed;
}
