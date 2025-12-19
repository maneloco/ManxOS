gdt_start:
    ; Tiene que empezar con 8 bytes de 0
    dq 0x0

gdt_code:
    ; Segmento de código base= 0, limite=0xfffff
    dw 0xffff ; límite (bits 0-15)
    dw 0x0    ; base =(bits 0-15)
    db 0x0    ; base (bits 16,23)
    db 10011010b ; flags de acceso (presente, privilegio 0, código, ejecutable)
    db 11001111b ; Granularidad (4kb)  + límite (bits 16-19
    db 0x0      ; base (bits 24-31)

gdt_data:
    ; Segmento de datos: igual al anterior pero con flags de datos
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b ; Flags (Presente, Privilegio 0, Datos, Escritura)
    db 11001111b
    db 0x0

gdt_end:
    ; Descriptor de la GDT (lo que se cargará en la cpu)
    gdt_descriptor:
        dw gdt_end - gdt_start - 1 ;tamaño de la GDT
        dd gdt_start            ;dirección del inicio

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
