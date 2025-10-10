#include "board.h"
#include "graphics.h"

void init_board(GameBoard* board) {
    // Обнуляем все клетки поля
    for (char i = 0; i < GRID_SIZE; i++) {
        for (char j = 0; j < GRID_SIZE; j++) {
            board->cells[i][j] = CELL_EMPTY; 
        }
    }
    
    // Обнуляем массив кораблей
    for (char i = 0; i < MAX_SHIPS; i++) {
        board->ships[i] = (Ship){0, 0, 0, 0, 0};
    }
    
    board->ship_count = 0;
}

GameLandmarks calculate_landmarks(const GraphicsContext* ctx){
    
    int total_width = 2 * ctx-> field_size + BETWEEN_GRIDS + 2 * EDGE;
    int start_x = (ctx-> width_of_window - total_width) / 2;  //просто добавляет лишние пиксили к EDGE  
    
    GameLandmarks landmarks;
    landmarks.cell_size = ctx-> cell_size;     
    landmarks.field_size = ctx-> field_size;        
    landmarks.player_x = start_x + EDGE; 
    landmarks.computer_x = landmarks.player_x + ctx->field_size + BETWEEN_GRIDS;
    landmarks.offset_y = ctx->height_of_window - ctx->field_size - EDGE; // 179
    
    return landmarks;
}