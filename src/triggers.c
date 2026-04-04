//
// Created by asgyu on 2026. márc. 14..
//

#include "triggers.h"

#include <math.h>
#include <stdio.h>

#include "ai.h"
#include "dialogue_data.h"
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
            game->gameObjects.ImmortalSnail.x = game->gameObjects.PunchPacificMonster.x;
            game->gameObjects.ImmortalSnail.y = game->gameObjects.PunchPacificMonster.y;
            game->gameObjects.ImmortalSnail.z = game->gameObjects.PunchPacificMonster.z;
            // scene_switch(game, PRE_LIDAR);
        }
    }

    if(!game->gameObjects.WhiteMonster.triggerZone.isActivated){
        const float dx = game->player.camera.x - game->gameObjects.WhiteMonster.triggerZone.x;
        const float dz = game->player.camera.z - game->gameObjects.WhiteMonster.triggerZone.z;
        const float distance = sqrtf(dx * dx + dz * dz);


        if(distance < game->gameObjects.WhiteMonster.triggerZone.radius){
            game->gameObjects.WhiteMonster.triggerZone.isActivated = true;
            scene_switch(game, PRE_LIDAR);
        }
    }

    if(!game->triggerZones.LidarChangesToFast.isActivated){
        const float dx = game->player.camera.x - game->triggerZones.LidarChangesToFast.x;
        const float dz = game->player.camera.z - game->triggerZones.LidarChangesToFast.z;
        const float distance = sqrtf(dx * dx + dz * dz);


        if(distance < game->triggerZones.LidarChangesToFast.radius){
            game->triggerZones.LidarChangesToFast.isActivated = true;
        }
    }
}

void check_miside_trigger_zones(Game* game, TextureAssets* texture_assets){
    if(!game->triggerZones.MitaAsksForAScissors.isActivated){
        const float dx = game->player.camera.x - game->triggerZones.MitaAsksForAScissors.x;
        const float dz = game->player.camera.z - game->triggerZones.MitaAsksForAScissors.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.MitaAsksForAScissors.radius){
            game->triggerZones.MitaAsksForAScissors.isActivated = true;

            game->visualNovelState.isShowingUI = true;
            increase_visual_novel_state(&game->visualNovelState, DLG_MITA_ASKS_FOR_A_SCISSORS);
        }
    }

    if(!game->gameObjects.Scissors.triggerZone.isActivated){
        const float dx = game->player.camera.x - game->gameObjects.Scissors.triggerZone.x;
        const float dz = game->player.camera.z - game->gameObjects.Scissors.triggerZone.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->gameObjects.Scissors.triggerZone.radius){
            game->gameObjects.Scissors.triggerZone.isActivated = true;
       }else{
            game->gameObjects.Scissors.triggerZone.isActivated = false;
        }
    }

    if(!game->triggerZones.MitaBonkChoice.isActivated && game->visualNovelState.quest_state == MITA_OR_BONK){
        const float dx = game->player.camera.x - game->triggerZones.MitaBonkChoice.x;
        const float dz = game->player.camera.z - game->triggerZones.MitaBonkChoice.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.MitaBonkChoice.radius){
            game->triggerZones.MitaBonkChoice.isActivated = true;
            playSoundWithLoops(game->sounds.Mita_Bonk, 100);
            game->visualNovelState.isShowingUI = true;
            increase_visual_novel_state(&game->visualNovelState, DLG_PLAYER_CONFUSED_BONKING);
        }
    }

    if(!game->triggerZones.TryToEnterMitasRoom.isActivated && game->visualNovelState.quest_state == MITA_OR_BONK){
        const float dx = game->player.camera.x - game->triggerZones.TryToEnterMitasRoom.x;
        const float dz = game->player.camera.z - game->triggerZones.TryToEnterMitasRoom.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.TryToEnterMitasRoom.radius){
            game->triggerZones.TryToEnterMitasRoom.isActivated = true;
            game->tryToEnterMitasRoom = SDL_GetTicks();
            game->visualNovelState.isShowingUI = true;
            increase_visual_novel_state(&game->visualNovelState, DLG_PLAYER_DIZZINESS);
        }
    }

    if(!game->triggerZones.MitaTakeScissors.isActivated && game->visualNovelState.quest_state == MITA_OR_BONK){
        const float dx = game->player.camera.x - game->triggerZones.MitaTakeScissors.x;
        const float dz = game->player.camera.z - game->triggerZones.MitaTakeScissors.z;
        const float distance = sqrtf(dx * dx + dz * dz);

        if(distance < game->triggerZones.MitaTakeScissors.radius){
            Dialogue newDialog = create_dialogue_from_id(DLG_PLAYER_GIVE_MITA_THE_SCISSORS, game->visualNovelState.playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
            game->visualNovelState.dialogue = newDialog;

            game->triggerZones.MitaTakeScissors.isActivated = true;
            game->visualNovelState.isShowingUI = true;
            increase_visual_novel_state(&game->visualNovelState, DLG_PLAYER_GIVE_MITA_THE_SCISSORS);
        }
    }
}