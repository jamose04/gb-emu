#include "control.h"
#include "memory.h"
#include "cpu.h"
#include "register.h"

bool init_machine(FILE *gbboot, FILE *rom)
{
    init_cpu();
    if (!init_mem(gbboot, rom)) {
        fprintf(stderr, "<ERROR> mem init failed.");
        return false;
    }
    imem_dump(256);

    return true;
}