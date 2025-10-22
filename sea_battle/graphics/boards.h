#ifndef BOARDS_H
#define BOARDS_H


typedef struct GraphicsContext GraphicsContext;
typedef struct GameBoard GameBoard;


void draw_board(const GraphicsContext* ctx, int base_x, int base_y, const GameBoard* board, char show_ships);
void draw_single_grid(const GraphicsContext* ctx, int offset_x, int offset_y);

#endif