#include "rmem.h"

struct {
    bool exram_enable;
} rmem_stat;

uint8_t *vram;
uint8_t *wram;
uint8_t *hram;

uint8_t *exram;

bool rmem_init()
{
    rmem_stat.exram_enable = false;
    vram = malloc(0x9fffu - 0x8000u + 1);
    wram = malloc(0xdfffu - 0xc000u + 1);
    hram = malloc(0xfffeu - 0xff80u + 1);
    return true;
}

void rmem_close()
{
    free(vram);
    free(wram);
    free(hram);
    if (rmem_stat.exram_enable)
        free(exram);
}

void vram_write(uint16_t addr, uint8_t val)
{
    if (!(0x8000u <= addr || addr <= 0x9fffu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for vram. This is a bug\n");
        exit(1);
    }
    vram[addr - 0x8000u] = val;
}

void vram_write16(uint16_t addr, uint16_t val)
{
    if (!(0x8000u <= addr || addr <= 0x9ffeu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for vram. This is a bug\n");
        exit(1);
    }
    *((uint16_t *) (vram + (addr - 0x8000u))) = val;
}

void wram_write(uint16_t addr, uint8_t val)
{
    if (!(0xc000u <= addr || addr <= 0xdfffu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for wram. This is a bug\n");
        exit(1);
    }
    wram[addr - 0xc0000u] = val;
}

void wram_write16(uint16_t addr, uint16_t val)
{
    if (!(0xc000u <= addr || addr <= 0xdffeu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for wram. This is a bug\n");
        exit(1);
    }
    *((uint16_t *) (wram + (addr - 0xc0000u))) = val;
}

void hram_write(uint16_t addr, uint8_t val)
{
    if (!(0xff80u <= addr || addr <= 0xfffeu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for hram. This is a bug\n");
        exit(1);
    }
    hram[addr - 0xff80u] = val;
}

void hram_write16(uint16_t addr, uint16_t val)
{
    if (!(0xff80u <= addr || addr <= 0xfffdu)) {
        fprintf(stderr, 
            "<ERROR> Wrong address space for hram. This is a bug\n");
        exit(1);
    }
    *((uint16_t *) (hram + (addr - 0xff80u))) = val;
}
