#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graphics_state.h"
#include "core/game_state.h"


WindowConfig create_default_config(void) {
    return (WindowConfig) {WIDTH, HEIGHT};
}

SDL_Window* create_game_window(WindowConfig config) 
{  // empty
    return SDL_CreateWindow( // SDL_Window - непрозрачная структура из SDL, 
        //SDL_CreateWindow создаёт HWND (идентификатор окна для ОС) 
        //экземпляр window заниимает мегабайты видеопамяти, это реальный объект в ОС который мы видим на экране
        "SEA BATTLE",   // Заголовок окна        
        SDL_WINDOWPOS_CENTERED,  // Позиция нашего окна в рамках экрана от левого края X (CENTERED - центр)
        30,  // Позиция Y от верха
        config.width, config.height,
        SDL_WINDOW_SHOWN // здесь флаги отображения, конкретно в этом случае - показать окно сразу
    );
}

SDL_Renderer* create_game_renderer(SDL_Window* window) 
{   //empty

    //Индекс драйвера (-1 = авто) OpenGL - старенький работяга для всех, Direct3D - специально для винды, Vulkan - крутой но не робит на маках
    //SDL_RENDERER_ACCELERATED  определяется аппаратное ускорение - здесь используй видюху(1000 FPS) а не проц(60FPS) 
    //или есть VSync - он заставляет видюху ждать обновления кадра на мониторе, но говорят для обучения это too mach но он нужен будет на анимацию
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer) {//отправляет в мейн
    GraphicsContext ctx;
    ctx.window = window;
    ctx.renderer = renderer;
    ctx.width_of_window = WIDTH;
    ctx.height_of_window = HEIGHT;
    ctx.cell_size = CELL_SIZE; 
    ctx.field_size = GRID_SIZE * CELL_SIZE;
    ctx.ship_jup_1p = load_texture_from_file(renderer, "../images/ship_jup_1p.png");
    ctx.ship_jup_2p = load_texture_from_file(renderer, "../images/ship_jup_2p.png");
    ctx.ship_jup_3p = load_texture_from_file(renderer, "../images/ship_jup_3p.png");
    ctx.ship_jup_4p = load_texture_from_file(renderer, "../images/ship_jup_4p.png");
    ctx.fire_ship0 = load_texture_from_file(renderer, "../images/fire_ship0.png");
    ctx.fire_ship1 = load_texture_from_file(renderer, "../images/fire_ship1.png");
    ctx.fire_ship2 = load_texture_from_file(renderer, "../images/fire_ship2.png");
    ctx.fire_ship3 = load_texture_from_file(renderer, "../images/fire_ship3.png");
    ctx.fire_ship4 = load_texture_from_file(renderer, "../images/fire_ship4.png");
    ctx.fire_ship5 = load_texture_from_file(renderer, "../images/fire_ship5.png");
    ctx.fire_ship6 = load_texture_from_file(renderer, "../images/fire_ship6.png");
    ctx.fire_ship7 = load_texture_from_file(renderer, "../images/fire_ship7.png");
    ctx.player_island_texture = load_texture_from_file(renderer, "../images/player_island.png");
    ctx.computer_island_texture = load_texture_from_file(renderer, "../images/computer_island.png");
    ctx.spray_texture = load_texture_from_file(renderer, "../images/spray.png");
    return ctx;
}

SDL_Texture* load_texture_from_file(SDL_Renderer* renderer, const char* filename) {
    SDL_Texture* texture = NULL;
    
    // Шаг 2: IMG_Load() - это как РАСПАКОВКА PNG файла в оперативную память
    SDL_Surface* surface = IMG_Load(filename);
    
    // Шаг 3: Проверяем - удалось ли распаковать файл?
    if (surface) {
        // Шаг 4: SDL_CreateTextureFromSurface() - это как перенести картинку в память видеокарты (она быстрее намного)
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        // Шаг 5: SDL_FreeSurface() - освобождаем оперативную память
        SDL_FreeSurface(surface);
    }
    // Шаг 6: Возвращаем готовую текстуру (или NULL если была ошибка)
    return texture;
}

void cleanup_graphics(GraphicsContext* ctx) { // вызывается из main()
    if (ctx->renderer) {
        SDL_DestroyRenderer(ctx->renderer); // если напишем после SDL_DestroyWindow - то ничего не будет удалено - не найдут его при отсутсвии окна
        ctx->renderer = NULL;
    }
    if (ctx->window) {
        SDL_DestroyWindow(ctx->window); // закрывает окно в ОС, освобождает всю связанную память, аннулирует указатель
        ctx->window = NULL;
    }
}

void clear_screen(const GraphicsContext* ctx) { // вызывается из run_game()
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255); // Черный фон
    SDL_RenderClear(ctx->renderer); // залить весь экран
}

void present_screen(const GraphicsContext* ctx) { // вызывается из run_game()
    SDL_RenderPresent(ctx->renderer);
}


