#include "menu.h"

#include <SDL2/SDL.h>

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

    // Белый цвет для текста
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);

    // Заголовок (прямоугольник)
    SDL_Rect title_rect = {ctx->width_of_window / 2 - 150, 100, 300, 60};
    SDL_RenderDrawRect(ctx->renderer, &title_rect);

    // Кнопка "Новая игра"
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    if (game->menu_selection == 0) {
        SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 0, 255);  // Желтый если выбрана
    }
    SDL_Rect new_game_rect = {ctx->width_of_window / 2 - 100, 250, 200, 40};
    SDL_RenderDrawRect(ctx->renderer, &new_game_rect);

    // Кнопка "Выход"
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    if (game->menu_selection == 1) {
        SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 0, 255);  // Желтый если выбрана
    }
    SDL_Rect exit_rect = {ctx->width_of_window / 2 - 100, 310, 200, 40};
    SDL_RenderDrawRect(ctx->renderer, &exit_rect);

    SDL_RenderPresent(ctx->renderer);  // ⭐ ПОКАЗАТЬ НА ЭКРАНЕ!
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