#define SDL_MAIN_HANDLED  // иначе при компиляции ошибка "undefined reference to `WinMain'"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#include "core/game_loop.h"
#include "core/game_state.h"
#include "graphics/graphics_state.h"

int main() {
    if (!initialize_sdl_systems()) {
        return 1;
    }
    WindowConfig config = create_default_config();
    SDL_Window* window = create_game_window(config);

    if (!window) {
        printf(
            "Window Error: %s\n",
            SDL_GetError());  // отсутсвует драйвер видеокарты, нехватает памяти видюхи, неадекватные размеры
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = create_game_renderer(window);
    if (!renderer) {
        SDL_DestroyWindow(window);
        return 1;
    }

    GraphicsContext ctx = create_graphics_context(window, renderer);  // сделаю через указатель
    GameLandmarks landmarks = calculate_landmarks(&ctx);
    srand((unsigned int)time(NULL));
    run_game(&ctx, &landmarks);

    cleanup_graphics(&ctx);
    shutdown_sdl_systems();
    return 0;
}