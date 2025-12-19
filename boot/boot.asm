[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov bp, 0x9000
    mov sp, bp

    call cargar_kernel
    call cambiar_a_modo_protegido

    jmp $

; --- ESCRIBE ESTA LÍNEA MANUALMENTE SI PUEDES ---
%include "gdt.asm"

[bits 16]
cargar_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13
    ret

cambiar_a_modo_protegido:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_32bits

[bits 32]
init_32bits:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55
