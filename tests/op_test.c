#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

extern const int (*op_imp[OP_NUM_OPCODES]) (cpu_state_t *cpu);
extern cpu_state_t cpu;


int main(int argc, char *argv[])
{
	init_cpu();
	FILE *rom = fopen("testrom.gb", "rb");
	FILE *gbboot = fopen("gbboot.rom", "rb");
	if (!mem_init(gbboot, rom)) {
		fprintf(stderr, "fail init\n");
	 	exit(1);
	}
	op_imp[OP_LD_RR](&cpu);
	printf("hello\n");
	return 0;
}
