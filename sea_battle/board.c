#include "board.h"

void init_board(GameBoard* board) {
    // Обнуляем все клетки поля
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            board->cells[i][j] = 0; // 0 = EMPTY
        }
    }
    
    // Обнуляем массив кораблей
    for (int i = 0; i < MAX_SHIPS; i++) {
        board->ships[i] = (Ship){0, 0, 0, 0, 0};
    }
    
    board->ship_count = 0;
}