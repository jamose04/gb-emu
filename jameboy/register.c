#include "register.h"

#include <stdio.h>
#include <stdlib.h>

uint16_t reg16_toi(reg16_t r)
{
    return ((uint16_t) r.hi) << 8 | r.lo;
}

void reg_write16(uint16_t val, reg_sel_t sel, cpu_reg_t *reg)
{
    uint8_t hi = (uint8_t) (val >> 8);
    uint8_t lo = (uint8_t) (val & 0x00ffu);
    reg->registers[sel].hi = hi;
    reg->registers[sel].lo = lo;
}

uint8_t *reg8_at(uint8_t i, cpu_reg_t *reg)
{
    switch (i) {
        case 0:
            return &reg->registers[BC].hi;
        case 1:
            return &reg->registers[BC].lo;
        case 2:
            return &reg->registers[DE].hi;
        case 3:
            return &reg->registers[DE].lo;
        case 4:
            return &reg->registers[HL].hi;
        case 5:
            return &reg->registers[HL].lo;
        case 7:
            return &reg->registers[AF].hi;
        default:
            fprintf(stderr, "<ERROR> Invalid register access...\n");
            exit(1);
    }
}
/*
 * Print registers for debugging info.
 */
void print_registers(const cpu_reg_t *cpu_reg)
{
    printf("REGISTER STATE:\n");
    printf("AF: %x\n", cpu_reg->registers[AF]);
    printf("BC: %x\n", cpu_reg->registers[BC]);
    printf("DE: %x\n", cpu_reg->registers[DE]);
    printf("HL: %x\n", cpu_reg->registers[HL]);
    printf("SP: %x\n", cpu_reg->registers[SP]);
    printf("PC: %x\n", cpu_reg->registers[PC]);
}
