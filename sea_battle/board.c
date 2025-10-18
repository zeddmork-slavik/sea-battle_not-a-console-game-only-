#include <stdlib.h>
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
    
    //printf("landmarks total_width = %d, start_x = %d, cell_size = %d ;"
    //"field_size = %d ; player_x = %d ; computer_x = %d, offset_y = %d", total_width, start_x, 
    //ctx-> cell_size, ctx-> field_size, start_x + EDGE, start_x + EDGE + ctx->field_size + BETWEEN_GRIDS, ctx->height_of_window - ctx->field_size - EDGE);
    GameLandmarks landmarks;        
    landmarks.player_x = start_x + EDGE; 
    landmarks.computer_x = landmarks.player_x + ctx->field_size + BETWEEN_GRIDS;
    landmarks.offset_y = ctx->height_of_window - ctx->field_size - EDGE; // 179
    
    return landmarks;
}

void auto_arrange_ships(GameBoard* board){
    
    for(char i = 0; i < MAX_SHIPS; i++){ // расставляем количество палуб
        board->ships[i].deck_count = i < 5 ? 1: 2;
    }
    
    const char max_attempts_per_ship = 50;

    for(char ship_index = 0; ship_index < MAX_SHIPS; ship_index++) {
        char attempts = 0;
        char success = 0;
            
        while(!success && attempts < max_attempts_per_ship) {
            char x = rand() % GRID_SIZE;
            char y = rand() % GRID_SIZE;
            //printf("GENERATED: x=%u, y=%u\n\n", x, y);
            if(check_place_for_first_deck(board, x, y)){
                if(board->ships[i].deck_count == 1) 
                    {add_ship_to_gameBoard(board, x, y, ship_index, 0); success = 1;
                } 
                char direction = place_for_others_decks(board,  &x, &y, board->ships[i].deck_count);
                else if (direction = 2){
                    add_ship_to_gameBoard(board, x, y, ship_index, direction); ; success = 1;
                }    
            }
            attempts++;
        }
    } 
}  

void add_ship_to_gameBoard(GameBoard* board, const char xh, const char yh, const char ship_index, const char direction){
    board->ships[ship_index].x = xh;
    board->ships[ship_index].y = yh;
    
    // direction для однопалубных не заполняем, дефолтный 0 это горизонтальный
    if(board->ships[ship_index].deck_count > 1){
        
        
        
        
        
        
        board->ships[ship_index].} 






    board->ship_count++;
}

char check_place_for_first_deck(const GameBoard* board, const char x, const char y){
    char flag = 0;
    if(board->cells[x][y] == CELL_EMPTY) { // решил налупить 8, 9, нулей и единиц чтоб не допустить лишних вычислений
        if (x > 0 && x < LAST && y == 0) { // top row except corners
            if (board->cells[x + 1][0] == CELL_EMPTY && board->cells[x - 1][0] == CELL_EMPTY 
                && board->cells[x][1] == CELL_EMPTY && board->cells[x - 1][1] == CELL_EMPTY 
                && board->cells[x + 1][1] == CELL_EMPTY) {flag = 1;}
        }
        
        if (x > 0 && x < LAST && y == LAST) { // bottom row except corners
            if (board->cells[x + 1][LAST] == CELL_EMPTY && board->cells[x - 1][LAST] == CELL_EMPTY 
                && board->cells[x][PENULTIMATE] == CELL_EMPTY && board->cells[x - 1][PENULTIMATE] == CELL_EMPTY 
                && board->cells[x + 1][PENULTIMATE] == CELL_EMPTY) {flag = 1;}
        }

        if (x == 0 && y > 0 && y < LAST) { // left col except corners
            if (board->cells[0][y -1] == CELL_EMPTY && board->cells[0][y + 1] == CELL_EMPTY 
                && board->cells[1][y - 1] == CELL_EMPTY && board->cells[1][y] == CELL_EMPTY 
                && board->cells[1][y + 1] == CELL_EMPTY) {flag = 1;}
        }
    
        if (x == 9 && y > 0 && y < LAST) { // right col except corners
            if (board->cells [LAST][y - 1] == CELL_EMPTY && board->cells [LAST][y + 1] == CELL_EMPTY 
                && board->cells [PENULTIMATE][y - 1] == CELL_EMPTY && board->cells [PENULTIMATE][y] == CELL_EMPTY 
                && board->cells [PENULTIMATE][y + 1] == CELL_EMPTY) {flag = 1;}
        }    
    
        check_corners_for_first_deck(board, x, y, &flag);
    
        if (x > 0 && x < LAST && y > 0 && y < LAST) { // without borders
            if (board->cells [x - 1][y - 1] == CELL_EMPTY && board->cells [x - 1][y] == CELL_EMPTY
                && board->cells [x - 1][y + 1] == CELL_EMPTY && board->cells [x][y - 1] == CELL_EMPTY 
                && board->cells [x][y + 1] == CELL_EMPTY && board->cells [x + 1][y + 1] == CELL_EMPTY
                && board->cells [x + 1][y] == CELL_EMPTY && board->cells [x + 1][y - 1] == CELL_EMPTY) {flag = 1;}
        }
    }

    //printf("\nflag %d", flag);
    return flag;
}

