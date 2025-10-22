#ifndef GRAPHICS_STATE_H
#define GRAPHICS_STATE_H




typedef struct GraphicsContext GraphicsContext;



typedef struct {
    int width, height;   
} WindowConfig;  




WindowConfig create_default_config(void);
SDL_Window* create_game_window(WindowConfig config);
SDL_Renderer* create_game_renderer(SDL_Window* window); 
GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* load_texture_from_file(SDL_Renderer* renderer, const char* filename);
void cleanup_graphics(GraphicsContext* ctx);
void clear_screen(const GraphicsContext* ctx); // Очистка экрана (заливка цветом)
void present_screen(const GraphicsContext* ctx); // Показ нарисованного кадра

#endif