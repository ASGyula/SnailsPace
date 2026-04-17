//
// Created by asgyu on 2026. ápr. 17..
//

#ifndef SNAILSPACE_SCENE_H
#define SNAILSPACE_SCENE_H

#include <SDL.h>
#include "game_manager.h"

void handle_scene_events(Game* game, SDL_Event* event);
void render_scene(Game* game, Uint32 currentTime, float deltaTime);

#endif // SNAILSPACE_SCENE_H