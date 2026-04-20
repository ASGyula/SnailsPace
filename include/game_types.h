//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_TYPES_H
#define SNAILSPACE_GAME_TYPES_H
#include "ui_manager.h"


typedef enum {
    NOTHING,
    KITCHEN_START,
    SEARCHING_FOR_SCISSORS,
    MITA_OR_BONK,
    PRE_TIC_TAC_TOE,
    PLAYING_TIC_TAC_TOE,
    AFTER_TIC_TAC_TOE,
    DECISION_TIME,
    ESCAPE_ROUTE,
    STAY_WITH_MITA
} QuestState;

typedef struct{
    int currentDialogID;
    char* playerName;
    bool isShowingUI;
    Dialogue dialogue;
    QuestState quest_state;
    TicTacToe ticTacToe;
    DecisionUI decisionUI;
} VisualNovelState;

typedef enum{
    MAIN_MENU,
    VN_INTRO,
    DEALER_ROOM,
    PRE_BAT_VISION,
    BAT_VISION,
    MITA_SAVES_PLAYER,
    PRE_LIDAR,
    LIDAR,
    LAST_ROOM,
    DEAD_ROOM,
    CREDITS
} GameScene;

typedef enum{
    TRIGGER_DIALOGUE,
    TRIGGER_SPAWN_MONSTER,
    TRIGGER_DRINK
} TriggerType;

typedef struct{
    float x, z;
    float radius;
    TriggerType type;
    bool isActivated;
} TriggerZone;

typedef struct{
    TriggerZone BatVisionHelsieTakeAHint;
    TriggerZone MitaAsksForAScissors;
    TriggerZone MitaBonkChoice;
    TriggerZone TryToEnterMitasRoom;
    TriggerZone MitaTakeScissors;
    TriggerZone LidarChangesToFast;
    TriggerZone PlayTicTacToe;
    TriggerZone InvestigateMitasCloset;
} GameTriggerZones;

#endif //SNAILSPACE_GAME_TYPES_H

