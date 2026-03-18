//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_GRAPHICS_H
#define SNAILSPACE_GRAPHICS_H
#include <SDL_opengl.h>

#include "camera.h"
#include "obj_loader.h"

typedef struct{
    float x, y, z;
    float r, g, b;
} PointData;

typedef struct{
    PointData point_data;
    float vx, vy, vz;
    float life;
    float size;
} Particle;

void setup_projection(int width, int height);

void render_lidar(const Vertex* vertices, const Camera* camera);
void render_lidar_eco(const Vertex* vertices, const Camera* camera, Uint32 currentTime);

void prepare_lidar_data(Vertex* vertices);
void render_lidar_fast(int number_of_vertices);

GLuint load_texture(const char* filename);

void render_model(const Model* model);
void render_model_without_texture(const Model* model);

void enableFog(float start, float end, float alpha);
void disableFog();

void render_bat_vision(const Model* model, Uint32 currentTime);

void add_sound_wave(float x, float y, float z, float speed, float max_dist, float width, char source);

void update_moveable_model_position(MoveableModel* objectum, float deltaTime);

void render_moveable_model(MoveableModel* object);

void render_vape_in_hand(MoveableModel* object, Camera* camera);

void update_snail_ai(MoveableModel* monster, Camera* player, float deltaTime);

void enable_vape_light(Vape* vape);
void disable_vape_light();
void spawn_smoke(PointData point_data, Camera* camera);
void update_and_render_smoke(float deltaTime);

void render_light_aura_model(Camera* camera, LightAuraModel* model);

#endif //SNAILSPACE_GRAPHICS_H