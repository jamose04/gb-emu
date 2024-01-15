#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include "iomem.h"
#include "rmem.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define IMEM_SIZE 0x8000
#define OAM_SIZE 0xa0

#define GBBOOT_SIZE 0x100

bool mem_init(FILE *gbboot, FILE *rom);

void imem_dump(int n);

uint8_t imem_get(uint16_t addr);

uint8_t mem_read(uint16_t addr);

/* Use this to write to an address */
void mem_write(uint16_t addr, uint8_t val);

void mem_write16(uint16_t addr, uint16_t val);

#endif //MEMORY_H