void check_corners_for_first_deck(const GameBoard* board, const char x, const char y, char* flag){
    
    if (x == LAST && y == 0) { // top right corner
            if (board->cells [LAST][1] == CELL_EMPTY && board->cells [8][0] == CELL_EMPTY 
                && board->cells [PENULTIMATE][1] == CELL_EMPTY) {*flag = 1;}
    }

    if (x == LAST && y == LAST) { // bottom right corner
            if (board->cells [LAST][PENULTIMATE] == CELL_EMPTY && board->cells [PENULTIMATE][LAST] == CELL_EMPTY 
                && board->cells [PENULTIMATE][PENULTIMATE] == CELL_EMPTY)  {*flag = 1;}
    }  

    if (x == 0 && y == LAST) { // bottom left corner
            if (board->cells [0][PENULTIMATE] == CELL_EMPTY && board->cells [1][LAST] == CELL_EMPTY 
                && board->cells [1][PENULTIMATE] == CELL_EMPTY) {*flag = 1;}
    }  

    if (x == 0 && y == 0) { // top left corner
            if (board->cells [0][1] == CELL_EMPTY && board->cells [1][0] == CELL_EMPTY 
                && board->cells [1][1] == CELL_EMPTY) {*flag = 1;}
    }  
}


char place_for_others_decks(const GameBoard* board, char* x, char* y, const char deck_count){
    char direction_for_vertical_ship = 2; // недопустимое значение чтобы можно было использовать как флаг
    char valid_mask = 0;

    // 2. Включаем выключатели для валидных направлений
    if (can_go_left(board, *x, *y))  valid_mask |= (1 << LEFT);   // включили LEFT
    if (can_go_right(board, *x, *y)) valid_mask |= (1 << RIGHT);  // включили RIGHT
    if (can_go_up(board, *x, *y))    valid_mask |= (1 << UP);     // включили UP
    if (can_go_down(board, *x, *y))  valid_mask |= (1 << DOWN);   // включили DOWN

    // 3. Проверяем конкретное направление
    //if (valid_mask & (1 << LEFT)) {
    //printf("Можно идти влево!\n");
    //}
    char direc = select_random_direction(valid_mask) 
   
  
    return direction_for_vertical_ship;
}

char can_go_left(const GameBoard* board, char x, char y){
    char flag = 0; 

    if (x != 0) { 
        if (x == 1) {
            flag = 1;}
        else if (y == 0) {
            flag = (board->cells[x - 2][0] == CELL_EMPTY && 
            board->cells[x - 2][1] == CELL_EMPTY);
        }
        else if (y >= 1 && y <= PENULTIMATE) {
            flag = (board->cells[x - 2][y - 1] == CELL_EMPTY && 
            board->cells[x - 2][y] == CELL_EMPTY && 
            board->cells[x - 2][y + 1] == CELL_EMPTY);
        }
        else if (y == LAST) {
            flag = (board->cells[x - 2][PENULTIMATE] == CELL_EMPTY && 
            board->cells[x - 2][LAST] == CELL_EMPTY);
            }
        }
    return flag; 
}

char can_go_right(const GameBoard* board, char x, char y){
    char flag = 0; 

    if (x != LAST) { 
        if (x == PENULTIMATE) {
            flag = 1; 
        } 
        else if (y == 0) {
            flag = (board->cells[x + 2][0] == CELL_EMPTY && 
                    board->cells[x + 2][1] == CELL_EMPTY);
        }
        else if (y >= 1 && y <= PENULTIMATE) {
            flag = (board->cells[x + 2][y - 1] == CELL_EMPTY && 
            board->cells[x + 2][y] == CELL_EMPTY && 
            board->cells[x + 2][y + 1] == CELL_EMPTY);
        }
        else if (y == LAST) {
            flag = (board->cells[x + 2][PENULTIMATE] == CELL_EMPTY && 
            board->cells[x + 2][LAST] == CELL_EMPTY);
        }
    }
    
    return flag; 
}

char can_go_up(const GameBoard* board, char x, char y){
    char flag = 0; 

    if (y != 0) { 
        if (y == 1) {
            flag = 1; 
        } 
        else if (x == 0) {
            flag = (board->cells[x][y - 2] == CELL_EMPTY && 
                    board->cells[x + 1][y - 2] == CELL_EMPTY);
        }
        else if (x >= 1 && x <= PENULTIMATE) {
            flag = (board->cells[x - 1][y - 2] == CELL_EMPTY && 
            board->cells[x][y - 2] == CELL_EMPTY && 
            board->cells[x + 1][y - 2] == CELL_EMPTY);
        }
        else if (x == LAST) {
            flag = (board->cells[PENULTIMATE][y - 2] == CELL_EMPTY && 
            board->cells[LAST][y - 2] == CELL_EMPTY);
        }
    }
    
    return flag; 
}

char can_go_down(const GameBoard* board, char x, char y){
    char flag = 0; 

    if (y != LAST) { 
        if (y == PENULTIMATE) {
            flag = 1; 
        } 
        else if (x == 0) {
            flag = (board->cells[x][y + 2] == CELL_EMPTY && 
                    board->cells[x + 1][y + 2] == CELL_EMPTY);
        }
        else if (x >= 1 && x <= PENULTIMATE) {
            flag = (board->cells[x - 1][y + 2] == CELL_EMPTY && 
            board->cells[x][y + 2] == CELL_EMPTY && 
            board->cells[x + 1][y + 2] == CELL_EMPTY);
        }
        else if (x == LAST) {
            flag = (board->cells[PENULTIMATE][y + 2] == CELL_EMPTY && 
            board->cells[LAST][y + 2] == CELL_EMPTY);
        }
    }
    
    return flag; 
}

char select_random_direction(const char valid_mask) {
    char count = 0;
    char directions[4]; // Массив для валидных направлений
    char selected_direction = INVALID_DIRECTION;
    
    // Собираем все валидные направления в массив
    for (char i = 0; i < 4; i++) {
        if (valid_mask & (1 << i)) {
            directions[count] = i;
            count++;
        }
    }
    
    // Выбираем случайное из собранных
    if (count > 0) {
        char random_index = rand() % count;
        selected_direction = directions[random_index];
    }
    
    return selected_direction;
}
