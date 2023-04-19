#include "cpu.h"
#include "register.h"
#include "cpu_util.h"

#include <stdio.h>

typedef struct F_Buf {
} f_buf_t;

typedef struct X_Buf {
    opcode_t op;
    // used to get bits from instruction
} x_buf_t;

struct {
    cpu_reg_t registers;
    f_buf_t f_buf;
    x_buf_t x_buf;
} cpu;

/* SP will be set properly by the boot ROM */
void init_cpu()
{
    cpu.registers.AF = 0x0;
    cpu.registers.BC = 0x0;
    cpu.registers.DE = 0x0;
    cpu.registers.HL = 0x0;
    cpu.registers.SP = 0x0;
    cpu.registers.PC = 0x0;
    cpu.x_buf.op = OP_NOP;
}

void print_cpu_state()
{
    printf("CPU STATE:\n");
    print_registers(&cpu.registers); 
}

// return number of machine cycles (1 for fetch)
int cpu_fetch()
{
    return 1;
}

int cpu_execute()
{
    return 1;
}

void step_cpu()
{
    int cx = cpu_execute();
    int cf = cpu_fetch();
}