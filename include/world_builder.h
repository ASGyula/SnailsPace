//
// Created by asgyu on 2026. márc. 19..
//

#ifndef SNAILSPACE_WORLD_BUILDER_H
#define SNAILSPACE_WORLD_BUILDER_H
#include "game_manager.h"
#include "game_types.h"

void world_initialization(Game* game, const char* playerName);

void build_scene(Game* game, GameScene scene);

#endif //SNAILSPACE_WORLD_BUILDER_H