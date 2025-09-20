OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
SECTIONS
 {
   . = 1K;
   .text BLOCK(4K) : ALIGN(4K)
   {
       CMakeFiles/kernel-mb2.dir/kernel_entry.asm.o(.multiboot)
       *(.text)
   }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }

