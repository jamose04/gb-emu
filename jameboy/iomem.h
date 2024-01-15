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

/* decrement by pos amt */
void iomem_dma_dots_dec(uint8_t);

/* return dots left in dma transfer */
int16_t iomem_dma_dots();

/* return dma src addr */
uint16_t iomem_dma_addr();

/* Determine whether the boot rom is enabled */
bool iomem_gbboot_enable();

#endif
