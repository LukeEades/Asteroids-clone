#include "raylib.h"
#include <stdio.h>
#include "player.h"
#include "asteroid.h"
#include <time.h>

#ifndef WIDTH
#define WIDTH 1500
#endif
#ifndef HEIGHT
#define HEIGHT 1080
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

    Asteroid *asteroid = asteroid_create(); 
    asteroid_scale_set(asteroid, (Vector2){25,25}); 
    asteroid_velocity_set(asteroid, (Vector2){1,0}); 
    Color backgroundColor = {0,0,0,255}; 
    double curr = (double)clock(); 
    double last = curr; 
    double dt = 0; 
    double timerLen = 0; 
    double timer = timerLen; 
    while(!WindowShouldClose()){
        srand(time(0)); 
        curr = (double)clock();  
        dt = GetFrameTime(); 
        last = curr; 

        timer -= dt; 
        if(timer < 0){
            timer = 0; 
            if(player->dead){
                player->dead = false; 
                player_reset(player); 
                
            }
            // do somthing
        }

        ClearBackground(backgroundColor); 
        BeginDrawing(); 
        asteroid_update(asteroid, dt);
        if(asteroid_check_collide_player(asteroid, player) && !player->dead){
            timer = player_destroy(player); 
            player->friction = -.05; 
        }
        // asteroid_check_collide_player(asteroid, player); 
        asteroid_render(asteroid, (Color){255,255,255,255});
        player_update(player, dt); 
        player_render(player, (Color){255,255,255,255});  
        EndDrawing(); 
    }
    player_delete(player); 
    asteroid_delete(asteroid); 
    CloseWindow(); 
    return 0; 
}