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

    keyb.c: PS/2 keyboard polling driver

*/

#include "keyb.h"

#include "port.h"
#include "plibc.h"

#define UNUSED(x) (void)(x)

const char keymap_en_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',   /* Tab */
  'q', 'w', 'e', 'r', /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,   /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,  /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',   /* 49 */
  'm', ',', '.', '/',   0,    /* Right shift */
  '*',
    0, /* Alt */
  ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
  '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
  '+',
    0, /* 79 - End key*/
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0,   0,   0,
    0, /* F11 Key */
    0, /* F12 Key */
    0, /* All other keys are undefined */
};

/*
    0x60	Read/Write	Data Port
    0x64	Read	    Status Register
    0x64	Write	    Command Register
*/

unsigned char
keyb_poll() {
	while (1) {
		if (port_byte_in(0x64) & 0x01) {
			unsigned char scancode = port_byte_in(0x60);
			if (scancode > 0)
				return scancode;
            else {
                kprintf("keyb_poll: keyb error\n");
                return 0;
            }
		}
	}
}

char 
keyb_scancode_char(unsigned char scancode) {
    if (scancode > 0 && scancode <= 0x7f)
        return keymap_en_us[scancode];
    else return '\0';
}

/* Blocks until valid key is pressed */
char
keyb_getc() {
    unsigned char scancode = 0;
    /* Poll for a keydown event */
    do {
        scancode = keyb_poll();
    } while (!(scancode > 0 && scancode <= 0x7f));

    return keymap_en_us[scancode];
}

