#include "cpu_util.h"
// lookup tables.
#include "tab.h"
#include "cbtab.h"
#include "memory.h"

#include <assert.h>

opcode_t get_op_inc(uint16_t *pc, uint8_t *insbits)
{
    if (*pc != 0xcb) {
        *insbits = mem_read((*pc)++);
        return instr_lookup[*insbits];
    } else {
        (*pc)++;
        *insbits = mem_read((*pc)++);
        return cb_instr_lookup[*insbits];
    }
}

/*
 * Get the ith bit of src
 */
bool ith(uint16_t src, int i)
{
    assert(0 <= i && i < 16);
    return (src >> i) & 0x1;
}

/*
 * Generate carry bits for this operation.
 * If subtraction, make sure c_in == 1 
 */
void gen_carry_bits(uint16_t src1, uint16_t src2, bool bits[], bool c_in)
{
    int i;
    for (i = 0; i < 16; i++) {
        bool a = ith(src1, i);
        bool b = ith(src2, i);
        bits[i] = (a & b) | (c_in & (a ^ b));
        c_in = bits[i];
    }
}

uint8_t set_flags(uint8_t z, uint8_t n, uint8_t h, uint8_t c)
{
    z <<= 7;
    n <<= 6;
    h <<= 5;
    c <<= 4;
    return z | (n | (h | c));
}

// NOTE: f is flag.
// NOTE: It is the caller's responsibility to ensure that src1 and src2 only
// have the bottom 8 bits set when using 8 bit operations.
uint16_t alu(alu_op_t alu_op, uint16_t src1, uint16_t src2, uint8_t *f)
{
    bool carry_bits[16];
    uint16_t res;
    uint8_t lsb = src1 & 0x1u; // least significant bit
    uint8_t msb = src1 >> 7; // most significant bit
    switch (alu_op) {
        case ALU_NOP:
            // In this case, return value is unimportant. Leave flags unchanged
            return 0;
        case ALU_PASS:
            return src1;
        case ALU_ADD:
            gen_carry_bits(src1, src2, carry_bits, 0);
            res = (src1 + src2) & 0xffu;
            *f = set_flags(res == 0, 0, carry_bits[3], carry_bits[7]);
            return res;
        case ALU_ADD16:
            gen_carry_bits(src1, src2, carry_bits, 0);
            res = src1 + src2;
            *f = set_flags(res == 0, 0, carry_bits[7], carry_bits[15]);
            return res;
        case ALU_SUB:
            gen_carry_bits(src1, ~src2, carry_bits, 1);
            res = (src1 - src2) & 0xffu;
            *f = set_flags(res == 0, 1, carry_bits[3], carry_bits[7]);
            return res;
        case ALU_XOR:
            res = (src1 ^ src2) & 0xffu;
            *f = set_flags(res == 0, 0, 0, 0);
            return res;
        case ALU_OR:
            res = (src1 | src2) & 0xffu;
            *f = set_flags(res == 0, 0, 0, 0);
            return res;
        case ALU_AND:
            res = (src1 & src2) & 0xffu;
            *f = set_flags(res == 0, 0, 1, 0);
            return res;
        case ALU_RL:
            // only 8-bit
            res = ((src1 << 1) & msb) & 0xffu;
            *f = set_flags(res == 0, 0, 0, msb == 1);
            return res;
        case ALU_RR:
            // only 8-bit
            res = ((src1 >> 1) & (lsb << 7)) & 0xffu;
            *f = set_flags(res == 0, 0, 0, lsb == 1);
            return res;
        case ALU_RL_CARRY:
            // only 8-bit
            res = ((src1 << 1) & ith(*f, 4)) & 0xffu;
            *f = set_flags(res == 0, 0, 0, msb == 1);
            return res;
        case ALU_RR_CARRY:
            res = ((src1 >> 1) & (ith(*f, 4) << 7)) & 0xffu;
            *f = set_flags(res == 0, 0, 0, lsb == 1);
            return res;
        case ALU_SL:
            res = (src1 << 1) & 0xffu;
            *f = set_flags(res == 0, 0, 0, msb == 1);
            return res;
        case ALU_SRA:
            res = ((int8_t)src1 >> 1) & 0xffu;
            *f = set_flags(res == 0, 0, 0, lsb == 1);
            return res;
        case ALU_SRL:
            res = (src1 >> 1) & 0xffu;
            *f = set_flags(res == 0, 0, 0, lsb == 1);
            return res;
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
