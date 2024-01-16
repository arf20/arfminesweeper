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

    vgaterm.c: VGA terminal driver

*/

#include "vgacon.h"

#include "port.h"
#include "int32.h"

/* VGA registers */
#define VGA_CTRL_REGISTER   0x3d4
#define VGA_DATA_REGISTER   0x3d5
#define VGA_OFFSET_LOW      0x0f
#define VGA_OFFSET_HIGH     0x0e

/* VGA text mode buffer */
#define VGA_ADDRESS 0xb8000

int vgatxt_rows = 0;
int vgatxt_cols = 0;

unsigned char *buff = (unsigned char*)VGA_ADDRESS;

void memcpy(const char *src, char *dst, int n) {
    for (int i = 0; i < n; i++)
        *(dst + i) = *(src + i);
}

/* ====== Computation operations ====== */
int
vga_offset_row(int off) {
    return off / (2 * vgatxt_cols);
}

int
vga_row_col_offset(int col, int row) {
    return 2 * (row * vgatxt_cols + col);
}

/* ====== Register operations ====== */
/* credit: https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/ */
void
vga_enable_blink() {
    port_byte_in(0x3da);    /* address mode */
    port_byte_out(0x3c0, 0x30); /* set address 0x30 */
    unsigned char am = port_byte_in(0x3c1); /* read AMCR */
    am |= 0x80; /* blink enable bit */
    port_byte_out(0x4c0, am);
}

void
vga_set_cursor_off(int off) {
    off /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(off >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(off & 0xff));
}

int
vga_get_cursor_off() {
    int off;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    off = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    off += port_byte_in(VGA_DATA_REGISTER);
    return off * 2;
}

/* ====== Buffer operations ====== */
void
vga_set_char_c(char c, int off, unsigned char color) {
    buff[off] = c;
    buff[off + 1] = color;
}

void
vga_set_char(char c, int off) {
    vga_set_char_c(c, off, WHITE_ON_BLACK);
}

void
vga_clear() {
    /* Clear buffer */
    int i;
    for (i = 0; i < vgatxt_cols * vgatxt_rows; i++)
        vga_set_char(' ', i * 2);
    /* Reset cursor to top left */
    vga_set_cursor_off(0);
}

int
vga_scroll_line(int off) {
    /* Move buffer */
    memcpy(
        (char *)(vga_row_col_offset(0, 1) + VGA_ADDRESS),
        (char *)(vga_row_col_offset(0, 0) + VGA_ADDRESS),
        vgatxt_cols * (vgatxt_rows - 1) * 2
    );

    /* Clear bottom line */
    for (int col = 0; col < vgatxt_cols; col++)
        vga_set_char(' ', vga_row_col_offset(col, vgatxt_rows - 1));
    
    return off - 2 * vgatxt_cols;
}

void
vga_print_char_c(char c, int off, unsigned char color) {
    /* if off < 0, place at cursor, else use given offset */
    if (off < 0)
        off = vga_get_cursor_off();
    
    /* handle control characters */
    if (c == '\n') off = vga_row_col_offset(0, vga_offset_row(off) + 1);
    else if (c == '\b') off -= 2;
    else {
        vga_set_char_c(c, off, color);
        off += 2;
    }

    /* if after printing, cursor is ouside the screen, do scroll */
    if (off >= vgatxt_rows * vgatxt_cols * 2)
        off = vga_scroll_line(off);
    if (off < 0) off = 0;

    vga_set_cursor_off(off);
}

void
vga_print_char(char c, int off) {
    vga_print_char_c(c, off, WHITE_ON_BLACK);
}

void
vga_print_string_c(const char *str, int off, unsigned char color) {
    if (off < 0)
        off = vga_get_cursor_off();
    else vga_set_cursor_off(off);
    int i = 0;
    while (str[i] != '\0') {
        vga_print_char_c(str[i], -1, color);
        i++;
    }
}

void
vga_print_string(const char *str, int off) {
    vga_print_string_c(str, off, WHITE_ON_BLACK);
}

/* default 3 */
void
vga_set_mode(unsigned char al) {
    regs16_t regs = { 0 };
    regs.al = al;
    int32(0x10, &regs);
}

/* default 3 */
void
vga_set_font(unsigned char al) {
    regs16_t regs = { 0 };
    regs.ah = 0x11;
    regs.al = al;
    int32(0x10, &regs);
}

/* text modes only*/
void
vga_init(unsigned char mode, unsigned char font) {
    vga_set_mode(mode);
    vga_set_font(font);

    switch (mode) {
        case 0x00: /* 320x200, 8x8 */
        case 0x01: /* 320x200, 8x8 */
            vgatxt_cols = 40; vgatxt_rows = 25;
        break;
        case 0x02: /* 720x400, 9x16 */
        case 0x03: /* 720x400, 9x16 */
        case 0x07: /* 720x350, 9x14 */
            vgatxt_cols = 80; vgatxt_rows = 25;
        break;
        case 0x17: /* 640x320, 8x8 */
        case 0x58: /* 640x348, 8x8 */
            vgatxt_cols = 80; vgatxt_rows = 43;
        break;
        case 0x66: /* 640x400, 8x8 */
            vgatxt_cols = 80; vgatxt_rows = 50;
        break;
    }

    if (font == 0x02 || font == 0x012) vgatxt_rows *= 2; /* 8x8 */
    
    vga_enable_blink();
    vga_clear();
}

/* graphics init */
void
vgag_init(unsigned char mode) {
    vga_set_mode(mode);
}
