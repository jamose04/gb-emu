#ifndef IOMEM_H
#define IOMEM_H

#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* Initialize iomem */
bool iomem_init();

/* Write to an iomem register */
void iomem_write(uint16_t addr, uint8_t val);

/* Determine whether the boot rom is enabled */
bool iomem_gbboot_enable();

#endif
