//
// Created by asgyu on 2026. febr. 26..
//

#ifndef SNAILSPACE_CAMERA_H
#define SNAILSPACE_CAMERA_H
#include <SDL_events.h>

#include "obj_loader.h"
#include "sounds.h"

typedef struct{
    float usedLiquid;
    float smokeAmount;
    bool isVaping;
    Uint32 endedTime;
} Vape;

typedef struct{
    float x, y, z;
    float pitch, yaw;
    float mouseSpeed;
    float moveSpeed;
    bool isInvertedMouseY;
    int screenWidth;
    int screenHeight;
    bool isEnabledMovement;
    bool isEnabledRotation;

    Uint32 lastShout;
    Uint32 nextShout;
    Vape vape;
    float auraLightBrightness;
} Camera;

void initialize_camera(Camera* camera);
void update_camera_view(Camera* camera);
void handle_mouse_input(SDL_Event* event, Camera* camera);
void handle_wasd_input(SDL_Event* event, Camera* camera, bool* isRunning, float deltaTime, Sounds sounds, GameScene gameScene);
void update_vaping(Camera* camera, float deltaTime);

#endif //SNAILSPACE_CAMERA_H