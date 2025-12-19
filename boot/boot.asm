[org 0x7c00]
    KERNEL_OFFSET equ 0x1000 ; Se guarda que el kernel empiece en la memoria ram

    mov byte [BOOT_DRIVE], dl    ; Se guarda en BOOT_DRIVE el contenido de dl (nombre del disco)

    mov bp, 0x9000      ; Inicio la pila en un lugar seguro
    mov sp, bp          ; Le decimos al puntero de la pila que empiece en el inicio

    call load_kernel      ; Guarda en la memoria RAM el kernel
    call switch_to_pm   ; Da el salto a los 32 bits y modo protegido
    jmp $               ; En caso de error la CPU se queda aquí

[bits 16]


load_kernel:
    mov bx, KERNEL_OFFSET   ; Registro donde va a empezar a escribir
    mov dh, 15              ; Número de sectores a leer (reserva 15 sectores para el kernel
    mov dl, [BOOT_DRIVE]    ; Por si cambia el valor del dl lo volvemos a cargar el disco en el que va a escribir
    
    call disk_load          ; Lo guarda en disco
    ret                     ; Acaba la función

disk_load:
    push dx                 ; Guarda el valor de DX para saber cuantos sectores pedimos
    mov ah, 0x02            ; Función de la BIOS de leer sectores
    mov al, dh              ; AL = número sectores
    mov ch, 0x00            ; Cilindro
    mov dh, 0x00            ; Cabeza
    mov cl, 0x02            ; Empieza en el sector 2, (1 es el bootloader)

    int 0x13                ; Interrupción de la BIOS

    jc disk_error           ; Si hay error salta
    pop dx
    ret

disk_error:
    jmp $                   ; Deja la CPU para que no haya más errores

%include "gdt.asm"

switch_to_pm:
    cli                     ; Desactiva las interrupciones de la BIOS
    lgdt [gdt_descriptor]   ; Le damos información de cómo se distribuye la memoria RAM

    mov eax, cr0            ; Ahora cambiamos el último bit de cr0 para entrar en modo seguro 32bits
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; Salta al apartado de 32 bits limpiando las instrucciones de 16 bits, preparando al procesador

[bits 32]
init_pm:
    mov ax, DATA_SEG        ; Le damos a los registros del procesador cómo leer datos
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000         ; Creamos una nueva pila en 32 bits
    mov esp, ebp            ; Que el puntero apunte al inicio de la pila

    call KERNEL_OFFSET      ; Se salta a la dirección donde se cargó el KERNEL

    jmp $                   ; En caso de que se cierre el kernel se queda aquí

BOOT_DRIVE db 0             ; Borrael  el disco donde se ejecutó el n bootloader (ahora estamos en la RAM)
times 510-($-$$) db 0       ; Escribe 0 hasta llenar los 510 primeros bytes
dw 0xaa55                   ; Escribe la palabra mágica del bootloader
