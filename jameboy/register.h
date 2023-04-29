#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

#include "types.h"

typedef struct Reg {
    uint16_t AF;
    uint16_t BC;
    uint16_t DE;
    uint16_t HL;
    uint16_t SP;
    uint16_t PC;
} cpu_reg_t;

enum Reg_sel {
    AF,
    BC,
    DE,
    HL,
    SP,
    PC,
};

/* We can read from any of the registers */

byte_t read_hi(uint16_t reg);

byte_t read_lo(uint16_t reg);

void write_hi(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val);

void write_lo(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val);

// for debug
void print_registers(const cpu_reg_t *cpu_reg);

#endif //REGISTER_H