#ifndef SHIPS_H
#define SHIPS_H

#include <SDL2/SDL.h>

typedef struct GraphicsContext GraphicsContext;
typedef struct GameState GameState;
typedef struct GameLandmarks GameLandmarks;

void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, char direction,
               char deck_count, SDL_Texture* texture);
void draw_all_fires(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks);

#endif