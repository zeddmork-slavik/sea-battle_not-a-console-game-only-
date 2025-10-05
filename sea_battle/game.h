#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "board.h"

typedef struct {
    int running;
    GameBoard player_board;
    GameBoard computer_board;
    // позже добавим состояние игры (расстановка, ход игрока и т.д.)
} GameState;

void run_game(GraphicsContext ctx);

#endif