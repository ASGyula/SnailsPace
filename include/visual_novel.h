//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_TEXTURE_ASSETS_H
#define SNAILSPACE_TEXTURE_ASSETS_H
#include "game_types.h"
#include "ui_manager.h"

typedef struct{
    UIElement Helsie_Happy;
    UIElement Helsie_Scared;
    UIElement Helsie_lt3_Gyula;
    UIElement Gyulasz_Brave;
    UIElement Gyulasz_Determined;
    UIElement Gyulasz_Thinking;
    UIElement Gyulasz_Have_an_evil_idea;
    UIElement Gyulasz_Suprised;
    UIElement Gyulasz_Scared;
    UIElement Gyulasz_Hand_Shake;
    UIElement Dealer;
    UIElement Snail;
    TTF_Font* mainFont;
    UIElement ESCButton;
    UIElement SpaceButton;
    UIElement VButton;
} TextureAssets;

static char* PLAYER_NAME;

void increase_visual_novel_state(VisualNovelState *visual_novel_state, int id);

#endif //SNAILSPACE_TEXTURE_ASSETS_H
