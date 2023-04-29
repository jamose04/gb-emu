#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

byte_t *imem;
byte_t *bootrom;
byte_t *dmem;
byte_t *vmem;

typedef enum Addrspace {
    ASPACE_ROM00,
    ASPACE_ROM01,
    ASPACE_VRAM,
    ASPACE_EXRAM,
    ASPACE_WRAM,
    ASPACE_OAM,
    ASPACE_IO_REG,
    ASPACE_HRAM,
    ASPACE_IE,
    ASPACE_INVALID
} addrspace_t;

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

    bootrom = (byte_t *) malloc(sizeof(byte_t) * GBBOOT_SIZE);
    if (!bootrom) 
        return false;
        
    i = 0;
    while (fread(&bootrom[i++], 1, 1, gbboot))
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

static addrspace_t get_addrspace(uint16_t addr)
{
    if (0x0000 <= addr && addr <= 0x3fff)
        return ASPACE_ROM00;
    else if (0x4000 <= addr && addr <= 0x7fff)
        return ASPACE_ROM01;
    else if (0x8000 <= addr && addr <= 0x9fff)
        return ASPACE_VRAM;
    else if (0xa000 <= addr && addr <= 0xbfff)
        return ASPACE_EXRAM;
    else if (0xc000 <= addr && addr <= 0xdfff)
        return ASPACE_WRAM;
    else if (0xfe00 <= addr && addr <= 0xfe9f)
        return ASPACE_OAM;
    else if (0xff00 <= addr && addr <= 0xff7f)
        return ASPACE_IO_REG;
    else if (0xff80 <= addr && addr <= 0xfffe)
        return ASPACE_HRAM;
    else if (addr == 0xffff)
        return ASPACE_IE;
    else
        return ASPACE_INVALID;
}

bool mem_write(uint16_t addr, byte_t val)
{
    addrspace_t space = get_addrspace(addr);
    switch (space) {
        case ASPACE_IO_REG:
            return iomem_write(addr, val);
        default:
            fprintf(stderr, 
                "<ERROR> Memory write to invalid address: %x\n", addr);
            exit(1);
    };
}