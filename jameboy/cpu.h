#ifndef CPU_H
#define CPU_H

#include "register.h"
#include "cpu_util.h"

typedef struct {
    cpu_reg_t reg;
    opcode_t x_op;
    uint8_t x_insbits;
} cpu_state_t;

void init_cpu();

void print_cpu_state();

int cpu_fetch();

int cpu_execute();

int step_cpu();

#endif
