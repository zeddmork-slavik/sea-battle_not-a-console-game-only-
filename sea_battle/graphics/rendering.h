#ifndef RENDERING_H
#define RENDERING_H

typedef struct GraphicsContext GraphicsContext;
typedef struct GameState GameState;
typedef struct GameLandmarks GameLandmarks;
typedef struct GameAudio GameAudio;


void compose_frame(GameState* game, double delta_time, Uint32 current_time, 
        const GraphicsContext* ctx, const GameLandmarks* landmarks, GameAudio* audio);
void draw_all_elements(GameState* game, Uint32 current_time, const GraphicsContext* ctx, 
        const GameLandmarks* landmarks);

#endif