#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include standart libraries and other headers
#include <SDL2/SDL.h>
#include "game.h"  // Чтобы видеть WindowConfig


typedef struct {
    int width, height; // ? чего
    const char* title; // ? what
    int cell_size;         // ? Размер клетки поля
} WindowConfig;  // ?? тоже интересно куда

typedef struct {
    SDL_Window* window; // непрозрачная структура из SDL, экземпляр создаётся в main 2. (это главное окно)
    SDL_Renderer* renderer; // непрозрачная структура из SDL, экземпляр создаётся в main 3.
    int cell_size; // размер клетки поля в пикселях
} GraphicsContext; // ??? куда ты?



SDL_Window* create_game_window(WindowConfig config)
GraphicsContext init_graphics(WindowConfig win_config);
void render_game(GraphicsContext ctx, Game* game);   // сама функция ещё не реализована ??? структуру гейм ещё не видел
void cleanup_graphics(GraphicsContext ctx); // сама фугкция ещё не реализована
#endif