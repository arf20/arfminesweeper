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

    convga.c: VGA console driver

*/

#include "convga.h"

#include "port.h"
#include "int32.h"

#include "plibc.h"

/* VGA registers */
#define VGA_CTRL_REGISTER   0x3d4
#define VGA_DATA_REGISTER   0x3d5
#define VGA_OFFSET_LOW      0x0f
#define VGA_OFFSET_HIGH     0x0e

/* VGA text mode buffer */
#define VGA_ADDRESS 0xb8000

static unsigned int con_height = 0, con_width = 0;
static unsigned char color = 0x0f;

static unsigned char *vgabuff = (unsigned char*)VGA_ADDRESS;

/* text mode colors */
static const unsigned char color_map[] = {
    /* VGA_WHITE_ON_BLACK       */ 0x0f,
    /* VGA_WHITE_ON_BLACK_BLINK */ 0x8f,
    /* VGA_BLACK_ON_BLACK       */ 0x00,
    /* VGA_RED_ON_BLACK         */ 0x0c,
    /* VGA_DRED_ON_BLACK        */ 0x04,
    /* VGA_GREEN_ON_BLACK       */ 0x0a,
    /* VGA_DGREEN_ON_BLACK      */ 0x02,
    /* VGA_BLUE_ON_BLACK        */ 0x09,
    /* VGA_DBLUE_ON_BLACK       */ 0x01,
    /* VGA_CYAN_ON_BLACK        */ 0x0b,
    /* VGA_DCYAN_ON_BLACK       */ 0x03,
    /* VGA_DGREY_ON_BLACK       */ 0x08
};


/* ====== Computation operations ====== */
int
convga_xy_offset(int x, int y) {
    return 2 * (y * con_width + x);
}

/* ====== Register operations ====== */
/* credit: https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/ */
void
convga_enable_blink() {
    port_byte_in(0x3da);    /* address mode */
    port_byte_out(0x3c0, 0x30); /* set address 0x30 */
    unsigned char am = port_byte_in(0x3c1); /* read AMCR */
    am |= 0x80; /* blink enable bit */
    port_byte_out(0x4c0, am);
}

int
convga_get_cursor_off() {
    int off;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    off = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    off += port_byte_in(VGA_DATA_REGISTER);
    return off * 2;
}

/* ====== Interface operations ====== */
void
convga_clear() {
    /* Clear buffer */
    for (int i = 0; i < 2 * con_width * con_height; i++)
        vgabuff[i] = 0x00;
    /* Reset cursor to top left */
    convga_set_cursor(0, 0);
}

void
convga_set_color(int color_index){
    color = color_map[color_index] & 0xff;
}

void
convga_set_char(char c, int x, int y) {
    int off = convga_xy_offset(x, y);
    vgabuff[off] = c;
    vgabuff[off + 1] = color;
}

void
convga_scroll_line() {
    /* Move buffer */
    memmove(
        (char*)(convga_xy_offset(0, 0) + VGA_ADDRESS),
        (char*)(convga_xy_offset(0, 1) + VGA_ADDRESS),
        con_width * (con_height - 1) * 2
    );

    /* Clear bottom line */
    for (int x = 0; x < con_width; x++)
        convga_set_char(' ', x, con_height - 1);
}

void
convga_set_cursor(int x, int y) {
    unsigned short off = (y * con_width) + x;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)((off >> 8) & 0xff));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(off & 0xff));
}



/* default 3 */
void
convga_set_mode(unsigned char al) {
    regs16_t regs = { 0 };
    regs.al = al;
    int32(0x10, &regs);
}

/* default 3 */
void
convga_set_font(unsigned char al) {
    regs16_t regs = { 0 };
    regs.ah = 0x11;
    regs.al = al;
    int32(0x10, &regs);
}

console_interface_t convga_con = {
    convga_clear,
    convga_set_color,
    convga_set_char,
    convga_scroll_line,
    convga_set_cursor,
    0, 0
};

/* text modes only*/
const console_interface_t *
convga_init(unsigned char mode, unsigned char font) {
    convga_set_mode(mode);
    convga_set_font(font);

    switch (mode) {
        case 0x00: /* 320x200, 8x8 */
        case 0x01: /* 320x200, 8x8 */
            con_width = 40; con_height = 25;
        break;
        case 0x02: /* 720x400, 9x16 */
        case 0x03: /* 720x400, 9x16 */
        case 0x07: /* 720x350, 9x14 */
            con_width = 80; con_height = 25;
        break;
        case 0x17: /* 640x320, 8x8 */
        case 0x58: /* 640x348, 8x8 */
            con_width = 80; con_height = 43;
        break;
        case 0x66: /* 640x400, 8x8 */
            con_width = 80; con_height = 50;
        break;
    }

    if (font == 0x02 || font == 0x012) con_height *= 2; /* 8x8 */
    
    convga_enable_blink();
    convga_clear();

    convga_con.width = con_width;
    convga_con.height = con_height;
    return &convga_con;
}

