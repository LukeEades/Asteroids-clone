#include "player.h"
#include "asteroid.h"

Player *player_create(){
    Player *player = (Player *)malloc(sizeof(Player) + sizeof(Line) * 7); 
    player->numLives = 3; 
    player->position = (Vector2){0,0}; 
    player->velocity = (Vector2){0,0}; 
    player->acceleration = (Vector2){0,0}; 
    Vector2 vertices[] = {
        (Vector2){0,1}, 
        (Vector2){-1, -1},
        (Vector2){1, -1},
        (Vector2){.7, -.7},
        (Vector2){-.7, -.7}
    };
    player->scale = (Vector2){1,-1}; 
    player->angle = 0; 
    player->numLines = 0; 
    memcpy(player->vertices,vertices,sizeof(vertices)); 
    return player; 
}
void player_render(Player *player, Color color){
    int i; 
    for(i = 0; i < player->numLines; i++){
        line_render(&(player->lines[i]), color); 
    }    
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

    Vector2 worldVec1 = player_world_to_screen(player, local_to_world(player,player->vertices[0])); 
    Vector2 worldVec2 = player_world_to_screen(player, local_to_world(player,player->vertices[1])); 
    Vector2 worldVec3 = player_world_to_screen(player, local_to_world(player,player->vertices[2])); 
    Line *lineA = line_create(worldVec1, worldVec2); 
    Line *lineB = line_create(worldVec1, worldVec3); 
    Line *lineC = line_create(worldVec2, worldVec3); 
    line_set_velocity(lineA, player->velocity); 
    line_set_velocity(lineB, player->velocity); 
    line_set_velocity(lineC, player->velocity); 
    line_update(lineA, dt); 
    player->numLines = 3; 
    memcpy(&(player->lines[0]), lineA, sizeof(Line)); 
    memcpy(&(player->lines[1]), lineB, sizeof(Line)); 
    memcpy(&(player->lines[2]), lineC, sizeof(Line)); 
    line_delete(lineA); 
    line_delete(lineB); 
    line_delete(lineC); 
}

void player_scale(Player *player, Vector2 scale){
   player->scale = scale; 
}

Vector2 local_to_world(Player *player, Vector2 vec){
    Vector2 vecScale = {vec.x * player->scale.x, vec.y * player->scale.y}; 
    Vector2 vecRotate = {vecScale.x * cos(player->angle) - vecScale.y * sin(player->angle), vecScale.y * cos(player->angle) + vecScale.x * sin(player->angle)};
    Vector2 vecWorld = {vecRotate.x + player->position.x, vecRotate.y + player->position.y};
    return vecWorld; 
}

Vector2 player_world_to_screen(Player *player, Vector2 vecWorld){
    return (Vector2){WIDTH/2 + vecWorld.x, HEIGHT/2 - vecWorld.y}; 
}
void player_angle_set_rads(Player *player, float angle){
    player->angle = angle; 
}

void player_position_set(Player *player, Vector2 pos){
    player->position = pos; 
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
        int speed = 2000; 
        Vector2 acc = {cos(player->angle + PI/2) * speed, sin(player->angle + PI/2) * speed}; 
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

void player_reset(Player *player){
    player_acceleration_set(player, (Vector2){0,0}); 
    player_velocity_set(player, (Vector2){0,0}); 
    player_angle_set_rads(player, 0); 
    player_position_set(player, (Vector2){0,0}); 
}

void player_delete(Player *player){
    free(player); 
}

void player_destroy(Player *player, float timer){
    Line *lineA = line_create(player->vertices[0], player->vertices[1]); 
    Line *lineB = line_create(player->vertices[0], player->vertices[2]); 
    Line *lineC = line_create(player->vertices[1], player->vertices[2]); 
 
}

Line *line_create(Vector2 a, Vector2 b){
    Line *line = (Line *)malloc(sizeof(Line)); 
    line->a = a; 
    line->b = b; 
    line->velocity = (Vector2){0,0}; 
    return line; 
}

void line_set_velocity(Line *line, Vector2 velocity){
    line->velocity = velocity; 
}
void line_render(Line *line, Color color){
    DrawLine(line->a.x, line->a.y, line->b.x, line->b.y, color); 
}

void line_update(Line *line, double dt){
    Vector2 velocity = line->velocity; 
    Vector2 newPosA = {velocity.x * dt + line->a.x, velocity.y * dt + line->a.y}; 
    Vector2 newPosB = {velocity.x * dt + line->b.x, velocity.y * dt + line->b.y}; 
    line->velocity.x += velocity.x * -.05; 
    line->velocity.y += velocity.x * -.05; 
    line->a = newPosA; 
    line->b = newPosB; 
}

void line_delete(Line *line){
    free(line); 
}