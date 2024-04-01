#include "player.h"

Player *player_create(){
    Player *player = (Player *)malloc(sizeof(Player)); 
    player->numLives = 0; 
    player->position = (Vector2){0,0}; 
    player->velocity = (Vector2){0,0}; 
    player->acceleration = (Vector2){0,0}; 
    Vector2 vertices[] = {
        (Vector2){0,1}, 
        (Vector2){-.7, -.7},
        (Vector2){.7, -.7},
    };
    player->scale = (Vector2){1,-1}; 
    player->angle = 0; 
    memcpy(player->vertices,vertices,sizeof(vertices)); 
    return player; 
}
void player_render(Player *player, Color color){
    Vector2 worldVec1 = local_to_world(player,player->vertices[0]); 
    Vector2 worldVec2 = local_to_world(player,player->vertices[1]); 
    Vector2 worldVec3 = local_to_world(player,player->vertices[2]); 
    DrawLine(worldVec1.x, worldVec1.y, worldVec2.x, worldVec2.y, color); 
    DrawLine(worldVec1.x, worldVec1.y, worldVec3.x, worldVec3.y, color); 
    DrawLine(worldVec2.x, worldVec2.y, worldVec3.x, worldVec3.y, color); 
}
void player_scale(Player *player, Vector2 scale){
   player->scale = scale; 
}

Vector2 local_to_world(Player *player, Vector2 vec){
    Vector2 vecScale = {vec.x * player->scale.x, vec.y * player->scale.y}; 
    Vector2 vecRotate = {vecScale.x * cos(player->angle) - vecScale.y * sin(player->angle), vecScale.y * cos(player->angle) + vecScale.x * sin(player->angle)};
    Vector2 vecWorld = {vecRotate.x + player->position.x, vecRotate.y + player->position.y};
    Vector2 vecScreen = {WIDTH/2 + vecWorld.x, HEIGHT/2 - vecWorld.y};
    return vecScreen; 
}

void player_set_angle_rads(Player *player, float angle){
    player->angle = angle; 
}

void player_set_position(Player *player, Vector2 pos){
    player->position = pos; 
}