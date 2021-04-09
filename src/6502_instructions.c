#include <stdint.h>
#include "cpu.h"
#include "bus.h"

// Struct defining a cpu instruction.
// "name" is the instruction name (to be used in the disassembler)
// "operate" is a function pointer to the operation of the instruction.
// "addrmode" is a function pointer to the address mode of the instruction.
// "cycles" is the number of cpu cycles needed by the cpu to complete the instruction (in some cases, extra cycles may be nedded, this is the minimum value).
typedef struct INSTRUCTION
{
    char name[4];		
    uint8_t     (*operate )(nes_system *);
    uint8_t     (*addrmode)(nes_system *);
    uint8_t     cycles;
} INSTRUCTION;



// Addressing modes.
// Addressing modes are the ways which the addressing of the data needed by the instruction can be done.
// Resulting address is stored in the "addr_abs" of the cpu during the execution of the instruction.
// The only exception is REL, which uses a relative address and thus is stored in the variable "addr_rel" of the cpu.
uint8_t IMP(nes_system *);	uint8_t IMM(nes_system *);	
uint8_t ZP0(nes_system *);	uint8_t ZPX(nes_system *);	
uint8_t ZPY(nes_system *);	uint8_t REL(nes_system *);
uint8_t ABS(nes_system *);	uint8_t ABX(nes_system *);	
uint8_t ABY(nes_system *);	uint8_t IND(nes_system *);	
uint8_t IZX(nes_system *);	uint8_t IZY(nes_system *);

// Operations.
uint8_t ADC(nes_system *);	uint8_t AND(nes_system *);	uint8_t ASL(nes_system *);	uint8_t BCC(nes_system *);
uint8_t BCS(nes_system *);	uint8_t BEQ(nes_system *);	uint8_t BIT(nes_system *);	uint8_t BMI(nes_system *);
uint8_t BNE(nes_system *);	uint8_t BPL(nes_system *);	uint8_t BRK(nes_system *);	uint8_t BVC(nes_system *);
uint8_t BVS(nes_system *);	uint8_t CLC(nes_system *);	uint8_t CLD(nes_system *);	uint8_t CLI(nes_system *);
uint8_t CLV(nes_system *);	uint8_t CMP(nes_system *);	uint8_t CPX(nes_system *);	uint8_t CPY(nes_system *);
uint8_t DEC(nes_system *);	uint8_t DEX(nes_system *);	uint8_t DEY(nes_system *);	uint8_t EOR(nes_system *);
uint8_t INC(nes_system *);	uint8_t INX(nes_system *);	uint8_t INY(nes_system *);	uint8_t JMP(nes_system *);
uint8_t JSR(nes_system *);	uint8_t LDA(nes_system *);	uint8_t LDX(nes_system *);	uint8_t LDY(nes_system *);
uint8_t LSR(nes_system *);	uint8_t NOP(nes_system *);	uint8_t ORA(nes_system *);	uint8_t PHA(nes_system *);
uint8_t PHP(nes_system *);	uint8_t PLA(nes_system *);	uint8_t PLP(nes_system *);	uint8_t ROL(nes_system *);
uint8_t ROR(nes_system *);	uint8_t RTI(nes_system *);	uint8_t RTS(nes_system *);	uint8_t SBC(nes_system *);
uint8_t SEC(nes_system *);	uint8_t SED(nes_system *);	uint8_t SEI(nes_system *);	uint8_t STA(nes_system *);
uint8_t STX(nes_system *);	uint8_t STY(nes_system *);	uint8_t TAX(nes_system *);	uint8_t TAY(nes_system *);
uint8_t TSX(nes_system *);	uint8_t TXA(nes_system *);	uint8_t TXS(nes_system *);	uint8_t TYA(nes_system *);

// Illegal opcode.
uint8_t XXX(nes_system *);

