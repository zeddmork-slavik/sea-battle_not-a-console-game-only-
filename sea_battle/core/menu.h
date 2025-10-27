#ifndef MENU_H
#define MENU_H

typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct GameState GameState;
typedef union SDL_Event SDL_Event;

void handle_menu_input(GameState* game, SDL_Event* event);
void render_main_menu(const GraphicsContext* ctx, const GameState* game);
void reset_game_for_new_match(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks);

#endif