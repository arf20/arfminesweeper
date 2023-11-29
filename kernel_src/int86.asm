; credit: https://github.com/jtsiomb/pcboot/releases/tag/int86_fixed_flags
; this is not boot loader code. It's called later on by the main kernel
; code in 32bit protected mode. It's placed here because it needs to be
; located in base memory as it returns and runs in real mode.

			[bits 32]
			align 4
			; place to save the protected mode IDTR pseudo-descriptor
			; with sidt, so that it can be restored before returning
			dw 0
saved_idtr:
idtlim:		dw 0
idtaddr:	dd 0
			; real mode IDTR pseudo-descriptor pointing to the IVT at addr 0
			dw 0
rmidt:		dw 0x3ff
			dd 0

saved_esp: 	dd 0
saved_ebp: 	dd 0
saved_eax: 	dd 0
saved_es: 	dw 0
saved_ds: 	dw 0
saved_flags:dw 0

	; drop back to unreal mode to call 16bit interrupt
global int86
int86:
	push ebp
	mov esp, ebp
	push al
	cli
	; save protected mode IDTR and replace it with the real mode vectors
	;sidt (saved_idtr) ; NOTE: we don't have an IDT in arfminesweeper
	lidt (rmidt)

	; modify the int instruction. do this here before the
	; cs-load jumps, to let them flush the instruction cache
	mov $int_op, ebx
	movb 8(ebp), al
	movb al, 1(ebx)

	; long jump to load code selector for 16bit code (6)
	ljmp $0x30,$0f
0:
	[bits 16]
	; disable protection
	mov %cr0, %eax
	and $0xfffe, %ax
	mov %eax, %cr0
	; load cs <- 0
	ljmp $0,$0f
0:	; zero data segments
	xor %ax, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	nop

	; load registers from the int86regs struct
	; point esp to the regs struct to load registers with popa/popf
	mov %esp, saved_esp
	mov %ebp, saved_ebp
	mov 12(%ebp), %esp
	popal
	popfw
	pop %es
	pop %ds
	; ignore fs and gs for now, don't think I'm going to need them
	mov saved_esp, %esp

	; move to the real-mode stack, accessible from ss=0
	; just in case the BIOS call screws up our unreal mode
	mov $0x7be0, %esp

	; call 16bit interrupt
int_op:	int $0
	; BIOS call might have enabled interrupts, cli for good measure
	cli

	; save all registers that we'll clobber before having the
	; chance to populate the int86regs structure
	mov %eax, saved_eax
	mov %ds, saved_ds
	mov %es, saved_es
	pushfw
	pop %ax
	mov %ax, saved_flags

	; re-enable protection
	mov %cr0, %eax
	or $1, %ax
	mov %eax, %cr0
	; long jump to load code selector for 32bit code (1)
	ljmp $0x8,$0f
0:
	[bits 32]
	; set data selector (2) to all segment regs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	nop

	; point the esp to our regs struct, to fill it with pusha/pushf
	mov saved_ebp, %ebp
	mov 12(%ebp), %esp
	add $38, %esp
	mov saved_ds, %ax
	pushw %ax
	mov saved_es, %ax
	pushw %ax
	mov saved_flags, %ax
	pushw %ax
	mov saved_eax, %eax
	pushal
	mov saved_esp, %esp

	; restore 32bit interrupt descriptor table
	;lidt (saved_idtr)
	;sti
	;popal
	;pop %ebp
	ret
