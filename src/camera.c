//
// Created by asgyu on 2026. febr. 26..
//

#include "camera.h"
#include <math.h>
#include <SDL_opengl.h>
#include <SDL_timer.h>
#include <stdio.h>

#include "graphics.h"

#define BASE_MOVEMENT_SPEED 5.0f

#define SHOUT_SHOUT_DISTANCE 20.0f
#define SHOUT_SHOUT_DURATION 3000
#define SHOUT_SHOUT_DELAY 200
#define MOVEMENT_SHOUT_DISTANCE 10.0f
#define MOVEMENT_SHOUT_DURATION 3000
#define MOVEMENT_SHOUT_DELAY 100

void initialize_camera(Camera* camera){
    camera->x = 0.0f;
    camera->y = 1.6f;
    camera->z = 5.0f;
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    camera->mouseSpeed = 0.1f;
    camera->moveSpeed = 1.0f;
    camera->isInvertedMouseY = true;

    SDL_SetRelativeMouseMode(SDL_TRUE);
}


void addSouthEffect(Camera* camera, bool isShoutSource, Uint32 startTime, Uint16 duration, Uint16 delay, float sens){
    camera->shout.isSouthSource = isShoutSource;
    camera->shout.startTime = startTime;
    camera->shout.duration = duration;
    camera->shout.delay = delay;
    camera->shout.sens = sens;
}

void handle_camera_input(SDL_Event* event, Camera* camera){
    if(event->type == SDL_MOUSEMOTION){
        camera->yaw += event->motion.xrel * camera->mouseSpeed;
        if(camera->isInvertedMouseY) {
            camera->pitch -= event->motion.yrel * camera->mouseSpeed;
        }else{
            camera->pitch += event->motion.yrel * camera->mouseSpeed;
        }

        if(camera->pitch > 89.0f)camera->pitch=89.0f;
        if(camera->pitch < -89.0f)camera->pitch=-89.0f;
    }
}

void handle_wasd_input(SDL_Event* event, Camera* camera, bool* isRunning, float deltaTime){
    if(event->type == SDL_QUIT)*isRunning = false;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    float rad_yaw = camera->yaw * (M_PI/180.0f);

    float moveSpeed = camera->moveSpeed * deltaTime * BASE_MOVEMENT_SPEED;

    if(state[SDL_SCANCODE_W]){
        camera->x += sinf(rad_yaw) * moveSpeed;
        camera->z -= cosf(rad_yaw) * moveSpeed;
        addSouthEffect(camera, false, SDL_GetTicks(), MOVEMENT_SHOUT_DURATION, MOVEMENT_SHOUT_DELAY, MOVEMENT_SHOUT_DISTANCE);
    }
    if(state[SDL_SCANCODE_A]){
        camera->x -= cosf(rad_yaw) * moveSpeed;
        camera->z -= sinf(rad_yaw) * moveSpeed;
        addSouthEffect(camera, false, SDL_GetTicks(), MOVEMENT_SHOUT_DURATION, MOVEMENT_SHOUT_DELAY, MOVEMENT_SHOUT_DISTANCE);
    }
    if(state[SDL_SCANCODE_S]){
        camera->x -= sinf(rad_yaw) * moveSpeed;
        camera->z += cosf(rad_yaw) * moveSpeed;
        addSouthEffect(camera, false, SDL_GetTicks(), MOVEMENT_SHOUT_DURATION, MOVEMENT_SHOUT_DELAY, MOVEMENT_SHOUT_DISTANCE);
    }
    if(state[SDL_SCANCODE_D]){
        camera->x += cosf(rad_yaw) * moveSpeed;
        camera->z += sinf(rad_yaw) * moveSpeed;
        addSouthEffect(camera, false, SDL_GetTicks(), MOVEMENT_SHOUT_DURATION, MOVEMENT_SHOUT_DELAY, MOVEMENT_SHOUT_DISTANCE);
    }

    if(state[SDL_SCANCODE_SPACE]){
        if((SDL_GetTicks() - camera->shout.startTime > camera->shout.duration + camera->shout.delay) && !camera->shout.isSouthSource){
            addSouthEffect(camera, true, SDL_GetTicks(), SHOUT_SHOUT_DURATION, SHOUT_SHOUT_DELAY, SHOUT_SHOUT_DISTANCE);
        }
    }

    if(state[SDL_SCANCODE_ESCAPE]){
        *isRunning = false;
    }
}


void update_camera(Camera* camera){
    glLoadIdentity();
    glRotatef(camera->pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(camera->yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camera->x, -camera->y, -camera->z);
}