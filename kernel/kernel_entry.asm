[bits 32]       ; Ya está en modo protegido
[extern main]   ; Se declara que el main está en otro archivo

_start:
    call main   ; Se salta al main del código C
    jmp $       ; En caso de que se cierre acaba ahí


