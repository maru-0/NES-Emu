#include <rendering.h>
#include <stdint.h>
#include <SDL2/SDL.h>

void draw_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) { 
            p[0] = (pixel >> 24) & 0xff;
            p[1] = (pixel >> 16) & 0xff;
            p[2] = (pixel >> 8) & 0xff;
            p[3] = pixel & 0xff;
        }else{
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
            p[3] = (pixel >> 24) & 0xff;
        }

        break;
    }  
}

void draw_element(SDL_Surface *surface, int offset_x, int offset_y, int width, int height, uint32_t elem[height][width]){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            draw_pixel(surface, offset_x + x, offset_y + y, elem[y][x]);
        }
    }
}