// Lookup table in the form of an array consisting of every instruction in the 6502 processor, indexed by the opcode
INSTRUCTION	lookup[]= 
	{
		{ "BRK", &BRK, &IMM, 7 },{ "ORA", &ORA, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ORA", &ORA, &ZP0, 3 },{ "ASL", &ASL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHP", &PHP, &IMP, 3 },{ "ORA", &ORA, &IMM, 2 },{ "ASL", &ASL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABS, 4 },{ "ASL", &ASL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BPL", &BPL, &REL, 2 },{ "ORA", &ORA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ZPX, 4 },{ "ASL", &ASL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLC", &CLC, &IMP, 2 },{ "ORA", &ORA, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABX, 4 },{ "ASL", &ASL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "JSR", &JSR, &ABS, 6 },{ "AND", &AND, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "BIT", &BIT, &ZP0, 3 },{ "AND", &AND, &ZP0, 3 },{ "ROL", &ROL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLP", &PLP, &IMP, 4 },{ "AND", &AND, &IMM, 2 },{ "ROL", &ROL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "BIT", &BIT, &ABS, 4 },{ "AND", &AND, &ABS, 4 },{ "ROL", &ROL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BMI", &BMI, &REL, 2 },{ "AND", &AND, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ZPX, 4 },{ "ROL", &ROL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEC", &SEC, &IMP, 2 },{ "AND", &AND, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ABX, 4 },{ "ROL", &ROL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "RTI", &RTI, &IMP, 6 },{ "EOR", &EOR, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "EOR", &EOR, &ZP0, 3 },{ "LSR", &LSR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHA", &PHA, &IMP, 3 },{ "EOR", &EOR, &IMM, 2 },{ "LSR", &LSR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &ABS, 3 },{ "EOR", &EOR, &ABS, 4 },{ "LSR", &LSR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BVC", &BVC, &REL, 2 },{ "EOR", &EOR, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ZPX, 4 },{ "LSR", &LSR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLI", &CLI, &IMP, 2 },{ "EOR", &EOR, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ABX, 4 },{ "LSR", &LSR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "RTS", &RTS, &IMP, 6 },{ "ADC", &ADC, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ADC", &ADC, &ZP0, 3 },{ "ROR", &ROR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLA", &PLA, &IMP, 4 },{ "ADC", &ADC, &IMM, 2 },{ "ROR", &ROR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &IND, 5 },{ "ADC", &ADC, &ABS, 4 },{ "ROR", &ROR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BVS", &BVS, &REL, 2 },{ "ADC", &ADC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ZPX, 4 },{ "ROR", &ROR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEI", &SEI, &IMP, 2 },{ "ADC", &ADC, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ABX, 4 },{ "ROR", &ROR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "???", &NOP, &IMP, 2 },{ "STA", &STA, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZP0, 3 },{ "STA", &STA, &ZP0, 3 },{ "STX", &STX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "DEY", &DEY, &IMP, 2 },{ "???", &NOP, &IMP, 2 },{ "TXA", &TXA, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "STY", &STY, &ABS, 4 },{ "STA", &STA, &ABS, 4 },{ "STX", &STX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
		{ "BCC", &BCC, &REL, 2 },{ "STA", &STA, &IZY, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZPX, 4 },{ "STA", &STA, &ZPX, 4 },{ "STX", &STX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "TYA", &TYA, &IMP, 2 },{ "STA", &STA, &ABY, 5 },{ "TXS", &TXS, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "???", &NOP, &IMP, 5 },{ "STA", &STA, &ABX, 5 },{ "???", &XXX, &IMP, 5 },{ "???", &XXX, &IMP, 5 },
		{ "LDY", &LDY, &IMM, 2 },{ "LDA", &LDA, &IZX, 6 },{ "LDX", &LDX, &IMM, 2 },{ "???", &XXX, &IMP, 6 },{ "LDY", &LDY, &ZP0, 3 },{ "LDA", &LDA, &ZP0, 3 },{ "LDX", &LDX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "TAY", &TAY, &IMP, 2 },{ "LDA", &LDA, &IMM, 2 },{ "TAX", &TAX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "LDY", &LDY, &ABS, 4 },{ "LDA", &LDA, &ABS, 4 },{ "LDX", &LDX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
		{ "BCS", &BCS, &REL, 2 },{ "LDA", &LDA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "LDY", &LDY, &ZPX, 4 },{ "LDA", &LDA, &ZPX, 4 },{ "LDX", &LDX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "CLV", &CLV, &IMP, 2 },{ "LDA", &LDA, &ABY, 4 },{ "TSX", &TSX, &IMP, 2 },{ "???", &XXX, &IMP, 4 },{ "LDY", &LDY, &ABX, 4 },{ "LDA", &LDA, &ABX, 4 },{ "LDX", &LDX, &ABY, 4 },{ "???", &XXX, &IMP, 4 },
		{ "CPY", &CPY, &IMM, 2 },{ "CMP", &CMP, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPY", &CPY, &ZP0, 3 },{ "CMP", &CMP, &ZP0, 3 },{ "DEC", &DEC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INY", &INY, &IMP, 2 },{ "CMP", &CMP, &IMM, 2 },{ "DEX", &DEX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "CPY", &CPY, &ABS, 4 },{ "CMP", &CMP, &ABS, 4 },{ "DEC", &DEC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BNE", &BNE, &REL, 2 },{ "CMP", &CMP, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ZPX, 4 },{ "DEC", &DEC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLD", &CLD, &IMP, 2 },{ "CMP", &CMP, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ABX, 4 },{ "DEC", &DEC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "CPX", &CPX, &IMM, 2 },{ "SBC", &SBC, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPX", &CPX, &ZP0, 3 },{ "SBC", &SBC, &ZP0, 3 },{ "INC", &INC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INX", &INX, &IMP, 2 },{ "SBC", &SBC, &IMM, 2 },{ "NOP", &NOP, &IMP, 2 },{ "???", &SBC, &IMP, 2 },{ "CPX", &CPX, &ABS, 4 },{ "SBC", &SBC, &ABS, 4 },{ "INC", &INC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BEQ", &BEQ, &REL, 2 },{ "SBC", &SBC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ZPX, 4 },{ "INC", &INC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SED", &SED, &IMP, 2 },{ "SBC", &SBC, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ABX, 4 },{ "INC", &INC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
	};



// Address mode: Implied
// No additional data, accumulator value is stored in fetched
uint8_t IMP(nes_system *nes){
    nes->cpu.fetched = nes->cpu.a;
    return 0x00;
}	

// Address mode: Immediate
// Instruction expects the data on the byte immediately after the opcode
uint8_t IMM(nes_system *nes){
    nes->cpu.addr_abs = nes->cpu.pc;
    nes->cpu.pc++;
    return 0x00;
}	

// Address mode: Zero Page Addressing
// The address of interest is located on page 0, therefore only one byte representing the offset of the page needs to be read
uint8_t ZP0(nes_system *nes){
    nes->cpu.addr_abs = 0x00FF & cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;
    return 0x00;
    }	

// Address mode: Zero Page plus x offset
// Similar to ZP0, but with the value in the x register added to the offset
uint8_t ZPX(nes_system *nes){
    nes->cpu.addr_abs = 0x00FF & (cpu_read(nes, nes->cpu.pc) + nes->cpu.x);
    nes->cpu.pc++;
    return 0x00;
}	

// Address mode: Zero Page plus y offset
// Similar to ZP0, but with the value in the y register added to the offset
uint8_t ZPY(nes_system *nes){
    nes->cpu.addr_abs = 0x00FF & (cpu_read(nes, nes->cpu.pc) + nes->cpu.y);
    nes->cpu.pc++;
    return 0x00;
}	

// Address mode: Relative
uint8_t REL(nes_system *nes){
    nes->cpu.addr_rel = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    if(nes->cpu.addr_rel & 0x80){
        nes->cpu.addr_rel |= 0xFF00;
    }
    return 0x00;
}

// Address mode: Absolute
uint8_t ABS(nes_system *nes){
    uint16_t lo = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;
    uint16_t hi = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    nes->cpu.addr_abs = (hi << 8) | lo;
    return 0x00;
}	

// Adress mode: Absolute with x offset
uint8_t ABX(nes_system *nes){
    uint16_t lo = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;
    uint16_t hi = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    nes->cpu.addr_abs = (hi << 8) | lo;
    nes->cpu.addr_abs += nes->cpu.x;

    // If the page changed, there may be a need for an additional clock cycle
    if((0xFF00 & nes->cpu.addr_abs) != (hi << 8)){
        return 0x01;
    }
    return 0x00;
}	

// Adress mode: Absolute with y offset
uint8_t ABY(nes_system *nes){
     uint16_t lo = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;
    uint16_t hi = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    nes->cpu.addr_abs = (hi << 8) | lo;
    nes->cpu.addr_abs += nes->cpu.y;

    // If the page changed, there may be a need for an additional clock cycle
    if((0xFF00 & nes->cpu.addr_abs) != (hi << 8)){
        return 0x01;
    }
    return 0x00;
}	

// Addressing mode: Indirect
uint8_t IND(nes_system *nes){
    uint16_t ptr_lo = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;
    uint16_t ptr_hi = cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    // Simulate hardware bug
    // When in the last address of a page, instead of reading the first address of the next page, it loops back to the beginning of the same page
    if(ptr_lo == 0x00FF){ 
        nes->cpu.addr_abs = (cpu_read(nes, ptr & 0xFF00) << 8) | cpu_read(nes, ptr);
    }else{ // Normal behavior
        nes->cpu.addr_abs = (cpu_read(nes, ptr+1) << 8) | cpu_read(nes, ptr);
    }
    
    return 0x00;
}	

// Addressing mode: Indirect address in page zero with x offset added
uint8_t IZX(nes_system *nes){
    uint16_t ptr = 0x00FF & (cpu_read(nes, nes->cpu.pc) + nes->cpu.x);
    nes->cpu.pc++;

    uint16_t lo = cpu_read(nes, ptr);
    uint16_t hi = cpu_read(nes, ptr+1);
    nes->cpu.addr_abs = (hi << 8) | lo;
    return 0x00;
}	

// Addressing mode: Indirect address in page zero with y added to the resulting offset read from page zero
uint8_t IZY(nes_system *nes){
    uint16_t ptr = 0x00FF & cpu_read(nes, nes->cpu.pc);
    nes->cpu.pc++;

    uint16_t lo = cpu_read(nes, ptr);
    uint16_t hi = cpu_read(nes, ptr+1);
    nes->cpu.addr_abs = (hi << 8) | lo;
    ptr += nes->cpu.y;

    // If the page changed, there may be a need for an additional clock cycle
    if((0xFF00 & nes->cpu.addr_abs) != (hi << 8)){
        return 0x01;
    }
    return 0x00;
}	

// ---- Instructions ----
// Operations descriptions opied from the 6502 datasheet
//
// Notes:
//   *  add 1 to cycles if page boundary is crossed
//
//   ** add 1 to cycles if branch occurs on same page
//      add 2 to cycles if branch occurs to different page
//
//
//      Legend to Flags:  + .... modified
//                        - .... not modified
//                        1 .... set
//                        0 .... cleared
//                       M6 .... memory bit 6
//                       M7 .... memory bit 7
//
//
// Note on assembler syntax:
// Most assemblers employ "OPC *oper" for forced zeropage addressing.



// ADC  Add Memory to Accumulator with Carry
//
//      A + M + C -> A, C                N Z C I D V
//                                       + + + - - +
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     ADC #oper     69    2     2
//      zeropage      ADC oper      65    2     3
//      zeropage,X    ADC oper,X    75    2     4
//      absolute      ADC oper      6D    3     4
//      absolute,X    ADC oper,X    7D    3     4*
//      absolute,Y    ADC oper,Y    79    3     4*
//      (indirect,X)  ADC (oper,X)  61    2     6
//      (indirect),Y  ADC (oper),Y  71    2     5*
uint8_t ADC(nes_system *nes){

    uint8_t a_prev = nes->cpu.a;
    cpu_fetch(nes);
    uint16_t r = (uint16_t)a_prev + (uint16_t)nes->cpu.fetched + (uint16_t)cpu_get_flag(nes, C);
    nes->cpu.a = r;


    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    cpu_set_flag(nes, C, r > 255);
    cpu_set_flag(nes, V, ( (a_prev & 0x80) && (nes->cpu.fetched & 0x80) && ~(nes->cpu.a & 0x80) ) || ( ~(a_prev & 0x80) && ~(nes->cpu.fetched & 0x80) && (nes->cpu.a & 0x80) ) ); // (a and f and not r) or (not a and not f and r) 

    return 0x01;
    }

// AND  AND Memory with Accumulator
//
//      A AND M -> A                     N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     AND #oper     29    2     2
//      zeropage      AND oper      25    2     3
//      zeropage,X    AND oper,X    35    2     4
//      absolute      AND oper      2D    3     4
//      absolute,X    AND oper,X    3D    3     4*
//      absolute,Y    AND oper,Y    39    3     4*
//      (indirect,X)  AND (oper,X)  21    2     6
//      (indirect),Y  AND (oper),Y  31    2     5*
uint8_t AND(nes_system *nes){
    nes->cpu.a &= cpu_fetch(nes);

    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);

    return 0x01;
    }

// ASL  Shift Left One Bit (Memory or Accumulator)
//
//      C <- [76543210] <- 0             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   ASL A         0A    1     2
//      zeropage      ASL oper      06    2     5
//      zeropage,X    ASL oper,X    16    2     6
//      absolute      ASL oper      0E    3     6
//      absolute,X    ASL oper,X    1E    3     7
uint8_t ASL(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = ((uint16_t)nes->cpu.fetched) << 1;

    if(lookup[nes->cpu.opcode].addrmode == &IMP){
        nes->cpu.a = r;
    }else{
        cpu_write(nes, nes->cpu.addr_abs, r);
    }

    cpu_set_flag(nes, N, r & 0x80);
    cpu_set_flag(nes, Z, r == 0x00);
    cpu_set_flag(nes, C, r > 255);
    return 0x00;
    }


// BCC  Branch on Carry Clear
//
//      branch on C = 0                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BCC oper      90    2     2**
uint8_t BCC(nes_system *nes){
    if(cpu_get_flag(nes, C) == 0){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;

        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}

// BCS  Branch on Carry Set
//
//      branch on C = 1                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BCS oper      B0    2     2**
uint8_t BCS(nes_system *nes){
    if(cpu_get_flag(nes, C)){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;

        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
    }

// BEQ  Branch on Result Zero
//
//      branch on Z = 1                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BEQ oper      F0    2     2**
uint8_t BEQ(nes_system *nes){
    if(cpu_get_flag(nes, Z)){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;

        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}

// BIT  Test Bits in Memory with Accumulator
//
//      bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V);
//      the zeroflag is set to the result of operand AND accumulator.
//
//      A AND M, M7 -> N, M6 -> V        N Z C I D V
//                                      M7 + - - - M6
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      BIT oper      24    2     3
//      absolute      BIT oper      2C    3     4
uint8_t BIT(nes_system *nes){
    cpu_fetch(nes);

    cpu_set_flag(nes, N, nes->cpu.fetched & 0x80);
    cpu_set_flag(nes, V, nes->cpu.fetched & 0x40);
    cpu_set_flag(nes, Z, (nes->cpu.a & nes->cpu.fetched) == 0);

    return 0x00;
}


// BMI  Branch on Result Minus
//
//      branch on N = 1                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BMI oper      30    2     2**
uint8_t BMI(nes_system *nes){
    if(cpu_get_flag(nes, N)){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;

        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}


// BNE  Branch on Result not Zero
//
//      branch on Z = 0                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BNE oper      D0    2     2**
uint8_t BNE(nes_system *nes){
    if(cpu_get_flag(nes, Z) == 0){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;
        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
    }


// BPL  Branch on Result Plus
//
//      branch on N = 0                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BPL oper      10    2     2**
uint8_t BPL(nes_system *nes){
    if(cpu_get_flag(nes, N) == 0){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;
        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}


// BRK  Force Break
//
//      interrupt,                       N Z C I D V
//      push PC+2, push SR               - - - 1 - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       BRK           00    1     7
uint8_t BRK(nes_system *nes){
    nes->cpu.pc++;
	cpu_set_flag(nes, I, 1);
	cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, (nes->cpu.pc >> 8) & 0x00FF);
	nes->cpu.stkp--;
	cpu_write(nes, 0x0100 + nes->cpu.stkp, nes->cpu.pc & 0x00FF);
	nes->cpu.stkp--;

	cpu_set_flag(nes, B, 1);
	cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.status);
	nes->cpu.stkp--;
	cpu_set_flag(nes, B, 0);
	nes->cpu.pc = (uint16_t)cpu_read(nes, 0xFFFE) | ((uint16_t)cpu_read(nes, 0xFFFF) << 8);
    return 0x00;
}


// BVC  Branch on Overflow Clear
//
//      branch on V = 0                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BVC oper      50    2     2**
uint8_t BVC(nes_system *nes){
    if(cpu_get_flag(nes, V) == 0){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;
        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}


// BVS  Branch on Overflow Set
//
//      branch on V = 1                  N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      relative      BVC oper      70    2     2**
uint8_t BVS(nes_system *nes){
    if(cpu_get_flag(nes, V)){
        nes->cpu.addr_abs = nes->cpu.pc + nes->cpu.addr_rel;
        nes->cpu.cycles++;
        if((nes->cpu.pc & 0xFF00) != (nes->cpu.addr_abs & 0xFF00)){
            nes->cpu.cycles++;
        }

        nes->cpu.pc = nes->cpu.addr_abs;
    }
    return 0x00;
}


// CLC  Clear Carry Flag
//
//      0 -> C                           N Z C I D V
//                                       - - 0 - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       CLC           18    1     2
uint8_t CLC(nes_system *nes){
    cpu_set_flag(nes, C, 0);
    return 0x00;
}


// CLD  Clear Decimal Mode
//
//      0 -> D                           N Z C I D V
//                                       - - - - 0 -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       CLD           D8    1     2
uint8_t CLD(nes_system *nes){
    cpu_set_flag(nes, D, 0);
    return 0x00;
}


// CLI  Clear Interrupt Disable Bit
//
//      0 -> I                           N Z C I D V
//                                       - - - 0 - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       CLI           58    1     2
uint8_t CLI(nes_system *nes){
    cpu_set_flag(nes, I, 0);
    return 0x00;
}


// CLV  Clear Overflow Flag
//
//      0 -> V                           N Z C I D V
//                                       - - - - - 0
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       CLV           B8    1     2
uint8_t CLV(nes_system *nes){
    cpu_set_flag(nes, V, 0);
    return 0x00;
}


// CMP  Compare Memory with Accumulator
//
//      A - M                            N Z C I D V
//                                     + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     CMP #oper     C9    2     2
//      zeropage      CMP oper      C5    2     3
//      zeropage,X    CMP oper,X    D5    2     4
//      absolute      CMP oper      CD    3     4
//      absolute,X    CMP oper,X    DD    3     4*
//      absolute,Y    CMP oper,Y    D9    3     4*
//      (indirect,X)  CMP (oper,X)  C1    2     6
//      (indirect),Y  CMP (oper),Y  D1    2     5*
uint8_t CMP(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = (uint16_t)nes->cpu.a - (uint16_t)nes->cpu.fetched;


    cpu_set_flag(nes, C, nes->cpu.a >= nes->cpu.fetched);
    cpu_set_flag(nes, N, r & 0x0080);
    cpu_set_flag(nes, Z, (r & 0x00FF) == 0x0000);
    return 0x01;
}


// CPX  Compare Memory and Index X
//
//      X - M                            N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     CPX #oper     E0    2     2
//      zeropage      CPX oper      E4    2     3
//      absolute      CPX oper      EC    3     4
uint8_t CPX(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = (uint16_t)nes->cpu.x - (uint16_t)nes->cpu.fetched;


    cpu_set_flag(nes, C, nes->cpu.x >= nes->cpu.fetched);
    cpu_set_flag(nes, N, r & 0x0080);
    cpu_set_flag(nes, Z, (r & 0x00FF) == 0x0000);
    return 0x00;
}


// CPY  Compare Memory and Index Y
//
//      Y - M                            N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     CPY #oper     C0    2     2
//      zeropage      CPY oper      C4    2     3
//      absolute      CPY oper      CC    3     4
uint8_t CPY(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = (uint16_t)nes->cpu.y - (uint16_t)nes->cpu.fetched;


    cpu_set_flag(nes, C, nes->cpu.y >= nes->cpu.fetched);
    cpu_set_flag(nes, N, r & 0x0080);
    cpu_set_flag(nes, Z, (r & 0x00FF) == 0x0000);
    return 0x00;
}


// DEC  Decrement Memory by One
//
//      M - 1 -> M                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      DEC oper      C6    2     5
//      zeropage,X    DEC oper,X    D6    2     6
//      absolute      DEC oper      CE    3     6
//      absolute,X    DEC oper,X    DE    3     7
uint8_t DEC(nes_system *nes){
    cpu_fetch(nes);
    uint8_t r = nes->cpu.fetched -1;
    cpu_write(nes, nes->cpu.addr_abs, r);

    cpu_set_flag(nes, N, r & 0x80);
    cpu_set_flag(nes, Z, r == 0x00);
    return 0x00;
}


// DEX  Decrement Index X by One
//
//      X - 1 -> X                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       DEC           CA    1     2
uint8_t DEX(nes_system *nes){
    nes->cpu.x--;

    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x00;
}


// DEY  Decrement Index Y by One
//
//      Y - 1 -> Y                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       DEC           88    1     2
uint8_t DEY(nes_system *nes){
    nes->cpu.y--;

    cpu_set_flag(nes, N, nes->cpu.y & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.y == 0x00);
    return 0x00;
}


// EOR  Exclusive-OR Memory with Accumulator
//
//      A EOR M -> A                     N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     EOR #oper     49    2     2
//      zeropage      EOR oper      45    2     3
//      zeropage,X    EOR oper,X    55    2     4
//      absolute      EOR oper      4D    3     4
//      absolute,X    EOR oper,X    5D    3     4*
//      absolute,Y    EOR oper,Y    59    3     4*
//      (indirect,X)  EOR (oper,X)  41    2     6
//      (indirect),Y  EOR (oper),Y  51    2     5*
uint8_t EOR(nes_system *nes){
    cpu_fetch(nes);
    nes->cpu.a ^= nes->cpu.fetched;

    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x01;
}


// INC  Increment Memory by One
//
//      M + 1 -> M                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      INC oper      E6    2     5
//      zeropage,X    INC oper,X    F6    2     6
//      absolute      INC oper      EE    3     6
//      absolute,X    INC oper,X    FE    3     7
uint8_t INC(nes_system *nes){
    cpu_fetch(nes);
    uint8_t r = nes->cpu.fetched + 1;
    cpu_write(nes, nes->cpu.addr_abs, r);

    cpu_set_flag(nes, N, r & 0x80);
    cpu_set_flag(nes, Z, r == 0x00);
    return 0x00;
}


// INX  Increment Index X by One
//
//      X + 1 -> X                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       INX           E8    1     2
uint8_t INX(nes_system *nes){
     nes->cpu.x++;

    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x00;
}


// INY  Increment Index Y by One
//
//      Y + 1 -> Y                       N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       INY           C8    1     2
uint8_t INY(nes_system *nes){
     nes->cpu.y++;

    cpu_set_flag(nes, N, nes->cpu.y & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.y == 0x00);
    return 0x00;
}


// JMP  Jump to New Location
//
//      (PC+1) -> PCL                    N Z C I D V
//      (PC+2) -> PCH                    - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      absolute      JMP oper      4C    3     3
//      indirect      JMP (oper)    6C    3     5
uint8_t JMP(nes_system *nes){
    nes->cpu.pc = nes->cpu.addr_abs;

    return 0x00;
}


// JSR  Jump to New Location Saving Return Address
//
//      push (PC+2),                     N Z C I D V
//      (PC+1) -> PCL                    - - - - - -
//      (PC+2) -> PCH
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      absolute      JSR oper      20    3     6
uint8_t JSR(nes_system *nes){
    
    cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, (nes->cpu.pc >> 8) & 0x00FF);
    nes->cpu.stkp--;
    cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.pc & 0x00FF);
    nes->cpu.stkp--;

    nes->cpu.pc = nes->cpu.addr_abs;
    
    return 0x00;
}


// LDA  Load Accumulator with Memory
//
//      M -> A                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     LDA #oper     A9    2     2
//      zeropage      LDA oper      A5    2     3
//      zeropage,X    LDA oper,X    B5    2     4
//      absolute      LDA oper      AD    3     4
//      absolute,X    LDA oper,X    BD    3     4*
//      absolute,Y    LDA oper,Y    B9    3     4*
//      (indirect,X)  LDA (oper,X)  A1    2     6
//      (indirect),Y  LDA (oper),Y  B1    2     5*
uint8_t LDA(nes_system *nes){
    cpu_fetch(nes);
    nes->cpu.a = nes->cpu.fetched;

    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x01;
}		


// LDX  Load Index X with Memory
//
//      M -> X                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     LDX #oper     A2    2     2
//      zeropage      LDX oper      A6    2     3
//      zeropage,Y    LDX oper,Y    B6    2     4
//      absolute      LDX oper      AE    3     4
//      absolute,Y    LDX oper,Y    BE    3     4*
uint8_t LDX(nes_system *nes){
    cpu_fetch(nes);
    nes->cpu.x = nes->cpu.fetched;

    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x01;
}


// LDY  Load Index Y with Memory
//
//      M -> Y                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     LDY #oper     A0    2     2
//      zeropage      LDY oper      A4    2     3
//      zeropage,X    LDY oper,X    B4    2     4
//      absolute      LDY oper      AC    3     4
//      absolute,X    LDY oper,X    BC    3     4*
uint8_t LDY(nes_system *nes){
    cpu_fetch(nes);
    nes->cpu.y = nes->cpu.fetched;

    cpu_set_flag(nes, N, nes->cpu.y & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.y == 0x00);
    return 0x01;
}


// LSR  Shift One Bit Right (Memory or Accumulator)
//
//      0 -> [76543210] -> C             N Z C I D V
//                                       0 + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   LSR A         4A    1     2
//      zeropage      LSR oper      46    2     5
//      zeropage,X    LSR oper,X    56    2     6
//      absolute      LSR oper      4E    3     6
//      absolute,X    LSR oper,X    5E    3     7
uint8_t LSR(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = ((uint16_t)nes->cpu.fetched) >> 1;

    if(lookup[nes->cpu.opcode].addrmode == &IMP){
        nes->cpu.a = r;
    }else{
        cpu_write(nes, nes->cpu.addr_abs, r);
    }

    cpu_set_flag(nes, N, 0);
    cpu_set_flag(nes, Z, r == 0x00);
    cpu_set_flag(nes, C, r > 255);
    return 0x00;
}


// NOP  No Operation
//
//      ---                              N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       NOP           EA    1     2
uint8_t NOP(nes_system *nes){
    
    return 0x00;
}


// ORA  OR Memory with Accumulator
//
//      A OR M -> A                      N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     ORA #oper     09    2     2
//      zeropage      ORA oper      05    2     3
//      zeropage,X    ORA oper,X    15    2     4
//      absolute      ORA oper      0D    3     4
//      absolute,X    ORA oper,X    1D    3     4*
//      absolute,Y    ORA oper,Y    19    3     4*
//      (indirect,X)  ORA (oper,X)  01    2     6
//      (indirect),Y  ORA (oper),Y  11    2     5*
uint8_t ORA(nes_system *nes){
    cpu_fetch(nes);
    nes->cpu.a |= nes->cpu.fetched;

    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x01;
}


// PHA  Push Accumulator on Stack
//
//      push A                           N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       PHA           48    1     3
uint8_t PHA(nes_system *nes){
    cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.a);
    nes->cpu.stkp--;    

    return 0x00;
    }

// PHP  Push Processor Status on Stack
//
//      push SR                          N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       PHP           08    1     3
uint8_t PHP(nes_system *nes){
    cpu_write(nes, nes->cpu.stkbase + nes->cpu.stkp, nes->cpu.status);
    nes->cpu.stkp--;
    return 0x00;
}


// PLA  Pull Accumulator from Stack
//
//      pull A                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       PLA           68    1     4
uint8_t PLA(nes_system *nes){
    nes->cpu.stkp++;
    nes->cpu.a = cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp);

    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    
    return 0x00;
    }

// PLP  Pull Processor Status from Stack
//
//      pull SR                          N Z C I D V
//                                       from stack
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       PLP           28    1     4
uint8_t PLP(nes_system *nes){
    nes->cpu.stkp++;
    nes->cpu.status = (nes->cpu.stkbase + nes->cpu.stkp);
    return 0x00;
}


// ROL  Rotate One Bit Left (Memory or Accumulator)
//
//      C <- [76543210] <- C             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   ROL A         2A    1     2
//      zeropage      ROL oper      26    2     5
//      zeropage,X    ROL oper,X    36    2     6
//      absolute      ROL oper      2E    3     6
//      absolute,X    ROL oper,X    3E    3     7
uint8_t ROL(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = ((uint16_t)nes->cpu.fetched << 1) | cpu_get_flag(nes, C);

    cpu_set_flag(nes, C, r & 0xFF00);
    cpu_set_flag(nes, N, r & 0x80);
    cpu_set_flag(nes, Z, (r & 0x00FF) == 0);
    if(lookup[nes->cpu.opcode].addrmode == &IMP){
        nes->cpu.a = r & 0x00FF;
    }else{
        cpu_write(nes, nes->cpu.addr_abs, r & 0x00FF);
    }
    return 0x00;
}


// ROR  Rotate One Bit Right (Memory or Accumulator)
//
//      C -> [76543210] -> C             N Z C I D V
//                                       + + + - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      accumulator   ROR A         6A    1     2
//      zeropage      ROR oper      66    2     5
//      zeropage,X    ROR oper,X    76    2     6
//      absolute      ROR oper      6E    3     6
//      absolute,X    ROR oper,X    7E    3     7
uint8_t ROR(nes_system *nes){
    cpu_fetch(nes);
    uint16_t r = (nes->cpu.fetched >> 1) | (cpu_get_flag(nes, C) << 7);

    cpu_set_flag(nes, C, nes->cpu.fetched & 0x01);
    cpu_set_flag(nes, N, r & 0x80);
    cpu_set_flag(nes, Z, (r & 0x00FF) == 0);
    if(lookup[nes->cpu.opcode].addrmode == &IMP){
        nes->cpu.a = r & 0x00FF;
    }else{
        cpu_write(nes, nes->cpu.addr_abs, r & 0x00FF);
    }
    return 0x00;
}


// RTI  Return from Interrupt
//
//      pull SR, pull PC                 N Z C I D V
//                                       from stack
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       RTI           40    1     6
uint8_t RTI(nes_system *nes){
    nes->cpu.stkp++;
	nes->cpu.status = cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp);
	nes->cpu.status &= ~B;
	nes->cpu.status &= ~U;

	nes->cpu.stkp++;
	nes->cpu.pc = (uint16_t)cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp);
	nes->cpu.stkp++;
	nes->cpu.pc |= (uint16_t)cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp) << 8;

    return 0x00;
    }

