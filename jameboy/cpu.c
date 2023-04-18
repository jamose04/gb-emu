#include "cpu.h"
#include "register.h"

#include <stdio.h>

struct {
    cpu_reg_t registers;
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
}

void print_cpu_state()
{
    printf("CPU STATE:\n");
    print_registers(&cpu.registers); 
}