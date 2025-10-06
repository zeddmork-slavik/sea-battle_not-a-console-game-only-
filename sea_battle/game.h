#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "board.h"
#define EDGE 21
#define BETWEEN_GRIDS 228


typedef struct {
    int running;
    GameBoard player_board;
    GameBoard computer_board;
    Cannon player_cannon;
    Cannon computer_cannon;
    // позже добавим состояние игры (расстановка, ход игрока и т.д.)
} GameState;

typedef struct {
    SDL_Texture* canon_platform_texture;    // неподвижная опора
    SDL_Texture* barrel_texture;  // вращающийся ствол
    int base_x, base_y;           // позиция опоры
    int barrel_pivot_x, barrel_pivot_y; // точка вращения ствола (90,55)
    double current_angle;         // текущий угол ствола
    double target_angle;          // целевой угол
    int is_animating;             // флаг анимации
} Cannon;

void run_game(GraphicsContext ctx);
void init_cannon(Cannon* cannon, char is_player_cannon, int base_x, int base_y, SDL_Renderer* renderer);

#endif