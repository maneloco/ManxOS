# --- 1. Variables de Archivos ---
# Buscamos todos los archivos .c en las carpetas del proyecto
C_SOURCES = $(wildcard kernel/*.c kernel/drivers/*.c kernel/cpu/*.c)
HEADERS = $(wildcard kernel/*.h kernel/drivers/*.h kernel/cpu/*.h)

# Lista de objetos final:
# 1. kernel_entry.o DEBE ir primero (es la puerta de entrada).
# 2. interrupt.o contiene los trampolines de ensamblador.
# 3. El resto son tus archivos C compilados.
OBJ = kernel/kernel_entry.o kernel/cpu/interrupt.o $(C_SOURCES:.c=.o)

# --- 2. Herramientas y Banderas ---
CC = gcc
GDB = gdb
# Añadimos -Ikernel/cpu para que encuentre los headers fácilmente
CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -g -Ikernel/drivers -Ikernel/cpu -Ikernel
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

# --- 3. Reglas Principales ---

# Regla por defecto: Construir la imagen del OS
os-image.bin: boot/boot.bin kernel.bin
	cat boot/boot.bin kernel.bin > os-image.bin

# Linker: Une todos los objetos en un solo binario plano
kernel.bin: ${OBJ}
	ld $(LDFLAGS) -o $@ $^

# --- 4. Reglas de Compilación ---

# Regla genérica para compilar cualquier archivo .c a .o
%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) -c $< -o $@

# Ensamblar el punto de entrada del Kernel
kernel/kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf32 -o $@

# ¡NUEVO! Ensamblar los manejadores de interrupción
kernel/cpu/interrupt.o: kernel/cpu/interrupt.asm
	nasm $< -f elf32 -o $@

# Ensamblar el Bootloader
boot/boot.bin: boot/boot.asm
	nasm $< -f bin -I boot/ -o $@

# --- 5. Utilidades ---

clean:
	rm -rf *.bin kernel/*.o kernel/drivers/*.o kernel/cpu/*.o boot/*.bin

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin
