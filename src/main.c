#include "raylib.h"
#include <stdio.h>
#include "player.h"
#include <time.h>

#ifndef WIDTH
#define WIDTH 1500
#endif
#ifndef HEIGHT
#define HEIGHT 1000
#endif

char screenState[32] = "start"; 
int main(){
    InitWindow(WIDTH, HEIGHT, "Asteroids"); 
    InitAudioDevice(); 
    if(IsWindowReady()){
        printf("Window is ready\n"); 
    }
    SetTargetFPS(60); 
    ChangeDirectory(GetApplicationDirectory());
    Player *player = player_create(); 
    player_scale(player, (Vector2){30,30}); 
    player_position_set(player, (Vector2){0,0}); 
    int numAsteroids = 6; 
    AsteroidList *list = asteroid_list_create(numAsteroids); 

    Sound explosion = LoadSound("./resources/explosion.wav"); 
    SetSoundVolume(explosion, 1.0); 
    Sound shoot = LoadSound("./resources/shoot.wav"); 
    SetSoundVolume(shoot, 1.0); 
    Sound collision = LoadSound("./resources/collision.wav"); 
    SetSoundVolume(collision, 1.0); 

    player_set_sound(player, shoot); 

    srandom((unsigned int)clock()); 
    Color backgroundColor = {0,0,0,255}; 
    Color fontColor = {255,255,255,255}; 
    double curr = (double)clock(); 
    double last = curr; 
    double dt = 0; 
    double timerLen = 0; 
    double timer = timerLen;
    int level = 0; 
    char levelText[32]; 
    Saucer **saucers = (Saucer**)malloc(sizeof(Saucer*));
    while(!WindowShouldClose()){
        if(!strcmp(screenState, "start")){
            BeginDrawing(); 
            char titleText[32] = "Asteroids";
            char titleSubText[32] = "Press space to begin"; 
            int titleFont = 50;
            ClearBackground(backgroundColor); 
            DrawText(titleText, WIDTH/2 - MeasureText(titleText, titleFont)/2, HEIGHT/2 - 50, titleFont, fontColor); 
            DrawText(titleSubText, WIDTH/2 - MeasureText(titleSubText, titleFont - 20)/2, HEIGHT/2, titleFont - 20, fontColor); 
            if(IsKeyPressed(32)){
                strcpy(screenState, "playing"); 
            }
            EndDrawing(); 
        }else if(!strcmp(screenState, "playing")){
            curr = (double)clock();
            dt = GetFrameTime();
            last = curr;
            
            timer -= dt; 
            if((int)((double)random()/RAND_MAX * 1100) == 100 && saucers[0] == NULL){
                saucers[0] = saucer_create(); 
                saucer_set_sound(saucers[0], shoot); 
                printf("my boy\n"); 
                saucers[0]->position = (Vector2){((double)random()/RAND_MAX * 2 -1) * WIDTH/2, ((double)random()/RAND_MAX * 2 -1) * HEIGHT/2}; 
                saucers[0]->velocity = (Vector2){(double)random()/RAND_MAX * 2 -1, (double)random()/RAND_MAX * 2 -1}; 
            }
            if(player->numLives <= 0){
                strcpy(screenState, "game over"); 
            }
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
                        player_reset(player); 
                    }
                }
            }
            if(list->length == 0 && !saucers[0]){
                level++; 
                if(level % 3 == 0){
                    numAsteroids++; 
                }
                for(int i = 0; i < numAsteroids; i++){
                    Asteroid * asteroid = asteroid_create(BIG); 
                    asteroid_position_set(asteroid, (Vector2){((double)random()/RAND_MAX * 2 -1) * WIDTH/2, ((double)random()/RAND_MAX * 2 -1) * HEIGHT/2}); 
                    asteroid_velocity_set(asteroid, (Vector2){(double)random()/RAND_MAX * 2 -1, (double)random()/RAND_MAX * 2 -1}); 
                    asteroid_list_add(list, asteroid); 
                }
            }
            ClearBackground(backgroundColor); 
            BeginDrawing(); 
            if(saucers[0]){
                saucer_update(saucers[0], player, dt); 
                saucer_render(saucers[0], (Color){255,255,255,255}); 
                for(int i = 0; i < player->bullets->length; i++){
                    if(saucer_collides_bullet(saucers[0], player->bullets->bullets[i])){
                        player_score_add(player, saucers[0]->type); 
                        saucer_delete(saucers[0]); 
                        PlaySound(explosion); 
                        saucers[0] = NULL; 
                        bulletlist_remove(player->bullets, player->bullets->bullets[i]); 
                        break; 
                    }   
                }
            if(saucers[0]){
                for(int i = 0; i < saucers[0]->list->length; i++){
                    if(player_collides_bullet(player, saucers[0]->list->bullets[i]) && !player->dead){
                        timer = player_destroy(player); 
                        player->friction = -.05; 
                        bulletlist_remove(saucers[0]->list, saucers[0]->list->bullets[i]); 
                        PlaySound(explosion); 
                    }
                }
            }
            }
            for(int i = 0; i < list->length; i++){
                for(int j = 0; j < player->bullets->length; j++){
                    if(bullet_collide_asteroid(player->bullets->bullets[j], list->data[i])){
                        asteroid_destroy(list->data[i], list); 
                        player_score_add(player, list->data[i]->type); 
                        if(player->score % 10000 < (player->score - list->data[i]->type) % 10000){
                            player->numLives++; 
                        }
                        PlaySound(explosion);
                        bulletlist_remove(player->bullets, player->bullets->bullets[j]); 
                    }
                }
            }
            for(int i = 0; i < list->length; i++){
                asteroid_update(list->data[i], dt);
                if(asteroid_check_collide_player(list->data[i], player) && !player->dead){
                    timer = player_destroy(player);
                    player_score_add(player, list->data[i]->type); 
                    if(player->score % 10000 < (player->score - list->data[i]->type) % 10000){
                        player->numLives++; 
                    }
                    PlaySound(collision); 
                    asteroid_destroy(list->data[i], list);
                    player->friction = -.05;
                    i--; 
                }
                asteroid_render(list->data[i], (Color){255,255,255,255});
            }
            player_update(player, dt); 
            player_render(player, (Color){255,255,255,255});  
            
            sprintf(levelText, "Level: %d", level);
            DrawText(levelText, WIDTH-200, 100, 20,(Color){255,255,255,255}); 
            EndDrawing(); 
        }else if(!strcmp(screenState, "game over")){
            BeginDrawing(); 
            char titleText[32] = "Game Over";
            char titleSubText[32] = "Press space to restart"; 
            int titleFont = 50;     
            ClearBackground(backgroundColor); 
            DrawText(titleText, WIDTH/2 - MeasureText(titleText, titleFont)/2, HEIGHT/2 - 50, titleFont, fontColor); 
            DrawText(titleSubText, WIDTH/2 - MeasureText(titleSubText, titleFont - 20)/2, HEIGHT/2, titleFont - 20, fontColor); 
                printf("%s\n", screenState); 
            if(IsKeyPressed(32)){
                strcpy(screenState, "playing"); 
                player->numLives = 3; 
                player_reset(player); 
                level = 0; 
                player->score = 0; 
                for(int i = 0; i < list->length; i++){
                    asteroid_destroy(list->data[i], list); 
                    i--; 
                }
                for(int j = 0; j < player->bullets->length; j++){
                    bulletlist_remove(player->bullets, player->bullets->bullets[j]); 
                    j--; 
                }
                if(saucers[0] != NULL){
                    saucer_delete(saucers[0]);
                    saucers[0] = NULL;
                }
                timer = 0; 
            }
            ClearBackground(backgroundColor); 
            EndDrawing(); 
        }
    }
    asteroid_list_delete(list); 
    player_delete(player); 
    UnloadSound(explosion); 
    CloseAudioDevice(); 
    CloseWindow(); 
    return 0; 
}