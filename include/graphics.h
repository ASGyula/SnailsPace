//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_GRAPHICS_H
#define SNAILSPACE_GRAPHICS_H
#include <SDL_opengl.h>
#include <stddef.h>

#include "camera.h"
#include "obj_loader.h"

typedef struct{
    float x, y, z;
    float r, g, b;
} PointData;

void setup_projection(int width, int height);

void render_lidar(const Vertex* vertices, int number_of_vertices, const Camera* camera);

void prepare_lidar_data(Vertex* vertices);

void render_lidar_fast(int number_of_vertices);

GLuint load_texture(const char* filename);

void render_model(const Model* model, GLuint textureID);

#endif //SNAILSPACE_GRAPHICS_H