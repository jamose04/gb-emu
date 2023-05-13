#include "register.h"

#include <stdio.h>
#include <stdlib.h>

/*** BEGIN DEPRECATED FUNCTIONS ***/

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
void write_hi(cpu_reg_t *cpu_reg, reg_sel_t sel, byte_t val)
{
    uint16_t wval = ((uint16_t) val) << 8;
    cpu_reg->registers[sel] = (cpu_reg->registers[sel] & 0xffu) | wval;
}

/*
 * Write to the lo bits of register sel.
 */
void write_lo(cpu_reg_t *cpu_reg, reg_sel_t sel, byte_t val)
{
    uint16_t wval = (uint16_t) val;
    cpu_reg->registers[sel] = (cpu_reg->registers[sel] & ~0xffu) | wval;
}

/*** END DEPRECATED FUNCTIONS ***/

/*
 *  NOTE: It is important to realize that for convenience, we have a 16-bit
 * register called FA instead of AF. This means the the 8 hi bits of this
 * register correspont to F, while the lo 8 bits correspond to A. This is
 * reverse of the actual hardware.
 */
uint8_t reg_read_hi(reg_sel_t sel, const cpu_reg_t *reg)
{
	return reg->registers[sel] >> 8;
}

uint8_t reg_read_lo(reg_sel_t sel, const cpu_reg_t *reg)
{
	return reg->registers[sel] & 0xffu;
}

void reg_write_hi(reg_sel_t sel, uint8_t val, cpu_reg_t *reg)
{
	*((uint8_t *) (reg->registers + sel)) = val;
}

void reg_write_lo(reg_sel_t sel, uint8_t val, cpu_reg_t *reg)
{
	*((uint8_t *) (reg->registers + sel) + 1) = val;
}

/*
 * Print registers for debugging info.
 */
void print_registers(const cpu_reg_t *cpu_reg)
{
    printf("REGISTER STATE:\n");
    printf("FA: %x\n", cpu_reg->registers[FA]);
    printf("BC: %x\n", cpu_reg->registers[BC]);
    printf("DE: %x\n", cpu_reg->registers[DE]);
    printf("HL: %x\n", cpu_reg->registers[HL]);
    printf("SP: %x\n", cpu_reg->registers[SP]);
    printf("PC: %x\n", cpu_reg->registers[PC]);
}
