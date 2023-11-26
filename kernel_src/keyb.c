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

    keyb.c: PS/2 keyboard IRQ1 driver

*/

#include "keyb.h"

#include "idt.h"
#include "isr_irq_handler.h"
#include "port.h"
#include "vgaterm.h"

#define UNUSED(x) (void)(x)

void
print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            vga_write_string("ERROR", -1);
            break;
        case 0x1:
            vga_write_string("ESC", -1);
            break;
        case 0x2:
            vga_write_string("1", -1);
            break;
        case 0x3:
            vga_write_string("2", -1);
            break;
        case 0x4:
            vga_write_string("3", -1);
            break;
        case 0x5:
            vga_write_string("4", -1);
            break;
        case 0x6:
            vga_write_string("5", -1);
            break;
        case 0x7:
            vga_write_string("6", -1);
            break;
        case 0x8:
            vga_write_string("7", -1);
            break;
        case 0x9:
            vga_write_string("8", -1);
            break;
        case 0x0A:
            vga_write_string("9", -1);
            break;
        case 0x0B:
            vga_write_string("0", -1);
            break;
        case 0x0C:
            vga_write_string("-", -1);
            break;
        case 0x0D:
            vga_write_string("+", -1);
            break;
        case 0x0E:
            vga_write_string("Backspace", -1);
            break;
        case 0x0F:
            vga_write_string("Tab", -1);
            break;
        case 0x10:
            vga_write_string("Q", -1);
            break;
        case 0x11:
            vga_write_string("W", -1);
            break;
        case 0x12:
            vga_write_string("E", -1);
            break;
        case 0x13:
            vga_write_string("R", -1);
            break;
        case 0x14:
            vga_write_string("T", -1);
            break;
        case 0x15:
            vga_write_string("Y", -1);
            break;
        case 0x16:
            vga_write_string("U", -1);
            break;
        case 0x17:
            vga_write_string("I", -1);
            break;
        case 0x18:
            vga_write_string("O", -1);
            break;
        case 0x19:
            vga_write_string("P", -1);
            break;
        case 0x1A:
            vga_write_string("[", -1);
            break;
        case 0x1B:
            vga_write_string("]", -1);
            break;
        case 0x1C:
            vga_write_string("ENTER", -1);
            break;
        case 0x1D:
            vga_write_string("LCtrl", -1);
            break;
        case 0x1E:
            vga_write_string("A", -1);
            break;
        case 0x1F:
            vga_write_string("S", -1);
            break;
        case 0x20:
            vga_write_string("D", -1);
            break;
        case 0x21:
            vga_write_string("F", -1);
            break;
        case 0x22:
            vga_write_string("G", -1);
            break;
        case 0x23:
            vga_write_string("H", -1);
            break;
        case 0x24:
            vga_write_string("J", -1);
            break;
        case 0x25:
            vga_write_string("K", -1);
            break;
        case 0x26:
            vga_write_string("L", -1);
            break;
        case 0x27:
            vga_write_string(";", -1);
            break;
        case 0x28:
            vga_write_string("'", -1);
            break;
        case 0x29:
            vga_write_string("`", -1);
            break;
        case 0x2A:
            vga_write_string("LShift", -1);
            break;
        case 0x2B:
            vga_write_string("\\", -1);
            break;
        case 0x2C:
            vga_write_string("Z", -1);
            break;
        case 0x2D:
            vga_write_string("X", -1);
            break;
        case 0x2E:
            vga_write_string("C", -1);
            break;
        case 0x2F:
            vga_write_string("V", -1);
            break;
        case 0x30:
            vga_write_string("B", -1);
            break;
        case 0x31:
            vga_write_string("N", -1);
            break;
        case 0x32:
            vga_write_string("M", -1);
            break;
        case 0x33:
            vga_write_string(",", -1);
            break;
        case 0x34:
            vga_write_string(".", -1);
            break;
        case 0x35:
            vga_write_string("/", -1);
            break;
        case 0x36:
            vga_write_string("Rshift", -1);
            break;
        case 0x37:
            vga_write_string("Keypad *", -1);
            break;
        case 0x38:
            vga_write_string("LAlt", -1);
            break;
        case 0x39:
            vga_write_string("Space", -1);
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                vga_write_string("Unknown key down", -1);
            } else if (scancode <= 0x39 + 0x80) {
                vga_write_string("key up ", -1);
                print_letter(scancode - 0x80);
            } else vga_write_string("Unknown key up", -1);
            break;
    }
}



void
keyboard_callback(registers_t *regs) {
    UNUSED(regs);
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    vga_write_string("\n", -1);
}

void
keyb_init() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}
