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

    common.c: common utility functions for kernel

*/

#include "common.h"

#include <stdarg.h>

#include "vgaterm.h"
#include "keyb.h"

void
reverse(char *str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char *
itoan(int num, char* buff, int n, int base) {
    int i = 0;
    int neg = 0;

    if (!n) return 0;

    if (num == 0) {
        buff[i++] = '0';
        buff[i] = '\0';
        return buff;
    }
 
    if (num < 0 && base == 10) {
        neg = 1;
        num = -num;
    }

    while (num != 0 && i < n) {
        int rem = num % base;
        buff[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    if (neg)
        buff[i++] = '-';
 
    buff[i] = '\0'; 
 
    reverse(buff, i);
 
    return buff;
}

int
strlen(const char *str) {
    int i = 0;
    while (str[i]) { i++; }
    return i;
}

char
putchar(char c) {
    vga_print_char(c, -1);
    return c;
}

int
puts(const char *s) {
    int i = 0;
    while (s[i]) {
        putchar(s[i]);
        i++;
    }
    return i;
}

char
getchar() {
    return keyb_getc();
}

int
kprintf(char *format, ...) {
    char *p;
    char intbuff[64];

    va_list arg;
    va_start(arg, format);
    p = format;

    int n = 0;

    for (p = format; *p != '\0'; p++) {
        if(*p != '%') {
            putchar(*p); continue;
        }
        p++;
        switch(*p) {
            case 'c': putchar(va_arg(arg, int)); n++; break;
            case 'd': /* fall down */
            case 'i': n += puts(itoan(va_arg(arg, int), intbuff, 64, 10));
                break;
            case 'o': n += puts(itoan(va_arg(arg, unsigned int), intbuff, 64, 8));
                break;
            case 's': n += puts(va_arg(arg, char *)); break;
            case 'u': n += puts(itoan(va_arg(arg, unsigned int), intbuff, 64, 10));
                break;
            case 'x': n += puts(itoan(va_arg(arg, unsigned int), intbuff, 64, 16));
                break;

            /*case 'f':
            case 'F': */
            /*case 'e': f = va_arg(arg, float); puts(); break;
            case 'E': f = va_arg(arg, float); puts(); break;
            case 'g': f = va_arg(arg, float); puts(); break;
            case 'G': f = va_arg(arg, float); puts(); break;
            case 'F': f = va_arg(arg, float); puts(); break;
            case 'a': f = va_arg(arg, float); puts(); break;
            case 'A': f = va_arg(arg, float); puts(); break;
            case 'p': u = va_arg(arg, unsigned int); puts(uintstr(u, 16)); break;
            case 'n': */
            case '%': putchar('%'); break;
            default: {
                puts("kprintf: unsupported format\n");
            } break;
        }
    }
    va_end(arg);

    return n;
}
