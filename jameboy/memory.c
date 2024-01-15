#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *imem;
static uint8_t *bootrom;

static uint8_t *oam;

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
bool mem_init(FILE *gbboot, FILE *rom)
{
    imem = malloc(sizeof(byte_t) * IMEM_SIZE);
    if (!imem)
        return false;

    int i = 0;
    while (fread(&imem[i++], 1, 1, rom))
        ;

    bootrom = malloc(sizeof(byte_t) * GBBOOT_SIZE);
    if (!bootrom) 
        return false;
        
    i = 0;
    while (fread(&bootrom[i++], 1, 1, gbboot))
        ;

    // init oam
    oam = malloc(sizeof(uint8_t) * OAM_SIZE);

	if (!iomem_init()) {
		return false;
	}
	
	if (!rmem_init()) {
		return false;
	}
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
uint8_t imem_get(uint16_t addr)
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

/*
 * Read a byte of memory located at addr.
 */
uint8_t mem_read(uint16_t addr)
{
    addrspace_t space = get_addrspace(addr);
    switch (space) {
        case ASPACE_INVALID:
            fprintf(stderr, "<ERROR> Invalid memory read. Exiting...\n");
            exit(1);
        case ASPACE_ROM00:
            if (addr < GBBOOT_SIZE && iomem_gbboot_enable())
                return bootrom[addr];
            else
                return imem[addr];
        case ASPACE_ROM01:
            return imem[addr];
        case ASPACE_HRAM:
			return hram_read(addr);
		case ASPACE_WRAM:
            if (iomem_dma_dots() == -1) {
                fprintf(stderr, "<WARNING> read during dma... exit\n");
                exit(1);
            }
			return wram_read(addr);
		case ASPACE_VRAM:
			return vram_read(addr);
        default:
            return 0;
    }
    return 0;
}

// If w16, then write the lower 8 bits of val to (addr).
static void write(uint16_t addr, uint16_t val, bool w16)
{
    addrspace_t space = get_addrspace(addr);
    switch (space) {
        case ASPACE_IO_REG:
            if (w16) {
                fprintf(stderr, "<ERROR> Unsupported 16-bit write to iomem\n");
                exit(1);
            }
            iomem_write(addr, val);
        case ASPACE_HRAM:
            if (w16)
                hram_write16(addr, val);
            else
                hram_write(addr, (uint8_t) (val & 0xffu));
            break;
		case ASPACE_WRAM:
            if (iomem_dma_dots() == -1) {
                fprintf(stderr, "<WARNING> writing to wram during dma"
                    "transfer. It will be ignored...\n");
                return;
            }
            if (w16)
                wram_write16(addr, val);
            else
                wram_write(addr, (uint8_t) (val & 0xffu));
            break;
		case ASPACE_VRAM:
            if (w16)
                vram_write16(addr, val);
            else
                vram_write(addr, (uint8_t) (val & 0xffu));
            break;
        default:
            fprintf(stderr, 
                "<ERROR> Memory write to invalid address: %x\n", addr);
            exit(1);
    };
}

void mem_write(uint16_t addr, uint8_t val)
{
    write(addr, val, false);
}

void mem_write16(uint16_t addr, uint16_t val)
{
    write(addr, val, true);
}

/* update after "dots" cycles */
void mem_update(uint8_t dots)
{
    //things we need to do:
    /*
     * 1. continue dma transfer if it is active
     *    (copy dots bytes from rom (dma_addr) to oam)
     * 2. ???
     */
    int16_t dr = iomem_dma_dots();
    uint16_t src_idx = iomem_dma_addr() + (0xa0 - dr);
    // NOTE: change this later ?? or maybe not!
    if (src_idx >= 0xdf9f) {
        fprintf(stderr, "<ERROR> bad dma src addr... %x\n", src_idx);
        exit(1);
    }

    if (dr != -1) {
        // dma transfer is active
        if (dots > dr) {
            //transfer dr bytes
            memcpy(oam, (imem + src_idx), dr);
        } else {
            memcpy(oam, (imem + src_idx), dots);
        }
    }
}