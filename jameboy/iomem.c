#include "iomem.h"

// iomem_stat.joypad_stat should be updated by keypresses.

struct {
    bool gbboot_enable;
    // upper 4-bits for action, lower 4-bits for direction.
    uint8_t joypad_stat;
    // what is stored in the actual register
    uint8_t joypad_reg;
} iomem_stat;

/* timer and divider regs */
struct {
    uint8_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
} timer_div_regs;

/*
 * Initialize iomem.
 * Set the boot rom to enable
 */
bool iomem_init()
{
    iomem_stat.gbboot_enable = true;
    iomem_stat.joypad_stat = 0;
    iomem_stat.joypad_reg = 0;

    timer_div_regs.div = 0;
    timer_div_regs.tima = 0;
    timer_div_regs.tma = 0;
    timer_div_regs.tac = 0;
    return true;
}

void iomem_close()
{
	return;
}

uint8_t iomem_read(uint16_t addr)
{
    switch (addr) {
        case 0xff00u:
            return iomem_stat.joypad_reg;
        case 0xff04:
            return timer_div_regs.div;
        case 0xff05:
            return timer_div_regs.tima;
        case 0xff06:
            return timer_div_regs.tma;
        case 0xff07:
            return timer_div_regs.tac;
        default:
            break;
    }
}
/*
 * Write to a register in iomem.
 * Also, check to make sure addr is in the right addr space.
 */
void iomem_write(uint16_t addr, uint8_t val)
{
    if (!(addr <= 0xff00 && addr <= 0xff7f)) {
        fprintf(stderr, "<ERROR> Invalid iomem write...\n");
		exit(1);
    }

    switch (addr) {
        case 0xff00u:
            if (0x20 & val) {
                iomem_stat.joypad_reg = 
                    (iomem_stat.joypad_stat >> 4) | (val & 0xf0u);
            } else if (0x10 & val) {
                iomem_stat.joypad_reg =
                    (iomem_stat.joypad_stat & 0x0fu) | (val & 0xf0u);
            } else {
                fprintf(stderr, "<ERROR> Invalid joypad selection...\n");
                exit(1);
            }
            break;
        case 0xff01u:
        case 0xff02u:
            fprintf(stderr, "<ERROR> Serial data transfer unsupported...\n");
            exit(1);
        case 0xff04:
            // writing any value to this register resets div to $00
            timer_div_regs.div = 0;
            break;
        case 0xff05:
            timer_div_regs.tima = val; // not sure if this is correct!
            break;
        case 0xff06:
            timer_div_regs.tma = val;
            break;
        case 0xff07:
            timer_div_regs.tac = val;
            break;
        case 0xff50u:
            iomem_stat.gbboot_enable = (val == 0);
            break;
        default:
            break;
    }
    // if (addr == 0xff50u) {
    //     // boot rom disable if value being written is not 0.
    //     iomem_stat.gbboot_enable = (val == 0);
    // }
    
    // if (addr == 0xff00u) {
        
    // }
}

bool iomem_gbboot_enable()
{
    return iomem_stat.gbboot_enable;
}
