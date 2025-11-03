#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>  // для uint32_t

#define DELAY_FIRE_CANON 500  // 0.5 секунды
#define PLAYER_CANNON_PIVOT_X 90
#define PLAYER_CANNON_PIVOT_Y 55
#define COMPUTER_CANNON_PIVOT_X 105
#define COMPUTER_CANNON_PIVOT_Y 43
#define BARREL_OR_CORE_ROTATION_SPEED_SECOND_PER_FRAME 0.033
#define ANGLE_BETWEEN_DIRECTION_PLAYER_BARREL_AND_ITS_TEXTURE -7.0
#define PLAYER_BARREL_LENGTH 82
#define SPEED_CONNONBALL 220.0f
#define DURATION_OF_SPLASHES 1000
#define Y_CRUTCH_FOR_SPLASHES 8
#define OFFSET_Y_FROM_COMPUTER_CANON 250
#define IDLE 0  // это флаги для Canon
#define ANIMATING 1
#define GAME_RUNNING 1
#define DONT_RUNNING 0
#define DONT_SHOW_SHIPS 0
#define GRID_SIZE 10  // нейронка пообещала что в интересах cells приведёт сам к (char)
#define MAX_SHIPS 10  // пока 4 четырёхпалубных
#define EDGE 21
#define BETWEEN_GRIDS 228
#define LAST 9         // про столб или строку при проверки валидности клетки
#define PENULTIMATE 8  // про столб или строку при проверки валидности клетки
#define LEFT 0         // эти 4 для проверки валидности направлений дальнейшего рассположения клеток у корабля
#define RIGHT 1
#define UP 2
#define DOWN 3
#define INVALID_DIRECTION 7  // для проверки валидности направления в расстановке
#define WIDTH 1270
#define HEIGHT 700
#define CELL_SIZE 50
#define GRID_SIZE 10
#define X_CRUTCH_VERTICAL_SHIPS 62
#define WIDTH_CANON_TEXTURE 176  // пока для ствола и опоры одинаковые, дублировать не буду
#define HEIGHT_CANON_TEXTURE 122
#define WIDTH_PLAYER_ISLAND_TEXTURE 167
#define HEIGHT_PLAYER_ISLAND_TEXTURE 173
#define WIDTH_COMPUTER_ISLAND_TEXTURE 187
#define HEIGHT_COMPUTER_ISLAND_TEXTURE 146
#define X_CRUTCH_COMPUTER_ISLAND -4
#define Y_CRUTCH_COMPUTER_ISLAND 15
#define WIDTH_PLAYER_FIRE_CANON_TEXTURE 165
#define HEIGHT_PLAYER_FIRE_CANON_TEXTURE 95
#define OFFSET_X_FROM_FIRE_OF_CANON 166
#define OFFSET_Y_FROM_FIRE_OF_CANON -3
#define PIVOT_X_FROM_FIRE_OF_CANON -76
#define PIVOT_Y_FROM_FIRE_OF_CANON 58
#define WIDTH_SPRAY_TEXTURE 50
#define HEIGHT_SPRAY_TEXTURE 66
#define STARTING_TRANSPARENCY 255      // совсем не прозрачная
#define SPEED_TRANSPARENCY_BY_FRAME 5  // этот для огня пушки и брызгов
#define SHOW_SHIPS 1                   // for draw_board()
#define OFFSET_X_FROM_BOARD \
    26  // под конкретную ширину - завязывать на переменные пока головняк. 26=20 корабли могут выступать из
        // полей и 6 это половина остатков ширины after картинки
#define OFFSET_Y_FROM_BOARD 70
#define ISLAND_BELOW_PLAYER_CANON 10
#define IS_PLAYER 1
#define IS_COMPUTER 0  // next 4 for init_cannon() on run_game()
#define TIME_EVERY_SHIPS_FIRE 170
#define STATE_MENU 0
#define STATE_PLAYING 1

typedef struct GameAudio GameAudio;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef uint32_t Uint32;

typedef struct GameLandmarks {
    int player_x;
    int computer_x;
    int offset_y;
} GameLandmarks;

typedef struct Ship {
    char x, y;  // это ориентация по клеткам
    char direction;
    char deck_count;
    char hits;
} Ship;

typedef struct GameBoard {
    char cells[GRID_SIZE][GRID_SIZE];
    Ship ships[MAX_SHIPS];  // 10 кораблей всего
    char ship_count;        // текущее количество расставленных
} GameBoard;

