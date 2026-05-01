//
// Created by asgyu on 2026. márc. 14..
//

#ifndef SNAILSPACE_TRIGGERS_H
#define SNAILSPACE_TRIGGERS_H
#include "game_manager.h"

void check_trigger_zones(Game* game);

void check_miside_trigger_zones(Game* game, TextureAssets* texture_assets, MoveableModel* Mita);

void rotate_camera_towards_mita(Game* game, const MoveableModel* Mita);

#endif //SNAILSPACE_TRIGGERS_H