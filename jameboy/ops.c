#include "ops.h"
#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

#include <stdlib.h>

#define REGS(RR) cpu->reg.registers[RR]

//extern cpu_state_t cpu;
// Maybe use this later
// then:
// reg16_t *regs = cpu->reg.registers;
// then we can do regs[RR] rather than cpu->reg.registers[RR];

// NOTE: Probably move this later. Why?
static uint16_t get_inc_pc(cpu_state_t *cpu)
{
    uint16_t pc = reg16_toi(cpu->reg.registers[PC]);
    reg_write16(pc + 1, PC, &cpu->reg);
    return pc;
}

/* Start: 8-bit memory operations */

int op_unimp(cpu_state_t *cpu)
{
    return 0;
}

int op_nop(cpu_state_t *cpu)
{
    return 1;
}

int op_hlt(cpu_state_t *cpu)
{
    return 1;
}

// NOTE: for neither of the following two functions shall i_dst or i_src
// be equal to 6 (this is flag).
int op_ld_rr(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t i_src = cpu->x_insbits & 0x07u;
    *reg8_at(i_dst, &cpu->reg) = *reg8_at(i_src, &cpu->reg);
    return 1;
}

int op_ld_rn(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read(get_inc_pc(cpu));
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    *reg8_at(i_dst, &cpu->reg) = n_byte;
    return 2;
}

int op_ld_rhl(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    *reg8_at(i_dst, &cpu->reg) = mem_read(reg16_toi(cpu->reg.registers[HL]));
    return 2;
}

int op_ld_hlr(cpu_state_t *cpu)
{
    uint8_t i_src = cpu->x_insbits & 0x07u;
    mem_write(reg16_toi(cpu->reg.registers[HL]), *reg8_at(i_src, &cpu->reg));
    return 2;
}

int op_ld_hln(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read(get_inc_pc(cpu));
    mem_write(reg16_toi(cpu->reg.registers[HL]), n_byte);
    return 3;
}

int op_ld_abc(cpu_state_t *cpu)
{
    cpu->reg.registers[AF].hi = mem_read(reg16_toi(cpu->reg.registers[BC]));
    return 2;
}

int op_ld_ade(cpu_state_t *cpu)
{
    cpu->reg.registers[AF].hi = mem_read(reg16_toi(cpu->reg.registers[DE]));
    return 2;
}

int op_ld_bca(cpu_state_t *cpu)
{
    mem_write(reg16_toi(cpu->reg.registers[BC]), cpu->reg.registers[AF].hi);
    return 2;
}

int op_ld_dea(cpu_state_t *cpu)
{
    mem_write(reg16_toi(cpu->reg.registers[DE]), cpu->reg.registers[AF].hi);
    return 2;
}

int op_ld_ann(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(get_inc_pc(cpu));
    uint8_t nn_byte = mem_read(get_inc_pc(cpu));
    uint16_t nn = (n_byte << 8) | nn_byte;
    cpu->reg.registers[AF].hi = mem_read(nn);
    return 4;
}

int op_ld_nna(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(get_inc_pc(cpu));
    uint8_t nn_byte = mem_read(get_inc_pc(cpu));
    uint16_t nn = (n_byte << 8) | nn_byte;
    mem_write(nn, cpu->reg.registers[AF].hi);
    return 4;
}

int op_ldh_ac(cpu_state_t *cpu)
{
    //register c + 0xff00
    cpu->reg.registers[AF].hi = mem_read(0xff00u + cpu->reg.registers[BC].lo);
    return 2;
}

int op_ldh_ca(cpu_state_t *cpu)
{
    mem_write(0xff00u + cpu->reg.registers[BC].lo, cpu->reg.registers[AF].hi);
    return 2;
}

int op_ldh_an(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    cpu->reg.registers[AF].hi = mem_read(0xff00u + n);
    return 3;
}

int op_ldh_na(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    mem_write(0xff00u + n, cpu->reg.registers[AF].hi);
    return 3;
}

// helper!
void op_ld_ahl(cpu_state_t *cpu)
{
    cpu->reg.registers[AF].hi = mem_read(reg16_toi(cpu->reg.registers[HL]));
}

