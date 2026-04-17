//
// Created by asgyu on 2026. ápr. 17..
//

#ifndef SNAILSPACE_GRAPHICS_LIGHTING_H
#define SNAILSPACE_GRAPHICS_LIGHTING_H

#include "graphics.h"

void enable_vape_light(Camera* camera);
void disable_vape_light(void);
void render_light_aura_model(Camera* camera, LightAuraModel* model);
void enable_snail_caught_lights(MoveableModel* model, Uint32 currentTime, Uint32 caughtTime);
void enable_pre_lidar_lights(LightAuraModel* map, Camera* camera);

#endif // SNAILSPACE_GRAPHICS_LIGHTING_H