typedef enum { CELL_EMPTY = 0, CELL_SHIP = 1, CELL_FIRE = 2 } CellState;

typedef struct Cannon {                   // без имени компилятор ананимусом обзывается
    SDL_Texture* canon_platform_texture;  // неподвижная опора
    SDL_Texture* barrel_texture;          // вращающийся ствол
    SDL_Texture* canon_fire_texture;
    int base_x, base_y;                   // позиция опоры
    char barrel_pivot_x, barrel_pivot_y;  // точка вращения ствола (90,55)
    double current_angle;                 // текущий угол ствола
    double target_angle;                  // целевой угол
    char is_animating;                    // показываем анимацию
    char is_firing;
    Uint32 animation_end_time;    // когда ЗАКОНЧИЛАСЬ анимация поворота
    Uint32 fire_delay;            // задержка перед показом огня (500ms)
    unsigned char current_alpha;  // 🆕 текущая прозрачность (255 = непрозрачный)
    double rotation_speed;
} Cannon;

typedef struct Cannonball {
    char is_active;  // летит ли ядро
    SDL_Texture* texture;
    int start_x, start_y;    // точка вылета (срез ствола)
    int target_x, target_y;  // центр целевой клетки
    char target_cell_x, target_cell_y;
    int current_x, current_y;  // текущая позиция
    float progress;  // 0.0 → 1.0 (пройденный путь) ради параболического полёта - первую половину подымаемся
    Uint32 start_time;       // время запуска
    Uint32 flight_duration;  // ???? зачем ? длительность полёта (ms)
    double rotation_angle;   // текущий угол при вращении
    double rotation_speed;   // скорость вращения
    float parabola_height;   // 🆕 высота параболы
} Cannonball;

typedef struct FireNode {
    unsigned char cell_x, cell_y;  // координаты клетки
    unsigned char current_frame;   // текущий кадр анимации (0-7)
    Uint32 last_frame_time;        // время последней смены кадра
    struct FireNode* next;         // указатель на следующий огонь
} FireNode;

typedef struct GameState {
    char running;
    GameBoard* player_board;
    GameBoard* computer_board;
    Cannon player_cannon;
    Cannon computer_cannon;
    Cannonball cannonball;
    int current_turn;  // 🆕 PLAYER_TURN или COMPUTER_TURN
    char show_spray;
    Uint32 spray_end_time;
    int spray_x;
    int spray_y;
    unsigned char spray_alpha;
    GameAudio* audio;
    FireNode* active_fires;  // 🆕 стек активных огней
    char background_music_on;
    char all_sound_on;
    unsigned char game_state;  // 🆕 STATE_MENU или STATE_PLAYING
    unsigned char menu_selection;
} GameState;

typedef struct GraphicsContext {  // рендерер, главное окно и размер игровой клетки поля в пикселях
    SDL_Window* window;           // главное окно
    SDL_Renderer* renderer;       // рисовальщик
    int width_of_window;
    int height_of_window;
    unsigned char cell_size;  // размер игровой клетки поля в пикселях, каждая клетка игрового поля будет
    int field_size;
    SDL_Texture* ship_jup_1p;  // однопалубный
    SDL_Texture* ship_jup_2p;  // двухпалубный
    SDL_Texture* ship_jup_3p;
    SDL_Texture* ship_jup_4p;  // четырехпалубный
    SDL_Texture* player_island_texture;
    SDL_Texture* computer_island_texture;
    SDL_Texture* fire_ship1;
    SDL_Texture* fire_ship2;
    SDL_Texture* fire_ship3;
    SDL_Texture* fire_ship4;
    SDL_Texture* fire_ship5;
    SDL_Texture* fire_ship6;
    SDL_Texture* fire_ship7;
    SDL_Texture* fire_ship0;
    SDL_Texture* spray_texture;
    TTF_Font* menu_small_font;
    TTF_Font* menu_big_font;
    SDL_Texture* menu_bg;
    SDL_Texture* button_on;
    SDL_Texture* button_off;
} GraphicsContext;

int initialize_sdl_systems(void);
void init_board(GameBoard* board);
GameLandmarks calculate_landmarks(const GraphicsContext* ctx);
void init_cannon(Cannon* cannon, char is_player, int base_x, int base_y, SDL_Renderer* renderer);
void reset_cannonball(Cannonball* ball);
void add_fire_to_stack(GameState* game, int cell_x, int cell_y, Uint32 current_time);
void cleanup_fires(GameState* game);
void cleanup_game(GameState* game);
void shutdown_sdl_systems(void);
#endif