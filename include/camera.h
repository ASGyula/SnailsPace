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

#define BASE_MOVEMENT_SPEED 5.0f

#define SHOUT_DISTANCE 30.0f
#define SHOUT_COOLDOWN 1500
#define SHOUT_SPEED 30
#define SHOUT_SOURCE 's'
#define SHOUT_SENS 20.0f
#define STEPS_DISTANCE 20.0f
#define STEPS_COOLDOWN 600
#define STEPS_SPEED 20
#define STEPS_SOURCE 'm'
#define STEPS_SENS 2.0f
#define VAPE_DISTANCE 10.0f
#define VAPE_COOLDOWN 1000
#define VAPE_SPEED 15
#define VAPE_SOURCE 'm'
#define VAPE_SENS 1.0f
#define DECREASE_VAPE_SMOKE_AMOUNT_SEC 3

void initialize_camera(Camera* camera);
void set_camera_position_default(Camera* camera);
void set_camera_position(Camera* camera, Coordinates coordinates, float yaw, float pitch);
void update_camera_view(Camera* camera);
void update_vaping(Camera* camera, float deltaTime);

#endif //SNAILSPACE_CAMERA_H