// RTS  Return from Subroutine
//
//      pull PC, PC+1 -> PC              N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       RTS           60    1     6
uint8_t RTS(nes_system *nes){
    nes->cpu.stkp++;
	nes->cpu.pc = (uint16_t)cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp);
    nes->cpu.stkp++;
	nes->cpu.pc |= (uint16_t)cpu_read(nes, nes->cpu.stkbase + nes->cpu.stkp) << 8;

    return 0x00;
}


// SBC  Subtract Memory from Accumulator with Borrow
//
//      A - M - C -> A                   N Z C I D V
//                                       + + + - - +
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      immidiate     SBC #oper     E9    2     2
//      zeropage      SBC oper      E5    2     3
//      zeropage,X    SBC oper,X    F5    2     4
//      absolute      SBC oper      ED    3     4
//      absolute,X    SBC oper,X    FD    3     4*
//      absolute,Y    SBC oper,Y    F9    3     4*
//      (indirect,X)  SBC (oper,X)  E1    2     6
//      (indirect),Y  SBC (oper),Y  F1    2     5*
uint8_t SBC(nes_system *nes){
    uint8_t a_prev = nes->cpu.a;
    cpu_fetch(nes);
    nes->cpu.fetched = (nes->cpu.fetched ^ 0xFF) + 1;
    uint16_t r = (uint16_t)a_prev + (uint16_t)nes->cpu.fetched + (uint16_t)cpu_get_flag(nes, C);
    nes->cpu.a = r;


    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    cpu_set_flag(nes, C, r > 255);
    cpu_set_flag(nes, V, ( (a_prev & 0x80) && (nes->cpu.fetched & 0x80) && ~(nes->cpu.a & 0x80) ) || ( ~(a_prev & 0x80) && ~(nes->cpu.fetched & 0x80) && (nes->cpu.a & 0x80) ) ); // (a and f and not r) or (not a and not f and r) 

    return 0x01;
    }

