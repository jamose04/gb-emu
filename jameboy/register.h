#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

typedef uint16_t jregister_t;
typedef unsigned char byte_t;

typedef struct Reg {
    jregister_t AF;
    jregister_t BC;
    jregister_t DE;
    jregister_t HL;
    jregister_t SP;
    jregister_t PC;
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

byte_t read_hi(jregister_t reg);

byte_t read_lo(jregister_t reg);

void write_hi(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val);

void write_lo(cpu_reg_t *cpu_reg, enum Reg_sel sel, byte_t val);

// for debug
void print_registers(const cpu_reg_t *cpu_reg);

#endif //REGISTER_H