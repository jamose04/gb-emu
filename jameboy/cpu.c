#include "cpu.h"
#include "register.h"
#include "cpu_util.h"

#include <stdio.h>

struct {
    cpu_reg_t registers;
    opcode_t x_op;
    unsigned x_insbits;
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
    cpu.x_op = OP_NOP;
}

void print_cpu_state()
{
    printf("CPU STATE:\n");
    print_registers(&cpu.registers); 
}

// return number of machine cycles (1 for fetch)
int cpu_fetch()
{
	// buffer the operation.
	cpu.op = get_op_inc(&cpu.registers.PC, &cpu.x_insbits);
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
