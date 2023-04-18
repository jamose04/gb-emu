#include "cpu.h"
#include "memory.h"
#include "control.h"

#include <stdio.h>
#include <stdlib.h>

void what(FILE *fp) {
    init_mem(fp, NULL);
    imem_dump(256);
}

int main(int argc, char *argv[])
{
    FILE *gbboot = fopen("gbboot.rom", "rb");
    if (!gbboot) {
        fprintf(stderr, "file not found\n");
        exit(1);
    }
    if (!init_machine(gbboot, NULL)) {
        fprintf(stderr, "could not init machine\n");
        exit(1);
    }

    return 0;
}