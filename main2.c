// #include <math.h>
//
// #include "include/obj_loader.h"
// #include <SDL.h>
// #include <SDL_opengl.h>
// #include <SDL_ttf.h>
// #include <stdio.h>
// #include <math.h>
//
// #define SCREEN_WIDTH 1000
// #define SCREEN_HEIGHT 800
//
// int main(int argc, char *argv[]){
//     bool isRunning = true;
//     float fov = 60.0f;
//     float aspec = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
//     float nearVal = 0.1f;
//     float farVal = 100.0f;
//
//     Vertex* palya = NULL;
//     int pontok_szama = 0;
//
//     SDL_Window *window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
//     SDL_GLContext *glContext = SDL_GL_CreateContext(window);
//
//     load_obj("assets/elso_folyoso.obj", &palya, &pontok_szama);
//
//     SDL_Event event;
//     while(isRunning){
//         while(SDL_PollEvent(&event)){
//             if(event.type == SDL_QUIT){
//                 isRunning = false;
//             }
//         }
//
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glLoadIdentity();
//         float top = nearVal * tanf(fov * 0.5f * (M_PI / 180.0f));
//         float bottom = -top;
//         float right = top * aspec;
//         float left = -right;
//         glFrustum(left, right, bottom, top, nearVal, farVal);
//         glMatrixMode(GL_MODELVIEW);
//
//         glTranslatef(0.0f, -0.0001f, 0.0f);
//
//
//         //pálya
//         if(palya != NULL && pontok_szama > 0){
//             glBegin(GL_POINTS);
//             for(int i = 0; i<pontok_szama;i++){
//                 glVertex3f(palya[i].x, palya[i].y, palya[i].z);
//             }
//             glEnd();
//         }
//
//         SDL_GL_SwapWindow(window);
//     }
//
//     free(palya);
//     SDL_GL_DeleteContext(glContext);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//
//     return 0;
// }