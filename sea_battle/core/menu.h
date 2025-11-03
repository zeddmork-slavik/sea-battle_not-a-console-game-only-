#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct GameState GameState;
typedef struct GameAudio GameAudio;

void handle_menu_input(GameState* game, SDL_Event* event, const GraphicsContext* ctx, GameAudio* audio);
void render_main_menu(const GraphicsContext* ctx, const GameState* game, GameAudio* audio);
void form_vertical_text(const GraphicsContext* ctx, SDL_Color white);
void render_text(const GraphicsContext* ctx, const char* text, int x, int y, SDL_Color color, int size);
void render_single_char(const GraphicsContext* ctx, char ch, int x, int y, SDL_Color color);
void render_text_with_shadow(const GraphicsContext* ctx, const char* text, int x, int y,
                             SDL_Color text_color);
void create_sound_control(const GraphicsContext* ctx, const GameState* game, SDL_Color white,
                          SDL_Color yellow);
void reset_game_for_new_match(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks);

#endif