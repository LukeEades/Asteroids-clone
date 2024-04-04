#include "raylib.h"
#include <stdio.h>
#include "player.h"
// #include "asteroid.h"
#include <time.h>

#ifndef WIDTH
#define WIDTH 1000
#endif
#ifndef HEIGHT
#define HEIGHT 1000
#endif
int main(){
    InitWindow(WIDTH, HEIGHT, "hello, world"); 
    if(IsWindowReady()){
        printf("Window is ready\n"); 
    }
    SetTargetFPS(60); 

    Player *player = player_create(); 
    player_scale(player, (Vector2){30,30}); 
    player_position_set(player, (Vector2){100,100}); 
    int numAsteroids = 5; 
    AsteroidList *list = asteroid_list_create(numAsteroids); 

    srandom((unsigned int)clock()); 
    for(int i = 0; i < 5; i++){
        Asteroid * asteroid = asteroid_create(BIG); 
        asteroid_position_set(asteroid, (Vector2){((double)random()/RAND_MAX * 2 -1) * WIDTH/2, ((double)random()/RAND_MAX * 2 -1) * HEIGHT/2}); 
        asteroid_velocity_set(asteroid, (Vector2){(double)random()/RAND_MAX * 2 -1, (double)random()/RAND_MAX * 2 -1}); 
        asteroid_list_add(list, asteroid); 
    }
    Color backgroundColor = {0,0,0,255}; 
    double curr = (double)clock(); 
    double last = curr; 
    double dt = 0; 
    double timerLen = 0; 
    double timer = timerLen; 
    while(!WindowShouldClose()){
        curr = (double)clock();  
        dt = GetFrameTime(); 
        last = curr; 

        timer -= dt; 
        if(timer < 0){
            timer = 0; 
            if(player->dead){
                bool canSpawn = true; 
                for(int i = 0; i < list->length; i++){
                    int distance = 60; 
                    if(list->data[i]->position.x < distance && list->data[i]->position.x > -distance && list->data[i]->position.y < distance && list->data[i]->position.y > -distance){
                        canSpawn = false; 
                    }
                }
                if(canSpawn){
                    player->dead = false; 
                    player_reset(player); 
                }
            }
        }

        ClearBackground(backgroundColor); 
        BeginDrawing(); 
        for(int i = 0; i < list->length; i++){
            for(int j = 0; j < player->bullets->length; j++){
                if(bullet_collide_asteroid(player->bullets->bullets[j], list->data[i])){
                    asteroid_destroy(list->data[i], list); 
                    player_score_add(player, list->data[i]->type); 
                    bulletlist_remove(player->bullets, player->bullets->bullets[j]); 

                }
            }
        }
        for(int i = 0; i < list->length; i++){
            asteroid_update(list->data[i], dt);
            if(asteroid_check_collide_player(list->data[i], player) && !player->dead){
                timer = player_destroy(player);
                player_score_add(player, list->data[i]->type); 
                asteroid_destroy(list->data[i], list);
                // asteroid_list_remove(list, list->data[i]); 
                player->friction = -.05;
            }
            asteroid_render(list->data[i], (Color){255,255,255,255});
        }
        player_update(player, dt); 
        player_render(player, (Color){255,255,255,255});  
        EndDrawing(); 
    }
    player_delete(player); 
    CloseWindow(); 
    return 0; 
}