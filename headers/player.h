#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <stdbool.h>


#ifndef WIDTH
#define WIDTH 1500
#endif
#ifndef HEIGHT
#define HEIGHT 1000
#endif


typedef struct Line{
    Vector2 a; 
    Vector2 b; 
    Vector2 velocity; 
} Line;

typedef struct Bullet{
    float timer; 
    float angle; 
    float speed;  
    Vector2 velocity;
    Vector2 position; 
}Bullet; 

typedef struct BulletList{
    int length; 
    int capacity; 
    Bullet **bullets; 
}BulletList; 

typedef struct Player{
    int numLives; 
    Vector2 position; 
    Vector2 velocity; 
    Vector2 acceleration; 
    Vector2 vertices[6];
    Vector2 scale; 
    int score; 
    float friction; 
    float angle; 
    int numLines; 
    bool dead; 
    BulletList *bullets; 
    Line lines[]; 
} Player; 

typedef enum asteroidType{
    BIG = 20, 
    MEDIUM = 50,
    SMALL = 100
} asteroidType;
typedef struct Asteroid{
    Vector2 scale; 
    Vector2 position; 
    Vector2 velocity; 
    Vector2 acceleration; 
    asteroidType type;
    int modelNum;  
    float speed; 
    int numVerts; 
    Vector2 vertices[]; 
} Asteroid; 

typedef struct AsteroidList{
    int length; 
    int capacity; 
    Asteroid **data;
}AsteroidList; 

typedef enum saucerType{
    BIGSAUCE = 200,
    SMALLSAUCE = 1000,
} saucerType; 

typedef struct Saucer{
    Vector2 position; 
    Vector2 velocity; 
    float moveTimer; 
    float moveTimerLim; 
    float speed; 
    float shootTimer; 
    float shootTimerLim; 
    saucerType type; 
    float scale; 
    float angle; 
    int numVerts; 
    BulletList *list; 
    Vector2 vertices[]; 
}Saucer; 


Saucer *saucer_create(saucerType type); 
void saucer_render(Saucer *saucer, Color color); 
void saucer_update(Saucer *saucer, Player *player, double dt); 
Vector2 saucer_to_world(Saucer *saucer, Vector2 vec); 
Vector2 saucer_to_screen(Saucer *saucer, Vector2 vec); 
Vector2 saucer_check_wrap(Saucer *saucer, Vector2 vec); 
void saucer_delete(Saucer *saucer); 
bool saucer_collides_bullet(Saucer *saucer, Bullet *bullet);
float angle_a_to_b(Vector2 a, Vector2 b); 

Asteroid *asteroid_create(asteroidType type); 
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
bool lines_intersect(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2); 
bool asteroid_check_collide_player(Asteroid *asteroid, Player *player); 
void asteroid_delete(Asteroid *asteroid); 
void asteroid_destroy(Asteroid *asteroid, AsteroidList *list);
AsteroidList *asteroid_list_create(int capacity);
void asteroid_list_add(AsteroidList *list, Asteroid *asteroid); 
void asteroid_list_remove(AsteroidList *list, Asteroid *asteroid); 
void asteroid_list_delete(AsteroidList *list); 


Bullet* bullet_create(Vector2 position, float angle); 
void bullet_update(Bullet *bullet, double dt); 
void bullet_render(Bullet *bullet, Color color); 
void bullet_delete(Bullet *bullet); 
void bullet_destroy(Bullet *bullet); 
void bullet_check_wrap(Bullet *bullet);
bool bullet_collide_asteroid(Bullet *bullet, Asteroid *asteroid);  

BulletList *bulletlist_create(); 
void bulletlist_append(BulletList *list, Bullet *bullet); 
void bulletlist_remove(BulletList *list, Bullet *bullet); 


Line *line_create(Vector2 a, Vector2 b); 
void line_render(Line *line, Color color); 
void line_update(Line *line, double dt);
void line_delete(Line *line); 
void line_set_velocity(Line *line, Vector2 vel, float angle); 
void line_position_set(Line *line, Vector2 a, Vector2 b); 

Player *player_create(); 
void player_render(Player *player, Color color); 
void player_lose_life(Player *player); 
void player_velocity_set(Player *player, Vector2 velocity);
void player_acceleration_set(Player *player, Vector2 acceleration); 
void player_position_set(Player *player, Vector2 position);  
void player_scale(Player *player, Vector2 scale); 
Vector2 local_to_world(Player *player, Vector2 vec);
Vector2 player_world_to_screen(Player *player, Vector2 vecWorld); 
void player_angle_set_rads(Player *player, float angle); 
void player_angle_add_rads(Player *player, float angle); 
void player_update(Player *player, double dt);  
void player_acceleration_add(Player *player, Vector2 add); 
void player_get_input(Player *player, double dt); 
void player_velocity_add(Player *player, Vector2 add); 
Vector2 player_check_wrap(Player *player, Vector2 newPos); 
void player_delete(Player *player); 
double player_destroy(Player *player); 
void player_reset(Player *player); 
void player_score_add(Player *player, int score); 
bool player_collides_bullet(Player *player, Bullet *bullet); 


#endif