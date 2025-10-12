#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "graphics.h"
#include "board.h"

void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks) { //зарефакторить её нужно будет скоро
    GameState game = {0};  // не трогать: убираю в инициализации поля в которых нужны нули
    game.running = GAME_RUNNING;
    game.current_turn = IS_PLAYER;
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
    
    double delta_time = BARREL_ROTATION_SPEED_SECOND_PER_FRAME; 
    game.cannonball.texture = load_texture_from_file(ctx->renderer, "../images/cannonball.png");
    game.cannonball.rotation_speed = 720.0;    // 720°/секунду = 2 оборота в секунду
    
    SDL_Event event;
        while (game.running) {
            Uint32 current_time = SDL_GetTicks(); // текущее время каждого кадра
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
            
                        aim_cannon_at(&game.player_cannon, mouse_x, mouse_y, ctx,
                            landmarks, &game.cannonball);
                    }
                }
            }
         
            update_cannon_animation(&game.player_cannon, delta_time);
        //    update_cannonball(&game.cannonball, current_time);
            if (!game.player_cannon.is_animating && 
            game.player_cannon.animation_end_time != 0) {
                if (current_time - game.player_cannon.animation_end_time >= 
                game.player_cannon.fire_delay) {
                    game.player_cannon.is_firing = 1;
                    game.player_cannon.animation_end_time = 0;
                    game.cannonball.is_active = 1;
                    game.cannonball.start_time = current_time; //? зачем вы?
                    fire_cannonball(&game.cannonball, &game.player_cannon, 
                        int target_x, int target_y)
                }
            }
            if (game.player_cannon.is_firing) {
                if (game.player_cannon.current_alpha > 0) {
                    game.player_cannon.current_alpha -= SPEED_TRANSPARENCY_BY_FRAME;
                } else {
                    game.player_cannon.is_firing = 0;
                    game.player_cannon.current_alpha = STARTING_TRANSPARENCY;  
                }
            }
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
                OFFSET_X_FROM_BOARD + X_CRUTCH_COMPUTER_ISLAND, 
                landmarks->offset_y + OFFSET_Y_FROM_COMPUTER_CANON + 
                ISLAND_BELOW_PLAYER_CANON + Y_CRUTCH_COMPUTER_ISLAND, IS_COMPUTER);
            if (game.current_turn){
                draw_cannon(ctx, &game.computer_cannon, IS_COMPUTER, game.cannonball.is_active);
                draw_cannon(ctx, &game.player_cannon, IS_PLAYER);
            }
            else {
                draw_cannon(ctx, &game.player_cannon, IS_PLAYER);
                draw_cannon(ctx, &game.computer_cannon, IS_COMPUTER);
            }   
            
    
            present_screen(ctx);
        
            SDL_Delay(33); // ~30 FPS для пошаговой игры
    }
}


void init_cannon(Cannon* cannon, char is_player, int base_x, 
    int base_y, SDL_Renderer* renderer) {
    cannon->base_x = base_x;
    cannon->base_y = base_y;
    cannon->fire_delay = DELAY_FIRE_CANON; 
    cannon->current_alpha = STARTING_TRANSPARENCY;
    cannon->rotation_speed = 45.0; // градусов в секунду (1.5° за кадр при 30 FPS)
    
    if (is_player) {
        cannon->canon_platform_texture = load_texture_from_file(renderer, "../images/player_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/player_canon_barrel.png");
        cannon->canon_fire_texture = load_texture_from_file(renderer, "../images/player_canon_fire.png");
        cannon->barrel_pivot_x = PLAYER_CANNON_PIVOT_X;
        cannon->barrel_pivot_y = PLAYER_CANNON_PIVOT_Y;
    } else {
        cannon->canon_platform_texture = load_texture_from_file(renderer, "../images/computer_canon_platform.png");
        cannon->barrel_texture = load_texture_from_file(renderer, "../images/computer_canon_barrel.png");
        cannon->barrel_pivot_x = COMPUTER_CANNON_PIVOT_X; // пока без огонька
        cannon->barrel_pivot_y = COMPUTER_CANNON_PIVOT_Y;
    }
}

void aim_cannon_at(Cannon* cannon, int target_x, int target_y, const GraphicsContext* ctx, const GameLandmarks* landmarks,
                  Cannonball* cannonball) {
    // Координаты точки вращения ствола
    int pivot_x = cannon->base_x + cannon->barrel_pivot_x;
    int pivot_y = cannon->base_y + cannon->barrel_pivot_y;
    
    // Вектор от точки вращения к цели
    int dx = target_x - pivot_x;
    int dy = target_y - pivot_y;

    // Вычисляем угол в градусах
    double angle_rad = atan2(dy, dx);  // повторяемся, такое же считаем в отрисовке но типа там графика а здесь логика, не создавать же отдельные поля в текстурах только ради этого
    double angle_deg = angle_rad * (180.0 / M_PI);

    cannon->target_angle = angle_deg;
    cannon->is_animating = ANIMATING;


    int cell_x = (mouse_x - landmarks->computer_x) / ctx->cell_size;
    int cell_y = (mouse_y - landmarks->offset_y) / ctx->cell_size;
    

    cannonball->target_x = landmarks->computer_x + cell_x * ctx->cell_size + ctx->cell_size/2;
    cannonball->target_y = landmarks->offset_y + cell_y * ctx->cell_size + ctx->cell_size/2;
}

void update_cannon_animation(Cannon* cannon, double delta_time) {
    if (!cannon->is_animating) return;
    
    double max_rotation = cannon->rotation_speed * delta_time;
    double angle_diff = cannon->target_angle - cannon->current_angle;
    
    // Нормализуем разницу углов (-180 до 180)
    if (angle_diff > 180) angle_diff -= 360;  // чтобы 
    if (angle_diff < -180) angle_diff += 360;
    
    // Плавное приближение
    if (fabs(angle_diff) <= max_rotation) {
        cannon->current_angle = cannon->target_angle;
        cannon->is_animating = IDLE;
        cannon->animation_end_time = SDL_GetTicks(); // ← запоминаем КОГДА закончили поворот
    // is_firing пока НЕ включаем!

    } 
    else {
        cannon->current_angle += (angle_diff > 0 ? max_rotation : -max_rotation);
    }
}


void fire_cannonball(Cannonball* ball, const Cannon* cannon) {
    
    // 🎯 Вычисляем точку вылета с учётом угла ствола
    double angle_rad = cannon->current_angle * (M_PI / 180.0);
    int barrel_length = 100; // эмпирическая длина ствола
    
    ball->start_x = cannon->base_x + cannon->barrel_pivot_x + 
                   (int)(barrel_length * cos(angle_rad));
    ball->start_y = cannon->base_y + cannon->barrel_pivot_y + 
                   (int)(barrel_length * sin(angle_rad));
    
    // Текущая позиция = точке вылета
    ball->current_x = ball->start_x;
    ball->current_y = ball->start_y;
    
    // 🎯 Длительность полёта = расстояние / скорость
    int dx = ball->target_x - ball->start_x;
    int dy = ball->target_y - ball->start_y;
    float distance = sqrtf(dx*dx + dy*dy);
    ball->flight_duration = (Uint32)(distance / 400.0f * 1000.0f); // 400 px/сек
}