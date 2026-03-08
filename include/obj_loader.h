//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_OBJ_LOADER_H
#define SNAILSPACE_OBJ_LOADER_H
#include <SDL_opengl.h>

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
    float x, y, z;
    float targetX, targetY, targetZ;
    float animSpeed;
    bool isMoving;
} MoveableModel;

typedef struct{
    Model Helsie;
    MoveableModel Dealer;
} GameObjects;

void load_obj(const char* filename, Vertex** vertices);

bool load_textured_obj(const char* filename, Model* model);

#endif //SNAILSPACE_OBJ_LOADER_H