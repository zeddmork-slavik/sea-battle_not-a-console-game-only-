#include <SDL2/SDL.h>
#include <math.h>
#include "cannons_logic.h"
#include "../game_state.h"
#include "../ships_logic.h"

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

void update_cannonball(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks, Uint32 current_time, GameAudio* audio) {
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
            process_shot_result(ctx, game, landmarks, current_time, audio);
            reset_cannonball(&game->cannonball); // вместо с выключением is_active обнулим всю структуру
        }
    }
}
