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
    // you have to call fetch to get the very first instruction.
    cpu_fetch();
    // here is where we need the loop (SDL powered)
    step_cpu();
}

// what is needed:
/*
loops are based on lcd stuff
int dotc
//mode 2
while dotc < (80 + 12)
 step cpu
scan oam and stuff

// mode 3 (draw shit) (also +12 above is mode 3)
while dotc < (92 + 160)

 
 
 */