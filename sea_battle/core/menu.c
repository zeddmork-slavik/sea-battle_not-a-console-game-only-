#include "menu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game_state.h"
#include "graphics/effects.h"

void handle_menu_input(GameState* game, SDL_Event* event, const GraphicsContext* ctx) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
                game->menu_selection = 1 - game->menu_selection;
                break;

            case SDLK_RETURN:  // enter
                if (game->menu_selection == 0) {
                    game->game_state = STATE_PLAYING;
                } else {
                    game->running = DONT_RUNNING;
                }
                break;
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int mouse_x = event->button.x;  // 🆕 получаем X координату
        int mouse_y = event->button.y;

        // 🆕 Проверяем попадание в прямоугольник "Новая игра"
        if (mouse_x >= ctx->width_of_window / 2 - 100 &&  // левая граница
            mouse_x <= ctx->width_of_window / 2 + 100 &&  // правая граница
            mouse_y >= 250 &&                             // верхняя граница
            mouse_y <= 290) {                             // нижняя граница
            game->game_state = STATE_PLAYING;
        }
        // 🆕 Проверяем попадание в прямоугольник "Выход"
        else if (mouse_x >= ctx->width_of_window / 2 - 100 && mouse_x <= ctx->width_of_window / 2 + 100 &&
                 mouse_y >= 310 && mouse_y <= 350) {
            game->running = DONT_RUNNING;
        }
    }
}

void render_main_menu(const GraphicsContext* ctx, const GameState* game) {
    // Черный фон
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color gray = {128, 128, 128, 255};

    draw_menu_bg(ctx);

    // КНОПКА "НОВАЯ ИГРА"
    SDL_Color new_game_color = (game->menu_selection == 0) ? yellow : white;
    SDL_Rect new_game_rect = {0, 0, 370, 74};
    SDL_SetRenderDrawColor(ctx->renderer, new_game_color.r, new_game_color.g, new_game_color.b, 255);
    SDL_RenderDrawRect(ctx->renderer, &new_game_rect);  // рисование прямоугольника по границам поля
    render_text(ctx, "NEW GAME", 10, 10, new_game_color, 2);

    // КНОПКА "ВЫХОД"
    SDL_Color exit_color = (game->menu_selection == 1) ? yellow : white;
    SDL_Rect exit_rect = {ctx->width_of_window / 2 - 60, 310, 130, 40};
    SDL_SetRenderDrawColor(ctx->renderer, exit_color.r, exit_color.g, exit_color.b, 255);
    SDL_RenderDrawRect(ctx->renderer, &exit_rect);
    render_text(ctx, "EXIT", ctx->width_of_window / 2 - 50, 310, exit_color, 1);

    // ПОДСКАЗКА
    render_text_with_shadow(ctx, "Use arrows and Enter", ctx->width_of_window / 2 - 120, 400, gray);
    SDL_RenderPresent(ctx->renderer);  // ⭐ ПОКАЗАТЬ НА ЭКРАНЕ!
}

void render_text(const GraphicsContext* ctx, const char* text, int x, int y, SDL_Color color, int size) {
    if (!ctx->menu_big_font) return;

    SDL_Surface* surface = TTF_RenderText_Solid((size == 1 ? ctx->menu_small_font : ctx->menu_big_font), text,
                                                color);  // сформировать текстуру в оперативке
    if (surface) {
        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(ctx->renderer, surface);  // передать текстуру видюхе
        if (texture) {
            SDL_Rect rect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(ctx->renderer, texture, NULL, &rect);  // типа внести на холст но нужно подумать
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

void render_text_with_shadow(const GraphicsContext* ctx, const char* text, int x, int y,
                             SDL_Color text_color) {
    if (!ctx->menu_small_font) return;

    // Тень (смещенная копия текста)
    SDL_Color shadow_color = {0, 0, 0, 180};  // Полупрозрачный черный
    SDL_Surface* shadow_surface = TTF_RenderText_Blended(ctx->menu_small_font, text, shadow_color);
    if (shadow_surface) {
        SDL_Texture* shadow_texture = SDL_CreateTextureFromSurface(ctx->renderer, shadow_surface);
        if (shadow_texture) {
            SDL_Rect shadow_rect = {x + 2, y + 2, shadow_surface->w, shadow_surface->h};  // Смещение
            SDL_RenderCopy(ctx->renderer, shadow_texture, NULL, &shadow_rect);
            SDL_DestroyTexture(shadow_texture);
        }
        SDL_FreeSurface(shadow_surface);
    }

    // Основной текст
    render_text(ctx, text, x, y, text_color, 1);
}

void reset_game_for_new_match(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks) {
    // Сбрасываем игровые поля
    init_board(game->player_board);
    init_board(game->computer_board);

    // Сбрасываем пушки
    init_cannon(&game->player_cannon, IS_PLAYER, landmarks->player_x + ctx->field_size + OFFSET_X_FROM_BOARD,
                landmarks->offset_y + OFFSET_Y_FROM_BOARD, ctx->renderer);

    init_cannon(&game->computer_cannon, IS_COMPUTER,
                landmarks->player_x + ctx->field_size + OFFSET_X_FROM_BOARD,
                landmarks->offset_y + OFFSET_Y_FROM_COMPUTER_CANON, ctx->renderer);

    // Сбрасываем ядро
    reset_cannonball(&game->cannonball);

    // Сбрасываем очередь хода
    game->current_turn = IS_PLAYER;  // тоже поменять на компа

    cleanup_fires(game);
}