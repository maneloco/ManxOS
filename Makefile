# Objetos que luego se unirán con el linker
OBJ = kernel/kernel_entry.o \
	  kernel/kernel.o

# Banderas de compilación
# -m32: Genera código de 32 bits (fundamental)
# -ffreestanding: No uses la biblioteca estándar de C (no hay printf, etc.)
# -fno-stack-protector: Desactiva la seguridad de pila que mencionamos antes
CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -c

os-image.bin: boot/boot.bin kernel/kernel.bin
	cat boot/boot.bin kernel/kernel.bin > os-image.bin

kernel/kernel.bin: ${OBJ} # Para crear el kernel.bin necesitamos que estén todos los objetos a unirf
	ld -m elf_i386 -Ttext 0x1000 --oformat binary -o kernel/kernel.bin ${OBJ} # Le dice que es bare metal, que empiece ene l 0x1000 y en formato binario todos los objetos

kernel/kernel.o: kernel/kernel.c
	gcc ${CFLAGS} kernel/kernel.c -o kernel/kernel.o

kernel/kernel_entry.o: kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf32 -o kernel/kernel_entry.o # Crea el objeto en formato 32 bits

boot/boot.bin: boot/boot.asm
	nasm boot/boot.asm -f bin -I boot/ -o boot/boot.bin

clean:
	rm *.bin *.o

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin
