//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_VISUAL_NOVEL_STATE_H
#define SNAILSPACE_VISUAL_NOVEL_STATE_H

#include "game_manager.h"
#include "game_types.h"
#include "ui_manager.h"
#include "visual_novel.h"

typedef enum{
    SPEAKER_PLAYER,
    SPEAKER_DEALER,
    SPEAKER_HELSIE,
} SpeakerID;

typedef enum{
    DLG_INTRO,
    DLG_GYULASZ_CALM_HIMSELF_DOWN,
    DLG_DEALER_GREETING,
    DLG_PLAYER_SEARCHING_HELSIE,
    DLG_HELSIE_GREETING,
    DLG_GYULASZ_GREETING_HELSIE,
    DLG_DEALER_KNOWS_HELSIE,
    DLG_GYULASZ_EYES_SPARKING,
    DLG_DEALER_MAKE_AN_OFFER,
    DLG_DEALER_MAKE_AN_OFFER2,
    DLG_GYULASZ_ASKING_ABOUT_OFFER,
    DLG_MONSTER_APPEARS,
    DLG_COUNT
} DialogueID;

Dialogue create_dialogue_from_id(DialogueID id, const char* name_placeholder, const UIElement* player_speaker_img);

void handle_mouse_input_visual_novel(SDL_Event* event, VisualNovelState* visual_novel_state, Dialogue* dialogue, TextureAssets* texture_assets);

#endif //SNAILSPACE_VISUAL_NOVEL_STATE_H