#ifndef _CPU_H_
#define _CPU_H_
#include <stdint.h>


// Struct defining the 6502 CPU state and other data used for its emulation.
typedef struct cpu_6502{

    //Registers
    uint8_t  a       ;		// Accumulator Register
	uint8_t  x       ;		// X Register
	uint8_t  y       ;		// Y Register
	uint8_t  stkp    ;		// Stack Pointer (points to location on bus). Offset in relation to the base of the stack
	uint16_t pc      ;   	// Program Counter
	uint8_t  status  ;		// Status Register

    uint16_t stkbase ;      // Base address of the stack. 0x0100 by design of the cpu     << talvez isso dê ruim, atenção com castings que possam levar a comportamentos estranhos
    uint8_t  fetched ;      // Last fetched data
    uint16_t addr_abs;      // Store address of the data to to be accessed by instruction
    uint16_t addr_rel;      // For use during relative address resolutions
    uint8_t  opcode  ;      // Current opcode
    uint8_t  cycles  ;      // Cycles left for the duration of current instruction
}cpu_6502;

#include "bus.h"

// Allocate and initializes a CPU with zeroes.
void cpu_init(nes_system *nes);

// Flags represented by each bit of the Status Register
enum FLAGS6502
	{
		C = (1 << 0),	// Carry Bit
		Z = (1 << 1),	// Zero
		I = (1 << 2),	// Disable Interrupts
		D = (1 << 3),	// Decimal Mode (unused in this implementation)
		B = (1 << 4),	// Break
		U = (1 << 5),	// Unused
		V = (1 << 6),	// Overflow
		N = (1 << 7),	// Negative
	};




// External events.

// Reset Interrupt - Forces CPU into known state
void cpu_reset(nes_system *);	
// Interrupt Request - Executes an instruction at a specific location
void cpu_irq(nes_system *);		
// Non-Maskable Interrupt Request - As above, but cannot be disabled
void cpu_nmi(nes_system *);		
// Perform one clock cycle's worth of update
void cpu_clock(nes_system *);	

// Returns 1 if flag "f" is set in the cpu contained in "nes", 0 otherwise.
// Note: flags are stored in the status register
// [C Z I D B U V N]
uint8_t cpu_get_flag(nes_system *nes, enum FLAGS6502 f);

// Set flag "f" in the CPU contained in "nes" to value "v" (either 0 or 1).
void cpu_set_flag(nes_system *nes, enum FLAGS6502 f, uint16_t v);

uint8_t cpu_fetch(nes_system *nes);

#endif