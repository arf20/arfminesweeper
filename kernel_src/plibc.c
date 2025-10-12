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
    Also very small subset of a pseudo libc

*/

#include "plibc.h"

#include <stdarg.h>
#include <stddef.h>

#include "console.h"
#include "keyb.h"

/* ======== Internal utils ======== */
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

/* ======== Parsing functions ======== */
char *
itoa(int num, int base) {
    static char buff[128];
    int i = 0;
    int neg = 0;

    if (num == 0) {
        buff[i++] = '0';
        buff[i] = '\0';
        return buff;
    }
 
    if (num < 0 && base == 10) {
        neg = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        buff[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    if (neg)
        buff[i++] = '-';
 
    reverse(buff, i);

    buff[i] = '\0';
 
    return buff;
}


char *
utoa(unsigned int num, int base) {
    static char buff[128];

    int i = 0;
    if (num == 0) {
        buff[i++] = '0';
	    buff[i] = '\0';
	    return buff;
    }

    while (num != 0) {
        unsigned int rem = num % base;
	    buff[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
	    num /= base;
    }
    
    buff[i] = '\0';

    reverse(buff, i);

    return buff;
}

int
atoi(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++)
        res = res * 10 + str[i] - '0';
    return res;
}

long
strtol(const char *nptr, char **endptr, int base) {
    const char *p = nptr, *endp;
    _Bool is_neg = 0, overflow = 0;
    /* Need unsigned so (-LONG_MIN) can fit in these: */
    unsigned long n = 0UL, cutoff;
    int cutlim;
    if (base < 0 || base == 1 || base > 36) {
#ifdef EINVAL /* errno value defined by POSIX */
        errno = EINVAL;
#endif
        return 0L;
    }
    endp = nptr;
    while (*p == ' ')
        p++;
    if (*p == '+') {
        p++;
    } else if (*p == '-') {
        is_neg = 1, p++;
    }
    if (*p == '0') {
        p++;
        /* For strtol(" 0xZ", &endptr, 16), endptr should point to 'x';
         * pointing to ' ' or '0' is non-compliant.
         * (Many implementations do this wrong.) */
        endp = p;
        if (base == 16 && (*p == 'X' || *p == 'x')) {
            p++;
        } else if (base == 2 && (*p == 'B' || *p == 'b')) {
            /* C23 standard supports "0B" and "0b" prefixes. */
            p++;
        } else if (base == 0) {
            if (*p == 'X' || *p == 'x') {
                base = 16, p++;
            } else if (*p == 'B' || *p == 'b') {
                base = 2, p++;
            } else {
                base = 8;
            }
        }
    } else if (base == 0) {
        base = 10;
    }
    cutoff = (is_neg) ? -(LONG_MIN / base) : LONG_MAX / base;
    cutlim = (is_neg) ? -(LONG_MIN % base) : LONG_MAX % base;
    while (1) {
        int c;
        if (*p >= 'A')
            c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
        else if (*p <= '9')
            c = *p - '0';
        else
            break;
        if (c < 0 || c >= base) break;
        endp = ++p;
        if (overflow) {
            /* endptr should go forward and point to the non-digit character
             * (of the given base); required by ANSI standard. */
            if (endptr) continue;
            break;
        }
        if (n > cutoff || (n == cutoff && c > cutlim)) {
            overflow = 1; continue;
        }
        n = n * base + c;
    }
    if (endptr) *endptr = (char *)endp;
    if (overflow) {
        puts("strtol: integer overflow\n");
        return ((is_neg) ? LONG_MIN : LONG_MAX);
    }
    return (long)((is_neg) ? -n : n);
}

/* ======== String functions ======== */
int
strlen(const char *str) {
    if (!str) {
        puts("strlen: imagine this is a segfault\n");
        return 0;
    }
    int i = 0;
    while (str[i]) { i++; }
    return i;
}

int
strcmp(const char *s1, const char *s2) {
    if (!(s1 && s2)) {
        puts("strcmp: imagine this is a segfault\n");
        return 0;
    }
    while (*s1 && *s2) {
        if (*s1 != *s2) return *s1 - *s2;
        s1++; s2++;
    }
    return 0;
}

int
strncmp(const char *s1, const char *s2, size_t n) {
    if (!(s1 && s2)) {
        puts("strncmp: imagine this is a segfault\n");
        return 0;
    }
    if (!n) return 0;

    size_t i = 0;
    while (s1[i] && s2[i] && (i < n)) {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        i++;
    }
    return 0;
}

char *
strchr(const char *str, char c) {
    if (!str) {
        puts("strchr: imagine this is a segfault\n");
        return 0;
    }

    while (*str) {
        if (*str == c) return (char*)str;
        str++;
    }

    return NULL;
}

char *
strstr(const char *haystack, const char *needle) {
    if (!(haystack && needle)) {
        puts("strchr: imagine this is a segfault\n");
        return NULL;
    }

    size_t needlelen = strlen(needle);
    size_t haystacklen = strlen(haystack);
    if (needlelen > haystacklen) return NULL;

    size_t off = 0;
    while (haystack + off < haystack + haystacklen) {
        if (strncmp(haystack + off, needle, strlen(needle)) == 0)
            return (char*)haystack + off;
        off++;
    }

    return NULL;
}

char *
strtok(char *str, const char *delim) {
    static char *istr = NULL;

    if (!delim) {
        puts("strtok: imagine this is a segfault\n");
        return NULL;
    }
    
    /* first call */
    if (str) {
        char *delimocc = strstr(str, delim);
        if (delimocc) { /* delim found */
            *delimocc = '\0';
            istr = delimocc + strlen(delim);
            return str;
        } else { /* not found, strtok sequence end */
            istr = NULL;
            return NULL;
        }
    }
    
    /* strtok called with NULL for the first time */
    if (!istr) {
        puts("strtok: imagine this is a segfault\n");
        return NULL;
    }

    /* secondary strtok call */
    char *delimocc = strstr(istr, delim);
    if (delimocc) { /* delim found */
        *delimocc = '\0';
        str = istr;
        istr = delimocc + strlen(delim);
        return str;
    } else { /* not found, strtok sequence end */
        str = istr;
        istr = NULL;
        return str;
    }
}

unsigned char
toupper(unsigned char c) {
    return c - ('a' - 'A');
}

unsigned char
tolower(unsigned char c) {
    return c + ('a' - 'A');
}

char *
atoupper(char *str) {
    char *ptr = str;
    while (*ptr) {
        if(*ptr >= 'a' && *ptr <= 'z')
            *ptr = toupper(*ptr);
        ptr++;
    }
    return str;
}

char *
atolower(char *str) {
    char *ptr = str;
    while (*ptr) {
        if(*ptr >= 'A' && *ptr <= 'Z')
            *ptr = tolower(*ptr);
        ptr++;
    }
    return str;
}

char *
leftpad(char *str, int n, char c) {
    int len = strlen(str);
    int pad = n - len;
    memmove(str + pad, str, len);
    memset(str, c, pad);
    return str;
}



char *
strcpy(char *dst, const char *src) {
    char *dstcur = dst;
    while (*src)
        *dstcur++ = *src++;
    *dstcur = '\0';
    return dst;
}

char *
strcat(char *dst, const char *src) {
    char *dstcur = dst;
    while (*dstcur)
        dstcur++;
    while (*src)
        *dstcur++ = *src++;
    *dstcur = '\0';
    return dst;
}

/* ======= Memory functions ======= */

void *
memmove(void *dest, const void *src, int n) {
    char *d = (char*)dest; const char *s = (char*)src;
    if (s > d)
        for (int i = 0; i < n; i++)
            d[i] = s[i];
    else
        for (int i = n - 1; i >= 0; i--)
            d[i] = s[i];
    return dest;
}

void *
memset(void *s, char c, size_t n) {
    for (size_t i = 0; i < n; i++)
        ((char*)s)[i] = c;
    return s;
}

void *
memcpy(void *dest, const void *src, int n) {
    char *d = (char*)dest; const char *s = (char*)src;
    for (int i = 0; i < n; i++)
        *(d + i) = *(s + i);
    return dest;
}

/* ======== Standard input and output functions ======== */
char
putchar(char c) {
    con_print_char(c);
    return c;
}

int
puts(const char *s) {
    if (!s) {
        puts("puts: imagine this is a segfault\n"); /* YIKES */
        return 0;
    }
    
    con_print_string(s);
    
    return strlen(s);
}

/* Simple minimal printf implementation
   Partial C99 */
int
kprintf(char *format, ...) {
    char *p;

    va_list arg;
    va_start(arg, format);
    p = format;

    int n = 0;

    char flag = 0;
    int width = 0;

    for (p = format; *p != '\0'; p++) {
        /* Begins specifier */
        if(*p != '%') {
            putchar(*p); continue;
        }
        p++;

        /* Flags */
        if (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#') {
            flag = *p;
            p++;
        }
        else flag = 0;

        /* Width */
        if (*p >= '0' && *p <= '9') {
            width = *p - '0';
            p++;
        }
        else width = 0;

        switch(*p) {
            case 'c': putchar(va_arg(arg, int)); n++; break;
            case 'd': /* fall down */
            case 'i': n += puts(itoa(va_arg(arg, int), 10));
                break;
            case 'o': n += puts(utoa(va_arg(arg, unsigned int), 8));
                break;
            case 's':  {
                char *str = va_arg(arg, char *);
                if (!str) {
                    puts("kprintf: imagine this is a segfault\n");
                    return 0;
                }
                n += puts(str);
             } break;
            case 'u': n += puts(utoa(va_arg(arg, unsigned int), 10));
                break;
            case 'x': n += puts(utoa(va_arg(arg, unsigned int), 16));
                break;
            case 'X': n += puts(leftpad(atoupper(utoa(va_arg(arg, unsigned int), 16)), width, '0'));
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

/* Get character and echo it */
char
getchar() {
    char c = keyb_getc();
    putchar(c);
    return c;
}

char *
getsn(char *buff, size_t n) {
    if (!buff) {
        puts("getsn: imagine this is a segfault\n");
        return 0;
    }

    char c = 1;
    size_t i = 0;
    while (((c = keyb_getc()) != '\n') && (i < n))  {
        if (c == '\b') {
            if (i == 0)
                continue;
            i--;
            puts("\b \b");
            con_swap_buffers();
            continue;
        }
        putchar(c);
        con_swap_buffers();
        buff[i] = c;
        i++;
    }
    buff[i] = '\0';
    putchar('\n');
    con_swap_buffers();
    return buff;
}

