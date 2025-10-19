#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "graphics.h"
#include "board.h"

void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks) { //зарефакторить её нужно будет скоро
    GameState game = {0};  // не трогать: убираю в инициализации поля в которых нужны нули
    game.running = GAME_RUNNING;
    game.current_turn = IS_PLAYER;
    GameBoard player_board_obj = {0};
    GameBoard computer_board_obj = {0};
    game.player_board = &player_board_obj;    // указатель на статический объект
    game.computer_board = &computer_board_obj;
    init_board(game.player_board);    // заполняем их
    init_board(game.computer_board);
    
    init_cannon(&game.player_cannon, IS_PLAYER, landmarks->player_x + 
        ctx->field_size + OFFSET_X_FROM_BOARD, landmarks->offset_y + 
        OFFSET_Y_FROM_BOARD, ctx->renderer); 
    init_cannon(&game.computer_cannon, IS_COMPUTER, landmarks->player_x + 
        ctx->field_size + OFFSET_X_FROM_BOARD, landmarks->offset_y + 
        OFFSET_Y_FROM_COMPUTER_CANON, ctx->renderer);
     
    game.cannonball.texture = load_texture_from_file(ctx->renderer, "../images/cannonball.png");
    game.cannonball.rotation_speed = 720.0;    // 720°/секунду = 2 оборота в секунду
    
    char audio_initialized = 0;
    GameAudio* audio = NULL;
    
    // Инициализация аудиосистемы
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == 0) {
        audio = load_audio();
        if (audio) {
            audio_initialized = 1;
        }
    }
    
    if (!audio_initialized) {
        printf("Audio system unavailable - continuing without sound\n");
    }
    
    event_processing(&game, ctx, landmarks, audio);
    
    if (audio_initialized) {
        cleanup_audio(audio);
        Mix_CloseAudio();
    }
}

void event_processing(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks, GameAudio* audio){
    
    double delta_time = BARREL_OR_CORE_ROTATION_SPEED_SECOND_PER_FRAME;
    SDL_Event event;
        while (game->running) {
            Uint32 current_time = SDL_GetTicks(); // текущее время каждого кадра
            // Обработка событий
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    game->running = DONT_RUNNING;
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
                                    aim_cannon_at(&game->player_cannon, mouse_x, mouse_y, ctx,
                                        landmarks, &game->cannonball);
                    }
                }
            }
         
            compose_frame(game, delta_time, current_time, ctx, landmarks, audio);
        
            SDL_Delay(33); // ~30 FPS для пошаговой игры
        }
}

