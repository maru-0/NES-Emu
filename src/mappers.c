#include <mappers.h>
#include "cartridge.h"
#include <stdlib.h>

#include "mappers/mapper_000.c"

void assign_mapper(cartridge * cart){
    switch (cart->mapper_id)
    {
    case 0x00:
        cart->mapper_f = &mapper_000;
        break;
    
    default:
        cart->mapper_f = NULL;
        break;
    }
}

uint16_t map_address(cartridge *cart, uint16_t addr){
    return cart->mapper_f(addr, cart->header.prg_rom_chunks, cart->header.chr_rom_chunks);
}