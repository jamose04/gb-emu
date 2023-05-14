#include "ops.h"
#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

#include <stdlib.h>

// NOTE: we don't really use the read and write functions for doing such
// operations on registers. Might remove later.

/* Start: 8-bit memory operations */

// NOTE: for neither of the following two functions shall i_dst or i_src
// be equal to 6 (this is flag).
int op_ld_rr(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t i_src = cpu->x_insbits & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = reg8[i_src];
    return 1;
}

int op_ld_rn(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = n_byte;
    return 2;
}

int op_ld_rhl(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    *((uint8_t *) cpu->reg.registers + i_dst)
        = mem_read(cpu->reg.registers[HL]);
    return 2;
}

int op_ld_hlr(cpu_state_t *cpu)
{
    uint8_t i_src = cpu->x_insbits & 0x07u;
    mem_write(cpu->reg.registers[HL], 
        *((uint8_t *) cpu->reg.registers + i_src));
    return 2;
}

int op_ld_hln(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC])++);
    mem_write(cpu->reg.registers[HL], n_byte);
    return 3;
}

int op_ld_abc(cpu_state_t *cpu)
{
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(cpu->reg.registers[BC]);
    return 2;
}

int op_ld_ade(cpu_state_t *cpu)
{
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(cpu->reg.registers[DE]);
    return 2;
}

int op_ld_bca(cpu_state_t *cpu)
{
    mem_write(cpu->reg.registers[BC], 
        *((uint8_t *) (cpu->reg.registers + FA) + 1));
    return 2;
}

int op_ld_dea(cpu_state_t *cpu)
{
    mem_write(cpu->reg.registers[DE],
        *((uint8_t *) (cpu->reg.registers + FA) + 1));
        
    return 2;
}

int op_ld_ann(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t nn_byte = mem_read((cpu->reg.registers[PC])++);
    uint16_t nn = (n_byte << 8) | nn_byte;
    *((uint8_t *) (cpu->reg.registers + FA) + 1)
        = mem_read(nn);
    return 4;
}

int op_ld_nna(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read((cpu->reg.registers[PC])++);
    uint8_t nn_byte = mem_read((cpu->reg.registers[PC])++);
    uint16_t nn = (n_byte << 8) | nn_byte;
    mem_write(nn, *((uint8_t *) (cpu->reg.registers + FA) + 1));
    return 4;
}

int op_ldh_ac(cpu_state_t *cpu)
{
    //register c + 0xff00
    uint8_t val = mem_read(0xff00u + (cpu->reg.registers[BC] & 0xffu));
    *((uint8_t *) (cpu->reg.registers + FA) + 1) = val;
    return 2;
}

int op_ldh_ca(cpu_state_t *cpu)
{
    uint16_t addr = 0xff00u + (cpu->reg.registers[BC] & 0xffu);
    mem_write(addr, *((uint8_t *) (cpu->reg.registers + FA) + 1));
    return 2;
}

int op_ldh_an(cpu_state_t *cpu)
{
    uint8_t val = mem_read(0xff00u + reg_read_lo(BC, &cpu->reg));
    reg_write_lo(FA, val, &cpu->reg);
    return 3;
}

int op_ldh_na(cpu_state_t *cpu)
{
    uint16_t addr = 0xff00u + reg_read_lo(BC, &cpu->reg);
    mem_write(addr, reg_read_lo(FA, &cpu->reg));
    return 3;
}

// helper!
void op_ld_ahl(cpu_state_t *cpu)
{
    uint8_t memval = mem_read(cpu->reg.registers[HL]);
    reg_write_lo(FA, memval, &cpu->reg);
}

// helper!
void op_ld_hla(cpu_state_t *cpu)
{
    uint16_t addr = cpu->reg.registers[HL];
    mem_write(addr, reg_read_lo(FA, &cpu->reg));
}

int op_ld_ahl_d(cpu_state_t *cpu)
{
    op_ld_ahl(cpu);
    cpu->reg.registers[HL]--;
    return 2;
}

int op_ld_hla_d(cpu_state_t *cpu)
{
    op_ld_hla(cpu);
    cpu->reg.registers[HL]--;
    return 2;
}

int op_ld_ahl_p(cpu_state_t *cpu)
{
    op_ld_ahl(cpu);
    cpu->reg.registers[HL]++;
    return 2;
}

int op_ld_hla_p(cpu_state_t *cpu)
{
    op_ld_hla(cpu);
    cpu->reg.registers[HL]++;
    return 2;
}

/* End: 8-bit memory operations */

/* Begin: 16-bit memory operations */

int op_ld_rr_nn(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(cpu->reg.registers[PC]++);
    uint8_t nn_byte = mem_read(cpu->reg.registers[PC]++);
    uint16_t wval = (n_byte << 8) | nn_byte;
    uint8_t ri = cpu->x_insbits >> 4;
    
    if (0 <= ri || ri <= 3) {
        fprintf(stderr, "<ERROR> Invalid RR write\n");
        exit(1);
    }
    
    if (ri == 3)
        ri = SP;
    cpu->reg.registers[ri] = wval;
    
    return 4;
}

int op_ld_nn_sp(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(cpu->reg.registers[PC]++);
    uint8_t nn_byte = mem_read(cpu->reg.registers[PC]++);
    uint16_t addr = (n_byte << 8) | nn_byte;
    mem_write16(addr, cpu->reg.registers[PC]);
    
    return 5;
}

int op_ld_sp_hl(cpu_state_t *cpu)
{
    cpu->reg.registers[SP] = cpu->reg.registers[HL];
    return 2;
}

int op_push_rr(cpu_state_t *cpu)
{
    cpu->reg.registers[SP] -= 2;
    uint8_t ir = (cpu->x_insbits >> 4) & 0x3u;
    uint16_t wval = cpu->reg.registers[ir];
    if (ir == 3) {
        uint16_t lo = wval & 0xffu;
        wval = (wval >> 8) | (lo << 8);
    }
    mem_write16(cpu->reg.registers[SP], wval);
    
    return 4;
}

int op_pop_rr(cpu_state_t *cpu)
{
    uint8_t ir = (cpu->x_insbits >> 4) & 0x3u;
    uint16_t rwval = mem_read(cpu->reg.registers[SP]++);
    if (ir == 3) {
        uint16_t lo = rwval & 0xffu;
        rwval = (lo << 8) | (rwval >> 8);
    }
    
    cpu->reg.registers[ir] = rwval;
    return 3;
}

/* End: 16-bit memory operations */

const int (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu) =
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
        op_ldh_ac,
        op_ldh_ca,
        op_ldh_an,
        op_ldh_na,
        op_ld_ahl_d,
        op_ld_hla_d,
        op_ld_ahl_p,
        op_ld_hla_p,
        op_ld_rr_nn,
        op_ld_nn_sp,
        op_ld_sp_hl,
        op_push_rr,
        op_pop_rr,
    };
