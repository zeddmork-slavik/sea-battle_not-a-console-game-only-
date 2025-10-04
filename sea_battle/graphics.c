#include "graphics.h"

WindowConfig create_default_config(void) {
    WindowConfig config = {
        .width = WIDTH,
        .height = HEIGHT,
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

GraphicsContext create_graphics_context(SDL_Window* window, SDL_Renderer* renderer) {
    GraphicsContext ctx;
    ctx.window = window;
    ctx.renderer = renderer;
    ctx.cell_size = CELL_SIZE; 
    ctx.ship_jup_1p = load_texture_from_file(renderer, "../images/ship_jup_1p.png");
    ctx.ship_jup_2p = load_texture_from_file(renderer, "../images/ship_jup_2p.png");
    ctx.ship_jup_4p = load_texture_from_file(renderer, "../images/ship_jup_4p.png");
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

void draw_game_boards(GraphicsContext ctx) {
    int field_size = GRID_SIZE * ctx.cell_size; // 10 × 50 = 500px
    
    // Вычисляем общую ширину двух сеток с отступами
    int total_width = 2 * field_size + BETWEEN_GRIDS + 2 * EDGE;
    
    // Центрируем по горизонтали
    int start_x = (WIDTH - total_width) / 2;  //просто добавляет лишние пиксили к EDGE  
    
    // Координаты левой сетки (игрок)
    int player_x = start_x + EDGE;
    
    // Координаты правой сетки (компьютер)
    int computer_x = player_x + field_size + BETWEEN_GRIDS;

    // выставляем начало по вертикали
    int offset_y = HEIGHT - field_size - EDGE;
    
    // Рисуем обе сетки
    draw_single_grid(ctx, player_x, offset_y, field_size);
    draw_single_grid(ctx, computer_x, offset_y, field_size);
    //клетки начинаются с нуля
    draw_ship(ctx, player_x, offset_y, 2, 5, 0, 1, ctx.ship_jup_1p);    // однопалубный
    draw_ship(ctx, player_x, offset_y, 0, 0, 0, 2, ctx.ship_jup_2p);    // двухпалубный гориз.
    draw_ship(ctx, player_x, offset_y, 4, 7, 1, 2, ctx.ship_jup_2p);    // двухпалубный вертик.
    draw_ship(ctx, player_x, offset_y, 6, 2, 0, 4, ctx.ship_jup_4p);    // четырехпалубный гориз.
    draw_ship(ctx, player_x, offset_y, 8, 4, 1, 4, ctx.ship_jup_4p); 
}

void draw_single_grid(GraphicsContext ctx, int offset_x, int offset_y, int field_size) {// offset - сдвиг
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255); // Белый цвет сетки
        
    // Рисуем вертикальные линии
    for (int x = 0; x <= GRID_SIZE; x++) {
        SDL_RenderDrawLine(
            ctx.renderer,
            offset_x + x * ctx.cell_size, offset_y,           
            offset_x + x * ctx.cell_size, offset_y + field_size 
        );
    }
    
    // Рисуем горизонтальные линии
    for (int y = 0; y <= GRID_SIZE; y++) {
        SDL_RenderDrawLine(
        ctx.renderer,
        offset_x, offset_y + y * ctx.cell_size,           
        offset_x + field_size, offset_y + y * ctx.cell_size 
        );
    }
}

void draw_ship(GraphicsContext ctx, int base_x, int base_y, int grid_x, int grid_y, 
               int direction, int deck_count, SDL_Texture* texture) {
    
    double angle = (direction == 1) ? 90.0 : 0.0;
    int ship_width, ship_height;
    SDL_QueryTexture(texture, NULL, NULL, &ship_width, &ship_height);
    
    SDL_Rect place_for_ship;
    if (direction == 0) { // горизонтальный
        place_for_ship = (SDL_Rect){
            .x = base_x + grid_x * ctx.cell_size - (ship_width - deck_count * ctx.cell_size) / 2,
            .y = base_y + grid_y * ctx.cell_size - (ship_height - ctx.cell_size) / 2,
            .w = ship_width,
            .h = ship_height
        };
    } else { // вертикальный
        place_for_ship = (SDL_Rect){
            .x = base_x + grid_x * ctx.cell_size - (ship_height - ctx.cell_size) / 2,
            .y = base_y + grid_y * ctx.cell_size - (ship_width - deck_count * ctx.cell_size) / 2,
            .w = ship_height,
            .h = ship_width
        };
    }
    /*SDL_RenderCopyEx(
    ctx.renderer,           // 1. Кто рисует - наш рисовальщик
    ctx.ship_jup_2p,        // 2. Что рисовать - текстура кораблика
    NULL,                   // 3. Какую часть текстуры (NULL = всю)
    &place_for_ship,        // 4. Где рисовать - наш прямоугольник
    angle,                  // 5. Угол поворота (90° для вертикального)
    NULL,                   // 6. Точка вращения (NULL = центр)
    SDL_FLIP_NONE           // 7. Отражение (без отражения)
);*/
SDL_RenderCopyEx(ctx.renderer, texture, NULL, &place_for_ship, angle, NULL, SDL_FLIP_NONE);
}
