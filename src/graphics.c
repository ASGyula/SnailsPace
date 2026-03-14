//
// Created by asgyu on 2026. febr. 25..
//

#include "graphics.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define FOV 60.0f
#define farVal 100.0f
#define LIDAR_DRAW_DISTANCE 10.0f

#define QUEUE_SIZE 10

#define ASSETS_PREFIX "../assets/"
#define MAX_PARTICLES 100
Particle smoke_particles[MAX_PARTICLES];

typedef struct{
    Uint32 start_time;
    float x, y, z;
    float speed;
    float max_distance;
    float width;
    bool active;
    char source;
} SoundWave;

static SoundWave wave_queue[QUEUE_SIZE];
static int queue_count = 0;
static SoundWave active_wave;
static bool is_wave_running = false;

void add_sound_wave(float x, float y, float z, float speed, float max_dist, float width, char source){
    if(queue_count < QUEUE_SIZE){
        wave_queue[queue_count].x = x;
        wave_queue[queue_count].y = y;
        wave_queue[queue_count].z = z;
        wave_queue[queue_count].speed = speed;
        wave_queue[queue_count].max_distance = max_dist;
        wave_queue[queue_count].width = width;
        wave_queue[queue_count].start_time = SDL_GetTicks();
        wave_queue[queue_count].active = true;
        wave_queue[queue_count].source = source;
        queue_count++;
    }
}

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
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, model->textureID);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < model->number_of_vertex; i++){
        if(model->vertices[i].nx == 0 && model->vertices[i].ny == 0 && model->vertices[i].nz == 0)glNormal3f(0.0f, 0.0f, 1.0f);
        else glNormal3f(model->vertices[i].nx, model->vertices[i].ny, model->vertices[i].nz);
        glTexCoord2f(model->vertices[i].u, model->vertices[i].v);
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void render_model_without_texture(const Model* model){
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

void enableFog(float start, float end, float alpha){
    glEnable(GL_FOG);

    float fogColor[] = {0.0f, 0.0f, 0.0f, alpha};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
}

void disableFog(){
    glDisable(GL_FOG);
}

void render_bat_vision(const Model* model, const Uint32 currentTime){
    if(!model || !model->vertices){
        printf("[HIBA] render_bat_vision: nem talalhato modell");
        return;
    }
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f);

    if(!is_wave_running && queue_count > 0){
        Uint32 requestTime = wave_queue[0].start_time;
        Uint32 waitTime = currentTime - requestTime;

        // if(waitTime > (wave_queue[0].speed * wave_queue[0].max_distance) * 1000.0f){
        if(waitTime > 600){
            for(int i = 0; i < queue_count - 1; i++){
                wave_queue[i] = wave_queue[i + 1];
            }
            queue_count--;
            return;
        }

        active_wave = wave_queue[0];
        active_wave.start_time = currentTime;
        is_wave_running = true;

        for(int i = 0; i < queue_count - 1; i++){
            wave_queue[i] = wave_queue[i + 1];
        }
        queue_count--;
    }

    if(!is_wave_running) return;

    Uint32 elapsed = currentTime - active_wave.start_time;
    float waveFront = (elapsed / 1000.0f) * active_wave.speed;
    float waveBack = waveFront - active_wave.width;

    if(waveFront > active_wave.max_distance){
        is_wave_running = false;
        return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < model->number_of_vertex; i++){
        glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);

    for(int i = 0; i < model->number_of_vertex; i += 3){
        float dx = model->vertices[i].x - active_wave.x;
        float dz = model->vertices[i].z - active_wave.z;
        float dist = sqrtf(dx*dx + dz*dz);

        if(dist >= (waveBack-active_wave.width) && dist <= (waveFront+active_wave.width)){
            const float brightness = (dist - (waveBack-5.0f)) / (waveFront - (waveBack-5.0f));
            glColor3f(brightness, brightness, brightness);
            if(i + 2 >= model->number_of_vertex) break;
            bool is_quad_part1 = false;
            bool is_quad_part2 = false;
            if(i % 6 == 0 && i + 5 < model->number_of_vertex){
                if(model->vertices[i].x == model->vertices[i + 3].x &&
                    model->vertices[i].y == model->vertices[i + 3].y &&
                    model->vertices[i].z == model->vertices[i + 3].z &&
                    model->vertices[i + 2].x == model->vertices[i + 4].x &&
                    model->vertices[i + 2].y == model->vertices[i + 4].y &&
                    model->vertices[i + 2].z == model->vertices[i + 4].z){
                        is_quad_part1 = true;
                }
            }else if(i % 6 == 3 && i - 3 >= 0){
                if(model->vertices[i - 3].x == model->vertices[i].x &&
                    model->vertices[i - 3].y == model->vertices[i].y &&
                    model->vertices[i - 3].z == model->vertices[i].z &&
                    model->vertices[i - 1].x == model->vertices[i + 1].x &&
                    model->vertices[i - 1].y == model->vertices[i + 1].y &&
                    model->vertices[i - 1].z == model->vertices[i + 1].z){
                        is_quad_part2 = true;
                    }
            }

            if(is_quad_part1){
                glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
                glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
            }else if(is_quad_part2){
                glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
            }else{
                glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
                glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                glVertex3f(model->vertices[i + 1].x, model->vertices[i + 1].y, model->vertices[i + 1].z);
                glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                glVertex3f(model->vertices[i + 2].x, model->vertices[i + 2].y, model->vertices[i + 2].z);
                glVertex3f(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
            }
        }
    }

    glEnd();

    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLuint load_texture(const char* name){
    printf("[INFO] Textura betoltese: %s\n", name);
    char filename[256];
    snprintf(filename, sizeof(filename), "%s%s", ASSETS_PREFIX, name);
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

void update_moveable_model_position(MoveableModel* objectum, float deltaTime){
    if(!objectum->isMoving) return;

    float dx = objectum->targetX - objectum->x;
    float dy = objectum->targetY - objectum->y;
    float dz = objectum->targetZ - objectum->z;
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);

    if(distance < 0.01f){
        objectum->x = objectum->targetX;
        objectum->y = objectum->targetY;
        objectum->z = objectum->targetZ;
        objectum->isMoving = false;
        return;
    }

    objectum->x += (dx/distance)*objectum->animSpeed*deltaTime;
    objectum->y += (dy/distance)*objectum->animSpeed*deltaTime;
    objectum->z += (dz/distance)*objectum->animSpeed*deltaTime;

}

void render_moveable_model(MoveableModel* object){
    glPushMatrix();
    glTranslatef(object->x, object->y, object->z);
    render_model(&object->model);
    glPopMatrix();
}


void render_vape_in_hand(MoveableModel* object, Camera* camera){
    glPushMatrix();
    glLoadIdentity();

    if(camera->vape.isVaping){
        glTranslatef(0.0f, -0.2f, -0.4f);
        glRotatef(65.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
    }else{
        glTranslatef(0.3f, -0.4f, -0.7f);
    }

    glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
    render_model(&object->model);
    glPopMatrix();
}

void enable_vape_light(Vape* vape){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float lightColor[] = {1.0f, 0.3f, 0.0f, vape->smokeAmount};
    float lightPos[] = {0.0f, -0.2f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

    float direction[] = {0.0f, 0.0f, -1.0f};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, (90.0f*vape->smokeAmount));
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);

    glPopMatrix();
}

void disable_vape_light(){
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
}

void update_snail_ai(MoveableModel* monster, Camera* player, float deltaTime){
    float dx = player->x - monster->x;
    float dy = player->y - monster->y;
    float dz = player->z - monster->z;
    float dist = sqrtf(dx*dx + dy*dy + dz*dz);

    if(dist > 0.5f){
        monster->x += (dx / dist) * monster->animSpeed * deltaTime;
        monster->z += (dz / dist) * monster->animSpeed * deltaTime;
    }
}

void spawn_smoke(PointData point_data, Camera* camera){
    float rad_yaw = camera->yaw * (M_PI / 180.0f);
    float rad_pitch = camera->pitch * (M_PI / 180.0f);

    float directionX = sinf(rad_yaw) * cosf(rad_pitch);
    float directionY = -sinf(rad_pitch);
    float directionZ = -cosf(rad_yaw) * cosf(rad_pitch);
    for(int i = 0; i < MAX_PARTICLES; i++){
        smoke_particles[i].point_data.x = point_data.x+directionX;
        smoke_particles[i].point_data.y = point_data.y+directionY-0.2f;
        smoke_particles[i].point_data.z = point_data.z+directionZ;

        float speed = ((rand() % 100) / 200.0f) + 0.1f;
        float spread = ((rand() % 100) / 1000.0f) - 0.05f;
        smoke_particles[i].vx = directionX * speed + spread;
        smoke_particles[i].vy = directionY * speed + 0.05f;
        smoke_particles[i].vz = directionZ * speed + spread;

        smoke_particles[i].life = 1.0f;
        smoke_particles[i].size = 1.0f + (rand()%100);
    }
}

void update_and_render_smoke(float deltaTime){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    for(int i = 0; i<MAX_PARTICLES; i++){
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

void render_light_aura_model(LightAuraModel* model){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    glDisable(GL_FOG);
    glMatrixMode(GL_MODELVIEW);

    float lightColor[] = {model->r, model->g, model->b, model->a};
    float lightPos[] = {model->x, model->y+0.5f, model->z, 1.0f};

    float emissionColor[] = { 0.0f, 0.4f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos);

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0f);

    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5f / model->radius);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f / (model->radius * model->radius));

    glPushMatrix();
    glTranslatef(model->x, model->y, model->z);
    glRotatef(model->yaw, 0.0f, 1.0f, 0.0f);

    render_model(&model->model);

    float noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(model->x, model->y, model->z);
    glRotatef(model->yaw, 0.0f, 1.0f, 0.0f);
    render_model(&model->model);
    glPopMatrix();

    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Alpha blending bekapcsolása [cite: 2026-01-23]
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(model->x, model->y, model->z);

    glColor4f(model->r, model->g, model->b, 0.3f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.f, 0.f, 0.f);
    for(int i = 0; i <= 360; i += 10){
        float angle = i * M_PI / 180.0f;
        glVertex3f(cosf(angle) * (model->radius*0.15), sinf(angle) * (model->radius*0.15) + 0.15, 0);
    }
    glEnd();

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