void compose_frame(GameState* game, double delta_time, Uint32 current_time, const GraphicsContext* ctx, const GameLandmarks* landmarks, GameAudio* audio){
    update_cannon_animation(&game->player_cannon, delta_time);
    update_cannonball(&game, current_time);
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
    clear_screen(ctx);  // это имя указателя
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
    if (game->current_turn){ // чтобы пушка ходящего не загораживалась чужим островом
        draw_cannon(ctx, &game->computer_cannon, IS_COMPUTER, &game->cannonball);
        draw_cannon(ctx, &game->player_cannon, IS_PLAYER, &game->cannonball);
    }
    else {draw_cannon(ctx, &game->player_cannon, IS_PLAYER, &game->cannonball);
        draw_cannon(ctx, &game->computer_cannon, IS_COMPUTER, &game->cannonball);
    }   
            
    present_screen(ctx);
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


    cannonball->target_cell_x = (target_x - landmarks->computer_x) / ctx->cell_size;
    cannonball->target_cell_y = (target_y - landmarks->offset_y) / ctx->cell_size;
    

    cannonball->target_x = landmarks->computer_x + cannonball->target_cell_x * ctx->cell_size + (ctx->cell_size >> 1);
    cannonball->target_y = landmarks->offset_y + cannonball->target_cell_y * ctx->cell_size + (ctx->cell_size >> 1);
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


void fire_cannonball(Cannonball* ball, const Cannon* cannon, Uint32 current_time) {
    ball->is_active = 1;
    ball->start_time = current_time; 
    // 🎯 Вычисляем точку вылета с учётом угла ствола
    double total_angle_rad = (cannon->current_angle + ANGLE_BETWEEN_DIRECTION_PLAYER_BARREL_AND_ITS_TEXTURE) * (M_PI / 180.0);
    
    ball->start_x = cannon->base_x + cannon->barrel_pivot_x + 
                   (int)(PLAYER_BARREL_LENGTH * cos(total_angle_rad));
    ball->start_y = cannon->base_y + cannon->barrel_pivot_y + 
                   (int)(PLAYER_BARREL_LENGTH * sin(total_angle_rad));
    
    // Текущая позиция = точке вылета
    ball->current_x = ball->start_x;
    ball->current_y = ball->start_y;
    
    int dx = ball->target_x - ball->start_x;
    int dy = ball->target_y - ball->start_y;
    float distance = sqrtf(dx*dx + dy*dy);
    
    ball->flight_duration = (Uint32)(distance / SPEED_CONNONBALL * 1000.0f); 
    
    // 🎯 Высота параболы зависит от расстояния
    ball->parabola_height = distance * 0.15f; // 15% от расстояния (регулируй)
}

void update_cannonball(GameState* game, Uint32 current_time) {
    // Выходим если ядро не активно
    if (game->cannonball.is_active) {
        float elapsed = (current_time - game->cannonball.start_time) / (float)game->cannonball.flight_duration;
        game->cannonball.progress = elapsed;

        if (elapsed < 1.0f) {
            // Ядро ещё летит
            game->cannonball.rotation_angle += game->cannonball.rotation_speed * BARREL_OR_CORE_ROTATION_SPEED_SECOND_PER_FRAME;
            
            // Баллистическая траектория
            game->cannonball.current_x = game->cannonball.start_x + (game->cannonball.target_x - game->cannonball.start_x) * game->cannonball.progress;
            float parabola = 4.0f * game->cannonball.progress * (1.0f - game->cannonball.progress);
            // 🎯 Используем динамическую высоту параболы:
            game->cannonball.current_y = game->cannonball.start_y + (game->cannonball.target_y - game->cannonball.start_y) * game->cannonball.progress - parabola * game->cannonball.parabola_height;
        } else {
            process_shot_result(game);
            reset_cannonball(&game->cannonball); // вместо с выключением is_active обнулим всю структуру
        }
    }
}

void reset_cannonball(Cannonball* ball){
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

void process_shot_result(GameState* game){
    
    if(game->computer_board->cells[game->cannonball.target_cell_x][game->cannonball.target_cell_y] == 1){
        game->computer_board->cells[game->cannonball.target_cell_x][game->cannonball.target_cell_y] = 2;
    }
    else if()
    → check_hit_or_miss()
    → update_cell_state() 
    → check_ship_sunk()
    → update_sunk_counter()


}

GameAudio* load_audio() {
    GameAudio* audio = malloc(sizeof(GameAudio));
    if (!audio) return NULL;
    
    audio->cannon_shot = Mix_LoadWAV("../sounds/cannon_shot.mp3");
    if (!audio->cannon_shot) {
        printf("Failed to load cannon_shot.mp3: %s\n", Mix_GetError());
    }
    audio->victory = Mix_LoadWAV("../sounds/victory.mp3");
    if (!audio->victory) {
        printf("Failed to load victory.mp3: %s\n", Mix_GetError());
    }
    /* Остальные звуки пока заглушки
    audio->water_splash = NULL;
    audio->ship_hit = NULL;
    audio->background = NULL;*/
    
    return audio;
}

void play_cannon_shot(GameAudio* audio) {
    if (audio && audio->cannon_shot) {
        Mix_PlayChannel(-1, audio->cannon_shot, 0);// автовыбор потока, ,количество повторений
    }
}

void play_victory(GameAudio* audio) {// !!!!!!!!!!!!!!!!!!!!!!!!!  пока никуда не вставляли
    if (audio && audio->victory) {
        Mix_PlayChannel(-1, audio->victory, 0);
    }
}

void cleanup_audio(GameAudio* audio) {
    Mix_FreeChunk(audio->cannon_shot); 
    free(audio);
}