#ifndef IDT_H
#define IDT_H

#include "../drivers/utils.h"

// Se define el segmento de código donde se encuentra el kernel.
// En la GDT el primero es nulo 0x00 y luego el 0x08 es ya código.
#define KERNEL_CS 0x08

// Número de puertas de la tabla
#define IDT_ENTRIES 256

typedef struct {
    uint16_t low_offset; //16 bits de la dirección de la función handler
    uint16_t sel;        //Selección del segmento del Kernel 0x08
    uint8_t always0;     //Byte que siempre tiene que ser 0, lo dice INTEL
    /* FLAGS (8 bits):
       Bit 7:    Presente (1 = activa)
       Bits 5-6: DPL (Privilegio, 0 = Kernel, 3 = Usuario)
       Bit 4:    Storage Segment (0 para interrupciones)
       Bits 0-3: Tipo de puerta (0xE = 1110 = Interrupción de 32 bits)
       
       Normalmente usaremos 0x8E (10001110)
    */
    uint8_t flags;
    
    uint16_t high_offset; //Los 16 bits de la dirección alta de la función handler
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16_t limit; // Tamaño de la tabla en bytes - 1
    uint32_t base;  // Dirección de memoria donde empieza la tabla
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uint32_t handler);

/*
   Esta función carga la tabla en la CPU usando ASM.
*/
void set_idt_register();

#endif
