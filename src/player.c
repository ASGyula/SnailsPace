//
// Created by asgyu on 2026. márc. 8..
//

#include <math.h>

#include "camera.h"
#include "obj_loader.h"

void check_player_collision(Camera* camera, const Model* model, float playerRadius){
    if(!model || !model->vertices) return;

    for(int i = 0; i < model->number_of_vertex; i++){
        float dx = camera->x - model->vertices[i].x;
        float dy = camera->y - model->vertices[i].y;
        float dz = camera->z - model->vertices[i].z;

        float distance = sqrtf(dx * dx + dy * dy + dz * dz);

        if(distance < playerRadius){
            float overlap = playerRadius - distance;

            float nx = dx / distance;
            float ny = dy / distance;
            float nz = dz / distance;

            camera->x += nx * overlap;
            camera->y += ny * overlap;
            camera->z += nz * overlap;
        }
    }
}
