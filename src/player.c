#include "player.h"

Player *player_create(){
    Player *player = (Player *)malloc(sizeof(Player) + sizeof(Line) * 7); 
    player->numLives = 3; 
    player->position = (Vector2){0,0}; 
    player->velocity = (Vector2){0,0}; 
    player->acceleration = (Vector2){0,0}; 
    player->bullets = bulletlist_create(); 
    Vector2 vertices[] = {
        (Vector2){0,1}, 
        (Vector2){-1, -1},
        (Vector2){1, -1},
        (Vector2){.7, -.7},
        (Vector2){-.7, -.7}, 
        (Vector2){0,-1.5}
    };
    player->scale = (Vector2){1,1}; 
    player->angle = 0; 
    player->friction = -.01; 
    player->numLines = 0; 
    player->dead = false;  
    memcpy(player->vertices,vertices,sizeof(vertices)); 
    Vector2 worldVec1 = player_world_to_screen(player, local_to_world(player,player->vertices[0])); 
    Vector2 worldVec2 = player_world_to_screen(player, local_to_world(player,player->vertices[1])); 
    Vector2 worldVec3 = player_world_to_screen(player, local_to_world(player,player->vertices[2])); 
    Vector2 worldVec4 = player_world_to_screen(player, local_to_world(player,player->vertices[3])); 
    Vector2 worldVec5 = player_world_to_screen(player, local_to_world(player,player->vertices[4])); 
    Vector2 worldVec6 = player_world_to_screen(player, local_to_world(player,player->vertices[5])); 
    Line *lineA = line_create(worldVec1, worldVec2); 
    Line *lineB = line_create(worldVec1, worldVec3); 
    Line *lineC = line_create(worldVec3, worldVec4); 
    Line *lineD = line_create(worldVec4, worldVec5); 
    Line *lineE = line_create(worldVec2, worldVec5); 
    Line *lineF = line_create(worldVec4, worldVec6);
    Line *lineG = line_create(worldVec5, worldVec6);
    memcpy(&(player->lines[0]), lineA, sizeof(Line));
    memcpy(&(player->lines[1]), lineB, sizeof(Line));
    memcpy(&(player->lines[2]), lineC, sizeof(Line));
    memcpy(&(player->lines[3]), lineD, sizeof(Line));
    memcpy(&(player->lines[4]), lineE, sizeof(Line));
    memcpy(&(player->lines[5]), lineF, sizeof(Line));
    memcpy(&(player->lines[6]), lineG, sizeof(Line));
    return player; 
}
void player_render(Player *player, Color color){
    int i; 
    for(i = 0; i < player->numLines; i++){
        line_render(&(player->lines[i]), color); 
    }
    for(int j = 0; j < player->bullets->length; j++){
        bullet_render(player->bullets->bullets[j], color); 

    }   
    char numText[32]; 
    char livesText[32]; 
    sprintf(numText, "Score: %d", player->score); 
    sprintf(livesText, "Lives: %d", player->numLives); 
    DrawText(numText, WIDTH - 200, 50, 20, color); 
    DrawText(livesText, 50, 50, 20, color); 
}

