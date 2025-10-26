#ifndef GAME_lOOP_H
#define GAME_lOOP_H

typedef struct GameAudio GameAudio;
typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct GameState GameState;

void run_game(const GraphicsContext* ctx, const GameLandmarks* landmarks);
void event_processing(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks,
                      GameAudio* audio);

#endif