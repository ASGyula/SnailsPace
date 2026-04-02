//
// Created by asgyu on 2026. márc. 22..
//


#include <math.h>
#include <SDL_events.h>
#include <stdio.h>

#include "camera.h"
#include "game_manager.h"
#include "graphics.h"

void handle_mouse_invert_input(SDL_Event* event, Camera* camera){
    if(camera->isEnabledRotation){
        if(event->type == SDL_MOUSEMOTION){
            camera->yaw += event->motion.xrel * camera->mouseSpeed;
            float randomFloat = rand()%10 + 1;
            if(camera->isInvertedMouseY){
                camera->pitch += event->motion.yrel * camera->mouseSpeed*randomFloat;
            }else{
                camera->pitch -= event->motion.yrel * camera->mouseSpeed*randomFloat;
            }

            if(camera->pitch > 89.0f)camera->pitch=89.0f;
            if(camera->pitch < -89.0f)camera->pitch=-89.0f;
        }
    }
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

void handle_esc_input(SDL_Event* event, Game* game, bool* isRunning, GameScene* gameScene){
    static bool is_esc_pressed = false;

    if(event->type == SDL_QUIT)*isRunning = false;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE]){
        if(!is_esc_pressed){
            if(*gameScene == MAIN_MENU){
                *isRunning = false;
            }else{
                scene_switch(game, MAIN_MENU);
            }
            is_esc_pressed = true;
        }
    }else{
        is_esc_pressed = false;
    }
}

void handle_wasd_input(Camera* camera, float deltaTime, Sounds sounds, GameScene gameScene){

    const Uint8* state = SDL_GetKeyboardState(NULL);
    float rad_yaw = camera->yaw * (M_PI/180.0f);
    float moveSpeed = camera->moveSpeed * deltaTime * BASE_MOVEMENT_SPEED;
    float dirX = 0, dirZ = 0;
    bool isEnabledSoundWave = gameScene == BAT_VISION || gameScene == DEALER_ROOM;

    if(camera->isEnabledMovement){
        if(state[SDL_SCANCODE_W]){
            dirX += sinf(rad_yaw);
            dirZ -= cosf(rad_yaw);
        }
        if(state[SDL_SCANCODE_S]){
            dirX -= sinf(rad_yaw);
            dirZ += cosf(rad_yaw);
        }
        if(state[SDL_SCANCODE_A]){
            dirX -= cosf(rad_yaw);
            dirZ -= sinf(rad_yaw);
        }
        if(state[SDL_SCANCODE_D]){
            dirX += cosf(rad_yaw);
            dirZ += sinf(rad_yaw);
        }

        float length = sqrtf(dirX * dirX + dirZ * dirZ);
        if(length > 0.0f){
            dirX /= length;
            dirZ /= length;

            camera->x += dirX * moveSpeed;
            camera->z += dirZ * moveSpeed;

            static Uint32 nextStepWave = 0;
            if(SDL_GetTicks() > nextStepWave && isEnabledSoundWave){
                add_sound_wave(camera->x, camera->y, camera->z, STEPS_SPEED, STEPS_DISTANCE, STEPS_SENS, STEPS_SOURCE);
                nextStepWave = SDL_GetTicks() + STEPS_COOLDOWN;
            }
        }

        if(state[SDL_SCANCODE_SPACE]){
            // if(SDL_GetTicks() - camera->lastShout > SHOUT_COOLDOWN  && isEnabledSoundWave){
            if(SDL_GetTicks() - camera->lastShout > SHOUT_COOLDOWN){
                printf("(x, y, z) = (%f %f %f)\n (p/w) = (%f, %f)\n", camera->x, camera->y, camera->z, camera->pitch, camera->yaw);
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
}
