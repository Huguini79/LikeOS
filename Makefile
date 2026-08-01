ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld
QEMU = qemu-system-i386

LD_FLAGS = -m elf_i386 -Map=kernel.map -g -relocatable
ASM_FLAGS = -f bin
ASM_FLAGS2 = -f elf32
CC_FLAGS = -I./include -c -m32 -g -ffreestanding -fno-pie -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -Os -Iinc -Wno-int-conversion
HEAD = boot/head.asm -o build/head.bin
KERNEL_ASM = kernel/kernel.asm -o build/kernel.asm.o
KERNEL_C = kernel/kernel.c -o build/kernel.o

OBJECT_FILES = build/kernel.asm.o build/kernel.o build/console.o build/printk.o build/panic.o build/gdt.o build/traps.o build/asm.asm.o build/ioport.asm.o build/pic.o build/sched.o build/signal.o

all:
	clear
	$(ASM) $(ASM_FLAGS) $(HEAD)
	$(ASM) $(ASM_FLAGS2) $(KERNEL_ASM)
	$(ASM) $(ASM_FLAGS2) kernel/asm.asm -o build/asm.asm.o
	$(ASM) $(ASM_FLAGS2) kernel/ioport.asm -o build/ioport.asm.o
	$(CC) $(CC_FLAGS) $(KERNEL_C)
	$(CC) $(CC_FLAGS) kernel/console.c -o build/console.o
	$(CC) $(CC_FLAGS) kernel/printk.c -o build/printk.o
	$(CC) $(CC_FLAGS) kernel/panic.c -o build/panic.o
	$(CC) $(CC_FLAGS) kernel/gdt.c -o build/gdt.o
	$(CC) $(CC_FLAGS) kernel/traps.c -o build/traps.o
	$(CC) $(CC_FLAGS) kernel/pic.c -o build/pic.o
	$(CC) $(CC_FLAGS) kernel/sched.c -o build/sched.o
	$(CC) $(CC_FLAGS) kernel/signal.c -o build/signal.o
	$(LD) $(LD_FLAGS) $(OBJECT_FILES) -o build/kernelfull.o
	$(CC) -m32 -T linker/linker.ld -o build/kernel.bin -ffreestanding -Os -nostdlib build/kernelfull.o
	dd if=build/head.bin > LikeOS.bin
	dd if=build/kernel.bin >> LikeOS.bin
	dd if=/dev/zero bs=1048576 count=16 >> LikeOS.bin
	$(QEMU) -d cpu_reset LikeOS.bin

clean:
	clear
	rm -rf build/*.o build/*.asm.o
