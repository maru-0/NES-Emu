#include <cartridge.h>
#include <stdlib.h>
#include <stdio.h>
#include "mappers.h"
#include "bus.h"

cartridge inserted_cart;

void cartridge_init(cartridge *cart, char* path){
    
    FILE *fp = fopen(path, "rb");

    // Read the header, determine the mapper, assign the mapper function and assign the mirroring type
    fread(&(cart->header), sizeof(header_t), 1, fp);
    
    cart->mapper_id = (cart->header.mapper2 & 0xF0) | (cart->header.mapper1 >> 4);
    assign_mapper(cart);

    cart->mirror = (cart->header.mapper1 & 0x01) ? VERTICAL: HORIZONTAL;

    // Ignore the trainer data
    uint8_t trainer_temp[512];
    fread(trainer_temp, (cart->header.mapper1 >> 2) & 0x01, 512, fp);


    // Here is where you would discover the the file format.
    // There are 3 types of ines files.
    // I'm gonna assume they are always type 1 instead.


    cart->prg = (uint8_t *)malloc(cart->header.prg_rom_chunks * 16384);
    fread(cart->prg, 16384, cart->header.prg_rom_chunks, fp);

    cart->chr = (uint8_t *)malloc(cart->header.chr_rom_chunks * 8192);
    fread(cart->chr, 8192, cart->header.chr_rom_chunks, fp);


    fclose(fp);
}

void cartridge_load(nes_system *nes, char *path){
    
    cartridge_init(&(nes->inserted_cart), path);
    
}