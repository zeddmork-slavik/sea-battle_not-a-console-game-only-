#include "game_state.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "arrangement/auto.h"
#include "audio.h"
#include "graphics/graphics_state.h"

void init_board(GameBoard* board) { auto_arrange_ships(board); }

GameLandmarks calculate_landmarks(const GraphicsContext* ctx) {
    int total_width = 2 * ctx->field_size + BETWEEN_GRIDS + 2 * EDGE;
    int start_x = (ctx->width_of_window - total_width) >> 1;  // просто добавляет лишние пиксили к EDGE

    GameLandmarks landmarks;
    landmarks.player_x = start_x + EDGE;
    landmarks.computer_x = landmarks.player_x + ctx->field_size + BETWEEN_GRIDS;
    landmarks.offset_y = ctx->height_of_window - ctx->field_size - EDGE;  // 179

    return landmarks;
}

void init_cannon(Cannon* cannon, char is_player, int base_x, int base_y, SDL_Renderer* renderer) {
    cannon->base_x = base_x;
    cannon->base_y = base_y;
    cannon->fire_delay = DELAY_FIRE_CANON;
    cannon->current_alpha = STARTING_TRANSPARENCY;
    cannon->rotation_speed = 45.0;  // градусов в секунду (1.5° за кадр при 30 FPS)

    if (is_player) {
        cannon->canon_platform_texture =
            load_texture_from_file(renderer, "../images/player_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/player_canon_barrel.png");
        cannon->canon_fire_texture = load_texture_from_file(renderer, "../images/player_canon_fire.png");
        cannon->barrel_pivot_x = PLAYER_CANNON_PIVOT_X;
        cannon->barrel_pivot_y = PLAYER_CANNON_PIVOT_Y;
    } else {
        cannon->canon_platform_texture =
            load_texture_from_file(renderer, "../images/computer_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/computer_canon_barrel.png");
        cannon->barrel_pivot_x = COMPUTER_CANNON_PIVOT_X;
        cannon->barrel_pivot_y = COMPUTER_CANNON_PIVOT_Y;
    }
}

void reset_cannonball(Cannonball* ball) {
    ball->is_active = 0;
    ball->start_x = 0;
    ball->start_y = 0;
    ball->target_x = 0;
    ball->target_y = 0;
    ball->target_cell_x = 0;
    ball->target_cell_y = 0;
    ball->current_x = 0;
    ball->current_y = 0;
    ball->progress = 0;
    ball->start_time = 0;
    ball->flight_duration = 0;
    ball->rotation_angle = 0;
    ball->parabola_height = 0;
}

void add_fire_to_stack(GameState* game, int cell_x, int cell_y, Uint32 current_time) {
    FireNode* new_fire = malloc(sizeof(FireNode));
    new_fire->cell_x = cell_x;
    new_fire->cell_y = cell_y;
    new_fire->current_frame = 1;
    new_fire->last_frame_time = current_time;
    new_fire->next = game->active_fires;  // записываем старую вершину
    game->active_fires = new_fire;
}

void cleanup_fires(GameState* game) {
    FireNode* current = game->active_fires;
    while (current != NULL) {
        FireNode* next = current->next;
        free(current);  // ⭐ ОСВОБОЖДАЕМ КАЖДЫЙ ОГОНЬ
        current = next;
    }
    game->active_fires = NULL;
}

void cleanup_game(GameState* game) {
    // Очищаем ВСЕ динамические ресурсы:
    cleanup_audio(game->audio);
    cleanup_fires(game);  // 🆕 очистка списка огней  !!!!!!!!!!!!!!!!!!! no realize !!!!!!!!!!!!!
}

/*void reset_board(GameBoard* board) {
    // Обнуляем все клетки поля
    for (char i = 0; i < GRID_SIZE; i++) {
        for (char j = 0; j < GRID_SIZE; j++) {
            board->cells[i][j] = CELL_EMPTY;
        }
    }

    // Обнуляем массив кораблей
    for (char i = 0; i < MAX_SHIPS; i++) {
        board->ships[i] = (Ship){0, 0, 0, 0, 0};
    }

    board->ship_count = 0;
}
*/