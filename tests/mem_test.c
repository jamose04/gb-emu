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
	printf("successful\n");
	
	mem_write(0xc000u, 32);
	mem_write(0xc7afu, 55);
	mem_write(0xdfffu, 22);
	
	// wram8 test
	
	test(32, mem_read(0xc000u), 0, 0, 1, "wram8");
	test(55, mem_read(0xc7afu), 0, 0, 2, "wram8");
	test(22, mem_read(0xdfffu), 0, 0, 3, "wram8");
	
	mem_write(0xc7afu, 96);
	
	test(96, mem_read(0xc7afu), 0, 0, 4, "wram8");
	
	mem_write16(0xc000u, 0x2784);
	
	test(0x84, mem_read(0xc000u), 0, 0, 5, "wram16");
	test(0x27, mem_read(0xc001u), 0, 0, 6, "wram16");
}

int main(int argc, char *argv[])
{
	FILE *gbboot = fopen("gbboot.rom", "rb");
	FILE *rom = fopen("testrom.gb", "rb");
	if (!gbboot || !rom) {
		fprintf(stderr, "lacking necessary test files\n");
		exit(1);
	}
	run_tests(gbboot, rom);
	fclose(gbboot);
	fclose(rom);
	return 0;
}
