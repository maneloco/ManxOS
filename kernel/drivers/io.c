#include "io.h"

void outb(unsigned short port, unsigned char data) {
    // __asm__ indica al compilador que es una instrucción en ensamblador
    // out es la instrucción para enviar datos por un puerto específico
    // el registro al es que lleva los datos y el dx el que lleva el puerto
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned char inb(unsigned short port) {
    unsigned char result;
    // "in %%dx, %%al" lee del puerto (dx) y lo guarda en al (result)
    __asm__ __volatile__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}
