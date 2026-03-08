//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_TYPES_H
#define SNAILSPACE_GAME_TYPES_H

typedef struct{
    int currentDialogID;
    char* playerName;
} VisualNovelState;

typedef enum{
    VN_INTRO,
    DEALER_ROOM,
    PRE_BAT_VISION,
    BAT_VISION,
    LIDAR,
    LAST_ROOM
} GameScene;

#endif //SNAILSPACE_GAME_TYPES_H

