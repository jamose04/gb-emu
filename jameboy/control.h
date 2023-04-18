#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <stdbool.h>

/* Initialize the machine
 * We need a boot rom and a game rom.
 */
bool init_machine(FILE *gbboot, FILE *rom);

/*
 * Fire up the cpu and begin execution.
 */
void machine_start();

#endif