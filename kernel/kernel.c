#include "drivers/video.h"
#include "drivers/keyboard.h"
#include "cpu/isr.h"
#include "cpu/idt.h"

/**
 * Punto de entrada del Kernel en C
 * En este punto, el sistema ya esta en Modo Protegido de 32 bits.
 */
void main() {
    /* 1. Inicializar el sistema de video */
    clear_screen();

    print("ManxOS Kernel v1.0\n");
    print("------------------\n");
    print("Estado: Configurando interrupciones...\n");

    /* 2. Instalar la IDT y los ISRs */
    isr_install();
    print("[OK] IDT configurada.\n");

    /* 3. Inicializar el teclado */
    /* Registramos el callback para la IRQ1 antes de activar interrupciones */
    init_keyboard();
    print("[OK] Teclado inicializado.\n");

    /* 4. Habilitar interrupciones de hardware */
    __asm__ __volatile__("sti");
    print("[OK] Interrupciones de hardware activas.\n");

    print("\nSistema listo. Escribe algo para probar el teclado:\n");
    print("> ");

    /* 5. Bucle de espera activa */
    while(1) {
        __asm__ __volatile__("hlt"); 
    }
}
