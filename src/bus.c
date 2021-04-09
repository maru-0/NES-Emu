#include "bus.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void system_init(nes_system *nes){
    cpu_init(nes);
    ppu_init(&(nes->ppu));
    nes->system_clock_counter = 0;
} // lembrar de inicializar o system clock counter com 0

void system_clock(nes_system *nes){

    if(nes->ppu.scanline == -1){
        nes->ppu.status.vertical_blank = 0;
    }

    ppu_clock(nes);
    if(nes->system_clock_counter % 3 == 0){
        cpu_clock(nes);
    }

    if(nes->ppu.nmi_flag){
        nes->ppu.nmi_flag = 0;
        cpu_nmi(nes);
    }
    

    nes->system_clock_counter++;
}

void system_reset();

// Returns data read from address "addr".
uint8_t cpu_read(nes_system *nes, uint16_t addr){
    uint8_t data = 0x0000;
    if (addr >= 0x0000 && addr <= 0x1FFF){          // Ram 
        data = nes->ram[addr & 0X07FF];
    }else if (addr >= 0x2000 && addr <= 0x3FFF){    // PPU, for mirroring, mask with 0x0007
        data = ppu_access_read(nes, addr & 0x0007);
    }else if (addr >= 0x4020 && addr <= 0xFFFF){    // Cartridge
        data = nes->inserted_cart.prg[nes->inserted_cart.mapper_f(addr, nes->inserted_cart.header.prg_rom_chunks, nes->inserted_cart.header.chr_rom_chunks)];
    }
    
    return data;
}

// Write value of "data" on address "addr".
void cpu_write(nes_system *nes, uint_fast16_t addr, uint8_t data){
    if (addr >= 0x0000 && addr <=0x1FFF){           // Ram 
        nes->ram[addr & 0x07FF] = data;
    }else if (addr >= 0x2000 && addr <= 0x3FFF){    // PPU, for mirroring mask with 0x0007
        ppu_access_write(nes, addr & 0x0007, data);
    }else if (addr >= 0x4020 && addr <= 0xFFFF){    // Cartridge
        nes->inserted_cart.prg[nes->inserted_cart.mapper_f(addr, nes->inserted_cart.header.prg_rom_chunks, nes->inserted_cart.header.chr_rom_chunks)] = data;
    }
    
}

