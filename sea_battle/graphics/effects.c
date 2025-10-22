#include <SDL2/SDL.h>
#include "effects.h"
#include "core/game_state.h"

void draw_spray(const GraphicsContext* ctx, GameState* game){

    if (game->spray_alpha > 0) {
        SDL_SetTextureAlphaMod(ctx->spray_texture, game->spray_alpha);
    

        SDL_Rect spray = {
            .x = game->spray_x,      
            .y = game->spray_y,
            .w = WIDTH_SPRAY_TEXTURE,
            .h = HEIGHT_SPRAY_TEXTURE,
        };
    
        SDL_RenderCopy(ctx->renderer, ctx->spray_texture, NULL, &spray);
        SDL_SetTextureAlphaMod(ctx->spray_texture, 255);
    }
}