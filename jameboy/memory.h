#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define IMEM_SIZE 0x8000

#define GBBOOT_SIZE 0x100

bool init_mem(FILE *gbboot, FILE *rom);

void imem_dump(int n);

byte_t imem_get(uint16_t addr);

/* Use this to write to an address */
void mem_write(uint16_t addr);

#endif //MEMORY_H