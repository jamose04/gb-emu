#include "ops.h"
#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

// NOTE: we don't really use the read and write functions for doing such
// operations on registers. Might remove later.

/* Start: 8-bit memory operations */

// NOTE: for neither of the following two functions shall i_dst or i_src
// be equal to 6 (this is flag).
void op_ld_rr(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t i_src = cpu->x_insbits & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = reg8[i_src];
}

void op_ld_rn(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = n_byte;
}

void op_ld_rhl(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    *((uint8_t *) cpu->reg.registers + i_dst)
        = mem_read(cpu->reg.registers[HL]);
}

void op_ld_hlr(cpu_state_t *cpu)
{
    uint8_t i_src = cpu->x_insbits & 0x07u;
    mem_write(cpu->reg.registers[HL], 
        *((uint8_t *) cpu->reg.registers + i_src));
}

void op_ld_hln(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    mem_write(cpu->reg.registers[HL], n_byte);
}

void op_ld_abc(cpu_state_t *cpu)
{
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(cpu->reg.registers[BC]);
}

void op_ld_ade(cpu_state_t *cpu)
{
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(cpu->reg.registers[DE]);
}

void op_ld_bca(cpu_state_t *cpu)
{
    mem_write(cpu->reg.registers[BC], 
        *((uint8_t *) (cpu->reg.registers + FA) + 1));
}

void op_ld_dea(cpu_state_t *cpu)
{
    mem_write(cpu->reg.registers[DE],
        *((uint8_t *) (cpu->reg.registers + FA) + 1));
}

void op_ld_ann(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t nn_byte = mem_read((cpu->reg.registers[PC])++);
    uint16_t nn = (n_byte << 8) | nn_byte;
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(nn);
}

void op_ld_nna(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t nn_byte = mem_read((cpu->reg.registers[PC])++);
    uint16_t nn = (n_byte << 8) | nn_byte;
    mem_write(nn, *((uint8_t *) (cpu->reg.registers + FA) + 1));
}

void op_ldh_ac(cpu_state_t *cpu)
{
    //register c + 0xff00
    uint8_t val = mem_read(0xff00u + (cpu->reg.registers[BC] & 0xffu));
    *((uint8_t *) (cpu->reg.registers + FA) + 1) = val;
}

void op_ldh_ca(cpu_state_t *cpu)
{
    uint16_t addr = 0xff00u + (cpu->reg.registers[BC] & 0xffu);
    mem_write(addr, *((uint8_t *) (cpu->reg.registers + FA) + 1));
}

void op_ldh_an(cpu_state_t *cpu)
{
    uint8_t val = mem_read(0xff00u + reg_read_lo(BC, &cpu->reg));
    reg_write_lo(FA, val, &cpu->reg);
}

void op_ldh_na(cpu_state_t *cpu)
{
    uint16_t addr = 0xff00u + reg_read_lo(BC, &cpu->reg);
    mem_write(addr, reg_read_lo(FA, &cpu->reg));
}

void op_ld_ahl_d(cpu_state_t *cpu)
{
    uint8_t memval = mem_read(cpu->reg.registers[HL]);
    reg_write_lo(FA, memval, &cpu->reg);
}

/* End: 8-bit memory operations */

const void (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu) =
    {
		op_ld_rr,
        op_ld_rn,
        op_ld_rhl,
        op_ld_hlr,
        op_ld_hln,
        op_ld_abc,
        op_ld_ade,
        op_ld_bca,
        op_ld_dea,
        op_ld_ann,
        op_ld_nna,
    };
