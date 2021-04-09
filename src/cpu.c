#include <stdlib.h>
#include "cpu.h"
#include "6502_instructions.c"
#include <stdio.h>


void cpu_init(nes_system *nes){
    nes->cpu.stkbase = 0x100;
	cpu_reset(nes);
	nes->cpu.cycles = 0;
}


void cpu_clock(nes_system *nes){
    if(nes->cpu.cycles == 0){
		printf("pc : %x\n", nes->cpu.pc);
        nes->cpu.opcode = cpu_read(nes, nes->cpu.pc);
        nes->cpu.pc++;
        nes->cpu.cycles = lookup[nes->cpu.opcode].cycles;
        uint8_t additional_cycle1 = lookup[nes->cpu.opcode].addrmode(nes);
        uint8_t additional_cycle2 = lookup[nes->cpu.opcode].operate(nes);
        nes->cpu.cycles += (additional_cycle1 & additional_cycle2);
    }
    nes->cpu.cycles--;
}

// Fetches the data to be used by the instruction.
uint8_t cpu_fetch(nes_system *nes){
    if(lookup[nes->cpu.opcode].addrmode != &IMP){
        nes->cpu.fetched = cpu_read(nes, nes->cpu.addr_abs);
    }
    return nes->cpu.fetched;
}

// Flag functions

uint8_t cpu_get_flag(nes_system *nes, enum FLAGS6502 f){
    return (nes->cpu.status & f);
}

void cpu_set_flag(nes_system *nes, enum FLAGS6502 f, uint16_t v){
    nes->cpu.status =  v ? nes->cpu.status | f : nes->cpu.status & ~f;
}


void cpu_reset(nes_system *nes){
    // Get address to set program counter to
	nes->cpu.addr_abs = 0xFFFC;
	uint16_t lo = cpu_read(nes, nes->cpu.addr_abs + 0);
	uint16_t hi = cpu_read(nes, nes->cpu.addr_abs + 1);

	// Set it
	nes->cpu.pc = (hi << 8) | lo;

	// Reset internal registers
	nes->cpu.a = 0;
	nes->cpu.x = 0;
	nes->cpu.y = 0;
	nes->cpu.stkp = 0xFD;
	nes->cpu.status = 0x00 | U;

	// Clear internal helper variables
	nes->cpu.addr_rel = 0x0000;
	nes->cpu.addr_abs = 0x0000;
	nes->cpu.fetched = 0x00;

	// Reset takes time
	nes->cpu.cycles = 8;
}

void cpu_irq(nes_system *nes){
    // If interrupts are allowed
	if (cpu_get_flag(nes, I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits dont
		// forget so that takes two pushes
		cpu_write(nes, 0x0100 + nes->cpu.stkp, (nes->cpu.pc >> 8) & 0x00FF);
		nes->cpu.stkp--;
		cpu_write(nes, 0x0100 + nes->cpu.stkp, nes->cpu.pc & 0x00FF);
		nes->cpu.stkp--;

		// Then Push the status register to the stack
		cpu_set_flag(nes, B, 0);
		cpu_set_flag(nes, U, 1);
		cpu_set_flag(nes, I, 1);
		cpu_write(nes, 0x0100 + nes->cpu.stkp, nes->cpu.status);
		nes->cpu.stkp--;

		// Read new program counter location from fixed address
		nes->cpu.addr_abs = 0xFFFE;
		uint16_t lo = cpu_read(nes, nes->cpu.addr_abs + 0);
		uint16_t hi = cpu_read(nes, nes->cpu.addr_abs + 1);
		nes->cpu.pc = (hi << 8) | lo;

		// IRQs take time
		nes->cpu.cycles = 7;
    }
}		

void cpu_nmi(nes_system *nes){
    cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, (nes->cpu.pc >> 8) & 0x00FF);
	nes->cpu.stkp--;
	cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.pc & 0x00FF);
	nes->cpu.stkp--;

	cpu_set_flag(nes, B, 0);
	cpu_set_flag(nes, U, 1);
	cpu_set_flag(nes, I, 1);
	cpu_write(nes,  nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.status);
	nes->cpu.stkp--;

	nes->cpu.addr_abs = 0xFFFA;
	uint16_t lo = cpu_read(nes, nes->cpu.addr_abs + 0);
	uint16_t hi = cpu_read(nes, nes->cpu.addr_abs + 1);
	nes->cpu.pc = (hi << 8) | lo;

	nes->cpu.cycles = 8;
}		

	