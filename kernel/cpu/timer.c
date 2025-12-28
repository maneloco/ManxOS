#include "timer.h"
#include "../drivers/io.h"
#include "../drivers/video.h"
#include "isr.h"

uint32_t tick = 0;

// Esta función se llamará CADA VEZ que el reloj haga "tick" (ej: 50 veces/segundo)
static void timer_callback(registers_t *regs) {
    tick++;
    
    // Para no saturar la pantalla, imprimimos un punto cada 100 ticks (aprox 2 segundos si freq=50)
    /*
    if (tick % 100 == 0) {
        print("Tick() ");
    }
    */
}

void init_timer(uint32_t freq) {
    // 1. Registramos el handler en la IRQ0 (INT 32)
    register_interrupt_handler(32, timer_callback);

    // 2. Calculamos el divisor para enviar al chip PIT
    // El reloj base del hardware es 1.193.180 Hz
    uint32_t divisor = 1193180 / freq;
    
    // 3. Enviamos el comando al puerto de control 0x43
    // 0x36 = 00110110 (Canal 0, Accesso lobyte/hibyte, Modo generador de ondas, Binario)
    outb(0x43, 0x36);

    // 4. Enviamos el divisor byte a byte (Low y High)
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor >> 8) & 0xFF );

    outb(0x40, l);
    outb(0x40, h);
}
