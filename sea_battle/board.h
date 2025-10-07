#ifndef BOARD_H
#define BOARD_H

#define GRID_SIZE 10
#define MAX_SHIPS 10
#define EDGE 21
#define BETWEEN_GRIDS 228

typedef struct GraphicsContext GraphicsContext;// Forward Declaration, подобно прототипам у функций

typedef struct { 
    char cell_size;      // ✅ копия из GraphicsContext
    int field_size;      // ✅ копия из GraphicsContext  
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

typedef struct {
    char cells[GRID_SIZE][GRID_SIZE]; 
    Ship ships[MAX_SHIPS]; // 10 кораблей всего
    char ship_count; // текущее количество расставленных
} GameBoard;

typedef struct {
    char x, y; // это ориентация по клеткам, поэтому char
    char direction; 
    char deck_count;
    char hits;
} Ship;



void init_board(GameBoard* board);
GameLandmarks calculate_landmarks(const GraphicsContext* ctx);
#endif