// SEC  Set Carry Flag
//
//      1 -> C                           N Z C I D V
//                                       - - 1 - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       SEC           38    1     2
uint8_t SEC(nes_system *nes){
    cpu_set_flag(nes, C, 1);
    return 0x00;
}


// SED  Set Decimal Flag
//
//      1 -> D                           N Z C I D V
//                                       - - - - 1 -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       SED           F8    1     2
uint8_t SED(nes_system *nes){
    cpu_set_flag(nes, D, 1);
    return 0x00;
}


// SEI  Set Interrupt Disable Status
//
//      1 -> I                           N Z C I D V
//                                       - - - 1 - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       SEI           78    1     2
uint8_t SEI(nes_system *nes){
    cpu_set_flag(nes, I, 1);
    return 0x00;
}


// STA  Store Accumulator in Memory
//
//      A -> M                           N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      STA oper      85    2     3
//      zeropage,X    STA oper,X    95    2     4
//      absolute      STA oper      8D    3     4
//      absolute,X    STA oper,X    9D    3     5
//      absolute,Y    STA oper,Y    99    3     5
//      (indirect,X)  STA (oper,X)  81    2     6
//      (indirect),Y  STA (oper),Y  91    2     6
uint8_t STA(nes_system *nes){
    cpu_write(nes, nes->cpu.addr_abs, nes->cpu.a);
    return 0x00;
}


