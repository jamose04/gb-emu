#include "ops.h"
#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

#include <stdlib.h>
#include <assert.h>

#define REGS(RR) cpu->reg.registers[RR]

#define IREG(RR) reg16_toi(REGS(RR))

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

static uint8_t get_rindex(uint8_t insbits, int start)
{
    return (insbits >> start) & 0x07u;
}

static int arith_r(cpu_state_t *cpu, alu_op_t alu_op)
{
    uint8_t ir = get_rindex(cpu->x_insbits, 0);
    REGS(AF).hi =
        (uint8_t) alu(alu_op, REGS(AF).hi, *reg8_at(ir, &cpu->reg), &REGS(AF).lo);

    return 1;
}

static int arith_hl(cpu_state_t *cpu, alu_op_t alu_op)
{
    uint8_t src2 = mem_read(IREG(HL));
    REGS(AF).hi =
        (uint8_t) alu(alu_op, REGS(AF).hi, src2, &REGS(AF).lo);
    
    return 2;
}

static int arith_n(cpu_state_t *cpu, alu_op_t alu_op)
{
    uint8_t n = mem_read(get_inc_pc(cpu));
    REGS(AF).hi =
        (uint8_t) alu(alu_op, REGS(AF).hi, n, &REGS(AF).lo);
    
    return 2;
}

int op_add_r(cpu_state_t *cpu)
{
    return arith_r(cpu, ALU_ADD);
}

int op_add_hl(cpu_state_t *cpu)
{
    return arith_hl(cpu, ALU_ADD);
}

int op_add_n(cpu_state_t *cpu)
{
    return arith_n(cpu, ALU_ADD);
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
    return arith_r(cpu, ALU_SUB);
}

int op_sub_hl(cpu_state_t *cpu)
{
    return arith_hl(cpu, ALU_SUB);
}

int op_sub_n(cpu_state_t *cpu)
{
    return arith_n(cpu, ALU_SUB);
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

static int incdec_r(cpu_state_t *cpu, alu_op_t alu_op)
{
    assert(alu_op == ALU_ADD || alu_op == ALU_SUB);
    int ir = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t oldc = REGS(AF).lo & 0x10u;
    *reg8_at(ir, &cpu->reg) =
        (uint8_t) alu(alu_op, *reg8_at(ir, &cpu->reg), 1, &REGS(AF).lo);
    REGS(AF).lo = (REGS(AF).lo & 0xefu) | oldc;
    return 1;
}

static int incdec_hl(cpu_state_t *cpu, alu_op_t alu_op)
{
    assert(alu_op == ALU_ADD || alu_op == ALU_SUB);
    uint8_t oldc = REGS(AF).lo & 0x10u;
    uint8_t src1 = mem_read(reg16_toi(REGS(HL)));
    uint8_t wval =
        (uint8_t) alu(alu_op, src1, 1, &REGS(AF).lo);
    mem_write(reg16_toi(REGS(HL)), wval);
    REGS(AF).lo = (REGS(AF).lo & 0xefu) | oldc;
    return 3;
}

int op_inc_r(cpu_state_t *cpu)
{
    return incdec_r(cpu, ALU_ADD);
}

int op_inc_hl(cpu_state_t *cpu)
{
    return incdec_hl(cpu, ALU_ADD);
}

int op_dec_r(cpu_state_t *cpu)
{
    return incdec_r(cpu, ALU_SUB);
}

int op_dec_hl(cpu_state_t *cpu)
{
    return incdec_hl(cpu, ALU_SUB);
}

int op_and_r(cpu_state_t *cpu)
{
    return arith_r(cpu, ALU_AND);
}

int op_and_hl(cpu_state_t *cpu)
{
    return arith_hl(cpu, ALU_AND);
}

int op_and_n(cpu_state_t *cpu)
{
    return arith_n(cpu, ALU_AND);
}

int op_or_r(cpu_state_t *cpu)
{
    return arith_r(cpu, ALU_OR);
}

int op_or_hl(cpu_state_t *cpu)
{
    return arith_hl(cpu, ALU_OR);
}

int op_or_n(cpu_state_t *cpu)
{
    return arith_n(cpu, ALU_OR);
}

int op_xor_r(cpu_state_t *cpu)
{
    return arith_r(cpu, ALU_XOR);
}

int op_xor_hl(cpu_state_t *cpu)
{
    return arith_hl(cpu, ALU_XOR);
}

int op_xor_n(cpu_state_t *cpu)
{
    return arith_n(cpu, ALU_XOR);
}

int op_ccf(cpu_state_t *cpu)
{
    uint8_t oldf = REGS(AF).lo;
    REGS(AF).lo = (((REGS(AF).lo ^ 0x10u) & 0x10u) | (oldf & 0xefu)) & 0x90u;
    return 1;
}

int op_scf(cpu_state_t *cpu)
{
    REGS(AF).lo = (REGS(AF).lo | 0x10u) & 0x90u;
    return 1;
}

int op_daa(cpu_state_t *cpu)
{
    return 1;
}

int op_cpl(cpu_state_t *cpu)
{
    REGS(AF).hi = REGS(AF).hi ^ 0xffu;
    REGS(AF).lo |= 0x60u;
    return 1;
}

/* Begin 16-bit arithmetic/logic ops */

int op_add_hl_rr(cpu_state_t *cpu)
{
    int rri = map_rri((cpu->x_insbits >> 4) & 0x3u);
    reg_write16(alu(ALU_ADD16, IREG(HL), IREG(rri), &REGS(AF).lo),
        HL, &cpu->reg);
    return 2;
}

int op_inc_rr(cpu_state_t *cpu)
{
    int rri = map_rri((cpu->x_insbits >> 4) & 0x3u);
    reg_write16(IREG(rri) + 1, rri, &cpu->reg);
    return 2;
}

int op_dec_rr(cpu_state_t *cpu)
{
    int rri = map_rri((cpu->x_insbits >> 4) & 0x3u);
    reg_write16(IREG(rri) - 1, rri, &cpu->reg);
    return 2;
}

int op_add_sp_nn(cpu_state_t *cpu)
{
    int8_t n = get_inc_pc(cpu);
    reg_write16(IREG(SP) + (int16_t) n, SP, &cpu->reg);
    return 4;
}

int op_ld_hl_sp_nn(cpu_state_t *cpu)
{
    int8_t n = get_inc_pc(cpu);
    reg_write16(IREG(SP) + (int16_t) n, HL, &cpu->reg);
    return 3;
}

/* End 16-bit arithmetic/logic ops */

/* Begin rotate and shift ops */

static int rotate_op(cpu_state_t *cpu, alu_op_t alu_op, uint8_t *reg)
{
    assert(alu_op == ALU_RL
        || alu_op == ALU_RL_CARRY
        || alu_op == ALU_RR
        || alu_op == ALU_RR_CARRY);

    *reg = (uint8_t) alu(alu_op, *reg, 0, &REGS(AF).lo);

    return 1;
}

int op_rlca(cpu_state_t *cpu)
{
    return rotate_op(cpu, ALU_RL, &REGS(AF).hi);
}

int op_rla(cpu_state_t *cpu)
{
    return rotate_op(cpu, ALU_RL_CARRY, &REGS(AF).hi);
}

int op_rrca(cpu_state_t *cpu)
{
    return rotate_op(cpu, ALU_RR, &REGS(AF).hi);
}

int op_rra(cpu_state_t *cpu)
{
    return rotate_op(cpu, ALU_RR_CARRY, &REGS(AF).hi);
}

static int sr_r(cpu_state_t *cpu, alu_op_t alu_op)
{
    assert(alu_op == ALU_RL
        || alu_op == ALU_RR
        || alu_op == ALU_RL_CARRY
        || alu_op == ALU_RR_CARRY
        || alu_op == ALU_SL
        || alu_op == ALU_SRA
        || alu_op == ALU_SRL
        || alu_op == ALU_SWAP);

    uint8_t ri = cpu->x_insbits & 0x7u;
    *reg8_at(ri, &cpu->reg)
        = (uint8_t) alu(alu_op, *reg8_at(ri, &cpu->reg), 0, &REGS(AF).lo);
    return 2;
}

int op_rlc_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_RL);
}

