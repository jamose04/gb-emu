#ifndef RMEM_H
#define RMEM_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* Initialize RAM. Must be called before other procedures here */
bool rmem_init();

/* Free RAM. Must be called at end of use */
void rmem_close();

/* Write an 8-byte value to vram at addr */
void vram_write(uint16_t addr, uint8_t val);

/* Write a 16-byte value to vram at addr */
void vram_write16(uint16_t addr, uint16_t val);

/* Write an 8-byte value to wram at addr */
void wram_write(uint16_t addr, uint8_t val);

/* Write a 16-byte value to wram at addr */
void wram_write16(uint16_t addr, uint16_t val);

/* Write an 8-byte value to hram at addr */
void hram_write(uint16_t addr, uint8_t val);

/* Write a 16-byte value to hram at addr */
void hram_write16(uint16_t addr, uint16_t val);

/* Read an 8-byte value to hram from addr */
uint8_t hram_read(uint16_t addr);

/* Read an 8-byte value to wram from addr */
uint8_t wram_read(uint16_t addr);

/* Read an 8-byte value to vram from addr */
uint8_t vram_read(uint16_t addr);

#endif //RMEM_H
