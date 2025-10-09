#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "graphics.h"
#include "board.h"

void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks) { //зарефакторить её нужно будет скоро
    GameState game = {0};
    game.running = GAME_RUNNING;
    GameBoard player_board_obj, computer_board_obj;  // статические объекты
    
    game.player_board = &player_board_obj;    // указатель на статический объект
    game.computer_board = &computer_board_obj;
    init_board(game.player_board);    // обнуляем их
    init_board(game.computer_board);
    
    game.player_board->ships[0] = (Ship){2, 5, 0, 1, 0}; // вынесу попозже
    game.player_board->ships[1] = (Ship){0, 0, 0, 2, 0};
    game.player_board->ships[2] = (Ship){4, 7, 1, 2, 0};
    game.player_board->ships[3] = (Ship){6, 3, 0, 4, 0};
    game.player_board->ships[4] = (Ship){6, 5, 0, 4, 0};
    game.player_board->ships[5] = (Ship){6, 2, 0, 4, 0};
    game.player_board->ships[6] = (Ship){6, 6, 0, 4, 0};
    game.player_board->ships[7] = (Ship){6, 7, 0, 4, 0};
    game.player_board->ship_count = 8;
    
    game.computer_board->ships[0] = (Ship){0, 3, 0, 4, 0};
    game.computer_board->ships[1] = (Ship){0, 2, 0, 4, 0};
    game.computer_board->ships[2] = (Ship){0, 6, 0, 4, 0};
    game.computer_board->ships[3] = (Ship){0, 7, 0, 4, 0};
    game.computer_board->ship_count = 4;
    
    init_cannon(&game.player_cannon, IS_PLAYER, landmarks->player_x + 
        ctx->field_size + OFFSET_X_FROM_BOARD, landmarks->offset_y + 
        OFFSET_Y_FROM_BOARD, ctx->renderer); 
    init_cannon(&game.computer_cannon, IS_COMPUTER, landmarks->player_x + 
        ctx->field_size + OFFSET_X_FROM_BOARD, landmarks->offset_y + 
        OFFSET_Y_FROM_COMPUTER_CANON, ctx->renderer);

    SDL_Event event;
        while (game.running) {
        // Обработка событий
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    game.running = DONT_RUNNING;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN && 
                    event.button.button == SDL_BUTTON_LEFT) {
                    int mouse_x = event.button.x;
                    int mouse_y = event.button.y;
        
                    // Проверяем клик по полю компьютера
                    if (mouse_x >= landmarks->computer_x && 
                        mouse_x <= landmarks->computer_x + ctx->field_size &&
                        mouse_y >= landmarks->offset_y && 
                        mouse_y <= landmarks->offset_y + ctx->field_size) {
            
                        // Немедленный поворот ствола
                        aim_cannon_at(&game.player_cannon, mouse_x, mouse_y);
                    }
                }
            }
        
        // Отрисовка 
            clear_screen(ctx);  // это имя указателя
            draw_board(ctx, landmarks->player_x, landmarks->offset_y, 
                game.player_board, SHOW_SHIPS); 
            draw_board(ctx, landmarks->computer_x, landmarks->offset_y, 
                game.computer_board, SHOW_SHIPS);
            draw_island(ctx, landmarks->player_x + ctx->field_size + 
                OFFSET_X_FROM_BOARD, 
                landmarks->offset_y + OFFSET_Y_FROM_BOARD + 
                ISLAND_BELOW_PLAYER_CANON, IS_PLAYER);
            draw_island(ctx, landmarks->player_x + ctx->field_size + 
                OFFSET_X_FROM_BOARD, 
                landmarks->offset_y + OFFSET_Y_FROM_COMPUTER_CANON + 
                ISLAND_BELOW_PLAYER_CANON, IS_COMPUTER);
            draw_cannon(ctx, &game.player_cannon);
            draw_cannon(ctx, &game.computer_cannon);
    
            present_screen(ctx);
        
            SDL_Delay(33); // ~30 FPS для пошаговой игры
    }
}


void init_cannon(Cannon* cannon, char is_player, int base_x, 
    int base_y, SDL_Renderer* renderer) {
    cannon->base_x = base_x;
    cannon->base_y = base_y;
    cannon->current_angle = 0;
    cannon->target_angle = 0;
    cannon->is_animating = CANNON_IDLE;
    
    if (is_player) {
        cannon->canon_platform_texture = load_texture_from_file(renderer, "../images/player_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/player_canon_barrel.png");
        cannon->barrel_pivot_x = PLAYER_CANNON_PIVOT_X;
        cannon->barrel_pivot_y = PLAYER_CANNON_PIVOT_Y;
    } else {
        cannon->canon_platform_texture = load_texture_from_file(renderer, "../images/computer_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/computer_canon_barrel.png");
        cannon->barrel_pivot_x = COMPUTER_CANNON_PIVOT_X;
        cannon->barrel_pivot_y = COMPUTER_CANNON_PIVOT_Y;
    }
}

void aim_cannon_at(Cannon* cannon, int target_x, int target_y) {
    // Координаты точки вращения ствола
    int pivot_x = cannon->base_x + cannon->barrel_pivot_x;
    int pivot_y = cannon->base_y + cannon->barrel_pivot_y;
    
    // Вектор от точки вращения к цели
    int dx = target_x - pivot_x;
    int dy = target_y - pivot_y;
    printf("► Клик: (%d, %d)\n", target_x, target_y);
    printf("► Точка вращения: (%d, %d)\n", pivot_x, pivot_y);
    printf("► Вектор: dx=%d, dy=%d\n", dx, dy);
    // Вычисляем угол в градусах
    double angle_rad = atan2(dy, dx);
    double angle_deg = angle_rad * (180.0 / M_PI);
    printf("► Угол: %.2f°\n", angle_deg);
    printf("► БЫЛ угол: %.2f°\n", cannon->current_angle);
    // Немедленно поворачиваем
    cannon->current_angle = angle_deg;
    printf("► СТАЛ угол: %.2f°\n", cannon->current_angle);
    printf("---\n");
}