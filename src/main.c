#include "raylib.h"
#include <stdio.h>
#include "player.h"

#ifndef WIDTH
#define WIDTH 500
#endif
#ifndef HEIGHT
#define HEIGHT 500
#endif
int main(){
    InitWindow(500, 500, "hello, world"); 
    if(IsWindowReady()){
        printf("Window is ready\n"); 
    }
    SetTargetFPS(60); 

    Player *player = player_create(); 
    player_scale(player, (Vector2){50,50}); 
    player_set_position(player, (Vector2){10,10}); 

    Color backgroundColor = {0,0,0,255}; 
    float time = 0; 
    while(!WindowShouldClose()){
        time += .01; 
        ClearBackground(backgroundColor); 
        BeginDrawing(); 
        player_set_angle_rads(player, time); 
        player_render(player, (Color){255,255,255,255});  
        EndDrawing(); 
    }
    CloseWindow(); 
    return 0; 
}