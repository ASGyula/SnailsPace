//
// Created by asgyu on 2026. márc. 14..
//

#include "triggers.h"

#include <math.h>
#include <stdio.h>

#include "ai.h"
#include "game_manager.h"

void check_trigger_zones(Game* game){
    if(!game->triggerZones.BatVisionHelsieTakeAHint.isActivated){
        const float dx = game->player.camera.x - game->triggerZones.BatVisionHelsieTakeAHint.x;
        const float dz = game->player.camera.z - game->triggerZones.BatVisionHelsieTakeAHint.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.BatVisionHelsieTakeAHint.radius){
            game->triggerZones.BatVisionHelsieTakeAHint.isActivated = true;

            game->visualNovelState.isShowingUI = true;
            increase_visual_novel_state(&game->visualNovelState, 30);
            change_snail_ai(game, true);
        }
    }

    if(!game->gameObjects.PunchPacificMonster.triggerZone.isActivated){
        const float dx = game->player.camera.x - game->gameObjects.PunchPacificMonster.triggerZone.x;
        const float dz = game->player.camera.z - game->gameObjects.PunchPacificMonster.triggerZone.z;
        const float distance = sqrtf(dx * dx + dz * dz);


        if(distance < game->gameObjects.PunchPacificMonster.triggerZone.radius){
            game->gameObjects.PunchPacificMonster.triggerZone.isActivated = true;
            game->gameObjects.ImmortalSnail.isMoving = false;
            scene_switch(game, PRE_LIDAR);
        }
    }
}