// helper!
void op_ld_hla(cpu_state_t *cpu)
{
    mem_write(reg16_toi(cpu->reg.registers[HL]), cpu->reg.registers[AF].hi);
}

int op_ld_ahl_d(cpu_state_t *cpu)
{
    op_ld_ahl(cpu);
    reg_write16(reg16_toi(cpu->reg.registers[HL]) - 1, HL, &cpu->reg);
    return 2;
}

int op_ld_hla_d(cpu_state_t *cpu)
{
    op_ld_hla(cpu);
    reg_write16(reg16_toi(cpu->reg.registers[HL]) - 1, HL, &cpu->reg);
    return 2;
}

int op_ld_ahl_p(cpu_state_t *cpu)
{
    op_ld_ahl(cpu);
    reg_write16(reg16_toi(cpu->reg.registers[HL]) + 1, HL, &cpu->reg);
    return 2;
}

int op_ld_hla_p(cpu_state_t *cpu)
{
    op_ld_hla(cpu);
    reg_write16(reg16_toi(cpu->reg.registers[HL]) + 1, HL, &cpu->reg);
    return 2;
}

/* End: 8-bit memory operations */

/* Begin: 16-bit memory operations */

int map_rri(int rri)
{
    switch (rri) {
        case 0:
            return BC;
        case 1:
            return DE;
        case 2:
            return HL;
        case 3:
            return SP;
        default:
            fprintf(stderr, "<ERROR> Invalid RR selection...\n");
            exit(1);
    }
}

int op_ld_rr_nn(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(get_inc_pc(cpu));
    uint8_t nn_byte = mem_read(get_inc_pc(cpu));
    uint16_t nn = (n_byte << 8) | nn_byte;
    uint8_t rri = map_rri(cpu->x_insbits >> 4);
    
    reg_write16(nn, rri, &cpu->reg);
    return 4;
}

int op_ld_nn_sp(cpu_state_t *cpu)
{
    uint16_t n_byte = mem_read(get_inc_pc(cpu));
    uint8_t nn_byte = mem_read(get_inc_pc(cpu));
    uint16_t addr = (n_byte << 8) | nn_byte;
    mem_write16(addr, reg16_toi(cpu->reg.registers[SP]));
    
    return 5;
}

int op_ld_sp_hl(cpu_state_t *cpu)
{
    reg_write16(reg16_toi(cpu->reg.registers[HL]), SP, &cpu->reg);
    return 2;
}

static int map_rri_p(uint8_t irr)
{
    switch (irr) {
        case 0:
            return BC;
        case 1:
            return DE;
        case 2:
            return HL;
        case 3:
            return AF;
        default:
            fprintf(stderr, "<ERROR> Invalid RR selection (bug)...\n");
            exit(1);
    }
}

int op_push_rr(cpu_state_t *cpu)
{
    reg_write16(reg16_toi(cpu->reg.registers[SP]) - 2, SP, &cpu->reg);
    uint8_t irr = map_rri_p((cpu->x_insbits >> 4) & 0x3u);
    uint16_t wval = reg16_toi(cpu->reg.registers[irr]);
    mem_write16(reg16_toi(cpu->reg.registers[SP]), wval);
    
    return 4;
}

int op_pop_rr(cpu_state_t *cpu)
{
    uint8_t irr = map_rri_p((cpu->x_insbits >> 4) & 0x3u);
    uint16_t rwval = mem_read(reg16_toi(cpu->reg.registers[SP]));
    
    reg_write16(rwval, irr, &cpu->reg);
    reg_write16(reg16_toi(cpu->reg.registers[SP]) + 2, SP, &cpu->reg);
    return 3;
}

/* End: 16-bit memory operations */

/* Begin: 8-bit arithmetic operations */

int op_add_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    cpu->reg.registers[AF].hi = 
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi,
        *reg8_at(ir, &cpu->reg), &cpu->reg.registers[AF].lo);

    return 1;
}

