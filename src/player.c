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

void player_angle_set_rads(Player *player, float angle){
    player->angle = angle; 
}

void player_position_set(Player *player, Vector2 pos){
    player->position = pos; 
}

void player_update(Player *player, double dt){
    player_get_input(player, dt); 
    Vector2 acceleration = player->acceleration; 
    Vector2 velocity = {player->velocity.x + acceleration.x * dt, player->velocity.y + acceleration.y * dt}; 
    Vector2 newPos = {(1/2)*acceleration.x*pow(dt,2) + velocity.x*dt + player->position.x, (1/2)*acceleration.y*pow(dt,2) + velocity.y * dt + player->position.y};
    newPos = player_check_wrap(player, newPos); 
    player_position_set(player, newPos); 
    player_velocity_set(player, velocity); 
    player_velocity_add(player, (Vector2){player->velocity.x * -.05, player->velocity.y * -.05}); 
    player_acceleration_set(player, (Vector2){0,0}); 
}

void player_acceleration_set(Player *player, Vector2 acceleration){
    player->acceleration = acceleration; 
}

void player_velocity_set(Player *player, Vector2 velocity){
    player->velocity = velocity; 
}

void player_acceleration_add(Player *player, Vector2 add){
    player->acceleration.x += add.x; 
    player->acceleration.y += add.y; 
}

void player_get_input(Player *player, double dt){
    if(IsKeyDown(87)){
        Vector2 acc = {cos(player->angle + PI/2) * 1000, sin(player->angle + PI/2) * 1000}; 
        player_acceleration_add(player, acc); 
    }
    if(IsKeyDown(65)){
        player_angle_add_rads(player, PI * dt); 
    }
    if(IsKeyDown(68)){
        player_angle_add_rads(player, -PI * dt); 
    }
}

void player_velocity_add(Player *player, Vector2 add){
    player->velocity.x += add.x; 
    player->velocity.y += add.y; 
}

void player_angle_add_rads(Player *player, float angle){
    player->angle += angle; 
}

Vector2 player_check_wrap(Player *player, Vector2 newPos){
    Vector2 result = newPos; 
    if(newPos.x > WIDTH/2){
        result.x = -WIDTH/2;  
    }else if(newPos.x < -WIDTH/2){
        result.x = WIDTH/2; 
    }
    if(newPos.y > HEIGHT/2){
        result.y = -HEIGHT/2; 
    }else if(newPos.y < -HEIGHT/2){
        result.y = HEIGHT/2; 
    }
    return result; 
}