#ifndef _MAPPERS_H_
#define _MAPPERS_H_
#include "cartridge.h"


void assign_mapper(cartridge * cart);

uint16_t map_address(cartridge *cart, uint16_t addr);

#endif