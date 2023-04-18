#include "cpu.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("<INFO> starting jameboy\n");
    init_cpu();
    printf("<INFO> cpu initialized\n");
    printf("unimplemented\n");
    printf("<INFO> finished... printing state and exiting\n");
    print_cpu_state();
    return 0;
}