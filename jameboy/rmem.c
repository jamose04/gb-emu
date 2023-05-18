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
    if (!vram)
        return false;
    wram = malloc(0xdfffu - 0xc000u + 1);
    if (!wram)
        return false;
    hram = malloc(0xfffeu - 0xff80u + 1);
    if (!hram)
        return false;
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

static void check_addr(uint16_t addr, uint16_t addr_l, uint16_t addr_u, char *what)
{
    if (!(addr_l <= addr || addr <= addr_u)) {
        fprintf(stderr,
            "<ERROR> wrong addr space for %s. Likely bug or overlapping write",
            what);
        exit(1);
    }
}

void vram_write(uint16_t addr, uint8_t val)
{
    check_addr(0x8000u, 0x9fffu, addr, "vram8");
    vram[addr - 0x8000u] = val;
}

void vram_write16(uint16_t addr, uint16_t val)
{
    check_addr(0x8000u, 0x9ffeu, addr, "vram16");
    *((uint16_t *) (vram + (addr - 0x8000u))) = val;
}

void wram_write(uint16_t addr, uint8_t val)
{
    check_addr(0xc000u, 0xdfffu, addr, "wram8");
    wram[addr - 0xc0000u] = val;
}

void wram_write16(uint16_t addr, uint16_t val)
{
    check_addr(0xc000u, 0xdffeu, addr, "wram16");
    *((uint16_t *) (wram + (addr - 0xc0000u))) = val;
}

void hram_write(uint16_t addr, uint8_t val)
{
    check_addr(0xff80u, 0xfffeu, addr, "hram8");
    hram[addr - 0xff80u] = val;
}

void hram_write16(uint16_t addr, uint16_t val)
{
    check_addr(0xff80u, 0xfffd, addr, "hram16");
    *((uint16_t *) (hram + (addr - 0xff80u))) = val;
}

uint8_t vram_read(uint16_t addr)
{
    check_addr(0x8000u, 0x9fffu, addr, "vram8");
    return vram[addr - 0x8000u];
}

uint8_t wram_read(uint16_t addr)
{
    check_addr(0xc000u, 0xdfffu, addr, "wram8");
    return wram[addr - 0xc0000u];
}

uint8_t hram_read(uint16_t addr)
{
    check_addr(0xff80u, 0xfffeu, addr, "hram8");
    return hram[addr - 0xff80u];
}
