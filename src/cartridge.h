#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_
#include <stdint.h>


typedef struct header{
    char name[4];               // 0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
    uint8_t prg_rom_chunks;     // 4: Size of PRG ROM in 16 KB units
    uint8_t chr_rom_chunks;     // 5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
    uint8_t mapper1;            // 6: Flags 6 - Mapper, mirroring, battery, trainer
    uint8_t mapper2;            // 7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
    uint8_t prg_ram_size;       // 8: Flags 8 - PRG-RAM size (rarely used extension)
    uint8_t tv_system1;         // 9: Flags 9 - TV system (rarely used extension)
    uint8_t tv_system2;         // 10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
    char unused[5];             // 11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
} header_t;

typedef struct cartridge{
    header_t header;
    uint8_t *prg;
    uint8_t *chr;

    uint16_t (*mapper_f)(uint16_t, uint8_t, uint8_t);    // Mapper function
    uint8_t mapper_id; 

    enum MIRROR{
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    }mirror;
}cartridge;

#include "bus.h"

// Initializes "cart" with data based on the ines rom indicated by "path"
void cartridge_init(cartridge *cart, char* path);

// Wrapper to call "cartridge_init()" passing the global "inserted_cart" variable and the given "path"
void cartridge_load(nes_system *nes, char *path);

#endif