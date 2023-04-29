#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

byte_t *imem;
byte_t *dmem;
byte_t *vmem;

/*
 * Initialize memory. Return true if success, false if failure.
 */
bool init_mem(FILE *gbboot, FILE *rom)
{
    imem = (byte_t *) malloc(sizeof(byte_t) * IMEM_SIZE);
    if (!imem)
        return false;

    int i = 0;
    while (fread(&imem[i++], 1, 1, rom))
        ;

    return true;
}

/*
 * For debugging. Print contents of imem (n bytes).
 */
void imem_dump(int n)
{
    int i;
    for (i = 0; i <= n/10; i++) {
        int j;
        for (j = 0; j < 10; j++) {
            if ((10 * i + j) >= n)
                break;
            printf("%2x ", imem[10 * i + j]);
        }
        printf("\n");
    }
}

/*
 * Get imem at addr.
 */
byte_t imem_get(uint16_t addr)
{
    if (addr < 0 || addr >= IMEM_SIZE) {
        fprintf(stderr, "<ERROR> Invalid imem read at %x. Exiting...\n", addr);
        exit(1);
    }
    return imem[addr];
}