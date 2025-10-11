#ifndef GAME_H
#define GAME_H

#define IDLE 0 // это флаги для Canon
#define ANIMATING 1
#define GAME_RUNNING 1
#define DONT_RUNNING 0
#define SHOW_SHIPS 1 // for draw_board()
#define DONT_SHOW_SHIPS 0
#define IS_PLAYER 1 
#define IS_COMPUTER 0       // next 4 for init_cannon() on run_game()
#define OFFSET_X_FROM_BOARD 26 // под конкретную ширину - завязывать на переменные пока головняк. 26=20 корабли могут выступать из полей и 6 это половина остатков ширины after картинки
#define OFFSET_Y_FROM_BOARD 70
#define OFFSET_Y_FROM_COMPUTER_CANON 250
#define ISLAND_BELOW_PLAYER_CANON 10
#define PLAYER_CANNON_PIVOT_X 90
#define PLAYER_CANNON_PIVOT_Y 55
#define COMPUTER_CANNON_PIVOT_X 105
#define COMPUTER_CANNON_PIVOT_Y 43
#define DELAY_FIRE_CANON 500 // 0.5 секунды
#define STARTING_TRANSPARENCY 255 // совсем не прозрачная


typedef struct GameBoard GameBoard;
typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;

typedef struct Cannon{ // без имени компилятор ананимусом обзывается
    SDL_Texture* canon_platform_texture;    // неподвижная опора
    SDL_Texture* barrel_texture;  // вращающийся ствол
    SDL_Texture* canon_fire_texture;
    int base_x, base_y;           // позиция опоры
    char barrel_pivot_x, barrel_pivot_y; // точка вращения ствола (90,55)
    double current_angle;         // текущий угол ствола
    double target_angle;          // целевой угол
    char is_animating; // показываем анимацию
    char is_firing;   
    Uint32 animation_end_time; // когда ЗАКОНЧИЛАСЬ анимация поворота
    Uint32 fire_delay;        // задержка перед показом огня (500ms)       
    unsigned char current_alpha;       // 🆕 текущая прозрачность (255 = непрозрачный)
    double rotation_speed;
} Cannon;

typedef struct {
    char running;
    GameBoard* player_board;
    GameBoard* computer_board;
    Cannon player_cannon;
    Cannon computer_cannon;
    char current_turn;  // 🆕 PLAYER_TURN или COMPUTER_TURN
    // позже добавим состояние игры (расстановка, ход игрока и т.д.)
} GameState;



void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks);
void init_cannon(Cannon* cannon, char is_player, int base_x, int base_y, SDL_Renderer* renderer);
void aim_cannon_at(Cannon* cannon, int target_x, int target_y);
void update_cannon_animation(Cannon* cannon, double delta_time);

#endif