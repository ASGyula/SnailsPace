//
// Created by asgyu on 2026. márc. 8..
//

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "obj_loader.h"

#define EYE_HEIGHT 1.6f

typedef struct { float x, y, z; } Vec3;

static Vec3 sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
static Vec3 add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
static Vec3 scale(Vec3 v, float s) { return (Vec3){v.x * s, v.y * s, v.z * s}; }
static float dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static float length_sq(Vec3 v) { return dot(v, v); }

static Vec3 closest_point_on_triangle(Vec3 p, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 ab = sub(b, a);
    Vec3 ac = sub(c, a);
    Vec3 ap = sub(p, a);

    float d1 = dot(ab, ap);
    float d2 = dot(ac, ap);

    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    Vec3 bp = sub(p, b);
    float d3 = dot(ab, bp);
    float d4 = dot(ac, bp);

    if (d3 >= 0.0f && d4 <= d3) return b;

    float vc = d1*d4 - d3*d2;
    if(vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f){
        float v = d1 / (d1 - d3);
        return add(a, scale(ab, v));
    }

    Vec3 cp = sub(p, c);
    float d5 = dot(ab, cp);
    float d6 = dot(ac, cp);

    if (d6 >= 0.0f && d5 <= d6) return c;

    float vb = d5*d2 - d1*d6;
    if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f){
        float w = d2 / (d2 - d6);
        return add(a, scale(ac, w));
    }

    float va = d3*d6 - d5*d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return add(b, scale(sub(c, b), w));
    }

    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return add(a, add(scale(ab, v), scale(ac, w)));
}

void check_player_collision(Camera* camera, const Model* model, float playerRadius){
    if(!model || !model->vertices) return;

    float bodyCenterY = camera->y - EYE_HEIGHT + playerRadius;

    Vec3 playerPos = {camera->x, bodyCenterY, camera->z};
    float radiusSq = playerRadius * playerRadius;

    for(int i = 0; i < model->number_of_vertex; i += 3){
        Vec3 v0 = {model->vertices[i].x, model->vertices[i].y, model->vertices[i].z};
        Vec3 v1 = {model->vertices[i+1].x, model->vertices[i+1].y, model->vertices[i+1].z};
        Vec3 v2 = {model->vertices[i+2].x, model->vertices[i+2].y, model->vertices[i+2].z};

        float minX = fminf(v0.x, fminf(v1.x, v2.x)) - playerRadius;
        float maxX = fmaxf(v0.x, fmaxf(v1.x, v2.x)) + playerRadius;
        float minY = fminf(v0.y, fminf(v1.y, v2.y)) - playerRadius;
        float maxY = fmaxf(v0.y, fmaxf(v1.y, v2.y)) + playerRadius;
        float minZ = fminf(v0.z, fminf(v1.z, v2.z)) - playerRadius;
        float maxZ = fmaxf(v0.z, fmaxf(v1.z, v2.z)) + playerRadius;

        if(playerPos.x < minX || playerPos.x > maxX ||
            playerPos.y < minY || playerPos.y > maxY ||
            playerPos.z < minZ || playerPos.z > maxZ) {
            continue;
        }

        Vec3 closest = closest_point_on_triangle(playerPos, v0, v1, v2);
        Vec3 diff = sub(playerPos, closest);
        float distSq = length_sq(diff);

        if(distSq < radiusSq && distSq > 0.0001f){
            float dist = sqrtf(distSq);
            float overlap = playerRadius - dist;

            Vec3 pushDir = scale(diff, 1.0f / dist);
            playerPos = add(playerPos, scale(pushDir, overlap));
        }
    }

    camera->x = playerPos.x;
    camera->z = playerPos.z;

    camera->y = playerPos.y - playerRadius + EYE_HEIGHT;
}

