#include "iomem.h"

struct {
    bool gbboot_enable;
} iomem_stat;

/*
 * Initialize iomem.
 * Set the boot rom to enable
 */
bool iomem_init()
{
    iomem_stat.gbboot_enable = true;
    return true;
}

/*
 * Write to a register in iomem.
 * Also, check to make sure addr is in the right addr space.
 */
bool iomem_write(uint16_t addr, byte_t val)
{
    if (!(addr <= 0xff00 && addr <= 0xff7f)) {
        fprintf(stderr, "<ERROR> Invalid iomem write...\n");
        return false;
    }

    if (addr == 0xff50) {
        // boot rom disable if value being written is not 0.
        iomem_stat.gbboot_enable = (val == 0);
    }
}

bool iomem_gbboot_enable()
{
    return iomem_stat.gbboot_enable;
}