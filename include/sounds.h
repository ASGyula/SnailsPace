//
// Created by asgyu on 2026. márc. 8..
//

#ifndef SNAILSPACE_SOUNDS_H
#define SNAILSPACE_SOUNDS_H

#include <SDL_mixer.h>

typedef struct{
    Mix_Chunk* Overture;
    Mix_Chunk* SoutSound;
} Sounds;

void playSound(Mix_Chunk *sound);

#endif //SNAILSPACE_SOUNDS_H