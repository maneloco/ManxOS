SOURCES = $(wildcard kernel/drivers/*.c) kernel/kernel.c
OBJ = kernel/kernel_entry.o $(SOURCES:.c=.o)

CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -Ikernel/drivers -c

# La imagen depende de kernel.bin que está en la raíz
os-image.bin: boot/boot.bin kernel.bin
	cat boot/boot.bin kernel.bin > os-image.bin

# Generamos kernel.bin directamente en la raíz del proyecto
kernel.bin: ${OBJ}
	ld -m elf_i386 -Ttext 0x1000 --oformat binary -o $@ ${OBJ}

%.o: %.c
	gcc ${CFLAGS} $< -o $@

kernel/kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf32 -o $@

boot/boot.bin: boot/boot.asm
	nasm $< -f bin -I boot -o $@

clean:
	rm -rf *.bin kernel/*.o kernel/drivers/*.o boot/*.bin

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin
