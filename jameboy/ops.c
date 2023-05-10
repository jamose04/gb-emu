#include "ops.h"
#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

// NOTE: for neither of the following two functions shall i_dst or i_src
// be equal to 6 (this is flag).
void op_ld_rr(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    uint8_t i_src = cpu->x_insbits & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = reg8[i_src];
}

void op_ld_rn(cpu_state_t *cpu)
{
    uint8_t n_byte = mem_read((cpu->reg.registers[PC]));
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    uint8_t *reg8 = (uint8_t *) cpu->reg.registers;
    reg8[i_dst] = n_byte;
}

void op_ld_rhl(cpu_state_t *cpu)
{
    uint8_t i_dst = (cpu->x_insbits >> 3) & 0x07u;
    
    *((uint8_t *) cpu->reg.registers + i_dst) = mem_read(cpu->reg.registers[HL]);
    
}
    

const void (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu) =
    {
		op_ld_rr,
    };
