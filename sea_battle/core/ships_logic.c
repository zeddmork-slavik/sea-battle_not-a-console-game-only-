#include <SDL2/SDL.h>
#include "ships_logic.h"
#include "audio.h" 
#include "game_state.h"

void process_shot_result(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks, Uint32 current_time, GameAudio* audio){
    
    if(game->computer_board->cells[game->cannonball.target_cell_x][game->cannonball.target_cell_y] == 0){
        game->show_spray = 1;
        game->spray_end_time = current_time + DURATION_OF_SPLASHES; 
        game->spray_x = (game->current_turn == IS_PLAYER ? landmarks->computer_x: landmarks->player_x) + (int) game->cannonball.target_cell_x * ctx->cell_size;      
        game->spray_y = landmarks->offset_y + (int) game->cannonball.target_cell_y * ctx->cell_size - Y_CRUTCH_FOR_SPLASHES;
        game->spray_alpha = STARTING_TRANSPARENCY;
        play_water_splash(audio);
    }
    
    if(game->computer_board->cells[game->cannonball.target_cell_x][game->cannonball.target_cell_y] == 1){
        game->computer_board->cells[game->cannonball.target_cell_x][game->cannonball.target_cell_y] = 2;
        add_fire_to_stack(game, game->cannonball.target_cell_x, game->cannonball.target_cell_y, current_time);   
    }
    /*
    → check_hit_or_miss()
    → update_cell_state() 
    → check_ship_sunk()
    → update_sunk_counter()

*/
}
