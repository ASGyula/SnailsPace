//
// Created by asgyu on 2026. márc. 8..
//

#include <SDL_mixer.h>
#include "sounds.h"

void playSound(Mix_Chunk* sound){
    Mix_PlayChannel(-1, sound, 0);
}
