;
;   arfminesweeper: Cross-plataform multi-frontend game
;   Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <https://www.gnu.org/licenses/>.
;
;   kernel_entry.asm: kernel entry point
;

[bits 32]
[extern kmain]

global _start

STACK_SIZE:  equ 0x4000

section .text
_start:
    jmp mb2_entry

mb2_entry:
    ; first: read the entirety of
    ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html

    ; set stack
    mov     esp, stack + STACK_SIZE
    ; clear EFLAGS
    push    0
    popf
    ; push magic, multiboot info
    push    ebx
    push    eax
    ; call C kernel
    call    kmain
    ; halt
    hlt

section .multiboot
    align 8
_mb2_hdr:
    ; mb2 header
    dd  0xE85250D6                   ; magic
    dd  0                            ; arch
    dd  _mb2_hdr_end - _mb2_hdr      ; size
    dd  -(0xE85250D6 + (_mb2_hdr_end - _mb2_hdr)) ; checksum

    align 8
_mb2_tag_entry:
    dw  3   ; tag type
    dw  0   ; flags
    dd  12  ; size
    dd  mb2_entry

    align 8
_mb2_tag_fb:
    dw  5       ; tag type
    dw  0       ; flags
    dd  20      ; size
    dd  1024    ; width
    dd  768     ; height
    dd  32

    align 8
_mb2_tag_end: 
    dw 0    ; end tag
    dw 0
    dd 8
_mb2_hdr_end:

section .bss
    common stack    0x4000

