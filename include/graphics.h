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

void render_lidar(const Vertex* vertices, const Camera* camera);
void render_lidar_eco(const Vertex* vertices, const Camera* camera, const Uint32 currentTime);

void prepare_lidar_data(Vertex* vertices);
void render_lidar_fast(int number_of_vertices);

GLuint load_texture(const char* filename);

void render_model(const Model* model);
void render_model_wt(const Model* model);

void render_bat_vision(const Model* model, const Camera* camera, const Uint32 currentTime);

void render_ui_texture(UIElement element, int screenWidth, int screenHeight, bool isShowingFog);
void trigger_bat_scream(Camera* camera);
void trigger_footsteps(Camera* camera);

void add_sound_wave(float x, float y, float z, float speed, float max_dist, float width);

#endif //SNAILSPACE_GRAPHICS_H