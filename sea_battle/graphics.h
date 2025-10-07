#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include standart libraries and other headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#define WIDTH 1270
#define HEIGHT 700
#define CELL_SIZE 50
#define GRID_SIZE 10


typedef struct {
    int width, height;   
} WindowConfig;  

typedef struct { // рендерер, главное окно и размер игровой клетки поля в пикселях
    SDL_Window* window; // главное окно
    SDL_Renderer* renderer; // рисовальщик
    int width_of_window;
    int height_of_window;
    char cell_size; // размер игровой клетки поля в пикселях, каждая клетка игрового поля будет занимать cell_size Х cell_size пикселей на экране.
    int field_size;
    SDL_Texture* ship_jup_1p;    // однопалубный
    SDL_Texture* ship_jup_2p;  // двухпалубный   
    SDL_Texture* ship_jup_3p;
    SDL_Texture* ship_jup_4p;  // четырехпалубный 
} GraphicsContext; 


WindowConfig create_default_config(void);
SDL_Window* create_game_window(WindowConfig config);
SDL_Renderer* create_game_renderer(SDL_Window* window); 
GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer);
void clear_screen(const GraphicsContext* ctx); // Очистка экрана (заливка цветом)
void present_screen(GraphicsContext ctx); // Показ нарисованного кадра
void draw_board(const GraphicsContext* ctx, int base_x, int base_y, const GameBoard* board, char show_ships);
void cleanup_graphics(GraphicsContext ctx);
void draw_single_grid(GraphicsContext ctx, int offset_x, int offset_y);
SDL_Texture* load_texture_from_file(SDL_Renderer* renderer, const char* filename);
void draw_ship(GraphicsContext ctx, int base_x, int base_y, int grid_x, int grid_y, 
               int direction, int deck_count, SDL_Texture* texture);
void draw_cannon(GraphicsContext ctx, const Cannon* cannon);
#endif