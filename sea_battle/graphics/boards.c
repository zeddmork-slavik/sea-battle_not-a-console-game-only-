#include <SDL2/SDL.h>
#include "boards.h"
#include "core/game_state.h"
#include "ships.h" 


void draw_board(const GraphicsContext* ctx, int base_x, int base_y, const GameBoard* board, char show_ships) { 
    
    draw_single_grid(ctx, base_x, base_y);
    
    if (show_ships) {
        for (char i = 0; i < board->ship_count; i++) {
            Ship ship = board->ships[i]; // 5 против 64 за копию
            SDL_Texture* texture;
    
        // Выбираем текстуру по количеству палуб
            switch(ship.deck_count) {
                case 1: texture = ctx->ship_jup_1p; break;
                case 2: texture = ctx->ship_jup_2p; break; 
                case 3: texture = ctx->ship_jup_3p; break;
                case 4: texture = ctx->ship_jup_4p; break;
                default: texture = NULL; break;
            }
    
            if (texture) {
            draw_ship(ctx, base_x, base_y, ship.x, ship.y, 
                         ship.direction, ship.deck_count, texture);
            }
        }
    }
}

void draw_single_grid(const GraphicsContext* ctx, int offset_x, int offset_y) {// offset - сдвиг
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255); // Белый цвет сетки
        
    // Рисуем вертикальные линии
    for (char x = 0; x <= GRID_SIZE; x++) {
        SDL_RenderDrawLine(
            ctx->renderer,
            offset_x + x * ctx->cell_size, offset_y,           
            offset_x + x * ctx->cell_size, offset_y + ctx->field_size 
        );
    }
    
    // Рисуем горизонтальные линии
    for (char y = 0; y <= GRID_SIZE; y++) {
        SDL_RenderDrawLine(
        ctx->renderer,
        offset_x, offset_y + y * ctx->cell_size,           
        offset_x + ctx->field_size, offset_y + y * ctx->cell_size 
        );
    }
}