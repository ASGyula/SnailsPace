//
// Created by asgyu on 2026. márc. 22..
//

#ifndef SNAILSPACE_INPUT_H
#define SNAILSPACE_INPUT_H
#include <SDL_events.h>

#include "camera.h"
#include "game_manager.h"

void handle_mouse_input(SDL_Event* event, Camera* camera);
void handle_mouse_invert_input(SDL_Event* event, Camera* camera);
void handle_esc_input(SDL_Event* event, Game* game, bool* isRunning, GameScene* gameScene);
void handle_wasd_input(Camera* camera, float deltaTime, Sounds sounds, GameScene gameScene);

void handle_miside_interact_input(Camera* camera, float deltaTime, VisualNovelState* visual_novel_state, TriggerZone* trigger_zone);

#endif //SNAILSPACE_INPUT_H