static int sr_hl(cpu_state_t *cpu, alu_op_t alu_op)
{
    assert(alu_op == ALU_RL
        || alu_op == ALU_RR
        || alu_op == ALU_RL_CARRY
        || alu_op == ALU_RR_CARRY
        || alu_op == ALU_SL
        || alu_op == ALU_SRA
        || alu_op == ALU_SRL
        || alu_op == ALU_SWAP);

    uint8_t src1 = mem_read(IREG(HL));
    uint8_t val = (uint8_t) alu(alu_op, src1, 0, &REGS(AF).lo);
    mem_write(IREG(HL), val);

    return 4;
}

int op_rlc_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_RL);
}

int op_rl_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_RL_CARRY);
}

int op_rl_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_RL_CARRY);
}

int op_rrc_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_RR);
}

int op_rrc_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_RR);
}

int op_rr_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_RR_CARRY);
}

int op_rr_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_RR_CARRY);
}

int op_sla_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_SL);
}

int op_sla_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_SL);
}

int op_swap_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_SWAP);
}

int op_swap_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_SWAP);
}

int op_sra_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_SRA);
}

int op_sra_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_SRA);
}

int op_srl_r(cpu_state_t *cpu)
{
    return sr_r(cpu, ALU_SRL);
}

int op_srl_hl(cpu_state_t *cpu)
{
    return sr_hl(cpu, ALU_SRL);
}

int op_bit_nr(cpu_state_t *cpu)
{
    uint8_t bindex = (cpu->x_insbits >> 3) & 0x7u;
    uint8_t rindex = get_rindex(cpu->x_insbits, 0);
    alu(ALU_BIT, *reg8_at(rindex, &cpu->reg), bindex, &REGS(AF).lo);
    return 2;
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
        op_add_r,
        op_add_hl,
        op_add_n,
        op_adc_r,
        op_adc_hl,
        op_adc_n,
        op_sub_r,
        op_sub_hl,
        op_sub_n,
        op_sbc_r,
        op_sbc_hl,
        op_sbc_n,
        op_cp_r,
        op_cp_hl,
        op_cp_n,
        op_inc_r,
        op_inc_hl,
        op_dec_r,
        op_dec_hl,
        op_and_r,
        op_and_hl,
        op_and_n,
        op_or_r,
        op_or_hl,
        op_or_n,
        op_xor_r,
        op_xor_hl,
        op_xor_n,
        op_ccf,
        op_scf,
        op_daa,
        op_cpl,
    };