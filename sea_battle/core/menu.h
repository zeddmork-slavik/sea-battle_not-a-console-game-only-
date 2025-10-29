#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct GameState GameState;
typedef union SDL_Event SDL_Event;

void handle_menu_input(GameState* game, SDL_Event* event, const GraphicsContext* ctx);
void render_main_menu(const GraphicsContext* ctx, const GameState* game);
void render_text(const GraphicsContext* ctx, const char* text, int x, int y, SDL_Color color);
void reset_game_for_new_match(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks);

#endif