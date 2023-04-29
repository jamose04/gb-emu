#include "cpu_util.h"
// lookup tables.
#include "tab.h"
#include "cbtab.h"
#include "memory.h"

opcode_t get_op_inc(uint16_t *pc)
{
    if (*pc != 0xcb) {
        return instr_lookup[imem_get((*pc)++)];
    } else {
        (*pc)++;
        return cb_instr_lookup[imem_get((*pc)++)];
    }
}