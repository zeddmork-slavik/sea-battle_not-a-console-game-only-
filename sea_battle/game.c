/*#include "game.h"
#include "graphics.h"
#include <stdio.h>

Game init_game(WindowConfig config) {
    Game game = {0}; // Обнуляем всю структуру
    
    // Настраиваем конфигурацию
    Game game = WindowConfig config; 
    game.config.grid_size = 10;
    game.config.fps = 60;
    
    
    // Инициализируем графику
    game.graphics = init_graphics(win_config);
    
    // Проверяем, что всё создалось успешно
    if (game.graphics.window && game.graphics.renderer) {
        game.game_is_running = 1;
        printf("Game initialized successfully!\n");
    } else {
        game.game_is_running = 0;
        printf("Failed to initialize graphics!\n");
    }
    
    return game;
}

void handle_events(Game* game) {
    SDL_Event event;
    
    // Обрабатываем все события в очереди
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game->game_is_running = 0;
                printf("Quit event received\n");
                break;
                
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game->game_is_running = 0;
                    printf("Escape key pressed\n");
                }
                break;
                
            // TODO: Добавить обработку кликов мыши и других событий
        }
    }
}

void update_game(Game* game) {
    // Пока ничего не обновляем, просто заглушка
    // Здесь будет логика игры: проверка победы, обновление состояний и т.д.
}

void render_game(Game* game) {
    // Очищаем экран
    clear_screen(game->graphics);
    
    // Рисуем сетку
    draw_grid(game->graphics);
    
    // Показываем результат
    present_screen(game->graphics);
}

void cleanup_game(Game* game) {
    printf("Cleaning up game resources...\n");
    cleanup_graphics(game->graphics);
    SDL_Quit();
    printf("Cleanup completed.\n");
}