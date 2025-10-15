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
    int placed_ships = 0;
    int attempts = 0;
    int max_attempts = 10000;
    
    srand(time(NULL));
    while (placed_ships < 1 && attempts < max_attempts) {
        char x = (char) rand() % GRID_SIZE;
        char y = (char) rand() % GRID_SIZE;
        
        if(check_place_for_first_deck(board, x, y)){
            board->cells[x][y] = CELL_SHIP;
            board->ships[placed_ships] = (Ship){x, y, 0, 1, 0};
            placed_ships++;
            board->ship_count++;            
        }
        
        attempts++;
    }
}   

 char check_place_for_first_deck(const GameBoard* board, char x, char y){
    char flag = 1;
    if(board->cells[x][y] == CELL_EMPTY) { // решил налупить 8, 9, нулей и единиц чтоб не допустить лишних вычислений
        if (x > 0 && x < 9 && y == 0) { // top row except corners
            if (board->cells[x + 1][0] == CELL_EMPTY && board->cells[x - 1][0] == CELL_EMPTY 
                && board->cells[x][1] == CELL_EMPTY && board->cells[x - 1][1] == CELL_EMPTY 
                && board->cells[x + 1][1] == CELL_EMPTY) {flag = 0;}
        }
        
        if (x > 0 && x < 9 && y == 9) { // bottom row except corners
            if (board->cells[x + 1][9] == CELL_EMPTY && board->cells[x - 1][9] == CELL_EMPTY 
                && board->cells[x][8] == CELL_EMPTY && board->cells[x - 1][8] == CELL_EMPTY 
                && board->cells[x + 1][8] == CELL_EMPTY) {flag = 0;}
        }

        if (x == 0 && y > 0 && y < 9) { // left col except corners
            if (board->cells[0][y -1] == CELL_EMPTY && board->cells[0][y + 1] == CELL_EMPTY 
                && board->cells[1][y - 1] == CELL_EMPTY && board->cells[1][y] == CELL_EMPTY 
                && board->cells[1][y + 1] == CELL_EMPTY) {flag = 0;}
        }
    
        if (x == 9 && y > 0 && y < 9) { // right col except corners
            if (board->cells [9][y - 1] == CELL_EMPTY && board->cells [9][y + 1] == CELL_EMPTY 
                && board->cells [8][y - 1] == CELL_EMPTY && board->cells [8][y] == CELL_EMPTY 
                && board->cells [8][y + 1] == CELL_EMPTY) {flag = 0;}
        }    
    
        check_corners_for_first_deck(board, x, y, &flag);
    
        if (x > 0 && x < 9 && y > 0 && y < 9) { // without borders
            if (board->cells [x - 1][y - 1] == CELL_EMPTY && board->cells [x - 1][y] == CELL_EMPTY
                && board->cells [x - 1][y + 1] == CELL_EMPTY && board->cells [x][y - 1] == CELL_EMPTY 
                && board->cells [x][y + 1] == CELL_EMPTY && board->cells [x + 1][y + 1] == CELL_EMPTY
                && board->cells [x + 1][y] == CELL_EMPTY && board->cells [x + 1][y - 1] == CELL_EMPTY) {flag = 0;}
        }
    }
    return flag;
}

void check_corners_for_first_deck(const GameBoard* board, char x, char y, char* flag){
    
    if (x == 9 && y == 0) { // top right corner
            if (board->cells [9][1] == CELL_EMPTY && board->cells [8][0] == CELL_EMPTY 
                && board->cells [8][1] == CELL_EMPTY) {*flag = 0;}
    }

    if (x == 9 && y == 9) { // bottom right corner
            if (board->cells [9][8] == CELL_EMPTY && board->cells [8][9] == CELL_EMPTY 
                && board->cells [8][8] == CELL_EMPTY)  {*flag = 0;}
    }  

    if (x == 0 && y == 9) { // bottom left corner
            if (board->cells [0][8] == CELL_EMPTY && board->cells [1][9] == CELL_EMPTY 
                && board->cells [1][8] == CELL_EMPTY) {*flag = 0;}
    }  

    if (x == 0 && y == 0) { // top left corner
            if (board->cells [0][1] == CELL_EMPTY && board->cells [1][0] == CELL_EMPTY 
                && board->cells [1][1] == CELL_EMPTY) {*flag = 0;}
    }  
}


/*void place_for_others_decks(const GameBoard* board, char x, char y, char deck_count){
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


}*/