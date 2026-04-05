//
// Created by asgyu on 2026. márc. 8..
//

#include <SDL_mixer.h>
#include "sounds.h"

#include <stdio.h>

void playSound(Mix_Chunk* sound){
    if(sound != NULL){
        Mix_PlayChannel(-1, sound, 0);
    }
}

void playSoundWithLoops(Mix_Chunk* sound, int loops){
    if(sound != NULL){
        Mix_PlayChannel(-1, sound, loops);
    }
}

void stopSounds(){
    Mix_HaltChannel(-1);
}