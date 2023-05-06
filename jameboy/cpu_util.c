#include "cpu_util.h"
// lookup tables.
#include "tab.h"
#include "cbtab.h"
#include "memory.h"

opcode_t get_op_inc(uint16_t *pc)
{
    if (*pc != 0xcb) {
        return instr_lookup[imem_get((*pc)++)];
    } else {
        (*pc)++;
        return cb_instr_lookup[imem_get((*pc)++)];
    }
}

// NOTE: f is flag.
uint8_t alu(alu_op_t alu_op, uint8_t src1, uint8_t src2, uint8_t *f)
{
    switch (alu_op) {
        case ALU_NOP:
            // In this case, return value is unimportant. Leave flags unchanged
            return 0;
        case ALU_PASS:
            return src1;
        case ALU_ADD:
            return src1 + src2;
        case ALU_SUB:
            return src1 - src2;
    }
}

void write_register(uint16_t *reg, reg_access_mode_t mode, uint16_t data)
{
    switch (mode) {
        case RA_HI:
            data <<= 8;
            *reg = (*reg & 0x00ffu) | data;
            break;
        case RA_LO:
            data &= 0xffu;
            *reg = (*reg & 0xff00u) | data;
            break;
        case RA_WH:
            *reg = data;
            break;
    }
}

uint16_t read_register(uint16_t reg, reg_access_mode_t mode)
{
    switch (mode) {
        case RA_HI:
            return reg >> 8;
        case RA_LO:
            return reg & 0xffu;
        case RA_WH:
            return reg;
    }
}