#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef WIDTH
#define WIDTH 500
#endif
#ifndef HEIGHT
#define HEIGHT 500
#endif

typedef struct Player{
    int numLives; 
    Vector2 position; 
    Vector2 velocity; 
    Vector2 acceleration; 
    Vector2 vertices[3];
    Vector2 scale; 
    float angle; 
} Player; 

Player *player_create(); 
void player_render(Player *player, Color color); 
void player_lose_life(Player *player); 
void player_set_velocity(Player *player, Vector2 velocity);
void player_set_acceleration(Player *player, Vector2 acceleration); 
void player_set_position(Player *player, Vector2 position);  
void player_scale(Player *player, Vector2 scale); 
Vector2 local_to_world(Player *player, Vector2 vec);
void player_set_angle_rads(Player *player, float angle);  


#endif