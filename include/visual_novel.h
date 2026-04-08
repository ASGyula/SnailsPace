//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_TEXTURE_ASSETS_H
#define SNAILSPACE_TEXTURE_ASSETS_H
#include "game_types.h"
#include "ui_manager.h"

static char* PLAYER_NAME;

void increase_visual_novel_state(VisualNovelState *visual_novel_state, int id);
void increase_visual_novel_state_by_one(VisualNovelState* visual_novel_state);

void jump_to_dialog_player_leaving(VisualNovelState* visual_novel_state, TextureAssets* texture_assets);

#endif //SNAILSPACE_TEXTURE_ASSETS_H
