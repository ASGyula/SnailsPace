//
// Created by asgyu on 2026. febr. 26..
//

#ifndef SNAILSPACE_CAMERA_H
#define SNAILSPACE_CAMERA_H
#include <SDL_events.h>

typedef struct{
    float x, y, z;
    float pitch, yaw;
    float mouseSpeed;
    float moveSpeed;
    bool isInvertedMouseY;
} Camera;

void initialize_camera(Camera* camera);
void update_camera(Camera* camera);
void handle_camera_input(SDL_Event* event, Camera* camera);
void handle_wasd_input(SDL_Event* event, Camera* camera, bool* isRunning, float deltaTime);

#endif //SNAILSPACE_CAMERA_H