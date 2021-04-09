#ifndef _RENDER_H_
#define _RENDER_H_

typedef struct layout_element{
    
}layout_element;


typedef struct window_layout{
    layout_element *elem;
    int n_elem;
} window_layout;

#include <stdint.h>
#include <SDL2/SDL.h>

void draw_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void draw_element(SDL_Surface *surface, int offset_x, int offset_y, int width, int height, uint32_t elem[height][width]);

#endif