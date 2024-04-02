#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifndef WIDTH
#define WIDTH 1500
#endif
#ifndef HEIGHT
#define HEIGHT 1080
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
void player_velocity_set(Player *player, Vector2 velocity);
void player_acceleration_set(Player *player, Vector2 acceleration); 
void player_position_set(Player *player, Vector2 position);  
void player_scale(Player *player, Vector2 scale); 
Vector2 local_to_world(Player *player, Vector2 vec);
void player_angle_set_rads(Player *player, float angle); 
void player_angle_add_rads(Player *player, float angle); 
void player_update(Player *player, double dt);  
void player_acceleration_add(Player *player, Vector2 add); 
void player_get_input(Player *player, double dt); 
void player_velocity_add(Player *player, Vector2 add); 
Vector2 player_check_wrap(Player *player, Vector2 newPos); 
#endif