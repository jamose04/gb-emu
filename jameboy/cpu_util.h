// This file contains opcodes, status codes, and some cpu hw elements.
#ifndef CPU_UTIL_H
#define CPU_UTIL_H

#include "register.h"

/*
 * All sm83 opcodes. There are a lot.
 */
typedef enum Opcodes {
    OP_UNIMP, // for unimplemented opcodes
    OP_NOP,
    OP_HLT,
    OP_LD_RR,
    OP_LD_RN,
    OP_LD_RHL, // load legister from mem
    OP_LD_HLR, // store register
    OP_LD_HLN, // store immediate
    OP_LD_ABC,
    OP_LD_ADE,
    OP_LD_BCA,
    OP_LD_DEA,
    OP_LD_Ann,
    OP_LD_nnA,
    // for modifying the stack?
    OP_LDH_AC,
    OP_LDH_CA,
    OP_LDH_An, // load from 0xff00 + n
    OP_LDH_nA, // load to 0xff00 + n
    OP_LD_AHL_d,
    OP_LD_HLA_d,
    OP_LD_AHL_p,
    OP_LD_HLA_p,
    /* 16-bit memory ops */
    OP_LD_RR_NN,
    OP_LD_NN_SP,
    OP_LD_SP_HL,
    OP_PUSH_RR,
    OP_POP_RR,
    /* 8-bit arithmetic ops */
    OP_ADD_R,
    OP_ADD_HL,
    OP_ADD_n,
    OP_ADC_R,
    OP_ADC_HL,
    OP_ADC_n,
    OP_SUB_R,
    OP_SUB_HL,
    OP_SUB_n,
    OP_SBC_R,
    OP_SBC_HL,
    OP_SBC_n,
    OP_CP_R,
    OP_CP_HL,
    OP_CP_n,
    OP_INC_R,
    OP_INC_HL,
    OP_DEC_R,
    OP_DEC_HL,
    OP_AND_R,
    OP_AND_HL,
    OP_AND_n,
    OP_OR_R,
    OP_OR_HL,
    OP_OR_n,
    OP_XOR_R,
    OP_XOR_HL,
    OP_XOR_n,
    OP_CCF,
    OP_SCF,
    OP_DAA,
    OP_CPL,
    /* 16-bit Arithmetic ops */
    OP_ADD_HL_RR,
    OP_INC_RR,
    OP_DEC_RR,
    OP_ADD_SP_nn,
    OP_LD_HL_SP_nn,
    /* Rotate and shift */
    OP_RLCA,
    OP_RLA,
    OP_RRCA,
    OP_RRA,
    OP_RLC_R,
    OP_RLC_HL,
    OP_RL_R,
    OP_RL_HL,
    OP_RRC_R,
    OP_RRC_HL,
    OP_RR_R,
    OP_RR_HL,
    OP_SLA_R,
    OP_SLA_HL,
    OP_SWAP_R,
    OP_SWAP_HL,
    OP_SRA_R,
    OP_SRA_HL,
    OP_SRL_R,
    OP_SRL_HL,
    /* single-bit operations */
    OP_BIT_NR,
    OP_BIT_NHL,
    OP_SET_NR,
    OP_SET_NHL,
    OP_RES_NR,
    OP_RES_NHL,
    /* CPU Control instructions */
    OP_STOP,
    OP_DI,
    OP_EI,
    /* jump instructions */
    OP_JP_NN,
    OP_JP_HL,
    OP_JP_FNN,
    OP_JR_PCDD,
    OP_JR_F_PCDD,
    OP_CALL_NN,
    OP_CALL_FNN,
    OP_RET,
    OP_RET_F,
    OP_RET_I,
    OP_RST_N,
    OP_NUM_OPCODES,
} opcode_t;

typedef enum Aluops {
    ALU_NOP,
    ALU_PASS,
    ALU_ADD,
    ALU_ADD16,
    ALU_SUB,
    ALU_XOR,
    ALU_OR,
    ALU_AND,
    ALU_RL,
    ALU_RR,
    ALU_RL_CARRY,
    ALU_RR_CARRY,
    ALU_SL,
    ALU_SRA,
    ALU_SRL,
    ALU_SWAP,
    ALU_BIT,
    ALU_SET,
    ALU_RESET,
} alu_op_t;

typedef enum Reg_access_mode {
    RA_HI,
    RA_LO,
    RA_WH,
} reg_access_mode_t;

/* Get op and increment pc accordingly (increments to byte after opcode) */
opcode_t get_op_inc(uint16_t *pc, uint8_t *insbits);

/* Perform an alu operation */
uint16_t alu(alu_op_t alu_op, uint16_t src1, uint16_t src2, uint8_t *f);

/* Write a register. If mode is HI or LO, write the low 8-bits of val into 
 * the 8 bit register. If mode is WH, write all 16 bits into the joint
 * register */
void write_register(uint16_t *reg, reg_access_mode_t mode, uint16_t val);

/* Read a register. For modes HI and LO, return 8-bit value */
uint16_t read_register(uint16_t reg, reg_access_mode_t mode);

#endif
