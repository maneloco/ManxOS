#include "keyboard.h"
#include "../cpu/isr.h"  // Necesario para register_interrupt_handler
#include "io.h"          // Necesario para inb (leer puerto)
#include "video.h"       // Necesario para print

// Callback que se ejecuta CADA VEZ que pulsas una tecla (Interrupción 33)
static void keyboard_callback(registers_t *regs) {
    // 1. LEER EL PUERTO DE DATOS DEL TECLADO (0x60)
    // Es OBLIGATORIO leerlo, si no el buffer del teclado se llena y deja de funcionar.
    uint8_t scancode = inb(0x60);
    
    // 2. DETECTAR SI ES "PRESS" O "RELEASE"
    // Si el bit más alto (0x80) está activo, significa que la tecla se ha soltado.
    // Ejemplo: 'A' pulsada = 0x1E, 'A' soltada = 0x9E (0x1E + 0x80)
    if (scancode & 0x80) {
        // Por ahora ignoramos cuando se suelta la tecla.
        // Aquí podrías manejar flags para Shift o Ctrl.
        return;
    }

    // 3. MAPA DE CARACTERES (US QWERTY - Scancode Set 1)
    // Array simple para traducir scancode -> ASCII
    // Los '?' son teclas no imprimibles (F1, F2...) o no implementadas
    char *sc_ascii[] = { 
        "?", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Back", "Tab", 
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\n", "Ctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LShft", "\\", 
        "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RShft", "*", "Alt", " "
    };

    // 4. IMPRIMIR EN PANTALLA
    // Solo manejamos los códigos básicos imprimibles (hasta el espacio 0x39)
    // El array tiene un tamaño limitado, si accedemos fuera crasheamos el kernel.
    if (scancode <= 0x39) {
        char *str = sc_ascii[scancode];
        print(str); // Usamos tu función de video para pintar la letra
    }
}

// Función de inicialización
void init_keyboard() {
    // Registramos nuestra función en la interrupción 33 (IRQ1)
    // Nota: Es 33 porque el PIC Maestro (IRQ 0-7) se remapeó al rango 32-39.
    register_interrupt_handler(33, keyboard_callback);
}
