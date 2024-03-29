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

static uint8_t lcdc;

// cycles left in dma transfer (if -1, transfer is not active)
static int16_t dma_dots;
// specifies src address in rom.
static uint16_t dma_addr;

static uint8_t scx;
static uint8_t scy;

static uint8_t wx;
static uint8_t wy;

static bool enable;
static bool disable;
// interrupt enable master flag
static bool ime;

//0xffff consider organizeing bettr
static uint8_t ie;

static uint8_t rif;

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

    scx = 0; // change later ? NOTE
    scy = 0;

    wx = 0;
    wy = 0;

    ime = false;
    enable = false;

    dma_dots = -1;
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
        case 0xff42:
            return scy;
        case 0xff43:
            return scx;
        case 0xff4a:
            return wy;
        case 0xff4b:
            return wx;
        case 0xff0f:
            return rif & 0x1f;
        case 0xffff:
            return ie & 0x1f;
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
        case 0xff40:
            lcdc = val;
            break;
        case 0xff42:
            scy = val;
            break;
        case 0xff43:
            scx = val;
            break;
        case 0xff46:
            // dma transfer
            dma_dots = 160;
            break;
        case 0xff4a:
            wy = val;
            break;
        case 0xff4b:
            wx = val;
            break;
        case 0xff50u:
            iomem_stat.gbboot_enable = (val == 0);
            break;
        case 0xff0fu:
            rif = (val & 0x1fu);
            break;
        case 0xffffu:
            ie = (val & 0x1fu);
            break;
        default:
            break;
    }
    // if (addr == 0xff50u) {
    //     // boot rom disable if value being written is not 0.
    //     iomem_stat.gbboot_enable = (val == 0);
    // } what is this for? are you fucking stupid?
    
    // if (addr == 0xff00u) {
        
    // }
}

/*
 * who knows what else this might be useful for.
 * call it in cpu step
 */
// void iomem_update(uint8_t dots)
// {
//     // for dma we do 1 byte per dot it seems
//     // from dma_addr in rom to fe00 in oam
//     if (dma_dots != -1) {
//         dma_dots -= dots;
//         if (dma_dots <= 0) {
//             // transfer dma_dots bytes
//         } else {
//             // transfer dots bytes
//         }
//     }
// }

// decreses by positive amt.
void iomem_dma_dots_dec(uint8_t n)
{
    dma_dots -= (int) n;
    if (dma_dots < 0) {
        dma_dots = -1;
    }
}

int16_t iomem_dma_dots()
{
    return dma_dots;
}

uint16_t iomem_dma_addr()
{
    return dma_addr;
}

// FUCK this one
// void iomem_dma_copy(const uint8_t *src)
// {
//     //memcpy()
// }

bool iomem_gbboot_enable()
{
    return iomem_stat.gbboot_enable;
}

void iomem_ei()
{
    if (disable)
        disable = false;
    enable = true;
}

void iomem_di()
{
    if (enable)
        enable = false;
    disable = true;
}

void iomem_update()
{
    // should not both be true;
    if (enable)
        ime = true;
    else if (disable)
        ime = false;
}