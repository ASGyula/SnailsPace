//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_TYPES_H
#define SNAILSPACE_GAME_TYPES_H

typedef struct{
    int currentDialogID;
    char* playerName;
    bool isShowingUI;
} VisualNovelState;

typedef enum{
    VN_INTRO,
    DEALER_ROOM,
    PRE_BAT_VISION,
    BAT_VISION,
    LIDAR,
    LAST_ROOM
} GameScene;

typedef enum {
    TRIGGER_DIALOGUE,
    TRIGGER_SPAWN_MONSTER
} TriggerType;

typedef struct{
    float x, z;
    float radius;
    TriggerType type;
    bool isActivated;
} TriggerZone;

typedef struct{
    TriggerZone BatVisionHelsieTakeAHint;
} GameTriggerZones;

#endif //SNAILSPACE_GAME_TYPES_H

