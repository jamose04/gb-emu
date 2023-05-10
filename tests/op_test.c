#include "cpu.h"
#include "cpu_util.h"
#include "memory.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	init_cpu();
	printf("hello\n");
	return 0;
}
