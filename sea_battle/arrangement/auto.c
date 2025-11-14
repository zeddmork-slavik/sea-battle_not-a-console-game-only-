#include "auto.h"

#include <stdio.h>  // для отладок
#include <stdlib.h>

#include "core/game_state.h"

void auto_arrange_ships(GameBoard* board) {
    for (int i = 0; i < MAX_SHIPS; i++) {  // расставляем количество палуб
        board->ships[i].deck_count = i < 5 ? 1 : 2;
    }

    for (int ship_index = 0; ship_index < MAX_SHIPS; ship_index++) {
        auto_arrange_one_ship(board, &ship_index);
    }
}

void auto_arrange_one_ship(GameBoard* board, int* ship_index) {
    const int max_attempts_per_ship = 50;
    int attempts = 0;
    bool success = false;
    while (!success && attempts < max_attempts_per_ship) {
        char x = rand() % GRID_SIZE;
        char y = rand() % GRID_SIZE;
        char direction = 0;
        if (check_place_for_first_deck(board, x, y)) {  // поработать с взаимоисключениями
            if (board->ships[*ship_index].deck_count == 1) {
                add_ship_to_gameBoard(board, x, y, *ship_index, direction);
                success = true;
            } else if (place_for_second_deck(board, &x, &y, &direction)) {
                if (board->ships[*ship_index].deck_count == 2) {
                    add_ship_to_gameBoard(board, x, y, *ship_index, direction);
                    ;
                    success = true;
                }
            }
            attempts++;
        }
    }
}

void add_ship_to_gameBoard(GameBoard* board, char x, char y, const char ship_index, const char direction) {
    board->ships[ship_index].x = x;
    board->ships[ship_index].y = y;

    board->ships[ship_index].direction = direction;

    for (char placed_deck = 0; placed_deck < board->ships[ship_index].deck_count; placed_deck++) {
        board->cells[x][y] = CELL_SHIP;
        (board->ships[ship_index].direction == 0) ? x++ : y++;
    }

    board->ship_count++;
}

char check_place_for_first_deck(const GameBoard* board, const char x, const char y) {
    char flag = 0;
    if (board->cells[x][y] ==
        CELL_EMPTY) {  // решил налупить 8, 9, нулей и единиц чтоб не допустить лишних вычислений
        if (x > 0 && x < LAST && y == 0) {  // top row except corners
            if (board->cells[x + 1][0] == CELL_EMPTY && board->cells[x - 1][0] == CELL_EMPTY &&
                board->cells[x][1] == CELL_EMPTY && board->cells[x - 1][1] == CELL_EMPTY &&
                board->cells[x + 1][1] == CELL_EMPTY) {
                flag = 1;
            }
        }

        if (x > 0 && x < LAST && y == LAST) {  // bottom row except corners
            if (board->cells[x + 1][LAST] == CELL_EMPTY && board->cells[x - 1][LAST] == CELL_EMPTY &&
                board->cells[x][PENULTIMATE] == CELL_EMPTY &&
                board->cells[x - 1][PENULTIMATE] == CELL_EMPTY &&
                board->cells[x + 1][PENULTIMATE] == CELL_EMPTY) {
                flag = 1;
            }
        }

        if (x == 0 && y > 0 && y < LAST) {  // left col except corners
            if (board->cells[0][y - 1] == CELL_EMPTY && board->cells[0][y + 1] == CELL_EMPTY &&
                board->cells[1][y - 1] == CELL_EMPTY && board->cells[1][y] == CELL_EMPTY &&
                board->cells[1][y + 1] == CELL_EMPTY) {
                flag = 1;
            }
        }

        if (x == 9 && y > 0 && y < LAST) {  // right col except corners
            if (board->cells[LAST][y - 1] == CELL_EMPTY && board->cells[LAST][y + 1] == CELL_EMPTY &&
                board->cells[PENULTIMATE][y - 1] == CELL_EMPTY &&
                board->cells[PENULTIMATE][y] == CELL_EMPTY &&
                board->cells[PENULTIMATE][y + 1] == CELL_EMPTY) {
                flag = 1;
            }
        }

        check_corners_for_first_deck(board, x, y, &flag);

        if (x > 0 && x < LAST && y > 0 && y < LAST) {  // without borders
            if (board->cells[x - 1][y - 1] == CELL_EMPTY && board->cells[x - 1][y] == CELL_EMPTY &&
                board->cells[x - 1][y + 1] == CELL_EMPTY && board->cells[x][y - 1] == CELL_EMPTY &&
                board->cells[x][y + 1] == CELL_EMPTY && board->cells[x + 1][y + 1] == CELL_EMPTY &&
                board->cells[x + 1][y] == CELL_EMPTY && board->cells[x + 1][y - 1] == CELL_EMPTY) {
                flag = 1;
            }
        }
    }

    return flag;
}

