//
// Created by asgyu on 2026. márc. 8..
//

#ifndef SNAILSPACE_SOUNDS_H
#define SNAILSPACE_SOUNDS_H

#include <SDL_mixer.h>

typedef struct{
    Mix_Chunk* Mita_Bonk;
    Mix_Chunk* Overture;
    Mix_Chunk* ShoutSound;
    Mix_Chunk* VapeFincsiVape;
} Sounds;

void playSound(Mix_Chunk *sound);

void playSoundWithLoops(Mix_Chunk *sound, int loops);

#endif //SNAILSPACE_SOUNDS_H