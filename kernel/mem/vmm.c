#include "vmm.h"
#include "../drivers/video.h"

/* Directorio de páginas del kernel (debe estar alineado a 4KB) */
/* Usamos pmm_alloc_block para obtener memoria física para esto */
static page_entry_t* kernel_directory = 0;

/**
 * Función en assembly para cargar el directorio y activar el bit de paginación
 */
static void vmm_switch_directory(page_entry_t* directory) {
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(directory));
}

static void vmm_enable_paging_cpu() {
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Activar el bit de paginación (PG)
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
}

void vmm_init() {
    /* 1. Reservamos un bloque físico para el Directorio de Páginas */
    kernel_directory = (page_entry_t*)pmm_alloc_block();
    
    /* 2. Limpiamos el directorio (ponemos todas las entradas a "no presente") */
    for (int i = 0; i < 1024; i++) {
        kernel_directory[i] = 0 | PAGE_WRITABLE; // Atributos por defecto, pero no presente
    }

    /* 3. Identity Mapping del primer MB (donde está el Kernel y Video) */
    /* Vamos a crear una Tabla de Páginas para los primeros 4MB */
    page_entry_t* first_table = (page_entry_t*)pmm_alloc_block();
    
    for (uint32_t i = 0; i < 1024; i++) {
        /* Dirección física i * 4096 */
        first_table[i] = (i * 4096) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* 4. Ponemos la tabla en el directorio */
    kernel_directory[0] = (uint32_t)first_table | PAGE_PRESENT | PAGE_WRITABLE;

    /* 5. Activamos la paginación */
    vmm_switch_directory(kernel_directory);
    vmm_enable_paging_cpu();

    print("[VMM]: Paginacion activada (Identity Mapping 0-4MB)\n");
}
