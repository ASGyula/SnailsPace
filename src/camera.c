//
// Created by asgyu on 2026. febr. 26..
//

#include "camera.h"
#include <math.h>
#include <SDL_opengl.h>
#include <stdio.h>

void initialize_camera(Camera* camera){
    camera->x = 0.0f;
    camera->y = 1.6f;
    camera->z = 5.0f;
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    camera->mouseSpeed = 0.1f;
    camera->moveSpeed = 0.05f;
    camera->isInvertedMouseY = true;

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void handle_camera_input(SDL_Event* event, Camera* camera, bool* isRunning) {
    if(event->type == SDL_QUIT)*isRunning = false;

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
    
    if(event->type == SDL_KEYDOWN){
        float rad_yaw = camera->yaw * (M_PI/180.0f);

        switch(event->key.keysym.sym){
            case SDLK_w:
                camera->x += sinf(rad_yaw) * camera->moveSpeed;
                camera->z -= cosf(rad_yaw) * camera->moveSpeed;
                break;
            case SDLK_s:
                camera->x -= sinf(rad_yaw) * camera->moveSpeed;
                camera->z += cosf(rad_yaw) * camera->moveSpeed;
                break;
            case SDLK_a:
                camera->x -= cosf(rad_yaw) * camera->moveSpeed;
                camera->z -= sinf(rad_yaw) * camera->moveSpeed;
                break;
            case SDLK_d:
                camera->x += cosf(rad_yaw) * camera->moveSpeed;
                camera->z += sinf(rad_yaw) * camera->moveSpeed;
                break;
            case SDLK_ESCAPE:
                *isRunning = false;
                break;
            default: break;
        }
    }

}

void update_camera(Camera* camera){
    glLoadIdentity();
    glRotatef(camera->pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(camera->yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camera->x, -camera->y, -camera->z);
}