#include "register.h"

#include <stdio.h>
#include <stdlib.h>

byte_t read_hi(uint16_t reg)
{
    return reg >> 8;
}

byte_t read_lo(uint16_t reg)
{
    return reg & 0xffu;
}

/*
 * Write to the hi part of register selected by sel.
 * Note that for SP and PC, all 16-bits must be written, so this and
 * write_lo cannot be used for those.
 */
void write_hi(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val)
{
    uint16_t wval = ((uint16_t) val) << 8;
    switch (sel) {
        case AF:
            cpu_reg->AF = (cpu_reg->AF & 0xffu) | wval;
            break;
        case BC:
            cpu_reg->BC = (cpu_reg->BC & 0xffu) | wval;
            break;
        case DE:
            cpu_reg->DE = (cpu_reg->DE & 0xffu) | wval;
            break;
        case HL:
            cpu_reg->HL = (cpu_reg->DE & 0xffu) | wval;
            break;
        default:
            fprintf(stderr, "Invalid hi write to Register: %d\n", sel);
            exit(1);
    }
}

/*
 * Write to the lo bits of register sel.
 * Note that to write to F, we use a different function.
 * In other words, flags cannot be set using this interface.
 */
void write_lo(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val)
{
    uint16_t wval = (uint16_t) val;
    switch (sel) {
        case BC:
            cpu_reg->BC = (cpu_reg->BC & ~0xffu) | wval;
            break;
        case DE:
            cpu_reg->DE = (cpu_reg->DE & ~0xffu) | wval;
            break;
        case HL:
            cpu_reg->HL = (cpu_reg->HL & ~0xffu) | wval;
            break;
        default:
            fprintf(stderr, "Invalid lo write to Register: %d\n", sel);
            exit(1);
    }
}

/*
 * Print registers for debugging info.
 */
void print_registers(const cpu_reg_t *cpu_reg)
{
    printf("REGISTER STATE:\n");
    printf("AF: %x\n", cpu_reg->AF);
    printf("BC: %x\n", cpu_reg->BC);
    printf("DE: %x\n", cpu_reg->DE);
    printf("HL: %x\n", cpu_reg->HL);
    printf("SP: %x\n", cpu_reg->SP);
    printf("PC: %x\n", cpu_reg->PC);
}