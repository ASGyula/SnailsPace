//
// Created by asgyu on 2026. ápr. 17..
//

#ifndef SNAILSPACE_GRAPHICS_PARTICLES_H
#define SNAILSPACE_GRAPHICS_PARTICLES_H

#include "graphics.h"

void spawn_smoke(PointData point_data, Camera* camera);
void update_and_render_smoke(float deltaTime);

#endif // SNAILSPACE_GRAPHICS_PARTICLES_H