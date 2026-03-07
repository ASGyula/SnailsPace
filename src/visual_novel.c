//
// Created by asgyu on 2026. márc. 7..
//

#include "visual_novel.h"

#include <stdio.h>

#include "game_manager.h"

void increase_visual_novel_state(VisualNovelState* visual_novel_state, int id){
    visual_novel_state->currentDialogID = id;
}

// void change_scene(){
    // scene_switch(&game, DEALER_ROOM);
// }
