//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_GRAPHICS_H
#define SNAILSPACE_GRAPHICS_H
#include <stddef.h>

#include "obj_loader.h"

typedef struct{
    float x, y, z;
    float r, g, b;
} PointData;

void setup_projection(int width, int height);

void render_lidar(const Vertex* vertices, int number_of_vertices, float x, float y, float z);

void prepare_lidar_data(Vertex* vertices, int number_of_vertices);

void render_lidar_fast(int number_of_vertices);


#endif //SNAILSPACE_GRAPHICS_H