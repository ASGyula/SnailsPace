//
// Created by asgyu on 2026. márc. 7..
//

#include "visual_novel.h"

#include <stdio.h>

#include "dialogue_data.h"
#include "game_manager.h"

void increase_visual_novel_state(VisualNovelState* visual_novel_state, int id){
    visual_novel_state->currentDialogID = id;
}

void increase_visual_novel_state_by_one(VisualNovelState* visual_novel_state){
    visual_novel_state->currentDialogID++;
}

void jump_to_dialog_player_leaving(VisualNovelState* visual_novel_state, TextureAssets* texture_assets){
    visual_novel_state->quest_state = ESCAPE_ROUTE;
    visual_novel_state->currentDialogID = DLG_PLAYER_LEAVING_MITA_ANGRY;
    visual_novel_state->dialogue = create_dialogue_from_id(DLG_PLAYER_LEAVING_MITA_ANGRY, visual_novel_state->playerName, &texture_assets->Mita_Angry);
    visual_novel_state->isShowingUI = true;
}

void jump_to_dialog_player_staying(VisualNovelState* visual_novel_state, TextureAssets* texture_assets){
    visual_novel_state->quest_state = STAY_WITH_MITA;
    visual_novel_state->currentDialogID = DLG_PLAYER_STAYING_MITA_HAPPY;
    visual_novel_state->dialogue = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
    visual_novel_state->isShowingUI = true;
}

// void change_scene(){
    // scene_switch(&game, DEALER_ROOM);
// }
