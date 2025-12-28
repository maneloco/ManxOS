#ifndef ISR_H
#define ISR_H

#include "../drivers/utils.h"

typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no;                                 // Interrupt number
    uint32_t err_code;                               // Error code
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically
} registers_t;

// Definimos el tipo "isr_t" como un puntero a una función que recibe registers_t*
// Esto nos permite crear arrays de funciones manejadoras.
typedef void (*isr_t)(registers_t*);

// Función pública para que los drivers (teclado, timer) registren sus callbacks
void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_install();
void isr_handler(registers_t r);
void irq_install();

#endif
