#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

#include "types.h"

typedef struct Reg {
    uint16_t registers[6];
} cpu_reg_t;

typedef enum Reg_sel {
    BC,
    DE,
    HL,
    FA, // really AF, but I want A to be stored after F in the register array
    SP,
    PC,
} reg_sel_t;

/* We can read from any of the registers */

/* BEGIN DEPRECATED FUNCTIONS */

byte_t read_hi(uint16_t reg);

byte_t read_lo(uint16_t reg);

void write_hi(cpu_reg_t *cpu_reg, reg_sel_t sel, byte_t val);

void write_lo(cpu_reg_t *cpu_reg, reg_sel_t sel, byte_t val);

/* END DEPRECATED FUNCTIONS */

uint8_t reg_read_hi(reg_sel_t sel, const cpu_reg_t *reg);

uint8_t reg_read_lo(reg_sel_t sel, const cpu_reg_t *reg);

void reg_write_hi(reg_sel_t sel, uint8_t val, cpu_reg_t *reg);

void reg_write_lo(reg_sel_t sel, uint8_t val, cpu_reg_t *reg);

// for debug
void print_registers(const cpu_reg_t *cpu_reg);

#endif //REGISTER_H
