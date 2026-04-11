//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_OBJ_LOADER_H
#define SNAILSPACE_OBJ_LOADER_H
#include <SDL_opengl.h>

#include "game_types.h"

typedef struct{
    float x, y, z;
} Coordinates;

typedef struct{
    float x, y, z;
    int number_of_vertex;
} Vertex;

typedef struct{
    float x, y, z;
    float u, v;
    float nx, ny, nz;
    GLboolean edgeFlag;
} ModelVertex;

typedef struct{
    ModelVertex* vertices;
    int number_of_vertex;
    GLuint textureID;
} Model;

typedef struct{
    Model model;
    float x, y, z, a;
    float yaw, pitch, roll;
    float r, g, b;
    float radius;
    TriggerZone triggerZone;
} LightAuraModel;

typedef struct{
    Model model;
    float x, y, z;
    float yaw, pitch, roll;
    float targetX, targetY, targetZ;
    float animSpeed;
    bool isMoving;
    TriggerZone triggerZone;
} MoveableModel;

typedef struct {
    Model Helsie;
    MoveableModel Mita;
    Model MitasRoom;
    Model BatVisionMap;
    Model LidarMapWT;
    Vertex* LidarMap;
    LightAuraModel PreLidarMap;
    MoveableModel Dealer;
    MoveableModel Vapelt3;
    LightAuraModel PunchPacificMonster;
    LightAuraModel WhiteMonster;
    LightAuraModel Scissors;
    MoveableModel ImmortalSnail;
    MoveableModel SpookyJumpscare;
} GameObjects;

void load_obj(const char* filename, Vertex** vertices);

bool load_textured_obj(const char* filename, Model* model);

void change_loaded_moveable_obj_positon(MoveableModel* object, float x, float y, float z, float pitch, float yaw, float roll);

Vertex* load_vertex_binary(const char* filename);

bool load_model_binary(const char* name, Model* model);

#endif //SNAILSPACE_OBJ_LOADER_H