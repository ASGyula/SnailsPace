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

    load_obj("assets/elso_folyoso.obj", &palya, &palya_pontok_szama);

    SDL_Event event;

    initialize_camera(&camera);

    while(isRunning){
        while(SDL_PollEvent(&event)){
            handle_camera_input(&event, &camera, &isRunning);
        }

        setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
        update_camera(&camera);
        render_lidar(palya, palya_pontok_szama, camera.x, camera.y, camera.z);

        SDL_GL_SwapWindow(window);
    }

    free(palya);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}