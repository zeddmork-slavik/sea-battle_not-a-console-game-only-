#ifndef EFFECTS_H
#define EFFECTS_H

typedef struct GraphicsContext GraphicsContext;
typedef struct GameState GameState;

void draw_spray(const GraphicsContext* ctx, GameState* game);
void draw_menu_bg(const GraphicsContext* ctx);

#endif