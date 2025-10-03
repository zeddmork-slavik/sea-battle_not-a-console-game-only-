#include "graphics.h"

WindowConfig create_default_config(void) {
    WindowConfig config = {
        .width = 1024,
        .height = 700,
        .cell_size = 40,
    };
    return config;
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

GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer, WindowConfig config) {
    GraphicsContext ctx;
    ctx.window = window;
    ctx.renderer = renderer;
    ctx.cell_size = config.cell_size;  // Берем cell_size из переданного config
    return ctx;
}

// Реализации функций отрисовки (добавляем к вашему коду)
void cleanup_graphics(GraphicsContext ctx) {
    if (ctx.renderer) {
        SDL_DestroyRenderer(ctx.renderer); // если напишем после SDL_DestroyWindow - то ничего не будет удалено - не найдут его при отсутсвии окна
    }
    if (ctx.window) {
        SDL_DestroyWindow(ctx.window); // закрывает окно в ОС, освобождает всю связанную память, аннулирует указатель
    }
}

void clear_screen(GraphicsContext ctx) {
    SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255); // Черный фон
    SDL_RenderClear(ctx.renderer); // залить весь экран
}

void present_screen(GraphicsContext ctx) {
    SDL_RenderPresent(ctx.renderer);
}

void draw_grid(GraphicsContext ctx) {
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255); // Белый цвет сетки
    
    int grid_size = 10; // Стандартное поле для морского боя
    
    // Рисуем вертикальные линии
    for (int x = 0; x <= grid_size; x++) {
        SDL_RenderDrawLine(
            ctx.renderer,
            x * ctx.cell_size, 0,
            x * ctx.cell_size, grid_size * ctx.cell_size
        );
    }
    
    // Рисуем горизонтальные линии
    for (int y = 0; y <= grid_size; y++) {
        SDL_RenderDrawLine(
            ctx.renderer,
            0, y * ctx.cell_size,
            grid_size * ctx.cell_size, y * ctx.cell_size
        );
    }
}