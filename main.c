#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "camera.h"
#include "graphics.h"
#include "player.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(int argc, char *argv[]){
    bool isRunning = true;
    Player player;
    player.camera.screenWidth = SCREEN_WIDTH;
    player.camera.screenHeight = SCREEN_HEIGHT;

    Vertex* palya = NULL;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("[HIBA] Nem sikerult inincializalni a Mixert");
    }


    Model Helsie;
    Model masodikFolyoso;
    UIElement spaceToShoutUIElement;
    SDL_Window *window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GLContext *glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    load_obj("assets/elso_folyoso.obj", &palya);
    load_textured_obj("assets/Blender/masodik_folyoso.obj", &masodikFolyoso);
    // load_obj("assets/External/MiSide/level_4_Miside.obj", &palya);
    // load_textured_obj("assets/External/MiSide/level_4_Miside.obj", &masodikFolyoso);
    // load_textured_obj("assets/Blender/masodik_szoba/masodik_szoba.obj", &palya);
    load_textured_obj("assets/External/Helsie/HelsieMidnightbyRedEyes.obj", &Helsie);


    Helsie.textureID = load_texture("assets/External/Helsie/T_MysticFang_Body_D.png");
    spaceToShoutUIElement.textureID = load_texture("assets/External/Gemini/SpaceToShout.png");

    // prepare_lidar_data(palya);

    SDL_Event event;

    initialize_camera(&player.camera);

    Uint32 lastTime = SDL_GetTicks();
    setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);

    while(isRunning){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime-lastTime)/1000.0f;
        lastTime = currentTime;

        if(deltaTime > 0.1f) deltaTime = 0.1f;
        while(SDL_PollEvent(&event)){
            handle_camera_input(&event, &player.camera);
        }

        handle_wasd_input(&event, &player.camera, &isRunning, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // spaceToShoutUIElement.isShowing = !camera.shout.isSouthSource || (SDL_GetTicks() - camera.shout.startTime > camera.shout.duration + camera.shout.delay);

        update_camera(&player.camera);
        render_bat_vision(&masodikFolyoso, currentTime);
        // render_bat_vision(&Helsie, currentTime);
        // render_lidar(palya, &camera);
        // render_lidar_eco(palya, &camera, currentTime);
        // render_lidar_fast(palya->number_of_vertex);

        // render_lidar(Helsie, Helsie->number_of_vertex, camera.x, camera.y, camera.z);

        // render_lidar_fast(Helsie->number_of_vertex);


        // glPushMatrix();
        // render_model_without_texture(&masodikFolyoso);
        render_model(&Helsie);
        render_ui_texture(spaceToShoutUIElement, SCREEN_WIDTH, SCREEN_HEIGHT, true);
        // glPopMatrix();

        SDL_GL_SwapWindow(window);
    }

    // free(palya);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}