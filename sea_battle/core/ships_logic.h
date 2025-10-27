#ifndef SHIPS_LOGIC_H
#define SHIPS_LOGIC_H

typedef struct GameAudio GameAudio;
typedef struct GraphicsContext GraphicsContext;
typedef struct GameState GameState;
typedef struct GameLandmarks GameLandmarks;
typedef unsigned int Uint32;

void process_shot_result(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks,
                         Uint32 current_time, GameAudio* audio);

#endif