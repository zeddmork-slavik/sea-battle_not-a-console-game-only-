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
    int placed_ships = 0;
    int attempts = 0;
    int max_attempts = 10;
    
    while (placed_ships < 4 && attempts < max_attempts) {
        unsigned char x = rand() % GRID_SIZE;
        unsigned char y = rand() % GRID_SIZE;
        printf("GENERATED: x=%u, y=%u\n\n", x, y);
        if(check_place_for_first_deck(board, x, y)){
            board->cells[x][y] = CELL_SHIP; // пустить через условие 1 палубности, для других будем делать запись иначе
            board->ships[placed_ships] = (Ship){x, y, 0, 1, 0};
            //printf("\nplaced_ships = %d\n", placed_ships++);
            placed_ships++;
            board->ship_count++;            
        }
        // printf("attempt %d", attempts);
        attempts++;
    }
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


void place_for_others_decks(const GameBoard* board, char x, char y, char deck_count){
    char valid_mask = 0;

    // 2. Включаем выключатели для валидных направлений
    if (can_go_left(board, x, y))  valid_mask |= (1 << LEFT);   // включили LEFT
    if (can_go_right(board, x, y)) valid_mask |= (1 << RIGHT);  // включили RIGHT
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

char can_go_left(const GameBoard* board, char x, char y){
    char flag = 0; 

    if (x != 0) { 
        if (x == 1) {
            flag = 1; // ✅ Уже проверено в check_place_for_first_deck()
        } 
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
            flag = 1; // ✅ Уже проверено в check_place_for_first_deck()
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


