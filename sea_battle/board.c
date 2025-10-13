#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "graphics.h"

void init_board(GameBoard* board){
    auto_arrange_ships(board);

}

/*void reset_board(GameBoard* board) {
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
*/
GameLandmarks calculate_landmarks(const GraphicsContext* ctx){
    
    int total_width = 2 * ctx-> field_size + BETWEEN_GRIDS + 2 * EDGE;
    int start_x = (ctx-> width_of_window - total_width) >> 1;  //просто добавляет лишние пиксили к EDGE  
    
    GameLandmarks landmarks;
    landmarks.cell_size = ctx-> cell_size;     
    landmarks.field_size = ctx-> field_size;        
    landmarks.player_x = start_x + EDGE; 
    landmarks.computer_x = landmarks.player_x + ctx->field_size + BETWEEN_GRIDS;
    landmarks.offset_y = ctx->height_of_window - ctx->field_size - EDGE; // 179
    
    return landmarks;
}

void auto_arrange_ships(GameBoard* board){
    srand(time(NULL));
    while (board->ship_count < MAX_SHIPS)
    {
        char x = (char) rand() % GRID_SIZE;
        char y = (char) rand() % GRID_SIZE;
        
        if(can_place_ship(x, y, 4)){board->ship_count++; 
                place_ship(пока нету ); // внесение изменений в CellState - не факт что понадобится - сможем это делать и в предудущей
        }
        
    }
    
    

}   

 char can_place_ship(GameBoard* board, char x, char y, char deck_count){
    char flag = 1, count_succes_decks = 0;
    if(board->cells[x][y] == CELL_EMPTY)
    
    
    return flag;
}