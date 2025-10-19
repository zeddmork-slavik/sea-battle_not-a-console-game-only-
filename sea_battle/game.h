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
#define SPEED_TRANSPARENCY_BY_FRAME 5
#define BARREL_OR_CORE_ROTATION_SPEED_SECOND_PER_FRAME 0.033
#define ANGLE_BETWEEN_DIRECTION_PLAYER_BARREL_AND_ITS_TEXTURE -7.0
#define PLAYER_BARREL_LENGTH 82
#define SPEED_CONNONBALL 220.0f

typedef struct GameBoard GameBoard;
typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct Mix_Chunk Mix_Chunk;
typedef struct Mix_Music Mix_Music;

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

typedef struct Cannonball{
    char is_active;           // летит ли ядро
    SDL_Texture* texture;
    int start_x, start_y;     // точка вылета (срез ствола)
    int target_x, target_y;   // центр целевой клетки
    char target_cell_x, target_cell_y;  
    int current_x, current_y; // текущая позиция
    float progress;           // 0.0 → 1.0 (пройденный путь) ради параболического полёта - первую половину подымаемся вторую опускаемся
    Uint32 start_time;        // время запуска
    Uint32 flight_duration;   // ???? зачем ? длительность полёта (ms)
    double rotation_angle;    // текущий угол при вращении
    double rotation_speed;    // скорость вращения
    float parabola_height;  // 🆕 высота параболы
} Cannonball;

typedef struct GameAudio{
    Mix_Chunk* cannon_shot;    // выстрел пушки
    /*Mix_Chunk* water_splash;   // промах 
    Mix_Chunk* ship_hit;       // попадание в корабль*/
    Mix_Chunk* victory;        // победа
    //Mix_Music* background;     // фоновая музыка
} GameAudio;

typedef struct {
    char running;
    GameBoard* player_board;
    GameBoard* computer_board;
    Cannon player_cannon;
    Cannon computer_cannon;
    Cannonball cannonball;
    char current_turn;  // 🆕 PLAYER_TURN или COMPUTER_TURN
    GameAudio audio; // позже добавим состояние игры (расстановка, ход игрока и т.д.)
} GameState;



void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks);
void event_processing(GameState* game, const GraphicsContext* ctx, 
    const GameLandmarks* landmarks, GameAudio* audio);
void compose_frame(GameState* game, double delta_time, Uint32 current_time, 
    const GraphicsContext* ctx, const GameLandmarks* landmarks, GameAudio* audio);
void init_cannon(Cannon* cannon, char is_player, int base_x, int base_y, 
    SDL_Renderer* renderer);
void aim_cannon_at(Cannon* cannon, int target_x, int target_y, 
    const GraphicsContext* ctx, const GameLandmarks* landmarks,
    Cannonball* cannonball);
void update_cannon_animation(Cannon* cannon, double delta_time);
void fire_cannonball(Cannonball* ball, const Cannon* cannon, Uint32 current_time);
void update_cannonball(GameState* game, Uint32 current_time);
void reset_cannonball(Cannonball* ball)
void process_shot_result(GameState* game);
GameAudio* load_audio();
void play_cannon_shot(GameAudio* audio);
void play_victory(GameAudio* audio);
void cleanup_audio(GameAudio* audio);
#endif