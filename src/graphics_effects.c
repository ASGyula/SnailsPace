//
// Created by asgyu on 2026. ápr. 17..
//

#include "graphics_effects.h"

#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <stdbool.h>

#include "graphics_particles.h"
#include "ui_manager.h"

void render_chromatic(Model* model, float intensity){
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    glPushMatrix();
    glTranslatef(-intensity, 0.0f, 0.0f);
    render_model(model);
    glPopMatrix();

    glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
    render_model(model);

    glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
    glPushMatrix();
    glTranslatef(intensity, 0.0f, 0.0f);
    render_model(model);
    glPopMatrix();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void render_game_over_scene(Model* model, Uint32 currentTime, float lightIntensity){
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enableFog(0.1f, 10.0f, 0.8f);
    glFogf(GL_FOG_DENSITY, 0.2f + (sinf(currentTime / 500.0f) * 0.1f));

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPulse = 0.5f + (sinf(currentTime / 100.0f) * 0.5f);
    float lightColor[] = {lightPulse, 0.0f, 0.0f, 1.0f};
    float lightPos[] = {0.0f, 2.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glPushMatrix();
    float scale = 1.0f + (sinf(currentTime / 1000.0f) * 0.1f);
    glScalef(scale, scale, scale);
    glRotatef(currentTime / 100.0f, 0.0f, 1.0f, 0.0f);

    render_model_without_texture(model);
    glPopMatrix();

    update_and_render_smoke(0.01f);
}