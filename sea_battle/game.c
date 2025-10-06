#include "game.h"
#include <SDL2/SDL.h>

void run_game(GraphicsContext ctx) { //зарефакторить её нужно будет скоро
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
    int total_width = 2 * ctx.field_size + BETWEEN_GRIDS + 2 * EDGE;
    int start_x = (WIDTH - total_width) / 2;  //просто добавляет лишние пиксили к EDGE  
    // Координаты левой сетки (игрок)
    int player_x = start_x + EDGE;
    // Координаты правой сетки (компьютер)
    int computer_x = player_x + ctx.field_size + BETWEEN_GRIDS;
    // выставляем начало по вертикали
    int offset_y = HEIGHT - ctx.field_size - EDGE;  
        // Отрисовка
    clear_screen(ctx);
    draw_board(ctx, player_x, offset_y, game.player_board, 1);    // 1 - показываем корабли
    draw_board(ctx, computer_x, offset_y, game.computer_board, 1);
    
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
    cannon->is_animating = 0;
    
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