void player_update(Player *player, double dt){
    for(int j = 0; j < player->bullets->length; j++){ 
        bullet_update(player->bullets->bullets[j], dt); 
        if(player->bullets->bullets[j]->timer < 0){
            bulletlist_remove(player->bullets, player->bullets->bullets[j]);
        }
    }
    if(!(player->dead)){
        player_get_input(player, dt); 
        Vector2 worldVec1 = player_world_to_screen(player, local_to_world(player,player->vertices[0])); 
        Vector2 worldVec2 = player_world_to_screen(player, local_to_world(player,player->vertices[1])); 
        Vector2 worldVec3 = player_world_to_screen(player, local_to_world(player,player->vertices[2])); 
        Vector2 worldVec4 = player_world_to_screen(player, local_to_world(player,player->vertices[3])); 
        Vector2 worldVec5 = player_world_to_screen(player, local_to_world(player,player->vertices[4])); 
        Vector2 worldVec6 = player_world_to_screen(player, local_to_world(player,player->vertices[5]));  
        line_position_set(&(player->lines[0]), worldVec1, worldVec2); 
        line_position_set(&(player->lines[1]), worldVec1, worldVec3); 
        line_position_set(&(player->lines[2]), worldVec3, worldVec4); 
        line_position_set(&(player->lines[3]), worldVec4, worldVec5); 
        line_position_set(&(player->lines[4]), worldVec2, worldVec5); 
        line_position_set(&(player->lines[5]), worldVec4, worldVec6); 
        line_position_set(&(player->lines[6]), worldVec5, worldVec6); 
    }else{
        line_set_velocity(&(player->lines[0]), (Vector2){-3, 3}, player->angle); 
        line_update(&(player->lines[0]), dt);
        line_set_velocity(&(player->lines[1]), (Vector2){3,3}, player->angle); 
        line_update(&(player->lines[1]), dt); 
        line_set_velocity(&(player->lines[2]), (Vector2){-3,-3}, player->angle); 
        line_update(&(player->lines[2]), dt); 
        line_set_velocity(&(player->lines[3]), (Vector2){0,-3}, player->angle); 
        line_update(&(player->lines[3]), dt); 
        line_set_velocity(&(player->lines[4]),(Vector2){3,-3}, player->angle); 
        line_update(&(player->lines[4]), dt); 
    }
    player->numLines = 5; 
    if(player->acceleration.x != 0 || player->acceleration.y != 0){
        player->numLines = 7; 
    }
    Vector2 acceleration = player->acceleration; 
    Vector2 velocity = {player->velocity.x + acceleration.x * dt, player->velocity.y + acceleration.y * dt}; 
    Vector2 newPos = {(1/2)*acceleration.x*pow(dt,2) + velocity.x*dt + player->position.x, (1/2)*acceleration.y*pow(dt,2) + velocity.y * dt + player->position.y};
    newPos = player_check_wrap(player, newPos); 
    player_position_set(player, newPos); 
    player_velocity_set(player, velocity);
    player_velocity_add(player, (Vector2){player->velocity.x * player->friction, player->velocity.y * player->friction}); 
    player_acceleration_set(player, (Vector2){0,0}); 
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
    if(IsKeyPressed(32)){
        // create new bullet entity
        // assign position of front vertex
        // give initial velocity
        Bullet *bullet = bullet_create(player->lines[0].a, player->angle); 
        bulletlist_append(player->bullets, bullet); 
        // printf("angle: %f\n", player->angle); 
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
    player->friction = -.01; 
}

void player_delete(Player *player){
    free(player); 
}

double player_destroy(Player *player){
    player->dead = true; 
    player->numLives--; 
    return 2; 
}

void player_score_add(Player *player, int score){
    player->score += score; 
}

Line *line_create(Vector2 a, Vector2 b){
    Line *line = (Line *)malloc(sizeof(Line)); 
    line->a = a; 
    line->b = b; 
    line->velocity = (Vector2){0,0}; 
    return line; 
}

void line_set_velocity(Line *line, Vector2 velocity, float angle){
    line->velocity = (Vector2){velocity.x * cos(angle) - velocity.y * sin(angle), velocity.y * cos(angle) + velocity.x * sin(angle)};
    // line->velocity = velocity;   
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

void line_position_set(Line *line, Vector2 a, Vector2 b){
    line->a = a; 
    line->b = b; 
}

Bullet* bullet_create(Vector2 position, float angle){
    Bullet *bullet = (Bullet *)malloc(sizeof(Bullet)); 
    bullet->position = position; 
    bullet->speed = 500; 
    bullet->angle = angle; 
    bullet->velocity = (Vector2){bullet->speed * cos(bullet->angle + PI/2), bullet->speed * sin(bullet->angle + PI/2)};  
    bullet->timer = 3;
    return bullet;  
}

void bullet_check_wrap(Bullet *bullet){
    if(bullet->position.x < 0){
        bullet->position.x = WIDTH; 
    }else if(bullet->position.x > WIDTH){
        bullet->position.x = 0; 
    }
    if(bullet->position.y < 0){
        bullet->position.y = HEIGHT; 
    }else if(bullet->position.y > HEIGHT){
        bullet->position.y = 0; 
    }
}

bool bullet_collide_asteroid(Bullet *bullet, Asteroid *asteroid){
//     for(int i = 1; i < asteroid->numVerts; i++){
//         Vector2 a = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, asteroid->vertices[i - 1])); 
//         Vector2 b = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, asteroid->vertices[i])); 
//         if(lines_intersect(a, b, bullet->position, (Vector2){bullet->position.x + 5 * cos(bullet->angle + PI/2), bullet->position.y - 5 * sin(bullet->angle + PI/2)})){
//             return true; 
//         }
//     }
//     return false; 
    for(int i = 1; i < asteroid->numVerts; i++){
        Vector2 asteroidA = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, asteroid->vertices[i -1])); 
        Vector2 asteroidB = asteroid_to_screen(asteroid, asteroid_to_world(asteroid, asteroid->vertices[i]));
        Vector2 asteroidCenter = asteroid_to_screen(asteroid, asteroid_to_world(asteroid,(Vector2){0,0})); 
        Vector2 bulletPos = (Vector2){bullet->position.x + 5 * cos(bullet->angle + PI/2), bullet->position.y - 5 * sin(bullet->angle + PI/2)};  
        if(lines_intersect(asteroidA, asteroidB, bulletPos, asteroidCenter)){
            return false; 
        }
    } 
    return true;  
}

