//
// Created by asgyu on 2026. márc. 14..
//

#include "triggers.h"

#include <math.h>

#include "ai.h"
#include "game_manager.h"

void check_trigger_zones(Game* game){
    if(!game->triggerZones.BatVisionHelsieTakeAHint.isActivated){
        float dx = game->player.camera.x - game->triggerZones.BatVisionHelsieTakeAHint.x;
        float dz = game->player.camera.z - game->triggerZones.BatVisionHelsieTakeAHint.z;
        float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.BatVisionHelsieTakeAHint.radius){
            game->triggerZones.BatVisionHelsieTakeAHint.isActivated = true;

            if(game->triggerZones.BatVisionHelsieTakeAHint.type == TRIGGER_DIALOGUE){
                game->visualNovelState.isShowingUI = true;
                increase_visual_novel_state(&game->visualNovelState, 30);
                change_snail_ai(game, true);
            }
        }
    }
}