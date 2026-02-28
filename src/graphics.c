//
// Created by asgyu on 2026. febr. 25..
//

#include "graphics.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define FOV 60.0f
#define farVal 100.0f
#define LIDAR_DRAW_DISTANCE 10.0f

#define MAX_WAVES 20

typedef struct {
    Uint32 start_time;
    float x, y, z;
    float speed;
    float max_distance;
    float width;
    bool active;
} SoundWave;
static SoundWave sound_waves[MAX_WAVES];

PointData* gpu_data = NULL;
void setup_projection(const int width, const int height){
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
    glDepthFunc(GL_LESS);
}

void render_lidar(const Vertex* vertices, const Camera* camera){
    const int number_of_vertices = vertices->number_of_vertex;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i<number_of_vertices;i++){
        const float dx = vertices[i].x - camera->x;
        const float dy = vertices[i].y - camera->y;
        const float dz = vertices[i].z - camera->z;

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

void render_lidar_eco(const Vertex* vertices, const Camera* camera, const Uint32 currentTime){
    const int number_of_vertices = vertices->number_of_vertex;

    glDisable(GL_CULL_FACE);
    glPointSize(10.0f);
    glBegin(GL_POINTS);

    float rad_yaw = camera->yaw * (M_PI/180.0f);
    float rad_pitch = camera->pitch * (M_PI/180.0f);

    float directionX = sinf(rad_yaw) * cosf(rad_pitch);
    float directionY = -sinf(rad_pitch);
    float directionZ = -cosf(rad_yaw) * cosf(rad_pitch);

    for(int i = 0; i < number_of_vertices; i++){
        const float dx = vertices[i].x - camera->x;
        const float dy = vertices[i].y - camera->y;
        const float dz = vertices[i].z - camera->z;
        float distanceSq = dx*dx + dy*dy + dz*dz;

        if(distanceSq > (LIDAR_DRAW_DISTANCE*LIDAR_DRAW_DISTANCE)) continue;

        float dot = directionX*dx + directionY*dy + directionZ*dz;
        if(dot > 0.0f){
            const float distance = sqrtf(distanceSq);
            if(distance < LIDAR_DRAW_DISTANCE){
                if(distance > 10){
                    const int randomHidePoint = rand()%10;
                    if(randomHidePoint <=7){
                        continue;
                    }
                }

                float intensity = 1.0f - (distance/LIDAR_DRAW_DISTANCE);
                intensity = fmaxf(0.0f, fminf(1.0f, intensity));
                float r = 1.0f - fabsf(intensity - 0.5f) * 2.0f;
                float g = fmaxf(0.0f, (intensity - 0.5f) * 2.0f);
                float b = fmaxf(0.0f, 1.0f - intensity * 2.0f);

                glColor3f(r, g, b);
                if(currentTime % 3 == 0){
                    float randomX = ((rand()%10)/2000.0f)-0.005f;
                    float randomY = ((rand()%10)/2000.0f)-0.005f;
                    float randomZ = ((rand()%10)/2000.0f)-0.005f;
                    glVertex3f(vertices[i].x + randomX, vertices[i].y + randomY, vertices[i].z + randomZ);
                }else{
                    glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
                }
           }
        }
    }

    glEnd();
}

void render_lidar_fast(int number_of_vertices){
    glDisable(GL_CULL_FACE);
    glPointSize(2.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(PointData), &gpu_data[0].x);
    glColorPointer(3, GL_FLOAT, sizeof(PointData), &gpu_data[0].r);
    glDrawArrays(GL_POINTS, 0, number_of_vertices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void prepare_lidar_data(Vertex* vertices){
    int number_of_vertices = vertices->number_of_vertex;
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


void render_model(const Model* model){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, model->textureID);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < model->number_of_vertex; i++) {
        glTexCoord2f(model->vertices[i].u, model->vertices[i].v);
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void render_model_wt(const Model* model){
    glDisable(GL_CULL_FACE);
    glColor3f(0.0f, 0.0f, 0.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < model->number_of_vertex; i++) {
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }

    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_CULL_FACE);
}

void add_sound_wave(float x, float y, float z, float speed, float max_dist, float width) {
    int index = -1;

    for(int i = 0; i < MAX_WAVES; i++){
        if(!sound_waves[i].active){
            index = i;
            break;
        }
    }

    if(index == -1){
        index = rand() % MAX_WAVES;
    }

    sound_waves[index].start_time = SDL_GetTicks();
    sound_waves[index].x = x;
    sound_waves[index].y = y;
    sound_waves[index].z = z;
    sound_waves[index].speed = speed;
    sound_waves[index].max_distance = max_dist;
    sound_waves[index].width = width;
    sound_waves[index].active = true;
}

void render_bat_vision(const Model* model, const Camera* camera, const Uint32 currentTime){
    if(!model || !model->vertices) return;

    glEnable(GL_FOG);
    float fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_MODE, GL_LINEAR);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < model->number_of_vertex; i++){
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    glEnd();

    for(int w = 0; w < MAX_WAVES; w++){
        const float waveSpeed = sound_waves[w].speed;
        Uint32 timeElapsed = currentTime - sound_waves[w].start_time;

        const float timeInSeconds = (float)timeElapsed/1000.0f;
        const float waveFront = timeInSeconds*waveSpeed;
        const float waveThickness = sound_waves[w].width;
        const float waveBack = waveFront-waveThickness;

        if(waveFront > sound_waves[w].max_distance){
            sound_waves[w].active = false;
            glFogf(GL_FOG_START, 0);
            continue;
        }

        glFogf(GL_FOG_START, waveFront);
        glFogf(GL_FOG_END, waveBack);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(0.0f, 0.0f, 0.0f);

        glBegin(GL_TRIANGLES);
        for(int i = 0; i < model->number_of_vertex; i++){
            glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
        }
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        glColor3f(0.0f, 1.0f, 1.0f);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0f, -1.0f);

        glBegin(GL_LINES);
        float originX = sound_waves[w].x;
        float originY = sound_waves[w].y;
        float originZ = sound_waves[w].z;

        for(int i = 0; i < model->number_of_vertex; i+=3){
            const float dx = model->vertices[i].x - originX;
            const float dy = model->vertices[i].y - originY;
            const float dz = model->vertices[i].z - originZ;
            const float distance = sqrtf(dx*dx + dy*dy + dz*dz);
            if(distance >= (waveBack-5.0f) && distance <= waveFront) {
                const float brightness = (distance - (waveBack-5.0f)) / (waveFront - (waveBack-5.0f));
                glColor3f(brightness, brightness, brightness);
                if(i + 2 >= model->number_of_vertex) break;
                bool is_quad_part1 = false;
                bool is_quad_part2 = false;
                if(i % 6 == 0 && i + 5 < model->number_of_vertex) {
                    if(model->vertices[i].x == model->vertices[i + 3].x &&
                        model->vertices[i].y == model->vertices[i + 3].y &&
                        model->vertices[i].z == model->vertices[i + 3].z &&
                        model->vertices[i + 2].x == model->vertices[i + 4].x &&
                        model->vertices[i + 2].y == model->vertices[i + 4].y &&
                        model->vertices[i + 2].z == model->vertices[i + 4].z) {
                        is_quad_part1 = true;

                    }
                }else if(i % 6 == 3 && i - 3 >= 0) {
                    if(model->vertices[i - 3].x == model->vertices[i].x &&
                        model->vertices[i - 3].y == model->vertices[i].y &&
                        model->vertices[i - 3].z == model->vertices[i].z &&
                        model->vertices[i - 1].x == model->vertices[i + 1].x &&
                        model->vertices[i - 1].y == model->vertices[i + 1].y &&
                        model->vertices[i - 1].z == model->vertices[i + 1].z) {
                        is_quad_part2 = true;

                    }
                }

                if(is_quad_part1) {
                    glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
                    glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                    glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                    glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);

                }else if(is_quad_part2) {
                    glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                    glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                    glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                    glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);

                }else {
                    glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
                    glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                    glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                    glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                    glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                    glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
                }
            }
        }
    }


    glEnd();
    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLuint load_texture(const char* filename){
    SDL_Surface* surface = IMG_Load(filename);
    if(!surface){
        printf("[HIBA] Nem sikerult betolteni a texturat: %s\n", IMG_GetError());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);
    return textureID;
}

void render_ui_texture(UIElement element, int screenWidth, int screenHeight, bool isShowingFog){
    if(isShowingFog)glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, element.textureID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f, 1.0f, 1.0f);

    int w = 300;
    int h = 100;
    int x = (screenWidth - w) / 2;
    int y = screenHeight - h - 20;

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + w, y);
    glTexCoord2f(1, 1); glVertex2i(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2i(x, y + h);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    if(isShowingFog)glEnable(GL_FOG);
}