#include "game.h"
#include <SDL2/SDL.h>

void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks) { //зарефакторить её нужно будет скоро
    GameState game = {0};
    game.running = GAME_RUNNING;
    
    // Инициализация досок
    init_board(&game.player_board);
    init_board(&game.computer_board);
    
    game.player_board.ships[0] = (Ship){2, 5, 0, 1, 0}; // вынесу попозже
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
                    game.running = DONT_RUNNING;
            }
        }
        // Отрисовка 
    clear_screen(ctx);  // это имя указателя
    draw_board(ctx, landmarks->player_x, landmarks->offset_y, game.player_board, SHOW_SHIPS); 
    draw_board(ctx, landmarks->computer_x, landmarks->offset_y, game.computer_board, SHOW_SHIPS);
    
    init_cannon(&game.player_cannon, 1, player_x + ctx.field_size + 26, offset_y + 70, ctx.renderer); // 1 - поле игрока или нет, 26=20 корабли могут выступать из полей и 6 это половина остатков ширины after картинки, компа пока не вызываем
    draw_cannon(ctx, &game.player_cannon);

    present_screen(ctx);
        
    SDL_Delay(33); // ~30 FPS для пошаговой игры
    }
}


void init_cannon(Cannon* cannon, char is_player_cannon, int base_x, int base_y, SDL_Renderer* renderer) {
    cannon->base_x = base_x;
    cannon->base_y = base_y;
    cannon->current_angle = 0;
    cannon->target_angle = 0;
    cannon->is_animating = CANNON_IDLE;
    
    if (is_player_cannon) {
        cannon->canon_platform_texture = load_texture(renderer, "../images/player_canon_platform.png");
        cannon->canon_barrel_texture = load_texture(renderer, "../images/player_canon_barrel.png");
        cannon->barrel_pivot_x = 90;
        cannon->barrel_pivot_y = 55;
    } else {
        cannon->canon_platform_texture = load_texture(renderer, "../images/computer_canon_platform.png");
        cannon->canon_barrel_texture = load_texture(renderer, "../images/computer_canon_barrel.png");
        cannon->barrel_pivot_x = 85;
        cannon->barrel_pivot_y = 60;
        cannon->current_angle = 180; // компьютер смотрит влево
        cannon->target_angle = 180;
    }
}