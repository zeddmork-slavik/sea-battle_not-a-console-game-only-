#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WIDTH 1270
#define HEIGHT 700
#define CELL_SIZE 50
#define GRID_SIZE 10
#define X_CRUTCH_VERTICAL_SHIPS 62
#define WIDTH_CANON_TEXTURE 176  // пока для ствола и опоры одинаковые, дублировать не буду
#define HEIGHT_CANON_TEXTURE 122
#define WIDTH_PLAYER_ISLAND_TEXTURE 167
#define HEIGHT_PLAYER_ISLAND_TEXTURE 173
#define WIDTH_COMPUTER_ISLAND_TEXTURE 187
#define HEIGHT_COMPUTER_ISLAND_TEXTURE 146
#define X_CRUTCH_COMPUTER_ISLAND - 4
#define Y_CRUTCH_COMPUTER_ISLAND 15
#define WIDTH_PLAYER_FIRE_CANON_TEXTURE 165
#define HEIGHT_PLAYER_FIRE_CANON_TEXTURE 95
#define OFFSET_X_FROM_FIRE_OF_CANON 166 
#define OFFSET_Y_FROM_FIRE_OF_CANON - 3
#define PIVOT_X_FROM_FIRE_OF_CANON -76
#define PIVOT_Y_FROM_FIRE_OF_CANON 58
#define WIDTH_SPRAY_TEXTURE 50
#define HEIGHT_SPRAY_TEXTURE 66

typedef struct GameBoard GameBoard;
typedef struct Cannon Cannon;
typedef struct Cannonball Cannonball;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef struct GameAudio GameAudio;



typedef struct {
    int width, height;   
} WindowConfig;  

typedef struct GraphicsContext{ // рендерер, главное окно и размер игровой клетки поля в пикселях
    SDL_Window* window; // главное окно
    SDL_Renderer* renderer; // рисовальщик
    int width_of_window;
    int height_of_window;
    unsigned char cell_size; // размер игровой клетки поля в пикселях, каждая клетка игрового поля будет занимать cell_size Х cell_size пикселей на экране.
    int field_size;
    SDL_Texture* ship_jup_1p;    // однопалубный
    SDL_Texture* ship_jup_2p;  // двухпалубный   
    SDL_Texture* ship_jup_3p;
    SDL_Texture* ship_jup_4p;  // четырехпалубный
    SDL_Texture* player_island_texture;
    SDL_Texture* computer_island_texture; 
    SDL_Texture* spray_texture; // брызги
} GraphicsContext; 


WindowConfig create_default_config(void);
SDL_Window* create_game_window(WindowConfig config);
SDL_Renderer* create_game_renderer(SDL_Window* window); 
GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* load_texture_from_file(SDL_Renderer* renderer, const char* filename);
void cleanup_graphics(GraphicsContext* ctx);
void clear_screen(const GraphicsContext* ctx); // Очистка экрана (заливка цветом)
void present_screen(const GraphicsContext* ctx); // Показ нарисованного кадра
void draw_board(const GraphicsContext* ctx, int base_x, int base_y, const GameBoard* board, char show_ships);
void draw_single_grid(const GraphicsContext* ctx, int offset_x, int offset_y);
void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, char direction, 
               char deck_count, SDL_Texture* texture);
void draw_island(const GraphicsContext* ctx, int base_x, int base_y, char is_player);
void draw_cannon(const GraphicsContext* ctx, const Cannon* cannon, char is_player, const Cannonball* cannonball);
void draw_fire_of_cannon(const GraphicsContext* ctx, const Cannon* cannon, int base_x, int base_y);
void draw_cannonball(const GraphicsContext* ctx, const Cannonball* core);
#endif