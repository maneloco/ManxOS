# --- Variables de archivos ---
# El orden en OBJ es vital: kernel_entry.o DEBE ser el primero
OBJ = kernel/kernel_entry.o \
      kernel/kernel.o \
      kernel/display/display.o \
      kernel/utils/utils.o

# --- Herramientas y Banderas ---
CC = gcc
LD = ld
NASM = nasm

# -m32: Compila para 32 bits
# -ffreestanding: Indica que no hay librería estándar de C
# -fno-pie/pic: Desactiva código independiente de posición (necesario para kernels simples)
# -Ikernel: Permite incluir headers usando #include "display/display.h"
CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -Ikernel -c

# Parametros del Linker
# -Ttext 0x1000: El código empieza en la dirección de memoria 0x1000
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

# --- Reglas Principales ---

# Regla por defecto: crea la imagen final del OS
os-image.bin: boot/boot.bin kernel.bin
	cat boot/boot.bin kernel.bin > os-image.bin

# Une todos los objetos en el binario del kernel
kernel.bin: ${OBJ}
	${LD} -o $@ ${LDFLAGS} $^

# --- Reglas de Compilación ---

# Compila cualquier archivo .c a .o
# $< es el archivo fuente (.c) y $@ es el destino (.o)
%.o: %.c
	${CC} ${CFLAGS} $< -o $@

# Ensambla archivos .asm (como kernel_entry) a formato ELF de 32 bits
kernel/kernel_entry.o: kernel/kernel_entry.asm
	${NASM} $< -f elf32 -o $@

# Ensambla el bootloader a formato binario plano (512 bytes)
boot/boot.bin: boot/boot.asm
	${NASM} $< -f bin -I boot/ -o $@

# --- Utilidades ---

# Limpia todos los archivos generados para compilar desde cero
clean:
	rm -rf *.bin *.o
	rm -rf kernel/*.o
	rm -rf kernel/display/*.o
	rm -rf kernel/utils/*.o
	rm -rf boot/*.bin

# Ejecuta el kernel en QEMU rápidamente
run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin
