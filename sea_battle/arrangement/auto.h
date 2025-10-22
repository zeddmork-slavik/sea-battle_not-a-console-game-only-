#ifndef AUTO_H
#define AUTO_H



typedef struct GameBoard GameBoard;


void auto_arrange_ships(GameBoard* board);
void add_ship_to_gameBoard(GameBoard* board, char x, char y, const char ship_index, const char direction);
char place_for_others_decks(const GameBoard* board, char* x, char* y, const char deck_count, char* direction);
char select_random_direction(const char valid_mask); 
char check_place_for_first_deck(const GameBoard* board, const char x, const char y);
void check_corners_for_first_deck(const GameBoard* board, const char x, const char y, char* flag);
char can_go_left(const GameBoard* board, char x, char y);
char can_go_right(const GameBoard* board, char x, char y); 
char can_go_up(const GameBoard* board, char x, char y);
char can_go_down(const GameBoard* board, char x, char y);
#endif