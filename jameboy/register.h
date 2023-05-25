#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

#include "types.h"

typedef struct Reg16 {
    uint8_t hi;
    uint8_t lo;
} reg16_t;

typedef struct Reg {
    reg16_t registers[6];
} cpu_reg_t;

typedef enum Reg_sel {
    AF,
    BC,
    DE,
    HL,
    SP,
    PC,
} reg_sel_t;

/* We can read from any of the registers */

uint8_t *reg8_at(uint8_t i, cpu_reg_t *reg);

uint16_t reg16_toi(reg16_t r);

void reg_write16(uint16_t val, reg_sel_t sel, cpu_reg_t *reg);

// for debug
void print_registers(const cpu_reg_t *cpu_reg);

#endif //REGISTER_H
