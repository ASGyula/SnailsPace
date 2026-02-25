#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

#include "graphics.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(int argc, char *argv[]){
    bool isRunning = true;
    float camX = 0.0f, camY = 1.6f, camZ = 3.00f;

    Vertex* palya = NULL;
    int palya_pontok_szama = 0;

    SDL_Window *window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GLContext *glContext = SDL_GL_CreateContext(window);

    load_obj("assets/elso_folyoso.obj", &palya, &palya_pontok_szama);

    SDL_Event event;
    while(isRunning){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                isRunning = false;
            }
        }

        setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
        render_lidar(palya, palya_pontok_szama, camX, camY, camZ);

        SDL_GL_SwapWindow(window);
    }

    free(palya);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}