//
// Created by asgyu on 2026. febr. 26..
//

#include "camera.h"
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <SDL_timer.h>
#include <stdio.h>

#include "graphics.h"

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

void initialize_camera(Camera* camera){
    camera->x = 0.0f;
    camera->y = 1.6f;
    camera->z = 5.0f;
    camera->yaw = 0.0f;
    camera->pitch = 0.0f;
    camera->mouseSpeed = 0.1f;
    camera->moveSpeed = 1.0f;
    camera->isInvertedMouseY = true;
    camera->isEnabledMovement = false;
    camera->isEnabledRotation = true;

    camera->vape.isVaping = false;
    camera->vape.smokeAmount = 0.0f;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void handle_mouse_input(SDL_Event* event, Camera* camera){
    if(camera->isEnabledRotation){
        if(event->type == SDL_MOUSEMOTION){
            camera->yaw += event->motion.xrel * camera->mouseSpeed;
            if(camera->isInvertedMouseY){
                camera->pitch -= event->motion.yrel * camera->mouseSpeed;
            }else{
                camera->pitch += event->motion.yrel * camera->mouseSpeed;
            }

            if(camera->pitch > 89.0f)camera->pitch=89.0f;
            if(camera->pitch < -89.0f)camera->pitch=-89.0f;
        }
    }
}

void handle_wasd_input(SDL_Event* event, Camera* camera, bool* isRunning, float deltaTime, Sounds sounds, GameScene gameScene){
    if(event->type == SDL_QUIT)*isRunning = false;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    float rad_yaw = camera->yaw * (M_PI/180.0f);

    float moveSpeed = camera->moveSpeed * deltaTime * BASE_MOVEMENT_SPEED;
    static Uint32 nextStepWave = 0;

    bool isEnabledSoundWave = gameScene == BAT_VISION || gameScene == DEALER_ROOM;

    if(camera->isEnabledMovement){
        if(state[SDL_SCANCODE_W]){
            camera->x += sinf(rad_yaw) * moveSpeed;
            camera->z -= cosf(rad_yaw) * moveSpeed;
            if(SDL_GetTicks() > nextStepWave && isEnabledSoundWave){
                add_sound_wave(camera->x, camera->y, camera->z, STEPS_SPEED, STEPS_DISTANCE, STEPS_SENS, STEPS_SOURCE);
                nextStepWave = SDL_GetTicks() + STEPS_COOLDOWN;
            }
        }
        if(state[SDL_SCANCODE_A]){
            camera->x -= cosf(rad_yaw) * moveSpeed;
            camera->z -= sinf(rad_yaw) * moveSpeed;
            if(SDL_GetTicks() > nextStepWave && isEnabledSoundWave){
                add_sound_wave(camera->x, camera->y, camera->z, STEPS_SPEED, STEPS_DISTANCE, STEPS_SENS, STEPS_SOURCE);
                nextStepWave = SDL_GetTicks() + STEPS_COOLDOWN;
            }
        }
        if(state[SDL_SCANCODE_S]){
            camera->x -= sinf(rad_yaw) * moveSpeed;
            camera->z += cosf(rad_yaw) * moveSpeed;
            if(SDL_GetTicks() > nextStepWave && isEnabledSoundWave){
                add_sound_wave(camera->x, camera->y, camera->z, STEPS_SPEED, STEPS_DISTANCE, STEPS_SENS, STEPS_SOURCE);
                nextStepWave = SDL_GetTicks() + STEPS_COOLDOWN;
            }
        }
        if(state[SDL_SCANCODE_D]){
            camera->x += cosf(rad_yaw) * moveSpeed;
            camera->z += sinf(rad_yaw) * moveSpeed;
            if(SDL_GetTicks() > nextStepWave && isEnabledSoundWave){
                add_sound_wave(camera->x, camera->y, camera->z, STEPS_SPEED, STEPS_DISTANCE, STEPS_SENS, STEPS_SOURCE);
                nextStepWave = SDL_GetTicks() + STEPS_COOLDOWN;
            }
        }

        if(state[SDL_SCANCODE_SPACE]){
            if(SDL_GetTicks() - camera->lastShout > SHOUT_COOLDOWN  && isEnabledSoundWave){
                printf("%f %f\n", camera->x, camera->z);
                playSound(sounds.ShoutSound);
                add_sound_wave(camera->x, camera->y, camera->z, SHOUT_SPEED, SHOUT_DISTANCE, SHOUT_SENS, SHOUT_SOURCE);
                camera->lastShout = SDL_GetTicks();
                camera->nextShout = SDL_GetTicks() + SHOUT_COOLDOWN;
            }
        }

        if(state[SDL_SCANCODE_V] && isEnabledSoundWave){
            camera->vape.isVaping = true;
            playSound(sounds.VapeFincsiVape);
            add_sound_wave(camera->x, camera->y, camera->z, VAPE_SPEED, VAPE_DISTANCE, VAPE_SENS, VAPE_SOURCE);
        }else{
            if(camera->vape.isVaping){
                camera->vape.endedTime = SDL_GetTicks();
                PointData point_data = {camera->x, camera->y, camera->z};
                spawn_smoke(point_data, camera);
            }
            camera->vape.isVaping = false;
            disable_vape_light();
        }
    }

    static bool kp_minus_pressed = false;
    if(state[SDL_SCANCODE_KP_MINUS]){
        if(!kp_minus_pressed){
            float newBrightnessValue = fmaxf(0.0f, camera->auraLightBrightness - 0.1f);
            camera->auraLightBrightness = newBrightnessValue;
            kp_minus_pressed = true;
        }
    }else{
        kp_minus_pressed = false;
    }

    static bool kp_plus_pressed = false;
    if(state[SDL_SCANCODE_KP_PLUS]){
        if(!kp_plus_pressed){
            float newBrightnessValue = fminf(1.0f, camera->auraLightBrightness + 0.1f);
            camera->auraLightBrightness = newBrightnessValue;
            kp_plus_pressed = true;
        }
    }else{
        kp_plus_pressed = false;
    }


    if(state[SDL_SCANCODE_ESCAPE]){
        *isRunning = false;
    }
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
        disableFog();
    }else{
        printf("%.2f\n", camera->vape.smokeAmount);
        float fogDensity = 0.01f + (camera->vape.smokeAmount * 0.1f);
        glFogf(GL_FOG_DENSITY, fogDensity);
        enableFog(1-(camera->vape.smokeAmount), 15.0f - (camera->vape.smokeAmount * 10.0f), camera->vape.smokeAmount);
    }
}