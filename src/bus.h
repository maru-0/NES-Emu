#ifndef _BUS_H_
#define _BUS_H_

#include <stdint.h>

struct nes_system;
typedef struct nes_system nes_system;

#include "cpu.h"
#include "cartridge.h"
#include "ppu_2C02.h"

struct nes_system{
    uint8_t ram[2048];
    cartridge inserted_cart;
    cpu_6502 cpu;
    ppu_2C02 ppu;

    uint32_t system_clock_counter;
};



void system_init(nes_system *nes);

void system_clock(nes_system *nes);

void system_reset();

uint8_t cpu_read(nes_system *nes, uint16_t addr);

void cpu_write(nes_system *nes, uint_fast16_t addr, uint8_t data);

#endif