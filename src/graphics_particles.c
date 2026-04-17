//
// Created by asgyu on 2026. ápr. 17..
//

#include "graphics_particles.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>

#define MAX_PARTICLES 100

Particle smoke_particles[MAX_PARTICLES];

void spawn_smoke(PointData point_data, Camera* camera){
    float rad_yaw = camera->yaw * (M_PI / 180.0f);
    float rad_pitch = camera->pitch * (M_PI / 180.0f);

    float directionX = sinf(rad_yaw) * cosf(rad_pitch);
    float directionY = -sinf(rad_pitch);
    float directionZ = -cosf(rad_yaw) * cosf(rad_pitch);

    for(int i = 0; i < MAX_PARTICLES; i++){
        smoke_particles[i].point_data.x = point_data.x + directionX;
        smoke_particles[i].point_data.y = point_data.y + directionY - 0.2f;
        smoke_particles[i].point_data.z = point_data.z + directionZ;

        float speed = ((rand() % 100) / 200.0f) + 0.1f;
        float spread = ((rand() % 100) / 1000.0f) - 0.05f;
        smoke_particles[i].vx = directionX * speed + spread;
        smoke_particles[i].vy = directionY * speed + 0.05f;
        smoke_particles[i].vz = directionZ * speed + spread;

        smoke_particles[i].life = 1.0f;
        smoke_particles[i].size = 1.0f + (rand() % 100);
    }
}

void update_and_render_smoke(float deltaTime){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    for(int i = 0; i < MAX_PARTICLES; i++){
        if(smoke_particles[i].life > 0.0f){
            smoke_particles[i].point_data.x += smoke_particles[i].vx * deltaTime;
            smoke_particles[i].point_data.y += smoke_particles[i].vy * deltaTime;
            smoke_particles[i].point_data.z += smoke_particles[i].vz * deltaTime;

            smoke_particles[i].life -= 0.1f * deltaTime;
            smoke_particles[i].size -= 0.1f * deltaTime;

            glColor4f(0.8f, 0.8f, 0.8f, smoke_particles[i].life);
            glPointSize(smoke_particles[i].size);
            glBegin(GL_POINTS);
            glVertex3f(smoke_particles[i].point_data.x, smoke_particles[i].point_data.y, smoke_particles[i].point_data.z);
            glEnd();
        }
    }
}