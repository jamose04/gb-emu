#include "cpu.h"
#include "memory.h"
#include "control.h"

#include <stdio.h>
#include <stdlib.h>

void what(FILE *fp) {
    mem_init(fp, NULL);
    imem_dump(256);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "no rom provided\n");
        exit(1);
    }
    FILE *gbboot = fopen("gbboot.rom", "rb");
    if (!gbboot) {
        fprintf(stderr, "file not found\n");
        exit(1);
    }
    FILE *rom = fopen(argv[1], "rb");
    if (!rom) {
        fprintf(stderr, "rom not found\n");
        exit(1);
    }

    if (!init_machine(gbboot, rom)) {
        fprintf(stderr, "could not init machine\n");
        exit(1);
    }

    return 0;
}
