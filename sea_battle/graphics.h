#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include standart libraries and other headers
#include <SDL2/SDL.h>


typedef struct {
    int width, height;   
    int cell_size;            // ???? ДОБАВИМ: чтобы передавать размер клетки   , посмотрим - нужно ли это здесь  
} WindowConfig;  

typedef struct { // рендерер, главное окно и размер игровой клетки поля в пикселях
    SDL_Window* window; // главное окно
    SDL_Renderer* renderer; // рисовальщик
    int cell_size; // размер игровой клетки поля в пикселях, cell_size = 30 каждая клетка игрового поля будет занимать 30×30 пикселей на экране.
} GraphicsContext; 


WindowConfig create_default_config(void);
SDL_Window* create_game_window(WindowConfig config);
SDL_Renderer* create_game_renderer(SDL_Window* window); 
GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer, WindowConfig config);
// Важные функции для отрисовки, которые мы скоро реализуем
void cleanup_graphics(GraphicsContext ctx);
void clear_screen(GraphicsContext ctx); // Очистка экрана (заливка цветом)
void present_screen(GraphicsContext ctx); // Показ нарисованного кадра
void draw_grid(GraphicsContext ctx); // Отрисовка сетки 10x10
#endif