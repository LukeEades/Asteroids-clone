#include "asteroid.h"
#include <stdlib.h>
#include <string.h>

Vector2 asteroidVertices[] = {(Vector2){0,1}, (Vector2){1,2}, (Vector2){2,1}, (Vector2){1.5, 0}, (Vector2){2, -1}, (Vector2){0,-2}, (Vector2){-1, -2}, (Vector2){-2, -1}, (Vector2){-2, 1}, (Vector2){-1, 2}}; 

Asteroid *asteroid_create(){
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid) + sizeof(asteroidVertices)); 
    memcpy(asteroid->vertices, asteroidVertices, sizeof(asteroidVertices)); 
    asteroid->position = (Vector2){0,0};
    asteroid->velocity = (Vector2){1,0}; 
    asteroid->scale = (Vector2){1,1}; 
    asteroid->acceleration = (Vector2){0,0}; 
    asteroid->type = BIG; 
    asteroid->speed = 50; 
    return asteroid; 
}

void asteroid_render(Asteroid *asteroid, Color color){
    Vector2 vertices[sizeof(asteroidVertices)];
    memcpy(vertices, asteroid->vertices, sizeof(asteroidVertices)); 
    int i; 
    for(i = 0; i < sizeof(asteroidVertices)/sizeof(Vector2); i++){ 
        int j = i + 1; 
        if(j == sizeof(asteroidVertices)/sizeof(Vector2)){
            j = 0; 
        }
        Vector2 a = asteroid_to_screen(asteroid, vertices[i]); 
        Vector2 b = asteroid_to_screen(asteroid, vertices[j]); 
        DrawLine(a.x, a.y, b.x, b.y, color); 
    }

}

void asteroid_scale_set(Asteroid *asteroid, Vector2 scale){
    asteroid->scale.x *= scale.x; 
    asteroid->scale.y *= scale.y; 
}

Vector2 asteroid_to_screen(Asteroid *asteroid, Vector2 vec){
    Vector2 vecScale = {vec.x * asteroid->scale.x, vec.y * asteroid->scale.y}; 
    Vector2 vecRotate = {vecScale.x, vecScale.y}; 
    Vector2 vecWorld = {vecRotate.x + asteroid->position.x, vecRotate.y + asteroid->position.y}; 
    Vector2 vecScreen = {vecWorld.x + WIDTH/2, HEIGHT/2 - vecWorld.y}; 
    return vecScreen; 
}

Vector2 vec_scaled(Vector2 vec, float scale){
    return (Vector2){vec.x * scale, vec.y * scale}; 
}

void asteroid_update(Asteroid *asteroid, double dt){
    Vector2 velocity = asteroid->velocity; 
    velocity = vec_scaled(velocity, asteroid->speed); 
    Vector2 newPos = {velocity.x * dt + asteroid->position.x, velocity.y * dt + asteroid->position.y}; 
    newPos = asteroid_check_wrap(asteroid, newPos); 
    asteroid_position_set(asteroid, newPos); 
}


void asteroid_position_set(Asteroid *asteroid, Vector2 pos){
    asteroid->position.x = pos.x; 
    asteroid->position.y = pos.y; 
}

void asteroid_velocity_set(Asteroid *asteroid, Vector2 vel){
    asteroid->velocity.x = vel.x; 
    asteroid->velocity.y = vel.y; 
}

void asteroid_acceleration_set(Asteroid *asteroid, Vector2 acc){
    asteroid->acceleration.x = acc.x; 
    asteroid->acceleration.y = acc.y; 
}

Vector2 asteroid_check_wrap(Asteroid *asteroid, Vector2 newPos){
    Vector2 result = newPos; 
    if(newPos.x > WIDTH/2){
        result.x = -WIDTH/2; 
    }else if(newPos.x < -WIDTH/2){
        result.x = WIDTH/2; 
    }
    if(newPos.y > WIDTH/2){
        result.y = -WIDTH/2; 
    }else if(newPos.y < -WIDTH/2){
        result.y = WIDTH/2; 
    }
    return result; 
}