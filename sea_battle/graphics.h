#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include standart libraries and other headers
#include <SDL2/SDL.h>
#include "game.h"  // Чтобы видеть WindowConfig


typedef struct { // пока очень похож на GameConfig(без сетки и fps) но в будущем...
    int width, height; 
    int cell_size;         
} WindowConfig;  

typedef struct { // рендерер, главное окно и размер игровой клетки поля в пикселях
    SDL_Window* window; // главное окно
    SDL_Renderer* renderer; // рисовальщик
    int cell_size; // размер игровой клетки поля в пикселях, cell_size = 30 каждая клетка игрового поля будет занимать 30×30 пикселей на экране.
} GraphicsContext; 



SDL_Window* create_game_window(WindowConfig config)
GraphicsContext init_graphics(WindowConfig win_config);
void render_game(GraphicsContext ctx, Game* game);   // сама функция ещё не реализована 
void cleanup_graphics(GraphicsContext ctx); // сама фугкция ещё не реализована
#endif