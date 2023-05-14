#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include "iomem.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define IMEM_SIZE 0x8000

#define GBBOOT_SIZE 0x100

bool init_mem(FILE *gbboot, FILE *rom);

void imem_dump(int n);

byte_t imem_get(uint16_t addr);

byte_t mem_read(uint16_t addr);

/* Use this to write to an address */
bool mem_write(uint16_t addr, byte_t val);

void mem_write16(uint16_t addr, uint16_t val);

#endif //MEMORY_H
