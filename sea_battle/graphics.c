#include "graphics.h"



SDL_Window* create_game_window(WindowConfig config) {     // ??? вообще всё разбирать нннадо!!!!!!!!!!!!!!!!!!
    return SDL_CreateWindow(
        config.title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config.width, config.height,
        SDL_WINDOW_SHOWN
    );
}

GraphicsContext init_graphics(WindowConfig win_config) {   // ????  тоже нужно всё это разбирать
    GraphicsContext ctx;
    ctx.window = create_game_window(win_config);
    ctx.renderer = SDL_CreateRenderer(ctx.window, -1, 
                                    SDL_RENDERER_ACCELERATED);
    ctx.cell_size = win_config.cell_size;
    return ctx;
}