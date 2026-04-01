//
// Created by asgyu on 2026. márc. 5..
//

#ifndef SNAILSPACE_PLAYER_H
#define SNAILSPACE_PLAYER_H
#include "camera.h"
#include "obj_loader.h"

typedef struct{
    Camera camera;
    char* playerName[64];
} Player;

void check_player_collision(Camera* camera, const Model* model, float playerRadius);

void check_player_collision_mesh(Camera* camera, const Model* model, float playerRadius);

#endif //SNAILSPACE_PLAYER_H