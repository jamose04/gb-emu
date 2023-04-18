// This file contains opcodes, status codes, and some cpu hw elements.
#ifndef CPU_UTIL_H
#define CPU_UTIL_H

#include "register.h"

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
} opcode_t;

#endif