// This file contains opcodes, status codes, and some cpu hw elements.
#ifndef CPU_UTIL_H
#define CPU_UTIL_H

#include "register.h"

/*
 * All sm83 opcodes. There are a lot.
 */
typedef enum Opcodes {
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
    OP_ADD_HL_SP_nn,
} opcode_t;

#endif