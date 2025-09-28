OUTPUT_FORMAT(elf32-i386)
ENTRY(_mb2_entry)
SECTIONS
{
    . = 1K;
    .text BLOCK(4K) : ALIGN(4K)
    {
        CMakeFiles/kernel-mb2.dir/kernel_entry.asm.o(.multiboot)
        CMakeFiles/kernel-mb2.dir/kernel_entry.asm.o(.text)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}

