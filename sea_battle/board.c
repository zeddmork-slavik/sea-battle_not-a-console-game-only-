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
        
        if(can_place_first_deck(board, x, y, 4)){
            place_for_others_decks(board, x, y, 4); 
            // внесение изменений в CellState and board->ship_count++; 
        }   
    }
}   

 char can_place_first_deck(const GameBoard* board, char x, char y, char deck_count){
    char flag = 1;
    if(board->cells[x][y] != CELL_EMPTY) {break;} 
    if (x > 0 && x < GRID_SIZE - 1 && y == 0) { // top row except corners
        if (board->cells[x + 1][0] == CELL_EMPTY && board->cells[x - 1][0] == CELL_EMPTY 
            && board->cells[x][1] == CELL_EMPTY && board->cells[x - 1][1] == CELL_EMPTY 
            && board->cells[x + 1][1] == CELL_EMPTY) {flag == 0;}
    }
        
    if (x == 0 && y > 0 && y < GRID_SIZE - 1) { // left col except corners
        if (board->cells[0][y -1] == CELL_EMPTY && board->cells[0][y + 1] == CELL_EMPTY 
            && board->cells[1][y - 1] == CELL_EMPTY && board->cells[1][y] == CELL_EMPTY 
            && board->cells[1][y + 1] == CELL_EMPTY) {flag == 0;}
    }
    
    return flag;
}

void place_for_others_decks(const GameBoard* board, char x, char y, char deck_count){
    char valid_mask = 0;

    // 2. Включаем выключатели для валидных направлений
    if (can_go_left)  valid_mask |= (1 << LEFT);   // включили LEFT
    if (can_go_right) valid_mask |= (1 << RIGHT);  // включили RIGHT
    if (can_go_up)    valid_mask |= (1 << UP);     // включили UP
    if (can_go_down)  valid_mask |= (1 << DOWN);   // включили DOWN

    // 3. Проверяем конкретное направление
    if (valid_mask & (1 << LEFT)) {
    printf("Можно идти влево!\n");
    }

    // 4. Считаем сколько направлений доступно
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (valid_mask & (1 << i)) {
            count++;  // считаем включённые выключатели
        }
}

// 5. Выбираем случайное направление
int random_index = rand() % count;
int current = 0;
for (int i = 0; i < 4; i++) {
    if (valid_mask & (1 << i)) {
        if (current == random_index) {
            int direction = i;  // нашли направление!
            break;
        }
        current++;
    }
}


}