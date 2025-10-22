#include <SDL2/SDL.h>
#include "rendering.h"
#include "core/game_state.h"
#include "core/combat/cannons_logic.h"
#include "core/audio.h"
#include "graphics_state.h"
#include "boards.h"
#include "cannons.h" 
#include "effects.h"

void compose_frame(GameState* game, double delta_time, Uint32 current_time, const GraphicsContext* ctx, const GameLandmarks* landmarks, GameAudio* audio){
    update_cannon_animation(&game->player_cannon, delta_time);
    update_cannonball(ctx, game, landmarks, current_time, audio);
    if (!game->player_cannon.is_animating && 
        game->player_cannon.animation_end_time != 0) {
        if (current_time - game->player_cannon.animation_end_time >= 
            game->player_cannon.fire_delay) {
            game->player_cannon.is_firing = 1;
            play_cannon_shot(audio); 
            game->player_cannon.animation_end_time = 0;
            fire_cannonball(&game->cannonball, &game->player_cannon, current_time);
        }
    }
    if (game->player_cannon.is_firing) {
        if (game->player_cannon.current_alpha > 0) {
            game->player_cannon.current_alpha -= SPEED_TRANSPARENCY_BY_FRAME;
        } else {
            game->player_cannon.is_firing = 0;
            game->player_cannon.current_alpha = STARTING_TRANSPARENCY;  
        }
    }
    clear_screen(ctx); 
    draw_board(ctx, landmarks->player_x, landmarks->offset_y, 
            game->player_board, SHOW_SHIPS); 
    draw_board(ctx, landmarks->computer_x, landmarks->offset_y, 
            game->computer_board, SHOW_SHIPS);
    draw_island(ctx, landmarks->player_x + ctx->field_size + 
            OFFSET_X_FROM_BOARD, landmarks->offset_y + OFFSET_Y_FROM_BOARD + 
            ISLAND_BELOW_PLAYER_CANON, IS_PLAYER);
    draw_island(ctx, landmarks->player_x + ctx->field_size + 
            OFFSET_X_FROM_BOARD + X_CRUTCH_COMPUTER_ISLAND, 
            landmarks->offset_y + OFFSET_Y_FROM_COMPUTER_CANON + 
            ISLAND_BELOW_PLAYER_CANON + Y_CRUTCH_COMPUTER_ISLAND, IS_COMPUTER);
    if (game->show_spray) {
        if(current_time < game->spray_end_time) {
            draw_spray(ctx, game);
            game->spray_alpha -= SPEED_TRANSPARENCY_BY_FRAME;
        } else {
        game->show_spray = 0;
        game->spray_end_time = 0;
        game->spray_x = 0;      
        game->spray_y = 0;
        }   
    }  
    if (game->current_turn){ // чтобы пушка ходящего не загораживалась чужим островом
        draw_cannon(ctx, &game->computer_cannon, IS_COMPUTER, &game->cannonball);
        draw_cannon(ctx, &game->player_cannon, IS_PLAYER, &game->cannonball);
    }
    else {draw_cannon(ctx, &game->player_cannon, IS_PLAYER, &game->cannonball);
        draw_cannon(ctx, &game->computer_cannon, IS_COMPUTER, &game->cannonball);
    }
     
    present_screen(ctx);
}