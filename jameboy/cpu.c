#include "cpu.h"
#include "register.h"
#include "cpu_util.h"

#include <stdio.h>
#include <stdbool.h>

cpu_state_t cpu;

extern const int (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu);

/* SP will be set properly by the boot ROM */
void init_cpu()
{
    cpu.reg.registers[AF].hi = 0;
    cpu.reg.registers[BC].hi = 0;
    cpu.reg.registers[DE].hi = 0;
    cpu.reg.registers[HL].hi = 0;
    cpu.reg.registers[SP].hi = 0;
    cpu.reg.registers[PC].hi = 0;
    cpu.reg.registers[AF].lo = 0;
    cpu.reg.registers[BC].lo = 0;
    cpu.reg.registers[DE].lo = 0;
    cpu.reg.registers[HL].lo = 0;
    cpu.reg.registers[SP].lo = 0;
    cpu.reg.registers[PC].lo = 0;
    cpu.x_op = OP_NOP;
}

void print_cpu_state()
{
    printf("CPU STATE:\n");
    print_registers(&cpu.reg); 
}

alu_op_t get_alu_op()
{
    // special cases:
    // swap.
    // sub16.
    // all single bit operations.
    switch (cpu.x_op) {
        case OP_NOP:
        case OP_HLT:
        case OP_STOP:
        case OP_EI:
        case OP_DI:
        case OP_SCF:
            return ALU_NOP;
        case OP_LD_RR:
        case OP_LD_RN:
        case OP_LD_RHL:
        case OP_LD_HLR:
        case OP_LD_HLN:
        case OP_LD_ABC:
        case OP_LD_ADE:
        case OP_LD_BCA:
        case OP_LD_DEA:
        case OP_LD_Ann:
        case OP_LD_nnA:
        case OP_LD_RR_NN:
        case OP_LD_NN_SP:
        case OP_LD_SP_HL:
        case OP_PUSH_RR:
        case OP_POP_RR:
        case OP_JP_NN:
        case OP_JP_HL:
        case OP_JP_FNN:
        case OP_JR_PCDD:
        case OP_JR_F_PCDD:
        case OP_CALL_NN:
        case OP_CALL_FNN:
        case OP_RET:
        case OP_RET_F:
        case OP_RET_I:
        case OP_RST_N:
            // in each of these cases, we just need the first source.
            return ALU_PASS;
        case OP_LDH_AC:
        case OP_LDH_CA:
        case OP_LDH_An:
        case OP_LDH_nA:
            // note: the _d instructions are special cases because
            // there is no ALU_SUB16
            return ALU_ADD16;
        case OP_ADD_R:
        case OP_ADD_HL:
        case OP_ADD_n:
        case OP_ADC_R:
        case OP_ADC_HL:
        case OP_ADC_n:
        case OP_INC_R:
        case OP_INC_HL:
            // note that for ADC, we must be sure to add the carry bit
            // to either of the sources.
            return ALU_ADD;
        case OP_SUB_R:
        case OP_SUB_HL:
        case OP_SUB_n:
        case OP_SBC_R:
        case OP_SBC_HL:
        case OP_SBC_n:
        case OP_CP_R:
        case OP_CP_HL:
        case OP_CP_n:
        case OP_DEC_R:
        case OP_DEC_HL:
            // note that for SBC, we must be sure to subtract the carry bit
            // from the final result.
            return ALU_SUB;
        case OP_AND_R:
        case OP_AND_HL:
        case OP_AND_n:
            return ALU_AND;
        case OP_OR_R:
        case OP_OR_HL:
        case OP_OR_n:
            return ALU_OR;
        case OP_XOR_R:
        case OP_XOR_HL:
        case OP_XOR_n:
        case OP_CCF:
        case OP_CPL:
            return ALU_XOR;
        case OP_ADD_HL_RR:
        case OP_INC_RR:
            // the other 16-bit arithmetic ops are special cases because
            // of sign extension or subtraction (flags also).
            return ALU_ADD16;
        case OP_RLCA:
        case OP_RLC_R:
        case OP_RLC_HL:
            return ALU_RL;
        case OP_RLA:
        case OP_RL_R:
        case OP_RL_HL:
            return ALU_RL_CARRY;
        case OP_RRCA:
        case OP_RRC_R:
        case OP_RRC_HL:
            return ALU_RR;
        case OP_RRA:
        case OP_RR_R:
        case OP_RR_HL:
            return ALU_RR_CARRY;
        case OP_SLA_R:
        case OP_SLA_HL:
            return ALU_SL;
        case OP_SRA_R:
        case OP_SRA_HL:
            return ALU_SRA;
        case OP_SRL_R:
        case OP_SRL_HL:
            return ALU_SRL;
        default:
            fprintf(stderr, "<WARNING> ALU op defaulting to NOP\n");
            return ALU_NOP;
	}
}

// what the absolute fuck is even happening

// return number of machine cycles (1 for fetch)
// ARE WE INCRMEMPNT PC????? yes but this is stupid
int cpu_fetch()
{
    // buffer the operation.
    uint16_t tmppc = reg16_toi(cpu.reg.registers[PC]);
    cpu.x_op = get_op_inc(&tmppc, &cpu.x_insbits);
    reg_write16(tmppc, PC, &cpu.reg);
    return 1;
}

// returns number of machine cycles.
int cpu_execute()
{
    return op_imp[cpu.x_op](&cpu);
}

int step_cpu()
{
    // make sure that fetch is called once before this starts, okay?
    int cx = cpu_execute();
    int cf = cpu_fetch();
    // overlapping so we should only care about cx i guess?
    // seems correct.
    return cx;
}
