//
// Created by asgyu on 2026. febr. 26..
//

#include "camera.h"
#include <SDL_opengl.h>
#include <SDL_timer.h>
#include <stdio.h>

#include "graphics.h"
#include "world_builder.h"

void initialize_camera(Camera* camera){
    camera->x = 0.0f;
    camera->y = 1.6f;
    camera->z = 5.0f;
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    camera->mouseSpeed = 0.1f;
    camera->moveSpeed = 1.0f;
    camera->isEnabledMovement = false;
    camera->isEnabledRotation = true;

    camera->vape.isVaping = false;
    camera->vape.smokeAmount = 0.0f;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void set_camera_position_default(Camera* camera){
    camera->x = 0.0f;
    camera->y = 1.6f;
    camera->z = 5.0f;
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    camera->mouseSpeed = 0.1f;
    camera->moveSpeed = 1.0f;
    camera->isEnabledMovement = false;
    camera->isEnabledRotation = true;
}

void update_camera_view(Camera* camera){
    glLoadIdentity();
    glRotatef(camera->pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(camera->yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camera->x, -camera->y, -camera->z);
}

void update_vaping(Camera* camera, float deltaTime){
    if(camera->vape.isVaping){
        enable_vape_light(&camera->vape);
        camera->vape.usedLiquid+=0.05f*deltaTime;
        camera->vape.smokeAmount += 0.25f * deltaTime;
        if(camera->vape.smokeAmount >= 1.0f)camera->vape.smokeAmount = 1.0f;
    }else{
        Uint32 currentTime = SDL_GetTicks();
        if((camera->vape.endedTime+DECREASE_VAPE_SMOKE_AMOUNT_SEC*1000) < currentTime){
            camera->vape.smokeAmount -= 0.11f * deltaTime;
            if(camera->vape.smokeAmount <= 0.0f)camera->vape.smokeAmount = 0.0f;
        }
    }

    if(camera->vape.smokeAmount <= 0.0f){
        enableFog(10,30,0.4f);
    }else{
        float fogDensity = 0.01f + (camera->vape.smokeAmount * 0.1f);
        glFogf(GL_FOG_DENSITY, fogDensity);
        enableFog(1-(camera->vape.smokeAmount), 15.0f - (camera->vape.smokeAmount * 10.0f), camera->vape.smokeAmount);
    }
}