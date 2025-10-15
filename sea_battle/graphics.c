#include <stdio.h>
#include "graphics.h"
#include "board.h"
#include "game.h"

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
    ctx.player_island_texture = load_texture_from_file(renderer, "../images/player_island.png");
    ctx.computer_island_texture = load_texture_from_file(renderer, "../images/computer_island.png");
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

void draw_board(const GraphicsContext* ctx, int base_x, int base_y, const GameBoard* board, char show_ships) { 
    
    draw_single_grid(ctx, base_x, base_y);
    
    if (show_ships) {
        for (char i = 0; i < board->ship_count; i++) {
            Ship ship = board->ships[i]; // 5 против 64 за копию
            SDL_Texture* texture;
    
        // Выбираем текстуру по количеству палуб
            switch(ship.deck_count) {
                case 1: texture = ctx->ship_jup_1p; break;
                case 2: texture = ctx->ship_jup_2p; break; 
                case 3: texture = ctx->ship_jup_3p; break;
                case 4: texture = ctx->ship_jup_4p; break;
                default: texture = NULL; break;
            }
    
            if (texture) {
            draw_ship(ctx, base_x, base_y, ship.x, ship.y, 
                         ship.direction, ship.deck_count, texture);
            }
        }
    }
}

void draw_single_grid(const GraphicsContext* ctx, int offset_x, int offset_y) {// offset - сдвиг
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255); // Белый цвет сетки
        
    // Рисуем вертикальные линии
    for (char x = 0; x <= GRID_SIZE; x++) {
        SDL_RenderDrawLine(
            ctx->renderer,
            offset_x + x * ctx->cell_size, offset_y,           
            offset_x + x * ctx->cell_size, offset_y + ctx->field_size 
        );
    }
    
    // Рисуем горизонтальные линии
    for (char y = 0; y <= GRID_SIZE; y++) {
        SDL_RenderDrawLine(
        ctx->renderer,
        offset_x, offset_y + y * ctx->cell_size,           
        offset_x + ctx->field_size, offset_y + y * ctx->cell_size 
        );
    }
}

void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, 
               char direction, char deck_count, SDL_Texture* texture) {
    
    double angle = (direction == 1) ? 90.0 : 0.0;
    int ship_width, ship_height;
    SDL_QueryTexture(texture, NULL, NULL, &ship_width, &ship_height);
    
    SDL_Rect place_for_ship;
    
    if (direction == 0) { // горизонтальный
        place_for_ship = (SDL_Rect){
            .x = base_x + grid_x * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) / 2),
            .y = base_y + grid_y * ctx->cell_size - ((ship_height - ctx->cell_size) / 2),
            .w = ship_width,
            .h = ship_height
        };
        printf("x = %d, y = %d", base_x + grid_x * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) / 2), base_y + grid_y * ctx->cell_size - ((ship_height - ctx->cell_size) / 2));
    } else { // вертикальный
        place_for_ship = (SDL_Rect){
            .x = base_x + X_CRUTCH_VERTICAL_SHIPS + grid_x * ctx->cell_size - ((ship_height - ctx->cell_size) >> 1), 
            .y = base_y + grid_y * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) >> 1),
            .w = ship_width,  
            .h = ship_height    
        };
    }
    
    // Ручная точка вращения - левый верхний угол
    SDL_Point pivot = {0, 0};
    SDL_RenderCopyEx(ctx->renderer, texture, NULL, &place_for_ship, angle, &pivot, SDL_FLIP_NONE);
}

void draw_island(const GraphicsContext* ctx, int base_x, int base_y, char is_player){
    SDL_Rect island_rect = {
        .x = base_x,      
        .y = base_y,
        .w = is_player? WIDTH_PLAYER_ISLAND_TEXTURE: WIDTH_COMPUTER_ISLAND_TEXTURE,
        .h = is_player? HEIGHT_PLAYER_ISLAND_TEXTURE: HEIGHT_COMPUTER_ISLAND_TEXTURE,
    };

    SDL_RenderCopy(ctx->renderer, is_player? ctx->player_island_texture: ctx->computer_island_texture, NULL, &island_rect);
}

void draw_cannon(const GraphicsContext* ctx, const Cannon* cannon, char is_player, const Cannonball* cannonball) {
    // Опора
    SDL_Rect base_rect = {
        .x = cannon->base_x,      
        .y = cannon->base_y,
        .w = WIDTH_CANON_TEXTURE,
        .h = HEIGHT_CANON_TEXTURE
    };
    SDL_RenderCopy(ctx->renderer, cannon->canon_platform_texture, NULL, &base_rect);
    
    if (cannonball->is_active) {
        draw_cannonball(ctx, cannonball);
    }

    if (is_player && cannon->is_firing) {draw_fire_of_cannon(ctx, cannon, cannon->base_x + OFFSET_X_FROM_FIRE_OF_CANON, 
    cannon->base_y + OFFSET_Y_FROM_FIRE_OF_CANON);}   
    // Ствол
    SDL_Rect barrel_rect = {
        .x = cannon->base_x,
        .y = cannon->base_y,  
        .w = WIDTH_CANON_TEXTURE,
        .h = HEIGHT_CANON_TEXTURE
    };
    
    SDL_Point pivot = {
        .x = cannon->barrel_pivot_x,
        .y = cannon->barrel_pivot_y
    };

    SDL_RenderCopyEx(
        ctx->renderer, 
        cannon->barrel_texture, 
        NULL, 
        &barrel_rect, 
        cannon->current_angle,    
        &pivot, 
        SDL_FLIP_NONE
    );
}

void draw_fire_of_cannon(const GraphicsContext* ctx, const Cannon* cannon, int base_x, int base_y) {
    
    SDL_SetTextureAlphaMod(cannon->canon_fire_texture, cannon->current_alpha);

    SDL_Rect fire = {
        .x = base_x,
        .y = base_y,  
        .w = WIDTH_PLAYER_FIRE_CANON_TEXTURE,
        .h = HEIGHT_PLAYER_FIRE_CANON_TEXTURE
    };

    SDL_Point pivot = {
        .x = PIVOT_X_FROM_FIRE_OF_CANON,
        .y = PIVOT_Y_FROM_FIRE_OF_CANON
    };
    
    SDL_RenderCopyEx(
        ctx->renderer, 
        cannon->canon_fire_texture, 
        NULL, 
        &fire, 
        cannon->current_angle,    
        &pivot, 
        SDL_FLIP_NONE
    );
}

void draw_cannonball(const GraphicsContext* ctx, const Cannonball* core) {
    if (!core->is_active) return;
    
    SDL_Rect core_rect = {
        .x = core->current_x - 5, // центрируем (10/2)
        .y = core->current_y - 5, // центрируем (11/2) 
        .w = 10,
        .h = 11
    };
    
    SDL_Point pivot = {5, 5}; // центр вращения
    
    SDL_RenderCopyEx(
        ctx->renderer,
        core->texture, 
        NULL,
        &core_rect,
        core->rotation_angle,
        &pivot,
        SDL_FLIP_NONE
    );
}