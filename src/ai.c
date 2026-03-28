//
// Created by asgyu on 2026. márc. 18..
//

#include "ai.h"
#include <math.h>
#include <stdio.h>
#include "game_manager.h"
#include "graphics.h"

void change_snail_ai(Game* game, bool value){
    game->gameObjects.ImmortalSnail.isMoving = value;
}

void update_snail_ai(Game* game, const float deltaTime){
    MoveableModel* monster = &game->gameObjects.ImmortalSnail;
    Camera* camera = &game->player.camera;
    const float dx = camera->x - monster->x;
    const float dy = camera->y - monster->y;
    const float dz = camera->z - monster->z;
    const float dist = sqrtf(dx*dx + dy*dy + dz*dz);

    if(dist > 1.7f){
        if(game->player.camera.vape.isVaping){
            monster->x += (dx / dist) * monster->animSpeed * 0.8f * deltaTime;
            monster->z += (dz / dist) * monster->animSpeed * 0.8f * deltaTime;
        }else{
            monster->x += (dx / dist) * monster->animSpeed * deltaTime;
            monster->z += (dz / dist) * monster->animSpeed * deltaTime;
        }
    }else{
        if(game->caughtBySnailAt == 0){
            game->caughtBySnailAt = SDL_GetTicks();
            add_sound_wave(monster->x, monster->y, monster->z, 10, 19, 10, 'm');
        }else if(game->caughtBySnailAt+1800 > SDL_GetTicks()){
            add_sound_wave(monster->x, monster->y, monster->z, 20, 40, 20, 'm');
        }

        float targetYaw = 180.0f;
        float targetPitch = 40.0f;
        float rotationSpeed = 10.0f * deltaTime;

        if(camera->yaw < targetYaw) camera->yaw += rotationSpeed * 20.0f;
        if(camera->yaw > targetYaw) camera->yaw -= rotationSpeed * 20.0f;

        if(camera->pitch < targetPitch) camera->pitch += rotationSpeed * 10.0f;

        monster->x = camera->x;
        monster->z = camera->z + 1.0f;
        monster->y = camera->y - 0.4f;
        change_camera_input_handler(game, false, false);
    }
}
