#include "menu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "audio.h"
#include "game_state.h"
#include "graphics/effects.h"
#include "graphics/ships.h"

void handle_menu_input(GameState* game, SDL_Event* event, const GraphicsContext* ctx, GameAudio* audio) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                if (game->menu_selection > 0) {
                    game->menu_selection--;
                } else {
                    game->menu_selection = 3;
                }
                break;
            case SDLK_DOWN:
                if (game->menu_selection < 3) {
                    game->menu_selection++;
                } else {
                    game->menu_selection = 0;
                }
                break;
            case SDLK_RETURN:  // enter
                switch (game->menu_selection) {
                    case 0:
                        game->game_state = STATE_PLAYING;  // "START GAME"
                        break;
                    case 1:
                        if (game->background_music_on) {
                            fade_out_background_music(DELAY_FIRE_CANON_OR_TIME_BAKGROUND_MUSIC_FADED);
                            game->background_music_on = false;
                        } else {
                            play_background(audio);
                            game->background_music_on = true;
                        }
                        break;
                    case 2:
                        if (game->all_sound_on) {
                            fade_out_background_music(DELAY_FIRE_CANON_OR_TIME_BAKGROUND_MUSIC_FADED);
                            game->background_music_on = false;
                            game->all_sound_on = false;
                        } else {
                            game->all_sound_on = true;
                            game->background_music_on = true;
                        }
                        break;
                    case 3:
                        game->running = DONT_RUNNING;  // "EXIT"
                        break;
                }
                break;
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        int mouse_x = event->button.x;  // 🆕 получаем X координату
        int mouse_y = event->button.y;

        if (mouse_x >= 0 && mouse_x <= 170 && mouse_y >= 0 && mouse_y <= 60) {
            game->game_state = STATE_PLAYING;
        } else if (mouse_x >= 125 && mouse_x <= 255 && mouse_y >= 650 && mouse_y <= 690) {
            game->running = DONT_RUNNING;
        } else if (mouse_x >= 229 && mouse_x <= 328 && mouse_y >= 490 && mouse_y <= 554) {
            if (game->background_music_on) {
                fade_out_background_music(DELAY_FIRE_CANON_OR_TIME_BAKGROUND_MUSIC_FADED);
                game->background_music_on = false;
            } else {
                play_background(audio);
                game->background_music_on = true;
            }
        } else if (mouse_x >= 229 && mouse_x <= 328 && mouse_y >= 574 && mouse_y <= 638) {
            if (game->all_sound_on) {
                fade_out_background_music(DELAY_FIRE_CANON_OR_TIME_BAKGROUND_MUSIC_FADED);
                game->background_music_on = false;
                game->all_sound_on = false;
            } else {
                game->all_sound_on = true;
                play_background(audio);
                game->background_music_on = true;
            }
        }
    }
}

void render_main_menu(const GraphicsContext* ctx, const GameState* game, GameAudio* audio) {
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
    render_text(ctx, "START GAME", 10, 10, new_game_color, 2);

    form_vertical_text(ctx, white);

    draw_ship(ctx, 100, 170, 0, 0, 0, 4, ctx->ship_jup_4p);
    draw_ship(ctx, 158, 250, 0, 0, 0, 3, ctx->ship_jup_3p);
    draw_ship(ctx, 212, 330, 0, 0, 0, 2, ctx->ship_jup_2p);
    draw_ship(ctx, 265, 410, 0, 0, 0, 1, ctx->ship_jup_1p);

    create_sound_control(ctx, game, white, yellow);

    SDL_Color exit_color = (game->menu_selection == 3) ? yellow : white;
    SDL_Rect exit_rect = {125, 650, 130, 40};
    SDL_SetRenderDrawColor(ctx->renderer, exit_color.r, exit_color.g, exit_color.b, 255);
    render_text(ctx, "EXIT", 125, 650, exit_color, 1);

    SDL_RenderPresent(ctx->renderer);
}

void form_vertical_text(const GraphicsContext* ctx, SDL_Color white) {
    SDL_Rect p_rect = {45, 65, 14, 38};
    SDL_Rect l_rect = {45, 94, 14, 38};
    SDL_Rect a_rect = {45, 109, 14, 38};
    SDL_Rect n_rect = {45, 126, 14, 38};

    SDL_Rect y_rect = {345, 74, 14, 38};
    SDL_Rect e_rect = {345, 101, 14, 38};
    SDL_Rect t_rect = {345, 126, 14, 38};

    render_single_char(ctx, 'p', p_rect.x, p_rect.y, white);
    render_single_char(ctx, 'l', l_rect.x, l_rect.y, white);
    render_single_char(ctx, 'a', a_rect.x, a_rect.y, white);
    render_single_char(ctx, 'n', n_rect.x, n_rect.y, white);
    render_single_char(ctx, 'y', y_rect.x, y_rect.y, white);
    render_single_char(ctx, 'e', e_rect.x, e_rect.y, white);
    render_single_char(ctx, 't', t_rect.x, t_rect.y, white);
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
            SDL_RenderCopy(ctx->renderer, texture, NULL,
                           &rect);  // типа внести на холст но нужно подумать
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

void render_single_char(const GraphicsContext* ctx, char ch, int x, int y, SDL_Color color) {
    if (!ctx->menu_small_font) return;

    char single_char[2] = {ch, '\0'};
    SDL_Surface* surface = TTF_RenderText_Blended(ctx->menu_small_font, single_char, color);

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

void create_sound_control(const GraphicsContext* ctx, const GameState* game, SDL_Color white,
                          SDL_Color yellow) {
    SDL_SetRenderDrawColor(ctx->renderer, white.r, white.g, white.b, 255);
    render_text(ctx, "song of the sea", 25, 500, white, 1);

    SDL_Rect place_for_backgroun_buttom;
    place_for_backgroun_buttom = (SDL_Rect){.x = 229, .y = 490, .w = 99, .h = 64};
    if (game->menu_selection == 1) {
        SDL_SetRenderDrawColor(ctx->renderer, yellow.r, yellow.g, yellow.b, 255);
        SDL_RenderDrawRect(ctx->renderer, &place_for_backgroun_buttom);
    }
    SDL_RenderCopy(ctx->renderer, game->background_music_on ? ctx->button_on : ctx->button_off, NULL,
                   &place_for_backgroun_buttom);

    SDL_SetRenderDrawColor(ctx->renderer, white.r, white.g, white.b, 255);
    render_text(ctx, "all sounds", 75, 590, white, 1);
    SDL_Rect place_for_all_sound_buttom;
    place_for_all_sound_buttom = (SDL_Rect){.x = 229, .y = 574, .w = 99, .h = 64};
    if (game->menu_selection == 2) {
        SDL_SetRenderDrawColor(ctx->renderer, yellow.r, yellow.g, yellow.b, 255);
        SDL_RenderDrawRect(ctx->renderer, &place_for_all_sound_buttom);
    }
    SDL_RenderCopy(ctx->renderer, game->all_sound_on ? ctx->button_on : ctx->button_off, NULL,
                   &place_for_all_sound_buttom);
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