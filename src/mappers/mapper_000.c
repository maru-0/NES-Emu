#include <stdint.h>
#include <stdio.h>

// CPU $6000-$7FFF: Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB window, write protectable with an external switch
// CPU $8000-$BFFF: First 16 KB of ROM.
// CPU $C000-$FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
uint16_t mapper_000(uint16_t addr, uint8_t n_prg_chunks, uint8_t n_chr_chunks){
    uint16_t mapped_addr = 0;
    if(addr >= 0x8000 && addr <= 0xFFFF){
        mapped_addr = addr & (n_prg_chunks == 1 ? 0x3FFF: 0x7FFF);
    }else if(addr >= 0x0000 && addr <= 0x1FFF){
        mapped_addr = addr;
    }

    return mapped_addr;
}