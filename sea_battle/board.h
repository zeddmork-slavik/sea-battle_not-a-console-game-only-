#ifndef BOARD_H
#define BOARD_H

#define GRID_SIZE 10  // нейронка пообещала что в интересах cells приведёт сам к (char)
#define MAX_SHIPS 10 // пока 4 четырёхпалубных
#define EDGE 21
#define BETWEEN_GRIDS 228
#define LAST 9 // про столб или строку при проверки валидности клетки
#define PENULTIMATE 8 // про столб или строку при проверки валидности клетки
#define LEFT  0 // эти 4 для проверки валидности направлений дальнейшего рассположения клеток у корабля
#define RIGHT 1  
#define UP    2
#define DOWN  3
#define INVALID_DIRECTION 7 // во какой скрюченный

typedef struct GraphicsContext GraphicsContext;// Forward Declaration, подобно прототипам у функций

typedef struct GameLandmarks{ 
    int player_x; 
    int computer_x; 
    int offset_y;  
} GameLandmarks; 

typedef enum {
    CELL_EMPTY = 0,
    CELL_SHIP = 1,
    CELL_MISS = 2, 
    CELL_HIT = 3
} CellState;

typedef struct Ship{
    char x, y; // это ориентация по клеткам, когда был знаковым происходила какая-нибудь дичь с псевдослучайками
    char direction; 
    char deck_count;
    char hits;
} Ship;

typedef struct GameBoard{
    char cells[GRID_SIZE][GRID_SIZE]; 
    Ship ships[MAX_SHIPS]; // 10 кораблей всего
    char ship_count; // текущее количество расставленных
} GameBoard;





void init_board(GameBoard* board);
GameLandmarks calculate_landmarks(const GraphicsContext* ctx);
void auto_arrange_ships(GameBoard* board);
void add_ship_to_gameBoard(GameBoard* board, char x, char y, const char ship_index, const char direction);
char place_for_others_decks(const GameBoard* board, char* x, char* y, const char deck_count, char* direction);
char select_random_direction(const char valid_mask); 

char check_place_for_first_deck(const GameBoard* board, const char x, const char y);
void check_corners_for_first_deck(const GameBoard* board, const char x, const char y, char* flag);


char can_go_left(const GameBoard* board, char x, char y);
char can_go_right(const GameBoard* board, char x, char y); 
char can_go_up(const GameBoard* board, char x, char y);
char can_go_down(const GameBoard* board, char x, char y);
#endif