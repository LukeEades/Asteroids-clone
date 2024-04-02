#include "raylib.h"
#include <stdio.h>
#include "player.h"
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
    player_scale(player, (Vector2){50,50}); 
    player_position_set(player, (Vector2){10,10}); 

    Color backgroundColor = {0,0,0,255}; 
    double curr = (double)clock(); 
    double last = curr; 
    double dt = 0; 
    while(!WindowShouldClose()){
        curr = (double)clock();  
        dt = GetFrameTime(); 
        last = curr; 

        ClearBackground(backgroundColor); 
        BeginDrawing(); 
        player_update(player, dt); 
        player_render(player, (Color){255,255,255,255});  
        EndDrawing(); 
    }
    CloseWindow(); 
    return 0; 
}