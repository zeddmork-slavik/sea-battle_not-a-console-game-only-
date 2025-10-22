#ifndef CANNONS_LOGIC_H
#define CANNONS_LOGIC_H

#include <SDL2/SDL.h>

typedef struct Cannon Cannon;
typedef struct GraphicsContext GraphicsContext;
typedef struct GameLandmarks GameLandmarks;
typedef struct Cannonball Cannonball;
typedef struct GameState GameState;
typedef struct GameAudio GameAudio;

void aim_cannon_at(Cannon* cannon, int target_x, int target_y, 
    const GraphicsContext* ctx, const GameLandmarks* landmarks,
    Cannonball* cannonball);
void update_cannon_animation(Cannon* cannon, double delta_time);
void fire_cannonball(Cannonball* ball, const Cannon* cannon, Uint32 current_time);
void update_cannonball(const GraphicsContext* ctx, GameState* game, const GameLandmarks* landmarks, Uint32 current_time, GameAudio* audio);
#endif