void check_corners_for_first_deck(const GameBoard* board, const char x, const char y, char* flag) {
    if (x == LAST && y == 0) {  // top right corner
        if (board->cells[LAST][1] == CELL_EMPTY && board->cells[8][0] == CELL_EMPTY &&
            board->cells[PENULTIMATE][1] == CELL_EMPTY) {
            *flag = 1;
        }
    }

    if (x == LAST && y == LAST) {  // bottom right corner
        if (board->cells[LAST][PENULTIMATE] == CELL_EMPTY && board->cells[PENULTIMATE][LAST] == CELL_EMPTY &&
            board->cells[PENULTIMATE][PENULTIMATE] == CELL_EMPTY) {
            *flag = 1;
        }
    }

    if (x == 0 && y == LAST) {  // bottom left corner
        if (board->cells[0][PENULTIMATE] == CELL_EMPTY && board->cells[1][LAST] == CELL_EMPTY &&
            board->cells[1][PENULTIMATE] == CELL_EMPTY) {
            *flag = 1;
        }
    }

    if (x == 0 && y == 0) {  // top left corner
        if (board->cells[0][1] == CELL_EMPTY && board->cells[1][0] == CELL_EMPTY &&
            board->cells[1][1] == CELL_EMPTY) {
            *flag = 1;
        }
    }
}

char can_go_left(const GameBoard* board, char x, char y) {
    char flag = 0;

    if (x != 0) {
        if (x == 1) {
            flag = 1;
        } else if (y == 0) {
            flag = (board->cells[x - 2][0] == CELL_EMPTY && board->cells[x - 2][1] == CELL_EMPTY);
        } else if (y >= 1 && y <= PENULTIMATE) {
            flag = (board->cells[x - 2][y - 1] == CELL_EMPTY && board->cells[x - 2][y] == CELL_EMPTY &&
                    board->cells[x - 2][y + 1] == CELL_EMPTY);
        } else if (y == LAST) {
            flag =
                (board->cells[x - 2][PENULTIMATE] == CELL_EMPTY && board->cells[x - 2][LAST] == CELL_EMPTY);
        }
    }
    return flag;
}

char can_go_right(const GameBoard* board, char x, char y) {
    char flag = 0;

    if (x != LAST) {
        if (x == PENULTIMATE) {
            flag = 1;
        } else if (y == 0) {
            flag = (board->cells[x + 2][0] == CELL_EMPTY && board->cells[x + 2][1] == CELL_EMPTY);
        } else if (y >= 1 && y <= PENULTIMATE) {
            flag = (board->cells[x + 2][y - 1] == CELL_EMPTY && board->cells[x + 2][y] == CELL_EMPTY &&
                    board->cells[x + 2][y + 1] == CELL_EMPTY);
        } else if (y == LAST) {
            flag =
                (board->cells[x + 2][PENULTIMATE] == CELL_EMPTY && board->cells[x + 2][LAST] == CELL_EMPTY);
        }
    }

    return flag;
}

char can_go_up(const GameBoard* board, char x, char y) {
    char flag = 0;

    if (y != 0) {
        if (y == 1) {
            flag = 1;
        } else if (x == 0) {
            flag = (board->cells[x][y - 2] == CELL_EMPTY && board->cells[x + 1][y - 2] == CELL_EMPTY);
        } else if (x >= 1 && x <= PENULTIMATE) {
            flag = (board->cells[x - 1][y - 2] == CELL_EMPTY && board->cells[x][y - 2] == CELL_EMPTY &&
                    board->cells[x + 1][y - 2] == CELL_EMPTY);
        } else if (x == LAST) {
            flag =
                (board->cells[PENULTIMATE][y - 2] == CELL_EMPTY && board->cells[LAST][y - 2] == CELL_EMPTY);
        }
    }

    return flag;
}

