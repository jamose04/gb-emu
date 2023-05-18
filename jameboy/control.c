#include "control.h"
#include "memory.h"
#include "cpu.h"
#include "register.h"

bool init_machine(FILE *gbboot, FILE *rom)
{
    init_cpu();
    if (!mem_init(gbboot, rom)) {
        fprintf(stderr, "<ERROR> mem init failed.");
        return false;
    }
    imem_dump(256);

    return true;
}

void machine_run()
{
    // at this point, pc should be set to zero and the boot rom is loaded
    //cpu_execute();
    //cpu_fetch();
}
