#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define BLOCK_SIZE 4096
#define BITMAP_SIZE 1024 // Suficiente para 128MB de RAM

void pmm_init(uint32_t mem_size);
void* pmm_alloc_block();
void pmm_free_block(void* addr);

#endif
