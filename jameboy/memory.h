#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdbool.h>

#define IMEM_SIZE 0x8000

bool init_mem(FILE *gbboot, FILE *rom);

void imem_dump(int n);

#endif //MEMORY_H