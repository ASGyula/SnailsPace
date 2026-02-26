//
// Created by asgyu on 2026. febr. 25..
//

#ifndef SNAILSPACE_OBJ_LOADER_H
#define SNAILSPACE_OBJ_LOADER_H

typedef struct{
    float x, y, z;
} Vertex;

void load_obj(const char* filename, Vertex** vertices, int* number_of_vertices);


#endif //SNAILSPACE_OBJ_LOADER_H