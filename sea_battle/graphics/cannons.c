#include <SDL2/SDL.h>
#include "cannons.h"
#include "core/game_state.h"

void draw_island(const GraphicsContext* ctx, int base_x, int base_y, char is_player){
    SDL_Rect island_rect = {
        .x = base_x,      
        .y = base_y,
        .w = is_player? WIDTH_PLAYER_ISLAND_TEXTURE: WIDTH_COMPUTER_ISLAND_TEXTURE,
        .h = is_player? HEIGHT_PLAYER_ISLAND_TEXTURE: HEIGHT_COMPUTER_ISLAND_TEXTURE,
    };

    SDL_RenderCopy(ctx->renderer, is_player? ctx->player_island_texture: ctx->computer_island_texture, NULL, &island_rect);
}

void draw_cannon(const GraphicsContext* ctx, const Cannon* cannon, char is_player, const Cannonball* cannonball) {
    // Опора
    SDL_Rect base_rect = {
        .x = cannon->base_x,      
        .y = cannon->base_y,
        .w = WIDTH_CANON_TEXTURE,
        .h = HEIGHT_CANON_TEXTURE
    };
    SDL_RenderCopy(ctx->renderer, cannon->canon_platform_texture, NULL, &base_rect);
    
    if (cannonball->is_active) {
        draw_cannonball(ctx, cannonball);
    }

    if (is_player && cannon->is_firing) {draw_fire_of_cannon(ctx, cannon, cannon->base_x + OFFSET_X_FROM_FIRE_OF_CANON, 
    cannon->base_y + OFFSET_Y_FROM_FIRE_OF_CANON);}   
    // Ствол
    SDL_Rect barrel_rect = {
        .x = cannon->base_x,
        .y = cannon->base_y,  
        .w = WIDTH_CANON_TEXTURE,
        .h = HEIGHT_CANON_TEXTURE
    };
    
    SDL_Point pivot = {
        .x = cannon->barrel_pivot_x,
        .y = cannon->barrel_pivot_y
    };

    SDL_RenderCopyEx(
        ctx->renderer, 
        cannon->barrel_texture, 
        NULL, 
        &barrel_rect, 
        cannon->current_angle,    
        &pivot, 
        SDL_FLIP_NONE
    );
}

void draw_fire_of_cannon(const GraphicsContext* ctx, const Cannon* cannon, int base_x, int base_y) {
    
    SDL_SetTextureAlphaMod(cannon->canon_fire_texture, cannon->current_alpha);

    SDL_Rect fire = {
        .x = base_x,
        .y = base_y,  
        .w = WIDTH_PLAYER_FIRE_CANON_TEXTURE,
        .h = HEIGHT_PLAYER_FIRE_CANON_TEXTURE
    };

    SDL_Point pivot = {
        .x = PIVOT_X_FROM_FIRE_OF_CANON,
        .y = PIVOT_Y_FROM_FIRE_OF_CANON
    };
    
    SDL_RenderCopyEx(
        ctx->renderer, 
        cannon->canon_fire_texture, 
        NULL, 
        &fire, 
        cannon->current_angle,    
        &pivot, 
        SDL_FLIP_NONE
    );
}

void draw_cannonball(const GraphicsContext* ctx, const Cannonball* core) {
    if (!core->is_active) return;
    
    SDL_Rect core_rect = {
        .x = core->current_x - 5, // центрируем (10/2)
        .y = core->current_y - 5, // центрируем (11/2) 
        .w = 10,
        .h = 11
    };
    
    SDL_Point pivot = {5, 5}; // центр вращения
    
    SDL_RenderCopyEx(
        ctx->renderer,
        core->texture, 
        NULL,
        &core_rect,
        core->rotation_angle,
        &pivot,
        SDL_FLIP_NONE
    );
}