//
// Created by asgyu on 2026. márc. 5..
//

#ifndef SNAILSPACE_PLAYER_H
#define SNAILSPACE_PLAYER_H
#include "camera.h"

typedef struct{
    Camera camera;
    char* playerName[64];
} Player;

#endif //SNAILSPACE_PLAYER_H