// STX  Store Index X in Memory
//
//      X -> M                           N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      STX oper      86    2     3
//      zeropage,Y    STX oper,Y    96    2     4
//      absolute      STX oper      8E    3     4
uint8_t STX(nes_system *nes){
    cpu_write(nes, nes->cpu.addr_abs, nes->cpu.x);
    return 0x00;
}


// STY  Sore Index Y in Memory
//
//      Y -> M                           N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      zeropage      STY oper      84    2     3
//      zeropage,X    STY oper,X    94    2     4
//      absolute      STY oper      8C    3     4
uint8_t STY(nes_system *nes){
    cpu_write(nes, nes->cpu.addr_abs, nes->cpu.y);
    return 0x00;
}


// TAX  Transfer Accumulator to Index X
//
//      A -> X                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TAX           AA    1     2
uint8_t TAX(nes_system *nes){
    nes->cpu.x = nes->cpu.a;
    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x00;
}


// TAY  Transfer Accumulator to Index Y
//
//      A -> Y                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TAY           A8    1     2
uint8_t TAY(nes_system *nes){
    nes->cpu.y = nes->cpu.a;
    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x00;
}


// TSX  Transfer Stack Pointer to Index X
//
//      SP -> X                          N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TSX           BA    1     2
uint8_t TSX(nes_system *nes){
    nes->cpu.x = nes->cpu.stkp;
    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x00;
}


// TXA  Transfer Index X to Accumulator
//
//      X -> A                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TXA           8A    1     2
uint8_t TXA(nes_system *nes){
    nes->cpu.a = nes->cpu.x;
    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x00;
}


// TXS  Transfer Index X to Stack Register
//
//      X -> SP                          N Z C I D V
//                                       - - - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TXS           9A    1     2
uint8_t TXS(nes_system *nes){
    nes->cpu.stkp = nes->cpu.x;
    cpu_set_flag(nes, N, nes->cpu.x & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.x == 0x00);
    return 0x00;
}


// TYA  Transfer Index Y to Accumulator
//
//      Y -> A                           N Z C I D V
//                                       + + - - - -
//
//      addressing    assembler    opc  bytes  cyles
//      --------------------------------------------
//      implied       TYA           98    1     2
uint8_t TYA(nes_system *nes){
    nes->cpu.a = nes->cpu.y;
    cpu_set_flag(nes, N, nes->cpu.a & 0x80);
    cpu_set_flag(nes, Z, nes->cpu.a == 0x00);
    return 0x00;
}

// XXX  Illegal opcode
uint8_t XXX(nes_system *nes){

    return 0x00;
}