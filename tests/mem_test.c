#include "memory.h"
#include "test_util.h"

#include <stdio.h>

void run_tests(FILE *gbboot, FILE *rom)
{
	printf("INIT MEM...\n");
	if (!mem_init(gbboot, rom)) {
		fprintf(stderr, "could not initialize memory system...\n");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	FILE *gbboot = fopen("boot.rom", "rb");
	FILE *rom = fopen("testrom", "rb");
	if (!gbboot || !rom) {
		fprintf(stderr, "lacking necessary test files\n");
		exit(1);
	}
	run_tests(gbboot, rom);
	fclose(gbboot);
	fclose(rom);
	return 0;
}
