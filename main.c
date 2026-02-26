#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

#include "camera.h"
#include "graphics.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(int argc, char *argv[]){
    bool isRunning = true;
    Camera camera;

    Vertex* palya = NULL;
    int palya_pontok_szama = 0;

    SDL_Window *window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GLContext *glContext = SDL_GL_CreateContext(window);

    load_obj("assets/elso_folyoso2.obj", &palya, &palya_pontok_szama);

    prepare_lidar_data(palya, palya_pontok_szama);

    SDL_Event event;

    initialize_camera(&camera);

    Uint32 lastTime = SDL_GetTicks();
    setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);

    while(isRunning){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime-lastTime)/1000.0f;
        lastTime = currentTime;

        if(deltaTime > 0.1f) deltaTime = 0.1f;
        while(SDL_PollEvent(&event)){
            handle_camera_input(&event, &camera);
        }

        handle_wasd_input(&event, &camera, &isRunning, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update_camera(&camera);
        render_lidar(palya, palya_pontok_szama, camera.x, camera.y, camera.z);
        // render_lidar_fast(palya_pontok_szama);

        SDL_GL_SwapWindow(window);
    }

    free(palya);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}