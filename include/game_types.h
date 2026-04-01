//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_TYPES_H
#define SNAILSPACE_GAME_TYPES_H
#include "ui_manager.h"

typedef struct{
    int currentDialogID;
    char* playerName;
    bool isShowingUI;
    Dialogue dialogue;
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
    DEAD_ROOM
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
    TriggerZone LidarChangesToFast;
} GameTriggerZones;

#endif //SNAILSPACE_GAME_TYPES_H

