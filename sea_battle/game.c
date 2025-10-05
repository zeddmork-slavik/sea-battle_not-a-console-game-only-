#include "game.h"
#include <SDL2/SDL.h>

void run_game(GraphicsContext ctx) {
    GameState game = {0};
    game.running = 1;
    
    // Инициализация досок
    init_board(&game.player_board);
    init_board(&game.computer_board);
    
    game.player_board.ships[0] = (Ship){2, 5, 0, 1, 0};
    game.player_board.ships[1] = (Ship){0, 0, 0, 2, 0};
    game.player_board.ships[2] = (Ship){4, 7, 1, 2, 0};
    game.player_board.ships[3] = (Ship){6, 2, 0, 4, 0};
    game.player_board.ships[4] = (Ship){8, 4, 1, 4, 0};
    game.player_board.ship_count = 5;
    
    SDL_Event event;
    
    while (game.running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.running = 0;
            }
        }
        
        // Отрисовка
        clear_screen(ctx);
        draw_game_boards(ctx, game.player_board);
        present_screen(ctx);
        
        SDL_Delay(33); // ~30 FPS для пошаговой игры
    }
}