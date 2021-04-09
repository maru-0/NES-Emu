#include <stdint.h>
#include "ppu_2C02.h"
#include "bus.h"


void ppu_init(ppu_2C02 *ppu){
    ppu->address_latch = 0x00;
    ppu->ppu_data_buffer = 0x00;
    ppu->scanline = 0;
    ppu->cycle = 0;
    ppu->nmi_flag = 0;
}

void ppu_clock(nes_system *nes){

    if(nes->ppu.scanline == 241 && nes->ppu.cycle == 1){
        nes->ppu.status.vertical_blank = 1;
    }
}

uint32_t colors[0x40] = {
0xFF545454,
0xFF001E74,
0xFF081090,
0xFF300088,
0xFF440064,
0xFF5C0030,
0xFF540400,
0xFF3C1800,
0xFF202A00,
0xFF083A00,
0xFF004000,
0xFF003C00,
0xFF00323C,
0xFF000000,
0xFF000000,
0xFF000000,
0xFF989698,
0xFF084CC4,
0xFF3032EC,
0xFF5C1EE4,
0xFF8814B0,
0xFFA01464,
0xFF982220,
0xFF783C00,
0xFF545A00,
0xFF287200,
0xFF087C00,
0xFF007628,
0xFF006678,
0xFF000000,
0xFF000000,
0xFF000000,
0xFFECEEEC,
0xFF4C9AEC,
0xFF787CEC,
0xFFB062EC,
0xFFE454EC,
0xFFEC58B4,
0xFFEC6A64,
0xFFD48820,
0xFFA0AA00,
0xFF746400,
0xFF4CD020,
0xFF38CC6C,
0xFF38B4CC,
0xFF3C3C3C,
0xFF000000,
0xFF000000,
0xFFECEEEC,
0xFFA8CCEC,
0xFFBCBCEC,
0xFFD4B2EC,
0xFFECAEEC,
0xFFECAED4,
0xFFECB4B0,
0xFFE4C490,
0xFFCCD278,
0xFFB4DE78,
0xFFA8E290,
0xFF98E2B4,
0xFFA0D6E4,
0xFFA0A2A0,
0xFF000000,
0xFF000000};



uint32_t get_color(nes_system *nes,uint8_t pal,uint8_t color_i){
	return colors[ppu_read(nes, 0x3F00 + pal * 4 + color_i) & 0x3F];
}

// Sets the pattern table pixel matrix with the given palette offset (0 through 7)
void get_pattern_table(nes_system *nes, uint8_t i, uint8_t pal){
	for (uint16_t tileY = 0; tileY < 16; tileY++){
		for (uint16_t tileX = 0; tileX < 16; tileX++){
			uint16_t offset = tileY * 256 + tileX * 16;
			for (uint16_t row = 0; row < 8; row++){
				uint8_t lsb = ppu_read(nes, i * 0x1000 + offset + row);
				uint8_t msb = ppu_read(nes, i * 0x1000 + offset + row + 8);
				for(uint16_t col = 0; col < 8; col++){
					uint8_t color_i = ((msb & 0x01) << 1) + (lsb & 0x01);
					msb >>= 1;
					lsb >>= 1;
					// nes->ppu.px_pattern_table[i][tileX * 8 + (7 - col)][tileY * 8 + row].ARGB = get_color(nes, pal, color_i);
					nes->ppu.px_pattern_table[i][tileY * 8 + row][tileX * 8 + (7 - col)].ARGB = get_color(nes, pal, color_i);
				}
			}
		}	
	}
}




uint8_t ppu_access_read(nes_system *nes, uint16_t addr){
    uint8_t data = 0x00;
    switch (addr)
		{
		case 0x0000: // Control
			break;
		case 0x0001: // Mask
			break;
		case 0x0002: // Status
			nes->ppu.status.vertical_blank = 1; //gambiarra
			data = (nes->ppu.status.reg & 0xE0) | (nes->ppu.ppu_data_buffer & 0x10); // 3 bits of flags and 5 of noise
            nes->ppu.status.vertical_blank = 0;
            nes->ppu.address_latch = 0;
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
            data = nes->ppu.ppu_data_buffer;
            nes->ppu.ppu_data_buffer = ppu_read(nes, nes->ppu.ppu_address);

			if (nes->ppu.ppu_address >= 0x3F00){
				data = nes->ppu.ppu_data_buffer;
			}
			
            // faltam coisinhas, comportamento diferente quando lê endereço dos pallettes
            nes->ppu.ppu_address += (nes->ppu.control.increment_mode ? 32: 1);
			break;
		}
    return data;
}

void ppu_access_write(nes_system *nes, uint16_t addr, uint8_t data){
    switch (addr)
		{
		case 0x0000: // Control
			nes->ppu.control.reg = data;
			break;
		case 0x0001: // Mask
			nes->ppu.mask.reg = data;
			break;
		case 0x0002: // Status
			nes->ppu.status.reg = data;
			break;
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
            if(nes->ppu.address_latch == 0x00){
                nes->ppu.ppu_address = (nes->ppu.ppu_address & 0x00FF) | (((uint16_t)data) << 8);
                nes->ppu.address_latch = 0x01;
            }else{
                nes->ppu.ppu_address = (nes->ppu.ppu_address & 0xFF00) | (uint16_t)data;
                nes->ppu.address_latch = 0x00;
            }
			break;
		case 0x0007: // PPU Data
            ppu_write(nes, nes->ppu.ppu_address, data);

            nes->ppu.ppu_address += (nes->ppu.control.increment_mode ? 32: 1);
            
			break;
		}
}


