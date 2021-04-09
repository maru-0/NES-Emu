#include <stdio.h>

#include "bus.h"
#include "cpu.h"
#include "ppu_2C02.h"
#include "cartridge.h"
#include <rendering.h>

#include <SDL2/SDL.h>


int main(int argc, char *argv[]){
    nes_system nes;
    cartridge_load(&nes, argv[1]);
    system_init(&nes);
    // for(int i = 0; i < nes.inserted_cart.header.chr_rom_chunks* 8192; i++){
    //     printf("%x \n", *(nes.inserted_cart.chr+i));
    // }
    if(SDL_Init(!SDL_INIT_VIDEO)) SDL_Log("Can't init %s", SDL_GetError());

    SDL_Window * window = SDL_CreateWindow("Uhul", 100, 100, 600, 500, 0);
    SDL_Event event;

    
    SDL_Surface *screen;

    screen = SDL_GetWindowSurface(window);

    uint32_t test[100][200];

    for (uint32_t i = 0; i < 100 ;i++){
        for (uint32_t j = 0; j < 200; j++){
            test[i][j] = i < 50 ? 0xFFFF0000 : 0xFFFFFFFF;
            
        }
    }

    nes.ppu.palletes[0] = 0x21;
    nes.ppu.palletes[1] = 0x19;
    nes.ppu.palletes[2] = 0x16;
    nes.ppu.palletes[3] = 0x30;
    get_pattern_table(&nes, 0, 0);
    get_pattern_table(&nes, 1, 0);
    // printf("%x\n", nes.ppu.px_pattern_table[0][1][16]);
    while(1){
        
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) break;

        system_clock(&nes);


        get_pattern_table(&nes, 0, 0);
        get_pattern_table(&nes, 1, 0);
        
        draw_element(screen, 400, 400, 200, 100, test);
        draw_element(screen, 0, 0, 128, 128, nes.ppu.px_pattern_table[0]);
        draw_element(screen, 128, 0, 128, 128, nes.ppu.px_pattern_table[1]);
        SDL_UpdateWindowSurface( window );
        // printf("%x\n", *(uint32_t *)screen->pixels);
    }
    return 0;
}