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
;   mbr_boot.asm: MBR boot sector bootloader
;

[bits 16]
[org 0x7c00]

; kernel load address
KERNEL_OFFSET   equ 0x1000
; kernel size in sectors
KERNEL_SIZE     equ 48  ; NOTE IMPORTANT FUCK: always the culprit 

; BIOS boot drive we were booted from
mov [bios_boot_drive], dl
; set up stack at 0x9000
mov bp, 0x9000
mov sp, bp
; NOTE: 32K between kernel begin and stack end

call load_kernel        ; read the kernel from disk
call switch_to_32bit    ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
jmp $                   ; never executed

; =========================== BIOS disk loading helpers ========================
; load 'dh' sectors from drive 'dl' into ES:BX
disk_load:
    pusha
    push dx         ; save dx

    mov ah, 0x02    ; ah <- int 0x13 function 0x02 = 'read'
    mov al, dh      ; al <- number of sectors to read (0x01 .. 0x80)
    ; CHR
    mov cl, 0x02    ; cl <- sector (0x01 .. 0x11)
                    ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00    ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    mov dh, 0x00    ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13        ; BIOS interrupt
    jc infinite_loop; if error (stored in the carry bit)

    pop dx
    cmp al, dh      ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne infinite_loop
    popa
    ret

; hang on error (not great)
infinite_loop:
    jmp $

; =========================== GDT ===========================
gdt_start:
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bit), always one less of its true size
    dd gdt_start                ; address (32 bit)

; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; =========================== 32-bit protected switch ==========================
[bits 16]
switch_to_32bit:
    cli                     ; 1. disable interrupts
    lgdt [gdt_descriptor]   ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_32bit ; 4. far jump by using a different segment

[bits 32]
init_32bit:                 ; we are now using 32-bit instructions
    mov ax, DATA_SEG        ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call begin_32bit        ; 7. Call a well-known label with useful code

; =========================== 32-bit protected switch ===========================
[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET       ; write to kernel location
    mov dh, KERNEL_SIZE         ; numer of sectors to read
    mov dl, [bios_boot_drive]   ; from BIOS boot drive
    call disk_load
    ret

[bits 32]
begin_32bit:
    call KERNEL_OFFSET  ; Give control to the kernel
    jmp $               ; hang on kernel return


bios_boot_drive db 0    ; BIOS boot drive number

; padding
times 510 - ($-$$) db 0
dw 0xaa55 ; MBR signature
