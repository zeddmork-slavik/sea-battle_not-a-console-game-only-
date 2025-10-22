#ifndef CANNONS_H
#define CANNONS_H

typedef struct GraphicsContext GraphicsContext;
typedef struct Cannon Cannon;
typedef struct Cannonball Cannonball;



void draw_island(const GraphicsContext* ctx, int base_x, int base_y, char is_player);
void draw_cannon(const GraphicsContext* ctx, const Cannon* cannon, char is_player, const Cannonball* cannonball);
void draw_fire_of_cannon(const GraphicsContext* ctx, const Cannon* cannon, int base_x, int base_y);
void draw_cannonball(const GraphicsContext* ctx, const Cannonball* core);

#endif