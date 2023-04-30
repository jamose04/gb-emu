#ifndef IOMEM_H
#define IOMEM_H

#include "types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* Initialize iomem */
bool iomem_init();

/* Write to an iomem register */
bool iomem_write(uint16_t addr, byte_t val);

/* Determine whether the boot rom is enabled */
bool iomem_gbboot_enable();

#endif