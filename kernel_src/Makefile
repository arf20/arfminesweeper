# $@ = target file
# $< = first dependency
# $^ = all dependencies

AS=nasm
CC=gcc
LD=ld
CFLAGS=-Wall -Wextra -Wpedantic -m32 -ffreestanding -nostdlib -fno-pie -DFRONTENDS_KERNEL

# First rule is the one executed when no parameters are fed to the Makefile
all: run

mbr_boot.bin: mbr_boot.asm
	$(AS) $< -f bin -o $@

kernel_entry.o: kernel_entry.asm
	$(AS) $< -f elf -o $@

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.asm $(DEPS)
	$(AS) $< -f elf -o $@

kernel.bin: kernel_entry.o kernel.o port.o vgacon.o keyb.o alloc.o random.o rtc_time.o int32.o plibc.o ../common/game.o kfrontends/vgacli.o kfrontends/vgatui.o 
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

arfminesweeper.bin: mbr_boot.bin kernel.bin
	cat $^ > $@ && truncate -s 64K arfminesweeper.bin

run: arfminesweeper.bin
	qemu-system-i386 -hda $<

clean:
	$(RM) *.bin *.o kfrontends/*.o *.dis
