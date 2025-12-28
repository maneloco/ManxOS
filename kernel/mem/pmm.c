#include "pmm.h"
#include "../drivers/video.h"
#include "../drivers/utils.h"

/* El bitmap: cada bit representa una página de 4KB */
/* Para 128MB de RAM necesitamos 32768 bits (1024 uint32_t) */
static uint32_t bitmap[BITMAP_SIZE];
static uint32_t total_blocks = 0;
static uint32_t used_blocks = 0;

/**
 * Marca un bloque (página) como ocupado en el bitmap
 */
static void pmm_set_block(uint32_t bit) {
    bitmap[bit / 32] |= (1 << (bit % 32));
}

/**
 * Marca un bloque como libre en el bitmap
 */
static void pmm_unset_block(uint32_t bit) {
    bitmap[bit / 32] &= ~(1 << (bit % 32));
}

/**
 * Comprueba si un bloque está ocupado
 */
static int pmm_test_block(uint32_t bit) {
    return bitmap[bit / 32] & (1 << (bit % 32));
}

/**
 * Busca el primer bloque libre en el bitmap
 */
static int pmm_find_first_free() {
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0xFFFFFFFF) { // Si el entero no está lleno
            for (int j = 0; j < 32; j++) {
                if (!(bitmap[i] & (1 << j))) {
                    return i * 32 + j;
                }
            }
        }
    }
    return -1;
}

/**
 * Inicializa el gestor de memoria física
 * @param mem_size Tamaño total de la RAM en bytes
 */
void pmm_init(uint32_t mem_size) {
    total_blocks = mem_size / BLOCK_SIZE;
    used_blocks = 0;

    // Inicialmente, toda la memoria está libre (0)
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0;
    }

    /* CRÍTICO: Reservar la memoria que ya está usando el Kernel */
    /* El kernel empieza en 0x1000 y ocupa unos 50 sectores (25KB aprox) */
    /* Reservamos los primeros 1MB por seguridad (BIOS, Video, Kernel) */
    for (uint32_t i = 0; i < (1024 * 1024) / BLOCK_SIZE; i++) {
        pmm_set_block(i);
        used_blocks++;
    }
    
    print("PMM: Inicializado con ");
    char s[10];
    int_to_ascii(total_blocks, s);
    print(s);
    print(" bloques.\n");
}

/**
 * Asigna un bloque de memoria física (4KB)
 */
void* pmm_alloc_block() {
    int free_block = pmm_find_first_free();
    if (free_block == -1) return 0; // Out of memory

    pmm_set_block(free_block);
    used_blocks++;
    return (void*)(free_block * BLOCK_SIZE);
}

/**
 * Libera un bloque de memoria física
 */
void pmm_free_block(void* addr) {
    uint32_t block = (uint32_t)addr / BLOCK_SIZE;
    pmm_unset_block(block);
    used_blocks--;
}
