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

    idt.c: configure IDT

*/

#include <stdint.h>

#include "idt.h"

#include "port.h"
#include "vgaterm.h"

#define LOSHORT(address) (uint16_t)((address) & 0xFFFF)
#define HISHORT(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define IDT_SIZE        256

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

typedef struct {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

idt_gate_t idt[IDT_SIZE];
idt_register_t idt_reg;

extern uint32_t isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9,
    isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19,
    isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29,
    isr30, isr31,
    irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9,
    irq10, irq11, irq12, irq13, irq14, irq15;


void
idt_set_gate(int n, uint32_t handler) {
    idt[n].low_offset = LOSHORT(handler);
    idt[n].selector = 0x08; /* see GDT */
    idt[n].always0 = 0;
    /* 0x8E = 1  00 0 1  110
              P DPL 0 D Type */
    idt[n].flags = 0x8E;
    idt[n].high_offset = HISHORT(handler);
}

void
idt_install_isrs() {
    /* CPU exception ISRs */
    idt_set_gate(0 , (uint32_t)isr0 );
    idt_set_gate(1 , (uint32_t)isr1 );
    idt_set_gate(2 , (uint32_t)isr2 );
    idt_set_gate(3 , (uint32_t)isr3 );
    idt_set_gate(4 , (uint32_t)isr4 );
    idt_set_gate(5 , (uint32_t)isr5 );
    idt_set_gate(6 , (uint32_t)isr6 );
    idt_set_gate(7 , (uint32_t)isr7 );
    idt_set_gate(8 , (uint32_t)isr8 );
    idt_set_gate(9 , (uint32_t)isr9 );
    idt_set_gate(10, (uint32_t)isr10);
    idt_set_gate(11, (uint32_t)isr11);
    idt_set_gate(12, (uint32_t)isr12);
    idt_set_gate(13, (uint32_t)isr13);
    idt_set_gate(14, (uint32_t)isr14);
    idt_set_gate(15, (uint32_t)isr15);
    idt_set_gate(16, (uint32_t)isr16);
    idt_set_gate(17, (uint32_t)isr17);
    idt_set_gate(18, (uint32_t)isr18);
    idt_set_gate(19, (uint32_t)isr19);
    idt_set_gate(20, (uint32_t)isr20);
    idt_set_gate(21, (uint32_t)isr21);
    idt_set_gate(22, (uint32_t)isr22);
    idt_set_gate(23, (uint32_t)isr23);
    idt_set_gate(24, (uint32_t)isr24);
    idt_set_gate(25, (uint32_t)isr25);
    idt_set_gate(26, (uint32_t)isr26);
    idt_set_gate(27, (uint32_t)isr27);
    idt_set_gate(28, (uint32_t)isr28);
    idt_set_gate(29, (uint32_t)isr29);
    idt_set_gate(30, (uint32_t)isr30);
    idt_set_gate(31, (uint32_t)isr31);

    /* Reprogram the two 8259 PICs */
    /* ICW1, initialize command  */
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    /* ICW2, IDT offset 0x20 0x28 */
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    /* ICW3, chain IRQ 2 (0x04) from makred (0x02) secondary PIC */
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    /* ICW4, 8086 mode 0x01 */
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    /* OCW1, enable all IRQs */
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    /* Set IRQs in IDT */
    /* primary PIC */
    idt_set_gate(32, (uint32_t)irq0);
    idt_set_gate(33, (uint32_t)irq1);
    idt_set_gate(34, (uint32_t)irq2);
    idt_set_gate(35, (uint32_t)irq3);
    idt_set_gate(36, (uint32_t)irq4);
    idt_set_gate(37, (uint32_t)irq5);
    idt_set_gate(38, (uint32_t)irq6);
    idt_set_gate(39, (uint32_t)irq7);
    /* secondary PIC */
    idt_set_gate(40, (uint32_t)irq8);
    idt_set_gate(41, (uint32_t)irq9);
    idt_set_gate(42, (uint32_t)irq10);
    idt_set_gate(43, (uint32_t)irq11);
    idt_set_gate(44, (uint32_t)irq12);
    idt_set_gate(45, (uint32_t)irq13);
    idt_set_gate(46, (uint32_t)irq14);
    idt_set_gate(47, (uint32_t)irq15);
    
    /* Load IDT */
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_SIZE * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_reg));

    /* Enable interrupts */
    asm volatile("sti");
}
