#include "ships.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core/game_state.h"

void draw_ship(const GraphicsContext* ctx, int base_x, int base_y, char grid_x, char grid_y, char direction,
               char deck_count, SDL_Texture* texture) {
    double angle = (direction == 1) ? 90.0 : 0.0;
    int ship_width, ship_height;
    SDL_QueryTexture(texture, NULL, NULL, &ship_width, &ship_height);

    SDL_Rect place_for_ship;

    if (direction == 0) {  // горизонтальный
        place_for_ship = (SDL_Rect){
            .x = base_x + (int)grid_x * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) >> 1),
            .y = base_y + (int)grid_y * ctx->cell_size - ((ship_height - ctx->cell_size) >> 1),
            .w = ship_width,
            .h = ship_height};

    } else {  // вертикальный
        place_for_ship = (SDL_Rect){
            .x = base_x + X_CRUTCH_VERTICAL_SHIPS + (int)grid_x * ctx->cell_size -
                 ((ship_height - ctx->cell_size) >> 1),
            .y = base_y + (int)grid_y * ctx->cell_size - ((ship_width - deck_count * ctx->cell_size) >> 1),
            .w = ship_width,
            .h = ship_height};
    }

    // Ручная точка вращения - левый верхний угол
    SDL_Point pivot = {0, 0};
    SDL_RenderCopyEx(ctx->renderer, texture, NULL, &place_for_ship, angle, &pivot, SDL_FLIP_NONE);
}

void draw_all_fires(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks) {
    FireNode* current = game->active_fires;
    while (current != NULL) {
        // Выбор текстуры по текущему кадру
        SDL_Texture* fire_texture;
        switch (current->current_frame) {
            case 0:
                fire_texture = ctx->fire_ship0;
                break;
            case 1:
                fire_texture = ctx->fire_ship1;
                break;
            case 2:
                fire_texture = ctx->fire_ship2;
                break;
            case 3:
                fire_texture = ctx->fire_ship3;
                break;
            case 4:
                fire_texture = ctx->fire_ship4;
                break;
            case 5:
                fire_texture = ctx->fire_ship5;
                break;
            case 6:
                fire_texture = ctx->fire_ship6;
                break;
            case 7:
                fire_texture = ctx->fire_ship7;
                break;
        }

        // Расчёт координат и отрисовка
        SDL_Rect fire_rect = {
            .x = (game->current_turn == IS_COMPUTER ? landmarks->player_x : landmarks->computer_x) +
                 current->cell_x * ctx->cell_size,
            .y = landmarks->offset_y + current->cell_y * ctx->cell_size,
            .w = CELL_SIZE,
            .h = CELL_SIZE};
        SDL_RenderCopy(ctx->renderer, fire_texture, NULL, &fire_rect);

        current = current->next;
    }
}