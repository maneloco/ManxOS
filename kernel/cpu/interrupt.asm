; Definimos símbolos externos (funciones de C a las que llamaremos)
[extern isr_handler]
[extern irq_handler]

; Definimos una macro para excepciones que NO tienen código de error
; La CPU no empuja nada extra, así que empujamos un 0 falso para que el formato de pila sea uniforme.
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli             ; Deshabilita interrupciones
    push byte 0     ; Empuja un 0 dummy como "código de error"
    push byte %1    ; Empuja el número de interrupción
    jmp isr_common_stub
%endmacro

; Definimos una macro para excepciones que SÍ tienen código de error
; La CPU ya ha empujado el código de error, así que no empujamos el 0 dummy.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli
    push byte %1    ; Empuja el número de interrupción
    jmp isr_common_stub
%endmacro

; Definimos macro para peticiones de hardware (IRQ)
; Mapeamos IRQ 0-15 a las interrupciones 32-47
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

; --- DEFINICIÓN DE LAS 32 EXCEPCIONES DE LA CPU ---
ISR_NOERRCODE 0   ; Division by zero
ISR_NOERRCODE 1   ; Debug
ISR_NOERRCODE 2   ; Non maskable interrupt
ISR_NOERRCODE 3   ; Breakpoint
ISR_NOERRCODE 4   ; Into detected overflow
ISR_NOERRCODE 5   ; Out of bounds
ISR_NOERRCODE 6   ; Invalid opcode
ISR_NOERRCODE 7   ; No coprocessor
ISR_ERRCODE   8   ; Double fault
ISR_NOERRCODE 9   ; Coprocessor segment overrun
ISR_ERRCODE   10  ; Bad TSS
ISR_ERRCODE   11  ; Segment not present
ISR_ERRCODE   12  ; Stack fault
ISR_ERRCODE   13  ; General protection fault
ISR_ERRCODE   14  ; Page fault
ISR_NOERRCODE 15  ; Unknown interrupt
ISR_NOERRCODE 16  ; Coprocessor fault
ISR_ERRCODE   17  ; Alignment check
ISR_NOERRCODE 18  ; Machine check
ISR_NOERRCODE 19  ; Reserved
ISR_NOERRCODE 20  ; Reserved
ISR_NOERRCODE 21  ; Reserved
ISR_NOERRCODE 22  ; Reserved
ISR_NOERRCODE 23  ; Reserved
ISR_NOERRCODE 24  ; Reserved
ISR_NOERRCODE 25  ; Reserved
ISR_NOERRCODE 26  ; Reserved
ISR_NOERRCODE 27  ; Reserved
ISR_NOERRCODE 28  ; Reserved
ISR_NOERRCODE 29  ; Reserved
ISR_NOERRCODE 30  ; Reserved
ISR_NOERRCODE 31  ; Reserved

; --- DEFINICIÓN DE LAS INTERRUPCIONES DE HARDWARE (IRQ) ---
IRQ   0,    32 ; Timer
IRQ   1,    33 ; Teclado
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

; --- CÓDIGO COMÚN PARA EXCEPCIONES ---
isr_common_stub:
    pusha           ; Guarda edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds      ; Guarda el segmento de datos actual
    push eax

    mov ax, 0x10    ; Carga el segmento de datos del Kernel (offset 0x10 en GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler ; ¡LLAMA AL CÓDIGO C!

    pop eax         ; Restaura el segmento de datos original
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa            ; Restaura los registros generales
    add esp, 8      ; Limpia el código de error y el número de interrupción de la pila
    iret            ; Vuelve de la interrupción (pops CS, EIP, EFLAGS, SS, ESP)

; --- CÓDIGO COMÚN PARA IRQ (HARDWARE) ---
irq_common_stub:
    pusha
    mov ax, ds
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call irq_handler ; Llama al manejador de hardware en C
    
    pop ebx          ; (Truco: usamos EBX para sacar de la pila pero restauramos en segmentos)
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    popa
    add esp, 8
    iret
