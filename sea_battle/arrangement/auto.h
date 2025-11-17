#ifndef AUTO_H
#define AUTO_H

typedef struct GameBoard GameBoard;

void auto_arrange_ships(GameBoard* board);
void auto_arrange_one_ship(GameBoard* board, int* ship_index);
void add_ship_to_gameBoard(GameBoard* board, char x, char y, const char ship_index, const char direction);

char check_place_for_first_deck(const GameBoard* board, const char x, const char y);
void check_corners_for_first_deck(const GameBoard* board, const char x, const char y, char* flag);
char can_go_left(const GameBoard* board, char x, char y);
char can_go_right(const GameBoard* board, char x, char y);
char can_go_up(const GameBoard* board, char x, char y);
char can_go_down(const GameBoard* board, char x, char y);
bool place_for_second_deck(const GameBoard* board, char* x, char* y, char* direction);
char select_random_direction(const char valid_mask);
bool place_for_multy_deck(const GameBoard* board, char* x1, char* y1, char* direction,
                          const char placed_decks);
void place_for_next_gorizontal_deck(const GameBoard* board, char* x1, char* y1, char* x2, char* y2,
                                    char* valid_mask, const char placed_decks);
void place_for_next_vertical_deck(const GameBoard* board, char* x1, char* y1, char* x2, char* y2,
                                  char* valid_mask, const char placed_decks);
void growing(char* valid_mask, char* x, char* y, bool* success);
#endif