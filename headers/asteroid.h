#ifndef ASTEROID_H
#define ASTEROID_H
#include <stdio.h>
#include "raylib.h"
#include <math.h>

#ifndef WIDTH
#define WIDTH 1500
#endif
#ifndef HEIGHT
#define HEIGHT 1080
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
    Vector2 vertices[]; 
} Asteroid; 

Asteroid *asteroid_create(); 
void asteroid_render(Asteroid * asteroid, Color color); 
Vector2 asteroid_to_screen(Asteroid *asteroid, Vector2 vec); 
void asteroid_scale_set(Asteroid *asteroid, Vector2 scale); 
void asteroid_acceleration_set(Asteroid *asteroid, Vector2 acc); 
void asteroid_velocity_set(Asteroid *asteroid, Vector2 vel); 
void asteroid_position_set(Asteroid *asteroid, Vector2 pos); 
Vector2 asteroid_check_wrap(Asteroid *asteroid, Vector2 newPos); 
void asteroid_update(Asteroid *asteroid, double dt); 
Vector2 vec_scaled(Vector2 vec, float scale); 

#endif