//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_GRAPHICS_H
#define SNAILSPACE_GRAPHICS_H
#include "obj_loader.h"

void setup_projection(int width, int height);

void render_lidar(const Vertex* vertices, int number_of_vertices, float x, float y, float z);

#endif //SNAILSPACE_GRAPHICS_H