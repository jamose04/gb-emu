#include "cpu_util.h"

const byte_t instr_lookup[] =
    {OP_NOP, OP_LD_RR_NN, OP_LD_BCA, OP_INC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_UNIMP, OP_LD_NN_SP, OP_ADD_HL_RR, OP_LD_RR, OP_DEC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_UNIMP,
     OP_UNIMP, OP_LD_RR_NN, OP_LD_RR, OP_LD_DEA, OP_INC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_UNIMP, OP_UNIMP, OP_ADD_HL_RR, OP_LD_ADE, OP_DEC_RR, OP_INC_R, OP_INC_R, OP_LD_RN, OP_UNIMP,
     OP_UNIMP, OP_LD_RR_NN, OP_LD_HLA_p, OP_INC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_DAA, OP_UNIMP, OP_ADD_HL_RR, OP_LD_AHL_p, OP_DEC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_UNIMP,
     OP_UNIMP, OP_LD_RR_NN, OP_LD_HLA_d, OP_INC_RR, OP_INC_HL, OP_DEC_HL, OP_LD_HLN, OP_SCF, OP_UNIMP, OP_ADD_HL_RR, OP_LD_AHL_d, OP_DEC_RR, OP_INC_R, OP_DEC_R, OP_LD_RN, OP_CCF,
     OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR,
     OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR,
     OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR,
     OP_LD_HLR, OP_LD_HLR, OP_LD_HLR, OP_LD_HLR, OP_LD_HLR, OP_LD_HLR, OP_HLT, OP_LD_HLR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RR, OP_LD_RHL, OP_LD_RR};


opcode_t get_op(byte_t code, jregister_t *pc)
{
    switch (code) {
        case 0x0:
            return OP_NOP;
        case 0x76:
            return OP_HLT;
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5f:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6f:
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7f:
            return OP_LD_RR;
        case 0x06:
        case 0x16:
        case 0x26:
        case 0x0e:
        case 0x1e:
        case 0x2e:
        case 0x3e:
            return OP_LD_RN;
        case 0x46:
        case 0x56:
        case 0x66:
        case 0x4e:
        case 0x5e:
        case 0x6e:
        case 0x7e:
            return OP_LD_RHL;
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x77:
            return OP_LD_HLR;
        case 0x36:
            return OP_LD_HLN;
        case 0x0a:
            return OP_LD_ABC;
        case 0x1a:
            return OP_LD_ADE;
        case 0x02:
            return OP_LD_BCA;
        case 0x12:
            return OP_LD_DEA;
        case 0xfa:
            return OP_LD_Ann;
        case 0xea:
            return OP_LD_nnA;
        case 0xf2:
            return OP_LDH_AC;
        case 0xe2:
            return OP_LDH_CA;
        case 0xf0:
            return OP_LDH_An;
        case 0xe0:
            return OP_LDH_nA;
        case 0x3a:
            return OP_LD_AHL_d;
        case 0x32:
            return OP_LD_HLA_d;
        case 0x2a:
            return OP_LD_AHL_p;
        case 0x22:
            return OP_LD_HLA_p;
        default:
            return OP_UNIMP;
    };
}