void check_player_collision_mesh(Camera* camera, const Model* model, float playerRadius) {
    if(!model || !model->vertices) return;

    static float lastSafeX = 0;
    static float lastSafeZ = 0;

    if(lastSafeX == 0 && lastSafeZ == 0){
        lastSafeX = camera->x;
        lastSafeZ = camera->z;
    }

    float footY = 0;
    float radiusSq = playerRadius * playerRadius;

    bool collidedX = false;
    Vec3 posX = {camera->x, footY + 0.8f, lastSafeZ};

    for(int i = 0; i < model->number_of_vertex; i += 3){
        Vec3 v0 = {model->vertices[i].x, model->vertices[i].y, model->vertices[i].z};
        Vec3 v1 = {model->vertices[i+1].x, model->vertices[i+1].y, model->vertices[i+1].z};
        Vec3 v2 = {model->vertices[i+2].x, model->vertices[i+2].y, model->vertices[i+2].z};
        if(fmaxf(v0.y, fmaxf(v1.y, v2.y)) < footY + 0.3f) continue;

        Vec3 closest = closest_point_on_triangle(posX, v0, v1, v2);
        if(length_sq(sub(posX, closest)) < radiusSq){
            collidedX = true;
            break;
        }
    }

    if(collidedX) camera->x = lastSafeX;
    else lastSafeX = camera->x;

    bool collidedZ = false;
    Vec3 posZ = {lastSafeX, footY + 0.8f, camera->z};

    for(int i = 0; i < model->number_of_vertex; i += 3){
        Vec3 v0 = {model->vertices[i].x, model->vertices[i].y, model->vertices[i].z};
        Vec3 v1 = {model->vertices[i+1].x, model->vertices[i+1].y, model->vertices[i+1].z};
        Vec3 v2 = {model->vertices[i+2].x, model->vertices[i+2].y, model->vertices[i+2].z};
        if(fmaxf(v0.y, fmaxf(v1.y, v2.y)) < footY + 0.3f) continue;

        Vec3 closest = closest_point_on_triangle(posZ, v0, v1, v2);
        if(length_sq(sub(posZ, closest)) < radiusSq){
            collidedZ = true;
            break;
        }
    }

    if (collidedZ) camera->z = lastSafeZ;
    else lastSafeZ = camera->z;

    camera->y = EYE_HEIGHT / 2;
}

void check_player_collision_mesh_miside_room(Camera* camera, const Model* model, float playerRadius) {
    if(!model || !model->vertices) return;

    static float lastSafeX = 0;
    static float lastSafeZ = 0;

    if(lastSafeX == 0 && lastSafeZ == 0){
        lastSafeX = camera->x;
        lastSafeZ = camera->z;
    }

    float footY = 1.0f;
    float radiusSq = playerRadius * playerRadius;

    bool collidedX = false;
    Vec3 testPosX = {camera->x, footY, lastSafeZ};

    for(int i = 0; i < model->number_of_vertex; i += 3){
        Vec3 v0 = {model->vertices[i].x, model->vertices[i].y, model->vertices[i].z};
        Vec3 v1 = {model->vertices[i+1].x, model->vertices[i+1].y, model->vertices[i+1].z};
        Vec3 v2 = {model->vertices[i+2].x, model->vertices[i+2].y, model->vertices[i+2].z};

        float minY = fminf(v0.y, fminf(v1.y, v2.y));
        float maxY = fmaxf(v0.y, fmaxf(v1.y, v2.y));

        float minX = fminf(v0.x, fminf(v1.x, v2.x));
        float maxX = fmaxf(v0.x, fmaxf(v1.x, v2.x));
        float minZ = fminf(v0.z, fminf(v1.z, v2.z));
        float maxZ = fmaxf(v0.z, fmaxf(v1.z, v2.z));

        if(maxY < footY - 0.3f) continue;
        if(minY > footY + 0.3f) continue;

        if (camera->x < minX - playerRadius || camera->x > maxX + playerRadius ||
            lastSafeZ < minZ - playerRadius || lastSafeZ > maxZ + playerRadius) {
            continue;
        }

        Vec3 closest = closest_point_on_triangle(testPosX, v0, v1, v2);
        if(length_sq(sub(testPosX, closest)) < radiusSq){
            collidedX = true;
            break;
        }
    }

    if(collidedX) camera->x = lastSafeX;
    else lastSafeX = camera->x;


    bool collidedZ = false;
    Vec3 testPosZ = {lastSafeX, footY, camera->z};

    for(int i = 0; i < model->number_of_vertex; i += 3){
        Vec3 v0 = {model->vertices[i].x, model->vertices[i].y, model->vertices[i].z};
        Vec3 v1 = {model->vertices[i+1].x, model->vertices[i+1].y, model->vertices[i+1].z};
        Vec3 v2 = {model->vertices[i+2].x, model->vertices[i+2].y, model->vertices[i+2].z};

        float minY = fminf(v0.y, fminf(v1.y, v2.y));
        float maxY = fmaxf(v0.y, fmaxf(v1.y, v2.y));

        float minX = fminf(v0.x, fminf(v1.x, v2.x));
        float maxX = fmaxf(v0.x, fmaxf(v1.x, v2.x));
        float minZ = fminf(v0.z, fminf(v1.z, v2.z));
        float maxZ = fmaxf(v0.z, fmaxf(v1.z, v2.z));


        if(maxY < footY - 0.3f) continue;
        if(minY > footY + 0.3f) continue;

        if (lastSafeX < minX - playerRadius || lastSafeX > maxX + playerRadius ||
            camera->z < minZ - playerRadius || camera->z > maxZ + playerRadius) {
            continue;
        }

        Vec3 closest = closest_point_on_triangle(testPosZ, v0, v1, v2);
        if(length_sq(sub(testPosZ, closest)) < radiusSq){
            collidedZ = true;
            break;
        }
    }

    if (collidedZ) camera->z = lastSafeZ;
    else lastSafeZ = camera->z;
}