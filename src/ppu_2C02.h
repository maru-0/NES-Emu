#ifndef _PPU_H_
#define _PPU_H_

typedef union {
	struct{
		uint32_t A : 8;
		uint32_t R : 8;
		uint32_t G : 8;
		uint32_t B : 8;
	};
	uint32_t ARGB;
}pixel;



typedef struct ppu_2C02{

    uint8_t nametable[2][1024]; // VRAM
	uint8_t palletes[32];

	// For rendering
	pixel px_pattern_table[2][128][128];

    union{
		struct{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};
		uint8_t reg;
	} status;

	union{
		struct{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};
		uint8_t reg;
	} mask;

	union PPUCTRL{
		struct{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};
		uint8_t reg;
	} control;

    // Internal communications
	uint8_t address_latch;
    uint8_t ppu_data_buffer;
    uint16_t ppu_address;


    int16_t scanline;
	int16_t cycle;

    uint8_t nmi_flag;
}ppu_2C02;

#include "bus.h"

void ppu_init(ppu_2C02 *ppu);

void ppu_clock(nes_system *nes);

uint32_t get_color(nes_system *nes,uint8_t pal,uint8_t color_i);

void get_pattern_table(nes_system *nes, uint8_t i, uint8_t pal);

uint8_t ppu_access_read(nes_system *nes, uint16_t addr);

void ppu_access_write(nes_system *nes, uint16_t addr, uint8_t data);

uint8_t ppu_read(nes_system *nes,uint16_t addr);

void ppu_write(nes_system *nes,uint_fast16_t addr, uint8_t data);


#endif