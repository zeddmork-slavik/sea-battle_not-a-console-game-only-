#ifndef BOARD_H
#define BOARD_H

#define GRID_SIZE 10
#define MAX_SHIPS 10

typedef enum {
    CELL_EMPTY = 0,
    CELL_SHIP = 1,
    CELL_MISS = 2, 
    CELL_HIT = 3
} CellState;

typedef struct {
    int x, y;
    int direction; 
    int deck_count;
    int hits;
} Ship;

typedef struct {
    int cells[GRID_SIZE][GRID_SIZE]; 
    Ship ships[MAX_SHIPS]; // 10 кораблей всего
    int ship_count; // текущее количество расставленных
} GameBoard;

void init_board(GameBoard* board);

#endif