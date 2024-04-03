#include "asteroid.h"
#include <stdlib.h>
#include <string.h>
#include "player.h"

Vector2 asteroidVertices[11] = {(Vector2){0,1}, (Vector2){1,2}, (Vector2){2,1}, (Vector2){1.5, 0}, (Vector2){2, -1}, (Vector2){0,-2}, (Vector2){-1, -2}, (Vector2){-2, -1}, (Vector2){-2, 1}, (Vector2){-1, 2}, (Vector2){0,1}}; 
Vector2 asteroid2[13] = {(Vector2){1,.5}, (Vector2){2,1}, (Vector2){1,2}, (Vector2){0, 1.5}, (Vector2){-1, 2}, (Vector2){-2,1}, (Vector2){-1.5, 0}, (Vector2){-2, -1}, (Vector2){-1, -2}, (Vector2){-.5, -1.5}, (Vector2){1, -2}, (Vector2){2, -.5}, (Vector2){1,.5}};
Vector2 (*asteroids)[sizeof(asteroidVertices)/sizeof(Vector2)] = {&asteroidVertices, &asteroid2}; 
// Line bigRocks[] = {(Line){(Vector2){0,1}, (Vector2){1,2}}, (Line){(Vector2){1,2}, (Vector2){2,1}}, (Line){(Vector2){2,1}, (Vector2){1.5, 0}}, (Line){(Vector2){1.5, 0}, (Vector2){2, -1}}, (Line){(Vector2){2,-1}, (Vector2){0,-2}}, (Line){(Vector2){0, -2}, (Vector2){-1,-2}}, (Line){(Vector2){-1, -2}, (Vector2){-2, -1}}, (Line){(Vector2){-2, -1}, (Vector2){-2,1}}, (Line){(Vector2){-2, 1}, (Vector2){-1, 2}}, (Line){(Vector2){-1, 2}, (Vector2){0,1}}}; 
int asteroidModelVertLens[4] = {11,13, 13}; 
Asteroid *asteroid_create(asteroidType type){
    int modelNum = 0; 
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid) + asteroidModelVertLens[modelNum] * sizeof(Vector2)); 
    asteroid->position = (Vector2){0,0};
    asteroid->velocity = (Vector2){0,0}; 
    asteroid->type = type; 
    if(type == BIG){
        asteroid->scale = (Vector2){25,25}; 

    }else if(type == MEDIUM){
        asteroid->scale = (Vector2){15, 15}; 
    }else if(type == SMALL){
        asteroid->scale = (Vector2){5,5}; 
    }
    asteroid->acceleration = (Vector2){0,0}; 
    asteroid->speed = 50; 
    asteroid->modelNum = modelNum; 
    asteroid->numVerts = asteroidModelVertLens[modelNum]; 
    memcpy(asteroid->vertices, asteroids[asteroid->modelNum], asteroid->numVerts * sizeof(Vector2)); 
    return asteroid; 
}

void asteroid_render(Asteroid *asteroid, Color color){
    Vector2 vertices[asteroid->numVerts];
    memcpy(vertices, asteroids[asteroid->modelNum], asteroid->numVerts * sizeof(Vector2)); 
    int i;
    for(i = 1; i < asteroid->numVerts; i++){ 
        // printf("(%f, %f) - (%f, %f)\n", asteroid->vertices[i - 1].x, asteroid->vertices[i - 1].y, asteroid->vertices[i].x, asteroid->vertices[i].y); 
        int j = i -1; 
        Vector2 a = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, vertices[i])); 
        Vector2 b = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, vertices[j])); 
        DrawLine(a.x, a.y, b.x, b.y, color); 
    }
}

void asteroid_scale_set(Asteroid *asteroid, Vector2 scale){
    asteroid->scale.x *= scale.x; 
    asteroid->scale.y *= scale.y; 
}

Vector2 asteroid_to_world(Asteroid *asteroid, Vector2 vec){
    Vector2 vecScale = {vec.x * asteroid->scale.x, vec.y * asteroid->scale.y}; 
    Vector2 vecRotate = {vecScale.x, vecScale.y}; 
    Vector2 vecWorld = {vecRotate.x + asteroid->position.x, vecRotate.y + asteroid->position.y}; 
    return vecWorld; 
}

Vector2 asteroid_to_screen(Asteroid *asteroid, Vector2 vecWorld){
    return (Vector2){vecWorld.x + WIDTH/2, HEIGHT/2 - vecWorld.y}; 
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

bool line_intersects(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2){
    float x1 = a1.x; 
    float y1 = a1.y; 
    float x2 = a2.x; 
    float y2 = a2.y; 
    float x3 = b1.x; 
    float y3 = b1.y; 
    float x4 = b2.x; 
    float y4 = b2.y; 

    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4); 
    float tNum = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4); 
    float uNum = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)); 
    float u; 
    float t; 
    // Vector2 result; 
    if(den){
        u = uNum/den; 
        t = tNum/den; 
    }

    if(t >= 0 && t <= 1 && u >= 0 && u <= 1){
        // float px = x1 + t * (x2 - x1); 
        // float py = y1 + t * (y2 - y1); 
        // result.x = px; 
        // result.y = py; 
        return true; 
    }
    return false; 
}

bool asteroid_check_collide_player(Asteroid *asteroid, Player *player){
    int i;
    for(i = 1; i < asteroid->numVerts; i++){
        Vector2 a = asteroid_to_world(asteroid, asteroid->vertices[i - 1]); 
        Vector2 b = asteroid_to_world(asteroid, asteroid->vertices[i]); 
        if(line_intersects(a, b, local_to_world(player, player->vertices[0]), local_to_world(player, player->vertices[1])) || line_intersects(a, b, local_to_world(player, player->vertices[0]), local_to_world(player, player->vertices[2]))){
            return true; 
        }
    }
    return false; 
}

void asteroid_delete(Asteroid *asteroid){
    free(asteroid); 
}

void asteroid_destroy(Asteroid *asteroid, AsteroidList *list){
    // do particles
    // spawn new asteroids
    if(asteroid->type == BIG){
        int numNew = 2; 
        // for(int i = 0; i < numNew; i++){
        //     asteroid_create()
        //     asteroid_list_add(list, )
        // }
    }else if(asteroid->type == MEDIUM){
        int numNew = 3; 
    }else if(asteroid->type == SMALL){
        int numNew = 0; 
    }
    asteroid_list_remove(list, asteroid); 

}

AsteroidList *asteroid_list_create(int capacity){
    AsteroidList *list = (AsteroidList *)malloc(sizeof(AsteroidList));
    list->capacity = capacity; 
    list->length = 0;
    list->data = (Asteroid**)malloc(sizeof(Asteroid*) * list->capacity); 
    return list;
}

void asteroid_list_add(AsteroidList *list, Asteroid *asteroid){
    if(list->length + 1 >= list->capacity){
        list->capacity *= 2; 
        list->data = (Asteroid **)realloc(list->data, sizeof(Asteroid*) * list->capacity); 
    }
    list->data[list->length++] = asteroid; 
}

void asteroid_list_remove(AsteroidList *list, Asteroid *asteroid){
    for(int i = 0; i < list->length; i++){
        if(list->data[i] == asteroid){
            while(i + 1 < list->length){
                list->data[i] = list->data[i +1]; 
                i++; 
            }
            list->length--; 
            asteroid_delete(asteroid); 
        }
    }
}

