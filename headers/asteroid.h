#ifndef ASTEROID_H
#define ASTEROID_H
#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include "player.h"
#include <stdbool.h>

#ifndef WIDTH
#define WIDTH 1000
#endif
#ifndef HEIGHT
#define HEIGHT 1000
#endif

enum asteroidType{
    BIG = 20, 
    MEDIUM = 50,
    SMALL = 100
};
typedef struct Asteroid{
    Vector2 scale; 
    Vector2 position; 
    Vector2 velocity; 
    Vector2 acceleration; 
    enum asteroidType type; 
    float speed; 
    int numVerts; 
    Vector2 vertices[]; 
} Asteroid; 

typedef struct AsteroidList{
    int length; 
    int capacity; 
    Asteroid **data;
}AsteroidList; 

Asteroid *asteroid_create(); 
void asteroid_render(Asteroid * asteroid, Color color); 
Vector2 asteroid_to_world(Asteroid *asteroid, Vector2 vec); 
Vector2 asteroid_to_screen(Asteroid *asteroid, Vector2 vecWorld); 
void asteroid_scale_set(Asteroid *asteroid, Vector2 scale); 
void asteroid_acceleration_set(Asteroid *asteroid, Vector2 acc); 
void asteroid_velocity_set(Asteroid *asteroid, Vector2 vel); 
void asteroid_position_set(Asteroid *asteroid, Vector2 pos); 
Vector2 asteroid_check_wrap(Asteroid *asteroid, Vector2 newPos); 
void asteroid_update(Asteroid *asteroid, double dt); 
Vector2 vec_scaled(Vector2 vec, float scale); 
Vector2 lines_intersects(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2); 
bool asteroid_check_collide_player(Asteroid *asteroid, Player *player); 
void asteroid_delete(Asteroid *asteroid); 
void asteroid_destroy(Asteroid *asteroid);

AsteroidList *asteroid_list_create(int capacity); 
void asteroid_list_add(AsteroidList *list, Asteroid *asteroid); 
void asteroid_list_remove(AsteroidList *list, Asteroid *asteroid); 


#endif