char can_go_down(const GameBoard* board, char x, char y) {
    char flag = 0;

    if (y != LAST) {
        if (y == PENULTIMATE) {
            flag = 1;
        } else if (x == 0) {
            flag = (board->cells[x][y + 2] == CELL_EMPTY && board->cells[x + 1][y + 2] == CELL_EMPTY);
        } else if (x >= 1 && x <= PENULTIMATE) {
            flag = (board->cells[x - 1][y + 2] == CELL_EMPTY && board->cells[x][y + 2] == CELL_EMPTY &&
                    board->cells[x + 1][y + 2] == CELL_EMPTY);
        } else if (x == LAST) {
            flag =
                (board->cells[PENULTIMATE][y + 2] == CELL_EMPTY && board->cells[LAST][y + 2] == CELL_EMPTY);
        }
    }

    return flag;
}

bool place_for_second_deck(const GameBoard* board, char* x, char* y, char* direction) {
    bool success = false;
    char valid_mask = 0;

    // 2. Включаем выключатели для валидных направлений
    if (can_go_left(board, *x, *y)) valid_mask |= (1 << LEFT);    // включили LEFT
    if (can_go_right(board, *x, *y)) valid_mask |= (1 << RIGHT);  // включили RIGHT
    if (can_go_up(board, *x, *y)) valid_mask |= (1 << UP);        // включили UP
    if (can_go_down(board, *x, *y)) valid_mask |= (1 << DOWN);    // включили DOWN

    char direction_for_growing = select_random_direction(valid_mask);

    if (direction_for_growing != INVALID_DIRECTION) {
        success = true;

        if (direction_for_growing == UP || direction_for_growing == DOWN) {
            *direction = 1;
        }

        if (direction_for_growing == LEFT) {
            *x -= 1;
        }  // !!!!!!!!!!! пока только для вершины двухпалубных, для трёшек нужно будет учитывать все
           // направления
        if (direction_for_growing == UP) {
            *y -= 1;
        }
    }
    return success;
}

char select_random_direction(const char valid_mask) {
    char count = 0;
    char directions[4];  // Массив для валидных направлений
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

bool place_for_therd_deck(const GameBoard* board, char* x1, char* y1, char* direction) {
    bool success = false;
    char valid_mask = 0;
    char x2, y2;
    if (*direction == 0) {
        place_for_therd_gorizontal_deck(board, x1, y1, &x2, &y2, &valid_mask, &success);
    } else {
        place_for_therd_vertical_deck(board, x1, y1, &x2, &y2, &valid_mask, &success);
    }

    char direction_for_growing = select_random_direction(
        valid_mask);  // универсальную для 3и 4 я то напишу, получитсч ли совместить с 2х?

    if (direction_for_growing != INVALID_DIRECTION) {
        success = true;

        if (direction_for_growing == UP || direction_for_growing == DOWN) {
        }

        if (direction_for_growing == LEFT) {
            *x -= 1;
        }  // !!!!!!!!!!! пока только для вершины двухпалубных, для трёшек нужно будет учитывать все
           // направления
        if (direction_for_growing == UP) {
            *y -= 1;
        }
    }
    return success;
}

void place_for_therd_gorizontal_deck(const GameBoard* board, char* x1, char* y1, char* x2, char* y2,
                                     char* valid_mask, bool* success) {
    *x2 = *x1 + 1;
    *y2 = *y1;
    if (can_go_left(board, *x1, *y1)) valid_mask |= (1 << LEFT);
    if (can_go_right(board, *x2, *y2)) valid_mask |= (1 << RIGHT);
}

void place_for_therd_vertical_deck(const GameBoard* board, char* x1, char* y1, char* x2, char* y2,
                                   char* valid_mask, bool* success) {
    *x2 = *x1;
    *y2 = *y1 + 1;
    if (can_go_up(board, *x1, *y1)) *valid_mask |= (1 << UP);
    if (can_go_down(board, *x2, *y2)) *valid_mask |= (1 << DOWN);
    return *valid_mask;
}