uint8_t ppu_read(nes_system *nes, uint16_t addr){
    uint8_t data = 0x00;
	addr &= 0x3FFF;

    if(addr >= 0x0000 && addr <= 0x1FFF){ // Pattern tables
		data = nes->inserted_cart.chr[nes->inserted_cart.mapper_f(addr, nes->inserted_cart.header.prg_rom_chunks, nes->inserted_cart.header.chr_rom_chunks)];
    }else if(addr >= 0x2000 && addr <= 0x3EFF){ // Nametables - VRAM
        // Resolver as bagaças de mirroring, bruxaria com bits
    }else if(addr >= 0x3F00 && addr <= 0x3FFF){ // Palletes
		addr &= 0x001F;						// deal with loopback to universal background later
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = nes->ppu.palletes[addr];
    }


	return data;
}

void ppu_write(nes_system *nes, uint_fast16_t addr, uint8_t data){
    addr &= 0x3FFF;

	 if(addr >= 0x0000 && addr <= 0x1FFF){ // Pattern tables
		nes->inserted_cart.chr[nes->inserted_cart.mapper_f(addr, nes->inserted_cart.header.prg_rom_chunks, nes->inserted_cart.header.chr_rom_chunks)] = data;
    }else if(addr >= 0x2000 && addr <= 0x3EFF){ // Nametables - VRAM
        // Resolver as bagaças de mirroring, bruxaria com bits
    }else if(addr >= 0x3F00 && addr <= 0x3FFF){ // Palletes

    }
}



// uint8_t colors[0x40];
// colors[0x00] = (uint8_t)0x545454FF;
// colors[0x01] = (uint8_t)0x001E74FF;
// colors[0x02] = (uint8_t)0x081090FF;
// colors[0x03] = (uint8_t)0x300088FF;
// colors[0x04] = (uint8_t)0x440064FF;
// colors[0x05] = (uint8_t)0x5C0030FF;
// colors[0x06] = (uint8_t)0x540400FF;
// colors[0x07] = (uint8_t)0x3C1800FF;
// colors[0x08] = (uint8_t)0x202A00FF;
// colors[0x09] = (uint8_t)0x083A00FF;
// colors[0x0A] = (uint8_t)0x004000FF;
// colors[0x0B] = (uint8_t)0x003C00FF;
// colors[0x0C] = (uint8_t)0x00323CFF;
// colors[0x0D] = (uint8_t)0x000000FF;
// colors[0x0E] = (uint8_t)0x000000FF;
// colors[0x0F] = (uint8_t)0x000000FF;

// colors[0x10] = (uint8_t)0x989698FF;
// colors[0x11] = (uint8_t)0x084CC4FF;
// colors[0x12] = (uint8_t)0x3032ECFF;
// colors[0x13] = (uint8_t)0x5C1EE4FF;
// colors[0x14] = (uint8_t)0x8814B0FF;
// colors[0x15] = (uint8_t)0xA01464FF;
// colors[0x16] = (uint8_t)0x982220FF;
// colors[0x17] = (uint8_t)0x783C00FF;
// colors[0x18] = (uint8_t)0x545A00FF;
// colors[0x19] = (uint8_t)0x287200FF;
// colors[0x1A] = (uint8_t)0x087C00FF;
// colors[0x1B] = (uint8_t)0x007628FF;
// colors[0x1C] = (uint8_t)0x006678FF;
// colors[0x1D] = (uint8_t)0x000000FF;
// colors[0x1E] = (uint8_t)0x000000FF;
// colors[0x1F] = (uint8_t)0x000000FF;

// colors[0x20] = (uint8_t)0xECEEECFF;
// colors[0x21] = (uint8_t)0x4C9AECFF;
// colors[0x22] = (uint8_t)0x787CECFF;
// colors[0x23] = (uint8_t)0xB062ECFF;
// colors[0x24] = (uint8_t)0xE454ECFF;
// colors[0x25] = (uint8_t)0xEC58B4FF;
// colors[0x26] = (uint8_t)0xEC6A64FF;
// colors[0x27] = (uint8_t)0xD48820FF;
// colors[0x28] = (uint8_t)0xA0AA00FF;
// colors[0x29] = (uint8_t)0x746400FF;
// colors[0x2A] = (uint8_t)0x4CD020FF;
// colors[0x2B] = (uint8_t)0x38CC6CFF;
// colors[0x2C] = (uint8_t)0x38B4CCFF;
// colors[0x2D] = (uint8_t)0x3C3C3CFF;
// colors[0x2E] = (uint8_t)0x000000FF;
// colors[0x2F] = (uint8_t)0x000000FF;

// colors[0x30] = (uint8_t)0xECEEECFF;
// colors[0x31] = (uint8_t)0xA8CCECFF;
// colors[0x32] = (uint8_t)0xBCBCECFF;
// colors[0x33] = (uint8_t)0xD4B2ECFF;
// colors[0x34] = (uint8_t)0xECAEECFF;
// colors[0x35] = (uint8_t)0xECAED4FF;
// colors[0x36] = (uint8_t)0xECB4B0FF;
// colors[0x37] = (uint8_t)0xE4C490FF;
// colors[0x38] = (uint8_t)0xCCD278FF;
// colors[0x39] = (uint8_t)0xB4DE78FF;
// colors[0x3A] = (uint8_t)0xA8E290FF;
// colors[0x3B] = (uint8_t)0x98E2B4FF;
// colors[0x3C] = (uint8_t)0xA0D6E4FF;
// colors[0x3D] = (uint8_t)0xA0A2A0FF;
// colors[0x3E] = (uint8_t)0x000000FF;
// colors[0x3F] = (uint8_t)0x000000FF;