void bullet_update(Bullet *bullet, double dt){
    bullet->timer -= dt; 
    bullet->position = (Vector2){bullet->velocity.x * dt + bullet->position.x, bullet->position.y - bullet->velocity.y * dt}; 
    bullet_check_wrap(bullet); 
}

void bullet_render(Bullet *bullet, Color color){
    Vector2 a = bullet->position; 
    Vector2 b = (Vector2){bullet->position.x + 5 * cos(bullet->angle + PI/2), bullet->position.y - 5 * sin(bullet->angle + PI/2)}; 
    DrawLine(a.x, a.y, b.x, b.y, color); 
}

void bullet_destroy(Bullet *bullet){

    bullet_delete(bullet); 
}
void bullet_delete(Bullet *bullet){
    free(bullet); 
}


BulletList *bulletlist_create(){
    BulletList *list = (BulletList *)malloc(sizeof(BulletList)); 
    list->capacity = 5; 
    list->length = 0; 
    list->bullets = (Bullet**)malloc(sizeof(Bullet *) * list->capacity); 
    return list; 
}

void bulletlist_append(BulletList *list, Bullet *bullet){
    if(list->length == list->capacity){
        list->capacity *= 2; 
        list->bullets = (Bullet **)realloc(list->bullets, sizeof(Bullet *) * list->capacity); 
    }
    list->bullets[list->length++] = bullet; 
}

void bulletlist_remove(BulletList *list, Bullet *bullet){
    for(int i = 0; i < list->length; i++){
        if(list->bullets[i] == bullet){
            while(i < list->length){
                list->bullets[i] = list->bullets[i + 1]; 
                i++; 
            }
            list->length--; 
            bullet_delete(bullet); 
        }
    }
}

Saucer *saucer_create(saucerType type){
    Saucer *saucer = (Saucer *)malloc(sizeof(Saucer) + sizeof(Vector2) * 9);
    saucer->type = type; 
    Vector2 vertices[] = {
        (Vector2){5,0},
        (Vector2){2, 1.5}, 
        (Vector2){1,3}, 
        (Vector2){-1, 3}, 
        (Vector2){-2, 1.5},
        (Vector2){-5, 0}, 
        (Vector2){-2, -2},
        (Vector2){2,-2},
        (Vector2){5,0}
    };
    memcpy(saucer->vertices, vertices, 9 * sizeof(Vector2)); 
    saucer->velocity = (Vector2){0,0}; 
    saucer->acceleration = (Vector2){0,0}; 
    saucer->position = (Vector2){0,0}; 
    saucer->numVerts = sizeof(vertices)/sizeof(Vector2); 
    if(type == SMALLSAUCE){
        saucer->scale = 5; 
    }else if(type == BIGSAUCE){
        saucer->scale = 10; 
    }
    return saucer; 
}

void saucer_update(Saucer *saucer, double dt){
    Vector2 acc = saucer->acceleration; 
    Vector2 vel = {saucer->velocity.x + acc.x * dt, saucer->velocity.y + acc.y *dt}; 
    Vector2 newPos = {saucer->position.x + vel.x * dt + (1/2) * acc.x * pow(dt,2), saucer->position.y + vel.y * dt + (1/2) * acc.y * pow(dt,2)}; 
    
    saucer->position = saucer_check_wrap(saucer,newPos); 
    saucer->acceleration = (Vector2){0,0}; 
}

void saucer_render(Saucer *saucer, Color color){
    for(int i = 1; i < saucer->numVerts; i++){
        Vector2 a = saucer->vertices[i -1]; 
        Vector2 b = saucer->vertices[i]; 
        a = saucer_to_screen(saucer, saucer_to_world(saucer, a)); 
        b = saucer_to_screen(saucer, saucer_to_world(saucer, b)); 
        DrawLine(a.x, a.y, b.x, b.y,color); 
    }   

}

Vector2 saucer_to_world(Saucer *saucer, Vector2 vec){
    Vector2 newVec = vec; 
    newVec.x *= saucer->scale; 
    newVec.y *= saucer->scale; 
    Vector2 world = {newVec.x + saucer->position.x, newVec.y + saucer->position.y}; 
    return world; 
}

Vector2 saucer_to_screen(Saucer *saucer, Vector2 vec){
    return (Vector2){WIDTH/2 + vec.x, HEIGHT/2 - vec.y}; 
}

// make into a general function for all objects
Vector2 saucer_check_wrap(Saucer *saucer, Vector2 newPos){
    Vector2 pos = newPos; 
    if(newPos.x > WIDTH/2){
        pos.x = -WIDTH/2; 
    }else if(newPos.x < -WIDTH/2){
        pos.x = WIDTH/2; 
    }
    if(newPos.y > HEIGHT/2){
        pos.y = -HEIGHT/2; 
    }else if(newPos.y < -HEIGHT/2){
        pos.y = HEIGHT/2; 
    }
    return pos; 
}