//
// Created by asgyu on 2026. márc. 18..
//

#include "ai.h"
#include <math.h>
#include <stdio.h>
#include "game_manager.h"

void change_snail_ai(Game* game, bool value){
    game->gameObjects.ImmortalSnail.isMoving = value;
}

void update_snail_ai(Game* game, const float deltaTime){
    MoveableModel* monster = &game->gameObjects.ImmortalSnail;
    const Camera* camera = &game->player.camera;
    const float dx = camera->x - monster->x;
    const float dy = camera->y - monster->y;
    const float dz = camera->z - monster->z;
    const float dist = sqrtf(dx*dx + dy*dy + dz*dz);

    if(dist > 1.7f){
        monster->x += (dx / dist) * monster->animSpeed * deltaTime;
        monster->z += (dz / dist) * monster->animSpeed * deltaTime;
    }else{
        monster->isMoving = false;
        scene_switch(game, DEAD_ROOM);
    }
}
