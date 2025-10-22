#ifndef SHIPS_H
#define SHIPS_H


typedef struct GraphicsContext GraphicsContext;

void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, char direction, 
               char deck_count, SDL_Texture* texture);

#endif