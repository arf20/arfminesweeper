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

#ifndef _INT86_H
#define _INT86_H

#include <stdint.h>

typedef struct {
    union {
        uint32_t edi;
        uint16_t di;
    };
    union {
        uint32_t esi;
        uint16_t si;
    };
    union {
        uint32_t ebp;
        uint16_t bp;
    };
    union { 
        uint32_t esp;
        uint16_t sp;
    };
    uint32_t ebx, edx, ecx, eax;
    uint16_t flags;
    uint16_t es, ds, fs, gs;
} __attribute__ ((packed)) int86regs;

void int86(int inum, int86regs *regs);

#endif /* _INT86_H */
