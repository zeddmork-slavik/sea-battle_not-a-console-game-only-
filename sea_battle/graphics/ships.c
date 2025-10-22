#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "core/game_state.h" 
#include "ships.h"

void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, 
               char direction, char deck_count, SDL_Texture* texture) {
    
    double angle = (direction == 1) ? 90.0 : 0.0;
    int ship_width, ship_height;
    SDL_QueryTexture(texture, NULL, NULL, &ship_width, &ship_height);
    
    SDL_Rect place_for_ship;
    
    if (direction == 0) { // горизонтальный
        place_for_ship = (SDL_Rect){
            .x = base_x + (int)grid_x * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) >> 1),
            .y = base_y + (int)grid_y * ctx->cell_size - ((ship_height - ctx->cell_size) >> 1),
            .w = ship_width,
            .h = ship_height
        };
        
    } else { // вертикальный
        place_for_ship = (SDL_Rect){
            .x = base_x + X_CRUTCH_VERTICAL_SHIPS + (int)grid_x * ctx->cell_size - ((ship_height - ctx->cell_size) >> 1), 
            .y = base_y + (int)grid_y * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) >> 1),
            .w = ship_width,  
            .h = ship_height    
        };
    }
    
    // Ручная точка вращения - левый верхний угол
    SDL_Point pivot = {0, 0};
    SDL_RenderCopyEx(ctx->renderer, texture, NULL, &place_for_ship, angle, &pivot, SDL_FLIP_NONE);
}