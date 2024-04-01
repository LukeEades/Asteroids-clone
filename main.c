#include "./headers/raylib.h"
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500
int main(){


    InitWindow(500, 500, "hello, world"); 
    if(IsWindowReady()){
        printf("Window is ready\n"); 
    }
    SetTargetFPS(60); 
    Color color = {255,0,0,255}; 
    while(!WindowShouldClose()){
        ClearBackground(color); 
        BeginDrawing(); 
        EndDrawing(); 
    }
    CloseWindow(); 
    return 0; 
}