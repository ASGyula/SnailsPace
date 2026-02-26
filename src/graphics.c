//
// Created by asgyu on 2026. febr. 25..
//

#include "graphics.h"
#include <SDL_opengl.h>
#include <math.h>
#include <stdio.h>

#define FOV 60.0f
#define farVal 100.0f
#define LIDAR_DRAW_DISTANCE 10.0f

PointData* gpu_data = NULL;

void setup_projection(const int width, const int height){
    // glEnable(GL_FOG);
    // float fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glFogfv(GL_FOG_COLOR, fogColor);
    // glFogf(GL_FOG_MODE, GL_LINEAR);
    // glFogf(GL_FOG_START, 5.0f);
    // glFogf(GL_FOG_END, 25.0f);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width/(float)height;
    float nearVal = 0.1f;
    float top = nearVal * tanf(FOV * 0.5f * (M_PI/180.0f));
    float right = top*aspect;

    glFrustum(-right, right, -top, top, nearVal, farVal);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void render_lidar(const Vertex* vertices, const int number_of_vertices, const float x, const float y, const float z){
    // update_camera() kezeli a Camerában
    // glLoadIdentity();
    // glTranslatef(-x, -y, -z);

    // glPointSize(4.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i<number_of_vertices;i++){
        const float dx = vertices[i].x - x;
        const float dy = vertices[i].y - y;
        const float dz = vertices[i].z - z;

        const float distance = sqrtf(dx*dx + dy*dy + dz*dz);
        if(distance < LIDAR_DRAW_DISTANCE){
            if(distance > 10){
                const int randomHidePoint = rand()%10;
                if(randomHidePoint <=5){
                    continue;
                }
            }

            float intensity = 1.0f - (distance/LIDAR_DRAW_DISTANCE);
            intensity = fmaxf(0.0f, fminf(1.0f, intensity));
            float r = 1.0f - fabsf(intensity - 0.5f) * 2.0f;
            float g = fmaxf(0.0f, (intensity - 0.5f) * 2.0f);
            float b = fmaxf(0.0f, 1.0f - intensity * 2.0f);

            glColor3f(r, g, b);

            float randomX = ((rand()%10)/2000.0f)-0.005f;
            float randomY = ((rand()%10)/2000.0f)-0.005f;
            float randomZ = ((rand()%10)/2000.0f)-0.005f;
            // float randomX = ((rand()%10)/20.0f)-0.005f;
            // float randomY = ((rand()%10)/20.0f)-0.005f;
            // float randomZ = ((rand()%10)/20.0f)-0.005f;

            glVertex3f(vertices[i].x + randomX, vertices[i].y + randomY, vertices[i].z + randomZ);
        }
    }

    glEnd();
}

void render_lidar_fast(int number_of_vertices){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(PointData), &gpu_data[0].x);
    glColorPointer(3, GL_FLOAT, sizeof(PointData), &gpu_data[0].r);
    glDrawArrays(GL_POINTS, 0, number_of_vertices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void prepare_lidar_data(Vertex* vertices, int number_of_vertices){
    gpu_data = malloc(sizeof(PointData)*number_of_vertices);

    for(int i = 0; i < number_of_vertices; i++){
        gpu_data[i].x = vertices[i].x + ((rand()%100)/5000.0f);
        gpu_data[i].y = vertices[i].y + ((rand()%100)/5000.0f);
        gpu_data[i].z = vertices[i].z + ((rand()%100)/5000.0f);

        float h = (vertices[i].y + 1.0f) / 3.0f;
        gpu_data[i].r = 0.1f;
        gpu_data[i].g = h;
        gpu_data[i].b = 1.0f-h;
    }
}