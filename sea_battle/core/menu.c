#include "menu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game_state.h"

void handle_menu_input(GameState* game, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
                game->menu_selection = 1 - game->menu_selection;
                break;

            case SDLK_RETURN:  // 13
                if (game->menu_selection == 0) {
                    game->game_state = STATE_PLAYING;
                } else {
                    game->running = DONT_RUNNING;
                }
                break;
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int mouse_y = event->button.y;

        if (mouse_y >= 250 && mouse_y <= 290) {
            game->game_state = STATE_PLAYING;
        } else if (mouse_y >= 310 && mouse_y <= 350) {
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

    // ЗАГОЛОВОК
    render_text(ctx, "SEA BATTLE", ctx->width_of_window / 2 - 80, 120, white);

    // КНОПКА "НОВАЯ ИГРА"
    SDL_Color new_game_color = (game->menu_selection == 0) ? yellow : white;
    SDL_Rect new_game_rect = {ctx->width_of_window / 2 - 125, 250, 225, 40};
    SDL_SetRenderDrawColor(ctx->renderer, new_game_color.r, new_game_color.g, new_game_color.b, 255);
    SDL_RenderDrawRect(ctx->renderer, &new_game_rect);
    render_text(ctx, "NEW GAME", ctx->width_of_window / 2 - 125, 250, new_game_color);

    // КНОПКА "ВЫХОД"
    SDL_Color exit_color = (game->menu_selection == 1) ? yellow : white;
    SDL_Rect exit_rect = {ctx->width_of_window / 2 - 60, 310, 130, 40};
    SDL_SetRenderDrawColor(ctx->renderer, exit_color.r, exit_color.g, exit_color.b, 255);
    SDL_RenderDrawRect(ctx->renderer, &exit_rect);
    render_text(ctx, "EXIT", ctx->width_of_window / 2 - 50, 310, exit_color);

    // ПОДСКАЗКА
    render_text(ctx, "Use arrows and Enter", ctx->width_of_window / 2 - 120, 400, gray);
    SDL_RenderPresent(ctx->renderer);  // ⭐ ПОКАЗАТЬ НА ЭКРАНЕ!
}

void render_text(const GraphicsContext* ctx, const char* text, int x, int y, SDL_Color color) {
    if (!ctx->menu_font) return;

    SDL_Surface* surface = TTF_RenderText_Solid(ctx->menu_font, text, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx->renderer, surface);
        if (texture) {
            SDL_Rect rect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(ctx->renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
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