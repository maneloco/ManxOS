[bits 32]
global _start   ; Le decimos al linker que esta etiqueta es pública

_start:         ; Este es el punto de entrada oficial
    [extern main]
    call main
    jmp $
