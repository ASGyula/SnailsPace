#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "camera.h"
#include "game_manager.h"
#include "graphics.h"
#include "player.h"
#include "ui_manager.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main2(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        printf("[HIBA] SDL Init hiba: %s\n", SDL_GetError());
        return 1;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("[HIBA] Nem sikerult inincializalni a Mixert: %s\n", Mix_GetError());
    }

    Game game = init_game(SCREEN_WIDTH, SCREEN_HEIGHT);

    Vertex* palya = NULL;

    Model Helsie;
    Model masodikFolyoso;
    UIElement spaceToShoutUIElement;

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

    while(game.isRunning){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime-game.lastTime)/1000.0f;
        game.lastTime = currentTime;

        if(deltaTime > 0.1f) deltaTime = 0.1f;
        while(SDL_PollEvent(&event)){
            handle_mouse_input(&event, &game.player.camera);
        }

        handle_wasd_input(&event, &game.player.camera, &game.isRunning, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // spaceToShoutUIElement.isShowing = !camera.shout.isSouthSource || (SDL_GetTicks() - camera.shout.startTime > camera.shout.duration + camera.shout.delay);

        update_camera_view(&game.player.camera);
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

        SDL_GL_SwapWindow(game.window);
    }

    // free(palya);
    SDL_GL_DeleteContext(game.glContext);
    SDL_DestroyWindow(game.window);
    SDL_Quit();

    return 0;
}