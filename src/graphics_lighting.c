//
// Created by asgyu on 2026. ápr. 17..
//

#include "graphics_lighting.h"

#include <math.h>
#include <SDL_opengl.h>
#include <stdio.h>

void enable_vape_light(Camera* camera){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    const float intensity = camera->vape.smokeAmount*camera->auraLightBrightness;
    const float brightness = intensity;
    float lightColor[] = {1.0f * brightness,0.3f * brightness,0.0f * brightness,1.0f};
    float lightPos[] = {0.0f, -0.2f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

    float direction[] = {0.0f, 0.0f, -1.0f};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0f * intensity);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);

    glPopMatrix();
}
void disable_vape_light(void){
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
}

void render_light_aura_model(Camera* camera, LightAuraModel* model){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT2);
    glMatrixMode(GL_MODELVIEW);

    float brightness = camera->auraLightBrightness;
    float lightColor[] = {model->r * brightness, model->g * brightness, model->b * brightness, 1.0f};
    float lightPos[] = {model->x, model->y + 0.5f, model->z, 1.0f};

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
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(model->x, model->y, model->z);

    if(model->a != 0){
        glColor4f(model->r, model->g, model->b, brightness * model->a);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.f, 0.f, 0.f);
        for(int i = 0; i <= 360; i += 10){
            float angle = i * M_PI / 180.0f;
            glVertex3f(cosf(angle) * (model->radius * 0.15), sinf(angle) * (model->radius * 0.15) + 0.15, 0);
        }
        glEnd();
    }

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

void enable_snail_caught_lights(MoveableModel* model, Uint32 currentTime, Uint32 caughtTime){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT3);
    glMatrixMode(GL_MODELVIEW);

    float lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightPos[] = {model->x, model->y + 3, model->z, 1.0f};

    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();

    enable_colored_fog(0.01f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    if(currentTime > (caughtTime + 500)){
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.0f, -1.0f, -1.8f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        render_model(&model->model);
        glPopMatrix();
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    disableFog();
}

void enable_pre_lidar_lights(LightAuraModel* map, Camera* camera){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT3);
    glMatrixMode(GL_MODELVIEW);

    float lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightPos[] = {map->x, map->y + 10, map->z, 1.0f};

    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();

    enable_colored_fog(0.01f, 3.0f, 0.89f, 0.22f, 0.31f, 1.0f, 1.0f);
    render_light_aura_model(camera, map);

    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    disableFog();
}