int op_add_hl(cpu_state_t *cpu)
{
    cpu->reg.registers[AF].hi = 
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi, 
        mem_read(reg16_toi(cpu->reg.registers[HL])), &cpu->reg.registers[AF].lo);

    return 2;
}

int op_add_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    cpu->reg.registers[AF].hi =
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi, n,
        &cpu->reg.registers[AF].lo);
    return 2;
}

static bool carry_enable(cpu_state_t *cpu)
{
    return (cpu->reg.registers[AF].lo >> 4) & 0x1u;
}

/* NOTE: As of now carry flag *might* be unreliable */
int op_adc_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    uint16_t tmp = *reg8_at(ir, &cpu->reg) + carry_enable(cpu);
    cpu->reg.registers[AF].hi =
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi, tmp,
        &cpu->reg.registers[AF].lo);
    return 1;
}

int op_adc_hl(cpu_state_t *cpu)
{
    uint16_t tmp = mem_read(reg16_toi(cpu->reg.registers[HL]))
    + carry_enable(cpu);
    cpu->reg.registers[AF].hi = 
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi, tmp,
        &cpu->reg.registers[AF].lo);
    return 2;
}

int op_adc_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    uint16_t tmp = n + carry_enable(cpu);
    REGS(AF).hi =
        (uint8_t) alu(ALU_ADD, cpu->reg.registers[AF].hi, tmp,
        &cpu->reg.registers[AF].lo);
    return 2;
}

int op_sub_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    uint16_t src2 = *reg8_at(ir, &cpu->reg);
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 1;
}

int op_sub_hl(cpu_state_t *cpu)
{
    uint16_t src2 = mem_read(reg16_toi(REGS(HL)));
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 2;
}

int op_sub_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, n, &REGS(AF).lo);
    return 2;
}

int op_sbc_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    uint16_t src2 = *reg8_at(ir, &cpu->reg) + carry_enable(cpu);
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 1;
}

int op_sbc_hl(cpu_state_t *cpu)
{
    uint16_t src2 = mem_read(reg16_toi(REGS(HL))) + carry_enable(cpu);
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 2;
}

int op_sbc_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    uint16_t src2 = n + carry_enable(cpu);
    REGS(AF).hi =
        (uint8_t) alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 2;
}

int op_cp_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    uint16_t src2 = *reg8_at(ir, &cpu->reg);
    alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 1;
}

int op_cp_hl(cpu_state_t *cpu)
{
    uint16_t src2 = mem_read(reg16_toi(REGS(HL)));
    alu(ALU_SUB, REGS(AF).hi, src2, &REGS(AF).lo);
    return 2;
}

int op_cp_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    alu(ALU_SUB, REGS(AF).hi, n, &REGS(AF).lo);
    return 2;
}

int op_inc_r(cpu_state_t *cpu)
{
    int ir = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t oldc = REGS(AF).lo & 0x10u;
    *reg8_at(ir, &cpu->reg) =
        (uint8_t) alu(ALU_ADD, *reg8_at(ir, &cpu->reg), 1, &REGS(AF).lo);
    REGS(AF).lo = (REGS(AF).lo & 0xefu) | oldc;
    return 1;
}

int op_and_r(cpu_state_t *cpu)
{
    uint8_t ir = cpu->x_insbits & 0x07u;
    uint16_t src2 = *reg8_at(ir, &cpu->reg);
    REGS(AF).hi =
        (uint8_t) alu(ALU_AND, REGS(AF).hi, src2, &REGS(AF).lo);
    return 1;
}

int op_and_hl(cpu_state_t *cpu)
{
    uint16_t src2 = mem_read(reg16_toi(REGS(HL))) + carry_enable(cpu);
    REGS(AF).hi =
        (uint8_t) alu(ALU_AND, REGS(AF).hi, src2, &REGS(AF).lo);
    return 2;
}

int op_and_n(cpu_state_t *cpu)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    REGS(AF).hi =
        (uint8_t) alu(ALU_AND, REGS(AF).hi, n, &REGS(AF).lo);
    return 1;
}

const int (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu) =
    {
        op_unimp,
        op_nop,
        op_hlt,
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