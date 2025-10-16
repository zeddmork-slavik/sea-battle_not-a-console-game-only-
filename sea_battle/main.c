#define SDL_MAIN_HANDLED // иначе при компиляции ошибка "undefined reference to `WinMain'"
#include <SDL2/SDL.h>
#include <time.h>
#include "graphics.h"
#include "board.h"
#include "game.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // подключение модулей SDL можно будет подключить аудио, Video (даёт доступ к SDL_CreateWindow и SDL_Renderer)
        printf("SDL Error: %s\n", SDL_GetError()); // при ошибке SDL_Init возвращает отрицательное число
        return 1; //SDL_GetError - функция для получения текстового человекочитаемого описания ошибки
    }
    
    WindowConfig config = create_default_config(); 
    SDL_Window* window = create_game_window(config);
    
    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());// отсутсвует драйвер видеокарты, нехватает памяти видюхи, неадекватные размеры 
        SDL_Quit(); 
        return 1;
    }
    \
    // 3. Создание рендерера - т.е. рисовальщика (перевод - средство визуализации)
    SDL_Renderer* renderer = create_game_renderer(window);
    if (!renderer) {
        SDL_DestroyWindow(window);
        return 1;
    }
    
    GraphicsContext ctx = create_graphics_context(window, renderer); //когда вырасту точно тебя сделаю через указатель, и у нейронки даже спрашивать не буду
    GameLandmarks landmarks = calculate_landmarks(&ctx);
    srand((unsigned int)time(NULL));
    run_game(&ctx, &landmarks);
    
    // 5. Очистка
    cleanup_graphics(&ctx);
    SDL_Quit(); // освобождает только СИСТЕМНЫЕ ресурсы: инициализированные модули но не наши созданные объекты
    
    return 0;
}